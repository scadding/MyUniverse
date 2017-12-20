#include "sysgen.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

long   T;
int makedetails(sistem *syst, long x, long y, long z);
body *getpla(body *s, char *name);
void hex(body *s);
void mkmap(body *s);

int main(int argc, char **argv) {
    sistem  syst;
    body    *s;
    long    x, y, z;
    char    inbuf[40];

    if((argc < 3) || (argc > 5)) {
        printf("Usage: sysgen X Y Z [time]\n");
        exit(1);
    }
    x = atol(argv[1]);
    y = atol(argv[2]);
    z = atol(argv[3]);
    if(argc == 5) {
        T = atol(argv[4]);
    } else {
        time(&T);
    }

    if(makedetails(&syst, x, y, z) == 0) {
        exit(1);
    }
    printf("%s system (%ld,%ld,%ld)\n", syst.mw->name, x, y, z);
    for(;;) {
        printf("Planet:");
        fgets(inbuf,255,stdin);
        if(inbuf[strlen(inbuf)-1] == '\n') {
            inbuf[strlen(inbuf)-1] = '\0';
        }
        printf("%s\n", inbuf);
        if(inbuf[0] == 0) {
            break;
        }
        sRand(T);
        s = getpla(syst.sys, inbuf);
        if(!s) {
            printf("No such planet\n");
            continue;
        }
        hex(s);
    }
    exit(0);
}

body *getpla(body *s, char *name) {
    body    *sptr, *sret;

    Rand();
    if(name[0] == '?') {
        printf("%s\n", s->name);
        //return(getpla(s, s->name));
    }
    if(!strncmp(name, s->name, strlen(name))) {
        return(s);
    }
    for(sptr = s->b; sptr; sptr = sptr->o)
        if((sret = getpla(sptr, name)) != 0) {
            return(sret);
        }
    return(0);
}

int getgrav(body *s, int garr[10]) {
    float   d, r;
    int     i;

    switch(s->type) {
    case T_STAR:
        d = 1.0;
        break;
    case T_WORLD:
    case T_MAIN:
        d = s->det.w.dens;
        r = s->det.w.diam;
        break;
    case T_TOIDS:
        for(i = 0; i < 10; i++) {
            garr[i] = 0;
        }
        return(0);
    case T_GIANT:
        d = s->det.g.dens;
        r = s->det.g.diam;
        break;
    }
    r /= 8.0*1600.0;
    d = d*r*r*r;
    for(i = 1; i < 10; i++) {
        r = 8.0*sqrt(d/(float)i);
        garr[i] = (int)(1600.0*r/25000.0/2.0);
    }
    return(0);
}

int getdiam(body *s) {
    float   d;

    switch(s->type) {
    case T_STAR:
        d = 100000.0;
        break;
    case T_WORLD:
    case T_MAIN:
        d = s->det.w.diam;
        break;
    case T_TOIDS:
        return(0);
    case T_GIANT:
        d = s->det.g.diam;
        break;
    }
    return((int)(d/25000.0/2.0));
}

#define ml struct maploc
ml {
    body    *ref;
    int     x, y;
    int     diam, grav[10];
    ml      *next;
};

ml     *smap;
FILE   *out;

void hex(body *s) {
    int     i, j;
    int     wide, high;
    ml      *mptr;
    char    inbuf[20];

    mkmap(s);
    for(mptr = smap; mptr; mptr = mptr->next) {
        printf("%s: (%d,%d) page ", mptr->ref->name, mptr->x, mptr->y);
        i = mptr->x;
        if(i < 0) {
            i = -i;
        }
        i = i/10 + 1;
        j = mptr->y;
        if(j < 0) {
            j = -j;
        }
        j = j/16 + 1;
        printf("%d,%d\n", i, j);
    }
    printf("Pages wide: ");
    fgets(inbuf,255,stdin);
    wide = atoi(inbuf);
    if(!wide) {
        return;
    }
    printf("Pages long: ");
    fgets(inbuf,255,stdin);
    high = atoi(inbuf);
    if(!high) {
        return;
    }
    printf("File: ");
    fgets(inbuf,255,stdin);
    if(inbuf[strlen(inbuf)-1] == '\n') {
        inbuf[strlen(inbuf)-1] = '\0';
    }
    if(inbuf[0]) {
        out = fopen(inbuf, "w");
        if(out == NULL) {
            perror(inbuf);
            return;
        }
    } else {
        out = stdout;
    }
    j = high*16;
    i = -wide*10;
    while(wide) {
        prmap(i, -j, i + 20, j);
        i += 20;
        wide--;
    }
}

void mkmap(body *s) {
    body    *sptr;
    ml      *mptr;
    int     bdiam, r, o, i;
    double  ftmp;

    smap = (ml *)malloc(sizeof(ml));
    smap->ref = s;
    smap->x = 0;
    smap->y = 0;
    smap->next = 0;
    smap->diam = getdiam(s);
    bdiam = smap->diam;
    getgrav(s, smap->grav);
    for(sptr = s->b; sptr; sptr = sptr->o) {
        if(s->type == T_TOIDS) {
            continue;
        }
        mptr = (ml *)malloc(sizeof(ml));
        mptr->ref = sptr;
        mptr->x = 0;
        mptr->y = 0;
        mptr->next = smap;
        smap = mptr;
        mptr->diam = getdiam(sptr);
        getgrav(sptr, mptr->grav);
        r = bdiam*(sptr->orbit + 1);
        if(sptr->type == T_GIANT) {
            ftmp = sptr->det.g.year;
        } else if((sptr->type == T_WORLD) || (sptr->type == T_MAIN)) {
            ftmp = sptr->det.w.year;
        } else {
            ftmp = 100.0;
        }
        ftmp = (double)T/ftmp;
        ftmp -= floor(ftmp);
        o = (int)((double)r*6.0*ftmp);
        for(i = 0; i < r; i++) {
            move(&mptr->x, &mptr->y, 1);
        }
        /* 3 direction */
        if(o <= 0) {
            continue;
        }
        if(o <= r) {
            i = o;
        } else {
            i = r;
        }
        o -= i;
        while(i--) {
            move(&mptr->x, &mptr->y, 3);
        }
        /* 4 direction */
        if(o <= 0) {
            continue;
        }
        if(o <= r) {
            i = o;
        } else {
            i = r;
        }
        o -= i;
        while(i--) {
            move(&mptr->x, &mptr->y, 4);
        }
        /* 5 direction */
        if(o <= 0) {
            continue;
        }
        if(o <= r) {
            i = o;
        } else {
            i = r;
        }
        o -= i;
        while(i--) {
            move(&mptr->x, &mptr->y, 5);
        }
        /* 0 direction */
        if(o <= 0) {
            continue;
        }
        if(o <= r) {
            i = o;
        } else {
            i = r;
        }
        o -= i;
        while(i--) {
            move(&mptr->x, &mptr->y, 0);
        }
        /* 1 direction */
        if(o <= 0) {
            continue;
        }
        if(o <= r) {
            i = o;
        } else {
            i = r;
        }
        o -= i;
        while(i--) {
            move(&mptr->x, &mptr->y, 1);
        }
        /* 2 direction */
        if(o <= 0) {
            continue;
        }
        if(o <= r) {
            i = o;
        } else {
            i = r;
        }
        o -= i;
        while(i--) {
            move(&mptr->x, &mptr->y, 2);
        }
    }
}

/* hex movement

00  01  02  03     2\ /3
  11  12  13     1<-   ->4
20  21  22  23     0/ \5

*/

void move(int *x, int *y, int dir) {
    int     odd;

    odd = ((*y)%2);
    switch(dir) {
    case 1:
        (*x)--;
        break;
    case 4:
        (*x)++;
        break;
    case 2:
        (*y)--;
        if(!odd) {
            (*x)--;
        }
        break;
    case 3:
        (*y)--;
        if(odd) {
            (*x)++;
        }
        break;
    case 5:
        (*y)++;
        if(odd) {
            (*x)++;
        }
        break;
    case 0:
        (*y)++;
        if(!odd) {
            (*x)--;
        }
        break;
    }
}

int    moveto(int x, int y, int X, int Y) {    /* returns direction to from x,y to X,Y */
    int     dy;
    int     odd;

    if(Y < 0) {
        odd = ((-Y)%2);
    } else {
        odd = (Y%2);
    }
    dy = y - Y;
    if(dy < 0) {
        dy = -dy;
    }
    if(x <= X - (dy + 3 - odd)/2) {
        return(4);
    }
    if(x >= X + (dy + 2 + odd)/2) {
        return(1);
    }
    if(odd)
        if(y < 0) {
            odd = ((-y)%2);
        } else {
            odd = (y%2);
        } else {
        odd = 1;
    }
    if(y < Y) {
        if(x >= X + 1 - odd) {
            return(0);
        } else {
            return(5);
        }
    }
    if(x >= X + 1 - odd) {
        return(2);
    }
    return(3);
}

int dist(int x, int y, int X, int Y) {
    int     d;

    for(d = 0; (x != X) || (y != Y); d++) {
        move(&x, &y, moveto(x, y, X, Y));
    }
    return(d);
}

int mdist(ml *m, int x, int y) {
    int     d, i;

    d = dist(m->x, m->y, x, y);
    if(!d) {
        return(-1);
    }
    if(d <= m->diam) {
        return(0);
    }
    for(i = 1; i < 9; i++)
        if((m->grav[i] >= d) && (d >= m->grav[i+1])) {
            break;
        }
    if(i != 9) {
        return(i);
    } else {
        return(9999);
    }
}

void prmap(int left, int top, int right, int bot) {
    int     i, j, jj;
    ml      *mptr, *mtmp;
    int     d, dmax;
    char    inbuf[256];

    printf("Ready printer and press any key\n");
    getchar();
    for(i = top; i <= bot; i++) {
        jj = 0;
        for(j = left; j <= right; j++, jj += 6) {
            strcpy(inbuf + jj, "<   >-");
            mtmp = smap;
            dmax = mdist(mtmp, i, j);
            for(mptr = smap->next; mptr; mptr = mptr->next)
                if((d = mdist(mptr, i, j)) < dmax) {
                    dmax = d;
                    mtmp = mptr;
                }
            if(d >= 9999) {
                continue;
            }
            if(d < 0) {
                strcpy(inbuf + jj, mtmp->ref->name);
                continue;
            }
            if(d == 0) {
                inbuf[jj + 1] = '#';
                inbuf[jj + 2] = '#';
                inbuf[jj + 3] = '#';
                continue;
            }
            d = moveto(j, i, mtmp->y, mtmp->x);
            switch(d) {
            case 0:
                inbuf[jj + 1] = '/';
                inbuf[jj + 2] = '0' + dmax;
                break;
            case 1:
                inbuf[jj + 1] = '<';
                inbuf[jj + 2] = '0' + dmax;
                break;
            case 2:
                inbuf[jj + 1] = '\\';
                inbuf[jj + 2] = '0' + dmax;
                break;
            case 3:
                inbuf[jj + 2] = '0' + dmax;
                inbuf[jj + 3] = '/';
                break;
            case 4:
                inbuf[jj + 2] = '0' + dmax;
                inbuf[jj + 3] = '>';
                break;
            case 5:
                inbuf[jj + 2] = '0' + dmax;
                inbuf[jj + 3] = '\\';
                break;
            }
        }
        if(i%2) {
            j--;
            fprintf(out, " >-");
        } else {
            inbuf[jj++] = '<';
        }
        inbuf[jj] = 0;
        fprintf(out, "%s\n", inbuf);
    }
    fprintf(out, "\f");
}
