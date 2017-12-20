#include <stdio.h>
#include <stdlib.h>

#include "sysgen.h"

void pworld(body *s);
void plevel(body *s, int depth);
void   printsyst(sistem *syst);
int makesystem(sistem *syst, long x, long y, long z);

int main(int argc, char **argv)
{
    sistem  syst;
    long    x, y, z;

    if ((argc < 3) || (argc > 4)) {
        printf("Usage: sysgen X Y [Z]\n");
        exit(1);
    }
    x = atol(argv[1]);
    y = atol(argv[2]);
    if (argc == 4)
        z = atol(argv[3]);
    else
        z = 0L;

    if (makesystem(&syst, x, y, z) == 0)
        exit(0);
    printsyst(&syst);
    exit(1);
}

int sysgen2Main(long x, long y, long z)
{
    sistem  syst;

    if(makesystem(&syst, x, y, z) == 0) {
        return(0);
    }
    printsyst(&syst);
    return(1);
}

void   printsyst(sistem *syst)
{
    printf("%s system\n", syst->mw->name);
    printf("Location %ld,%ld,%ld\n", syst->x, syst->y, syst->z);
    plevel(syst->sys, 0);
}

void plevel(body *s, int depth)
{
    int     i;

    if(!s) {
        return;
    }
    for(i = 0; i < depth; i++) {
        if(i+1 == depth) {
            printf("%d", s->orbit);
        }
        printf("\t");
    }
    pworld(s);
    for(s = s->b; s; s = s->o) {
        plevel(s, depth + 1);
    }
}

void pworld(body *s)
{
    switch(s->type) {
    case T_STAR:
        printf("Star %c%d %d       %s\n", s->upp.s.classification,
               s->upp.s.decimal, s->upp.s.size, s->name);
        break;
    case T_GIANT:
        if(s->upp.g.size) {
            printf("Large Gas Giant %s\n", s->name);
        } else {
            printf("Small Gas Giant %s\n", s->name);
        }
        break;
    case T_TOIDS:
        if(s->p->type == T_GIANT) {
            printf("Ring            %s\n", s->name);
        } else {
            printf("Asteroids       %s\n", s->name);
        }
        break;
    case T_MAIN:
    case T_WORLD:
        printf("%c%1x%1x%1x%1x%1x%1x-%x %c%c%c  %c%s\n",
               s->upp.w.starport, s->upp.w.size,
               s->upp.w.atmos, s->upp.w.hydro, s->upp.w.pop, s->upp.w.gov, s->upp.w.law,
               s->upp.w.tech, (s->upp.w.bases&NAVAL_BASE)? 'N' : ' ',
               (s->upp.w.bases&SCOUT_BASE)? 'S' : ' ',
               (s->upp.w.bases&LOCAL_BASE)? 'M' : ' ',
               (s->type == T_MAIN) ? '*' : ' ',
               s->name);
        break;
    }
}
