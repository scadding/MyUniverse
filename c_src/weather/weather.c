/**************************************************************************
 *  Weather - generates random weather                                    *
 *                                                                        *
 *  Copyright 1988 by Brett Slocum.  All rights reserved.                 *
 *  Permission is granted to distribute or modify this program, or use    *
 *  portions of this program in other programs, as long as this notice    *
 *  remains intact.                                                       *
 *  This program or its derivatives may not be sold for profit without    *
 *  permission of the author.                                             *
 *                                                                        *
 *  Original UNIX Version: Brett Slocum UUCP: ...uunet!hi-csc!slocum      *
 *                                      ARPA: hi-csc!slocum@uunet.uu.net  *
 *                                                                        *
 **************************************************************************/

#include "weather.h"

typedef UBYTE MONTH_T;

/* Calendar choices */
#ifdef GREGORIAN
#include "gregorian.h"
#else
#ifdef JAPAN
#include "japan.h"
#else
#ifdef MIDDLE_EARTH
#include "middle_earth.h"
#endif
#endif
#endif

/* Climate choices */
#ifdef N_ATLANTIC
#include "n_atlantic.h"
#else
#ifdef MID_ATLANTIC
#include "mid_atlantic.h"
#else
#ifdef S_ATLANTIC
#include "s_atlantic.h"
#else
#ifdef GULF
#include "gulf.h"
#else
#ifdef N_PACIFIC
#include "n_pacific.h"
#endif
#endif
#endif
#endif
#endif

UBYTE Stat_Table[N_MONTHS][N_STAT] = {
    {200,0,0}, {200,0,0}, {200,0,0}, {200,0,0}, {200,0,0}, {200,0,0}, 
    {200,0,0}, {200,0,0}, {200,0,0}, {200,0,0}, {200,0,0}, {200,0,0}
};

WIND_T     Wind = CALM;          /* today's wind conditions */
TEMP_T     Temp_Class = CHILLY;  /* today's temperature class */
PRECIP_T   Precip_Class = NONE;  /* today's precipitation */
WEATHER_T  Weather;              /* today's weather */
OVERCAST_T Sky;                  /* today's sky conditions */

UBYTE Temperature = 0,           /* today's actual temperature */
      Duration = 0;              /* today's precipitation duration */

BOOL Duration_Hours = TRUE,      /* flag that indicates whether precip lasted */
                                 /* hours (TRUE) or minutes (FALSE) */
     Catastrophe = FALSE,        /* flag that indicates whether a catastrophe */
                                 /* occurred today */
     Storm = FALSE,              /* flag that indicates whether precip was a storm */
     Long_Storm = FALSE,         /* flag that indicates whether precip was a */
                                 /* long storm lasting more than one day */
     Snow = FALSE;               /* flag that indicates whether snow fell today */

float Rainfall = 0.0,            /* yearly total rainfall */
      Snowfall = 0.0,            /* yearly total snowfall */
      Snow_Depth = 0.0;          /* today's snow depth, including new accumulations */
                                 /* and melting */

WEATHER_T 
gen_weather(month, ptr_catastrophe)
    MONTH_T month;
    BOOL *ptr_catastrophe;
    /* This routine generates the overall weather for the day. */
    /* It also determines whether a catastrophe has occurred.  */
    /* A catastrophe can be localized or wide-spread, natural  */
    /* or man-made. Such things as fires, earthquakes, floods, */
    /* etc. are possible.                                      */
{
     int       roll;
     WEATHER_T i, 
               return_val;

     /* get die-roll.  if catastrophe, set flag and roll again. */
     do {  
         roll = DIE(100);
         if (roll == 100)
             *ptr_catastrophe = TRUE;
     }
     while (roll == 100);
 
     /* if it rained more than a sprinkle yesterday, it more likely */
     /* to be at least overcast, if not rainy today. */
     if (Precip_Class > SPRINKLE) {
         roll = roll - 10;
         if (roll < 1)
             roll = 1;
     }

     /* find the type of weather from the die-roll */
     return_val = CLEAR;
     for (i = CLEAR; i >= PRECIPITATION; i--)
         if (roll <= Weather_Table[month][i])
             return_val = i;
     return(return_val);
}

TEMP_T 
gen_temp(month)
    MONTH_T month;
    /* generates the day's temperature and set the temp type */
{
     int roll, data, i;
     TEMP_T return_val;

     /* determine temperature variation from monthly average */
     /*   +/- 20 degrees */
     roll = DIE(100);
     data = 8;
     for (i = 8; i >= 0; i--)
         if (roll <= Temp_Variation[i])
             data = i;
     Temperature = Ave_Temp[month] + ((data-4) * 5);

     /* classify the temperature class */
     if (Temperature < 25) 
         return_val = COLD;
     else
     if ((Temperature >= 25) AND (Temperature < 40)) 
         return_val = CHILLY;
     else
     if ((Temperature >= 40) AND (Temperature < 65)) 
         return_val = FAIR;
     else
     if ((Temperature >= 65) AND (Temperature < 80)) 
         return_val = WARM;
     else
     if (Temperature >= 80) 
         return_val = HOT;

     /* accumulate monthly statistics */
     Stat_Table[month][HIGH] = MAX(Temperature, Stat_Table[month][HIGH]);
     Stat_Table[month][LOW] = MIN(Temperature, Stat_Table[month][LOW]);
     return(return_val);
}

WIND_T
gen_wind(month)
    MONTH_T month;
    /* generate the wind class */
{
     int roll;
     WIND_T return_val, i;

     /* modify the roll for current weather.  Clear weather is less likely */
     /* to be windy, and rainy weather is more likely. */
     roll = DIE(100);
     switch (Weather) {
     case CLEAR : 
         roll -= 20;
         break;
     case PRECIPITATION:
         roll += 20;
         break;
     } /* switch */

     /* if yesterday was very windy, today will be windier */
     if (Wind >= BLUSTERY) 
         roll += 20;

     /* limit to boundaries 1-100 */
     if (roll < 1) 
         roll = 1;
     if (roll > 100) 
         roll = 100;

     /* find wind type */
     return_val = GALE;
     for (i = GALE; i >= CALM; i--)
         if (roll <= Wind_Table[month][i]) 
             return_val = i;
     if (Weather == TAIFUN) 
         return_val = GALE;
     return(return_val);
 }

PRECIP_T
gen_precip(month)
    MONTH_T month;
    /* generate today's precipitation */
{
     int roll;
     PRECIP_T return_val, i;

     return_val = NONE;
     if (Weather == PRECIPITATION)
     {
         roll = DIE(100);
         return_val = STEADY;
         for (i = STEADY; i >= SPRINKLE; i--)
             if (roll <= Precip_Table[month][i])
                 return_val = i;
     }

     /* if yesterday had steady precipitation, there is a 1/3 chance */ 
     /* on the second day and 1/6 chance on succeeding days that today */
     /* also has steady precipitation */
     if ((Precip_Class == STEADY) AND (DIE(6) <= 2 - Long_Storm))
     {
         Weather = PRECIPITATION;
         return_val = STEADY;
     }

     return(return_val);
}

void
precip_statistics()
     /* determines various flags including Snow, Storm and Long_Storm. */
     /* Also determines precipitation duration and amount, updates */
     /* yearly Rainfall and Snowfall statistics, daily Snow_Depth, */
     /* and snow melting */
{
     float amount;

     if (Weather == PRECIPITATION)
     {
         /* set flags */
         if (Precip_Class != STEADY) 
             Long_Storm = FALSE;
         if (Temperature < 35)
             Snow = TRUE;
         else
             Snow = FALSE;
         if ((Temp_Class >= WARM) AND ((Precip_Class == SHOWER) OR (Precip_Class == HEAVY)))
             Storm = TRUE;
         else
             Storm = FALSE;
         if (Snow AND (Precip_Class >= HEAVY) AND (Wind >= BLUSTERY))
             Storm = TRUE;

         /* determine duration and amount of precipitation */
         switch (Precip_Class) {
         case SPRINKLE :
             Duration = 5 * DIE(6);
             Duration_Hours = FALSE;
             amount = 0.1;
             break;
         case SHOWER :
             Duration = 10 * (DIE(6) + DIE(6));
             Duration_Hours = FALSE;
             amount = Duration / 100;
             break;
         case HEAVY : 
             Duration = 1 + DIE(6);
             Duration_Hours = TRUE;
             amount = (DIE(4) + (Storm * Duration));
             break;
         case STEADY :
             Duration = 24;
             Duration_Hours = TRUE;
             amount = (DIE(6) + DIE(6)) + (Storm * (DIE(6) + DIE(6))); 
             break;
         }
         if (Storm)
             amount *= 2.0;

         /* update yearly and daily statistics */
         Rainfall += (amount / 10);
         if (Snow)
             {
                 Snow_Depth += amount;
                 Snowfall += amount;
             }
         else
             switch (Precip_Class) {
             case SHOWER : 
                 Snow_Depth -= Duration / 60;
                 break;
             case HEAVY : 
                 Snow_Depth -= Duration;
                 break;
             case STEADY :
                 Snow_Depth -= 12;
                 break;
             }
     }

     /* melt snow in weather above freezing */
     switch (Temp_Class) {
     case CHILLY :
         if (Temperature > 32) 
             Snow_Depth -= 1.0;
         break;
     case FAIR :
         Snow_Depth -= Temperature / 8;
         break;
     case WARM :
         Snow_Depth -= Temperature - 40;
         break;
     case HOT : 
         Snow_Depth -= Temperature - 20;
         break;
     }
     if (Snow_Depth < 0.1)
         Snow_Depth = 0.0;
}

OVERCAST_T
gen_overcast(weather)
    WEATHER_T weather;
    /* generate today's level of cloudiness */
{
     int roll;
     OVERCAST_T return_val, i;

     return_val = CLOUDY;
     switch (weather) {
     case PRECIPITATION:
     case OVERCAST :
          roll = DIE(100);
          return_val = CLOUDY;
          for (i = CLOUDY; i >= HEAVY_FOG; i--)
              if (roll <= Overcast_Table[i])
                  return_val = i;
          break;
     case TAIFUN : return_val = CLOUDY;
     }
     return(return_val);
}

void
print_notes(day, month)
    UBYTE day;
    MONTH_T month;
    /* print snow depth, precipitation type and duration, */
    /* catastrophe, and holidays */
{
     char *precip,
          *dur;

     if (Precip_Class != NONE) 
     {
         if (Snow) 
             precip = (Storm ? "Blizzard" : "Snow");
         else
             precip = (Storm ? "Storm" : "Rain");
         dur = (Duration_Hours ? "hours" : "minutes");
         printf(" %s - %2d %s,", precip, Duration, dur);
     }
     if (Catastrophe)
         printf(" Catastrophe,");
     print_holiday(day, month);
}

void
print_weather(day, month)
    UBYTE day;
    MONTH_T month;
    /* print today's weather */
{
     printf("%4d  %13s", day, Weather_Name[Weather]);
     printf("%11s", ((Weather==CLEAR) ? "Clear" : Overcast_Name[Sky]));
     printf("%8s (%3d)  %8s  %8s", Temp_Name[Temp_Class], Temperature, Precip_Name[Precip_Class], Wind_Name[Wind]);
     if (Snow_Depth > 0.0) 
         printf(" %5.1f\" ", Snow_Depth);
     else
         printf("%8c", ' ');
     print_notes(day, month);
     printf("\n");
 }

void
init_winter()
     /* initialize last year's winter */
{
     UBYTE day;
     MONTH_T month;

     /* calculate snow depth for Oct. thru Dec. */
     for (month = LAST_MONTH-2; month <= LAST_MONTH; month++)
         for (day = 1; day <= Day_Table[month]; day++)
         {
             Catastrophe = FALSE;
             if (DIE(100) > 50) 
                 Weather = gen_weather(month, &Catastrophe);
             Wind = gen_wind(month);
             Temp_Class = gen_temp(month);
             Precip_Class = gen_precip(month);
             precip_statistics();
             Sky = gen_overcast(Weather);
         }
}

int init_stat()
{
    MONTH_T month;
    STAT_T stat;

    for (month = FIRST_MONTH; month <= LAST_MONTH; month++)
    {
        Stat_Table[month][LOW] = 200;
        for (stat = AVERAGE; stat <= HIGH; stat++)
            Stat_Table[month][stat] = 0;
    }
    return(0);
}

main(argc, argv)  
int argc;
char *argv[];
{
     int cum_temp;               /* cumulative temp used for calculating average temp */
     unsigned int sunshine = 0;  /* number of days of sunshine */
     MONTH_T month;              /* month counter */
     UBYTE day,                  /* day counter */
           year,                 /* year counter */
           n_years = 1;          /* number of years */

     srandom((int)time(0));
     if (argc > 1)
         n_years = atoi(argv[1]);
     init_winter();
     for (year = 1; year <= n_years; year++)
     {
         Rainfall = 0.0;
         Snowfall = 0.0;
         sunshine = 0;
         init_stat();
         printf("%64c%6s\n", ' ',"Snow");
         printf("Year %3d%11s%11s%14s%10s%10s%7s%7s\n", year, "Weather", "Sky", "Temperature", "Precip", "Wind", "Depth", "Notes");
         for (month = FIRST_MONTH; month <= LAST_MONTH; month++)
         {
             cum_temp = 0;
             printf("%-9s\n", Month_Name[month]);
             for (day = 1; day <= Day_Table[month]; day++)
             {
                 Catastrophe = FALSE;
    
                 /* half the time, Weather is the same as yesterday */
                 if (DIE(100) > 50) 
                     Weather = gen_weather(month, &Catastrophe);
                 Wind = gen_wind(month);
                 Temp_Class = gen_temp(month);
                 cum_temp += Temperature;
                 Precip_Class = gen_precip(month);
                 precip_statistics();
                 Sky = gen_overcast(Weather);
    
                 /* count sunny days */
                 if (Weather == CLEAR) 
                     sunshine++;
                 print_weather(day, month);
             }
             Stat_Table[month][AVERAGE] = (cum_temp / Day_Table[month]);
             printf("High Temp = %3d, Low Temp = %3d, Average Temp = %3d\n\n", Stat_Table[month][HIGH], Stat_Table[month][LOW], Stat_Table[month][AVERAGE]);
         }
         /* print yearly statistics */
         printf("Sunshine = %3d days\n", sunshine);
         printf("Rainfall = %4.1f\"\n", Rainfall);
         printf("Snowfall = %4.1f\"\n\n", Snowfall);
     }
}

