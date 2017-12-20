#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "room.h"

int main(int argc, char **argv)
{
    int loop;
    int num_room;
    int seed;
    int is_html = 0;

    if (argc < 2) {
        printf("useage: %s <number of rooms> [seed]\n", argv[0]);
        return(0);
    } else if (argc == 2) {

        if (strcmp("HTML",argv[1]) == 0) {
            num_room = 1;
            seed = time(NULL);
            is_html = 1;
        } else {
            num_room = atoi(argv[1]);
            seed = time(NULL);
        }
    } else {
        num_room = atoi(argv[1]);
        seed = atoi(argv[2]);
    }
    srand(seed);

    if (num_room < 1) {
        for(loop = 0; loop < 100; loop++)
            printf("air_currents[%d] [%s]\n",loop+1,air_currents[loop]);
        printf("\n");
        for(loop = 0; loop < 100; loop++)
            printf("odors[%d] [%s]\n",loop+1,odors[loop]);
        printf("\n");
        for(loop = 0; loop < 100; loop++)
            printf("air[%d] [%s]\n",loop+1,air[loop]);
        printf("\n");

        for(loop = 0; loop < 100; loop++)
            printf("junk[%d] [%s]\n",loop+1,junk[loop]);
        printf("\n");

        for(loop = 0; loop < 100; loop++)
            printf("sounds[%d] [%s]\n",loop+1,sounds[loop]);
        printf("\n");

        for(loop = 0; loop < 100; loop++)
            printf("traps[%d] [%s]\n",loop+1,traps[loop]);
        printf("\n");

    } else {
        if (is_html) {
            printf("<HTML><TITLE>A Random Room</TITLE><H4>");
        }
        for (loop = 0; loop < num_room; loop++) {
            if (is_html) {
                printf("A Dungeon Room<BR><P>");
            } else {
                printf("Room %d\n",loop + 1);
            }
            printf("Air Current: %s\n",air_currents[rand() % 100]);
            if (is_html) {
                printf("<BR>");
            }
            printf("General Odor: %s\n",odors[rand() % 100]);
            if (is_html) {
                printf("<BR>");
            }
            printf("Air State : %s\n",air[rand() %100]);
            if (is_html) {
                printf("<BR>");
            }
            int num_junk = rand() % 11;
            if (num_junk) {
                if (is_html) {
                    printf("<HR>");
                }
                printf("General Items in room:\n");
                if (is_html) {
                    printf("<OL>");
                }
                for (int loop2 = 0; loop2 < num_junk; loop2++) {
                    if (is_html) {
                        printf("<LI>%s<BR>\n",junk[rand() % 100]);
                    } else {
                        printf("\t%d -- %s\n",loop2 + 1,junk[rand() % 100]);
                    }
                }
                if (is_html) {
                    printf("</OL>");
                }
            }
            int has_sound = rand() % 4;
            if (!has_sound) {
                if (is_html) {
                    printf("<HR>");
                }
                printf("Sound in room: %s\n",sounds[rand() % 100]);
            }
            int has_trap = rand() % 10;
            if (!has_trap) {
                if (is_html) {
                    printf("<HR>");
                }
                printf("Trap in room: %s\n",traps[rand() % 100]);
            }
            printf("\n");
        }
        if (is_html) {
            printf("</H4></HTML>");
        }
    }
    return(0);
}
