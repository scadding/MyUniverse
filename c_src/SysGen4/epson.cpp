#include "sysgen.h"
#include <stdio.h>

#define V_HEIGHT 7
int    line[1024];
int    hpos = 0, vpos = 0;
int    hmax;
extern FILE    *out;

void   bclear() {
    int     i;

    for(i = 0; i < 1024; i++) {
        line[i] = 0;
    }
    hmax = 0;
}

void   epad(int n) {
    if(!hpos && !vpos) {
        bclear();
    }
    hpos += n;
    if(hpos > hmax) {
        hmax = hpos;
    }
}

void   enl() {
    if(!hpos && !vpos) {
        bclear();
    }
    hpos = 0;
    vpos++;
    if(vpos != V_HEIGHT) {
        return;
    }
    while(hmax > 1)
        if(line[hmax - 1]) {
            break;
        } else {
            hmax--;
        }
    fputc(0x1b, out);
    fputc('1', out);
    fputc(0x1b, out);
    fputc('L', out);
    fputc(hmax%256, out);
    fputc(hmax/256, out);
    for(hpos = 0; hpos < hmax; hpos++) {
        fputc(line[hpos], out);
    }
    fputc('\r', out);
    fputc('\n', out);
    hpos = 0;
    vpos = 0;
}

void ep2(int c, int mode) {
    int     bit1, bit2;

    if(!hpos && !vpos) {
        bclear();
    }
    bit1 = 0;
    bit2 = 0;
    switch(c) {
    case C_WATER:
    case C_DEEP:
    case C_SICE:
    case C_WICE:
        break;
    case C_TUNDRA:
    case C_ROUGH:
        if((hpos%3) != 2) {
            bit1 = 1;
        }
        if(((hpos+1)%3) != 2) {
            bit2 = 1;
        }
        break;
    case C_MTNS:
        if((hpos%4) != 3) {
            bit1 = 1;
        }
        if(((hpos+1)%4) != 3) {
            bit2 = 1;
        }
        break;
    case C_FOREST:
    case C_JUNGLE:
        if(hpos%2) {
            bit1 = 1;
        } else {
            bit2 = 1;
        }
        break;
    case C_OPEN:
        if((hpos%3) == 0) {
            bit1 = 1;
        }
        if(((hpos+1)%3) == 0) {
            bit2 = 1;
        }
        break;
    case C_DESERT:
        if((hpos%4) == 0) {
            bit1 = 1;
        }
        if(((hpos+1)%4) == 0) {
            bit2 = 1;
        }
        break;
    }
    if(bit1) {
        line[hpos] |= (1<<(V_HEIGHT-1-vpos));
    }
    hpos++;
    if(bit2) {
        line[hpos] |= (1<<(V_HEIGHT-1-vpos));
    }

    hpos++;
    if(hpos > hmax) {
        hmax = hpos;
    }
}
