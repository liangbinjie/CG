#include "interface.h"
#include "zoom.h"
#include <stdio.h>

void zoom(double z) {
    double Xc = (Xmin + Xmax) / 2;
    double Yc = (Ymin + Ymax) / 2;
    
    Xmin = ((Xmin - Xc) * z) + Xc;
    Xmax = ((Xmax - Xc) * z) + Xc;
    Ymin = ((Ymin - Yc) * z) + Yc;
    Ymax = ((Ymax - Yc) * z) + Yc; 
}

double get_zoomIn_value() {
    int mode = get_mode();
    
    if(mode == 1){
        printf("CTRL\n");
        return ZOOM_IN_SLOW;
    }
    if(mode == 2) {
        printf("SHIFT\n");
        return ZOOM_IN_FAST;
    }
    return ZOOM_IN_NORMAL;
}

double get_zoomOut_value() {
    int mode = get_mode();
    
    if(mode == 1){
        printf("CTRL\n");
        return ZOOM_OUT_SLOW;
    }
    if(mode == 2) {
        printf("SHIFT\n");
        return ZOOM_OUT_FAST;
    }
    return ZOOM_OUT_NORMAL;
}
