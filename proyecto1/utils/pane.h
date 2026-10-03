#ifndef PANE_H
#define PANE_H

#define PANE_NORMAL  0.05
#define PANE_SLOW    0.01
#define PANE_FAST    0.15

void pan_left(double delta);
void pan_right(double delta);
void pan_up(double delta);
void pan_down(double delta);
double get_pane_value();

#endif
