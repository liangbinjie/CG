#ifndef ROTATE_H
#define ROTATE_H

#include "coordinates.h"

#define PI 3.14159265358979323846

#define ROTATE_NORMAL  5.0
#define ROTATE_SLOW    1.0
#define ROTATE_FAST    15.0

extern double angle;

COORDINATES rotate_point(COORDINATES point);

void rotate_clockwise(double angle);
void rotate_counterclockwise(double angle);
double get_rotate_value();

#endif
