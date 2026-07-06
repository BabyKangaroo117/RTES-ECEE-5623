#include <pthread.h>
#include <sched.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syslog.h>
#include <sys/utsname.h>

typedef struct {
  int threadIdx;
} threadParams_t;

#define NUM_THREADS 128

// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];

atomic_int agsum = 0;

void *sumThreadIDS(void *threadp) {
  threadParams_t *threadParams = (threadParams_t *)threadp;

  atomic_fetch_add(&agsum, threadParams->threadIdx);
  syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:2] Thread idx=%d, sum[1...%d]=%d",
         threadParams->threadIdx, threadParams->threadIdx, agsum);

  return (void *)0;
}

int main(int argc, char *argv[]) {
  struct utsname sysInfo;

  // Not bothering with error checking for this
  uname(&sysInfo);
  openlog("Program Name: Sum Thread IDS - Process ID", LOG_PID | LOG_CONS,
          LOG_USER);

  // Log it like `uname -a`:
  // sysname nodename release version machine
  syslog(LOG_INFO, "%s %s %s %s %s", sysInfo.sysname, sysInfo.nodename,
         sysInfo.release, sysInfo.version, sysInfo.machine);

  for (uint8_t index = 0; index < NUM_THREADS; index++) {

    threadParams[index].threadIdx = index + 1;
    pthread_create(&threads[index], // pointer to thread descriptor
                   (void *)0,       // use default attributes
                   sumThreadIDS,    // thread function entry point
                   (void *)&(threadParams[index]) // parameters to pass in
    );
  }

  for (uint8_t index = 0; index < NUM_THREADS; index++)
    pthread_join(threads[index], NULL);

  printf("TEST COMPLETE\n");

  closelog();
}
