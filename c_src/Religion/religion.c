/* Religion */
/* World Builders Handbook*/
/* DGP */
/* Computer version by David Burden */
/* v0.36 5 aug 90 */
/* Lattice C V5 */
/* STarport File Loading */
/* All text in string file */
/* Streamlined filewrite, suppress added */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
/*#include <dos.h>*/
#include <time.h>
#include <ncurses.h>

#define FALSE 0
#define TRUE 1

#define MAXSTRINGS 300
#define NUMPARAM 7
#define UWPPORT 0
#define UWPSIZE 1
#define UWPATMOS 2
#define UWPHYDRO 3
#define UWPPOP 4
#define UWPGOV 5
#define UWPLAW 6
#define UWPTECH 8
#define HELPCODE "hp"
#define CLCODE "zz"

/* min/max from math.h */

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef min
#define min(a,b) ((a)<=(b)?(a):(b))
#endif           


FILE *fp;
int numstrings = 0;
int numlines = 0;
char *lines[MAXSTRINGS];
struct strdef {
	char *idx;
	char *txt;
} strptr[MAXSTRINGS];
struct ecpdef {
   int value;
   int idx;
   char code[4];
} ecp[] = {
   0, 0, "gv",
   0, 0, "sa",
   0, 0, "dr",
   0, 0, "os",
   0, 0, "lf",
   0, 0, "mf",
   0, 0, "na",
};
int filewrite = FALSE;
int suppress = FALSE;
int runs = 1;
char uwp[9];
int uwpset = FALSE;
   
void main(argc, argv)
char *argv[];
int argc;
{
   int LoadText();
   void OutputCode(), GenerateECP(), DisplayECP(), GenerateUWP();
   void OutputString();
   int Arguements();
   extern int filewrite, runs;
   int loop, valid, quit;
   int wantedhelp = FALSE;
   
   srand((unsigned int)(time(NULL)));
   valid = LoadText();
   GenerateUWP();
  	if (valid && (argc > 1)) {
   	wantedhelp = Arguements(argc, argv);
		if (wantedhelp)
			valid = FALSE;
	}   	    
   if (valid) {
   	OutputString("\n");
      OutputCode("zz0");
      quit = FALSE;
      do {
         for (loop = 0; loop < runs; loop++) {
            OutputString("\n");
            GenerateECP();
            DisplayECP();
            OutputString("\n");
         }
         if (!filewrite) {
         	OutputCode("zz1");
            if (getch() != 'y')
               quit = TRUE;
         } else {
            quit = TRUE;
         }
      } while (!quit);
   } else {
   	if (!wantedhelp) {
      	puts("\nCannot load religion.str text file!\n");
      	(void)getch();
     	}
   }
   if (filewrite)
      (void)fclose(fp);
}

int LoadText()
{
	int loadfile();
   extern struct strdef strptr[];
   extern char *lines[];
   extern int numstrings, numlines;
   int loop, iloop, done, error;
   int valid = TRUE;
   
   error = FALSE;
   numstrings = 0;
                                                 
   numlines = loadfile("religion.str", lines);
	if (numlines < 1) {            
      valid = FALSE;            
	} else {                     
    	valid = TRUE;
      for(loop = 0; loop < numlines; loop++ ) {
			if (lines[loop][0] == ':') {
				iloop = 1;
				done = FALSE;
				do {
					if (lines[loop][iloop] == ':') {
						lines[loop][iloop] = '\0';
						strptr[numstrings].idx = &lines[loop][1];
						strptr[numstrings].txt = &lines[loop][iloop + 1];
						numstrings++;
						done = TRUE;
					}
					iloop++;
				} while ((lines[loop][iloop] != '\0') && !done);
			}
		}
   }
   return(valid);
}


int rnd(dice)
int dice;
{
   int val;
   unsigned int c1, c2;

   dice = (dice < 2) ? 2 : dice;
   dice = (dice > 256) ? 256 : dice;
   c1 = rand();
   c2 = c1 % 100000;
   c1 = c2 * dice / 100000;
   val = c1;
   val++;
   return(val);
}

void GenerateECP()
{
   int rnd(), IsInd(), hexctoi();
   extern struct ecpdef ecp[];
   extern int uwpset;
   extern char uwp[];
   
   if (uwp[UWPGOV] == 'D') {
      ecp[0].value = rnd(6) + rnd(6) - 2 + hexctoi(uwp[UWPTECH]); /* view */
   } else {
      ecp[0].value = rnd(6) + rnd(6) + rnd(6) - 3; /* view */
   }  
   ecp[0].value = min(ecp[0].value, 15);
   ecp[0].value = max(ecp[0].value, 0);
   ecp[1].value = rnd(6) + rnd(6) - 2 + (ecp[0].value / 3); /* aim */
   ecp[1].value = min(ecp[1].value, 15);
   ecp[1].value = max(ecp[1].value, 0);
   ecp[2].value = rnd(6) + rnd(6) - 7 + ecp[1].value; /* devotion */
   ecp[2].value = min(ecp[2].value, 15);
   ecp[2].value = max(ecp[2].value, 0);
   ecp[3].value = rnd(6) + rnd(6) - 7 + ecp[2].value; /* org */
   ecp[3].value = min(ecp[3].value, 15);
   ecp[3].value = max(ecp[3].value, 0);
   ecp[4].value = rnd(6) + rnd(6) - 7 + ecp[3].value; /* liturgical */
   ecp[4].value = min(ecp[4].value, 15);
   ecp[4].value = max(ecp[4].value, 0);
   ecp[5].value = rnd(6) + rnd(6) - 2; /* missionary fervor */
   ecp[5].value = min(ecp[5].value, 10);
   ecp[5].value = max(ecp[5].value, 0);
   ecp[6].value = rnd(6) + rnd(6) + rnd(6) - ecp[5].value; /* number */
   if (uwp[UWPGOV] == 'D' || uwp[UWPGOV] == 'E')
      ecp[6].value = max(ecp[6].value, hexctoi(uwp[UWPPOP]) - 1);
   ecp[6].value = min(ecp[6].value, 11);
   ecp[6].value = max(ecp[6].value, 0);
}

void DisplayECP()
{
	void OutputCode();
   char *itoa();
   extern struct ecpdef ecp[];
   char val[7], target[5], *p;
   int loop;
  
   for (loop = 0; loop < NUMPARAM; loop++) {
      /* match line */
      p = strcpy(target, ecp[loop].code);
      (void)itoa(ecp[loop].value, val);
      p = strcat(target, val);
      OutputCode(target);
   }
}

void GenerateUWP()
{
   char itohexc();
   int rnd();
   extern char uwp[];
   int val;

   val = rnd(6) + rnd(6);
   uwp[UWPPORT] = 'X';
   if (val > 1)
      uwp[UWPPORT] = 'A';
   if (val > 4)
      uwp[UWPPORT] = 'B';
   if (val > 6)
      uwp[UWPPORT] = 'C';
   if (val > 8)
      uwp[UWPPORT] = 'D';
   if (val > 9)
      uwp[UWPPORT] = 'E';
   uwp[UWPSIZE] = itohexc(rnd(6) + rnd(6) - 2);
   if (uwp[UWPSIZE] == '0') {
      uwp[UWPATMOS] = '0';
   } else {
      uwp[UWPATMOS] = rnd(6) + rnd(6) - 7 + hexctoi(uwp[UWPSIZE]);
   }
   if (hexctoi(uwp[UWPSIZE]) <= 1) {
      uwp[UWPHYDRO] = '0';
   } else {
      val = rnd(6) + rnd(6) - 7 + hexctoi(uwp[UWPSIZE]);
      if (hexctoi(uwp[UWPATMOS]) <= 1)
         val -= 4;
      if (hexctoi(uwp[UWPATMOS]) >= 10)
         val -= 4;
      uwp[UWPHYDRO] = itohexc(val);
   }
   uwp[UWPPOP] = itohexc(rnd(6) + rnd(6) - 2);
   uwp[UWPGOV] = itohexc(rnd(6) + rnd(6) - 7 + hexctoi(uwp[UWPPOP]));
   uwp[UWPLAW] = itohexc(rnd(6) + rnd(6) - 7 + hexctoi(uwp[UWPGOV]));
   val = rnd(6); 
   if (uwp[UWPPORT] == 'A')
      val += 6;
   if (uwp[UWPPORT] == 'B')
      val += 4;
   if (uwp[UWPPORT] == 'C')
      val += 2;
   if (uwp[UWPPORT] == 'X')
      val -= 4;
   if (hexctoi(uwp[UWPSIZE]) < 5)
      val++;
   if (hexctoi(uwp[UWPSIZE]) < 2)
      val++;
   if (hexctoi(uwp[UWPATMOS]) < 4)
      val++;
   if (hexctoi(uwp[UWPATMOS]) > 9)
      val++;
   if (uwp[UWPHYDRO] == '9')
      val++;
   if (uwp[UWPHYDRO] == 'A')
      val += 2;
   if ((hexctoi(uwp[UWPPOP]) >= 1) && (hexctoi(uwp[UWPPOP]) <= 5))
      val++;
   if (uwp[UWPPOP] == '9')
      val += 2;
   if (uwp[UWPPOP] == 'A')
      val += 4;
   if (uwp[UWPGOV] == '0')
      val++;
   if (uwp[UWPGOV] == '5')
      val++;
   if (uwp[UWPGOV] == 'D')
      val -= 2;
   uwp[UWPTECH] = itohexc(val);
}
   
char *itoa(val, buf)
char *buf;
int val;
{
   int loop, ptr, fac, started;
   
   started = FALSE;
   ptr = 0;
   if (val < 0) {
      buf[0] = '-';
      ptr = 1;
      val = -1 * val;
   }
   for(loop = 100000; loop > 1; loop = loop / 10) {
      fac = (int)(val / loop);
      if ((fac > 0) || started) {
         started = TRUE;
         buf[ptr] = '0' + fac;
         ptr++;
         val = val % loop;
      }
   }
   buf[ptr] = '0' + val;
   ptr++;
   buf[ptr] = '\0';
   return(buf);
}

int Arguements(argc, argv)
int argc;
char *argv[];
{
   void OutputString(), OutputCode(), HelpList();
   extern FILE *fp;
   extern int filewrite, suppress, runs, uwpset;
   extern char uwp[];
   char helpstr[5];
   int loop;
   int wantedhelp = FALSE;
   int wantssuppress = FALSE;
   char *p;

   for(loop = 1; loop < argc; loop++) {
      if (argv[loop][0] == '-') {
         /* parameter */
         switch (argv[loop][1]) {

				case 's':
					wantssuppress = TRUE;
					break;
					
            case 'n':
               runs = atoi(&argv[loop][2]);
               break;
               
            case 'u':
               p = strcpy(uwp, &argv[loop][2]);
               uwpset = TRUE;
               break;
            
            case 'o':
               fp = fopen(&argv[loop][2], "w");
               if (fp == NULL) {
                  OutputCode("zz4");
                  OutputString("\n\n");
                  OutputCode("zz5");
                  OutputString("\n");
                  (void)getchar();
               } else {
                  filewrite = TRUE;
               }
               break;

            case 'h':
               if (argv[loop][2] == 0) {
                  p = strcpy(helpstr, HELPCODE);
               } else {
                  p = strcpy(helpstr, &argv[loop][2]);
               }
               wantedhelp = TRUE;
         }
      }
   }
   if (wantssuppress && filewrite)
   	suppress = TRUE;
   if (wantedhelp) {
   	OutputCode(helpstr);
   	if (!filewrite) {
        	OutputString("\n");
        	OutputCode("zz5");
        	OutputString("\n");
        	(void)getchar();
      } 
   }       	
   return(wantedhelp);
}

int hexctoi(txt)
char txt;
{
   int val;
   
   if ((txt >= '0') && (txt <= '9')) {
      val = (int)(txt - '0');
   } else {
      val = (int)(txt - 'A' + 10);
   }
   val = (val < 0) ? 0 : val;
   return(val);
}

char itohexc(val)
int val;
{
   char fig;
   
   if ((val >= 0) && (val <= 9)) {
      fig = '0' + val;
   } else {
      fig = 'A' + val - 10;
   }
   return(fig);
}

/* fol is modified from Starport release */

int loadfile(name, lines)
char *name, *lines[];
{
	int error, blocks; 
	int length, i; 
	FILE *fp;
	size_t a; 
	char *area, *p;

	fp = fopen(name, "rb");			if(fp == NULL)return(-1);
	error = fseek(fp, 0, SEEK_END);	if(error == -1)return(-2);
	length = ftell(fp);				if(length == -1L)return(-3);
	error = fseek(fp, 0, SEEK_SET);	if(error == -1)return(-4);
	blocks = length % 512;
	area = malloc(length + 512);	if(area == NULL)return(-5);
	a = fread(area, 512, blocks, fp);	if(a == -1)return(-6);
	error = fclose(fp);				if(error == -1)return(-7);

	i = 0;
	lines[i] = area;

	for (p = area; p < (area+length); ) {
		if (*p == '\n' || *p == '\r') {
			*p++ = '\0';
			/* p++; */
			lines[++i] = p;
		} else {
			p++;
		}
	}
	lines[++i] = NULL;
	return(i - 1);
}

void OutputString(char *txt);

void OutputCode(code)
char *code;
{
	extern struct strdef strptr[];
	extern int numstrings;
	static int recurse = FALSE;
	int loop;
	int found = FALSE;
	
   loop = 0;
   do {
   	if (!strcmp(strptr[loop].idx, code)) {
   		OutputString(strptr[loop].txt);
      	found = TRUE;
     	}
     	loop++;
   } while ( loop < numstrings);
   
   if (!found) {
  		if (!recurse) {
	  		recurse = TRUE;
  			OutputCode("zz3");
  		} else {
  			recurse = FALSE;
  			OutputString("Error: String not Found");
  		}
  	}
}

void OutputString(char *txt)
{
	extern FILE *fp;
	extern int filewrite, suppress;

	if (!suppress)  		
  		puts(txt);
  	if (filewrite) {
  		fputs(txt, fp);
  		fputs("\n", fp);
  	}
}

