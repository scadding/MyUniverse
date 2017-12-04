/*
Planetary system generator
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


#define BUFFER_SIZE   256
#define MAX_EARTHLIKE 2
#define MAX_FEATURES  3
#define MAX_PLANETS   15
#define MAX_STARS     10

#define DIAMETER_SCALE   1000.0
#define EARTH_DAY        24.0
#define GRAVITY_CONSTANT 70000.0
#define MIN_PERIOD_RATIO 1.25


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

typedef enum
  {
  LC_EARTHLIKE,
  LC_PROTEIN_BASED,
  LC_CARBON_BASED,
  LC_NON_CARBON_BASED
  }
  life_chemistry;

typedef enum
  {
  LL_NO_LIFE,
  LL_SINGLE_CELLED,
  LL_SIMPLE_INVERTEBRATES,
  LL_ADVANCED_INVERTEBRATES,
  LL_SIMPLE_VERTEBRATES,
  LL_ADVANCED_VERTEBRATES
  }
  life_level;

typedef enum
  {
  SF_ECCENTRIC_ORBIT,
  SF_HIGH_INCLINATION,
  SF_HIGH_TIDES,
  SF_METEOR_STORMS,
  SF_NO_SEASONS,
  SF_RINGS,
  SF_RADIATION_HAZARD,
  SF_RUGGED_TERRAIN,
  SF_VOLCANIC_ACTIVITY,
  SF_FRESHWATER_OCEANS,
  SF_MANY_ISLANDS,
  SF_POISONOUS_OCEANS,
  SF_SWAMPY_SURFACE,
  SF_CLOUD_COVER,
  SF_TOXIC_GAS,
  SF_UNSTABLE_CLIMATE,
  SF_VERY_DENSE_ATMOSPHERE,
  SF_VERY_THIN_ATMOSPHERE,
  SF_INTELLIGENT_LIFE,
  SF_SEMI_INTELLIGENT_LIFE,
  SF_WORLD_FOREST,
  SF_ANCIENT_ARTIFACTS,
  SF_NONE
  }
  special_feature;

/* Planet description */
typedef struct
  {
  char ptype;                            /* Planet type code */
  int features;                          /* Number of special features */
  int minerals;                          /* Mineral resources rating */
  int moons;                             /* Number of moons */
  int oceans;                            /* Ocean area (%) */
  int temperature;                       /* Average temperature (°C) */
  double density;                        /* Density (g/cm³) */
  double diameter;                       /* Diameter (km) */
  double gravity;                        /* Gravity (G) */
  double period;                         /* Orbit period (d) */
  double rotation;                       /* Rotation period (d) */
  life_chemistry biochemistry;           /* Biochemistry type */
  life_level evolution;                  /* Life development level */
  special_feature feature[MAX_FEATURES]; /* Special features */
  }
  planet_stats;

/* Star description */
typedef struct
  {
  int planets;  /* Number of planets */
  int planets2; /* Number of planets not counting asteroids */
  int spcode;   /* Coded spectral type */
  int zone_b;   /* First planet in zone B */
  int zone_c;   /* First planet in zone C */
  }
  star_stats;


const char *life_chemistry_name[] =
  {
  "Earthlike",
  "protein-based",
  "carbon-based",
  "non-carbon-based"
  };

const char *life_level_name[] =
  {
  "no life",
  "single-celled organisms",
  "simple invertebrates/plants",
  "advanced invertebrates/plants",
  "simple vertebrates",
  "advanced vertebrates"
  };

const char *special_feature_name[] =
  {
  "eccentric orbit",
  "high inclination",
  "high tides",
  "meteor storms",
  "no seasons",
  "rings",
  "radiation hazard",
  "rugged terrain",
  "volcanic activity",
  "freshwater oceans",
  "many islands",
  "poisonous oceans",
  "swampy surface",
  "cloud cover",
  "toxic gas",
  "unstable climate",
  "very dense atmosphere",
  "very thin atmosphere",
  "intelligent life",
  "semi-intelligent life",
  "world forest",
  "ancient artifacts",
  ""
  };

/* Determine whether planets are present */
boolean check_planets(star_stats *st);

/* Generate dice result */
int dice(int number, int faces);

/* Display Earthlike planet details */
void display_earthlike(planet_stats *pl, int starnum, int planetnum,
                       boolean multiple);

/* Display planetary system details */
void display_planets(planet_stats *planetlist, star_stats *st, int starnum,
                     boolean multiple);

/* Print a formatted error message and quit */
void error(char *format, ...);

/* Check two special features for compatibility */
boolean features_compatible(special_feature feature1,
                            special_feature feature2);

/* Check a special feature for validity for a particular planet */
boolean feature_valid(special_feature feature, planet_stats *pl,
                      star_stats *st, boolean multiple);

/* Generate other Earthlike planet details */
void generate_earthlike_details(planet_stats *pl, star_stats *st,
                                boolean multiple);

/* Generate orbit period for Earthlike planet */
void generate_earthlike_period(planet_stats *pl, star_stats *st);

/* Generate temperature for Earthlike planet */
void generate_earthlike_temperature(planet_stats *pl);

/* Generate basic planetary details */
void generate_planet(planet_stats *pl, star_stats *st, char zone,
                     boolean multiple);

/* Generate planetary system details */
void generate_planetary_system(planet_stats *planetlist, star_stats *st,
                               boolean multiple);

/* Random integer from min to max */
long randint(long min, long max);

/* Read star system details */
int read_stars(star_stats *starlist, int column);

/* Seed random number sequence from time */
long timeseed(void);

/* Print usage information and quit */
void usage(void);


/* Main function */
int main(int argc, char **argv)
  {

  boolean planets;
  char text[BUFFER_SIZE];
  int column, skip, line, star, stars;
  planet_stats *planet_list;
  star_stats *star_list;

  if (argc < 2)
    usage();
  if (argc < 3 || argc > 4)
    error("Wrong number of arguments.");

  skip = strtol(argv[1], NULL, 0);
  if (skip < 0)
    error("Invalid skip count.");

  column = strtol(argv[2], NULL, 0);
  if (column < 0)
    error("Invalid column number.");

  if (argc >= 4)
    srand(strtoul(argv[3], NULL, 0));
  else
    timeseed();

  planet_list = malloc(MAX_PLANETS*sizeof(planet_stats));
  if (!planet_list)
    error("Insufficient memory.");
  star_list = malloc(MAX_STARS*sizeof(star_stats));
  if (!star_list)
    {
    free(planet_list);
    error("Insufficient memory.");
    }

  for (line = 1; line <= skip; line++)
    {
    fgets(text, BUFFER_SIZE, stdin);
    if (feof(stdin))
      break;
    fputs(text, stdout);
    }

  if (!feof(stdin))
    for (;;)
      {
      stars = read_stars(star_list, column);
      if (feof(stdin))
        break;
      planets = check_planets(star_list);
      for (star = 0; star < stars; star++)
        {
        if (planets)
          generate_planetary_system(planet_list, &star_list[star],
                                    stars > 1);
        else
          star_list[star].planets = star_list[star].planets2 = 0;
        display_planets(planet_list, &star_list[star], star, stars > 1);
        }
      printf("\n");
      }

  free(planet_list);
  free(star_list);

  return EXIT_SUCCESS;

  }


/* Determine whether planets are present */
boolean check_planets(star_stats *st)
  {
  int planets_percent;
  if (spart1(st->spcode) < 3)
    planets_percent = 10;
  else if (spart1(st->spcode) < 5)
    planets_percent = 20;
  else if (spart1(st->spcode) > 5)
    planets_percent = 10;
  else
    switch (spart2(st->spcode))
      {
      case 0: case 1: planets_percent = 10; break;
      case 2:         planets_percent = 50; break;
      case 3: case 4: planets_percent = 99; break;
      case 5:         planets_percent = 99; break;
      case 6:         planets_percent = 50; break;
      }
  return (randint(1, 100) <= planets_percent);
  }


/* Generate dice result */
int dice(int number, int faces)
  {
  int count, total = 0;
  for (count = 0; count < number; count++)
    total += (int)randint(1, faces);
  return total;
  }


/* Display Earthlike planet details */
void display_earthlike(planet_stats *pl, int starnum, int planetnum,
                       boolean multiple)
  {

  char rotation_unit;
  int count, rotation_figure;
  double period_ratio;

  if (pl->rotation <= 2.0)
    {
    rotation_figure = round(EARTH_DAY*pl->rotation);
    rotation_unit = 'h';
    }
  else
    {
    rotation_figure = round(pl->rotation);
    rotation_unit = 'd';
    }

  period_ratio = pl->period/pl->rotation;

  printf("    (");
  if (multiple)
    printf("%c.", 'A' + starnum);
  printf("%d):  %s, diameter %.0f km, density %.1f g/cm³,\n",
         planetnum, pl->ptype == 'E' ? "Earthlike" : "Marginal",
         pl->diameter, pl->density);

  printf("        gravity %.1f G, rotation %d %c, period %.0f d (",
         pl->gravity, rotation_figure, rotation_unit, pl->period);
  if      (period_ratio >= 100.0) printf("%.0f", period_ratio);
  else if (period_ratio >= 10.0)  printf("%.1f", period_ratio);
  else                            printf("%.2f", period_ratio);
  printf(" local days),\n");

  printf("        temperature %d°C, oceans %d%%, mineral rating %d, ",
         pl->temperature, pl->oceans, pl->minerals);
  switch (pl->moons)
    {
    case 0: printf("no moons"); break;
    case 1: printf("1 moon"); break;
    default: printf("%d moons", pl->moons); break;
    }
  printf(";\n");

  printf("        %s", life_level_name[pl->evolution]);
  if (pl->evolution)
    printf(", %s biochemistry", life_chemistry_name[pl->biochemistry]);

  if (pl->features)
    {
    printf(";\n        ");
    for (count = 0; count < pl->features; count++)
      printf("%s%s",
             count ? ", " : "", special_feature_name[pl->feature[count]]);
    }
  printf(".\n");

  }


/* Display planetary system details */
void display_planets(planet_stats *planetlist, star_stats *st, int starnum,
                     boolean multiple)
  {

  int planet, planetnum;

  printf("    ");
  if (multiple)
    printf("(%c) ", 'A' + starnum);

  switch (spart1(st->spcode))
    {
    case 1: printf("c"); break;
    case 3: printf("g"); break;
    case 7: printf("D"); break;
    case 8: printf("ns"); break;
    case 9: printf("bh"); break;
    default: break;
    }

  if (st->spcode < 800)
    switch (spart2(st->spcode))
      {
      case 0: printf("O"); break;
      case 1: printf("B"); break;
      case 2: printf("A"); break;
      case 3: printf("F"); break;
      case 4: printf("G"); break;
      case 5: printf("K"); break;
      case 6: printf("M"); break;
      }

  if (st->spcode < 700)
    printf("%d", spart3(st->spcode));

  if (st->planets)
    {

    printf(", %d planet%s: ", st->planets, st->planets == 1 ? "" : "s");

    for (planet = 0; planet < st->planets; planet++)
      {
      if (planet == st->zone_b)
        printf(" /");
      if (planet == st->zone_c)
        printf(" /");
      printf(" %c", planetlist[planet].ptype);
      if (planetlist[planet].ptype != 'A')
        printf("%d", round(planetlist[planet].diameter/DIAMETER_SCALE));
      if (planetlist[planet].moons)
        printf(".%d", planetlist[planet].moons);
      }

    if (st->zone_b >= st->planets)
      printf(" /");
    if (st->zone_c >= st->planets)
      printf(" /");

    printf("\n");

    for (planet = 0, planetnum = 0; planet < st->planets; planet++)
      {
      if (planetlist[planet].ptype != 'A')
        planetnum++;
      if (planetlist[planet].ptype == 'E'
          || planetlist[planet].ptype == 'M')
        display_earthlike(&planetlist[planet], starnum, planetnum,
                          multiple);
      }

    }

  else

    printf(", no planets\n");

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


/* Check two special features for compatibility */
boolean features_compatible(special_feature feature1,
                            special_feature feature2)
  {

  if (feature1 == SF_NONE || feature2 == SF_NONE)
    return TRUE;

  if (feature1 == SF_NO_SEASONS
      && (feature2 == SF_ECCENTRIC_ORBIT
          || feature2 == SF_HIGH_INCLINATION))
    return FALSE;

  if (feature2 == SF_NO_SEASONS
      && (feature1 == SF_ECCENTRIC_ORBIT
          || feature1 == SF_HIGH_INCLINATION))
    return FALSE;

  if (feature1 == SF_RUGGED_TERRAIN && feature2 == SF_SWAMPY_SURFACE)
    return FALSE;

  if (feature2 == SF_RUGGED_TERRAIN && feature1 == SF_SWAMPY_SURFACE)
    return FALSE;

  if (feature1 == SF_VERY_THIN_ATMOSPHERE
      && (feature2 == SF_CLOUD_COVER
          || feature2 == SF_VERY_DENSE_ATMOSPHERE))
    return FALSE;

  if (feature2 == SF_VERY_THIN_ATMOSPHERE
      && (feature1 == SF_CLOUD_COVER
          || feature1 == SF_VERY_DENSE_ATMOSPHERE))
    return FALSE;

  if (feature1 == SF_INTELLIGENT_LIFE
      && feature2 == SF_SEMI_INTELLIGENT_LIFE)
    return FALSE;

  if (feature2 == SF_INTELLIGENT_LIFE
      && feature1 == SF_SEMI_INTELLIGENT_LIFE)
    return FALSE;

  return TRUE;

  }


/* Check a special feature for validity for a particular planet */
boolean feature_valid(special_feature feature, planet_stats *pl,
                      star_stats *st, boolean multiple)
  {

  boolean status;

  switch (feature)
    {

    case SF_METEOR_STORMS:
    case SF_RADIATION_HAZARD:
    case SF_VOLCANIC_ACTIVITY:
    case SF_TOXIC_GAS:
    case SF_VERY_DENSE_ATMOSPHERE:
    case SF_VERY_THIN_ATMOSPHERE:
      status = pl->ptype == 'M';
      break;

    case SF_HIGH_TIDES:
      status = st->spcode >= 550 || pl->moons;
      break;

    case SF_NO_SEASONS:
      status = !multiple;
      break;

    case SF_RUGGED_TERRAIN:
      status = pl->oceans < 100;
      break;

    case SF_FRESHWATER_OCEANS:
      status = !!pl->oceans;
      break;

    case SF_MANY_ISLANDS:
      status = pl->oceans && pl->oceans < 100;
      break;

    case SF_POISONOUS_OCEANS:
      status = !!pl->oceans;
      break;

    case SF_SWAMPY_SURFACE:
      status = pl->oceans < 100;
      break;

    case SF_CLOUD_COVER:
      status = pl->temperature > 0;
      break;

    case SF_INTELLIGENT_LIFE:
      status = pl->evolution >= LL_ADVANCED_VERTEBRATES
               || (pl->evolution >= LL_ADVANCED_INVERTEBRATES
                   && randint(1, 4) == 1);
      break;

    case SF_SEMI_INTELLIGENT_LIFE:
      status = pl->evolution >= LL_ADVANCED_VERTEBRATES
               || (pl->evolution >= LL_ADVANCED_INVERTEBRATES
                   && randint(1, 4) == 1);
      break;

    case SF_WORLD_FOREST:
      status = pl->evolution && pl->oceans < 100;
      break;

    default:
      status = TRUE;
      break;

    }

  return status;

  }


/* Generate other Earthlike planet details */
void generate_earthlike_details(planet_stats *pl, star_stats *st,
                                boolean multiple)
  {

  boolean ok;
  int count, count2, percent, random_number;
  special_feature temp_feature;

  pl->density = 0.5*(double)(randint(7, 12));
  pl->gravity = pl->density*pl->diameter/GRAVITY_CONSTANT;

  percent = randint(1, 100) + 10*pl->moons;
  if (percent <= 65)
    pl->rotation = ((double)randint(10, 29))/EARTH_DAY;
  else if (percent <= 90)
    pl->rotation = ((double)randint(21, 40))/EARTH_DAY;
  else if (percent <= 98)
    pl->rotation = (double)randint(1, 10);
  else if (percent <= 103)
    pl->rotation = (double)randint(1, 100);
  else
    pl->rotation = (double)randint(1, 10);

  pl->oceans = (dice(2, 6) - 2)*10;
  if (pl->temperature == 0)
    pl->oceans -= 20;
  else if (pl->temperature < 0)
    pl->oceans -= 40;
  if (pl->oceans < 0)
    pl->oceans = 0;

  pl->minerals = round(pl->density + pl->diameter/10000.0
                 + (double)randint(-3, 2));

  percent = randint(1, 100);
  if (pl->ptype == 'E')
    {
    if      (percent <= 10) pl->evolution = LL_SINGLE_CELLED;
    else if (percent <= 20) pl->evolution = LL_SIMPLE_INVERTEBRATES;
    else if (percent <= 30) pl->evolution = LL_ADVANCED_INVERTEBRATES;
    else if (percent <= 40) pl->evolution = LL_SIMPLE_VERTEBRATES;
    else                    pl->evolution = LL_ADVANCED_VERTEBRATES;
    }
  else
    {
    if      (percent <= 1)  pl->evolution = LL_NO_LIFE;
    else if (percent <= 30) pl->evolution = LL_SINGLE_CELLED;
    else if (percent <= 45) pl->evolution = LL_SIMPLE_INVERTEBRATES;
    else if (percent <= 60) pl->evolution = LL_ADVANCED_INVERTEBRATES;
    else if (percent <= 75) pl->evolution = LL_SIMPLE_VERTEBRATES;
    else                    pl->evolution = LL_ADVANCED_VERTEBRATES;
    }

  percent = randint(1, 100);
  if (pl->ptype == 'E')
    {
    if      (percent <= 20) pl->biochemistry = LC_EARTHLIKE;
    else if (percent <= 80) pl->biochemistry = LC_PROTEIN_BASED;
    else                    pl->biochemistry = LC_CARBON_BASED;
    }
  else
    {
    if      (percent <= 5)  pl->biochemistry = LC_EARTHLIKE;
    else if (percent <= 60) pl->biochemistry = LC_PROTEIN_BASED;
    else if (percent <= 99) pl->biochemistry = LC_CARBON_BASED;
    else                    pl->biochemistry = LC_NON_CARBON_BASED;
    }

  pl->features = 0;

  for (count = 0; count < MAX_FEATURES; count++)
    {
    do
      {
      random_number = randint(0, 99);
      pl->feature[count] =
        random_number <= SF_NONE ? random_number : SF_NONE;
      ok = feature_valid(pl->feature[count], pl, st, multiple);
      if (ok)
        for (count2 = 0; count2 < count; count2++)
          if (!features_compatible(pl->feature[count2],
                                   pl->feature[count]))
            {
            ok = FALSE;
            break;
            }
      }
      while (!ok);
    if (pl->feature[count] != SF_NONE)
      pl->features++;
    }

  for (count = 0; count < MAX_FEATURES - 1; count++)
    for (count2 = count + 1; count2 < MAX_FEATURES; count2++)
      if (pl->feature[count] > pl->feature[count2])
        {
        temp_feature = pl->feature[count];
        pl->feature[count] = pl->feature[count2];
        pl->feature[count2] = temp_feature;
        }

  }


/* Generate orbit period for Earthlike planet */
void generate_earthlike_period(planet_stats *pl, star_stats *st)
  {
  if (st->spcode <= 534)
    pl->period = 600.0 + 3.0*(double)dice(2, 100);
  else if (st->spcode <= 539)
    pl->period = 400.0 + 2.0*(double)dice(2, 100);
  else if (st->spcode <= 544)
    pl->period = 270.0 + (double)dice(2, 100);
  else if (st->spcode <= 549)
    pl->period = 150.0 + (double)dice(2, 100);
  else if (st->spcode <= 554)
    pl->period = 120.0 + (double)randint(1, 100);
  else
    pl->period = 70.0 + (double)randint(1, 100);
  }


/* Generate temperature for Earthlike planet */
void generate_earthlike_temperature(planet_stats *pl)
  {
  switch (pl->ptype)
    {
    case 'E':
      pl->temperature = (dice(2, 6) - 4)*5;
      break;
    case 'M':
      pl->temperature = (randint(1, 10) + randint(1, 6) - 6)*5;
      break;
    }
  }


/* Generate basic planetary details */
void generate_planet(planet_stats *pl, star_stats *st, char zone,
                     boolean multiple)
  {

  int percent, random_number;

  percent = randint(1, 100);

  if (zone == 'A')
    {

    if      (percent <= 5)  pl->ptype = 'A';
    else if (percent <= 60) pl->ptype = 'R';
    else if (percent <= 70) pl->ptype = 'D';
    else                    pl->ptype = 'H';

    }
  else if (zone == 'B')
    {

    if (st->spcode >= 530 && st->spcode < 560)

      {

      if      (percent <= 5)  pl->ptype = 'A';
      else if (percent <= 8)  pl->ptype = 'G';
      else if (percent <= 40) pl->ptype = 'R';
      else if (percent <= 60) pl->ptype = 'D';
      else if (percent <= 80) pl->ptype = 'H';
      else if (percent <= 90) pl->ptype = 'M';
      else                    pl->ptype = 'E';

      }
    else
      {

      if      (percent <= 5)  pl->ptype = 'A';
      else if (percent <= 8)  pl->ptype = 'G';
      else if (percent <= 40) pl->ptype = 'R';
      else if (percent <= 70) pl->ptype = 'D';
      else                    pl->ptype = 'H';

      }

    }
  else
    {

    if      (percent <= 5)  pl->ptype = 'A';
    else if (percent <= 75) pl->ptype = 'G';
    else if (percent <= 80) pl->ptype = 'R';
    else if (percent <= 95) pl->ptype = 'J';
    else                    pl->ptype = 'H';

    }

  if (st->spcode >= 800 && pl->ptype == 'H')
    pl->ptype = 'J';

  switch (pl->ptype)
    {
    case 'A': pl->diameter = 0.0; break;
    case 'G': pl->diameter = 10000.0*(double)dice(3, 6); break;
    case 'R': pl->diameter = 1000.0*(double)randint(1, 10); break;
    case 'J': pl->diameter = 1000.0*(double)randint(1, 10); break;
    case 'D': pl->diameter = 1000.0*(double)(dice(2, 6) + 2); break;
    case 'H': pl->diameter = 1000.0*(double)(dice(3, 6) + 1); break;
    case 'M': pl->diameter = 1000.0*(double)(dice(2, 6) + 5); break;
    case 'E': pl->diameter = 1000.0*(double)(dice(2, 6) + 5); break;
    }

  switch (pl->ptype)
    {
    case 'A':
      pl->moons = 0;
      break;
    case 'G':
      pl->moons = dice(2, 10);
      break;
    default:
      random_number = randint(1, 10);
      if      (random_number <= 4) pl->moons = 0;
      else if (random_number <= 7) pl->moons = 1;
      else if (random_number <= 9) pl->moons = 2;
      else                         pl->moons = 3;
      break;
    }

  }


/* Generate planetary system details */
void generate_planetary_system(planet_stats *planetlist, star_stats *st,
                               boolean multiple)
  {

  char zone;
  int count, count2, diff, min_diff, number_earthlike, planet, planets_d1,
      planets_d2, planets_d3, earthlike[MAX_EARTHLIKE];
  double min_ratio, ratio, temp_period;

  if (spart1(st->spcode) < 3)
    {

    planets_d1 = 1;
    planets_d2 = 6;
    planets_d3 = 0;

    }
  else if (spart1(st->spcode) < 5)
    {

    planets_d1 = 1;
    planets_d2 = 6;
    planets_d3 = 0;

    }
  else if (spart1(st->spcode) > 5)
    {

    planets_d1 = 1;
    planets_d2 = 3;
    planets_d3 = 0;

    }
  else

    switch (spart2(st->spcode))
      {

      case 0:
      case 1:
        planets_d1 = 1;
        planets_d2 = 10;
        planets_d3 = 0;
        break;

      case 2:
        planets_d1 = 1;
        planets_d2 = 10;
        planets_d3 = 0;
        break;

      case 3:
      case 4:
        planets_d1 = 2;
        planets_d2 = 6;
        planets_d3 = 3;
        break;

      case 5:
        planets_d1 = 2;
        planets_d2 = 6;
        planets_d3 = 0;
        break;

      case 6:
        planets_d1 = 1;
        planets_d2 = 6;
        planets_d3 = 0;
        break;

      }

  st->planets2 = 0;
  st->planets = dice(planets_d1, planets_d2) + planets_d3;

  if (st->spcode >= 700)
    st->zone_b = st->zone_c = 0;
  else
    {
    if (st->planets <= 3)
      st->zone_b = 0;
    else if (st->planets <= 7)
      st->zone_b = 1;
    else
      st->zone_b = 2;
    if (st->planets <= 5)
      st->zone_c = st->zone_b + 1;
    else
      st->zone_c = st->zone_b + 2;
    }

  if (multiple)
    {
    st->planets -= randint(1, 6) + 1;
    if (st->planets < 0)
      st->planets = 0;
    }

  number_earthlike = 0;

  for (planet = 0; planet < st->planets; planet++)
    {
    if (planet < st->zone_b)
      zone = 'A';
    else if (planet < st->zone_c)
      zone = 'B';
    else
      zone = 'C';
    generate_planet(&planetlist[planet], st, zone, multiple);
    if (planetlist[planet].ptype != 'A')
      st->planets2++;
    if (planetlist[planet].ptype == 'E' || planetlist[planet].ptype == 'M')
      earthlike[number_earthlike++] = planet;
    }

  if (number_earthlike == 1)
    {

    generate_earthlike_period(&planetlist[earthlike[0]], st);
    generate_earthlike_temperature(&planetlist[earthlike[0]]);

    }
  else if (number_earthlike > 1)
    {

    do
      {
      for (count = 0; count < number_earthlike; count++)
        generate_earthlike_period(&planetlist[earthlike[count]], st);
      for (count = 0; count < number_earthlike - 1; count++)
        for (count2 = count + 1; count2 < number_earthlike; count2++)
          if (planetlist[earthlike[count]].period
              > planetlist[earthlike[count2]].period)
            {
            temp_period = planetlist[earthlike[count]].period;
            planetlist[earthlike[count]].period =
              planetlist[earthlike[count2]].period;
            planetlist[earthlike[count2]].period = temp_period;
            }
      min_ratio = MIN_PERIOD_RATIO + 1.0;
      for (count = 0, count2 = 1; count2 < number_earthlike;
           count++, count2++)
        {
        ratio = planetlist[earthlike[count2]].period
                /planetlist[earthlike[count]].period;
        if (ratio < min_ratio)
          {
          min_ratio = ratio;
          if (min_ratio < MIN_PERIOD_RATIO)
            break;
          }
        }
      }
      while (min_ratio < MIN_PERIOD_RATIO);

    do
      {
      for (count = 0; count < number_earthlike; count++)
        generate_earthlike_temperature(&planetlist[earthlike[count]]);
      min_diff = 0;
      for (count = 0, count2 = 1; count2 < number_earthlike;
           count++, count2++)
        {
        diff = planetlist[earthlike[count]].temperature
               - planetlist[earthlike[count2]].temperature;
        if (diff < min_diff)
          min_diff = diff;
        }
      }
      while (min_diff < 0);

    }

  for (count = 0; count < number_earthlike; count++)
    generate_earthlike_details(&planetlist[earthlike[count]], st,
                               multiple);

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


/* Read star system details */
int read_stars(star_stats *starlist, int column)
  {

  boolean last_alnum = FALSE, this_alnum;
  char *cp,
       text[BUFFER_SIZE];
  int stars = 0;

  fgets(text, BUFFER_SIZE, stdin);
  if (feof(stdin))
    return 0;
  fputs(text, stdout);

  if (column)
    cp = text + column - 1;
  else
    for (cp = text; *cp && *cp != '('; cp++);
  while (*cp && !isalnum(*cp))
    cp++;

  starlist[0].spcode = 500;

  while (*cp && isgraph(*cp))
    {

    this_alnum = isalnum(*cp);

    if (this_alnum)
      {

      if (!last_alnum)
        {
        if (stars >= MAX_STARS)
          break;
        stars++;
        starlist[stars - 1].spcode = 500;
        }

      if (isalpha(*cp))
        {
        switch (*cp)
          {
          case 'c': starlist[stars - 1].spcode = 100; break;
          case 'g': starlist[stars - 1].spcode = 300; break;
          case 'D': starlist[stars - 1].spcode = 700; break;
          case 'n': starlist[stars - 1].spcode = 800; break;
          case 'b': starlist[stars - 1].spcode = 900; break;
          case 'B': starlist[stars - 1].spcode += 10; break;
          case 'A': starlist[stars - 1].spcode += 20; break;
          case 'F': starlist[stars - 1].spcode += 30; break;
          case 'G': starlist[stars - 1].spcode += 40; break;
          case 'K': starlist[stars - 1].spcode += 50; break;
          case 'M': starlist[stars - 1].spcode += 60; break;
          default: break;
          }
        }
      else if (isdigit(*cp))
        starlist[stars - 1].spcode += *cp - '0';

      }

    last_alnum = this_alnum;
    cp++;

    }

  return stars;

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
  printf("  Planetary system generator\n");
  printf("    Copyright 1993 by Ross Smith\n");
  printf("    Version 1.1 2-Oct-93\n");
  printf("  Usage: PLANETS skip column [seed]\n");
  printf("    skip   = Input lines to skip.\n");
  printf("    column = Column number for spectral types.\n");
  printf("    seed   = Seed for random number sequence.\n");
  printf("  A list of stars is read from standard input. Spectral\n");
  printf("  types are extracted from a specified column, or, if no\n");
  printf("  column number is supplied, from after the first left\n");
  printf("  parenthesis.\n");
  exit(EXIT_SUCCESS);
  }

/*
... Ross Smith (Wanganui, New Zealand) ... alien@acheron.amigans.gen.nz ...
    "A Real Cat's aim is to get through life peacefully, with as little
interference from human beings as possible.  Very much like real humans, in
fact."  (Terry Pratchett)
*/
