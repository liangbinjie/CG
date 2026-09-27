#include "interface.h"

void pan_right(double delta) {
    Xmin -= delta;
    Xmax -= delta;
}

void pan_left(double delta) {
    Xmin += delta;
    Xmax += delta;
}

void pan_up(double delta) {
    Ymin -= delta;
    Ymax -= delta;
}
void pan_down(double delta) {
    Ymin += delta;
    Ymax += delta;
}
