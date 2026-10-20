#include "object.h"
#include "../config/config.h"

#include <stdio.h>   // sscanf
#include <stdlib.h>  // malloc, free
#include <string.h>  // strncmp
#include <float.h>
#include <math.h>

int parse_sphere(const char *line, Position *position, Color *color, double *radius) {
    // Example line format: "sphere x y z alpha red green blue radius"
    if (sscanf(line, "sphere %lf %lf %lf %hhu %hhu %hhu %hhu %lf",
               &position->x, &position->y, &position->z,
               &color->a, &color->r, &color->g, &color->b,
               radius) != 8) {
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

    if (strncmp(line, "sphere", 6) == 0) {
        if (parse_sphere(line, &position, &color, &radius) == 0) {
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
    Color color;
    Intersection *intersection = first_intersection(eye, ray, config);
    
    if (!intersection) {
        color = config.background_color;
    } else {
        color = intersection->object->color;
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

