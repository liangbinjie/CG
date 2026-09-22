#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "mesa.h"
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// The buffer is needed for plot
extern COLOR **buffer;
extern int resolution;
extern Plot plot;
COLOR color;

// Function definition

void plotEmpty(int x, int y) {
    (void)x;
    (void)y;
    return;
}

void plotMesa(int x, int y) {
    int newX = x + resolution / 2;
    int newY = y + resolution / 2;
    
    if (newX >= 0 && newX < resolution && newY >= 0 && newY < framebufferHeight) {
        buffer[newY][newX].r = color.r;
        buffer[newY][newX].g = color.g;
        buffer[newY][newX].b = color.b;
    }
}

void plotFramebuffer(int x, int y) {
    if (x >= 0 && x < resolution && y >= 0 && y < framebufferHeight) {
        buffer[y][x] = color;
        glClear(GL_COLOR_BUFFER_BIT);
        glRasterPos2i(0, 0);
        glDrawPixels(resolution, framebufferHeight, GL_RGB, GL_FLOAT, buffer[0]);
        glFlush();
        glutSwapBuffers();
#ifdef _WIN32
        Sleep(4);
#else
        usleep(4000);
#endif
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

void bezierCubic(int x0, int y0, int x1, int y1,
                 int x2, int y2, int x3, int y3, int steps) {
    int previousX = x0;
    int previousY = y0;

    for (int i = 1; i <= steps; i++) {
        double t = (double)i / (double)steps;
        double inverseT = 1.0 - t;
        double x = inverseT * inverseT * inverseT * x0
                 + 3.0 * inverseT * inverseT * t * x1
                 + 3.0 * inverseT * t * t * x2
                 + t * t * t * x3;
        double y = inverseT * inverseT * inverseT * y0
                 + 3.0 * inverseT * inverseT * t * y1
                 + 3.0 * inverseT * t * t * y2
                 + t * t * t * y3;
        int currentX = (int)lround(x);
        int currentY = (int)lround(y);

        bresenham(previousX, previousY, currentX, currentY);
        previousX = currentX;
        previousY = currentY;
    }
}
