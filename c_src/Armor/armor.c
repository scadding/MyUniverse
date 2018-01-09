 /*   Armor
      This program was written by William E. Wilson as a game
      aid for the MegaTraveller and Striker game systems.  
*/

#include <stdio.h>
#include <math.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

char *p,buffer[20];

main()
{
     static double av[5] = {8.0, 14.0, 18.0, 28.0, 30.0};
     static double avprice[5] = {81320.0,166920.0,166920.0,353100.0,353100.0};
     typedef char typ[6];
     int          num, i, j, tl;
     double       price, cm, modcm;
     typ          ltyp;
     char         done;
     char         ans;
     done = FALSE;

     price = 0; tl = 11; cm = 0.1;
     do {
        printf("Enter Tech Level: [%d] ",tl);
        buffer[0]=2;
        p=gets(buffer);
        if (strlen(buffer)!=0) tl=atoi(buffer);
        printf("Enter the CM in thickness (Light  .1-.15 /  Med .16-.25\n");
        printf("                     Hvy .26-.5) for the armor : [%.2lf] ",cm);
        buffer[0]=5;
        p=gets(buffer);
        if (strlen(buffer)!=0) cm=atof(buffer);
        printf("\n");
        if ((cm < 0.16)) strcpy(ltyp, "Light");
        if (cm >= 0.16) strcpy(ltyp, "Medium");
        if ((cm > 0.25)) strcpy(ltyp, "Heavy");
        price = avprice[tl-11] * cm;
        modcm = cm * av[tl-11];

        printf("TL %d %s Duty Combat Armor. %10.2lf Credits.\n", tl,ltyp,price);
        printf("Modified CM value of %5.2lf  based on a normal body size.\n", modcm);
        printf("Add 5000 credits for oxygen tanks and vacuum kit.\n\n");
        if (tl>12)
          if  (tl>=14)  printf("Battle dress configuration add 300,000 credits.\n");
             else  printf("Battle dress configuration add 170,000 credits.\n");
        printf("Enter a Q to quit or any  other key to  continue : \n");
        ans=getchar();
        if (toupper(ans) == 'Q') done = TRUE;
     }  while (!done) ;
}

