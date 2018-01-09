/* Aspects of Culture */
/* TD-16 p 46 - 48 */
/* Nancy Parker */
/* Computer version by David Burden */
/* v0.20 aug 90 */
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

#define FALSE 0
#define TRUE 1

#define MAXSTRINGS 600
#define NUMPARAM 17
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
   0, 0, "ic",
   0, 0, "sm",
   0, 0, "pm",
   0, 0, "wv",
   0, 0, "fe",
   0, 0, "gd",
   0, 0, "wa",
   0, 0, "wd",
   0, 0, "pr",
   0, 0, "bz",
   0, 0, "mo",
   0, 0, "hl",
   0, 0, "ol",
   0, 0, "cr",
   0, 0, "ho",
   0, 0, "we",
   0, 0, "at"
};
int filewrite = FALSE;
int suppress = FALSE;
int runs = 1;
char uwp[9];
int uwpset = FALSE;
   
int main(argc, argv)
char *argv[];
int argc;
{
   int LoadText();
   void OutputCode(), GenerateECP(), DisplayECP(), GenerateUWP();
   void OutputString();
   int Arguements();
   extern int filewrite, runs, uwpset;
   int loop, valid, quit;
   int wantedhelp = FALSE;
   
   srand((unsigned int)(time(NULL)));
   valid = LoadText();
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
            if (!uwpset)
            	GenerateUWP();
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
      	puts("\nError: Cannot load CULTURE.STR text file!\n");
      	(void)getch();
     	}
   }
   if (filewrite)
      (void)fclose(fp);
return 0;
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
                                                 
   numlines = loadfile("culture.str", lines);
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
   int rnd(), IsInd();
   extern struct ecpdef ecp[];
   extern int uwpset;
   extern char uwp[];
   
   ecp[0].value = rnd(6) + rnd(6); /* collective */
   ecp[1].value = rnd(6) + rnd(6); /* social mobility */
   ecp[2].value = rnd(6); /* physical mobility */
   if (uwp[8] > '8') {
      ecp[2].value += 3;
   }
   if (IsInd(uwp)) {
      ecp[2].value++;
   }
   ecp[3].value = rnd(6) + rnd(6); /* worldview */
   ecp[4].value = rnd(6); /* emontionalism */
   ecp[5].value = rnd(6) + rnd(6); /* gender discrim */
   ecp[6].value = rnd(6); /* wealth acquired */
   ecp[7].value = rnd(6); /* wealth distributed */
   ecp[8].value = rnd(6) + rnd(6); /* privacy */
   ecp[9].value = rnd(6); /* bodyzone */
   ecp[10].value = rnd(6) + rnd(6); /* modesty */
   ecp[11].value = rnd(6) + rnd(6) + rnd(6); /* human life */
   ecp[12].value = rnd(6) + rnd(6); /* other life */
   ecp[13].value = rnd(6) + rnd(6); /* child rearing */
   ecp[14].value = rnd(6) + rnd(6); /* honesty */
   ecp[15].value = rnd(6); /* work ethic */
   ecp[16].value = rnd(6) + rnd(6); /* time */
}

void DisplayECP()
{
	void OutputCode(), OutputString();
   char *itoa();
   extern struct strdef string[];
   extern struct ecpdef ecp[];
   extern int numstrings;
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
   void OutputString(), OutputCode(), OutputCodeStub();
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

int IsInd(uwp)
char uwp[];
{
   extern int uwpset;
   int ind = FALSE;
   
   if (uwpset) {
      if ((uwp[2] == '2') || (uwp[2] == '3') || (uwp[2] == '4'))
         ind = TRUE;
      if ((uwp[2] == '7') || (uwp[2] == '9'))
         ind = TRUE;
      
      if (ind && (uwp[4] > '8')) {
         ind = TRUE;
      } else {
         ind = FALSE;
      }
   }
   return(ind);
}

void GenerateUWP()
{
   char itohexc();
   int rnd(), hexctoi();
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

void OutputString(txt)
char *txt;
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
