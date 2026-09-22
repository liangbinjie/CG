#ifndef UTILS_H
#define UTILS_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Function declaration (or prototype)

void bresenham(int x0, int y0, int x1, int y1);

void bressenham_line(int x0, int y0, int x1, int y1);

void bressenham_line_empty(int x0, int y0, int x1, int y1);

// Auxiliary functions

int max(int a, int b);

void setColor(float r, float g, float b);

typedef void (*Algorithm)(int, int, int, int);

typedef void (*Plot)(int, int);

void plotEmpty(int x, int y);

void plotMesa(int x, int y);

void plotFramebuffer(int x, int y);

void bezierCubic(int x0, int y0, int x1, int y1,
				int x2, int y2, int x3, int y3, int steps);

#endif
