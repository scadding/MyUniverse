#include "sysgen.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

sistem syst;
void pdlevel();
void   tview();
void   mview();
void showMap(long x, long y, long z, char *planetName);
body *makemap(long x, long y, long z, char *name, int plates[20][28],
              int land[20][28], int cov[20][28]);

int main(int argc, char **argv) {
    long    x, y, z;

    if(argc != 5) {
        printf("Usage: sysgen4 X Y Z planetName\n");
        exit(1);
    }
    x = atol(argv[1]);
    y = atol(argv[2]);
    z = atol(argv[3]);

    showMap(x, y, z, argv[4]);
}

void showMap(long x, long y, long z, char *planetName) {
    body    *s;
    int     plates[20][28];
    int     land[20][28];
    int     cov[20][28];

    if((s = makemap(x, y, z, planetName, plates, land, cov)) == 0) {
        exit(0);
    }
    if(s->det.w.nplates > 1) {
        printf("%s Tectonic plates:\n", s->name);
        printw(plates, 0);
    }
    printf("%s Surface:\n", s->name);
    printw(land, 1);
    printf("%s Cover:\n", s->name);
    mview(cov, 2);
}


void   mview(int buf[20][28], int mode) {
    char    inbuf[255];

    for(;;) {
        printw(buf, mode);
        printf("Enter triangle [1-20]:");
        fgets(inbuf,255,stdin);
        int i = atoi(inbuf) - 1;
        if((i < 0) || (i > 19)) {
            return;
        }
        tview(buf[i], 0, i, 0L, mode, (i/5)%2);
    }
}

void   tview(int tri[28], int depth, int triang, long dir, int mode, int ori) {
    int     exp[28];

    for(;;) {
        ploc(triang, depth, dir);
        ptri(tri, mode, ori);
        printf("Type 0-3 for expansion, anything else to go up...");
        int c = getchar() - '0';
        printf("\n");
        if((c < 0) || (c > 3)) {
            return;
        }
        long ndir = dir | (c<<(depth*2));
        expand(tri, exp, c, dir);
        tview(exp, depth + 1, triang, ndir, mode, (c)?ori:!ori);
    }
}
