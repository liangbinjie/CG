#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>

typedef struct {
    unsigned char a;
    unsigned char r;
    unsigned char g;
    unsigned char b;
} TEXEL;

typedef struct {
    int width;
    int height;
    TEXEL **pixels;
} TEXTURE;

TEXTURE *load_avs(const char *filename);
TEXTURE *get_province_texture(const char *province_name);
void free_all_province_textures(void);
void load_all_province_textures(void);
void free_texture(TEXTURE *texture);


#endif