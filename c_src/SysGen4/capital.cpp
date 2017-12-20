#include <stdio.h>
#include <stdlib.h>
#include "sysgen.h"

int mainwrld(mainworld *mw, long x, long y, long z);

int val(mainworld *mw)
{
    return(mw->tech);
}

int main(int argc, char **argv)
{
    mainworld       mw, best;
    long    x, y, z;
    long    A, B, C;
    long    bA = 0, bB = 0, bC = 0;
    char    first;

    if((argc < 3) || (argc > 4)) {
        printf("Usage: captial X Y [Z]\n");
        exit(1);
    }
    x = atol(argv[1]);
    y = atol(argv[2]);
    if(argc == 4) {
        z = atol(argv[3]);
    } else {
        z = 0L;
    }

    first = 1;
    for(A = x; A < x + 10L; A++) {
        printf("%ld", A);
        for(B = y; B < y + 10L; B++) {
            printf(".");
            for(C = z; C < z + 10L; C++)
                if(mainwrld(&mw, A, B, C) == 0) {
                    continue;
                } else {
                    if(first || (val(&mw) >= val(&best))) {
                        best = mw;
                        bA = A;
                        bB = B;
                        bC = C;
                        first = 0;
                        printf("%d", val(&mw));
                        printf("%c%1x%1x%1x%1x%1x%1x-%x %c%c%c %8ld %8ld %8ld %s\n",
                               best.starport, best.size,
                               best.atmos, best.hydro, best.pop, best.gov, best.law,
                               best.tech, (best.bases&NAVAL_BASE)? 'N' : ' ',
                               (best.bases&SCOUT_BASE)? 'S' : ' ',
                               (best.bases&LOCAL_BASE)? 'M' : ' ',
                               bA, bB, bC, best.name);
                    }
                }
        }
    }
    printf("\n");
    printf("%c%1x%1x%1x%1x%1x%1x-%x %c%c%c %8ld %8ld %8ld %s\n",
           best.starport, best.size,
           best.atmos, best.hydro, best.pop, best.gov, best.law,
           best.tech, (best.bases&NAVAL_BASE)? 'N' : ' ',
           (best.bases&SCOUT_BASE)? 'S' : ' ',
           (best.bases&LOCAL_BASE)? 'M' : ' ',
           bA, bB, bC, best.name);
    exit(1);
}
