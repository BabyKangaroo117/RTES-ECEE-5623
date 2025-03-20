#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 128
typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];

// Safer ATOMIC global
atomic_int agsum=0;

/*
 * Adds the thread index to the global sum
 */
void *sumThread(void *threadp)
{
    threadParams_t *threadParams = (threadParams_t *)threadp;
    int threadIdx = threadParams->threadIdx;
    // Increase the global sum by the index of the thread.
    agsum=agsum+threadIdx;

    syslog(LOG_CRIT, "[COURSE:1][ASSIGNMENT:2]: Thread idx=%d, sum[1...%d]=%d", threadIdx, threadIdx, agsum);
    return (void *)0;
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
      syslog(LOG_ERR, "[COURSE:1][ASSIGNMENT:2]: Failed to run uname command");
      return 1;
   }

   char buffer[256];
   // Read command output from internal buffer into input buffer
   if(fgets(buffer, sizeof(buffer), fp) != NULL)
   {
      syslog(LOG_CRIT, "[COURSE:1][ASSIGNMENT:2]: %s", buffer);
   }
   
   // Close the pipe
   pclose(fp);
   
   for (int i = 0; i < NUM_THREADS; i++)
   { 
      // Start from ID 1
      threadParams[i].threadIdx=i+1;
      pthread_create(&threads[i],   // pointer to thread descriptor
                     (void *)0,     // use default attributes
                     sumThread, // thread function entry point
                     (void *)&(threadParams[i]) // parameters to pass in
                    );
   }
   
   // Join threads so that parent process waits until all threads have completed.
   for(int i=0; i<NUM_THREADS; i++)
       pthread_join(threads[i], NULL);

   printf("TEST COMPLETE\n");
}
