/******************************************************************************
 **  Program:           ssv
 **
 **  Description:       X11-based previewer for Traveller Sub-Sector map data.
 **
 **  File:              ssv.c, containing the following subroutines/functions:
 **                       main()
 **                       gen_sector()
 **                       load_sector_file()
 **                       print_sector_file()
 **                       repaint_buttons()
 **                       mark_border()
 **                       print_subsector()
 **                       Get_Colors()
 **                       _swaplong()
 **                       _swapshort()
 **
 **  Copyright 1990 by Mark F. Cook and Hewlett-Packard,
 **                             Interface Technology Operation
 **  Modified 6/1990 by Dan Corrin (dan@engrg.uwo.ca)
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
/*
#define NORMAL_FONT "8x13"
#define BOLD_FONT   "8x13bold"
#define SMALL_FONT  "sr-6"
*/
#define NORMAL_FONT "8x13"
#define BOLD_FONT   "8x13bold"
#define SMALL_FONT  "6x9"
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

XSegment t_route[320], bdr_seg[160];
World sec_world[1280];
int button_state[7] = { FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE };

char line[128], ch, title[80];

static char map_name[] = {"SUB-SECTOR"};
static char panel_name[] = {"PANEL"};
static int DISP_ALL = 1;
static int DISP_TRADE = 1;
static int DISP_EXPL = 0;

#define NUM_HEX_PTS   7
#define NUM_HEXES     8
#define NUM_LINES    40
#define LINE_INC    30 /* distance between lines */

#define PAD          16
#define HEX_PAD       4

static XPoint hex_ctr[NUM_HEXES+8] = { /* no idea */
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
                {1060, 110} };

static XPoint hex_pts[NUM_HEX_PTS] = { /* relative stepping matrix for a hex */
                {  0,   0},
                { 18,   0},
                { 9,  15},
                {-9,  15},
                {-18,   0},
                {-9, -15},
                { 9, -15} };

static XPoint hex_loc[NUM_HEXES] = { /* starting location for each row */
                { 17, 14},
                { 45, 29},
                { 73, 14},
                {101, 29},
                {129, 14},
                {157, 29},
                {185, 14},
                {213, 29} };

Display      *dpy;
Window        win, panel, button[8];
GC             black_gc, white_gc, neg_gc, flicker_gc;
int         w_cnt, tr_cnt, bdr_cnt, cur_bdr_cnt=0, ScrDepth;
XFontStruct *fptr, *fBptr, *fsptr;
Pixmap solid, chex, Naval1Pix, Naval2Pix, Scout1Pix, Scout2Pix, DepotPix;
Pixmap AslanPix, CorsairPix, MilPix, TlaukhuPix, ZhodanePix;
unsigned long black, white;
XEvent    event;

main(argc,argv)
int argc;
char *argv[];
{
  KeySym      key;
  XSizeHints  xsh1, xsh2;
  XSetWindowAttributes win_attrib;
  unsigned long w_a_mask;
  int      screen, i, j, done;
  int load_sector_file(), print_sector_file();
  char   text[10];

  if (!load_sector_file(argc, argv)) {
      fprintf(stderr, "%s: Invalid datafile \"%s\"\n", argv[0], argv[1]);
      exit(1); }

  if ((dpy = XOpenDisplay(NULL)) == NULL) {
      fprintf(stderr, "%s: Cannot open %s\n", argv[0], XDisplayName(NULL));
      exit(1); }

  ScrDepth = XDefaultDepth(dpy, DefaultScreen(dpy));

  fptr = XLoadQueryFont(dpy, NORMAL_FONT);
  if (fptr == NULL) {
      fprintf(stderr, "%s: Cannot open font \"%s\"\n", argv[0],NORMAL_FONT);
      exit(1); }
  fBptr = XLoadQueryFont(dpy, BOLD_FONT);
  if (fBptr == NULL) {
      fprintf(stderr, "%s: Cannot open font \"%s\"\n", argv[0],BOLD_FONT);
      exit(1); }
  fsptr = XLoadQueryFont(dpy, SMALL_FONT);
  if (fsptr == NULL) {
      fprintf(stderr, "%s: Cannot open font \"%s\"\n", argv[0],SMALL_FONT);
      exit(1); }

  screen = DefaultScreen(dpy);
  black = BlackPixel(dpy, screen);    white = WhitePixel(dpy, screen);

  /*###*/
  xsh1.flags  = (PPosition | PSize);
  xsh1.height = 1255;    xsh1.width  = 925;
  xsh1.x      = 10;          xsh1.y      = 10;

  win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
                              xsh1.x, xsh1.y, xsh1.width,
                              xsh1.height, 1, black, white);

  XSetStandardProperties(dpy, win, map_name, map_name, None, argv, argc, &xsh1);

  xsh2.flags  = (PPosition | PSize);
  xsh2.height = 180;    xsh2.width  = 100;
  xsh2.x      = 10;     xsh2.y      = 10;

  panel = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
                              xsh2.x, xsh2.y, xsh2.width,
                              xsh2.height, 1, black, white);

  XSetStandardProperties(dpy, panel, panel_name, panel_name, None, argv,
                        argc, &xsh2);

  button[1] = XCreateSimpleWindow(dpy, panel, 5,  5, 90, 20, 1, black, white);
  button[2] = XCreateSimpleWindow(dpy, panel, 5, 30, 90, 20, 1, black, white);
  button[3] = XCreateSimpleWindow(dpy, panel, 5, 55, 90, 20, 1, black, white);
  button[4] = XCreateSimpleWindow(dpy, panel, 5, 80, 90, 20, 1, black, white);
  button[5] = XCreateSimpleWindow(dpy, panel, 5, 105, 90, 20, 1, black, white);
  button[6] = XCreateSimpleWindow(dpy, panel, 5, 130, 90, 20, 1, black, white);
  button[7] = XCreateSimpleWindow(dpy, panel, 5, 155, 90, 20, 1, black, white);

  black_gc   = XCreateGC(dpy, win, 0, 0);
  white_gc   = XCreateGC(dpy, win, 0, 0);
  neg_gc     = XCreateGC(dpy, win, 0, 0);
  flicker_gc = XCreateGC(dpy, win, 0, 0);

  XSetFont(dpy, black_gc, fptr->fid);
  XSetFont(dpy, white_gc, fptr->fid);
  XSetFont(dpy, neg_gc, fptr->fid);

  XSetForeground(dpy, black_gc, black);
  XSetBackground(dpy, black_gc, white);
  XSetForeground(dpy, white_gc, white);
  XSetBackground(dpy, white_gc, black);
  XSetForeground(dpy, flicker_gc, black);
  XSetBackground(dpy, flicker_gc, white);
  XSetForeground(dpy, neg_gc, white);
  XSetBackground(dpy, neg_gc, black);

  XSetFunction(dpy, flicker_gc, GXinvert);
  XSetLineAttributes(dpy, flicker_gc, 1, LineSolid, CapButt, JoinMiter);
  XSetPlaneMask(dpy, flicker_gc, 1);

  chex  = XCreatePixmapFromBitmapData(dpy, win, sm_chex_bits,
                sm_chex_width, sm_chex_height, white, black, ScrDepth);

  XSetTile(dpy, black_gc, chex);
  XSetTile(dpy, white_gc, chex);

  Naval1Pix = XCreatePixmapFromBitmapData(dpy, win, naval1_bits, naval1_width,
                naval1_height, black, white, ScrDepth);
  Naval2Pix = XCreatePixmapFromBitmapData(dpy, win, naval2_bits, naval2_width,
                naval2_height, black, white, ScrDepth);
  Scout1Pix = XCreatePixmapFromBitmapData(dpy, win, scout1_bits, scout1_width,
                scout1_height, black, white, ScrDepth);
  Scout2Pix = XCreatePixmapFromBitmapData(dpy, win, scout2_bits, scout2_width,
                scout2_height, black, white, ScrDepth);
  DepotPix  = XCreatePixmapFromBitmapData(dpy, win, depot_bits, depot_width,
                depot_height, black, white, ScrDepth);
  AslanPix  = XCreatePixmapFromBitmapData(dpy, win, aslan_bits, aslan_width,
                aslan_height, black, white, ScrDepth);
  CorsairPix = XCreatePixmapFromBitmapData(dpy, win, corsair_bits,
                corsair_width, corsair_height, black, white, ScrDepth);
  MilPix    = XCreatePixmapFromBitmapData(dpy, win, military_bits,
                military_width, military_height, black, white, ScrDepth);
  TlaukhuPix = XCreatePixmapFromBitmapData(dpy, win, tlaukhu_bits,
                tlaukhu_width, tlaukhu_height, black, white, ScrDepth);
  ZhodanePix = XCreatePixmapFromBitmapData(dpy, win, zhodane_bits,
                zhodane_width, zhodane_height, black, white, ScrDepth);

  XSelectInput(dpy, win, ButtonPressMask | PointerMotionMask |
                        KeyPressMask | ExposureMask);
  for (j=1;j<8;j++)
  	XSelectInput(dpy, button[j], ButtonPressMask | ExposureMask);
  XMapWindow(dpy, win);
  XMapSubwindows(dpy, panel);
  XMapWindow(dpy, panel);

  done = FALSE;
  while (!done) {
      XNextEvent(dpy, &event);
      switch (event.type) {
        case Expose:
              if (event.xexpose.count == 0) {
                if (event.xexpose.window == win)
                  gen_sector(win);
                if ((event.xexpose.window == button[1]) ||
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
              if (event.xbutton.window == button[1])
                mark_border();
              else if (event.xbutton.window == button[2]) {
                button_state[1] = TRUE;
                repaint_buttons();
                XClearWindow(dpy, win);
                bdr_cnt = cur_bdr_cnt;
                gen_sector(win);
                button_state[1] = FALSE;
                repaint_buttons();
               }
              else if (event.xbutton.window == button[3])
                print_subsector();
              else if (event.xbutton.window == button[4]) {
		DISP_ALL = 1-DISP_ALL;
		button_state[3] = DISP_ALL;
                XClearWindow(dpy, win);
                gen_sector(win);
                repaint_buttons();
		}
              else if (event.xbutton.window == button[5]) {
		DISP_TRADE = 1-DISP_TRADE;
		button_state[4] = DISP_TRADE;
                XClearWindow(dpy, win);
                gen_sector(win);
                repaint_buttons();
		}
              else if (event.xbutton.window == button[6]) {
		DISP_EXPL = 1-DISP_EXPL;
		button_state[5] = DISP_EXPL;
                XClearWindow(dpy, win);
                gen_sector(win);
                repaint_buttons();
		}
              else if (event.xbutton.window == button[7]) {
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

#define SECTOR_WIDTH 224

gen_sector(d)
Drawable d;
{
  int i, j, x_ctr, y_ctr, x, y, len, nsides;
  short x1, y1, x2, y2;
  World *w;

/*--- Step 1: generate the trade-routes within the grid ---*/
/*--- Step 2: generate the empty grid ---*/
  XSetLineAttributes(dpy, black_gc, 0, LineSolid, CapButt, JoinMiter);
  for (i=12; i<900; i+=SECTOR_WIDTH)
    for (j=30; j<1200; j+=300)
      XDrawRectangle(dpy, d, black_gc, i,j,SECTOR_WIDTH,300);

  XSetLineAttributes(dpy, black_gc, 3, LineSolid, CapButt, JoinMiter);
  XDrawRectangle(dpy, d, black_gc, 7, 11+PAD, 906, 1222); 
  XSetLineAttributes(dpy, black_gc, 1, LineSolid, CapButt, JoinMiter);

  for (i=0;i<NUM_LINES*LINE_INC; i+=LINE_INC) {
     for (j=0; j<NUM_HEXES; j++) {
	nsides = 5;
	hex_pts[0].x = hex_loc[j].x;
	hex_pts[0].y = hex_loc[j].y + i + PAD;
	if (j == 0) nsides = NUM_HEX_PTS;
	XDrawLines(dpy, d, black_gc, hex_pts, nsides, CoordModePrevious);
	hex_pts[0].x += SECTOR_WIDTH;
	XDrawLines(dpy, d, black_gc, hex_pts, nsides, CoordModePrevious);
	hex_pts[0].x += SECTOR_WIDTH;
	XDrawLines(dpy, d, black_gc, hex_pts, nsides, CoordModePrevious);
	hex_pts[0].x += SECTOR_WIDTH;
	XDrawLines(dpy, d, black_gc, hex_pts, nsides, CoordModePrevious);
     }
  }

  /*--- Print the sector/subsector title ---*/
  len = XTextWidth(fptr, title, strlen(title)-1); 
  XSetFont(dpy, black_gc, fBptr->fid);
  XDrawImageString(dpy, d, black_gc, (930-len)/2, 16, title, strlen(title)-1);
  XSetFont(dpy, black_gc, fsptr->fid);
  XFlush(dpy);

/*--- Step 3: if zone borders exist, generate them ---*/
/*--- Step 4: generate each system within the grid ---*/
  for (i=0; i<w_cnt; i++) {
    w = &sec_world[i];
    x = w->location.x;
    y = w->location.y;
    x_ctr = 28*x+26;
    y_ctr = 30*y+47; 
    if ((x & 1) == 1) y_ctr += 15;

    XFillArc(dpy, d, white_gc, x_ctr-4, y_ctr-4, 8, 8, 0, 360*64);
    if ((w->WorldType == GARDEN) || (DISP_EXPL))
      XFillArc(dpy, d, black_gc, x_ctr-3, y_ctr-3, 6, 6, 0, 360*64);
    else if (w->WorldType == DESERT)
      XDrawArc(dpy, d, black_gc, x_ctr-3, y_ctr-3, 6, 6, 0, 360*64);
    else {
      XFillArc(dpy, d, black_gc, x_ctr-4, y_ctr-2, 2, 2, 0, 360*64);
      XFillArc(dpy, d, black_gc, x_ctr-0, y_ctr+0, 2, 2, 0, 360*64);
      XFillArc(dpy, d, black_gc, x_ctr-3, y_ctr+2,   2, 2, 0, 360*64);
      XFillArc(dpy, d, black_gc, x_ctr+1, y_ctr+3, 2, 2, 0, 360*64);
     }

    len = XTextWidth(fsptr, w->hex, 4); 
    XDrawImageString(dpy, d, black_gc, x_ctr-(len/2), y_ctr-11, w->hex, 4);

    if (DISP_EXPL)
		continue;

    if (w->GasGiant) {
      XFillArc(dpy, d, white_gc, x_ctr+8, y_ctr-7, 4, 4, 0, 360*64);
      XFillArc(dpy, d, black_gc, x_ctr+7, y_ctr-6, 3, 3, 0, 360*64);
     }

/*###*/
    if (DISP_ALL) {
    	len = XTextWidth(fsptr, w->Starport, 1); 
    	XDrawImageString(dpy, d, black_gc, x_ctr-(len/2), y_ctr-5,
                w->Starport, 1);
    }
    if (strlen(w->name)) {
	if (w->uwp[3] >= '9') {
    	        len = XTextWidth(fsptr, w->name, strlen(w->name)); 
    	    	XDrawImageString(dpy, d, black_gc, x_ctr-(len/2), y_ctr+11, 
				w->name, strlen(w->name));
	} 
    }
   }
  XSetFont(dpy, black_gc, fptr->fid);
  XFlush(dpy);
}

load_sector_file(argc, argv)
int argc;
char *argv[];
{
  int done, count, i, atmosphere, hydrology, x_off, y_off;
  char str[10], ch, *status, t_start[5], t_end[5], offset[5];
  World *w;
  FILE *fd;

  if (argc != 2) {
      fprintf(stderr, "Usage: %s datafile \n", argv[0]);
      exit(1); }

  fd = fopen(argv[1], "r");
  if (fd == NULL) {
      fprintf(stderr, "%s: Cannot open %s for input\n", argv[0], argv[1]);
      exit(1); }

  w_cnt = 0;
  tr_cnt = 0;
  bdr_cnt = 0;
  done = FALSE;
  while ((w_cnt < 1280) && (tr_cnt < 320) && !done) {
    status = fgets(line, 81, fd);
    if (status == NULL) done++;
    if (line[0] == '#')
      continue;
    if (line[0] == '@') {
      strcpy(title, &line[1]);
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
    w->location.x = (atoi(str) - 1);
    strncpy(str, &(w->hex[2]), 2);
    str[2] = NULL;
    sec_world[w_cnt].location.y = (atoi(str) - 1);

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
  int len;
  GC  tgc;

  len = XTextWidth(fptr, "MARK BORDER", 11);
  if (button_state[0])
    tgc = neg_gc;
  else
    tgc = black_gc;
  XDrawImageString(dpy, button[1], tgc, (90-len)/2, 18, "MARK BORDER", 11);
  len = XTextWidth(fptr, "CLEAR BORDER", 12);
  if (button_state[1])
    tgc = neg_gc;
  else
    tgc = black_gc;
  XDrawImageString(dpy, button[2], tgc, (90-len)/2, 18, "CLEAR BORDER", 12);
  len = XTextWidth(fptr, "PRINT MAP", 9);
  if (button_state[2])
    tgc = neg_gc;
  else
    tgc = black_gc;
  XDrawImageString(dpy, button[3], tgc, (90-len)/2, 18, "PRINT MAP", 9);
  len = XTextWidth(fptr, "ALLEGANCE", 9);
  if (button_state[3])
    tgc = neg_gc;
  else
    tgc = black_gc;
  XDrawImageString(dpy, button[4], tgc, (90-len)/2, 18, "ALLEGANCE", 9);
  len = XTextWidth(fptr, "TRADE", 5);
  if (button_state[4])
    tgc = neg_gc;
  else
    tgc = black_gc;
  XDrawImageString(dpy, button[5], tgc, (90-len)/2, 18, "TRADE", 5);
  len = XTextWidth(fptr, "EXP MAP", 7);
  if (button_state[5])
    tgc = neg_gc;
  else
    tgc = black_gc;
  XDrawImageString(dpy, button[6], tgc, (90-len)/2, 18, "EXP MAP", 7);
  len = XTextWidth(fptr, "QUIT", 4);
  if (button_state[6])
    tgc = neg_gc;
  else
    tgc = black_gc;
  XDrawImageString(dpy, button[7], tgc, (90-len)/2, 18, "QUIT", 4);
  XFlush(dpy);
}


mark_border()
{
  int pressed, x, y, old_x, old_y, done, first;

  button_state[0] = TRUE;
  repaint_buttons();

  cur_bdr_cnt = bdr_cnt;
  XSetLineAttributes(dpy, black_gc, 5, LineSolid, CapButt, JoinMiter);
  XSetFillStyle(dpy, black_gc, FillTiled);
  pressed = FALSE;
  done = FALSE;
  first = TRUE;
  while(!done) {
    XNextEvent(dpy, &event);
    switch(event.type) {
      case ButtonPress    : if (!pressed) {
                              if (event.xbutton.window == win) {
                                pressed = TRUE;
                                x = ((event.xbutton.x+5) / 9) * 9;
                                y = ((event.xbutton.y+5) / 9) * 9 - 4;
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
                                x = ((event.xbutton.x+5) / 9) * 9;
                                y = ((event.xbutton.y+5) / 9) * 9 - 4;
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
                                x = ((event.xbutton.x+5) / 9) * 9;
                                y = ((event.xbutton.y+5) / 9) * 9 - 4;
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
  XSetFillStyle(dpy, black_gc, FillSolid);
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

  PrintPix = XCreatePixmap(dpy, win, 925, 1255+PAD, ScrDepth);
  if (PrintPix == NULL)
    return (FALSE);

  XFillRectangle(dpy, PrintPix, white_gc, 0, 0, 925, 1255+PAD);

  gen_sector(PrintPix);

  out = fopen("ssv.xwd", "w");
  win_name = "ssv.xwd";

/*-- sizeof(char) is included for the null string terminator. --*/
  win_name_size = strlen(win_name) + sizeof(char);

/*-- Snarf the pixmap with XGetImage --*/
  ImagePix = XGetImage(dpy, PrintPix, 0, 0, 925, 1255, AllPlanes, ZPixmap); 
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
