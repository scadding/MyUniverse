
#ifndef IMAGE_H
#define IMAGE_H

class PlanetImage
{
public:
    PlanetImage(int width, int height);
    unsigned short Color(int i, int j, int c = -1);
private:
    int _width;
    int _height;
    unsigned short **_col;
};

#endif
