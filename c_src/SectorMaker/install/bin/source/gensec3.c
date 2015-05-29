/*

    gensec - a simplistic traveller sector generator

    Simplistic Traveller sector generator, based on Marc Miller,
    "Traveller Sector Generator", Using Your Model 2/BIS, Challenge
    No.  25; and other Traveller material (Referee's Manual and
    issues of Traveller's Digest.)

    The format of the output of gensec is a superset of the material
    produced by Mr.  Miller's published program.  It now conforms to the
    format in the MegaTraveller Referee's Manual p.  16, except the
    travel zone has been put last similar to entries in Traveller's
    Digest.

    This program rewritten C and Unix Aug 18 1987 by James T. Perkins.
    (jamesp@dadla.la.tek.com, @uunet.uu.net:jamesp@dadla.la.tek.com)

    Additions made Mar 17 1989 by Fred Schiff
    (vu0141@bingvaxu.cc.binghamton.edu) to conform with Basic generation
    in MegaTraveller Referee's Manual.

*/
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

/* System include files */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Defines for various flavors of unix */
extern long time();
#ifdef BSD
    extern long srandom(), random();
#   define srand(x) ((int)srandom(x))
#   define rand() ((int)random())
#else !BSD
    extern int srand(), rand();
#endif

/*
 * The VERSION_STRING is the first part of the line which identifies
 * what format the file's data are in.  This same string is used by the
 * mapsub program to interpret the data correctly.
 * The VERSION_NUMBER is the actual version number of the data format
 * produced by this program.  Versions were as follows:
 *
 * Version	Description
 *	1	Compatible with early gensec/mapsub program.  Traveller format.
 #	2	First MegaTraveller-compatible format.
 */

#define VERSION_STRING	"#Version: "
#define VERSION_NUMBER	3

/* Local macros */
#define D2 nd(2, 6)
#define D1 d(6)
#define DM(test, dm) ((test) ? (dm) : 0)
#define limit(x, l, u) (((x) < (l)) ? (l) : (((x) > (u)) ? (u) : (x)))

/* Boolean type declaration */
typedef int bool;

/* Variables for controlling generation procedure */
int maturity = 1;	/* determines how well travelled sector is */
int density = 50;	/* stellar density for system presence */

/* Forward declarations */
char *progname;
char hexchar();

/* Machine-readable copyright notice */
#ifndef lint
char *copyright = "Copyright 1989 James T. Perkins";
#endif

main(ac, av)
int ac;
char **av;
{
    int x, y;
    int x_start = 1, x_end = 32;
    int y_start = 1, y_end = 40;
    char *ali = "Im";	/* Star system alignment */

    (void)srand(time((long *)NULL));
    progname = av[0];
    av++; ac--;

    options(ac, av, &x_start, &x_end, &y_start, &y_end, &ali);

    fprintf(stdout, "%s%d\n", VERSION_STRING, VERSION_NUMBER);
    for (x = x_start; x <= x_end; x++)
    {
        for (y = y_start; y <= y_end; y++)
        {
            if (d(100) <= density)
            {
                gensys(x, y, ali, stdout);
            }
        }
    }
    exit(0); 
}

gensys(x, y, ali, fp)
int x, y;
char *ali;
FILE *fp;
{
    static char *nam = "Junk";
    static int giants[] = {1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5};
    static int belts[] = {1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3};
    char cla, bas, zon, tra[50], out[81];
    int siz, atm, hyd, pop, gov, law, tl, gas, pla, mul;
    bool sco, nav, dep, mil, way;


    /* Starport class */
    switch (maturity) {
	case 0:
	    cla = *("AABBCCCDEEX" + D2 - 2); /* Backwater */
	    break;
	case 1:
	    cla = *("AAABBCCDEEX" + D2 - 2); /* Standard */
	    break;
	case 2:
	    cla = *("AAABBCCDEEE" + D2 - 2); /* Mature */
	    break;
	case 3:
	    cla = *("AAAABBCCDEX" + D2 - 2); /* Cluster */
	    break;
	default:
	    (void)fprintf(stderr,
		"%s: interal error - maturity out of range (%d)\n",
		progname, maturity);
	   exit(1);
    }

    /* Physical characteristics */
    siz = D2 - 2;
    atm = ((siz == 0) ? 0 : (D2 - 7 + siz));
    atm = limit(atm, 0, 15);
    hyd = D2 - 7 + siz + DM(atm < 2 || atm > 9, -4);
    hyd = ((siz < 2) ? 0 : hyd);
    hyd = limit(hyd, 0, 10);

    /* Demographics */
    pop = D2 - 2;
    gov = D2 - 7 + pop;
    gov = limit(gov, 0, 15);
    law = D2 - 7 + gov;
    law = limit(law, 0, 20);

    /* Technological level */
    tl = D1 + DM(cla == 'A', 6) + DM(cla == 'B', 4) + DM(cla == 'C', 2) +
        DM(cla == 'X', -4);
    tl = tl + DM(siz < 5, 1) + DM(siz < 2, 1);
    tl = tl + DM(atm < 4, 1) + DM(atm > 9 && atm < 15, 1);
    tl = tl + DM(hyd == 8, 1) + DM(hyd == 9, 2);
    tl = tl + DM(pop > 0 && pop < 6, 1) + DM(pop == 9, 2) +
        DM(pop == 10, 4);
    tl = tl + DM(gov == 0 || gov == 5, 1) + DM(gov == 13, -2);
    tl = limit(tl, 0, 16);

    /* System characteristics */
    mul = d(5) + ((D1 > 3) ? -1 : 4);		/* population multiplier */
    pla = ((D2 < 8) ? 0 : belts[D2 - 2]);	/* planetoid belts */
    gas = ((D2 < 5) ? 0 : giants[D2 - 2]);	/* gas giants */

    /* Travel advisories 
    zon = ((cla == 'X') ? 'R' : ' ');
    if (gov > 9 && law > 15)
      zon = *("    A   AA  AAA AAAR AARRAARRR" + 5 * (gov-10) + law-15); */
	zon = ((cla == 'X') ? 'R' : ((D2 > 11) ? 'A' : ' '));

    /* Bases */
    nav = (cla < 'C' && D2 > 7);
    sco = (cla < 'E' && (D2 + DM(cla == 'A', -3) + DM(cla == 'B', -2) + DM(cla == 'C', -1)) > 6) ;
   /* mil = (cla < 'D' && (D2 + DM(cla == 'A', -2) + DM(cla == 'B', -1)) > 7); */
    mil = (cla < 'D' && (D2 + DM(pop > 8, -1) + DM((atm > 1 && atm < 6 && hyd < 4), -20)) > 11);
    dep = (cla < 'B' && gov > 9);
    way = (cla < 'B' && (hyd > 4));
    bas = (nav && sco ? 'A' : (nav && way ? 'B' : (way ? 'W' : (dep && nav ? 'D' : (nav ? 'N' : (sco ? 'S' : (mil ? 'M' : ' ')))))));

    /* Trade classifications */
    *tra = '\0';
    if (pop > 8)
        (void)strcat(tra, "Hi ");        /* High Population */
    if (pop < 4)
        (void)strcat(tra, "Lo ");        /* Low Population */
    if (pop == 0 && gov == 0 && law == 0)
        (void)strcat(tra, "Ba ");        /* Barren */
    if (atm > 3 && atm < 10 && hyd > 3 && hyd < 9 && pop > 4 && pop < 8)
        (void)strcat(tra, "Ag ");        /* Agricultural */
    if (atm < 4 && hyd < 4 && pop > 5)
        (void)strcat(tra, "Na ");        /* Non-Agricultural */
    if (((atm > 1 && atm < 5) || atm == 7 || atm == 9) && pop > 8)
        (void)strcat(tra, "In ");        /* Industrial */
    if (pop < 7)
        (void)strcat(tra, "Ni ");        /* Non-Industrial */
    if ((atm == 6 || atm == 8) && pop > 5 && pop < 9 && gov > 3 && gov < 10)
        (void)strcat(tra, "Ri ");        /* Rich */
    if (atm > 1 && atm < 6 && hyd < 4)
        (void)strcat(tra, "Po ");        /* Poor */
    if (hyd == 0 && atm > 1)
        (void)strcat(tra, "De ");        /* Desert World */
    if (hyd == 10)
        (void)strcat(tra, "Wa ");        /* Water World */
    if (siz == 0 && atm == 0 && hyd == 0)
        (void)strcat(tra, "As ");        /* Asteroid Belt */
    else if (atm == 0)
        (void)strcat(tra, "Va ");        /* Vaccuum World */
    if (siz > 9 && atm > 0)
        (void)strcat(tra, "Fl ");        /* Fluid */
    if (atm < 2 && hyd > 0)
        (void)strcat(tra, "Ic ");        /* Ice-Capped */

    /* Create world data string */
    (void)sprintf(out,
        "%-18s %02d%02d %c%c%c%c%c%c%c-%c %c %-15s %1d%1d%1d %2s %c\n",
        nam, x, y, cla,
        hexchar(siz), hexchar(atm), hexchar(hyd), hexchar(pop),
        hexchar(gov), hexchar(law), hexchar(tl),
        bas, tra, mul, pla, gas, ali, zon);

    /* Write it out to file (or stdout) */
    (void)fwrite(out, strlen(out), 1, fp);
}

char hexchar(i)
int i;
{
    if (i < 0 || i > 34)
        return '?';
    else
        return *("0123456789ABCDEFGHJKLMNPQRSTUVWXYZ" + i);
}

int d(nsides)
int nsides;
{
    return (int)(rand() % (long)nsides + 1L);
}

int nd(ndice, nsides)
int ndice, nsides;
{
    if (ndice < 1)
    {
        return 0;
    }
    else
    {
        return nd(ndice - 1, nsides) + d(nsides);
    }
}

options(ac, av, x_start, x_end, y_start, y_end, ali)
int ac;
char **av;
int *x_start, *x_end, *y_start, *y_end;
char **ali;
{
    int sub;
    static int xstarts[] = {1,9,17,25};
    static int ystarts[] = {1,11,21,31};

    for (; ac > 0; av++, ac--)
    {
        if ((**av == '-') && ((sub = (*(*av + 1))) >= 'A') && (sub <= 'P'))
        {
            sub = sub - 'A';
            *x_start = xstarts[sub % 4];
            *y_start = ystarts[((sub > 11) ? 3 :
                ((sub > 7) ? 2 : ((sub > 3) ? 1 : 0)))];
            *x_end = *x_start + 7;
            *y_end = *y_start + 9;
        }
        else if (isdigit(**av))
	{
            density = atoi(*av);
	}
        else if (strcmp(*av, "rift") == 0)
	{
	    density = 4;
	}
        else if (strcmp(*av, "sparse") == 0)
	{
	    density = 16;
	}
	else if (strcmp(*av, "scattered") == 0)
	{
	    density = 33;
	}
	else if (strcmp(*av, "dense") == 0)
	{
            density = 66;
	}
        else if (strcmp(*av, "backwater") == 0)
	{
	    maturity = 0;
	}
        else if (strcmp(*av, "frontier") == 0)  /* Standard - default */
	{
	    maturity = 1;
	}
        else if (strcmp(*av, "mature") == 0)
	{
	    maturity = 2;
	}
        else if (strcmp(*av, "cluster") == 0)
	{
	    maturity = 3;
	}
	else if (strlen(*av) == 2 && isalpha(**av))
	{
	    *ali = *av;
	}
        else
	{
	    usage();
	}
    }
}

usage()
{
    char *usemess = "\
  Generates Traveller sector or subsector.\n\
\n\
  S = {A..P}, subsector to generate\n\
  density = generation percentage or rift|sparse|scattered|dense\n\
  maturity = backwater|frontier|mature|cluster\n\
  ac = two-letter system alignment code (first character must be alphabetic)\n\
  defaults are:\n\
    entire sector, 50%% stellar density, standard frontier maturity,\n\
    Im (Imperium) alignment.\n";

    fprintf(stderr,
	"usage: %s {-S | density | maturity | ac }\n", progname);

    fprintf(stderr, usemess);
        exit(1);
}
