#ifndef INTERFACE_H
#define INTERFACE_H

#define Hres 1920
#define Vres 1080

#define Xmin -87.11766516876261
#define Xmax -82.56283687636407

#define Ymin 5.515082115173527
#define Ymax 11.209937007468433

void init_interface(int argc, char **argv);
void keyboard(unsigned char key, int x, int y);
void special_keyboard(int key, int x, int y);

#endif
