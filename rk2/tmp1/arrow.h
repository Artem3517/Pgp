
#define NPOINTS 1    //количество точек для отрисоки
#define C_DOT_RADIUS 20     //радиус центральной точки для диспатча
#define DELAY (1<<13)

/*структура стрелы*/
typedef struct
{
	float A;
    float q;
    float w;        /* угловые координаты каждой точки от центра  */
    float dA;       /* смещение угловых координат */


    XPoint c;
    XPoint pq;     /* центральная точка стрелы */
    XPoint pw;
    int R;        /* радиус каждой точки*/

    int L;
    	//int Lw;
} XArrow;

int overlap(XEvent* ev);
int configure_arrow(XArrow*, int, int);        /* задание точек стрелки */
int redraw(XEvent*, GC, XArrow*);             /* перерисовка потерянных фрагментов окна */
int amod2pi(XArrow*);                /* изменение координат углов поворота, проверка на выход их из диапазона возможных значений */
int draw_arrow(Display *, Window , GC  ,XArrow *pr);     /*отрисовка стрелы в окне*/
int key_check(XEvent* ev, Display* dpy, Window win, XArrow* pr, GC gc);              /* ответ на собития нажатия клавиш */
