#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv) {
  int sem, status, pid1, nbWaiting;

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
      printf("Semaphore %d was acquired by child\n", sem);
    else
      perror("sem_acquire call");

    sleep(2); // waiting for parent to acquire too

    // debug -> parent is in the waitlist
    if(nbWaiting = syscall(341, sem) == -1) {
      perror("sem_debug");
      exit(1);
    }
    else
      printf("Number of process in waitinglist : %d\n", nbWaiting);

    // release semaphore
    if(syscall(340,sem) != -1)
      printf("Semaphore %d was released by child\n", sem);
    else
      perror("sem_release call");

    // debug -> empty waitlist, no free resource
    if(nbWaiting = syscall(341, sem) == -1) {
      perror("sem_debug");
      exit(1);
    }
    else
      printf("Number of process in waitinglist : %d\n", nbWaiting);

    exit(0);
  }
  else {
    // pere
    sleep(3); // waiting to be sure child will acquire first

    // acquire semaphore
    if(syscall(339,sem) != -1)
      printf("Semaphore %d was acquired by parent\n", sem);
    else
      perror("sem_acquire call");

    wait(&status); // waiting for child

    // release semaphore
    if(syscall(340,sem) != -1)
      printf("Semaphore %d was released by parent\n", sem);
    else
      perror("sem_release call");

    // debug -> waitlist empty, 1 free resource
    if(nbWaiting = syscall(341, sem) == -1) {
      perror("sem_debug");
      exit(1);
    }
    else
      printf("Number of process in waitinglist : %d\n", nbWaiting);
  }
  return 0;
}
