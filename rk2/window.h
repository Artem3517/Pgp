#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define WHITE 0xFFFFFF
#define BLACK 0x000000

#define W 800
#define H 600

typedef struct {
    Display *dpy;
    Window win;
    GC gc[3];
    unsigned mask;
    unsigned emask;
    int height;
    int width;
} Units;

#include "drawing.h"

void setup(Units *units, Arrow *arrow);
void dispatch(Units *units, Arrow *arrow);

#endif  // WINDOW_H