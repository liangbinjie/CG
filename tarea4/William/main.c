#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "mesa.h"

void draw_scene();
void render();
void initialize_buffer();
void animate(int value);

COLOR **buffer;
int resolution = 600;

BEZIER_CURVE signature[] = {
    // Curve 0
    {
        {-250, 100},   // P0
        {-235, -100},  // P1
        {-220, -100},  // P2
        {-205, 100}    // P3
    },
    // Curve 1
    {
        {-205, 100},
        {-190, -100},
        {-175, -100},
        {-160, 100}
    },
    // Curve 2
    {
        {-180, -50},
        {0, 0},
        {100, 50},
        {250, 100}
    },
    // Curve 3
    {
        {-80, 0},
        {-100, 10},
        {-100, 20},
        {-80, 30}
    },
    // Curve 4
    {
        {-80, 30},
        {-30, 80},
        {5, 150},
        {-10, 180}
    },
    // Curve 5
    {
        {-10, 180},
        {-50, 80},
        {0, -10},
        {-55, -180}
    },
    // Curve 6
    {
        {-55, -180},
        {-80, -200},
        {-100, -180},
        {0, 10}
    },
    // Curve 7
    {
        {0, 10},
        {5, 20},
        {10, 40},
        {5, 60}
    },
    // Curve 8
    {
        {5, 60},
        {8, 40},
        {12, 20},
        {30, 18}
    },
    // Curve 9
    {
        {5, 70},
        {5, 70},
        {5, 70},
        {5, 70}
    },
    // Curve 10
    {
        {30, 18},
        {40, 35},
        {30, 50},
        {40, 65}
    },
    // Curve 11
    {
        {40, 65},
        {60, 52},
        {50, 39},
        {60, 30}
    },
    // Curve 12
    {
        {60, 30},
        {70, 49},
        {60, 65},
        {70, 80}
    },
    // Curve 13
    {
        {70, 80},
        {90, 65},
        {80, 54},
        {90, 40}
    },
    // Curve 14
    {
        {90, 40},
        {100, 60},
        {90, 75},
        {100, 92}
    },
    // Curve 15
    {
        {100, 92},
        {120, 75},
        {110, 60},
        {120, 52}
    },
    // Curve 16
    {
        {165, 72},
        {165, 110},
        {170, 150},
        {160, 180}
    },
    // Curve 17
    {
        {160, 180},
        {157, 180},
        {155, 160},
        {152, 140}
    },
    // Curve 18
    {
        {165, 75},
        {170, 100},
        {180, 130},
        {220, 190}
    },
    // Curve 19
    {
        {220, 100},
        {220, 100},
        {220, 100},
        {220, 100}
    }
};

int curveCount = sizeof(signature) / sizeof(signature[0]);
int currentCurve = 0;
double currentT = 0.0;
double deltaT = 0.01;
POINT previousPoint;
int hasPreviousPoint = 0;

void initialize_buffer() {
    buffer = (COLOR **)malloc(resolution * sizeof(COLOR *));
    for (int i = 0; i < resolution; i++) {
        buffer[i] = (COLOR *)malloc(resolution * sizeof(COLOR));
        for (int j = 0; j < resolution; j++) {
            buffer[i][j].r = 0.0;
            buffer[i][j].g = 0.0;
            buffer[i][j].b = 0.0;
        }
    }
}

// Function that make the signatures'animation

void animate(int value) {
    if (currentCurve >= curveCount) {
        return;
    }
    
    BEZIER_CURVE curve = signature[currentCurve];
    
    if (!hasPreviousPoint) {
        previousPoint = curve.p0;
        plot(previousPoint.x, previousPoint.y);
        hasPreviousPoint = 1;
    }
    
    currentT += deltaT;
    
    if (currentT > 1.0) {
        currentT = 1.0;
    }

    POINT currentPoint = bezierPoint(
        curve.p0,
        curve.p1,
        curve.p2,
        curve.p3,
        currentT
    );
    
    bresenham(previousPoint.x, previousPoint.y, currentPoint.x, currentPoint.y);
    previousPoint = currentPoint;
    
    // Verifies if the actual curve is finished

    if (currentT >= 1.0) {
        currentCurve++;
        currentT = 0.0;
        hasPreviousPoint = 0;
    }
    
    glutPostRedisplay();


   // Verifies if there are curves left
    if (currentCurve < curveCount) {
        glutTimerFunc(16, animate, 0);
    }
}

void draw_scene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            glColor3f(
                buffer[i][j].r,
                buffer[i][j].g,
                buffer[i][j].b
            );
            glVertex2i(i - resolution / 2, j - resolution / 2);
        }
    }

    glEnd();
    glFlush();
}

int main(int argc, char *argv[]) {

    initialize_buffer();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(resolution, resolution);
    glutCreateWindow("Firma con Curvas de Bezier");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(-resolution/2,resolution/2,-resolution/2,resolution/2);
    
    setColor(1.0, 1.0, 1.0);
    glutDisplayFunc(draw_scene);
    glutTimerFunc(16, animate, 0);
    glutMainLoop();
    
    return 0;
}
