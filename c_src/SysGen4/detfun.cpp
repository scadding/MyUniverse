#include "sysgen.h"
#include <math.h>

int    roltilt()
{
    int     tilt;

    switch(D(2)) {
    case 2:
    case 3:
        tilt = 0;
        break;
    case 4:
    case 5:
        tilt = 10;
        break;
    case 6:
    case 7:
        tilt = 20;
        break;
    case 8:
    case 9:
        tilt = 30;
        break;
    case 10:
    case 11:
        tilt = 40;
        break;
    case 12:
        switch(D(1)) {
        case 1:
        case 2:
            tilt = 50;
            break;
        case 3:
            tilt = 60;
            break;
        case 4:
            tilt = 70;
            break;
        case 5:
            tilt = 80;
            break;
        case 6:
            tilt = 90;
            break;
        }
    }
    if(tilt != 90) {
        tilt += D(2) - 2;
    }
    return(tilt);
}

float  roleccn()
{
    switch(D(2)) {
    case 8:
        return(0.005);
    case 9:
        return(0.010);
    case 10:
        return(0.015);
    case 11:
        return(0.020);
    case 12:
        switch(D(1)) {
        case 1:
            return(0.025);
        case 2:
            return(0.050);
        case 3:
            return(0.100);
        case 4:
            return(0.200);
        case 5:
            return(0.250);
        case 6:
            return(0.300);
        }
    }
    return(0.0);
}

int    cliphex(int val)
{
    if(val < 1) {
        return(0);
    }
    else if(val > 14) {
        return(15);
    }
    return(val);
}

int    Clip(int val, int h, int l)
{
    if(val < l) {
        return(l);
    }
    else if(val > h) {
        return(h);
    }
    return(val);
}

int    tlm()
{
    switch(D(2)) {
    case 2:
        return(-D(1));
    case 3:
        return(-2);
    case 4:
        return(-1);
    case 10:
        return(1);
    case 11:
        return(2);
    case 12:
        return(D(1));
    }
    return(0);
}
