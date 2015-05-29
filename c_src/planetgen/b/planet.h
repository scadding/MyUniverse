

int planetMain(int ac, char **av);


int min(int x, int y);
int max(int x, int y);
double fmin(double x, double y);
double fmax(double x, double y);
void readcolors(char* colorsname);
void makeoutline(int do_bw);
void readmap();
void smoothshades();
void mercator();
void peter();
void squarep();
void mollweide();
void sinusoid();
void stereo();
void orthographic();
void gnomonic();
void azimuth();
void conical();
void heightfield();
void search();
int planet0(double x, double y, double z, int i, int j);
double planet(double a, double b, double c, double d, double as, double bs, double cs, double ds,
              double ax, double ay, double az, double bx, double by, double bz, double cx, double cy,
              double cz, double dx, double dy, double dz,
              double x, double y, double z, int level);
double planet1(double x, double y, double z);
double rand2(double p, double q); /* random number generator taking two seeds */
void printppm(FILE *outfile); /* prints picture in PPM (portable pixel map) format */
void printppmBW(FILE *outfile); /* prints picture in b/w PPM format */
void printbmp(FILE *outfile); /* prints picture in BMP format */
void printbmpBW(FILE *outfile); /* prints picture in b/w BMP format */
char *nletters(int n, int c);
void printxpm(FILE *outfile); /* prints picture in XPM (X-windows pixel map) format */
void printxpmBW(FILE *outfile); /* prints picture in XPM (X-windows pixel map) format */
void printheights(FILE *outfile); /* prints heightfield */
double log_2(double x);
void print_error(const char *filename, const char *ext);
void doShade(int doshade);

