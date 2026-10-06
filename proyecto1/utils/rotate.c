#include "rotate.h"
#include "interface.h"
#include <stdio.h>
#include <math.h>

double angle = 0.0;

COORDINATES rotate_point(COORDINATES point) {
    COORDINATES rotated;
    
    double radians = angle * PI / 180.0;
    double Xc = (Xmin + Xmax) / 2;
    double Yc = (Ymin + Ymax) / 2;

    // Translate point so rotation center is the middle
    double x = point.x - Xc;
    double y = point.y - Yc;

    // Rotate
    rotated.x = cos(radians)*x - sin(radians)*y;
    rotated.y = sin(radians)*x + cos(radians)*y;

    // Translate back
    rotated.x += Xc;
    rotated.y += Yc;

    return rotated;
}

void rotate_clockwise(double a) {
    angle -= a;
}

void rotate_counterclockwise(double a) {
    angle += a;
}

double get_rotate_value() {
    int mode = get_mode();
    
    if(mode == 1){
        printf("CTRL\n");
        return ROTATE_SLOW;
    }
    if(mode == 2) {
        printf("SHIFT\n");
        return ROTATE_FAST;
    }
    return ROTATE_NORMAL;
}
