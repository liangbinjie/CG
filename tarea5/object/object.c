#include "object.h"
#include "../config/config.h"

#include <stdio.h>   // sscanf
#include <stdlib.h>  // malloc, free
#include <string.h>  // strncmp
#include <float.h>
#include <math.h>

int parse_sphere(const char *line, Position *position, Color *color, double *radius, double *reflection_coefficient, double *illumination_coefficient) {
    // Example line format: "sphere x y z alpha red green blue radius reflection_coefficient illumination_coefficient"
    if (sscanf(line, "sphere %lf %lf %lf %hhu %hhu %hhu %hhu %lf %lf %lf",
               &position->x, &position->y, &position->z,
               &color->a, &color->r, &color->g, &color->b,
               radius,
               reflection_coefficient, illumination_coefficient) != 10) {
        return -1; 
    }
    color->a = 255; // Default alpha value
    return 0; 
}

Object *create_object(ObjectType type, Position position, Color color, double kd, double ka, void *data) {
    Object *new_object = (Object *)malloc(sizeof(Object));
    if (!new_object) {
        return NULL; // Memory allocation failed
    }

    new_object->type = type;
    new_object->position = position;
    new_object->color = color;
    new_object->data = data;
    new_object->reflection_coefficient = kd;
    new_object->illumination_coefficient = ka;

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
    double reflection_coefficient;
    double illumination_coefficient;

    if (strncmp(line, "sphere", 6) == 0) {
        double radius;
        if (parse_sphere(line, &position, &color, &radius, &reflection_coefficient, &illumination_coefficient) == 0) {
            Sphere *sphere_data = (Sphere *)malloc(sizeof(Sphere));
            if (!sphere_data) {
                return -1; // Memory allocation failed
            }
            sphere_data->radius = radius;

            Object *new_object = create_object(SPHERE, position, color, reflection_coefficient, illumination_coefficient, sphere_data);
            if (!new_object) {
                free(sphere_data);
                return -1; // Memory allocation failed
            }
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

Color de_que_color(Position eye, Position ray, Config config) {
    Intersection intersection = first_intersection(eye, ray, config);
    if (!intersection.object) {
        return config.background_color;
    }

    Object *object = intersection.object; // Q
    double intensity = 0.0;

    // N vector for sphere
    if (object->type == SPHERE) {
        double nx = intersection.position.x - object->position.x;
        double ny = intersection.position.y - object->position.y;
        double nz = intersection.position.z - object->position.z;
        double normal_length = sqrt(nx * nx + ny * ny + nz * nz);
        if (normal_length > 0.0) {
            nx /= normal_length;
            ny /= normal_length;
            nz /= normal_length;
        }

        for (int k = 0; k < config.light_count; ++k) {
            const Light *light = &config.lights[k];
            double lx = light->position.x - intersection.position.x;
            double ly = light->position.y - intersection.position.y;
            double lz = light->position.z - intersection.position.z;
            double distance = sqrt(lx * lx + ly * ly + lz * lz);
            if (distance <= 0.0) continue;
            lx /= distance;
            ly /= distance;
            lz /= distance;

            double diffuse = nx * lx + ny * ly + nz * lz;
            if (diffuse <= 0.0) continue;

            double denominator = light->c1 + light->c2 * distance +
                                 light->c3 * distance * distance;
            if (denominator <= 0.0) continue;
            double attenuation = fmin(1.0, 1.0 / denominator);
            intensity += diffuse * object->reflection_coefficient *
                         light->light_intensity * attenuation;
        }
    }

    intensity += config.ambient_intensity * object->illumination_coefficient;
    intensity = fmin(1.0, fmax(0.0, intensity));
    Color color = object->color;
    color.r = (uint8_t)lround(color.r * intensity);
    color.g = (uint8_t)lround(color.g * intensity);
    color.b = (uint8_t)lround(color.b * intensity);
    return color;
}

Intersection first_intersection(Position eye, Position ray, Config config) {
    double tmin = DBL_MAX;
    double t;
    Intersection intersection = {0};

    Object *current_object = config.objects;

    while (current_object != NULL) {
        if (current_object->type == SPHERE) {
            Position position = intersect_sphere(eye, ray, current_object, &t);
            if (t > 0.0) {
                if (t < tmin) {
                    tmin = t;

                    intersection.object = current_object; // puntero a objeto
                    intersection.t = t; // incluye t
                    intersection.position = position;
                }
            }
        }
        current_object = current_object->next;
    }

    return intersection;
}

Position intersect_sphere(Position eye, Position ray, Object *object, double *t) {
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
        *t = -1.0;
        return (Position){0.0, 0.0, 0.0};
    }

    t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    t2 = (-b + sqrt(discriminant)) / (2.0 * a);

    Position position;

    if (t1 > 0.0) {
        *t = t1;
        position.x = eye.x + ray.x*t1;
        position.y = eye.y + ray.y*t1;
        position.z = eye.z + ray.z*t1;
        //printf("Intersection: (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
        return position;
    }

    if (t2 > 0.0) {
        *t = t2;
        position.x = eye.x + ray.x*t2;
        position.y = eye.y + ray.y*t2;
        position.z = eye.z + ray.z*t2;
        //printf("Intersection: (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
        return position;
    }

    *t = -1.0;
    return (Position){0.0, 0.0, 0.0};
}
