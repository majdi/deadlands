/*
**
** Malicious Code - Project: deadlands
** Filename	: [ deadlands-idt.h ]
** Method	: [ Interrupt Descriptor Table ]
** Epi-Tek4 : [ Armand Morgan, Hochwelker Florent, Toumi Majdi ]
**
*/

#ifndef __DEADLANDS_IDT_H__
# define __DEADLANDS_IDT_H__

#include <ntddk.h>

/*
** - Defines:
*/
# define DEVICE_NAME	L"\\Device\\DeadlandsIDT"
# define DOSDEVICE_NAME	L"\\DosDevices\\DeadlandsIDT"

# define NAKED	__declspec(naked)
# define nINT	0x0

/*
** - Type definitions:
*/
typedef int				INT32;
typedef unsigned long	DWORD;
typedef unsigned char	BYTE;

#pragma pack(1)

typedef struct	_IDTENTRY
{
	unsigned short LowOffset;
	unsigned short selector;
	BYTE	unused_lo;
	BYTE	segment_type:4;
		//0x0E is an interrupt gate
	BYTE	system_segment_flag:1;
	BYTE	DPL:2;
		// descriptor privilege level
	BYTE	P:1;
		// present 
	unsigned short HiOffset;
} 				IDTENTRY;

typedef struct	_IDTINFO
{
	unsigned short IDTLimit;
	unsigned short LowIDTbase;
	unsigned short HiIDTbase;
} 				IDTINFO; 

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

// ALGORITHM: HOOK IDT
void		HookIDT(); //ULONG myHandler, PULONG realHandler, ULONG interruption);
void		unHookIDT(); //ULONG interruption);


#endif /* !__DEADLANDS_H__ */
