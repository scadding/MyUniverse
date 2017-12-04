#include "sysgen.h"

int    makesystem(syst, x, y, z)
system *syst;
long   x, y, z;
{
       body    *s, *sptr;
       int     roll, tmp;
       world   mw;

       if (!mainwrld(&mw, x, y, z))
               return(0);

       syst->sys = (body *)malloc(sizeof(body));
       syst->x = x;
       syst->y = y;
       syst->z = z;
       s = syst->sys;
       s->name[0] = 0;
       s->orbit = 0;
       s->i = 0;
       s->o = 0;
       s->p = 0;
       s->b = 0;
       primary(s, &mw);
       if (mw.name[0])
               forcemain(s, &mw);
       clean(s);
       syst->mw = getmain(syst->sys);
}

void   primary(s, mw)
body   *s;
world  *mw;
{
       body    *sptr;
       int     roll, tmp;

       /* generate primary */
       s->type = T_STAR;
       roll = D(2);    /* generate star type */
       if (mw)
               if (((mw->atmos >= 4) && (mw->atmos <= 9)) ||
                       (mw->pop >= 8))
                               roll += 4;
       switch (roll)
       {
               case 2:
                       s->upp.s.class = 'A';
                       break;
               case 3:
               case 4:
               case 5:
               case 6:
               case 7:
                       s->upp.s.class = 'M';
                       break;
               case 8:
                       s->upp.s.class = 'K';
                       break;
               case 9:
                       s->upp.s.class = 'G';
                       break;
               case 10:
               case 11:
               case 12:
               default:
                       s->upp.s.class = 'F';
                       break;
       }

       roll = D(2);    /* generate star size */
       if (mw)
               if (((mw->atmos >= 4) && (mw->atmos <= 9)) ||
                       (mw->pop >= 8))
                               roll += 4;
       switch (roll)
       {
               case 2:
                       s->upp.s.size = 2;
                       break;
               case 3:
                       s->upp.s.size = 3;
                       break;
               case 4:
                       s->upp.s.size = 4;
                       break;
               case 5:
               case 6:
               case 7:
               case 8:
               case 9:
               case 10:
                       s->upp.s.size = 5;
                       break;
               case 11:
                       s->upp.s.size = 6;
                       break;
               case 12:
               default:
                       s->upp.s.size = 7;
                       break;
       }
       tmp = roll;     /* save size for later dice mod */
       s->upp.s.decimal = rand()%10;
       roll = staroff(s);
       if ((s->upp.s.size == 6) && (roll < 5))
                       s->upp.s.size = 5;
       else if ((s->upp.s.size == 4) && (roll > 8))
                       s->upp.s.size = 5;

       /* calculate max orbits */
       roll = D(2);
       if (s->upp.s.size <= 2)
               roll += 8;
       else if (s->upp.s.size == 3)
               roll += 4;
       if (s->upp.s.class == 'M')
               roll -= 4;
       else if (s->upp.s.class == 'K')
               roll -= 2;
       if (roll < 1)
               roll = 1;
       s->b = (body *)malloc(sizeof(body));
       s->b->p = s;
       s->b->i = 0;
       s->b->o = 0;
       s->b->b = 0;
       s->b->name[0] = 0;
       s->b->type = T_UNSET;
       s->b->orbit = roll;
       while (--roll >= 0)
       {
               sptr = (body *)malloc(sizeof(body));
               sptr->p = s;
               sptr->b = 0;
               sptr->i = 0;
               sptr->o = s->b;
               s->b->i = sptr;
               sptr->name[0] = 0;
               sptr->type = T_UNSET;
               sptr->orbit = roll;
               s->b = sptr;
       }

       /* calculate number of stars */
       roll = D(2);
       if (roll > 7)
       {       /* binary star */
               addcomp(s, 0, tmp);
               if (roll > 11)
                       addcomp(s, 4, tmp);
       }

       mw = populate(s, mw);
       for (sptr = s->b; sptr; sptr = sptr->o)
               if (sptr->type == T_STAR)
               {
                       if (sptr->orbit < 1000)
                               mw = populate(sptr, mw);
                       else    /* far companion */
                               primary(sptr, mw);
               }
}

void   clean(s)
body   *s;
{
       body    *sptr;

       if (s->name[0] == 0)
               strcpy(s->name, getname());
       for (sptr = s->b; sptr; sptr = sptr->o)
       {
               if ((sptr->type == T_EMPTY) || (sptr->type == T_INSIDE))
               {
                       if (sptr->i)
                               sptr->i->o = sptr->o;
                       else
                               sptr->p->b = sptr->o;
                       if (sptr->o)
                               sptr->o->i = sptr->i;
                       free(sptr);
               }
               else
               {
                       if (sptr->b)
                               clean(sptr);
                       else if (sptr->name[0] == 0)
                               strcpy(sptr->name, getname());
               }
       }
}

body   *getmain(s)
body   *s;
{
       body    *sptr;

       for (sptr = s->b; sptr; sptr = sptr->o)
               if (sptr->type == T_MAIN)
                       return(sptr);
               else if (sptr->b)
               {
                       s = getmain(sptr);
                       if (s)
                               return(s);
               }
       return(0);
}

void   forcemain(s, mw)
body   *s;
mainworld      *mw;
{
       body    *sptr;
       int     orb;
       char    z;

       /* find hab zone */
       for (orb = 0; orb < 13; orb++)
       {
               z = zone(s, orb);
               if ((z == 'H') || (z == 'O'))
                       break;
       }
       if (!s->b)
       {       /* no planets at all! */
               sptr = (body *)malloc(sizeof(body));
               s->b = sptr;
               sptr->i = 0;
               sptr->o = 0;
               sptr->b = 0;
               sptr->p = s;
               sptr->orbit = orb;
               sptr->type = T_MAIN;
               sptr->upp.w = *mw;
               strcpy(sptr->name, mw->name);
               mw->name[0] = 0;
               return;
       }
       /* try to force into empty orbit */
       for (sptr = s->b; sptr; sptr = sptr->o)
               if (sptr->type == T_EMPTY)
               {
                       sptr->type = T_MAIN;
                       strcpy(sptr->name, mw->name);
                       sptr->upp.w = *mw;
                       mw->name[0] = 0;
                       return;
               }
               else if (sptr->type == T_STAR)
               {
                       forcemain(sptr, mw);
                       if (!mw->name[0])
                               return;
               }
       /* force onto last orbit */
       for (sptr = s->b; sptr->o; sptr = sptr->o)
               break;
       sptr->o = (body *)malloc(sizeof(body));
       sptr->o->i = sptr;
       sptr = sptr->o;
       sptr->o = 0;
       sptr->b = 0;
       sptr->p = s;
       sptr->orbit = sptr->i->orbit + 1;
       sptr->type = T_MAIN;
       sptr->upp.w = *mw;
       strcpy(sptr->name, mw->name);
       mw->name[0] = 0;
}
