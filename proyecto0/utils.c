#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "mesa.h"
#include <math.h>

// The buffer is needed for plot
extern COLOR **buffer;
extern int resolution;

// Function definition

int max(int a, int y) {
    if (a >= y) {
        return a;
    }
    else {
        return y;
    }
}

void plot(int x, int y, float r, float g, float b) {
    if (x >= 0 && x < resolution && y >= 0 && y < resolution) {
        buffer[x][y].r = r;
        buffer[x][y].g = g;
        buffer[x][y].b = b;
    }
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

// Straight Lines Alghorithms

void brute_force(int x0, int y0, int x1, int y1) {
    long double m, b;
    int i;
    
    // Case to avoid division by zero
    if (x0 == x1 && y0 == y1) {
        plot(x0, y0, 1.0, 0, 0);
        return;
    }
    
    if (abs(y1 - y0) > abs(x1 - x0)) { // Greater density in y
        
        // Verifies if the points need a change of variables
        if (y0 > y1) {
            swap(&x0, &y0, &x1, &y1);
        }
    
        m = (long double)(x1 - x0) / (y1 - y0);
        b = x0 - m * y0;

        for (i = y0; i <= y1; i++) {
            long double x = m * i + b;
            plot(round(x), i, 1.0, 0, 0);
        }
    } else { // Greater density in x
            
        // Verifies if the points need a change of variables
        if (x0 > x1) {
            swap(&x0, &y0, &x1, &y1);
        }
    
        m = (long double)(y1 - y0) / (x1 - x0);
        b = y0 - m * x0;

        for (i = x0; i <= x1; i++) {
            long double y = m * i + b;
            plot(i, round(y), 1.0, 0, 0);
        }
    }
};

void incremental(int x0, int y0, int x1, int y1) {
    long double m;
    int i;
    
    // Case to avoid division by zero
    if (x0 == x1 && y0 == y1) {
        plot(x0, y0, 1.0, 0, 0);
        return;
    }
    
    if (abs(y1 - y0) > abs(x1 - x0)) { // Greater density in y
        
        // Verifies if the points need a change of variables
        if (y0 > y1) {
            swap(&x0, &y0, &x1, &y1);
        }
        
        long double x;
        m = (long double)(x1 - x0) / (y1 - y0);
        x = x0;

        for (i = y0; i <= y1; i++) {
            plot(round(x), i, 0, 1.0, 0);
            x += m;
        }
        
     } else {
    
        // Verifies if the points need a change of variables
        if (x0 > x1) {
            swap(&x0, &y0, &x1, &y1);
        }
    
        long double y;
        m = (long double)(y1 - y0) / (x1 - x0);
        y = y0;

        for (i = x0; i <= x1; i++) {
            plot(i, round(y), 0, 1.0, 0);
            y += m;
        }
    }
};

void incremental_v2(int x0, int y0, int x1, int y1) {
    long double x, y, paso_x, paso_y;
    int i, ancho;

    ancho = max(abs(x1 - x0), abs(y1 - y0));
    paso_x = (x1 - x0) / ancho;
    paso_y = (y1 - y0) / ancho;
    x = x0; y = y0;

    for (i = x0; i <= x1; i++) {
        plot(round(x), round(y), 0, 0, 1.0);
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

    plot(xp, yp, 1.0, 1.0, 0);

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
        plot(xp, yp, 1.0, 1.0, 0);
    }
};
