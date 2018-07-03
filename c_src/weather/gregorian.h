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
#define JANUARY   0
#define FEBRUARY  1
#define MARCH     2
#define APRIL     3
#define MAY       4
#define JUNE      5
#define JULY      6
#define AUGUST    7
#define SEPTEMBER 8
#define OCTOBER   9
#define NOVEMBER  10
#define DECEMBER  11

#define FIRST_MONTH JANUARY
#define LAST_MONTH  DECEMBER

#define N_MONTHS (LAST_MONTH - FIRST_MONTH) + 1

char *Month_Name[N_MONTHS] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

UBYTE Day_Table[N_MONTHS] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void
print_holiday(day, month)
UBYTE day;
MONTH_T month;
{
    switch (month) {
    case JANUARY :
        if (day == 1)
            printf(" New Years (s)");
        if (day == 6)
            printf(" Twelfth Night");
        break;
    case FEBRUARY :
        if (day == 2)
            printf(" Candlemas");
        if (day == 14)
            printf(" St. Valentine's Day");
        break;
    case MARCH :
        if (day == 21)
            printf(" Spring Equinox");
        break;
    case APRIL :
        break;
    case MAY :
        if (day == 1)
            printf(" May Day");
        break;
    case JUNE :
        if (day == 21)
            printf(" Midsummer Day");
        break;
    case JULY :
        break;
    case AUGUST :
        if (day == 2)
            printf(" Lammas");
        break;
    case SEPTEMBER :
        if (day == 21)
            printf(" Fall Equinox");
        break;
    case OCTOBER :
        if (day == 31)
            printf(" Halloween");
        break;
    case NOVEMBER :
        break;
    case DECEMBER :
        if (day == 21)
            printf(" Winter Solstice");
        if (day == 25)
            printf(" Christmas");
        break;
    }
}

