#include "sysgen.h"
#include "magic.h"
#define abs(t) ((t<0L)?(-t):(t))

int exist(long x, long y, long z) {
    int     chance, roll;
    long    mag;

    mag = abs(x) + abs(y) + abs(z);
    chance = 80 - (int)(80L*mag/GAL_RADIUS);
    xyzmagic(x>>4, y>>4, z>>4, EXIST_MAGIC);        /* sector density variation */
    chance += (Rand()%40) - 20;
    xyzmagic(x, y, z, EXIST_MAGIC);
    roll = Rand()%100;
    if(roll > chance) {
        return(0);
    }
    chance /= 20;
    if(++chance > 4) {
        chance = 4;
    }
    return(chance);
}
