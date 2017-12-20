#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysgen.h"
#include "stelfun.h"
#include "system.h"
#include "populate.h"
#include "details.h"

#include "custom.h"
#include <math.h>
#ifdef min
#undef min
#endif

void pworld(body *s);
void pdlevel(body *s, int depth);
void printdetails(sistem *syst);
void pdetails(body *s, int depth);

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

    if (makedetails(&syst, x, y, z) == 0)
        exit(0);
    printdetails(&syst);
    exit(1);
}

int sysgen3Main(long x, long y, long z)
{
    sistem  syst;

    if(makedetails(&syst, x, y, z) == 0) {
        return(0);
    }
    printdetails(&syst);
    return(1);
}

void printdetails(sistem *syst)
{
    printf("%s system\n", syst->mw->name);
    printf("Location %ld,%ld,%ld\n", syst->x, syst->y, syst->z);
    pdlevel(syst->sys, 0);
}

void tab(int n)
{
    while(n-- > 0) {
        printf("\t");
    }
}

void pdlevel(body *s, int depth)
{
    if(!s) {
        return;
    }
    tab(depth - 1);
    printf("%d\t", s->orbit);
    pworld(s);
    tab(depth);
    if(s->type != T_STAR) {
        pdetails(s, depth);
    }
    for(s = s->b; s; s = s->o) {
        pdlevel(s, depth + 1);
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

void pdetails(body *s, int depth)
{
    int     i;
    city    *cptr;

    switch(s->type) {
    case T_GIANT:
        printf("Diam: %gkm Dens:%g Grav:%g\n", s->det.g.diam, s->det.g.dens,
               s->det.g.dens*s->det.g.diam/12800.0);
        tab(depth);
        printf("Year:%g", floor(s->det.g.year));
        printf(" Tilt:%d~", int(s->det.g.tilt));
        if(s->det.g.ecnt > 0.004) {
            printf(" Eccn:%g", s->det.g.ecnt);
        }
        printf("\n");
        break;
    case T_TOIDS:
        printf("%s", bodsiz[s->det.b.pbod]);
        if(s->det.b.pbod != s->det.b.maxbod) {
            printf("/%s", bodsiz[s->det.b.maxbod]);
        }
        printf(" n-%2d m-%2d c-%2d", s->det.b.n, s->det.b.m, s->det.b.c);
        printf(" %2d.%02d AU\n", s->det.b.wid/100, s->det.b.wid%100);
        tab(depth);
        printf("Year:%g\n", floor(s->det.b.year));
        break;
    case T_MAIN:
    case T_WORLD:
        printf("Grav:%g", s->det.w.dens*s->det.w.diam/12800.0);
        printf(" Temp:%.1f ", s->det.w.temp - 273.0);
        if(s->det.w.life == 'Y') {
            printf(" Life");
        }
        printf("\n");
        tab(depth);
        printf("Year:%g Day:%g", floor(s->det.w.year),
               floor(s->det.w.day));
        if(s->det.w.tilt) {
            printf(" Tilt:%d~", s->det.w.tilt);
        }
        if(s->det.w.ecnt > 0.004) {
            printf(" Eccn:%g", s->det.w.ecnt);
        }
        if(s->det.w.stress) {
            printf(" Stress:%d", s->det.w.stress);
        }
        printf("\n");
        tab(depth);
        i = s->det.w.maj + s->det.w.min + s->det.w.isl
            + s->det.w.arch;
        if(s->det.w.water >= 50) {
            if(s->det.w.maj) {
                printf("%d Major continents ", s->det.w.maj);
            }
            if(s->det.w.min) {
                printf("%d Minor continents ", s->det.w.min);
            }
            if(s->det.w.isl) {
                printf("%d islands ", s->det.w.isl);
            }
            if(s->det.w.arch) {
                printf("%d arch", s->det.w.arch);
            }
        } else {
            if(s->det.w.maj) {
                printf("%d Major oceans ", s->det.w.maj);
            }
            if(s->det.w.min) {
                printf("%d Minor oceans ", s->det.w.min);
            }
            if(s->det.w.isl) {
                printf("%d seas ", s->det.w.isl);
            }
            if(s->det.w.arch) {
                printf("%d lakes", s->det.w.arch);
            }
        }
        if(i) {
            printf("\n");
            tab(depth);
        }
        for(cptr = s->det.w.bc; cptr; cptr = cptr->next) {
            printf("%s %g\n", cptr->name, cptr->pop);
            tab(depth);
        }
        if(s->det.w.tpop >= 10.0) {
            printf("Total Pop:%g ", s->det.w.tpop);
            i = s->det.w.pop_2 + s->det.w.pop_3 +
                s->det.w.pop_4 + (int)s->det.w.pop_5;
            if(i)
                printf("ML:%d MS:%d SM:%d VS:%ld",
                       s->det.w.pop_2, s->det.w.pop_3,
                       s->det.w.pop_4, s->det.w.pop_5);
            printf("\n");
            tab(depth);
        }
        if(s->upp.w.pop >= 2) {
            printf("Progressiveness: %s %s\n",
                   patt[int(s->det.w.prog_att)], pact[int(s->det.w.prog_att)]);
            tab(depth);
            printf("Aggressiveness: %s %s\n",
                   aatt[int(s->det.w.prog_att)], aact[int(s->det.w.prog_att)]);
            tab(depth);
            printf("Extensiveness: %s %s\n",
                   eglo[int(s->det.w.prog_att)], eint[int(s->det.w.prog_att)]);
            tab(depth);
            for(i = 0; (s->det.w.customs[i][0] >= 0) && (i < 6); i++) {
                printf(soccust[s->det.w.customs[i][0]],
                       pracgroup[s->det.w.customs[i][1]],
                       pracgroup[s->det.w.customs[i][2]]);
                printf("\n");
                tab(depth);
            }
            printf("Legal: %x-%x%x%x%x%x",
                   s->upp.w.law, s->det.w.lweap,
                   s->det.w.ltrade, s->det.w.lcrim,
                   s->det.w.lcivi, s->det.w.lfree);
            printf("\n");
            tab(depth);
            printf("Tech:%x%x-%x%x%x%x%x-%x%x%x%x-%x%x",
                   s->upp.w.tech,
                   s->det.w.tlow, s->det.w.tenergy,
                   s->det.w.trobot, s->det.w.tcommo,
                   s->det.w.tmed, s->det.w.tenv,
                   s->det.w.tland, s->det.w.twater,
                   s->det.w.tair, s->det.w.tspace,
                   s->det.w.tpmil, s->det.w.thmil);
            printf("\n");
            tab(depth);
        }
        switch(s->det.w.core) {
        case 'h':
            printf("Heavy core");
            break;
        case 'm':
            printf("Molten core");
            break;
        case 'r':
            printf("Rocky body");
            break;
        case 'i':
            printf("Icy body");
            break;
        }
        printf(" Diam: %gkm Dens:%g\n", s->det.w.diam, s->det.w.dens);
        break;
    }
}
