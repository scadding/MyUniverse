


class cartPt{
    public:
        cartPt(): x(0.0), y(0.0), z(0.0) {};
        cartPt(double _x, double _y, double _z): x(_x), y(_y), z(_z) {};
        double x;
        double y;
        double z;
};


class mapPt{
    public:
        mapPt(double _x, double _y): x(_x), y(_y) {};
        double x;
        double y;
};

class tetraHedron{
    public:
        tetraHedron(cartPt &a, cartPt &b, cartPt &c, cartPt &d);
        cartPt &a() { return(*pt[order[0]]); };
        cartPt &b() { return(*pt[order[1]]); };
        cartPt &c() { return(*pt[order[2]]); };
        cartPt &d() { return(*pt[order[3]]); };
        int orderA() { return(order[0]); };
        int orderB() { return(order[1]); };
        int orderC() { return(order[2]); };
        int orderD() { return(order[3]); };
        double distanceAB() { return(distance(a(), b())); };
    private:
        double distance(cartPt &a, cartPt &b);
        cartPt *pt[4];
        int order[4];
};

double planet(double a, double b, double c, double d, double as, double bs, double cs, double ds,
              double ax, double ay, double az, double bx, double by, double bz, double cx, double cy,
              double cz, double dx, double dy, double dz,
              double x, double y, double z, int level);

double planet1(double x, double y, double z, int depth);

extern double ssa,ssb,ssc,ssd, ssas,ssbs,sscs,ssds,
       ssax,ssay,ssaz, ssbx,ssby,ssbz, sscx,sscy,sscz, ssdx,ssdy,ssdz;
extern double M;   /* initial altitude (slightly below sea level) */
extern int Depth; /* depth of subdivisions */
extern double r1,r2,r3,r4; /* seeds */
#define PI 3.14159265358979
extern double dd1;  /* weight for altitude difference */
extern double dd2; /* weight for distance */
extern double POW;  /* power for distance function */

double rand2(double p, double q); /* random number generator taking two seeds */
double log_2(double x);

