/*
 *  file: linkedlist.h
 *  class: linkedlist
 *
 *   description:
 *
 *
 *    copyright:
*/


#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <element.h>

using namespace std;

class linkedlist
{
public:
    linkedlist();
    ~linkedlist();
    int push(char(*ptr));
    char *pop();
    int setHead(element *set);
    element *getHead();
    int setTail(element *set);
    element *getTail();
    int setCount(int set);
    int getCount();
private:
    element *head;
    element *tail;
    int count;
protected:
}; /* linkedlist */

#endif /* LINKEDLIST_H */
