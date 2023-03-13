#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef XPoint XVertex;
typedef XSegment XEdge;

typedef struct {
  XPoint *top;
  int count;
  int color;
  Region zone;
} XFace;

typedef struct {
  XVertex *vertex;
  XEdge *edge;
  XFace *face;
} XPolyGraph;

#define MB0 4
#define NF3 0
#define NF4 7
#define NF5 0
#define NF6 0
#define NF7 0
#define NF8 0

#define NFACE (NF3 + NF4 + NF5 + NF6 + NF7 + NF8)
#define NEDGE (3 * NF3 + 4 * NF4 + 5 * NF5 + 6 * NF6 + 7 * NF7 + 8 * NF8)
#define NVERT (NEDGE - (NFACE + 1) + 2)

#define NTONE 4
#define DEFTONE 0
#define VDOT 8
#define EWIDTH 2
#define NUNIT 18

int relink(XPolyGraph *pg);
int retrace();
int resize(unsigned int w, unsigned int h);
int rescale(unsigned int w, unsigned int h);
int rebuild();
int reconf(unsigned int w, unsigned int h);
int zotone(int x, int y);

int assoc(XPolyGraph *pg);
GC congraph(Display *dpy);
Window wingraph(Display *dpy, char *title);
int colorite(Display *dpy);
int regraph(Display *dpy, Window win, GC gc, int NoFillFace);
int reset(Display *dpy, Window win, int FillFace);
int reface(Display *dpy, Window win, GC gc, int f);

int rekey(XEvent *ev);
int dispatch(Display *dpy, Window win, GC gc);