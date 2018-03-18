#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv) {
  int sem, status, pid1, pid2, pid3, i, nbWaiting;

  // init and print critical value
  int criticalValue = 42;
  printf("Initial value is %d\n", criticalValue);

  // init semaphore
  if(sem = sem_initialize() == -1) {
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
    sem_acquire(sem);
    write(1,"fils 1 increments value\n",24);
    for(i = 0; i < 1000; i++)
      criticalValue += 1;
    sem_release(sem);
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
      sem_acquire(sem);
      write(1,"fils 3 decrements value\n",24);
      for(i = 0; i < 1000; i++)
        criticalValue -= 1;
      sem_release(sem);
      exit(0);
    }
    else {
      // fils 2
      sem_acquire(sem);
      write(1,"fils 2 increments value\n",24);
      for(i = 0; i < 1000; i++)
        criticalValue += 1;
      sem_release(sem);
      exit(0);
    }
  }

  // pere
  sem_acquire(sem);
  write(1,"pere decrements value\n",22);
  for(i = 0; i < 1000; i++)
    criticalValue -= 1;
  sem_release(sem);

  // waiting for children
  wait(&status);
  wait(&status);

  // print final critical value
  printf("Final value is %d\n", criticalValue);
  return 0;
