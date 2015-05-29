
#include <string.h>
#include "sysgen.h"

const char   *strprt[4] = {
    "AABBCCCDEEX",  /* backwater */
    "AAABBCCDEEX",  /* standard */
    "AAABBCCDEEE",  /* mature */
    "AAAABBCCDEX"   /* cluster */
};

int mainwrld(mainworld *mw, long x, long y, long z)
{
    int     roll;

    roll = exist(x, y, z);
    if(roll == 0) {
        return(0);
    }
    mw->starport = strprt[roll - 1][D(2) - 2];

    mw->size = D(2) - 2;

    if(mw->size == 0) {
        mw->atmos = 0;
    }
    else {
        mw->atmos = D(2) - 7 + mw->size;
        if(mw->atmos < 0) {
            mw->atmos =0;
        }
    }

    if(mw->size <= 1) {
        mw->hydro = 0;
    }
    else {
        roll = D(2) - 7 + mw->size;
        if((mw->atmos <= 1) || (mw->atmos >= 0xa)) {
            roll -= 4;
        }
        if(roll < 0) {
            mw->hydro = 0;
        }
        else if(roll > 10) {
            mw->hydro = 10;
        }
        else {
            mw->hydro = roll;
        }
    }

    mw->pop = D(2) - 2;

    mw->gov = D(2) - 7 + mw->pop;
    if((mw->pop == 0) || (mw->gov < 0)) {
        mw->gov = 0;
    }

    mw->law = D(2) - 7 + mw->gov;
    if((mw->gov == 0) || (mw->law < 0)) {
        mw->law = 0;
    }

    roll = D(1);
    if(mw->starport == 'A') {
        roll += 6;
    }
    else if(mw->starport == 'B') {
        roll += 4;
    }
    else if(mw->starport == 'C') {
        roll += 2;
    }
    else if(mw->starport == 'X') {
        roll -= 4;
    }
    if(mw->size <= 1) {
        roll += 2;
    }
    else if(mw->size <= 4) {
        roll++;
    }
    if((mw->atmos <= 3) || (mw->atmos >= 10)) {
        roll++;
    }
    if(mw->hydro >= 10) {
        roll += 2;
    }
    else if(mw->hydro >= 9) {
        roll++;
    }
    if((mw->pop >= 1) && (mw->pop <= 5)) {
        roll++;
    }
    else if(mw->pop >= 10) {
        roll += 4;
    }
    else if(mw->pop >= 9) {
        roll += 2;
    }
    else if(mw->pop == 0) {
        roll = -99;
    }
    if((mw->gov == 0) || (mw->gov == 5)) {
        roll++;
    }
    else if(mw->gov == 0xd) {
        roll -= 2;
    }
    if(roll < 0) {
        mw->tech = 0;
    }
    else {
        mw->tech = roll;
    }

    mw->bases = 0;
    if((mw->starport == 'A') || (mw->starport == 'B'))
        if(D(2) >= 8) {
            mw->bases |= NAVAL_BASE;
        }
    if((mw->starport == 'A') && (D(2) >= 10)) {
        mw->bases |= SCOUT_BASE;
    }
    else if((mw->starport == 'B') && (D(2) >= 9)) {
        mw->bases |= SCOUT_BASE;
    }
    else if((mw->starport == 'C') && (D(2) >= 8)) {
        mw->bases |= SCOUT_BASE;
    }
    else if((mw->starport == 'D') && (D(2) >= 7)) {
        mw->bases |= SCOUT_BASE;
    }
    if((mw->starport == 'A') && (D(2) >= 10)) {
        mw->bases |= LOCAL_BASE;
    }
    else if((mw->starport == 'B') && (D(2) >= 9)) {
        mw->bases |= LOCAL_BASE;
    }
    else if((mw->starport == 'C') && (D(2) >= 8)) {
        mw->bases |= LOCAL_BASE;
    }

    seedlang(x, y, z);
    strcpy(mw->name, getname());
    return(1);
}
