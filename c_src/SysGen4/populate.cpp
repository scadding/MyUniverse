#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysgen.h"
#include "stelfun.h"
#include "populate.h"

void doempties(body *s, int n)
{
    body    *sptr;

    while(n--) {
        sptr = rollorb(s, 2, -2);
        if(!sptr) {
            return;
        }
        sptr->type = T_EMPTY;
    }
}

void dogases(body *s, int n)
{
    body    *sptr;
    int     i;
    char    z;

    for(i = 0; i < 14; i++) {
        z = zone(s, i);
        if((z == 'H') || (z == 'O')) {
            break;
        }
    }
    while(n--) {
        sptr = rollorb(s, 2, i-3);
        if(!sptr) {
            return;
        }
        sptr->type = T_GIANT;
        if(Rand()%2) {
            sptr->upp.g.size = 1;
        }
        else {
            sptr->upp.g.size = 0;
        }
    }
}

void dotoids(body *s, int n)
{
    body    *sptr;

    if(s->b == 0) {
        return;
    }
    for(sptr = s->b; sptr->o; sptr = sptr->o) {
        ;
    }
    /* work from out to in */
    while(sptr->i) {
        sptr = sptr->i;
        if((sptr->type == T_UNSET) && (sptr->o->type == T_GIANT)) {
            sptr->type = T_TOIDS;
            if(!--n) {
                return;
            }
        }
    }
    while(n--) {
        sptr = rollorb(s, 2, 0);
        if(!sptr) {
            return;
        }
        sptr->type = T_TOIDS;
    }
}

/* fill star with planets */
mainworld *populate(body *s, mainworld *mw)
{
    int     horb;
    int     roll;
    char    z;
    body    *sptr;

    for(sptr = s->b; sptr; sptr = sptr->o) {
        z = zone(s, sptr->orbit);
        if(((z == '-') || (z == '.')) && (sptr->type == T_UNSET)) {
            sptr->type = T_INSIDE;
        }
        else if(z == 'H') {
            horb = sptr->orbit;
        }
    }

    if(D(1) >= 5) {
        roll = D(1);
        if(roll < 3) {
            doempties(s, 1);
        }
        else if(roll > 3) {
            doempties(s, 3);
        }
        else {
            doempties(s, 2);
        }
    }

    if(D(2) >= 5)
        switch(D(2)) {
        case 2:
        case 3:
            dogases(s, 1);
            break;
        case 4:
        case 5:
            dogases(s, 2);
            break;
        case 6:
        case 7:
            dogases(s, 3);
            break;
        case 8:
        case 9:
        case 10:
            dogases(s, 4);
            break;
        case 11:
        case 12:
            dogases(s, 5);
            break;
        }

    if(D(2) >= 8) {
        roll = D(2);
        if(roll < 8) {
            dotoids(s, 1);
        }
        else if(roll > 11) {
            dotoids(s, 3);
        }
        else {
            dotoids(s, 2);
        }
    }

    if(mw->name[0]) {
        sptr = getorb(s, horb);
        if(sptr) {
            if((sptr->type == T_UNSET) || (sptr->type == T_EMPTY)) {
                sptr->type = T_MAIN;
                strcpy(sptr->name, mw->name);
                sptr->upp.w = *mw;
                mw->name[0] = 0;
            }
            else if(sptr->type == T_GIANT) {
#ifdef DEBUG
                printf("Add main to giant");
#endif
                sptr->b = balloc();
                sptr->b->type = T_MAIN;
                strcpy(sptr->b->name, mw->name);
                sptr->b->upp.w = *mw;
                sptr->b->i = 0;
                sptr->b->o = 0;
                sptr->b->b = 0;
                sptr->b->p = sptr;
                strcpy(sptr->b->name, mw->name);
                roll = D(2);
                sptr->b->orbit = D(2) + 1;
                if(roll == 12) {
                    sptr->b->orbit *= 25;
                }
                else if(roll > 7) {
                    sptr->b->orbit *= 5;
                }
                mw->name[0] = 0;
            }
        }
    }

    for(sptr = s->b; sptr; sptr = sptr->o) {
        if(sptr->type == T_GIANT) {
            satelite(s, sptr, D(2) - ((sptr->upp.g.size)? 0 : 4), mw);
            continue;
        }
        if(sptr->type != T_UNSET) {
            continue;
        }
        sptr->type = T_WORLD;
        /* size */
        roll = D(2) - 2;
        if(sptr->orbit == 0) {
            roll -= 5;
        }
        else if(sptr->orbit == 1) {
            roll -= 4;
        }
        else if(sptr->orbit == 2) {
            roll -= 2;
        }
        if(s->upp.s.classification == 'M') {
            roll -= 2;
        }
        if(roll < 0) {
            sptr->upp.w.size = 0;    /* Small */
        }
        else {
            sptr->upp.w.size = roll;
        }
        /* atmos */
        z = zone(s, sptr->orbit);
        if(sptr->upp.w.size == 0) {
            sptr->upp.w.atmos = 0;
        }
        else if((zone(s, sptr->orbit - 1) == 'O') && (D(2) == 12)) {
            sptr->upp.w.atmos = 10;
        }
        else {
            roll = D(2) - 7 + sptr->upp.w.size;
            if(z == 'I') {
                roll -= 2;
            }
            else if(z == 'O') {
                roll -= 4;
            }
            if(roll < 0) {
                sptr->upp.w.atmos = 0;
            }
            else {
                sptr->upp.w.atmos = roll;
            }
        }
        /* hydrographics */
        if((z == 'I') || (sptr->upp.w.size <= 0)) {
            sptr->upp.w.hydro = 0;
        }
        else {
            roll = D(2) - 7 + sptr->upp.w.size;
            if(z == 'O') {
                roll -= 2;
            }
            if((sptr->upp.w.atmos <= 1) || (sptr->upp.w.atmos >= 10)) {
                roll -= 2;
            }
            if(roll < 0) {
                sptr->upp.w.hydro = 0;
            }
            else if(roll > 10) {
                sptr->upp.w.hydro = 10;
            }
            else {
                sptr->upp.w.hydro = roll;
            }
        }
        /* population */
        roll = D(2) - 2;
        if(z == 'I') {
            roll -= 5;
        }
        else if(z == 'O') {
            roll -= 3;
        }
        if(roll < 0) {
            sptr->upp.w.pop = 0;
        }
        else {
            sptr->upp.w.pop = roll;
        }
        /* social */
        if(sptr->upp.w.pop) {
            roll = D(1);
            if(mw->gov == 6) {
                roll += sptr->upp.w.pop;
            }
            else if(mw->gov >= 7) {
                roll--;
            }
            if(roll <= 1) {
                sptr->upp.w.gov = 0;
            }
            else if(roll == 2) {
                sptr->upp.w.gov = 1;
            }
            else if(roll == 3) {
                sptr->upp.w.gov = 2;
            }
            else if(roll == 4) {
                sptr->upp.w.gov = 3;
            }
            else if(roll >= 5) {
                sptr->upp.w.gov = 6;
            }
            if(sptr->upp.w.gov == 0) {
                sptr->upp.w.law = 0;
            }
            else {
                sptr->upp.w.law = D(1) - 3 + mw->law;
                if(sptr->upp.w.law < 0) {
                    sptr->upp.w.law = 0;
                }
            }
            if(mw->tech) {
                sptr->upp.w.tech = mw->tech - 1;
            }
            else {
                sptr->upp.w.tech = 0;
            }
        }
        else {
            sptr->upp.w.gov = 0;
            sptr->upp.w.law = 0;
            sptr->upp.w.tech = 0;
        }
        sptr->upp.w.bases = 0;
        /* military base */
        if(mw->pop >= 8) {
            roll = D(2);
            if(mw->pop >= 9) {
                roll++;
            }
            if(sptr->upp.w.atmos == mw->atmos) {
                roll++;
            }
            if(roll >= 12) {
                sptr->upp.w.bases |= LOCAL_BASE;
            }
        }
        /* naval base */
        if((mw->bases&NAVAL_BASE) && (sptr->upp.w.pop >= 3)) {
            sptr->upp.w.bases |= NAVAL_BASE;
        }
        /* scout base */
        if((mw->bases&NAVAL_BASE) && (sptr->upp.w.pop >= 2)) {
            sptr->upp.w.bases |= NAVAL_BASE;
        }
        /* port */
        roll = D(1);
        if(sptr->upp.w.pop >= 6) {
            roll += 2;
        }
        else if(sptr->upp.w.pop == 1) {
            roll -= 2;
        }
        else if(sptr->upp.w.pop == 0) {
            roll -= 3;
        }
        if(roll < 3) {
            sptr->upp.w.starport = 'Y';
        }
        else if(roll > 5) {
            sptr->upp.w.starport = 'F';
        }
        else if(roll == 3) {
            sptr->upp.w.starport = 'H';
        }
        else {
            sptr->upp.w.starport = 'G';
        }

        if(sptr->upp.w.size > 1) {
            satelite(s, sptr, D(1) - 3, mw);
        }
    }
    return(mw);
}

void insert(body *s, body *sub)
{
    body    *sptr;

    if(!s->b || (s->b->orbit > sub->orbit)) {
        sub->o = s->b;
        if(s->b) {
            s->b->i = sub;
        }
        sub->i = 0;
        s->b = sub;
    }
    else {
        for(sptr = s->b; sptr->o; sptr = sptr->o)
            if(sptr->o->orbit > sub->orbit) {
                break;
            }
        sub->o = sptr->o;
        if(sptr->o) {
            sptr->o->i = sub;
        }
        sptr->o = sub;
        sub->i = sptr;
    }
}

void satelite(body *pstar, body *s, int num, mainworld *mw)
{
    body    *sat;
    int     roll;
    char    z;

    z = zone(pstar, s->orbit);
    while(num-- > 0) {
#ifdef DEBUG
        printf("Add statlite");
#endif
        sat = balloc();
        sat->i = 0;
        sat->o = 0;
        sat->b = 0;
        sat->p = s;
        sat->name[0] = 0;
        sat->type = T_WORLD;
        if(s->type == T_GIANT) {
            roll = D(2);
            if(s->upp.g.size) {
                roll -= 4;
            }
            else {
                roll -= 6;
            }
        }
        else {
            roll = D(1) - s->upp.w.size;
        }
        if(roll < 0) {
            roll = 0;
        }
        else if(roll == 0) {
            roll = -1;
        }
        sat->upp.w.size = roll;
        if(sat->upp.w.size == -1) {     /* i.e. a ring */
            for(roll = 1; roll <= 3; roll++)
                if(!getorb(s, roll)) {
                    break;
                }
            if(roll > 3) {
                sat->upp.w.size = 0;
            }
            else {
                do {
                    roll = (Rand()%3) + 1;
                }
                while(getorb(s, roll));
                sat->orbit = roll;
                sat->type = T_TOIDS;
            }
        }
        if(sat->upp.w.size != -1) {     /* i.e. not a ring */
            do {
                roll = D(2);
                sat->orbit = D(2) + 1;
                if(roll == 12) {
                    sat->orbit *= 25;
                }
                else if(roll > 7) {
                    sat->orbit *= 5;
                }
            }
            while(getorb(s, sat->orbit));
        }
        /* atmos */
        if(sat->upp.w.size < 2) {
            sat->upp.w.atmos = 0;
        }
        else if((zone(pstar, s->orbit - 1) == 'O') && (D(2) == 12)) {
            sat->upp.w.atmos = 10;
        }
        else {
            roll = D(2) - 7 + sat->upp.w.size;
            if(z != 'H') {
                roll -= 4;
            }
            if(roll < 0) {
                sat->upp.w.atmos = 0;
            }
            else {
                sat->upp.w.atmos = roll;
            }
        }
        /* hydro */
        if((sat->upp.w.size < 2) || (z == 'I')) {
            sat->upp.w.hydro = 0;
        }
        else {
            roll = D(2) - 7 + sat->upp.w.size;
            if((sat->upp.w.atmos < 2) || (sat->upp.w.atmos > 9)) {
                roll -= 4;
            }
            if(z == 'O') {
                roll -= 4;
            }
            if(roll < 0) {
                sat->upp.w.hydro = 0;
            }
            else {
                sat->upp.w.hydro = roll;
            }
        }
        /* pop */
        if(sat->upp.w.size < 0) {
            sat->upp.w.pop = 0;
        }
        else {
            roll = D(2) - 2;
            if(z == 'I') {
                roll -= 5;
            }
            else if(z == 'O') {
                roll -= 4;
            }
            if(sat->upp.w.size <= 4) {
                roll -= 2;
            }
            if((sat->upp.w.atmos != 5) && (sat->upp.w.atmos != 6) &&
               (sat->upp.w.atmos != 8)) {
                roll -= 2;
            }
            if(roll < 0) {
                sat->upp.w.pop = 0;
            }
            else {
                sat->upp.w.pop = roll;
            }
        }
        /* social */
        if(sat->upp.w.pop == 0) {
            sat->upp.w.law = 0;
            sat->upp.w.gov = 0;
            sat->upp.w.tech = 0;
        }
        else {
            roll = D(1);
            if(mw->gov == 6) {
                roll += sat->upp.w.pop;
            }
            else if(mw->gov >= 7) {
                roll--;
            }
            if(roll <= 1) {
                sat->upp.w.gov = 0;
            }
            else if(roll == 2) {
                sat->upp.w.gov = 1;
            }
            else if(roll == 3) {
                sat->upp.w.gov = 2;
            }
            else if(roll == 4) {
                sat->upp.w.gov = 3;
            }
            else if(roll >= 5) {
                sat->upp.w.gov = 6;
            }
            if(sat->upp.w.gov == 0) {
                sat->upp.w.law = 0;
            }
            else {
                sat->upp.w.law = D(1) - 3 + mw->law;
                if(sat->upp.w.law < 0) {
                    sat->upp.w.law = 0;
                }
            }
            if(mw->tech) {
                sat->upp.w.tech = mw->tech - 1;
            }
            else {
                sat->upp.w.tech = 0;
            }
        }
        sat->upp.w.bases = 0;
        /* military base */
        if(mw->pop >= 8) {
            roll = D(2);
            if(mw->pop >= 9) {
                roll++;
            }
            if(sat->upp.w.atmos == mw->atmos) {
                roll++;
            }
            if(roll >= 12) {
                sat->upp.w.bases |= LOCAL_BASE;
            }
        }
        /* naval base */
        if((mw->bases&NAVAL_BASE) && (sat->upp.w.pop >= 3)) {
            sat->upp.w.bases |= NAVAL_BASE;
        }
        /* scout base */
        if((mw->bases&NAVAL_BASE) && (sat->upp.w.pop >= 2)) {
            sat->upp.w.bases |= NAVAL_BASE;
        }
        /* port */
        roll = D(1);
        if(sat->upp.w.pop >= 6) {
            roll += 2;
        }
        else if(sat->upp.w.pop == 1) {
            roll -= 2;
        }
        else if(sat->upp.w.pop == 0) {
            roll -= 3;
        }
        if(roll < 3) {
            sat->upp.w.starport = 'Y';
        }
        else if(roll > 5) {
            sat->upp.w.starport = 'F';
        }
        else if(roll == 3) {
            sat->upp.w.starport = 'H';
        }
        else {
            sat->upp.w.starport = 'G';
        }
        insert(s, sat);
    }
}
