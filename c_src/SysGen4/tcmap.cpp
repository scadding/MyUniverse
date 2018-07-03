#include <stdio.h>
#include "sysgen.h"
#include <math.h>

sistem syst;
extern FILE    *out;
void   prtri();
void   pline();
void   phalf();

void   main(argc, argv)
int    argc;
char   **argv;
{
    long    x, y, z, atol();
    body    *s;
    int     t, depth, size;
    long    loc;
    int     ori;
    char    inbuf[20];
    int     plates[20][28];
    int     land[20][28];
    int     cov[20][28];

    if(argc != 5) {
        printf("Usage: prmap X Y Z planetName\n");
        exit(1);
    }
    x = atol(argv[1]);
    y = atol(argv[2]);
    z = atol(argv[3]);

    printf("Making %s...", argv[4]);
    if((s = makemap(x, y, z, argv[4], plates, land, cov)) == 0) {
        exit(1);
    }
    Initialise();
    for(;;) {
        printf("%s Cover:\n", s->name);
        printw(cov, 2);
        printf("Triangle ? ");
        gets(inbuf);
        if(inbuf[0] == 0) {
            break;
        }
        t = atoi(inbuf) - 1;
        ori = ((t/5)%2);
        printf("Specifier ? ");
        gets(inbuf);
        loc = 0L;
        for(depth = 0; depth < 16; depth++) {
            if(inbuf[depth] == 0) {
                break;
            }
            if((inbuf[depth] < '0') || (inbuf[depth] > '3')) {
                break;
            }
            if(inbuf[depth] == '0') {
                ori = !ori;
            }
            loc <<= 2;
            loc |= (inbuf[depth] - '0');
        }
        printf("Size: ");
        gets(inbuf);
        size = atoi(inbuf);
        out = fopen("PRN", "wb");
        prtri(&cov[t][0], loc, depth, size, ori);
        getch();
        bclear();
        if(out != stdout) {
            fclose(out);
            out = stdout;
        }
    }
}

void   prtri(buf, loc, depth, size, ori)
int    buf[28];
long   loc;
int    depth, size, ori;
{
    int     side;

    for(side = 1; size; size--) {
        side *= 2;
    }
    phalf(buf, 0, loc, depth, side, side, ori);
}

void   gettri(buf, xbuf, tloc, depth)
int    buf[28], xbuf[28];
long   tloc;
int    depth;
{
    int     d, i;
    long    seed;
    char    c;
    int     tbuf[28];

    seed = 0L;
    for(i = 0; i < 28; i++) {
        xbuf[i] = buf[i];
    }
    for(d = 0; d < depth; d++) {
        c = ((tloc>>(d*2))&3);
        expand(xbuf, tbuf, c, seed);
        seed |= (c<<(d*2));
        for(i = 0; i < 28; i++) {
            xbuf[i] = tbuf[i];
        }
    }
}

long   rot(loc, depth)
long   loc;
int    depth;
{
    int     val;

    if(depth < 0) {
        return(loc);
    }
    val = (int)(loc>>(depth*2))&3;
    loc &= ~(3L<<(depth*2));
    if(val == 2) {
        return(loc);
    }
    if(val == 0) {
        return(loc|(3L<<(depth*2)));
    }
    loc = rot(loc, depth - 1);
    if(val == 3) {
        return(loc|(1L<<(depth*2)));
    }
    return(loc|(2L<<(depth*2)));
}

void   phalf(buf, just, loc, depth, side, len, ori)
int    buf[28];
int    just, depth, side, len, ori;
long   loc;
{
    int     ll;

    if(side>1) {
        if(ori) {
            phalf(buf, just, loc|(2<<(depth*2)),
                  depth+1, side/2, len, ori);
            phalf(buf, just + 3*side, loc|(1<<(depth*2)),
                  depth+1, side/2, len - side/2, ori);
        } else {
            phalf(buf, just + 3*side, loc|(1<<(depth*2)),
                  depth+1, side/2, len - side/2, ori);
            phalf(buf, just, loc|(2<<(depth*2)),
                  depth+1, side/2, len, ori);
        }
        return;
    }
    /* print a strip */
    len = len*2 - 1;
    if(!just && ori) {
        pline(buf, loc, 6, just, len, depth);
    }
    for(ll = 0; ll < 6; ll++) {
        pline(buf, loc, (ori)?(5-ll):ll, just, len, depth);
    }
    if(!just && !ori) {
        pline(buf, loc, 6, just, len, depth);
    }
}

void   pline(buf, tloc, l, just, len, depth)
int    buf[28];
long   tloc;
int    l, just, len, depth;
{
    int     odd, s;
    int     i, ii;
    int     xbuf[28];

    tpad(just + 6 - l);
    odd = 1;
    for(s = 0; s < len; s++) {
        gettri(buf, xbuf, tloc, depth);
        if(odd) {
            if(l) {
                i = left[l - 1];
            } else {
                i = 0;
            }
            ii = l + 1;
            if(l == 6) {
                ii--;
            }
        } else {
            if(l != 6) {
                i = left[5 - l];
            } else {
                i = 0;
            }
            ii = 7 - l;
            i++;
            ii -= 2;
        }
        while(ii-- > 0) {
            tp2(xbuf[i++], 2);
        }
        tloc = rot(tloc, depth - 1);
        odd = !odd;
    }
    if(l == 6) {
        tp2(xbuf[i], 2);
    }
    tnl();
}
