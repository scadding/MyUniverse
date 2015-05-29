/*
 *  file: queue.cpp
 *  class: queue
 *
 *   description:
 *
 *
 *    copyright:
*/


#include "queue.h"


/*
 *    function: queue::queue
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/

#if 1
queue::queue()
{
	//fprintf(stderr, "queue\n");
	sem_init(&sem, 0, 1);
	head = NULL;
	tail = NULL;
	sem_wait(&sem);
	sem_post(&sem);
	count = 0;
	/* create list of fresh elements to use */
	fresh = NULL;
	//refresh(1000);
	//fprintf(stderr, "queue end\n");
}

/*
 *    function: queue::refresh
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
void queue::refresh(int count)
{
	int i;
	element *ptr;

	if(fresh == NULL){
		fresh = new element;
		fresh->next = NULL;
	}
	
	ptr = fresh;

	while(ptr->next != NULL) ptr = ptr->next;

	for(i = 0;i < count;i++){
		ptr->next = new element;
		ptr = ptr->next;
		ptr->next = NULL;
	}
}

/*
 *    function: queue::~queue
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
queue::~queue()
{
	//fprintf(stderr, "queue\n");
	sem_wait(&sem);
	sem_post(&sem);
	//fprintf(stderr, "queue\n");
}

/*
 *    function: queue::get
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
int queue::get(char *buffer)
{
	//fprintf(stderr, "queue get\n");
	element *e;
	int size = -1;
	
	if(sem_trywait(&sem) == -1){
		return(0);
	}
	if(head != NULL){
		e = head;
		head = e->next;
		if(head == NULL){
			tail = NULL;
		}
		//fprintf(stderr, "get - %s[%d]\n", e->buffer, e->len);
		size = e->len;
		memcpy(buffer, e->buffer, e->len);
		delete e->buffer;
		e->next = fresh;
		fresh = e;
		count--;
	}
	
	sem_post(&sem);
	//fprintf(stderr, "queue get end\n");
	return(size);
}

/*
 *    function: queue::getwait
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
int queue::getwait(char *buffer)
{
	//fprintf(stderr, "queue getwait\n");
	element *e;
	int size = -1;
	
	while(1){
		sem_wait(&sem);
		if(head != NULL){
			e = head;
			head = e->next;
			if(head == NULL){
				tail = NULL;
			}
			size = e->len;
			memcpy(buffer, e->buffer, e->len);
			delete e->buffer;
			e->next = fresh;
			fresh = e;
			count--;
			sem_post(&sem);
			return(size);
		}
		sem_post(&sem);
	}
	//fprintf(stderr, "queue getwait end\n");
	return(size);
}

/*
 *    function: queue::get
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
char *queue::get()
{
	//fprintf(stderr, "queue get\n");
	element *e;
	char *ptr = NULL;
	
	if(sem_trywait(&sem) == -1){
		return(NULL);
	}
	if(head != NULL){
		e = head;
		head = e->next;
		if(head == NULL){
			tail = NULL;
		}
		//fprintf(stderr, "get - %s[%d]\n", e->buffer, e->len);
		ptr = e->buffer;
		e->next = fresh;
		fresh = e;
		count--;
	}
	
	sem_post(&sem);
	//fprintf(stderr, "queue get end\n");
	return(ptr);
}

/*
 *    function: queue::getwait
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
char *queue::getwait()
{
	//fprintf(stderr, "queue getwait\n");
	element *e;
	char *ptr = NULL;
	
	while(1){
		sem_wait(&sem);
		if(head != NULL){
			e = head;
			head = e->next;
			if(head == NULL){
				tail = NULL;
			}
			ptr = e->buffer;
			e->next = fresh;
			fresh = e;
			count--;
		}
		sem_post(&sem);
	}
	//fprintf(stderr, "queue getwait end\n");
	return(ptr);
}

/*
 *    function: queue::put
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/

int queue::put(char *buffer, int s)
{
	//fprintf(stderr, "queue.put\n");
	element *e;
	
	sem_wait(&sem);
	if(fresh == NULL) refresh(1000);
	e = fresh;
	fresh = fresh->next;

	e->buffer = new char[s];
	e->len = s;
	e->next = NULL;
	memcpy(e->buffer, buffer, s);
	if(head == NULL){
		head = e;
	} else {
		tail->next = e;
	}
	tail = e;
	count++;
	
	//fprintf(stderr, "put - %s\n", tail->buffer);
	
	sem_post(&sem);
	//fprintf(stderr, "queue.put end\n");
	return(s);
}


/*
 *    function: queue::put
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/

int queue::put(char *buffer)
{
	//fprintf(stderr, "queue.put\n");
	element *e;
	
	sem_wait(&sem);
	if(fresh == NULL) refresh(1000);
	e = fresh;
	fresh = fresh->next;

	e->buffer = buffer;
	e->next = NULL;
	if(head == NULL){
		head = e;
	} else {
		tail->next = e;
	}
	tail = e;
	count++;
	
	//fprintf(stderr, "put - %s\n", tail->buffer);
	
	sem_post(&sem);
	//fprintf(stderr, "queue.put end\n");
	return(0);
}

/*
 *    function: queue::setQ
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
void queue::setQ(int set)
{

}

/*
 *    function: queue::getQ
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
int queue::getQ()
{
	return(q);
}

#endif
#if 0
/*
 *    function: queue::queue
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
queue::queue()
{
	//struct msqid_ds ds;
	//key_t ftok (const char *path, int id);
	q = msgget(IPC_PRIVATE, IPC_CREAT | 0660);
	if(q == -1){
		fprintf(stderr, "queue error - %s\n", strerror(errno));
		exit(0);
	}
}

/*
 *    function: queue::~queue
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
queue::~queue()
{


}

/*
 *    function: queue::get
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
int queue::get(char *buffer)
{
	int s = msgrcv(q, buffer, 1024, 0, IPC_NOWAIT); 
	if((s == -1)&&(errno != EAGAIN)&&(errno != ENOMSG)){
		fprintf(stderr, "get error - [%d]%s\n", errno, strerror(errno));
		exit(0);
	}
	return(s);
}

/*
 *    function: queue::getwait
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
int queue::getwait(char *buffer)
{
	int s = msgrcv(q, buffer, 1024, 0, 0); 
	if(s == -1){
		fprintf(stderr, "getwait error - %s\n", strerror(errno));
		exit(0);
	}
	return(s);
}

/*
 *    function: queue::put
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
int queue::put(char *buffer, int s)
{
	int j = msgsnd(q, buffer, s, 0);
	if(j == -1){
		fprintf(stderr, "put error - %s\n", strerror(errno));
		exit(0);
	}
	return(0);
}

/*
 *    function: queue::setQ
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
void queue::setQ(int set)
{

}

/*
 *    function: queue::getQ
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
int queue::getQ()
{
	return(q);
}


#endif

