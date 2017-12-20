#include <stdio.h>
#include <stdlib.h>
#include "sysgen.h"
#include <math.h>
extern sistem  syst;
extern char    *soccust[];
extern char    *pracgroup[];
extern char    *agodv[];
extern char    *asaim[];
extern char    *adreq[];
extern char    *arog[];
extern char    *litf[];
extern char    *amiss[];
extern char    *bodsiz[];
extern char    *patt[];
extern char    *pact[];
extern char    *aatt[];
extern char    *aact[];
extern char    *eglo[];
extern char    *eint[];
extern char    *aatmos[];
extern char    *agov[];

void pDlevel(body *s);
void filltemp(body *s, tempdata *t);
void   pWorld(body *s);
void Print(sistem *syst);
void   Printw();
void   pDetails();
float tempat(tempdata *t, int row, int mode);
void map(body *s, int plate[20][28], int land[20][28], int cov[20][28]);

void Print(sistem *syst)
{
    pDlevel(syst->sys);
}

void pDlevel(body *s)
{
    if(!s) {
        return;
    }
    if((s->type == T_WORLD) || (s->type == T_MAIN)) {
        if(s->upp.w.pop > 2) {
            pWorld(s);
            printf(".PA\n");
        }
    }
    for(s = s->b; s; s = s->o) {
        pDlevel(s);
    }
}

void   pWorld(body *s)
{
    int     i;
    city    *cptr;
    float   rpop, cpop;
    world   *upp;
    dworld  *det;
    tempdata        t;
    int     plates[20][28];
    int     land[20][28];
    int     cov[20][28];

    printf(".H2\t\t%s\n", s->name);
    printf(".TC#\t\t%s\n", s->name);
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
    printf("Surface gravity:%.1f", det->dens*det->diam/12800.0);
    if(det->life == 'Y') {
        printf(" Native life present");
    }
    printf(" Total Pop:%.0f\n", det->tpop);
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
    printf(" Diam: %gkm Dens:%g ", det->diam, det->dens);
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
    printf("Year:%g ", floor(det->year));
    if(s->p->type == T_STAR) {
        printf("days, ");
    } else {
        printf("days, Solar year:");
        if(s->p->type == T_GIANT) {
            printf("%g days, ", floor(s->p->det.g.year));
        } else {
            printf("%g days, ", floor(s->p->det.w.year));
        }
    }
    printf("Day:%g hours\n", floor(det->day));
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
        if(cpop >= 100.0) {
            printf("\t%5d : Meduim large cities (~%.0f)\n", det->pop_2, cpop);
        }
        rpop -= cpop*(float)det->pop_2;
        cpop /= 10.0;
        if(cpop >= 100.0) {
            printf("\t%5d : Moderate size cities (~%.0f)\n", det->pop_3, cpop);
        }
        rpop -= cpop*(float)det->pop_3;
        cpop /= 10.0;
        if(cpop >= 100.0) {
            printf("\t%5d : Small cities (~%.0f)\n", det->pop_4, cpop);
        }
        rpop -= cpop*(float)det->pop_4;
        cpop /= 10.0;
        if(cpop >= 100.0) {
            printf("\t%5ld : Very small cities (~%.0f)\n", det->pop_5, cpop);
        }
        rpop -= cpop*(float)det->pop_5;
    }
    printf("\t%.0f Rural population\n", rpop);
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
        printf(".CP8\n");
        printf("Local religion:\n");
        printf("            God View: %s\n", agodv[det->godv]);
        printf("       Spiritual Aim: %s\n", asaim[det->saim]);
        printf("   Devotion Required: %s\n", adreq[det->dreq]);
        printf("        Organization: %s\n", arog[det->rorg]);
        printf("Liturgical Formality: %s\n", litf[det->litf]);
        printf("   Missionary Fervor: %s\n", amiss[det->miss]);
        printf("\n");
        printf(".CP8\n");
        printf("Law Level: %x\n", upp->law);
        printf("\t     Weapons law: %x\n", det->lweap);
        printf("\t       Trade law: %x\n", det->ltrade);
        printf("\t    Criminal law: %x\n", det->lcrim);
        printf("\t       Civil law: %x\n", det->lcivi);
        printf("\tPersonal freedom: %x\n", det->lfree);
        printf("\n");
        printf(".CP14\n");
        printf("Tech Level:\n");
        printf("\t      High Common: %x\t", upp->tech);
        printf("\t       Low Common: %x\n", det->tlow);
        printf("\t           Energy: %x\t", det->tenergy);
        printf("\tCompters/Robotics: %x\n", det->trobot);
        printf("\t   Communications: %x\t", det->tcommo);
        printf("\t          Medical: %x\n", det->tmed);
        printf("\t      Environment: %x\n", det->tenv);
        printf("\t   Land transport: %x\t", det->tland);
        printf("\t  Water transport: %x\n", det->twater);
        printf("\t    Air transport: %x\t", det->tair);
        printf("\t  Space transport: %x\n", det->tspace);
        printf("\tPersonal military: %x\t", det->tpmil);
        printf("\t   Heavy military: %x\n", det->thmil);
        printf("\n");
    }
    filltemp(s, &t);
    printf(".CP15\n");
    printf("Temperatures:\n");
    printf("\tSummer\t\tWinter\n");
    printf("\tDay\tNight\tDay\tNight\n");
    for(i = 0; i < 11; i++) {
        printf("Row %2d:\t%d\t%d\t%d\t%d\n", i + 1,
               (int)tempat(&t, i, 32+1+4),
               (int)tempat(&t, i, 32+1+8),
               (int)tempat(&t, i, 32+2+4),
               (int)tempat(&t, i, 32+2+8));
    }
    printf("\n\n.CP19\n");
    printf("Surface coverage:\n");
    map(s, plates, land, cov);
    printf(".OJoff\n");
    Printw(cov, 2);
    printf(".OJon\n");
}

extern char    *acover[16];

void p1(int val, int type)
{
    if(type == 0) {
        printf("%02d", val);
    } else if(type == 1) {
        switch(val) {
        case 0:
            printf(".");
            break;
        case 1:
            printf("L");
            break;
        case 2:
            printf("l");
            break;
        case -1:
            printf("W");
            break;
        case -2:
            printf("w");
            break;
        }
    } else if(type == 2) {
        if(val < 0) {
            printf("?");
        } else {
            printf("%.1s", acover[val%16]);
        }
    }
}
