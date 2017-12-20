/*
 *  file: ThreadObject.h
 *  class: ThreadObject
 *
 *   description:
 *
 *
 *    copyright:
*/


#ifndef THREADOBJECT_H
#define THREADOBJECT_H

#include <pthread.h>

using namespace std;

class ThreadObject
{
public:
    ThreadObject();
    virtual ~ThreadObject();
    virtual void run(void) = 0;
    int create();
    static void task(void *arg);
    pthread_t getThread();
private:
    pthread_t thread;
protected:
}; /* ThreadObject */

#endif /* THREADOBJECT_H */
