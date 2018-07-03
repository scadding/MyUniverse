#define ship struct SH
#define ARNELIA 0
#define GALATICA 1
#define DELGADO 2

ship {
    char    name[MAX_NAME];
    long    ox, oy, oz;
    int     tl, range, finance;
    int     high, low;
    int     fh, fm, fl;
    long    dx, dy, dz;
    long    leaving;
    ship    *next;
};
