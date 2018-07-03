/*
 *  file: queue.h
 *  class: queue
 *
 *   description:
 *
 *
 *    copyright:
*/


#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include <sys/msg.h>


using namespace std;

class queue
{
public:
    queue();
    ~queue();
    int get(char *buffer);
    int getwait(char *buffer);
    char *get();
    char *getwait();
    int put(char *buffer, int s);
    int put(char *buffer);
    void setQ(int set);
    int getQ();
    int getCount()
    {
        return(count);
    }
    void refresh(int count);
private:
    struct element {
        char *buffer;
        int len;
        struct element *next;
    } *head, *tail, *fresh;
    sem_t sem;
    int q;
    int count;
protected:
}; /* queue */

#endif /* QUEUE_H */
