#include "sysgen.h"
#include "custom.h"
#include <math.h>

void   pworld();
void pdlevel();
void   print();
void   pdetails();

void   main(argc, argv)
int    argc;
char   **argv;
{
    sistem  syst;
    long    x, y, z, atol();

    if(argc != 5) {
        printf("Usage: sysprint X Y Z planetName\n");
        exit(1);
    }
    x = atol(argv[1]);
    y = atol(argv[2]);
    z = atol(argv[3]);

    if(makedetails(&syst, x, y, z) == 0) {
        exit(0);
    }
    print(&syst, argv[4]);
    exit(1);
}

void   print(syst, name)
sistem *syst;
char   *name;
{
    pdlevel(syst->sys, name);
}

void pdlevel(s, name)
body   *s;
char   *name;
{
    int     i;

    if(!s) {
        return;
    }
    if(!strcmp(s->name, name)) {
        pdetails(s);
        return;
    }
    for(s = s->b; s; s = s->o) {
        pdlevel(s, name);
    }
}

void   pstar(s)
body   *s;
{
    printf("Star %c%d %d       %s\n", s->upp.s.class,
           s->upp.s.decimal, s->upp.s.size, s->name);
}

void   pgiant(s)
body   *s;
{
    if(s->upp.g.size) {
        printf("Large Gas Giant %s\n", s->name);
    } else {
        printf("Small Gas Giant %s\n", s->name);
    }
}

void   ptoids(s)
body   *s;
{
    if(s->p->type == T_GIANT) {
        printf("Ring            %s\n", s->name);
    } else {
        printf("Asteroids       %s\n", s->name);
    }
}

void   pdetails(s)
body   *s;
{
    switch(s->type) {
    case T_STAR:
        pstar(s);
        break;
    case T_GIANT:
        pgiant(s);
        break;
    case T_TOIDS:
        ptoids(s);
        break;
    case T_MAIN:
    case T_WORLD:
        pworld(s);
        break;
    }
}

void   pworld(s)
body   *s;
{
    printf("%c%1x%1x%1x%1x%1x%1x-%x %c%c%c  %c%s",
           s->upp.w.starport, s->upp.w.size,
           s->upp.w.atmos, s->upp.w.hydro, s->upp.w.pop,
           s->upp.w.gov, s->upp.w.law,
           s->upp.w.tech, (s->upp.w.bases&NAVAL_BASE)? 'N' : ' ',
           (s->upp.w.bases&SCOUT_BASE)? 'S' : ' ',
           (s->upp.w.bases&LOCAL_BASE)? 'M' : ' ',
           (s->type == T_MAIN) ? '*' : ' ',
           s->name);
}
