
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "image.h"

extern int BLACK;
extern int WHITE;

extern int nocols;

extern int rtable[65536];
extern int gtable[65536];
extern int btable[65536];

extern int Width;
extern int Height;

extern unsigned short **col;

extern int **heights;

/* Character table for XPM output */

char letters[64] = {
    '@','$','.',',',':',';','-','+','=','#','*','&','A','B','C','D',
    'E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T',
    'U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j',
    'k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
};

extern int doshade;
extern unsigned short **shades;

PlanetImage::PlanetImage(int width, int height) :
    _width(width), _height(height)
{
    _col = (unsigned short**)calloc(_width,sizeof(unsigned short*));
    if (_col == 0) {
        fprintf(stderr, "Memory allocation failed.");
        exit(1);
    }
    for (int i=0; i<_width; i++) {
        _col[i] = (unsigned short*)calloc(_height,sizeof(unsigned short));
        if (_col[i] == 0) {
            fprintf(stderr,
                    "Memory allocation failed at %d out of %d cols\n",
                    i+1,_width);
            exit(1);
        }
    }
}

void printppm(FILE *outfile) /* prints picture in PPM (portable pixel map) format */
{
    int i,j,c,s;

    fprintf(outfile,"P6\n");
    fprintf(outfile,"#fractal planet image\n");
    fprintf(outfile,"%d %d 255\n",Width,Height);

    if (doshade) {
        for (j=0; j<Height; j++) {
            for (i=0; i<Width; i++) {
                s =shades[i][j];
                c = s*rtable[col[i][j]]/150;
                if (c>255) c=255;
                putc(c,outfile);
                c = s*gtable[col[i][j]]/150;
                if (c>255) c=255;
                putc(c,outfile);
                c = s*btable[col[i][j]]/150;
                if (c>255) c=255;
                putc(c,outfile);
            }
        }
    } else {
        for (j=0; j<Height; j++)
            for (i=0; i<Width; i++) {
                putc(rtable[col[i][j]],outfile);
                putc(gtable[col[i][j]],outfile);
                putc(btable[col[i][j]],outfile);
            }
    }
    fclose(outfile);
}

void printppmBW(FILE *outfile) /* prints picture in b/w PPM format */
{
    int i,j,c;

    fprintf(outfile,"P6\n");
    fprintf(outfile,"#fractal planet image\n");
    fprintf(outfile,"%d %d 1\n",Width,Height);

    for (j=0; j<Height; j++)
        for (i=0; i<Width; i++) {
            if (col[i][j] < WHITE)
                c=0;
            else c=1;
            putc(c,outfile);
            putc(c,outfile);
            putc(c,outfile);
        }
    fclose(outfile);
}

void printbmp(FILE *outfile) /* prints picture in BMP format */
{
    int i,j,c,s, W1;

    fprintf(outfile,"BM");

    W1 = (3*Width+3);
    W1 -= W1 % 4;
    s = 54+W1*Height; /* file size */
    putc(s&255,outfile);
    putc((s>>8)&255,outfile);
    putc((s>>16)&255,outfile);
    putc(s>>24,outfile);

    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(54,outfile); /* offset to data */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(40,outfile); /* size of infoheader */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(Width&255,outfile);
    putc((Width>>8)&255,outfile);
    putc((Width>>16)&255,outfile);
    putc(Width>>24,outfile);

    putc(Height&255,outfile);
    putc((Height>>8)&255,outfile);
    putc((Height>>16)&255,outfile);
    putc(Height>>24,outfile);

    putc(1,outfile);  /* no. of planes = 1 */
    putc(0,outfile);

    putc(24,outfile);  /* bpp */
    putc(0,outfile);

    putc(0,outfile); /* no compression */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(0,outfile); /* image size (unspecified) */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(0,outfile); /* h. pixels/m */
    putc(32,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(0,outfile); /* v. pixels/m */
    putc(32,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(0,outfile); /* colours used (unspecified) */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);


    putc(0,outfile); /* important colours (all) */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    if (doshade) {
        for (j=Height-1; j>=0; j--) {
            for (i=0; i<Width; i++) {
                s =shades[i][j];
                c = s*btable[col[i][j]]/150;
                if (c>255) c=255;
                putc(c,outfile);
                c = s*gtable[col[i][j]]/150;
                if (c>255) c=255;
                putc(c,outfile);
                c = s*rtable[col[i][j]]/150;
                if (c>255) c=255;
                putc(c,outfile);
            }
            for (i=3*Width; i<W1; i++) putc(0,outfile);
        }
    } else {
        for (j=Height-1; j>=0; j--) {
            for (i=0; i<Width; i++) {
                putc(btable[col[i][j]],outfile);
                putc(gtable[col[i][j]],outfile);
                putc(rtable[col[i][j]],outfile);
            }
            for (i=3*Width; i<W1; i++) putc(0,outfile);
        }
    }
    fclose(outfile);
}

void printbmpBW(FILE *outfile) /* prints picture in b/w BMP format */
{
    int i,j,c,s, W1;

    fprintf(outfile,"BM");

    W1 = (Width+31);
    W1 -= W1 % 32;
    s = 62+(W1*Height)/8; /* file size */
    putc(s&255,outfile);
    putc((s>>8)&255,outfile);
    putc((s>>16)&255,outfile);
    putc(s>>24,outfile);

    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(62,outfile); /* offset to data */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(40,outfile); /* size of infoheader */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(Width&255,outfile);
    putc((Width>>8)&255,outfile);
    putc((Width>>16)&255,outfile);
    putc(Width>>24,outfile);

    putc(Height&255,outfile);
    putc((Height>>8)&255,outfile);
    putc((Height>>16)&255,outfile);
    putc(Height>>24,outfile);

    putc(1,outfile);  /* no. of planes = 1 */
    putc(0,outfile);

    putc(1,outfile);  /* bpp */
    putc(0,outfile);

    putc(0,outfile); /* no compression */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(0,outfile); /* image size (unspecified) */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(0,outfile); /* h. pixels/m */
    putc(32,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(0,outfile); /* v. pixels/m */
    putc(32,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(2,outfile); /* colours used */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);


    putc(2,outfile); /* important colours (2) */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(0,outfile); /* colour 0 = black */
    putc(0,outfile);
    putc(0,outfile);
    putc(0,outfile);

    putc(255,outfile); /* colour 1 = white */
    putc(255,outfile);
    putc(255,outfile);
    putc(255,outfile);

    for (j=Height-1; j>=0; j--)
        for (i=0; i<W1; i+=8) {
            if (i<Width && col[i][j] >= WHITE)
                c=128;
            else c=0;
            if (i+1<Width && col[i+1][j] >= WHITE)
                c+=64;
            if (i+2<Width && col[i+2][j] >= WHITE)
                c+=32;
            if (i+3<Width && col[i+3][j] >= WHITE)
                c+=16;
            if (i+4<Width && col[i+4][j] >= WHITE)
                c+=8;
            if (i+5<Width && col[i+5][j] >= WHITE)
                c+=4;
            if (i+6<Width && col[i+6][j] >= WHITE)
                c+=2;
            if (i+7<Width && col[i+7][j] >= WHITE)
                c+=1;
            putc(c,outfile);
        }
    fclose(outfile);
}

char *nletters(int n, int c)
{
    int i;
    static char buffer[8];

    buffer[n] = '\0';

    for (i = n-1; i >= 0; i--) {
        buffer[i] = letters[c & 0x001F];
        c >>= 5;
    }

    return buffer;
}

void printxpm(FILE *outfile) /* prints picture in XPM (X-windows pixel map) format */
{
    int x,y,i,nbytes;

    x = nocols - 1;
    for (nbytes = 0; x != 0; nbytes++)
        x >>= 5;

    fprintf(outfile,"/* XPM */\n");
    fprintf(outfile,"static char *xpmdata[] = {\n");
    fprintf(outfile,"/* width height ncolors chars_per_pixel */\n");
    fprintf(outfile,"\"%d %d %d %d\",\n", Width, Height, nocols, nbytes);
    fprintf(outfile,"/* colors */\n");
    for (i = 0; i < nocols; i++)
        fprintf(outfile,"\"%s c #%2.2X%2.2X%2.2X\",\n",
                nletters(nbytes, i), rtable[i], gtable[i], btable[i]);

    fprintf(outfile,"/* pixels */\n");
    for (y = 0 ; y < Height; y++) {
        fprintf(outfile,"\"");
        for (x = 0; x < Width; x++)
            fprintf(outfile, "%s", nletters(nbytes, col[x][y]));
        fprintf(outfile,"\",\n");
    }
    fprintf(outfile,"};\n");

    fclose(outfile);
}

void printxpmBW(FILE *outfile) /* prints picture in XPM (X-windows pixel map) format */
{
    int x,y,nbytes;

    x = nocols - 1;
    nbytes = 1;

    fprintf(outfile,"/* XPM */\n");
    fprintf(outfile,"static char *xpmdata[] = {\n");
    fprintf(outfile,"/* width height ncolors chars_per_pixel */\n");
    fprintf(outfile,"\"%d %d %d %d\",\n", Width, Height, 2, nbytes);
    fprintf(outfile,"/* colors */\n");

    fprintf(outfile,"\". c #FFFFFF\",\n");
    fprintf(outfile,"\"X c #000000\",\n");

    fprintf(outfile,"/* pixels */\n");
    for (y = 0 ; y < Height; y++) {
        fprintf(outfile,"\"");
        for (x = 0; x < Width; x++)
            fprintf(outfile, "%s",
                    (col[x][y] < WHITE)
                    ? "X" : ".");
        fprintf(outfile,"\",\n");
    }
    fprintf(outfile,"};\n");

    fclose(outfile);
}

void printheights(FILE *outfile) /* prints heightfield */
{
    int i,j;

    for (j=0; j<Height; j++) {
        for (i=0; i<Width; i++)
            fprintf(outfile,"%d ",heights[i][j]);
        putc('\n',outfile);
    }
    fclose(outfile);
}
