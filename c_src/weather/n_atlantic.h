/* Most of the info provided in this file could be found in an almanac. */

/* Ave_Temp       - the average temperature for each month */
/* Weather_Table  - each entry in the table is the weather profile for each month */
/*                  for a random 1-100 die roll. */
/*  month profile - {precipitation, overcast, taifun/hurricane, clear} */
/*        example - {30,35,0,100} means that in this month a roll of : */
/*                  1-30  = precipitation        */
/*                 31-35  = overcast             */
/*                 no chance of taifun/hurricane */
/*                 36-100 = clear                */

/* Wind_Table     - is structured like the Weather_Table. */
/*  month profile - {Calm, Light, Brisk, Blustery, Gale}  */

/* Precip_Table   - as above */
/*  month profile - {None, Sprinkle, Shower, Heavy, Steady} */

/* Overcast_Table - {Heavy Fog, Light Fog, Mist, Cloudy} */

UBYTE Ave_Temp[N_MONTHS] = {10, 15, 25, 40, 55, 70, 85, 80, 60, 35, 20, 15};

UBYTE Weather_Table[N_MONTHS][N_WEATHER] = {
    {30,40,0,100}, /* January or the first month in non-Gregorian calendar */
    {25,35,0,100}, /* February */
    {20,30,0,100}, /* March */
    {35,50,0,100}, /* April */
    {25,40,0,100}, /* May */
    {15,25,0,100}, /* June */
    {10,15,0,100}, /* July */
    {10,15,0,100}, /* August */
    {15,30,0,100}, /* September */
    {25,40,0,100}, /* October */
    {25,40,0,100}, /* November */
    {30,45,0,100}  /* December */
};

UBYTE Wind_Table[N_MONTHS][N_WIND] = {
    {60,80,90,100,0},    /* January */
    {50,70,85,100,0},    /* February */
    {40,60,75,95,100},   /* March */
    {60,85,95,100,0},    /* April */
    {70,90,100,0,0},     /* May */
    {80,90,100,0,0},     /* June */
    {80,100,0,0,0},      /* July */
    {80,100,0,0,0},      /* August */
    {60,80,95,100,0},    /* September */
    {40,65,85,95,100},   /* October */
    {40,60,80,90,100},   /* November */
    {60,80,90,95,100}    /* December */
};

UBYTE Precip_Table[N_MONTHS][N_PRECIP] = {
    {0,40,70,80,100},   /* January */
    {0,50,80,95,100},   /* February */
    {0,60,80,100,0},    /* March */
    {0,40,70,90,100},   /* April */
    {0,50,80,100,0},    /* May */
    {0,60,90,100,0},    /* June */
    {0,80,95,100,0},    /* July */
    {0,80,95,100,0},    /* August */
    {0,60,80,90,100},   /* September */
    {0,50,80,90,100},   /* October */
    {0,40,70,85,100},   /* November */
    {0,40,70,80,100}    /* December */
};

UBYTE Overcast_Table[N_OVERCAST] = {10,20,70,100};


