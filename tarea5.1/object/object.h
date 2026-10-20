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

    // Lightning coefficient for objects
    double reflection_coefficient; // kd -> [0,1]
    double ambient_coefficient; // ka -> [0,1]

    void *data; // pointer to object-specific data (e.g., radius for sphere)
    struct Object *next; // pointer to the next object in the linked list
};

int parse_sphere(const char *line, Position *position, Color *color, double *radius,
                 double *reflection_coefficient, double *ambient_coefficient);

Object *create_object(ObjectType type, Position position, Color color, void *data);
Object *add_object(Object *head, Object *new_object);

int parse_object(const char *line, Object **objects);
void free_objects(Object *objects);

Position vectorN_sphere(const Object *object, Position intersection_position);
Position getVectorN(const Object *object, Position intersection_position);
Position get_vectorL(Position intersection_position, Position light_position);
double get_crossProduct(Position vectorN, Position vectorL);
double get_attenuation_factor(Light light, Position intersection_position);

Color de_que_color(Position eye, Position ray, Config config);
Intersection *first_intersection(Position eye, Position ray, Config config);
int intersect_sphere(Position eye, Position ray, Object *object, double *t);

#endif
