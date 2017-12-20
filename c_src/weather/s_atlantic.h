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

UBYTE Ave_Temp[N_MONTHS] = {50, 60, 65, 70, 75, 80, 85, 80, 75, 65, 55, 50};

UBYTE Weather_Table[N_MONTHS][N_WEATHER] = {
    {30,35,0,100}, {25,35,0,100}, {20,30,0,100}, {35,50,0,100}, {25,40,0,100}, {15,25,0,100},
    {10,15,0,100}, {10,15,0,100}, {15,25,0,100}, {25,35,0,100}, {25,40,0,100}, {30,45,0,100}
};

UBYTE Wind_Table[N_MONTHS][N_WIND] = {
    {60,80,90,100,0}, {50,70,85,100,0}, {40,60,75,99,100}, {60,85,95,100,0}, {70,90,100,0,0}, {80,90,100,0,0},
    {80,100,0,0,0}, {80,100,0,0,0}, {60,80,95,100,0}, {40,65,85,97,100}, {40,60,80,95,100}, {60,80,90,100,0}
};

UBYTE Precip_Table[N_MONTHS][N_PRECIP] = {
    {0,40,70,80,100},
    {0,50,80,95,100},
    {0,60,80,100,0},
    {0,40,70,90,100},
    {0,50,80,100,0},
    {0,60,90,100,0},
    {0,80,95,100,0},
    {0,80,95,100,0},
    {0,60,80,90,100},
    {0,50,80,90,100},
    {0,40,70,85,100},
    {0,40,70,80,100}
};

UBYTE Overcast_Table[N_OVERCAST] = {10,20,70,100};

