#include "sysgen.h"
#include "ship.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

mainworld      plan;
mainworld      mw;
long   x, y, z, tim;
ship   *itin;
void   doday();
int mainwrld(mainworld *mw, long x, long y, long z);

void findvar(long *xx, long *yy, long *zz, int var)
{
    long    tmp;

    tmp = randx.val;
    do {
        randx.val = tmp;
        *xx = x + (Rand()%(var*2)) - var;
        *yy = y + (Rand()%(var*2)) - var;
        *zz = z + (Rand()%(var*2)) - var;
        tmp = randx.val;
    } while(!mainwrld(&mw, *xx, *yy, *zz));
    randx.val = tmp;
}

int    passmod()
{
    int     mod;

    mod = plan.tech - mw.tech;
    if(mw.pop <= 4) {
        mod -= 3;
    } else if(mw.pop >= 8) {
        mod++;
    }
    return(mod);
}

int    main(int argc, char **argv)
{
    int     mod, j;
    ship    *shptr;
    char    inbuf[40];
    int     h, m, l;

    if(argc > 1) {
        x = atol(argv[1]);
    } else {
        printf("X: ");
        x = atol(fgets(inbuf,255,stdin));
    }
    if(argc > 2) {
        y = atol(argv[2]);
    } else {
        printf("Y: ");
        y = atol(fgets(inbuf,255,stdin));
    }
    if(argc > 3) {
        z = atol(argv[3]);
    } else {
        printf("Z: ");
        z = atol(fgets(inbuf,255,stdin));
    }
    if(argc > 4) {
        tim = atol(argv[4]);
    } else {
        printf("TIME: ");
        tim = atol(fgets(inbuf,255,stdin));
    }
    if(!mainwrld(&plan, x, y, z)) {
        printf("No planet at %ld,%ld,%ld!\n", x, y, z);
        exit(1);
    }
    mod = plan.tech;
    switch(plan.starport) {
    case 'A':
        mod += 4;
        break;
    case 'B':
        mod += 3;
        break;
    case 'C':
        mod += 2;
        break;
    case 'D':
        mod += 1;
        break;
    case 'E':
        break;
    default:
        mod -= 4;
        break;
    }
    itin = 0;
    for(j = 0; j < 7; j++) {
        doday(tim + (long)j, mod);
    }
    printf("%s Timetable\n", plan.name);
    printf("Date: %ld\n\n", tim);
    for(shptr = itin; shptr; shptr = shptr->next) {
        mainwrld(&mw, shptr->ox, shptr->oy, shptr->oz);
        printf("The %s (", shptr->name);
        if(shptr->finance == ARNELIA) {
            printf("Arnelia");
        } else if(shptr->finance == GALATICA) {
            printf("Galatica");
        } else {
            printf("Delgado");
        }
        printf(") out of %s (%ld %ld %ld)\n",
               mw.name, shptr->ox, shptr->oy, shptr->oz);
        mainwrld(&mw, shptr->dx, shptr->dy, shptr->dz);
        printf("\tDeparting %ld for %s (%ld %ld %ld)\n",
               shptr->leaving, mw.name,
               shptr->dx, shptr->dy, shptr->dz);
        j = (int)(shptr->leaving - tim);
        h = shptr->fh*(10 - j)/10;
        m = shptr->fm*(10 - j)/10;
        l = shptr->fl*(10 - j)/10;
        printf("\t%d High berths %d booked, %d standby\n",
               shptr->high, h, m);
        printf("\t%d Low berths %d booked\n", shptr->low, l);
    }
}

int high(int mod)
{
    int     ret;

    switch(plan.pop) {
    case 0:
    case 1:
        ret = 0;
        break;
    case 2:
        ret = D(1) - D(1);
        break;
    case 3:
        ret = D(2) - D(2);
        break;
    case 4:
    case 5:
        ret = D(2) - D(1);
        break;
    case 6:
    case 7:
        ret = D(3) - D(2);
        break;
    case 8:
    case 9:
        ret = D(3) - D(1);
        break;
    default:
        ret = D(3);
        break;
    }
    ret += mod;
    if(ret < 0) {
        return(0);
    }
    return(ret);
}

int middle(int mod)
{
    int     ret;

    switch(plan.pop) {
    case 0:
        ret = 0;
        break;
    case 1:
        ret = D(1) - 2;
        break;
    case 2:
        ret = D(1);
        break;
    case 3:
    case 4:
        ret = D(2) - D(1);
        break;
    case 5:
    case 6:
        ret = D(3) - D(2);
        break;
    case 7:
    case 8:
        ret = D(3) - D(1);
        break;
    case 9:
        ret = D(3);
        break;
    default:
        ret = D(4);
        break;
    }
    ret += mod;
    if(ret < 0) {
        return(0);
    }
    return(ret);
}

int low(int mod)
{
    int     ret;

    switch(plan.pop) {
    case 0:
        ret = 0;
        break;
    case 1:
        ret = D(2) - 6;
        break;
    case 2:
    case 3:
        ret = D(2);
        break;
    case 4:
    case 5:
        ret = D(3) - D(1);
        break;
    case 6:
    case 7:
        ret = D(3);
        break;
    case 8:
        ret = D(4);
        break;
    case 9:
        ret = D(5);
        break;
    default:
        ret = D(6);
        break;
    }
    ret += mod;
    if(ret < 0) {
        return(0);
    }
    return(ret);
}

void doday(long tim, int sh)
{
    long    tx, ty, tz;
    int     mod, roll, i, k;
    ship    *shptr;

    xyzmagic(x, y, z, tim);
    for(k = (sh + D(2) - 7)/7; k >= 0; k--) {
        shptr = (ship *)malloc(sizeof(ship));
        shptr->next = itin;
        itin = shptr;
        do {
            findvar(&shptr->ox, &shptr->oy, &shptr->oz, (sh<=0)? 2 : sh*2);
        } while((mw.starport < 'A') || (mw.starport > 'B')
                || (mw.tech < 9));
        strcpy(shptr->name, getname());
        shptr->tl = mw.tech;
        shptr->range = (mw.tech + 1)/4;
        if(shptr->range > 2) {
            shptr->range = (Rand()%(shptr->range - 1)) + 2;
        }
        roll = Rand()%20;
        if(roll < 13) {
            shptr->finance = ARNELIA;
        } else if(roll < 17) {
            shptr->finance = GALATICA;
        } else {
            shptr->finance = DELGADO;
        }
        roll = shptr->tl*D(1);
        if(shptr->finance == ARNELIA) {
            shptr->high = roll*8/10;
        } else if(shptr->finance == GALATICA) {
            shptr->high = roll*7/10;
        } else {
            shptr->high = roll*6/10;
        }
        shptr->low = roll - shptr->high;
        findvar(&shptr->dx, &shptr->dy, &shptr->dz, shptr->range*2);
        mod = passmod();
        for(i = 0; i < 4; i++) {
            findvar(&tx, &ty, &tz, shptr->range*2);
            if(passmod() > mod) {
                mod = passmod();
                shptr->dx = tx;
                shptr->dy = ty;
                shptr->dz = tz;
            }
        }
        shptr->leaving = tim;
        shptr->fh = high(mod);
        shptr->fm = middle(mod);
        shptr->fl = low(mod);
    }
}
