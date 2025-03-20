#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sched.h>
#include <stdatomic.h>
#include <syslog.h>
#include <unistd.h>

#define NUM_THREADS 128
#define NUM_CPUS 4 

typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];

// Attributes that are init and set then used during thread creation
//
pthread_t threads[NUM_THREADS];
pthread_t mainthread;
pthread_t startthread;
threadParams_t threadParams[NUM_THREADS];

pthread_attr_t fifo_sched_attr;
pthread_attr_t orig_sched_attr;
struct sched_param fifo_param;

// First in first out policy used for real time priority based system
#define SCHED_POLICY SCHED_FIFO

void print_scheduler(void)
{
   // Schedule type for the current running process
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

   // Initialize with default values 
   pthread_attr_init(&fifo_sched_attr);
   // Threads using the fifo_sched_attr object inherit its scheduling attributes
   // This can be seen below, pthread_create passes in fifo_sched_attr.
   pthread_attr_setinheritsched(&fifo_sched_attr, PTHREAD_EXPLICIT_SCHED);
   // Set sched policy to FIFO
   pthread_attr_setschedpolicy(&fifo_sched_attr, SCHED_POLICY);
   // cpuset is a data structure that represents a set of CPUs
   // CPU_ZERO clears the set so it contains no CPUs
   CPU_ZERO(&cpuset);
   cpuidx=(3);
   // Add cpu to the set
   CPU_SET(cpuidx, &cpuset);
   // Sets the affinity mask attribute of the fifo_sched_attr to the value specified in cpuset 
   
   pthread_attr_init(&fifo_sched_attr);
   pthread_attr_setinheritsched(&fifo_sched_attr, PTHREAD_EXPLICIT_SCHED);
   pthread_attr_setschedpolicy(&fifo_sched_attr, SCHED_POLICY);
   CPU_ZERO(&cpuset);
   cpuidx=(1);
   CPU_SET(cpuidx, &cpuset);
   pthread_attr_setaffinity_np(&fifo_sched_attr, sizeof(cpu_set_t), &cpuset);

   max_prio=sched_get_priority_max(SCHED_POLICY);
   fifo_param.sched_priority=max_prio;    
   // Set the current pthread to the SCHED_POLICY at max prio
   if((rc=sched_setscheduler(getpid(), SCHED_POLICY, &fifo_param)) < 0)
      perror("sched_setscheduler");
   // Sets the scheduling params of the fifo_sched_attr

   if((rc=sched_setscheduler(getpid(), SCHED_POLICY, &fifo_param)) < 0)
      perror("sched_setscheduler");

   pthread_attr_setschedparam(&fifo_sched_attr, &fifo_param);

   printf("ADJUSTED "); print_scheduler();
}

// Safer ATOMIC global
atomic_int agsum=0;

/*
 * Add the ID of the thread to the global sum
 */
void *sumThread(void *threadp)
{
    int i;
    threadParams_t *threadParams = (threadParams_t *)threadp;
    int threadIdx = threadParams->threadIdx;
    // Increment the global sum by the index value of the thread
    agsum += threadIdx; 
    syslog(LOG_CRIT, "[COURSE:1][ASSIGNMENT:3]: Thread idx=%d, sum[1...%d]=%d, Running on core:%d", threadIdx, threadIdx, agsum, sched_getcpu());
    return (void *)0;
}

/*
 * Entry point of program
 */
int main (int argc, char *argv[])
{
   set_scheduler(); 
   // Run "uname -a" and open a pipe for reading
   FILE *fp = popen("uname -a", "r");
   if (fp == NULL)
   {
      syslog(LOG_ERR, "[COURSE:1][ASSIGNMENT:3] Failed to run uname command");
      return 1;
   }

   char buffer[256];
   // Read command output from internal buffer into input buffer
   if(fgets(buffer, sizeof(buffer), fp) != NULL)
   {
      syslog(LOG_CRIT, "[COURSE:1][ASSIGNMENT:3] %s", buffer);
   }
   
   // Close the pipe
   pclose(fp);
   
   for (int i = 0; i < NUM_THREADS; i++)
	   
   {  
      // Even threads will increment the sum
      threadParams[i].threadIdx=i+1;
      pthread_create(&threads[i],   // pointer to thread descriptor
                     &fifo_sched_attr, // use FIFO RT max priority attributes 
                     (void *)0,     // use default attributes
                     sumThread, // thread function entry point
                     (void *)&(threadParams[i]) // parameters to pass in
                    );
   }
   // Process waits on the threads to complete
   for(int i=0; i<NUM_THREADS; i++)
       pthread_join(threads[i], NULL);

   printf("TEST COMPLETE\n");
}
