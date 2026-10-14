#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define CONFIG_FILE "config.cfg"

typedef struct {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

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
    int width;
    int height;
    Window window;
    char output_name[100];
    Position eye;
    Pixel background_color;

    // Linked List of Objects
    // Object *objects;
} Config;

int load_config(Config *config);

#endif // CONFIG_H