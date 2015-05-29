#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysgen.h"
#include "stelfun.h"
#include "system.h"
#include "populate.h"
#include "details.h"
#include "detfun.h"

#include <math.h>
#ifdef min
#undef min
#endif

int makedetails(sistem *syst, long x, long y, long z)
{
    if(!makesystem(syst, x, y, z)) {
        return(0);
    }

    detail(syst->sys, 0, x, y, z);
    return(1);
}

float  odist[] = {
    0.2, 0.4, 0.7, 1.0, 1.6, 2.8, 5.2, 10.0, 19.6, 38.8, 77.2,
    154.0, 307.6, 614.8, 1229.2, 2458.0, 4915.6, 9830.8, 19661.2, 39322.0
};
float  smass[8][13] = {
    {
        60.0, 30.0, 18.0, 15.0, 13.0, 12.0, 12.0, 13.0, 14.0, 18.0, 20.0, 25.0,
        30.0
    },
    {
        50.0, 25.0, 16.0, 13.0, 12.0, 10.0, 10.0, 12.0, 13.0, 16.0, 16.0, 20.0,
        25.0
    },
    {30.0, 20.0, 14.0, 11.0, 10.0, 8.1, 8.1, 10.0, 11.0, 14.0, 14.0, 16.0, 18.0},
    {25.0, 15.0, 12.0, 9.0, 8.0, 5.0, 2.5, 3.2, 4.0, 5.0, 6.3, 7.4, 9.2},
    {20.0, 10.0, 6.0, 4.0, 2.5, 2.0, 1.75, 2.0, 2.3, 2.3, 2.3, 2.3, 2.3},
    {18.0, 6.5, 3.2, 2.1, 1.7, 1.3, 1.04, 0.94, 0.825, 0.570, 0.489, 0.331, 0.215},
    {0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.6, 0.528, 0.430, 0.330, 0.154, 0.104, 0.058},
    {0.26, 0.26, 0.36, 0.36, 0.42, 0.42, 0.63, 0.63, 0.83, 0.83, 1.11, 1.11}
};
float  slumen[8][13] = {
    {27.36, 21.25, 18.09, 16.87, 15.72, 15.03, 16.09, 17.27, 17.65, 18.09, 18.49, 18.95, 19.38},
    {22.80, 14.70, 11.07, 10.40, 9.27, 8.45, 8.84, 9.49, 10.40, 11.95, 14.65, 17.27, 18.49},
    {20.31, 11.68, 6.85, 5.40, 4.95, 4.75, 4.86, 5.22, 5.46, 7.04, 8.24, 11.05, 11.28},
    {18.09, 9.05, 4.09, 3.08, 2.70, 2.56, 2.66, 2.94, 3.12, 4.23, 4.66, 6.91, 7.20},
    {16.87, 6.69, 3.53, 2.47, 2.09, 1.86, 1.6, 1.49, 1.47, 1.47, 1.47, 1.47, 1.47},
    {15.38, 6.12, 3.08, 2.0, 1.69, 1.37, 1.05, 0.9, 0.81, 0.53, 0.45, 0.29, 0.18},
    {0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.75, 0.66, 0.58, 0.40, 0.32, 0.21, 0.09},
    {0.46, 0.46, 0.27, 0.27, 0.13, 0.09, 0.07, 0.07, 0.07}
};
float  ofact[20] = {
    836.345, 591.385, 447.045, 374.025, 295.693, 223.523, 164.021, 118.277,
    84.484, 60.046, 42.569, 30.14, 21.326, 15.085, 10.668, 7.544, 5.335, 3.772, 2.667, 1.886
};

void detail(body *s, int konst,long x, long y, long z)
{
    body    *sptr;
    int     roll;
    float   tmp;
    char    zo;

    xyzmagic(x, y, z, (long)konst*DETAIL_MAGIC);
    if(s->p) {
        if(s->p->type == T_STAR) {
            zo = zone(s->p, s->orbit);
        }
        else {
            zo = zone(s->p->p, s->p->orbit);
        }
        /* compute year */
        if(s->p->type == T_STAR) {
            tmp = smass[s->p->upp.s.size][staroff(s->p)];
            if((s->p->b->type == T_STAR) && (s->p->b->orbit == 0)
               && (s->orbit != 0)) { /* i.e. a close binary */
                tmp += smass[s->p->b->upp.s.size][staroff(s->p->b)];
            }
    		int dist;
    		if(s->orbit > 20)
    			dist = 0;
            else
            	dist = odist[s->orbit];
            tmp = dist*dist*dist/tmp;
            tmp = sqrt(tmp)*365.25;
        } else {
            if(s->p->type == T_GIANT) {
                tmp = s->p->det.g.dens;
            }
            else {
                tmp = s->p->det.w.dens;
            }
            tmp *= (float)(s->orbit);
            tmp *= (float)(s->orbit);
            tmp *= (float)(s->orbit);
            tmp = (float)sqrt((double)tmp);
        }
    }
    switch(s->type) {
    case T_GIANT:
        s->det.g.year = tmp;
        detgiant(s, zo);
        break;
    case T_MAIN:
    case T_WORLD:
        s->det.w.year = tmp;
        detworld(s, zo);
        break;
    case T_TOIDS:
        s->det.b.year = tmp;
        dettoids(s, zo);
        break;
    }
    roll = 0;
    for(sptr = s->b; sptr; sptr = sptr->o) {
        detail(sptr, konst*20 + roll++, x, y, z);
    }
}

/* asteroid belt zones */
int    nzone[11][3] = {
    {40, 30, 30},
    {40, 40, 20},
    {40, 40, 20},
    {40, 40, 20},
    {40, 40, 20},
    {50, 40, 10},
    {50, 40, 10},
    {50, 40, 10},
    {50, 30, 20},
    {60, 30, 10},
    {60, 40, 0}
};
int    mzone[11][3] = {
    {20, 50, 30},
    {30, 50, 20},
    {20, 60, 20},
    {20, 60, 20},
    {30, 60, 10},
    {20, 70, 10},
    {10, 70, 20},
    {10, 80, 10},
    {10, 80, 10},
    {0, 80, 20},
    {0, 90, 10}
};
int    czone[11][3] = {
    {20, 30, 50},
    {20, 30, 50},
    {20, 30, 50},
    {10, 30, 60},
    {10, 30, 60},
    {10, 20, 70},
    {10, 20, 70},
    {10, 10, 80},
    {0, 10, 90},
    {0, 10, 90},
    {0, 20, 80}
};

void dettoids(body *s, char zo)
{
    int     roll, tmp;

    s->det.b.pbod = D(2) - 2;
    if(s->det.b.pbod == 9) {
        s->det.b.pbod = 10;
    }
    if(s->det.b.pbod == 10) {
        s->det.b.pbod = 12;
    }
    roll = D(1);
    switch(roll) {
    case 1:
    case 2:
        s->det.b.maxbod = s->det.b.pbod;
        break;
    case 3:
        s->det.b.maxbod = 7;
        break;
    case 4:
        s->det.b.maxbod = 9;
        break;
    case 5:
        s->det.b.maxbod = 11;
        break;
    case 6:
        s->det.b.maxbod = 13;
        break;
    }
    roll = D(2);
    if(zo == 'I') {
        roll -= 4;
    }
    else if(zo == 'O') {
        roll += 2;
    }
    tmp = D(2) - 2;
    if(roll < 5) {
        s->det.b.n = nzone[tmp][0];
        s->det.b.m = nzone[tmp][1];
        s->det.b.c = nzone[tmp][2];
    }
    else if(roll > 8) {
        s->det.b.n = czone[tmp][0];
        s->det.b.m = czone[tmp][1];
        s->det.b.c = czone[tmp][2];
    }
    else {
        s->det.b.n = mzone[tmp][0];
        s->det.b.m = mzone[tmp][1];
        s->det.b.c = mzone[tmp][2];
    }
    if(s->p->type == T_STAR) {
        tmp = s->orbit;
    }
    else {
        tmp = s->p->orbit;
    }
    roll = D(2);
    if(tmp <= 4) {
        roll -= 3;
    }
    else if(tmp <= 8) {
        roll--;
    }
    else if(tmp <= 12) {
        roll++;
    }
    else {
        roll += 2;
    }
    switch(roll) {
    case -1:
    case 0:
    case 1:
    case 2:
        s->det.b.wid = 1;
        break;
    case 3:
        s->det.b.wid = 5;
        break;
    case 4:
    case 5:
        s->det.b.wid = 10;
        break;
    case 6:
    case 7:
        s->det.b.wid = 50;
        break;
    case 8:
        s->det.b.wid = 100;
        break;
    case 9:
        s->det.b.wid = 150;
        break;
    case 10:
        s->det.b.wid = 200;
        break;
    case 11:
        s->det.b.wid = 500;
        break;
    case 12:
    case 13:
    case 14:
        s->det.b.wid = 1000;
        break;
    }
}

int    sgiant[11] = { 20, 30, 40, 50, 60, 70, 80, 80, 90, 100 };
int    lgiant[16] = { 110, 120, 130, 140, 150, 150, 160, 170, 180,
                      190, 200, 210, 220, 230, 240, 300
                    };
int    gdens[16] = { 10, 11, 12, 13, 14, 16, 18, 20, 22, 23, 24, 26, 27, 28, 29, 30 };

void detgiant(body *s, char zo)
{
    int     roll, tmp;

    roll = D(2) - 2;
    if(s->upp.g.size) {
        tmp = lgiant[roll];
    }
    else {
        tmp = sgiant[roll];
    }
    do {
        roll = D(2) - 7;
    }
    while(roll == 5);
    tmp += roll;
    s->det.g.diam = (float)tmp*(1000.0*1.6);
    s->det.g.dens = (float)gdens[D(3) - 3]/100.0;
    /* tilt */
    s->det.w.tilt = roltilt();
    /* eccentricity */
    s->det.w.ecnt = roleccn();
}

int    hdens[16] = { 110, 115, 120, 125, 130, 135, 140, 145, 150, 155,
                     160, 170, 180, 190, 200, 225
                   };
int    mdens[16] = { 82, 84, 86, 88, 90, 92, 94, 96, 98, 100, 102, 104,
                     106, 108, 110, 112
                   };
int    rdens[16] = { 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74,
                     76, 78, 80
                   };
int    idens[16] = { 18, 20, 22, 24, 26, 28, 30, 32, 35, 36, 38, 40, 42,
                     44, 46, 48
                   };
/* energy adsorbtion */
int    ads[8][11] = {
    {800, 744, 736, 752, 738, 753, 767, 782, 796, 810, 818},
    {900, 829, 803, 811, 782, 789, 795, 802, 808, 814, 818},
    {800, 811, 789, 799, 774, 747, 718, 687, 654, 619, 619},
    {900, 900, 860, 860, 820, 780, 740, 700, 660, 620, 619},
    {680, 646, 635, 644, 625, 599, 570, 537, 500, 500, 500},
    {740, 697, 672, 676, 648, 613, 577, 539, 500, 500, 500},
    {800, 811, 807, 817, 813, 809, 805, 800, 794, 787, 773},
    {900, 900, 882, 883, 866, 850, 836, 821, 807, 793, 773}
};

void detworld(body *s, char zo)
{
    char    c;
    int     roll, tmp, u, l;
    float   mass, ftmp, tpop, fpop;
    body    *sptr;
    city    *cptr;
    world   *upp;
    dworld  *det;

    det = &s->det.w;
    upp = &s->upp.w;
    /* diameter */
    do {
        roll = D(2) - 7;
    }
    while(roll == 5);
    if(upp->size) {
        roll += upp->size*10;
    }
    else {
        roll += 6;
    }
    det->diam = (float)roll*(100.0*1.6);
    /* core/density */
    roll = D(2);
    if(upp->size <= 4) {
        roll++;
    }
    else if(upp->size >= 6) {
        roll -= 2;
    }
    if(upp->atmos <= 3) {
        roll++;
    }
    else if(upp->atmos >= 6) {
        roll -= 2;
    }
    if(zo == 'O') {
        roll += 6;
    }
    tmp = D(3) - 3;
    if(roll <= 1) {
        det->core = 'h';
        tmp = hdens[tmp];
    }
    else if(roll <= 10) {
        det->core = 'm';
        tmp = mdens[tmp];
    }
    else if(roll <= 14) {
        det->core = 'r';
        tmp = rdens[tmp];
    }
    else {
        det->core = 'i';
        tmp = idens[tmp];
    }
    det->dens = (float)tmp/100.0;
    /* day */
    if(s->p->type == T_STAR) {
        ftmp = smass[s->p->upp.s.size][staroff(s->p)]/odist[s->orbit];
    }
    else
        ftmp = det->dens*det->diam*det->diam
               *(400000.0/ (12800.0*12800.0*12800.0))/(float)s->orbit;
    ftmp += 5.0 + (float)(D(2) - 2)*4.0;
    if(ftmp > 40.0)
        switch(D(2)) {
        case 2:
            ftmp = -(float)D(1)*10.0;
            break;
        case 3:
            ftmp = (float)D(1)*20.0;
            break;
        case 4:
        case 10:
            ftmp = (float)D(1)*10.0;
            break;
        case 5:
        case 9:
            break;
        case 6:
        case 7:
        case 8:
            ftmp = det->year;
            break;
        case 11:
            ftmp = (float)D(1)*50.0;
            break;
        case 12:
            ftmp = -(float)D(1)*50.0;
            break;
        }
    det->day = ftmp;
    /* tilt */
    det->tilt = roltilt();
    /* eccentricity */
    det->ecnt = roleccn();
    /* stress */
    ftmp = (float)(D(1) - 3);
    if(det->core == 'h') {
        ftmp += (float)(D(1) - 2);
    }
    else if(det->core == 'm') {
        ftmp += (float)(D(1) - 3);
    }
    for(sptr = s->b; sptr; sptr = sptr->o) {
        ftmp += (float)sptr->upp.w.size/(float)sptr->orbit*(1600.0/64.0);
    }
    if(s->p->type == T_STAR) {
        ftmp += smass[s->p->upp.s.size][staroff(s->p)]/odist[s->orbit];
    }
    else {
        ftmp += smass[s->p->p->upp.s.size][staroff(s->p->p)]/odist[s->p->orbit];
    }
    det->stress = (int)(floor(ftmp + 0.5));
    if(det->stress < 0) {
        det->stress = 0;
    }
    /* base temp */
    if(s->p->type == T_STAR) {
        ftmp = slumen[s->p->upp.s.size][staroff(s->p)];
        ftmp *= ofact[s->orbit];
    }
    else {
        ftmp = slumen[s->p->p->upp.s.size][staroff(s->p->p)];
        ftmp *= ofact[s->p->orbit];
    }
    if(upp->atmos < 4) {
        roll = 0;
    }
    else if(upp->atmos < 0x0a) {
        roll = 2;
    }
    else if(upp->atmos == 0x0e) {
        roll = 8;
    }
    else {
        roll = 6;
    }
    if(zo == 'H') {
        roll++;
    }
    ftmp *= (float)ads[roll][upp->hydro]/1000.0;
    switch(upp->atmos) {
    case 4:
    case 5:
        ftmp *= 1.05;
        break;
    case 6:
    case 7:
    case 0x0e:
        ftmp *= 1.1;
        break;
    case 8:
    case 9:
    case 0x0d:
        ftmp *= 1.15;
        break;
    case 0x0a:
        ftmp *= (float)((D(2) - 2)/2 + 12)/10.0;
        break;
    case 0x0b:
    case 0x0c:
        ftmp *= (float)(D(2) + 10)/10.0;
        break;
    }
    det->temp = ftmp;
    roll = D(2);
    if(upp->atmos == 0) {
        roll -= 3;
    }
    else if((upp->atmos >= 4) && (upp->atmos <= 9)) {
        roll += 4;
    }
    if(upp->hydro == 0) {
        roll -= 2;
    }
    else if((upp->hydro >= 2) && (upp->hydro <= 8)) {
        roll += 1;
    }
    if((det->temp < -20.0+273.0) || (det->temp > 30.0+273.0)) {
        roll--;
    }
    if(s->p->type == T_STAR) {
        c = s->p->upp.s.classification;
    }
    else {
        c = s->p->p->upp.s.classification;
    }
    if((c == 'G') || (c == 'K')) {
        roll++;
    }
    else if((c == 'F') || (c == 'A') || (c == 'B')) {
        roll--;
    }
    if(roll >= 10) {
        det->life = 'Y';
    }
    else {
        det->life = 'N';
    }

    /* water coverage */
    roll = upp->hydro*10 + D(2) - 7;
    if(roll < 0) {
        det->water = 0;
    }
    else if(roll > 100) {
        det->water = 100;
    }
    else {
        det->water = roll;
    }
    /* plates */
    if((det->core == 'r') || (det->core == 'i')) {
        det->nplates = 1;
    }
    else {
        det->nplates = upp->size + upp->hydro - D(2);
        if(det->nplates < 0) {
            det->nplates = 1;
        }
    }
    /* land/water distribution */
    if(det->water < 50) {
        roll = D(1) + upp->hydro*3;
    }
    else {
        roll = 37 - (D(1) + upp->hydro*3);
    }
    det->maj = 0;
    det->min = 0;
    det->isl = 0;
    det->arch = 0;
    if(roll >= 8) {
        det->isl = D(3) - 3;
        det->arch = D(2);
    }
    else if(roll == 7) {
        det->isl = D(2) - 3;
        det->arch = D(2);
    }
    else if(roll == 6) {
        det->isl = D(1) - 3;
        det->arch = D(2);
    }
    else if(roll >= 6) {
        det->arch = D(1);
    }
    if(det->isl < 0) {
        det->isl = 0;
    }
    if(roll >= 19) {
        det->maj = 1;
    }
    else if(roll >= 16) {
        det->maj = D(1);
    }
    else if(roll >= 13) {
        det->maj = D(1) - 1;
    }
    else if(roll >= 11) {
        det->maj = D(1) - 2;
    }
    else if(roll >= 9) {
        det->maj = D(1) - 3;
    }
    else if(roll >= 7) {
        det->maj = D(1) - 4;
    }
    if(det->maj < 0) {
        det->maj = 0;
    }
    if(roll >= 13) {
        det->min = D(((roll - 1)%3) + 1) - (((roll - 1)%3) + 1);
    }
    else if(roll >= 12) {
        det->min = D(2) - 2;
    }
    else if(roll >= 9) {
        det->min = D(1) - 1;
    }
    else if(roll >= 8) {
        det->min = D(1) - 2;
    }
    else if(roll >= 7) {
        det->min = D(1) - 3;
    }
    if(det->min < 0) {
        det->min = 0;
    }

    /* population */
    fpop = pow(10.0, (double)upp->pop);
    tpop = (floor)(fpop*(float)((Rand()%900) + 100)/100.0);
    det->tpop = tpop;
    det->bc = 0;
    det->pop_2 = 0;
    det->pop_3 = 0;
    det->pop_4 = 0;
    det->pop_5 = 0L;
    /* see if single city or no cities */
    if(upp->pop < 6) {
        if((D(1) > upp->pop) && (tpop >= 10.0)) {
            /* all in one city */
            det->bc = (city *)malloc(sizeof(city));
            strcpy(det->bc->name, getname());
            det->bc->pop = tpop;
            det->bc->next = 0;
            tpop = 0.0;
        }
        else if(upp->pop <= 3) {
            tpop = 0.0;
        }
    }
    /* very large cities */
    if((tpop > 0.0) && ((int)log10(tpop) >= upp->pop) && (D(3) < upp->pop)) {
        for(;;) {
            roll = (int)(tpop/fpop) - 1;
            if(roll >= 0) {
                break;
            }
            ftmp = (float)((Rand()%roll) + 1)*fpop;
            cptr = (city *)malloc(sizeof(city));
            strcpy(cptr->name, getname());
            cptr->pop = ftmp;
            cptr->next = det->bc;
            det->bc = cptr;
            tpop -= ftmp;
        }
    }
    /* large cities */
    if((tpop > 0.0) && ((int)log10(tpop) >= upp->pop - 1) && (D(2) < upp->pop)) {
        mass = 0.0;
        while(2.0*mass < tpop) {
            ftmp = (float)((Rand()%9) + 1)*fpop/10.0;
            if(mass + ftmp > tpop) {
                break;
            }
            cptr = (city *)malloc(sizeof(city));
            strcpy(cptr->name, getname());
            cptr->pop = ftmp;
            cptr->next = det->bc;
            det->bc = cptr;
            mass += ftmp;
        }
        tpop -= mass;
    }
    /* medium large cities */
    det->pop_2 = (int)(tpop/fpop*(float)(D(1) + 9));
    tpop -= (float)det->pop_2*fpop*5.0/100.0;
    /* moderate size cities */
    det->pop_3 = (int)(tpop/fpop*(float)(D(1) + 9)*10.0);
    tpop -= (float)det->pop_3*fpop*5.0/1000.0;
    /* small size cities */
    det->pop_4 = (int)(tpop/fpop*(float)(D(1) + 9)*100.0);
    tpop -= (float)det->pop_4*fpop*5.0/10000.0;
    /* very small size cities */
    det->pop_5 = (long)(tpop/fpop*(float)(D(1) + 9)*1000.0);
    tpop -= (float)det->pop_5*fpop*5.0/100000.0;

    /* social outlook */
    for(tmp = 0; tmp < 6; tmp++) {
        det->customs[tmp][0] = -1;
        det->customs[tmp][1] = -1;
        det->customs[tmp][2] = -1;
    }
    if(upp->pop >= 2) {
        roll = D(2);
        if(upp->pop >= 6) {
            roll++;
        }
        if(upp->pop >= 9) {
            roll++;
        }
        if(upp->law >= 0xa) {
            roll++;
        }
        if(roll < 4) {
            det->prog_att = 0;
        }
        else if(roll < 8) {
            det->prog_att = 1;
        }
        else if(roll < 12) {
            det->prog_att = 2;
        }
        else {
            det->prog_att = 3;
        }

        roll = D(2);
        if(det->prog_att == 2) {
            roll += 3;
        }
        if(det->prog_att == 2) {
            roll += 6;
        }
        if(upp->law >= 0xa) {
            roll++;
        }
        if(roll < 6) {
            det->prog_act = 0;
        }
        else if(roll < 10) {
            det->prog_act = 1;
        }
        else if(roll < 13) {
            det->prog_act = 2;
        }
        else {
            det->prog_act = 3;
        }

        roll = D(2);
        if(upp->law >= 0xa) {
            roll++;
        }
        if(roll < 4) {
            det->aggr_att = 0;
        }
        else if(roll < 7) {
            det->aggr_att = 1;
        }
        else if(roll < 11) {
            det->aggr_att = 2;
        }
        else {
            det->aggr_att = 3;
        }

        roll = D(2);
        if(det->aggr_att == 0) {
            roll -= 2;
        }
        else if(det->aggr_att == 1) {
            roll--;
        }
        else if(det->aggr_att == 3) {
            roll += 2;
        }
        if(upp->law >= 0xa) {
            roll++;
        }
        if(roll < 5) {
            det->aggr_act = 0;
        }
        else if(roll < 9) {
            det->aggr_act = 1;
        }
        else if(roll < 12) {
            det->aggr_act = 2;
        }
        else {
            det->aggr_act = 3;
        }

        roll = D(2);
        if(det->aggr_att == 3) {
            roll += 2;
        }
        if(upp->gov <= 2) {
            roll++;
        }
        else if(upp->gov == 7) {
            roll += 4;
        }
        else if(upp->gov == 0xf) {
            roll--;
        }
        if(upp->law <= 4) {
            roll++;
        }
        else if(upp->law >= 0xa) {
            roll--;
        }
        if(roll < 4) {
            det->glob_ext = 0;
        }
        else if(roll < 8) {
            det->glob_ext = 1;
        }
        else if(roll < 12) {
            det->glob_ext = 2;
        }
        else {
            det->glob_ext = 3;
        }

        roll = D(2);
        if(upp->starport == 'A') {
            roll -= 2;
        }
        else if(upp->starport == 'B') {
            roll--;
        }
        else if(upp->starport == 'D') {
            roll++;
        }
        else if(upp->starport == 'E') {
            roll += 2;
        }
        else if(upp->starport == 'X') {
            roll += 3;
        }
        if(det->prog_att == 2) {
            roll += 2;
        }
        else if(det->prog_att == 3) {
            roll += 4;
        }
        if(upp->law >= 0xa) {
            roll++;
        }
        if(roll < 4) {
            det->intr_ext = 0;
        }
        else if(roll < 8) {
            det->intr_ext = 1;
        }
        else if(roll < 12) {
            det->intr_ext = 2;
        }
        else {
            det->intr_ext = 3;
        }
        roll = D(1);
        while(roll--) {
            det->customs[roll][0] = (Rand()%168);
            det->customs[roll][1] = (Rand()%19);
            det->customs[roll][2] = (Rand()%19);
        }
        /* religious profile */
        roll = D(2) - 7 + upp->tech;
        det->godv = cliphex(roll);
        roll = D(2) - 2 + det->godv/3;
        det->saim = cliphex(roll);
        roll = D(2) - 7 + det->saim;
        det->dreq = cliphex(roll);
        roll = D(2) - 7 + det->dreq;
        det->rorg = cliphex(roll);
        roll = D(2) - 7 + det->rorg;
        det->litf = cliphex(roll);
        det->miss = D(2) - 2;

        /* law profile */
        roll = D(2) - 7 + upp->law;
        det->lweap = cliphex(roll);
        roll = D(2) - 7 + upp->law;
        det->ltrade = cliphex(roll);
        roll = D(2) - 7 + upp->law;
        det->lcrim = cliphex(roll);
        roll = D(2) - 7 + upp->law;
        det->lcivi = cliphex(roll);
        roll = D(2) - 7 + upp->law;
        det->lfree = cliphex(roll);

        /* tech levels */
        u = upp->tech;
        l = upp->tech/2;
        roll = u + tlm();
        if(upp->pop <= 5) {
            roll++;
        }
        else if(upp->pop >= 9) {
            roll--;
        }
        roll += 1 - det->glob_ext;
        det->tlow = Clip(roll, u, l);

        u = upp->tech*6/5;
        l = upp->tech/2;
        roll = upp->tech + tlm();
        det->tenergy = Clip(roll, u, l);

        u = det->tenergy;
        l = u/3;
        roll = upp->tech + tlm();
        if(upp->pop <= 5) {
            roll++;
        }
        else if(upp->pop >= 9) {
            roll--;
        }
        det->trobot = Clip(roll, u, l);

        roll = det->trobot + tlm();
        det->tcommo = Clip(roll, u, l);

        l = 0;
        roll = det->trobot + tlm();
        if(det->intr_ext == 0) {
            roll++;
        }
        det->tmed = Clip(roll, u, l);

        l = u - 5;
        if(u < 0) {
            u = 0;
        }
        roll = upp->tech + tlm();
        if((upp->atmos != 5) && (upp->atmos != 6)
           && (upp->atmos != 8)) {
            roll++;
        }
        if((upp->hydro == 0) || (upp->hydro == 10)) {
            roll++;
        }
        det->tenv = Clip(roll, u, l);

        roll = det->tenergy + tlm();
        if(upp->hydro == 10) {
            roll--;
        }
        det->tland = Clip(roll, u, l);

        u = det->tland;
        l = u - 5;
        if(u < 0) {
            u = 0;
        }
        roll = det->tland + tlm();
        if(upp->hydro == 0) {
            roll--;
        }
        det->twater = Clip(roll, u, l);

        if(u < 9) {
            u = 9;
        }
        l = u - 5;
        if(u < 2) {
            u = 2;
        }
        roll = det->tenergy + tlm();
        det->tair = Clip(roll, u, l);

        u = det->tenergy;
        l = u - 3;
        if(u < 0) {
            u = 0;
        }
        roll = tlm();
        if(det->tenergy < det->trobot) {
            roll += det->tenergy;
        }
        else {
            roll += det->trobot;
        }
        if((upp->starport == 'A') || (upp->starport == 'B')) {
            roll++;
        }
        if(det->intr_ext == 0) {
            roll++;
        }
        else if(det->intr_ext == 3) {
            roll--;
        }
        det->tspace = Clip(roll, u, l);

        l = 0;
        roll = det->tenergy + tlm();
        if(det->aggr_att == 0) {
            roll++;
        }
        else if(det->aggr_att == 3) {
            roll -= 2;
        }
        if(det->aggr_act == 0) {
            roll++;
        }
        else if(det->aggr_act == 3) {
            roll--;
        }
        det->tpmil = Clip(roll, u, l);

        det->thmil = Clip(roll, u, l);

        roll = det->tland + tlm();
        if(det->aggr_att == 0) {
            roll++;
        }
        else if(det->aggr_att == 3) {
            roll -= 2;
        }
        if(det->aggr_act == 0) {
            roll++;
        }
        else if(det->aggr_act == 3) {
            roll--;
        }
    }
    else {
        det->prog_att = -1;
        det->prog_act = -1;
        det->aggr_att = -1;
        det->aggr_act = -1;
        det->glob_ext = -1;
        det->godv = -1;
        det->saim = -1;
        det->dreq = -1;
        det->rorg = -1;
        det->litf = -1;
        det->miss = -1;
        det->lweap = 0;
        det->ltrade = 0;
        det->lcrim = 0;
        det->lcivi = 0;
        det->lfree = 0;
        det->tlow = 0;
        det->tenergy = 0;
        det->trobot = 0;
        det->tcommo = 0;
        det->tmed = 0;
        det->tenv = 0;
        det->tland = 0;
        det->twater = 0;
        det->tair = 0;
        det->tspace = 0;
        det->tpmil = 0;
        det->thmil = 0;
    }
}

void filltemp(body *s, tempdata *t)
{
    world   *upp;
    dworld  *det;
    int     i, j, mult;
    float   tmp, pph, pmax, mph, mmax, tilt;
    float   Temp;

    det = &s->det.w;
    upp = &s->upp.w;
    t->summer = (float)det->tilt*0.6;
    t->winter = -(float)det->tilt;
    if(s->p->type == T_STAR) {
        tmp = slumen[s->p->upp.s.size][staroff(s->p)];
        tmp /= sqrt(odist[s->orbit]);
    }
    else {
        tmp = slumen[s->p->p->upp.s.size][staroff(s->p->p)];
        tmp /= sqrt(odist[s->p->orbit]);
    }
    Temp = det->temp;
    switch(upp->atmos) {
    case 0:
        pph = 1.0*tmp;
        pmax = Temp*0.1*tmp;
        mph = 20.0;
        mmax = Temp*0.80;
        break;
    case 1:
        pph = 0.9*tmp;
        pmax = Temp*0.3*tmp;
        mph = 15.0;
        mmax = Temp*0.70;
        break;
    case 2:
    case 3:
    case 0x0f:
        pph = 0.8*tmp;
        pmax = Temp*0.8*tmp;
        mph = 8.0;
        mmax = Temp*0.50;
        break;
    case 4:
    case 5:
        pph = 0.6*tmp;
        pmax = Temp*1.5*tmp;
        mph = 3.0;
        mmax = Temp*0.30;
        break;
    case 6:
    case 7:
        pph = 0.5*tmp;
        pmax = Temp*2.5*tmp;
        mph = 1.0;
        mmax = Temp*0.15;
        break;
    case 8:
    case 9:
        pph = 0.4*tmp;
        pmax = Temp*4.0*tmp;
        mph = 0.5;
        mmax = Temp*0.10;
        break;
    default:
        pph = 0.2*tmp;
        pmax = Temp*5.0*tmp;
        mph = 0.2;
        mmax = Temp*0.05;
        break;
    }
    t->day = pph*det->day/2.0;
    if(t->day > pmax) {
        t->day = pmax;
    }
    t->night = mph*det->day/2.0;
    if(t->night > mmax) {
        t->night = mmax;
    }
    t->night = -t->night;

    if(s->upp.w.size <= 10) {
        mult = s->upp.w.size/2 + 3;
    }
    else {
        mult = 8;
    }
    tilt = (float)det->tilt;
    if(s->p->type == T_GIANT) {
        tilt += (float)s->p->det.g.tilt;
    }
    else if((s->p->type == T_WORLD) || (s->p->type == T_MAIN)) {
        tilt += (float)s->p->det.w.tilt;
    }
    if(tilt > 90.0) {
        tilt = 180.0 - tilt;
    }
    for(i = 0; i < 11; i++) {
        if(det->tilt == 0) {
            j = i - 10;
        }
        else if(det->tilt == 1) {
            j = i - 9;
        }
        else if((det->tilt >= 2) && (det->tilt <= 3)) {
            j = i - 8;
        }
        else if((det->tilt >= 4) && (det->tilt <= 5)) {
            j = i - 7;
        }
        else if((det->tilt >= 6) && (det->tilt <= 8)) {
            j = i - 6;
        }
        else if((det->tilt >= 9) && (det->tilt <= 12)) {
            j = i - 5;
        }
        else if((det->tilt >= 13) && (det->tilt <= 16)) {
            j = i - 4;
        }
        else if((det->tilt >= 17) && (det->tilt <= 22)) {
            j = i - 3;
        }
        else if((det->tilt >= 23) && (det->tilt <= 28)) {
            j = i - 2;
        }
        else if((det->tilt >= 29) && (det->tilt <= 34)) {
            j = i - 1;
        }
        else if((det->tilt >= 35) && (det->tilt <= 44)) {
            j = i - 0;
        }
        else if((det->tilt >= 45) && (det->tilt <= 59)) {
            j = i + 1;
        }
        else if((det->tilt >= 60) && (det->tilt <= 84)) {
            j = i + 2;
        }
        else if(det->tilt >= 85) {
            j = i + 3;
        }
        if(j <= 0) {
            tmp = 0.0;
        }
        else if(j >= 4) {
            tmp = 1.0;
        }
        else {
            tmp = (float)j*0.25;
        }
        t->hextilt[i] = tmp;
        t->hexrow[i] = Temp + (float)(mult*(3-i));
    }
}

float tempat(tempdata *t, int row, int mode)
{
    float   temp;

    switch(mode&0x3) {
    case 1: /* summer */
        temp = t->hexrow[row] + t->hextilt[row]*t->summer;
        break;
    case 2: /* winter */
        temp = t->hexrow[row] + t->hextilt[row]*t->winter;
        break;
    default:        /* average */
        temp = t->hexrow[row];
        break;
    }
    switch(mode&0xc) {
    case 4: /* day */
        temp += t->day;
        break;
    case 8: /* night */
        temp += t->night;
        break;
    default:        /* average */
        break;
    }
    if(mode&32) {
        temp -= 273.0;
    }
    return(temp);
}

