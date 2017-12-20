/*
 *  file: logger.cpp
 *  class: logger
 *
 *   description:
 *
 *
 *    copyright:
*/


#include "logger.h"

/*
 *    function: logger::logger
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
logger::logger()
{


}

/*
 *    function: logger::~logger
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
logger::~logger()
{


}

/*
 *    function: logger::logger
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
logger::logger(char *filename)
{
    outfile = fopen(filename, "wb");
    q = new queue;
    create();
}

/*
 *    function: logger::write
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
int logger::write(char *buffer, int size)
{

    return(0);
}

/*
 *    function: Connection::run
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
void logger::run(void)
{
    int len;
    /* Buffer for data */
    char buffer[1024];

    while(1) {
        //fprintf(stderr, "*");
        if(q != NULL) {
            len = q->get((char *)buffer);
            if(len <= 0) {
                continue;
            }
            fprintf(outfile, "%s", buffer);
        } else {
            usleep(100);
        }
        usleep(5);
    }

}

