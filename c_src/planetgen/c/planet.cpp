/* planet.c */
/* planet generating program */
/* Copyright 1988--2009 Torben AE. Mogensen */

/* version of January 2009 */

/* The program generates planet maps based on recursive spatial subdivision */
/* of a tetrahedron containing the globe. The output is a colour PPM bitmap. */

/* The colours may optionally be modified according to latitude to move the */
/* icecaps lower closer to the poles, with a corresponding change in land colours. */

/* The Mercator map at magnification 1 is scaled to fit the Width */
/* it uses the full height (it could extend infinitely) */
/* The orthographic projections are scaled so the full view would use the */
/* full Height. Areas outside the globe are coloured black. */
/* Stereographic and gnomic projections use the same scale as orthographic */
/* in the center of the picture, but distorts scale away from the center. */

/* It is assumed that pixels are square */
/* I have included procedures to print the maps as bmp (Windows) or */
/* ppm(portable pixel map) bitmaps  on standard output or specified files. */

/* I have tried to avoid using machine specific features, so it should */
/* be easy to port the program to any machine. Beware, though that due */
/* to different precision on different machines, the same seed numbers */
/* can yield very different planets. */

/* The primitive user interface is primarily a result of portability concerns */

#ifdef THINK_C
#define macintosh 1
#endif

#ifdef macintosh
#include <console.h>
#include <unix.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "planet.h"


tetraHedron::tetraHedron(cartPt &a, cartPt &b, cartPt &c, cartPt &d)
{
    double l[6];
    l[0] = distance(a, b);
    l[1] = distance(a, c);
    l[2] = distance(a, d);
    l[3] = distance(b, c);
    l[4] = distance(b, d);
    l[5] = distance(c, d);

    int t = 0;
    for(int i = 1; i < 6; i++) {
        if(l[i] > l[t]) t = i;
    }
    pt[0] = &a;
    pt[1] = &b;
    pt[2] = &c;
    pt[3] = &d;
    switch(t) {
    case 0: {
        order[0] = 0;
        order[1] = 1;
        order[2] = 2;
        order[3] = 3;
        break;
    }
    case 1: {
        order[0] = 0;
        order[2] = 1;
        order[1] = 2;
        order[3] = 3;
        break;
    }
    case 2: {
        order[0] = 0;
        order[3] = 1;
        order[1] = 2;
        order[2] = 3;
        break;
    }
    case 3: {
        order[1] = 0;
        order[2] = 1;
        order[0] = 2;
        order[3] = 3;
        break;
    }
    case 4: {
        order[1] = 0;
        order[3] = 1;
        order[0] = 2;
        order[2] = 3;
        break;
    }
    default: {
        order[2] = 0;
        order[3] = 1;
        order[0] = 2;
        order[1] = 3;
        break;
    }
    };
}

double tetraHedron::distance(cartPt &a, cartPt &b)
{
    double abx = a.x-b.x;
    double aby = a.y-b.y;
    double abz = a.z-b.z;

    return(sqrt(abx*abx+aby*aby+abz*abz));
}

double planet1(double x, double y, double z, int depth)
{
    double retval = planet(M,M,M,M,
                           /* initial altitude is M on all corners of tetrahedron */
                           r1,r2,r3,r4,
                           /* same seed set is used in every call */
                           -sqrt(3.0)-0.20, -sqrt(3.0)-0.22, -sqrt(3.0)-0.23,
                           -sqrt(3.0)-0.19,  sqrt(3.0)+0.18,  sqrt(3.0)+0.17,
                           sqrt(3.0)+0.21, -sqrt(3.0)-0.24,  sqrt(3.0)+0.15,
                           sqrt(3.0)+0.24,  sqrt(3.0)+0.22, -sqrt(3.0)-0.25,
                           /* coordinates of vertices of tetrahedron*/
                           x,y,z,
                           /* coordinates of point we want colour of */
                           depth);
    /* subdivision depth */
    return(retval);
}

double planet(double a, double b, double c, double d, double as, double bs, double cs, double ds,
              double ax, double ay, double az, double bx, double by, double bz, double cx, double cy,
              double cz, double dx, double dy, double dz,
              double x, double y, double z, int level)
//double a,b,c,d;		      /* altitudes of the 4 verticess */
//double as,bs,cs,ds;	      /* seeds of the 4 verticess */
//double ax,ay,az, bx,by,bz,  /* vertex coordinates */
//       cx,cy,cz, dx,dy,dz;
//double x,y,z;		          /* goal point */
//int level;		          /* levels to go */
{
    double abx,aby,abz, acx,acy,acz, adx,ady,adz;
    double bcx,bcy,bcz, bdx,bdy,bdz, cdx,cdy,cdz;
    double lab, lac, lad, lbc, lbd, lcd;
    double ex, ey, ez, e, es, es1, es2, es3;
    double eax,eay,eaz, epx,epy,epz;
    double ecx,ecy,ecz, edx,edy,edz;

    if (level>0) {
        abx = ax-bx;
        aby = ay-by;
        abz = az-bz;
        acx = ax-cx;
        acy = ay-cy;
        acz = az-cz;
        lab = abx*abx+aby*aby+abz*abz;
        lac = acx*acx+acy*acy+acz*acz;

        /* reorder vertices so ab is longest edge */
        if (lab<lac)
            return(planet(a,c,b,d, as,cs,bs,ds,
                          ax,ay,az, cx,cy,cz, bx,by,bz, dx,dy,dz,
                          x,y,z, level));
        else {
            adx = ax-dx;
            ady = ay-dy;
            adz = az-dz;
            lad = adx*adx+ady*ady+adz*adz;
            if (lab<lad)
                return(planet(a,d,b,c, as,ds,bs,cs,
                              ax,ay,az, dx,dy,dz, bx,by,bz, cx,cy,cz,
                              x,y,z, level));
            else {
                bcx = bx-cx;
                bcy = by-cy;
                bcz = bz-cz;
                lbc = bcx*bcx+bcy*bcy+bcz*bcz;
                if (lab<lbc)
                    return(planet(b,c,a,d, bs,cs,as,ds,
                                  bx,by,bz, cx,cy,cz, ax,ay,az, dx,dy,dz,
                                  x,y,z, level));
                else {
                    bdx = bx-dx;
                    bdy = by-dy;
                    bdz = bz-dz;
                    lbd = bdx*bdx+bdy*bdy+bdz*bdz;
                    if (lab<lbd)
                        return(planet(b,d,a,c, bs,ds,as,cs,
                                      bx,by,bz, dx,dy,dz, ax,ay,az, cx,cy,cz,
                                      x,y,z, level));
                    else {
                        cdx = cx-dx;
                        cdy = cy-dy;
                        cdz = cz-dz;
                        lcd = cdx*cdx+cdy*cdy+cdz*cdz;
                        if (lab<lcd)
                            return(planet(c,d,a,b, cs,ds,as,bs,
                                          cx,cy,cz, dx,dy,dz, ax,ay,az, bx,by,bz,
                                          x,y,z, level));
                        else { /* ab is longest, so cut ab */
                            es = rand2(as,bs);
                            es1 = rand2(es,es);
                            es2 = 0.5+0.1*rand2(es1,es1);
                            es3 = 1.0-es2;
                            if (ax<bx) {
                                ex = es2*ax+es3*bx;
                                ey = es2*ay+es3*by;
                                ez = es2*az+es3*bz;
                            } else if (ax>bx) {
                                ex = es3*ax+es2*bx;
                                ey = es3*ay+es2*by;
                                ez = es3*az+es2*bz;
                            } else { /* ax==bx, very unlikely to ever happen */
                                ex = 0.5*ax+0.5*bx;
                                ey = 0.5*ay+0.5*by;
                                ez = 0.5*az+0.5*bz;
                            }
                            if (lab>1.0) lab = pow(lab,0.5);
                            /* decrease contribution for very long distances */

                            /* new altitude is: */
                            e = 0.5*(a+b) /* average of end points */
                                + es*dd1*fabs(a-b) /* plus contribution for altitude diff */
                                + es1*dd2*pow(lab,POW); /* plus contribution for distance */
                            eax = ax-ex;
                            eay = ay-ey;
                            eaz = az-ez;
                            epx =  x-ex;
                            epy =  y-ey;
                            epz =  z-ez;
                            ecx = cx-ex;
                            ecy = cy-ey;
                            ecz = cz-ez;
                            edx = dx-ex;
                            edy = dy-ey;
                            edz = dz-ez;
                            if ((eax*ecy*edz+eay*ecz*edx+eaz*ecx*edy
                                 -eaz*ecy*edx-eay*ecx*edz-eax*ecz*edy)*
                                (epx*ecy*edz+epy*ecz*edx+epz*ecx*edy
                                 -epz*ecy*edx-epy*ecx*edz-epx*ecz*edy)>0.0)
                                return(planet(c,d,a,e, cs,ds,as,es,
                                              cx,cy,cz, dx,dy,dz, ax,ay,az, ex,ey,ez,
                                              x,y,z, level-1));
                            else
                                return(planet(c,d,b,e, cs,ds,bs,es,
                                              cx,cy,cz, dx,dy,dz, bx,by,bz, ex,ey,ez,
                                              x,y,z, level-1));
                        }
                    }
                }
            }
        }
    }
    return((a+b+c+d)/4);
}

double planetj(double a, double b, double c, double d, double as, double bs, double cs, double ds,
               double ax, double ay, double az, double bx, double by, double bz, double cx, double cy,
               double cz, double dx, double dy, double dz,
               double x, double y, double z, int level)
//double a,b,c,d;		    /* altitudes of the 4 verticess */
//double as,bs,cs,ds;	    /* seeds of the 4 verticess */
//double ax,ay,az, bx,by,bz,  /* vertex coordinates */
//  cx,cy,cz, dx,dy,dz;
//double x,y,z;		    /* goal point */
//int level;		    /* levels to go */
{
    cartPt ptA(ax, ay, az);
    cartPt ptB(bx, by, bz);
    cartPt ptC(cx, cy, cz);
    cartPt ptD(dx, dy, dz);
    tetraHedron t(ptA, ptB, ptC, ptD);
    double altitude[4] = {a, b, c, d};
    double seed[4] = {as, bs, cs, ds};
    double ex, ey, ez, e, es, es1, es2, es3;
    double eax,eay,eaz, epx,epy,epz;
    double ecx,ecy,ecz, edx,edy,edz;

    double lab = t.distanceAB();

    as = seed[t.orderA()];
    bs = seed[t.orderB()];
    cs = seed[t.orderC()];
    ds = seed[t.orderD()];

    a = altitude[t.orderA()];
    b = altitude[t.orderB()];
    c = altitude[t.orderC()];
    d = altitude[t.orderD()];

    ax = t.a().x;
    ay = t.a().y;
    az = t.a().z;
    bx = t.b().x;
    by = t.b().y;
    bz = t.b().z;
    cx = t.c().x;
    cy = t.c().y;
    cz = t.c().z;
    dx = t.d().x;
    dy = t.d().y;
    dz = t.d().z;

    if (level<=0) {
        return((a+b+c+d)/4);
    }
    es = rand2(as,bs);
    es1 = rand2(es,es);
    es2 = 0.5+0.1*rand2(es1,es1);
    es3 = 1.0-es2;
    if (ax<bx) {
        ex = es2*ax+es3*bx;
        ey = es2*ay+es3*by;
        ez = es2*az+es3*bz;
    } else if (ax>bx) {
        ex = es3*ax+es2*bx;
        ey = es3*ay+es2*by;
        ez = es3*az+es2*bz;
    } else { /* ax==bx, very unlikely to ever happen */
        ex = 0.5*ax+0.5*bx;
        ey = 0.5*ay+0.5*by;
        ez = 0.5*az+0.5*bz;
    }
    if (lab>1.0) lab = pow(lab,0.5);
    /* decrease contribution for very long distances */

    /* new altitude is: */
    e = 0.5*(a+b) /* average of end points */
        + es*dd1*fabs(a-b) /* plus contribution for altitude diff */
        + es1*dd2*pow(lab,POW); /* plus contribution for distance */
    eax = ax-ex;
    eay = ay-ey;
    eaz = az-ez;
    epx =  x-ex;
    epy =  y-ey;
    epz =  z-ez;
    ecx = cx-ex;
    ecy = cy-ey;
    ecz = cz-ez;
    edx = dx-ex;
    edy = dy-ey;
    edz = dz-ez;
    if ((eax*ecy*edz+eay*ecz*edx+eaz*ecx*edy
         -eaz*ecy*edx-eay*ecx*edz-eax*ecz*edy)*
        (epx*ecy*edz+epy*ecz*edx+epz*ecx*edy
         -epz*ecy*edx-epy*ecx*edz-epx*ecz*edy)>0.0)
        return(planet(c,d,a,e, cs,ds,as,es,
                      cx,cy,cz, dx,dy,dz, ax,ay,az, ex,ey,ez,
                      x,y,z, level-1));
    else
        return(planet(c,d,b,e, cs,ds,bs,es,
                      cx,cy,cz, dx,dy,dz, bx,by,bz, ex,ey,ez,
                      x,y,z, level-1));
    return((a+b+c+d)/4);
}




