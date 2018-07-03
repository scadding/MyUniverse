/* Psyche */
/* Albedo by Thoughts and Images */
/* Computer version by David Burden */
/* v0.18 18 Aug 90 */
/* Lattice C V5 */
/* STarport File Loading */
/* All text in string file */
/* Streamlined filewrite, suppress added */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

#ifdef MSDOS
#include <dos.h>
#endif

int getch();

#define FALSE 0
#define TRUE 1

#define MAXSTRINGS 300
#define NUMPARAM 11
#define UPPSTR 0
#define UPPDEX 1
#define UPPEND 2
#define UPPINT 3
#define UPPEDN 4
#define UPPSOC 5
#define SPITEN 0
#define SPIONE 1
#define COREDISP 2
#define INCLDISP 3
#define DRIVE 4
#define ALLEGIANCE 5
#define DEVOTION 6
#define STABILITY 7
#define LEADERSHIP 8
#define RESPECT 9
#define TOLERANCE 10

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
char *sarea;
struct strdef {
	char *idx;
	char *txt;
} strptr[MAXSTRINGS];
struct ecpdef {
   int value;
   int idx;
   char code[5];
} ecp[] = {
   0, 0, "spf",
   0, 0, "spf",
   0, 0, "cd",
   0, 0, "id",
   0, 0, "dr",
   0, 0, "al",
   0, 0, "dv",
   0, 0, "st",
   0, 0, "ld",
   0, 0, "re",
   0, 0, "tl"
};
int filewrite = FALSE;
int suppress = FALSE;
int runs = 1;
char upp[6];
int uppset = FALSE;
int worldpsyche = FALSE;

void OutputString(const char * s);


int main(argc, argv)
char *argv[];
int argc;
{
   int LoadText();
   void OutputCode(), GenerateECP(), DisplayECP(), GenerateUPP();
   int Arguements();
   extern int worldpsyche, filewrite, runs, uppset;
   extern int numlines, numstrings;
   extern char *sarea;
   int loop, valid, quit;
   int wantedhelp = FALSE;
   
   srand((unsigned int)(time(NULL)));
   valid = LoadText();
  	if (valid) {
   	wantedhelp = Arguements(argc, argv);
		if (wantedhelp)
			valid = FALSE;
	}   	    
   if (valid) {
   	OutputString("\n");
   	if (!worldpsyche) {
      	OutputCode("zz0");
      } else {
      	OutputCode("zz6");
      }
      quit = FALSE;
      do {
         for (loop = 0; loop < runs; loop++) {
            OutputString("\n");
            if (!uppset)
            	GenerateUPP();
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
      	puts("\nError: Cannot load psyche.str text file!\n");
      	(void)getch();
     	}
   }
   if (filewrite)
      (void)fclose(fp);
   free(sarea);
   return(0);
}

int LoadText()
{
   int loadfile();
   extern struct strdef strptr[];
   extern char *sarea, *lines[];
   extern int numstrings, numlines;
   int loop, iloop, done, error;
   int valid = TRUE;
   
   error = FALSE;
   numstrings = 0;
                                                 
   numlines = loadfile("psyche.str", sarea, lines);
	if (numlines < 1) {         
      valid = FALSE;
      printf("Error: Cannot load textfile - Error code %d\n", numlines);            
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

int RangeLimit(val, ll, ul)
int val, ll, ul;
{
	int temp;
	
	temp = max(ll, val);
	temp = min(ul, temp);
	return(temp);
}

void GenerateECP()
{
   int rnd(), hexctoi(), RangeLimit();
   extern struct ecpdef ecp[];
   extern char upp[];
   extern int worldpsyche;
   int spi, temp;

	if (!worldpsyche) {
		temp = rnd(6) + rnd(6) - 7 + hexctoi(upp[UPPSOC]);
		ecp[ALLEGIANCE].value = RangeLimit(temp, 0, 15);
	} else {
		ecp[ALLEGIANCE].value = 6 + rnd(9);
	}
	
	temp = rnd(6) + rnd(6) - 7 + ecp[ALLEGIANCE].value;
	ecp[DEVOTION].value = RangeLimit(temp, 0, 15);
	
	temp = rnd(6) + rnd(6) - 7 + ecp[DEVOTION].value;
	ecp[STABILITY].value = RangeLimit(temp, 0, 15);
	
	temp = rnd(6) + rnd(6) - 7 + ecp[STABILITY].value;
	ecp[LEADERSHIP].value = RangeLimit(temp, 0, 15);
	
	temp = rnd(6) + rnd(6) - 7 + hexctoi(upp[UPPSOC]);
	ecp[RESPECT].value = RangeLimit(temp, 0, 15);
	
	temp = rnd(6) + rnd(6) - 7 + ecp[DEVOTION].value;
	ecp[TOLERANCE].value = RangeLimit(temp, 0, 15);
	
	temp = rnd(6) + rnd(6) - 7 + hexctoi(upp[UPPEND]);
	ecp[DRIVE].value = RangeLimit(temp, 0, 15);
	
	
	ecp[COREDISP].value = rnd(20);
	ecp[INCLDISP].value = rnd(20);
	if (ecp[COREDISP].value == ecp[INCLDISP].value) {
		ecp[COREDISP].value += 50;
		ecp[INCLDISP].value = -1;
	}  
	spi = hexctoi(upp[UPPINT]) + ecp[STABILITY].value + ecp[DRIVE].value;
	spi += hexctoi(upp[UPPEDN]) / 3;
	if (ecp[COREDISP].value > 50) {
		spi -= 12;
	} else {
		if (ecp[COREDISP].value > 10)
			spi -= 6;
		if (ecp[INCLDISP].value > 10)
			spi -= 3;
	}
	spi = RangeLimit(spi, 0, 50);
	ecp[SPITEN].value = spi / 10;
	ecp[SPIONE].value = spi % 10;
	ecp[SPIONE].value = min(9, ecp[SPIONE].value);
}

void DisplayECP()
{
	void OutputCode(), OutputStringNcr();
	void OutputCodeNcr(), OutputCodeSF();
	void OutputChar();
   char *itoa();
   extern struct ecpdef ecp[];
   extern char upp[];
   extern int numstrings, worldpsyche;
   char blk[80], target[6], *p;
   int loop;
  
  	if (!worldpsyche) {
	  	OutputStringNcr("UPP: ");
  		for (loop = UPPSTR; loop <= UPPSOC; loop++ ) {
  			OutputChar(upp[loop]);
  		}
  		OutputString("\n");
	
		for (loop = DRIVE; loop <= TOLERANCE; loop++ ) {
  			p = strcpy(target, ecp[loop].code);
  			p = strcat(target, itoa(ecp[loop].value, blk));
  			OutputCode(target);
  		}
  	} else {
  		p = strcpy(target, ecp[DEVOTION].code);
  		p = strcat(target, itoa(ecp[DEVOTION].value, blk));
  		OutputCode(target);
  	}
   
   OutputString(" ");
   p = strcpy(target, ecp[COREDISP].code);
   p = strcat(target, itoa(ecp[COREDISP].value, blk));
   OutputCode(target);
   if (ecp[COREDISP].value < 50 ) {
   	p = strcpy(target, ecp[INCLDISP].code);
   	p = strcat(target, itoa(ecp[INCLDISP].value, blk));
   	OutputCode(target);
   }
   
   OutputString(" ");
   if (!worldpsyche) {
   	p = strcpy(target, itoa(ecp[SPITEN].value, blk));
   	p = strcat(target, ".");
   	p = strcat(target, itoa(ecp[SPIONE].value, blk));
   	OutputCodeSF(ecp[SPITEN].code, target);
		OutputString(" ");
	}
   OutputString(" ");
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
   void OutputCode(), OutputCodeStub();
   extern FILE *fp;
   extern int worldpsyche, filewrite, suppress, runs, uppset;
   extern char upp[];
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
               p = strcpy(upp, &argv[loop][2]);
               uppset = TRUE;
               break;
     
     			case 'w':
     				worldpsyche = TRUE;
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
               break;
         }
      }
   }
   if (wantssuppress && filewrite)
   	suppress = TRUE;
   if (wantedhelp) {
   	OutputCodeStub(helpstr);
   	if (!filewrite) {
        	OutputString("\n");
        	OutputCode("zz5");
        	OutputString("\n");
        	(void)getchar();
      } 
   }       	
   return(wantedhelp);
}

void GenerateUPP()
{
   int rnd();
   char itohexc();
   extern char upp[];
   int loop;

	for (loop = 0; loop < 6; loop++ ) {
		upp[loop] = itohexc(rnd(6) + rnd(6));
	}
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


int loadfile(char *name, char *area, char *lines[]) {
	int error, blocks; 
	int length, i;
	FILE *fp;
	size_t a; 
	char *p;

	fp = fopen(name, "rb");
	if(fp == NULL)
	    return(-1);
	error = fseek(fp, 0, SEEK_END);
	if(error == -1)
	    return(-2);
	length = (int)ftell(fp);
	if(length == -1L)
	    return(-3);
	error = fseek(fp, 0, SEEK_SET);	
	if(error == -1)
	    return(-4);
	blocks = length / 512;  /* not % !!! */
	area = malloc(length + 512);	
	if(area == NULL)
	    return(-5);
	a = fread(area, 512, blocks + 1, fp);
	if(a == -1)
	    return(-6);
	error = fclose(fp);
	if(error == -1)
	    return(-7);

	i = 0;
	lines[i] = area;

	for (p = area; p < (area+length); ) {
		if (*p == '\n' || *p == '\r') {
			*p++ = '\0';
/*			p++;  *//* to skip linefeed */
			lines[++i] = p;
		} else {
			p++;
		}
	}
	lines[++i] = NULL;
	return(i-1);  /* returns number of lines */
}

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
      	recurse = FALSE;
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

void OutputCodeNcr(code)
char *code;
{
	void OutputStringNcr();
	extern struct strdef strptr[];
	extern int numstrings;
	static int recurse = FALSE;
	int loop;
	int found = FALSE;
	
   loop = 0;
   do {
   	if (!strcmp(strptr[loop].idx, code)) {
   		OutputStringNcr(strptr[loop].txt);
      	found = TRUE;
      	recurse = FALSE;
     	}
     	loop++;
   } while ( loop < numstrings);
   
   if (!found) {
  		if (!recurse) {
	  		recurse = TRUE;
  			OutputCodeNcr("zz3");
  		} else {
  			recurse = FALSE;
  			OutputString("Error: String not Found");
  		}
  	}
}

void OutputCodeSF(code, str)
char *code, *str;
{
	void OutputStringSF();
	extern struct strdef strptr[];
	extern int numstrings;
	static int recurse = FALSE;
	int loop;
	int found = FALSE;
	
   loop = 0;
   do {
   	if (!strcmp(strptr[loop].idx, code)) {
   		OutputStringSF(strptr[loop].txt, str);
      	found = TRUE;
      	recurse = FALSE;
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

void OutputString(const char *txt)
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

void OutputStringSF(txt, str)
char *txt, *str;
{
	extern FILE *fp;
	extern int filewrite, suppress;

	if (!suppress)  		
  		printf(txt, str);
  	if (filewrite) {
  		fprintf(fp, txt, str);
  	}
}

void OutputStringNcr(txt)
char *txt;
{
	extern FILE *fp;
	extern int filewrite, suppress;

	if (!suppress)  		
  		printf("%s", txt);
  	if (filewrite) {
  		fputs(txt, fp);
  	}
}

void OutputChar(cter)
char cter;
{
	extern FILE *fp;
	extern int filewrite, suppress;

	if (!suppress)  		
  		putchar(cter);
  	if (filewrite) {
  		fputc(cter, fp);
  	}
}
  	
void OutputCodeStub(code)
char *code;
{
	extern struct strdef strptr[];
	extern int numstrings;
	static int recurse = FALSE;
	int loop;
	int found = FALSE;
	
   loop = 0;
   do {
   	if (!strncmp(strptr[loop].idx, code, 2)) {
   		OutputString(strptr[loop].txt);
      	found = TRUE;
      	recurse = FALSE;
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
  	
#ifndef MSDOS
int getch() {
    int charact, ch;
    while ((ch=getchar()) != '\n') {
    charact=ch;
    }
    return charact;
}
#endif 
