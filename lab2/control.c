#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <stdio.h>

#include "graph.h"

int rekey(XEvent *ev) {
    Display *dpy = ev->xkey.display;
    Window win = ev->xkey.window;
    KeySym ks = XkbKeycodeToKeysym(dpy, ev->xkey.keycode, 0, ev->xkey.state & ShiftMask ? 1 : 0);
    if (ks == XK_F10) {
        return 10;
    }
    int FillFace = (ev->xkey.state & Mod1Mask) ? NFACE : 0;
    if (ks == XK_Escape) {
        reset(dpy, win, FillFace);
    }
    return 0;
}

int dispatch(Display *dpy, Window win, GC gc) {
    int NoFillFace = 0;
    XEvent event;
    int done = 0;
    while (!done) {
        XNextEvent(dpy, &event);
        switch (event.type) {
            case Expose:
                if (event.xexpose.count > 0) {
                    break;
                }
                regraph(dpy, win, gc, NoFillFace);
                break;
            case ConfigureNotify:
                NoFillFace = reconf(event.xconfigure.width, event.xconfigure.height);
                break;
            case ButtonPress:
                reface(dpy, win, gc, zotone(event.xbutton.x, event.xbutton.y));
                break;
            case FocusIn:
                NoFillFace = 0;
                regraph(dpy, win, gc, NoFillFace);
                reset(dpy, win, NFACE);
                break;
            case KeyPress:
                done = rekey(&event);
                break;
            default:
                break;
        }
    }
    return done;
}