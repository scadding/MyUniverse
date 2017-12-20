/* Gliese-3 stardata generator, $Revision$
 * Written by Brad Jones 960322
 * Based on the 2.0 stardata generator by Winchell Chung
 * This program is released under the GNU General Program License.
 * It may be redistributed and modified freely, but attributions must be made.
 * 
 * Command line options: (NOTE: X, Y, and Z are numbers; FILE is a filename)
 * '-n' turns on non-interactive mode
 * '-ref X Y Z' sets the reference point to (X, Y, Z)
 *     Default is (0, 0, 0)
 * '-mindist X' changes minimum distance to X; default is 0
 * '-maxdist X' changes maximum distance to X; default is 5
 * '-minlum X' changes minimum luminosity to X; default is 0
 * '-maxlum X' changes maximum luminosity to X; default is 200
 * '-nobin' turns off binaries; default is binaries on
 * '-long' turns on long output; default is short
 * '-xlong' turns on extra-long output; default is short
 * '-o FILE' sets the output filename to FILE; default is 'gliese3.out'
 *
 * Revision history:
 * Revision 1.0: created by Brad Jones
 * Revision 1.1: 970204 Winchell Chung: added RA and Dec to xlong output
 * Revision 1.2: 970731 Winchell Chung: fixed "remarks" bug
 *                      (if the remarks field was empty in data file,
 *                       program would print what was left over in buffer)
 *                       Altered StringCopy()
 * 
 */

#include <stdio.h>
#include <math.h>
#include <string.h>

/* Added by Winchell Chung */
#include <stddef.h>
#include <stdlib.h>
#include <float.h>


typedef int boolean;
#define TRUE 1
#define FALSE 0

#define DEG_TO_RAD 0.0174532925

/* This structure holds the data for one record of the Gliese-3 catalog.
   Each field is commented with the name of its field in the catalog data
   and its datatype in [square brackets].  Lengths of character arrays have
   had 1 added to them to allow room for the terminating '\0'. */
typedef struct stardata
{
  char    identifier[9];        /* Identifier of the star [A8]. */
  char    components[3];        /* Component of multi-star system [A2] */
  char    distrel;              /* Reliability of the distance [A1] */
  int     ra_hours;             /* Right ascension, hours component [I2] */
  int     ra_minutes;           /* Right ascension, minutes component [I2] */
  int     ra_seconds;           /* Right ascension, seconds component [I2] */
  int     dec_degrees;          /* Declination, degrees incl. sign [A1 + I2] */
  double  dec_minutes;          /* Declination, minutes component [F4.1] */
  double  propermotion;         /* Total proper motion [F6.3] */
  boolean pm_uncertain;         /* TRUE if proper motion uncertain [A1] */
  double  pm_direction;         /* Direction angle of proper motion [F5.1] */
  double  radialvelocity;       /* Radial velocity [F6.1] */
  char    rv_remark[4];         /* Remarks on radial velocity [A3] */
  char    spectraltype[13];     /* Spectral type or color class [A12] */
  char    spectraltype_source;  /* Selected source for spectral type [A1] */
  double  magnitude;            /* Apparent magnitude [F6.2] */
  char    magnitude_origin;     /* Origin of magnitude [A1] */
  boolean joint_magnitude;      /* TRUE if joint magnitude [A1] */
  double  b_v_magnitude;        /* B-V color magnitude [F5.2] */
  char    b_v_origin;           /* B-V magnitude origin [A1] */
  boolean joint_b_v;            /* TRUE if joint B-V magnitude [A1] */
  double  u_b_magnitude;        /* U-B color magnitude [F5.2] */
  char    u_b_origin;           /* U-B magnitude origin [A1] */
  boolean joint_u_b;            /* TRUE if joint U-B magnitude [A1] */
  double  r_i_magnitude;        /* R-I color magnitude [F5.2] */
  char    r_i_origin;           /* R-I magnitude origin [A1] */
  boolean joint_r_i;            /* TRUE if joint R-I magnitude [A1] */
  double  trig_parallax;        /* Trigonometric parallax [F6.1] */
  double  trig_parallax_error;  /* Error in trig. parallax [F5.1] */
  double  parallax;             /* Resulting parallax [F6.1] */
  double  parallax_error;       /* Parallax error [F5.1] */
  char    parallax_code;        /* Code for parallax origin [A1] */
  double  absolute_magnitude;   /* Absolute visual magnitude [F5.2] */
  char    abs_mag_notes[3];     /* Notes on visual magnitude [A2] */
  char    abs_mag_quality;      /* Qualify of absolute magnitude [A1] */
  int     velocity_u;           /* U component of velocity [I4] */
  int     velocity_v;           /* V component of velocity [I4] */
  int     velocity_w;           /* W component of velocity [I4] */
  long    hd_designation;       /* HD Designation [I6] */
  char    dm_designation[13];   /* DM designation [A12] */
  char    giclas_number[10];    /* Giclas number [A9] */
  char    lhs_number[6];        /* LHS number [A5] */
  char    other_designation[6]; /* Other designations [A5] */
  char    remarks[70];          /* Additional information [A69] */
  /* Data below here is calculated by the program, not read from the file */
  double  distance;             /* Distance from Sol, equals 1/parallax */
  double  ra;                   /* Right ascension in decimal degrees */
  double  declination;          /* Declination in decimal degrees */
  double  x;                    /* X position in earth-centric coords */
  double  y;                    /* Y position in earth-centric coords */
  double  z;                    /* Z position in earth-centric coords */
  double  xg;                   /* X position in galactic coords */
  double  yg;                   /* Y position in galactic coords */
  double  zg;                   /* Z position in galactic coords */
  double  ref_distance;         /* Distance from reference point Xr, Yr, Zr */
  double  luminosity;           /* Luminosity relative to Sol */
} stardata;

/* This structure holds the current constraints about what to print */
typedef struct constraints
{
  double  ref_x;                /* X position of reference point */
  double  ref_y;                /* Y position of reference point */
  double  ref_z;                /* Z position of reference point */
  double  mindist;              /* Minimum distance from reference point */
  double  maxdist;              /* Maximum distance from reference point */
  double  minlum;               /* Minimum luminosity */
  double  maxlum;               /* Maximum luminosity */
  boolean allowbinaries;        /* Allow binaries */
  int     outputstyle;          /* SHORT, LONG, or XLONG */
  char    outputfilename[256];  /* Name of output file */
  FILE   *outputfile;           /* Output file handle */
  boolean interactive;          /* Ask the user for information? */
} constraints;

#define OUTPUTSTYLE_SHORT 0
#define OUTPUTSTYLE_LONG  1
#define OUTPUTSTYLE_XLONG 2

/* This function takes two strings and the array offsets in the source string
   to copy.  NOTE WELL: The offsets are _NOT_ C offsets; they are the offsets
   as given in GLIESE3.DOC.  All newline and linefeed characters are turned
   into spaces. */

void string_copy(char *deststr, char *srcstr, int start, int end)
{
  char *substring = srcstr + start - 1;
  char *temp;

  if (strlen(srcstr) < start) {	/* added 970731 Winchell Chung */
	deststr[0] = '\0';
    return;
  }

  strncpy(deststr, substring, (end - start + 1));
  deststr[end - start + 1] = '\0';

  /* Remove 'carriage return/newline' from strings */
  temp = strchr(deststr, '\n');
  if (temp)
    *temp = ' ';
  temp = strchr(deststr, '\r');
  if (temp)
    *temp = ' ';
  
}

/* Take a section of the source string and get an int out of it.  Same caveats
   as for string_copy. */
int string_to_int(char *srcstr, int start, int end)
{
  int outval;
  char formatstring[4];
  char *tempstring = (char *)malloc(end - start + 2);
  string_copy(tempstring, srcstr, start, end);

  sprintf(formatstring, "%%%dd", (end - start + 1));
  if (sscanf(tempstring, formatstring, &outval) != 1)
    return 0;
  else
    return outval;
}
  
/* Take a section of the source string and get a double out of it.  Same
   caveats as for string_copy. */
double string_to_double(char *srcstr, int start, int end)
{
  double outval;
  char formatstring[5];
  char *tempstring = (char *)malloc(end - start + 2);
  string_copy(tempstring, srcstr, start, end);

  sprintf(formatstring, "%%%dlf", (end - start + 1));
  if (sscanf(tempstring, formatstring, &outval) != 1)
    return 0.0;
  else
    return outval;
}

/* Take a section of the source string and get a long out of it.  Same caveats
   as for string_copy. */
long string_to_long(char *srcstr, int start, int end)
{
  long outval;
  char formatstring[5];
  char *tempstring = (char *)malloc(end - start + 2);
  string_copy(tempstring, srcstr, start, end);

  sprintf(formatstring, "%%%dld", (end - start + 1));
  if (sscanf(tempstring, formatstring, &outval) != 1)
    return 0;
  else
    return outval;
}
  
/* Do the math on the raw data to find the calculated data, like distance
   and luminosity */
void calculate_data(stardata *star, constraints *settings)
{
  double rvect, phi, theta, rho;
  double delta_x, delta_y, delta_z;

  if (star->parallax != 0.0)
    star->distance = 1.0 / star->parallax;
  else
    star->distance = 0.0;
  star->ra = (star->ra_hours * 15) + (star->ra_minutes * 0.25) +
    (star->ra_seconds * 0.0041666666);
  star->declination = fabs(star->dec_degrees) + (star->dec_minutes / 60.0);
  if (star->dec_degrees < 0.0) star->declination = -star->declination;
  
  phi = star->ra * DEG_TO_RAD;
  theta = star->declination * DEG_TO_RAD;
  rho = star->distance;

  rvect = rho * cos(theta);

  star->x = rvect * cos(phi);
  star->y = rvect * sin(phi);
  star->z = rho * sin(theta);

  star->xg = -(0.0672 * star->x) - (0.8727 * star->y) - (0.4835 * star->z);
  star->yg = (0.4927 * star->x) - (0.4504 * star->y) + (0.7445 * star->z);
  star->zg = -(0.8676 * star->x) - (0.1884 * star->y) + (0.4602 * star->z);

  delta_x = star->x - settings->ref_x;
  delta_y = star->y - settings->ref_y;
  delta_z = star->z - settings->ref_z;
  star->ref_distance = sqrt((delta_x * delta_x) + (delta_y * delta_y) +
                            (delta_z * delta_z));

  star->luminosity = pow(10, (0.4 * (4.9 - star->absolute_magnitude)));
}

/* Convert a text string into a stardata object */
void convert_record(char *instring, stardata *star, constraints *settings)
{
  string_copy(star->identifier, instring, 1, 8);
  string_copy(star->components, instring, 9, 10);
  star->distrel = instring[10];
  star->ra_hours = string_to_int(instring, 13, 14);
  star->ra_minutes = string_to_int(instring, 16, 17);
  star->ra_seconds = string_to_int(instring, 19, 20);
  star->dec_degrees = string_to_int(instring, 22, 24);
  star->dec_minutes = string_to_double(instring, 26, 29);
  star->propermotion = string_to_double(instring, 31, 36);
  star->pm_uncertain = (instring[36] != ' ');
  star->pm_direction = string_to_double(instring, 38, 42);
  star->radialvelocity = string_to_double(instring, 44, 49);
  string_copy(star->rv_remark, instring, 51, 53);
  string_copy(star->spectraltype, instring, 55, 66);
  star->spectraltype_source = instring[66];
  star->magnitude = string_to_double(instring, 68, 73);
  star->magnitude_origin = instring[73];
  star->joint_magnitude = (instring[74] != ' ');
  star->b_v_magnitude = string_to_double(instring, 76, 80);
  star->b_v_origin = instring[80];
  star->joint_b_v = (instring[81] != ' ');
  star->u_b_magnitude = string_to_double(instring, 83, 87);
  star->u_b_origin = instring[87];
  star->joint_u_b = (instring[88] != ' ');
  star->r_i_magnitude = string_to_double(instring, 90, 94);
  star->r_i_origin = instring[94];
  star->joint_r_i = (instring[95] != ' ');
  star->trig_parallax = string_to_double(instring, 97, 102) / 1000;
  star->trig_parallax_error = string_to_double(instring, 103, 107) / 1000;
  star->parallax = string_to_double(instring, 109, 114) / 1000;
  star->parallax_error = string_to_double(instring, 115, 119) / 1000;
  star->parallax_code = instring[119];
  star->absolute_magnitude = string_to_double(instring, 122, 126);
  string_copy(star->abs_mag_notes, instring, 127, 128);
  star->abs_mag_quality = instring[128];
  star->velocity_u = string_to_int(instring, 132, 135);
  star->velocity_v = string_to_int(instring, 137, 140);
  star->velocity_w = string_to_int(instring, 142, 145);
  star->hd_designation = string_to_long(instring, 147, 152);
  string_copy(star->dm_designation, instring, 154, 165);
  string_copy(star->giclas_number, instring, 167, 175);
  string_copy(star->lhs_number, instring, 177, 181);
  string_copy(star->other_designation, instring, 183, 187);
  string_copy(star->remarks, instring, 189, 257);

  calculate_data(star, settings);
}

/* Check if this star should be printed; print it if it should */
void print_record(stardata *star, constraints *settings)
{
  if ((star->ref_distance <= settings->maxdist) &&
      (star->ref_distance >= settings->mindist) &&
      (star->luminosity <= settings->maxlum) &&
      (star->luminosity >= settings->minlum) &&
      (settings->allowbinaries || (star->components[0] == ' ')))
  {
    if (settings->outputstyle == OUTPUTSTYLE_SHORT)
    {
      fprintf(settings->outputfile,
              "%8s%2s %12s%9.4f %9.4f %+8.3f %+8.3f %+8.3f %8.5f %s\n",
              star->identifier, star->components, star->spectraltype,
              star->distance, star->ref_distance, star->x, star->y, star->z,
              star->luminosity, star->remarks);
    }
    else
    {
      fprintf(settings->outputfile, 
              "-----------------------------------------------------------\n");
      fprintf(settings->outputfile, "Gliese code: %s%s", star->identifier,
              star->components);
      fprintf(settings->outputfile, "Spectral type: %s", star->spectraltype);
      fprintf(settings->outputfile, "Luminosity: %8.5f\n", star->luminosity);
      fprintf(settings->outputfile, "Remarks: %s\n", star->remarks);
      fprintf(settings->outputfile, "Distance: %9.4f pc", star->distance);
      fprintf(settings->outputfile, "    (%5.4f pc from reference point)\n",
              star->ref_distance);
      fprintf(settings->outputfile,
              "Coordinates: Earth-based (%+5.3f, %+5.3f, %+5.3f)\n",
              star->x, star->y, star->z);
      fprintf(settings->outputfile,
              "             Galactic (%+5.3f, %+5.3f, %+5.3f)\n",
              star->xg, star->yg, star->zg);
      if (settings->outputstyle == OUTPUTSTYLE_XLONG)
      {
        fprintf(settings->outputfile, 
                "HD Designation: %12ld    DM Number: %s\n",
                star->hd_designation, star->dm_designation);
        fprintf(settings->outputfile,
     "Giclas designation: %s   LHS Designation: %s\nOther designation: %s\n",
                star->giclas_number, star->lhs_number,
				star->other_designation);
		fprintf(settings->outputfile,
				"Right Ascension: %dh %dm %ds   Declination: %dd %4.1fs\n",
				star->ra_hours, star->ra_minutes, star->ra_seconds,
				star->dec_degrees, star->dec_minutes);
	  }
    }
  }
}


/* Input one double-precision value.  Repeat until a valid value is entered. */
double input_double(void)
{
  char input[256];
  double output;

  while (TRUE)
  {
    fgets(input, 256, stdin);
    if (sscanf(input, " %lf ", &output) != 1)
    {
      printf("Entry not understood.  Please try again.\n");
    }
    else
    {
      return output;
    }
  }
}

/* Input an integer.  Repeat until a valid value is entered. */
int input_integer(void)
{
  char input[256];
  int output;

  while (TRUE)
  {
    fgets(input, 256, stdin);
    if (sscanf(input, " %d ", &output) != 1)
    {
      printf("Entry not understood.  Please try again.\n");
    }
    else
    {
      return output;
    }
  }
}

/* Set the settings to reasonable defaults */
void init_settings(constraints *settings)
{
  settings->ref_x = 0.0;
  settings->ref_y = 0.0;
  settings->ref_z = 0.0;
  settings->mindist = 0.0;
  settings->maxdist = 5.0;
  settings->minlum = 0.0;
  settings->maxlum = 200.0;
  settings->allowbinaries = TRUE;
  settings->outputstyle = OUTPUTSTYLE_SHORT;
  strcpy(settings->outputfilename, "gliese3.out");
  settings->interactive = TRUE;
  return;
}

/* Get the settings from the user */
void get_user_settings(constraints *settings)
{
  boolean done = FALSE;

  while (!done)
  {
    printf("\nCurrent settings:\n\n");
    printf("1) Reference point:    (%4.3f, %4.3f, %4.3f)\n", settings->ref_x,
           settings->ref_y, settings->ref_z);
    printf("2) Minimum distance:   %4.3f\n", settings->mindist);
    printf("3) Maximum distance:   %4.3f\n", settings->maxdist);
    printf("4) Minimum luminosity: %4.3f\n", settings->minlum);
    printf("5) Maximum luminosity: %4.3f\n", settings->maxlum);
    printf("6) Allow multiples:    %s\n", (settings->allowbinaries ? "TRUE" 
                                           : "FALSE"));
    printf("7) Output style:       %s\n",
           (settings->outputstyle == OUTPUTSTYLE_SHORT ? "SHORT" :
            (settings->outputstyle == OUTPUTSTYLE_LONG ? "LONG" :
             "EXTRALONG")));
    printf("8) Output filename:    %s\n", settings->outputfilename);
    printf("9) DONE\n\n");
    printf("Enter selection: ");
    switch(input_integer())
    {
    case 1:
      printf("Enter reference point X coordinate: ");
      settings->ref_x = input_double();
      printf("Enter reference point Y coordinate: ");
      settings->ref_y = input_double();
      printf("Enter reference point Z coordinate: ");
      settings->ref_z = input_double();
      break;
    case 2:
      printf("Enter minimum distance: ");
      settings->mindist = input_double();
      if (settings->maxdist < settings->mindist)
        settings->maxdist = settings->mindist;
      break;
    case 3:
      printf("Enter maximum distance: ");
      settings->maxdist = input_double();
      if (settings->mindist > settings->maxdist)
        settings->mindist = settings->maxdist;
      break;
    case 4:
      printf("Enter minimum luminosity: ");
      settings->minlum = input_double();
      if (settings->maxlum < settings->minlum)
        settings->maxlum = settings->minlum;
      break;
    case 5:
      printf("Enter maximum luminosity: ");
      settings->maxlum = input_double();
      if (settings->minlum > settings->maxlum)
        settings->minlum = settings->maxlum;
      break;
	case 6:
      settings->allowbinaries = !settings->allowbinaries;
      break;
    case 7:
      settings->outputstyle++;
      if (settings->outputstyle > OUTPUTSTYLE_XLONG)
        settings->outputstyle = OUTPUTSTYLE_SHORT;
      break;
    case 8:
      printf("Enter new filename: ");
      fgets(settings->outputfilename, 256, stdin);
      /* Remove the trailing return */
      settings->outputfilename[strlen(settings->outputfilename) - 1] = '\0';
      break;
    case 9:
      done = TRUE;
      break;
    default:
      printf("Input not understood.\n");
      break;
    }
  }
}

/* Print the header for the output file */
void print_header(constraints *settings)
{
  settings->outputfile = fopen(settings->outputfilename, "w");
  if (settings->outputfile == (FILE *)NULL)
  {
    printf("Unable to open file; printing to standard output.\n");
    settings->outputfile = stdout;
  }
  fprintf(settings->outputfile,
          "Output settings: Reference point (%4.3f, %4.3f, %4.3f)\n",
          settings->ref_x, settings->ref_y, settings->ref_z);
  fprintf(settings->outputfile, 
          "Distance range %4.3f-%4.3f pc; Luminosity range %4.3f-%4.3f\n",
          settings->mindist, settings->maxdist, settings->minlum,
          settings->maxlum);
  fprintf(settings->outputfile, "Binaries are%s allowed.\n", 
          (settings->allowbinaries ? "" : " not"));
  if (settings->outputstyle == OUTPUTSTYLE_SHORT)
    fprintf(settings->outputfile,
            "GlieseID   Spectrum     DistPC    RefDistPC   X        Y       Z       Lum      Remarks\n");
}

/* Read options from the command line */
void read_options(int argc, char *argv[], constraints *settings)
{
  double tmp1, tmp2, tmp3;

  while (--argc > 0)
  {
    argv++;
    if (!strcmp(*argv, "-help"))
    {
      fprintf(stderr, "Command line options are:\n");
      fprintf(stderr, "  -n: Run noninteractively\n");
      fprintf(stderr, "  -long: Print long output\n");
      fprintf(stderr, "  -xlong: Print extra-long output\n");
      fprintf(stderr, "  -nobin: Do not print binary/trinary/etc. stars\n");
      fprintf(stderr,
              "  -o FILE: Use FILE as output file (default 'gliese3.out)\n");
      fprintf(stderr, "  -mindist X: Set minimum distance to X (default 0)\n");
      fprintf(stderr, "  -maxdist X: Set maximum distance to X (default 5)\n");
      fprintf(stderr,
              "  -minlum X: Set minimum luminosity to X (default 0)\n");
      fprintf(stderr,
              "  -maxlum X: Set maximum luminosity to X (default 200)\n");
      fprintf(stderr,
             "  -ref X Y Z: Set reference point to (X Y Z) (default 0 0 0)\n");
      fprintf(stderr, "  -help: Print this message\n");
      exit(0);
    }
    else if (!strcmp(*argv, "-n"))
    {
      settings->interactive = FALSE;
    }
    else if (!strcmp(*argv, "-long"))
    {
      settings->outputstyle = OUTPUTSTYLE_LONG;
    }
    else if (!strcmp(*argv, "-xlong"))
    {
      settings->outputstyle = OUTPUTSTYLE_XLONG;
    }
    else if (!strcmp(*argv, "-nobin"))
    {
      settings->allowbinaries = FALSE;
    }
    else if (!strcmp(*argv, "-o"))
    {
      argc--;
      if (*++argv)
        strcpy(settings->outputfilename, *argv);
      else
        fprintf(stderr,
                "Not enough parameters to \"-o\" flag. Ignoring it.\n");
    }
    else if (!strcmp(*argv, "-mindist"))
    {
      argc--;
      if (*++argv)
      {
        if (sscanf(*argv, "%lf", &tmp1) != 1)
          fprintf(stderr, 
                  "Invalid parameter to \"-mindist\" flag.  Ignoring it.\n");
        else
        {
          settings->mindist = tmp1;
          if (settings->maxdist < settings->mindist)
            settings->maxdist = settings->mindist;
        }
      }
      else
        fprintf(stderr,
                "Not enough parameters to \"-mindist\" flag. Ignoring it.\n");
    }
    else if (!strcmp(*argv, "-maxdist"))
    {
      argc--;
      if (*++argv)
      {
        if (sscanf(*argv, "%lf", &tmp1) != 1)
          fprintf(stderr, 
                  "Invalid parameter to \"-maxdist\" flag.  Ignoring it.\n");
        else
        {
          settings->maxdist = tmp1;
          if (settings->maxdist < settings->mindist)
            settings->mindist = settings->maxdist;
        }
      }
      else
        fprintf(stderr,
                "Not enough parameters to \"-maxdist\" flag. Ignoring it.\n");
    }
    else if (!strcmp(*argv, "-minlum"))
    {
      argc--;
      if (*++argv)
      {
        if (sscanf(*argv, "%lf", &tmp1) != 1)
          fprintf(stderr, 
                  "Invalid parameter to \"-minlum\" flag.  Ignoring it.\n");
        else
        {
          settings->minlum = tmp1;
          if (settings->maxlum < settings->minlum)
            settings->maxlum = settings->minlum;
        }
      }
      else
        fprintf(stderr,
                "Not enough parameters to \"-minlum\" flag. Ignoring it.\n");
    }
    else if (!strcmp(*argv, "-maxlum"))
    {
      argc--;
      if (*++argv)
      {
        if (sscanf(*argv, "%lf", &tmp1) != 1)
          fprintf(stderr, 
                  "Invalid parameter to \"-maxlum\" flag.  Ignoring it.\n");
        else
        {
          settings->maxlum = tmp1;
          if (settings->maxlum < settings->minlum)
            settings->minlum = settings->maxlum;
        }
      }
      else
        fprintf(stderr,
                "Not enough parameters to \"-maxlum\" flag. Ignoring it.\n");
    }
    else if (!strcmp(*argv, "-ref"))
    {
      argc -= 3;
      if (*++argv)
      {
        if (sscanf(*argv, "%lf", &tmp1) != 1)
        {
          fprintf(stderr, 
                  "Invalid parameter to \"-ref\" flag.  Ignoring it.\n");
          argv += 2; /* Skip the next two items on the command line */
        }
        else
        {
          if (*++argv)
          {
            if (sscanf(*argv, "%lf", &tmp2) != 1)
            {
              fprintf(stderr, 
                      "Invalid parameter to \"-ref\" flag.  Ignoring it.\n");
              argv++;
            }
            else
            {
              if (*++argv)
              {
                if (sscanf(*argv, "%lf", &tmp3) != 1)
                {
                  fprintf(stderr, 
                        "Invalid parameter to \"-ref\" flag.  Ignoring it.\n");
                }
                else
                {
                  settings->ref_x = tmp1;
                  settings->ref_y = tmp2;
                  settings->ref_z = tmp3;
                }
              }
              else
                fprintf(stderr,
                   "Not enough parameters to \"-ref\" flag. Ignoring it.\n");
            }
          }
          else
            fprintf(stderr,
                    "Not enough parameters to \"-ref\" flag. Ignoring it.\n");
        }
      }
      else
        fprintf(stderr,
                "Not enough parameters to \"-ref\" flag. Ignoring it.\n");
    }
  }
}
     

int main(int argc, char *argv[])
{
  stardata star;
  constraints settings;
  FILE *datafile;
  char instring[258];
  short a; /* added 26mar96 by winchell chung */
  
  init_settings(&settings);
  if (argc > 1)
    read_options(argc, argv, &settings);
  if (settings.interactive)
    get_user_settings(&settings);

  print_header(&settings);

  a = 3805; /* added 26mar96 by winchell chung */
  if (datafile = fopen("gliese3.dat", "r")) {
  }
  else {
      printf("file gliese3.dat not found\n");
      return 1;
  }
  while (fgets(instring, 258, datafile))
  {
    convert_record(instring, &star, &settings);
	print_record(&star, &settings);
	  printf("[%d] %8s%2s %s\n",a, 
			  star.identifier, star.components, star.remarks);  /* added 26mar96 by winchell chung */
	a--;  /* added 26mar96 by winchell chung */
  }
}
