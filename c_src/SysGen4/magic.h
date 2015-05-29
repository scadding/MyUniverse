/* compile constants: */
/*#define bit16        *//* if you system's ints are 16 bits or less
                       this must be set */
#define GAL_RADIUS 40000L

#define LANG_MAGIC 1L
#define EXIST_MAGIC 2L
#define DETAIL_MAGIC 8L
#define MAP_MAGIC 16L

void xyzmagic(long x, long y, long z, long magic);

int D(int n);

void sRand(long seed);

unsigned int Rand();

