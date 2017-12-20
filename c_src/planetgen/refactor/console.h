/*
 *  file: console.h
 *  class: console
 *
 *   description:
 *
 *
 *    copyright:
*/


#ifndef CONSOLE_H
#define CONSOLE_H

#include <unistd.h>
#include <fcntl.h>
#include <ThreadObject.h>

#include <queue.h>

using namespace std;

class console : public ThreadObject
{
public:
    console();
    ~console();
    void run(void);
    queue *getRead()
    {
        return(&read);
    };
    queue *getWrite()
    {
        return(&write);
    };
private:
    queue read;
    queue write;
protected:
}; /* console */

#endif /* CONSOLE_H */
