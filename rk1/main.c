#include "calculate.h"
#include "control.h"
#include "graphics.h"

int main(int argc, char** argv) {
    int size = check_args(argc, argv);
    if (size == 0) {
        return 1;
    }
    Units* units;
    if (allocate_memory(&units, size)) {
        delete_memory(units, size);
        return 1;
    }
    create_windows(units, size);
    dispatch(units);
    XDestroySubwindows(units->dpy, units->root);
    XDestroyWindow(units->dpy, units->root);
    XCloseDisplay(units->dpy);
    delete_memory(units, size);
    return 0;
}