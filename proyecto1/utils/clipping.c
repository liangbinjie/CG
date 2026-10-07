#include "interface.h"
#include "clipping.h"
#include <stdlib.h>

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

static int inside_boundary(COORDINATES point, int boundary) {
    switch (boundary) {
        case LEFT:
            return point.x >= Xmin;
        case RIGHT:
            return point.x <= Xmax;
        case BOTTOM:
            return point.y >= Ymin;
        case TOP:
            return point.y <= Ymax;
    }

    return 0;
}

static COORDINATES boundary_intersection(COORDINATES start,
                                          COORDINATES end,
                                          int boundary) {
    COORDINATES intersection = start;
    double t;

    if (boundary == LEFT || boundary == RIGHT) {
        double boundary_x = boundary == LEFT ? Xmin : Xmax;
        t = (boundary_x - start.x) / (end.x - start.x);
        intersection.x = boundary_x;
        intersection.y = start.y + t * (end.y - start.y);
    } else {
        double boundary_y = boundary == BOTTOM ? Ymin : Ymax;
        t = (boundary_y - start.y) / (end.y - start.y);
        intersection.x = start.x + t * (end.x - start.x);
        intersection.y = boundary_y;
    }

    return intersection;
}

int clip_polygon(const COORDINATES *input, int input_count,
                COORDINATES *output, int output_capacity) {
    if (input == NULL || output == NULL || input_count < 3 ||
        output_capacity < 3) {
        return 0;
    }

    COORDINATES *working = malloc((size_t)output_capacity * sizeof(*working));
    COORDINATES *next = malloc((size_t)output_capacity * sizeof(*next));
    if (working == NULL || next == NULL) {
        free(working);
        free(next);
        return 0;
    }

    int working_count = input_count < output_capacity
        ? input_count
        : output_capacity;
    for (int i = 0; i < working_count; i++) {
        working[i] = input[i];
    }

    for (int boundary = LEFT; boundary <= TOP; boundary <<= 1) {
        int next_count = 0;

        if (working_count == 0) break;

        COORDINATES previous = working[working_count - 1];
        int previous_inside = inside_boundary(previous, boundary);

        for (int i = 0; i < working_count; i++) {
            COORDINATES current = working[i];
            int current_inside = inside_boundary(current, boundary);

            if (current_inside != previous_inside &&
                next_count < output_capacity) {
                next[next_count++] = boundary_intersection(
                    previous, current, boundary);
            }

            if (current_inside && next_count < output_capacity) {
                next[next_count++] = current;
            }

            previous = current;
            previous_inside = current_inside;
        }

        COORDINATES *temporary = working;
        working = next;
        next = temporary;
        working_count = next_count;
    }

    int result_count = working_count < output_capacity
        ? working_count
        : output_capacity;
    for (int i = 0; i < result_count; i++) {
        output[i] = working[i];
    }

    free(working);
    free(next);
    return result_count;
}