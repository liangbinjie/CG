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

/* Intersection struct */
typedef struct {
    Object *object;
    double t;
    Position position;
} Intersection;

/* Object Node LinkedList */
typedef struct Object {
    ObjectType type;
    Position position; // x, y, z coordinates
    Color color; // r, g, b, a values
    void *data; // pointer to object-specific data (e.g., radius for sphere)
    struct Object *next; // pointer to the next object in the linked list

    /* Light setion */
    double reflection_coefficient; // known as K_D
    double illumination_coefficient; // known as K_A

} Object;

int parse_sphere(const char *line, Position *position, Color *color, double *radius, double *reflection_coefficient, double *illumination_coefficient);

Object *create_object(ObjectType type, Position position, Color color, double kd, double ka, void *data);
Object *add_object(Object *head, Object *new_object);

int parse_object(const char *line, Object **objects);
void free_objects(Object *objects);

Color de_que_color(Position eye, Position ray, Config config);
Intersection first_intersection(Position eye, Position ray, Config config);
Position intersect_sphere(Position eye, Position ray, Object *object, double *t);

#endif
