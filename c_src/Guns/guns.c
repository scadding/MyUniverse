/**************************************************************************/
/*                                                                        */
/*   Program Name : guns.c                                                */
/*                                                                        */
/*   Description : Determines the availability of a requested weapon      */
/*                 based on the tech/law level of the world were you      */
/*                 are trying to purchase it. If the weapon is available  */
/*                 a listed price modifier is given and the delivery time.*/
/*                 This is based on an old, old White Dwarf article by    */
/*                 Steve Cook entitled 'Guns for Sale'.                   */
/*                                                                        */
/*                 Compiled using TurboC v2.0 (Ansii C) with:             */
/*                     tcc -mt -lt guns    (flags just make it a .com)    */
/*                                                                        */
/*                 Conversion to unix/non-ansii C will just require       */
/*                 removal of a few #includes and renaming of the         */
/*                 randomising functions.                                 */
/*                                                                        */
/*                 Additional weapons may be added to the table just by   */
/*                 adding a new line in the following format:             */
/*                 "gun",<base chance>,<tech level min>,<law level max>,  */
/*                                                                        */
/*------------------------------------------------------------------------*/
/* Paul J. Netherwood  Kingston Polytechnic               |8th Mar 1990   */
/*                                                        |               */
/*------------------------------------------------------------------------*/
/* Modification history:                                                  */
/*------------------------------------------------------------------------*/
/* none                                                   |               */
/**************************************************************************/

#include <stdio.h>
/*#include <dos.h>
#include <conio.h>*/
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

/* Declaration and initialisation of gun table */

struct base_chance 
{
     char *gun;
     int chance,       /* Base percentage chance weapon is available */
     tech_min,         /* Minimum tech level required to make weapon */
     law_max;          /* Maximum law level that weapon is allowed */
} gun_table[] = {
     /* Weapon */  /* Base Chance *//* Tech *//* Law */
     "Accelerator Rifle",    50,       9,       2,
     "ACR",                  40,       10,      2,
     "Assault Rifle",        60,       7,       3,
     "Auto Cannon",          1,        8,       2,
     "Auto Pistol",          55,       5,       4,
     "Auto Rifle",           35,       6,       3,
     "Auto Shotgun",         30,       6,       3,
     "Bayonet",              75,       3,       7,
     "Blade",                75,       3,       7,
     "Body Pistol",          60,       7,       0,
     "Broadsword",           45,       2,       7,
     "Carbine",              60,       5,       5,
     "Cutlass",              55,       3,       7,
     "Dagger",               95,       1,       8,
     "FGMP 14",              1,        14,      1,
     "FGMP 15",              1,        15,      1,
     "Flamethrower",         1,        6,       1,
     "Gauss Rifle",          25,       12,      2,
     "Halberd",              40,       2,       7,
     "Heavy Body Pistol",    40,       7,       0,
     "Laser Carbine",        20,       8,       1,
     "Laser Rifle",          20,       9,       1,
     "LAG",                  40,       8,       3,
     "Needle Rifle",         10,       14,      1,
     "PGMP 12",              2,        12,      1,
     "PGMP 13",              2,        13,      1,
     "PGMP 14",              1,        14,      1,
     "Plasma Pistol",        -1,       16,      1,
     "Pike",                 40,       1,       7,
     "RAM GL",               25,       8,       0,
     "Revolver",             70,       4,       4,
     "Rifle",                55,       5,       5,
     "Shock Disablers",      20,       9,       4,
     "Shotgun",              70,       4,       6,
     "SMG",                  35,       5,       3,
     "Snub Pistol",          50,       8,       4,
     "Snub Rifle",           35,       8,       5,
     "Spear",                60,       0,       7,
     "Stun Pistol",          30,       8,       4,
     "Stun Rifle",           25,       8,       5,
     "Submachine Pistol",    40,       7,       3,
     "Sword",                50,       1,       7,
     "Tonite Pistol",        20,       12,      1,
     "Tonite Carbine",       30,       12,      1,
     "VRF Gauss Gun",        1,        10,      3,
     "Warper",               -1,       17,      1,
     "Wiper",                -1,       18,      1
};

#define NKEYS   ( sizeof gun_table / sizeof(struct base_chance) )
#define MAX_ACROSS 3         /* maximum gun names displayed across screen */

#define d(x)   ((rand() % x) + 1)

void
main(void)
{
int n,m,tech,law,streetwise,bribe,modifier,total_chance,price_mod,roll;
char choice;
long now;

     /* Randomise */
     srand(time(&now) % 37);

     printf("Weapon Availability\n");

     /* Display all the weapons */
     for(n = 0; n < NKEYS; n += MAX_ACROSS)
     {
           /* Display one row of weapons */
           for(m = 0; m < MAX_ACROSS; m++)
           {
                 if(n+m >= NKEYS)
                       break;

                 printf("%3d: %-20s ",n+m,gun_table[n+m].gun);
           }
           printf("\n");     /* Next line */
     }

     printf("Choose weapon: "); scanf("%d",&n);

     printf("\nThe %s is a tech level %d weapon and is banned\n",
           gun_table[n].gun,gun_table[n].tech_min);
     printf("at law levels %d and over\n",gun_table[n].law_max+1);


     if(gun_table[n].chance < 0)
     {
           printf("The %s is not available through know channels\n",gun_table[n].gun);
           return;
     }

     if(gun_table[n].chance < 10)
           printf("Chance of finding one of these is pretty slim\n");
     else
           if(gun_table[n].chance < 50)
                 printf("Availabilty for this weapon is low\n");
           else
                 printf("Availabilty for this weapon is high\n");

     printf("\nEnter the worlds tech level: "); scanf("%d",&tech);
     printf("Enter the worlds law level: "); scanf("%d",&law);

     total_chance = gun_table[n].chance;       /* Get base chance */
     price_mod = 0;

     /* Tech level modifier */
     if(tech >= gun_table[n].tech_min)
     {
           modifier = (tech - gun_table[n].tech_min) * 10;
           if(modifier > 50)
                 modifier = 50;
     }
     else
           modifier = (tech - gun_table[n].tech_min) * 15;

     total_chance += modifier;

     /* Law level modifier */
     if(law >= gun_table[n].law_max)
     {
           modifier = (law - gun_table[n].law_max) * -25;
           price_mod = (law - gun_table[n].law_max) * 15;
     }
     else
     {
           modifier = (gun_table[n].law_max - law) * 10;
           price_mod = (gun_table[n].law_max - law) * -5;
           if(modifier > 30)
                 modifier = 30;
     }

     total_chance += modifier;

     printf("\nPurchased at a general shop? [y/n]");
     do
           choice = tolower(getchar());
     while(choice != 'y' && choice != 'n');

     if(choice == 'y')
     {
           total_chance -= 35;
           price_mod += 30;
     }
     else
     {
           printf("\nPurchased at a non-specialist shop? (e.g hunting shop) [y/n]");
           do
                 choice = tolower(getchar());
           while(choice != 'y' && choice != 'n');
           if(choice == 'y')
           {
                 total_chance -= 15;
                 price_mod += 10;
           }
     }

     printf("\nStreetwise skill (enter 0 for no skill): "); scanf("%d",&streetwise);

     total_chance += streetwise * 10;
     price_mod -= streetwise * 5;

     printf("\nPurchased on the black market? [y/n]");
     do
           choice = tolower(getchar());
     while(choice != 'y' && choice != 'n');

     if(choice == 'y')
     {
           if(law >= gun_table[n].law_max)
           {
                 modifier = (law - gun_table[n].law_max) * 15;
                 price_mod = (law - gun_table[n].law_max) * 25;
                 total_chance += modifier;
           }

           printf("\nBribery skill (enter 0 for no skill): "); scanf("%d",&bribe);

           total_chance += bribe * 10;
           price_mod += bribe * 10;
     }
     if(d(100) <= total_chance)
     {
           printf("\n%s available for purchase at %-+d percent listed price\n",
                 gun_table[n].gun,price_mod);
           roll = d(100);
           if(roll <= 50)
                 printf("available straight away\n");
           else
                 if(roll <= 75)
                       printf("available after %d hours\n",d(6) + d(6));
                 else
                       if(roll <= 90)
                             printf("available after %d days\n",d(6) + d(6));
                       else
                             printf("available after %d weeks\n",d(6) + d(6));
     }
     else
           printf("\nWeapon unavailable\n");
}
