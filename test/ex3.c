#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

#include "sys.h"

static int *criticalValue;

int main(int argc, char* argv) {
  int sem, status, pid, i, j, nbWaiting;

  // init and print critical value
  criticalValue = mmap(NULL, sizeof *criticalValue, PROT_READ | PROT_WRITE,
                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  // init semaphore
  if(sem = sem_initialize(1) == -1) {
    perror("sem_initialize");
    exit(1);
  }

  *criticalValue = 42;
  printf("Initial value is %d\n", *criticalValue);

  // fork
  for(i = 0; i < 3; i++) {
    if((pid = fork()) == -1) {
      perror("fork");
      exit(1);
    }
    if(pid == 0){
      // child
      // acquire semaphore
      if(sem_acquire(sem) != -1)
        printf("Semaphore %d was acquired by child (%d)\n", sem, getpid());
      else
        perror("sem_acquire call");

      if(i%2 == 0) {
        printf("Child (%d) increments value\n", getpid());
        for(j = 0; j < 1000; j++)
          *criticalValue += 1;
      }
      else {
        printf("Child (%d) decrements value\n", getpid());
        for(j = 0; j < 1000; j++)
          *criticalValue -= 1;
      }

      printf("Value is %d\n", *criticalValue);

      // debug
      if((nbWaiting = sem_dbg(sem)) == -1) {
        perror("sem_debug");
        exit(1);
      }

      // release semaphore
      if(sem_release(sem) != -1)
        printf("Semaphore %d was released by child (%d)\n", sem, getpid());
      else
        perror("sem_release call");

      // debug
      if((nbWaiting = sem_dbg(sem)) == -1) {
        perror("sem_debug");
        exit(1);
      }
      exit(0);
    }
  }
  //parent
  // acquire semaphore
  if (sem_acquire(sem) != -1)
    printf("Semaphore %d was acquired by parent (%d)\n", sem, getpid());
  else
    perror("sem_acquire call");

  printf("Parent (%d) decrements value\n", getpid());
  for (i = 0; i < 1000; i++)
    *criticalValue -= 1;
  printf("Value is %d\n", *criticalValue);

  // debug
  if((nbWaiting = sem_dbg(sem)) == -1) {
    perror("sem_debug");
    exit(1);
  }

  // release semaphore
  if (sem_release(sem) != -1)
    printf("Semaphore %d was released by parent (%d)\n", sem, getpid());
  else
    perror("sem_release call");

    // debug
    if((nbWaiting = sem_dbg(sem)) == -1) {
      perror("sem_debug");
      exit(1);
    }

  // waiting for children
  wait(&status);
  wait(&status);
  wait(&status);

  // print final critical value
  printf("Final value is %d\n", *criticalValue);
  munmap(criticalValue, sizeof *criticalValue);
  return 0;
}
