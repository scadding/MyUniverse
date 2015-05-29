
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
//#include "planet.h"

#define PI 3.14159265358979

#if 0

class Planet
{
    public:
        Planet();
        void mercator();
        double elevation(double x, double y, double z);
        double adjust(double a, double b, double c, double d, double as, double bs, double cs, double ds,
                      double ax, double ay, double az, double bx, double by, double bz, double cx, double cy,
                      double cz, double dx, double dy, double dz,
                      double x, double y, double z, int level);
    private:
        double lat;
        int Width;
        int Height;
        double scale;
        int debug;
        int Depth;
        double longi;
};

Planet::Planet() :
    lat(0.0), Width(800), Height(600),
    scale(1.0), debug(1), Depth(3*((int)(log_2(scale1*Height)))+3)
{
}

void Planet::mercator()
{
    double y,scale1,cos2,theta1;
    int i,j,k;

    y = sin(lat);
    y = (1.0+y)/(1.0-y);
    y = 0.5*log(y);
    k = (int)(0.5*y*Width*scale/PI);
    for (j = 0; j < Height; j++) {
        if (debug && ((j % (Height/25)) == 0)) {
            fprintf (stderr, "%d\n", j);
        }
        y = PI*(2.0*(j-k)-Height)/Width/scale;
        y = exp(2.*y);
        y = (y-1.)/(y+1.);
        scale1 = scale*Width/Height/sqrt(1.0-y*y)/PI;
        cos2 = sqrt(1.0-y*y);
        for (i = 0; i < Width ; i++) {
            theta1 = longi-0.5*PI+PI*(2.0*i-Width)/Width/scale;
            elevation(cos(theta1)*cos2,y,-sin(theta1)*cos2);
        }
    }
}

double Planet::elevation(double x, double y, double z)
{
    double abx,aby,abz, acx,acy,acz, adx,ady,adz, apx,apy,apz;
    double bax,bay,baz, bcx,bcy,bcz, bdx,bdy,bdz, bpx,bpy,bpz;

    abx = ssbx-ssax;
    aby = ssby-ssay;
    abz = ssbz-ssaz;
    acx = sscx-ssax;
    acy = sscy-ssay;
    acz = sscz-ssaz;
    adx = ssdx-ssax;
    ady = ssdy-ssay;
    adz = ssdz-ssaz;
    apx = x-ssax;
    apy = y-ssay;
    apz = z-ssaz;
    if ((adx*aby*acz+ady*abz*acx+adz*abx*acy
         -adz*aby*acx-ady*abx*acz-adx*abz*acy)*
        (apx*aby*acz+apy*abz*acx+apz*abx*acy
         -apz*aby*acx-apy*abx*acz-apx*abz*acy)>0.0) {
        /* p is on same side of abc as d */
        if ((acx*aby*adz+acy*abz*adx+acz*abx*ady
             -acz*aby*adx-acy*abx*adz-acx*abz*ady)*
            (apx*aby*adz+apy*abz*adx+apz*abx*ady
             -apz*aby*adx-apy*abx*adz-apx*abz*ady)>0.0) {
            /* p is on same side of abd as c */
            if ((abx*ady*acz+aby*adz*acx+abz*adx*acy
                 -abz*ady*acx-aby*adx*acz-abx*adz*acy)*
                (apx*ady*acz+apy*adz*acx+apz*adx*acy
                 -apz*ady*acx-apy*adx*acz-apx*adz*acy)>0.0) {
                /* p is on same side of acd as b */
                bax = -abx;
                bay = -aby;
                baz = -abz;
                bcx = sscx-ssbx;
                bcy = sscy-ssby;
                bcz = sscz-ssbz;
                bdx = ssdx-ssbx;
                bdy = ssdy-ssby;
                bdz = ssdz-ssbz;
                bpx = x-ssbx;
                bpy = y-ssby;
                bpz = z-ssbz;
                if ((bax*bcy*bdz+bay*bcz*bdx+baz*bcx*bdy
                     -baz*bcy*bdx-bay*bcx*bdz-bax*bcz*bdy)*
                    (bpx*bcy*bdz+bpy*bcz*bdx+bpz*bcx*bdy
                     -bpz*bcy*bdx-bpy*bcx*bdz-bpx*bcz*bdy)>0.0) {
                    /* p is on same side of bcd as a */
                    /* Hence, p is inside tetrahedron */
                    return(adjust(ssa,ssb,ssc,ssd, ssas,ssbs,sscs,ssds,
                                  ssax,ssay,ssaz, ssbx,ssby,ssbz,
                                  sscx,sscy,sscz, ssdx,ssdy,ssdz,
                                  x,y,z, 11));
                }
            }
        }
    } /* otherwise */
    double retval = adjust(M,M,M,M,
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
                           Depth);
    /* subdivision depth */
    return(retval);
}

double Planet::adjust(double a, double b, double c, double d, double as, double bs, double cs, double ds,
                      double ax, double ay, double az, double bx, double by, double bz, double cx, double cy,
                      double cz, double dx, double dy, double dz,
                      double x, double y, double z, int level)
{
//double a,b,c,d;		    /* altitudes of the 4 verticess */
//double as,bs,cs,ds;	    /* seeds of the 4 verticess */
//double ax,ay,az, bx,by,bz,  /* vertex coordinates */
//  cx,cy,cz, dx,dy,dz;
//double x,y,z;		    /* goal point */
//int level;		    /* levels to go */
    double abx,aby,abz, acx,acy,acz, adx,ady,adz;
    double bcx,bcy,bcz, bdx,bdy,bdz, cdx,cdy,cdz;
    double lab, lac, lad, lbc, lbd, lcd;
    double ex, ey, ez, e, es, es1, es2, es3;
    double eax,eay,eaz, epx,epy,epz;
    double ecx,ecy,ecz, edx,edy,edz;

    if (level>0) {
        if (level==11) {
            ssa=a;
            ssb=b;
            ssc=c;
            ssd=d;
            ssas=as;
            ssbs=bs;
            sscs=cs;
            ssds=ds;
            ssax=ax;
            ssay=ay;
            ssaz=az;
            ssbx=bx;
            ssby=by;
            ssbz=bz;
            sscx=cx;
            sscy=cy;
            sscz=cz;
            ssdx=dx;
            ssdy=dy;
            ssdz=dz;
        }
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
            return(adjust(a,c,b,d, as,cs,bs,ds,
                          ax,ay,az, cx,cy,cz, bx,by,bz, dx,dy,dz,
                          x,y,z, level));
        else {
            adx = ax-dx;
            ady = ay-dy;
            adz = az-dz;
            lad = adx*adx+ady*ady+adz*adz;
            if (lab<lad)
                return(adjust(a,d,b,c, as,ds,bs,cs,
                              ax,ay,az, dx,dy,dz, bx,by,bz, cx,cy,cz,
                              x,y,z, level));
            else {
                bcx = bx-cx;
                bcy = by-cy;
                bcz = bz-cz;
                lbc = bcx*bcx+bcy*bcy+bcz*bcz;
                if (lab<lbc)
                    return(adjust(b,c,a,d, bs,cs,as,ds,
                                  bx,by,bz, cx,cy,cz, ax,ay,az, dx,dy,dz,
                                  x,y,z, level));
                else {
                    bdx = bx-dx;
                    bdy = by-dy;
                    bdz = bz-dz;
                    lbd = bdx*bdx+bdy*bdy+bdz*bdz;
                    if (lab<lbd)
                        return(adjust(b,d,a,c, bs,ds,as,cs,
                                      bx,by,bz, dx,dy,dz, ax,ay,az, cx,cy,cz,
                                      x,y,z, level));
                    else {
                        cdx = cx-dx;
                        cdy = cy-dy;
                        cdz = cz-dz;
                        lcd = cdx*cdx+cdy*cdy+cdz*cdz;
                        if (lab<lcd)
                            return(adjust(c,d,a,b, cs,ds,as,bs,
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
                                return(adjust(c,d,a,e, cs,ds,as,es,
                                              cx,cy,cz, dx,dy,dz, ax,ay,az, ex,ey,ez,
                                              x,y,z, level-1));
                            else
                                return(adjust(c,d,b,e, cs,ds,bs,es,
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
#endif

int planetMain(int ac, char **av);

int main(int argc, char **argv)
{
#if 1
    return(planetMain(argc, argv));
#else
    Planet p;
    p.mercator();
    return(0);
#endif
}
