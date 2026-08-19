#include <stdio.h>
#include <stdlib.h>

#include "malloc.h"
#include "utils.h"
#include "mesa.h"

void draw_scene();

COLOR **buffer;

int resolution; // global resolution variable

int main(int argc, char *argv[])
{
    if (argc < 4) // if number of arguments < 4 -> print usage command
    {
        printf("Usage: %s <resolution> <# of lines> <# of times>\n", argv[0]);
    }
    else
    {
        // get arguments
        // should we check for valid numbers?
        int xlines, xtimes;
        resolution = atoi(argv[1]);
        xlines = atoi(argv[2]);
        xtimes = atoi(argv[3]);

        printf("Creating a %d x %d screen\nDrawing %d lines for %d times\n", resolution, resolution, xlines, xtimes);
        
        int i, j;

        buffer = (COLOR **)malloc(resolution * sizeof(COLOR*));
        for (i = 0; i < resolution; i++) 
            {
            buffer[i] = (COLOR *)malloc(resolution * sizeof(COLOR));
            }

        for (i = 0; i < resolution; i++) 
            {
            for (j = 0; j < resolution; j++) 
                {
                    buffer[i][j].r = 0;
                    buffer[i][j].g = 1;
                    buffer[i][j].b = 0;
                }
            }

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(resolution, resolution);
        glutCreateWindow("Mesa Example");
        glClear(GL_COLOR_BUFFER_BIT);
        gluOrtho2D(-0.5, resolution +0.5, -0.5, resolution + 0.5);
        glutDisplayFunc(draw_scene);
        glutMainLoop();
    }
}

void draw_scene() {
    int i, j;
    for (i = 0; i < resolution; i++) 
        {
        for (j = 0; j < resolution; j++) 
            {
                glColor3f(buffer[i][j].r, buffer[i][j].g, buffer[i][j].b);
                glBegin(GL_POINTS);
                glVertex2i(i, j);
                glEnd();
            }
        }
    glFlush();
}