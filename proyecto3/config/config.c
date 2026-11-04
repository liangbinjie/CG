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
    double background_alpha = 1.0;
    config->objects = NULL;
    config->lights = NULL;
    config->light_count = 0;

    FILE *file = fopen(CONFIG_FILE, "r");
    char line[1024];

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
        } 

        else if (strncmp(line, "light=", 6) == 0) {
            Light light;
            if (sscanf(line + 6, " %lf %lf %lf %lf %lf %lf %lf",
                       &light.position.x, &light.position.y, &light.position.z,
                       &light.light_intensity, &light.c1, &light.c2, &light.c3) != 7) {
                free(config->lights);
                config->lights = NULL;
                config->light_count = 0;
                fclose(file); // if error
                return 1;
            }

            Light *lights = realloc(config->lights,
                                    (size_t)(config->light_count + 1) * sizeof(*lights));
            if (!lights) {
                free(config->lights);
                config->lights = NULL;
                config->light_count = 0;
                fclose(file);
                return 1;
            }
            config->lights = lights;
            config->lights[config->light_count++] = light;
        }
        
        else {
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
            sscanf(line, "background_color_a=%lf", &background_alpha);
            sscanf(line, "background_color_r=%Lf", &config->background_color.r);
            sscanf(line, "background_color_g=%Lf", &config->background_color.g);
            sscanf(line, "background_color_b=%Lf", &config->background_color.b);
            sscanf(line, "ambient_intensity=%lf",&config->ambient_intensity );
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
        sscanf(line, "background_color_a=%lf", &background_alpha);
        sscanf(line, "background_color_r=%Lf", &config->background_color.r);
        sscanf(line, "background_color_g=%Lf", &config->background_color.g);
        sscanf(line, "background_color_b=%Lf", &config->background_color.b);
    }

    if (background_alpha < 0.0 || background_alpha > 1.0 ||
        config->background_color.r < 0.0 || config->background_color.r > 1.0 ||
        config->background_color.g < 0.0 || config->background_color.g > 1.0 ||
        config->background_color.b < 0.0 || config->background_color.b > 1.0) {
        free(config->lights);
        free_objects(config->objects);
        config->lights = NULL;
        config->objects = NULL;
        fclose(file);
        return 1;
    }

    config->background_color.a = (uint8_t)(background_alpha * 255.0);

    fclose(file);
    return 0;
}
