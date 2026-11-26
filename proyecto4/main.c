#include "config/config.h"
#include "object/object.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils/avs.h"

static const int MAX_RECURSION = 5;
static const long double MAX_COLOR_DISTANCE = 1.7320508075688772935L;
static const int ANTIALIASING_ENABLED = 1;

Color **create_buffer(Config config) {
    Color **framebuffer = (Color **)malloc(config.height * sizeof(Color *));

    for (int i = 0; i < config.height; i++) {
        framebuffer[i] = (Color *)malloc(config.width * sizeof(Color));
    }
    return framebuffer;
}

static long double max_color_distance(const Color colors[4]) {
    long double maximum = 0.0L;
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            long double dr = colors[i].r - colors[j].r;
            long double dg = colors[i].g - colors[j].g;
            long double db = colors[i].b - colors[j].b;
            long double d = sqrtl(dr * dr + dg * dg + db * db);
            if (d > maximum) 
                maximum = d;
        }
    }
    return maximum;
}

static Color average_colors(Color a, Color b, Color c, Color d) {
    return (Color){
        .a = (uint8_t)((a.a + b.a + c.a + d.a + 2) / 4),
        .r = (a.r + b.r + c.r + d.r) / 4.0L,
        .g = (a.g + b.g + c.g + d.g) / 4.0L,
        .b = (a.b + b.b + c.b + d.b) / 4.0L
    };
}

// Trace a ray through any point in pixel coordinates.
static Color sample_ray(double x, double y, Config config) {
    double Xw = (x / config.width) * (config.window.xmax - config.window.xmin) + config.window.xmin;
    double Yw = (y / config.height) * (config.window.ymax - config.window.ymin) + config.window.ymin;

    Position ray = {Xw - config.eye.x, Yw - config.eye.y, -config.eye.z};
    double L = sqrt(ray.x * ray.x + ray.y * ray.y + ray.z * ray.z);
    
    if (L == 0.0) 
        return config.background_color;
    ray.x /= L;
    ray.y /= L;
    ray.z /= L;
    return de_que_color(config.eye, ray, config, MAX_REFLECTION_DEPTH);
}

// Corner order: (x0,y0), (x1,y0), (x0,y1), (x1,y1).
static Color sampling(double x0, double y0, double x1, double y1, Color c1, Color c2, Color c3, Color c4, int depth, Config config) {
    Color colors[4] = {c1, c2, c3, c4};
    if (depth >= MAX_RECURSION || max_color_distance(colors) <= 0.05L * MAX_COLOR_DISTANCE) {
        return average_colors(c1, c2, c3, c4);
    }

    double xm = (x0 + x1) / 2.0;
    double ym = (y0 + y1) / 2.0;

    Color top    = sample_ray(xm, y0, config);
    Color left   = sample_ray(x0, ym, config);
    Color center = sample_ray(xm, ym, config);
    Color right  = sample_ray(x1, ym, config);
    Color bottom = sample_ray(xm, y1, config);

    Color s1 = sampling(x0, y0, xm, ym, c1, top, left, center, depth + 1, config);
    Color s2 = sampling(xm, y0, x1, ym, top, c2, center, right, depth + 1, config);
    Color s3 = sampling(x0, ym, xm, y1, left, center, c3, bottom, depth + 1, config);
    Color s4 = sampling(xm, ym, x1, y1, center, right, bottom, c4, depth + 1, config);
    return average_colors(s1, s2, s3, s4);
}

void fill_buffer(Color **framebuffer, Config config) {
    for (int i = 0; i < config.width; i++) {
        for (int j = 0; j < config.height; j++) {
            if (!ANTIALIASING_ENABLED) {
                framebuffer[j][i] = sample_ray(
                    i + 0.5, j + 0.5, config
                );
                continue;
            }
            
            Color c1 = sample_ray(i, j, config);
            Color c2 = sample_ray(i + 1.0, j, config);
            Color c3 = sample_ray(i, j + 1.0, config);
            Color c4 = sample_ray(i + 1.0, j + 1.0, config);
            framebuffer[j][i] = sampling(i, j, i + 1.0, j + 1.0, c1, c2, c3, c4, 0, config);
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
            
        } else if (current->type == POLYGON) {
            Polygon *polygon = (Polygon *)current->data;
            printf("Polygon with %d vertices, normal (%.3f, %.3f, %.3f)\n", 
            polygon->vertex_count, polygon->normal.x, polygon->normal.y, polygon->normal.z);
        } else if (current->type == DISK) {
            Disk *disk = (Disk *)current->data;

            printf("Disk with outer radius %.3f, inner radius %.3f, normal (%.3f, %.3f, %.3f)\n", disk->outer_radius, disk->inner_radius, disk->normal.x, disk->normal.y, disk->normal.z);
        }      
        current = current->next;
    }
}

int main(int argc, char **argv) {
    Config config;
    Color **framebuffer;
    char avs_filename[sizeof(config.output_name) + 5];

    if (load_config(&config) != 0) {
        fprintf(stderr, "ERROR: Failed to load config file.\n");
        return 1;
    }
    
    framebuffer = create_buffer(config);
    
    fill_buffer(framebuffer, config);

    if (snprintf(avs_filename, sizeof(avs_filename), "%s.avs",
                 config.output_name) < 0 ||
        save_avs(avs_filename, framebuffer, config.width, config.height) != 0) {
        fprintf(stderr, "ERROR: Failed to save AVS file.\n");
        free_buffer(framebuffer, config);
        free_objects(config.objects);
        for (int i = 0; i < config.texture_count; i++) free_texture(&config.textures[i]);
        free(config.textures);
        return 1;
    }

    if (display_avs(avs_filename) != 0) {
        fprintf(stderr, "ERROR: Could not display AVS with ImageMagick.\n");
        free_buffer(framebuffer, config);
        free_objects(config.objects);
        for (int i = 0; i < config.texture_count; i++) free_texture(&config.textures[i]);
        free(config.textures);
        return 1;
    }
    
    free_buffer(framebuffer, config);
    
    free_objects(config.objects);
    for (int i = 0; i < config.texture_count; i++) free_texture(&config.textures[i]);
    free(config.textures);

    return 0;
}
