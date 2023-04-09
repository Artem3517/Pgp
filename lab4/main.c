#include "drawing.h"
#include "window.h"

int main() {
    Units units;
    setup(&units);
    dispatch(&units);
    XDestroyWindow(units.dpy, units.win);
    XCloseDisplay(units.dpy);
    delete_circles(&units);
    return 0;
}