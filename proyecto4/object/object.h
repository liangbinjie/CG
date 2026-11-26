#ifndef OBJECT_H
#define OBJECT_H

#include "../config/config.h"

#define EPSILON 0.0005
#define MAX_REFLECTION_DEPTH 5

typedef enum {
    SPHERE,
    POLYGON,
    DISK,
    ELLIPSE,
    QUADRIC
    // Add more object types as needed
} ObjectType;

/* Sphere-specific data */
typedef struct {
    double radius;
} Sphere;

/* Polygon-specific data */
typedef struct { double u, v; } Vertex2D;
typedef struct {
    int vertex_count;
    Position *vertices3d;
    Vertex2D *vertices2d;
    Position normal;       // unit normal, fixed for the polygon
    double plane_d;       // normalized A*x+B*y+C*z+D=0
    int dropped_axis;     // 0:x, 1:y, 2:z
    int texture_id;       // 0 uses the polygon's solid color
} Polygon;

/* Disk-specific data */
typedef struct {
    double outer_radius;
    double inner_radius;
    Position normal;
    double plane_d;
} Disk;

/* Ellipse-specific data */
typedef struct {
    Position focus1;
    Position focus2;
    double R;
    Position normal;
    double plane_d;
} Ellipse;

/* Coefficients for A*x^2+B*y^2+C*z^2+D*x*y+E*x*z+F*y*z
   +G*x+H*y+I*z+J=0 in coordinates relative to the object position. */
typedef struct {
    double coefficients[10];
} Quadric;

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
    double specular_coefficient; // ks -> [0,1]
    int shininess; // kn >= 1

    void *data; // pointer to object-specific data (e.g., radius for sphere)
    double o1; // Original color
    double o2; // Reflected color
    double o3; // Transmitted color
    CutPlane *cut_planes;
    int cut_plane_count;
    struct Object *next; // pointer to the next object in the linked list
};

/* Functions Signature */

int parse_sphere(const char *line, Position *position, Color *color, double *radius,
                 double *reflection_coefficient, double *ambient_coefficient,
                 double *specular_coefficient, int *shininess, double *o1,
                 double *o2, double *o3);

Object *create_object(ObjectType type, Position position, Color color, void *data);
Object *add_object(Object *head, Object *new_object);

int parse_object(const char *line, Object **objects);
void free_objects(Object *objects);

Position vectorN_sphere(const Object *object, Position intersection_position);
Position getVectorN(const Object *object, Position intersection_position);
Position get_vectorL(Position intersection_position, Position light_position);
double get_crossProduct(Position vectorN, Position vectorL);
double get_attenuation_factor(Light light, Position intersection_position);

Color de_que_color(Position eye, Position ray, Config config, int depth);
Intersection *first_intersection(Position eye, Position ray, Config config);

int intersect_sphere(Position eye, Position ray, Object *object, double *t);
int intersect_polygon(Position eye, Position ray, Object *object, double *t);
int contained_in_polygon(const Polygon *polygon, Position point);
int intersect_disk(Position eye, Position ray, Object *object, double *t);
int intersect_ellipse(Position eye, Position ray, Object *object, double *t);
int intersect_quadric(Position eye, Position ray, Object *object, double *t);

#endif
