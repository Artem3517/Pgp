#include "graph.h"

int main() {
    XPolyGraph graph;
    assoc(&graph);
    relink(&graph);
    Display *dpy = XOpenDisplay(NULL);
    colorite(dpy);
    GC gc = congraph(dpy);
    Window win = wingraph(dpy, "Barnetta's Graph");
    dispatch(dpy, win, gc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    return 0;
}