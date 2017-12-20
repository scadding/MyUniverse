#include "sysgen.h"
#include "custom.h"
/*#define Z_HERC*/
#include <math.h>
#include <graphics.h>

mainworld      mw;
sistem syst;
void   peruse(long, long, long);
int    sysview(long, long, long);
void   viewlevel(body *);

void   main(argc, argv)
int    argc;
char   **argv;
{
    long    x, y, z, atol();
    char    c;

    if(argc > 1) {
        x = atol(argv[1]);
    } else {
        x = 10000L;
    }
    if(argc > 2) {
        y = atol(argv[2]);
    } else {
        y = 10000L;
    }
    if(argc > 3) {
        z = atol(argv[3]);
    } else {
        z = 10000L;
    }
    Initialise();
    peruse(x, y, z);
    closegraph();
}

int    MaxX, MaxY;

Initialise() {
    int     i, j;

#ifdef Z_HERC
    i = HERCMONO;
    j = HERCMONOHI;
    initgraph(&i, &j, "C:\TC\HERC.BGI");
    setgraphmode(CGAC0);
#else
    i = CGA;
    j = CGAHI;
    initgraph(&i, &j, "C:\TC\CGA.BGI");
    setgraphmode(CGAHI);
#endif
    setfillstyle(EMPTY_FILL, 0);
    MaxX = getmaxx();
    MaxY = getmaxy();
}

#define XX (MaxX*3/5)
#define A_X (MaxX*2/90)
#define B_X (-MaxX*3/90)
#define C_X 0

#define YY (9)
#define A_Y (MaxY/54)
#define B_Y (MaxY/54)
#define C_Y (MaxY*2/27)

void   abc2xy(a, b, c, x, y)
int    a, b, c;
int    *x, *y;
{
    *x = XX + a*A_X + b*B_X + c*C_X;
    *y = YY + a*A_Y + b*B_Y + c*C_Y;
}

void   draw(a, b, c)
long   a, b, c;
{
    int     A, B, C;
    int     x, y;

    for(A = 0; A < 10; A++) {
        for(B = 0; B < 10; B++)
            for(C = 0; C < 10; C++)
                if(exist(a + (long)A, b + (long)B, c + (long)C)) {
                    abc2xy(A, B, C, &x, &y);
                    putpixel(x, y, 1);
                }
    }
}

int    oneline(M, N, O, inbuf)
long   M, N, O;
char   inbuf[];
{
    if(mainwrld(&mw, M, N, O) == 0) {
        return(0);
    }
    sprintf(inbuf, "%c%1x%1x%1x%1x%1x%1x-%x %c%c%c %ld,%ld,%ld %s",
            mw.starport, mw.size,
            mw.atmos, mw.hydro, mw.pop, mw.gov, mw.law,
            mw.tech, (mw.bases&NAVAL_BASE)? 'N' : ' ',
            (mw.bases&SCOUT_BASE)? 'S' : ' ',
            (mw.bases&LOCAL_BASE)? 'M' : ' ',
            M, N, O, mw.name);
    return(1);
}

void   peruse(A, B, C)
long   A, B, C;
{
    int     x, y;
    int     a, b, c;
    long    M, N, O;
    char    ch, inbuf[80];

    draw(A, B, C);
    a = 0;
    b = 0;
    c = 0;
    for(;;) {
        M = A + (long)a;
        N = B + (long)b;
        O = C + (long)c;
        bar(0, 0, MaxX, 8);
        if(oneline(M, N, O, inbuf)) {
            outtextxy(1, 1, inbuf);
        }
        abc2xy(a, b, c, &x, &y);
        putpixel(x - 1, y - 1, 1);
        putpixel(x - 1, y + 1, 1);
        putpixel(x + 1, y - 1, 1);
        putpixel(x + 1, y + 1, 1);
        ch = getch();
        if(!ch) {
            ch = 0x80 + getch();
        }
        putpixel(x - 1, y - 1, 0);
        putpixel(x - 1, y + 1, 0);
        putpixel(x + 1, y - 1, 0);
        putpixel(x + 1, y + 1, 0);
        switch(ch) {
        case 0x1b:
            return;
        case 'x':
        case 0xd1:
            a = (a + 1)%10;
            break;
        case 'X':
        case 0xc7:
            a = (a + 9)%10;
            break;
        case 'y':
        case 0xcf:
            b = (b + 1)%10;
            break;
        case 'Y':
        case 0xc9:
            b = (b + 9)%10;
            break;
        case 'z':
        case 0xd0:
            c = (c + 1)%10;
            break;
        case 'Z':
        case 0xc8:
            c = (c + 9)%10;
            break;
        case '\r':
        case '\n':
            if(!sysview(M, N, O)) {
                break;
            }
            bar(0, 0, MaxX, MaxY);
            draw(A, B, C);
            break;
        }
    }
}

void   pworld(s, inbuf)
body   *s;
{
    switch(s->type) {
    case T_STAR:
        sprintf(inbuf, "Star %c%d %d       %s", s->upp.s.class,
                s->upp.s.decimal, s->upp.s.size, s->name);
        break;
    case T_GIANT:
        if(s->upp.g.size) {
            sprintf(inbuf, "Large Gas Giant %s", s->name);
        } else {
            sprintf(inbuf, "Small Gas Giant %s", s->name);
        }
        break;
    case T_TOIDS:
        if(s->p->type == T_GIANT) {
            sprintf(inbuf, "Ring            %s", s->name);
        } else {
            sprintf(inbuf, "Asteroids       %s", s->name);
        }
        break;
    case T_MAIN:
    case T_WORLD:
        sprintf(inbuf, "%c%1x%1x%1x%1x%1x%1x-%x %c%c%c  %c%s",
                s->upp.w.starport, s->upp.w.size,
                s->upp.w.atmos, s->upp.w.hydro, s->upp.w.pop,
                s->upp.w.gov, s->upp.w.law,
                s->upp.w.tech, (s->upp.w.bases&NAVAL_BASE)? 'N' : ' ',
                (s->upp.w.bases&SCOUT_BASE)? 'S' : ' ',
                (s->upp.w.bases&LOCAL_BASE)? 'M' : ' ',
                (s->type == T_MAIN) ? '*' : ' ',
                s->name);
        break;
    }
}

int    sysview(A, B, C)
long   A, B, C;
{
    if(makedetails(&syst, A, B, C) == 0) {
        return(0);
    }
    viewlevel(syst.sys);
    sys3free(&syst);
    return(1);
}

void   pdetail(s)
body   *s;
{
    int     i, top;
    city    *cptr;
    char    inbuf[80];
    char    tmpbuf[80];

    top = 1;
    switch(s->type) {
    case T_GIANT:
        sprintf(inbuf, "Diam: %gkm Dens:%g Grav:%g",
                s->det.g.diam, s->det.g.dens,
                s->det.g.dens*s->det.g.diam/12800.0);
        outtextxy(MaxX/2, top, inbuf);
        top += 10;
        sprintf(inbuf, "Year:%g Tilt:%d~",
                floor(s->det.g.year), s->det.g.tilt);
        if(s->det.w.ecnt > 0.004) {
            sprintf(tmpbuf, " Eccn:%g", s->det.g.ecnt);
            strcat(inbuf, tmpbuf);
        }
        outtextxy(MaxX/2, top, inbuf);
        top += 10;
        break;
    case T_TOIDS:
        sprintf(inbuf, "%s", bodsiz[s->det.b.pbod]);
        if(s->det.b.pbod != s->det.b.maxbod) {
            sprintf(tmpbuf, "/%s", bodsiz[s->det.b.maxbod]);
            strcat(inbuf, tmpbuf);
        }
        sprintf(tmpbuf, " n-%2d m-%2d c-%2d %2d.%02d AU",
                s->det.b.n, s->det.b.m, s->det.b.c,
                s->det.b.wid/100, s->det.b.wid%100);
        strcat(inbuf, tmpbuf);
        outtextxy(MaxX/2, top, inbuf);
        top += 10;
        sprintf(inbuf, "Year:%g", floor(s->det.b.year));
        outtextxy(MaxX/2, top, inbuf);
        top += 10;
        break;
    case T_MAIN:
    case T_WORLD:
        sprintf(inbuf, "Grav:%g Temp:%.1f ",
                s->det.w.dens*s->det.w.diam/12800.0,
                s->det.w.temp - 273.0);
        if(s->det.w.life == 'Y') {
            strcat(inbuf, " Life");
        }
        outtextxy(MaxX/2, top, inbuf);
        top += 10;
        sprintf(inbuf, "Year:%g Day:%g", floor(s->det.w.year),
                floor(s->det.w.day));
        if(s->det.w.tilt) {
            sprintf(tmpbuf, " Tilt:%d~", s->det.w.tilt);
            strcat(inbuf, tmpbuf);
        }
        if(s->det.w.ecnt > 0.004) {
            sprintf(tmpbuf, " Eccn:%g", s->det.w.ecnt);
            strcat(inbuf, tmpbuf);
        }
        if(s->det.w.stress) {
            sprintf(tmpbuf, " Stress:%d", s->det.w.stress);
            strcat(inbuf, tmpbuf);
        }
        outtextxy(MaxX/2, top, inbuf);
        top += 10;
        for(cptr = s->det.w.bc; cptr; cptr = cptr->next) {
            sprintf(inbuf, "%s %g", cptr->name, cptr->pop);
            outtextxy(MaxX/2, top, inbuf);
            top += 10;
        }
        if(s->det.w.tpop >= 10.0) {
            sprintf(inbuf, "Total Pop:%g ", s->det.w.tpop);
            i = s->det.w.pop_2 + s->det.w.pop_3 +
                s->det.w.pop_4 + (int)s->det.w.pop_5;
            if(i) {
                sprintf(tmpbuf, "ML:%d MS:%d SM:%d VS:%ld",
                        s->det.w.pop_2, s->det.w.pop_3,
                        s->det.w.pop_4, s->det.w.pop_5);
                strcat(inbuf, tmpbuf);
            }
            outtextxy(MaxX/2, top, inbuf);
            top += 10;
        }
        if(s->upp.w.pop >= 2) {
            sprintf(inbuf, "Progressiveness: %s %s",
                    patt[s->det.w.prog_att], pact[s->det.w.prog_att]);
            outtextxy(MaxX/2, top, inbuf);
            top += 10;
            sprintf(inbuf, "Aggressiveness: %s %s",
                    aatt[s->det.w.prog_att], aact[s->det.w.prog_att]);
            outtextxy(MaxX/2, top, inbuf);
            top += 10;
            sprintf(inbuf, "Extensiveness: %s %s",
                    eglo[s->det.w.prog_att], eint[s->det.w.prog_att]);
            outtextxy(MaxX/2, top, inbuf);
            top += 10;
            for(i = 0; (i < 6) && (s->det.w.customs[i][0] > = 0); i++) {
                sprintf(inbuf, soccust[s->det.w.customs[i][0]],
                        pracgroup[s->det.w.customs[i][1]],
                        pracgroup[s->det.w.customs[i][2]]);
                outtextxy(MaxX/2, top, inbuf);
                top += 10;
            }
            sprintf(inbuf, "Legal: %x-%x%x%x%x%x",
                    s->upp.w.law, s->det.w.lweap,
                    s->det.w.ltrade, s->det.w.lcrim,
                    s->det.w.lcivi, s->det.w.lfree);
            outtextxy(MaxX/2, top, inbuf);
            top += 10;
            sprintf(inbuf, "Tech:%x%x-%x%x%x%x%x-%x%x%x%x-%x%x",
                    s->upp.w.tech,
                    s->det.w.tlow, s->det.w.tenergy,
                    s->det.w.trobot, s->det.w.tcommo,
                    s->det.w.tmed, s->det.w.tenv,
                    s->det.w.tland, s->det.w.twater,
                    s->det.w.tair, s->det.w.tspace,
                    s->det.w.tpmil, s->det.w.thmil);
            outtextxy(MaxX/2, top, inbuf);
            top += 10;
        }
        switch(s->det.w.core) {
        case 'h':
            strcpy(inbuf, "Heavy core");
            break;
        case 'm':
            strcpy(inbuf, "Molten core");
            break;
        case 'r':
            strcpy(inbuf, "Rocky body");
            break;
        case 'i':
            strcpy(inbuf, "Icy body");
            break;
        }
        sprintf(tmpbuf, " Diam: %gkm Dens:%g", s->det.w.diam, s->det.w.dens);
        strcat(inbuf, tmpbuf);
        outtextxy(MaxX/2, top, inbuf);
        top += 10;
        break;
    }
}

void   viewlevel(sys)
body   *sys;
{
    body    *s, *sptr;
    int     at, i;
    char    ch;
    char    inbuf[80];

    at = 0;
    s = sys->b;
redraw:
    bar(0, 0, MaxX, MaxY);
    outtextxy(1, 1, "Parent:");
    pworld(sys, inbuf);
    outtextxy(20, 10, inbuf);
    i = 26;
    for(sptr = sys->b; sptr; sptr = sptr->o) {
        pworld(sptr, inbuf);
        if(sptr->b) {
            outtextxy(1, i, "@");
        }
        outtextxy(20, i, inbuf);
        i += 10;
    }
    for(;;) {
        outtextxy(10, 26 + at*10, ">");
        ch = getch();
        if(!ch) {
            ch = 0x80 + getch();
        }
        bar(10, 26 + at*10, 19, 35 + at*10);
        switch(ch) {
        case 'K':
        case 'k':
        case 0xc8:
            if(s->i) {
                s = s->i;
                at--;
            }
            break;
        case 'J':
        case 'j':
        case 0xd0:
            if(s->o) {
                s = s->o;
                at++;
            }
            break;
        case '?':
            bar(MaxX/2, 0, MaxX, MaxY);
            pdetail(s);
            break;
        case 'P':
            sprintf(inbuf, "detprint %ld %ld %ld %s > prn",
                    syst.x, syst.y, syst.z,
                    s->name);
            system(inbuf);
            break;
        case '\r':
        case '\n':
            if(!s->b) {
                break;
            }
            viewlevel(s);
            goto redraw;
        case 0x1b:
        case 0xcf:
            return;
        }
    }
}
