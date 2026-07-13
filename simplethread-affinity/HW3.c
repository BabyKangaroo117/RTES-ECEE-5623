// Create 128 threads with sched fifo and cpu affinity. Each thread adds its id
// to an atomic sum value.

#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syslog.h>
#include <sys/utsname.h>
#include <unistd.h>

typedef struct {
  int threadIdx;
} threadParams_t;

#define NUM_THREADS 128

// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
pthread_t mainthread;
threadParams_t threadParams[NUM_THREADS];

// Get the current process ID and print the scheduler policy
// The policy of the parent process is used by all created threads within the
// process
void print_scheduler(void) {
  int schedType = sched_getscheduler(getpid());

  switch (schedType) {
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

#define SCHED_POLICY SCHED_FIFO // Priority preemptive
struct sched_param fifo_param;
pthread_attr_t fifo_sched_attr;

void set_scheduler(void) {
  int max_prio, scope, rc, cpuidx;
  cpu_set_t cpuset;

  printf("INITIAL ");
  print_scheduler();

  // Initialize the pthread_attr_t object to default values
  pthread_attr_init(&fifo_sched_attr);

  // Inherit values from the current thread and set them in the fifo_sched_attr
  // object
  pthread_attr_setinheritsched(&fifo_sched_attr, PTHREAD_EXPLICIT_SCHED);

  // Set the shed polic in the fifo_sched_attr object
  pthread_attr_setschedpolicy(&fifo_sched_attr, SCHED_POLICY);

  // Zero the cpuset object values
  CPU_ZERO(&cpuset);
  cpuidx = (3);

  // Set the cpu index to core 3
  CPU_SET(cpuidx, &cpuset);

  // Set affinity in the fifo_sched_attr object to the cpuset object values
  pthread_attr_setaffinity_np(&fifo_sched_attr, sizeof(cpu_set_t), &cpuset);

  max_prio = sched_get_priority_max(SCHED_POLICY);
  fifo_param.sched_priority = max_prio;

  // Set the priority to max prio and policy to sched fif of the current running
  // thread
  if ((rc = sched_setscheduler(getpid(), SCHED_POLICY, &fifo_param)) < 0)
    perror("sched_setscheduler");

  // Sets the scheduling parameter attributes of the fifo_sched_attr object to
  // the values specified in the fifo_param object. Determines the params of
  // threads created using the attr.
  pthread_attr_setschedparam(&fifo_sched_attr, &fifo_param);

  printf("ADJUSTED ");
  print_scheduler();
}

atomic_int agsum = 0;

// Add the thread id to the global atomic thread sum
void *sumThreadIDS(void *threadp) {
  threadParams_t *threadParams = (threadParams_t *)threadp;

  atomic_fetch_add(&agsum, threadParams->threadIdx);
  syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:2] Thread idx=%d, sum[1...%d]=%d",
         threadParams->threadIdx, threadParams->threadIdx, agsum);

  return (void *)0;
}

int main(int argc, char *argv[]) {
  struct utsname sysInfo;
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
  else {
    printf("main thread running on CPU=%d, CPUs =", sched_getcpu());

    for (j = 0; j < CPU_SETSIZE; j++)
      if (CPU_ISSET(j, &cpuset))
        printf(" %d", j);

    printf("\n");
  }

  // Not bothering with error checking for this
  uname(&sysInfo);
  openlog("Program Name: Sum Thread IDS - Process ID", LOG_PID | LOG_CONS,
          LOG_USER);

  // Log it like `uname -a`:
  // sysname nodename release version machine
  syslog(LOG_INFO, "%s %s %s %s %s", sysInfo.sysname, sysInfo.nodename,
         sysInfo.release, sysInfo.version, sysInfo.machine);

  // Create new threads that all have sched fifo policy with affinity to CPU 3
  // and max prio Each thread runs to completion once its created since all
  // threads are the same prio. The process preempts all other processes because
  // it is highes prio
  for (uint8_t index = 0; index < NUM_THREADS; index++) {

    threadParams[index].threadIdx = index + 1;
    pthread_create(&threads[index],          // pointer to thread descriptor
                   (void *)&fifo_sched_attr, // use default attributes
                   sumThreadIDS,             // thread function entry point
                   (void *)&(threadParams[index]) // parameters to pass in
    );
  }

  for (uint8_t index = 0; index < NUM_THREADS; index++)
    pthread_join(threads[index], NULL);

  printf("TEST COMPLETE\n");

  closelog();
}
