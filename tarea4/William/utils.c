#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "mesa.h"
#include <math.h>

extern COLOR **buffer;
extern int resolution;
COLOR color;

// Function definition

void plot(int x, int y) {
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

POINT bezierPoint(POINT p0, POINT p1, POINT p2, POINT p3, double t) {
    POINT Q;
    double x, y;

    x = (1.0 - t) * (1.0 - t) * (1.0 - t) * p0.x + 
        3 * (1.0 - t) * (1.0 - t) * t * p1.x +
        3 * (1.0 - t) * t * t * p2.x +
        t * t * t * p3.x;

    y = (1.0 - t) * (1.0 - t) * (1.0 - t) * p0.y +
        3 * (1.0 - t) * (1.0 - t) * t * p1.y +
        3 * (1.0 - t) * t * t * p2.y +
        t * t * t * p3.y;

    Q.x = round(x);
    Q.y = round(y);

    return Q;
}

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
