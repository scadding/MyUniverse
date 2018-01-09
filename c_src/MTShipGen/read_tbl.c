
#include <ctype.h>
#include <math.h>
#include "datatbl.h"

extern char *malloc();
extern char *realloc();
extern double atof();

static struct tbl_ent *theTable = NULL;
static long tblLength = 0;

/* This routine will increase the size of theTable by one entry.
 *	It will allocate larger blocks of memory than necessary and when
 *	the previous block is full, it will attempt to increase the available
 *	store.  This may result in some wasted space after the entire data
 *	file has been read in, but should improve throughput quite a lot.
 *	After the size of the data file is known this routine could be
 *	tuned so as to maximise allocation size.  It will return a
 *	pointer to the entry that was just added (i.e. theTable[tblLength]).
 */
static struct tbl_ent *
inc_tbl()
{
    static long maxSize=0;	/* this is how big the allocated table really is */
#define ALLOC_SIZE 100
#define INIT_ALLOC 2000
    if(!theTable) {	/* haven't allocated any memory yet... */
        theTable = (struct tbl_ent *)malloc(INIT_ALLOC*sizeof(struct tbl_ent));
        maxSize = INIT_ALLOC;
    }
    if(tblLength == maxSize) {
        maxSize += ALLOC_SIZE;
        theTable = (struct tbl_ent *)
                   realloc(theTable,maxSize*sizeof(struct tbl_ent));
        if(!theTable) {	/* oops, it failed... */
            fprintf(stderr, "Fatal error reading data: out of memory\n");
            exit(1);
        }
    }
    return &(theTable[tblLength++]);
#undef ALLOC_SIZE
#undef INIT_ALLOC
}

static void
default_tbl(t)
struct tbl_ent *t;
{
    short i;
    t->key = 0x0000;
    t->override = ' ';
    t->unique = '0';
    t->code = '\0';
    t->techlevel = 0;
    t->submenu = 0x0000;
    t->power = t->volume = t->weight = t->price = t->waste = 0.0;
    /* not sure if the next line should be 1.0 or 0.0, I think it will be 0.0
     *	but this is subject to change!
     */
    t->mulpower =t->mulvolume =t->mulweight =t->mulprice =t->mulwaste =0.0;
    t->hardpoints = 0;
    t->minVol = 0.0;
    t->cp = 0.0;
    t->cpmult = 1.0;
    for(i=0; i<NAMELEN; i++)
        t->name[i] = '\0';
}

/* Convert a string into a real (may have internal '/' sign)
 *	and store result into first arg if no leading * and into
 *	second if leading *.  Of course, if the second pointer is NULL
 *	put the result into the first arg.  This means that this routine
 *	can be used for the normal real case and for the multiplicative/additive
 *	pair.
 */
static void
add_mul(s, f, fmul)
char *s;
REAL *f, *fmul;
{
    short i, len;
    short flag;
    REAL num, den = 1.0;
    if(*s == '*') {	/* multiplicative entry */
        flag = 1;
        s++;
    } else
        flag = 0;	/* additive entry */

    len = strlen(s);
    for(i=0; i<len; i++)
        if(s[i] == '/') {
            s[i] = '\0';
            den = atof(&(s[i+1]));
            break;
        }
    num = atof(s) / den;
    /* try to counteract rounding errors, make anything really close to
     *	zero exactly zero.  Should probably make things very close
     *	to integral, exactly integral instead.  Maybe try:
     *
     *		if(ceil(num) - num < 1e-8) num = ceil(num);
     *		else if(num - floor(num) < 1e-8) num = floor(num);
     */
    if(fabs(num) < 1e-8) num = 0;

    if(flag && fmul != NULL) *fmul = num;
    else *f = num;
}

/* Define a pseudo function that skips spaces from the pointer.
 *	Adjust the pointer so that it points to the first non-space
 *	character in the string.
 */
#define skspc(s) while(isspace(*s)) s++

/* This macros grabs a field off the front of the string and advances the
 *	pointer to the start of the next field.  It handles the special case
 *	of a leading '%' in the field, which forces a skip to the name entry
 *	code.  If also handles the '$' default value for the field.  It should
 *	be followed by ONE (and only one) statement that performs the actualy
 *	extraction of the value [ this statement CAN be a {...} block ].
 *	p is changed to point just after the field and q is set to point at
 *	the start of the field extracted.
 */
#define get_field(p, q)											\
	skspc(p);													\
	if(*p == '%') goto getname;									\
	q = p;														\
	while(!isspace(*p)) p++; /* skip till space */				\
	*p++ = '\0';	/* null terminate it */						\
	if(*q != '$')	/* if not default allow next stmt */

/* This routine tries to read a four character hex field from the pased
 *	pointer.  It will skip any leading spaces before attempting to parse
 *	the hex field.  It returns the value of this field and updates the
 *	pointer so it points past the end of the field.
 */
static unsigned short
get_hex(pp, lineNo)
char **pp;
int lineNo;
{
    short i;
    unsigned short val = 0;
    char *p = *pp;
    skspc(p);
    for(i=0; i<4; i++,p++) {
        val <<= 4;
        switch(*p) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            val |= (*p - '0');
            break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            val |= (*p - 'a') + 10;
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            val |= (*p - 'A') + 10;
            break;
        default:
            fprintf(stderr,"Illegal character in hex field line %d\n", lineNo);
            exit(1);
            break;
        }
    }
    skspc(p);	/* skip till the following field */
    *pp = p;	/* return the resulting pointer */
    return val;
}

/* This beasty will process one (null-terminated, no \n) line of input
 *	that is passed in p.  It also requires the current line of the file
 *	to be passed in, since it is necessary for error messages.  The tbl
 *	parameter is a pointer to a table entry that is to be filled.
 */
static void
parse(p, tbl, lineNo)
char *p;
struct tbl_ent *tbl;
int lineNo;
{
    char *q;
    short i;
    default_tbl(tbl);	/* initialise everything to nice values */

    /* start reading the data out of the line */
    tbl->key = get_hex(&p, lineNo);
    /* now read in the override code character */
    tbl->override = *p++;
    if(tbl->override == '!') {	/* special stuff for the menu entries */
        tbl->submenu = get_hex(&p, lineNo);
        /* optional field here is the techlevel this menu become available */
        get_field(p, q)
        tbl->techlevel = atoi(q);
        goto getname;	/* and recover the text of the menu item */
    }
    if(tbl->override == ':') {
        tbl->unique = '1';
        tbl->override = *p++;
    };
    if(tbl->override!='+' && tbl->override!='-' && tbl->override!='*') {
        fprintf(stderr,"Illegal override character specified line %d\n",lineNo);
        exit(1);
    }
    skspc(p);
    tbl->code = *p++;
    /* from here on all fields can be $ of %name */
    get_field(p, q)
    tbl->techlevel = atoi(q);
    get_field(p, q)
    add_mul(q, &(tbl->power), &(tbl->mulpower));
    get_field(p, q)
    add_mul(q, &(tbl->volume), &(tbl->mulvolume));
    get_field(p, q)
    add_mul(q, &(tbl->weight), &(tbl->mulweight));
    get_field(p, q)
    add_mul(q, &(tbl->price), &(tbl->mulprice));
    get_field(p, q)
    add_mul(q, &(tbl->hardpoints), NULL);
    get_field(p, q)
    add_mul(q, &(tbl->minVol), NULL);
    get_field(p, q)
    add_mul(q, &(tbl->cp), NULL);
    get_field(p, q)
    tbl->cpmult = atoi(q);
    get_field(p, q)
    add_mul(q, &(tbl->waste), &(tbl->mulwaste));

    /* all names come here */
    skspc(p);
    /* should I check that the name has a '%' character at the start?? */

getname:
    if(*p=='%') p++;	/* skip a leading % sign */
    skspc(p);			/* and bust any following spaces */
    strncpy(tbl->name, p, NAMELEN);	/* finally, copy the name */

    /* if the code is special, append it to the name */
    if((tbl->code >= 'A' && tbl->code <= 'Z') ||
       (tbl->code >= '1' && tbl->code <= '9')) {
        i = strlen(tbl->name);
        switch(NAMELEN - i) {	/* see how many characters are left over */
        default:
            tbl->name[i++] = ' ';
        case 3:
            tbl->name[i++] = '-';
        case 2:
            tbl->name[i++] = ' ';
        case 1:
            tbl->name[i++] = tbl->code;
            tbl->name[i] = '\0';
        case 0:
            break;
        }
    }
}

/* This defines an order on the data table that makes access slightly
 *	easier.  Basically everything gets sorted on the key first then
 *	the code.  If they have identical keys and codes and overrides, sort
 *	on TL (treat overrides '-' and '*' as the same for this purpose).
 *	If the overrides are different (and the keys/codes are the same), place
 *	the one with override '+' first.
 * Menu items are treated slightly differently, they are still sorted by key
 *	(with all the other data), but they always preceed any real items with the
 *	same key (not sure if these should occur yet).  Since the techlevel field
 *	is used as the second key (for the sub-selection), menu items are sorted
 *	on this field as a secondary index.
 */
static int
compar(a, b)
struct tbl_ent *a, *b;
{
    if(a->key < b->key) return -1;
    if(a->key > b->key) return 1;
    if(a->override == '!' && b->override == '!') {
        if(a->submenu < b->submenu) return -1;
        if(a->submenu > b->submenu) return 1;
        if(a->techlevel < b->techlevel) return -1;
        if(a->techlevel > b->techlevel) return 1;
        /* we will only get here if there is a duplicate line in the file */
        return 0;
    }
    if(a->override == '!') return -1;
    if(b->override == '!') return 1;
    if(a->code < b->code) return -1;
    if(a->code > b->code) return 1;
    if(a->override == b->override || (a->override=='*' && b->override=='-') ||
       (a->override == '-' && b->override == '*')) {
        if(a->techlevel < b->techlevel) return 1;
        if(a->techlevel > b->techlevel) return -1;
        return 0;
    }
    if(a->override == '+') return -1;
    if(b->override == '+') return 1;
    /* we should never reach this point */
    return 0;
}

/* This exported routine, reads the entire data file into memory.
 *	it is responsible for ignoring comment lines '#...' and blank
 *	lines.
 */
void
tbl_input(fl)
FILE *fl;
{
    char buf[512];
    char *p;
    int line = 0;
    while(!feof(fl)) {
        p = fgets(buf, 512, fl);	/* read the thing in */
        line++;						/* keep a line counter for error checking */
        if(!p) {
            break;					/* this will happen at eof also */
        }
        /* remove any trailing white space from the string */
        for(; *p!='\n'; p++);
        for(; isspace(*p); --p)
            *p = '\0';
        p = buf;
        if(*p == '\0') continue;	/* blank lines are ignored */
        skspc(p);					/* ignore leading spaces */
        if(*p=='#') continue;		/* ignore comment lines also */
        /* if we get here it MUST be a data line */
        parse(p, inc_tbl(), line);	/* process the data line */
    }
    /* finally sort the table into an order that is very good for
     *	retrival.
     */
    qsort(theTable, tblLength, sizeof(struct tbl_ent), compar);
}

/* state variables - used to remember where in the database we got
 *	to during our scan.
 */
static unsigned short lastKey = 0xffff;
static long lastIndex = -1;

/* given a key and a tech level, this routine will return the
 *	first matching item on the first call, the second on the second
 *	and so on until all matching items are exhausted when it returns
 *	a NULL pointer.
 */
struct tbl_ent *
tbl_get(k, tl)
unsigned short k;
short tl;
{
    long i,j;
    short found = 0;
    if(k != lastKey) {
        lastKey = k;
        lastIndex = -1;
    }

    /* Sequentially search the table for the next entry with matching key
     *	This could be changed into a binary search for the first/last element
     *	of the table that match the given key, this should work since the
     *	table is sorted into quite a nice sequence.  I do not think that the
     *	speedup that would be gained would be sufficient to warrent the
     *	extra complexity, for a larger database it might become effective.
     */
    for(i=lastIndex+1; i<tblLength; i++)
        if(theTable[i].key == k && theTable[i].techlevel <= tl) {
            if(theTable[i].override == '+' || theTable[i].override == '!') {
                lastIndex = i;	/* where to continue searching from */
                found = 1;
                break;
            }
            /* getting here means that the override is '-' which means we've
             *	got to discard any following things of the same code as
             *	ourselves (remember that the table is sorted on desc TL!)
             */
            for(j=i+1; j<tblLength; j++) {
                if(!(theTable[j].code == theTable[i].code &&
                     theTable[j].key == theTable[i].key))
                    break;
            }
            if(theTable[i].override == '-') {
                lastIndex = j-1;	/* location of the last element */
                found = 1;
                break;				/* got to get out of this loop */
            } /* else override == '*' */
            i = j-1;	/* skip over those that are overridden */
        }

    /* return whatever we located */
    if(found) return &(theTable[i]);
    else {
        lastIndex = -1;
        lastKey = 0xffff;
        return NULL;
    }
}

#ifdef DEBUG
/* from here on everything is testing code */

/* print out some of the fields of the item */
void
print(t)
struct tbl_ent *t;
{
    if(t->override == '!') {
        printf("%04x%c %04x %s\n", t->key, t->override, t->techlevel,
               t->name);
    } else {
        printf("%04x%c %c %2d %10g %10g %6ld %10g %s\n",
               t->key, t->override, t->code, t->techlevel, t->volume, t->mulvolume,
               t->hardpoints, t->minVol, t->name);
    }
}

/* a simple prompt for key/tl and print matching entries */
main(argc, argv)
int argc;
char *argv[];
{
    int key, tl;
    struct tbl_ent *p;
    FILE *fl;
    if(argc!=2) {
        fprintf(stderr, "Usage: %s data_file\n", argv[0]);
        exit(1);
    }
    fl = fopen(argv[1], "r");
    if(fl == NULL) exit(1);
    tbl_input(fl);	/* read the table in */
    close(fl);
    while(1) {
        printf("Enter key, tl pair\n");
        fflush(stdout);
        if(2!=scanf("%4x %d", &key, &tl)) break;
        while((p = tbl_get(key, tl)) != NULL)
            print(p);
    }
}
#endif

