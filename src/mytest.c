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

struct task_struct *sleeping;

/** définit la fonction **/
SYSCALL_DEFINE1(mysleep, int, x)
// int sys_mysleep(int x)
{
    sleeping = current;
    /* 
        code pour s'endormir
    */
    if (x < 0){
        return (EINVAL);
    }
    return x+1;
}

SYSCALL_DEFINE0(mywakeup)
// int sys_mywakeup()
{
    /** 
    if (sleeping == 0){
        return 0;
    }
    else{
        // code pour se révéiller

        sleeping = 0;
        return 0;
    }
    **/
    printk(KERN_DEBUG "BINGO\n");
    return 0;
}