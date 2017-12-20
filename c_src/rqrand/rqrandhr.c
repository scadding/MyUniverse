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

int main( int argc, char *argv[]) {

/* Variablendeklaration */
int zz[21], i, k, laenge, parameter[5], times;
char *arg;

/* Zufallsgenerator Initialisieren */
diceinit();

/* parameterauswertung */
switch (argc)
   {
   case 1:	printf("\n                   -=[RuneQuest III Character Generation]=-\n");
		printf("			   [House Rules Version  1.0]\n");
		printf("                           <kludged up by Peter Keel>\n\n");
		printf(" RuneQuest(TM) Zufalls-Charaktergenerator.  Das Ziel dieses Programms ist es im\n");
		printf(" wesentlichen die Arbeit des Spielleiters bei der Erschaffung von Nichtspieler-\n");
		printf(" Charakteren zu erleichtern.  Dem Programm kann eine Zahl als Parameter gegeben\n");
		printf(" werden.  Entsprechend viele Charaktere werden dann generiert,  die, sinvoller-\n");
		printf(" weise, am besten in eine Datei umgelenkt werden.\n");
		printf(" Das kopieren,  verbreiten und weitergeben dieses Programms ist erlaubt.  Nicht\n");
		printf(" erlaubt ist der  Verkauf  dieses Programms,  oder Versuche es dem Urheberrecht\n");
		printf(" unterstellen.  Daten wollen frei sein.  Danke                          - Peter\n\n");
		printf(" Erreichen kann man mich via E-Mail: killer@lugs.ch\n\n");
		printf(" Kein Parameter spezifiziert.. Bitte als 'RQRANDHR [zahl]' laufen lassen. Durch\n");
		printf(" eingabe von  'RQRANDHR [zahl] > [ausgabe.erw]'  kann die Ausgabe in eine Datei\n");
		printf(" Umgelenkt werden.\n");
		break;
   default:	printf("\n");
		printf("                     -=[RuneQuest III Character Generation]=-\n");
		printf("			   [House Rules Version  1.0]\n");
		printf("                           <kludged up by Peter Keel>\n\n");

                times = atoi(argv[1]);
		printf("  Es wurden %d Charaktere generiert.\n\n", times);
		for (k=0; k < times; k++) { 

/* Werte Generieren */
rqgen();


/* Ausgabe */
printf("\nEigenschaften : ST %2d  KO %2d  GR %2d  IN %2d  MA %2d  GE %2d  AU %2d  GW %2d",
       ch.eigen[0], ch.eigen[1], ch.eigen[2], ch.eigen[3], ch.eigen[4], ch.eigen[5], ch.eigen[6], ch.eigen[7]);
printf("\nAttribute     : LP %2d  AP %2d  MP %2d  ", ch.eigen[8], ch.eigen[9], ch.eigen[10]);
printf("BW %.1f ", sqrt((ch.eigen[2]+ch.eigen[5])/2.5));
printf("  GR-Klasse %2d  GE-Klasse %2d  ",ch.attr[0], ch.attr[1], ch.attr[2]);
printf("\nBoni          : ");
printf("Bewegung %2.0f  Gewandtheit %2.0f  Handhabung %2.0f  ", ch.boni[1], ch.boni[0], ch.boni[2]);
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

rqgen(){

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
ch.eigen[7]= (dice(2,6)+7+ch.eigen[2])/2;
ch.eigen[8] = (ch.eigen[1]+ch.eigen[7]+1)/2;
ch.eigen[9] = ch.eigen[1]+ch.eigen[7];
ch.eigen[10] = ch.eigen[4];

/* Boni berechnen */
ch.boni[0] = (ch.eigen[5]-10)+((ch.eigen[0]-10)/2)-(ch.eigen[2]-10);
ch.boni[1] = (ch.eigen[5]-10)+((ch.eigen[0]-10)/2)+((ch.eigen[2]-10)/2)-(ch.eigen[7]-10);
ch.boni[2] = (ch.eigen[5]-10)+(ch.eigen[3]-10)+((ch.eigen[0]-10)/2);
ch.boni[3] = (ch.eigen[5]-10)-(ch.eigen[2]-10)-((ch.eigen[4])/2);
ch.boni[4] = (ch.eigen[3]-10)+(ch.eigen[6]-10)+((ch.eigen[4]-10)/2);
ch.boni[5] = (ch.eigen[3]-10);
ch.boni[6] = (ch.eigen[3]-10)+(ch.eigen[4]-10)+((ch.eigen[1]-10)/2);
ch.boni[7] = (ch.eigen[3]-10)+(ch.eigen[4]-10)+((ch.eigen[5]-10)/2);
ch.boni[8] = ch.boni[1];
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
