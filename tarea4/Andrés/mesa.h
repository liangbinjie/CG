#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

typedef struct {
  float r;
  float g;
  float b;
} COLOR;

typedef struct {
  int x0, y0;
  int x1, y1;
} LINE_POINT;

extern int framebufferHeight;
