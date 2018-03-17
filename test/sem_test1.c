#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int result = 0;
    if((result = syscall(337,2)) != -1)
        printf("Sem id : %d\n", result);
    else
        perror("sem_initialize call");
    if ((result = syscall(337, 2)) != -1)
        printf("Sem id : %d\n", result);
    else
        perror("sem_initialize call");
    if ((result = syscall(337, 2)) != -1)
        printf("Sem id : %d\n", result);
    else
        perror("sem_initialize call");

    /*  if((result = syscall(338,2)) != -1)
        printf("Sem destroy : %d\n", result);
    else
        perror("sem_destroy call");*/
    return 0;
}
