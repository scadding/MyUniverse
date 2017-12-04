/*
I've had quite a few requests for my star and planet generation programs,
so rather than blow my email bill through the roof by mailing them
individually, I'll post them here, since there's obviously quite a bit
of interest.

There are two programs, "stars.c" (this post) and "planets.c" (next post).
They're written in standard C and should work with any compiler; let me
know if you have any problems.

"Stars" generates star systems; "planets" reads the output of "stars"
and generates planetary systems for the stars. I wrote them as two separate
programs so the "planets" program could read other star catalogues as well
(i.e. you can either generate a bunch of complete systems at random, or
generate planets for an existing list of stars). The "column" and "skip"
parameters to the "planets" program should help with reading star catalogues
in different formats.

To generate planetary systems for, say, a 50-light-year sector, you would
use something like

    stars >starlist 50
    planets <starlist >planetlist 0 0

or

    stars 50 | planets 0 0 >planetlist

depending on how your OS handles redirection, pipes, etc.

----------------------------------- cut here ------------------------------*/

/*
Star system generator
Copyright 1993 by Ross Smith
Version 1.1 2-Oct-93
*/


#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_STARS 4

#define VOLUME_PER_SYSTEM 900.0


/* Round to nearest integer */
#define round(value) ((long)floor((value) + 0.5))

/* First digit of coded spectral type */
#define spart1(spcode) ((spcode)/100)

/* Second digit of coded spectral type */
#define spart2(spcode) (((spcode)/10)%10)

/* Third digit of coded spectral type */
#define spart3(spcode) ((spcode)%10)


typedef enum
  {
  FALSE,
  TRUE
  }
  boolean;

/* Star system description */
typedef struct
  {
  int spcode[MAX_STARS]; /* Coded spectral type */
  int stars;             /* Number of stars */
  int x;                 /* X coordinate */
  int y;                 /* Y coordinate */
  int z;                 /* Z coordinate */
  }
  star_system;


/* Compare systems by position */
int compare_systems(const void *sysptr1, const void *sysptr2);

/* Generate dice result */
int dice(int number, int faces);

/* Display system details */
void display_system(star_system *syslist, int index);

/* Print a formatted error message and quit */
void error(char *format, ...);

/* Generate system details */
void generate_system(star_system *syslist, int index, int edge);

/* Random integer from min to max */
long randint(long min, long max);

/* Normally distributed random real */
double randnorm(double mean, double sd);

/* Sort systems by coordinates */
void sort_systems(star_system *syslist, int systems);

/* Seed random number sequence from time */
long timeseed(void);

/* Print usage information and quit */
void usage(void);


/* Main function */
int main(int argc, char **argv)
  {

  int count, edge, systems;
  double mean_systems;
  star_system *system_list;

  if (argc < 2)
    usage();
  if (argc > 3)
    error("Too many arguments.");

  edge = strtol(argv[1], NULL, 0);
  if (edge <= 0)
    error("Invalid sector size.");

  if (argc >= 3)
    srand(strtoul(argv[2], NULL, 0));
  else
    timeseed();

  mean_systems = pow((double)edge, 3.0)/VOLUME_PER_SYSTEM;
  do
    systems = round(randnorm(mean_systems, sqrt(mean_systems)));
    while (systems < 0);

  system_list = malloc(systems*sizeof(star_system));
  if (!system_list)
    error("Insufficient memory.");

  for (count = 0; count < systems; count++)
    generate_system(system_list, count, edge);

  sort_systems(system_list, systems);

  for (count = 0; count < systems; count++)
    display_system(system_list, count);

  free(system_list);

  return EXIT_SUCCESS;

  }


/* Compare systems by position */
int compare_systems(const void *sysptr1, const void *sysptr2)
  {

  int x1, x2, y1, y2, z1, z2;

  x1 = ((star_system *)sysptr1)->x;
  x2 = ((star_system *)sysptr2)->x;
  if (x1 < x2)
    return -1;
  if (x1 > x2)
    return 1;

  y1 = ((star_system *)sysptr1)->y;
  y2 = ((star_system *)sysptr2)->y;
  if (y1 < y2)
    return -1;
  if (y1 > y2)
    return 1;

  z1 = ((star_system *)sysptr1)->z;
  z2 = ((star_system *)sysptr2)->z;
  if (z1 < z2)
    return -1;
  if (z1 > z2)
    return 1;

  return 0;

  }


/* Generate dice result */
int dice(int number, int faces)
  {
  int count, total = 0;
  for (count = 0; count < number; count++)
    total += (int)randint(1, faces);
  return total;
  }


/* Display system details */
void display_system(star_system *syslist, int index)
  {

  int star;
  star_system *sys;

  sys = &syslist[index];

  printf("%d. (", index + 1);

  for (star = 0; star < sys->stars; star++)
    {

    if (star)
      printf(",");

    switch (spart1(sys->spcode[star]))
      {
      case 1: printf("c"); break;
      case 3: printf("g"); break;
      case 7: printf("D"); break;
      case 8: printf("ns"); break;
      case 9: printf("bh"); break;
      default: break;
      }

    if (sys->spcode[star] < 800)
      switch (spart2(sys->spcode[star]))
        {
        case 0: printf("O"); break;
        case 1: printf("B"); break;
        case 2: printf("A"); break;
        case 3: printf("F"); break;
        case 4: printf("G"); break;
        case 5: printf("K"); break;
        case 6: printf("M"); break;
        }

    if (sys->spcode[star] < 700)
      printf("%d", spart3(sys->spcode[star]));

    }

  printf(") (%d,%d,%d)\n", sys->x, sys->y, sys->z);

  }


/* Print a formatted error message and quit */
void error(char *format, ...)
  {
  va_list arg_list;
  fprintf(stderr, "*** Error: ");
  va_start(arg_list, format);
  vfprintf(stderr, format, arg_list);
  va_end(arg_list);
  fprintf(stderr, "\n");
  exit(EXIT_FAILURE);
  }


/* Generate system details */
void generate_system(star_system *syslist, int index, int edge)
  {

  boolean ok;
  int count, star, star2, temptype;
  long random_number;
  star_system *sys;

  sys = &syslist[index];

  sys->stars = (dice(3, 6) + 2)/4 - 1;
  if (sys->stars < 1)
    sys->stars = 1;

  for (star = 0; star < sys->stars; star++)
    {

    random_number = randint(1, 1000000);

    if (random_number <= 10)          sys->spcode[star] = 110; /* cB */
    else if (random_number <= 20)     sys->spcode[star] = 120; /* cA */
    else if (random_number <= 40)     sys->spcode[star] = 130; /* cF */
    else if (random_number <= 60)     sys->spcode[star] = 140; /* cG */
    else if (random_number <= 80)     sys->spcode[star] = 150; /* cK */
    else if (random_number <= 100)    sys->spcode[star] = 160; /* cM */
    else if (random_number <= 500)    sys->spcode[star] = 330; /* gF */
    else if (random_number <= 1000)   sys->spcode[star] = 340; /* gG */
    else if (random_number <= 5500)   sys->spcode[star] = 350; /* gK */
    else if (random_number <= 10000)  sys->spcode[star] = 360; /* gM */
    else if (random_number <= 10092)  sys->spcode[star] = 500; /* O */
    else if (random_number <= 19200)  sys->spcode[star] = 510; /* B */
    else if (random_number <= 37600)  sys->spcode[star] = 520; /* A */
    else if (random_number <= 74400)  sys->spcode[star] = 530; /* F */
    else if (random_number <= 148000) sys->spcode[star] = 540; /* G */
    else if (random_number <= 295200) sys->spcode[star] = 550; /* K */
    else if (random_number <= 930000) sys->spcode[star] = 560; /* M */
    else if (random_number <= 944000) sys->spcode[star] = 710; /* DB */
    else if (random_number <= 958000) sys->spcode[star] = 720; /* DA */
    else if (random_number <= 972000) sys->spcode[star] = 730; /* DF */
    else if (random_number <= 986000) sys->spcode[star] = 740; /* DG */
    else if (random_number <= 999300) sys->spcode[star] = 750; /* DK */
    else if (random_number <= 999965) sys->spcode[star] = 800; /* ns */
    else                              sys->spcode[star] = 900; /* bh */

    if (sys->spcode[star] < 700)
      sys->spcode[star] += randint(spart2(sys->spcode[star]) ? 0 : 5, 9);

    }

  for (star = 0; star < sys->stars - 1; star++)
    for (star2 = star + 1; star2 < sys->stars; star2++)
      if (sys->spcode[star] > sys->spcode[star2])
        {
        temptype = sys->spcode[star];
        sys->spcode[star] = sys->spcode[star2];
        sys->spcode[star2] = temptype;
        }

  do
    {

    ok = TRUE;
    sys->x = randint(0, edge - 1);
    sys->y = randint(0, edge - 1);
    sys->z = randint(0, edge - 1);

    for (count = 0; count < index; count++)
      if (syslist[count].x == sys->x && syslist[count].y == sys->y
          && syslist[count].z == sys->z)
        {
        ok = FALSE;
        break;
        }

    }
    while (!ok);

  }


/* Random integer from min to max */
long randint(long min, long max)
  {
  unsigned long divisor, range, temp;
  range = max - min + 1;
  divisor = ((unsigned long)RAND_MAX + 1)/range;
  do
    temp = rand()/divisor;
    while (temp >= range);
  return (min + temp);
  }


/* Normally distributed random real */
double randnorm(double mean, double sd)
  {
  double gauss, unit1, unit2;
  unit1 = (double)rand()/((double)RAND_MAX + 1.0);
  unit2 = (double)rand()/((double)RAND_MAX + 1.0);
  gauss = sqrt(-2.0*log(unit1))*sin(2.0*M_PI*unit2);
  return (mean + sd*gauss);
  }


/* Sort systems by coordinates */
void sort_systems(star_system *syslist, int systems)
  {
  qsort(syslist, systems, sizeof(star_system), compare_systems);
  }


/* Seed random number sequence from time */
long timeseed(void)
  {
  long seed;
  seed = time(NULL);
  srand(seed);
  return seed;
  }


/* Print usage information and quit */
void usage(void)
  {
  printf("  Star system generator\n");
  printf("    Copyright 1993 by Ross Smith\n");
  printf("    Version 1.1 2-Oct-93\n");
  printf("  Usage: STARS sector [seed]\n");
  printf("    sector = Sector size in light-years.\n");
  printf("    seed   = Seed for random number sequence.\n");
  exit(EXIT_SUCCESS);
  }

/*----------------------------------- cut here ------------------------------


--
... Ross Smith (Wanganui, New Zealand) ... alien@acheron.amigans.gen.nz ...
    "A Real Cat's aim is to get through life peacefully, with as little
interference from human beings as possible.  Very much like real humans, in
fact."  (Terry Pratchett)
*/
