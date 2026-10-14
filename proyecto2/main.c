#include "utils/utils.h"
#include "utils/interface.h"
#include "config/config.h"
#include "object/object.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils/avs.h"

Color **create_buffer(Config config) {
    Color **framebuffer = (Color **)malloc(config.height * sizeof(Color *));

    for (int i = 0; i < config.height; i++) {
        framebuffer[i] = (Color *)malloc(config.width * sizeof(Color));
    }
    return framebuffer;
}

void fill_buffer(Color **framebuffer, Config config) {
    Color color;
    Position ray;
    
    double Xw, Yw, Zw, L;
    double Xe = config.eye.x;
    double Ye = config.eye.y;
    double Ze = config.eye.z;
    
    for (int i = 0; i < config.width; i++) {
        for (int j = 0; j < config.height; j++) {
            
            Xw = ((i+0.5)/config.width) * 
                 (config.window.xmax - config.window.xmin) +
                 config.window.xmin;
            Yw = ((j+0.5)/config.height) * 
                 (config.window.ymax - config.window.ymin) +
                 config.window.ymin;
            Zw = 0.0;
            
            L = sqrt((Xw-Xe)*(Xw-Xe) + 
                     (Yw-Ye)*(Yw-Ye) + 
                     (Zw-Ze)*(Zw-Ze));
            
            ray.x = (Xw-Xe)/L;
            ray.y = (Yw-Ye)/L;
            ray.z = (Zw-Ze)/L;
	    
	    color = de_que_color(config.eye, ray, config);
	    
            framebuffer[j][i] = color;

        }
    }
}

void free_buffer(Color **framebuffer, Config config) {
    for (int i = 0; i < config.height; i++) {
        free(framebuffer[i]);
    }

    free(framebuffer);
}

void load_objects(Config config) {
    printf("Objects loaded:\n");
    Object *current = config.objects;
    while (current != NULL) {
        if (current->type == SPHERE) {
            Sphere *sphere_data = (Sphere *)current->data;
            printf("Sphere at (%.2f, %.2f, %.2f) with radius %.2f and color (A: %d, R: %d, G: %d, B: %d)\n",
                   current->position.x, current->position.y, current->position.z,
                   sphere_data->radius,
                   current->color.a, current->color.r, current->color.g, current->color.b);
        }
        current = current->next;
    }
}

int main(int argc, char **argv) {
    Config config;
    Color **framebuffer;

    if (load_config(&config) != 0) {
        fprintf(stderr, "ERROR: Failed to load config file.\n");
        return 1;
    }
    
    load_objects(config);
    
    framebuffer = create_buffer(config);
    
    fill_buffer(framebuffer, config);
    
    free_buffer(framebuffer, config);
    
    free_objects(config.objects);

    return 0;
}
