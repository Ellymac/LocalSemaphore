#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "sys.h"

int main(int argc, char* argv) {
  int sem, nbWaiting;

  // init semaphores
  if(sem = sem_initialize(2) == -1) {
    perror("sem_initialize");
    exit(1);
  }
  else
    printf("Init semaphore with id : %d\n", sem);

  // debug -> waitlist empty
  if((nbWaiting = sem_dbg(sem)) == -1) {
    perror("sem_debug");
    exit(1);
  }
  else
    printf("Number of process in waitinglist : %d\n", nbWaiting);

  // acquire semaphore
  if(sem_acquire(sem) != -1)
    printf("Semaphore %d was acquired\n", sem);
  else
    perror("sem_acquire call");

  // debug -> resource blocked
  if((nbWaiting = sem_dbg(sem)) == -1) {
    perror("sem_debug");
    exit(1);
  }
  else
    printf("Number of process in waitinglist : %d\n", nbWaiting);

  // release semaphore
  if(sem_release(sem) != -1)
    printf("Semaphore %d was released\n", sem);
  else
    perror("sem_release call");

  // debug -> resource released
  if((nbWaiting = sem_dbg(sem)) == -1) {
    perror("sem_debug");
    exit(1);
  }
  else
    printf("Number of process in waitinglist : %d\n", nbWaiting);

  // destroy semaphore
  printf("Semaphore %d will be destroyed\n", sem);
  if(sem_destroy(sem) != -1)
    printf("Semaphore has been destroyed.\n");
  else
    perror("sem_destroy call");

  // debug -> error EFAULT(14) because semaphore sem not found (has been destroyed)
  if((nbWaiting = sem_dbg(sem)) == 14) {
    perror("sem_debug");
    exit(1);
  }
  else
    printf("Should not print that line -> error because semaphore destroyed.\n");

  return 0;
}
