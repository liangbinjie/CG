#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "mesa.h"
#include <math.h>

// The buffer is needed for plot
extern COLOR **buffer;
extern int resolution;
extern Plot plot;
COLOR color;

// Function definition

void plotEmpty(int x, int y) {
    return;
}

void plotMesa(int x, int y) {
    int newX = x + resolution / 2;
    int newY = y + resolution / 2;
    
    if (newX >= 0 && newX < resolution && newY >= 0 && newY < resolution) {
        buffer[newX][newY].r = color.r;
        buffer[newX][newY].g = color.g;
        buffer[newX][newY].b = color.b;
    }
}

void setColor(float r, float g, float b) {
    color.r = r;
    color.g = g;
    color.b = b;
}

int max(int a, int y) {
    if (a >= y) {
        return a;
    }
    else {
        return y;
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
        plot(x0, y0);
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
            plot(round(x), i);
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
            plot(i, round(y));
        }
    }
};

void incremental(int x0, int y0, int x1, int y1) {
    long double m;
    int i;
    
    // Case to avoid division by zero
    if (x0 == x1 && y0 == y1) {
        plot(x0, y0);
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
            plot(round(x), i);
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
            plot(i, round(y));
            y += m;
        }
    }
};

void incremental_v2(int x0, int y0, int x1, int y1) {
    long double x, y, paso_x, paso_y;
    int i, ancho;

    ancho = max(abs(x1 - x0), abs(y1 - y0));
    paso_x = (long double)(x1 - x0) / ancho;
    paso_y = (long double)(y1 - y0) / ancho;
    x = x0; y = y0;

    for (i = 0; i <= ancho; i++) {
        plot(round(x), round(y));
        x += paso_x;
        y += paso_y;
    }
};

void bresenham(int x0, int y0, int x1, int y1) {
    int Delta_NE, Delta_N, Delta_NO, Delta_O;
    int Delta_SO, Delta_S, Delta_SE, Delta_E;
    int d, Xp, Yp;
    int delta_x = abs(x1 - x0);
    int delta_y = abs(y1 - y0);

    Xp = x0;
    Yp = y0;
    plot(Xp, Yp);

    // Un solo punto
    if (x0 == x1 && y0 == y1) {
        return;
    }

    // Octantes 1, 4, 5 y 8: X domina
    if (delta_x >= delta_y) {
        Delta_E = 2 * delta_y;
        Delta_NE = 2 * (delta_y - delta_x);
        Delta_O = 2 * delta_y;
        Delta_NO = 2 * (delta_y - delta_x);
        Delta_SO = 2 * (delta_y - delta_x);
        Delta_SE = 2 * (delta_y - delta_x);

        d = 2 * delta_y - delta_x;

        if (x1 > x0 && y1 >= y0) {
            // Primer octante: E o NE
            while (Xp < x1) {
                if (d <= 0) {
                    Xp++;
                    d += Delta_E;
                }
                else {
                    Xp++;
                    Yp++;
                    d += Delta_NE;
                }
                plot(Xp, Yp);
            }
        }
        else if (x1 < x0 && y1 >= y0) {
            // Cuarto octante: O o NO
            while (Xp > x1) {
                if (d <= 0) {
                    Xp--;
                    d += Delta_O;
                }
                else {
                    Xp--;
                    Yp++;
                    d += Delta_NO;
                }
                plot(Xp, Yp);
            }
        }
        else if (x1 < x0 && y1 < y0) {
            // Quinto octante: O o SO
            while (Xp > x1) {
                if (d <= 0) {
                    Xp--;
                    d += Delta_O;
                }
                else {
                    Xp--;
                    Yp--;
                    d += Delta_SO;
                }
                plot(Xp, Yp);
            }
        }
        else {
            // Octavo octante: E o SE
            while (Xp < x1) {
                if (d <= 0) {
                    Xp++;
                    d += Delta_E;
                }
                else {
                    Xp++;
                    Yp--;
                    d += Delta_SE;
                }
                plot(Xp, Yp);
            }
        }
    }
    // Octantes 2, 3, 6 y 7: Y domina
    else {
        Delta_N = 2 * delta_x;
        Delta_NE = 2 * (delta_x - delta_y);
        Delta_NO = 2 * (delta_x - delta_y);
        Delta_S = 2 * delta_x;
        Delta_SO = 2 * (delta_x - delta_y);
        Delta_SE = 2 * (delta_x - delta_y);

        d = 2 * delta_x - delta_y;

        if (y1 > y0 && x1 >= x0) {
            // Segundo octante: N o NE
            while (Yp < y1) {
                if (d <= 0) {
                    Yp++;
                    d += Delta_N;
                }
                else {
                    Yp++;
                    Xp++;
                    d += Delta_NE;
                }
                plot(Xp, Yp);
            }
        }
        else if (y1 > y0 && x1 < x0) {
            // Tercer octante: N o NO
            while (Yp < y1) {
                if (d <= 0) {
                    Yp++;
                    d += Delta_N;
                }
                else {
                    Yp++;
                    Xp--;
                    d += Delta_NO;
                }
                plot(Xp, Yp);
            }
        }
        else if (y1 < y0 && x1 < x0) {
            // Sexto octante: S o SO
            while (Yp > y1) {
                if (d <= 0) {
                    Yp--;
                    d += Delta_S;
                }
                else {
                    Yp--;
                    Xp--;
                    d += Delta_SO;
                }
                plot(Xp, Yp);
            }
        }
        else {
            // Septimo octante: S o SE
            while (Yp > y1) {
                if (d <= 0) {
                    Yp--;
                    d += Delta_S;
                }
                else {
                    Yp--;
                    Xp++;
                    d += Delta_SE;
                }
                plot(Xp, Yp);
            }
        }
    }
};
