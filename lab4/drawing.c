#include "drawing.h"
#include <stdio.h>

int setFirstPoint(Units *units, XEvent *event, int *coords) {
    int track = 0;
    if (event->xbutton.button == Button1) {
        coords[0] = event->xbutton.x;
        coords[1] = event->xbutton.y;
        XGrabPointer(units->dpy, units->win, False, units->mask, GrabModeAsync, GrabModeSync, units->win,
                     None, CurrentTime);
        track = 1;
    }
    return track;
}

void setCircleParam(Units *units, XArc *tmp) {
    units->circles = (XArc *)((units->count == 0) ? calloc(1, sizeof(XArc)) : realloc(units->circles, (units->count + 1) * sizeof(XArc)));
    units->circles[units->count] = *tmp;
    units->count++;
}

int checkBoarders(Units *units, XEvent *event) {
    int del_circle = -10;
    int i = units->count - 1;
    while (i >= 0) {
        int x0 = units->circles[i].x + units->circles[i].width / 2;
        int y0 = units->circles[i].y + units->circles[i].height / 2;
        if (sqrt(pow(event->xbutton.x - x0, 2) + pow(event->xbutton.y - y0, 2)) < units->circles[i].width / 2) {
            del_circle = i;
            break;
        }
        i--;
    }
    return del_circle;
}

void setFrame(XArc *tmp, int xm, int ym, int* coords) {
    int dx = abs(xm - coords[0]);
    int dy = abs(ym - coords[1]);
    int dr = (int)sqrt(pow(dx, 2) + pow(dy, 2));
    int rx = coords[0] - dr;
    int ry = coords[1] - dr;
    if (rx > 0 && ry > 0 && rx + 2 * dr < 640 && ry + 2 * dr < 480) {
        tmp->width = tmp->height = 2 * dr;
        tmp->x = rx;
        tmp->y = ry;
    }
    tmp->angle1 = 0;
    tmp->angle2 = 360 * 64;
}

void drawFrame(Units *units, XArc *tmp) {
    XSetFunction(units->dpy, units->gc, GXcopy);
    XClearWindow(units->dpy, units->win);
    redraw(units);
    XSetLineAttributes(units->dpy, units->gc, 1, LineOnOffDash, CapNotLast, JoinMiter);
    XSetForeground(units->dpy, units->gc, BLACK);
    XDrawArc(units->dpy, units->win, units->gc, tmp->x, tmp->y, tmp->width, tmp->height, tmp->angle1, tmp->angle2);
}

void redraw(Units *units) {
    XSetFunction(units->dpy, units->gc, GXcopy);
    XClearWindow(units->dpy, units->win);
    for (int i = 0; i < units->count; i++) {
        if (i == units->mid) {
            XSetForeground(units->dpy, units->gc, RED);
        } else {
            XSetForeground(units->dpy, units->gc, BLACK);
        }
        XSetLineAttributes(units->dpy, units->gc, 1, LineSolid, CapNotLast, JoinMiter);
        XDrawArc(units->dpy, units->win, units->gc, units->circles[i].x, units->circles[i].y,
                       units->circles[i].width, units->circles[i].height, units->circles[i].angle1, units->circles[i].angle2);
    }
}