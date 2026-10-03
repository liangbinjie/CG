#ifndef INTERFACE_H
#define INTERFACE_H

#define Hres 750
#define Vres 750

#define INITIAL_XMIN -87.11766516876261
#define INITIAL_XMAX -82.56283687636407
#define INITIAL_YMIN 5.515082115173527
#define INITIAL_YMAX 11.209937007468433

extern double Xmin;
extern double Xmax;
extern double Ymin;
extern double Ymax;
extern int fill_enabled;

void init_interface(int argc, char **argv);
void keyboard(unsigned char key, int x, int y);
void special_keyboard(int key, int x, int y);
void reset();
int get_mode();

#endif
