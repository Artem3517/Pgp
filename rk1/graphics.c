#include "graphics.h"

int allocate_memory(Units **units, int size) {
    int error = 0;
    Units *tmp_units;
    int **tmp_matrix;
    Window *tmp_rows;
    Window **tmp_boxes;
    tmp_units = (Units *)malloc(sizeof(Units));
    if (tmp_units == NULL) {
        error = 1;
    }
    if (!error) {
        tmp_matrix = (int **)malloc(size * sizeof(int *));
        if (tmp_matrix == NULL) {
            error = 1;
        }
    }
    if (!error) {
        for (int i = 0; i < size; i++) {
            tmp_matrix[i] = (int *)calloc(size, sizeof(int));
            if (tmp_matrix[i] == NULL) {
                error = 1;
                break;
            }
        }
    }
    if (!error) {
        tmp_rows = (Window *)calloc(size + 1, sizeof(Window));
        if (tmp_rows == NULL) {
            error = 1;
        }
    }
    if (!error) {
        tmp_boxes = (Window **)calloc(size, sizeof(Window *));
        if (tmp_boxes == NULL) {
            error = 1;
        }
    }
    if (!error) {
        for (int i = 0; i < size; i++) {
            tmp_boxes[i] = (Window *)calloc(size, sizeof(Window));
            if (tmp_boxes[i] == NULL) {
                error = 1;
                break;
            }
        }
    }
    if (error) {
        fprintf(stderr, "Memory error\n");
    } else {
        tmp_units->matrix = tmp_matrix;
        tmp_units->rows = tmp_rows;
        tmp_units->boxes = tmp_boxes;
        tmp_units->size = size;
        tmp_units->mark[0] = "0";
        tmp_units->mark[1] = "1";
        *units = tmp_units;
    }
    return error;
}

void delete_memory(Units *units, int size) {
    if (units != NULL) {
        if (units->rows != NULL) {
            free(units->rows);
        }
        if (units->boxes != NULL) {
            for (int i = 0; i < size; i++) {
                if (units->boxes[i] != NULL) {
                    free(units->boxes[i]);
                }
            }
            free(units->boxes);
        }
        if (units->matrix != NULL) {
            for (int i = 0; i < size; i++) {
                if (units->matrix[i] != NULL) {
                    free(units->matrix[i]);
                }
            }
            free(units->matrix);
        }
        free(units);
    }
}

int create_windows(Units *units, int size) {
    int XSP = 4;
    int YSP = 4;
    int BW = 64;
    int BH = 64;
    int x, y;
    unsigned w, h;
    int dx, dy;
    XSetWindowAttributes attr;
    XSizeHints hint;
    int i, j;
    int NY = size;
    int NX = size;

    units->dpy = XOpenDisplay(NULL);

    /* get X defaults */
    int src = DefaultScreen(units->dpy);
    units->gc[0] = DefaultGC(units->dpy, src);
    int depth = DefaultDepth(units->dpy, src);

    /* Font custom */
    char *fontname = "9x15";
    XFontStruct *fn = XLoadQueryFont(units->dpy, fontname);
    XSetFont(units->dpy, units->gc[0], fn->fid);

    /* box cell geometry */
    units->cell.width = fn->max_bounds.width;
    units->cell.height = fn->max_bounds.ascent + fn->max_bounds.descent;
    units->cell.x = (BW - fn->max_bounds.width) / 2;
    units->cell.y = BH / 2 + (fn->max_bounds.ascent - fn->max_bounds.descent) / 2;
    NY += 1;  // geometric increment of NY to draw detrement window

    /* Main root window */
    w = NX * BW + (NX + 1) * XSP + 2 * NX + 2;
    h = NY * BH + (NY + 1) * YSP + 4 * NY;
    attr.override_redirect = False;
    attr.background_pixel = WhitePixel(units->dpy, src);
    x = 0;
    y = 0;
    units->root = XCreateWindow(units->dpy, DefaultRootWindow(units->dpy), x, y, w, h, 1, depth, InputOutput,
                                CopyFromParent, (CWOverrideRedirect | CWBackPixel), &attr);
    hint.flags = (PMinSize | PMaxSize | PPosition);
    hint.min_width = hint.max_width = w;
    hint.min_height = hint.max_height = h;
    hint.x = x;
    hint.y = y;
    XSetNormalHints(units->dpy, units->root, &hint);

    /* box row  root's subwindow and detrement subwindow */
    attr.override_redirect = True;
    attr.background_pixel = WhitePixel(units->dpy, src);
    attr.event_mask = (ButtonPressMask | KeyPressMask | ExposureMask);
    w = NX * BW + (NX - 1) * XSP + 2 * NX + 2;
    h = BH + 2 + 2;
    x = XSP;
    dy = h + YSP;
    for (i = 0, y = YSP; i < NY; i++, y += dy)
        units->rows[i] =
            XCreateWindow(units->dpy, units->root, x, y, w, h, 0, depth, InputOutput, CopyFromParent,
                          (CWOverrideRedirect | CWBackPixel | CWEventMask), &attr);
    NY -= 1;

    /* Box row's subwindows */
    attr.override_redirect = True;
    attr.background_pixel = 0xFFFFFF;
    attr.event_mask = (KeyPressMask | ExposureMask);
    w = BW;
    h = BH;
    dx = w + XSP + 2;
    for (i = 0, y = 0; i < NY; i++)
        for (j = 0, x = 0; j < NX; j++, x += dx)
            units->boxes[i][j] =
                XCreateWindow(units->dpy, units->rows[i], x, y, w, h, 1, depth, InputOutput, CopyFromParent,
                              (CWOverrideRedirect | CWBackPixel | CWEventMask), &attr);

    /* Display windows */
    XMapWindow(units->dpy, units->root);
    XMapSubwindows(units->dpy, units->root);
    for (i = 0; i < NY + 1; i++) XMapSubwindows(units->dpy, units->rows[i]);
    XStoreName(units->dpy, units->root, "W09");

    /* Create clear GC */
    units->gc[1] = XCreateGC(units->dpy, units->root, 0, 0);
    XCopyGC(units->dpy, units->gc[0], GCFont, units->gc[1]);
    XSetForeground(units->dpy, units->gc[1], 0xFFFFFF);
    return 0;
}

void change(Units *units, int i, int j, int m) {
    XDrawString(units->dpy, units->boxes[i][j], units->gc[1], units->cell.x, units->cell.y,
                units->mark[units->matrix[i][j]], 1);
    units->matrix[i][j] = m;
    XDrawString(units->dpy, units->boxes[i][j], units->gc[0], units->cell.x, units->cell.y, units->mark[m],
                1);
    XFlush(units->dpy);
}

void rebox(Units *units, XEvent *event) {
    for (int i = 0; i < units->size; i++) {
        for (int j = 0; j < units->size; j++) {
            if (event->xexpose.window == units->boxes[i][j])
                XDrawString(units->dpy, units->boxes[i][j], units->gc[0], units->cell.x, units->cell.y,
                            units->mark[units->matrix[i][j]], 1);
        }
    }
    if (event->xexpose.window == units->rows[units->size]) detout(units);
}

void reset(Units *units, XEvent *event) {
    int i, j;
    for (i = 0; i < units->size; i++) {
        if (event->xbutton.window == units->rows[i]) break;
    }
    if (i == units->size) return;
    for (j = 0; j < units->size; j++) {
        if (event->xbutton.subwindow == units->boxes[i][j]) break;
    }
    if (j == units->size) return;
    int m = (units->matrix[i][j] == 1) ? 0 : 1;
    change(units, i, j, m);
    detout(units);
}

void detout(Units *units) {
    char str[3];
    sprintf(str, "%d", units->determ);
    XDrawString(units->dpy, units->rows[units->size], units->gc[1], units->cell.x, units->cell.y, str,
                1 + (abs(units->determ / 10) > 0) + (units->determ < 0));
    units->determ = det(units->matrix, units->size);
    sprintf(str, "%d", units->determ);
    XDrawString(units->dpy, units->rows[units->size], units->gc[0], units->cell.x, units->cell.y, str,
                1 + (abs(units->determ / 10) > 0) + (units->determ < 0));
    XFlush(units->dpy);
}