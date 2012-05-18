/*
** hookSYS.h for ** MALICIOUS CODE: PROJECT - DEADLANDS **
** 
** Made by majdi
** Login   <majdi.toumi@gmail.com>
** 
*/

#ifndef __HOOK_SYS_H__
# define __HOOK_SYS_H__

/*
** ~ Defines:
*/
# define nSYSCALL	__NR_close
//# define nSYSCALL	__NR_delete_module

/*
** ~ Function prototypes:
*/
unsigned long	**get_syscall_table(void);
void		epiHook(int nSYS, void *old_syscall, void *new_syscall);
unsigned long	my_sys_close(unsigned int fd);
void		my_handler(void);

#endif /* !__HOOK_SYS_H__ */
