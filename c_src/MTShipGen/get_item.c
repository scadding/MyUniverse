
#include "datatbl.h"
#include <stdio.h>

#define MAX_ITEM_LIST	200
#define MAX_PATH_LEN	10

/* something to keep the current path in the item tree in */
static unsigned short path[MAX_PATH_LEN] = {
    0x0000,	0x0000,	0x0000,	0x0000,	0x0000,
    0x0000,	0x0000,	0x0000,	0x0000,	0x0000,
};
static short pathlen = 0;

#define SCRN_WID 80		/* the number of characters across the display */
#define SCRN_HT  21		/* the number of rows we're going to use */

/* This routine prompts the user for an item, it traverses the menu tree
 *	and will return the item selected when the user finally makes a real
 *	choice.  It requires the tl to be passed in (this should be changed
 *	into the more diverse list of techlevels at some stage).
 */
struct tbl_ent *
item_get(tl)
unsigned short tl;
{
    struct tbl_ent *item_list[MAX_ITEM_LIST];
    int item_nameLen[MAX_ITEM_LIST];
    short max_item;
    int i,j,k,n;
    int maxLen, len;

    while(1) {
        /* grab the list of items into our internal buffer */
        for(max_item=0; max_item<MAX_ITEM_LIST; max_item++)
            if(!(item_list[max_item] = tbl_get(path[pathlen], tl)))
                break;

        /* figure out which has the longest name ... */
        for(maxLen=i=0; i<max_item; i++) {
            len = strlen(item_list[i]->name);
            item_nameLen[i] = len;
            if(len > maxLen)
                maxLen = len;
        }

        /* and display them nicely on the screen */
        /*		if(max_item <= SCRN_HT) k = 1;
         *		else
         */
        k = SCRN_WID / (maxLen + 6);	/* number of columns */
        for(i=0; i<max_item; i+=k) {
            for(j=0; j<k && i+j<max_item; j++) {
                printf("%-3d:%s", i+j, item_list[i+j]->name);
                if(i+j+1 == max_item) {	/* at end of list put full stop */
                    putchar('.');
                } else {
                    putchar(',');	/* after each item goes a comma */
                    if(j != k-1) {	/* and a space, except for the last one */
                        putchar(' ');
                    }
                }
                /* pad the field out to the correct width */
                for(n=item_nameLen[i+j]; n<maxLen; n++) {
                    putchar(' ');
                }
            }
            putchar('\n');
        }

        /* output a prompt thing */
        printf("-1 :Up one Level\t-2 :To top Level\t-3 :Return without item\n");
        do {
            if(1!=scanf("%d", &i))
                exit(0);
        } while(i<-3 || i >= max_item);
        printf("\n\n");
        if(i==-1) {
            path[pathlen] = 0x0000;
            pathlen--;
            if(pathlen < 0)
                pathlen = 0;
        } else if(i==-2) {
            for(i=0; i<=pathlen; i++)
                path[pathlen] = 0x0000;
            pathlen = 0;
        } else if(i==-3) {
            return NULL;
        } else if(item_list[i]->override == '!') {	/* sub menu selection */
            if(pathlen < MAX_PATH_LEN)
                path[++pathlen] = item_list[i]->submenu;
        } else
            return item_list[i];
    }
}

#ifdef DEBUG
/* print out some of the fields of the item */
void
print(t)
struct tbl_ent *t;
{
    if(t->override == '!') {
        printf("%04x%c %04x %2d %s\n", t->key, t->override, t->submenu,
               t->techlevel, t->name);
    } else {
        printf("%04x%c %c %2d %10g %10g %6ld %10g %s\n",
               t->key, t->override, t->code, t->techlevel, t->volume, t->mulvolume,
               t->hardpoints, t->minVol, t->name);
    }
}

/* a quick rotutine that repeatedly asks the user for items to be selected
 */
main(argc, argv)
int argc;
char **argv;
{
    short tl;
    FILE *fl;
    if(argc != 3) {
        fprintf(stderr, "usage: %s data_file tl\n", argv[0]);
        exit(1);
    }
    tl = atoi(argv[2]);
    fl = fopen(argv[1], "r");
    if(fl == NULL) exit(1);
    tbl_input(fl);	/* read the table in */
    close(fl);
    while(1) {
        print(item_get(tl));
    }
}

#endif
