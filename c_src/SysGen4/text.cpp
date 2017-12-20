#include "sysgen.h"
#include <stdio.h>
#include <stdlib.h>

FILE *out;

void   pad(int n) {
    FILE *out = stdout;
    while(n-- > 0) {
        fprintf(out, " ");
    }
}

const char *acover[16] = { "ww", "!!", "**", "MM", "^^", "..", "--", "++",
                           "@@", "WW", "II", "ii"
                         };

void   p2(int val, int type) {
    FILE *out = stdout;
    if(type == 0) {
        fprintf(out, "%02d", val);
    } else if(type == 1) {
        switch(val) {
        case 0:
            fprintf(out, "..");
            break;
        case 1:
            fprintf(out, "LL");
            break;
        case 2:
            fprintf(out, "ll");
            break;
        case -1:
            fprintf(out, "WW");
            break;
        case -2:
            fprintf(out, "ww");
            break;
        }
    } else if(type == 2) {
        if(val < 0) {
            fprintf(out, "??");
        } else {
            fprintf(out, "%.2s", acover[val%16]);
        }
    }
}

void printw(int buf[20][28], int type) {
    int     l, t, j;
    FILE *out = stdout;

    for(l = 0; l < 6; l++) {
        pad(1);
        for(t = 0; t < 5; t++) {
            pad(5 - l);
            if(l == 0) {
                p2(buf[t][0], type);
            } else {
                j = left[l - 1];
                while(j <= right[l - 1]) {
                    p2(buf[t][j++], type);
                }
            }
            pad(5 - l);
        }
        fprintf(out, "\n");
    }
    for(t = 0; t < 5; t++)
        for(j = 21; j < 27; j++) {
            p2(buf[t][j], type);
        }
    p2(buf[4][27], type);
    fprintf(out, "\n");
    for(l = 0; l < 5; l++) {
        pad(l + 1);
        for(t = 5; t < 10; t++)

        {
            for(j = left[4 - l]; j < right[4 - l]; j++) {
                p2(buf[t][j], type);
            }
            for(j = left[l]; j < right[l]; j++) {
                p2(buf[t+5][j], type);
            }
        }
        p2(buf[15][right[l]], type);
        fprintf(out, "\n");
    }
    pad(6);
    for(t = 15; t < 20; t++)
        for(j = 21; j < 27; j++) {
            p2(buf[t][j], type);
        }
    p2(buf[19][27], type);
    fprintf(out, "\n");
    for(l = 5; l >= 0; l--) {
        pad(7);
        for(t = 15; t < 20; t++) {
            pad(5 - l);
            if(l == 0) {
                p2(buf[t][0], type);
            } else {
                j = left[l - 1];
                while(j <= right[l - 1]) {
                    p2(buf[t][j++], type);
                }
            }
            pad(5 - l);
        }
        fprintf(out, "\n");
    }
}

void   ptri(int tri[28], int mode, int ori) {
    int     l, ll, j;
    FILE *out = stdout;

    for(l = 0; l < 7; l++) {
        if(ori) {
            ll = 6 - l;
        } else {
            ll = l;
        }
        pad(6 - ll);
        if(ll == 0) {
            p2(tri[0], mode);
        } else {
            j = left[ll - 1];
            while(j <= right[ll - 1]) {
                p2(tri[j++], mode);
            }
        }
        fprintf(out, "\n");
    }
}
