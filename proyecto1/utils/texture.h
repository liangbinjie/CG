#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} TEXEL;

typedef struct {
    int width;
    int height;
    TEXEL **pixels;
} TEXTURE;

// use imagemagick to generate avi image

TEXTURE *load_avs(const char *filename); // load .avs image texture

void free_texture(TEXTURE *texture); // free the allocated memory for the texture

#endif