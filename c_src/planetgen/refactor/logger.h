/*
 *  file: logger.h
 *  class: logger
 *
 *   description:
 *
 *
 *    copyright:
*/


#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <ThreadObject.h>
#include <queue.h>
#include <unistd.h>

using namespace std;

class logger : public ThreadObject {
	public:
		logger();
		~logger();
		logger(char *filename);
		int write(char *buffer, int size);
		void run(void);
		queue *getQueue() {return(q);};
	private:
		FILE *outfile;
		queue *q;
	protected:
}; /* logger */

#endif /* LOGGER_H */
