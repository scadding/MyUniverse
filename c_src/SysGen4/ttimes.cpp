#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysgen.h"
#include "stelfun.h"
#include "system.h"
#include "populate.h"
#include "details.h"
#include <math.h>

float  odist[] = {
    0.2, 0.4, 0.7, 1.0, 1.6, 2.8, 5.2, 10.0, 19.6, 38.8, 77.2,
    154.0, 307.6, 614.8, 1229.2, 2458.0, 4915.6, 9830.8, 19661.2, 39322.0
};

int main(int argc, char **argv) {
    sistem  syst;
    body    *i, *j;
    float   tmp, g;
    long    x, y, z;

    if((argc < 3) || (argc > 4)) {
        printf("Usage: sysgen X Y [Z]\n");
        exit(1);
    }
    x = atol(argv[1]);
    y = atol(argv[2]);
    if(argc == 4) {
        z = atol(argv[3]);
    } else {
        z = 0L;
    }

    if(makesystem(&syst, x, y, z) == 0) {
        exit(0);
    }
    printf("%s system\n", syst.mw->name);
    printf("Location %ld,%ld,%ld\n", syst.x, syst.y, syst.z);
    for(g = 1.0; g <= 6.0; g += 1.0) {
        printf("Travel times at %gG\n", g);
        for(i = syst.sys->b; i; i = i->o) {
            printf("| ");
            for(j = syst.sys->b; j != i; j = j->o) {
                tmp = odist[i->orbit] - odist[j->orbit];
                tmp = sqrt(tmp*149600.0/32.4/g);
                if(tmp < 24.0) {
                    printf("%4.1fh | ", tmp);
                } else {
                    printf("%4.1fd | ", tmp/24.0);
                }
            }
            printf("%s\n", i->name);
        }
        printf("\n");
    }
    exit(1);
}
