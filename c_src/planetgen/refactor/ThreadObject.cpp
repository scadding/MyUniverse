/*
 *  file: ThreadObject.cpp
 *  class: ThreadObject
 *
 *   description:
 *
 *
 *    copyright:
*/


#include "ThreadObject.h"

/*
 *    function: ThreadObject::ThreadObject
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
ThreadObject::ThreadObject()
{


}

/*
 *    function: ThreadObject::~ThreadObject
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
ThreadObject::~ThreadObject()
{


}
#include <stdio.h>
/*
 *    function: ThreadObject::run
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
void ThreadObject::run(void)
{

    fprintf(stderr, "here\n");
}

/*
 *    function: ThreadObject::create
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
int ThreadObject::create()
{
    return(pthread_create(&thread, NULL, (void*(*)(void*))&task, this));
}

/*
 *    function: ThreadObject::*task
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
void ThreadObject::task(void *arg)
{
    ThreadObject *o;
    o = (ThreadObject *)arg;
    o->run();
}

/*
 *    function: ThreadObject::getThread
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
pthread_t ThreadObject::getThread()
{
    return(thread);
}


