/* Universal Hazard Profile */
/* Challenge 31*/
/* GDW/Marcus Rowland */
/* Computer version by David Burden */
/* v0.21 5 aug 90 */
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
#define NUMPARAM 9
#define UHPNATURE 0
#define UHPSUBCLASS 1
#define UHPSPECIES 2
#define UHPATMOS 3
#define UHPTEMP 4
#define UHPHUMID 5
#define UHPGRAV 6
#define UHPFORM 7
#define UHPMASS 8
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
struct uhpdef {
   int value;
   int idx;
   char code[4];
   char hex[2];
} uhp[] = {
   0, 0, "na", " ",
   0, 0, "sc", " ",
   0, 0, "sp", " ",
   0, 0, "at", " ",
   0, 0, "tm", " ",
   0, 0, "hu", " ",
   0, 0, "gv", " ",
   0, 0, "fm", " ",
   0, 0, "ms", " "
};
int filewrite = FALSE;
int suppress = FALSE;
int runs = 1;
char uwp[9];
int uhpset = FALSE;
   
int main(argc, argv)
char *argv[];
int argc;
{
   int LoadText();
   void OutputCode(), GenerateUHP(), DisplayUHP();
   void OutputString();
   int Arguements();
   extern int filewrite, runs, uhpset;
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
            if (!uhpset)
            	GenerateUHP();
            DisplayUHP();
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
      	puts("\nError: Cannot load HAZARD.STR text file!\n");
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
                                                 
   numlines = loadfile("hazard.str", lines);
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

void GenerateUHP()
{
   int rnd();
   char itohexc();
   extern struct uhpdef uhp[];
   char *p;
   
   uhp[0].value = rnd(8) - 1; /* type */
   uhp[0].hex[0] = itohexc(uhp[0].value);
   
   switch(uhp[0].value) { 
   
      case 0:
         uhp[1].value = 0; 
         uhp[1].hex[0] = '0';
         p = strcpy(uhp[1].code, "sh"); 
         break;
         
      case 1:
         uhp[1].value = rnd(9) - 1;
         uhp[1].hex[0] = itohexc(uhp[1].value);
         p = strcpy(uhp[1].code, "tx"); 
         break;
         
      case 2:
         uhp[1].value = rnd(10) - 1;
         uhp[1].hex[0] = itohexc(uhp[1].value);
         p = strcpy(uhp[1].code, "ox"); 
         break;
         
      case 3:
         uhp[1].value = rnd(15) - 1;
         uhp[1].hex[0] = itohexc(uhp[1].value);
         p = strcpy(uhp[1].code, "co"); 
         break;
         
      case 4:
         uhp[1].value = rnd(10) - 1; 
         uhp[1].hex[0] = itohexc(uhp[1].value);
         p = strcpy(uhp[1].code, "ex"); 
         break;
         
      case 5:
         uhp[1].value = rnd(17) - 1;
         uhp[1].hex[0] = itohexc(uhp[1].value);
         if (uhp[1].value == 16)
            uhp[1].hex[0] = 'X';
         p = strcpy(uhp[1].code, "hi"); 
         break;
         
      case 6:
         uhp[1].value = rnd(10) - 1;
         uhp[1].hex[0] = itohexc(uhp[1].value);
         p = strcpy(uhp[1].code, "bi"); 
         break;
         
      case 7:
         uhp[1].value = rnd(11) - 1;
         uhp[1].hex[0] = itohexc(uhp[1].value);
         p = strcpy(uhp[1].code, "ra"); 
         break;
   }
   uhp[2].value = rnd(10) - 1; /* race */
   uhp[2].hex[0] = itohexc(uhp[2].value);
   uhp[3].value = rnd(15) - 1; /* atmos */
   uhp[3].hex[0] = itohexc(uhp[3].value);
   if (uhp[3].value == 13)
      uhp[3].hex[0] = 'X';
   if (uhp[3].value == 14)
      uhp[3].hex[0] = 'Y';
   uhp[4].value = rnd(18) - 1; /* temp */
   uhp[4].hex[0] = itohexc(uhp[4].value);
   if (uhp[4].value == 16)
      uhp[4].hex[0] = 'X';
   if (uhp[4].value == 17)
      uhp[4].hex[0] = 'Y';
   uhp[5].value = rnd(22) - 1; /* humidity */
   uhp[5].hex[0] = itohexc(uhp[5].value);
   if (uhp[5].value == 21)
      uhp[5].hex[0] = 'X';
   uhp[6].value = rnd(13) - 1; /* gravity */
   uhp[6].hex[0] = itohexc(uhp[6].value);
   if (uhp[6].value == 11)
      uhp[6].hex[0] = 'X';
   if (uhp[6].value == 12)
      uhp[6].hex[0] = 'Y';
   uhp[7].value = rnd(12); /* form */
   uhp[7].hex[0] = itohexc(uhp[7].value);
   uhp[8].value = rnd(19) - 1; /* mass */
   uhp[8].hex[0] = itohexc(uhp[8].value);
}

void DecodeUHP()
{
   int hexctoi();
   extern struct uhpdef uhp[];
   char *p;
   
   uhp[0].value = hexctoi(uhp[0].hex[0]);
   
   switch(uhp[0].value) { /* subcode */
   
      case 0:
         uhp[1].value = 0; 
         p = strcpy(uhp[1].code, "sh"); 
         break;
         
      case 1:
         uhp[1].value = hexctoi(uhp[1].hex[0]);
         p = strcpy(uhp[1].code, "tx"); 
         break;
         
      case 2:
         uhp[1].value = hexctoi(uhp[1].hex[0]);
         p = strcpy(uhp[1].code, "ox"); 
         break;
         
      case 3:
         uhp[1].value = hexctoi(uhp[1].hex[0]);
         p = strcpy(uhp[1].code, "co"); 
         break;
         
      case 4:
         uhp[1].value = hexctoi(uhp[1].hex[0]);
         p = strcpy(uhp[1].code, "ex"); 
         break;
         
      case 5:
         uhp[1].value = hexctoi(uhp[1].hex[0]);
         if (uhp[1].hex[0] == 'X')
            uhp[1].value = 16;
         p = strcpy(uhp[1].code, "hi"); 
         break;
         
      case 6:
         uhp[1].value = hexctoi(uhp[1].hex[0]);
         p = strcpy(uhp[1].code, "bi"); 
         break;
         
      case 7:
         uhp[1].value = hexctoi(uhp[1].hex[0]);
         p = strcpy(uhp[1].code, "ra"); 
         break;
   }
   uhp[2].value = hexctoi(uhp[2].hex[0]); /* race */
   uhp[3].value = hexctoi(uhp[3].hex[0]); /* atmos */
   if (uhp[3].hex[0] == 'X')
      uhp[3].value = 13;
   if (uhp[3].hex[0] == 'Y')
      uhp[3].value = 14;
   uhp[4].value = hexctoi(uhp[4].hex[0]); /* temp */
   if (uhp[4].hex[0] == 'X')
      uhp[4].value = 16;
   if (uhp[4].hex[0] == 'Y')
      uhp[4].value = 17;
   uhp[5].value = hexctoi(uhp[5].hex[0]); /* humidity */
   if (uhp[5].hex[0] == 'X')
      uhp[5].value = 21;
   uhp[6].value = hexctoi(uhp[6].hex[0]); /*12 gravity */
   if (uhp[6].hex[0] == 'X')
      uhp[6].value = 11;
   if (uhp[6].hex[0] == 'Y')
      uhp[6].value = 12;
   uhp[7].value = hexctoi(uhp[7].hex[0]); /*12 form */
   uhp[8].value = hexctoi(uhp[8].hex[0]); /*18  mass */
}

void DisplayUHP()
{
	void OutputCode(), OutputChar();
   char *itoa();
   extern struct strdef string[];
   extern struct uhpdef uhp[];
   extern int numstrings;
   char val[7], target[5], *p;
   int loop;

   OutputChar('U');  
   OutputChar('H');  
   OutputChar('P');  
   OutputChar(':');  
   OutputChar(' ');  
   for (loop = 0; loop < NUMPARAM; loop++) {
      if ((loop == 3) || (loop == 7))
         OutputChar('-');
      OutputChar(uhp[loop].hex[0]);
   }
   OutputString("\n");
   for (loop = 0; loop < NUMPARAM; loop++) {
      /* match line */
      p = strcpy(target, uhp[loop].code);
      (void)itoa(uhp[loop].value, val);
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
   void OutputString(), OutputCode(), OutputCodeStub(), DecodeUHP();
   extern FILE *fp;
	extern struct uhpdef uhp[];
   extern int filewrite, suppress, runs, uhpset;
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
            
            case 'c':
               uhpset = TRUE;
               uhp[0].hex[0] = argv[loop][2];
               uhp[1].hex[0] = argv[loop][3];
               uhp[2].hex[0] = argv[loop][4];
               uhp[3].hex[0] = argv[loop][6];
               uhp[4].hex[0] = argv[loop][7];
               uhp[5].hex[0] = argv[loop][8];
               uhp[6].hex[0] = argv[loop][9];
               uhp[7].hex[0] = argv[loop][11];
               uhp[8].hex[0] = argv[loop][12]; 
               DecodeUHP();
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
