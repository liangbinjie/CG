#include "draw_line.h"
#include <GL/gl.h>
#include <stdlib.h>

void plot(int x, int y) {
    glVertex2i(x, y);
}

void setColor(float r, float g, float b) {
    glColor3f(r, g, b);
}

void swap(int *x0, int *y0, int *x1, int *y1) {
    int temp;

    temp = *x0;
    *x0 = *x1;
    *x1 = temp;

    temp = *y0;
    *y0 = *y1;
    *y1 = temp;
}

void bressenhamH(int x0, int y0, int x1, int y1) {
    // always start from left to right
    // draws NE, E, SE

    int dx, dy;

    if (x0 > x1) { 
        swap(&x0, &y0, &x1, &y1);
    }

    dx = x1-x0;
    dy = y1-y0;

    int dirY;

    if (dy >= 0) {
        dirY = 1;      // NE
    } else {
        dirY = -1;     // SE
        dy = -dy;
    }

    int delta_E = 2 * dy; 
    int delta_D = 2 * (dy - dx); // valid delta for NE or SE movement
    int d = 2 * dy - dx;

    int xp = x0;
    int yp = y0;

    plot(xp, yp);

    while (xp < x1) {

        if (d <= 0) {
            // E
            xp++;
            d += delta_E;
        }
        else {
            // NE or SE
            xp++;
            yp += dirY;
            d += delta_D;
        }
        plot(xp, yp);
    }
};

void bressenhamV(int x0, int y0, int x1, int y1)
{
    // Always draw from bottom to top
    if (y0 > y1) {
        swap(&x0, &y0, &x1, &y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;

    int dirX;

    // Determine whether diagonal movement is NE or NW
    if (dx >= 0) {
        dirX = 1;      // NE
    }
    else {
        dirX = -1;     // NW
        dx = -dx;
    }

    int delta_N = 2 * dx;
    int delta_D = 2 * (dx - dy);

    int d = 2 * dx - dy;

    int xp = x0;
    int yp = y0;

    plot(xp, yp);

    while (yp < y1) {

        if (d <= 0) {
            // N
            yp++;
            d += delta_N;
        }
        else {
            // NE or NW
            xp += dirX;
            yp++;
            d += delta_D;
        }

        plot(xp, yp);
    }
}

void draw_line(int x0, int y0, int x1, int y1) {
    // check if X is dominant or Y
    if (abs(x1-x0) > abs(y1-y0)) {
        // if X is dominant, most likely is Horizontal displacement
        bressenhamH(x0,y0,x1,y1);
    } else {
        // else Y is dominant
        bressenhamV(x0,y0,x1,y1);
    }
}



