#ifndef AVS_H
#define AVS_H

#include "../config/config.h"

int save_avs(const char *filename, Color **framebuffer, int width, int height);
int display_avs(const char *filename);
int load_texture_avs(const char *filename, int id, Texture *texture);
void free_texture(Texture *texture);

#endif // AVS_H
