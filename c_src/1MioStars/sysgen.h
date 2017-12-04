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
