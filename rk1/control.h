#ifndef CONTROL_H
#define CONTROL_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <stdio.h>
#include <stdlib.h>

#include "calculate.h"
#include "graphics.h"

int check_args(int count, char **args);
void dispatch(Units *units);
int kb_parse(Units *units, XEvent *event);
void restore(Units *units);
void invert(Units *units);

#endif  // CONTROL_H