#include "drawing.h"

void config_arrow(Arrow *arrow, const int height, const int width) {
    arrow->r = (width > height) ? height : width;
    arrow->a = arrow->center.x = width / 2;
    arrow->b = arrow->center.y = height / 2;
    arrow->angle = PI / 2;
    arrow->da = PI / 360;
}

void draw_arrow(Units *units, Arrow *arrow, int cond) {  
	double x_tmp = sqrt(1 / pow(arrow->a, 2) + pow(tan(arrow->angle) / arrow->b, 2));
	if (((arrow->angle >= PI / 2) && (arrow->angle <= PI * 3 / 2)) || ((arrow->angle <= -PI / 2) && (arrow->angle >= -PI * 3 / 2))) {
        x_tmp = -x_tmp;
    }
	double dx = x_tmp;
	double dy = x_tmp * tan(arrow->angle);
	if (arrow->angle > 2 * PI || arrow->angle < -2 * PI) {
        arrow->angle = 0;
    }
    XDrawLine(units->dpy, units->win, units->gc[cond], arrow->center.x, arrow->center.y, arrow->center.x + dx, arrow->center.y - dy);
 	XFlush(units->dpy);
}

// void redraw(XEvent *event, Units *units, Arrow *arrow) {
// 	static XRectangle clip[32];     
// 	static int count = 0;               
//     clip[count].x = event->xexpose.x;
//     clip[count].y = event->xexpose.y;
//     clip[count].width =  event->xexpose.width;
//     clip[count].height =  event->xexpose.height;
//     count++;
//     if (event->xexpose.count <= 0 || count >= 32) {
//         XSetClipRectangles(event->xexpose.display, units->gc, 0, 0, clip, count, Unsorted);
//         draw_arrow(units, arrow, 0);
//         XSetClipMask(event->xexpose.display, units->gc, None);
//         count = 0;
//     }
// }