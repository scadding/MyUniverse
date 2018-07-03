#include <stdio.h>
#include <stdlib.h>
#include "sysgen.h"
#include <time.h>
#include "anyoption.h"

void printSystem(long x, long y, long z, int detail)
{
    switch(detail) {
    case 1:
        sysgen1Main(x, y, z);
        break;
    case 2:
        sysgen2Main(x, y, z);
        break;
    case 3:
        sysgen3Main(x, y, z);
        break;
    default:
        break;
    }
}

int   main(int argc, char **argv)
{
    AnyOption *opt = new AnyOption();

    opt->addUsage("");
    opt->addUsage("Usage: ");
    opt->addUsage("");
    opt->addUsage(" -h  --help          Print usage ");
    opt->addUsage(" -L  --subsecLet     Letter of Subsector (A-P) to generate, if omitted will generate entire sector ");
    opt->addUsage(" -d  --detail       %|zero|rift|sparse|scattered|dense ");
    opt->addUsage(" -m  --maturity      Tech level, backwater|frontier|mature|cluster ");
    opt->addUsage(" -a  --ac            Two-letter system alignment code ");
    opt->addUsage(" -s  --secName       Name of sector. For default output file name and sectorName_names.txt file");
    opt->addUsage(" -p  --path          Path to sectorName_names.txt file ");
    opt->addUsage(" -o  --outFormat     1|2|3|4|5|6 : v1.0, v2.0, v2.1 v2.1b, v2.2, v2.5 ");
    opt->addUsage(" -u  --outPath       Path and name of output file ");
    opt->addUsage("");

    opt->setCommandFlag("main", 'm');
    opt->setCommandFlag("system", 'S');
    opt->setCommandFlag("sector", 's');
    opt->setCommandFlag("subsector", 'u');
    opt->setCommandFlag("help", 'h');
    opt->setCommandOption("", 'x');
    opt->setCommandOption("", 'y');
    opt->setCommandOption("", 'z');
    opt->setCommandOption("detail", 'd');
    opt->setCommandOption("seed");

    //opt->setVerbose(); /* print warnings about unknown options */
    //opt->autoUsagePrint(true); /* print usage for bad options */

    opt->processCommandArgs(argc, argv);
#if 0
    if(! opt->hasOptions()) {  /* print usage if no options */
        opt->printUsage();
        delete opt;
        return 0;
    }
#endif
    if(opt->getFlag("help") || opt->getFlag('h')) {
        opt->printUsage();
        delete opt;
        return 0;
    }
    //long seed = 12345;
    long x = 0;
    long y = 10;
    long z = 0;
    int detail = 3;
    if(opt->getValue('x') != NULL) {
        x = atol(opt->getValue('x'));
    }
    if(opt->getValue('y') != NULL) {
        y = atol(opt->getValue('y'));
    }
    if(opt->getValue('z') != NULL) {
        z = atol(opt->getValue('z'));
    }
    if(opt->getValue("seed") != NULL) {
        //seed = atol(opt->getValue("seed"));
    }
    if(opt->getValue("detail") != NULL) {
        detail = atol(opt->getValue("detail"));
    }

    long startX, startY, startZ;
    long endX, endY, endZ;
    long sectorX = 8 * 4;
    long sectorY = 10 * 4;
    long sectorZ = 1;
    if(opt->getFlag("sector")) {
        startX = x / sectorX;
        endX = startX + sectorX - 1;
        startY = y / sectorY;
        endY = startY + sectorY - 1;
        startZ = z / sectorZ;
        endZ = startZ + sectorZ - 1;
    } else {
        startX = x;
        endX = x;
        startY = y;
        endY = y;
        startZ = z;
        endZ = z;
    }

    for(int i = startX; i <= endX; i++) {
        for(int j = startY; j <= endY; j++) {
            for(int k = startZ; k <= endZ; k++) {
                printSystem(x, y, z, detail);
            }
        }
    }

}
