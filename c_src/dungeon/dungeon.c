/*
 * Program: dungeon
 *
 * written by Jeff Standish/Morpheus Nosferatu
 *	morpheus@sage.cc.purdue.edu
 *	standish@mentor.cc.purdue.edu
 *
 * Copyright 1993, Jeff Standish.  All Rights Reserved.
 * Permission given to copy, run, and modify this code so long as this header
 * remains intact and no profit is made from it (unless I get some).
 *
 * Send me mail at the above e-mail addresses if you find any bugs or make
 * any significant modifications.
 *
 */
#include <stdio.h>
#include "dungeon1.h"
#define MAX(a,b)	((a > b) ? (a) : (b))
#define MIN(a,b)	((a < b) ? (a) : (b))
#define EXITSTACK	0
#define ROOM		1
#define CHAMBER		2
#define PASSAGE		3
#define	JUNCTION	4
#define STAIRWAY	5
#define DOORWAY		10
#define ARCHWAY		11
#define OPENING		12
#define NORTH		20
#define SOUTH		21
#define EAST		22
#define WEST		23
#define UP		24
#define DOWN		25
#define PRINTLATEX	1
#define PRINTPS		2
struct queuestruct {
    struct exitstruct  *exit;
    struct queuestruct *queuenext;
};
struct exitstruct {
    int    orientation, x1, y1, x2, y2, exittype, locationtype, level;
    struct locationstruct *parent;
    struct exitstruct *next;
};
struct linestruct {
    int x1, y1, x2, y2;
    struct linestruct *next;
};
struct locationstruct {
    int    xmin, xmax, ymin, ymax, locationtype, locationdata, level;
    struct linestruct     *firstline;
    struct locationstruct *next, *previous;
};
typedef struct queuestruct QUEUE;
typedef struct exitstruct  EXITS;
typedef struct linestruct  LINES;
typedef struct locationstruct LOCAT;
#define LATEXHEADERLEN	10
static char *latexheader[LATEXHEADERLEN] = {
"\\documentstyle{report}",
"\\pagestyle{empty}",
"\\addtolength{\\textwidth}{3in}",
"\\addtolength{\\oddsidemargin}{-1.5in}",
"\\addtolength{\\topmargin}{-1.5in}",
"\\addtolength{\\textheight}{3in}",
"",
"\\begin{document}",
"\\begin{picture}(500,730)",
""
};
#define HELPLEN 15
static char *helpmesg[HELPLEN] = {
"    -m [level]\t - maximum level of the dungeon,",
"\t\t\t\t defaults to level = 1",
"    -help\t\t - this help message",
"    -f [name]\t - set the prefix name of output files,",
"\t\t\t\t defaults to name = \"map\"",
"    -d [direction] - select the direction that",
"\t\t\t\t the entrance to the first level is facing,",
"\t\t\t\t select from \"north\", \"south\", \"east\",",
"\t\t\t\t or \"west\", defaults to \"north\"",
"    -e[type] - select form of entrance to first level,",
"\t\t\t\t select from \"door\" or \"stair\"",
"\t\t\t\t defaults to \"stair\"",
"    -o[format]\t - set the map file format as either \"latex\"",
"\t\t\t\t or \"postscript\", defaluts to \"latex\"",
"    -s[x y]\t - set the width, height of the map (use ten's of feet)"
};
char	filename[100];
int	maxwidth = 500, maxheight = 700;
int	mapwidth = 200, mapheight = 280;
int	entrancetype = STAIRWAY, entrancedirection = NORTH;
int	startlevel = 1, maxlevel = 1;
int	gc = 0, printout = PRINTLATEX;
float   ratio = 2.5;
EXITS	*exitfreestack = NULL;
LOCAT	*firstlocation = NULL, *lastlocation = NULL;
QUEUE	*queuehead = NULL, *queuetail = NULL, *queuefreestack = NULL;
EXITS	*new_exit_node();
EXITS	*dequeue_exit();
LINES	*new_line_node();
LOCAT	*new_location_node();
LOCAT	*box_empty();
QUEUE	*new_queue_node();
/****************************************************************************
****************************************************************************/
int main(int argc, char **argv)
{
    int ac, i;

    strcpy(filename, "map");

    ac = 1;
    while (ac < argc) {
	if (!strcmp(argv[ac], "-e")) {
	    ++ac;
	    if (ac < argc) {
		if (!strcmp(argv[ac], "stair"))
		    entrancetype = STAIRWAY;
		else if (!strcmp(argv[ac], "door"))
		    entrancetype = DOORWAY;
		else
		    fatal_error("unknown entrance type \"%s\"", argv[ac]);
	    }
	    else
		fatal_error("entrance type not given in command line");
	}
	else if (!strcmp(argv[ac], "-d")) {
	    ++ac;
	    if (ac < argc) {
		if (!strcmp(argv[ac], "north"))
		    entrancedirection = NORTH;
		else if (!strcmp(argv[ac], "south"))
		    entrancedirection = SOUTH;
		else if (!strcmp(argv[ac], "east"))
		    entrancedirection = EAST;
		else if (!strcmp(argv[ac], "west"))
		    entrancedirection = WEST;
		else
		    fatal_error("invalid entrance direction \"%s\"", argv[ac]);
	    }
	    else
		fatal_error("entrance direction not given in command line");
	}
	else if (!strcmp(argv[ac], "-m")) {
	    ++ac;
	    if (ac < argc)
		maxlevel = atoi(argv[ac]);
	    else
		fatal_error("maximum level value not given");
	}
	else if (!strcmp(argv[ac], "-o")) {
	    ++ac;
	    if (ac < argc) {
		if (!strcmp(argv[ac], "latex"))
		    printout = PRINTLATEX;
		else if ((!strcmp(argv[ac], "postscript")) ||
                   (!strcmp(argv[ac],"ps")))
		    printout = PRINTPS;
		else
		    fatal_error("unknown output format \"%s\"", argv[ac]);
	    }
	    else
		fatal_error("output form not given");
	}
	else if (!strcmp(argv[ac], "-s")) {
	    ++ac;
	    if (ac < argc)
		mapwidth = atoi(argv[ac]);
	    else
		fatal_error("map width was not given");
	    ++ac;
	    if (ac < argc)
		mapheight = atoi(argv[ac]);
	    else
		fatal_error("map height was not given");
        }
	else if (!strcmp(argv[ac], "-f")) {
	    ++ac;
	    if (ac < argc) {
		strcpy(filename, argv[ac]);
	    }
	    else
		fatal_error("name of output files not given");
	}
	else if ((!strcmp(argv[ac], "-help")) || (!strcmp(argv[ac], "-h"))) {
	    printf("Usage: %s [argument list]\n", argv[0]);
	    for (i = 0; i < HELPLEN; ++i)
		printf("%s\n", helpmesg[i]);
	    exit(0);
	}
	else
	    fatal_error("unknown parameter \"%s\"", argv[ac]);
	++ac;
    }
    srand(time(0));
    printf("dimensions \"%d\" \"%d\"\n",mapwidth,mapheight); 
    if ((maxheight/mapheight) < (maxwidth/mapwidth)) {
        ratio = maxheight;
        ratio = (ratio/mapheight); }
    else  {
        ratio = maxwidth;
        ratio = (ratio/mapwidth);}
    printf("ratio: %f\n",ratio); 
    create_entrance();
    process_exit_queue();
    stock_dungeon();
    if (printout == PRINTPS)
	print_postscript_file();
    else
	print_latex_file();
    return 0;
}
/****************************************************************************
****************************************************************************/
int stock_dungeon()
{
    int   bottomlevel = 0;
    LOCAT *room;
    room = firstlocation;
    while (room != NULL) {
	bottomlevel = MAX(bottomlevel, room->level);
	room = room->next;
    }
    maxlevel = bottomlevel;
}
/****************************************************************************
****************************************************************************/
int create_entrance()
{
    int   xpos, ypos;
    LOCAT *room;
    EXITS *exit;
    room = new_location_node();
    add_location_to_queue(room);
    room->xmax = mapwidth;
    room->ymax = mapheight;
    room->level = startlevel;
    exit = new_exit_node();
    add_exit_to_queue(exit);
    exit->level = startlevel;
    exit->orientation = entrancedirection;
    exit->locationtype = ROOM;
    exit->parent = room;
    if (entrancetype == DOORWAY) {
            exit->exittype = DOORWAY;
	switch (entrancedirection) {
	    case NORTH:
	    case SOUTH:
		if (entrancedirection == SOUTH)
		    ypos = mapheight - 2;
		else
		    ypos = 2;
		exit->x1 = (mapwidth / 2) - 5;
		exit->x2 = (mapwidth / 2) + 5;
		exit->y1 = ypos;
		exit->y2 = ypos;

	add_line_to_location(room,exit->x1,exit->y1,exit->x1-10, exit->y1);
	add_line_to_location(room, exit->x2,exit->y2,
				exit->x2+10,exit->y2);
		break;
	    case EAST:
	    case WEST:
		if (entrancedirection == WEST)
		    xpos = mapwidth - 2;
		else
		    xpos = 2;
		exit->x1 = xpos;
		exit->x2 = xpos;
		exit->y1 = (mapheight / 2) - 5;
		exit->y2 = (mapheight / 2) + 5;
	add_line_to_location(room,exit->x1,exit->y1,exit->x1, exit->y1 - 10);
	add_line_to_location(room,exit->x2,exit->y2,exit->x2, exit->y2 + 10);
		break;
	    default:
		fatal_error("create_entrance(): unknown entrance direction");
	}
    }
    else if (entrancetype == STAIRWAY) {
	xpos = mapwidth / 2;
	ypos = mapheight / 2;
	exit->x1 = xpos;
	exit->y1 = ypos;
	exit->x2 = xpos;
	exit->y2 = ypos;
	exit->exittype = OPENING;
	switch (entrancedirection) {
	    case NORTH:
		exit->x2 -= 10;
		draw_stairs_up(room, SOUTH, 10, xpos, ypos);
		break;
	    case SOUTH:
		exit->x2 += 10;
		draw_stairs_up(room, NORTH, 10, xpos, ypos);
		break;
	    case EAST:
		exit->y2 += 10;
		draw_stairs_up(room, WEST, 10, xpos, ypos);
		break;
	    case WEST:
		exit->y2 -= 10;
		draw_stairs_up(room, EAST, 10, xpos, ypos);
		break;
	    default:
		fatal_error("create_entrance(): unknown entrance direction");
	}
    }
    else
	fatal_error("create_entrance(): unknown entrance type");
    compress_allocated_space(room);
}



/****************************************************************************
****************************************************************************/

int process_exit_queue()
{
    int   delta, width, xpos, ypos, flag;
    EXITS *exit;
    LOCAT *room;
    while (queuehead != NULL) {
	exit = dequeue_exit();
	if (exit == NULL)
	    continue;
	if (exit->parent == NULL)
	    fatal_error("process_exit_queue(): found exit with no parent");
	if (join_exits(exit)) {
	    free_exit_node(exit);
	    continue;
	}
	room = new_location_node();
	room->xmin = MIN(exit->x1, exit->x2);
	room->xmax = MAX(exit->x1, exit->x2);
	room->ymin = MIN(exit->y1, exit->y2);
	room->ymax = MAX(exit->y1, exit->y2);
	room->locationtype = exit->locationtype;
	room->level = exit->level;
	switch(exit->orientation) {
	   case NORTH: room->ymin += 1; room->ymax += 1; break;
	   case SOUTH: room->ymin -= 1; room->ymax -= 1; break;
	   case EAST:  room->xmin += 1; room->xmax += 1; break;
	   case WEST:  room->xmin -= 1; room->xmax -= 1; break;
	}
	find_exit_position(exit, &width, &xpos, &ypos);
	if (exit->locationtype == JUNCTION)
	    delta = expand_allocated_space(room, exit->orientation, width);
	else if (exit->locationtype == STAIRWAY)
	    delta = expand_allocated_space(room, exit->orientation, 20);
	else if (exit->locationtype == PASSAGE)
	    delta = expand_allocated_space(room, exit->orientation, 
				((rand() % 8) * 5) + 25);
	else
	    delta = expand_allocated_space(room, exit->orientation,
				((rand() % 10) * 5) + 10);
	if (delta < width) {
	    seal_exit(exit);
	    free_exit_node(exit);
	    continue;
	}
	flag = 0;
	switch (exit->locationtype) {
	    case ROOM:
	    case CHAMBER:
		flag = draw_room(room, exit);
		break;
	    case PASSAGE:
		flag = draw_passage(room, exit, delta);
		break;
	    case JUNCTION:
		flag = draw_junction(room, exit, delta, width);
		break;
	    case STAIRWAY:
		flag = draw_stairway(room, exit, delta, width, xpos, ypos);
		break;
	    default:
		fatal_error("process_exit_queue(): unknown location type");
	}
	if (flag) {
	    seal_exit(exit);
	    free_exit_node(exit);
	    continue;
	}

	draw_exit(exit);
		/* do not add room to queue until last so will not interfer
		   with checking of allocated space */
	compress_allocated_space(room);
	add_location_to_queue(room);
	free_exit_node(exit);
    }
}
/****************************************************************************
****************************************************************************/
int join_exits(exit)
EXITS *exit;
{
    int   mergeflag;
    int   sl, sr, nl, nr, et, eb, wt, wb;
    int   sflag, nflag, eflag, wflag;
    int   delta;
    EXITS *otherexit;
    LOCAT *newroom;
    QUEUE *que;
	/* do not mess with exits for stairways */
    if (exit->locationtype == STAIRWAY)
	return 0;
    mergeflag = 0;
    nflag = sflag = eflag = wflag = 0;
    que = queuehead;
    while (que != NULL) {
	if ((que->exit != NULL) && (que->exit->level == exit->level)
			&& (que->exit->locationtype != STAIRWAY)) {
	    otherexit = que->exit;
	    if (otherexit->parent == NULL)
		fatal_error("join_exits(): second exit has no parent\n");
	    if (exit->orientation == NORTH) {
		sl = MIN(exit->x1, exit->x2);
		sr = MAX(exit->x1, exit->x2);
		sflag = 1;
		if (otherexit->orientation == EAST) {
		    wt = MAX(otherexit->y1, otherexit->y2);
		    wb = MIN(otherexit->y1, otherexit->y2);
		    if ((otherexit->x1 < sl) && (exit->y1 < wb)
				&& ((wt - exit->y1) <= 61)
				&& ((sr - otherexit->x1) <= 61)
				&& ((newroom = box_empty(wt, exit->y1+1,
					otherexit->x1+1, sr, exit->level))
						!= NULL)) {
			wflag = 1;
			mergeflag = SOUTH;
			break;
		    }
		}
		else if (otherexit->orientation == SOUTH) {
		    nl = MIN(otherexit->x1, otherexit->x2);
		    nr = MAX(otherexit->x1, otherexit->x2);
		    if (((otherexit->y1 - 1) > (exit->y1 + 1))
				&& ((otherexit->y1 - exit->y1) <= 62)
				&& ((otherexit->y1 - exit->y1) > 11)
				&& (abs(nl - sl) < 60)
				&& ((newroom = box_empty(otherexit->y1 - 1,
					exit->y1 + 1, MIN(nl, sl), MAX(nr, sr),
						exit->level)) != NULL)) {
			nflag = 1;
			mergeflag = SOUTH;
			break;
		    }
		}
		else if (otherexit->orientation == WEST) {
		    et = MAX(otherexit->y1, otherexit->y2);
		    eb = MIN(otherexit->y1, otherexit->y2);
		    if ((otherexit->x1 > sr) && (exit->y1 < eb)
				&& ((et - exit->y1) <= 61)
				&& ((otherexit->x1 - sl) <= 61)
				&& ((newroom = box_empty(et, exit->y1 + 1, sl,
					otherexit->x1 - 1, exit->level))
						!= NULL)) {
			eflag = 1;
			mergeflag = SOUTH;
			break;
		    }
		}
	    }
	    else if (exit->orientation == SOUTH) {
		nl = MIN(exit->x1, exit->x2);
		nr = MAX(exit->x1, exit->x2);
		nflag = 1;
		if (otherexit->orientation == EAST) {
		    wt = MAX(otherexit->y1, otherexit->y2);
		    wb = MIN(otherexit->y1, otherexit->y2);
		    if ((otherexit->x1 < nl) && (exit->y1 > wt)
				&& ((exit->y1 - wb) <= 61)
				&& ((nr - otherexit->x1) <= 61)
				&& ((newroom = box_empty(exit->y1-1, wb,
					otherexit->x1+1, nr, exit->level))
						!= NULL)) {
			wflag = 1;
			mergeflag = NORTH;
			break;
		    }
		}
		else if (otherexit->orientation == NORTH) {
		    sl = MIN(otherexit->x1, otherexit->x2);
		    sr = MAX(otherexit->x1, otherexit->x2);
		    if (((otherexit->y1 + 1) < (exit->y1 - 1))
				&& ((exit->y1 - otherexit->y1) <= 62)
				&& ((exit->y1 - otherexit->y1) > 11)
				&& (abs(nl - sl) <= 60)
				&& ((newroom = box_empty(exit->y1 - 1,
					otherexit->y1 + 1, MIN(nl,sl),
					MAX(nr,sr), exit->level)) != NULL)) {
			sflag = 1;
			mergeflag = NORTH;
			break;
		    }
		}
		else if (otherexit->orientation == WEST) {
		    et = MAX(otherexit->y1, otherexit->y2);
		    eb = MIN(otherexit->y1, otherexit->y2);
		    if ((otherexit->x1 > nr) && (exit->y1 > et)
				&& ((exit->y1 - eb) <= 61)
				&& ((otherexit->x1 - nl) <= 61)
				&& ((newroom = box_empty(exit->y1 - 1, eb,
					nl, otherexit->x1 - 1, exit->level))
						!= NULL)) {
			eflag = 1;
			mergeflag = NORTH;
			break;
		    }
		}
	    }
	    else if (exit->orientation == EAST) {
		wt = MAX(exit->y1, exit->y2);
		wb = MIN(exit->y1, exit->y2);
		wflag = 1;
		if (otherexit->orientation == NORTH) {
		    sl = MIN(otherexit->x1, otherexit->x2);
		    sr = MAX(otherexit->x1, otherexit->x2);
		    if ((exit->x1 < sl) && (otherexit->y1 < wb)
				&& ((sr - exit->x1) <= 61)
				&& ((wt - otherexit->y1) <= 61)
				&& ((newroom = box_empty(wt, otherexit->y1 + 1,
					exit->x1 + 1, sr, exit->level))
						!= NULL)) {
			sflag = 1;
			mergeflag = WEST;
			break;
		    }
		}
		else if (otherexit->orientation == SOUTH) {
		    nl = MIN(otherexit->x1, otherexit->x2);
		    nr = MAX(otherexit->x1, otherexit->x2);
		    if ((exit->x1 < nl) && (wt < otherexit->y1)
				&& ((nr - exit->x1) <= 61)
				&& ((otherexit->y1 - wb) <= 61)
				&& ((newroom = box_empty(otherexit->y1 - 1, wb,
					exit->x1 + 1, nr, exit->level))
						!= NULL)) {
			nflag = 1;
			mergeflag = WEST;
			break;
		    }
		}
		else if (otherexit->orientation == WEST) {
		    et = MAX(otherexit->y1, otherexit->y2);
		    eb = MIN(otherexit->y1, otherexit->y2);
		    if (((exit->x1 + 1) < (otherexit->x1 - 1))
				&& ((otherexit->x1 - exit->x1) <= 62)
				&& ((otherexit->x1 - exit->x1) > 11)
				&& (abs(et - wt) < 60)
				&& ((newroom = box_empty(MAX(et, wt),
					MIN(eb, wb), exit->x1 + 1,
					otherexit->x1 - 1, exit->level))
						!= NULL)) {
			eflag = 1;
			mergeflag = WEST;
			break;
		    }
		}
	    }
	    else if (exit->orientation == WEST) {
		et = MAX(exit->y1, exit->y2);
		eb = MIN(exit->y1, exit->y2);
		eflag = 1;
		if (otherexit->orientation == NORTH) {
		    sl = MIN(otherexit->x1, otherexit->x2);
		    sr = MAX(otherexit->x1, otherexit->x2);
		    if ((otherexit->y1 < eb) && (sr < exit->x1)
				&& ((exit->x1 - sl) <= 61)
				&& ((et - otherexit->y1) <= 61)
				&& ((newroom = box_empty(et, otherexit->y1 + 1,
					sl, exit->x1 - 1, exit->level))
						!= NULL)) {
			sflag = 1;
			mergeflag = EAST;
			break;
		    }
		}
		else if (otherexit->orientation == SOUTH) {
		    nl = MIN(otherexit->x1, otherexit->x2);
		    nr = MAX(otherexit->x1, otherexit->x2);
		    if ((nr < exit->x1) && (et < otherexit->y1)
				&& ((exit->x1 - nl) <= 61)
				&& ((otherexit->y1 - eb) <= 61)
				&& ((newroom = box_empty(otherexit->y1 - 1,
					eb, nl, exit->x1 - 1, exit->level))
						!= NULL)) {
			nflag = 1;
			mergeflag = EAST;
			break;
		    }
		}
		else if (otherexit->orientation == EAST) {
		    wt = MAX(otherexit->y1, otherexit->y2);
		    wb = MIN(otherexit->y1, otherexit->y2);
		    if (((otherexit->x1 + 1) < (exit->x1 - 1))
				&& ((exit->x1 - otherexit->x1) <= 62)
				&& ((exit->x1 - otherexit->x1) > 11)
				&& (abs(et - wt) <= 61)
				&& ((newroom = box_empty(MAX(et, wt),
					MIN(eb, wb), otherexit->x1 + 1,
					exit->x1 - 1, exit->level))
						!= NULL)) {
			wflag = 1;
			mergeflag = EAST;
			break;
		    }
		}
	    }
	}
	que = que->queuenext;
    }
    if (!mergeflag)
	return (0);
printf("\t\t\tsuccessful merging of two exits! (%d,%d)\n", newroom->xmax,
newroom->ymax);
    newroom->level = exit->level;
    newroom->locationtype = ROOM;
    add_location_to_queue(newroom);
    if (nflag) {
	if (mergeflag == NORTH)
	    draw_exit(exit);
	else {
	    draw_exit(otherexit);
	    que->exit = NULL;
	    free_exit_node(otherexit);
	}
    }
    else {
	delta = expand_allocated_space(newroom, NORTH, (rand() % 3) * 5);
	if (rand() % 2) {
	    nflag = 1;
	    create_exit(newroom, NORTH, &nr, &nl);
	}
    }
    if (sflag) {
	if (mergeflag == SOUTH)
	    draw_exit(exit);
	else {
	    draw_exit(otherexit);
	    que->exit = NULL;
	    free_exit_node(otherexit);
	}
    }
    else {
	delta = expand_allocated_space(newroom, SOUTH, (rand() % 3) * 5);
	if (rand() % 2) {
	    sflag = 1;
	    create_exit(newroom, SOUTH, &sr, &sl);
	}
    }
    if (eflag) {
	if (mergeflag == EAST)
	    draw_exit(exit);
	else {
	    draw_exit(otherexit);
	    que->exit = NULL;
	    free_exit_node(otherexit);
	}
    }
    else {
	delta = expand_allocated_space(newroom, EAST, (rand() % 3) * 5);
	if (rand() % 2) {
	    eflag = 1;
	    create_exit(newroom, EAST, &et, &eb);
	}
    }
    if (wflag) {
	if (mergeflag == WEST)
	    draw_exit(exit);
	else {
	    draw_exit(otherexit);
	    que->exit = NULL;
	    free_exit_node(otherexit);
	}
    }
    else {
	delta = expand_allocated_space(newroom, WEST, (rand() % 3) * 5);
	if (rand() % 2) {
	    wflag = 1;
	    create_exit(newroom, WEST, &wt, &wb);
	}
    }
    if (nflag) {
	add_line_to_location(newroom, newroom->xmin, newroom->ymax, nl,
				newroom->ymax);
	add_line_to_location(newroom, nr, newroom->ymax, newroom->xmax,
				newroom->ymax);
    }
    else
	add_line_to_location(newroom, newroom->xmin, newroom->ymax,
					newroom->xmax, newroom->ymax);
    if (sflag) {
	add_line_to_location(newroom, newroom->xmin, newroom->ymin, sl,
				newroom->ymin);
	add_line_to_location(newroom, sr, newroom->ymin, newroom->xmax,
				newroom->ymin);
    }
    else
	add_line_to_location(newroom, newroom->xmin, newroom->ymin,
					newroom->xmax, newroom->ymin);
    if (eflag) {
	add_line_to_location(newroom, newroom->xmax, newroom->ymax,
				newroom->xmax, et);
	add_line_to_location(newroom, newroom->xmax, eb, newroom->xmax,
				newroom->ymin);
    }
    else
	add_line_to_location(newroom, newroom->xmax, newroom->ymax,
					newroom->xmax, newroom->ymin);
    if (wflag) {
	add_line_to_location(newroom, newroom->xmin, newroom->ymax,
				newroom->xmin, wt);
	add_line_to_location(newroom, newroom->xmin, wb, newroom->xmin,
				newroom->ymin);
    }
    else
	add_line_to_location(newroom, newroom->xmin, newroom->ymax,
					newroom->xmin, newroom->ymin);
    return 1;
}
/****************************************************************************
****************************************************************************/
LOCAT *box_empty(top, bottom, left, right, level)
int top, bottom, left, right, level;
{
    LOCAT *room, *newroom;
    room = firstlocation;
    while (room != NULL) {
	if ((room->level == level)
		&& ((room->xmax >= left) && (room->xmin <= right))
		&& ((room->ymax >= bottom) && (room->ymin <= top)))
	    return (NULL);

	room = room->next;
    }

    newroom = new_location_node();
    newroom->xmax = right;
    newroom->xmin = left;
    newroom->ymax = top;
    newroom->ymin = bottom;

    return (newroom);
}



/****************************************************************************
****************************************************************************/

int location_beyond_opening()
{
    int i, rtn;

    i = rand() % 14;

    if (i < 3)
	rtn = CHAMBER;
    else if (i < 5)
	rtn = PASSAGE;
    else if (i < 13)
	rtn = JUNCTION;
    else
	rtn = STAIRWAY;

    return (rtn);
}



/****************************************************************************
****************************************************************************/

int location_beyond_doorway()
{
    int i, rtn;

    i = rand() % 18;

    if (i < 8)
	rtn = ROOM;
    else if (i < 10)
	rtn = CHAMBER;
    else if (i < 14)
	rtn = PASSAGE;
    else
	rtn = JUNCTION;

    return (rtn);
}



/****************************************************************************
****************************************************************************/

find_exit_position(exit, width, xpos, ypos)
EXITS *exit;
int   *width, *xpos, *ypos;
{
    switch (exit->orientation) {
	case NORTH:
		*width = MAX(exit->x1, exit->x2) - MIN(exit->x1, exit->x2);
		*xpos = MIN(exit->x1, exit->x2);
		*ypos = exit->y1;
		break;

	case SOUTH:
		*width = MAX(exit->x1, exit->x2) - MIN(exit->x1, exit->x2);
		*xpos = MAX(exit->x1, exit->x2);
		*ypos = exit->y1;
		break;

	case EAST:
		*width = MAX(exit->y1, exit->y2) - MIN(exit->y1, exit->y2);
		*xpos = exit->x1;
		*ypos = MAX(exit->y1, exit->y2);
		break;

	case WEST:
		*width = MAX(exit->y1, exit->y2) - MIN(exit->y1, exit->y2);
		*xpos = exit->x1;
		*ypos = MIN(exit->y1, exit->y2);
		break;
    }
}



/****************************************************************************
****************************************************************************/

int draw_room(room, exit)
LOCAT *room;
EXITS *exit;
{
    int   xne, yne, xse, yse, xsw, ysw, xnw, ynw;
    int   nflag, sflag, eflag, wflag;
    int   sl, sr, nl, nr, et, eb, wt, wb;
    int   delta2, delta3;

printf("drawing a room\n");
    delta2 = expand_allocated_space(room, left_direction(exit->orientation),
					(rand() % 5) * 5);
    delta3 = expand_allocated_space(room, right_direction(exit->orientation),
					(rand() % 5) * 5);

    xne = xse = room->xmax;
    xnw = xsw = room->xmin;
    yne = ynw = room->ymax;
    yse = ysw = room->ymin;

	/* make certain that the room measures at least ten feet on a side */
    if (MIN((room->xmax - room->xmin), (room->ymax - room->ymin)) < 10)
	return 1;

    nflag = sflag = eflag = wflag = 0;

    if (exit->orientation == NORTH) {
	sl = MIN(exit->x1, exit->x2);
	sr = MAX(exit->x1, exit->x2);
	sflag = 1;
    }
    else {
	if (rand() % 2) {
	    sflag = 1;
	    create_exit(room, SOUTH, &sr, &sl);
	}
    }

    if (exit->orientation == SOUTH) {
	nl = MIN(exit->x1, exit->x2);
	nr = MAX(exit->x1, exit->x2);
	nflag = 1;
    }
    else {
	if (rand() % 2) {
	    nflag = 1;
	    create_exit(room, NORTH, &nr, &nl);
	}
    }

    if (exit->orientation == EAST) {
	wt = MAX(exit->y1, exit->y2);
	wb = MIN(exit->y1, exit->y2);
	wflag = 1;
    }
    else {
	if (rand() % 2) {
	    wflag = 1;
	    create_exit(room, WEST, &wt, &wb);
	}
    }

    if (exit->orientation == WEST) {
	et = MAX(exit->y1, exit->y2);
	eb = MIN(exit->y1, exit->y2);
	eflag = 1;
    }
    else {
	if (rand() % 2) {
	    eflag = 1;
	    create_exit(room, EAST, &et, &eb);
	}
    }

    if (nflag) {
	add_line_to_location(room, xnw, ynw, nl, yne);
	add_line_to_location(room, nr, ynw, xne, yne);
    }
    else
	add_line_to_location(room, xnw, ynw, xne, yne);

    if (sflag) {
	add_line_to_location(room, xsw, ysw, sl, yse);
	add_line_to_location(room, sr, ysw, xse, yse);
    }
    else
	add_line_to_location(room, xsw, ysw, xse, yse);

    if (eflag) {
	add_line_to_location(room, xne, yne, xne, et);
	add_line_to_location(room, xse, eb, xse, yse);
    }
    else
	add_line_to_location(room, xne, yne, xse, yse);

    if (wflag) {
	add_line_to_location(room, xnw, ynw, xnw, wt);
	add_line_to_location(room, xsw, wb, xsw, ysw);
    }
    else
	add_line_to_location(room, xnw, ynw, xsw, ysw);

    return 0;
}



/****************************************************************************
****************************************************************************/

create_exit(room, orient, high, low)
LOCAT *room;
int   orient, *high, *low;
{
    int   i, length, over, exitwidth;
    EXITS *newexit;

    if ((orient == NORTH) || (orient == SOUTH))
	length = room->xmax - room->xmin;
    else
	length = room->ymax - room->ymin;

    i = rand() % 10;

    if ((i < 2) && (length >= 15))
	exitwidth = 15;
    else if ((i < 8) && (length >= 10))
	exitwidth = 10;
    else
	exitwidth = 5;

    if ((length - exitwidth) > 1)
	over = rand() % (length - exitwidth);
    else
	over = 0;

    newexit = new_exit_node();
    add_exit_to_queue(newexit);
    newexit->parent = room;
    newexit->level = room->level;
    newexit->orientation = orient;

    if ((orient == NORTH) || (orient == SOUTH)) {
	newexit->x1 = *high = room->xmax - over;
	newexit->x2 = *low = room->xmax - over - exitwidth;

	if (orient == NORTH)
	    newexit->y1 = newexit->y2 = room->ymax;
	else
	    newexit->y1 = newexit->y2 = room->ymin;
    }
    else {
	newexit->y1 = *high = room->ymax - over;
	newexit->y2 = *low = room->ymax - over - exitwidth;

	if (orient == EAST)
	    newexit->x1 = newexit->x2 = room->xmax;
	else
	    newexit->x1 = newexit->x2 = room->xmin;
    }

    if (room->locationtype == ROOM) {
	newexit->exittype = DOORWAY;
      newexit->locationtype = location_beyond_doorway();
    }
    else {
	newexit->exittype = OPENING;
	newexit->locationtype = location_beyond_opening();
    }
}



/****************************************************************************
****************************************************************************/

int draw_passage(room, exit, delta)
LOCAT *room;
EXITS *exit;
int   delta;
{
    EXITS *newexit;

printf("drawing a passage\n");
    newexit = new_exit_node();
    add_exit_to_queue(newexit);
    newexit->orientation = exit->orientation;
    newexit->parent = room;
    newexit->level = room->level;
    newexit->exittype = ((rand() % 4) ? OPENING : DOORWAY);

    if (newexit->exittype == DOORWAY)
	newexit->locationtype = location_beyond_doorway();
    else
	newexit->locationtype = location_beyond_opening();

    switch (exit->orientation) {
	case NORTH:
	case SOUTH:
		if (exit->orientation == NORTH) {
		    newexit->y1 = exit->y1 + delta + 1;
		    newexit->y2 = exit->y2 + delta + 1;
		}
		else {
		    newexit->y1 = exit->y1 - delta - 1;
		    newexit->y2 = exit->y2 - delta - 1;
		}

		newexit->x1 = exit->x1;
		newexit->x2 = exit->x2;
		add_line_to_location(room, room->xmin, room->ymin,
					room->xmin, room->ymax);
		add_line_to_location(room, room->xmax, room->ymin,
					room->xmax, room->ymax);
		break;

	case EAST:
	case WEST:
		if (exit->orientation == EAST) {
		    newexit->x1 = exit->x1 + delta + 1;
		    newexit->x2 = exit->x2 + delta + 1;
		}
		else {
		    newexit->x1 = exit->x1 - delta - 1;
		    newexit->x2 = exit->x2 - delta - 1;
		}

		newexit->y1 = exit->y1;
		newexit->y2 = exit->y2;
		add_line_to_location(room, room->xmin, room->ymin,
					room->xmax, room->ymin);
		add_line_to_location(room, room->xmin, room->ymax,
					room->xmax, room->ymax);
		break;
    }

    return 0;
}



/****************************************************************************
****************************************************************************/

int draw_junction(room, exit, delta, width)
LOCAT *room;
EXITS *exit;
int   delta, width;
{
    int   xnw, ynw, xne, yne, xse, yse, xsw, ysw, count;
    EXITS *newexit;

printf("drawing a junction\n");
    count = 0;

    if (delta < width)
	seal_exit(exit);
    else {
	switch (exit->orientation) {
	    case NORTH:
		xnw = xsw = MIN(exit->x1, exit->x2);
		xne = xse = MAX(exit->x1, exit->x2);
		ynw = yne = exit->y1 + 1 + width;
		ysw = yse = exit->y1 + 1;
		break;

	    case SOUTH:
		xnw = xsw = MIN(exit->x1, exit->x2);
		xne = xse = MAX(exit->x1, exit->x2);
		ynw = yne = exit->y1 - 1;
		ysw = yse = exit->y1 - 1 - width;
		break;

	    case EAST:
		xne = xse = exit->x1 + 1 + width;
		xnw = xsw = exit->x1 + 1;
		ynw = yne = MAX(exit->y1, exit->y2);
		ysw = yse = MIN(exit->y1, exit->y2);
		break;

	    case WEST:
		xne = xse = exit->x1 - 1;
		xnw = xsw = exit->x1 - 1 - width;
		ynw = yne = MAX(exit->y1, exit->y2);
		ysw = yse = MIN(exit->y1, exit->y2);
		break;
	}

		/* add zero-length lines at each corner to assure that
		   the necessary allocated space is not compressed */
	add_line_to_location(room, xnw, ynw, xnw, ynw);
	add_line_to_location(room, xne, yne, xne, yne);
	add_line_to_location(room, xse, yse, xse, yse);
	add_line_to_location(room, xsw, ysw, xsw, ysw);

	if (exit->orientation != SOUTH) {
	    if (rand() % 3) {
		++count;
		newexit = new_exit_node();
		add_exit_to_queue(newexit);
		newexit->orientation = NORTH;
		newexit->parent = room;
		newexit->level = room->level;
		newexit->exittype = ((rand() % 4) ? OPENING : DOORWAY);
		newexit->x1 = xnw;
		newexit->y1 = ynw;
		newexit->x2 = xne;
		newexit->y2 = yne;

		if (newexit->exittype == DOORWAY)
		    newexit->locationtype = location_beyond_doorway();
		else
		    newexit->locationtype = location_beyond_opening();
	    }
	    else
		add_line_to_location(room, xnw, ynw, xne, yne);
	}

	if (exit->orientation != NORTH) {
	    if (rand() % 3) {
		++count;
		newexit = new_exit_node();
		add_exit_to_queue(newexit);
		newexit->orientation = SOUTH;
		newexit->parent = room;
		newexit->level = room->level;
		newexit->exittype = ((rand() % 4) ? OPENING : DOORWAY);
		newexit->x1 = xse;
		newexit->y1 = yse;
		newexit->x2 = xsw;
		newexit->y2 = ysw;

		if (newexit->exittype == DOORWAY)
		    newexit->locationtype = location_beyond_doorway();
		else
		    newexit->locationtype = location_beyond_opening();
	    }
	    else
		add_line_to_location(room, xsw, ysw, xse, yse);
	}

	if (exit->orientation != WEST) {
	    if (rand() % 3) {
		++count;
		newexit = new_exit_node();
		add_exit_to_queue(newexit);
		newexit->orientation = EAST;
		newexit->parent = room;
		newexit->level = room->level;
		newexit->exittype = ((rand() % 4) ? OPENING : DOORWAY);
		newexit->x1 = xne;
		newexit->y1 = yne;
		newexit->x2 = xse;
		newexit->y2 = yse;

		if (newexit->exittype == DOORWAY)
		    newexit->locationtype = location_beyond_doorway();
		else
		    newexit->locationtype = location_beyond_opening();
	    }
	    else
		add_line_to_location(room, xne, yne, xse, yse);
	}

	if (exit->orientation != EAST) {
	    if (rand() % 3) {
		++count;
		newexit = new_exit_node();
		add_exit_to_queue(newexit);
		newexit->orientation = WEST;
		newexit->parent = room;
		newexit->level = room->level;
		newexit->exittype = ((rand() % 4) ? OPENING : DOORWAY);
		newexit->x1 = xsw;
		newexit->y1 = ysw;
		newexit->x2 = xnw;
		newexit->y2 = ynw;

		if (newexit->exittype == DOORWAY)
		    newexit->locationtype = location_beyond_doorway();
		else
		    newexit->locationtype = location_beyond_opening();
	    }
	    else
		add_line_to_location(room, xnw, ynw, xsw, ysw);
	}
    }

    return (!count);
}



/****************************************************************************
****************************************************************************/

int draw_stairway(room, exit, delta, width, xpos, ypos)
LOCAT *room;
EXITS *exit;
int   delta, width, xpos, ypos;
{
    LOCAT *newroom;
    EXITS *newexit;

printf("drawing a stairway\n");
    if (delta < 20)
	return 1;

    newroom = new_location_node();
    newroom->xmin = MIN(exit->x1, exit->x2);
    newroom->xmax = MAX(exit->x1, exit->x2);
    newroom->ymin = MIN(exit->y1, exit->y2);
    newroom->ymax = MAX(exit->y1, exit->y2);
    newroom->locationtype = STAIRWAY;

    newexit = new_exit_node();
    newexit->x1 = exit->x1;
    newexit->y1 = exit->y1;
    newexit->x2 = exit->x2;
    newexit->y2 = exit->y2;
    newexit->orientation = exit->orientation;
    newexit->locationtype = location_beyond_opening();
    newexit->exittype = OPENING;
    newexit->parent = newroom;

    switch (exit->orientation) {
	case NORTH: newexit->y1 += 20; newexit->y2 += 20; break;
	case SOUTH: newexit->y1 -= 20; newexit->y2 -= 20; break;
	case EAST:  newexit->x1 += 20; newexit->x2 += 20; break;
	case WEST:  newexit->x1 -= 20; newexit->x2 -= 20; break;
    }

    if (rand() % 2) {
	if (room->level <= startlevel)
	    return 1;

	room->locationdata = UP;
	newroom->locationdata = DOWN;
	newroom->level = room->level - 1;

	delta = expand_allocated_space(newroom, exit->orientation, 20);

	if (delta < 20)
	    return 1;

	draw_stairs_up(room, exit->orientation, width, xpos, ypos);

	switch (exit->orientation) {
	    case NORTH:
		draw_stairs_down(newroom, SOUTH, width, xpos+width, ypos+20);
		break;
	    case SOUTH:
		draw_stairs_down(newroom, NORTH, width, xpos-width, ypos-20);
		break;
	    case EAST:
		draw_stairs_down(newroom, WEST, width, xpos+20, ypos-width);
		break;
	    case WEST:
		draw_stairs_down(newroom, EAST, width, xpos-20, ypos+width);
		break;
	}
    }
    else {
	if (room->level >= maxlevel)
	    return 1;

	room->locationdata = DOWN;
	newroom->locationdata = UP;
	newroom->level = room->level + 1;

	delta = expand_allocated_space(newroom, exit->orientation, 20);

	if (delta < 20)
	    return 1;

	draw_stairs_down(room, exit->orientation, width, xpos, ypos);

	switch (exit->orientation) {
	    case NORTH:
		draw_stairs_up(newroom, SOUTH, width, xpos+width, ypos+20);
		break;
	    case SOUTH:
		draw_stairs_up(newroom, NORTH, width, xpos-width, ypos-20);
		break;
	    case EAST:
		draw_stairs_up(newroom, WEST, width, xpos+20, ypos-width);
		break;
	    case WEST:
		draw_stairs_up(newroom, EAST, width, xpos-20, ypos+width);
		break;
	}
    }

    newexit->level = newroom->level;
    add_location_to_queue(newroom);
    add_exit_to_queue(newexit);

    return 0;
}



/****************************************************************************
****************************************************************************/

draw_stairs_down(room, orient, width, xpos, ypos)
LOCAT *room;
int   orient, width, xpos, ypos;
{
    int i;

    for (i = 0; i < 11; ++i) {
	switch (width) {
	    case 5:
		add_line_to_location(room,
			xpos + stairwaydownlines5[orient-NORTH][i][0],
			ypos + stairwaydownlines5[orient-NORTH][i][1],
			xpos + stairwaydownlines5[orient-NORTH][i][2],
			ypos + stairwaydownlines5[orient-NORTH][i][3]);
		break;

	    case 10:
		add_line_to_location(room,
			xpos + stairwaydownlines10[orient-NORTH][i][0],
			ypos + stairwaydownlines10[orient-NORTH][i][1],
			xpos + stairwaydownlines10[orient-NORTH][i][2],
			ypos + stairwaydownlines10[orient-NORTH][i][3]);
		break;

	    case 15:
		add_line_to_location(room,
			xpos + stairwaydownlines15[orient-NORTH][i][0],
			ypos + stairwaydownlines15[orient-NORTH][i][1],
			xpos + stairwaydownlines15[orient-NORTH][i][2],
			ypos + stairwaydownlines15[orient-NORTH][i][3]);
		break;

	    default:
		printf("width: %d\n", width);
		fatal_error("draw_stairs_down(): bad width of stairway");
	}
    }
}



/****************************************************************************
****************************************************************************/
draw_stairs_up(room, orient, width, xpos, ypos)
LOCAT *room;
int   orient, width, xpos, ypos;
{
    int i;

    for (i = 0; i < 11; ++i) {
	switch (width) {
	    case 5:
		add_line_to_location(room,
				xpos + stairwayuplines5[orient-NORTH][i][0],
				ypos + stairwayuplines5[orient-NORTH][i][1],
				xpos + stairwayuplines5[orient-NORTH][i][2],
				ypos + stairwayuplines5[orient-NORTH][i][3]);
		break;

	    case 10:
		add_line_to_location(room,
				xpos + stairwayuplines10[orient-NORTH][i][0],
				ypos + stairwayuplines10[orient-NORTH][i][1],
				xpos + stairwayuplines10[orient-NORTH][i][2],
				ypos + stairwayuplines10[orient-NORTH][i][3]);
		break;

	    case 15:
		add_line_to_location(room,
				xpos + stairwayuplines15[orient-NORTH][i][0],
				ypos + stairwayuplines15[orient-NORTH][i][1],
				xpos + stairwayuplines15[orient-NORTH][i][2],
				ypos + stairwayuplines15[orient-NORTH][i][3]);
		break;

	    default:
		printf("width: %d\n", width);
		fatal_error("draw_stairs_up(): bad width of stairway");
	}
    }
}



/****************************************************************************
****************************************************************************/

draw_exit(exit)
EXITS *exit;
{
    int width, xpos, ypos;

    find_exit_position(exit, &width, &xpos, &ypos);

    if (exit->parent == NULL)
	fatal_error("draw_exit(): exit has no parent pointer");

    if (exit->exittype == DOORWAY)
	draw_doorway(exit->parent, exit->orientation, width, xpos, ypos);
    else if (exit->exittype == OPENING)
	draw_opening(exit->parent, exit->orientation, width, xpos, ypos);
    else
	fatal_error("process_exit_queue(): unknown exit type");
}



/****************************************************************************
****************************************************************************/

draw_doorway(room, orient, width, xpos, ypos)
LOCAT *room;
int   orient, width, xpos, ypos;
{
    int i;

printf("drawing a doorway\n");
    switch (width) {
	case 5:
		for (i = 0; i < 8; ++i)
		    add_line_to_location(room,
				xpos + doorwaylines5[orient-NORTH][i][0],
				ypos + doorwaylines5[orient-NORTH][i][1],
				xpos + doorwaylines5[orient-NORTH][i][2],
				ypos + doorwaylines5[orient-NORTH][i][3]);
		break;

	case 10:
		for (i = 0; i < 8; ++i)
		    add_line_to_location(room,
				xpos + doorwaylines10[orient-NORTH][i][0],
				ypos + doorwaylines10[orient-NORTH][i][1],
				xpos + doorwaylines10[orient-NORTH][i][2],
				ypos + doorwaylines10[orient-NORTH][i][3]);
		break;

	case 15:
		for (i = 0; i < 9; ++i)
		    add_line_to_location(room,
				xpos + doorwaylines15[orient-NORTH][i][0],
				ypos + doorwaylines15[orient-NORTH][i][1],
				xpos + doorwaylines15[orient-NORTH][i][2],
				ypos + doorwaylines15[orient-NORTH][i][3]);
		break;

	default:
		printf("width given as %d\n", width);
		fatal_error("draw_doorway(): bad width of doorway");
    }
}



/****************************************************************************
****************************************************************************/

draw_opening(room, orient, width, xpos, ypos)
LOCAT *room;
int   orient, width, xpos, ypos;
{
printf("drawing an opening\n");
    switch (orient) {
	case NORTH:
		add_line_to_location(room, xpos, ypos, xpos, ypos+1);
		add_line_to_location(room, xpos+width, ypos, xpos+width,
				ypos+1);
		break;

	case SOUTH:
		add_line_to_location(room, xpos, ypos, xpos, ypos-1);
		add_line_to_location(room, xpos-width, ypos, xpos-width,
				ypos-1);
		break;

	case EAST:
		add_line_to_location(room, xpos, ypos, xpos+1, ypos);
		add_line_to_location(room, xpos, ypos-width, xpos+1,
				ypos-width);
		break;

	case WEST:
		add_line_to_location(room, xpos, ypos, xpos-1, ypos);
		add_line_to_location(room, xpos, ypos+width, xpos-1,
				ypos+width);
		break;
   }
}



/****************************************************************************
****************************************************************************/

seal_exit(exit)
EXITS *exit;
{
printf("sealing an exit\n");
    add_line_to_location(exit->parent, exit->x1, exit->y1, exit->x2, exit->y2);
}



/****************************************************************************
****************************************************************************/

add_line_to_location(room, x1, y1, x2, y2)
LOCAT *room;
int   x1, y1, x2, y2;
{
    LINES *line;

    line = new_line_node();
    line->next = room->firstline;
    room->firstline = line;
    line->x1 = x1;
    line->y1 = y1;
    line->x2 = x2;
    line->y2 = y2;
}



/****************************************************************************
****************************************************************************/

compress_allocated_space(room)
LOCAT *room;
{
    int   minx = mapwidth, miny = mapheight, maxx = 0, maxy = 0;
    LINES *line;

    line = room->firstline;

	/* do nothing if here are no lines */
    if (line == NULL)
	return;

    while (line != NULL) {
	minx = MIN(minx, line->x1);
	minx = MIN(minx, line->x2);
	maxx = MAX(maxx, line->x1);
	maxx = MAX(maxx, line->x2);
	miny = MIN(miny, line->y1);
	miny = MIN(miny, line->y2);
	maxy = MAX(maxy, line->y1);
	maxy = MAX(maxy, line->y2);

	line = line->next;
    }

    room->xmin = minx;
    room->xmax = maxx;
    room->ymin = miny;
    room->ymax = maxy;
}



/****************************************************************************
****************************************************************************/

int expand_allocated_space(location, direction, maxdelta)
LOCAT *location;
int   direction, maxdelta;
{
    int   delta, lvl;
    LOCAT *room;

	/* just return zero if maxdelta is less than one */
    if (maxdelta < 1)
	return (0);

    lvl = location->level;

    switch (direction) {
	case NORTH:
		delta = mapheight;
		room = firstlocation;
		while (room != NULL) {
		    if ((room->level == lvl)
				&& (room->xmax >= location->xmin)
				&& (room->xmin <= location->xmax)
				&& (room->ymax >= location->ymin))
			delta = MIN(delta, room->ymin);
		    room = room->next;
		}
		delta = delta - location->ymax - 1;
		if (delta > maxdelta)
		    delta = maxdelta;
		if (delta > 0)
		    location->ymax += delta;
		break;

	case SOUTH:
		delta = 0;
		room = firstlocation;
		while (room != NULL) {
		    if ((room->level == lvl)
				&& (room->xmax >= location->xmin)
				&& (room->xmin <= location->xmax)
				&& (room->ymin <= location->ymax))
			delta = MAX(delta, room->ymax);
		    room = room->next;
		}
		delta = location->ymin - delta - 1;
		if (delta > maxdelta)
		    delta = maxdelta;
		if (delta > 0)
		    location->ymin -= delta;
		break;

	case EAST:
		delta = mapwidth;
		room = firstlocation;
		while (room != NULL) {
		    if ((room->level == lvl)
				&& (room->ymax >= location->ymin)
				&& (room->ymin <= location->ymax)
				&& (room->xmax >= location->xmin))
			delta = MIN(delta, room->xmin);
		    room = room->next;
		}
		delta = delta - location->xmax - 1;
		if (delta > maxdelta)
		    delta = maxdelta;
		if (delta > 0)
		    location->xmax += delta;
		break;

	case WEST:
		delta = 0;
		room = firstlocation;
		while (room != NULL) {
		    if ((room->level == lvl)
				&& (room->ymax >= location->ymin)
				&& (room->ymin <= location->ymax)
				&& (room->xmin <= location->xmax))
			delta = MAX(delta, room->xmax);
		    room = room->next;
		}
		delta = location->xmin - delta - 1;
		if (delta > maxdelta)
		    delta = maxdelta;
		if (delta > 0)
		    location->xmin -= delta;
		break;

	default:
		fatal_error("expand_allocated_space(): unknown direction");
    }

    return (delta);
}



/****************************************************************************
****************************************************************************/

int opposite_direction(dir)
int dir;
{
    int i;

    switch (dir) {
	case NORTH: i = SOUTH; break;
	case SOUTH: i = NORTH; break;
	case EAST:  i = WEST;  break;
	case WEST:  i = EAST;  break;
	case UP:    i = DOWN;  break;
	case DOWN:  i = UP;    break;
	default: fatal_error("opposite_direction(): unknown direction");
    }

    return (i);
}



/****************************************************************************
****************************************************************************/

int left_direction(dir)
int dir;
{
    int i;

    switch (dir) {
	case NORTH: i = WEST;  break;
	case SOUTH: i = EAST;  break;
	case EAST:  i = NORTH; break;
	case WEST:  i = SOUTH; break;
	default: fatal_error("left_direction(): unknown direction");
    }

    return (i);
}



/****************************************************************************
****************************************************************************/

int right_direction(dir)
int dir;
{
    int i;

    switch (dir) {
	case NORTH: i = EAST;  break;
	case SOUTH: i = WEST;  break;
	case EAST:  i = SOUTH; break;
	case WEST:  i = NORTH; break;
	default: fatal_error("right_direction(): unknown direction");
    }

    return (i);
}



/****************************************************************************
****************************************************************************/

print_postscript_file()
{
    char  fname[100];
    int   lvl;
    int   x,y,p,q;
    FILE  *fopen(), *outfile;
    LOCAT *room;
    LINES *line;

    for (lvl = startlevel; lvl <= maxlevel; ++lvl) {
	printf("Drawing dungeon level %d.\n", lvl);
	sprintf(fname, "%s%d.ps", filename, lvl);

	if ((outfile = fopen(fname, "w")) == NULL)
	    fatal_error("unable to open file \"%s\" for output", fname);
        fprintf(outfile, "%%!PS-ADOBE-2.0\n");
        fprintf(outfile, "%%%%Creator: Dungeon modified 21/7/93\n");
	fprintf(outfile, "%%%%Title: %s%d.ps\n", filename, lvl);
        fprintf(outfile, "%%%%Pages: 1\n");
        room = firstlocation;
	while (room != NULL) {
	    if (room->level == lvl) {
		line = room->firstline;
		fprintf(outfile, "newpath\n");
		while (line != NULL) {
     		    x = (line->x1*ratio);
                    p = (line->x2*ratio);
		    y = (line->y1*ratio);
                    q = (line->y2*ratio);
		    if ((line->x1 != line->x2) || (line->y1 != line->y2)) 
			fprintf(outfile, " %4d %4d moveto\n %4d %4d lineto\n",
			     (x + 50),(y + 50),(p + 50),(q + 50));
		    line = line->next;
		}
		fprintf(outfile, "stroke\n\n");
	    }
	    room = room->next;
	}
	fprintf(outfile, "showpage\n");
	fclose(outfile);
    }
}



/****************************************************************************
****************************************************************************/

print_latex_file()
{
    char  fname[100];
    int   i, dx, dy, length, lvl;
    int   x,y;
    FILE  *fopen(), *outfile;
    LOCAT *room;
    LINES *line;

    for (lvl = startlevel; lvl <= maxlevel; ++lvl) {
	printf("Drawing dungeon level %d.\n", lvl);
	sprintf(fname, "%s%d.tex", filename, lvl);

	if ((outfile = fopen(fname, "w")) == NULL)
	    fatal_error("unable to open file \"%s\" for output", fname);

	for (i = 0; i < LATEXHEADERLEN; ++i)
	    fprintf(outfile, "%s\n", latexheader[i]);

	fprintf(outfile, "\n\\put(10,720){\\shortstack{Level %d}}\n\n", lvl);

	room = firstlocation;
	while (room != NULL) {
	    if (room->level == lvl) {
		switch(room->locationtype) {
		    case ROOM: fprintf(outfile, "%%room\n"); break;
		    case CHAMBER: fprintf(outfile, "%%chamber\n"); break;
		    case PASSAGE: fprintf(outfile, "%%passage\n"); break;
		    case STAIRWAY: fprintf(outfile, "%%stairway\n"); break;
		    case JUNCTION: fprintf(outfile, "%%junction\n"); break;
		}

		line = room->firstline;
		while (line != NULL) {
		    dx = 0;
		    dy = 0;
		    if (line->x1 != line->x2) {
			dx = ((line->x1 < line->x2) ? (1) : (-1));
			length = abs(line->x1 - line->x2);
		    }
		    else if (line->y1 != line->y2) {
			dy = ((line->y1 < line->y2) ? (1) : (-1));
			length = abs(line->y1 - line->y2);
		    }
		    else {
			line = line->next;
			continue;
		    }
		    x = (line->x1*ratio);
                    y = (line->y1*ratio);
		    fprintf(outfile, "\\put(%d,%d){\\line(%d,%d){%d}}\n",
				x,y, dx, dy, length);

		    line = line->next;
		}
	    }
	    room = room->next;
	}

	fprintf(outfile, "\n\\end{picture}\n\\end{document}\n");
	fclose(outfile);
    }
}



/****************************************************************************
****************************************************************************/

EXITS *new_exit_node()
{
    EXITS *ext;

    if (exitfreestack != NULL) {
	ext = exitfreestack;
	exitfreestack = exitfreestack->next;
    }
    else if ((ext = (EXITS*)malloc(sizeof(EXITS))) == NULL)
	fatal_error("new_Exit_node(): malloc memory failed");

    ext->orientation = 0;
    ext->x1 = 0;
    ext->y1 = 0;
    ext->x2 = 0;
    ext->y2 = 0;
    ext->exittype = 0;
    ext->locationtype = ROOM;
    ext->parent = NULL;
    ext->next = NULL;
    ext->level = 0;

    return (ext);
}



/****************************************************************************
****************************************************************************/

free_exit_node(ext)
EXITS *ext;
{
    ext->parent = NULL;
    ext->next = exitfreestack;
    exitfreestack = ext;
}



/****************************************************************************
****************************************************************************/

LINES *new_line_node()
{
    LINES *lin;

    if ((lin = (LINES*)malloc(sizeof(LINES))) == NULL)
	fatal_error("new_line_node(): malloc memory failed");

    lin->x1 = 0;
    lin->y1 = 0;
    lin->x2 = 0;
    lin->y2 = 0;
    lin->next = NULL;

    return (lin);
}



/****************************************************************************
****************************************************************************/

LOCAT *new_location_node()
{
    LOCAT *loc;

    if ((loc = (LOCAT*)malloc(sizeof(LOCAT))) == NULL)
	fatal_error("new_location_node(): malloc memory failed");

    loc->xmin = 0;
    loc->xmax = 0;
    loc->ymin = 0;
    loc->ymax = 0;
    loc->locationtype = ROOM;
    loc->locationdata = -666;
    loc->level = 0;
    loc->firstline = NULL;
    loc->next = NULL;
    loc->previous = NULL;

    return (loc);
}



/****************************************************************************
****************************************************************************/

QUEUE *new_queue_node()
{
    QUEUE *que;

    if (queuefreestack != NULL) {
	que = queuefreestack;
	queuefreestack = queuefreestack->queuenext;
    }
    else if ((que = (QUEUE*)malloc(sizeof(QUEUE))) == NULL)
	fatal_error("new_queue_node(): malloc memory failed");

    que->exit = NULL;
    que->queuenext = NULL;

    return (que);
}



/****************************************************************************
****************************************************************************/

free_queue_node(que)
QUEUE *que;
{
    que->queuenext = queuefreestack;
    queuefreestack = que;
    que->exit = NULL;
}



/****************************************************************************
****************************************************************************/

add_exit_to_queue(exit)
EXITS *exit;
{
    QUEUE *que;

++gc;
printf("adding exit to queue, newsize: %d\n", gc);
    que = new_queue_node();
    que->exit = exit;

    if (queuehead == NULL)
	queuehead = queuetail = que;
    else {
	    /* decude whether to place new exit at front or end of queue */
	if (EXITSTACK) {	/* do as stack */
	    que->queuenext = queuehead;
	    queuehead = que;
	}
	else {			/* do as queue */
	    queuetail->queuenext = que;
	    queuetail = que;
	}
    }
}



/****************************************************************************
****************************************************************************/

EXITS *dequeue_exit()
{
    EXITS *ext;
    QUEUE *que;

--gc;
    if (queuehead == NULL)
	fatal_error("dequeue_exit(): attempt to dequeue from empty queue");

    ext = queuehead->exit;
    que = queuehead;

    if (queuehead->queuenext == NULL)
	queuehead = queuetail = NULL;
    else
	queuehead = queuehead->queuenext;

    free_queue_node(que);

    return (ext);
}



/****************************************************************************
****************************************************************************/

add_location_to_queue(ptr)
LOCAT *ptr;
{
    if (firstlocation == NULL)
	firstlocation = lastlocation = ptr;
    else {
	ptr->next = NULL;
	ptr->previous = lastlocation;
	lastlocation->next = ptr;
	lastlocation = ptr;
    }
}



/****************************************************************************
****************************************************************************/

fatal_error(strg, arg)
char strg[], arg[];
{
    printf("Error: ");
    printf(strg, arg);
    printf("\n\n");
    exit(1);
}
