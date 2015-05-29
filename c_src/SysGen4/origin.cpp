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
#include <stdio.h>
#include <time.h>

/*#define DEBUG*/
void findnth(float ff);
int main(int argc, char **argv);
float gpop(body *sys);
void census(long x, long y, long z);
float fRand(void);
body *findpla(body *sys);
void pworld(body *s);
void sys2free(sistem *syst);
void sys3free(sistem *sys);

mainworld      mw;
sistem syst;
struct node {
    long    x, y, z;
    float   pop;
    struct node     *next;
};
struct node    *slist;
float  tpop, fRand(), target;
float  popv[20] = {5e00, 5e01, 5e02, 5e03, 5e04, 5e05, 5e06, 5e07,
                   5e08, 5e09, 5e10, 5e11, 5e12, 5e13, 5e14, 5e15,
                   5e16, 5e17, 5e18, 5e19
                  };

void   pworld();

void findnth(float ff)
{
    struct node     *n;
    body            *sys;
    city            *c;

    for(n = slist; n; n = n->next) {
        ff -= n->pop;
        if(ff < 0.0) {
            break;
        }
    }
    if(!n) {
        printf("Problem.\n");
        return;
    }
    makedetails(&syst, n->x, n->y, n->z);
    target = gpop(syst.sys)*fRand();
    sys = findpla(syst.sys);
    target += sys->det.w.tpop;
    for(c = sys->det.w.bc; c; c = c->next) {
        target -= c->pop;
        if(target < 0.0) {
            break;
        }
    }
    printf("%5ld,%5ld,%5ld %s", n->x, n->y, n->z, sys->name);
    if(c) {
        printf("/%s", c->name);
    }
    sys3free(&syst);
    printf("\n");
}

int main(int argc, char **argv)
{
    long    x, y, z;
    int     count;

    if(argc > 1) {
        x = atol(argv[1]);
    }
    else {
        x = 10000L;
    }
    if(argc > 2) {
        y = atol(argv[2]);
    }
    else {
        y = 10000L;
    }
    if(argc > 3) {
        z = atol(argv[3]);
    }
    else {
        z = 10000L;
    }
    if(argc > 4) {
        count = atoi(argv[4]);
    }
    else {
        count = 10;
    }
    slist = 0;
    tpop = 0.0;
    fprintf(stderr, "Counting");
    census(x, y, z);
    printf("\nPopulation base: %g\n", tpop);
    while(count--) {
        findnth((float)(tpop*fRand()));
    }
}


float gpop(body *sys)
{
    float   t;
    body    *s;

    t = 0.0;
    for(s = sys->b; s; s = s->o) {
        if((s->type == T_MAIN) || (s->type == T_WORLD)) {

            if(s->upp.w.pop) {
                t += popv[s->upp.w.pop];
            }
        }
        if(s->b) {
            t += gpop(s);
        }
    }
    return(t);
}

void census(long x, long y, long z)
{
    long    A, B, C;
    struct node     *n;
    float   spop;

    /* survey all within 5 */
    x += 5L;
    y += 5L;
    z += 5L;
    for(A = x - 10; A <= x; A++)
        for(B = y - 10; B <= y; B++)
            for(C = z - 10; C <= z; C++) {
#ifdef DEBUG
                printf("(%ld,%ld,%ld)\n", A, B, C);
#endif
                if(makesystem(&syst, A, B, C)) {
                    spop = gpop(syst.sys);
                    sys2free(&syst);
                    n = (struct node *)malloc(sizeof(struct
                                                     node));
                    fprintf(stderr, ".");
                    n->x = A;
                    n->y = B;
                    n->z = C;
                    n->pop = spop;
                    tpop += n->pop;
                    n->next = slist;
                    slist = n;
                }
            }
}

float  fRand()
{
    long    t;
    unsigned        u;

    time(&t);
    sRand(t);
    u = Rand();
    return((float)u/65536.0);
}

body *findpla(body *sys)
{
    body    *s, *sptr;

    if((sys->type == T_MAIN) || (sys->type == T_WORLD)) {
        target -= sys->det.w.tpop;
        if(target < 0.0) {
            return(sys);
        }
    }
    for(sptr = sys->b; sptr; sptr = sptr->o) {
        s = findpla(sptr);
        if(s) {
            return(s);
        }
    }
    return(0);
}

void pworld(body *s)
{
    int     i;

    printf("\nGrav:%g", s->det.w.dens*s->det.w.diam/12800.0);
    printf(" Temp:%.1f ", s->det.w.temp - 273.0);
    if(s->det.w.life == 'Y') {
        printf(" Life");
    }
    printf("\n");
    if(s->upp.w.pop >= 2) {
        printf("Progressiveness: %s %s\n",
               patt[int(s->det.w.prog_att)], pact[int(s->det.w.prog_att)]);
        printf("Aggressiveness: %s %s\n",
               aatt[int(s->det.w.prog_att)], aact[int(s->det.w.prog_att)]);
        printf("Extensiveness: %s %s\n",
               eglo[int(s->det.w.prog_att)], eint[int(s->det.w.prog_att)]);
        for(i = 0; (s->det.w.customs[i][0] >= 0) && (i < 6); i++) {
            printf(soccust[s->det.w.customs[i][0]],
                   pracgroup[s->det.w.customs[i][1]],
                   pracgroup[s->det.w.customs[i][2]]);
            printf("\n");
        }
        printf("Legal: %x-%x%x%x%x%x\n",
               s->upp.w.law, s->det.w.lweap,
               s->det.w.ltrade, s->det.w.lcrim,
               s->det.w.lcivi, s->det.w.lfree);
    }
}
