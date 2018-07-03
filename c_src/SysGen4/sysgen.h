#include "magic.h"


#define MAX_NAME 40
//#define mainworld      struct MW
#define mainworld        world
//#define world          mainworld
//#define dworld         struct dMW
//#define sistem         struct SYS
//#define body           struct BDY
//#define giant          struct GAS
//#define dgiant         struct dGAS
//#define dbelt          struct dBELT
//#define star           struct ST
//#define city           struct SITY
//#define tempdata       struct TMPDAT

#define NAVAL_BASE 0x0001
#define SCOUT_BASE 0x0002
#define LOCAL_BASE 0x0004

#define T_STAR 's'
#define T_UNSET 'u'
#define T_EMPTY 'E'
#define T_INSIDE 'e'
#define T_WORLD 'w'
#define T_MAIN 'm'
#define T_TOIDS 'a'
#define T_GIANT 'g'

class world
{
public:
    char    name[MAX_NAME];
    char    starport;
    int     size;
    int     atmos;
    int     hydro;
    int     pop;
    int     gov;
    int     law;
    int     tech;
    int     bases;
};

class dworld
{
public:
    char    core;
    float   diam;
    float   dens;
    float   year;   /* in days */
    float   day;    /* in hours */
    int     tilt;
    float   ecnt;
    int     stress;
    float   temp;
    char    life;
    int     water;
    int     nplates;
    int     maj, min, isl, arch;
    class city    *bc;
    float   tpop;
    int     pop_2, pop_3, pop_4;
    long    pop_5;
    int     port_2, port_3, port_4, port_5;
    char    prog_att, prog_act;
    char    aggr_att, aggr_act;
    char    glob_ext, intr_ext;
    int     customs[6][3];
    int     godv, saim, dreq;
    int     rorg, litf, miss;
    int     lweap, ltrade, lcrim, lcivi, lfree;
    int     tlow;
    int     tenergy, trobot, tcommo, tmed, tenv;
    int     tland, twater, tair, tspace;
    int     tpmil, thmil;
};

class giant
{
public:
    int     size;
};

class dgiant
{
public:
    float   diam;   /* in kilometers */
    float   dens;   /* w.r.t. water */
    float   year;   /* in days */
    float   tilt, ecnt;
};

class dbelt
{
public:
    int     pbod, maxbod;   /* size is offset to
1m, 5m, 10m, 25m, 50m, 100m, 300m, 1km, 5km, 10km, 50km, 100km, 500km, 1000km */
    int     n, m, c;        /* nickel, mixed, carbonacous zone size in % */
    int     wid;    /* width of belt in .01 AUs */
    float   year;   /* in days */
};

class star
{
public:
    char    classification;
    int     decimal, size;
};

class sistem
{
public:
    class body    *sys;
    class body    *mw;
    long    x, y, z;
};

class body
{
public:
    char    name[MAX_NAME];
    char    type;
    union {
        dworld  w;
        dgiant  g;
        dbelt   b;
    }       det;
    union {
        world   w;
        giant   g;
        star    s;
    }       upp;
    body    *p, *i, *o, *b;
    int     orbit;
};

class city
{
public:
    char    name[MAX_NAME];
    float   pop;
    char    port;
    city    *next;
};

struct long32 {
#ifdef bit16
    long            val;
#else
    unsigned        val:32;
#endif
};

class tempdata
{
public:
    float   hexrow[11];
    float   hextilt[11];
    float   summer, winter, day, night;
};

#define C_WATER 0
#define C_TUNDRA 2
#define C_MTNS 3
#define C_ROUGH 4
#define C_OPEN 5
#define C_DESERT 6
#define C_FOREST 7
#define C_JUNGLE 8
#define C_DEEP 9
#define C_SICE 10
#define C_WICE 11

extern float   odist[];
extern float   smass[8][13];
extern float   slumen[8][13];
extern struct long32   randx;

extern int     left[5];
extern int     right[5];

/* Main Functions */
int sysgen1Main(long x, long y, long z);
int sysgen2Main(long x, long y, long z);
int sysgen3Main(long x, long y, long z);

#include "proto.h"


