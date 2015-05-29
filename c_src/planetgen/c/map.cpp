#include <math.h>
#include <stdio.h>

#include "planet.h"
#include "map.h"


cartPt mercator::convert(int i, int j)
{
    double theta;
    cartPt pt;
    if(jj == -1){
        y = sin(latitude);
        y = (1.0+y)/(1.0-y);
        y = 0.5*log(y);
        // y - equator (0)
        k = (int)(0.5*y*Width*scale/PI);
        // k - equator (0) scaled(map)
    }
    if(j != jj){
        y = PI*(2.0*(j-k)-Height)/Width/scale;
        y = exp(2.*y);
        y = (y-1.)/(y+1.);
        cos2 = sqrt(1.0-y*y);
    }
    if(i != ii){
        theta = longitude-0.5*PI+PI*(2.0*i-Width)/Width/scale;
        x = cos(theta)*cos2;
        z = -sin(theta)*cos2;
    }
    ii = i;
    jj = j;
    pt.x = x;
    pt.y = y;
    pt.z = z;
    return pt;
}

double mercator::Y(int j)
{
    if(jj == -1){
        y = sin(latitude);
        y = (1.0+y)/(1.0-y);
        y = 0.5*log(y);
        // y - equator (0)
        k = (int)(0.5*y*Width*scale/PI);
        // k - equator (0) scaled(map)
    }
    if(j != jj){
        y = PI*(2.0*(j-k)-Height)/Width/scale;
        y = exp(2.*y);
        y = (y-1.)/(y+1.);
        cos2 = sqrt(1.0-y*y);
    }
    return(y);
}

mercator::mercator() : map(),
  ii(-1), jj(-1)
{

}


sinusoid::sinusoid() : map()
{

}


cartPt sinusoid::convert(int i, int j)
{
    double x,y,z,y2,theta1,theta2,cos2,l1,i1,scale1;
    int k,l;
#if 0
    // lat - 0 latitude (in radians)
    k = (int)(lat*Width*scale/PI);
    for (j = 0; j < Height; j++) {
        y = (2.0*(j-k)-Height)/Width/scale*PI;
        if (fabs(y)>=0.5*PI) for (i = 0; i < Width ; i++) {
                col[i][j] = BACK;
                if (doshade>0) shades[i][j] = 255;
            }
        else {
            cos2 = cos(y);
            if (cos2>0.0) {
                scale1 = scale*Width/Height/cos2/PI;
                Depth = 3*((int)(log_2(scale1*Height)))+3;
                for (i = 0; i<Width; i++) {
                    l = i*12/Width;
                    l1 = l*Width/12.0;
                    i1 = i-l1;
                    theta2 = longi-0.5*PI+PI*(2.0*l1-Width)/Width/scale;
                    theta1 = (PI*(2.0*i1-Width/12)/Width/scale)/cos2;
                    if (fabs(theta1)>PI/12.0) {
                        col[i][j] = BACK;
                        if (doshade>0) shades[i][j] = 255;
                    } else {
                        x = cos(theta1+theta2)*cos2;
                        y2 = sin(y);
                        z = -sin(theta1+theta2)*cos2;
                        double alt = planet1(x,y2,z);
                        col[i][j] = alt2color(alt);
                        xxx[i][j] = x;
                        yyy[i][j] = y2;
                        zzz[i][j] = z;
                        if (doshade > 0) shades[i][j] = shade;
                    }
                }
            }
        }
    }
#endif
    return(cartPt(0.0, 0.0, 0.0));
}


