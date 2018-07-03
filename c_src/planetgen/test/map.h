
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


class map
{
public:
    map() : longitude(0), latitude(0),
        Width(800), Height(600),
        scale(1.0)
    {
        alt = new int*[Width];
        for(int i=0; i<Width; i++) {
            alt[i] = new int[Height];
        }
    };
    virtual cartPt convert(int, int j) = 0;
    void setLongitude(double l)
    {
        longitude = l;
    };
    void setLatitude(double l)
    {
        latitude = l;
    };
    void setHeight(int h)
    {
        Height = h;
    };
    void setWidth(int w)
    {
        Width = w;
    };
    void setScale(int s)
    {
        scale = s;
    };
    void run()
    {
        for(int i = 0; i < Width; i++)
            for(int j = 0; j < Height; j++)
                alt[i][j] = 0;
    };
protected:
    // longitude and latitude of center in degrees
    double longitude;
    double latitude;
    // Width and Height of map in pixels
    int Width;
    int Height;
    double scale;
    int **alt;
};

class mercator : public map
{
public:
    mercator();
    cartPt convert(int i, int j);
    sphericalPt sconvert(int i, int j);
private:
    double x,y,z;
    double cos2;
    int ii;
    int jj;
    int k;
};

class sinusoid : public map
{
public:
    sinusoid();
    cartPt convert(int i, int j);
private:

};


#define PI 3.14159265358979




