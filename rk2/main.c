#include "window.h"
#include "drawing.h"

int main() {
    Units units;
    Arrow arrow;
    setup(&units, &arrow);
    dispatch(&units, &arrow);
    XDestroyWindow(units.dpy, units.win);
    // XFreeGC(dpy, gc[0]);
    // XFreeGC(dpy, gc[1]);
    XCloseDisplay(units.dpy);
    return 0;
}