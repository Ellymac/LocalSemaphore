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

struct task_struct *p;

SYSCALL_DEFINE1(sem_initialize, int, nb)
// int sem_initialize(int nb)
{
  p = current;
  t_sem t;
  t.nb_max = nb;
  t.nb_available = nb;
  t.id =0;
  t.nb_elt_proc = 0;
  t.count_ref = 0;
  return t.id;
}

SYSCALL_DEFINE1(sem_destroy, int, sid)
// int sem_destroy(int sid)
{
  p = current;
  t_sem *s;
  t_sem_ens *sems;
  sems = p->lsem;
  s = sems->all_sem;
  struct task_struct **tab = s[sid].waitlist;
  kfree(tab);
  //kfree(s);
  return 0;
}
