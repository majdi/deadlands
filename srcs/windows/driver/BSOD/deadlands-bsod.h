/*
**
** Malicious Code - Project: deadlands
** Filename	: [ deadlands-bsod.h ]
** Epi-Tek4 : [ Armand Morgan, Hochwelker Florent, Toumi Majdi ]
**
*/

#ifndef __DEADLANDS_BSOD_H__
# define __DEADLANDS_BSOD_H__

#include <ntddk.h>

/*
** - Defines:
*/
# define DEVICE_NAME	L"\\Device\\DeadlandsBSOD"
# define DOSDEVICE_NAME	L"\\DosDevices\\DeadlandsBSOD"

# define EPITECH		42

/*
** - Function prototypes:
*/
// DRIVER FUNCTIONS:
NTSTATUS	DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);
NTSTATUS	DriverUnload(PDRIVER_OBJECT pDriverObject);

#endif /* !__DEADLANDS_BSOD_H__ */
