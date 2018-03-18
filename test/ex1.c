#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "sys.h"

int main(int argc, char* argv) {
  int sem, nbWaiting;

  // init semaphores
  if(sem = syscall(337,2) == -1) {
    perror("sem_initialize");
    exit(1);
  }
  else
    printf("Init semaphore with id : %d\n", sem);

  // debug -> waitlist empty
  if(nbWaiting = syscall(341, sem) == -1) {
    perror("sem_debug");
    exit(1);
  }
  else
    printf("Number of process in waitinglist : %d\n", nbWaiting);

  // acquire semaphore
  if(syscall(339,sem) != -1)
    printf("Semaphore %d was acquired\n", sem);
  else
    perror("sem_acquire call");

  // debug -> resource blocked
  if(nbWaiting = syscall(341, sem) == -1) {
    perror("sem_debug");
    exit(1);
  }
  else
    printf("Number of process in waitinglist : %d\n", nbWaiting);

  // release semaphore
  if(syscall(340,sem) != -1)
    printf("Semaphore %d was released\n", sem);
  else
    perror("sem_release call");

  // debug -> resource released
  if(nbWaiting = syscall(341, sem) == -1) {
    perror("sem_debug");
    exit(1);
  }
  else
    printf("Number of process in waitinglist : %d\n", nbWaiting);

  // destruct semaphore
  printf("Semaphore %d will be destroyed\n", sem);
  if(syscall(338,sem) != -1)
    printf("Semaphore has been destroyed.\n");
  else
    perror("sem_destroy call");

  // debug -> error because semaphore sem not found (has been destroyed)
  if(nbWaiting = syscall(341, sem) == -1) {
    perror("sem_debug");
    exit(1);
  }
  else
    printf("Should not print that line -> error because semaphore destroyed.\n");

  return 0;
}
