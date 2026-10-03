#include "interface.h"
#include "pane.h"
#include <stdio.h>

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

double get_pane_value() {
    int mode = get_mode();
    
    if(mode == 1){
        printf("CTRL\n");
        return PANE_SLOW;
    }
    if(mode == 2) {
        printf("SHIFT\n");
        return PANE_FAST;
    }
    return PANE_NORMAL;
}
