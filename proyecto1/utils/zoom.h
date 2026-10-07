#ifndef ZOOM_H
#define ZOOM_H

#define ZOOM_IN_NORMAL  0.88
#define ZOOM_IN_SLOW    0.98
#define ZOOM_IN_FAST    0.75

#define ZOOM_OUT_NORMAL  1.1
#define ZOOM_OUT_SLOW    1.02
#define ZOOM_OUT_FAST    1.2

void zoom(double z);
double get_zoomIn_value();
double get_zoomOut_value();

#endif
