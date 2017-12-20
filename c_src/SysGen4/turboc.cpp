#include "sysgen.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <graphics.h>

char   *pathtodriver = "\turboc\cga.bgi";

int    GraphDriver;            /* The Graphics device driver           */
int    GraphMode;              /* The Graphics mode value              */
double AspectRatio;            /* Aspect ratio of a pixel on the screen*/
int    MaxX, MaxY;             /* The maximum resolution of the screen */
int    MaxColors;              /* The maximum # of colors available    */
int    ErrorCode;              /* Reports any graphics errors          */
struct palettetype palette;            /* Used to read palette info    */

/*                                                                     */
/*     INITIALIZE: Initializes the graphics system and reports         */
/*     any errors which occured.                                       */
/*                                                                     */

void Initialise(void)
{
    int xasp, yasp;                      /* Used to read the aspect ratio*/

    GraphDriver = DETECT;                        /* Request auto-detection
*/
    initgraph(&GraphDriver, &GraphMode, "");
    ErrorCode = graphresult();           /* Read result of initialization*/
    if(ErrorCode != grOk) {              /* Error occured during init    */
        printf(" Graphics System Error: %s\n", grapherrormsg(ErrorCode));
        exit(1);
    }

    setgraphmode(VGAHI);
    getpalette(&palette);                /* Read the palette from board  */
    MaxColors = getmaxcolor() + 1;       /* Read maximum number of colors*/

    MaxX = getmaxx();
    MaxY = getmaxy(); /* Read size of screen             */

    getaspectratio(&xasp, &yasp);        /* read the hardware aspect     */
    AspectRatio = (double)xasp / (double)yasp; /* Get correction factor  */
    clearviewport();

}

int    hpos = 0, vpos = 0;

void   bclear()
{
    hpos = 0;
    vpos = 0;
}

void   tpad(n)
{
    if(!hpos && !vpos) {
        bclear();
    }
    hpos += n;
}

void   tnl()
{
    if(!hpos && !vpos) {
        bclear();
    }
    hpos = 0;
    vpos++;
}

int    tccolor[C_WICE + 1] = { 1, 0, 11, 0, 8, 6, 14, 2, 10, 9, 15, 7 };

void   tp2(c, mode)
int    c, mode;
{
    int     bit1, bit2;

    if(!hpos && !vpos) {
        bclear();
    }
    putpixel(hpos++, vpos, tccolor[c]);
    putpixel(hpos++, vpos, tccolor[c]);
}
