#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/console.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/nmi.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/interrupt.h> /* For in_interrupt() */
#include <linux/delay.h>
#include <linux/smp.h>
#include <linux/security.h>
#include <linux/bootmem.h>
#include <linux/syscalls.h>
#include <linux/kexec.h>
#include <linux/unistd.h>

#include <stdarg.h>

#include <asm/uaccess.h>

struct task_struct *p;

SYSCALL_DEFINE1(sem_initialize, int, nb)
// int sem_initialize(int nb)
{
  if(nb < 0 || nb > INT_MAX)
    return (EFAULT);
  t_sem *s;
  t_sem_ens *sem_ens;
  t_waitlist *waitlist;
  p = current;

  //init lsem
  if (p->lsem == NULL){
      p->lsem = vmalloc(sizeof(struct t_sem_ens));
      (p->lsem)->nb_sem = 0;
  }
  sem_ens = p->lsem;

  // init s
  s = vmalloc(sizeof(t_sem));
  s->nb_max = nb;
  s->nb_available = nb;

  // waitlist
  waitlist = vmalloc(sizeof(t_waitlist));
  waitlist->top = 0;
  waitlist->bottom = 0;

  s->waitlist = waitlist;

  s->nb_elt_proc = 0; // empty waitlist

  s->id = sem_ens->nb_sem;
  s->count_ref = 1; // 1 process is using semaphore s

  // add s to sem_ens of task_struct
  if(s->id >= MAX_SEM)
    return (ENOMEM); // limited number of sem in a process
  else
  {
    sem_ens->all_sem[s->id] = s;
    sem_ens->nb_sem += 1;
  }

  return s->id;
}

SYSCALL_DEFINE1(sem_destroy, int, sid)
// int sem_destroy(int sid)
{
    p = current;
    if (p->lsem == NULL){
        printk("lsem\n");
        return (EFAULT);
    }
    if (sid >= MAX_SEM){
        printk("max_sem\n");
        return (EFAULT);
    }
    t_sem *s = (p->lsem)->all_sem[sid];
    if (s == NULL){
        printk("sem\n");
        return (EFAULT);
    }
    if (s->waitlist != NULL){
        printk("waitlist\n");
        vfree(s->waitlist);
        s->waitlist = NULL;
    }
    if (s->waitlist != NULL)
    {
        printk("la désallocation de waitlist n'a pas fonctionné\n");
    }
    vfree((p->lsem)->all_sem[sid]);
    (p->lsem)->all_sem[sid] = NULL;
    if ((p->lsem)->all_sem[sid] != NULL)
    {
        printk("la désallocation n'a pas fonctionné\n");
    }


    return 0;
}

SYSCALL_DEFINE1(sem_acquire, int, id){
    p = current;
    t_sem_ens *sem_ens = p->lsem;
    if (sem_ens == NULL){
        printk(KERN_DEBUG "sem_ens\n");
        return (EFAULT);
    }
    else{
        int nb_tab = sem_ens->nb_sem;
        if (nb_tab > id)
        {
            t_sem *s = sem_ens->all_sem[id];
            if (s == NULL)
            {
                printk(KERN_DEBUG "sem\n");
                return (EFAULT);
            }
            else
            {
                if (s->nb_available == 0)
                {
                    int bottom = (s->waitlist)->bottom;
                    int top = (s->waitlist)->top;
                    if (bottom - top == 1001 || bottom + top == 1001){
                        return (ENOMEM);
                    }
                    else{
                        (s->waitlist)->tabproc[bottom] = current;
                        (s->waitlist)->bottom = ((s->waitlist)->bottom + 1)%1001;
                        /*s->waitlist[s->nb_elt_proc] = current;
                        s->nb_elt_proc++;
                        mysleep();*/
                    }
                }
                else
                {
                    s->nb_available--;
                }
            }
        }
        else
        {
            printk(KERN_DEBUG "id\n");
            return (EFAULT);
        }
    }
}

SYSCALL_DEFINE1(sem_release, int, id)
{
    p = current;
    t_sem_ens *sem_ens = p->lsem;
    if (sem_ens == NULL)
    {
        return (EFAULT);
    }
    else{
        int nb_tab = sem_ens->nb_sem;
        if (nb_tab > id)
        {
            t_sem *s = sem_ens->all_sem[id];
            if (s == NULL)
            {
                return (EFAULT);
            }
            else
            {
                if (s->nb_max == s->nb_available)
                {
                    return (ENOMEM);
                    /** erreur à faire :
                 * nombre de ressources à desallouer
                 * impossible
                */
                }
                else if (s->nb_elt_proc > 0)
                {
                    (s->nb_elt_proc)--;
                    (s->waitlist)->top = ((s->waitlist)->top + 1)%1001;
                    //wake_up_process(sleeping);
                }
                else
                {
                    s->nb_available++;
                }
            }
        }
        else
        {
            return (EFAULT);
        }
    }
}

SYSCALL_DEFINE1(sem_dbg, int, id){
    p = current;
    if (p->lsem == NULL){
      printk(KERN_DEBUG "error : p->lsem == NULL\n");
        return (EFAULT);
    }
    if (id >= MAX_SEM){
      printk(KERN_DEBUG "error : id >= MAX_SEM\n");
        return (EFAULT);
    }
    if ((p->lsem)->all_sem[id] == NULL){
      printk(KERN_DEBUG "error : p->lsem->all_sem[id]== NULL\n");
        return (EFAULT);
    }
    t_sem *s = (p->lsem)->all_sem[id];
    printk(KERN_DEBUG "***** Semaphore with id %d *****\n", s->id);
    printk(KERN_DEBUG "nb_max : %d\n", s->nb_max);
    printk(KERN_DEBUG "nb_available : %d\n", s->nb_available);
    int i;
    t_waitlist *w = s->waitlist;
    if (w == NULL){
      printk(KERN_DEBUG "error : w == NULL\n");
        return (EFAULT);
    }
    for (i = w->top ; i != w->bottom ; i = (i+1)%1001){
        printk(KERN_DEBUG "pid_proc %d : %d\n", i, (w->tabproc[i])->pid);
    }
    printk(KERN_DEBUG "nb_elt_proc : %d\n", s->nb_elt_proc);
    printk(KERN_DEBUG "count_ref : %d\n\n", s->count_ref);
    return s->nb_elt_proc;
}
