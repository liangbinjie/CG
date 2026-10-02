#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

typedef struct {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} TEXEL;

// use imagemagick to generate avi image

TEXEL **load_avs(const char *filename); // load .avs image texture

#endif