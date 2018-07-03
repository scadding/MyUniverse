/*
 *  file: options.cpp
 *  class: options
 *
 *   description:
 *
 *
 *    copyright:
*/


#include "options.h"

/*
 *    function: options::options
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
options::options()
{


}

/*
 *    function: options::~options
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
options::~options()
{


}

/*
 *    function: options::options
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
options::options(int argc, char **argv)
{
    addCmdLineOptions(argc, argv);

}


/*
 *    function: options::options
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
options::options(char *filename)
{
    readFile(filename);

}


/*
 *    function: options::readFile
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
int options::readFile(char *filename)
{
    return(0);
}


/*
 *    function: options::addCmdLineOptions
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
int options::addCmdLineOptions(int argc, char **argv)
{
    int i;

    for(i = 0; i < argc; i++) {
        fprintf(stderr, "argument [%d] = %s\n", i, argv[i]);
    }
    return(0);
}

