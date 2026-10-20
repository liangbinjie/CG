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

typedef struct Object Object;

/* Intersection struct */
typedef struct {
    Object *object;
    double t;
    Position position;
} Intersection;

/* Object Node LinkedList */
struct Object {
    ObjectType type;
    Position position; // x, y, z coordinates
    Color color; // r, g, b, a values
    void *data; // pointer to object-specific data (e.g., radius for sphere)
    struct Object *next; // pointer to the next object in the linked list
};

int parse_sphere(const char *line, Position *position, Color *color, double *radius);

Object *create_object(ObjectType type, Position position, Color color, void *data);
Object *add_object(Object *head, Object *new_object);

int parse_object(const char *line, Object **objects);
void free_objects(Object *objects);

Color de_que_color(Position eye, Position ray, Config config);
Intersection *first_intersection(Position eye, Position ray, Config config);
int intersect_sphere(Position eye, Position ray, Object *object, double *t);

#endif
