#include "window.h"
#include <stdio.h>

void setup(Units *units) {
    units->dpy = XOpenDisplay(NULL);
    int src = DefaultScreen(units->dpy);
    int depth = DefaultDepth(units->dpy, src);
    Window root = DefaultRootWindow(units->dpy);
    units->gc = DefaultGC(units->dpy, src);
    XSetForeground(units->dpy, units->gc, BLACK);
    units->mask = CWOverrideRedirect | CWEventMask | CWBackPixel;
    XSetWindowAttributes attr;
    attr.override_redirect = False;
    attr.event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask | ButtonMotionMask | KeyPressMask;
    attr.background_pixel = WHITE;
    units->win = XCreateWindow(units->dpy, root, 50, 50, 640, 480, 1, depth, InputOutput, CopyFromParent,
                               units->mask, &attr);
    XMapWindow(units->dpy, units->win);
    units->mask = ButtonReleaseMask | ButtonMotionMask;
    units->count = 0;
    units->mid = -1;
}

void dispatch(Units *units) {
    int flag = 1;
    int coords[4];
    XEvent event;
    int tracking = 0;
    XArc tmp;
    while (flag) {
        XNextEvent(units->dpy, &event);
        switch (event.type) {
            case Expose: {
                if ((event.xexpose.window == units->win) && (units->count > 0)) {
                    XSetFunction(units->dpy, units->gc, GXcopy);
                }
                XClearWindow(units->dpy, units->win);
                redraw(units);
            } break;
            case ButtonPress:
                tracking = setFirstPoint(units, &event, coords);
                break;
            case ButtonRelease:
                coords[2] = event.xbutton.x;
                coords[3] = event.xbutton.y;
                XUngrabPointer(units->dpy, CurrentTime);
                if (coords[0] != coords[2] && coords[1] != coords[3]) {
                    if (event.xbutton.button == Button1 && tracking) {
                        setCircleParam(units, &tmp);
                        tracking = 0;
                    } else if (event.xbutton.button == Button3) {
                        int del_circle = checkBoarders(units, &event);
                        if (del_circle != -10) {
                            for (int i = del_circle; i < units->count - 1; i++) {
                                units->circles[i] = units->circles[i + 1];
                            }
                            units->count--;
                        }
                    }
                    sortCircles(units, 0, units->count - 1);
                    units->mid = (units->count - 1) / 2;
                    redraw(units);
                }
                break;
            case MotionNotify:
                if (tracking) {
                    int xm = event.xmotion.x;
                    int ym = event.xmotion.y;
                    setFrame(&tmp, xm, ym, coords);
                    drawFrame(units, &tmp);
                }
                break;
            case KeyPress:
                if ((event.xkey.state & ControlMask) && (event.xkey.keycode == 32)) {
                    flag = 0;
                }
                break;
            default:
                break;
        }
    }
}

void sortCircles(Units *units, int first, int last) {
    int i, j, pivot;
    XArc tmp;
    if (first < last) {
        pivot = first;
        i = first;
        j = last;
        while (i < j) {
            while (units->circles[i].width <= units->circles[pivot].width && i < last) i++;
            while (units->circles[j].width > units->circles[pivot].width) j--;
            if (i < j) {
                tmp = units->circles[i];
                units->circles[i] = units->circles[j];
                units->circles[j] = tmp;
            }
        }
        tmp = units->circles[pivot];
        units->circles[pivot] = units->circles[j];
        units->circles[j] = tmp;
        sortCircles(units, first, j - 1);
        sortCircles(units, j + 1, last);
    }
}

void delete_circles(Units *units) {
    if (units->count > 0) {
        free(units->circles);
    }
}