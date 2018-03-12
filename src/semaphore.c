#include <linux/kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include "semaphore.h"

sem sem_initialize(int nb)
{
    sem t = malloc(sizeof(struct t_sem));
    t->nb_max = nb;
    t->nb_available = nb;
    t->id = 0;
    t->waitlist = malloc(sizeof(int));
    return t;
}

int sem_destroy(int sid)
{
    struct task_struct *c;
    c = current;
    sem s = c->tab_sem[sid];
    int *tab = s->waitlist;
    free(tab);
    free(s);
    return 0;
}

int main(int argc, char *argv[])
{
    sem result = sem_initialize(2);
    printf("%d", result->nb_max);
    return 0;
}