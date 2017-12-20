
/*
 *  file: app.cpp
 *  class: app
 *
 *   description:
 *
 *
 *    copyright:
*/


#include "app.h"

/*
 *    function: app::app
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
app::app()
{
    o = new options(0, NULL);
    sem_init(&sem, 0, 1);
    sem_wait(&sem);
    create();

    sem_wait(&sem);
}

/*
 *    function: app::~app
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
app::~app()
{


}

/*
 *    function: app::app
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
app::app(int argc, char **argv)
{
    o = new options(argc, argv);
    sem_init(&sem, 0, 1);
    sem_wait(&sem);
    create();
    sem_wait(&sem);
}

/*
 *    function: app::run
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
void app::run()
{
    char *temp;
    char *s;
    int i = 0;

    while(1) {
        i++;
        s = new char[16];
        sprintf(s, "time %d\r", i);
        con(s);
        if((temp = c.getRead()->get()) != NULL) {
            if(strncmp(temp, "quit", 4) == 0) {
                fprintf(stderr, "\n\n%s\n\n", temp);
                break;
            }
            delete temp;
        }
        usleep(1000);
    }


    /* release the reset sem */
    sem_post(&sem);
}

void app::con(char *s)
{
    c.getWrite()->put(s, strlen(s) + 1);
};


/*
 *    function: app::setCommand
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
int app::setCommand(queue *set)
{
    return(0);
}

/*
 *    function: app::*getCommand
 *    inputs:
 *    output:
 *
 *    description:
 *
 *
*/
queue *app::getCommand()
{
    return(command);
}



int main(int argc, char **argv)
{
    app app(argc, argv);

    fprintf(stderr, "exiting\n");
    /* need to exit to kill the running tasks */
    exit(0);
}


