#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sched.h>
#include <unistd.h>
#include <syslog.h>
#define NUM_THREADS 128
#define NUM_CPUS 4 

typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
pthread_t threads[NUM_THREADS];
pthread_t mainthread;
pthread_t startthread;
threadParams_t threadParams[NUM_THREADS];

pthread_attr_t fifo_sched_attr;
pthread_attr_t orig_sched_attr;
struct sched_param fifo_param;

#define SCHED_POLICY SCHED_FIFO
#define MAX_ITERATIONS (1000000)


void print_scheduler(void)
{
    int schedType = sched_getscheduler(getpid());

    switch(schedType)
    {
        case SCHED_FIFO:
            printf("Pthread policy is SCHED_FIFO\n");
            break;
        case SCHED_OTHER:
            printf("Pthread policy is SCHED_OTHER\n");
            break;
        case SCHED_RR:
            printf("Pthread policy is SCHED_RR\n");
            break;
        default:
            printf("Pthread policy is UNKNOWN\n");
    }
}


void set_scheduler(void)
{
    int max_prio, scope, rc, cpuidx;
    cpu_set_t cpuset;

    printf("INITIAL "); print_scheduler();
    // Initialize schedule attributes
    pthread_attr_init(&fifo_sched_attr);
    // Schedule attributes are set to be inherited
    pthread_attr_setinheritsched(&fifo_sched_attr, PTHREAD_EXPLICIT_SCHED);
    // Policy is set to SCHED FIFO
    pthread_attr_setschedpolicy(&fifo_sched_attr, SCHED_POLICY);
    CPU_ZERO(&cpuset);
    cpuidx=(3);
    // Mask CPU set to execute tasks on core 3
    CPU_SET(cpuidx, &cpuset);
    // Set scheduler attributes to CPU core 3
    pthread_attr_setaffinity_np(&fifo_sched_attr, sizeof(cpu_set_t), &cpuset);
    // Max priority of the system
    max_prio=sched_get_priority_max(SCHED_POLICY);
    fifo_param.sched_priority=max_prio;    

    // Set the current thread to SCHED FIFO
    if((rc=sched_setscheduler(getpid(), SCHED_POLICY, &fifo_param)) < 0)
        perror("sched_setscheduler");

    // Set the current thread params to the parameters from above
    pthread_attr_setschedparam(&fifo_sched_attr, &fifo_param);

    printf("ADJUSTED "); print_scheduler();
}


/*
 * Sum the thread IDs from 1 to the current thread
 */

void *counterThread(void *threadp)
{
    int sum=0, i, rc, iterations;
    threadParams_t *threadParams = (threadParams_t *)threadp;
    pthread_t mythread;
    double start=0.0, stop=0.0;
    struct timeval startTime, stopTime;
    
    // Start the timer
    gettimeofday(&startTime, 0);
    start = ((startTime.tv_sec * 1000000.0) + startTime.tv_usec)/1000000.0;


    // Iterate from 0 to ID and take the sum of the value
    for(iterations=0; iterations < MAX_ITERATIONS; iterations++)
    {
        sum=0;
        for(i=1; i < (threadParams->threadIdx)+1; i++)
            sum=sum+i;
    }

    // Stop the timer
    gettimeofday(&stopTime, 0);
    stop = ((stopTime.tv_sec * 1000000.0) + stopTime.tv_usec)/1000000.0;

//    syslog(LOG_CRIT, "\n[COURSE:1][ASSIGNMENT:4]: Thread idx=%d, sum[0...%d]=%d, running on CPU=%d", 
//           threadParams->threadIdx,
//           threadParams->threadIdx, sum, sched_getcpu());
           

    syslog(LOG_CRIT, "\n[COURSE:1][ASSIGNMENT:4]: Thread idx=%d, sum[0...%d]=%d, running on CPU=%d, time=%lf micro seconds", 
           threadParams->threadIdx,
           threadParams->threadIdx, sum, sched_getcpu(),
           stop-start);

}


/*
 * Entry thread from main that creates all other threads
 */
void *starterThread(void *threadp)
{
   int i, rc;

   printf("starter thread running on CPU=%d\n", sched_getcpu());

   for(i=0; i < NUM_THREADS; i++)
   {
       threadParams[i].threadIdx=i;

       pthread_create(&threads[i],   // pointer to thread descriptor
                      &fifo_sched_attr,     // use FIFO RT max priority attributes
                      counterThread, // thread function entry point
                      (void *)&(threadParams[i]) // parameters to pass in
                     );

   }

   for(i=0;i<NUM_THREADS;i++)
       pthread_join(threads[i], NULL);

}


int main (int argc, char *argv[])
{
   int rc;
   int i, j;
   cpu_set_t cpuset;

   set_scheduler();

   CPU_ZERO(&cpuset);

   // get affinity set for main thread
   mainthread = pthread_self();

   // Check the affinity mask assigned to the thread 
   rc = pthread_getaffinity_np(mainthread, sizeof(cpu_set_t), &cpuset);
   if (rc != 0)
       perror("pthread_getaffinity_np");
   else
   {
       printf("main thread running on CPU=%d, CPUs =", sched_getcpu());

       for (j = 0; j < CPU_SETSIZE; j++)
           if (CPU_ISSET(j, &cpuset))
               printf(" %d", j);

       printf("\n");
   }

   // Run "uname -a" and open a pipe for reading
   FILE *fp = popen("uname -a", "r");
   if (fp == NULL)
   {
      syslog(LOG_ERR, "[COURSE:1][ASSIGNMENT:4]: Failed to run uname command");
      return 1;
   }

   char buffer[256];
   // Read command output from internal buffer into input buffer
   if(fgets(buffer, sizeof(buffer), fp) != NULL)
   {
      syslog(LOG_CRIT, "[COURSE:1][ASSIGNMENT:4]: %s", buffer);
   }
   
   // Close the pipe
   pclose(fp);
   
   pthread_create(&startthread,   // pointer to thread descriptor
                  &fifo_sched_attr,     // use FIFO RT max priority attributes
                  starterThread, // thread function entry point
                  (void *)0 // parameters to pass in
                 );

   pthread_join(startthread, NULL);

   printf("\nTEST COMPLETE\n");
}
