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

void pworld(body *s);
void pdlevel(body *s, char *name);
void print(sistem *syst, char *name);
void pdetails(body *s);

int main(int argc, char **argv)
{
    sistem  syst;
    long    x, y, z;

    if(argc != 5) {
        printf("Usage: detprint X Y Z planetName\n");
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

void print(sistem *syst, char *name)
{
    printf("%s system\n", syst->mw->name);
    printf("Location %ld,%ld,%ld\n", syst->x, syst->y, syst->z);
    pdlevel(syst->sys, name);
}

void pdlevel(body *s, char *name)
{
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

void pstar(body *s)
{
    printf("Star %c%d %d       %s\n", s->upp.s.classification,
           s->upp.s.decimal, s->upp.s.size, s->name);
}

void pgiant(body *s)
{
    if(s->upp.g.size) {
        printf("Large Gas Giant %s\n", s->name);
    } else {
        printf("Small Gas Giant %s\n", s->name);
    }
    printf("Diameter: %gkm Density:%g Surface gravity:%g\n",
           s->det.g.diam, s->det.g.dens,
           s->det.g.dens*s->det.g.diam/12800.0);
    printf("Year:%g ", floor(s->det.g.year));
    if(s->p->type == T_STAR) {
        printf("years");
    } else {
        printf("days");
    }
    printf(" Tilt:%d~", int(s->det.g.tilt));
    if(s->det.w.ecnt > 0.004) {
        printf(" Eccn:%g", s->det.g.ecnt);
    }
    printf("\n");
}

void ptoids(body *s)
{
    if(s->p->type == T_GIANT) {
        printf("Ring            %s\n", s->name);
    } else {
        printf("Asteroids       %s\n", s->name);
    }
    printf("Predominant body diameter is %s.\n", bodsiz[s->det.b.pbod]);
    if(s->det.b.pbod != s->det.b.maxbod) {
        printf("Maximum body diameter is %s.\n", bodsiz[s->det.b.maxbod]);
    }
    printf("Nickel/iron zone:%2d%% Mixed zone:%2d%% Carbonaceous/ice zone:%2d%%\n",
           s->det.b.n, s->det.b.m, s->det.b.c);
    if(s->p->type != T_GIANT) {
        printf("Belt width in %2d.%02d AU\n", s->det.b.wid/100, s->det.b.wid%100);
    }
    printf("Average year :%g ", floor(s->det.b.year));
    if(s->p->type != T_STAR) {
        printf("years\n");
    } else {
        printf("days\n");
    }
}

void pdetails(body *s)
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

void pworld(body *s)
{
    int     i;
    city    *cptr;
    float   rpop, cpop;
    world   *upp;
    dworld  *det;
    tempdata        t;

    det = &s->det.w;
    upp = &s->upp.w;
    printf("Name:%s\t", s->name);
    if(s->type == T_MAIN) {
        printf("\t(mainworld)");
    }
    if(upp->bases&SCOUT_BASE) {
        printf("\tScout base");
    }
    if(upp->bases&NAVAL_BASE) {
        printf("\tNaval base");
    }
    if(upp->bases&LOCAL_BASE) {
        printf("\tLocal base");
    }
    printf("\nStarport: %c     ", upp->starport);
    printf("Atmosphere: %s     ", aatmos[upp->atmos]);
    printf("Water cover: %d%%\n", det->water);
    printf("Government: %s\n", agov[upp->gov]);
    printf("Surface gravity:%g", det->dens*det->diam/12800.0);
    if(det->life == 'Y') {
        printf("          Native life present");
    }
    printf("\n");
    switch(det->core) {
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
    printf(" Diam: %gkm Dens:%g\n", det->diam, det->dens);
    printf("Year:%g ", floor(det->year));
    if(s->p->type == T_STAR) {
        printf("years, ");
    } else {
        printf("days, Solar year:");
        if(s->p->type == T_GIANT) {
            printf("%g years, ", floor(s->p->det.g.year));
        } else {
            printf("%g years, ", floor(s->p->det.w.year));
        }
    }
    printf("Day:%g hours\n", floor(det->day));
    if(det->tilt) {
        printf("Tilt:%d~ ", det->tilt);
    }
    if(det->ecnt > 0.004) {
        printf("Eccntricity:%g ", det->ecnt);
    }
    if(det->stress) {
        printf("Seismic stress:%d ", det->stress);
    }
    printf("\n");
    printf("Total Pop:%g\n", det->tpop);
    rpop = det->tpop;
    if(det->bc) {
        printf("Major cities:\n");
        for(cptr = det->bc; cptr; cptr = cptr->next) {
            printf("\t%s %g\n", cptr->name, cptr->pop);
            rpop -= cptr->pop;
        }
    }
    i = det->pop_2 + det->pop_3 + det->pop_4 + (int)det->pop_5;
    if(i) {
        cpop = 5.0*pow(10.0, (float)(upp->pop - 2));
        printf("\tMeduim large cities (~%g) :%d\n", cpop, det->pop_2);
        rpop -= cpop*(float)det->pop_2;
        cpop /= 10.0;
        printf("\tModerate size cities (~%g) :%d\n", cpop, det->pop_3);
        rpop -= cpop*(float)det->pop_3;
        cpop /= 10.0;
        printf("\tSmall cities (~%g) :%d\n", cpop, det->pop_4);
        rpop -= cpop*(float)det->pop_4;
        cpop /= 10.0;
        printf("\tVery small cities (~%g) :%ld\n", cpop, det->pop_5);
        rpop -= cpop*(float)det->pop_5;
    }
    printf("\tRural population:%g\n", rpop);
    if(upp->pop >= 2) {
        printf("Progressiveness: %s %s\n",
               patt[int(det->prog_att)], pact[int(det->prog_att)]);
        printf("Aggressiveness : %s %s\n",
               aatt[int(det->prog_att)], aact[int(det->prog_att)]);
        printf("Extensiveness  : %s %s\n",
               eglo[int(det->prog_att)], eint[int(det->prog_att)]);
        printf("\nLocal customs:\n");
        for(i = 0; (det->customs[i][0] >= 0) && (i < 6); i++) {
            printf("\t");
            printf(soccust[det->customs[i][0]],
                   pracgroup[det->customs[i][1]],
                   pracgroup[det->customs[i][2]]);
            printf("\n");
        }
        printf("\n");
        printf("Local religion:\n");
        printf("\t            God View: %s\n", agodv[det->godv]);
        printf("\t       Spiritual Aim: %s\n", asaim[det->saim]);
        printf("\t   Devotion Required: %s\n", adreq[det->dreq]);
        printf("\t        Organization: %s\n", arog[det->rorg]);
        printf("\tLiturgical Formality: %s\n", litf[det->litf]);
        printf("\t   Missionary Fervor: %s\n", amiss[det->miss]);
        printf("\n");
        printf("Law Level: %x\n", upp->law);
        printf("\t     Weapons law: %x\n", det->lweap);
        printf("\t       Trade law: %x\n", det->ltrade);
        printf("\t    Criminal law: %x\n", det->lcrim);
        printf("\t       Civil law: %x\n", det->lcivi);
        printf("\tPersonal freedom: %x\n", det->lfree);
        printf("\n");
        printf("Tech Level:\n");
        printf("\t      High Common: %x\n", upp->tech);
        printf("\t       Low Common: %x\n", det->tlow);
        printf("\t           Energy: %x\n", det->tenergy);
        printf("\tCompters/Robotics: %x\n", det->trobot);
        printf("\t   Communications: %x\n", det->tcommo);
        printf("\t          Medical: %x\n", det->tmed);
        printf("\t      Environment: %x\n", det->tenv);
        printf("\t   Land transport: %x\n", det->tland);
        printf("\t  Water transport: %x\n", det->twater);
        printf("\t    Air transport: %x\n", det->tair);
        printf("\t  Space transport: %x\n", det->tspace);
        printf("\tPersonal military: %x\n", det->tpmil);
        printf("\t   Heavy military: %x\n", det->thmil);
        printf("\n");
    }
    filltemp(s, &t);
    printf("Temperatures:\n");
    /*
    printf("Summer:%g Winter:%g Night:%g Day:%g\n",
    t.summer, t.winter, t.night, t.day);
    printf("Hexrow:");
    for (i = 0; i < 11; i++)
    printf("%g ", t.hexrow[i]);
    printf("\n");
    printf("Hextilt:");
    for (i = 0; i < 11; i++)
    printf("%g ", t.hextilt[i]);
    printf("\n");
    */
    printf("\tSummer\t\tWinter\n");
    printf("\tDay\tNight\tDay\tNight\n");
    for(i = 0; i < 11; i++) {
        printf("Row %2d:\t%d\t%d\t%d\t%d\n", i + 1,
               (int)tempat(&t, i, 32+1+4),
               (int)tempat(&t, i, 32+1+8),
               (int)tempat(&t, i, 32+2+4),
               (int)tempat(&t, i, 32+2+8));
    }
}
