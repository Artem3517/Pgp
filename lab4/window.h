#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>
#include <stdlib.h>

#define W 640
#define H 480

typedef struct Units {
    Display *dpy;
    Window win;
    GC gc;
    unsigned long mask;
    int count;
    XArc *circles;
    int mid;
} Units;

#include "drawing.h"

void setup(Units *units);
void dispatch(Units *units);
void sortCircles(Units *units, int first, int last);
void delete_circles(Units *units);

#endif  // WINDOW_H