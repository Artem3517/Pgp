#ifndef DRAWING_H
#define DRAWING_H

#include "window.h"

#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define RED 0xFF0000

int setFirstPoint(Units *units, XEvent *event, int *coords);
void setCircleParam(Units *units, const XArc *tmp);
int checkBoarders(Units *units, XEvent *event);
void redraw(Units *units);
void setFrame(XArc *tmp, int xm, int ym, const int *coords);
void drawFrame(Units *units, XArc *tmp);

#endif  // DRAWING_H