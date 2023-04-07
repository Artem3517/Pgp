#ifndef DRAWING_H
#define DRAWING_H

#include "window.h"

int setFirstPoint(Units *units, XEvent *event, int *coords);
void setRectParam(Units *units, int *coords);
int checkBoarders(Units *units, XEvent *event);
void redraw(Units *units);

#endif  // DRAWING_H