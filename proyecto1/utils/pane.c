#include "interface.h"
#include "pane.h"
#include "rotate.h"
#include <stdio.h>
#include <math.h>

static void pan_screen(double deltaX, double deltaY) {
    double radians = angle * PI / 180.0;

    // Rotate the points using the inverse rotation. 
    double x = cos(radians) * deltaX + sin(radians) * deltaY;
    double y = -sin(radians) * deltaX + cos(radians) * deltaY;
    
    Xmin -= x;
    Xmax -= x;

    Ymin -= y;
    Ymax -= y;
}


void pan_right(double delta) {
    pan_screen(delta, 0.0);
}


void pan_left(double delta) {
    pan_screen(-delta, 0.0);
}


void pan_up(double delta) {
    pan_screen(0.0, delta);
}


void pan_down(double delta) {
    pan_screen(0.0, -delta);
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
