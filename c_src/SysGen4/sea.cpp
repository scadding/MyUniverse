#include "sysgen.h"

void seas(int plates[20][28], int buf[20][28], int arch, int islands, int minor, int major, int landpc)
{
    int     land, i, tot;
    int     majsize[15], minsize[15];

    for(i = 0; i < 20; i++)
        for(tot = 0; tot < 28; tot++) {
            buf[i][tot] = 0;
        }
    if(major || minor) {
        land = (362*landpc)/100 - (arch + 1)/2 - islands;
        minsize[0] = land/(minor + 2*major);
        for(i = 1; i < minor; i++) {
            minsize[i] = minsize[0];
        }
        for(i = 0; i < major; i++) {
            majsize[i] = minsize[0];
        }
        tot = minsize[0]*(minor + 2*major);
        while(tot < land) {
            if(major && minor) {
                if(Rand()%2) {
                    majsize[Rand()%major]++;
                } else {
                    minsize[Rand()%minor]++;
                }
            } else if(major) {
                majsize[Rand()%major]++;
            } else {
                minsize[Rand()%minor]++;
            }
            tot++;
        }
        for(i = 0; i < major; i++) {
            place(plates, buf, majsize[i], -1);
        }
        for(i = 0; i < minor; i++) {
            place(plates, buf, minsize[i], -1);
        }
    }
    for(i = 0; i < islands; i++) {
        place(plates, buf, 1, -1);
    }
    for(i = 0; i < arch; i++) {
        place(plates, buf, 1, -2);
    }
    for(i = 0; i < 20; i++)
        for(tot = 0; tot < 28; tot++)
            if(buf[i][tot] == 0) {
                buf[i][tot] = 1;
            }
}
