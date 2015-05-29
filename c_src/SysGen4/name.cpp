#include "sysgen.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

int mainwrld(mainworld *mw, long x, long y, long z);

int main(int argc, char **argv)
{
    long    x, y, z;
    int     i;
    mainworld       mw;

    if((argc < 4) || (argc > 5)) {
        printf("Usage: name X Y Z [num]\n");
        exit(1);
    }
    x = atol(argv[1]);
    y = atol(argv[2]);
    z = atol(argv[3]);
    if(argc == 5) {
        i = atoi(argv[4]);
    }
    else {
        i = 1;
    }

    if(mainwrld(&mw, x, y, z) == 0) {
        exit(0);
    }
    time(&x);
    sRand(x);
    while(i--) {
        printf("%s\n", getname());
    }
}
