#ifndef AVS_H
#define AVS_H

#include "../config/config.h"

int save_avs(const char *filename, Color **framebuffer, int width, int height);
int display_avs(const char *filename);

#endif // AVS_H
