#include "sysgen.h"

void   xyzmagic(x, y, z, magic)
long   x, y, z, magic;
{
       long    seed;

       x &= 0x07ff;
       x <<= (32 - 11);
       y &= 0x07ff;
       y <<= (32 - 11 - 11);
       z &= 0x03ff;
       seed = x|y|z;
       srand(seed + magic);
}

int    D(n)
{
       int     sum;

       sum = n;
       while (n--)
               sum += rand()%6;
       return(sum);
}

struct long32 {
       unsigned        val:32;
       } randx;
char   randf = 0;

void   srand(seed)
long   seed;
{
       randx.val = seed;
       randf = 1;
}

unsigned int   rand()
{
       long    l;
       if (!randf)
               srand(1L);
       l = randx.val;
       randx.val = l*1103515245L + 12345L;
       return ((unsigned int)(((randx.val)>>16) & 0777777));
}
