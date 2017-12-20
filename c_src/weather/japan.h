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
#define RAT    0
#define BULL   1
#define TIGER  2
#define HARE   3
#define DRAGON 4
#define SNAKE  5
#define HORSE  6
#define SHEEP  7
#define MONKEY 8
#define BIRD   9
#define DOG    10
#define BOAR   11

#define FIRST_MONTH RAT
#define LAST_MONTH  BOAR

#define N_MONTHS (LAST_MONTH - FIRST_MONTH) + 1

/* text version of month names */
char *Month_Name[N_MONTHS] = {
    "Rat",
    "Bull",
    "Tiger",
    "Hare",
    "Dragon",
    "Snake",
    "Horse",
    "Sheep",
    "Monkey",
    "Bird",
    "Dog",
    "Boar"
};

/* table of days per month */
UBYTE Day_Table[N_MONTHS] = {29, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30};

void
print_holiday(day,month)
    UBYTE day;
    MONTH_T month;
    /* prints holidays that fall on a given day */
{
     if (day == 15)
         printf(" Full Moon,");
     if (day == Day_Table[month])
         printf(" New Moon,");
     switch (day) { 
     case 1:
     case 15 : printf(" Shinto,");
          break;
     case 8:
     case 18 : printf(" Buddhist,");
          break;
     case 28 : printf(" Buddhist, Shinto,");
          break;
     }
     switch (month) {
     case RAT : 
         if ((day >= 1) AND (day <= 6))
             printf(" New Years (s)");
         break;
     case BULL :
         switch (day) { 
         case 1: case 2: case 4: case 5: case 6: case 7:
             printf(" Equinox (b)");
             break;
         case 3: 
             printf(" Setsuban (1st Day of Spring) (s)");
             break;
         }
         break;
     case TIGER :
         if (day == 3) 
             printf(" Doll Festival (c)");
         break;
     case HARE :
         if (day == 8)
             printf(" Buddha's Birthday (b)");
         break;
     case DRAGON :
         if (day == 5) 
             printf(" Boy's Day (c)");
         break;
     case SNAKE :
         switch (day) {
         case 1: case 2: case 3: case 4: case 5: case 6: case 7: 
             printf(" Rice Planting Festival (p)");
             break;
         case 30 : 
             printf(" All debts paid (p)");
         }
         break;
     case HORSE :
         switch (day) {
         case 1 : 
             printf(" Midyear (p)");
             break;
         case 7 : 
             printf(" Tanabata (Star Festival) (c)");
             break;
         case 13: case 14: case 15: 
             printf(" Bon Festival (for the dead) (b)");
         }
         break;
     case SHEEP :
         if ((day >= 1) AND (day <= 7)) 
             printf(" Equinox (b)");
         break;
     case BIRD :
         switch (day) { 
         case 15 : 
              printf(" Rice Harvest Festival (p)");
              break;
         case 20 : 
              printf(" Festival of Ebishu (s)");
         } 
         break;
     case DOG :
         if (day == 8) 
             printf(" Feast of the Bellows (s)");
         break;
     case BOAR :
         if (day == 30) 
             printf(" All debts paid (p)");
     }
}

