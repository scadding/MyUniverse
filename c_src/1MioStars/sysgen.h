#include "magic.h"

#define MAX_NAME 40
#define mainworld      struct MW
#define world          struct MW
#define system         struct SYS
#define body           struct BDY
#define giant          struct GAS
#define star           struct ST

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

#define rand myRand
#define srand mySrand
#define class starClass

mainworld {
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

giant {
    int     size;
};

star {
    char    class;
    int     decimal, size;
};

system {
    body    *sys;
    body    *mw;
    long    x, y, z;
};

body {
    char    name[MAX_NAME];
    char    type;
    union   {
        world   w;
        giant   g;
        star    s;
    }       upp;
    body    *p, *i, *o, *b;
    int     orbit;
};

char   *getname();
void   srand();
unsigned int   rand();
int    staroff();
char   zone();
void   addcomp();
body   *getorb();
body   *rollorb();
void   doempties();
void   dogases();
void   dotoids();
mainworld      *populate();
int    makesystem();
void   primary();
void   clean();
body   *getmain();
void   satelite();
void   insert();
void   forcemain();
int    exist(long x, long y, long z);
void   xyzmagic(long x, long y, long z, long magic);
int    D(int n);
void   seedlang(long x, long y, long z);
int mainwrld(mainworld *mw, long x, long y, long z);
void   scatter(int *arr, unsigned int max_arr, int slide);
int    lookup(int *arr, int tot);
void   drift();
void   srand(long seed);
int    makesystem(system *syst, long x, long y, long z);
void   primary(body *s, world *mw);
void   clean(body *s);
void   forcemain(body *s, mainworld *mw);
body *getmain(body *s);
void   printsyst(system *syst);
void plevel(body *s, int depth);
void   pworld(body *s);
void   doempties(body *s, int n);
void   dogases(body *s, int n);
void   dotoids(body *s, int n);
mainworld      *populate(body *s, mainworld *mw);
void   insert(body *s, body *sub);
void   satelite(body *pstar, body *s, int num, mainworld *mw);
body   *rollorb(body *s, int nd, int mod);
char   zone(body *s, int orbit);
body   *getorb(body *s, int orbit);
int    staroff(body *s);
void   addcomp(body *s, int orbmod, int sizmod);
