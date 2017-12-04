#include "sysgen.h"

int    main(argc, argv)
int    argc;
char   **argv;
{
       mainworld       mw;
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

       if (mainwrld(&mw, x, y, z) == 0)
               exit(0);
       printf("%c%1x%1x%1x%1x%1x%1x-%x %c%c%c %8ld %8ld %8ld %s\n",
               mw.starport, mw.size,
               mw.atmos, mw.hydro, mw.pop, mw.gov, mw.law,
               mw.tech, (mw.bases&NAVAL_BASE)? 'N' : ' ',
               (mw.bases&SCOUT_BASE)? 'S' : ' ',
               (mw.bases&LOCAL_BASE)? 'M' : ' ',
               x, y, z, mw.name);
       exit(1);
}
