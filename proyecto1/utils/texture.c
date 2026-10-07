#include "texture.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// WIDTH [4] HEIGHT [4] PIXEL [4] -> 4 bytes per pixel
static TEXTURE *SanJose_texture = NULL;
static TEXTURE *Alajuela_texture = NULL;
static TEXTURE *Heredia_texture = NULL;
static TEXTURE *Cartago_texture = NULL;
static TEXTURE *Guanacaste_texture = NULL;
static TEXTURE *Puntarenas_texture = NULL;
static TEXTURE *Limon_texture = NULL;

// TEXTURE[x%TH][y%TV] = TEXEL

// Load .avs file into a matrix
// Input: filename
// Output: returns a texel struct matrix
TEXTURE *load_avs(const char *filename) {
    int height, width;
    FILE *file = fopen(filename, "rb"); // read file as binaries

    if (file == NULL) {
        printf("No file found.\n");
        return NULL;
    }

    // define a 4 bytes buffer to read the header
    unsigned char buffer[4];

    // read first 4 bytes of the header -> width
    fread(buffer, 1, 4, file);

    // arrange big endian bytes to little endian
    width = ((int)buffer[0] << 24) |
            ((int)buffer[1] << 16) |
            ((int)buffer[2] << 8) |
            buffer[3];

    // read the follwing 4 bytes -> height
    fread(buffer, 1, 4, file);

    // arrange header big endian bytes to little endian
    height = ((int)buffer[0] << 24) |
            ((int)buffer[1] << 16) |
            ((int)buffer[2] << 8) |
            buffer[3];

    // this initializeas a malloc of x height of the image, with the size of the pixel
    TEXTURE *texture = malloc(sizeof(TEXTURE));

     if (texture == NULL) {
        fclose(file);
        return NULL;
    }

    // asign the width and height to the texture struct
    texture->width = width;
    texture->height = height;

    // Allocate rows
    texture->pixels = malloc(height * sizeof(TEXEL *));

    if (texture->pixels == NULL) {
        free(texture);
        fclose(file);
        return NULL;
    }

    // Allocate each row
    for (int y = 0; y < height; y++) {
        texture->pixels[y] = malloc(width * sizeof(TEXEL));

        if (texture->pixels[y] == NULL) {
            // Free previously allocated rows
            for (int i = 0; i < y; i++) {
                free(texture->pixels[i]);
            }

            free(texture->pixels);
            free(texture);
            fclose(file);

            return NULL;
        }
    }

    // Read pixels
    for (int y = height-1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {

            unsigned char pixel[4];

            fread(pixel, 1, 4, file);

            texture->pixels[y][x].a = pixel[0];
            texture->pixels[y][x].r = pixel[1];
            texture->pixels[y][x].g = pixel[2];
            texture->pixels[y][x].b = pixel[3];
        }
    }

    fclose(file);

    return texture;
}

void free_texture(TEXTURE *texture) {
    if (texture == NULL)
        return;

    for (int y = 0; y < texture->height; y++) {
        free(texture->pixels[y]);
    }

    free(texture->pixels);
    free(texture);
}


TEXTURE *get_province_texture(const char *province_name) {

    if (strcmp(province_name, "SanJose") == 0) {
        return SanJose_texture;
    }

    if (strcmp(province_name, "Alajuela") == 0) {
        return Alajuela_texture;
    }

    if (strcmp(province_name, "Heredia") == 0) {
        return Heredia_texture;
    }

    if (strcmp(province_name, "Cartago") == 0) {
        return Cartago_texture;
    }

    if (strcmp(province_name, "Guanacaste") == 0) {
        return Guanacaste_texture;
    }

    if (strcmp(province_name, "Puntarenas") == 0) {
        return Puntarenas_texture;
    }

    if (strcmp(province_name, "Limon") == 0) {
        return Limon_texture;
    }

    return NULL;
}

void free_all_province_textures(void) {
    free_texture(SanJose_texture);
    free_texture(Alajuela_texture);
    free_texture(Heredia_texture);
    free_texture(Cartago_texture);
    free_texture(Guanacaste_texture);
    free_texture(Puntarenas_texture);
    free_texture(Limon_texture);

    SanJose_texture = NULL;
    Alajuela_texture = NULL;
    Heredia_texture = NULL;
    Cartago_texture = NULL;
    Guanacaste_texture = NULL;
    Puntarenas_texture = NULL;
    Limon_texture = NULL;
}

void load_all_province_textures(void) {
    SanJose_texture = load_avs("textures/SanJose.avs");
    Alajuela_texture = load_avs("textures/Alajuela.avs");
    Heredia_texture = load_avs("textures/Heredia.avs");
    Cartago_texture = load_avs("textures/Cartago.avs");
    Guanacaste_texture = load_avs("textures/Guanacaste.avs");
    Puntarenas_texture = load_avs("textures/Puntarenas.avs");
    Limon_texture = load_avs("textures/Limon.avs");
}

