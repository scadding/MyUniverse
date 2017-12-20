/* Copyright 1988 by Brett D. Slocum - 12/12/88 */
/* All rights reserved */
/* This is the calendar include file is for weather generation */
/* It includes calendar and holiday information */

/* To create a new calendar file, 
       1) define month names for use by MONTH_T type,
       2) define FIRST_MONTH and LAST_MONTH,
       3) define string array Month_Name with printable version of month,
       4) define number of days per month array Day_Table,
       5) add printf statements in print_holidays for holidays
*/

/* Month types */
#define AFTERYULE   0
#define SOLMATH     1
#define RETHE       2
#define ASTRON      3
#define THRIMIDGE   4
#define FORELITHE   5
#define AFTERLITHE  6
#define WEDMATH     7
#define HALIMATH    8
#define WINTERFILTH 9
#define BLOTMATH    10
#define FOREYULE    11

#define FIRST_MONTH AFTERYULE
#define LAST_MONTH  FOREYULE

#define N_MONTHS (LAST_MONTH - FIRST_MONTH) + 1

char *Month_Name[N_MONTHS] = {
    "Afteryule",
    "Solmath",
    "Rethe",
    "Astron",
    "Thrimidge",
    "Forelithe",
    "Afterlithe",
    "Wedmath",
    "Halimath",
    "Winterfilth",
    "Blotmath",
    "Foreyule",
};

UBYTE Day_Table[N_MONTHS] = {30, 30, 30, 30, 30, 31, 30, 30, 30, 30, 30, 34};

void
print_holiday(day, month)
    UBYTE day;
    MONTH_T month;
{
     switch (month) {
     case AFTERYULE:
         break;
     case SOLMATH:
         break;
     case RETHE:
         break;
     case ASTRON:
         break;
     case THRIMIDGE:
         break;
     case FORELITHE:
         if (day == 31) 
             printf(" Midsummer Day");
         break;
     case AFTERLITHE:
         break;
     case WEDMATH:
         break;
     case HALIMATH:
         break;
     case WINTERFILTH:
         break;
     case BLOTMATH:
         break;
     case FOREYULE:
         if ((day >= 31) AND (day <= 34))
             printf(" Yule");
         break;
     }
}

