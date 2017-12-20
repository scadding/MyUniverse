#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysgen.h"

void   drawview();
int mainwrld(mainworld *mw, long x, long y, long z);

mainworld      mw;
sistem syst;

int main(int argc, char **argv) {
    long    x, y, z;
    int     axis;
    char    inbuf[40];
    char    c;

    if(argc > 1) {
        x = atol(argv[1]);
    } else {
        x = 0L;
    }
    if(argc > 2) {
        y = atol(argv[2]);
    } else {
        y = 0L;
    }
    if(argc > 3) {
        z = atol(argv[3]);
    } else {
        z = 0L;
    }

    axis = 2;
    do {
        drawview(x, y, z, axis);
        c = getchar();
        switch(c) {
        case 'A':
        case 'a':
        case '+':
            axis = (axis + 1)%3;
            break;
        case 'x':
            x++;
            break;
        case 'X':
            x--;
            break;
        case 'y':
            y++;
            break;
        case 'Y':
            y--;
            break;
        case 'z':
            z++;
            break;
        case 'Z':
            z--;
            break;
        case '2':
            sprintf(inbuf, "sysgen2 %ld %ld %ld", x, y, z);
            system(inbuf);
            getchar();
            break;
        case '3':
            sprintf(inbuf, "sysgen3 %ld %ld %ld", x, y, z);
            system(inbuf);
            getchar();
            break;
        }
    } while(c != 'Q');
    exit(0);
}

void drawview(long x, long y, long z, int axis) {
    long    h, v;
    long    xx, yy, zz;

    printf("\033[2J");
    if(axis == 1) {
        printf("\t    Z\n");
    } else {
        printf("\t    Y\n");
    }
    for(v = -2L; v <= 2L; v++) {
        if(!v) {
            if(axis != 0) {
                printf("X");
            } else {
                printf("Z");
            }
        }
        printf("\t");
        for(h = -2L; h <= 2L; h++) {
            if(axis == 0) {
                xx = x;
                yy = y + v;
                zz = z + h;
            } else if(axis == 1) {
                xx = x + h;
                yy = y;
                zz = z + v;
            } else {
                xx = x + h;
                yy = y + v;
                zz = z;
            }
            if(mainwrld(&mw, xx, yy, zz)) {
                printf("%c", mw.starport);
            } else {
                printf(" ");
            }
            if((h == 0L) && (v == 0L)) {
                printf("<");
            } else {
                printf(" ");
            }
        }
        if(!v) {
            if(axis != 0) {
                printf("   x");
            } else {
                printf("   z");
            }
        }
        printf("\n");
    }
    if(axis == 1) {
        printf("\t    z\n");
    } else {
        printf("\t    y\n");
    }
    if(mainwrld(&mw, x, y, z)) {
        printf("\nLocation %ld,%ld,%ld\n", x, y, z);
        printf("%c%1x%1x%1x%1x%1x%1x-%x %c%c%c %s\n",
               mw.starport, mw.size,
               mw.atmos, mw.hydro, mw.pop, mw.gov, mw.law,
               mw.tech, (mw.bases&NAVAL_BASE)? 'N' : ' ',
               (mw.bases&SCOUT_BASE)? 'S' : ' ',
               (mw.bases&LOCAL_BASE)? 'M' : ' ',
               mw.name);
    }
}
