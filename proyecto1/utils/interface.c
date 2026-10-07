#include "interface.h"
#include "pane.h"
#include "zoom.h"
#include "rotate.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "texture.h"

double Xmin = INITIAL_XMIN;
double Xmax = INITIAL_XMAX;
double Ymin = INITIAL_YMIN;
double Ymax = INITIAL_YMAX;
int fill_enabled = 0;
int texture_enabled = 0;
void reset() {
    angle = 0.0;
    Xmin = INITIAL_XMIN;
    Xmax = INITIAL_XMAX;
    Ymin = INITIAL_YMIN;
    Ymax = INITIAL_YMAX;
}

void keyboard(unsigned char key, int x, int y) {
    switch (key)
    {
        case 27:  // ESC
            exit(0);
            break;
        
        case 'r':  // Reiniciar
        case 'R':
            reset();
            glutPostRedisplay();
            break;
        
        case 43:  // Zoom +
            printf("+ Zoom\n");
            zoom(0.9);
            glutPostRedisplay();
            break;
        
        case 45:  // Zoom -
            printf("- Zoom\n");
            zoom(1.1);
            glutPostRedisplay();
            break;

        case 'c':
        case 'C':
            fill_enabled = !fill_enabled;
            texture_enabled = 0;
            glutPostRedisplay();
            break;
        
        case 't':
        case 'T':
            texture_enabled = !texture_enabled;
            fill_enabled = 0;
            glutPostRedisplay();
            break;

        case 'q':  // Rotate to left
        case 'Q':
            printf("Q pressed\n");
            rotate_counterclockwise(5.0);
            glutPostRedisplay();
            break;
        
        case 'e':  // Rotate to right
        case 'E':
            printf("E pressed\n");
            rotate_clockwise(5.0);
            glutPostRedisplay();
            break;
    }
}

void special_keyboard(int key, int x, int y) {
    switch (key)
    {
        case GLUT_KEY_UP:  // Pan Up
            printf("UP pressed\n");
            pan_up(0.05);
            glutPostRedisplay();
            break;

        case GLUT_KEY_DOWN:  // Pan Down
            printf("DOWN pressed\n");
            pan_down(0.05);
            glutPostRedisplay();
            break;

        case GLUT_KEY_LEFT:  // Pan Left
            printf("LEFT pressed\n");
            pan_left(0.05);
            glutPostRedisplay();
            break;

        case GLUT_KEY_RIGHT:  // Pan Right
            printf("RIGHT pressed\n");
            pan_right(0.05);
            glutPostRedisplay();
            break;
    }
}

void init_interface(int argc, char **argv) {
    glutInit(&argc, argv);

    load_all_province_textures();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(Hres, Vres);
    glutCreateWindow("Costa Rica Map");

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keyboard);
    
    gluOrtho2D(0, Hres, 0, Vres);
};
