#ifndef POLYGONS_H
#define POLYGONS_H

#include "coordinates.h"
#include "texture.h"

void draw_map(void);
void fill_polygon(const POLYGON *polygon);
void fill_textured_polygon(
    const POLYGON *polygon,
    TEXTURE *texture
);

#endif