#include "interface.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

void keyboard(unsigned char key, int x, int y) {
    switch (key)
    {
        case 27:  // ESC
            exit(0);
            break;

        case 'q':
        case 'Q':
            printf("Q pressed\n");
            break;

        case 'w':
        case 'W':
            printf("W pressed\n");
            break;

        case 'a':
        case 'A':
            printf("A pressed\n");
            break;

        case 's':
        case 'S':
            printf("S pressed\n");
            break;

        case 'd':
        case 'D':
            printf("D pressed\n");
            break;
    }
}

void special_keyboard(int key, int x, int y) {
    switch (key)
    {
        case GLUT_KEY_UP:
            printf("UP pressed\n");
            break;

        case GLUT_KEY_DOWN:
            printf("DOWN pressed\n");
            break;

        case GLUT_KEY_LEFT:
            printf("LEFT pressed\n");
            break;

        case GLUT_KEY_RIGHT:
            printf("RIGHT pressed\n");
            break;
    }
}

void init_interface(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Costa Rica Map");

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keyboard);
};
