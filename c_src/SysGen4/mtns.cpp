#include "sysgen.h"

void mplace(int cover[20][28], int plates[20][28], int size)
{
    int     t, h, pval;
    int     tmax, hmax, max, i;
    int     len;
    int     hadj[6][2];
    int     best[20][28];

    for(t = 0; t < 20; t++)
        for(h = 0; h < 20; h++) {
            if(cover[t][h] >= 0) {
                best[t][h] = -999;
                continue;
            }
            best[t][h] = 0;
            pval = plates[t][h];
            len = adj(t, h, hadj);
            for(i = 0; i < len; i++)
                if(plates[hadj[i][0]][hadj[i][1]] != pval) {
                    best[t][h] = 10;
                    break;
                }
        }
    while(--size > 0) {
        tmax = 0;
        hmax = 0;
        max = best[tmax][hmax];
        for(t = 0; t < 20; t++)
            for(h = 0; h < 20; h++) {
                if((best[t][h] > max)
                   && (Rand()%2)) {
                    max = best[t][h];
                    tmax = t;
                    hmax = h;
                }
            }
        if(max < 0) {
            break;
        }
        if(Rand()%3) {
            set(cover, tmax, hmax, C_ROUGH);
            i = 2;
        } else {
            set(cover, tmax, hmax, C_MTNS);
            i = 1;
        }
        set(best, tmax, hmax, -999);
        len = adj(t, h, hadj);
        for(i = 0; i < len; i++)
            set(best, hadj[i][0], hadj[i][1],
                best[hadj[i][0]][hadj[i][1]] + i);
    }
}
