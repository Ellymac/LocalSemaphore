#include "new_semaphore.h"

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
  sem_ens = p->lsem;
  waitlist = s->waitlist;

  // init s
  s->nb_max = nb;
  s->nb_available = nb;
  // waitlist
  waitlist->top = 0;
  waitlist->bottom = 0;
  /*if((waitlist->tabproc = vmalloc(MAX_TAB * sizeof(struct task_struct))) == NULL)
    return (ENOMEM);*/
  s->nb_elt_proc = 0; // empty waitlist

  s->id = sem_ens->nb_sem + 1;
  s->count_ref = 1; // 1 process is using semaphore s

  // add s to sem_ens of task_struct
  if(s->id > MAX_SEM)
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
/*  p = current;
  t_sem *s;
  t_sem_ens *sem_ens = p->lsem;
  s = sem_ens->all_sem[sid];
  //struct task_struct **tab = s->waitlist;
  kfree(tab);
  //kfree(s);*/
  return 0;
}

SYSCALL_DEFINE1(sem_acquire, int, id){
    p = current;
    t_sem_ens *sem_ens = p->lsem;
    if (sem_ens == NULL){
        /** erreur : sem_ens non alloué*/
    }
    else{
        int nb_tab = sem_ens->nb_sem;
        if (nb_tab <= id)
        {
            t_sem *s = sem_ens->all_sem[id];
            if (s == NULL)
            {
                /** erreur à faire : semaphore non alloué */
            }
            else
            {
                if (s->nb_available == 0)
                {
                    /*s->waitlist[s->nb_elt_proc] = current;
                    s->nb_elt_proc++;
                    mysleep();*/
                }
                else
                {
                    s->nb_available--;
                }
            }
        }
        else
        {
            /** erreur à faire : semaphore non alloué */
        }
    }
}

SYSCALL_DEFINE1(sem_release, int, id)
{
    p = current;
    t_sem_ens *sem_ens = p->lsem;
    if (sem_ens == NULL)
    {
        /** erreur : sem_ens non alloué*/
    }
    else{
        int nb_tab = sem_ens->nb_sem;
        if (nb_tab <= id)
        {
            t_sem *s = sem_ens->all_sem[id];
            if (s == NULL)
            {
                /** erreur à faire : semaphore non alloué */
            }
            else
            {
                if (s->nb_max == s->nb_available)
                {
                    /** erreur à faire :
                 * nombre de ressources à desallouer
                 * impossible
                */
                }
                else if (s->nb_elt_proc > 0)
                {

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
            /** erreur à faire : semaphore non alloué */
        }
    }
}
