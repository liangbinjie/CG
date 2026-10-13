#include <stdio.h>
#include <string.h>
#include "config.h"

int load_config(Config *config) {
    FILE *file = fopen(CONFIG_FILE, "r");
    char line[100];

    if (file == NULL) {
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "width=%d", &config->width);
        sscanf(line, "height=%d", &config->height);
        sscanf(line, "eye_x=%lf", &config->eye_x);
        sscanf(line, "eye_y=%lf", &config->eye_y);
        sscanf(line, "eye_z=%lf", &config->eye_z);
        sscanf(line, "background_color_a=%hhu", &config->background_color.a);
        sscanf(line, "background_color_r=%hhu", &config->background_color.r);
        sscanf(line, "background_color_g=%hhu", &config->background_color.g);
        sscanf(line, "background_color_b=%hhu", &config->background_color.b);

    }

    fclose(file);
    return 0;
}