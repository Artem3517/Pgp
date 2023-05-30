#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>
#include <math.h>
#include "rot.h"

int main(int argc, char **argv) {
    Display *dpy;
    GC gc[3];
    int scr;

    XLine line;
    line.sw = ( line.cen.x =  WIN_WIDTH/2 );
    line.sh = ( line.cen.y = WIN_HEIGHT/2 );
     
    dpy = XOpenDisplay(NULL);
    scr = DefaultScreen(dpy);
    Window root = DefaultRootWindow(dpy);
    gc[0] = XCreateGC(dpy, root, 0, 0); 
    gc[1] = XCreateGC(dpy, root, 0, 0);
    gc[2] = XCreateGC(dpy, root, 0, 0);
    XSetForeground(dpy, gc[1], WhitePixel(dpy, scr));
    XSetForeground(dpy, gc[2], WhitePixel(dpy, scr));
    XGCValues gval1;
    unsigned long gmask = GCLineStyle;
    gval1.line_style = LineOnOffDash;
    XChangeGC(dpy, gc[2], gmask, &gval1);    
    XGCValues gval;            
    unsigned long amask = (CWOverrideRedirect | CWBackPixel);              
    XGetGCValues(dpy, gc[1], GCBackground, &gval);
    XSetWindowAttributes attr; 
    attr.background_pixel = gval.background;
    attr.override_redirect = False;
    Window win1 = XCreateWindow(dpy, root, 0, 0, WIN_WIDTH, WIN_HEIGHT, 1, CopyFromParent, InputOutput, CopyFromParent, amask, &attr);
    Window win = XCreateWindow(dpy, win1, 0, 0, WIN_WIDTH, WIN_HEIGHT, 1, CopyFromParent, InputOutput, CopyFromParent, amask, &attr);
    // XUnmapWindow(dpy, root);
    XUnmapWindow(dpy, win1);
    // XFlush(dpy);
    // XMapWindow(dpy, win1);
    XSizeHints hint;
    hint.flags = PMinSize | PMaxSize;
    hint.min_width = ( hint.max_width = WIN_WIDTH ); 
    hint.min_height = ( hint.max_height = WIN_HEIGHT );
    XSetNormalHints(dpy, win, &hint);
    XMapWindow(dpy, win);
    unsigned long emask = (ExposureMask | KeyPressMask | FocusChangeMask | VisibilityChangeMask);        
    XSelectInput(dpy, win, emask);
    
    unsigned delay=(1<<13);
    int count=0;  

    int process = 1, anim = 1, focus = 0, clockwise = -1;
    double angle = 0;
    XEvent event;
    while (process) { 
        event.type = 0;
        XCheckWindowEvent(dpy, win, emask,  &event);
        switch(event.type) {
            case Expose:
                XDrawArc(dpy, win, gc[2], 0, 0, WIN_WIDTH-1, WIN_HEIGHT-1, 0, 360*64);
                redraw(dpy, win, gc, &line, 1);
                break;
            case VisibilityNotify:
                if (event.xvisibility.state != VisibilityUnobscured) {
                    anim = 0;
                } else {
                    clockwise = (clockwise == 1) ? -1 : 1;
                    angle = 360 - angle;
                    anim = 1;
                }
                break;
            case FocusIn:
                focus = 1;
                break;
            case FocusOut:
                focus = 0;
                break;
            case KeyPress:
                if (XkbKeycodeToKeysym(dpy, event.xkey.keycode, 0, 0) == XK_e && 
                    event.xkey.state & ControlMask && focus) 
                    process = 0;
                break;
            default:
                break;
        } 

        if(count++ < delay)                
            continue;
        count = 0; 
        if (anim) {          
            redraw(dpy, win, gc, &line, 0);
            rotate(&line, angle, clockwise);
            redraw(dpy, win, gc, &line, 1);
            angle += 0.5 ; if (angle > 360) angle -= 360;
        }
        
    }
    XFreeGC(dpy, gc[0]);
    XFreeGC(dpy, gc[1]);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    return 0;
}