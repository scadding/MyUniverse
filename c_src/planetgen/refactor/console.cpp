/*
 *  file: console.cpp
 *  class: console
 *
 *   description:
 *
 *
 *    copyright:
*/


#include "console.h"

/*
 *    function: console::console
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
console::console()
{
	create();
}

/*
 *    function: console::~console
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
console::~console()
{


}

/*
 *    function: console::run
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
void console::run()
{
	int len;
	char buffer[1024];
	char *ptr;
	int test;

	fcntl(0, F_SETFL, O_NONBLOCK);
	while(1){
		//len = write.get((char *)buffer);
		ptr = write.get();
		//fprintf(stderr, "*");
		if(ptr != NULL){
			fprintf(stderr, "%s", ptr);
			delete ptr;
		}
		//rc = recv(0, buffer, 1024, 0);
		if((test = getc(stdin)) >= 0){
			ungetc(test, stdin);
			fgets(buffer, 1024, stdin);
			read.put((char *)buffer, strlen(buffer) + 1);
		}
		//if(!feof(stdin)){
			//scanf("%s\n", buffer);
			//fgets(buffer, 1024, stdin);
		//}
	}
}


