#include "sysgen.h"

void setcount(int vbuf[20][28], int cbuf[20][28], int t, int h, int val)
{
    int     i, len;
    int     hadj[6][2];

    set(vbuf, t, h, val);
    set(cbuf, t, h, -999);
    len = adj(t, h, hadj);
    for(i = 0; i < len; i++) {
        set(cbuf, hadj[i][0], hadj[i][1], cbuf[hadj[i][0]][hadj[i][1]] + 1);
    }
}

void   plates(int buf[20][28], int num)
{
    int     i, j, cont;
    int     size;
    int     t, h;
    int     tmax, hmax, max;
    int     cbuf[20][28];

    for(i = 0; i < 20; i++)
        for(j = 0; j < 28; j++) {
            buf[i][j] = 0;
        }
    if(num < 2) {
        return;
    }
    num--;
    for(cont = 1; cont <= num; cont++) {
        size = D(2)*5*D(1);
        for(i = 0; i < 20; i++)
            for(j = 0; j < 28; j++)
                if(buf[i][j] == 0) {
                    cbuf[i][j] = 0;
                }
                else {
                    cbuf[i][j] = -999;
                }
        if(find(buf, &t, &h, 0)) {
            break;
        }
        setcount(buf, cbuf, t, h, 16 + cont);
        while(--size) {
            tmax = 0;
            hmax = 0;
            max = cbuf[tmax][hmax];
            for(t = 0; t < 20; t++)
                for(h = 0; h < 28; h++) {
                    if((cbuf[t][h] > max)
                       || ((cbuf[t][h] == max) && (D(1) <= 2))) {
                        tmax = t;
                        hmax = h;
                        max = cbuf[tmax][hmax];
                    }
                }
            if(max <= 0) {
                break;
            }
            setcount(buf, cbuf, tmax, hmax, 16 + cont);
        }
    }
}

int find(int buf[20][28], int *t, int *h, int val)
{
    int     tt, hh;

    *t = Rand()%20;
    *h = Rand()%28;

    for(tt = 0; tt < 20; tt++)
        for(hh = 0; hh < 28; hh++)
            if(buf[(*t + tt)%20][(*h + hh)%28] == val) {
                *t = (*t + tt)%20;
                *h = (*h + hh)%28;
                return(0);
            }
    return(1);
}

int    findt, findh;

int    findfirst(int buf[20][28], int *t, int *h, int val)
{
    findt = Rand()%20;
    findh = Rand()%28;
    *t = findt;
    *h = findh;
    if(buf[findt][findh] == val) {
        return(0);
    }
    return(findnext(buf, t, h, val));
}

int findnext(int buf[20][28], int *t, int *h, int val)
{
    for(;;) {
        (*h)++;
        if(*h == 28) {
            *h = 0;
            (*t)++;
            if(*t == 20) {
                *t = 0;
            }
        }
        if((*h == findh) && (*t == findt)) {
            return(1);
        }
        if(buf[*t][*h] == val) {
            return(0);
        }
    }
}

int counthex(int buf[20][28], int t, int h, int val)
{
    int     i, len, tot;
    int     hadj[6][2];

    tot = 0;
    len = adj(t, h, hadj);
    for(i = 0; i < len; i++)
        if(buf[hadj[i][0]][hadj[i][1]] == val) {
            tot++;
        }
    return(tot);
}
