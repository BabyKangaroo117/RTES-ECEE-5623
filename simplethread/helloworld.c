#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <sys/utsname.h>
#include <syslog.h>

typedef struct
{
    int threadIdx;
} threadParams_t;


void *helloworldThread(void *threadp)
{
  threadParams_t *threadParams = (threadParams_t *)threadp;

  printf("Hello World from Thread!");
  syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello world from Thread");
}

int main (int argc, char *argv[])
{
  struct utsname sysInfo;

  // Not bothering with error checking for this
  uname(&sysInfo);
  openlog("Hello World Thread", LOG_PID | LOG_CONS, LOG_USER);
  
  // Log it like `uname -a`:
  // sysname nodename release version machine
  syslog(LOG_INFO, "%s %s %s %s %s",
         sysInfo.sysname,
         sysInfo.nodename,
         sysInfo.release,
         sysInfo.version,
         sysInfo.machine);

  closelog();

  syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello world from Main");

  pthread_t thread;
  threadParams_t threadParams = { 0 };

  pthread_create(&thread,   // pointer to thread descriptor
                (void *)0,     // use default attributes
                helloworldThread, // thread function entry point
                (void *)&(threadParams) // parameters to pass in
                );
   

   printf("TEST COMPLETE\n");
}
