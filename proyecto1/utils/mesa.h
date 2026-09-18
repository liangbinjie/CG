#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

typedef struct {
  double r;
  double g;
  double b;
} COLOR;

typedef struct {
  int x0, y0;
  int x1, y1;
} LINE_POINT;

typedef struct {
  double x;
  double y;
} COORDINATES;