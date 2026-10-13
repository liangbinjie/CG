#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_FILE "config.cfg"

typedef struct {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

typedef struct {
    int width;
    int height;
    double eye_x;
    double eye_y;
    double eye_z;
    Pixel background_color;
} Config;

int load_config(Config *config);

#endif // CONFIG_H