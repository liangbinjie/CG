#include "interface.h"

void zoom(double z) {
    double Xc = (Xmin + Xmax) / 2;
    double Yc = (Ymin + Ymax) / 2;
    
    Xmin = ((Xmin - Xc) * z) + Xc;
    Xmax = ((Xmax - Xc) * z) + Xc;
    Ymin = ((Ymin - Yc) * z) + Yc;
    Ymax = ((Ymax - Yc) * z) + Yc; 
}
