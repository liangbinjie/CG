#include "mesa.h"
#include "utils.h"
#include <stdlib.h>

#define WIDTH 600
#define HEIGHT 400
#define MAX_SIGNATURE_POINTS 50000
#define DRAW_DELAY_MS 1
#define SIGNATURE_SOURCE_WIDTH 800
#define SIGNATURE_SIDE_MARGIN 12

typedef struct {
    int x;
    int y;
} SIGNATURE_POINT;

COLOR **buffer;
int resolution = WIDTH;
int framebufferHeight = HEIGHT;
Plot plot = plotFramebuffer;
static SIGNATURE_POINT signature[MAX_SIGNATURE_POINTS];
static int signaturePoints = 0;
static int currentPoint = 0;

static void recordPoint(int x, int y) {
    if (signaturePoints < MAX_SIGNATURE_POINTS) {
        signature[signaturePoints].x = SIGNATURE_SIDE_MARGIN
            + (x * (WIDTH - 2 * SIGNATURE_SIDE_MARGIN)) / SIGNATURE_SOURCE_WIDTH;
        signature[signaturePoints].y = y;
        signaturePoints++;
    }
}

static void clearFramebuffer(void) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            buffer[y][x].r = 1.0;
            buffer[y][x].g = 1.0;
            buffer[y][x].b = 1.0;
        }
    }
}

static void drawSignature(void) {
    setColor(0.05, 0.05, 0.12);

    /* A */
    bezierCubic(18, 55, 29, 110, 40, 205, 52, 255, 22);
    bezierCubic(52, 255, 63, 205, 74, 115, 86, 55, 22);
    bezierCubic(30, 135, 45, 129, 61, 130, 73, 136, 10);

    /* n */
    bezierCubic(98, 75, 98, 98, 98, 120, 98, 140, 8);
    bezierCubic(98, 140, 106, 155, 120, 155, 126, 137, 12);
    bezierCubic(126, 137, 130, 120, 130, 96, 130, 75, 8);

    /* d */
    bezierCubic(174, 75, 174, 120, 174, 166, 174, 205, 12);
    bezierCubic(174, 75, 157, 67, 143, 78, 143, 108, 12);
    bezierCubic(143, 108, 143, 140, 164, 149, 174, 132, 12);
    bezierCubic(174, 132, 174, 112, 174, 92, 174, 75, 8);

    bezierCubic(187, 75, 187, 98, 187, 122, 187, 140, 8);
    bezierCubic(187, 140, 195, 153, 210, 151, 220, 136, 10);

    /* e */
    bezierCubic(224, 100, 224, 123, 233, 143, 250, 143, 14);
    bezierCubic(250, 143, 266, 143, 268, 121, 252, 115, 14);
    bezierCubic(252, 115, 240, 111, 227, 114, 224, 118, 10);
    bezierCubic(224, 118, 220, 99, 233, 89, 260, 95, 14);
    /* Tilde */
    bezierCubic(236, 155, 242, 161, 249, 167, 255, 171, 8);

    /* s */
    bezierCubic(305, 132, 296, 145, 278, 144, 276, 128, 12);
    bezierCubic(276, 128, 275, 112, 303, 119, 305, 101, 12);
    bezierCubic(305, 101, 307, 82, 282, 73, 275, 90, 12);

    /* C  */
    bezierCubic(397, 220, 384, 246, 348, 237, 348, 166, 18);
    bezierCubic(348, 166, 348, 91, 376, 61, 397, 82, 18);

    /* a */
    bezierCubic(440, 108, 440, 140, 414, 143, 414, 111, 12);
    bezierCubic(414, 111, 414, 82, 438, 78, 440, 108, 12);
    bezierCubic(440, 75, 440, 103, 440, 124, 440, 132, 8);

    /* n */

    bezierCubic(449, 75, 449, 108, 449, 130, 449, 140, 8);
    bezierCubic(449, 140, 457, 155, 471, 153, 478, 136, 12);
    bezierCubic(478, 136, 481, 120, 481, 97, 481, 75, 8);

    /* o */
    bezierCubic(508, 150, 489, 150, 489, 94, 508, 94, 14);
    bezierCubic(508, 94, 527, 94, 527, 150, 508, 150, 14);

    /* s s */
    bezierCubic(568, 132, 559, 145, 542, 144, 540, 128, 12);
    bezierCubic(540, 128, 539, 112, 566, 119, 568, 101, 12);
    bezierCubic(568, 101, 570, 82, 546, 73, 539, 90, 12);

    bezierCubic(605, 132, 596, 145, 579, 144, 577, 128, 12);
    bezierCubic(577, 128, 576, 112, 603, 119, 605, 101, 12);
    bezierCubic(605, 101, 607, 82, 583, 73, 576, 90, 12);

    /* a */
    bezierCubic(640, 108, 640, 140, 614, 143, 614, 111, 12);
    bezierCubic(614, 111, 614, 82, 638, 78, 640, 108, 12);
    bezierCubic(640, 75, 640, 103, 640, 124, 640, 132, 8);

    /* C . */
    bezierCubic(760, 215, 746, 240, 710, 230, 710, 160, 18);
    bezierCubic(710, 160, 710, 88, 738, 61, 760, 83, 18);
    bresenham(790, 75, 790, 75);
}

static void display(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0, 0);
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, buffer[0]);
    glutSwapBuffers();
}

static void animateSignature(int value) {
    (void)value;

    if (currentPoint < signaturePoints) {
        plotFramebuffer(signature[currentPoint].x, signature[currentPoint].y);
        currentPoint++;
        glutTimerFunc(DRAW_DELAY_MS, animateSignature, 0);
    }
}

static void initializeBuffer(void) {
    buffer = malloc(HEIGHT * sizeof(*buffer));
    COLOR *pixels = calloc(WIDTH * HEIGHT, sizeof(*pixels));
    for (int y = 0; y < HEIGHT; y++) {
        buffer[y] = pixels + y * WIDTH;
    }
}

static void freeBuffer(void) {
    free(buffer[0]);
    free(buffer);
}

int main(int argc, char **argv) {
    initializeBuffer();
    clearFramebuffer();
    plot = recordPoint;
    drawSignature();
    plot = plotFramebuffer;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Firma Andres Canossa");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glutDisplayFunc(display);
    glutTimerFunc(DRAW_DELAY_MS, animateSignature, 0);
    glutMainLoop();
    freeBuffer();
    return 0;
}
