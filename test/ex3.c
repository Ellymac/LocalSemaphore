#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "sys.h"

int main(int argc, char* argv) {
  int sem, status, pid1, pid2, pid3, i, nbWaiting;

  // init and print critical value
  int criticalValue = 42;
  printf("Initial value is %d\n", criticalValue);

  // init semaphore
  if(sem = syscall(337,2) == -1) {
    perror("sem_initialize");
    exit(1);
  }

  // fork
  if(pid1 = fork() == -1) {
    perror("fork");
    exit(1);
  }
  if(pid1 == 0){
    // fils 1
    // acquire semaphore
    if(syscall(339,sem) != -1)
      printf("Semaphore %d was acquired by child 1\n", sem);
    else
      perror("sem_acquire call");

    printf("Child 1 increments value\n");
    for(i = 0; i < 1000; i++)
      criticalValue += 1;

    // release semaphore
    if(syscall(340,sem) != -1)
      printf("Semaphore %d was released by child 1\n", sem);
    else
      perror("sem_release call");
    exit(0);
  }

  if(pid2 = fork() == -1) {
    perror("fork");
    exit(1);
  }
  if(pid2 == 0) {
    // fils 2
    if(pid3 = fork() == -1) {
      perror("fork");
      exit(1);
    }
    if(pid3 == 0) {
      // fils 3
      // acquire semaphore
      if(syscall(339,sem) != -1)
        printf("Semaphore %d was acquired by child 3\n", sem);
      else
        perror("sem_acquire call");

      printf("Child 3 increments value\n");
      for(i = 0; i < 1000; i++)
        criticalValue -= 1;

      // release semaphore
      if(syscall(340,sem) != -1)
        printf("Semaphore %d was released by child 3\n", sem);
      else
        perror("sem_release call");

      exit(0);
    }
    else {
      // fils 2
      // acquire semaphore
      if(syscall(339,sem) != -1)
        printf("Semaphore %d was acquired by child 2\n", sem);
      else
        perror("sem_acquire call");

      printf("Child 2 increments value\n");
      for(i = 0; i < 1000; i++)
        criticalValue += 1;

      // release semaphore
      if(syscall(340,sem) != -1)
        printf("Semaphore %d was released by child 2\n", sem);
      else
        perror("sem_release call");

      exit(0);
    }
  }

  // pere
  // acquire semaphore
  if(syscall(339,sem) != -1)
    printf("Semaphore %d was acquired by parent\n", sem);
  else
    perror("sem_acquire call");

  printf("Parent decrements value\n");
  for(i = 0; i < 1000; i++)
    criticalValue -= 1;

  // release semaphore
  if(syscall(340,sem) != -1)
    printf("Semaphore %d was released by parent\n", sem);
  else
    perror("sem_release call");

  // waiting for children
  wait(&status);
  wait(&status);

  // print final critical value
  printf("Final value is %d\n", criticalValue);
  return 0;
}
