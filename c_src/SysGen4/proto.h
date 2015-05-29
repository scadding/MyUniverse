/* anyoption.cpp */
/* capital.cpp */
int main(int argc, char **argv);
/* cover.cpp */
int contigious(int buf[20][28], int val, int nval, int size);
int iscontig(int tbuf[20][28], int t, int h, int *size);
int setcontig(int tbuf[20][28], int t, int h, int val, int nval, int *size);
/* details.cpp */
/* detfun.cpp */
int roltilt(void);
float roleccn(void);
int cliphex(int val);
int Clip(int val, int h, int l);
int tlm(void);
/* epmap.cpp */
void prtri(int buf[28], long loc, int depth, int size, int ori);
void gettri(int buf[28], int xbuf[28], long tloc, int depth);
long rot(long loc, int depth);
void phalf(int buf[28], int just, long loc, int depth, int side, int len, int ori);
void pline(int buf[28], long tloc, int l, int just, int len, int depth);
/* epson.cpp */
void bclear(void);
void epad(int n);
void enl(void);
void ep2(int c, int mode);
/* exist.cpp */
int exist(long x, long y, long z);
/* fill.cpp */
int pm(int a);
void filltri(int buf[20][28]);
/* free.cpp */
/* g.cpp */
/* graph.cpp */
/* hexmap.cpp */
void move(int *x, int *y, int dir);
int moveto(int x, int y, int X, int Y);
int dist(int x, int y, int X, int Y);
int mdist(struct maploc *m, int x, int y);
void prmap(int left, int top, int right, int bot);
/* land.cpp */
void lands(int plates[20][28], int buf[20][28], int arch, int islands, int minor, int major, int landpc);
void place(int plates[20][28], int buf[20][28], int size, int val);
int countlhex(int buf[20][28], int plates[20][28], int t, int h, int cval, int pval);
/* lang.cpp */
void seedlang(long x, long y, long z);
void drift(void);
void scatter(int arr[], int max_arr, int slide);
int lookup(int arr[], int tot);
char *getname(void);
/* magic.cpp */
void xyzmagic(long x, long y, long z, long magic);
int D(int n);
void sRand(long seed);
unsigned int Rand(void);
/* mainwrld.cpp */
/* makemap.cpp */
/* map.cpp */
void set(int buf[20][28], int tri, int hex, int val);
int adj(int t, int h, int buf[6][2]);
int getrow(int t, int h);
/* mtns.cpp */
void mplace(int cover[20][28], int plates[20][28], int size);
/* origin.cpp */
void findnth(float ff);
void census(long x, long y, long z);
float fRand(void);
/* plandet.cpp */
void p1(int val, int type);
/* planpack.cpp */
void tab(int n);
void ttimes(void);
/* planprnt.cpp */
void Printw(int buf[20][28], int type);
void Printw2(int buf[20][28], int type);
void Printw3(int buf[20][28], int type);
/* plates.cpp */
void setcount(int vbuf[20][28], int cbuf[20][28], int t, int h, int val);
void plates(int buf[20][28], int num);
int find(int buf[20][28], int *t, int *h, int val);
int findfirst(int buf[20][28], int *t, int *h, int val);
int findnext(int buf[20][28], int *t, int *h, int val);
int counthex(int buf[20][28], int t, int h, int val);
/* populate.cpp */
/* prmap.cpp */
void prtri(int buf[28], long loc, int depth, int size, int ori);
void gettri(int buf[28], int xbuf[28], long tloc, int depth);
long rot(long loc, int depth);
void phalf(int buf[28], int just, long loc, int depth, int side, int len, int ori);
void pline(int buf[28], long tloc, int l, int just, int len, int depth);
/* scan.cpp */
int funct(long x, long y, long z);
/* scanner.cpp */
void drawview(long x, long y, long z, int axis);
/* sea.cpp */
void seas(int plates[20][28], int buf[20][28], int arch, int islands, int minor, int major, int landpc);
/* secprnt.cpp */
void prpopname(char *name, int pop);
/* stelfun.cpp */
/* sysgen1.cpp */
int sysgen1Main(long x, long y, long z);
/* sysgen2.cpp */
int sysgen2Main(long x, long y, long z);
/* sysgen3.cpp */
int sysgen3Main(long x, long y, long z);
void tab(int n);
/* sysgen4.cpp */
void showMap(long x, long y, long z, char *planetName);
void mview(int buf[20][28], int mode);
void tview(int tri[28], int depth, int triang, long dir, int mode, int ori);
/* sysgen.cpp */
void printSystem(long x, long y, long z, int detail);
/* system.cpp */
/* tcmap.cpp */
void prtri(int buf[28], long loc, int depth, int size, int ori);
void gettri(int buf[28], int xbuf[28], long tloc, int depth);
long rot(long loc, int depth);
void phalf(int buf[28], int just, long loc, int depth, int side, int len, int ori);
void pline(int buf[28], long tloc, int l, int just, int len, int depth);
/* text.cpp */
void pad(int n);
void p2(int val, int type);
void printw(int buf[20][28], int type);
void ptri(int tri[28], int mode, int ori);
/* timtbl.cpp */
void findvar(long *xx, long *yy, long *zz, int var);
int passmod(void);
int high(int mod);
int middle(int mod);
int low(int mod);
void doday(long tim, int sh);
/* turboc.cpp */
/* tview.cpp */
void ploc(int tri, int depth, long dir);
void expand(int from[28], int to[28], int dir, long seed);
