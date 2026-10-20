#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

typedef struct Object Object;

#define CONFIG_FILE "config.cfg"

typedef struct {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

typedef struct {
    int xmin;
    int ymin;
    int xmax;
    int ymax;
} Window;

typedef struct {
    double x;
    double y;
    double z;
} Position;

typedef struct {
    double x;
    double y;
    double z;
} Vector;

// La luz tiene una posicion
// Como tambein una intensidad I_p
// La distancia hacia cierta luz tiende a decaer, eso nos da el factor de atenuacion, el factor de atenuacion tiene
// c1, c2, c3, en ninguna circumstancia deberia dar todos 0.

typedef struct {
    Position position;
    double light_intensity; // [0-1]

    // valores requeridos por el factor de atenuacion
    // el factor de atenuacion debe ser un numero entre 0 y 1
    double c1;
    double c2;
    double c3;
} Light;

typedef struct {
    int width;
    int height;
    char output_name[100];
    double ambient_intensity;
    
    Window window;
    Position eye;
    Color background_color;
    
    Light *lights;
    int light_count;
    // Linked List of Objects
    Object *objects;
} Config;

int load_config(Config *config);

#endif // CONFIG_H
