#include "sysgen.h"
void filltemp(body *s, tempdata *t);
float tempat(tempdata *t, int row, int mode);

void cover(int cov[20][28],
           int land[20][28],
           int plates[20][28],
           body *s)
{
    int     h, t, row, i, len;
    int     sumfreeze, winfreeze;
    int     nland;
    tempdata        temp;
    float   tmp;
    int     hadj[6][2];

    for(t = 0; t < 20; t++)
        for(h = 0; h < 28; h++) {
            cov[t][h] = -1;
        }
    filltemp(s, &temp);
    for(sumfreeze = 0; sumfreeze < 11; sumfreeze++)
        if(tempat(&temp, sumfreeze, 32+1+4) <= 0.0) {
            break;
        }
    for(winfreeze = 0; winfreeze < 11; winfreeze++)
        if(tempat(&temp, winfreeze, 32+2+8) <= 0.0) {
            break;
        }
    /* sort out water */
    for(t = 0; t < 20; t++)
        for(h = 0; h < 28; h++) {
            if(land[t][h] != -1) {
                continue;
            }
            row = getrow(t, h);
            if(row < winfreeze) {
                len = adj(t, h, hadj);
                for(i = 0; i < len; i++)
                    if(land[hadj[i][0]][hadj[i][1]] != -1) {
                        break;
                    }
                if(i == len) {
                    cov[t][h] = C_DEEP;
                }
                else {
                    cov[t][h] = C_WATER;
                }
            }
            else if(row < sumfreeze) {
                cov[t][h] = C_WICE;
            }
            else {
                cov[t][h] = C_SICE;
            }
        }
    /* sort out mountains */
    nland = 0;
    for(t = 0; t < 20; t++)
        for(h = 0; h < 28; h++)
            if(land[t][h] == 1) {
                nland++;
                set(land, t, h, 10);
            }
    for(t = 0; t < 20; t++)
        for(h = 0; h < 28; h++)
            if(land[t][h] == 10) {
                set(land, t, h, 1);
            }
    i = (D(2) - 2)*D(1);
    len = (int)(((long)nland*(long)i)/100L);
    if(len) {
        mplace(cov, plates, len);
    }
    /* sort out tundra */
    for(t = 0; t < 20; t++)
        for(h = 0; h < 28; h++) {
            if(land[t][h] < 1) {
                continue;
            }
            if(cov[t][h] >= 0) {
                continue;
            }
            row = getrow(t, h);
            if(row >= sumfreeze) {
                set(cov, t, h, C_TUNDRA);
            }
        }
    /* desert */
    if(D(2) >= s->upp.w.hydro) {
        contigious(cov, -1, C_DESERT, D(2) - 1);
    }
    /* set non-tropical to open */
    len = 0;
    for(t = 0; t < 20; t++)
        for(h = 0; h < 28; h++) {
            if(cov[t][h] != -1) {
                continue;
            }
            row = getrow(t, h);
            tmp = tempat(&temp, row, 32);
            if((tmp < 20.0) || (tmp > 40.0)) {
                set(cov, t, h, C_OPEN);
            }
            else {
                len = 1;
            }
        }
    /* set jungle */
    if(len)
        while(D(2) <= s->upp.w.hydro)
            if(!contigious(cov, -1, C_JUNGLE, D(2) - 1)) {
                break;
            }
    /* set open to open/forest */
    for(t = 0; t < 20; t++)
        for(h = 0; h < 28; h++) {
            if(cov[t][h] == -1) {
                set(cov, t, h, C_OPEN);
            }
            else if(cov[t][h] != C_OPEN) {
                continue;
            }
            row = getrow(t, h);
            tmp = tempat(&temp, row, 32);
            i = D(2);
            if(tmp > 40.0) {
                i -= (int)(tmp/10.0);
            }
            if(i >= s->upp.w.pop) {
                set(cov, t, h, C_FOREST);
            }
        }
}

int contigious(int buf[20][28],
               int val,
               int nval,
               int size)
{
    int     t, h;
    int     tt, hh;
    int     s;
    int     tbuf[20][28];   /* -1 = no go, 0 = possible */

    tt = Rand()%20;
    hh = Rand()%20;
    s = 0;
    for(t = 0; t < 20; t++)
        for(h = 0; h < 28; h++)
            if(buf[t][h] == val) {
                tbuf[t][h] = 0;
                s = 1;
            }
            else {
                tbuf[t][h] = -1;
            }
    if(!s) {
        return(0);
    }
    for(t = 0; t < 20; t++)
        for(h = 0; h < 28; h++) {
            if(tbuf[(t+tt)%20][(h+hh)%28]) {
                continue;
            }
            s = size;
            if(!iscontig(tbuf, (t+tt)%20, (h+hh)%28, &s)) {
                continue;
            }
            setcontig(buf, (t+tt)%20, (h+hh)%28, val, nval, &size);
            return(1);
        }
    return(0);
}

int iscontig(int tbuf[20][28], int t, int h, int *size)
{
    int     i, ii, len;
    int     tt, hh;
    int     hadj[6][2];
    int     chk[6];

    if(tbuf[t][h] == -1) {
        return(0);
    }
    else if(tbuf[t][h] == 0) {
        tbuf[t][h] = 1;
        (*size)--;
    }
    len = adj(t, h, hadj);
    ii = Rand()%len;
    for(i = 0; i < len; i++) {
        chk[i] = 0;
        tt = hadj[(i+ii)%len][0];
        hh = hadj[(i+ii)%len][1];
        if(tbuf[tt][hh] != 0) {
            continue;
        }
        (*size)--;
        chk[i] = 1;
        tbuf[tt][hh] = 1;
    }
    if(*size < 0) {
        return(1);
    }
    for(i = 0; i < len; i++) {
        if(!chk[i]) {
            continue;
        }
        if(iscontig(tbuf, hadj[(i+ii)%len][0], hadj[(i+ii)%len][1], size)) {
            return(1);
        }
    }
    return(0);
}

int setcontig(int tbuf[20][28], int t, int h, int val, int nval, int *size)
{
    int     i, ii, len;
    int     tt, hh;
    int     hadj[6][2];
    int     chk[6];

    if(tbuf[t][h] == val) {
        set(tbuf, t, h, nval);
        (*size)--;
    }
    len = adj(t, h, hadj);
    ii = Rand()%len;
    for(i = 0; i < len; i++) {
        chk[i] = 0;
        tt = hadj[(i+ii)%len][0];
        hh = hadj[(i+ii)%len][1];
        if(tbuf[tt][hh] != val) {
            continue;
        }
        (*size)--;
        chk[i] = 1;
        set(tbuf, tt, hh, nval);
        if(*size < 0) {
            return(1);
        }
    }
    for(i = 0; i < len; i++) {
        if(!chk[i]) {
            continue;
        }
        setcontig(tbuf, hadj[(i+ii)%len][0], hadj[(i+ii)%len][1],
                  val, nval, size);
        if(*size < 0) {
            return(1);
        }
    }
    return(0);
}
