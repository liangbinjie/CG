#ifndef ROTATE_H
#define ROTATE_H

#include "coordinates.h"

extern double angle;

COORDINATES rotate_point(COORDINATES point);

void rotate_clockwise(double angle);
void rotate_counterclockwise(double angle);

#endif
