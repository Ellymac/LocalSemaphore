#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "sys.h"

int main(int argc, char *argv[])
{
    int t[3];
    int r;
    
    if((t[0] = syscall(337,1)) != -1)
        printf("Sem id : %d\n", t[0]);
    else
        perror("sem_initialize call");
    if ((t[1] = syscall(337, 2)) != -1)
        printf("Sem id : %d\n", t[1]);
    else
        perror("sem_initialize call");
    if ((t[2] = syscall(337, 3)) != -1)
        printf("Sem id : %d\n", t[2]);
    else
        perror("sem_initialize call");
    syscall(341,t[1]);
    r = syscall(338, t[1]);
    printf("%d\n", r);
    r = syscall(341, t[1]);
    printf("%d\n", r);

    //syscall(341,result);
    /*  if((result = syscall(338,2)) != -1)
        printf("Sem destroy : %d\n", result);
    else
        perror("sem_destroy call");*/
    return 0;
}
