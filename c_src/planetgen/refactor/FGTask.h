/*
 *  file: FGTask.h
 *  class: FGTask
 *
 *   description:
 *
 *
 *    copyright:
*/


#ifndef FGTASK_H
#define FGTASK_H

#include <linkedlist.h>
#include <ThreadObject.h>
#include <func.h>

using namespace std;

class FGTask : public ThreadObject
{
public:
    FGTask();
    ~FGTask();
    int setInQueue(linkedlist set);
    linkedlist getInQueue();
    int setOutQueue(linkedlist set);
    linkedlist getOutQueue();
    int init();
    int setTrackRequired(int set);
    int getTrackRequired();
    int setRoute(int set);
    int getRoute();
    int setProcess(func *set);
    func *getProcess();
    int setId(long set);
    long getId();
private:
    linkedlist inQueue;
    linkedlist outQueue;
    int trackRequired;
    int route;
    func *process;
    long id;
protected:
}; /* FGTask */

#endif /* FGTASK_H */
