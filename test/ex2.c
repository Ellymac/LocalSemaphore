#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "sys.h"

int main(int argc, char* argv) {
  int sem, status, pid1, nbWaiting;

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
      printf("Semaphore %d was acquired by child %d\n", sem, getpid());
    else
      perror("sem_acquire call");

    sleep(2); // waiting for parent to acquire too

    // debug -> parent is in the waitlist
    if((nbWaiting = sem_dbg(sem)) == -1) {
      perror("sem_debug");
      exit(1);
    }
    else
      printf("(child acquire) Nb process in waitinglist : %d\n\n", nbWaiting);

    // release semaphore
    if(sem_release(sem) != -1)
      printf("Semaphore %d was released by child %d\n", sem, getpid());
    else
      perror("sem_release call");

    // debug -> empty waitlist, no free resource
    if((nbWaiting = sem_dbg(sem)) == -1) {
      perror("sem_debug");
      exit(1);
    }
    else
      printf("(child release) Nb process in waitinglist : %d\n\n", nbWaiting);

    exit(0);
  }
  else {
    // pere
    sleep(1); // waiting to be sure child will acquire first

    // acquire semaphore
    if(sem_acquire(sem) != -1)
      printf("Semaphore %d was acquired by parent %d\n", sem, getpid());
    else
      perror("sem_acquire call");

    // debug -> empty waitlist, no free resource
    if((nbWaiting = sem_dbg(sem)) == -1) {
      perror("sem_debug");
      exit(1);
    }
    else
      printf("(parent acquire) Nb process in waitinglist : %d\n\n", nbWaiting);

    wait(&status); // waiting for child

    // release semaphore
    if(sem_release(sem) != -1)
      printf("Semaphore %d was released by parent %d\n", sem, getpid());
    else
      perror("sem_release call");

    // debug -> waitlist empty, 1 free resource
    if((nbWaiting = sem_dbg(sem)) == -1) {
      perror("sem_debug");
      exit(1);
    }
    else
      printf("(parent release) Nb process in waitinglist : %d\n\n", nbWaiting);
  }
  return 0;
}
