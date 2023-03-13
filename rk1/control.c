#include "control.h"

int check_args(int count, char **args) {
    int n = 0;
    if (count < 2) {
        fprintf(stderr, "Matrix size not set\n");
        n = 0;
    } else {
        int i = 0;
        while (args[1][i] != '\0') {
            if (args[1][i] < '0' || args[1][i] > '9') {
                i = -1;
                fprintf(stderr, "Matrix size is incorrect\n");
                break;
            } else {
                i++;
            }
        }
        if (i != -1) {
            n = atoi(args[1]);
        } else {
            n = 0;
        }
    }
    return n;
}

void dispatch(Units *units) {
    XEvent event;
    int flag = 1;
    while (flag) {
        XNextEvent(units->dpy, &event);
        switch (event.type) {
            case Expose:
                rebox(units, &event);
                break;
            case ButtonPress:
                reset(units, &event);
                break;
            case KeyPress:
                flag = kb_parse(units, &event);
                break;
            default:
                break;
        }
    }
}

int kb_parse(Units *units, XEvent *event) {
    KeySym sym;
    XLookupString((XKeyEvent *)event, NULL, 0, &sym, NULL);
    switch (sym) {
        case XK_Escape:
            restore(units);
            break;
        case XK_Alt_L:
        case XK_Alt_R:
            invert(units);
            break;
        case XK_q:
        case XK_Q:
            return 0;
            break;
        default:
            break;
    }
    return 1;
}

void restore(Units *units) {
    for (int i = 0; i < units->size; i++) {
        for (int j = 0; j < units->size; j++) change(units, i, j, 0);
    }
    detout(units);
}

void invert(Units *units) {
    for (int i = 0; i < units->size; i++)
        for (int j = 0; j < units->size; j++) change(units, i, j, units->matrix[i][j] == 0 ? 1 : 0);
    detout(units);
}