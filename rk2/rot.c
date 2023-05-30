#include <X11/Xlib.h>
#include <math.h>
#include "rot.h"

void redraw(Display *dpy, Window win, GC *gc, XLine *line, int cond) {
    XDrawLine(dpy, win, gc[cond], line->cen.x, line->cen.y, line->ver.x, line->ver.y);
    XFlush(dpy);
}

void rotate(XLine *line, double angle, int clockwise) {
    double r, a, b;
    a = pow(line->sw-2, 2);
    b = pow(line->sh-2, 2);
    r = sqrt( a*b / (b*pow(cos(angle*PI/180), 2) + a*pow(sin(angle*PI/180), 2)) );

    line->ver.x = line->cen.x + r*cos(angle*PI/180);
    line->ver.y = line->cen.y + clockwise*r*sin(angle*PI/180);
}