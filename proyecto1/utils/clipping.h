#ifndef CLIPPING_H
#define CLIPPING_H

#include "coordinates.h"

// Region codes are defined
#define IN  0      // 0000
#define LEFT    1  // 0001
#define RIGHT   2  // 0010
#define BOTTOM  4  // 0100
#define TOP     8  // 1000

int get_code(double x, double y);
int cohen_sutherland(COORDINATES *p0, COORDINATES *p1);

#endif
