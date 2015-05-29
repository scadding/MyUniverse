#include <stdio.h>
#include "sysgen.h"
#include <stdlib.h>
#include <string.h>

mainworld      mw;
sistem syst;
int mainwrld(mainworld *mw, long x, long y, long z);

int main(int argc, char **argv)
{
    long    x, y, z;
    long    A, a, B, b, C, c;

    if(argc > 1) {
        x = atol(argv[1]);
    }
    else {
        x = 10000L;
    }
    if(argc > 2) {
        y = atol(argv[2]);
    }
    else {
        y = 10000L;
    }
    if(argc > 3) {
        z = atol(argv[3]);
    }
    else {
        z = 10000L;
    }

    A = a = x;
    B = b = y;
    C = c = z;
    while(1) {
        /* top & bottom */
        for(x = A; x <= a; x++)
            for(y = B; y <= b; y++) {
                funct(x, y, C);
                funct(x, y, c);
            }
        /* left & right */
        for(x = A; x <= a; x++)
            for(z = C + 1; z < c; z++) {
                funct(x, B, z);
                funct(x, b, z);
            }
        /* fore & aft */
        for(y = B+1; y < b; y++)
            for(z = C + 1; z < c; z++) {
                funct(A, y, z);
                funct(a, y, z);
            }
        A--;
        a++;
        B--;
        b++;
        C--;
        c++;
    }
}

int    funct(long x, long y, long z)
{
    if(mainwrld(&mw, x, y, z) == 0) {
        return(0);
    }
    /* modify the following section to scan for what you want */
    if(strcmp(mw.name, "Polo")) {
        return(0);
    }
    printf("%c%1x%1x%1x%1x%1x%1x-%x %c%c%c %8ld %8ld %8ld %s\n",
           mw.starport, mw.size,
           mw.atmos, mw.hydro, mw.pop, mw.gov, mw.law,
           mw.tech, (mw.bases&NAVAL_BASE)? 'N' : ' ',
           (mw.bases&SCOUT_BASE)? 'S' : ' ',
           (mw.bases&LOCAL_BASE)? 'M' : ' ',
           x, y, z, mw.name);
    exit(0);
    return(0);
}
