#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/console.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/nmi.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/interrupt.h>			/* For in_interrupt() */
#include <linux/delay.h>
#include <linux/smp.h>
#include <linux/security.h>
#include <linux/bootmem.h>
#include <linux/syscalls.h>
#include <linux/kexec.h>

#include <asm/uaccess.h>

#include "semaphore.h"

sem sem_initialize(int nb)
{
    sem t = vmalloc(sizeof(struct t_sem));
    t->nb_max = nb;
    t->nb_available = nb;
    t->id = 0;
    t->waitlist = vmalloc(sizeof(int));
    return t;
}

int sem_destroy(int sid)
{
    struct task_struct *c;
    c = current;
    sem s = c->tab_sem[sid];
    int *tab = s->waitlist;
    kfree(tab);
    kfree(s);
    return 0;
}

int main(int argc, char *argv[])
{
    sem result = sem_initialize(2);
    printk(KERN_DEBUG "%d", result->nb_max);
    return 0;
}
