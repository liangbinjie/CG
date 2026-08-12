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
            buffer[i][j].r = 0;
            buffer[i][j].g = 0;
            buffer[i][j].b = 0;
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


void draw_scene() {
  static int last_x = 0;
  int i, j;
  COLOR color;
  
  //Variables para hacer el dibujo
  int ojo_y = 250;
  int ojo_izq_x = 150;
  int ojo_der_x = 250;
  int ojo_radio = 15;

  int sonrisa_inicio_x = 135;
  int sonrisa_fin_x = 265;
  int sonrisa_inicio_y = 150;
  int sonrisa_fin_y = 175;
  
  int cara_x = 200;
  int cara_y = 215;
  int cara_radio = 100;

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

  for (i = last_x; i < H_SIZE; i++) 
      {
       for (j = 0; j < V_SIZE; j++) 
         {
          buffer[i][j].r = (double)(i % (H_SIZE / 10)) / (double)(H_SIZE / 10);
          buffer[i][j].g = (double)(j % (V_SIZE / 10)) / (double)(V_SIZE / 10);
          buffer[i][j].b = (double)(i) / (double)(H_SIZE);
          
          // Condicionales para la creacion de la cara
          
          // -- Cara --
	  if ((i-cara_x)*(i-cara_x) +
	     (j-cara_y)*(j-cara_y) < cara_radio*cara_radio)
	  {
	   buffer[i][j].r = 1.0;
	   buffer[i][j].g = 1.0;
	   buffer[i][j].b = 0.0;
	  }
          
          // -- Ojo Izquierdo --
	  if ((i-ojo_izq_x)*(i-ojo_izq_x) +
	     (j-ojo_y)*(j-ojo_y) < ojo_radio*ojo_radio)
	  {
	   buffer[i][j].r = 0.0;
	   buffer[i][j].g = 0.0;
	   buffer[i][j].b = 0.0;
	  }

	  // -- Ojo Derecho --
	  if ((i-ojo_der_x)*(i-ojo_der_x) +
	     (j-ojo_y)*(j-ojo_y) < ojo_radio*ojo_radio)
	  {
	   buffer[i][j].r = 0.0;
	   buffer[i][j].g = 0.0;
	   buffer[i][j].b = 0.0;
	  }

	  // -- Sonrisa --
	  if (j > sonrisa_inicio_y && j < sonrisa_fin_y &&
	      i > sonrisa_inicio_x && i < sonrisa_fin_x)
	  {
	   buffer[i][j].r = 0.0;
	   buffer[i][j].g = 0.0;
	   buffer[i][j].b = 0.0;
	  }
          
          glColor3f (buffer[i][j].r,buffer[i][j].g,buffer[i][j].b);
          glBegin(GL_POINTS);
          glVertex2i(i,j);
          glEnd();
          last_x = i;
         }
      }

  glFlush();
}




