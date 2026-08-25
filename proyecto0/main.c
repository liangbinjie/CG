#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "malloc.h"
#include "utils.h"
#include "mesa.h"

void draw_scene();

// GLOBAL VARIABLES: include color buffer, points buffer, resolution, x lines, x times
COLOR **buffer;
LINE_POINT *points_buffer;
int resolution, xlines, xtimes;

int main(int argc, char *argv[])
{
    if (argc < 4) { // if number of arguments < 4 -> print usage command
    
        printf("Usage: %s <resolution> <# of lines> <# of times>\n", argv[0]);
    }

    else {
        // get arguments
        // should we check for valid numbers?
        resolution = atoi(argv[1]);
        xlines = atoi(argv[2]);
        xtimes = atoi(argv[3]);
        
        // define color buffer of resolution x resolution
        buffer = (COLOR **)malloc(resolution * sizeof(COLOR*));
        for (int i = 0; i < resolution; i++) {
            buffer[i] = (COLOR *)malloc(resolution * sizeof(COLOR));
        }

        // create random points based on how many lines requiered
        // max random number is resolution-1

        // start by initializing the points buffer
        points_buffer = malloc(xlines * sizeof(LINE_POINT));

        // generate random seed
        srand(time(NULL));

        // fill the points buffer with random points
        for (int i = 0; i < xlines; i++) {  // for every line asked to create
            points_buffer[i].x0 = (rand() % resolution) - resolution/2;
            points_buffer[i].y0 = (rand() % resolution) - resolution/2;
            points_buffer[i].x1 = (rand() % resolution) - resolution/2;
            points_buffer[i].y1 = (rand() % resolution) - resolution/2;
            printf("(%d, %d) - (%d, %d)\n", points_buffer[i].x0, points_buffer[i].y0, points_buffer[i].x1, points_buffer[i].y1 );
        }

        printf("Creating a %d x %d screen\nDrawing %d lines for %d times\n", resolution, resolution, xlines, xtimes);

        // create the glut display
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(resolution, resolution);
        glutCreateWindow("Tarea_Programada_0");
        glClear(GL_COLOR_BUFFER_BIT);
        gluOrtho2D(-1*resolution/2, resolution/2, -1*resolution/2, resolution/2);
        glutDisplayFunc(draw_scene);
        glutMainLoop();
    }
}

void draw_axes() {
    int min = -resolution / 2;
    int max = resolution / 2;
    setColor(1.0, 1.0, 1.0);

    for (int x = min; x < max; x++) {
        plot(x, 0);
    }
    for (int y = min; y < max; y++) {
        plot(0, y);
    }
}

void draw_scene() {
     int i, j;
     
     draw_axes();
     
     for (int i = 0; i < xlines; i++) {
         setColor(1.0, 0, 0);
         brute_force (points_buffer[i].x0,points_buffer[i].y0,points_buffer[i].x1,points_buffer[i].y1);
         
         //setColor(0, 1.0, 0);
         //incremental (points_buffer[i].x0,points_buffer[i].y0,points_buffer[i].x1,points_buffer[i].y1);
         
         //setColor(0, 0, 1.0);
         //incremental_v2 (points_buffer[i].x0,points_buffer[i].y0,points_buffer[i].x1,points_buffer[i].y1);
         
         //setColor(1.0, 1.0, 0);
         //bresenham (points_buffer[i].x0,points_buffer[i].y0,points_buffer[i].x1,points_buffer[i].y1);
     }
     
     for (i = 0; i < resolution; i++) 
         {
          for (j = 0; j < resolution; j++) 
            {
             glColor3f (buffer[i][j].r,buffer[i][j].g,buffer[i][j].b);
             glBegin(GL_POINTS);
             glVertex2i(i - resolution/2, j - resolution/2);
             glEnd();
            }
         }
     glFlush();
}
