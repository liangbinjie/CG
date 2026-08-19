#ifndef UTILS_H
#define UTILS_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Function declaration (or prototype)
void brute_force(int x0, int y0, int x1, int y1);

void incremental(int x0, int y0, int x1, int y1);

void incremental_v2(int x0, int y0, int x1, int y1);

void bresenham(int x0, int y0, int x1, int y1);

int max(int a, int b);

#endif