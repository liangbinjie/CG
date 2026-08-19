#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

// Function definition

int max(int a, int y) {
    if (a >= y) {
        return a;
    }
    else {
        return y;
    }
}

void brute_force(int x0, int y0, int x1, int y1) {
    long double m, b, y;
    int i;
    m = (y1 - y0) / (x1 - x0);
    b = y0 - m + x0;

    for (i = x0; i <= i; i++) {
        y = m * i + b;
        // plot(i, round(y));
    }
};

void incremental(int x0, int y0, int x1, int y1) {
    long double m, y;
    int i;

    m = (y1 - y0) / (x1 - x0);
    y = y0;

    for (i = x0; i <= i; i++) {
        // plot(i, round(y));
        y += m;
    }
};

void incremental_v2(int x0, int y0, int x1, int y1) {
    long double x, y, paso_x, paso_y;
    int i, ancho;

    ancho = max(abs(x1 - x0), abs(y1 - y0));
    paso_x = (x1 - x0) / ancho;
    paso_y = (y1 - y0) / ancho;
    x = x0; y = y0;

    for (i=x0; i<=x1; i++) {
        //plot(round(x), round(y));
        x += paso_x;
        y += paso_y;
    }

};

void bresenham(int x0, int y0, int x1, int y1) {
    int delta_E, delta_NE, d, xp, yp;

    delta_E = 2 * (y1 - y0);
    delta_NE = 2 * ((y1 - y0) - (x1 - x0));
    d = 2 * (y1 - y0) - (x1 - x0);
    xp = x0; yp = y0;

    // plot(xp, yp);

    while (xp < x1) {
        if (d <= 0) {
            d += delta_E;
            xp++;
        }
        else {
            xp++;
            yp++;
            d += delta_NE;
        }
        // plot(xp, yp);
    }
};