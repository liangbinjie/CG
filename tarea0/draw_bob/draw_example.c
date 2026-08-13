/*
 * Instituto Tecnologico de Costa Rica
 * Escuela de Ingenieria en Computacion
 * Computer Graphics
 *
 * Programa: Draw Example
 * Archivo:  draw_example.c
 */

#include "draw_example.h"
#include "malloc.h"

void draw_scene ();

COLOR **buffer;

int main(int argc, char** argv) 
{
  int i, j;

  buffer = (COLOR **)malloc(H_SIZE * sizeof(COLOR*));
  for (i = 0; i < H_SIZE; i++) 
      {
       buffer[i] = (COLOR *)malloc(V_SIZE * sizeof(COLOR));
      }

  for (i = 0; i < H_SIZE; i++) 
      {
       for (j = 0; j < V_SIZE; j++) 
           {
            buffer[i][j].r = 0.294;
            buffer[i][j].g = 0.537;
            buffer[i][j].b = 0.58;
           }
      }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(H_SIZE,V_SIZE);
  glutCreateWindow("Draw Example");
  glClear(GL_COLOR_BUFFER_BIT);
  gluOrtho2D(-0.5, H_SIZE +0.5, -0.5, V_SIZE + 0.5);
  glutDisplayFunc(draw_scene);
  glutMainLoop();
}



/*
 * Auxiliary Functions
 * Functions that helps to draw circles and rectangles
 * Also is needed a function to set the pixels in the figures
 */

void set_pixel(int x, int y, float r, float g, float b) {
  if (x >= 0 && x < H_SIZE && y >= 0 && y < V_SIZE) {
    buffer[x][y].r = r;
    buffer[x][y].g = g;
    buffer[x][y].b = b;
  }
}

void draw_circle(int cx, int cy, int radius, float r, float g, float b) {
  int x, y;

  for (x = cx - radius; x <= cx + radius; x++) {
    for (y = cy - radius; y <= cy + radius; y++) {
      if ((x - cx) * (x - cx) + (y - cy) * (y - cy) <= radius * radius) {
        set_pixel(x, y, r, g, b);
      }
    }
  }
}

void draw_rectangle(int x1, int y1, int x2, int y2, float r, float g, float b) {
  int x, y;

  for (x = x1; x <= x2; x++) {
    for (y = y1; y <= y2; y++) {
      set_pixel(x, y, r, g, b);
    }
  }
}

/*
 * Functions to draw the different parts of the character
 */

void draw_body() {
  draw_rectangle(100, 100, 300, 350, 1.0, 1.0, 0.0);
}

void draw_spots() {
  draw_circle(130, 300, 12, 0.75, 0.75, 0.0);
  draw_circle(270, 315, 10, 0.75, 0.75, 0.0);
  draw_circle(120, 180, 8, 0.75, 0.75, 0.0);
  draw_circle(280, 220, 13, 0.75, 0.75, 0.0);
  draw_circle(240, 155, 9, 0.75, 0.75, 0.0);
}

void draw_eye(int x, int y) {
  draw_circle(x, y, 30, 1.0, 1.0, 1.0);
  draw_circle(x, y, 14, 0.0, 0.5, 1.0);
  draw_circle(x, y, 7, 0.0, 0.0, 0.0);
}

void draw_eyes() {
  // Left eye
  draw_eye(160, 290);
  // Right eye
  draw_eye(240, 290);
}

void draw_mouth() {
  // Mouth
  draw_rectangle(150, 185, 250, 200, 0.0, 0.0, 0.0);
  draw_rectangle(190, 165, 230, 185, 0.0, 0.0, 0.0);
  // Mouth Left Corner
  draw_circle(195, 180, 15, 0.0, 0.0, 0.0);
  // Mouth Right Corner
  draw_circle(235, 180, 15, 0.0, 0.0, 0.0);
  draw_circle(230, 186, 15, 0.0, 0.0, 0.0);
  // Teeth
  draw_rectangle(185, 180, 205, 200, 1.0, 1.0, 1.0);
  draw_rectangle(215, 180, 235, 200, 1.0, 1.0, 1.0);
}

void draw_shirt() {
  draw_rectangle(100, 100, 300, 140, 1.0, 1.0, 1.0);
}

void draw_pants() {
  draw_rectangle(100, 70, 300, 105, 0.45, 0.20, 0.05);
  draw_rectangle(110, 85, 140, 95, 0.0, 0.0, 0.0);
  draw_rectangle(160, 85, 190, 95, 0.0, 0.0, 0.0);
  draw_rectangle(210, 85, 240, 95, 0.0, 0.0, 0.0);
  draw_rectangle(260, 85, 290, 95, 0.0, 0.0, 0.0);
}

void draw_tie() {
  draw_rectangle(190, 105, 210, 140, 0.8, 0.0, 0.0);
  draw_circle(200, 105, 10, 0.8, 0.0, 0.0);
}

void draw_arms() {
  // Left arm
  draw_rectangle(60, 190, 100, 205, 1.0, 1.0, 0.0);
  draw_rectangle(60, 160, 75, 205, 1.0, 1.0, 0.0);
  // Right arm
  draw_rectangle(300, 190,340, 205,1.0, 1.0, 0.0);
  draw_rectangle(325, 160, 340, 205, 1.0, 1.0, 0.0);
  
}

void draw_legs() {
  // Left leg
  draw_rectangle(140, 40, 155, 70, 1.0, 1.0, 0.0);
  // Right leg
  draw_rectangle(245, 40, 260, 70, 1.0, 1.0, 0.0);
}

void draw_socks() {
  // Left leg
  draw_rectangle(140, 40, 155, 60, 1.0, 1.0, 1.0);
  // Right leg
  draw_rectangle(245, 40, 260, 60, 1.0, 1.0, 1.0);
}

void draw_shoes() {
  // Left Shoe
  draw_rectangle(120, 35, 160, 50, 0.0, 0.0, 0.0);
  // Right Shoe
  draw_rectangle(240, 35, 280, 50, 0.0, 0.0, 0.0);
}

void draw_bubbles() {
  draw_circle(50, 350, 12, 0.094, 0.294, 0.329);
  draw_circle(80, 70, 10, 0.094, 0.294, 0.329);
  draw_circle(350, 75, 8, 0.094, 0.294, 0.329);
  draw_circle(350, 350, 13, 0.094, 0.294, 0.329);
  draw_circle(180, 25, 9, 0.094, 0.294, 0.329);
}

void draw_scene() {
  static int last_x = 0;
  int i, j;
  COLOR color;
  
  for (i = 0; i < last_x; i++) 
      {
       for (j = 0; j < V_SIZE; j++) 
           {
            glColor3f (buffer[i][j].r,buffer[i][j].g,buffer[i][j].b);
            glBegin (GL_POINTS);
            glVertex2i (i,j);
            glEnd();
           }
      }
  
  // Here the pixels needed for the draw are setted

  draw_body();
  draw_spots();

  draw_eyes();
  draw_mouth();

  draw_shirt();
  draw_pants();
  draw_tie();

  draw_arms();
  draw_legs();
  draw_socks();
  draw_shoes();
  
  draw_bubbles();
  
  for (i = last_x; i < H_SIZE; i++) 
      {
       for (j = 0; j < V_SIZE; j++) 
         {
          glColor3f (buffer[i][j].r,buffer[i][j].g,buffer[i][j].b);
          glBegin(GL_POINTS);
          glVertex2i(i,j);
          glEnd();
          last_x = i;
         }
      }

  glFlush();
}


