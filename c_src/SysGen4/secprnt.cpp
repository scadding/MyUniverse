#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysgen.h"
#include "custom.h"

mainworld      mw;
sistem syst;

struct strip {
    char    name[MAX_NAME];
    char    g, n, s, m, port;
    int     pop, law, tech;
}       s[10];

void prpopname();
void sys2free(sistem *syst);
int makesystem(sistem *syst, long x, long y, long z);
void survp(body *s, struct strip *st);
void survey(sistem *syst, struct strip *st);

#define ch(x) (((x)<10)?((x)+'0'):((x)+'A'-10))

int main(int argc, char **argv) {
    long    x, y, z;
    long    A, B, C;
    int     i;

    if(argc > 1) {
        x = atol(argv[1]);
    } else {
        x = 10000L;
    }
    if(argc > 2) {
        y = atol(argv[2]);
    } else {
        y = 10000L;
    }
    if(argc > 3) {
        z = atol(argv[3]);
    } else {
        z = 10000L;
    }

//       printf("\x1bt");
    //fputc(0, stdout);       /* select table 0 i.e. italics */
    for(C = z; C < z + 10; C++) {
//               printf("\x0e"); /* one line double width */
        printf("%05ld, %05ld, %05ld\n", x, y, C);
//               printf("\x0f"); /* select condensed */
//               printf("\x1b-");        /* underline on */
//               fputc(1, stdout);
        printf("     |");
        for(A = x; A < x + 10; A++) {
            printf("%-11ld|", A);
        }
        printf("\n");
//               printf("\x1b-");        /* underline off */
        fputc(0, stdout);
        for(B = y; B < y + 10; B++) {
            for(i = 0; i < 10; i++) {
                s[i].name[0] = 0;
                s[i].g = ' ';
                s[i].n = ' ';
                s[i].s = ' ';
                s[i].m = ' ';
                s[i].port = ' ';
                s[i].pop = 0;
                s[i].law = 0;
            }
            i = 0;
            for(A = x; A < x + 10; A++, i++) {
                if(makesystem(&syst, A, B, C) == 0) {
                    continue;
                }
                survey(&syst, &s[i]);
                sys2free(&syst);
            }
            printf("%5ld|", B);
            for(i = 0; i < 10; i++) {
                prpopname(s[i].name, s[i].pop);
            }
            printf("\n");
//                       printf("\x1b-");        /* underline on */
//                       fputc(1, stdout);
            printf("     |");
            for(i = 0; i < 10; i++) {
                printf("%c%c %c%c%c ", s[i].port, s[i].g, s[i].s, s[i].n, s[i].m);
                if(s[i].pop) {
                    printf("%2d %c|", s[i].law, ch(s[i].tech));
                } else {
                    printf("    |");
                }
            }
//                       printf("\x1b-");        /* underline off */
            fputc(0, stdout);
            printf("\n");
        }
//               printf("\x18"); /* condenced printing off */
        printf("\n");
        printf("\n");
        printf("\n");
        if(getchar() == 'Q') {
            break;
        }
    }
}

void prpopname(char *name, int pop) {
    int     i;

    for(i = 0; i < 10; i++)
        if(!name[i]) {
            break;
        } else if(pop <= 5) {
            if(isupper(name[i])) {
                name[i] = tolower(name[i]);
            }
        } else if(pop >= 9) {
            if(islower(name[i])) {
                name[i] = toupper(name[i]);
            }
        }
    name[10] = 0;
#if 0
    if(pop >= 9) {
        printf("\x1bG");    /* select double strike */
    } else if(pop >= 6) {
        printf("\x1b\x34");    /* select italic */
    }
#endif
    printf("%-11s", name);
#if 0
    if(pop >= 9) {
        printf("\x1bH");    /* de-select double strike */
    } else if(pop >= 6) {
        printf("\x1b\x35");    /* de-select italic */
    }
#endif
    printf("|");
}

void survey(sistem *syst, struct strip *st) {
    if(syst->mw->upp.w.bases&NAVAL_BASE) {
        st->n = 'N';
    }
    if(syst->mw->upp.w.bases&SCOUT_BASE) {
        st->s = 'S';
    }
    if(syst->mw->upp.w.bases&LOCAL_BASE) {
        st->m = 'M';
    }
    survp(syst->sys, st);
}

void survp(body *s, struct strip *st) {
    body    *sptr;

    if(s->type == T_GIANT) {
        st->g = '@';
    }
    if(s->type == T_MAIN) {
        strcpy(st->name, s->name);
        st->port = s->upp.w.starport;
    } else if((s->type == T_WORLD) && (s->upp.w.pop > st->pop)) {
        st->pop = s->upp.w.pop;
        st->law = s->upp.w.law;
        st->tech = s->upp.w.tech;
    } else if((s->type == T_MAIN) && (s->upp.w.pop >= st->pop)) {
        st->pop = s->upp.w.pop;
        st->law = s->upp.w.law;
        st->tech = s->upp.w.tech;
    }
    for(sptr = s->b; sptr; sptr = sptr->o) {
        survp(sptr, st);
    }
}
