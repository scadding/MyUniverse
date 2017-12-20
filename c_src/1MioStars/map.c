#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysgen.h"

int main(int argc, char **argv)
{
    long    x, y, z;
    long    x1, y1, z1;
    int     tot;

    if ((argc < 3) || (argc > 4)) {
        printf("Usage: %s X Y [Z]\n", argv[0]);
        exit(1);
    }
    x = atol(argv[1]);
    y = atol(argv[2]);
    if (argc == 4)
        z = atol(argv[3]);
    else
        z = 0L;

    printf("%ld,%ld,%ld\n", x, y, z);
    for (y1 = y; y1 < y + 10L; y1++) {
        for (x1 = x; x1 < x + 10L; x1++) {
            tot = 0;
            for (z1 = z; z1 < z + 10L; z1++)
                if (exist(x1, y1, z1))
                    tot++;
            printf("%2x", tot);
        }
        printf("\n");
    }
    exit(1);
}
