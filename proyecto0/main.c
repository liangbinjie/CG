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
Plot plot;
int resolution, xlines, xtimes;

int is_valid_Int(char *str) {
    int i = 0;

    if (str[i] == '\0') {
        return 0;
    }

    while (str[i] != '\0') {
        if ((str[i] < '0' || str[i] > '9')) {
            return 0;
        }
        i++;
    }

    return 1;
}

int validate_input(int argc, char *argv[]) {
    if (argc < 4) { // if number of arguments < 4 -> print usage command
        printf("Usage: %s <resolution> <# of lines> <# of times>\n", argv[0]);
        return 0;
    } 

    if (is_valid_Int(argv[1]) != 1 || is_valid_Int(argv[2]) != 1 || is_valid_Int(argv[3]) != 1 ) {
        printf("Usage: %s <resolution> <# of lines> <# of times>\n", argv[0]);
        return 0;
    }

    resolution = atoi(argv[1]);
    xlines = atoi(argv[2]);
    xtimes = atoi(argv[3]);
    return 1;
}

void render() {
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            glColor3f (buffer[i][j].r,buffer[i][j].g,buffer[i][j].b);
            glBegin(GL_POINTS);
            glVertex2i(i - resolution/2, j - resolution/2);
            glEnd();
        }
    }
    glFlush();
}

int main(int argc, char *argv[])
{
    if (validate_input(argc, argv) == 0) {
        return 0;
    } 

    else {
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
            // printf("(%d, %d) - (%d, %d)\n", points_buffer[i].x0, points_buffer[i].y0, points_buffer[i].x1, points_buffer[i].y1 );
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

double calculate_time(Algorithm algorithm, int xlines, int xtimes){
    clock_t start, end;
    
    start = clock();
     for (int j = 0; j < xtimes; j++){
         for (int i = 0; i < xlines; i++) {
             algorithm (points_buffer[i].x0,points_buffer[i].y0,points_buffer[i].x1,points_buffer[i].y1);
         }
     }
     end = clock();
     
     return (double)(end - start) / CLOCKS_PER_SEC;
}

void draw_scene() {
    static int run = 0;

    if (run) {
        render(); // if it is running, just render the actual lines
        return;
    }

    run = 1;

    double time;
    
    plot = plotMesa;
    draw_axes();
    
    time = 0.0;
    setColor(1.0, 0, 0);
    time = calculate_time(brute_force, xlines, xtimes);
    printf("Brute Force: %f seconds\n", time);
    
    render();

    time = 0.0;
    setColor(0, 1.0, 0);
    time = calculate_time(incremental, xlines, xtimes);
    printf("Incremental: %f seconds\n", time);
    
    render();

    time = 0.0;
    setColor(0, 0, 1.0);
    time = calculate_time(incremental_v2, xlines, xtimes);
    printf("Incremental V2: %f seconds\n", time);
    
    render();

    time = 0.0;
    setColor(1.0, 1.0, 0);
    time = calculate_time(bresenham, xlines, xtimes);
    printf("Bresenham: %f seconds\n", time);

    render();
    
    time = 0.0;
    setColor(1.0, 0, 1.0);
    time = calculate_time(bressenham_line, xlines, xtimes);
    printf("Bresenham Assembly: %f seconds\n", time);

    render();
    
    
}
