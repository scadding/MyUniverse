#include <stdlib.h>
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

    setgraphmode(CGAC0);
    getpalette(&palette);                /* Read the palette from board  */
    MaxColors = getmaxcolor() + 1;       /* Read maximum number of colors*/

    MaxX = getmaxx();
    MaxY = getmaxy(); /* Read size of screen             */

    getaspectratio(&xasp, &yasp);        /* read the hardware aspect     */
    AspectRatio = (double)xasp / (double)yasp; /* Get correction factor  */

}

void   plottri(buf, len, ori, X, Y, scale)
int    *buf, len, ori, X, Y, scale;
{
    int     i, j;
    int     x, y;
    int     fs, col;

    for(i = 0; len; i++)
        for(j = 0; j <= i; j++) {
            if(*buf < 0) {  /* sea */
                fs = WIDE_DOT_FILL;
                col = -*buf;
            }
            else {
                fs = SOLID_FILL;
                col = *buf;
            }
            setfillstyle(fs, col);
            x = X + (j*2 - i)*scale;
            if(ori) {
                y = Y + i*scale;
            }
            else {
                y = Y - i*scale;
            }
            bar(x, y, x + 2*scale - 1, y + scale - 1);
            buf++;
            len--;
        }
}

void   Plottri(buf, scale)
int    buf[20][28];
int    scale;
{
    int     i;

    for(i = 0; i < 5; i++) {
        plottri(buf[i],    28, 1, (7  + 12*i)*scale,  0,       scale);
        plottri(buf[i+5],  28, 0, (7  + 12*i)*scale, 12*scale, scale);
        plottri(buf[i+10], 28, 1, (13 + 12*i)*scale,  6*scale, scale);
        plottri(buf[i+15], 28, 0, (13 + 12*i)*scale, 18*scale, scale);
    }
}
