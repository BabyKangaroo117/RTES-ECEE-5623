#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>

#include <stdatomic.h>

#define COUNT  1000
#define NUM_THREADS 128
typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];


// Unsafe global
int gsum=0;

// Safer ATOMIC global
atomic_int agsum=0;

void *incThread(void *threadp)
{
    int i;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    for(i=0; i<COUNT; i++)
    {
        gsum=gsum+i;
        agsum=agsum+i;
       // printf("Increment thread idx=%d, gsum=%d,  agsum=%d\n", threadParams->threadIdx, gsum, agsum);
    }
    int threadIdx = threadParams->threadIdx;
    syslog(LOG_CRIT, "[COURSE:1][ASSIGNMENT:2]: Thread idx=%d, sum[1...%d]=%d", threadIdx, threadIdx, agsum);
    return (void *)0;
}


void *decThread(void *threadp)
{
    int i;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    for(i=0; i<COUNT; i++)
    {
        gsum=gsum-i;
        agsum=agsum-i;
       // printf("Decrement thread idx=%d, gsum=%d, agsum=%d\n", threadParams->threadIdx, gsum, agsum);
    }
    int threadIdx = threadParams->threadIdx;
    syslog(LOG_CRIT, "[COURSE:1][ASSIGNMENT:2]: Thread idx=%d, sum[1...%d]=%d", threadIdx, threadIdx, agsum);

    return (void *)0;
}




int main (int argc, char *argv[])
{
  
   // Run "uname -a" and open a pipe for reading
   FILE *fp = popen("uname -a", "r");
   if (fp == NULL)
   {
      syslog(LOG_ERR, "[Course:1][ASSIGNMENT:2] Failed to run uname command");
      return 1;
   }

   char buffer[256];
   // Read command output from internal buffer into input buffer
   if(fgets(buffer, sizeof(buffer), fp) != NULL)
   {
      syslog(LOG_CRIT, "[Course:1][ASSIGNMENT:2] %s", buffer);
   }
   
   // Close the pipe
   pclose(fp);
   
   for (int i = 0; i < NUM_THREADS; i+=2)
   {  
      // Even threads will increment the sum
      threadParams[i].threadIdx=i;
      pthread_create(&threads[i],   // pointer to thread descriptor
                     (void *)0,     // use default attributes
                     incThread, // thread function entry point
                     (void *)&(threadParams[i]) // parameters to pass in
                    );
      // Odd threads will decrement the sum
      threadParams[i + 1].threadIdx=i + 1;
      pthread_create(&threads[i + 1], (void *)0, decThread, (void *)&(threadParams[i + 1]));
   }

   for(int i=0; i<NUM_THREADS; i++)
       pthread_join(threads[i], NULL);

   printf("TEST COMPLETE\n");
}
