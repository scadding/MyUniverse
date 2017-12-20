#include <string.h>
#include "sysgen.h"

int    olen[6] = { 19, 11, 5, 1, 0, 0 };
int    oinitial[4] = { 5, 11, 8, 12 };
int    ofinal[4] = { 17, 3, 8, 8 };
int    ofcon[29] = { 10, 2, 13, 3, 9, 10, 2, 8, 2, 14, 2, 4, 13, 12, 6, 7,
                     2, 2, 7, 10, 7, 3, 13, 21, 2, 9, 13, 3, 7
                   };
const char   *afcon[29] = { "b", "br", "c", "ch", "d", "f", "fr", "g", "gr", "h",
                            "j", "k", "l", "m", "n", "p", "pl", "pr", "r", "s", "sh", "st",
                            "t", "th", "tr", "v", "w", "wh", "y"
                          };
int    ovowel[6] = { 49, 73, 40, 38, 7, 9 };
const char   *avowel[6] = { "a", "e", "i", "o", "ou", "u" };
int    olcon[29] = { 3, 3, 22, 5, 2, 3, 2, 3, 2, 5, 2, 4, 41, 9, 9, 2,
                     3, 3, 36, 2, 2, 16, 2, 4, 21, 2, 3, 2, 3
                   };
const char   *alcon[29] = { "c", "ck", "d", "f", "ft", "gh", "ht", "l", "ld",
                            "ll", "ly", "m", "n", "nd", "ng", "ns", "nt", "p", "r", "rd",
                            "rs", "s", "ss", "st", "t", "th", "w", "x", "y"
                          };

int    len[6];
int    initial[4];
int    final[4];
int    fcon[29];
int    vowel[6];
int    lcon[29];

void drift();
void scatter(int arr[], int max_arr, int slide);

void seedlang(long x, long y, long z) {
    int     i;

    for(i = 0; i < 6; i++) {
        len[i] = olen[i];
    }
    for(i = 0; i < 4; i++) {
        initial[i] = oinitial[i];
        final[i] = ofinal[i];
    }
    for(i = 0; i < 6; i++) {
        vowel[i] = ovowel[i];
    }
    for(i = 0; i < 29; i++) {
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

void drift() {
    scatter(initial, 4, 6);
    scatter(len, 6, 6);
    scatter(final, 4, 6);
    scatter(fcon, 29, 36);
    scatter(vowel, 6, 36);
    scatter(lcon, 29, 36);
}

void scatter(int arr[], int max_arr, int slide) {
    int     i;

    while(slide--) {
        /* take one away */
        do {
            i = Rand()%max_arr;
        } while(arr[i] == 0);
        arr[i]--;
        /* add back on */
        i = Rand()%max_arr;
        arr[i]++;
    }
}

int lookup(int arr[], int tot) {
    int     i;

    i = 0;
    do {
        tot -= arr[i++];
    } while(tot >= 0);
    return(i - 1);
}

char   *getname() {
    static char     nbuf[40];
    int     leng, type;

    leng = lookup(len, Rand()%36) + 1; /* assume names one extra syllable */
    nbuf[0] = 0;
    type = 0;
    do {
        type = lookup(type ? final : initial, Rand()%36);
        if(type%2) {
            strcat(nbuf, afcon[lookup(fcon, Rand()%216)]);
        }
        strcat(nbuf, avowel[lookup(vowel, Rand()%216)]);
        if(type>1) {
            strcat(nbuf, alcon[lookup(lcon, Rand()%216)]);
        }
        type = type/2;
    } while(leng--);
    nbuf[0] += 'A' - 'a';
    return(nbuf);
}
