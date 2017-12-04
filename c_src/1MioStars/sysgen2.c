#include "sysgen.h"

void   pworld();
void plevel();
void   printsyst();

int    main(argc, argv)
int    argc;
char   **argv;
{
       system  syst;
       long    x, y, z, atol();

       if ((argc < 3) || (argc > 4))
       {
               printf("Usage: %s X Y [Z]\n", argv[0]);
               exit(1);
       }
       x = atol(argv[1]);
       y = atol(argv[2]);
       if (argc == 4)
               z = atol(argv[3]);
       else
               z = 0L;

       if (makesystem(&syst, x, y, z) == 0)
               exit(0);
       printsyst(&syst);
       exit(1);
}

void   printsyst(syst)
system *syst;
{
       printf("%s system\n", syst->mw->name);
       printf("Location %ld,%ld,%ld\n", syst->x, syst->y, syst->z);
       plevel(syst->sys, 0);
}

void plevel(s, depth)
body   *s;
int    depth;
{
       int     i;

       if (!s)
               return;
       for (i = 0; i < depth; i++)
       {
               if (i+1 == depth)
                       printf("%d", s->orbit);
               printf("\t");
       }
       pworld(s);
       for (s = s->b; s; s = s->o)
               plevel(s, depth + 1);
}

void   pworld(s)
body   *s;
{
       switch (s->type)
       {
               case T_STAR:
                       printf("Star %c%d %d       %s\n", s->upp.s.class,
                               s->upp.s.decimal, s->upp.s.size, s->name);
                       break;
               case T_GIANT:
                       if (s->upp.g.size)
                               printf("Large Gas Giant %s\n", s->name);
                       else
                               printf("Small Gas Giant %s\n", s->name);
                       break;
               case T_TOIDS:
                       if (s->p->type == T_GIANT)
                               printf("Ring            %s\n", s->name);
                       else
                               printf("Asteroids       %s\n", s->name);
                       break;
               case T_MAIN:
               case T_WORLD:
                       printf("%c%1x%1x%1x%1x%1x%1x-%x %c%c%c  %c%s\n",
                               s->upp.w.starport, s->upp.w.size,
                               s->upp.w.atmos, s->upp.w.hydro, s->upp.w.pop, s->upp.w.gov, s->upp.w.law,
                               s->upp.w.tech, (s->upp.w.bases&NAVAL_BASE)? 'N': ' ',
                               (s->upp.w.bases&SCOUT_BASE)? 'S' : ' ',
                               (s->upp.w.bases&LOCAL_BASE)? 'M' : ' ',
                               (s->type == T_MAIN) ? '*' : ' ',
                               s->name);
                       break;
       }
}
