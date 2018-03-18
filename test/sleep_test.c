#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "sys.h"

int main(int argc, char *argv[])
{
    int statut = 0;
    while(1){
        statut = syscall(335);
        if (statut == -1)
        {
            fprintf(stderr, "%s : sc335 : %s\n", argv[0], strerror(errno));
            exit(1);
        }
        printf("Bien reveillé\n");
    }
    return 0;
}