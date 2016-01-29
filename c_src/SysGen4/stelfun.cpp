
#include <stdio.h>
#include <stdlib.h>
#include "sysgen.h"
#include "stelfun.h"

int staroff(body *s)
{
#ifdef DEBUG
    if(s->type != T_STAR) {
        printf("STAROFF CALLED ON NON-STAR!\n\007");
        return(-1);
    }
#endif
    switch(s->upp.s.classification) {
    case 'B':
        if(s->upp.s.decimal < 5) {
            return(0);
        }
        else {
            return(1);
        }
    case 'A':
        if(s->upp.s.decimal < 5) {
            return(2);
        }
        else {
            return(3);
        }
    case 'F':
        if(s->upp.s.decimal < 5) {
            return(4);
        }
        else {
            return(5);
        }
    case 'G':
        if(s->upp.s.decimal < 5) {
            return(6);
        }
        else {
            return(7);
        }
    case 'K':
        if(s->upp.s.decimal < 5) {
            return(8);
        }
        else {
            return(9);
        }
    case 'M':
        if(s->upp.s.decimal < 5) {
            return(10);
        }
        else if(s->upp.s.decimal < 9) {
            return(11);
        }
        else {
            return(12);
        }

    }
    return(-1);
}

const char   *zone_data[6][14] = {
    /* SIZE II */{
        ".............",
        "---------....",
        "---IIIIII-...",
        "--IIIIIIII...",
        "--IIIIIIIII..",
        "-IIIIIIIIII..",
        "-IIIIIIIIIIII",
        "IIIIIIIIIIIII",
        "IIIHHHHHIIIII",
        "IIHOOOOOHHIII",
        "IIOOOOOOOOHII",
        "IHOOOOOOOOOHH",
        "HOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
    },/* SIZE III */{
        "----IIIII-...",
        "--IIIIIIII-..",
        "--IIIIIIIII..",
        "--IIIIIIIII..",
        "-IIIIIIIIIII.",
        "-IIIIIIIIIIII",
        "-IIIHHHIIIIII",
        "IIIHOOOHHIIII",
        "IIHOOOOOOHHII",
        "IIOOOOOOOOOHH",
        "IHOOOOOOOOOOO",
        "IOOOOOOOOOOOO",
        "HOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
    },/* SIZE IV */{
        "---IIIIII",
        "--IIIIIII",
        "--IIIIIII",
        "-IIIIIIII",
        "-IIIIIIIH",
        "-IIIIHHHO",
        "-IIHHOOOO",
        "IIHOOOOOO",
        "IIOOOOOOO",
        "IHOOOOOOO",
        "IOOOOOOOO",
        "IOOOOOOOO",
        "HOOOOOOOO",
        "OOOOOOOOO",
    },/* SIZE V */{
        "--IIIIIIIHHOO",
        "--IIIIIIIOOOO",
        "--IIIIIHHOOOO",
        "--IIIIHOOOOOO",
        "-IIIIHOOOOOOO",
        "-IIIHOOOOOOOO",
        "IIIHOOOOOOOOO",
        "IIHOOOOOOOOOO",
        "IIOOOOOOOOOOO",
        "IHOOOOOOOOOOO",
        "IOOOOOOOOOOOO",
        "IOOOOOOOOOOOO",
        "HOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
    },/* SIZE VI */{
        "     IIIIOOOO",
        "     IIHHOOOO",
        "     IHOOOOOO",
        "     HOOOOOOO",
        "     OOOOOOOO",
        "     OOOOOOOO",
        "     OOOOOOOO",
        "     OOOOOOOO",
        "     OOOOOOOO",
        "     OOOOOOOO",
        "     OOOOOOOO",
        "     OOOOOOOO",
        "     OOOOOOOO",
        "     OOOOOOOO",
    },/* SIZE D */{
        "HHOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
        "OOOOOOOOOOOOO",
        "OOOOOOOOOOOOO"
    }
};


char zone(body *s, int orbit)
{
    int     off, tmp;

#ifdef DEBUG
    if(s->type != T_STAR) {
        printf("ZONE CALLED ON NON-STAR!\n\007");
        return(-1);
    }
#endif
    if(orbit > 13) {
        return('O');
    }
#ifdef DEBUG
    if(s->upp.s.size < 2) {
        printf("ZONE CALLED ON NON-STAR!\n\007");
        return(-1);
    }
#endif
    tmp = s->upp.s.size - 2;
    off = staroff(s);
    return(zone_data[tmp][orbit][off]);
}

/* for calculating available orbits */
int    lowavail[13] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6 };
int    hiavail[13] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
int    comavail[13] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6 };

void   addcomp(body *s, int orbmod, int sizmod)
{
    int     roll, orbit;
    body    *sptr, *ssub;

    if(s->orbit >= 1000) {
        orbmod = -4;
    }
    roll = D(2) + orbmod;
    switch(roll) {
    case -2:
    case -1:
    case 0:
    case 1:
    case 2:
    case 3:
        orbit = 0;
        break;
    case 4:
        orbit = 1;
        break;
    case 5:
        orbit = 2;
        break;
    case 6:
        orbit = 3;
        break;
    case 7:
        orbit = 4 + D(1);
        break;
    case 8:
        orbit = 5 + D(1);
        break;
    case 9:
        orbit = 6 + D(1);
        break;
    case 10:
        orbit = 7 + D(1);
        break;
    case 11:
        orbit = 8 + D(1);
        break;
    case 12:
    default:
        orbit = 1000*D(1);
        break;
    }
    for(ssub = s->b; ssub->orbit < orbit; ssub = ssub->o)
        if(ssub->o == 0) {
#ifdef DEBUG
            printf("Companion");
#endif
            ssub->o = balloc();
            ssub->o->i = ssub;
            ssub->o->o = 0;
            ssub->o->b = 0;
            ssub->o->name[0] = 0;
            ssub->o->type = T_UNSET;
            ssub->o->p = ssub->p;
            ssub->o->orbit = orbit;
        }
    if(ssub->type == T_STAR) {
        return;
    }
    ssub->type = T_STAR;

    /* determine companion type */
    roll = D(2) + sizmod;
    switch(roll) {
    case 2:
        ssub->upp.s.classification = 'A';
        break;
    case 3:
    case 4:
        ssub->upp.s.classification = 'F';
        break;
    case 5:
    case 6:
        ssub->upp.s.classification = 'G';
        break;
    case 7:
    case 8:
        ssub->upp.s.classification = 'K';
        break;
    case 9:
    case 10:
    case 11:
    case 12:
    default:
        ssub->upp.s.classification = 'M';
        break;
    }
    ssub->upp.s.decimal = Rand()%10;

    /* determine companion size */
    roll = D(2) + sizmod;
    switch(roll) {
    case 2:
        ssub->upp.s.size = 2;
        break;
    case 3:
        ssub->upp.s.size = 3;
        break;
    case 4:
        ssub->upp.s.size = 4;
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        ssub->upp.s.size = 5;
        break;
    case 11:
        ssub->upp.s.size = 6;
        break;
    case 12:
    default:
        ssub->upp.s.size = 7;
        break;
    }
    roll = staroff(s);
    if((ssub->upp.s.size == 6) && (roll < 5)) {
        ssub->upp.s.size = 5;
    }
    else if((ssub->upp.s.size == 4) && (roll > 8)) {
        ssub->upp.s.size = 5;
    }

    if(orbit >= 1000) {
        return;
    }
    /* knock off orbits */
    if(orbit > 12) {
        orbit = 12;
    }
    for(sptr = s->b; sptr; sptr = sptr->o)
        if((sptr->orbit > lowavail[orbit]) && (sptr->orbit < hiavail[orbit])
           && (sptr->type == T_UNSET)) {
            sptr->type = T_INSIDE;
        }
    orbit = comavail[orbit];
    if(orbit == 0) {
        ssub->b = 0;
        return;
    }
#ifdef DEBUG
    printf("Add companion first orbit");
#endif
    ssub->b = balloc();
    ssub->b->p = ssub;
    ssub->b->i = 0;
    ssub->b->o = 0;
    ssub->b->b = 0;
    ssub->b->type = T_UNSET;
    ssub->b->name[0] = 0;
    ssub->b->orbit = orbit;
    while(--orbit >= 0) {
#ifdef DEBUG
        printf("Add companion next orbit");
#endif
        sptr = balloc();
        sptr->p = ssub;
        sptr->i = 0;
        sptr->b = 0;
        sptr->o = ssub->b;
        ssub->b->i = sptr;
        sptr->type = T_UNSET;
        sptr->name[0] = 0;
        sptr->orbit = orbit;
        ssub->b = sptr;
    }
}

body *getorb(body *s, int orbit)
{
    if(!s->b) {
        return(0);
    }
    s = s->b;
    do {
        if(s->orbit == orbit) {
            return(s);
        }
        s = s->o;
    }
    while(s);
    return(0);
}

body *rollorb(body *s, int nd, int mod)
{
    body    *sptr;
    int     min, max;

    min = nd + mod;
    max = nd*6 + mod;
    for(sptr = s->b; sptr; sptr = sptr->o) {
        if((sptr->orbit >= min) && (sptr->orbit <= max) && (sptr->type == T_UNSET)) {
            break;
        }
    }
    if(!sptr) {
        return(0);
    }
    do {
        sptr = getorb(s, D(nd) + mod);
    }
    while(!sptr || (sptr->type != T_UNSET));
    return(sptr);
}

body   *balloc()
{
    body    *b;

#ifdef DEBUG
    printf("*");
#endif
    b = (body *)malloc(sizeof(body));
    if(!b) {
        printf("Fatal malloc error!\n");
        printf("Request:%ld\n", sizeof(body));
        exit(1);
    }
    return(b);
}
