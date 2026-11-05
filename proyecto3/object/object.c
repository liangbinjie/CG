#include "object.h"
#include "../config/config.h"

#include <stdio.h>   // sscanf
#include <stdlib.h>  // malloc, free
#include <string.h>  // strncmp
#include <float.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>

/* Polygons */

static Position subtract(Position a, Position b) {
    return (Position){a.x - b.x, a.y - b.y, a.z - b.z};
}

static Position cross(Position a, Position b) {
    return (Position){a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x};
}

static double dot(Position a, Position b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

static Vertex2D project(Position p, int dropped_axis) {
    if (dropped_axis == 0) return (Vertex2D){p.y, p.z};
    if (dropped_axis == 1) return (Vertex2D){p.x, p.z};
    return (Vertex2D){p.x, p.y};
}

static int parse_polygon(const char *line, Object **objects) {
    int n, used;
    double alpha;
    long double red, green, blue;
    double kd, ka, ks = 0.0, kn = 32.0;
    const char *cursor = line;
    if (sscanf(cursor, "polygon %d %lf %Lf %Lf %Lf %lf %lf %n",
               &n, &alpha, &red, &green, &blue, &kd, &ka, &used) != 7 ||
        n < 3 || alpha < 0.0 || alpha > 1.0 ||
        red < 0 || red > 1 || green < 0 || green > 1 || blue < 0 || blue > 1 ||
        kd < 0 || kd > 1 || ka < 0 || ka > 1)
        return -1;
    cursor += used;
    size_t value_count = 0;
    size_t value_capacity = (size_t)n * 3 + 2;
    double *values = malloc(value_capacity * sizeof(*values));
    if (!values) return -1;
    while (value_count < value_capacity) {
        char *end;
        double value;
        while (isspace((unsigned char)*cursor)) cursor++;
        if (*cursor == '\0' || *cursor == '#') break;
        value = strtod(cursor, &end);
        if (end == cursor) {
            free(values);
            values = NULL;
            return -1;
        }
        values[value_count++] = value;
        cursor = end;
    }
    if (value_count != (size_t)n * 3 && value_count != (size_t)n * 3 + 2) {
        free(values);
        return -1;
    }
    if (value_count == (size_t)n * 3 + 2) {
        ks = values[n * 3];
        kn = values[n * 3 + 1];
    }
    if (ks < 0.0 || ks > 1.0 || kn < 1.0 || kn != floor(kn)) {
        free(values);
        return -1;
    }
    Polygon *polygon = calloc(1, sizeof(*polygon));
    if (!polygon) {
        free(values);
        return -1;
    }
    polygon->vertex_count = n;
    polygon->vertices3d = calloc((size_t)n, sizeof(Position));
    polygon->vertices2d = calloc((size_t)n, sizeof(Vertex2D));
    if (!polygon->vertices3d || !polygon->vertices2d) goto fail;
    for (int i = 0; i < n; i++) {
        Position *v = &polygon->vertices3d[i];
        v->x = values[i * 3];
        v->y = values[i * 3 + 1];
        v->z = values[i * 3 + 2];
    }
    free(values);

    /* Find a noncollinear triple even if the first three vertices are collinear. */
    Position origin = polygon->vertices3d[0], normal = {0,0,0};
    for (int i = 1; i < n - 1; i++) {
        normal = cross(subtract(polygon->vertices3d[i], origin),
                       subtract(polygon->vertices3d[i+1], origin));
        if (dot(normal, normal) > 0.00000001) break;
    }
    double len = sqrt(dot(normal, normal));
    if (len <= 0.00000001) goto fail;
    polygon->normal = (Position){normal.x/len, normal.y/len, normal.z/len};
    polygon->plane_d = -dot(polygon->normal, origin);
    for (int i=0; i<n; i++) {
        if (fabs(dot(polygon->normal, polygon->vertices3d[i]) + polygon->plane_d) > 0.00000001)
            goto fail;
    }
    double ax=fabs(normal.x), ay=fabs(normal.y), az=fabs(normal.z);
    polygon->dropped_axis = ax >= ay && ax >= az ? 0 : (ay >= az ? 1 : 2);
    for (int i = 0; i < n; i++)
        polygon->vertices2d[i] = project(polygon->vertices3d[i], polygon->dropped_axis);

    Color color = {(uint8_t)(alpha * 255.0), red, green, blue};
    Object *o = create_object(POLYGON, origin, color, polygon);
    if (!o) goto fail;
    o->reflection_coefficient = kd;
    o->ambient_coefficient = ka;
    o->specular_coefficient = ks;
    o->shininess = (int)kn;
    *objects = add_object(*objects, o);
    return 0;
fail:
    free(values);
    free(polygon->vertices3d);
    free(polygon->vertices2d);
    free(polygon);
    return -1;
}

/* Crossing count along the positive U ray after translating the candidate
   point to (0, 0). Half-open V interval avoids double-counting vertices. */
int contained_in_polygon(const Polygon *polygon, Position point) {
    if (!polygon || polygon->vertex_count < 3) return 0;
    Vertex2D q = project(point, polygon->dropped_axis);
    int count = 0;
    for (int i = 0, j = polygon->vertex_count - 1;
         i < polygon->vertex_count; j = i++) {
        double u1=polygon->vertices2d[j].u-q.u, v1=polygon->vertices2d[j].v-q.v;
        double u2=polygon->vertices2d[i].u-q.u, v2=polygon->vertices2d[i].v-q.v;
        
        /* Trivial V rejection: both on same side of U axis. */
        if ((v1 > 0) == (v2 > 0)) continue;
        /* Trivial rejection: both endpoints to the left. */
        if (u1 < 0 && u2 < 0) continue;
        /* Trivial acceptance: both endpoints on nonnegative U. */
        if (u1 >= 0 && u2 >= 0) { count++; continue; }
        /* Mixed U signs: calculate intersection with the U axis. */
        double u_cross = u1 + (u2-u1)*(-v1)/(v2-v1);
        if (u_cross >= 0) count++;
    }
    return count % 2;
}

int intersect_polygon(Position eye, Position ray, Object *object, double *t) {
    if (!object || object->type != POLYGON || !object->data || !t) return 0;
    const Polygon *p = (const Polygon *)object->data;
    /* Parallel ray: the plane-intersection denominator is zero. */
    double denom = dot(p->normal, ray);
    if (fabs(denom) < 0.00000001) return 0;
    double candidate = -(dot(p->normal, eye) + p->plane_d) / denom;
    if (candidate <= EPSILON) return 0;
    Position point = {eye.x+candidate*ray.x, eye.y+candidate*ray.y,
                      eye.z+candidate*ray.z};
    if (!contained_in_polygon(p, point)) return 0;
    *t = candidate;
    return 1;
}


/* Sphere and Object's parser */

int parse_sphere(const char *line, Position *position, Color *color, double *radius,
                 double *reflection_coefficient, double *ambient_coefficient,
                 double *specular_coefficient, int *shininess) {
    int used;
    double alpha;
    double parsed_shininess = 32.0;
    *specular_coefficient = 0.0;
    
    if (sscanf(line, "sphere %lf %lf %lf %lf %Lf %Lf %Lf %lf %lf %lf %n",
               &position->x, &position->y, &position->z,
               &alpha, &color->r, &color->g, &color->b,
               radius, reflection_coefficient, ambient_coefficient, &used) != 10) {
        return -1; 
    }

    // Parse optional specular coefficient and shininess if present
    sscanf(line + used, " %lf %lf", specular_coefficient, &parsed_shininess);
    if (alpha < 0.0 || alpha > 1.0 ||
        color->r < 0.0 || color->r > 1.0 ||
        color->g < 0.0 || color->g > 1.0 ||
        color->b < 0.0 || color->b > 1.0 ||
        *specular_coefficient < 0.0 || *specular_coefficient > 1.0 ||
        parsed_shininess < 1.0 || parsed_shininess > INT_MAX ||
        parsed_shininess != floor(parsed_shininess)) return -1;
    *shininess = (int)parsed_shininess;
    color->a = (uint8_t)(alpha * 255.0);
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
    double specular_coefficient;
    int shininess;

    if (strncmp(line, "sphere", 6) == 0) {
        if (parse_sphere(line, &position, &color, &radius,
                         &reflection_coefficient, &ambient_coefficient,
                         &specular_coefficient, &shininess) == 0) {
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
            new_object->specular_coefficient = specular_coefficient;
            new_object->shininess = shininess;
            *objects = add_object(*objects, new_object);
            return 0; 
        }
    }
    
    if (strncmp(line, "polygon", 7) == 0)
        return parse_polygon(line, objects);
    return -1;
}

void free_objects(Object *objects) {
    Object *current = objects;
    Object *next;

    while (current != NULL) {
        next = current->next;
        
        if (current->type == POLYGON && current->data) {
            Polygon *p = (Polygon *)current->data;
            free(p->vertices3d);
            free(p->vertices2d);
        }

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
        case POLYGON:
            return ((const Polygon *)object->data)->normal;
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

static double dot_product(Position a, Position b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
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
                Position reflected = {
                    2.0 * normal.x * cross_product_LN - l_vector.x,
                    2.0 * normal.y * cross_product_LN - l_vector.y,
                    2.0 * normal.z * cross_product_LN - l_vector.z
                };
                Position view = {-ray.x, -ray.y, -ray.z};
                double reflected_view = dot_product(reflected, view);
                
                // Calculate distance from intersection to light
                double dx = config.lights[k].position.x - intersection->position.x;
                double dy = config.lights[k].position.y - intersection->position.y;
                double dz = config.lights[k].position.z - intersection->position.z;
                double light_distance = sqrt(dx * dx + dy * dy + dz * dz);
                
                // Verifies if there is an obstacle's intersection
                Intersection *obstacle = first_intersection(intersection->position, l_vector, config);
                
                if (!obstacle || obstacle->t >= light_distance) {
                    double fatt = get_attenuation_factor(config.lights[k], intersection->position);
                    intensity += cross_product_LN * Q->reflection_coefficient * fatt * config.lights[k].light_intensity;
                    if (reflected_view > 0.0) {
                        intensity += pow(reflected_view, Q->shininess) *
                                     Q->specular_coefficient * fatt *
                                     config.lights[k].light_intensity;
                    }
                }
                
                free(obstacle);
            }
        };
        intensity += config.ambient_intensity * Q->ambient_coefficient;
        if (intensity > 1.0) intensity = 1.0;
        if (intensity < 0.0) intensity = 0.0;

        color.a = Q->color.a;
        color.r = Q->color.r * (long double)intensity;
        color.g = Q->color.g * (long double)intensity;
        color.b = Q->color.b * (long double)intensity;
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
                if (t < tmin && t > EPSILON) {
                    tmin = t;
                    closest = current;
                }
            }
        } else if (current->type == POLYGON) {
            if (intersect_polygon(eye, ray, current, &t) && t > EPSILON && t < tmin) {
                tmin = t;
                closest = current;
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
