#ifndef UTILS_H
#define UTILS_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "mesa.h"

// Function declaration (or prototype)

void bresenham(int x0, int y0, int x1, int y1);

POINT bezierPoint(POINT p0, POINT p1, POINT p2, POINT p3, double t);

// Auxiliary functions

void setColor(float r, float g, float b);

void plot(int x, int y);

#endif
