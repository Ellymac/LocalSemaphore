#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

#include "sys.h"

static int *criticalValue;

int main(int argc, char* argv) {
  int sem, status, pid1, pid2, pid3, i, nbWaiting;

  // init and print critical value
  criticalValue = mmap(NULL, sizeof *criticalValue, PROT_READ | PROT_WRITE,
                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  *criticalValue = 42;
  printf("Initial value is %d\n", *criticalValue);

  // init semaphore
  if(sem = sem_initialize(1) == -1) {
    perror("sem_initialize");
    exit(1);
  }

  // fork
  if((pid1 = fork()) == -1) {
    perror("fork");
    exit(1);
  }
  if(pid1 == 0){
    // fils 1
    // acquire semaphore
    if(sem_acquire(sem) != -1)
      printf("Semaphore %d was acquired by child 1 (%d)\n", sem, getpid());
    else
      perror("sem_acquire call");
    sleep(2);
    printf("Child 1 (%d) increments value\n", getpid());
    for(i = 0; i < 1000; i++)
      *criticalValue += 1;
    printf("Value is %d\n", *criticalValue);

    // debug
    if((nbWaiting = sem_dbg(sem)) == -1) {
      perror("sem_debug");
      exit(1);
    }

    // release semaphore
    if(sem_release(sem) != -1)
      printf("Semaphore %d was released by child 1 (%d)\n", sem, getpid());
    else
      perror("sem_release call");

      // debug
      if((nbWaiting = sem_dbg(sem)) == -1) {
        perror("sem_debug");
        exit(1);
      }
    exit(0);
  }
  else{
    if ((pid2 = fork()) == -1)
    {
      perror("fork");
      exit(1);
    }
    if (pid2 == 0)
    {
      // fils 2
      sleep(1);
      // acquire semaphore
      if (sem_acquire(sem) != -1)
        printf("Semaphore %d was acquired by child 2 (%d)\n", sem, getpid());
      else
        perror("sem_acquire call");

      printf("Child 2 (%d) increments value\n", getpid());
      for (i = 0; i < 1000; i++)
        *criticalValue += 1;
      printf("Value is %d\n", *criticalValue);

      // debug
      if((nbWaiting = sem_dbg(sem)) == -1) {
        perror("sem_debug");
        exit(1);
      }

      // release semaphore
      if (sem_release(sem) != -1)
        printf("Semaphore %d was released by child 2 (%d)\n", sem, getpid());
      else
        perror("sem_release call");

        // debug
        if((nbWaiting = sem_dbg(sem)) == -1) {
          perror("sem_debug");
          exit(1);
        }

      exit(0);
    }
    else{
      if ((pid3 = fork()) == -1)
      {
        perror("fork");
        exit(1);
      }
      if (pid3 == 0)
      {
        // fils 3
        sleep(1);
        // acquire semaphore
        if (sem_acquire(sem) != -1)
          printf("Semaphore %d was acquired by child 3 (%d)\n", sem, getpid());
        else
          perror("sem_acquire call");

        printf("Child 3 (%d) decrements value\n", getpid());
        for (i = 0; i < 1000; i++)
          *criticalValue -= 1;
        printf("Value is %d\n", *criticalValue);

        // debug
        if((nbWaiting = sem_dbg(sem)) == -1) {
          perror("sem_debug");
          exit(1);
        }
        else
          printf("Number of process in waitinglist : %d\n", nbWaiting);

        // release semaphore
        if (sem_release(sem) != -1)
          printf("Semaphore %d was released by child 3 (%d)\n", sem, getpid());
        else
          perror("sem_release call");

          // debug
          if((nbWaiting = sem_dbg(sem)) == -1) {
            perror("sem_debug");
            exit(1);
          }

        exit(0);
      }
      else{
        // pere
        sleep(1);
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
      }
    }
  }



  return 0;
}
