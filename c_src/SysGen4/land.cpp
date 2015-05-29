#include "sysgen.h"

void lands(int plates[20][28], int buf[20][28], int arch, int islands, int minor, int major, int landpc)
{
    int     land, i, tot;
    int     majsize[15], minsize[15];

    for(i = 0; i < 20; i++)
        for(tot = 0; tot < 28; tot++) {
            buf[i][tot] = 0;
        }
    if(major || minor) {
        land = (362*landpc)/100 - (arch + 1)/2 - islands;
        minsize[0] = land/(minor + 2*major);
        for(i = 1; i < minor; i++) {
            minsize[i] = minsize[0];
        }
        for(i = 0; i < major; i++) {
            majsize[i] = minsize[0];
        }
        tot = minsize[0]*(minor + 2*major);
        while(tot < land) {
            if(major && minor) {
                if(Rand()%2) {
                    majsize[Rand()%major]++;
                }
                else {
                    minsize[Rand()%minor]++;
                }
            }
            else if(major) {
                majsize[Rand()%major]++;
            }
            else {
                minsize[Rand()%minor]++;
            }
            tot++;
        }
        for(i = 0; i < major; i++) {
            place(plates, buf, majsize[i], 1);
        }
        for(i = 0; i < minor; i++) {
            place(plates, buf, minsize[i], 1);
        }
    }
    for(i = 0; i < islands; i++) {
        place(plates, buf, 1, 1);
    }
    for(i = 0; i < arch; i++) {
        place(plates, buf, 1, 2);
    }
    for(i = 0; i < 20; i++)
        for(tot = 0; tot < 28; tot++)
            if(buf[i][tot] == 0) {
                buf[i][tot] = -1;
            }
}

void place(int plates[20][28], int buf[20][28], int size, int val)
{
    int     t, h, pval;
    int     tmax, hmax, max, i;

    if(find(buf, &t, &h, 0)) {
        return;
    }
    set(buf, t, h, val);
    pval = plates[t][h];
    while(--size) {
        if(findfirst(buf, &t, &h, 0)) {
            break;
        }
        tmax = t;
        hmax = h;
        max = countlhex(buf, plates, t, h, val, pval);
        while(!findnext(buf, &t, &h, 0)) {
            i = countlhex(buf, plates, t, h, val, pval);
            if((i > max) ||
               ((i == max) && !(Rand()%3))) {
                tmax = t;
                hmax = h;
                max = i;
            }
        }
        set(buf, tmax, hmax, val);
    }
}

int countlhex(int buf[20][28], int plates[20][28], int t, int h, int cval, int pval)
{
    int     i, len, tot;
    int     hadj[6][2];

    tot = 0;
    len = adj(t, h, hadj);
    for(i = 0; i < len; i++)
        if(buf[hadj[i][0]][hadj[i][1]] == cval) {
            break;
        }
    if(i == len) {
        return(0);
    }
    tot = 66;
    for(i = 0; i < len; i++) {
        if(buf[hadj[i][0]][hadj[i][1]] == cval) {
            tot += 5;
        }
        else if(buf[hadj[i][0]][hadj[i][1]] != 0) {
            tot -= 10;
        }
        if(plates[hadj[i][0]][hadj[i][1]] == pval) {
            tot++;
        }
        else {
            tot--;
        }
    }
    return(tot);
}
