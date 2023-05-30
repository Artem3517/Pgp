#ifndef _ROT_H
#define _ROT_H

#define PI acos(-1)

typedef struct {
    XPoint ver;
    XPoint cen;
    int sw;
    int sh;
} XLine;

#define WIN_HEIGHT 600
#define WIN_WIDTH  800

void redraw(Display*, Window, GC*, XLine*, int);
void rotate(XLine*, double, int);

#endif