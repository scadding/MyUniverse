#include <string.h>
#include "sysgen.h"

static int    olen[6] = { 19, 11, 5, 1, 0, 0 };
static int    oinitial[4] = { 5, 11, 8, 12 };
static int    ofinal[4] = { 17, 3, 8, 8 };
static int    ofcon[29] = { 10, 2, 13, 3, 9, 10, 2, 8, 2, 14, 2, 4, 13, 12, 6, 7,
                            2, 2, 7, 10, 7, 3, 13, 21, 2, 9, 13, 3, 7
                          };
static const char   *afcon[29] = { "b", "br", "c", "ch", "d", "f", "fr", "g", "gr", "h",
                                   "j", "k", "l", "m", "n", "p", "pl", "pr", "r", "s", "sh", "st",
                                   "t", "th", "tr", "v", "w", "wh", "y"
                                 };
static int    ovowel[6] = { 49, 73, 40, 38, 7, 9 };
static const char   *avowel[6] = { "a", "e", "i", "o", "ou", "u" };
static int    olcon[29] = { 3, 3, 22, 5, 2, 3, 2, 3, 2, 5, 2, 4, 41, 9, 9, 2,
                            3, 3, 36, 2, 2, 16, 2, 4, 21, 2, 3, 2, 3
                          };
static const char   *alcon[29] = { "c", "ck", "d", "f", "ft", "gh", "ht", "l", "ld",
                                   "ll", "ly", "m", "n", "nd", "ng", "ns", "nt", "p", "r", "rd",
                                   "rs", "s", "ss", "st", "t", "th", "w", "x", "y"
                                 };

static int    len[6];
static int    initial[4];
static int    final[4];
static int    fcon[29];
static int    vowel[6];
static int    lcon[29];

void   seedlang(long x, long y, long z)
{
    int     i;

    for (i = 0; i < 6; i++)
        len[i] = olen[i];
    for (i = 0; i < 4; i++) {
        initial[i] = oinitial[i];
        final[i] = ofinal[i];
    }
    for (i = 0; i < 6; i++)
        vowel[i] = ovowel[i];
    for (i = 0; i < 29; i++) {
        fcon[i] = ofcon[i];
        lcon[i] = olcon[i];
    }
    /* sector drift */
    xyzmagic(x>>6, y>>6, z>>6, LANG_MAGIC);
    drift();
    /* sub-sector drift */
    xyzmagic(x>>4, y>>4, z>>4, LANG_MAGIC);
    drift();
    /* local drift */
    xyzmagic(x, y, z, LANG_MAGIC);
    drift();
}

void   drift()
{
    scatter(initial, 4, 6);
    scatter(len, 6, 6);
    scatter(final, 4, 6);
    scatter(fcon, 29, 36);
    scatter(vowel, 6, 36);
    scatter(lcon, 29, 36);
}

void   scatter(int *arr, unsigned int max_arr, int slide)
{
    unsigned int     i;

    while (slide--) {
        /* take one away */
        do {
            i = rand()%max_arr;
        } while (arr[i] == 0);
        arr[i]--;
        /* add back on */
        i = rand()%max_arr;
        arr[i]++;
    }
}

int    lookup(int *arr, int tot)
{
    int     i;

    i = 0;
    do {
        tot -= arr[i++];
    } while (tot >= 0);
    return(i - 1);
}

char   *getname()
{
    static char     nbuf[40];
    int     leng, type;

    leng = lookup(len, rand()%36) + 1; /* assume names one extra syllable */
    nbuf[0] = 0;
    type = 0;
    do {
        type = lookup(type ? final : initial, rand()%36);
        if (type%2) {
            strcat(nbuf, afcon[lookup(fcon, rand()%216)]);
        }
        strcat(nbuf, avowel[lookup(vowel, rand()%216)]);
        if (type>1) {
            strcat(nbuf, alcon[lookup(lcon, rand()%216)]);
        }
        type = type/2;
    } while (leng--);
    nbuf[0] += 'A' - 'a';
    return(nbuf);
}
