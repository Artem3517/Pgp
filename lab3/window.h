#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>
#include <stdlib.h>

#define H1 80
#define W1 80

#define WHITE 0xFFFFFF
#define BLACK 0x000000

typedef struct Units {
    Display *dpy;
    Window win;
    GC gc;
    unsigned long mask;
    int count;
    XRectangle *rects;
} Units;

#include "drawing.h"

void setup(Units *units);
void dispatch(Units *units);
void delete_rects(Units *units);

#endif  // WINDOW_H