#include "drawing.h"

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

void setRectParam(Units *units, int *coords) {
    XRectangle tmp;
    tmp.x = (coords[0] > coords[2]) ? coords[2] : coords[0];
    tmp.width = abs(coords[0] - coords[2]);
    tmp.y = (coords[1] < coords[3]) ? coords[1] : coords[3];
    tmp.height = abs(coords[1] - coords[3]);
    units->rects =
        (XRectangle *)((units->count == 0) ? calloc(1, sizeof(XRectangle))
                                           : realloc(units->rects, (units->count + 1) * sizeof(XRectangle)));
    units->rects[units->count] = tmp;
    units->count++;
}

int checkBoarders(Units *units, XEvent *event) {
    int del_rect = -10;
    int i = units->count - 1;
    while (i >= 0) {
        if ((event->xbutton.x > units->rects[i].x) &&
            (event->xbutton.x < (units->rects[i].x + units->rects[i].width)) &&
            (event->xbutton.y > units->rects[i].y) &&
            (event->xbutton.y < (units->rects[i].y + units->rects[i].height))) {
            del_rect = i;
            break;
        }
        i--;
    }
    return del_rect;
}

void redraw(Units *units) {
    for (int i = 0; i < units->count; i++) {
        XDrawRectangle(units->dpy, units->win, units->gc, units->rects[i].x, units->rects[i].y,
                       units->rects[i].width, units->rects[i].height);
        XSetForeground(units->dpy, units->gc, WHITE);
        XFillRectangle(units->dpy, units->win, units->gc, units->rects[i].x + 1, units->rects[i].y + 1,
                       units->rects[i].width - 1, units->rects[i].height - 1);
        XSetForeground(units->dpy, units->gc, BLACK);
    }
}