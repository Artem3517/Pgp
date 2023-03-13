#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define W 128
#define H 128

int main() {
    Display *dpy = XOpenDisplay(NULL);
    int src = DefaultScreen(dpy);
    GC gc = DefaultGC(dpy, src);
    int depth = DefaultDepth(dpy, src);
    Window root = DefaultRootWindow(dpy);
    Pixmap pix = XCreatePixmap(dpy, root, W, H, depth);
    XSetWindowAttributes attr;
    XEvent event;
    XSetBackground(dpy, gc, WhitePixel(dpy, src));
    XSetForeground(dpy, gc, WhitePixel(dpy, src));
    XFillRectangle(dpy, pix, gc, 0, 0, W, H);
    XSetForeground(dpy, gc, BlackPixel(dpy, src));
    XDrawLine(dpy, pix, gc, 0, 32, 32, 0);
    XDrawLine(dpy, pix, gc, 0, 32, 32, 64);
    XDrawLine(dpy, pix, gc, 32, 0, 64, 32);
    XDrawLine(dpy, pix, gc, 32, 64, 32, 128);
    XDrawLine(dpy, pix, gc, 32, 64, 64, 32);
    XDrawLine(dpy, pix, gc, 64, 32, 128, 32);
    XFlush(dpy);
    unsigned long amask = (CWOverrideRedirect | CWBackPixel);
    attr.override_redirect = False;
    attr.background_pixmap = pix;
    Window win =
        XCreateWindow(dpy, root, 0, 0, 800, 600, 1, depth, InputOutput, CopyFromParent, amask, &attr);
    unsigned long emask = (ButtonPressMask | ButtonReleaseMask | KeyPressMask);
    XSelectInput(dpy, win, emask);
    XMapRaised(dpy, win);
    XStoreName(dpy, win, "floor");
    XSetFunction(dpy, gc, GXinvert);
    XSetWindowBackgroundPixmap(dpy, win, pix);
    unsigned int done = 0;
    while (!done) {
        XNextEvent(dpy, &event);
        switch (event.type) {
            case ButtonPress:
            case ButtonRelease:
                XCopyArea(dpy, pix, pix, gc, 0, 0, W, H, 0, 0);
                XSetWindowBackgroundPixmap(dpy, win, pix);
                XClearWindow(dpy, win);
                break;
            case KeyPress:
                done = event.xkey.keycode;
                break;
            default:
                break;
        }
    }
    XFreePixmap(dpy, pix);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    return 0;
}