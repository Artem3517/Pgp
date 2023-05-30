/* 2 centred spiral main function */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include "arrow.h"

/* Main function */

int main()
{
	Display* dpy;              /* Graphic Display */
	Window win;                /* programm main window */
	GC gc;                  /* Black & White Graphic Context */
	int scr;                   /* screen number = 0 */
	XArrow r;                  /* spiral structure */

    /* Display Block */
    unsigned long tone;       /* Light drawing tone */
    dpy = XOpenDisplay(NULL);
    scr = DefaultScreen(dpy);
    win = DefaultRootWindow(dpy);
    scr = DefaultScreen(dpy);
    gc = XCreateGC(dpy, win, 0, 0);
    XSetForeground(dpy, gc, WhitePixel(dpy, scr));
     /* Display block */

    /* Window block  */
    unsigned w = 800, h = 480;             /* main window width & height */
    XSetWindowAttributes attr; /* window attributes structure */
    XGCValues gval;            /* GC structure */
    unsigned long amask;       /* window attributes mask */
    Window root=win;           /* Display root window */
    XSizeHints hint;           /* Geometry WM hints */
    configure_arrow(&r, w, h);
    amask = (CWOverrideRedirect | CWBackPixel);
    XGetGCValues(dpy, gc, GCBackground, &gval);
    attr.background_pixel = gval.background; /* = 0x0 */
    attr.override_redirect = False;
    win = XCreateWindow(dpy, root, 0, 0, w, h, 1, CopyFromParent,
                            InputOutput, CopyFromParent, amask, &attr);
	hint.flags = PMinSize | PMaxSize;
	hint.min_width = 800; hint.min_height = 480;
	hint.max_width = 800; hint.max_height = 480;
    XSetNormalHints(dpy, win, &hint);
	//XSetWMNormalHints(dpy, win, &hint);
    XStoreName(dpy, win, "rk2");
    XMapWindow(dpy, win);


    /* window block */

    /* Multi Block */

    unsigned long emask;       /* window event mask */
    XEvent event;              /* graphic event structure */
    int freeze = -1;              /* window visibility stop state */
    unsigned delay=DELAY;    /* значение отложения */
    int multi=(1<<12);         /* multi code */
    int count = 0;               /* счетчик */
             /*ширина и высота окна*/

    emask = (ExposureMask | KeyPressMask | VisibilityChangeMask | EnterWindowMask | LeaveWindowMask);
    XSelectInput(dpy, win, emask);

    while(multi != 0) {  /* асинхронная обработка событий сервера */
            event.type = 0;
            XCheckWindowEvent(dpy, win, emask,  &event);
                switch (event.type) {
                    case Expose:
                        redraw(&event, gc, &r);
                        break;
                    // case EnterNotify:
					//     freeze = 1;
					// 	break;
                    // case LeaveNotify:
					// 	r.dA = r.dA * -1;
					// 	freeze = 0;
	                //     break;
					case VisibilityNotify:
	                    freeze = overlap(&event);
						printf("%d\n", freeze);
	                    break;
                    case KeyPress:
                    	multi = key_check(&event, dpy, win, &r, gc);
                        break;
                    default:  break;
                }

            if((freeze > 0))     /* не двигать стрелку*/
                continue;
            if(count++ < delay)                 /* увеличение и проверка счетчика для отрисовки */
                continue;
            count = 0;                    /* сброс счетчика */
            draw_arrow(dpy, win, gc, &r);
			XSetLineAttributes(dpy, gc, 1, LineOnOffDash, CapNotLast, JoinMiter);
			// for(int i=0; i<=360*64; i+=640){
			// 	XDrawArc(dpy, win, gc, 0, 0, 799, 479, i, 640);
			// 	i+=640;
			// }
			XDrawArc(dpy, win, gc, 0, 0, 799, 479, 0, 360*64);
			XSetLineAttributes(dpy, gc, 1, LineSolid, CapNotLast, JoinMiter);
            amod2pi(&r);
        } /* while event */
    /* multi block */

    /* Exit block */
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
	return(0);
    /* exit block */

} /* main */
