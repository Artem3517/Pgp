#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Units Units;

struct Units {
    Display *dpy;
    GC gc[2];
    Window root;
    Window *rows;
    Window **boxes;
    XRectangle cell;
    int **matrix;
    char *mark[2];
    int size;
    int determ;
};

#include "calculate.h"
#include "control.h"

int allocate_memory(Units **units, int size);
void delete_memory(Units *units, int size);
int create_windows(Units *units, int size);
void change(Units *units, int i, int j, int m);
void rebox(Units *units, XEvent *event);
void reset(Units *units, XEvent *event);
void detout(Units *units);

#endif  // GRAPHICS_H