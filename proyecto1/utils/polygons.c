#include "polygons.h"
#include "color.h"
#include "draw_line.h"
#include "interface.h"
#include "mesa.h"
#include "rotate.h"
#include "texture.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>


// Map Texel coordinates to polygon coordinates
// Input: Width and height of the texture, and the polygon coordinates
// Output: Texel coordinates mapped to polygon coordinates
// TEXTURE[x%TH][y%TV] = TEXEL position in Framebuffer
static int mapTexelX_toPolygon(double Xw, int texture_width) {
    return (int)(texture_width * ((Xw - Xmin) / (Xmax - Xmin)));
}

static int mapTexelY_toPolygon(double Yw, int texture_height) {
    return (int)(texture_height * ((Yw - Ymin) / (Ymax - Ymin)));
}

// WIDTH [4] HEIGHT [4] PIXEL [4] -> 4 bytes per pixel
TEXTURE *avs_file;
int texture_width;
int texture_height;
TEXEL **texture;

// Load the texture for a specific province given its name
// Textures are stored in the "textures" folder with the format "ProvinceName.avs"
void load_province_texture(const char *ProvinceName) {
    if (avs_file != NULL) {
        free_texture(avs_file);
        avs_file = NULL;
        texture = NULL;
    }

    texture_width = 0;
    texture_height = 0;

    char filename[256];
    strcpy(filename, "textures/");
    strcat(filename, ProvinceName);
    strcat(filename, ".avs");

    avs_file = load_avs(filename);

    if (avs_file == NULL) {
        printf("Failed to load texture: %s\n", filename);
        return;
    }

    texture_width = avs_file->width;
    texture_height = avs_file->height;
    texture = avs_file->pixels;
}


// =============================================================================

static int mapX_toBuffer(double Xw) {
    return (int)(Hres * ((Xw - Xmin) / (Xmax - Xmin)));
}

static int mapY_toBuffer(double Yw) {
    return (int)(Vres * ((Yw - Ymin) / (Ymax - Ymin)));
}

static COLOR province_color(const char *name, int polygon_index) {
    if (strcmp(name, "Alajuela") == 0) return (COLOR){1.0, 0.0, 0.0};
    if (strcmp(name, "Heredia") == 0) return (COLOR){1.0, 1.0, 0.0};
    if (strcmp(name, "Puntarenas") == 0) return (COLOR){1.0, 0.5, 0.0};
    if (strcmp(name, "Limon") == 0) return (COLOR){0.0, 0.7, 0.0};
    if (strcmp(name, "Cartago") == 0) return (COLOR){0.0, 0.3, 1.0};
    if (strcmp(name, "Guanacaste") == 0) return (COLOR){0.45, 0.2, 0.05};
    return (COLOR){0.55, 0.0, 0.75};
}

static int compare_intersections(const void *left, const void *right) {
    double first = *(const double *)left;
    double second = *(const double *)right;

    return (first > second) - (first < second);
}

void fill_textured_polygon(const POLYGON *polygon) {
    int point_count = polygon->size - 1;
    if (point_count < 3 || texture == NULL) return;

    int *x_points = malloc((size_t)point_count * sizeof(*x_points));
    int *y_points = malloc((size_t)point_count * sizeof(*y_points));
    double *intersections = malloc((size_t)point_count * sizeof(*intersections));
    if (x_points == NULL || y_points == NULL || intersections == NULL) {
        free(x_points);
        free(y_points);
        free(intersections);
        return;
    }

    int minimum_y = Vres;
    int maximum_y = 0;
    for (int i = 0; i < point_count; i++) {
        COORDINATES point = rotate_point(polygon->coordinates[i]);
        x_points[i] = mapX_toBuffer(point.x);
        y_points[i] = mapY_toBuffer(point.y);
        if (y_points[i] < minimum_y) minimum_y = y_points[i];
        if (y_points[i] > maximum_y) maximum_y = y_points[i];
    }

    if (minimum_y < 0) minimum_y = 0;
    if (maximum_y >= Vres) maximum_y = Vres - 1;

    glBegin(GL_POINTS);
    for (int y = minimum_y; y <= maximum_y; y++) {
        double scan_y = y + 0.5;
        int intersection_count = 0;

        for (int i = 0; i < point_count; i++) {
            int next = (i + 1) % point_count;
            int y0 = y_points[i];
            int y1 = y_points[next];

            if ((y0 <= scan_y && y1 > scan_y) ||
                (y1 <= scan_y && y0 > scan_y)) {
                double x0 = x_points[i];
                double x1 = x_points[next];
                intersections[intersection_count++] =
                    x0 + (scan_y - y0) * (x1 - x0) / (y1 - y0);
            }
        }

        qsort(intersections, (size_t)intersection_count,
              sizeof(*intersections), compare_intersections);

        for (int i = 0; i + 1 < intersection_count; i += 2) {
            int start_x = (int)ceil(intersections[i]);
            int end_x = (int)floor(intersections[i + 1]);
            for (int x = start_x; x <= end_x; x++) {
                if (x >= 0 && x < Hres) {
                    double Xw = Xmin + (Xmax - Xmin) * x / Hres;
                    double Yw = Ymin + (Ymax - Ymin) * y / Vres;
                    int texel_x = mapTexelX_toPolygon(Xw, texture_width);
                    int texel_y = mapTexelY_toPolygon(Yw, texture_height);
                    TEXEL texel = texture[texel_y % texture_height][texel_x % texture_width];
                    setColor(texel.r / 255.0, texel.g / 255.0, texel.b / 255.0);
                    glVertex2i(x, y);
                }
            }
        }
    }
    glEnd();
    free(x_points);
    free(y_points);
    free(intersections);
}

void fill_polygon(const POLYGON *polygon) {
    int point_count = polygon->size - 1;
    if (point_count < 3) return;

    int *x_points = malloc((size_t)point_count * sizeof(*x_points));
    int *y_points = malloc((size_t)point_count * sizeof(*y_points));
    double *intersections = malloc((size_t)point_count * sizeof(*intersections));
    if (x_points == NULL || y_points == NULL || intersections == NULL) {
        free(x_points);
        free(y_points);
        free(intersections);
        return;
    }

    int minimum_y = Vres;
    int maximum_y = 0;
    for (int i = 0; i < point_count; i++) {
        COORDINATES point = rotate_point(polygon->coordinates[i]);
        x_points[i] = mapX_toBuffer(point.x);
        y_points[i] = mapY_toBuffer(point.y);
        if (y_points[i] < minimum_y) minimum_y = y_points[i];
        if (y_points[i] > maximum_y) maximum_y = y_points[i];
    }

    if (minimum_y < 0) minimum_y = 0;
    if (maximum_y >= Vres) maximum_y = Vres - 1;

    glBegin(GL_POINTS);
    for (int y = minimum_y; y <= maximum_y; y++) {
        double scan_y = y + 0.5;
        int intersection_count = 0;

        for (int i = 0; i < point_count; i++) {
            int next = (i + 1) % point_count;
            int y0 = y_points[i];
            int y1 = y_points[next];

            if ((y0 <= scan_y && y1 > scan_y) ||
                (y1 <= scan_y && y0 > scan_y)) {
                double x0 = x_points[i];
                double x1 = x_points[next];
                intersections[intersection_count++] =
                    x0 + (scan_y - y0) * (x1 - x0) / (y1 - y0);
            }
        }

        qsort(intersections, (size_t)intersection_count,
              sizeof(*intersections), compare_intersections);

        for (int i = 0; i + 1 < intersection_count; i += 2) {
            int start_x = (int)ceil(intersections[i]);
            int end_x = (int)floor(intersections[i + 1]);
            for (int x = start_x; x <= end_x; x++) {
                if (x >= 0 && x < Hres) glVertex2i(x, y);
            }
        }
    }
    glEnd();

    free(x_points);
    free(y_points);
    free(intersections);
}

static void draw_polygon_outline(const POLYGON *polygon) {
    glBegin(GL_POINTS);
    for (int i = 0; i < polygon->size - 1; i++) {
        COORDINATES p0 = rotate_point(polygon->coordinates[i]);
        COORDINATES p1 = rotate_point(polygon->coordinates[i + 1]);

        draw_line(mapX_toBuffer(p0.x), mapY_toBuffer(p0.y),
                  mapX_toBuffer(p1.x), mapY_toBuffer(p1.y));
    }
    glEnd();
}

void draw_map(void) {
    // from coordinates.h -> get provinces and province_count
    for (int i = 0; i < province_count; i++) {
        PROVINCE province = provinces[i];

        // Load the texture for the current province
        load_province_texture(province.name);
        for (int j = 0; j < province.polygon_count; j++) {
            POLYGON polygon = province.polygons[j];

            if (fill_enabled) {
                COLOR color = province_color(province.name, j);
                setColor(color.r, color.g, color.b);
                fill_polygon(&polygon);
            }

            if (texture_enabled && texture != NULL) {
                fill_textured_polygon(&polygon);
            }

            setColor(1.0, 1.0, 1.0);
            draw_polygon_outline(&polygon);
        }
    }
}
