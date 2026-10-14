#ifndef OBJECT_H
#define OBJECT_H

#include "../config/config.h"

typedef enum {
    SPHERE,
    // Add more object types as needed
} ObjectType;

/* Sphere-specific data */
typedef struct {
    double radius;
} Sphere;

/* Object Node LinkedList */
typedef struct Object {
    ObjectType type;
    Position position; // x, y, z coordinates
    Color color; // r, g, b, a values
    void *data; // pointer to object-specific data (e.g., radius for sphere)
    struct Object *next; // pointer to the next object in the linked list
} Object;

int parse_sphere(const char *line, Position *position, Color *color, double *radius);

Object *create_object(ObjectType type, Position position, Color color, void *data);
Object *add_object(Object *head, Object *new_object);

int parse_object(const char *line, Object **objects);

#endif
