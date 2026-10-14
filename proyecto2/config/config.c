/*

* Check the config.h file for the structure of the Config struct and the load_config function.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "../object/object.h"

int load_config(Config *config) {
    if (!config) return 1;
    config->objects = NULL;

    FILE *file = fopen(CONFIG_FILE, "r");
    char line[256];

    if (file == NULL) {
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "object=", 7) == 0) {
            // Parse the object line
            if (parse_object(line + 7, &config->objects) != 0) {
                fclose(file);
                return 1; // Error parsing object
            }
        } else {
            // Parse other configuration parameters
            sscanf(line, "width=%d", &config->width);
            sscanf(line, "height=%d", &config->height);
            sscanf(line, "window_xmin=%d", &config->window.xmin);
            sscanf(line, "window_ymin=%d", &config->window.ymin);
            sscanf(line, "window_xmax=%d", &config->window.xmax);
            sscanf(line, "window_ymax=%d", &config->window.ymax);
            sscanf(line, "output_name=%s", config->output_name);
            sscanf(line, "eye_x=%lf", &config->eye.x);
            sscanf(line, "eye_y=%lf", &config->eye.y);
            sscanf(line, "eye_z=%lf", &config->eye.z);
            sscanf(line, "background_color_a=%hhu", &config->background_color.a);
            sscanf(line, "background_color_r=%hhu", &config->background_color.r);
            sscanf(line, "background_color_g=%hhu", &config->background_color.g);
            sscanf(line, "background_color_b=%hhu", &config->background_color.b);
        }

        sscanf(line, "width=%d", &config->width);
        sscanf(line, "height=%d", &config->height);
        sscanf(line, "window_xmin=%d", &config->window.xmin);
        sscanf(line, "window_ymin=%d", &config->window.ymin);
        sscanf(line, "window_xmax=%d", &config->window.xmax);
        sscanf(line, "window_ymax=%d", &config->window.ymax);
        sscanf(line, "output_name=%s", config->output_name);
        sscanf(line, "eye_x=%lf", &config->eye.x);
        sscanf(line, "eye_y=%lf", &config->eye.y);
        sscanf(line, "eye_z=%lf", &config->eye.z);
        sscanf(line, "background_color_a=%hhu", &config->background_color.a);
        sscanf(line, "background_color_r=%hhu", &config->background_color.r);
        sscanf(line, "background_color_g=%hhu", &config->background_color.g);
        sscanf(line, "background_color_b=%hhu", &config->background_color.b);
    }

    fclose(file);
    return 0;
}
