/******************************************************************************
 **  Program:           ssv
 **
 **  Description:       X11-based previewer for Traveller Sub-Sector map data.
 **
 **  File:              ssv.c, containing the following subroutines/functions:
 **                       main()
 **                       gen_sector()
 **                       load_sector_file()
 **                       load_bdr_seg()
 **                       print_sector_file()
 **                       repaint_buttons()
 **                       mark_border()
 **                       print_subsector()
 **                       Get_Colors()
 **                       _swaplong()
 **                       _swapshort()
 **                       parse_args()
 **			  usage()
 **
 **  Copyright 1990 by Mark F. Cook and Hewlett-Packard,
 **                             Interface Technology Operation
 **  Modified 6/1990 by Dan Corrin (dan@engrg.uwo.ca)
 **  Modified 7/1993 by Jonathan Sari (surge@cqs.washington.edu)
 **
 **  Permission to use, copy, and modify this software is granted, provided
 **  that this copyright appears in all copies and that both this copyright
 **  and permission notice appear in all supporting documentation, and that
 **  the name of Mark F. Cook and/or Hewlett-Packard not be used in advertising
 **  without specific, writen prior permission.  Neither Mark F. Cook or
 **  Hewlett-Packard make any representations about the suitibility of this
 **  software for any purpose.  It is provided "as is" without express or
 **  implied warranty.
 **
 *****************************************************************************/

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <strings.h>

/******************************
#define NORMAL_FONT "hp8.8x16"
#define BOLD_FONT   "hp8.8x16b"
******************************/

/*
#define NORMAL_FONT "iso1.16"
#define BOLD_FONT   "iso1.16b"
*/
#define NORMAL_FONT "8x13"
#define BOLD_FONT   "8x13bold"
#define SMALL_FONT  "6x10"

/*****************************************************************************
 **
 **  These bitmaps define the base code symbols used for each star system.
 **
 *****************************************************************************/

#include "bitmaps/naval1.xbm"
#include "bitmaps/naval2.xbm"
#include "bitmaps/scout1.xbm"
#include "bitmaps/scout2.xbm"
#include "bitmaps/depot.xbm"
#include "bitmaps/aslan.xbm"
#include "bitmaps/corsair.xbm"
#include "bitmaps/military.xbm"
#include "bitmaps/tlaukhu.xbm"
#include "bitmaps/zhodane.xbm"

/*****************************************************************************
 **
 **  This next typedef is lifted straight out of the xwd/xwud utility pair,
 **  and defines the file format used by them and the xpr print utility.
 **
 *****************************************************************************/

typedef unsigned long imgval;

#define XWD_FILE_VERSION    7

typedef struct _img_file_header {
        imgval header_size;       /* Size of the entire file header (bytes). */
        imgval file_version;      /* FILE_VERSION */
        imgval pixmap_format;     /* Pixmap format */
        imgval pixmap_depth;      /* Pixmap depth */
        imgval pixmap_width;      /* Pixmap width */
        imgval pixmap_height;     /* Pixmap height */
        imgval xoffset;           /* Bitmap x offset */
        imgval byte_order;        /* MSBFirst, LSBFirst */
        imgval bitmap_unit;       /* Bitmap unit */
        imgval bitmap_bit_order;  /* MSBFirst, LSBFirst */
        imgval bitmap_pad;        /* Bitmap scanline pad */
        imgval bits_per_pixel;    /* Bits per pixel */
        imgval bytes_per_line;    /* Bytes per scanline */
        imgval visual_class;      /* Class of colormap */
        imgval red_mask;          /* Z red mask */
        imgval green_mask;        /* Z green mask */
        imgval blue_mask;         /* Z blue mask */
        imgval bits_per_rgb;      /* Log base 2 of distinct color values */
        imgval colormap_entries;  /* Number of entries in colormap */
        imgval ncolors;           /* Number of Color structures */
        imgval window_width;      /* Window width */
        imgval window_height;     /* Window height */
        long window_x;            /* Window upper left X coordinate */
        long window_y;            /* Window upper left Y coordinate */
        imgval window_bdrwidth;   /* Window border width */
} IMGFileHeader;

/*****************************************************************************
 **
 **  This checker pattern is used to draw the lines for political/military
 **  control boundaries within a given subsector.
 **
 *****************************************************************************/

#define sm_chex_width 16
#define sm_chex_height 16
static char sm_chex_bits[] = {
   0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa,
   0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa,
   0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa};

#define  TRUE   1
#define  FALSE  0

#define  GARDEN   2
#define  ASTEROID 1
#define  DESERT   0

typedef struct _worldstruct {
        XPoint location;
        int WorldType;
        int GasGiant;
        char Starport[2];
        char Base[2];
        char Zone[2];
        char hex[5];
        char name[21];
	char uwp[9];
	char notes[13];
        char allegiance[3];
        } World;

XSegment t_route[80], bdr_seg[160], file_bdr_seg[160];
World sec_world[80];

char line[128], ch, title[80];

static char map_name[] = {"SUB-SECTOR"};
static char panel_name[] = {"PANEL"};
static int DISP_ALL = 1;
static int DISP_TRADE = 1;
static int DISP_CODE = 1;

#define NUM_HEX_PTS   7
#define NUM_HEXES     8
#define NUM_LINES    10
#define LINE_INC    100

#define BTN_WIDTH   100
#define BTN_HEIGHT   20
#define NUM_BTNS      7

#define PAD          16
#define HEX_PAD       6	/* The number of hexes around the current sector that
			   we store location information on */

char *button_label[NUM_BTNS] = { "MARK BORDER", "CLEAR BORDER",
      "PRINT MAP", "ALLEGIANCE", "TRADE CODES", "UWP", "QUIT" };

int button_state[NUM_BTNS] = { FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE };

/****************************************************************************
 *  This array contains the list of hex centers (from left to right) with   *
 *  6 additional centers listed on either end of the row.  This allows X-   *
 *  boat routes to be drawn up to 4 hexes off to either side of the current *
 *  subsector map.                                                          *
 ****************************************************************************/

static XPoint hex_ctr[NUM_HEXES + 2*HEX_PAD] = {
		{-470,  60},
		{-380, 110},
                {-290,  60},
                {-200, 110},
                {-110,  60},
                { -20, 110},
                {  70,  60},
                { 160, 110},
                { 250,  60},
                { 340, 110},
                { 430,  60},
                { 520, 110},
                { 610,  60},
                { 700, 110},
                { 790,  60},
                { 880, 110},
                { 970,  60},
                {1060, 110},
	        {1150,  60},
	        {1240, 110},
	    };

/*****************************************************************************
 *  The next values are the 6 points of each hex, arranged in CoordModePrev. *
 *  format.  The 1st value is filled in during each pass thru     0-1        *
 *  a double 'for' loop.  The points are ordered as follows:    5/   \2      *
 *                                                               \4_3/       *
 *****************************************************************************/

static XPoint hex_pts[NUM_HEX_PTS] = {
                {  0,   0},
                { 60,   0},
                { 30,  50},
                {-30,  50},
                {-60,   0},
                {-30, -50},
                { 30, -50} };

/*****************************************************************************
 *  The next values are the 6 points of each hex, arranged in CoordModeAbs.  *
 *  format (sort of).  Each point is an absolute value, relative to the      *
 *  0th [x,y] point in the array.  This is used for rendering static borders *
 *  loaded from the data file, rather than entered interactively.  The point *
 *  ordering is the same as that in the hex_pts array (above).               *
 *****************************************************************************/

static XPoint abs_hex_pts[NUM_HEX_PTS] = {
                {  0,   0},
                { 60,   0},
                { 90,  50},
                { 60, 100},
                {  0, 100},
                {-30,  50},
                {  0,   0} };

/*****************************************************************************
 *  These values are the '0' point for each hex in a row (from left to       *
 *  right).  These values are substituted into the hex_pt array (above)      *
 *  as each hex is rendered.                                                 *
 *****************************************************************************/

static XPoint hex_loc[NUM_HEXES] = {
                { 40, 10},
                {130, 60},
                {220, 10},
                {310, 60},
                {400, 10},
                {490, 60},
                {580, 10},
                {670, 60} };

#define RED_COLOR	"red"
#define AMBER_COLOR	"LightGoldenrod"
#define XBOAT_COLOR	"MidnightBlue"
#define BORDER_COLOR	"LightSlateGrey"
#define ICON_COLOR	"blue"

#define CL_BACKGROUND	0	/* The color list index for the background */
#define CL_FOREGROUND	1				/*	foreground */
#define CL_HEXES	2				/*	hexes */
#define CL_BORDER	3				/*	border */
#define CL_XBOAT	4				/*	xboat routes */
#define CL_REDZONE	5				/*	red zone */
#define CL_AMBERZONE	6				/*	amber zone */
#define CL_ICONS	7				/*	base icons */
#define MAX_COLORS	8	/* Must be at least 1 + last CL index */

/* A structure to hold all of the info gleaned from argc, argv. */
struct argument_data {
    /* These should always be set */
    char *progname;	/* Store argv[0] here for convenience */
    char *datafile;	/* The datafile */

    /* The following are flags, unset by default */
    char black_white;	/* Force black and white display */
    char print_only;	/* Print instead of doing an X display */

    /* The following are optional parameters, NULL if non-set */

    /* X stuff */
    char *display;	/* The X display, if specified on the command line */
    /* Font information */
    char *normal_font;	/* The font for most information */
    char *bold_font;	/* The font for the starport type and high pop */
    char *small_font;	/* The font for the UPP code */
    char *foreground;	/* The foreground color, if appropriate */
    char *background;	/* The background color, if appropriate */
    char *red_color;	/* The color for red zones */
    char *amber_color;	/* The color for amber zones */
    char *xboat_color;	/* The color for trade/xboat routes */
    char *border_color;	/* The color for borders */
    char *icon_color;	/* The color for base/planet/gas giant icons */
    char *hex_color;	/* The color for the hexes */
};

/* The global declaration of that structure */
struct argument_data arginfo;

/* Global variables */
Display		*dpy;
Window		win, panel, button[NUM_BTNS];
GC		black_gc, white_gc, neg_gc, flicker_gc;
int		w_cnt, tr_cnt, bdr_cnt, cur_bdr_cnt=0, arg_cnt;
int		private_bdr_cnt, ScrDepth;
Visual		*visual;
XFontStruct	*fptr, *fBptr, *fsptr;
Pixmap		solid, chex, Naval1Pix, Naval2Pix;
Pixmap		Scout1Pix, Scout2Pix, DepotPix;
Pixmap		AslanPix, CorsairPix, MilPix, TlaukhuPix, ZhodanePix;
unsigned long	black, white;
XEvent		event;
unsigned long	color_list[MAX_COLORS];
static int parse_args();

int main(int argc, char **argv)
{
  KeySym      key;
  XSizeHints  xsh1, xsh2;
  int      screen, i, j, done;
  int load_sector_file(), print_sector_file();
  unsigned long w_a_mask;
  void usage(), exit();
  char   text[10];

  if (!parse_args(argc, argv)) {
      usage();
      exit(1);
  }

  if (!load_sector_file()) {
      fprintf(stderr, "%s: Invalid datafile \"%s\"\n", argv[0],
	      arginfo.datafile);
      exit(1); }

  if ((dpy = XOpenDisplay(arginfo.display ? arginfo.display : NULL))
      == NULL) {
      fprintf(stderr, "%s: Cannot open display \"%s\"\n", arginfo.progname,
	      XDisplayName(arginfo.display ? arginfo.display : NULL));
      exit(1); }

  ScrDepth = XDefaultDepth(dpy, DefaultScreen(dpy));

  fptr = XLoadQueryFont(dpy,
	arginfo.normal_font ? arginfo.normal_font : NORMAL_FONT);
  if (fptr == NULL) {
      fprintf(stderr, "%s: Cannot open font \"%s\"\n", arginfo.progname,
	arginfo.normal_font ? arginfo.normal_font : NORMAL_FONT);
      if (!arginfo.normal_font)
	  fprintf(stderr, "\tTry specifying a font for me to use with -fn\n");
      exit(1); }
  fBptr = XLoadQueryFont(dpy,
	arginfo.bold_font ? arginfo.bold_font : BOLD_FONT);
  if (fBptr == NULL) {
      fprintf(stderr, "%s: Cannot open font \"%s\"\n", arginfo.progname,
	arginfo.bold_font ? arginfo.bold_font : BOLD_FONT);
      if (!arginfo.bold_font)
	  fprintf(stderr, "\tTry specifying a font for me to use with -fb\n");
      exit(1); }
  fsptr = XLoadQueryFont(dpy,
	arginfo.small_font ? arginfo.small_font : SMALL_FONT);
  if (fsptr == NULL) {
      fprintf(stderr, "%s: Cannot open font \"%s\"\n", arginfo.progname,
	arginfo.small_font ? arginfo.small_font : SMALL_FONT);
      if (!arginfo.small_font)
	  fprintf(stderr, "\tTry specifying a font for me to use with -fs\n");
      exit(1); }

  screen = DefaultScreen(dpy);

  visual = DefaultVisual(dpy, screen);
  black = BlackPixel(dpy, screen);    white = WhitePixel(dpy, screen);
  if (ScrDepth == 1 || arginfo.black_white) {
      /* Defaults */
      color_list[CL_BACKGROUND]	= WhitePixel(dpy, screen);
      color_list[CL_FOREGROUND]	= BlackPixel(dpy, screen);
      color_list[CL_HEXES]	= BlackPixel(dpy, screen);
      color_list[CL_BORDER]	= BlackPixel(dpy, screen);
      color_list[CL_XBOAT]	= BlackPixel(dpy, screen);
      color_list[CL_REDZONE]	= BlackPixel(dpy, screen);
      color_list[CL_AMBERZONE]	= BlackPixel(dpy, screen);
      color_list[CL_ICONS]	= BlackPixel(dpy, screen);
  } else {
      void allocate_color();

      if (visual->class != GrayScale && visual->class != StaticGray) {
	  Colormap cmap;
	  XColor current;

	  cmap = DefaultColormap(dpy, screen);
	  if (arginfo.background) {
	      int f;

	      if (!(f = XParseColor(dpy, cmap, arginfo.background, &current))){
		  fprintf(stderr, "%s: %s - no such color in database\n",
			 arginfo.progname, arginfo.background);
	      } else if (!(f = XAllocColor(dpy, cmap, &current))) {
		  fprintf(stderr, "%s: all colorcells allocated!\n",
			 arginfo.progname);
	      }
	      if (f) color_list[CL_BACKGROUND] = current.pixel;
	      else {
		  color_list[CL_BACKGROUND] = WhitePixel(dpy, screen);
		  fprintf(stderr, "\tusing default value\n");
	      }
	  } else {
	      color_list[CL_BACKGROUND] = WhitePixel(dpy, screen);
	  }
	  allocate_color(CL_FOREGROUND, arginfo.foreground, screen);
	  allocate_color(CL_HEXES, arginfo.hex_color, screen);
	  allocate_color(CL_ICONS,
		arginfo.icon_color ? arginfo.icon_color : ICON_COLOR,
		screen);
	  allocate_color(CL_BORDER,
		arginfo.border_color ? arginfo.border_color : BORDER_COLOR,
		screen);
	  allocate_color(CL_XBOAT,
		arginfo.xboat_color ? arginfo.xboat_color : XBOAT_COLOR,
		screen);
	  allocate_color(CL_REDZONE,
		arginfo.red_color ? arginfo.red_color : RED_COLOR,
		screen);
	  allocate_color(CL_AMBERZONE,
		arginfo.amber_color ? arginfo.amber_color : AMBER_COLOR,
		screen);
      }
  }

  xsh1.flags  = (PPosition | PSize);
#ifndef ANY_SIZE
  xsh1.height = 1070+PAD;    xsh1.width  = 770;
  xsh1.x      = 10;          xsh1.y      = 10;
#else
  if (DisplayHeight(dpy, screen) >= 1070 + PAD) {
      xsh1.height = 1070 + PAD;
      xsh1.width  = 770;	/* For now, assume that we have enough width */
  } else {
      xsh1.height = DisplayHeight(dpy, screen);	/* As big as we can get */
      xsh1.width  = (1000 * xsh1.height) / 137;	/* Assure proper ratio */
  }
#endif

  win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
		xsh1.x, xsh1.y, xsh1.width, xsh1.height, 1,
		color_list[CL_FOREGROUND], color_list[CL_BACKGROUND]);

  XSetStandardProperties(dpy, win, map_name, map_name, None, argv, argc, &xsh1);

  xsh2.flags = (PPosition | PSize);
  xsh2.x     = 10;		xsh2.y      = 10;
  xsh2.width = BTN_WIDTH + 10;
  xsh2.height = (BTN_HEIGHT+5) * NUM_BTNS + 5;

  panel = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
		xsh2.x, xsh2.y, xsh2.width, xsh2.height, 1,
		color_list[CL_FOREGROUND], color_list[CL_BACKGROUND]);

  XSetStandardProperties(dpy, panel, panel_name, panel_name, None, argv,
                        argc, &xsh2);

  for (i=0 ; i<NUM_BTNS ; ++i)
      button[i] = XCreateSimpleWindow(dpy, panel, 5, ((BTN_HEIGHT+5)*i)+5,
		BTN_WIDTH, BTN_HEIGHT, 1, color_list[CL_FOREGROUND],
		color_list[CL_BACKGROUND]);

  black_gc   = XCreateGC(dpy, win, 0, 0);
  white_gc   = XCreateGC(dpy, win, 0, 0);
  neg_gc     = XCreateGC(dpy, win, 0, 0);
  flicker_gc = XCreateGC(dpy, win, 0, 0);

  XSetFont(dpy, black_gc, fptr->fid);
  XSetFont(dpy, white_gc, fptr->fid);
  XSetFont(dpy, neg_gc, fptr->fid);

  XSetForeground(dpy, black_gc, color_list[CL_FOREGROUND]);
  XSetBackground(dpy, black_gc, color_list[CL_BACKGROUND]);
  XSetForeground(dpy, white_gc, color_list[CL_BACKGROUND]);
  XSetBackground(dpy, white_gc, color_list[CL_FOREGROUND]);
  XSetForeground(dpy, flicker_gc, color_list[CL_FOREGROUND]);
  XSetBackground(dpy, flicker_gc, color_list[CL_BACKGROUND]);
  XSetForeground(dpy, neg_gc, color_list[CL_BACKGROUND]);
  XSetBackground(dpy, neg_gc, color_list[CL_FOREGROUND]);

  XSetFunction(dpy, flicker_gc, GXinvert);
  XSetLineAttributes(dpy, flicker_gc, 1, LineSolid, CapButt, JoinMiter);
  XSetPlaneMask(dpy, flicker_gc, 1);

  chex = XCreatePixmapFromBitmapData(dpy, win, sm_chex_bits, sm_chex_width,
		sm_chex_height, color_list[CL_BACKGROUND],
		color_list[CL_FOREGROUND], ScrDepth);

  XSetTile(dpy, black_gc, chex);
  XSetTile(dpy, white_gc, chex);

  Naval1Pix = XCreatePixmapFromBitmapData(dpy, win, naval1_bits, naval1_width,
                naval1_height, color_list[CL_ICONS], color_list[CL_BACKGROUND],
		ScrDepth);
  Naval2Pix = XCreatePixmapFromBitmapData(dpy, win, naval2_bits, naval2_width,
                naval2_height, color_list[CL_ICONS], color_list[CL_BACKGROUND],
		ScrDepth);
  Scout1Pix = XCreatePixmapFromBitmapData(dpy, win, scout1_bits, scout1_width,
                scout1_height, color_list[CL_ICONS], color_list[CL_BACKGROUND],
		ScrDepth);
  Scout2Pix = XCreatePixmapFromBitmapData(dpy, win, scout2_bits, scout2_width,
                scout2_height, color_list[CL_ICONS], color_list[CL_BACKGROUND],
		ScrDepth);
  DepotPix  = XCreatePixmapFromBitmapData(dpy, win, depot_bits, depot_width,
                depot_height, color_list[CL_ICONS], color_list[CL_BACKGROUND],
		ScrDepth);
  AslanPix  = XCreatePixmapFromBitmapData(dpy, win, aslan_bits, aslan_width,
                aslan_height, color_list[CL_ICONS], color_list[CL_BACKGROUND],
		ScrDepth);
  CorsairPix = XCreatePixmapFromBitmapData(dpy, win, corsair_bits,
                corsair_width, corsair_height, color_list[CL_ICONS],
		color_list[CL_BACKGROUND], ScrDepth);
  MilPix    = XCreatePixmapFromBitmapData(dpy, win, military_bits,
                military_width, military_height, color_list[CL_ICONS],
		color_list[CL_BACKGROUND], ScrDepth);
  TlaukhuPix = XCreatePixmapFromBitmapData(dpy, win, tlaukhu_bits,
                tlaukhu_width, tlaukhu_height, color_list[CL_ICONS],
		color_list[CL_BACKGROUND], ScrDepth);
  ZhodanePix = XCreatePixmapFromBitmapData(dpy, win, zhodane_bits,
                zhodane_width, zhodane_height, color_list[CL_ICONS],
		color_list[CL_BACKGROUND], ScrDepth);

  done = FALSE;
  if (arginfo.print_only) {
      print_subsector();
      done = TRUE;
  } else {
      XSelectInput(dpy, win, ButtonPressMask | PointerMotionMask |
		   KeyPressMask | ExposureMask);
      for (j=0 ; j<NUM_BTNS ; ++j)
	  XSelectInput(dpy, button[j], ButtonPressMask | ExposureMask);
      XMapWindow(dpy, win);
      XMapSubwindows(dpy, panel);
      XMapWindow(dpy, panel);
  }

  while (!done) {
      XNextEvent(dpy, &event);
      switch (event.type) {
        case Expose:
              if (event.xexpose.count == 0) {
                if (event.xexpose.window == win)
                  gen_sector(win);
                if ((event.xexpose.window == button[0]) ||
                    (event.xexpose.window == button[1]) ||
                    (event.xexpose.window == button[2]) ||
                    (event.xexpose.window == button[3]) ||
                    (event.xexpose.window == button[4]) ||
                    (event.xexpose.window == button[5]) ||
                    (event.xexpose.window == button[6]) ||
		    (event.xexpose.window == button[7]))
                  repaint_buttons();
               }
              break;
        case MappingNotify:
              XRefreshKeyboardMapping ( &event);
              break;
        case ButtonPress:
              if (event.xbutton.window == button[0])
                mark_border();
              else if (event.xbutton.window == button[1]) {
                button_state[1] = TRUE;
                repaint_buttons();
                XClearWindow(dpy, win);
                bdr_cnt = cur_bdr_cnt;
                gen_sector(win);
                button_state[1] = FALSE;
                repaint_buttons();
               }
              else if (event.xbutton.window == button[2])
                print_subsector();
              else if (event.xbutton.window == button[3]) {
		DISP_ALL = 1-DISP_ALL;
		button_state[3] = DISP_ALL;
                XClearWindow(dpy, win);
                gen_sector(win);
                repaint_buttons();
		}
              else if (event.xbutton.window == button[4]) {
		DISP_TRADE = 1-DISP_TRADE;
		button_state[4] = DISP_TRADE;
                XClearWindow(dpy, win);
                gen_sector(win);
                repaint_buttons();
		}
              else if (event.xbutton.window == button[5]) {
		DISP_CODE = 1-DISP_CODE;
		button_state[5] = DISP_CODE;
                XClearWindow(dpy, win);
                gen_sector(win);
                repaint_buttons();
		}
              else if (event.xbutton.window == button[6]) {
                button_state[6] = TRUE;
                repaint_buttons();
                done++;
               }
              break;
        case KeyPress:
              i = XLookupString(&event, text, 10, &key, NULL);
              if (i == 1 && text[0] == 'q') done++;
              break;
      } /* switch */
  } /* while (!done) */
  XDestroyWindow(dpy, win);
  XDestroyWindow(dpy, panel);
  XCloseDisplay(dpy);
  exit(0);
}

gen_sector(d)
Drawable d;
{
  int i, j, x_ctr, y_ctr, x, y, len;
  short x1, y1, x2, y2;
  World *w;

/*--- Step 1: generate the trade-routes within the grid ---*/
  XSetForeground(dpy, black_gc, color_list[CL_XBOAT]);
  XSetLineAttributes(dpy, black_gc, 5, LineSolid, CapRound, JoinMiter);
  for (i=0; i<tr_cnt; i++) {
    x1 = t_route[i].x1;
    y1 = t_route[i].y1;
    x2 = t_route[i].x2;
    y2 = t_route[i].y2;
    y1 = hex_ctr[x1+HEX_PAD].y + (y1 * LINE_INC);
    x1 = hex_ctr[x1+HEX_PAD].x;
    y2 = hex_ctr[x2+HEX_PAD].y + (y2 * LINE_INC);
    x2 = hex_ctr[x2+HEX_PAD].x;
    if ((x1 > -5000) && (x1 < 5000) &&
        (y1 > -5000) && (y1 < 5000) &&
        (x2 > -5000) && (x2 < 5000) &&
        (y2 > -5000) && (y2 < 5000))
      XDrawLine(dpy, d, black_gc, x1, y1+PAD, x2, y2+PAD);
   }
  XFlush(dpy);
  XSetLineAttributes(dpy, black_gc, 1, LineSolid, CapButt, JoinMiter);

/*--- Step 2: generate the empty grid ---*/
  XFillRectangle(dpy, d, white_gc, 0, 0, 770, 10+PAD);
  XFillRectangle(dpy, d, white_gc, 0, 0, 10, 1070+PAD);
  XFillRectangle(dpy, d, white_gc, 0, 1060+PAD, 770, 10); 
  XFillRectangle(dpy, d, white_gc, 760, 0, 10, 1070); 
  XSetForeground(dpy, black_gc, color_list[CL_HEXES]);
  XSetLineAttributes(dpy, black_gc, 0, LineSolid, CapButt, JoinMiter);
  for (i=0; i<NUM_LINES*LINE_INC; i+=LINE_INC) {
    for (j=0; j<NUM_HEXES; j++) {
      hex_pts[0].x = hex_loc[j].x;
      hex_pts[0].y = hex_loc[j].y + i + PAD;
      XDrawLines(dpy, d, black_gc, hex_pts, NUM_HEX_PTS, CoordModePrevious);
     }
   }
  XSetForeground(dpy, black_gc, color_list[CL_FOREGROUND]);
  XDrawLine(dpy, d, black_gc, 730, 60+PAD, 760, 10+PAD);
  XDrawLine(dpy, d, black_gc, 40, 1010+PAD, 10, 1060+PAD);
  XSetLineAttributes(dpy, black_gc, 3, LineSolid, CapButt, JoinMiter);
  XDrawRectangle(dpy, d, black_gc, 10, 10+PAD, 750, 1050); 
  XSetLineAttributes(dpy, black_gc, 1, LineSolid, CapButt, JoinMiter);
  /*--- Print the sector/subsector title ---*/
  XSetForeground(dpy, black_gc, color_list[CL_FOREGROUND]);
  len = XTextWidth(fptr, title, strlen(title)-1); 
  XSetFont(dpy, black_gc, fBptr->fid);
  XDrawImageString(dpy, d, black_gc, (770-len)/2, 16, title, strlen(title)-1);
  XSetFont(dpy, black_gc, fptr->fid);
  XFlush(dpy);

/*--- Step 3: if zone borders exist, generate them ---*/
  if (bdr_cnt || private_bdr_cnt) {
    XSetForeground(dpy, black_gc, color_list[CL_BORDER]);
    XSetFillStyle(dpy, black_gc, (color_list[CL_BORDER] == black) ? FillTiled
	: FillSolid);
    XSetLineAttributes(dpy, black_gc, 5, LineSolid, CapButt, JoinMiter);
    if (bdr_cnt)
      for (i=0 ; i<bdr_cnt ; ++i)
	XDrawLine(dpy, d, black_gc, bdr_seg[i].x1, bdr_seg[i].y1,
                                bdr_seg[i].x2, bdr_seg[i].y2);
    if (private_bdr_cnt)
	XDrawSegments(dpy, d, black_gc, file_bdr_seg, private_bdr_cnt);
    if (color_list[CL_BORDER] == black)
	XSetFillStyle(dpy, black_gc, FillSolid);
    XSetLineAttributes(dpy, black_gc, 1, LineSolid, CapButt, JoinMiter);
   }

/*--- Step 4: generate each system within the grid ---*/
  for (i=0; i<w_cnt; i++) {
    w = &sec_world[i];
    x = w->location.x;
    y = w->location.y;
    x_ctr = hex_ctr[x+HEX_PAD].x;
    y_ctr = hex_ctr[x+HEX_PAD].y + (y * LINE_INC);

    if (w->Zone[0] == 'R') {
	XSetFillStyle(dpy, black_gc, (color_list[CL_REDZONE] == black)
		? FillTiled : FillSolid);
        XSetForeground(dpy, black_gc, color_list[CL_REDZONE]);
        XFillArc(dpy, d, black_gc, x_ctr-45, y_ctr-45+PAD, 90, 90, 0, 360*64);
	if (color_list[CL_REDZONE] == black)
	    XSetFillStyle(dpy, black_gc, FillSolid);
    }
    if (w->Zone[0] == 'A') {
	XSetFillStyle(dpy, black_gc, (color_list[CL_AMBERZONE] == black)
		? FillTiled : FillSolid);
        XSetForeground(dpy, black_gc, color_list[CL_AMBERZONE]);
        XSetLineAttributes(dpy, black_gc, 5, LineSolid, CapButt, JoinMiter);
        XDrawArc(dpy, d, black_gc, x_ctr-45, y_ctr-45+PAD, 90, 90, 0, 360*64);
	if (color_list[CL_AMBERZONE] == black)
	    XSetFillStyle(dpy, black_gc, FillSolid);
        XSetLineAttributes(dpy, black_gc, 1, LineSolid, CapButt, JoinMiter);
    }

    XSetForeground(dpy, black_gc, color_list[CL_FOREGROUND]);
    XFillArc(dpy, d, white_gc, x_ctr-12, y_ctr-15+PAD,   24, 24, 0, 360*64);
    if (w->WorldType == DESERT) {
      XSetLineAttributes(dpy, black_gc, 2, LineSolid, CapButt, JoinMiter);
      XDrawArc(dpy, d, black_gc, x_ctr-10, y_ctr-13+PAD, 20, 20, 0, 360*64);
      XSetLineAttributes(dpy, black_gc, 1, LineSolid, CapButt, JoinMiter);
    } else if (w->WorldType == GARDEN)
      XFillArc(dpy, d, black_gc, x_ctr-10, y_ctr-13+PAD, 20, 20, 0, 360*64);
    else {
      XFillArc(dpy, d, black_gc, x_ctr-8, y_ctr-11+PAD,   5,  5, 0, 360*64);
      XFillArc(dpy, d, black_gc, x_ctr,   y_ctr-7+PAD,    5,  5, 0, 360*64);
      XFillArc(dpy, d, black_gc, x_ctr-8, y_ctr-3+PAD,    5,  5, 0, 360*64);
      XFillArc(dpy, d, black_gc, x_ctr+2, y_ctr-1+PAD,    5,  5, 0, 360*64);
     }

    if (w->GasGiant) {
      XSetForeground(dpy, black_gc, color_list[CL_ICONS]);
      XFillArc(dpy, d, white_gc, x_ctr+26, y_ctr-30+PAD, 13, 13, 0, 360*64);
      XFillArc(dpy, d, black_gc, x_ctr+28, y_ctr-28+PAD, 9, 9, 0, 360*64);
     }

    XSetForeground(dpy, black_gc, color_list[CL_ICONS]);
    switch(w->Base[0]) {
      case 'A'  : XCopyArea(dpy, Naval1Pix, d, black_gc, 0, 0,
                        naval2_width, naval2_height, x_ctr-35, y_ctr-20+PAD);
                  XCopyArea(dpy, Scout1Pix, d, black_gc, 0, 0,
                        scout1_width, scout1_height, x_ctr-35, y_ctr-4+PAD);
                  break;
      case 'B'  : XCopyArea(dpy, Naval1Pix, d, black_gc, 0, 0,
                        naval2_width, naval2_height, x_ctr-35, y_ctr-20+PAD);
                  XCopyArea(dpy, Scout2Pix, d, black_gc, 0, 0,
                        scout2_width, scout2_height, x_ctr-35, y_ctr-4+PAD);
                  break;
      case 'C'  : XCopyArea(dpy, CorsairPix, d, black_gc, 0, 0,
                        corsair_width, corsair_height, x_ctr-35, y_ctr-4+PAD);
                  break;
      case 'D'  : XCopyArea(dpy, DepotPix, d, black_gc, 0, 0,
                        depot_width, depot_height, x_ctr-35, y_ctr-20+PAD);
                  break;
      case 'H'  : XCopyArea(dpy, CorsairPix, d, black_gc, 0, 0,
                        corsair_width, corsair_height, x_ctr-35, y_ctr-4+PAD);
      case 'F'  :
      case 'G'  :
      case 'J'  : XCopyArea(dpy, Naval2Pix, d, black_gc, 0, 0,
                        naval2_width, naval2_height, x_ctr-35, y_ctr-20+PAD);
                  break;
      case 'M'  : XCopyArea(dpy, MilPix, d, black_gc, 0, 0,
                        military_width, military_height, x_ctr-35, y_ctr-4+PAD);
                  break;
      case 'N'  : XCopyArea(dpy, Naval1Pix, d, black_gc, 0, 0,
                        naval1_width, naval1_height, x_ctr-35, y_ctr-20+PAD);
                  break;
      case 'R'  : XCopyArea(dpy, AslanPix, d, black_gc, 0, 0,
                        aslan_width, aslan_height, x_ctr-35, y_ctr-20+PAD);
                  break;
      case 'S'  : XCopyArea(dpy, Scout1Pix, d, black_gc, 0, 0,
                        scout1_width, scout1_height, x_ctr-35, y_ctr-20+PAD);
                  break;
      case 'T'  : XCopyArea(dpy, TlaukhuPix, d, black_gc, 0, 0,
                        tlaukhu_width, tlaukhu_height, x_ctr-35, y_ctr-4+PAD);
                  break;
      case 'W'  : XCopyArea(dpy, Scout2Pix, d, black_gc, 0, 0,
                        scout2_width, scout2_height, x_ctr-35, y_ctr-20+PAD);
                  break;
      case 'Z'  : XCopyArea(dpy, ZhodanePix, d, black_gc, 0, 0,
                        zhodane_width, zhodane_height, x_ctr-35, y_ctr-20+PAD);
      default   : break;
     }

    XSetForeground(dpy, black_gc, color_list[CL_FOREGROUND]);
    /* hex location number */
    len = XTextWidth(fptr, w->hex, 4); 
    XDrawImageString(dpy, d, black_gc, x_ctr-(len/2), y_ctr-36+PAD, w->hex, 4);

    XSetFont(dpy, black_gc, fBptr->fid);
    XDrawImageString(dpy, d, black_gc, x_ctr-4, y_ctr-18+PAD, w->Starport, 1);
    XSetFont(dpy, black_gc, fptr->fid);
    if (DISP_ALL) {
    	len = XTextWidth(fptr, w->allegiance, 2); 
    	XDrawImageString(dpy, d, black_gc, x_ctr-30-(len/2), y_ctr+18+PAD,
                w->allegiance, 2);
    }
    if (w->notes && DISP_TRADE) {
    	len = XTextWidth(fptr, w->notes, strlen(w->notes));
    	XDrawImageString(dpy, d, black_gc, x_ctr+25-(len/2), y_ctr+18+PAD,
		w->notes, strlen(w->notes));
    }
    if (strlen(w->name)) {
	if (w->uwp[3] >= '9') {
		XSetFont(dpy, black_gc, fBptr->fid);
    	        len = XTextWidth(fBptr, w->name, strlen(w->name)); 
    	    	XDrawImageString(dpy, d, black_gc, x_ctr-(len/2), y_ctr+36+PAD, 
				w->name, strlen(w->name));
		XSetFont(dpy, black_gc, fptr->fid);
	} else {
    	        len = XTextWidth(fptr, w->name, strlen(w->name)); 
    	    	XDrawImageString(dpy, d, black_gc, x_ctr-(len/2), y_ctr+36+PAD, 
				w->name, strlen(w->name));
	}
    }
    if (DISP_CODE) {
    	XSetFont(dpy, black_gc, fsptr->fid);
    	len = XTextWidth(fsptr, w->name, strlen(w->uwp)); 
    	XDrawImageString(dpy, d, black_gc, x_ctr-(len/2), y_ctr+46+PAD, 
				w->uwp, strlen(w->uwp));
    	XSetFont(dpy, black_gc, fptr->fid);
    }
   }
  XFlush(dpy);
}

load_sector_file()
{
  int done, i, atmosphere, hydrology, x_off, y_off;
  char str[10], *status, t_start[5], t_end[5], offset[5];
  World *w;
  FILE *fd;

  if (!strcmp(arginfo.datafile, "-")) {	/* "-" means stdin */
      fd = stdin;
      arginfo.datafile = "stdin";	/* For error messages */
  } else
      fd = fopen(arginfo.datafile, "r");

  if (fd == NULL) {
      fprintf(stderr, "%s: Cannot open %s for input\n",
	      arginfo.progname, arginfo.datafile);
      exit(1); }

  done = FALSE;
  w_cnt = 0;
  tr_cnt = 0;
  bdr_cnt = 0;
  private_bdr_cnt = 0;
  while ((w_cnt < 80) && (tr_cnt < 80) && !done) {
    status = fgets(line, 81, fd);
    if (status == NULL) done++;
    if (line[0] == '#')
      continue;
    if (line[0] == '@') {
      strcpy(title, &line[1]);
      continue;
    }
    if (line[0] == '^') {
      load_bdr_seg(line);
      continue;
    }
    if (line[0] == '$') {
/*--- get trade route segment ---*/
      strncpy(t_start, &line[1], 4);
      strncpy(t_end, &line[6], 4);
      strncpy(offset, &line[11], 4);
      t_start[4] = NULL;
      t_end[4] = NULL;
      offset[4] = NULL;
      strncpy(str, t_start, 2);
      str[2] = NULL;
      t_route[tr_cnt].x1 = (atoi(str) - 1) % 8;
      strncpy(str, &(t_start[2]), 2);
      str[2] = NULL;
      t_route[tr_cnt].y1 = (atoi(str) - 1) % 10;
      strncpy(str, t_end, 2);
      str[2] = NULL;
      t_route[tr_cnt].x2 = (atoi(str) - 1) % 8;
      strncpy(str, &(t_end[2]), 2);
      str[2] = NULL;
      t_route[tr_cnt].y2 = (atoi(str) - 1) % 10;

      strncpy(str, offset, 2);
      str[2] = NULL;
      x_off = atoi(str);
      if (x_off) {
        if (x_off < 0)
          t_route[tr_cnt].x2 -= 8;
        else
          t_route[tr_cnt].x2 += 8;
       }
      strncpy(str, &(offset[2]), 2);
      str[2] = NULL;
      y_off = atoi(str);
      if (y_off) {
        if (y_off < 0)
          t_route[tr_cnt].y2 -= 10;
        else
          t_route[tr_cnt].y2 += 10;
       }
      tr_cnt++;
      continue;
     }

    w = &sec_world[w_cnt];
/*--- get the world name ---*/
    i = 12;
    while ((line[i--] == ' ') && (i >= 0));
    if (i <= 0)
      w->name[0] = NULL;
    else
      strncpy(w->name, line, i+2);
      w->name[i+2] = NULL;

/*--- get world hex location string ---*/
    strncpy(w->hex, &line[14], 4);
    w->hex[4] = NULL;

/*--- get world Starport type ---*/
    w->Starport[0] = line[19];
    w->Starport[1] = NULL;

/*--- get uwp ---*/
    w->uwp[0] = line[20];
    w->uwp[1] = line[21];
    w->uwp[2] = line[22];
    w->uwp[3] = line[23];
    w->uwp[4] = line[24];
    w->uwp[5] = line[25];
    w->uwp[6] = line[26];
    w->uwp[7] = line[27];
    w->uwp[8] = NULL;

/*--- get world base (Naval, Scout, etc.) type ---*/
    w->Base[0] = line[30];
    w->Base[1] = NULL;

/*--- get world notes ---*/
    if ((w->notes[0] = line[32]) == ' ') w->notes[0] = NULL;
    else {
    	w->notes[1] = line[33];
    	if ((w->notes[2] = line[35]) == ' ') w->notes[2] = NULL;
    	else {
            w->notes[3] = line[36];
            if ((w->notes[4] = line[38]) == ' ') w->notes[4] = NULL;
	    else {
    	    	w->notes[5] = line[39];
    	    	if ((w->notes[6] = line[41]) == ' ') w->notes[6] = NULL;
	    	else {
    		    w->notes[7] = line[42];
    		    if ((w->notes[8] = line[44]) == ' ') w->notes[8] = NULL;
		    else {
    		    	w->notes[9] = line[45];
    		    	w->notes[10] = NULL;
		    }
	        }
	    }
        }
    }

/*--- get world zone type ---*/
    w->Zone[0] = line[48];
    w->Zone[1] = NULL;

/*--- get world allegiance ---*/
    strncpy(w->allegiance, &line[55], 2);
    w->allegiance[2] = NULL;

/*--- get no. of Gas Giants ---*/
    strncpy(str, &line[53], 1);
    str[1] = NULL;
    w->GasGiant = atoi(str);

/*--- convert hex string to digits ---*/
    strncpy(str, w->hex, 2);
    str[2] = NULL;
    w->location.x = (atoi(str) - 1) % 8;
    strncpy(str, &(w->hex[2]), 2);
    str[2] = NULL;
    sec_world[w_cnt].location.y = (atoi(str) - 1) % 10;

/*--- get WorldType (GARDEN/DESERT) ---*/
    str[0] = line[21];
    str[1] = NULL;
    switch (str[0]) {
        case '0' :
        case '1' :
        case '2' :
        case '3' : atmosphere = 0;
                   break;
        default  : atmosphere = 10;
                   break;
     }
    str[0] = line[22];
    str[1] = NULL;
    switch (str[0]) {
        case '0' : hydrology = 0;
                   break;
        default  : hydrology = 10;
     }
/********************************************
    if ((atmosphere < 4) || (hydrology == 0))
*********************************************/
    if (hydrology == 0)
      w->WorldType = DESERT;
    else
      w->WorldType = GARDEN;
    if (line[20] == '0')
      w->WorldType = ASTEROID;
    w_cnt++;
   }
  w_cnt--;
  fclose(fd);
  return (TRUE);
}

/*****************************************************************************
 *                                                                           *
 * Routine:  load_bdr_seg                                                    *
 *                                                                           *
 * Purpose:  This routine reads a static border element from the datafile    *
 *           and converts it into an Xsegment containing absolute values     *
 *           for those endpoints.  Each segment listed in the datefile has   *
 *           the following format (starting in column 0):                    *
 *                                                                           *
 *                 ^nnnn m                                                   *
 *                                                                           *
 *           Following the carat (^), the nnnn value is the hex number and   *
 *           the m value is the side of the hex.  Valid values for n are     *
 *           0-5, with the top edge of the hex as the 0 edge, and the other  *
 *           edges numbered clockwise.                                       *
 *                                                                           *
 *****************************************************************************/

load_bdr_seg(str)
char *str;
{
  int i, lx, ly, x_off, y_off, edge, next_edge;
  char bdr_hex[3], edge_char[2];

/*--- convert hex location strings & edge strings to digits ---*/
  bdr_hex[0] = str[1];
  bdr_hex[1] = str[2];
  bdr_hex[2] = NULL;
  lx = (atoi(bdr_hex) - 1) % 8;
  bdr_hex[0] = str[3];
  bdr_hex[1] = str[4];
  bdr_hex[2] = NULL;
  ly = (atoi(bdr_hex) - 1) % 10;
  edge_char[0] = str[6];
  edge_char[1] = NULL;
  edge = atoi(edge_char);
  next_edge = (edge + 1) % 6;
  i = private_bdr_cnt;
  x_off = hex_loc[lx].x;
  y_off = hex_loc[lx].y + (ly * LINE_INC) + PAD;
  file_bdr_seg[i].x1 = abs_hex_pts[edge].x + x_off;
  file_bdr_seg[i].y1 = abs_hex_pts[edge].y + y_off;
  file_bdr_seg[i].x2 = abs_hex_pts[next_edge].x + x_off;
  file_bdr_seg[i].y2 = abs_hex_pts[next_edge].y + y_off;
  private_bdr_cnt++;
}

print_sector_file()
{
  int i;

  fprintf(stdout, "Total trade-routes read = %d\n", tr_cnt);
  for (i=0; i<tr_cnt; i++) {
    fprintf(stdout, "Route: [%d,%d] - [%d,%d]\n",
                t_route[i].x1, t_route[i].y1, t_route[i].x2, t_route[i].y2);
   }

  fprintf(stdout, "Total worlds read = %d\n", w_cnt);
  for (i=0; i<w_cnt; i++) {
    fprintf(stdout, "World: \"%s\"              ", sec_world[i].name);
    fprintf(stdout, "[%d,%d]  ", sec_world[i].location.x,
                                        sec_world[i].location.y);
    fprintf(stdout, "STARPORT:%c ", sec_world[i].Starport[0]);
    if (sec_world[i].WorldType == DESERT)
      fprintf(stdout, "DESERT(%c%c)  ",
                sec_world[i].allegiance[0], sec_world[i].allegiance[1]);
    else if (sec_world[i].WorldType == GARDEN)
      fprintf(stdout, "GARDEN(%c%c)  ",
                sec_world[i].allegiance[0], sec_world[i].allegiance[1]);
    else
      fprintf(stdout, "ASTERIODS(%c%c)  ",
                sec_world[i].allegiance[0], sec_world[i].allegiance[1]);
    if (sec_world[i].GasGiant > 0)
      fprintf(stdout, "#GasGiants:%d\n", sec_world[i].GasGiant);
    else
      fprintf(stdout, "\n");
   }
}


repaint_buttons()
{
  int i, len, y_pos;
  GC  tgc;

  y_pos = (BTN_HEIGHT - fptr->ascent - fptr->descent) / 2 + fptr->ascent;

  for ( i=0 ; i<NUM_BTNS ; ++i) {
    len = XTextWidth(fptr, button_label[i], strlen(button_label[i]));
    if (button_state[i]) {
      tgc = neg_gc;
      XSetWindowBackground(dpy, button[i], color_list[CL_FOREGROUND]);
    }
    else {
      tgc = black_gc;
      XSetWindowBackground(dpy, button[i], color_list[CL_BACKGROUND]);
    }
    XClearWindow(dpy, button[i]);
    XDrawImageString(dpy, button[i], tgc, (BTN_WIDTH-len)/2, y_pos,
		     button_label[i], strlen(button_label[i]));
  } /* for (buttons) */
  XFlush(dpy);
}


mark_border()
{
  int pressed, x, y, old_x, old_y, done, first;

  button_state[0] = TRUE;
  repaint_buttons();

  cur_bdr_cnt = bdr_cnt;
  XSetForeground(dpy, black_gc, color_list[CL_BORDER]);
  XSetFillStyle(dpy, black_gc, (color_list[CL_BORDER] == black) ? FillTiled
	: FillSolid);
  XSetLineAttributes(dpy, black_gc, 5, LineSolid, CapButt, JoinMiter);
  pressed = FALSE;
  done = FALSE;
  first = TRUE;
  while(!done) {
    XNextEvent(dpy, &event);
    switch(event.type) {
      case ButtonPress    : if (!pressed) {
                              if (event.xbutton.window == win) {
                                pressed = TRUE;
                                x = ((event.xbutton.x+5) / 10) * 10;
                                y = ((event.xbutton.y+5) / 10) * 10 - 4;
                                old_x = x;
                                old_y = y;
                               }
                             }
                            else {
                            if (event.xbutton.window == win) {
                                XDrawLine(dpy, win, black_gc,
                                        old_x, old_y, x, y);
                                bdr_seg[bdr_cnt].x1 = old_x;
                                bdr_seg[bdr_cnt].y1 = old_y;
                                bdr_seg[bdr_cnt].x2 = x;
                                bdr_seg[bdr_cnt].y2 = y;
                                bdr_cnt++;
                                if (event.xbutton.button == Button3)
                                  done = TRUE;
                                x = ((event.xbutton.x+5) / 10) * 10;
                                y = ((event.xbutton.y+5) / 10) * 10 - 4;
                                old_x = x;
                                old_y = y;
                               }
                             }
                            break;
      case MotionNotify   : if (pressed) {
                              if (event.xbutton.window == win) {
                                if (!first) {
                                  XDrawLine(dpy, win, flicker_gc,
                                                old_x, old_y, x, y);
                                 }
                                x = ((event.xbutton.x+5) / 10) * 10;
                                y = ((event.xbutton.y+5) / 10) * 10 - 4;
                                XDrawLine(dpy, win, flicker_gc,
                                                old_x, old_y, x, y);
                                first = FALSE;
                               }
                             }
                            XFlush(dpy);
                            break;
     }
   }
  XSetLineAttributes(dpy, black_gc, 1, LineSolid, CapButt, JoinMiter);
  if (color_list[CL_BORDER] == black)
      XSetFillStyle(dpy, black_gc, FillSolid);
  XSetForeground(dpy, black_gc, color_list[CL_FOREGROUND]);
  button_state[0] = FALSE;
  repaint_buttons();
}


print_subsector()
{
  unsigned long swaptest = TRUE;
  XColor *colors;
  Pixmap PrintPix;
  unsigned buffer_size;
  int win_name_size;
  int header_size;
  int ncolors, i;
  char *win_name;
  XImage *ImagePix;
  XWindowAttributes win_info;
  FILE *out;

  IMGFileHeader header;
    
  button_state[2] = TRUE;
  repaint_buttons();

/*-- Get the parameters of the window being dumped --*/
  if(!XGetWindowAttributes(dpy, win, &win_info))
    return(FALSE);

  PrintPix = XCreatePixmap(dpy, win, 770, 1070+PAD, ScrDepth);
  if (PrintPix == NULL)
    return (FALSE);

  XFillRectangle(dpy, PrintPix, white_gc, 0, 0, 770, 1070+PAD);

  gen_sector(PrintPix);

  out = fopen("ssv.xwd", "w");
  win_name = "ssv.xwd";

/*-- sizeof(char) is included for the null string terminator. --*/
  win_name_size = strlen(win_name) + sizeof(char);

/*-- Snarf the pixmap with XGetImage --*/
  ImagePix = XGetImage(dpy, PrintPix, 0, 0, 770, 1080, AllPlanes, ZPixmap); 
  XSync(dpy, FALSE);

  if (ImagePix == NULL)
    return(FALSE);

/*-- Determine the pixmap size --*/
  buffer_size = ImagePix->bytes_per_line * ImagePix->height;

/*-- Get the RGB values for the current color cells --*/
  if ((ncolors = Get_Colors(&colors)) == 0)
    return(FALSE);

  XFlush(dpy);

/*-- Calculate header size --*/
  header_size = sizeof(header) + win_name_size;

/*-- Assemble the file header information --*/
/*-- First, write out the image info --*/
  header.header_size = (imgval) header_size;
  header.file_version = (imgval) XWD_FILE_VERSION;
  header.pixmap_format = (imgval) ZPixmap;
  header.pixmap_depth = (imgval) ImagePix->depth;
  header.pixmap_width = (imgval) ImagePix->width;
  header.pixmap_height = (imgval) ImagePix->height;
  header.xoffset = (imgval) ImagePix->xoffset;
  header.byte_order = (imgval) ImagePix->byte_order;
  header.bitmap_unit = (imgval) ImagePix->bitmap_unit;
  header.bitmap_bit_order = (imgval) ImagePix->bitmap_bit_order;
  header.bitmap_pad = (imgval) ImagePix->bitmap_pad;
  header.bits_per_pixel = (imgval) ImagePix->bits_per_pixel;
  header.bytes_per_line = (imgval) ImagePix->bytes_per_line;
  header.visual_class = (imgval) win_info.visual->class;
  header.red_mask = (imgval) win_info.visual->red_mask;
  header.green_mask = (imgval) win_info.visual->green_mask;
  header.blue_mask = (imgval) win_info.visual->blue_mask;
  header.bits_per_rgb = (imgval) win_info.visual->bits_per_rgb;
  header.colormap_entries = (imgval) win_info.visual->map_entries;
  header.ncolors = ncolors;
  header.window_width = (imgval) ImagePix->width;
  header.window_height = (imgval) ImagePix->height;
  header.window_x = (imgval) 0;
  header.window_y = (imgval) 0;
  header.window_bdrwidth = (imgval) 0;

  if (*(char *) &swaptest) {
    _swaplong((char *) &header, sizeof(header));
    for (i = 0; i < ncolors; i++) {
        _swaplong((char *) &colors[i].pixel, sizeof(long));
        _swapshort((char *) &colors[i].red, 3 * sizeof(short));
    }
  }

/*-- Write out the file header information --*/
  (void) fwrite((char *)&header, sizeof(header), 1, out);
  (void) fwrite(win_name, win_name_size, 1, out);

/*-- Write out the color cell RGB values --*/
  (void) fwrite((char *) colors, sizeof(XColor), ncolors, out);

/*-- Write out the buffer --*/
  (void) fwrite(ImagePix->data, (int) buffer_size, 1, out);

/*-- free the color buffer --*/
  if(ncolors > 0) free(colors);

/*-- Free image --*/
  XDestroyImage(ImagePix);
  XFreePixmap(dpy, PrintPix);

  fclose(out);
  button_state[2] = FALSE;
  repaint_buttons();

  return(TRUE);
}


/***************************************************************************
 *                                                                         *
 * Routine:   Get_Colors                                                   *
 *                                                                         *
 * Purpose:   Given a ptr. to an XColor struct., return the total number   *
 *            of cells in the current colormap, plus all of their RGB      *
 *            values.                                                      *
 *                                                                         *
 *X11***********************************************************************/

Get_Colors(colors)
XColor **colors;
{
  int i, ncolors;

  ncolors = DisplayCells(dpy, DefaultScreen(dpy));

  if ((*colors = (XColor *) malloc (sizeof(XColor) * ncolors)) == NULL)
    return(FALSE);

  for (i=0; i<ncolors; i++)
      (*colors)[i].pixel = i;

  XQueryColors(dpy, XDefaultColormap(dpy, XDefaultScreen(dpy)),
                *colors, ncolors);
  return(ncolors);
}

_swapshort (bp, n)
register char *bp;
register unsigned n;
{
  register char c;
/* register char *ep = bp + n; */
  register char *ep;

  ep = bp + n;
  while (bp < ep) {
    c = *bp;
    *bp = *(bp + 1);
    bp++;
    *bp++ = c;
  }
}

_swaplong (bp, n)
register char *bp;
register unsigned n;
{
  register char c;
/* register char *ep = bp + n; */
  register char *sp;
  register char *ep;

  ep = bp + n;
  while (bp < ep) {
    sp = bp + 3;
    c = *sp;
    *sp = *bp;
    *bp++ = c;
    sp = bp + 1;
    c = *sp;
    *sp = *bp;
    *bp++ = c;
    bp += 2;
  }
}

/*
 * Allocates the named color at index in color_list, using BlackPixel if
 *	name is null, or if an error occurs.
 */
void allocate_color(index, name, screen)
	int index;
	char *name;
	int screen;
{
    Colormap cmap;
    XColor current;
    int f;

    cmap = DefaultColormap(dpy, screen);
    if (name) {
	if (!(f = XParseColor(dpy, cmap, name, &current))) {
	    fprintf(stderr, "%s: %s - not in color database",
		    arginfo.progname);
	} else if (!(f = XAllocColor(dpy, cmap, &current))) {
	    fprintf(stderr, "%s: all colorcells allocated",
		    arginfo.progname);
	}
	if (f) color_list[index] = current.pixel;
	else {
	    fprintf(stderr, "...using black instead\n");
	    color_list[index] = black;
	}
    } else
	color_list[index] = black;
}

static void bad_flag();
static void no_parameter();

static int parse_args(argc, argv)
	int argc;
	char **argv;
{
    void bzero();
    int i;

    bzero(&arginfo, sizeof(arginfo));
    arginfo.progname = argv[0];
    if (argc < 2) return 0;	/* Required parameter not provided */

    for ( i = 1 ; i < argc ; ++i ) {

	if (argv[i][0] == '-'	/* It's a flag! */
	    && argv[i][1]) {	/* Unless it's specifically "-" (stdin) */
	    switch (argv[i][1]) {
	      case 'p':
		if (argv[i][2]) {
		    bad_flag(argv[i]);
		    return 0;
		}
		arginfo.print_only = TRUE;
		break;
	      case 'd':
		if (strncmp(argv[i], "-display", 8)) {
		    bad_flag(argv[i]);
		    return 0;
		}
		if (argv[i][8])
		    arginfo.display = &argv[i][8];
		else {
		    if (++i == argc) {
			no_parameter(argv[i-1]);
			return 0;
		    }
		    arginfo.display = argv[i];
		}
		break;
	      case 'f':		/* Parameter is a font specifier, or -fg */
		switch (argv[i][2]) {
		  case 'n':
		    if (argv[i][3]) arginfo.normal_font = &argv[i][3];
		    else {
			if (++i == argc) {
			    no_parameter(argv[i-1]);
			    return 0;
			}
			arginfo.normal_font = argv[i];
		    }
		    break;
		  case 's':
		    if (argv[i][3]) arginfo.small_font = &argv[i][3];
		    else {
			if (++i == argc) {
			    no_parameter(argv[i-1]);
			    return 0;
			}
			arginfo.small_font = argv[i];
		    }
		    break;
		  case 'b':
		    if (argv[i][3]) arginfo.bold_font = &argv[i][3];
		    else {
			if (++i == argc) {
			    no_parameter(argv[i-1]);
			    return 0;
			}
			arginfo.bold_font = argv[i];
		    }
		    break;
		  case 'g':
		    if (argv[i][3]) arginfo.foreground = &argv[i][3];
		    else {
			if (++i == argc) {
			    no_parameter(argv[i-1]);
			    return 0;
			}
			arginfo.foreground = argv[i];
		    }
		    break;
		  default:
		    bad_flag(argv[i]);
		    return 0;
		} /* switch (font) */
		break;
	      case 'b':
		switch (argv[i][2]) {
		  case 'w':
		    if (argv[i][3]) {
			bad_flag(argv[i]);
			return 0;
		    }
		    arginfo.black_white = 1;
		    break;
		  case 'g':
		    if (argv[i][3]) arginfo.background = &argv[i][3];
		    else {
			if (++i == argc) {
			    no_parameter(argv[i-1]);
			    return 0;
			}
			arginfo.background = argv[i];
		    }
		    break;
		  case 'c':
		    if (argv[i][3]) arginfo.border_color = &argv[i][3];
		    else {
			if (++i == argc) {
			    no_parameter(argv[i-1]);
			    return 0;
			}
			arginfo.border_color = argv[i];
		    }
		    break;
		  default:
		    bad_flag(argv[i]);
		    return 0;
		}
		break;
	      case 'h':
		if (strncmp(argv[i], "-hc", 3)) {
		    bad_flag(argv[i]);
		    return 0;
		}
		if (argv[i][3]) arginfo.hex_color = &argv[i][3];
		else {
		    if (++i == argc) {
			no_parameter(argv[i-1]);
			return 0;
		    }
		    arginfo.hex_color = argv[i];
		}
		break;
	      case 'i':
		if (strncmp(argv[i], "-ic", 3)) {
		    bad_flag(argv[i]);
		    return 0;
		}
		if (argv[i][3]) arginfo.icon_color = &argv[i][3];
		else {
		    if (++i == argc) {
			no_parameter(argv[i-1]);
			return 0;
		    }
		    arginfo.icon_color = argv[i];
		}
		break;
	      case 'x':
		if (strncmp(argv[i], "-xc", 3)) {
		    bad_flag(argv[i]);
		    return 0;
		}
		if (argv[i][3]) arginfo.xboat_color = &argv[i][3];
		else {
		    if (++i == argc) {
			no_parameter(argv[i-1]);
			return 0;
		    }
		    arginfo.xboat_color = argv[i];
		}
		break;
	      case 'a':
		if (strncmp(argv[i], "-ac", 3)) {
		    bad_flag(argv[i]);
		    return 0;
		}
		if (argv[i][3]) arginfo.amber_color = &argv[i][3];
		else {
		    if (++i == argc) {
			no_parameter(argv[i-1]);
			return 0;
		    }
		    arginfo.amber_color = argv[i];
		}
		break;
	      case 'r':
		if (strncmp(argv[i], "-rc", 3)) {
		    bad_flag(argv[i]);
		    return 0;
		}
		if (argv[i][3]) arginfo.red_color = &argv[i][3];
		else {
		    if (++i == argc) {
			no_parameter(argv[i-1]);
			return 0;
		    }
		    arginfo.red_color = argv[i];
		}
		break;
	      default:		/* Invalid flag */
		bad_flag(argv[i]);
		return 0;
	    }	/* switch (argv[i][1]) */
	} /* if (is a flag) */
	else {	/* It's a file! */
	    /* Perhaps allow multiple data files in the future. */
	    if (arginfo.datafile) {
		fprintf(stderr,
			"%s: No support for multiple data files (%s and %s)\n",
			arginfo.progname, arginfo.datafile, argv[i]);
		return 0;
	    }
	    arginfo.datafile = argv[i];
	} /* else (it's a file) */
    }	/* for ( i = 1 ; i < argc ; ++i ) */
    if (!arginfo.datafile)	/* Check for required parameters */
	return 0;
    return 1;
}

static void bad_flag(flag)
	char *flag;
{
    fprintf(stderr, "%s: Unrecognized flag '%s'\n",
	    arginfo.progname, flag);
}

static void no_parameter(flag)
	char *flag;
{
    fprintf(stderr, "%s: No parameter supplied for argument '%s'\n",
	    arginfo.progname, flag);
}

void usage()
{
    fprintf(stderr,
    "Usage: %s [-p] [-display X_display] [-fn normal_font] [-fb bold_font]\n",
	    arginfo.progname);
    fprintf(stderr,
	    "\t[-bw] [-fs small_font] [-bg color] [-fg color] [-xc color]\n");
    fprintf(stderr, "\t[-bc color] [-rc color] [-ac color] [-hc color]\n");
    fprintf(stderr, "\t[-ic color] subsector_file\n");
    fprintf(stderr,
	    "\n\t-display X_display\tspecifies an alternate display\n");
    fprintf(stderr,
	    "\t-fn normal_font\t\tspecifies the font for most info\n");
    fprintf(stderr,
	    "\t-fb bold_font\t\tspecifies the font for starport/high pop\n");
    fprintf(stderr,
	    "\t-fs small_font\t\tspecifies the font for the UPP\n");
    fprintf(stderr,
	    "\t-p\t\t\tindicates that a print file should be generated\n");
    fprintf(stderr,
	    "\t-bw\t\t\tindicates that black & white should be used\n");
    fprintf(stderr,
	    "\t-fg color\t\tspecifies the foreground color\n");
    fprintf(stderr,
	    "\t-bg color\t\tspecifies the background color\n");
    fprintf(stderr,
	    "\t-xc color\t\tspecifies the xboat route color\n");
    fprintf(stderr,
	    "\t-bc color\t\tspecifies the sector border color\n");
    fprintf(stderr,
	    "\t-rc color\t\tspecifies the color for red zones\n");
    fprintf(stderr,
	    "\t-ac color\t\tspecifies the color of amber zones\n");
    fprintf(stderr,
	    "\t-hc color\t\tspecifies the color for the hexes\n");
    fprintf(stderr,
	    "\t-ic color\t\tspecifies the color for icons\n");
    fprintf(stderr,
	    "\n\tsubsector_file\t\tspecifies the subsector data file\n");
    fprintf(stderr,
	    "\t\t(\"-\" for standard input)\n");
}
