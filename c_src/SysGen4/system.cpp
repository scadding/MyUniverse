
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysgen.h"
#include "stelfun.h"
#include "system.h"
#include "populate.h"

int mainwrld(mainworld *mw, long x, long y, long z);

int makesystem(sistem *syst, long x, long y, long z)
{
    body    *s;
    world   mw;

    if(!mainwrld(&mw, x, y, z)) {
        return(0);
    }

    syst->sys = balloc();
#ifdef DEBUG
    printf("System");
#endif
    syst->x = x;
    syst->y = y;
    syst->z = z;
    s = syst->sys;
    s->name[0] = 0;
    s->orbit = 0;
    s->i = 0;
    s->o = 0;
    s->p = 0;
    s->b = 0;
    primary(s, &mw);
    if(mw.name[0]) {
        forcemain(s, &mw);
    }
#ifdef DEBUG
    printf("Cleaning");
#endif
    clean(s);
#ifdef DEBUG
    printf(".\n");
#endif
    syst->mw = getmain(syst->sys);
    return(1);
}

void primary(body *s, world *mw)
{
    body    *sptr;
    int     roll, tmp;

    /* generate primary */
    s->type = T_STAR;
    roll = D(2);    /* generate star type */
    if(mw)
        if(((mw->atmos >= 4) && (mw->atmos <= 9)) ||
           (mw->pop >= 8)) {
            roll += 4;
        }
    switch(roll) {
    case 2:
        s->upp.s.classification = 'A';
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        s->upp.s.classification = 'M';
        break;
    case 8:
        s->upp.s.classification = 'K';
        break;
    case 9:
        s->upp.s.classification = 'G';
        break;
    case 10:
    case 11:
    case 12:
    default:
        s->upp.s.classification = 'F';
        break;
    }

    roll = D(2);    /* generate star size */
    if(mw)
        if(((mw->atmos >= 4) && (mw->atmos <= 9)) ||
           (mw->pop >= 8)) {
            roll += 4;
        }
    switch(roll) {
    case 2:
        s->upp.s.size = 2;
        break;
    case 3:
        s->upp.s.size = 3;
        break;
    case 4:
        s->upp.s.size = 4;
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        s->upp.s.size = 5;
        break;
    case 11:
        s->upp.s.size = 6;
        break;
    case 12:
    default:
        s->upp.s.size = 7;
        break;
    }
    tmp = roll;     /* save size for later dice mod */
    s->upp.s.decimal = Rand()%10;
    roll = staroff(s);
    if((s->upp.s.size == 6) && (roll < 5)) {
        s->upp.s.size = 5;
    } else if((s->upp.s.size == 4) && (roll > 8)) {
        s->upp.s.size = 5;
    }

    /* calculate max orbits */
    roll = D(2);
    if(s->upp.s.size <= 2) {
        roll += 8;
    } else if(s->upp.s.size == 3) {
        roll += 4;
    }
    if(s->upp.s.classification == 'M') {
        roll -= 4;
    } else if(s->upp.s.classification == 'K') {
        roll -= 2;
    }
    if(roll < 1) {
        roll = 1;
    }
    s->b = balloc();
#ifdef DEBUG
    printf("First orbit");
#endif
    s->b->p = s;
    s->b->i = 0;
    s->b->o = 0;
    s->b->b = 0;
    s->b->name[0] = 0;
    s->b->type = T_UNSET;
    s->b->orbit = roll;
    while(--roll >= 0) {
        sptr = balloc();
#ifdef DEBUG
        printf("Next orbit");
#endif
        sptr->p = s;
        sptr->b = 0;
        sptr->i = 0;
        sptr->o = s->b;
        s->b->i = sptr;
        sptr->name[0] = 0;
        sptr->type = T_UNSET;
        sptr->orbit = roll;
        s->b = sptr;
    }

    /* calculate number of stars */
    roll = D(2);
    if(roll > 7) {
        /* binary star */
        addcomp(s, 0, tmp);
        if(roll > 11) {
            addcomp(s, 4, tmp);
        }
    }

    mw = populate(s, mw);
    for(sptr = s->b; sptr; sptr = sptr->o)
        if(sptr->type == T_STAR) {
            if(sptr->orbit < 1000) {
                mw = populate(sptr, mw);
            } else { /* far companion */
                primary(sptr, mw);
            }
        }
}

void clean(body *s)
{
    body    *sptr;

    if(s->name[0] == 0) {
        strcpy(s->name, getname());
    }
#ifdef DEBUG
    printf("{%s:%d}", s->name, sptr->orbit);
#endif
    for(sptr = s->b; sptr; sptr = sptr->o) {
        if((sptr->type == T_EMPTY) || (sptr->type == T_INSIDE)) {
            if(sptr->i) {
                sptr->i->o = sptr->o;
            } else {
                sptr->p->b = sptr->o;
            }
            if(sptr->o) {
                sptr->o->i = sptr->i;
            }
#ifdef DEBUG
            printf("{%d}", sptr->orbit);
#endif
            free(sptr);
        } else {
            if(sptr->b) {
                clean(sptr);
            } else if(sptr->name[0] == 0) {
                strcpy(sptr->name, getname());
#ifdef DEBUG
                printf("{%s::%d}", s->name, sptr->orbit);
#endif
            }
        }
    }
}

body *getmain(body *s)
{
    body    *sptr;

    for(sptr = s->b; sptr; sptr = sptr->o)
        if(sptr->type == T_MAIN) {
            return(sptr);
        } else if(sptr->b) {
            s = getmain(sptr);
            if(s) {
                return(s);
            }
        }
    return(0);
}

void forcemain(body *s, mainworld *mw)
{
    body    *sptr;
    int     orb;
    char    z;

    /* find hab zone */
    for(orb = 0; orb < 13; orb++) {
        z = zone(s, orb);
        if((z == 'H') || (z == 'O')) {
            break;
        }
    }
    if(!s->b) {
        /* no planets at all! */
        sptr = balloc();
#ifdef DEBUG
        printf("Force main");
#endif
        s->b = sptr;
        sptr->i = 0;
        sptr->o = 0;
        sptr->b = 0;
        sptr->p = s;
        sptr->orbit = orb;
        sptr->type = T_MAIN;
        sptr->upp.w = *mw;
        strcpy(sptr->name, mw->name);
        mw->name[0] = 0;
        return;
    }
    /* try to force into empty orbit */
    for(sptr = s->b; sptr; sptr = sptr->o)
        if(sptr->type == T_EMPTY) {
            sptr->type = T_MAIN;
            strcpy(sptr->name, mw->name);
            sptr->upp.w = *mw;
            mw->name[0] = 0;
            return;
        } else if(sptr->type == T_STAR) {
            forcemain(sptr, mw);
            if(!mw->name[0]) {
                return;
            }
        }
    /* force onto last orbit */
    for(sptr = s->b; sptr->o; sptr = sptr->o) {
        ;
    }
#ifdef DEBUG
    printf("Force last (to %d)", sptr->orbit);
#endif
    sptr->o = balloc();
    sptr->o->i = sptr;
    sptr = sptr->o;
    sptr->o = 0;
    sptr->b = 0;
    sptr->p = s;
    sptr->orbit = sptr->i->orbit + 1;
    sptr->type = T_MAIN;
    sptr->upp.w = *mw;
    strcpy(sptr->name, mw->name);
    mw->name[0] = 0;
}
