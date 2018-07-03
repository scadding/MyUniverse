
#include <stdio.h>

/* define REAL to be the native floating point type for the machine
 */
#ifdef mac
#define REAL extended
#else
#define REAL double
#endif

/* define the structure that we'll be using for the data items
 *	For a description of what the fields do/are see the top
 *	of the data table.
 */
struct tbl_ent {
    unsigned short key;
    unsigned short submenu;
    char override;
    char unique;
    char code;
    short techlevel;
    REAL power, volume, weight, price, mulpower, mulvolume;
    REAL mulweight, mulprice, waste, mulwaste;
    REAL hardpoints;
    REAL minVol;
    REAL cp;
    unsigned long cpmult;
#define NAMELEN	50
    char name[NAMELEN];
};

/* prototype the functions that are exported by the program
 */
extern void tbl_input();
extern struct tbl_ent *tbl_get();
extern struct tbl_ent *item_get();

