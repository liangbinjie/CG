#include "object.h"
#include "../config/config.h"

#include <stdio.h>   // sscanf
#include <stdlib.h>  // malloc, free
#include <string.h>  // strncmp
#include <float.h>
#include <math.h>

int parse_sphere(const char *line, Position *position, Color *color, double *radius,
                 double *reflection_coefficient, double *ambient_coefficient) {
    // Format: "sphere x y z alpha red green blue radius K_D K_A"
    if (sscanf(line, "sphere %lf %lf %lf %hhu %hhu %hhu %hhu %lf %lf %lf",
               &position->x, &position->y, &position->z,
               &color->a, &color->r, &color->g, &color->b,
               radius, reflection_coefficient, ambient_coefficient) != 10) {
        return -1; 
    }
    color->a = 255; // Default alpha value
    return 0; 
}

Object *create_object(ObjectType type, Position position, Color color, void *data) {
    Object *new_object = (Object *)malloc(sizeof(Object));
    if (!new_object) {
        return NULL; // Memory allocation failed
    }

    new_object->type = type;
    new_object->position = position;
    new_object->color = color;
    new_object->data = data;
    new_object->next = NULL;

    return new_object;
}

Object *add_object(Object *head, Object *new_object) {
    if (!new_object) {
        return head;
    }

    new_object->next = head;
    return new_object;
}

int parse_object(const char *line, Object **objects) {
    if (!objects) return -1;
    Position position;
    Color color;
    double radius;
    double reflection_coefficient;
    double ambient_coefficient;

    if (strncmp(line, "sphere", 6) == 0) {
        if (parse_sphere(line, &position, &color, &radius,
                         &reflection_coefficient, &ambient_coefficient) == 0) {
            Sphere *sphere_data = (Sphere *)malloc(sizeof(Sphere));
            if (!sphere_data) {
                return -1; // Memory allocation failed
            }
            sphere_data->radius = radius;

            Object *new_object = create_object(SPHERE, position, color, sphere_data);
            if (!new_object) {
                free(sphere_data);
                return -1; // Memory allocation failed
            }
            new_object->reflection_coefficient = reflection_coefficient;
            new_object->ambient_coefficient = ambient_coefficient;
            *objects = add_object(*objects, new_object);
            return 0; 
        }
    }

    return -1;
}

void free_objects(Object *objects) {
    Object *current = objects;
    Object *next;

    while (current != NULL) {
        next = current->next;

        free(current->data);
        free(current);

        current = next;
    }
}

Position vectorN_sphere(const Object *object, Position intersection_position) {
    Position normal = {0.0, 0.0, 0.0};
    if (!object || object->type != SPHERE || !object->data) return normal;

    const Sphere *sphere = (const Sphere *)object->data;
    if (sphere->radius == 0.0) return normal;

    normal.x = (intersection_position.x - object->position.x) / sphere->radius;
    normal.y = (intersection_position.y - object->position.y) / sphere->radius;
    normal.z = (intersection_position.z - object->position.z) / sphere->radius;
    return normal;
}

Position getVectorN(const Object *object, Position intersection_position) {
    Position normal = {0.0, 0.0, 0.0};
    if (!object) return normal;

    switch (object->type) {
        case SPHERE:
            return vectorN_sphere(object, intersection_position);
        default:
            return normal;
    }
}

Position get_vectorL(Position intersection_position, Position light_position) {
    Position vector = {
        light_position.x - intersection_position.x,
        light_position.y - intersection_position.y,
        light_position.z - intersection_position.z
    };
    double length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

    // The direction is undefined when the intersection and light positions coincide.
    if (length == 0.0) return (Position){0.0, 0.0, 0.0};

    vector.x /= length;
    vector.y /= length;
    vector.z /= length;
    return vector;
}

double get_crossProduct(Position vectorN, Position vectorL) {
    // For normalized N and L, the dot product is cos(theta)
    double cosine = vectorN.x * vectorL.x +
                    vectorN.y * vectorL.y +
                    vectorN.z * vectorL.z;
    return cosine > 0.0 ? cosine : 0.0;
}

double get_attenuation_factor(Light light, Position intersection_position) {
    double dx = light.position.x - intersection_position.x;
    double dy = light.position.y - intersection_position.y;
    double dz = light.position.z - intersection_position.z;
    double distance = sqrt(dx * dx + dy * dy + dz * dz);
    double denominator = light.c1 + light.c2 * distance +
                         light.c3 * distance * distance;

    if (denominator <= 1.0) return 1.0;
    return 1.0 / denominator;
}

Color de_que_color(Position eye, Position ray, Config config) {
    Color color;
    Intersection *intersection = first_intersection(eye, ray, config);
    
    if (!intersection) {
        color = config.background_color;
    } else {
        Object *Q = intersection->object; 
        Position normal = getVectorN(Q, intersection->position);
        double intensity = 0.0;
        for (int k=0; k < config.light_count; k++) {
            Position l_vector = get_vectorL(intersection->position, config.lights[k].position);
            double cross_product_LN = get_crossProduct(normal, l_vector);
            if (cross_product_LN > 0.0) {
                double fatt = get_attenuation_factor(config.lights[k], intersection->position);
                intensity += cross_product_LN * Q->reflection_coefficient * fatt *
                             config.lights[k].light_intensity;
            }
        };
        intensity += config.ambient_intensity * Q->ambient_coefficient;
        if (intensity > 1.0) intensity = 1.0;
        if (intensity < 0.0) intensity = 0.0;

        color.a = Q->color.a;
        color.r = (uint8_t)(Q->color.r * intensity);
        color.g = (uint8_t)(Q->color.g * intensity);
        color.b = (uint8_t)(Q->color.b * intensity);
        free(intersection);
    }
    
    return color;
}

Intersection *first_intersection(Position eye, Position ray, Config config) {
    double tmin = DBL_MAX;
    double t;
    Object *closest = NULL;

    Object *current = config.objects;

    while (current != NULL) {
        if (current->type == SPHERE) {
            if (intersect_sphere(eye, ray, current, &t)) {
                if (t < tmin) {
                    tmin = t;
                    closest = current;
                }
            }
        }
        current = current->next;
    }

    if (!closest) {
        return NULL;
    }

    Intersection *intersection = malloc(sizeof(Intersection));
    if (!intersection) {
        return NULL;
    }

    intersection->object = closest;
    intersection->t = tmin;
    intersection->position.x = eye.x + tmin * ray.x;
    intersection->position.y = eye.y + tmin * ray.y;
    intersection->position.z = eye.z + tmin * ray.z;
    return intersection;
}

int intersect_sphere(Position eye, Position ray, Object *object, double *t) {
    Sphere *sphere = (Sphere *)object->data;
    double t1, t2;

    double dx = eye.x - object->position.x;
    double dy = eye.y - object->position.y;
    double dz = eye.z - object->position.z;

    double a = ray.x * ray.x + ray.y * ray.y + ray.z * ray.z;
    double b = 2.0 * (ray.x * dx + ray.y * dy + ray.z * dz);
    double c = dx * dx + dy * dy + dz * dz -
               sphere->radius * sphere->radius;

    double discriminant = b * b - 4.0 * a * c;

    if (a == 0.0 || discriminant < 0.0) {
        return 0;
    }

    t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    t2 = (-b + sqrt(discriminant)) / (2.0 * a);

    if (t1 > 0.0) {
        *t = t1;
        return 1;
    }

    if (t2 > 0.0) {
        *t = t2;
        return 1;
    }

    return 0;
}

