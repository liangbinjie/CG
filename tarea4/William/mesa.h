#ifndef MESA_H
#define MESA_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Base structures

typedef struct {
    double r;
    double g;
    double b;
} COLOR;

typedef struct {
    int x0, y0;
    int x1, y1;
} LINE_POINT;

// Structures for Bezier

typedef struct {
    int x, y;
} POINT;

typedef struct {
    POINT p0;
    POINT p1;
    POINT p2;
    POINT p3;
} BEZIER_CURVE;

#endif
