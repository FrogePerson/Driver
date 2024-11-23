#include <ntddk.h>
#pragma warning(disable: 4100)

#define LINK_NAME L"\\??\\Lab1Link"

void OnUnload(PDRIVER_OBJECT pDrv)
{
	UNICODE_STRING LinklName;
	DbgPrint("Lab1: OnUnload\n");
	//delete device
	while (pDrv->DeviceObject)
	{
		IoDeleteDevice(pDrv->DeviceObject);
	}
	
	//delete sumbol link
	RtlInitUnicodeString(&LinklName, LINK_NAME);
	IoDeleteSymbolicLink(&LinklName);

}

NTSTATUS OnCreateClose(PDEVICE_OBJECT pDev, PIRP pIrp)
{
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	switch (pStack->MajorFunction)
	{

	case IRP_MJ_CREATE:
		DbgPrint("Lab1: OnCreateClose: IRP_MJ_CREATE, FOBJ=%p, fname=%ws\n",
			pStack->FileObject,
			pStack->FileObject->FileName.Buffer? pStack->FileObject->FileName.Buffer: L"NO NAME");
		break;
	case IRP_MJ_CLOSE:
		DbgPrint("Lab1: OnCreateClose: IRP_MJ_CLOASE, FOBJ=%p\n", pStack->FileObject);
		break;

	default:
		break;
	}
	pIrp->IoStatus.Information = 0;
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(pIrp, 0);//close recuest
	return STATUS_SUCCESS; //STATUS_PENDING
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDrv, PUNICODE_STRING pRegPath)
{
	UNICODE_STRING DevName, LinklName;
	PDEVICE_OBJECT pDev;
	NTSTATUS status;

	DbgPrint("LAB1: in DriverEntry\n");

	//init unicode strings with device name
	RtlInitUnicodeString(&DevName, L"\\Device\\Lab1Dev");
	RtlInitUnicodeString(&LinklName, LINK_NAME);
	
	//creating device
	status = IoCreateDevice(pDrv, 0, &DevName, 0x8000, 0, FALSE, &pDev);
	if (STATUS_SUCCESS != status)
	{
		DbgPrint("LAB1: ERR: IoCreateDevice\n");
		return status;
	}

	//creating sumbol link
	IoCreateSymbolicLink(&LinklName, &DevName);

	//init functions

	pDrv->MajorFunction[IRP_MJ_CREATE] = OnCreateClose;
	pDrv->MajorFunction[IRP_MJ_CLOSE] = OnCreateClose;
	pDrv->DriverUnload = OnUnload;



	pDrv->DriverUnload = OnUnload;

	return STATUS_SUCCESS;
}