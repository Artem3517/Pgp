#include "graph.h"

static XVertex vertex[NVERT];
static XEdge edge[NEDGE];
static XFace face[(NFACE + 1)];
static XPoint face4[NF4][(4 + 1)];
static XPoint scale;

int assoc(XPolyGraph *pg) {
    pg->vertex = vertex;
    pg->edge = edge;
    pg->face = face;
    retrace();
    return 0;
}

int retrace() {
    int i = 0;
    for (int j = 0; j < NF4; j++, i++) {
        face[i].top = face4[j];
        face[i].count = 4;
        face[i].color = DEFTONE;
        face[i].zone = XCreateRegion();
    }
    face[NF3].color = DEFTONE;
    return 0;
}

int resize(unsigned int w, unsigned int h) {
    static XRectangle back = {0, 0, 0, 0};
    if ((back.width == w) && (back.height == h)) {
        return 0;
    }
    back.width = w;
    back.height = h;
    return NFACE;
}

int rescale(unsigned int w, unsigned int h) {
    int x = w / NUNIT;
    int y = h / NUNIT;
    if ((scale.x == x) && (scale.y == y)) {
        return 0;
    }
    scale.x = x;
    scale.y = y;
    return NFACE;
}

int rebuild() {
    static XPoint vconf[] = {{4, 4},  {14, 4},  {14, 14}, {4, 14}, {8, 6},
                             {12, 6}, {12, 12}, {6, 12},  {6, 8},  {8, 8}};
    static int fconf4[NF4][(4 + 1)] = {{0, 1, 5, 4, 0}, {0, 4, 9, 8, 0}, {0, 8, 7, 3, 0}, {4, 5, 6, 9, 4},
                                       {8, 9, 6, 7, 8}, {3, 7, 6, 2, 3}, {5, 1, 2, 6, 5}};
    static int econf[NEDGE][2] = {{0, 1}, {0, 3}, {0, 4}, {0, 8}, {1, 2}, {1, 5}, {2, 3}, {2, 6},
                                  {3, 7}, {4, 5}, {4, 9}, {5, 6}, {6, 7}, {6, 9}, {7, 8}, {8, 9}};
    for (int i = 0; i < NVERT; i++) {
        vertex[i].x = scale.x * vconf[i].x;
        vertex[i].y = scale.y * vconf[i].y;
    }
    for (int i = 0; i < NVERT; i++) {
        edge[i].x1 = vertex[econf[i][0]].x;
        edge[i].y1 = vertex[econf[i][0]].y;
        edge[i].x2 = vertex[econf[i][1]].x;
        edge[i].y2 = vertex[econf[i][1]].y;
    }
    for (int i = 0; i < NF4; i++) {
        for (int j = 0; j < (4 + 1); j++) {
            face4[i][j].x = vertex[fconf4[i][j]].x;
            face4[i][j].y = vertex[fconf4[i][j]].y;
        }
    }
    return 0;
}

int reconf(unsigned int w, unsigned int h) {
    if (resize(w, h) == 0) {
        return 0;
    }
    if (rescale(w, h) != 0) {
        rebuild();
    }
    return NFACE;
}

int zotone(int x, int y) {
    static XPoint back = {0, 0};
    int f = 0;
    if ((back.x == scale.x) && (back.y == scale.y)) {
        f = NFACE;
    }
    for (; f < NFACE; f++) {
        XDestroyRegion(face[f].zone);
        face[f].zone = XPolygonRegion(face[f].top, face[f].count, 0);
    }
    back.x = scale.x;
    back.y = scale.y;
    for (f = 0; f < NFACE; f++) {
        if (XPointInRegion(face[f].zone, x, y)) {
            break;
        }
    }
    face[f].color = (face[f].color + 1) % NTONE;
    return f;
}