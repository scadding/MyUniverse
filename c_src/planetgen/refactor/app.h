/*
 *  file: app.h
 *  class: app
 *
 *   description:
 *
 *
 *    copyright:
*/


#ifndef APP_H
#define APP_H

#include <unistd.h>

#include <ThreadObject.h>
#include <queue.h>
#include <console.h>
#include <logger.h>
#include <options.h>
#include <FGTask.h>
#include <semaphore.h>


using namespace std;

class app : public ThreadObject {
	public:
		app();
		~app();
		app(int argc, char **argv);
		void run();
		int setCommand(queue *set);
		queue *getCommand();
		void con(char *s);

	private:
		queue *command;
		console c;
		logger *l;
		options *o;
		sem_t sem;
	protected:
}; /* app */

#endif /* APP_H */
