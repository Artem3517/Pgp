#ifndef DRAWING_H
#define DRAWING_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>

#define PI acos(-1)

typedef struct {
    XPoint center;
    int r;
    double a;
    double b;
    double angle;
    double da;
} Arrow;

#include "window.h"

void config_arrow(Arrow *arrow, const int height, const int width);
void draw_arrow(Units *units, Arrow *arrow, int cond);
void redraw(XEvent *event, Units *units, Arrow *arrow);

#endif  // DRAWING_H