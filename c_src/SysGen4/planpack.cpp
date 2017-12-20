#include <stdio.h>
#include <stdlib.h>
#include "sysgen.h"
#include "custom.h"
#include <math.h>

void Print(sistem *syst);
void printdetails(sistem *syst);
void pworld(body *s);
void pdetails(body *s, int depth);
void Pdlevel(body *s, int depth);

sistem syst;
extern int     dice_rolls;
int makedetails(sistem *syst, long x, long y, long z);

int main(int argc, char **argv) {
    long    x, y, z;

    if((argc < 3) || (argc > 4)) {
        printf("Usage: sysgen X Y [Z]\n");
        exit(1);
    }
    x = atol(argv[1]);
    y = atol(argv[2]);
    if(argc == 4) {
        z = atol(argv[3]);
    } else {
        z = 0L;
    }

    if(makedetails(&syst, x, y, z) == 0) {
        exit(0);
    }
    printf(".H1 %s system", syst.mw->name);
    printf("     (Location %ld,%ld,%ld)\n", syst.x, syst.y, syst.z);
    printdetails(&syst);
    printf(".PA\n");
    Print(&syst);
    ttimes();
    printf(".. Total number of rolls = %d\n", dice_rolls);
    exit(1);
}

void printdetails(sistem *syst) {
    printf(".H2 %s system overview\n", syst->mw->name);
    Pdlevel(syst->sys, 0);
}

void tab(int n) {
    while(n-- > 0) {
        printf("\t");
    }
}

void Pdlevel(body *s, int depth) {
    if(!s) {
        return;
    }
    printf(".CP 8\n");
    if(depth) {
        printf("\n");
        tab(depth - 1);
        printf("%d\t", s->orbit);
    }
    pworld(s);
    tab(depth);
    if(s->type != T_STAR) {
        pdetails(s, depth);
    }
    for(s = s->b; s; s = s->o) {
        Pdlevel(s, depth + 1);
    }
}

void   pworld(body *s) {
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
               s->upp.w.atmos, s->upp.w.hydro, s->upp.w.pop,
               s->upp.w.gov, s->upp.w.law,
               s->upp.w.tech, (s->upp.w.bases&NAVAL_BASE)? 'N' : ' ',
               (s->upp.w.bases&SCOUT_BASE)? 'S' : ' ',
               (s->upp.w.bases&LOCAL_BASE)? 'M' : ' ',
               (s->type == T_MAIN) ? '*' : ' ',
               s->name);
        break;
    }
}

void pdetails(body *s, int depth) {
    switch(s->type) {
    case T_GIANT:
        printf("Diam: %gkm Dens:%g Grav:%.1f\n", s->det.g.diam,
               s->det.g.dens, s->det.g.dens*s->det.g.diam/12800.0);
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
        printf("Grav:%.1f", s->det.w.dens*s->det.w.diam/12800.0);
        printf(" Temp:%.1f ", s->det.w.temp - 273.0);
        if(s->det.w.life == 'Y') {
            printf(" Life");
        }
        if(s->det.w.tpop >= 10.0) {
            printf(" Total Pop:%.0f ", s->det.w.tpop);
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
            printf(" Seismic Stress:%d", s->det.w.stress);
        }
        printf("\n");
        tab(depth);
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
        if(s->upp.w.pop > 2) {
            tab(depth);
            printf("(see details)\n");
        }
        break;
    }
}

void ttimes() {
    body    *i, *j;
    float   tmp, man;

    printf(".H2 Travel Times\n");
    printf(".TC#\t\tTravel Times\n");
    for(man = 1.0; man <= 6.0; man++) {
        printf(".CP10\n");
        printf("Maneuver %d Travel Times:\n", (int)man);
        for(i = syst.sys->b; i; i = i->o) {
            printf("| ");
            for(j = syst.sys->b; j != i; j = j->o) {
                tmp = odist[i->orbit] - odist[j->orbit];
                tmp = sqrt(tmp*149600.0/32.4/man);
                if(tmp < 24.0) {
                    printf("%4.1fh | ", tmp);
                } else {
                    printf("%4.1fd | ", tmp/24.0);
                }
            }
            printf("%s\n", i->name);
        }
        printf("\n\n");
    }
}
