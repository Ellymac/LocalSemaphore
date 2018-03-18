#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "sys.h"

int main(int argc, char *argv[])
{
    int statut = 0;
    statut = syscall(336);
    if (statut == -1)
    {
        fprintf(stderr, "%s : sc336 : %s\n", argv[0], strerror(errno));
        exit(1);
    }
    printf("Envoi d'un signal de réveil\n");
    return 0;
}