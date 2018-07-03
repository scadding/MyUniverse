#ifdef SYSV
#define srandom srand48
#define random lrand48
#endif

#define DIE(n) ((int)(random() % (unsigned)n)+1)
#define MIN(a,b) ((a<b) ? a : b)
#define MAX(a,b) ((a>b) ? a : b)

#define AND &&
#define OR  ||

typedef unsigned char UBYTE;
typedef UBYTE BOOL;

typedef short STAT_T;
typedef short WEATHER_T;
typedef short OVERCAST_T;
typedef short WIND_T;
typedef short TEMP_T;
typedef short PRECIP_T;

/* Boolean types */
#define FALSE 0
#define TRUE  1

/* Statistics types */
#define N_STAT  3
#define LOW     0
#define AVERAGE 1
#define HIGH    2

/* Weather types */
#define N_WEATHER     4
#define PRECIPITATION 0
#define OVERCAST      1
#define TAIFUN        2
#define CLEAR         3

char *Weather_Name[N_WEATHER] = {
    "Precipitation",
    "Overcast",
    "Typhoon",
    "Clear"
};

/* Overcast types */
#define N_OVERCAST 4
#define HEAVY_FOG  0
#define LIGHT_FOG  1
#define MIST       2
#define CLOUDY     3

char *Overcast_Name[N_OVERCAST] = {
    "Heavy Fog",
    "Light Fog",
    "Mist",
    "Cloudy",
};

/* Wind types */
#define N_WIND   5
#define CALM     0
#define LIGHT    1
#define BRISK    2
#define BLUSTERY 3
#define GALE     4

char *Wind_Name[N_WIND] = {
    "Calm",
    "Light",
    "Brisk",
    "Blustery",
    "Gale",
};

/* Temperature types */
#define N_TEMP 5
#define COLD   0
#define CHILLY 1
#define FAIR   2
#define WARM   3
#define HOT    4

char *Temp_Name[N_TEMP] = {
    "Cold",
    "Chilly",
    "Fair",
    "Warm",
    "Hot",
};

/* Precipitation types */
#define N_PRECIP 5
#define NONE     0
#define SPRINKLE 1
#define SHOWER   2
#define HEAVY    3
#define STEADY   4

char *Precip_Name[N_PRECIP] = {
    "None",
    "Sprinkle",
    "Shower",
    "Heavy",
    "Steady",
};

UBYTE Temp_Variation[9] = {1, 5, 13, 25, 75, 87, 95, 99, 100};
