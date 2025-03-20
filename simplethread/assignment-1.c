#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>
#define NUM_THREADS 1

typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];

/* 
 * Thread that prints hello world to the syslog
 */
void *helloWorldThread(void *threadp)
{
    threadParams_t *threadParams = (threadParams_t *)threadp;
    syslog(LOG_CRIT, "[Course:1][ASSIGNMENT:1] Hello World from Thread!");
}

/*
 * Entry point of the program
 */
int main (int argc, char *argv[])
{
   // Run "uname -a" and open a pipe for reading
   FILE *fp = popen("uname -a", "r");
   if (fp == NULL)
   {
      syslog(LOG_ERR, "[Course:1][ASSIGNMENT:1] Failed to run uname command");
      return 1;
   }

   char buffer[256];
   // Read command output from internal buffer into input buffer
   if(fgets(buffer, sizeof(buffer), fp) != NULL)
   {
      syslog(LOG_CRIT, "[Course:1][ASSIGNMENT:1] %s", buffer);
   }
   else
   {
      syslog(LOG_ERR, "[Course:1][ASSIGNMENT:1] Failed to run read uname information from pipe");
   }
   
   // Close the pipe
   pclose(fp);
   
   syslog(LOG_CRIT, "[Course:1][ASSIGNMENT:1] Hello World from Main!");

   for(int i=0; i < NUM_THREADS; i++)
   {
       threadParams[i].threadIdx=i;

       pthread_create(&threads[i],   // pointer to thread descriptor
                      (void *)0,     // use default attributes
                      helloWorldThread, // thread function entry point
                      (void *)&(threadParams[i]) // parameters to pass in
                     );

   }
   // Join threads so that parent process waits for all threads to finish before exiting.
   for(int i=0;i<NUM_THREADS;i++)
       pthread_join(threads[i], NULL);

   printf("TEST COMPLETE\n");
}
