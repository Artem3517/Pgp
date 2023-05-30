#include "window.h"

#define C_DOT_RADIUS 20

void setup(Units *units, Arrow *arrow) {
    units->dpy = XOpenDisplay(NULL);
    int src = DefaultScreen(units->dpy);
    int depth = DefaultDepth(units->dpy, src);
    Window root = DefaultRootWindow(units->dpy);
    for (int i = 0; i < 3; i++) {
        units->gc[i] = XCreateGC(units->dpy, units->win, 0, 0);
    }
    XSetForeground(units->dpy, units->gc[1], BlackPixel(units->dpy, src));
    XSetForeground(units->dpy, units->gc[2], BlackPixel(units->dpy, src));
    XGCValues gval;
    unsigned long gmask = GCLineStyle;
    gval.line_style = LineOnOffDash;
    XChangeGC(units->dpy, units->gc[2], gmask, &gval);
    XSetWindowAttributes attr;
    attr.override_redirect = False;
    attr.background_pixel = WHITE;
    units->mask = CWOverrideRedirect | CWBackPixel;
    units->win = XCreateWindow(units->dpy, root, 100, 100, W, H, 1, depth, InputOutput, CopyFromParent,
                               units->mask, &attr);
    XSizeHints hint;
    hint.flags = (PMinSize | PMaxSize | PPosition);
    units->height = hint.min_height = hint.max_height = H;
    units->width = hint.min_width = hint.max_width = W;
    XSetNormalHints(units->dpy, units->win, &hint);
    XMapWindow(units->dpy, units->win);
    config_arrow(arrow, units->height, units->width);
    units->emask = ExposureMask | KeyPressMask | VisibilityChangeMask | EnterWindowMask | LeaveWindowMask;
}

void dispatch(Units *units, Arrow *arrow) {
    unsigned delay = 1<<13;
    unsigned count = 0;
    int drawing = 1;
    int animation = 1;
    XEvent event;
    while (drawing) {
        event.type = 0;
        XCheckWindowEvent(units->dpy, units->win, units->emask, &event);
        switch (event.type) {
            // case Expose:
                // XDrawArc(units->dpy, units->win, units->gc[2], 0, 0, units->width - 1, units->height - 1, 0, 360*64);
                // redraw(&event, units, arrow);
                break;
            // case VisibilityNotify:
            //     break;
            case KeyPress:
                if ((event.xkey.state & ControlMask) && (event.xkey.keycode == 26)) {
                    drawing = 0;
                }
                break;
            default:
                break;
        }
        if (count++ < delay) {
            continue;
        }
        count = 0;
        if (animation) {
            draw_arrow(units, arrow, 0);
            XFillArc(units->dpy, units->win, units->gc[1], arrow->center.x - C_DOT_RADIUS / 2, arrow->center.y - C_DOT_RADIUS / 2, C_DOT_RADIUS, C_DOT_RADIUS, 0, 360*64);
            XDrawArc(units->dpy, units->win, units->gc[2], 0, 0, units->width - 1, units->height - 1, 0, 360*64);
            arrow->angle += arrow->da;
            draw_arrow(units, arrow, 1);
        }
    }
}