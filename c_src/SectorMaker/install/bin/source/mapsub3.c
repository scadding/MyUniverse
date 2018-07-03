/*************************************************************************
 *
 * NAME
 *	mapsub - Traveller Subsector Mapper
 *
 * SYNOPSIS
 *	mapsub [ -a | -p ] < Subsector.dat
 *
 * DESCRIPTION
 *	This program takes Traveller subsector world data and converts it
 *	into a subsector map, either ascii or postscript.  Each subsector
 *	hex has the following data in it:
 *
 *	          ______
 *	         / XXYY \	<- Hex number
 *	        /  * S T \	<- *=Naval base, S=Starport class, T=Tech level
 *	       /  ^ @ .   \	<- ^=Scout base, @=Liquid world, O=Desert
 *	       \  System  /	   world, %=Asteroids, .=Gas giant presence
 *	        \  Name  /	<- System Name
 *	         \______/	<- CAPITOL, Capitol, RED, or amber travel zones
 *
 * OPTIONS
 *	-a	Produce Ascii output, like the above.  This is the default.
 *	-p	Produce Postscript output.  Looks much more like the real
 *		format published in the Spinward Marches map.
 *
 * FILES
 *	This program depends on two files being available:
 *
 *	./mapsub.hex	- contains an ascii hex grid, which is filled in
 *			  with world data by mapsub.
 *			  This file's pathname may be easily changed by
 *			  changing the MAP_TEMPLATE #define.
 *	./mapsub.pro	- contains postscript function definitions which
 *			  are needed to plot the worlds efficiently.
 *			  This file's pathname may be easily changed by
 *			  changing the PS_PROLOGUE #define.
 *	Subsector.dat	- the input file (actually this program eats whatever
 *			  it is given on its stdin).  The format can be in
 *			  one of two formats; version 1 format or version 2
 *			  format:
 *
 *	VERSION 1 format: The first line of the file is "#Version: 1".
 *	Each following line of input is 59 characters long, as follows:
 *
 *	"XXYY System_Name_______ CSAHPGL-T B Trade_Codes____ Al Z GP"
 *
 *	Version 1 is still around, so that data generated with the old
 *	version of gensec can still be used.
 *
 *	VERSION 2 format: The first line of the file is "#Version: 2".
 *	Each following line of the file is 60 characters long, as follows:
 *
 *	"System_Name_______ XXYY CSAHPGL-T B Trade_Codes____ MPG Al Z"
 *
 *	Version 2 supports most of the MegaTraveller changes, except it
 *	does not have the star information.  It is the current version of
 *	the gensec program's output.
 *
 *	Fortunately, if there are a few additional columns of data added to
 *	the right, this program can tolerate the change.
 *
 * BUGS
 *	Ascii output looks passible, but not terrific.
 *	Postscript output looks terrific.
 *
 * AUTHOR
 *	James T. Perkins, jamesp@dadla.la.tek.com
 *
 * HISTORY
 *	Based on an earlier program by James Perkins.
 *	Fred Schiff (vu0141@bingvaxu.cc.binghamton.edu) added MegaTraveller
 *	capability to the original mapsub.  Yngve Larsson (yla@ida.liu.se)
 *	added postscript output to the original, and authored the mapsub.pro
 *	file.  James Perkins then rototilled the original program and made
 *	it much more modifiable, incorporating the changes by Fred and Yngve.
 *      The 891009 release added better support of way stations, naval depots
 *	and capitols.
 *
 *	When	Who			What
 *	------	-----------------------	------------------------------------
 *	890417	jamesp@dadla.la.tek.com	Initial Release
 *	891009	jamesp@dadla.wr.tek.com Second Release
 *
 * VERSION TRACKING
 *
 * $Header: /m/dadla/usr1/jamesp/usr/src/games/traveller/RCS/mapsub.c,v 1.7 89/10/09 12:17:10 jamesp Exp $
 * $Locker:  $
 *
 *************************************************************************/
/*

Copyright 1989 James T. Perkins

	This notice and any statement of authorship must be reproduced
	on all copies.  The author does not make any warranty expressed
	or implied, or assumes any liability or responsiblity for the
	use of this software.

	Any distributor of copies of this software shall grant the
	recipient permission for further redistribution as permitted
	by this notice.	 Any distributor must distribute this software
	without any fee or other monetary gains, unless expressed written
	permission is granted by the author.

	This software or its use shall not be: sold, rented, leased,
	traded, or otherwise marketed without the expressed written
	permission of the author.

	If the software is modified in a manner creating derivative
	copyrights, appropriate legends may be placed on derivative
	work in addition to that set forth above.

	Permission is hereby granted to copy, reproduce, redistribute or
	otherwise use this software as long as the conditions above
	are met.

	All rights not granted by this notice are reserved.

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/************************************/
/*                                  */
/*  PROGRAM IDENTIFICATION          */
/*                                  */
/************************************/

/*
 * RCS id and Copyright.
 */

#ifdef Lint
static char *rcsid = "$Header: /m/dadla/usr1/jamesp/usr/src/games/traveller/RCS/mapsub.c,v 1.7 89/10/09 12:17:10 jamesp Exp $";
static char *copyright = "Copyright 1989 James T. Perkins";
#endif

/************************************/
/*                                  */
/*  DEFINES                         */
/*                                  */
/************************************/

#ifdef BSD
#define index strchr
#define rindex strrchr
#endif

/*
 * Modes of program operation
 */

#define M_ASCII		0
#define M_POSTSCRIPT	1
#define NUM_MODES	2

/*
 * Maximum Length of an input or output line
 */

#define LINE_SIZE	81

/*
 * MAP_TEMPLATE is the pathname of the empty hex-grid program.
 * PS_PROLOGUE is the pathname of the postscript prologue file.
 */

#ifndef MAP_TEMPLATE
#define MAP_TEMPLATE	"./mapsub.hex"
#endif MAP_TEMPLATE
#ifndef PS_PROLOGUE
#define PS_PROLOGUE	"./mapsub.pro"
#endif PS_PROLOGUE

/*
 * VERSION is the string which identifies a line containing the file
 * format version number.  This permits this program to parse and
 * recognaize different input formats.
 */

#define VERSION_STRING	"#Version: "
#define DEFAULT_VERSION 1

/************************************/
/*                                  */
/*  STRUCTS                         */
/*                                  */
/************************************/

/*
 * Entry points for different modes
 */

struct output_mode {
    char flag;		/* command-line option flag for this mode */
    void (*init)();	/* ptr to function to initialize output */
    void (*map)();	/* ptr to function to map 1 system's data */
    void (*finish)();	/* ptr to function to finish output */
};

/*
 * What composes a system...
 */

struct sys {
    char name[19];		/* System name */
    int x, y;			/* Hex location */
    char uwp[10];		/* CSAHPGL-T */
    char base;			/* S, N, A, B, or M for bases */
    char trade[16];		/* Up to 5 trade codes, space seperated */
    int pmul;			/* pop multiplier */
    int belt;			/* number of planetoid belts */
    int gas;			/* number of gas giants */
    char align[3];		/* system alignment */
    char zone;			/* travel zone: ' ', 'A', 'R' */
};

/************************************/
/*                                  */
/*  GLOBAL DATA                     */
/*                                  */
/************************************/

/*
 * Usage message
 */

char *usage = "usage: %s [ -ascii | -postscript ]\n";

/*
 * Forward declarations
 */

void bzero();
struct sys *line2system();
void ascii_init(), ascii_map(), ascii_finish();
void ps_init(), ps_map(), ps_finish();
void rem_trail_sp();
char *ascii_center_name();
void ascii_overwrite();

/*
 * The actual modes of output we support -
 * 	a - ascii mode
 *	p - postscript mode
 */

struct output_mode output[NUM_MODES] = {
    { 'a',	ascii_init,	ascii_map,	ascii_finish	},
    { 'p',	ps_init,	ps_map,		ps_finish	}
};

/*
 * program name
 */

char *progname;

/************************************/
/*                                  */
/*  FUNCTIONS                       */
/*                                  */
/************************************/


/*************************************************************************
 *
 * NAME
 *	main - mainline for mapsub
 *
 * PARAMETERS
 *	ac - argument count
 *	av - array of pointers to strings containing the arguments
 *
 * DESCRIPTION
 *	Parses arguments to determine what output mode we're to be in,
 *	then initializes that output module, reads the data from stdin
 *	and maps it into the output, and finishes the output.
 *
 * RETURN VALUE
 *	int - but value is ignored
 *
 * CAVEATS
 *	None.
 *
 *************************************************************************/

main(ac, av)
int ac;
char **av;
{
    int mode;			/* indicates ASCII or POSTSCRIPT mode */
    char line[LINE_SIZE];	/* stores a line of system input */
    struct sys *system;		/* stores a system, broken down from line */

    progname = *av;
    mode = get_mode(ac, av);

    /*
     * Initialize the output mode
     */

    output[mode].init();

    /*
     * Get each line of output, stuff it into a system structure, and map it.
     */

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if ((system = line2system(line)) != NULL) {
            output[mode].map(system);
        }
    }

    /*
     * Finish up output processing
     */

    output[mode].finish();
    exit(0);
}


/*************************************************************************
 *
 * NAME
 *	get_mode - parses arguments, returns the output mode
 *
 * PARAMETERS
 *	ac - argument count
 *	av - array of pointers to strings containing the arguments
 *
 * DESCRIPTION
 *	Parses the arguments and returns the output mode of the program.
 *	If no arguments are given, defaults to ascii mode.  If a bad
 *	argument is given, prints an error message and exits.
 *
 * RETURN VALUE
 *	int - M_ASCII for ascii output, M_POSTSCRIPT for postscript output,
 *	etc.
 *
 * CAVEATS
 *	Uses the output global.
 *
 *************************************************************************/

int
get_mode(ac, av)
int ac;
char **av;
{
    int mode;

    mode = M_ASCII;

    if (ac > 1) {
        mode = -1;
        if (av[1][0] == '-') {
            for (mode = NUM_MODES - 1; mode >= 0; mode--) {
                if (av[1][1] == output[mode].flag) {
                    break;
                }
            }
        }
    }

    if (mode == -1) {
        fprintf(stderr, usage, progname);
        exit(1);
    }

    return (mode);
}


/*************************************************************************
 *
 * NAME
 *	line2system - parses an input line and returns a system structure
 *
 * PARAMETERS
 *	line - one line of uwp system data from the stdin.  Or, a
 *		comment line, empty line, or version line.
 *
 * DESCRIPTION
 *	Examines the given line, parses it into a system structure, and
 *	returns a pointer to the structure.  If the input is empty or
 *	a comment line, it ignores it.  If the input is a version line,
 *	this function remembers to parse future input consistent with that
 *	format.
 *
 * RETURN VALUE
 *	struct sys * - pointer to parsed system structure corresponding to
 *	the line of input, or NULL of the line was a comment or version
 *	number.
 *
 * CAVEATS
 *	Pointer returned is to static data.
 *	Remembers the input version between invocations.
 *
 *************************************************************************/

struct sys *
line2system(line)
char *line;
{
    static struct sys s;
    static int version = DEFAULT_VERSION;

    if (line[0] == '#' || line[0] == '\0' || line[0] == '\n') {
        /*
         * This is a comment line of some kind.
         */

        if (strncmp(line, VERSION_STRING, strlen(VERSION_STRING)) == 0) {
            /*
             * Get file format version number
             */

            sscanf(line, "%*s %d", &version);
        }
        return NULL;
    }

    /*
     * Zero out the system structure
     */

    bzero((char *)&s, sizeof(s));

    /*
     * Parse the line into the system struct
     */

    if (version == 1) {
        char gas = ' ', belt = ' ';

        (void)sscanf(line,
                     "%2d%2d%*c%18c%*c%9c%*c%c%*c%15c%*c%2c%*c%c%*c%1c%1c",
                     &s.x, &s.y, s.name, s.uwp, &s.base, s.trade,
                     s.align, &s.zone, &gas, &belt);
        s.gas = ((gas == 'G') ? 1 : 0);
        s.belt = ((belt == 'P') ? 1 : 0);
    } else if (version > 1) {
        (void)sscanf(line,
                     "%18c%*c%2d%2d%*c%9c%*c%c%*c%15c%*c%1d%1d%1d%*c%2c%*c%c",
                     s.name, &s.x, &s.y, s.uwp, &s.base, s.trade,
                     &s.pmul, &s.belt, &s.gas, s.align, &s.zone);
    } else {
        fprintf(stderr, "%s: Unknown input format version number %d.\n",
                progname, version);
        exit(1);
    }

    /*
     * Null-terminate...
     */

    s.name[18] = '\0';
    s.uwp[9] = '\0';
    s.trade[15] = '\0';

    rem_trail_sp(s.name);
    rem_trail_sp(s.uwp);
    rem_trail_sp(s.trade);

    return &s;
}


/*************************************************************************
 *
 * NAME
 *	rem_trail_sp - remove trailing spaces from a string
 *
 * PARAMETERS
 *	s - the string to change
 *
 * DESCRIPTION
 *	eats off trailing spaces from the given string, turning them into
 *	NULLs.
 *
 * RETURN VALUE
 *	void
 *
 * CAVEATS
 *	Changes string pointed to by its argument.
 *
 *************************************************************************/

void
rem_trail_sp(s)
char *s;
{
    int i = strlen(s) - 1;

    while (i >= 0 && s[i] == ' ') {
        s[i--] = '\0';
    }
}


/*************************************************************************
 *
 * NAME
 *	bzero - zero a block of memory
 *
 * PARAMETERS
 *	s - pointer to beginning of memory area
 *	n - number of bytes to zero
 *
 * DESCRIPTION
 *	clears the given area of memory.
 *
 * RETURN VALUE
 *	void
 *
 * CAVEATS
 *	Destructive to data pointed to by the first argument
 *
 *************************************************************************/

/* #ifndef BSD
void
bzero(s, n)
char *s;
int n;
{
    while (n-- > 0) {
        *s++ = '\0';
    }
}
#endif
*/


/*************************************************************************
 *
 * NAME
 *	capitalize - capitalize a string
 *
 * PARAMETERS
 *	s - the string to capitalize
 *
 * DESCRIPTION
 *	converts all lower-case letters in the given string to upper-case
 *	letters.
 *
 * RETURN VALUE
 *	void
 *
 * CAVEATS
 *	Changes the data pointed to by it's input argument.
 *
 *************************************************************************/

void
capitalize(s)
char *s;
{
    for (; *s; s++) {
        if (islower(*s)) {
            *s = toupper(*s);
        }
    }
}

/************************************/
/*                                  */
/*  ASCII FUNCTIONS                 */
/*                                  */
/************************************/

char ascii_output[70][LINE_SIZE];	/* an image of the output lines */
int ascii_n_lines = 0;			/* number of lines in ascii_output */


/*************************************************************************
 *
 * NAME
 *	ascii_init - prepare for ascii mapping
 *
 * PARAMETERS
 *	None.
 *
 * DESCRIPTION
 *	Reads the map template into memory, and remembers how many lines
 *	were read in.
 *
 * RETURN VALUE
 *	void
 *
 * CAVEATS
 *	Destroys the static data space of ascii_output and ascii_n_lines.
 *
 *************************************************************************/

void
ascii_init()
{
    FILE *template;
    char *map;

    ascii_n_lines = 0;

    if ((template = fopen(MAP_TEMPLATE, "r")) == NULL) {
        perror(MAP_TEMPLATE);
        exit(1);
    }

    for (map = &ascii_output[0][0]; fgets(map, LINE_SIZE, template) != NULL;
         map += LINE_SIZE) {
        ascii_n_lines++;
    }

    (void)fclose(template);
}

#define ascii_xy(x, y, dx, dy) \
    &ascii_output[(((y) - 1) % 10) * 6 + \
    ((2 * ((x) / 2) == (x)) ? 4 : 1) + (dy)]\
    [(((x) - 1) % 8) * 9 + 1 + (dx)]


/*************************************************************************
 *
 * NAME
 *	ascii_map - map a star system onto the ascii output
 *
 * PARAMETERS
 *	s - system to map
 *
 * DESCRIPTION
 *	Places the system onto the ascii_output region of memory.
 *	This is a terrible kludge.  Depends on the icky ascii_xy macro
 *	which returns memory locations to overwrite.
 *
 * RETURN VALUE
 *	void
 *
 * CAVEATS
 *	Overwrites static data space (ascii_output)
 *
 *************************************************************************/


void
ascii_map(s)
struct sys *s;
{
    char t[80], *u;

    /*
     * Plot each item onto the output image; write hex:
     *      ______
     *    &/ XXYY \        & is where the x, y offset is
     *    /  * S T \
     *   /  ^ @ .   \
     *   \  System  /
     *    \  Name  /
     *     \______/		<- CAPITOL, Capitol, RED, or Amber go here
     */

    (void)sprintf(t, "%02d%02d", s->x, s->y);
    ascii_overwrite(ascii_xy(s->x, s->y, 3, 0), t, 4);
    ascii_overwrite(ascii_xy(s->x, s->y, 3, 1),
                    ((s->base == 'N' || s->base == 'A' || s->base == 'B') ? "*"
                     : ((s->base == 'D') ? "#" : " ")), 1);
    ascii_overwrite(ascii_xy(s->x, s->y, 5, 1), s->uwp, 1);
    ascii_overwrite(ascii_xy(s->x, s->y, 7, 1), &s->uwp[8], 1);
    ascii_overwrite(ascii_xy(s->x, s->y, 2, 2),
                    ((s->base == 'S' || s->base == 'A') ? "^"
                     : ((s->base == 'W' || s->base == 'B') ? "|" : " ")), 1);
    ascii_overwrite(ascii_xy(s->x, s->y, 4, 2),
                    ((s->uwp[1] == '0') ? "%" : ((s->uwp[3] == '0') ? "O" : "@")), 1);
    ascii_overwrite(ascii_xy(s->x, s->y, 6, 2),
                    ((s->gas > 0) ? "." : " "), 1);

    u = s->trade;
    if (u = strchr(u, 'H')) {
        if (u[1] == 'i' || u[1] == 'I') {
            capitalize(s->name);
        }
    } else if (s->uwp[4] == '9' || s->uwp[4] == 'A') {
        capitalize(s->name);
    }

    u = ascii_center_name(s->name, s->x, s->y, 0, 3, 10);
    if (u) {
        (void)ascii_center_name(u + 1, s->x, s->y, 1, 4, 8);
    }

    switch (s->zone) {
    case 'A': /* Amber */
        ascii_overwrite(ascii_xy(s->x, s->y, 2, 5), "amber", 5);
        break;
    case 'R': /* Red */
        ascii_overwrite(ascii_xy(s->x, s->y, 2, 5), "RED", 3);
        break;
    }

    u = s->trade;
    while (u = strchr(u, 'C')) {
        if (u[1] == 'a' || u[1] == 'A') {
            ascii_overwrite(ascii_xy(s->x, s->y, 2, 5), "Capitol", 7);
            break;
        } else if (u[1] == 'x' || u[1] == 'X') {
            ascii_overwrite(ascii_xy(s->x, s->y, 2, 5), "CAPITOL", 7);
            break;
        } else {
            u++;
        }
    }
}


/*************************************************************************
 *
 * NAME
 *	ascii_finish - write out the ascii output to stdout
 *
 * PARAMETERS
 *	none.
 *
 * DESCRIPTION
 *	writes the ascii_output array to stdout.
 *
 * RETURN VALUE
 *	void
 *
 * CAVEATS
 *	None.
 *
 *************************************************************************/

void
ascii_finish()
{
    int i;

    /*
     * Write out finished map
     */

    for (i = 0; i < ascii_n_lines; i++) {
        printf("%s", ascii_output[i]);
    }
}


/*************************************************************************
 *
 * NAME
 *	ascii_overwrite - overwrite memory with new data
 *
 * PARAMETERS
 *	p - address to begin overwriting at
 *	s - address of string of data to overwrite with
 *	n - number of bytes to overwrite
 *
 * DESCRIPTION
 *	Overwrites the section of memory at p with the first n characters
 *	of string s.
 *
 * RETURN VALUE
 *	void
 *
 * CAVEATS
 *	Destroys memory pointed to by one of it's arguments.
 *
 *************************************************************************/

void
ascii_overwrite(p, s, n)
char *p, *s;
int n;
{
    while (n-- > 0) {
        *p++ = *s++;
    }
}


/*************************************************************************
 *
 * NAME
 *	ascii_center_name - place part or all of a name into ascii_output
 *
 * PARAMETERS
 *	name - name to place
 *	x, y - hex x,y location in the subsector
 *	dx, dy - screen offsets from origin of this hex's xy coords
 *	width - width of this line of output.
 *
 * DESCRIPTION
 *	Take the given name and try to place as many words as will fit in given
 *	width into the map at x, y, dx, dy.  Also center each line of output in
 *	the width.  Unfortunately, a word may be too long to fit, in which case
 *	place it anyway, overlapping.
 *
 * RETURN VALUE
 *	char * - Returns 0 if it was able to place the entire name, or a pointer
 *	to the space just following the last word placed.
 *
 * CAVEATS
 *	Nasty kludges, beware.
 *
 *************************************************************************/

char *
ascii_center_name(name, x, y, dx, dy, width)
char *name;
int x, y, dx, dy, width;
{
    char *end, *cur, *oldcur;
    int offset;

    cur = strchr(name, ' ');
    if (cur == NULL) {
        end = name + strlen(name) - 1;
    } else {
        end = cur - 1;
        oldcur = NULL;
        while (cur && cur - name <= width && cur != oldcur) {
            end = cur - 1;
            oldcur = cur;
            cur = strchr(cur + 1, ' ');
            if (cur == NULL) {
                cur = name + strlen(name);
            }
        }
    }
    offset = (width - (end - name + 1)) / 2;
    dx = dx + offset;
    ascii_overwrite(ascii_xy(x, y, dx, dy), name, end - name + 1);

    if (strchr(end, ' ')) {
        return end + 1;
    } else {
        return NULL;
    }
}

/************************************/
/*                                  */
/*  POSTSCRIPT FUNCTIONS            */
/*                                  */
/************************************/


/*************************************************************************
 *
 * NAME
 *	ps_init - prepare to map postscript output
 *
 * PARAMETERS
 *	none.
 *
 * DESCRIPTION
 *	Copies the prologue file to the stdout, in preparation for world
 *	mapping.  This file sets up functions used later and prints
 *	the underlying hex grid.
 *
 * RETURN VALUE
 *	void
 *
 * CAVEATS
 *	None.
 *
 *************************************************************************/

void
ps_init()
{
    FILE *fp;
    char s[LINE_SIZE];

    if ((fp = fopen(PS_PROLOGUE, "r")) == NULL) {
        perror(PS_PROLOGUE);
        exit(1);
    }

    while (fgets(s, sizeof(s), fp) != NULL) {
        (void)fputs(s, stdout);
    }

    (void)fclose(fp);
}


/*************************************************************************
 *
 * NAME
 *	ps_map - map worlds into postscript output
 *
 * PARAMETERS
 *	s - system to put out
 *
 * DESCRIPTION
 *	Emits the appropriate postscript commands to render the system
 *	in its hex.
 *
 * RETURN VALUE
 *	void
 *
 * CAVEATS
 *	None.
 *
 *************************************************************************/

void
ps_map(s)
struct sys *s;
{
    char *u;

    printf("%2d %2d mapCoords \n", s->x, s->y);

    switch (s->zone) {
    case 'A':
        printf("amberZone \n");
        break;
    case 'R':
        printf("redZone \n");
        break;
    }

    printf("(%.2d%.2d) hexNumber \n", s->x, s->y);

    if (s->uwp[1] == '0') {
        printf("asteroids \n");
    } else if (s->uwp[3] == '0') {
        printf("desPlnt \n");
    } else {
        printf("liqPlnt \n");
    }

    u = s->trade;
    if (u = strchr(u, 'H')) {
        if (u[1] == 'i' || u[1] == 'I') {
            capitalize(s->name);
        }
    } else if (s->uwp[4] == '9' || s->uwp[4] == 'A') {
        capitalize(s->name);
    }

    u = s->trade;
    if (u = strchr(u, 'C')) {
        if (u[1] == 'a' || u[1] == 'A' || u[1] == 'x' || u[1] == 'X') {
            printf("(%s) nameCapitol \n", s->name);
        } else {
            printf("(%s) name \n", s->name);
        }
    } else {
        printf("(%s) name \n", s->name);
    }

    printf("(%c) starPort \n", s->uwp[0]);

    if (s->base == 'N' || s->base == 'A' || s->base == 'B') {
        printf("navalBase \n");
    }
    if (s->base == 'M') {
        printf("militaryBase \n");
    }
    if (s->base == 'D') {
        printf("navalDepot \n");
    }
    if (s->base == 'S' || s->base == 'A') {
        printf("scoutBase \n");
    }
    if (s->base == 'W' || s->base == 'B') {
        printf("scoutWayStation \n");
    }

    if (s->gas > 0) {
        printf("gasGiant \n");
    }

    printf("\n");
}


/*************************************************************************
 *
 * NAME
 *	ps_finish - complete postscript output
 *
 * PARAMETERS
 *	none.
 *
 * DESCRIPTION
 *	emits the final postscript command which causes the page to be
 *	printed.
 *
 * RETURN VALUE
 *	void
 *
 * CAVEATS
 *	None.
 *
 *************************************************************************/

void
ps_finish()
{
    printf("showpage \n");
}
