/*
** Malicious code - project: deadlands
** Filename: driver.h
*/

#ifndef __SERVICE_H__
# define __SERVICE_H__

#include <windows.h>
#include <stdio.h>

/*
** - Type definitions:
*/
typedef struct	s_option
{
	char		*opt;
	int   		(*fun)(char **args);
}				t_option;

/*
** - Function prototypes:
*/
int		InstallService(char **args);
int		RemoveService(char **args);
int     SendDriverData(char **args);

#endif /* !__SERVICE_H__ */
