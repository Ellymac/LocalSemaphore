#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "sys.h"

int main(int argc, char *argv[]){
    int v = atoi(argv[1]);
    int statut = 0;
    statut = syscall(335);
    if (statut == -1){
        fprintf(stderr, "%s : sc336 : %s\n", argv[0], strerror(errno));
        exit(1);
    }
    printf("oui\n");
    statut = syscall(335,v);
    if (statut == -1){
        fprintf(stderr, "%s : sc335 : %s\n", argv[0], strerror(errno));
        exit(1);
    }
    else{
        printf("%d +1 = %d\n", v, statut);
     
    }
    return 0;

}