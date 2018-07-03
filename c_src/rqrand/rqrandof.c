#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct character {
    int eigen[11];
    int attr[3];
    float boni[10];
    int lp[6];
} ch;

int main( int argc, char *argv[])
{

    /* Variablendeklaration */
    int zz[21], i, k, laenge, parameter[5], times;
    char *arg;

    /* Zufallsgenerator Initialisieren */
    diceinit();

    /* parameterauswertung */
    switch (argc) {
    case 1:
        printf("\n");
        printf("                   -=[RuneQuest III Character Generation]=-\n");
        printf("			 [Official Rules Version 1.0]\n");
        printf("                          <kludged up by Peter Keel>\n\n");
        printf("  RuneQuest(TM) Character Generator. Primarily thought as aid for gamemasters\n");
        printf("  for helping them to create non-player characters. Usage is as simple as can\n");
        printf("  be:  give a number (how much characters you want) as parameter and redirect\n");
        printf("  it into a file, if you want. Like 'rqrand 200 > outfile'. Parameter needed.\n");
        printf("\n");
        printf("  Free for copying, using, spreading, collecting and reverse-engineering. \n");
        printf("  Not free for sale or for being copyrighted.                                \n");
        printf("\n");
        printf("  Internet:   Email: killer@lugs.ch     Homepage: http://www.lugs.ch/~killer \n\n");
        break;
    default:
        printf("\n                     -=[RuneQuest Character Generation]=-\n");
        printf("			   [Official Rules Version]\n");
        printf("                          <kludged up by Peter Keel>\n");
        printf("				 <Version 1.0>\n\n");

        times = atoi(argv[1]);
        printf("  Es wurden %d Charaktere generiert.\n\n", times);
        for (k=0; k < times; k++) {

            /* Werte Generieren */
            rqgen();

            /* Ausgabe */
            printf("\nEigenschaften : ST %2d  KO %2d  GR %2d  IN %2d  MA %2d  GE %2d  AU %2d  ",
                   ch.eigen[0], ch.eigen[1], ch.eigen[2], ch.eigen[3], ch.eigen[4], ch.eigen[5], ch.eigen[6]);
            printf("\nAttribute     : LP %2d  AP %2d  MP %2d  ", ch.eigen[8], ch.eigen[9], ch.eigen[10]);
            printf("BW %2d  GR-Klasse %2d  GE-Klasse %2d  ",ch.attr[0], ch.attr[1], ch.attr[2]);
            printf("\nBoni          : ");
            printf("Gewandtheit %2.0f  Handhabung %2.0f  ", ch.boni[0], ch.boni[2]);
            printf("Heimlichkeit %2.0f  ", ch.boni[3]);
            printf("\n                ");
            printf("Kommunikation %2.0f  Wahrnehmung %2.0f  Wissen %2.0f  ", ch.boni[4], ch.boni[6], ch.boni[5]);
            printf("Magie %2.0f  ", ch.boni[7]);
            printf("\n                ");
            printf("Angriff %2.0f  Abwehr %2.0f  ", ch.boni[8], ch.boni[9]);
            printf("\nKoerperzonen  : ");
            printf("Kopf %2d LP ", ch.lp[0]);
            printf("Arme %2d LP ", ch.lp[1]);
            printf("Brust %2d LP ", ch.lp[2]);
            printf("Bauch %2d LP ", ch.lp[3]);
            printf("Beine %2d LP ", ch.lp[4]);
            printf("\n\n");
        }
    }
}

/***********************************************************
** Roll Dice Functions  -  Needs time.h                   **
**   dice(number of dices, number of sides)               **
**   diceinit()                                           **
************************************************************/
dice(num, size)
int num, size;
{
    int val=0;

    while (num) {
        val += (rand() % size+1);
        --num;
    }
    return(val);
}

diceinit(void)
{
    long tick;

    srand(time(&tick) %20000 );
}

/***********************************************************************
** RuneQuest Character Generations Sequence - needs dice and diceinit **
**   rqgen()
************************************************************************/

rqgen()
{

    /* Do not use these...
    extern struct character {
    	int eigen[11];
    	int attr[3];
    	int boni[10];
            float lp[6];
    	} ch;
    ...must be global */

    /* Charakterwerte berechnen */
    ch.eigen[0] = dice(3,6);
    ch.eigen[1] = dice(3,6);
    ch.eigen[2] = dice(2,6)+6;
    ch.eigen[3] = dice(2,6)+6;
    ch.eigen[4] = dice(3,6);
    ch.eigen[5] = dice(3,6);
    ch.eigen[6] = dice(3,6);
    ch.eigen[8] = (ch.eigen[1]+ch.eigen[2]+1)/2;
    ch.eigen[9] = ch.eigen[1]+ch.eigen[2];
    ch.eigen[10] = ch.eigen[4];

    /* Boni berechnen */
    ch.boni[0] = (ch.eigen[5]-10)+((ch.eigen[0]-10)/2)-(ch.eigen[2]-10);
    ch.boni[2] = (ch.eigen[5]-10)+(ch.eigen[3]-10)+((ch.eigen[0]-10)/2);
    ch.boni[3] = (ch.eigen[5]-10)-(ch.eigen[2]-10)-((ch.eigen[4]-10));
    ch.boni[4] = (ch.eigen[3]-10)+(ch.eigen[6]-10)/2+((ch.eigen[4]-10)/2);
    ch.boni[5] = (ch.eigen[3]-10);
    ch.boni[6] = (ch.eigen[3]-10)+(ch.eigen[4]-10)/2+((ch.eigen[1]-10)/2);
    ch.boni[7] = (ch.eigen[3]-10)+(ch.eigen[4]-10)+((ch.eigen[5]-10)/2);
    ch.boni[8] = ch.boni[2];
    ch.boni[9] = ch.boni[0];

    ch.lp[0]=(int)ch.eigen[8]/3;		/* Kopf */
    ch.lp[1]=(int)ch.eigen[8]/4;		/* Arme */
    ch.lp[2]=(int)(ch.eigen[8]*2)/5;	/* Brust */
    ch.lp[3]=(int)ch.eigen[8]/3;		/* Bauch */
    ch.lp[4]=(int)ch.eigen[8]/3;		/* Beine */

    ch.attr[0] = 3; /*BW*/

    if (ch.eigen[2] < 11)   /* GR-Klasse */
        ch.attr[1]=3;
    else if (ch.eigen[2] > 10 && ch.eigen[2] < 15)
        ch.attr[1]=2;
    else if (ch.eigen[2] > 19)
        ch.attr[1]=0;
    else
        ch.attr[1]=1;

    if (ch.eigen[5] < 11)  /* GE-Klasse */
        ch.attr[2]=4;
    else if (ch.eigen[5] > 10 && ch.eigen[2] < 15)
        ch.attr[2]=3;
    else if (ch.eigen[5] > 19)
        ch.attr[2]=1;
    else
        ch.attr[2]=2;
}

