#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <X11/Xutil.h>
#include "arrow.h"
#include <stdio.h>
#include <math.h>

#define MAX(X,Y) ((X) > (Y))?(X):(Y)
#define MIN(X,Y) ((X) > (Y))?(Y):(X)

/* задание точек стрелки */

int configure_arrow(XArrow* pr, int w, int h)
{
	pr->R = MIN(w, h);
    	pr->A = M_PI / 2;

    	//pr->L = pr->R / 5;

    	pr->c.x = w / 2;
    	pr->c.y = h / 2;

    	pr->dA = M_PI / 360;

    return 0;
}

/* изменение координат стрелки, при изменении размеров окна*/

/*отрисовка стрелы в окне*/
int draw_arrow(Display *dpy, Window win, GC gc ,XArrow *pr)
{
	XClearWindow(dpy, win);

	float x1, y1, x2, y2;         //координаты стрелки
	double x_h;        //для промежуточного вычисления x
	double A;
	float a=800/2, b=480/2;
	A = pr->A;
	x_h = 1 / sqrt(1 / pow(a, 2) + pow(tan(A) / b, 2));
	if ((A >= M_PI / 2) && (A <= M_PI * 3 / 2) || (A <= -M_PI / 2) && (A >= -M_PI * 3 / 2)) x_h = -x_h;
	x1 = x_h;
	y1 = x_h * tan(A);
	if (A > 2*M_PI || A < -2*M_PI) pr->A = 0;
    XDrawLine(dpy, win, gc, pr->c.x, pr->c.y, pr->c.x+x1, pr->c.y-y1); //отрисовка основания с началам в центре экрана с концом, координаты которого прибавляется к координатам центра экрана



    XFillArc(dpy, win, gc, pr->c.x - C_DOT_RADIUS / 2, pr->c.y - C_DOT_RADIUS / 2, C_DOT_RADIUS, C_DOT_RADIUS, 0, 460*64);
 	XFlush(dpy);
    return 0;
}

/* перерисовка потерянных фрагментов окна */
int redraw(XEvent* ev, GC gc, XArrow* pr)
{
	static XRectangle clip[32];     /* clip rectangles buffer */
	static int n = 0;               /* clip rectangles number */
    	clip[n].x = ev->xexpose.x;
    	clip[n].y = ev->xexpose.y;
    	clip[n].width =  ev->xexpose.width;
    	clip[n].height =  ev->xexpose.height;
    	n++;
    	if((ev->xexpose.count > 0) && (n < 32))
        	return(0);
    	XSetClipRectangles(ev->xexpose.display, gc, 0, 0, clip, n, Unsorted);

    	draw_arrow(ev->xexpose.display, ev->xexpose.window, gc, pr);
    	XSetClipMask(ev->xexpose.display, gc, None);
    return(n=0);
} /* redraw */


int overlap(XEvent* ev) {
    if(ev->xvisibility.state == VisibilityUnobscured)
        return(-1);
	if(ev->xvisibility.state == VisibilityPartiallyObscured)
	    return(1);
} /* overlap */

/* изменение координат углов поворота, проверка на выход их из диапазона возможных значений */
int amod2pi(XArrow* pr)
{
        pr->A += (pr->dA);
        pr->q += (pr->dA);
        pr->w += (pr->dA);
	return 0;
} /* amod2pi */


/* ответ на собития нажатия клавиш */
int key_check(XEvent* ev, Display* dpy, Window win, XArrow* pr, GC gc)
{
	KeySym ks = XLookupKeysym((XKeyEvent*) ev, 1);
		if((ks == XK_E || ks == XK_e) && (ev->xkey.state & ControlMask))
			return 0;  /*код выхода*/
	return ev->xkey.keycode;
} /* rekey */
