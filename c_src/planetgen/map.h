

class map
{
public:
    map() : longitude(0), latitude(0),
        Width(800), Height(600),
        scale(1.0) {};
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
protected:
    // longitude and latitude of center in degrees
    double longitude;
    double latitude;
    // Width and Height of map in pixels
    int Width;
    int Height;
    double scale;
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

