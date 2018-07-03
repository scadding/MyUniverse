#include "sysgen.h"
#include <stdio.h>

void ploc(int tri, int depth, long dir)
{
    int     i;
    printf("Triangle %d level %d ", tri, depth);
    for(i = 0; i < depth; i++) {
        printf("%ld", dir&3);
        dir >>= 2;
    }
    printf("\n");
}

int    c_cross[C_WICE + 1][C_WICE + 1] = {
    {
        C_WATER, 0,C_TUNDRA,C_MTNS, C_ROUGH, C_OPEN,  C_DESERT,C_FOREST,
        C_JUNGLE,C_WATER,C_WATER, C_WICE
    },
    {
        0,	0,0,       0,      0,       0,       0,      0,
        0,       0,      0,       0
    },
    {
        C_TUNDRA,0,C_TUNDRA,C_MTNS, C_TUNDRA,C_OPEN,  C_TUNDRA,C_TUNDRA,
        C_FOREST,C_WATER,C_TUNDRA,C_WICE
    },
    {
        C_MTNS,  0,C_MTNS,  C_MTNS, C_ROUGH, C_ROUGH, C_ROUGH, C_ROUGH,
        C_ROUGH, C_WATER,C_ROUGH, C_ROUGH
    },
    {
        C_ROUGH, 0,C_TUNDRA,C_ROUGH,C_ROUGH, C_ROUGH, C_ROUGH, C_ROUGH,
        C_JUNGLE,C_WATER,C_SICE,  C_WICE
    },
    {
        C_OPEN,  0,C_OPEN,  C_ROUGH,C_ROUGH, C_OPEN,  C_DESERT,C_OPEN,
        C_FOREST,C_WATER,C_OPEN,  C_OPEN
    },
    {
        C_DESERT,0,C_TUNDRA,C_ROUGH,C_ROUGH, C_DESERT,C_DESERT,C_DESERT,
        C_ROUGH, C_WATER,C_DESERT,C_DESERT
    },
    {
        C_FOREST,0,C_TUNDRA,C_ROUGH,C_ROUGH, C_OPEN,  C_DESERT,C_FOREST,
        C_FOREST,C_WATER,C_SICE,  C_WICE
    },
    {
        C_JUNGLE,0,C_FOREST,C_ROUGH,C_JUNGLE,C_FOREST,C_ROUGH, C_FOREST,
        C_JUNGLE,C_WATER,C_SICE,  C_WICE
    },
    {
        C_WATER, 0,C_WATER, C_WATER,C_WATER, C_WATER, C_WATER, C_WATER,
        C_WATER, C_DEEP, C_DEEP,  C_WICE
    },
    {
        C_WATER, 0,C_TUNDRA,C_ROUGH,C_SICE,  C_OPEN,  C_DESERT,C_SICE,
        C_SICE,  C_DEEP, C_SICE,  C_WICE
    },
    {
        C_WICE,  0,C_WICE,  C_ROUGH,C_WICE,  C_OPEN,  C_DESERT,C_WICE,
        C_WICE,  C_WICE, C_WICE,  C_WICE
    }
};

void   expand(int from[28], int to[28], int dir, long seed)
{
    extern sistem   syst;
    int     i;

    xyzmagic(syst.x, syst.y, syst.z, seed);
    switch(dir) {
    case 0:
        to[0] = from[24];
        to[3] = from[17];
        to[5] = from[18];
        to[10] = from[11];
        to[12] = from[12];
        to[14] = from[13];
        to[21] = from[6];
        to[23] = from[7];
        to[25] = from[8];
        to[27] = from[9];
        break;
    case 1:
        to[0] = from[0];
        to[3] = from[1];
        to[5] = from[2];
        to[10] = from[3];
        to[12] = from[4];
        to[14] = from[5];
        to[21] = from[6];
        to[23] = from[7];
        to[25] = from[8];
        to[27] = from[9];
        break;
    case 2:
        to[0] = from[6];
        to[3] = from[10];
        to[5] = from[11];
        to[10] = from[15];
        to[12] = from[16];
        to[14] = from[17];
        to[21] = from[21];
        to[23] = from[22];
        to[25] = from[23];
        to[27] = from[24];
        break;
    case 3:
        to[0] = from[9];
        to[3] = from[13];
        to[5] = from[14];
        to[10] = from[18];
        to[12] = from[19];
        to[14] = from[20];
        to[21] = from[24];
        to[23] = from[25];
        to[25] = from[26];
        to[27] = from[27];
        break;
    }
    /* edges must be fixed so they match */
    to[1] = c_cross[to[0]][to[3]];
    to[6] = c_cross[to[3]][to[10]];
    to[15] = c_cross[to[10]][to[21]];
    to[22] = c_cross[to[21]][to[23]];
    to[24] = c_cross[to[23]][to[25]];
    to[26] = c_cross[to[25]][to[27]];
    to[20] = c_cross[to[27]][to[14]];
    to[9] = c_cross[to[14]][to[5]];
    to[2] = c_cross[to[5]][to[0]];
    /* middle bits */
    i = Rand();
    if(i%2) {
        to[4] = to[3];
    } else {
        to[4] = to[5];
    }
    to[4] = c_cross[to[3]][to[5]];
    i = Rand();
    if(i%2) {
        to[7] = to[3];
    } else {
        to[7] = to[12];
    }
    to[4] = c_cross[to[3]][to[12]];
    i = Rand();
    if(i%2) {
        to[8] = to[12];
    } else {
        to[8] = to[5];
    }
    to[4] = c_cross[to[5]][to[12]];
    i = Rand();
    if(i%2) {
        to[11] = to[10];
    } else {
        to[11] = to[12];
    }
    to[4] = c_cross[to[10]][to[12]];
    i = Rand();
    if(i%2) {
        to[13] = to[12];
    } else {
        to[13] = to[14];
    }
    to[4] = c_cross[to[14]][to[12]];
    i = Rand();
    if(i%2) {
        to[16] = to[10];
    } else {
        to[16] = to[23];
    }
    to[4] = c_cross[to[10]][to[23]];
    i = Rand();
    if(i%2) {
        to[17] = to[12];
    } else {
        to[17] = to[23];
    }
    to[4] = c_cross[to[12]][to[23]];
    i = Rand();
    if(i%2) {
        to[18] = to[12];
    } else {
        to[18] = to[25];
    }
    to[4] = c_cross[to[12]][to[25]];
    i = Rand();
    if(i%2) {
        to[19] = to[25];
    } else {
        to[19] = to[14];
    }
    to[4] = c_cross[to[14]][to[25]];
}
