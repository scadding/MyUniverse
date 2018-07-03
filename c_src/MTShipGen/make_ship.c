
#include "datatbl.h"
#include <stdio.h>
#include <math.h>
#include <strings.h>

struct ary_ent {
    struct tbl_ent	*item;
    double			quant;
};

static short techlevel;

extern char *malloc();
extern char *realloc();

static struct ary_ent *theAry = NULL;
static int curSize = 0;
static int maxSize = 0;

static struct ary_ent *
add_item()
{
#define ALLOC_SIZE 100
#define INIT_ALLOC 500
    if(!theAry) {
        theAry = (struct ary_ent *)malloc(INIT_ALLOC*sizeof(struct ary_ent));
        maxSize = INIT_ALLOC;
    }
    if(curSize == maxSize) {
        maxSize += ALLOC_SIZE;
        theAry=(struct ary_ent *)realloc(theAry,maxSize*sizeof(struct ary_ent));
        if(!theAry) {
            fprintf(stderr, "Fatal error adding item: out of memory\n");
            exit(1);
        }
    }
    return &(theAry[curSize++]);
}

struct ary_ent *
find_item(item)
struct tbl_ent *item;
{
    int i;
    struct ary_ent *it;
    for(i=0; i<curSize; i++)
        if(theAry[i].item == item)
            return(&(theAry[i]));

    /* special checks for items that are unique, if the given item is a
     *	unique one and there is another item with the same first byte in
     *	its key field, we change the old value to be the same as the new
     *	one.  Thus forcing a kind of uniqueness into the program.
     */
    if(item->unique == '1')
        for(i=0; i<curSize; i++)
            if((item->key & 0xff00) == (theAry[i].item->key & 0xff00)) {
                theAry[i].item = item;
                theAry[i].quant = 1;	/* this shouldn't be necessary!! */
                return(&(theAry[i]));
            }

    it = add_item();
    it->item = item;
    it->quant = (item->unique == '1') ? 1 : 0;
    return it;
}

/* define these as global variables so that the print_ship routine can get
 *	access to them.  I couldn't see a clean way around this.
 */
static REAL tvol=0, tpow, tcp, tcpm;
static REAL power, volume, weight, price, cp, cpinp;
static long hp, thp;
static REAL waste;
static long crew, active_crew;
static REAL section_cp[16];	/* the Cp for each (and every section) */

void
recalc(fl)
FILE *fl;
{
    int i, j;
    long m, n;
    REAL r,s,t;
    REAL volConsidered;
    unsigned short aKey;
    /* initialise the globals */
    tvol=0;
    tpow=0;
    tcp=0;
    tcpm=1;
    power=0;
    volume=0;
    weight=0;
    price=0;
    cp=0;
    cpinp=0;
    hp=0;
    thp=1;
    waste=0;
    crew=0;
    active_crew=0;
    for(i=0; i<16; i++)
        section_cp[i] = 0;

    /* find hull volume */
    for(i=0; i<curSize; i++) {
        t = theAry[i].item->volume * theAry[i].quant;
        if(t < tvol) {
            tvol = t;
            thp = theAry[i].item->hardpoints * theAry[i].quant + 0.00001;
        }
    }
    tvol = fabs(tvol);
    if(thp < 0) thp = -thp;
    if(thp < 1) thp = 1;

    /* waste and power */
    for(i=0; i<curSize; i++) {
        t = theAry[i].item->waste;
        s = theAry[i].item->mulwaste * tvol;
        waste += (s+t) * theAry[i].quant;

        t = theAry[i].item->power;
        s = theAry[i].item->mulpower * tvol;
        r = (s+t) * theAry[i].quant;
        if(r < tpow) tpow = r;
    }
    tpow = fabs(tpow);

    /* Special case for section 1 (hull):
     *	the weight multipliers of this section are use on the entire hull volume
     *	and are multiplied together.  Without this hack, each item would be on a
     *	per kL basis and would be added together.  Resulting in much lower weights
     *	and prices for hulls.  The basic hull provided a weight/price and no
     *	multiplier, everything else provides only multipliers to avoid processing
     *	everything twice, set the values to 1 and multiply by the values.
     * This will work because section 1 has only negative volumes/hardpoints and
     *	no energy usage.
     */
    price = 1;
    weight = 1;
    for(i=0; i<curSize; i++) {
        if(theAry[i].quant < 0.000001) continue;	/* zeros don't count */
        if((theAry[i].item->key & 0xf000) != 0x1000)
            continue;	/* only consider the hull here */
        if(theAry[i].item->weight != 0)
            weight *= theAry[i].item->weight * theAry[i].quant;
        else if(theAry[i].item->mulweight != 0)
            weight *= theAry[i].item->mulweight * theAry[i].quant;

        if(theAry[i].item->price != 0)
            price *= theAry[i].item->price * theAry[i].quant;
        else if(theAry[i].item->mulprice != 0)
            price *= theAry[i].item->mulprice * theAry[i].quant;
    }

    /* CP's */
    /* because the hull is treated specially, don't do it's Cp's normally */
    cp = price / 100000 * techlevel;
    section_cp[1] = cp;

    for(i=0; i<curSize; i++) {
        /* do the cp totals for each section */
        j = theAry[i].item->key;
        j &= 0xf000;	/* get the high-level section from the key */
        if(j == 0x1000) continue;
        j >>= 12;	/* should give a range 0..15 */
        t = theAry[i].item->price + theAry[i].item->mulprice * tvol;
        t *= theAry[i].quant;
        section_cp[j] += t / 100000 * techlevel;
        if(j <= 7)	/* power,loco,comms,sens,weap,scrns sections */
            cp += t / 100000 * techlevel;
        else if(j == 8) {	/* only partially include bridge section */
            j = theAry[i].item->key;
            j &= 0x0f00;	/* sub-section */
            j >>= 8;		/* range 0..15 */
            if(j <= 4)	/* environmental controls */
                cp += t / 100000 * techlevel;
        }

        if((theAry[i].item->key & 0xff00) != 0x8500)
            tcp += (-theAry[i].item->cp) * theAry[i].quant;	/* normal item */
        else {				/* computer thingy */
            if(theAry[i].item->cp > cpinp) {
                cpinp = theAry[i].item->cp;
                tcpm = theAry[i].item->cpmult;
            }
        }
    }
    if(tcp > cpinp) {
        tcp = cpinp;
        fprintf(fl, "Warning: computer's cp input exceeded by controls\n");
    }
    if(tcpm < 1) tcpm = 1;
    tcp *= tcpm;	/* adjust for the cp multiplier */

    /* sum everything else */
    for(i=0; i<curSize; i++) {
        aKey = theAry[i].item->key;
        if((aKey & 0xf000) == 0x1000)
            continue;	/* skip hull section */
        /* figure out volume for multaplicative entries.  In Challenge #15 (?)
         *	it is stated that volume of fuel tankage etc requires inertial
         *	compensators/grav plates (life support??).  Currently, this
         *	program doesn't do this.  It may need to be changed, I will when
         *	I see the actual article rather than just hearing about it over
         *	the tml.
         */
        if((aKey & 0xf000) == 0x8000)
            volConsidered = tvol-waste;	/* exclude fuel tankage/some cargo */
        else
            volConsidered = tvol;	/* total ship volume is the default */

        t = theAry[i].item->power + theAry[i].item->mulpower * volConsidered;
        if(t>0)
            power += t * theAry[i].quant;
        t = theAry[i].item->volume + theAry[i].item->mulvolume * volConsidered;
        if(t>0)
            volume += t * theAry[i].quant;
        t = theAry[i].item->weight + theAry[i].item->mulweight * volConsidered;
        if(t>0)
            weight += t * theAry[i].quant;
        t = theAry[i].item->price + theAry[i].item->mulprice * volConsidered;
        if(t>0)
            price += t * theAry[i].quant;
        if(theAry[i].item->hardpoints > 0)
            hp += theAry[i].item->hardpoints * theAry[i].quant;
    }
    /* consistancy checks going in here */
    if(volume > tvol)
        fprintf(fl, "Warning: hull volume exceeded\n");
    if(power > tpow)
        fprintf(fl, "Warning: power plant output exceeded\n");
    if(hp > thp)
        fprintf(fl, "Warning: ship's hardpoint total exceeded\n");
    if(cp > tcp)
        fprintf(fl, "Warning: insufficient control panels/add-ons\n");

    fprintf(fl, "Power %g(%g)\tVolume %g(%g)\tWeight %g\nPrice %g\tCp %g(%g)\tHp %ld(%ld)\n",
            power, tpow, volume, tvol, weight, price, cp, tcp, hp, thp);

    /* finally work out crewing requirements */
    if(tvol >= 1350) {	/* starship / spacecraft */
        /* bridge crew */
        m = ceil((cp / tcpm) / 750.0);
        if(m < 2) m=2;
        else if(m > 10) m = 10 + ceil(m / 10.0);
        crew += m;
        active_crew += m;		/* all bridge crew are active */

        /* engineering crew */
        m = ceil(((section_cp[2] + section_cp[3]) / tcpm) / 400);
        crew += m;
        active_crew += 0.5 * m;	/* half the engineers are */

        /* maintenance crew */
        m = floor(((tvol / 100 + section_cp[1]) / tcpm) / 400);
        crew += m;

        /* gunners */
        m = floor(((section_cp[6] + section_cp[7]) / tcpm) / 10);
        if(m > 50)
            m = 50 + ceil(m/50.0);
        crew += m;
        active_crew += m;

        /* flight crew -- ignored ??? */
        /* ship's troops -- ignored ??? */
        /* command */
        m = floor(crew / 6.0);
        crew += m;
        active_crew += 0.85 * m;	/* 85% of these */

        /* stewards --- must follow command section */
        m = floor(m/8.0 + (crew - m)/50.0);
        crew += m;

        /* frozen watch -- ignored ??? */
        /* medical crew */
        m = floor(crew / 120.0);
        crew += m;

        /* crew segments */
        m = ceil(tvol / 13500.0);
    } else {	/* small craft / vehicle */
        printf("Small craft crews are not implemented yet\n");
        m = 1;
    }
    fprintf(fl, "Crew needed = %ld\tMax active crew = %ld\nSegments = %ld\n",
            crew, active_crew, m);

    /* finally work out the ship's agility bounded between 0 and 6 */
    r = floor((tpow - power) / weight * 5.4);
    if(r>6) r=6;
    else if(r<0) r=0;
    fprintf(fl, "Agility = %g\n\n", r);
}

void list(FILE *fl)
{
    int i;
    for(i=0; i<curSize; i++) {
        if(theAry[i].quant < 1e-8) continue;
        if(fabs(theAry[i].quant - 1) > 1e-8)	/* not one of the things */
            fprintf(fl, "%5gx", theAry[i].quant);
        else fprintf(fl, "     ");

        fprintf(fl, "%-40s %7g %7g %7g %7g\n", theAry[i].item->name,
                theAry[i].quant * (theAry[i].item->power + theAry[i].item->mulpower * tvol),
                theAry[i].quant * (theAry[i].item->volume + theAry[i].item->mulvolume * tvol),
                theAry[i].quant * (theAry[i].item->weight + theAry[i].item->mulweight * tvol),
                theAry[i].quant * (theAry[i].item->price + theAry[i].item->mulprice * tvol)/1e6
               );
    }
}

static char *tl_names[] = {
    "0 (Primitive)", "1 (Bronze/Iron age)", "2 (Pre-Industrial)",
    "3 (Pre-Industrial)", "4 (Industrial)", "5 (Industrial)",
    "6 (Pre-Stellar)", "7 (Pre-Stellar)", "8 (Pre-Stellar)",
    "9 (Early Stellar)", "A (Early Stellar)", "B (Average Stellar)",
    "C (Average Stellar)", "D (Average Stellar)", "E (High Stellar)",
    "F (High Stellar)", "G (High Stellar)", "H (Extreme Stellar)",
    "J (Extreme Stellar)", "K (Extreme Stellar)", "L (Extreme Stellar)",
    "M (Extreme Stellar)",
};

static struct ary_ent *
locate_item(key, mask)
unsigned short key, mask;
{
    int i;
    for(i=0; i<curSize; i++)
        if((theAry[i].item->key & mask) == key)
            return &(theAry[i]);
    return NULL;
}

/* print out the ship in a nice neat format
 */
void
print_ship(fl)
FILE *fl;
{
    struct ary_ent *item1, *item2;
    char *s1, *s2;
    REAL x,y,z;
    int i,j,k;
    recalc(fl);
    fprintf(fl, "\n\n");

    fprintf(fl,"%-10s", "CraftID:");
    fprintf(fl,"%20s, Type     TL=%s, MCr%g","", tl_names[techlevel],
            price/1e6);

    fprintf(fl,"\n%-10s", "Hull:");
    fprintf(fl,"\n%-10s", "Power:");
    fprintf(fl,"\n%-10s", "Loco:");
    fprintf(fl,"\n%-10s", "Commo:");
    fprintf(fl,"\n%-10s", "Sensors:");
    fprintf(fl,"\n%-10s", "Off:");
    fprintf(fl,"\n%-10s", "Def:");
    fprintf(fl,"\n%-10s", "Control:");
    fprintf(fl,"\n%-10s", "Accom:");
    fprintf(fl,"\n%-10s", "Other:");
    fprintf(fl,"\n%-10s", "Comments:");

    fprintf(fl, "\n\n");
    list(fl);
    fprintf(fl, "\n\n");
}

main(argc, argv)
int argc;
char *argv[];
{
    struct tbl_ent *item;
    struct ary_ent *it;
    int i;
    double val;
    double temp;
    FILE *out;

    if(argc != 4) {
        fprintf(stderr, "Usage: %s techlevel datafile outputfile\n", argv[0]);
        exit(1);
    };
    fprintf(stderr, "The MegaTraveller starship design utility\n");
    fprintf(stderr, "Program written by Paul Dale in 1990\n");
    fprintf(stderr, "Some design help by Dan Corrin\n");
    fprintf(stderr, "And beta testing by Bertil Jonell, Eric Halil, Iain Fogg\n\n");
    fflush(stderr);

    techlevel = atoi(argv[1]);
    out = fopen(argv[2], "r");
    if(out == NULL) {
        fprintf(stderr, "Cannot open datafile\n");
        exit(1);
    }
    tbl_input(out);
    close(out);
    do {
        printf("Choose item\n");
        it = NULL;
        item = item_get(techlevel);
        if(item==NULL) {
            printf("Enter 1 to list ship or 2 to dump and quit\n");
            scanf("%lg", &val);
            val = -val;
        } else {
            it = find_item(item);
            if(it->item->unique == '1') {
                val = 1;	/* have to give a value here, or it might fail */
            } else {
                val = it->quant;
                printf("Current quantity = %g\n-1 = list ship, -2 = dump and quit\nHow many >\n", val);
                scanf("%lg", &val);
                if(val>=0) {
                    /* check for minimum volume constraints here, I assume that the tvol variable
                     *	remains valid.  For changes in the hull size, this is not true but such
                     *	changes have no minimum volume requirements.  Thruster based maneuver drives
                     *	are the only thing I can think of with multaplicative volume and minimum
                     *	volume in the same entry.
                     */
                    it->quant = val;
                    temp = it->item->volume + it->item->mulvolume * tvol;
                    if(temp > 0 && (temp * val) < it->item->minVol) {
                        printf("Insufficient volume for that item. Left out\n");
                        it->quant = 0;
                    }
                }
            }
        }
        recalc(stdout);
        if(val==-1)
            list(stdout);
        if(val==-2)
            break;
    } while (!feof(stdin));
    out = fopen(argv[3], "w");
    if(out != NULL) {
        print_ship(out);
        fclose(out);
    }
}
