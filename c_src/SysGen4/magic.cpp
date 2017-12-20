#include "sysgen.h"

void xyzmagic(long x, long y, long z, long magic) {
    long    seed;

    x &= 0x07ff;
    x <<= (32 - 11);
    y &= 0x07ff;
    y <<= (32 - 11 - 11);
    z &= 0x03ff;
    seed = x|y|z;
    sRand(seed + magic);
}

int D(int n) {
    int     sum;

    sum = n;
    while(n--) {
        sum += Rand()%6;
    }
    return(sum);
}

struct long32  randx;
char randf = 0;

void sRand(long seed) {
    randx.val = seed;
    randf = 1;
}

int dice_rolls = 0;

unsigned int Rand() {
    long    l;
    if(!randf) {
        sRand(1L);
    }
    dice_rolls++;
    l = randx.val;
    randx.val = l*1103515245L + 12345L;
    return ((unsigned int)(((randx.val)>>16) & 0777777));
}


