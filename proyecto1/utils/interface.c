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

int get_mode() {
    // Verifies if the shift or ctrl are being pressed
    int mods = glutGetModifiers();

    if (mods & GLUT_ACTIVE_SHIFT) {
        return 2; // Fast mode
    }

    if (mods & GLUT_ACTIVE_CTRL) {
        return 1; // Slow mode
    }

    return 0; // Normal mode
}

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
        
        case 'c':
        case 'C':
            fill_enabled = !fill_enabled;
            glutPostRedisplay();
            break;
        
        case 'r':  // Reiniciar
        case 'R':
            reset();
            glutPostRedisplay();
            break;
        
        case 'i':  // Zoom +
        case 'I':
        case 9: // case for ctrl + i
            printf("+ Zoom\n");
            zoom(get_zoomIn_value());
            glutPostRedisplay();
            break;
        
        case 'o':  // Zoom -
        case 'O':
        case 15: // case for ctrl + o
            printf("- Zoom\n");
            zoom(get_zoomOut_value());
            glutPostRedisplay();
            break;

        case 'c':
        case 'C':
            texture_enabled = 0;
            fill_enabled = !fill_enabled;
            glutPostRedisplay();
            break;
        
        case 't':
        case 'T':
            fill_enabled = 0;
            texture_enabled = !texture_enabled;
            glutPostRedisplay();
            break;

        case 'q':  // Rotate to left
        case 'Q':
        case 17: // case for ctrl + q
            printf("Q pressed\n");
            rotate_counterclockwise(get_rotate_value());
            glutPostRedisplay();
            break;
        
        case 'e':  // Rotate to right
        case 'E':
        case 5: // case for ctrl + e
            printf("E pressed\n");
            rotate_clockwise(get_rotate_value());
            glutPostRedisplay();
            break;
    }
}

void special_keyboard(int key, int x, int y) {
    switch (key)
    {
        case GLUT_KEY_UP:  // Pan Up
            printf("UP pressed\n");
            pan_up(get_pane_value());
            glutPostRedisplay();
            break;

        case GLUT_KEY_DOWN:  // Pan Down
            printf("DOWN pressed\n");
            pan_down(get_pane_value());
            glutPostRedisplay();
            break;

        case GLUT_KEY_LEFT:  // Pan Left
            printf("LEFT pressed\n");
            pan_left(get_pane_value());
            glutPostRedisplay();
            break;

        case GLUT_KEY_RIGHT:  // Pan Right
            printf("RIGHT pressed\n");
            pan_right(get_pane_value());
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
