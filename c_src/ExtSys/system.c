
/***************************************************************************/
/*                                                                         */
/*  Traveller System Generator                                             */
/*                                                                         */
/*                                                                         */
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "charts.h"

#define MAX_ORBITS   20

#define HABITNUM    374.025

#define PI          M_PI

#define EARTHMASS 1086781291301.061
#define EARTHGRAV 26716.103896

/* Global Parameters */
short   GaiaFactor = 1;
short   FDegrees   = 0;
short   Debug      = 0;      /* No program would be complete without ... */
short   PresetStar = 0;
long    SeedValue  = 0;
short   TravInfo   = 0;
short   Collapse   = 0;

typedef struct WORLD_INFO {
    char   OrbitType;
    char   Occupied;
    double Range;
} World;

typedef struct PLANET_INFO {
    short size;
    char  dense;
    short atmosphere;
    short hydro;
    short maxpop;
    double orbit;
    double tilt;
    double ecc;
    double rotation;
    double temp;
    /* Traveller Info */
    char   StarPort;
    short  Pop;
    short  PopMult;
    short  TechLevel;
    short  Government;
    double summer[22];
    double fall[22];
    double winter[22];
} Planet;

char  SystemNature = 0;
char  SystemHabitability = 0;
short HZone;

char  PrimStarType = 0;
char  PrimLumClass = 0;
char  PrimDecClass = 0;
double PrimStellarMass = 0.0;
double PrimLuminosity = 0.0;

char  ComStarType = 0;
char  ComLumClass = 0;
char  ComDecClass = 0;
double ComStellarMass = 0.0;
double ComLuminosity = 0.0;

double ComLumAddFromPrim = 0.0;

short  ComOrbit = 0;

Planet CurrPlanet;
World  Orbits[MAX_ORBITS];
World  ComOrbits[MAX_ORBITS];

static int PrimStarTypeRoll;
static int PrimLumClassRoll;

/***************************************************************************/

double CtoF(double CDegrees)
{
    return ((CDegrees*1.8)+32);
}

/***************************************************************************/

int d6()
{
    int die;

    die = rand();
    while (die > 32000) die = rand();

    return ((die % 6) +1);


} /* end of d6 */

int d10()
{
    int die;

    die = rand();
    while (die > 32000) die = rand();

    return ((die % 10) +1);


} /* end of d10 */

/*************************************************************************/

double Change(double Original)
{
    double X, Y;

    X = (d10()*10)+d10();
    Y = X/100;

    if (d6() < 4) /* reduce value */
        return (Original * Y);
    else
        return (Original * (1.0+Y));
} /* end of Change */

/*************************************************************************/

double PlanetVolume(double diameter, char dense)
{
    double R, V;

    R = diameter / 2;
    V = (4 * PI)/3 * (R*R*R);
    V = V / EARTHMASS;

    switch (dense) {
    case 'L':
        return(V*0.55);
        break;
    case 'A':
        return(V);
        break;
    case 'H':
        return(V*1.55);
        break;
    }
    return (V);

} /* end of Planet Mass */

/*************************************************************************/

double PlanetGrav(double diameter, char dense)
{
    double R, V;

    R = diameter / 2;
    V = (4 * PI)/3 * R;
    V = V / EARTHGRAV;

    switch (dense) {
    case 'L':
        return(V*0.55);
        break;
    case 'A':
        return(V);
        break;
    case 'H':
        return(V*1.55);
        break;
    }
    return (V);

} /* end of Planet Grav */

/*************************************************************************/

char ChartSysNat()
{
    int  dieroll;

    dieroll = d6()+d6();

    if (dieroll < 8) return ('S');      /* Solo */

    if (dieroll == 12) return ('T');    /* Trinary */

    return ('B');                       /* Binary */

} /* end of ChartSysNat */

char *PrintSysNat(char SysNat)
{
    if (SysNat == 'S') return ("Solo");
    if (SysNat == 'T') return ("Trinary");
    if (SysNat == 'B') return ("Binary");
    return "Unknown";

} /* end of PrintSysNat */

/*************************************************************************/

char ChartPrimStarType ()
{
    int  dieroll;

    /* Check for rare values */
    dieroll = d6()+d6()+d6();
    PrimStarTypeRoll = 2;
    if (dieroll == 3) return ('O');
    if (dieroll == 4) return ('B');

    dieroll = d6()+d6();

    PrimStarTypeRoll = dieroll;

    switch (dieroll) {
    case 2:
        return('A');
        break;
    case 3:
        return('M');
        break;
    case 4:
        return('M');
        break;
    case 5:
        return('M');
        break;
    case 6:
        return('M');
        break;
    case 7:
        return('M');
        break;
    case 8:
        return('K');
        break;
    case 9:
        return('G');
        break;
    case 10:
        return('G');
        break;
    case 11:
        return('F');
        break;
    case 12:
        return('F');
        break;
    default:
        printf("Star Type Error %d\n", dieroll);
        break;
    }

} /* end of ChartStarType */

char ChartComStarType ()
{
    int  dieroll;

    dieroll = d6()+d6()+PrimStarTypeRoll;

    switch (dieroll) {
    case 4:
        return('A');
        break;
    case 5:
        return('F');
        break;
    case 6:
        return('F');
        break;
    case 7:
        return('G');
        break;
    case 8:
        return('G');
        break;
    case 9:
        return('K');
        break;
    case 10:
        return('K');
        break;
    case 11:
        return('M');
        break;
    case 12:
        return('M');
        break;
    case 13:
        return('M');
        break;
    default:
        break;
    }
    return('M');
} /* end of ChartcomStarType */

/*************************************************************************/

char ChartPrimLumClass(char StarType)
{
    int  dieroll;


    /* Check for rare values */
    dieroll = d6()+d6()+d6();
    PrimLumClassRoll = 2;
    if (dieroll == 3) return ('a');
    if (dieroll == 4) return ('b');

    dieroll = d6()+d6();

    PrimLumClassRoll = dieroll;

    switch (dieroll) {
    case 2:
        return('2');
        break;
    case 3:
        return('3');
        break;
    case 4:
        if (StarType == 'M')
            return('5');
        else
            return('4');
        break;
    case 5:
        return('5');
        break;
    case 6:
        return('5');
        break;
    case 7:
        return('5');
        break;
    case 8:
        return('5');
        break;
    case 9:
        return('5');
        break;
    case 10:
        return('5');
        break;
    case 11:
        return('5');
        break;
    case 12:
        return('5');
        break;
    default:
        printf("Lumm Class Error %d\n", dieroll);
        break;
    }

} /* end of ChartLumClass */

char ChartComLumClass(char StarType)
{
    int  dieroll;


    dieroll = d6()+d6()+PrimLumClassRoll;

    switch (dieroll) {
    case 4:
        return('2');
        break;
    case 5:
        return('3');
        break;
    case 6:
        if (StarType == 'M')
            return('5');
        else
            return('4');
        break;
    case 7:
        return('5');
        break;
    case 8:
        return('5');
        break;
    case 9:
        return('5');
        break;
    case 10:
        return('5');
        break;
    case 11:
        return('5');
        break;
    case 12:
        return('5');
        break;
    case 13:
        return('5');
        break;
    default:
        if (d6() < 4) return('5');
        else return ('D');
        break;
    }

} /* end of ChartComLumClass */

char *PrintLumClass(char LumClass)
{
    if (LumClass == 'a') return ("Ia");
    if (LumClass == 'b') return ("Ib");
    if (LumClass == '1') return ("I");
    if (LumClass == '2') return ("II");
    if (LumClass == '3') return ("III");
    if (LumClass == '4') return ("IV");
    if (LumClass == '5') return ("V");
    if (LumClass == 'D') return ("D");
    return ("X");

} /* end of PrintLumClass */

/*************************************************************************/

char ChartDecClass (char StarType, char LumClass)
{
    int  dieroll;

    dieroll = d10() - 1;

    if ((StarType == 'O') && (dieroll < 5)) dieroll = dieroll + 5;

    if ((StarType == 'K') && (LumClass == '4') && (dieroll > 4)) dieroll = dieroll - 5;

    switch (dieroll) {
    case 0:
        return('0');
        break;
    case 1:
        return('1');
        break;
    case 2:
        return('2');
        break;
    case 3:
        return('3');
        break;
    case 4:
        return('4');
        break;
    case 5:
        return('5');
        break;
    case 6:
        return('6');
        break;
    case 7:
        return('7');
        break;
    case 8:
        return('8');
        break;
    case 9:
        return('9');
        break;
    default:
        printf("Dec Class Error %d\n", dieroll);
        break;
    }

} /* end ChartDecClass */

/*************************************************************************/

double ChartStellarMass (char StarType, char DecClass, char LumClass)
{

    int    StarSize;
    double A, B, X;

    switch (StarType) {
    case 'O':
        StarSize = 0;
        break;
    case 'B':
        StarSize = 0;
        break;
    case 'A':
        StarSize = 2;
        break;
    case 'F':
        StarSize = 4;
        break;
    case 'G':
        StarSize = 6;
        break;
    case 'K':
        StarSize = 8;
        break;
    case 'M':
        StarSize = 10;
        break;
    }

    X = DecClass - '0';

    switch (LumClass) {
    case 'a':
        A = MassTableIa[StarSize];
        B = MassTableIa[StarSize+1];
        break;
    case 'b':
        A = MassTableIb[StarSize];
        B = MassTableIb[StarSize+1];
        break;
    case '2':
        A = MassTableII[StarSize];
        B = MassTableII[StarSize+1];
        break;
    case '3':
        A = MassTableIII[StarSize];
        B = MassTableIII[StarSize+1];
        break;
    case '4':
        A = MassTableIV[StarSize];
        B = MassTableIV[StarSize+1];
        break;
    case '5':
        A = MassTableV[StarSize];
        B = MassTableV[StarSize+1];
        break;
    case 'D':
        A = MassTableD[StarSize];
        B = MassTableD[StarSize+1];
        break;
    }

    return ( (((B-A)/5)*X) + A );

} /* end ChartDecClass */

/*************************************************************************/

double ChartLuminosity(char StarType, char DecClass, char LumClass)
{

    int    StarSize;
    double A, B, X;


    switch (StarType) {
    case 'O':
        StarSize = 0;
        break;
    case 'B':
        StarSize = 0;
        break;
    case 'A':
        StarSize = 2;
        break;
    case 'F':
        StarSize = 4;
        break;
    case 'G':
        StarSize = 6;
        break;
    case 'K':
        StarSize = 8;
        break;
    case 'M':
        StarSize = 10;
        break;
    }

    X = DecClass - '0';

    switch (LumClass) {
    case 'a':
        A = LumTableIa[StarSize];
        B = LumTableIa[StarSize+1];
        if ((StarType == 'M') && (DecClass == '9'))
            return (LumTableIa[12]);
        break;
    case 'b':
        A = LumTableIb[StarSize];
        B = LumTableIb[StarSize+1];
        if ((StarType == 'M') && (DecClass == '9'))
            return (LumTableIb[12]);
        break;
    case '2':
        A = LumTableII[StarSize];
        B = LumTableII[StarSize+1];
        if ((StarType == 'M') && (DecClass == '9'))
            return (LumTableII[12]);
        break;
    case '3':
        A = LumTableIII[StarSize];
        B = LumTableIII[StarSize+1];
        if ((StarType == 'M') && (DecClass == '9'))
            return (LumTableIII[12]);
        break;
    case '4':
        A = LumTableIV[StarSize];
        B = LumTableIV[StarSize+1];
        if ((StarType == 'M') && (DecClass == '9'))
            return (LumTableIV[12]);
        break;
    case '5':
        A = LumTableV[StarSize];
        B = LumTableV[StarSize+1];
        if ((StarType == 'M') && (DecClass == '9'))
            return (LumTableV[12]);
        break;
    case 'D':
        A = LumTableD[StarSize];
        B = LumTableD[StarSize+1];
        if ((StarType == 'M') && (DecClass == '9'))
            return (LumTableD[12]);
        break;
    }

    return ( (((B-A)/5)*X) + A );

} /* end of Luminosity */

/*************************************************************************/

short ChartComOrbit()
{
    int dieroll;

    dieroll = d6()+d6();

    switch (dieroll) {
    case 2:
        return(0);
        break;
    case 3:
        return(0);
        break;
    case 4:
        return(1);
        break;
    case 5:
        return(2);
        break;
    case 6:
        return(3);
        break;
    case 7:
        return(4+d6());
        break;
    case 8:
        return(5+d6());
        break;
    case 9:
        return(6+d6());
        break;
    case 10:
        return(7+d6());
        break;
    case 11:
        return(8+d6());
        break;
    case 12:
        return(99);
        break;
    default:
        printf("Dec Class Error %d\n", dieroll);
        break;
    }

} /* end of ChartComOrbit */

/*************************************************************************/

double ChartSatOrbit()
{
    int dieroll;

    dieroll = d6()+d6();

    if (dieroll < 8) {
        switch (d6()+d6()) {
        case 2:
            return(3);
            break;
        case 3:
            return(4);
            break;
        case 4:
            return(5);
            break;
        case 5:
            return(6);
            break;
        case 6:
            return(7);
            break;
        case 7:
            return(8);
            break;
        case 8:
            return(9);
            break;
        case 9:
            return(10);
            break;
        case 10:
            return(11);
            break;
        case 11:
            return(12);
            break;
        case 12:
            return(12);
            break;
        }
    } else if (dieroll < 12) {
        switch (d6()+d6()) {
        case 2:
            return(15);
            break;
        case 3:
            return(20);
            break;
        case 4:
            return(25);
            break;
        case 5:
            return(30);
            break;
        case 6:
            return(35);
            break;
        case 7:
            return(40);
            break;
        case 8:
            return(45);
            break;
        case 9:
            return(50);
            break;
        case 10:
            return(55);
            break;
        case 11:
            return(60);
            break;
        case 12:
            return(65);
            break;
        }
    } else {
        switch (d6()+d6()) {
        case 2:
            return(75);
            break;
        case 3:
            return(100);
            break;
        case 4:
            return(125);
            break;
        case 5:
            return(150);
            break;
        case 6:
            return(175);
            break;
        case 7:
            return(200);
            break;
        case 8:
            return(225);
            break;
        case 9:
            return(250);
            break;
        case 10:
            return(275);
            break;
        case 11:
            return(300);
            break;
        case 12:
            return(325);
            break;
        }
    }

} /* end ChartSatOrbit */

/*************************************************************************/

short ChartNumOrbits(char StarType, char LumClass)
{
    int dieroll;

    dieroll = d6()+d6();

    /* Class O stars (Blue) are still forming */
    if (StarType == 'O') return (0);

    if (LumClass == '1') dieroll = dieroll + 8;
    if (LumClass == 'a') dieroll = dieroll + 8;
    if (LumClass == 'b') dieroll = dieroll + 8;
    if (LumClass == '2') dieroll = dieroll + 8;
    if (LumClass == '3') dieroll = dieroll + 4;
    if (LumClass == '4') dieroll = dieroll + 2;

    if (StarType == 'K') dieroll = dieroll - 2;
    if (StarType == 'M') dieroll = dieroll - 4;

    if (dieroll < 0) return (0);
    if (dieroll > 19) return (19);

    return (dieroll);

} /* end of ChartNumOrbits */

/*************************************************************************/

double ChartOrbitRange(short orbitnum)
{

    double r;

    switch (orbitnum) {
    case 0 :
        r =     0.1 + Change(    0.1);
        break; /* 0.2 */
    case 1 :
        r =     0.3 + Change(    0.1);
        break; /* 0.4 */
    case 2 :
        r =     0.6 + Change(    0.1);
        break; /* 0.7 */
    case 3 :
        r =     0.8 + Change(    0.2);
        break; /* 1.0 */
    case 4 :
        r =     1.2 + Change(    0.4);
        break; /* 1.6 */
    case 5 :
        r =     2.0 + Change(    0.8);
        break; /* 2.8 */
    case 6 :
        r =     3.6 + Change(    1.6);
        break; /* 5.2 */
    case 7 :
        r =     6.8 + Change(    3.2);
        break; /* 10.0 */
    case 8 :
        r =    13.2 + Change(    6.4);
        break; /* 19.6 */
    case 9 :
        r =    26.0 + Change(   12.8);
        break; /* 38.8 */
    case 10 :
        r =    51.6 + Change(   25.6);
        break; /* 77.2 */
    case 11 :
        r =   102.8 + Change(   51.2);
        break; /* 154.0 */
    case 12 :
        r =   205.2 + Change(  102.4);
        break; /* 307.6 */
    case 13 :
        r =   410.0 + Change(  204.8);
        break; /* 614.8 */
    case 14 :
        r =   819.6 + Change(  409.6);
        break; /* 1229.2 */
    case 15 :
        r =  1638.8 + Change(  819.2);
        break; /* 2548.0 */
    case 16 :
        r =  3277.2 + Change( 1638.4);
        break; /* 4915.6 */
    case 17 :
        r =  6554.0 + Change( 3276.8);
        break; /* 9830.8 */
    case 18 :
        r = 13107.6 + Change( 6553.6);
        break; /* 19661.2 */
    case 19 :
        r = 26214.8 + Change(13107.2);
        break; /* 39322.0 */
    default:
        printf("Orbit Number Error %d\n", orbitnum);
        break;
    }
    return (r);

} /* end of Chart Orbit Range */

/*************************************************************************/

#define HabitLow   330
#define HabitHigh  400

char ChartOrbitType(double Range, double Luminosity)
{
    double L, O, LO;

    L  = Luminosity;
    O  = HABITNUM / sqrt(Range);

    LO = L * O;

    LO = LO + ComLumAddFromPrim;

    if (Debug)
        printf ("L is %5.2f  O is %5.2f (%5.2f)  LO is %6.2f  ", L, O, Range, LO);

    if (LO > 8 * HabitHigh) {
        if (Debug) printf ("X\n");
        return ('X');
    }

    if (LO > HabitHigh) {
        if (Debug) printf ("I\n");
        return ('I');
    }

    if (LO < HabitLow) {
        if (Debug) printf ("O\n");
        return ('O');
    }

    if (Debug) printf ("H\n");
    return ('H');

} /* end of ChartOrbitType */

/*************************************************************************/

char ChartNumGasGiants()
{
    int dieroll;

    dieroll = d6()+d6();

    if (dieroll < 5) return (0);

    dieroll = d6()+d6();

    switch (dieroll) {
    case 2:
        return(1);
        break;
    case 3:
        return(1);
        break;
    case 4:
        return(2);
        break;
    case 5:
        return(2);
        break;
    case 6:
        return(3);
        break;
    case 7:
        return(3);
        break;
    case 8:
        return(4);
        break;
    case 9:
        return(4);
        break;
    case 10:
        return(4);
        break;
    case 11:
        return(5);
        break;
    case 12:
        return(5);
        break;
    default:
        printf("Number Of GG Error %d\n", dieroll);
        break;
    }
} /* end of NumGasGiants */

/*************************************************************************/

char ChartNumPlanetoids()
{
    int dieroll;

    dieroll = d6()+d6();

    if (dieroll < 8) return (0);

    dieroll = d6()+d6();

    switch (dieroll) {
    case 2:
        return(1);
        break;
    case 3:
        return(1);
        break;
    case 4:
        return(1);
        break;
    case 5:
        return(1);
        break;
    case 6:
        return(1);
        break;
    case 7:
        return(2);
        break;
    case 8:
        return(2);
        break;
    case 9:
        return(2);
        break;
    case 10:
        return(2);
        break;
    case 11:
        return(2);
        break;
    case 12:
        return(3);
        break;
    }
} /* end of NumPlanetoids */

/*************************************************************************/

char ChartNumEmptyOrbits(char StarType)
{
    int dieroll;

    dieroll = d6();

    if ((StarType == 'A') || (StarType == 'B')) dieroll++;

    if (dieroll < 5) return (0);

    dieroll = d6();

    if ((StarType == 'A') || (StarType == 'B')) dieroll++;

    switch (dieroll) {
    case 1:
        return(1);
        break;
    case 2:
        return(1);
        break;
    case 3:
        return(2);
        break;
    case 4:
        return(3);
        break;
    case 5:
        return(3);
        break;
    default:
        return(3);
        break;
    }
} /* end of NumEmptyOrbits */

/*************************************************************************/

char ChartNumCaptPlanets(char StarType)
{
    int dieroll;

    dieroll = d6();

    if ((StarType == 'A') || (StarType == 'B')) dieroll++;

    if (dieroll < 5) return (0);

    dieroll = d6();

    switch (dieroll) {
    case 1:
        return(1);
        break;
    case 2:
        return(1);
        break;
    case 3:
        return(1);
        break;
    case 4:
        return(2);
        break;
    case 5:
        return(2);
        break;
    default:
        return(3);
        break;
    }
} /* end of NumCaptPlanets */

/*************************************************************************/

double ChartDaytime(short Atmosphere, double DayLength, double Lum,
                    double Dist, double Temp)
{
    double R, X;
    double Max;

    R = Lum / (sqrt(Dist));

    switch (Atmosphere) {
    case 0:
        X = 1.0 * R * DayLength;
        Max = 0.1 * (Temp+273) * R;
        if (X > Max) X = Max;
        return (X);
        break;
    case 1:
        X = 0.9 * R * DayLength;
        Max = 0.3 * (Temp+273) * R;
        if (X > Max) X = Max;
        return (X);
        break;
    case 2:
    case 3:
    case 14:
        X = 0.8 * R * DayLength;
        Max = 0.8 * (Temp+273) * R;
        if (X > Max) X = Max;
        return (X);
        break;
    case 4:
    case 5:
        X = 0.6 * R * DayLength;
        Max = 1.5 * (Temp+273) * R;
        if (X > Max) X = Max;
        return (X);
        break;
    case 6:
    case 7:
        X = 0.5 * R * DayLength;
        Max = 2.5 * (Temp+273) * R;
        if (X > Max) X = Max;
        return (X);
        break;
    case 8:
    case 9:
        X = 0.4 * R * DayLength;
        Max = 4.0 * (Temp+273) * R;
        if (X > Max) X = Max;
        return (X);
        break;
    default:
        X = 0.2 * R * DayLength;
        Max = 5.0 * (Temp+273) * R;
        if (X > Max) X = Max;
        return (X);
        break;
    }
} /* end of Chart Daytime */

/*************************************************************************/

double ChartNighttime(short Atmosphere, double DayLength, double Temp)
{
    double X;
    double Max;

    switch (Atmosphere) {
    case 0:
        X = 20.0 * DayLength;
        Max = 0.80 * (Temp+273);
        if (X > Max) X = Max;
        return (-X);
        break;
    case 1:
        X = 15.0 * DayLength;
        Max = 0.70 * (Temp+273);
        if (X > Max) X = Max;
        return (-X);
        break;
    case 2:
    case 3:
    case 14:
        X = 8.0 * DayLength;
        Max = 0.50 * (Temp+273);
        if (X > Max) X = Max;
        return (-X);
        break;
    case 4:
    case 5:
        X = 3.0 * DayLength;
        Max = 0.30 * (Temp+273);
        if (X > Max) X = Max;
        return (-X);
        break;
    case 6:
    case 7:
        X = 1.0 * DayLength;
        Max = 0.15 * (Temp+273);
        if (X > Max) X = Max;
        return (-X);
        break;
    case 8:
    case 9:
        X = 0.5 * DayLength;
        Max = 0.10 * (Temp+273);
        if (X > Max) X = Max;
        return (-X);
        break;
    default:
        X = 0.2 * DayLength;
        Max = 0.05 * (Temp+273);
        if (X > Max) X = Max;
        return (-X);
        break;
    }
} /* end of Chart Nighttime */

/*************************************************************************/

double ChartOrbitEcc()
{
    int dieroll;

    dieroll = d6()+d6();

    switch (dieroll) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        return(0.0);
        break;
    case 8:
        return(0.005);
        break;
    case 9:
        return(0.010);
        break;
    case 10:
        return(0.015);
        break;
    case 11:
        return(0.020);
        break;
    default:
        dieroll = d6()+d6();

        switch (dieroll) {
        case 1:
            return(0.025);
            break;
        case 2:
            return(0.050);
            break;
        case 3:
            return(0.100);
            break;
        case 4:
            return(0.200);
            break;
        case 5:
            return(0.250);
            break;
        default:
            return(0.300);
            break;
        }
        break;
    }
} /* end of Orbit Eccentriciy */

/*************************************************************************/

double ChartAxialTilt()
{
    int dieroll;

    dieroll = d6()+d6();

    switch (dieroll) {
    case 2:
    case 3:
        return(-1.0 + d10());
        break;
    case 4:
    case 5:
        return(9.0 + d10());
        break;
    case 6:
    case 7:
        return(19.0 + d10());
        break;
    case 8:
    case 9:
        return(20.0 + d10());
        break;
    case 10:
    case 11:
        return(29.0 + d10());
        break;
    default:
        dieroll = d6();

        switch (dieroll) {
        case 1:
        case 2:
            return(49 + d10());
            break;
        case 3:
            return(59 + d10());
            break;
        case 4:
            return(69 + d10());
            break;
        case 5:
            return(79 + d10());
            break;
        default:
            return(90.00);
            break;
        }
        break;
    }
} /* end of Axial Tilt */

/*************************************************************************/

double CalcGaiaFactor(double l, double o, double g, double e)
{
    /* Calculate Ideal E */
    double Ideal;
    double TheFactor;

    Ideal = 288.00/l/o/g;

    if (Debug)
        printf ("     E is %f, Ideal E is %f\n", e, Ideal);
    TheFactor = (Ideal - e)/2;

    if (TheFactor > 0.0) {
        if (TheFactor > 0.1) TheFactor = 0.1;
    } else {
        if (TheFactor < -0.1) TheFactor = -0.1;
    }

    if (Debug)
        printf ("     The Factor is %f\n", TheFactor);

    return(e+TheFactor);
}

/*************************************************************************/

double PrintPlanet (char orbit, double range, Planet *p)
{
    double X;
    double PlanetMass;
    double PlanetDiam;
    double Grav;
    char   Units[4];

    printf ("                        Summer     Fall/Spring    Winter\n");
    printf ("     ");
    switch (p->size) {
    case 0:
        printf("Very Small ");
        X =   200 + Change(300);
        break;
    case 1:
        printf("Small      ");
        X =   800 + Change(800);
        break;
    case 2:
        printf("Small      ");
        X =  2400 + Change(1600);
        break;
    case 3:
        printf("Small      ");
        X =  4000 + Change(800);
        break;
    case 4:
        printf("Small      ");
        X =  5600 + Change(800);
        break;
    case 5:
        printf("Medium     ");
        X =  7200 + Change(800);
        break;
    case 6:
        printf("Medium     ");
        X =  8800 + Change(800);
        break;
    case 7:
        printf("Medium     ");
        X = 10400 + Change(800);
        break;
    case 8:
        printf("Large      ");
        X = 12000 + Change(800);
        break;
    case 9:
        printf("Large      ");
        X = 13600 + Change(800);
        break;
    case 10:
        printf("Large      ");
        X = 15200 + Change(800);
        break;
    }
    PlanetDiam = X;
    printf ("    %6.0f km  (%c)         Day/Night    Day/Night    Day/Night\n",
            PlanetDiam, p->size+'0');

    printf ("     ");
    switch (p->dense) {
    case 'L':
        printf("Low Density    ");
        break;
    case 'A':
        printf("Average Density");
        break;
    case 'H':
        printf("High Density   ");
        break;
    }
    PlanetMass = PlanetVolume(PlanetDiam, p->dense);
    Grav = PlanetGrav(PlanetDiam, p->dense);

    printf ("  %3.2f G           1  %4.0f/ %4.0f   %4.0f/ %4.0f   %4.0f/ %4.0f\n",
            Grav, p->summer[0], p->summer[1], p->fall[0], p->fall[1],
            p->winter[0], p->winter[1]);

    printf ("     ");
    switch (p->atmosphere) {
    case 0:
        printf("Vacuum       ");
        X = 0;
        break;
    case 1:
        printf("Vacumm       ");
        X = 0.00 + Change(0.01);
        break;
    case 2:
        printf("Very Thin (t)");
        X = 0.10 + Change(0.16);
        break;
    case 3:
        printf("Very Thin    ");
        X = 0.10 + Change(0.16);
        break;
    case 4:
        printf("Thin (taint) ");
        X = 0.42 + Change(0.14);
        break;
    case 5:
        printf("Thin         ");
        X = 0.42 + Change(0.14);
        break;
    case 6:
        printf("Standard     ");
        X = 0.70 + Change(0.40);
        break;
    case 7:
        printf("Standard (t) ");
        X = 0.70 + Change(0.40);
        break;
    case 8:
        printf("Dense        ");
        X = 1.50 + Change(0.50);
        break;
    case 9:
        printf("Dense (taint)");
        X = 1.50 + Change(0.50);
        break;
    case 10:
        printf("Exotic       ");
        X = 0    + Change(3.00);
        break;
    case 11:
        printf("Corrosive    ");
        X = 0    + Change(3.00);
        break;
    case 12:
        printf("Insidious    ");
        X = 0    + Change(3.00);
        break;
    default:
        printf("Exotic       ");
        X = 0    + Change(3.00);
        break;
    }
    printf ("    %3.2f     (%c)     2  %4.0f/ %4.0f   %4.0f/ %4.0f   %4.0f/ %4.0f\n",
            X, ((p->atmosphere < 10) ? p->atmosphere+'0': p->atmosphere+'8'),
            p->summer[2], p->summer[3], p->fall[2], p->fall[3],
            p->winter[2], p->winter[3]);

    printf ("     ");
    switch (p->hydro) {
    case 0:
        printf("Desert      ");
        break;
    case 1:
        printf("Dry World   ");
        break;
    case 2:
        printf("Dry World   ");
        break;
    case 3:
        printf("Wet (30%%)   ");
        break;
    case 4:
        printf("Wet (40%%)   ");
        break;
    case 5:
        printf("Wet (50%%)   ");
        break;
    case 6:
        printf("Wet (60%%)   ");
        break;
    case 7:
        printf("Wet (70%%)   ");
        break;
    case 8:
        printf("Wet (80%%)   ");
        break;
    case 9:
        printf("Wet (90%%)   ");
        break;
    default:
        printf("Water World ");
        break;
    }
    printf ("      %3d %%   (%c)     3  %4.0f/ %4.0f   %4.0f/ %4.0f   %4.0f/ %4.0f\n",
            p->hydro*10, p->hydro+'0',
            p->summer[4], p->summer[5], p->fall[4], p->fall[5],
            p->winter[4], p->winter[5]);

    printf ("     ");
    switch (p->maxpop) {
    case 0:
        printf("None     ");
        break;
    case 1:
        printf("Very Poor");
        break;
    case 2:
        printf("Very Poor");
        break;
    case 3:
        printf("Very Poor");
        break;
    case 4:
        printf("Very Poor");
        break;
    case 5:
        printf("Poor     ");
        break;
    case 6:
        printf("Poor     ");
        break;
    case 7:
        printf("Good     ");
        break;
    case 8:
        printf("Good     ");
        break;
    case 9:
        printf("Habitable");
        break;
    default:
        printf("Habitable");
        break;
    }
    printf ("                 (%c)     4  %4.0f/ %4.0f   %4.0f/ %4.0f   %4.0f/ %4.0f\n",
            p->maxpop+'0',
            p->summer[6], p->summer[7], p->fall[6], p->fall[7],
            p->winter[6], p->winter[7]);

    X = range * 149.6;
    printf ("     Range       ");
    if (X < 0.01) {
        X = X * 100000000;
        strcpy (Units, "km ");
    } else
        strcpy (Units, "Mkm");

    if (X < 10.0)
        printf ("%9.2f", X);
    else if(X < 100.0)
        printf ("%9.1f", X);
    else
        printf ("%9.0f", X);
    printf (" %s         5  %4.0f/ %4.0f   %4.0f/ %4.0f   %4.0f/ %4.0f\n",
            Units,
            p->summer[8], p->summer[9], p->fall[8], p->fall[9],
            p->winter[8], p->winter[9]);

    X = p->orbit;
    printf ("     Orbit         ");
    if (X < 3.0)
        printf ("%7.2f Days ", X*365.25);
    else
        printf ("%7.2f Years", X);
    printf ("       6  %4.0f/ %4.0f   %4.0f/ %4.0f   %4.0f/ %4.0f\n",
            p->summer[10], p->summer[11], p->fall[10], p->fall[11],
            p->winter[10], p->winter[11]);

    printf ("     Rotation        %5.2f Hours       7  %4.0f/ %4.0f   %4.0f/ %4.0f   %4.0f/ %4.0f\n",
            p->rotation,
            p->summer[12], p->summer[13], p->fall[12], p->fall[13],
            p->winter[12], p->winter[13]);

    printf ("     Orbit Ecc       %5.3f             8  %4.0f/ %4.0f   %4.0f/ %4.0f   %4.0f/ %4.0f\n",
            p->ecc,
            p->summer[14], p->summer[15], p->fall[14], p->fall[15],
            p->winter[14], p->winter[15]);

    printf ("     Axial Tilt      %5.1f Degrees     9  %4.0f/ %4.0f   %4.0f/ %4.0f   %4.0f/ %4.0f\n",
            p->tilt,
            p->summer[16], p->summer[17], p->fall[16], p->fall[17],
            p->winter[16], p->winter[17]);

    if (FDegrees)
        printf ("     Temperature   %7.2f F          ", CtoF(p->temp));
    else
        printf ("     Temperature   %7.2f Celsius    ", p->temp);

    printf ("10  %4.0f/ %4.0f   %4.0f/ %4.0f   %4.0f/ %4.0f\n",
            p->summer[18], p->summer[19], p->fall[18], p->fall[19],
            p->winter[18], p->winter[19]);

    printf ("                                      11  %4.0f/ %4.0f   %4.0f/ %4.0f   %4.0f/ %4.0f\n",
            p->summer[20], p->summer[21], p->fall[20], p->fall[21],
            p->winter[20], p->winter[21]);

    printf ("\n");

    return (PlanetDiam);

} /* end of Print Planets */

/* Flesh Out Satelite should ideally be combined with Flesh Out, oh well */

char FleshOutSat(short WorldSize, double Range, char Orbit, char Object,
                 char OrbitType, char StarType, double PlanetDiam, char PlanetDense,
                 double Luminosity, char HZone)
{
    int i, k;
    int dieroll;

    short Size;
    short Atmosphere;
    short Hydro;
    short Pop;
    short MaxPop;
    char  Dense;

    double PlanetMass;
    double OrbitPeriod;
    double RotatePeriod;
    double OrbitNum;
    double OrbitRange;
    double MeanTemp;
    double A;                   /* Axial Tilt */
    double M;                   /* Stellar Mass */
    double D;                   /* Orbital Distance */
    double L;                   /* Luminosity */
    double O;                   /* Orbit Factor/Eccentrisity */
    double G;                   /* Greenhouse Factor */
    double E;                   /* Energy Absorbtion Factor */
    double X;
    double DayPlus;
    double NightMinus;

    printf ("  %02d%c  Satelite     ", Orbit, OrbitType);
    /* Size */
    if (WorldSize == -1) /* Small Gas Giant */
        dieroll = d6()+d6()-6;
    else if (WorldSize == -2) /* Large Gas Giant */
        dieroll = d6()+d6()-4;
    else
        dieroll = d6() - 3;

    if (dieroll == 0) {
        printf ("    Ring\n\n");
        return (0);
    }

    if ((dieroll >= WorldSize) && (WorldSize >= 0))
        dieroll = WorldSize - 1;

    if (dieroll < 1) dieroll = 0;
    Size = dieroll;
    /* Density */
    dieroll = d6();
    if (OrbitType == 'O') dieroll -= 2;
    if (dieroll < 2) Dense = 'L';
    else if (dieroll > 4) Dense = 'H';
    else Dense = 'A';
    /* Atmosphere */
    dieroll = d6()+d6()-7+Size;
    if (OrbitType == 'I') dieroll -= 2;
    if (OrbitType == 'O') dieroll -= 4;
    if (Size == 0) dieroll = 0;
    if (dieroll < 1) dieroll = 0;
    Atmosphere = dieroll;
    /* Hydrographics */
    dieroll = d6()+d6()-2;
    if (OrbitType == 'I') dieroll = 0;
    if (OrbitType == 'O') dieroll -= 2;
    if (Atmosphere < 2) dieroll -= 1;
    if (Atmosphere < 4) dieroll -= 1;
    if (Size == 0) dieroll -= 4;
    if (Size == 1) dieroll -= 3;
    if (Size == 2) dieroll -= 1;
    if (dieroll < 1) dieroll = 0;
    if (dieroll > 10) dieroll = 10;
    Hydro = dieroll;
    /* Max Population */
    MaxPop = 10;
    if (Size < 4) MaxPop -= 1;
    if (Size < 7) MaxPop -= 1;
    if (Atmosphere == 5) MaxPop -= 1;
    if (Atmosphere == 7) MaxPop -= 1;
    if (Atmosphere == 9) MaxPop -= 1;
    if (Atmosphere == 4) MaxPop -= 2;
    if (Atmosphere > 12) MaxPop -= 3;
    if (Hydro < 3)   MaxPop -= 1;
    if (Hydro == 10) MaxPop -= 1;
    if (Hydro == 0)  MaxPop -= 2;
    if (Atmosphere < 4) MaxPop = 0;
    if (Atmosphere > 9) MaxPop = 0;
    if (Atmosphere == 10) MaxPop = 0;
    if (Atmosphere == 11) MaxPop = 0;
    if (Atmosphere == 12) MaxPop = 0;
    if (OrbitType != 'H') {
        if (Orbit > HZone) MaxPop = MaxPop - ((Orbit-HZone)*2);
        else MaxPop = MaxPop - ((HZone-Orbit)*2);
    }
    if (MaxPop < 0) MaxPop = 0;

    /* Satelite Characteristics */

    PlanetMass = PlanetVolume(PlanetDiam, PlanetDense);
    M = PlanetMass;

    /* 149600000 km (149.6 Mkm) in one AU */
    OrbitNum = ChartSatOrbit();
    OrbitRange = ((PlanetDiam/2) * OrbitNum);
    D = OrbitRange;
    if (Debug) printf ("\n     Orbit Range is %f km\n", D);
    OrbitRange = OrbitRange / 100000000;
    OrbitRange = OrbitRange / 149.6;
    if (Debug) printf ("     Orbit Number is %f  Range is %f\n",
                           OrbitNum, OrbitRange);

    D = D/400000;  /* ? */

    X = (D*D*D*793.64)/M; /* ? */

    if (Debug) printf ("     M is %4.2f  D is %5.2f   X is %7.3f\n", M, D, X);

    OrbitPeriod = sqrt(X);

    /* Convert Years to Days */
    OrbitPeriod = OrbitPeriod / 365.25;

    X = (4 * (d6()+d6()-2)) + 5;  /* + (M/D) */;
    RotatePeriod = X;

    L = Luminosity;

    O  = HABITNUM / sqrt(Range);
    {
        char AtmosCode;
        if (Atmosphere < 4) AtmosCode = 0;
        else if (Atmosphere < 10) AtmosCode = 1;
        else if (Atmosphere < 16) AtmosCode = 2;
        else AtmosCode = 3;

        if (OrbitType == 'H')
            E = EnergyAbsorbHZ[Hydro][AtmosCode];
        else
            E = EnergyAbsorbNHZ[Hydro][AtmosCode];
    }
    G = GreenHouse[Atmosphere];

    if ((GaiaFactor) && (MaxPop > 5))
        E = CalcGaiaFactor(L, O, G, E);

    X = ((L*O)+ComLumAddFromPrim)*E*G;

    if (Debug) {
        if (ComLumAddFromPrim > 0.0)
            printf ("     Adding %6.2f to L*O\n", ComLumAddFromPrim);
        printf ("     L is %4.2f  O is %6.3f   E is %4.3f  G is %3.2f (%6.2f)\n",
                L, O, E, G, X);
    }

    MeanTemp = X - 273;

    DayPlus = ChartDaytime(Atmosphere, RotatePeriod/2,L, Range, MeanTemp);
    NightMinus = ChartNighttime(Atmosphere, RotatePeriod/2, MeanTemp);

    A = ChartAxialTilt();
    /* k is tilt effect column */
    if (A == 0.0) k = 0;
    else if (A < 6.0) k = 1;
    else if (A < 11.0) k = 2;
    else if (A < 16.0) k = 3;
    else if (A < 21.0) k = 4;
    else if (A < 26.0) k = 5;
    else if (A < 31.0) k = 6;
    else if (A < 36.0) k = 7;
    else if (A < 46.0) k = 8;
    else if (A < 61.0) k = 9;
    else if (A < 85.0) k = 10;
    else k = 11;

    O = ChartOrbitEcc();

    for (i=0; i<22; ++i) {
        if ((i%2) == 0) X = DayPlus;
        else X = NightMinus;

        CurrPlanet.summer[i] =
            MeanTemp + LatitudeMods[i/2][Size] + /* (O*30) + */
            ((0.6*A) * AxialTiltEffects[i/2][k]) + X;
        CurrPlanet.fall[i] = MeanTemp + LatitudeMods[i/2][Size] + X;
        CurrPlanet.winter[i] =
            MeanTemp + LatitudeMods[i/2][Size] - /* (O*30) - */
            (A * AxialTiltEffects[i/2][k]) + X;
        /* Convert to Faren here as it is easier */
        if (FDegrees) {
            CurrPlanet.summer[i] = CtoF(CurrPlanet.summer[i]);
            CurrPlanet.fall[i] = CtoF(CurrPlanet.fall[i]);
            CurrPlanet.winter[i] = CtoF(CurrPlanet.winter[i]);
        }
    }

    CurrPlanet.size = Size;
    CurrPlanet.dense = Dense;
    CurrPlanet.atmosphere = Atmosphere;
    CurrPlanet.hydro = Hydro;
    CurrPlanet.maxpop = MaxPop;
    CurrPlanet.orbit = OrbitPeriod;
    CurrPlanet.tilt = A;
    CurrPlanet.ecc = O;
    CurrPlanet.rotation = RotatePeriod;
    CurrPlanet.temp = MeanTemp;
    PrintPlanet (Orbit, OrbitRange, &CurrPlanet);

    return (MaxPop);

} /* end of FleshOutSat */

char FleshOut(char Orbit, double Range, char Object, char OrbitType,
              char StarType, double StellarMass, double Luminosity,
              char HZone)
{
    int dieroll;

    short i, k;
    short Size;
    short Atmosphere;
    short Hydro;
    short Pop;
    short MaxPop = 0;
    char  Dense;
    int   NumSats = 0;
    double Diam = 0.0;

    double OrbitPeriod;
    double RotatePeriod;
    double MeanTemp;
    double A;                   /* Axial Tilt */
    double M;                   /* Stellar Mass */
    double D;                   /* Orbital Distance */
    double L;                   /* Luminosity */
    double O;                   /* Orbit Factor/Eccentrisity */
    double G;                   /* Greenhouse Factor */
    double E;                   /* Energy Absorbtion Factor */
    double X;
    double DayPlus;
    double NightMinus;

    /* Everything has an orbit period */
    M = StellarMass;
    D = Range;

    X = (D*D*D)/M;

    if (Debug)
        printf ("     M is %4.2f  D is %5.2f   X is %7.3f\n", M, D, X);

    OrbitPeriod = sqrt(X);

    switch (Object) {
    case 'E':
    case ' ':
        if (Debug) printf ("%02d%c  Empty Orbit\n", Orbit, OrbitType);
        break;
    case 'G':
        if (d6() < 3) {
            printf ("%02d%c  Gas Giant\n", Orbit, OrbitType);
            NumSats = d6()+d6()-4;
            Size = -1;
            Diam = 20000 + Change(20000);
            printf ("     Small          %6.0f km\n", Diam);
        } else {
            printf ("%02d%c  Gas Giant\n", Orbit, OrbitType);
            NumSats = d6()+d6();
            Size = -2;
            Diam = 60000 + Change(100000);
            printf ("     Large          %6.0f km\n", Diam);
        }

        printf ("     Range       ");

        if (Range < 10.0)
            printf ("%9.2f Mkm\n", Range);
        else if(Range < 100.0)
            printf ("%9.1f Mkm\n", Range);
        else
            printf ("%9.0f Mkm\n", Range);

        /* Orbital Period for the Gas Giant */
        if (OrbitPeriod < 3.0)
            printf ("     Orbit         %7.2f Days\n\n", OrbitPeriod*365.25);
        else
            printf ("     Orbit         %7.2f Years\n\n", OrbitPeriod);

        /* Satellites */
        if (NumSats < 1) NumSats = 0;

        for (i=NumSats; i > 0; --i) {
            k = FleshOutSat(Size, Range, Orbit, Object, OrbitType,
                            StarType, Diam, 'L', Luminosity, HZone);
            if (k > MaxPop) MaxPop = k;
        }
        break;
    case 'C':
    case 'W':
        if (Object == 'C')
            printf ("%02d%c  Captured Planet",
                    Orbit, OrbitType);
        else
            printf ("%02d%c  Planet         ",
                    Orbit, OrbitType);
        /* Size */
        dieroll = d6()+d6()-2;
        if (Orbit == 0) dieroll -= 5;
        if (Orbit == 1) dieroll -= 4;
        if (Orbit == 2) dieroll -= 2;
        if (StarType == 'M') dieroll -= 2;
        if (dieroll < 1) dieroll = 0;
        Size = dieroll;
        /* Density */
        dieroll = d6();
        if (OrbitType == 'O') dieroll -= 2;
        if (dieroll < 2)
            Dense = 'L';
        else if (dieroll > 4)
            Dense = 'H';
        else
            Dense = 'A';
        /* Atmosphere */
        dieroll = d6()+d6()-7+Size;
        if (OrbitType == 'I') dieroll -= 2;
        if (OrbitType == 'O') dieroll -= 4;
        if (Size == 0) dieroll = 0;
        if (dieroll < 1) dieroll = 0;
        Atmosphere = dieroll;
        /* Hydrographics */
        dieroll = d6()+d6()-2;
        if (OrbitType == 'I') dieroll = 0;
        if (OrbitType == 'O') dieroll -= 2;
        if (Atmosphere < 2) dieroll -= 1;
        if (Atmosphere < 4) dieroll -= 1;
        if (Size == 0) dieroll -= 4;
        if (Size == 1) dieroll -= 3;
        if (Size == 2) dieroll -= 1;
        if (dieroll < 1) dieroll = 0;
        if (dieroll > 10) dieroll = 10;
        Hydro = dieroll;
        /* Max Population */
        MaxPop = 10;
        if (Size < 4) MaxPop -= 1;
        if (Size < 7) MaxPop -= 1;
        if (Atmosphere == 5) MaxPop -= 1;
        if (Atmosphere == 7) MaxPop -= 1;
        if (Atmosphere == 9) MaxPop -= 1;
        if (Atmosphere == 4) MaxPop -= 2;
        if (Atmosphere > 12) MaxPop -= 3;
        if (Hydro < 3)   MaxPop -= 1;
        if (Hydro == 10) MaxPop -= 1;
        if (Hydro == 0)  MaxPop -= 2;
        if (Atmosphere < 4) MaxPop = 0;
        if (Atmosphere == 10) MaxPop = 0;
        if (Atmosphere == 11) MaxPop = 0;
        if (Atmosphere == 12) MaxPop = 0;
        if (OrbitType != 'H') {
            if (Orbit > HZone) MaxPop = MaxPop - ((Orbit-HZone)*2);
            else MaxPop = MaxPop - ((HZone-Orbit)*2);
        }
        if (MaxPop < 0) MaxPop = 0;

        /* Planetary Characteristics */
        X = (4 * (d6()+d6()-2)) + 5 + (M/D);
        RotatePeriod = X;
        if (Debug) {
            if (RotatePeriod < 90.0)
                printf ("     Rotational Period is %7.2f hours\n", RotatePeriod);
            else
                printf ("     Rotational Period is %7.3f days\n", RotatePeriod/24);
        }

        L = Luminosity;

        O  = HABITNUM / sqrt(D);
        {
            char AtmosCode;
            if (Atmosphere < 4) AtmosCode = 0;
            else if (Atmosphere < 10) AtmosCode = 1;
            else if (Atmosphere < 16) AtmosCode = 2;
            else AtmosCode = 3;

            if (OrbitType == 'H')
                E = EnergyAbsorbHZ[Hydro][AtmosCode];
            else
                E = EnergyAbsorbNHZ[Hydro][AtmosCode];
        }
        G = GreenHouse[Atmosphere];

        if ((GaiaFactor) && (MaxPop > 5))
            E = CalcGaiaFactor(L, O, G, E);

        X = ((L*O)+ComLumAddFromPrim)*E*G;

        if (Debug) {
            if (ComLumAddFromPrim > 0.0)
                printf ("     Adding %6.2f to L*O\n", ComLumAddFromPrim);
            printf ("     L is %4.2f  O is %6.3f   E is %4.3f  G is %3.2f (%6.2f)\n",
                    L, O, E, G, X);
        }

        MeanTemp = X - 273;

        if (Debug)
            printf ("     Mean Temperature %7.3f Celsius (%7.3fF)\n", MeanTemp,
                    (MeanTemp*1.8)+32);

        DayPlus = ChartDaytime(Atmosphere, RotatePeriod/2,L, D, MeanTemp);
        NightMinus = ChartNighttime(Atmosphere, RotatePeriod/2, MeanTemp);

        if (Debug)
            printf ("     Day Plus %7.3f    Night Minus %7.3f\n",
                    DayPlus, NightMinus);

        A = ChartAxialTilt();
        /* k is tilt effect column */
        if (A == 0.0) k = 0;
        else if (A < 6.0) k = 1;
        else if (A < 11.0) k = 2;
        else if (A < 16.0) k = 3;
        else if (A < 21.0) k = 4;
        else if (A < 26.0) k = 5;
        else if (A < 31.0) k = 6;
        else if (A < 36.0) k = 7;
        else if (A < 46.0) k = 8;
        else if (A < 61.0) k = 9;
        else if (A < 85.0) k = 10;
        else k = 11;

        O = ChartOrbitEcc();

        for (i=0; i<22; ++i) {
            if ((i%2) == 0) X = DayPlus;
            else X = NightMinus;

            CurrPlanet.summer[i] =
                MeanTemp + LatitudeMods[i/2][Size] + (O*30) +
                ((0.6*A) * AxialTiltEffects[i/2][k]) + X;
            CurrPlanet.fall[i] = MeanTemp + LatitudeMods[i/2][Size] + X;
            CurrPlanet.winter[i] =
                MeanTemp + LatitudeMods[i/2][Size] - (O*30) -
                (A * AxialTiltEffects[i/2][k]) + X;
            /* Convert to Faren here as it is easier */
            if (FDegrees) {
                CurrPlanet.summer[i] = CtoF(CurrPlanet.summer[i]);
                CurrPlanet.fall[i] = CtoF(CurrPlanet.fall[i]);
                CurrPlanet.winter[i] = CtoF(CurrPlanet.winter[i]);
            }
        }

        CurrPlanet.size = Size;
        CurrPlanet.dense = Dense;
        CurrPlanet.atmosphere = Atmosphere;
        CurrPlanet.hydro = Hydro;
        CurrPlanet.maxpop = MaxPop;
        CurrPlanet.orbit = OrbitPeriod;
        CurrPlanet.tilt = A;
        CurrPlanet.ecc = O;
        CurrPlanet.rotation = RotatePeriod;
        CurrPlanet.temp = MeanTemp;

        Diam = PrintPlanet (Orbit, Range, &CurrPlanet);

        /* Satellites */
        if (Size == 0)
            NumSats = 0;
        else if (Object == 'C')
            NumSats = 0;
        else
            NumSats = d6()-3;

        if (NumSats < 1) NumSats = 0;

        for (i=NumSats; i > 0; --i) {
            k = FleshOutSat(Size, Range, Orbit, Object, OrbitType,
                            StarType, Diam, Dense, Luminosity, HZone);
            if (k > MaxPop) MaxPop = k;
        }
        break;
    case 'P':
        printf ("%02d%c  Asteroid Field\n", Orbit, OrbitType);
        printf ("     Range       ");

        if (Range < 10.0)
            printf ("%9.2f Mkm\n", Range);
        else if(Range < 100.0)
            printf ("%9.1f Mkm\n", Range);
        else
            printf ("%9.0f Mkm\n", Range);
        if (OrbitPeriod < 3.0)
            printf ("     Orbit         %7.2f Days\n\n", OrbitPeriod*365.25);
        else
            printf ("     Orbit         %7.2f Years\n\n", OrbitPeriod);
        break;
    case 'S':
        printf ("%02d%c  Companion Star\n", Orbit, OrbitType);
        printf ("     Range       ");

        if (Range < 10.0)
            printf ("%9.2f Mkm\n", Range);
        else if(Range < 100.0)
            printf ("%9.1f Mkm\n", Range);
        else
            printf ("%9.0f Mkm\n", Range);

        if (OrbitPeriod < 3.0)
            printf ("     Orbit         %7.2f Days\n\n", OrbitPeriod*365.25);
        else
            printf ("     Orbit         %7.2f Years\n\n", OrbitPeriod);

        break;
    }

    return (MaxPop);

} /* end of FleshOut */

/*************************************************************************/
/*                                                                       */
/*                                 MAIN                                  */
/*                                                                       */
/*************************************************************************/

int main (int argc, char *argv[])
{
    int  i, k, hitcount;

    int  DieRoll;

    short NumOrbits;
    char  GasGiants, Planetoids, EmptyOrbits, CaptPlanets;

    /* Initialize Random Number Generator */
    SeedValue = time(NULL);
    srand(SeedValue);

    for (i = 1; i < argc; ++i) {
        if (argv[i][0] != '-')
            printf ("Unknown parameter %s\n", argv[i]);
        else switch (argv[i][1]) {
            case 's':
                SeedValue = atol(&argv[i][2]);
                break;
            case 'D':
                Debug = 1;
                break;
            case 'g':
                GaiaFactor = 0;
                break;
            case 'f':
                FDegrees = 1;
                break;
            case 't':
                TravInfo   = 1;
                break;
            case 'c':
                Collapse   = 1;
                break;
            case 'O':
            case 'B':
            case 'A':
            case 'F':
            case 'G':
            case 'K':
            case 'M':
                PresetStar = 1;
                SystemNature = 'S';

                PrimStarType = argv[i][1];
                PrimDecClass = argv[i][2];
                PrimLumClass = argv[i][3];
                if (strlen(argv[i]) > 4) {
                    SystemNature = 'B';
                    ComStarType = argv[i][4];
                    ComDecClass = argv[i][5];
                    ComLumClass = argv[i][6];
                }
                break;
            default:
                printf ("Unknown parameter %s\n", argv[i]);
                break;
            }
    }

    srand(SeedValue);

    if (PresetStar == 0) {
        SystemNature = ChartSysNat();

        PrimStarType = ChartPrimStarType ();
        PrimLumClass = ChartPrimLumClass(PrimStarType);
        PrimDecClass = ChartDecClass (PrimStarType,
                                      PrimLumClass);

    }

    PrimStellarMass = ChartStellarMass (PrimStarType, PrimDecClass,
                                        PrimLumClass);
    PrimLuminosity = ChartLuminosity (PrimStarType, PrimDecClass,
                                      PrimLumClass);

    printf ("System is a %s\n", PrintSysNat(SystemNature));
    printf ("Primary Star is a %c%c (%s)\n", PrimStarType,
            PrimDecClass, PrintLumClass(PrimLumClass));
    printf ("  Stellar Mass is %5.2f", PrimStellarMass);
    printf ("  Luminosity is %5.2f\n", PrimLuminosity);

    /* Initialize System and Worlds */
    k = 0;
    HZone = -2;
    for (i=0; i<MAX_ORBITS; ++i) {
        Orbits[i].Range = ChartOrbitRange(i);
        Orbits[i].OrbitType = ChartOrbitType(
                                  Orbits[i].Range, PrimLuminosity);
        Orbits[i].Occupied = ' ';
        if (Orbits[i].OrbitType == 'H') HZone = i;
        if ((Orbits[i].OrbitType == 'O') && (HZone == -2)) HZone = i-1;
    }
    if (HZone == -2) HZone = 10;

    NumOrbits = ChartNumOrbits(PrimStarType,
                               PrimLumClass);

    EmptyOrbits = ChartNumEmptyOrbits(PrimStarType);
    if (Debug) printf ("%d Empty Orbits\n", EmptyOrbits);
    i = EmptyOrbits;
    hitcount = 0;
    while (i > 0) {
        /* Place Empty Orbits */
        DieRoll = d6()+d6()-2;
        if (Orbits[DieRoll].Occupied == ' ') {
            Orbits[DieRoll].Occupied = 'E';
            i--;
        } else {
            ++hitcount;
            if (hitcount > 100) {
                printf ("Cannot place empty orbit\n");
                break;
            }
        }
    }

    CaptPlanets = ChartNumCaptPlanets(PrimStarType);
    if (Debug) printf ("%d Captured Planets\n", CaptPlanets);
    i = CaptPlanets;
    hitcount = 0;
    while (i > 0) {
        /* Place Captive Planets */
        DieRoll = d6()+d6();
        if (Orbits[DieRoll].Occupied == ' ') {
            Orbits[DieRoll].Occupied = 'C';
            i--;
        } else {
            ++hitcount;
            if (hitcount > 100) {
                printf ("Cannot place planetoid\n");
                break;
            }
        }
    }

    GasGiants = ChartNumGasGiants();
    if (Debug) printf ("%d Gas Giants\n", GasGiants);
    i = GasGiants;
    while (i > 0) {
        /* Place Gas Giants */

        DieRoll = d6()+d6()-3+HZone;

        if (DieRoll >= MAX_ORBITS) DieRoll = MAX_ORBITS-1;

        if (Orbits[DieRoll].Occupied == ' ') {
            Orbits[DieRoll].Occupied = 'G';
            i--;
        } else {
            ++hitcount;
            if (hitcount > 100) {
                printf ("Cannot place Gas Giants (%d)\n", DieRoll);
                break;
            }
        }
    }

    Planetoids = ChartNumPlanetoids();
    if (Debug) printf ("%d Planetoids\n", Planetoids);
    i = Planetoids;
    while (i > 0) {
        /* Place Planetoids */
        DieRoll = d6()+d6()-1;
        if (Orbits[DieRoll].Occupied == ' ') {
            Orbits[DieRoll].Occupied = 'P';
            i--;
        } else if ((Orbits[DieRoll].Occupied == 'G') &&
                   (Orbits[DieRoll-1].Occupied == ' ')) {
            Orbits[DieRoll-1].Occupied = 'P';
            i--;
        } else {
            ++hitcount;
            if (hitcount > 100) {
                printf ("Cannot place Planetoid\n");
                break;
            }
        }
    }

    for (i=0; i<MAX_ORBITS; ++i) {
        if (i > NumOrbits) {
            if (Orbits[i].Occupied != 'C')
                Orbits[i].OrbitType = ' ';
        }
    }

    if (SystemNature != 'S') {

        if (PresetStar == 0) {
            ComStarType = ChartComStarType ();
            ComLumClass = ChartComLumClass(ComStarType);
            ComDecClass = ChartDecClass (ComStarType,
                                         ComLumClass);
        }

        ComOrbit = ChartComOrbit();

        printf ("Secondary Star is a %c%c (%s)",
                ComStarType, ComDecClass,
                PrintLumClass(ComLumClass));
        if (ComOrbit == 99)
            printf (" in Far Orbit");
        else
            printf (" in Orbit %2d\n", ComOrbit);
        ComStellarMass = ChartStellarMass (ComStarType, ComDecClass,
                                           ComLumClass);
        printf ("  Stellar Mass is %5.2f", ComStellarMass);
        ComLuminosity = ChartLuminosity (ComStarType, ComDecClass,
                                         ComLumClass);
        printf ("  Luminosity is %5.2f\n", ComLuminosity);

        if (ComOrbit < 99) {
            /* 1 and 2 do not follow the general rule */
            if (ComOrbit > 2)
                for (i=(ComOrbit/2)+1;
                     i < ComOrbit; ++i) {
                    Orbits[i].OrbitType = 'S';
                    Orbits[i].Occupied = ' ';
                }
            Orbits[ComOrbit].Occupied = 'S';
            Orbits[ComOrbit].OrbitType = 'S';
            Orbits[ComOrbit+1].OrbitType = 'S';
            Orbits[ComOrbit+1].Occupied = ' ';
        }

    }

    printf ("\nPRIMARY SYSTEM\n");
    if (Debug) printf ("Habitable Zone is %d   ", HZone);
    if (Debug) printf ("Number of Orbits %d\n", NumOrbits);

    /* Flesh Out Worlds */
    for (i=0; i<MAX_ORBITS; ++i) {
        switch (Orbits[i].OrbitType) {
        case 'X':
            Orbits[i].Occupied = ' ';
            break;
        case ' ':
            Orbits[i].Occupied = ' ';
            break;
        case 'I':
            if (Orbits[i].Occupied == ' ')
                Orbits[i].Occupied = 'W';
            break;
        case 'H':
            if (Orbits[i].Occupied == ' ')
                Orbits[i].Occupied = 'W';
            break;
        case 'O':
            if (Orbits[i].Occupied == ' ')
                Orbits[i].Occupied = 'W';
            break;
        }

        k = FleshOut(i, Orbits[i].Range, Orbits[i].Occupied,
                     Orbits[i].OrbitType, PrimStarType, PrimStellarMass,
                     PrimLuminosity, HZone);
        if (k > SystemHabitability) SystemHabitability = k;
    }

    if (SystemNature != 'S') {
        printf ("\nSECONDARY SYSTEM\n");

        ComLumAddFromPrim = HABITNUM / sqrt(Orbits[ComOrbit].Range);
        if (Debug) printf ("Primary Star adds %6.2f\n", ComLumAddFromPrim);

        /* Initialize System and Worlds */

        k = 0;
        HZone = -2;
        for (i=0; i<MAX_ORBITS; ++i) {
            ComOrbits[i].Range = ChartOrbitRange(i);
            ComOrbits[i].OrbitType = ChartOrbitType(
                                         ComOrbits[i].Range, ComLuminosity);
            ComOrbits[i].Occupied = ' ';
            if (ComOrbits[i].OrbitType == 'H') HZone = i;
            if ((ComOrbits[i].OrbitType == 'O') && (HZone == -2)) HZone = i-1;
        }
        if (HZone == -2) HZone = 10;

        if (Debug) printf ("Habitable Zone is %d   ", HZone);

        NumOrbits = ChartNumOrbits(ComStarType,
                                   ComLumClass);
        if (NumOrbits > (ComOrbit/2)) NumOrbits = (ComOrbit/2);

        if (ComOrbit == 0) /* No Satelites */
            ComOrbits[0].OrbitType = 'X';

        if (Debug) printf ("Number of Orbits %d\n", NumOrbits);

        EmptyOrbits = ChartNumEmptyOrbits(ComStarType);
        if (Debug) printf ("%d Empty Orbits\n", EmptyOrbits);
        i = EmptyOrbits;
        hitcount = 0;
        while (i > 0) {
            /* Place Empty Orbits */
            DieRoll = d6()+d6()-2;
            if (ComOrbits[DieRoll].Occupied == ' ') {
                ComOrbits[DieRoll].Occupied = 'E';
                i--;
            } else {
                ++hitcount;
                if (hitcount > 100) {
                    printf ("Cannot place empty orbit\n");
                    break;
                }
            }
        }

        CaptPlanets = ChartNumCaptPlanets(ComStarType);
        if (Debug) printf ("%d Captured Planets\n", CaptPlanets);
        i = CaptPlanets;
        hitcount = 0;
        while (i > 0) {
            /* Place Captive Planets */
            DieRoll = d6()+d6();
            if (ComOrbits[DieRoll].Occupied == ' ') {
                ComOrbits[DieRoll].Occupied = 'C';
                i--;
            } else {
                ++hitcount;
                if (hitcount > 100) {
                    printf ("Cannot place planetoid\n");
                    break;
                }
            }
        }

        GasGiants = ChartNumGasGiants();
        if (Debug) printf ("%d Gas Giants\n", GasGiants);
        i = GasGiants;
        while (i > 0) {
            /* Place Gas Giants */

            DieRoll = d6()+d6()-3+HZone;

            if (DieRoll >= MAX_ORBITS) DieRoll = MAX_ORBITS-1;

            if (ComOrbits[DieRoll].Occupied == ' ') {
                ComOrbits[DieRoll].Occupied = 'G';
                i--;
            } else {
                ++hitcount;
                if (hitcount > 100) {
                    printf ("Cannot place Gas Giants (%d)\n", DieRoll);
                    break;
                }
            }
        }

        Planetoids = ChartNumPlanetoids();
        if (Debug) printf ("%d Planetoids\n", Planetoids);
        i = Planetoids;
        while (i > 0) {
            /* Place Planetoids */
            DieRoll = d6()+d6()-1;
            if (ComOrbits[DieRoll].Occupied == ' ') {
                ComOrbits[DieRoll].Occupied = 'P';
                i--;
            } else if ((ComOrbits[DieRoll].Occupied == 'G') &&
                       (ComOrbits[DieRoll-1].Occupied == ' ')) {
                ComOrbits[DieRoll-1].Occupied = 'P';
                i--;
            } else {
                ++hitcount;
                if (hitcount > 100) {
                    printf ("Cannot place Planetoid\n");
                    break;
                }
            }
        }
        /* Flesh Out Worlds */
        for (i=0; i<MAX_ORBITS; ++i) {
            if (i > NumOrbits) {
                ComOrbits[i].OrbitType = ' ';
            }
            switch (ComOrbits[i].OrbitType) {
            case 'X':
                ComOrbits[i].Occupied = ' ';
                break;
            case ' ':
                ComOrbits[i].Occupied = ' ';
                break;
            case 'I':
                if (ComOrbits[i].Occupied == ' ')
                    ComOrbits[i].Occupied = 'W';
                break;
            case 'H':
                if (ComOrbits[i].Occupied == ' ')
                    ComOrbits[i].Occupied = 'W';
                break;
            case 'O':
                if (ComOrbits[i].Occupied == ' ')
                    ComOrbits[i].Occupied = 'W';
                break;
            }

            k = FleshOut(i, ComOrbits[i].Range, ComOrbits[i].Occupied,
                         ComOrbits[i].OrbitType, ComStarType, ComStellarMass,
                         ComLuminosity, HZone);
            if (k > SystemHabitability) SystemHabitability = k;
        }
    }

    printf ("\nSystem Habitability is %d\n", SystemHabitability);
    printf ("System ID Number is %ld\n", SeedValue);

} /* end of main */

/* end of file */
