#include "window.h"

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
    XSizeHints hint;
    hint.flags = (PMinSize | PMaxSize | PPosition);
    hint.min_width = hint.max_width = 640;
    hint.min_height = hint.max_height = 480;
    XSetNormalHints(units->dpy, root, &hint);
    units->win = XCreateWindow(units->dpy, root, 50, 50, 640, 480, 1, depth, InputOutput, CopyFromParent,
                               units->mask, &attr);
    XMapWindow(units->dpy, units->win);
    units->mask = ButtonReleaseMask | ButtonMotionMask;
    units->count = 0;
}

void dispatch(Units *units) {
    int flag = 1;
    int coords[4];
    units->count = 0;
    XEvent event;
    int tracking = 0;
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
                        setRectParam(units, coords);
                        redraw(units);
                        tracking = 0;
                    } else if (event.xbutton.button == Button3) {
                        int del_rect = checkBoarders(units, &event);
                        if (del_rect != -10) {
                            for (int i = del_rect; i < units->count - 1; i++) {
                                units->rects[i] = units->rects[i + 1];
                            }
                            units->count--;
                        }
                        XSetFunction(units->dpy, units->gc, GXcopy);
                        XClearWindow(units->dpy, units->win);
                        redraw(units);
                    }
                }
                break;
            case MotionNotify:
                if (tracking) {
                    XSetFunction(units->dpy, units->gc, GXcopy);
                    XClearWindow(units->dpy, units->win);
                    redraw(units);
                    int xm = event.xmotion.x;
                    int ym = event.xmotion.y;
                    XRectangle tmp;
                    tmp.x = (coords[0] < xm) ? coords[0] : xm;
                    tmp.width = abs(coords[0] - xm);
                    tmp.y = (coords[1] < ym) ? coords[1] : ym;
                    tmp.height = abs(coords[1] - ym);
                    XSetLineAttributes(units->dpy, units->gc, 1, LineOnOffDash, CapNotLast, JoinMiter);
                    XSetForeground(units->dpy, units->gc, BLACK);
                    XDrawRectangle(units->dpy, units->win, units->gc, tmp.x, tmp.y, tmp.width, tmp.height);
                    XSetForeground(units->dpy, units->gc, BLACK);
                    XSetLineAttributes(units->dpy, units->gc, 1, LineSolid, CapNotLast, JoinMiter);
                }
                break;
            case KeyPress:
                if ((event.xkey.state & ControlMask) && (event.xkey.keycode == 27)) {
                    flag = 0;
                }
                break;
            default:
                break;
        }
    }
}

void delete_rects(Units *units) {
    if (units->count > 0) {
        free(units->rects);
    }
}