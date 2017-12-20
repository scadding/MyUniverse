


class cartPt
{
public:
    cartPt(): x(0.0), y(0.0), z(0.0) {};
    cartPt(double _x, double _y, double _z): x(_x), y(_y), z(_z) {};
    double x;
    double y;
    double z;
};


class sphericalPt
{
public:
    sphericalPt(): longitude(0.0), latitude(0.0), radius(0.0) {};
    sphericalPt(double _longitude, double _latitude, double _radius = 1.0):
        longitude(_longitude), latitude(_latitude), radius(_radius) {};
    double longitude;
    double latitude;
    double radius;
};


class mapPt
{
public:
    mapPt(double _x, double _y): x(_x), y(_y) {};
    double x;
    double y;
};

class planet
{
public:
    planet();
    double generate(double a, double b, double c, double d, double as, double bs, double cs, double ds,
                    double ax, double ay, double az, double bx, double by, double bz, double cx, double cy,
                    double cz, double dx, double dy, double dz,
                    double x, double y, double z, int level);
    double elevation(double x, double y, double z, int depth);
    double elevation(double longitude, double latitude, int depth);
    double rand2(double p, double q); /* random number generator taking two seeds */
    void seed(double s);
private:
    double M; /* initial altitude (slightly below sea level) */
    double r1,r2,r3,r4; /* seeds */
    /* these three values can be changed to change world characteristica */
    double dd1;  /* weight for altitude difference */
    double dd2; /* weight for distance */
    double POW;  /* power for distance function */
    double rseed;
};

#define PI 3.14159265358979



