/* word.c - traveller word generation */
/* Created July 1990 by Dan Corrin (dan@engrg.uwo.ca) */
/* This program contains no copyrighted material, however the data
   files could be considered (c) GDW or DGP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "word.h"


#ifdef unix	/* Specific Unix configurations */
#define HAS_RANDOM

/* systems that don't have random and have to stick with rand */
#if defined(_SEQUENT_) /* ptx */
#undef HAS_RANDOM
#endif

#endif

#ifdef unix

#ifdef HAS_RANDOM
#define randomize()	srandom(time((long)0));
#else
#define randomize()	srand(time((long)0));
#endif

#endif

/* 2 levels of debug for following data input */
#define DEBUG 0
#define DEBUG2 0
/* Trace will allow user to follow which charts and die rolls were used */
#define TRACE 0
/* Define seperate "letters" with a . and syllables with a - */
#define DEFINE 0
/* write to stdout words generated (usually 1) */
#define ECHO 1

#define LINE_LEN	99

/* Use defines so that the original non-unix functions can be called for the
 * non-unix version, and new unix functions can be called for the unix
 * version
 */

#ifdef unix
#define READ_A_LINE(xxx, yyy)	fgets(xxx, LINE_LEN, yyy)

#ifdef HAS_RANDOM
#define d6	(random()%6 + 1)
#else
#define d6	(rand()%6 + 1)
#endif

#else
#define READ_A_LINE(xxx, yyy)	fread(xxx, sizeof(char), LINE_LEN, yyy)
#define d6	(random(6))
#endif

/*	Use a separate define to discard a line, to clarify code */
#define DISCARD_LINE(xyz, zyx)
/*#define READ_A_LINE(xyz, zyx)*/

char basic[6][6][5];
char alternate[6][6][5];
char alt2[6][6][5];

char ic[6][6][6][5];
char v[6][6][6][5];
char fc[6][6][6][5];

char next_table = 'B';
char wordout[73]; /* 6 sylables x 3 (CVC) x 4 (max. length) */

#ifdef __STDC__
void main(int argc, char **argv)
#else
void main(argc, argv)
	int argc;
	char **argv;
#endif
{
   int i;

   randomize();
   if (argc < 2)
      usage(argv);
#if (TRACE)
   fprintf(stderr,"Reading word data from %s\n",argv[1]);
#endif
   read_table(argv);
#if (DEBUG2)
   print_tables();
#endif
   if (argc < 3) do_word(d6);
   for (i=2; i<argc; i++)
      do_word(atoi(argv[i]));
#if (ECHO)
   printf("\n");
#endif
   exit(0);
}

#ifdef __STDC__
void do_word(int syl)
#else
void do_word(syl)
	int syl;
#endif
{
   int i;

   wordout[0] = '\0';
   if (syl == 0) syl = d6;

   for (i=0; i<syl; i++)
      if (next_table == 'B')
         do_syl(1);
      else if (next_table == 'A')
         do_syl(0);
      else
         do_syl(2);
   next_table = 'B'; /* set up for next word */
#if (ECHO)
   printf(" ");
#endif
}

#ifdef __STDC__
void do_syl(int isbasic)
#else
void do_syl(isbasic)
	int isbasic;
#endif
{
   char pattern[3];
   int i,j;

   i = d6-1;
   j = d6-1;
   if (isbasic == 1) {
#if (TRACE)
   fprintf(stderr,"(Basic%d%d)",i+1,j+1);
#endif
      next_table = 'B';
      pattern[0] = basic[i][j][0];
      if ((pattern[1] = basic[i][j][1]) == '*')
         next_table = 'A';
      else if (pattern[1] == '%')
         next_table = '2';
      if ((pattern[2] = basic[i][j][2]) == '*')
         next_table = 'A';
      else if (pattern[2] == '%')
         next_table = '2';
      if (basic[i][j][3] == '*')
         next_table = 'A';
      else if (basic[i][j][3] == '%')
         next_table = '2';
   } else if (isbasic == 2) {
#if (TRACE)
   fprintf(stderr,"(Alt'%d%d)",i+1,j+1);
#endif
      next_table = 'B';
      pattern[0] = alt2[i][j][0];
      if ((pattern[1] = alt2[i][j][1]) == '*')
         next_table = 'A';
      else if (pattern[1] == '%')
         next_table = '2';
      if ((pattern[2] = alt2[i][j][2]) == '*')
         next_table = 'A';
      else if (pattern[2] == '%')
         next_table = '2';
      if (alt2[i][j][3] == '*')
         next_table = 'A';
      else if (alt2[i][j][3] == '%')
         next_table = '2';
   } else {
#if (TRACE)
   fprintf(stderr,"(Alter%d%d)",i+1,j+1);
#endif
      next_table = 'B';
      pattern[0] = alternate[i][j][0];
      if ((pattern[1] = alternate[i][j][1]) == '*')
         next_table = 'A';
      else if (pattern[1] == '%')
         next_table = '2';
      if ((pattern[2] = alternate[i][j][2]) == '*')
         next_table = 'A';
      else if (pattern[2] == '%')
         next_table = '2';
      if (alternate[i][j][3] == '*')
         next_table = 'A';
      else if (alternate[i][j][3] == '%')
         next_table = '2';
   }
   for (i=0; i<3; i++)
      do_element(pattern[i],i);
#if (DEFINE)
   fprintf(stdout,"-");
#endif
}

#ifdef __STDC__
void do_element(char ch, int count)
#else
void do_element(ch, count)
	char ch;
	int count;
#endif
{
   if (ch == 'V')
      do_component(2); /* vowel */
   else if (ch == 'C')
      if (count == 0)
         do_component(1); /* initial consonant */
      else
         do_component(3); /* final consonant */

}

#ifdef __STDC__
void do_component(int which)
#else
void do_component(which)
	int which;
#endif
{
   char output[5];
   int i,j,k,l;

   i = d6-1;
   j = d6-1;
   k = d6-1;

   switch (which) {
      case 1: /* initial consonant */
#if (TRACE)
   fprintf(stderr,"(C%d%d%d)",i+1,j+1,k+1);
#endif
         for (l=0;l<4; l++)
            output[l] = ic[i][j][k][l];
         break;
      case 2: /* vowel */
#if (TRACE)
   fprintf(stderr,"(V%d%d%d)",i+1,j+1,k+1);
#endif
         for (l=0;l<4; l++)
            output[l] = v[i][j][k][l];
         break;
      case 3: /* final consonant */
#if (TRACE)
   fprintf(stderr,"(K%d%d%d)",i+1,j+1,k+1);
#endif
         for (l=0;l<4; l++)
            output[l] = fc[i][j][k][l];
         break;
   }
   for (i=3; i>0; i--)
      if (output[i] != ' ') {
         output[i+1] = '\0';
         break;
      }
   strcat(wordout,output);
#if (ECHO)
   printf("%s",output);
#endif
#if (DEFINE)
   fprintf(stdout,".");
#endif
}

#ifdef __STDC__
void read_table(char **argv)
#else
void read_table(argv)
	char **argv;
#endif
{
   FILE *inp;
   char line[LINE_LEN + 1];
   char test[20];
   int i, j;

   inp = fopen(argv[1],"r");
   if (inp == NULL) {
      fprintf(stderr,"Couldn't open file %s\n",argv[1]);
      usage(argv);
      exit(0);
   }

   do {
       if (!READ_A_LINE(line, inp)) {
	   fprintf(stderr, "Can't find 'Basic' token!\n");
	   exit(1);
       }
       sscanf(line,"%s",test);
   } while (strcmp(test,"Basic") != 0);
   DISCARD_LINE(line, inp);
   for (i=0;i<6;i++) {
       if (!READ_A_LINE(line, inp)) {
	   fprintf(stderr, "Couldn't read Basic/Alternate info\n");
	   exit(1);
       }
       get_syll_line(i,line);
   }
   do {
       if (!READ_A_LINE(line, inp)) {
	   fprintf(stderr, "Can't find 'Initial' token!\n");
	   exit(1);
       }
       sscanf(line,"%s",test);
   } while (strcmp(test,"Initial") != 0);
   DISCARD_LINE(line, inp);
   for (i=0; i<6; i++)
      for (j=0; j<6; j++) {
	  if (!READ_A_LINE(line, inp)) {
	      fprintf(stderr, "Couldn't read syllable data!\n");
	      exit(1);
	  }
	  get_elem_line(i,j,line,inp);
      }
#if (DEBUG)
   fprintf(stderr,"Finished reading in file\n");
#endif
   fclose(inp);
}

#ifdef __STDC__
void get_syll_line(int i, char line[])
#else
void get_syll_line(i, line)
	int i;
	char line[];
#endif
{
   int test;

   sscanf(line,"%d",&test);
   if (test != (i+1))
      fprintf(stderr,"Error in syllable line %d >%s<\n",i,line);
   sscanf(line,"%*d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
      basic[i][0], basic[i][1], basic[i][2], 
      basic[i][3], basic[i][4], basic[i][5],
      alternate[i][0], alternate[i][1], alternate[i][2], 
      alternate[i][3], alternate[i][4], alternate[i][5],
      alt2[i][0], alt2[i][1], alt2[i][2], 
      alt2[i][3], alt2[i][4], alt2[i][5]
      );
#if (DEBUG)
   fprintf(stderr,"line is %s\n",line);
   fprintf(stderr,"basic[%d][0] = %s\n",i,basic[i][0]);
#endif
}

#ifdef __STDC__
void get_elem_line(int i, int j, char line[], FILE *inp)
#else
void get_elem_line(i, j, line, inp)
	int i;
	int j;
	char line[];
	FILE *inp;
#endif
{
   int test;

   sscanf(line,"%d",&test);
#if (DEBUG2)
   fprintf(stderr,"line %d,%d, test is %d against %d\n",
               i,j,test,((i+1)*10+j+1));
#endif
   if (test != (i+1)*10+j+1) {
#if (DEBUG1)
      fprintf(stderr,"Skipping line >%s<\n", line);
#endif
      if (!READ_A_LINE(line, inp)) {
	  fprintf(stderr, "Can't read element line!\n");
	  exit(1);
      }
   }
   sscanf(line,"%d",&test);
   if (test != (i+1)*10+j+1)
      fprintf(stderr,"Error in element line %d,%d >%s<\n",i,j,line);
   sscanf(line,"%*d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
      ic[i][j][0], ic[i][j][1], ic[i][j][2], 
      ic[i][j][3], ic[i][j][4], ic[i][j][5], 
      v[i][j][0], v[i][j][1], v[i][j][2], 
      v[i][j][3], v[i][j][4], v[i][j][5], 
      fc[i][j][0], fc[i][j][1], fc[i][j][2], 
      fc[i][j][3], fc[i][j][4], fc[i][j][5] 
      );
}

#ifdef __STDC__
void print_tables(void)
#else
void print_tables()
#endif
{
   int i,j;

   fprintf(stderr,"\tBasic\t\t\t\t\tAlternate\n");
   fprintf(stderr,"  1    2    3    4    5    6\t\t1    2    3    4    5    6\n");
   for (i=0;i<6;i++)
      fprintf(stderr,"%d %4s %4s %4s %4s %4s %4s\t\t%4s %4s %4s %4s %4s %4s\n",i+1,
         basic[i][0], basic[i][1], basic[i][2], 
         basic[i][3], basic[i][4], basic[i][5],
         alternate[i][0], alternate[i][1], alternate[i][2], 
         alternate[i][3], alternate[i][4], alternate[i][5]
         );
   fprintf(stderr,"(Only four choices on table V, CV, VC, and CVC, append * to use alternate table)\n\n");
   fprintf(stderr,"\tInitial Consonant\t\tVowel\t\tFinal Consonant\n");
   fprintf(stderr,"   1   2   3   4   5   6  \t1   2   3   4   5   6   1   2   3   4   5   6\n");
   for (i=0; i<6; i++) {
      for (j=0;j<6;j++)
         fprintf(stderr,"%0d%0d %4s %4s %4s %4s %4s %4s\t%4s %4s %4s %4s %4s %4s %4s %4s %4s %4s %4s %4s\n",i+1,j+1,
            ic[i][j][0], ic[i][j][1], ic[i][j][2], 
            ic[i][j][3], ic[i][j][4], ic[i][j][5], 
            v[i][j][0], v[i][j][1], v[i][j][2], 
            v[i][j][3], v[i][j][4], v[i][j][5], 
            fc[i][j][0], fc[i][j][1], fc[i][j][2], 
            fc[i][j][3], fc[i][j][4], fc[i][j][5] 
            );
      fprintf(stderr,"\n");
   }

}

#ifdef __STDC__
void usage(char **argv) {
   fprintf(stderr,"Usage: %s wordfile syllables...\n",argv[0]); 
   exit(0);
}
#endif
/*
exit(1);
       }
       sscanf(line,"%s",test);
   } while (strcmp(test,"Basic") != 0);
   DISCARD_LINE(line, inp);
   for (i=0;i<6;i++) {
       if (!READ_A_LINE(line, inp)) {
	   fprintf(stderr, "Couldn't read Basic/Alternate info\n");
	   exit(1);
       }
       get_syll_line(i,line);
   }
   do {
       if (!READ_A_LINE(line, inp)) {
	   fprintf(stderr, "Can't find 'Initial' token!\n");
	   exit(1);
       }
       sscanf(line,"%s",test);
   } while (s

*/