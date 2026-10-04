#include "interface.h"
#include "clipping.h"

int get_code(double x, double y) {
    int code = IN;

    if (x < Xmin)
        code |= LEFT;
    else if (x > Xmax)
        code |= RIGHT;
    if (y < Ymin)
        code |= BOTTOM;
    else if (y > Ymax)
        code |= TOP;

    return code;
}


int cohen_sutherland(COORDINATES *p0, COORDINATES *p1) {
    int code0 = get_code(p0->x, p0->y);
    int code1 = get_code(p1->x, p1->y);

    while (1)
    {
        // Both points inside
        if ((code0 | code1) == 0) {
            return 1;
        }
        // GBoth points outside
        if ((code0 & code1) != 0) {
            return 0;
        }

        // The line crosses a border
        int out;
        double x, y;

        if (code0 != 0)
            out = code0;
        else
            out = code1;
            
        if (out & TOP) { // Intersection with top
            x = p0->x + (p1->x - p0->x) *
                (Ymax - p0->y) / (p1->y - p0->y);
            y = Ymax;
        } else if (out & BOTTOM) { // Intersection with bottom
            x = p0->x + (p1->x - p0->x) *
                (Ymin - p0->y) / (p1->y - p0->y);
            y = Ymin;
        } else if (out & RIGHT) {  // Intersection with right
            y = p0->y + (p1->y - p0->y) *
                (Xmax - p0->x) / (p1->x - p0->x);
            x = Xmax;
        } else { // Intersection with right
            y = p0->y + (p1->y - p0->y) *
                (Xmin - p0->x) / (p1->x - p0->x);
            x = Xmin;
        }

        // The point outside is replaced
        if (out == code0) {
            p0->x = x;
            p0->y = y;
            code0 = get_code(p0->x, p0->y);
        } else {
            p1->x = x;
            p1->y = y;
            code1 = get_code(p1->x, p1->y);
        }
    }
}
