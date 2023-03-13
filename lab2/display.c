#include "graph.h"

static XVertex *vertex;
static XEdge *edge;
static XFace *face;
static unsigned long palette[(NTONE + 1)];

int relink(XPolyGraph *pg) {
    vertex = pg->vertex;
    edge = pg->edge;
    face = pg->face;
    return 0;
}

GC congraph(Display *dpy) {
    int scr = DefaultScreen(dpy);
    GC gc = DefaultGC(dpy, scr);
    XGCValues gcval;
    gcval.line_width = EWIDTH;
    gcval.background = palette[DEFTONE];
    XChangeGC(dpy, gc, GCLineWidth | GCBackground, &gcval);
    return gc;
}

Window wingraph(Display *dpy, char *title) {
    Window root = DefaultRootWindow(dpy);
    int scr = DefaultScreen(dpy);
    int depth = DefaultDepth(dpy, scr);
    unsigned int w = DisplayWidth(dpy, scr) / 2;
    unsigned int h = DisplayHeight(dpy, scr) / 2;
    int x = w / 2;
    int y = h / 2;
    XSetWindowAttributes attr;
    unsigned long mask = CWOverrideRedirect | CWBackPixel | CWEventMask;
    attr.override_redirect = False;
    attr.background_pixel = palette[DEFTONE];
    attr.event_mask = (ButtonPressMask | KeyPressMask | ExposureMask | StructureNotifyMask | FocusChangeMask);
    Window win = XCreateWindow(dpy, root, x, y, w, h, 1, depth, InputOutput, CopyFromParent, mask, &attr);
    XSizeHints hint;
    hint.flags = (PMinSize | PPosition | PMaxSize);
    hint.min_width = hint.min_height = (8 * VDOT);
    hint.max_width = 2 * w;
    hint.max_height = 2 * h;
    hint.x = x;
    hint.y = y;
    XSetNormalHints(dpy, win, &hint);
    XStoreName(dpy, win, title);
    XMapWindow(dpy, win);
    return win;
}

int colorite(Display *dpy) {
    int scr = DefaultScreen(dpy);
    Colormap cmap = DefaultColormap(dpy, scr);
    static char *spector[] = {"#ffffff", "#ff0000", "#00ff00", "#0000ff", "#000000"};
    XColor rgb;
    for (int i = 0; i < (NTONE + 1); i++) {
        XParseColor(dpy, cmap, spector[i], &rgb);
        XAllocColor(dpy, cmap, &rgb);
        palette[i] = rgb.pixel;
    }
    return 0;
}

int regraph(Display *dpy, Window win, GC gc, int NoFillFace) {
    for (int i = NoFillFace; i < NFACE; i++) {
        XSetForeground(dpy, gc, palette[face[i].color]);
        XFillPolygon(dpy, win, gc, face[i].top, face[i].count, Convex, CoordModeOrigin);
    }
    XSetForeground(dpy, gc, palette[NTONE]);
    XDrawSegments(dpy, win, gc, edge, NEDGE);
    for (int i = 0; i < NVERT; i++) {
        XFillArc(dpy, win, gc, vertex[i].x - (VDOT >> 1), vertex[i].y - (VDOT >> 1), VDOT, VDOT, 0,
                 (64 * 360));
    }
    return 0;
}

int reset(Display *dpy, Window win, int FillFace) {
    int f = FillFace;
    for (; f < NFACE; f++) {
        face[f].color = face[NFACE].color;
    }
    XSetWindowBackground(dpy, win, palette[face[f].color]);
    XClearArea(dpy, win, 0, 0, 0, 0, True);
    return f;
}

int reface(Display *dpy, Window win, GC gc, int f) {
    if (f == NFACE) {
        return (reset(dpy, win, f));
    }
    XSetForeground(dpy, gc, palette[face[f].color]);
    XFillPolygon(dpy, win, gc, face[f].top, face[f].count, Convex, CoordModeOrigin);
    XFlush(dpy);
    XSetForeground(dpy, gc, palette[NTONE]);
    XDrawLines(dpy, win, gc, face[f].top, face[f].count + 1, CoordModeOrigin);
    for (int i = 0; i < face[f].count; i++) {
        XFillArc(dpy, win, gc, face[f].top[i].x - (VDOT / 2), face[f].top[i].y - (VDOT / 2), VDOT, VDOT, 0,
                 (64 * 360));
    }
    return 0;
}