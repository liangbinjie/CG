#include "object.h"
#include "../config/config.h"

#include <stdio.h>   // sscanf
#include <stdlib.h>  // malloc, free
#include <string.h>  // strncmp

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
