#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "sys.h"

int main(int argc, char *argv[])
{
    int result = 0;
    int res;
    if((result = syscall(337,2)) != -1)
      if((res = syscall(341, result)) != -1)
        printf("nb waiting = %d\n", res);
      else
        perror("sem_debug call");  
    else
      perror("sem_initialize call");
    if ((result = syscall(337, 2)) != -1)
      if((res = syscall(341, result)) != -1)
        printf("nb waiting = %d\n", res);
      else
        perror("sem_debug call");
    else
      perror("sem_initialize call");
    if ((result = syscall(337, 2)) != -1)
      if((res = syscall(341, result)) != -1)
        printf("nb waiting = %d\n", res);
      else
        perror("sem_debug call");
    else
      perror("sem_initialize call");

    /*  if((result = syscall(338,2)) != -1)
        printf("Sem destroy : %d\n", result);
    else
        perror("sem_destroy call");*/
    return 0;
}
