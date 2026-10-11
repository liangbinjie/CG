#include "interface.h"

void init_interface(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(Hres, Vres);
    glutCreateWindow("3D Image Viewer");
    
    gluOrtho2D(0, Hres, 0, Vres);
};