#include <ntddk.h>
#pragma warning(disable: 4100)

#define LINK_NAME_B L"\\??\\Lab3LinkB"
#define LINK_NAME_D L"\\??\\Lab3LinkD"
#define LINK_NAME_N L"\\??\\Lab3LinkN"

#define BSIZE 20

typedef struct _DEV_EXIT
{
	unsigned char buf[BSIZE];
}DEV_EXT,*PDEV_EXT;

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
	RtlInitUnicodeString(&LinklName, LINK_NAME_B);
	IoDeleteSymbolicLink(&LinklName);

	RtlInitUnicodeString(&LinklName, LINK_NAME_D);
	IoDeleteSymbolicLink(&LinklName);

	RtlInitUnicodeString(&LinklName, LINK_NAME_N);
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
			pStack->FileObject->FileName.Buffer ? pStack->FileObject->FileName.Buffer : L"NO NAME");
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

NTSTATUS OnReadWrite(PDEVICE_OBJECT pDev, PIRP pIrp)
{
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	PDEV_EXT pde = (PDEV_EXT)pDev->DeviceExtension;
	void* pbuf;
	ULONG bsize = 0;

	pIrp->IoStatus.Information = 0;

	if (pDev->Flags & DO_BUFFERED_IO)
	{
		//buffered 
		DbgPrint("DO_BUFFERED_IO\n");
		pbuf = pIrp->AssociatedIrp.SystemBuffer;
	}
	else 
	{
		if (pDev->Flags & DO_DIRECT_IO)
		{
			DbgPrint("DO_DIRECT_IO\n");
			pbuf = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority);
		}
		else
		{
			DbgPrint("DO_NITHER_IO\n");
			pbuf = pIrp->UserBuffer;
		}
	}
	switch (pStack->MajorFunction)
	{

	case IRP_MJ_READ:
		DbgPrint("IRP_MJ_READ\n");
		bsize = pStack->Parameters.Read.Length;
		bsize = bsize > BSIZE ?  BSIZE : bsize;//<------!!!
		if (!pbuf)
		{
			DbgPrint("pbuf==NULL\n");
			break;
		}
		RtlMoveMemory(pbuf, pde->buf, bsize);
		pIrp->IoStatus.Information = bsize;
		break;
	case IRP_MJ_WRITE:
		DbgPrint("IRP_MJ_WRITE\n");
		bsize = pStack->Parameters.Write.Length;
		bsize = bsize > BSIZE ? BSIZE : bsize; //<------!!!
		if (!pbuf)
		{
			DbgPrint("pbuf==NULL\n");
			RtlMoveMemory(pde->buf, pbuf, bsize);
			break;
		}
		DbgPrint("    pbuf=%p, bsize=%d\n", pbuf, bsize);

		pIrp->IoStatus.Information = bsize;
		break;
	}

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(pIrp, 0);//close recuest
	return STATUS_SUCCESS; //STATUS_PENDING
}

#define CTL_B CTL_CODE(   0x8000, 0, METHOD_BUFFERED,   FILE_ANY_ACCESS)
#define CTL_IND CTL_CODE( 0x8000, 1, METHOD_IN_DIRECT,  FILE_ANY_ACCESS)
#define CTL_OUTD CTL_CODE(0x8000, 2, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define CTL_N CTL_CODE(   0x8000, 3, METHOD_NEITHER,    FILE_ANY_ACCESS)
NTSTATUS OnDevIoCtl(PDEVICE_OBJECT pDev, PIRP pIrp)
{
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	//PDEV_EXT pde = (PDEV_EXT)pDev->DeviceExtension;
	void* pinbuf = NULL;
	void *poutbuf = NULL;
	ULONG inbsize, outbsize;
	ULONG method;

	switch (pStack->Parameters.DeviceIoControl.IoControlCode)
	{
	case CTL_B:
		DbgPrint("LAB3: OnDevIoCtl: CTL_B\n");
		pinbuf = pIrp->AssociatedIrp.SystemBuffer;
		poutbuf = pIrp->AssociatedIrp.SystemBuffer;
		break;
	case CTL_IND:
	case CTL_OUTD:
		DbgPrint("LAB3: OnDevIoCtl: CTL_IND/CTL_OUTD\n");
		pinbuf = MmGetSystemAddressForMdl(pIrp->MdlAddress);
		poutbuf = MmGetSystemAddressForMdl(pIrp->MdlAddress);
		break;
	case CTL_N:
		DbgPrint("LAB3: OnDevIoCtl: CTL_N\n");
		pinbuf = pIrp->UserBuffer;
		poutbuf = pIrp->UserBuffer;
		break;
	default:
		method = pStack->Parameters.DeviceIoControl.IoControlCode & 3;
		switch (method)
		{
		case METHOD_BUFFERED:
			break;
		case METHOD_IN_DIRECT:
		case METHOD_OUT_DIRECT:
			break;
		case METHOD_NEITHER:
			break;
		}
	}

	inbsize = pStack->Parameters.DeviceIoControl.InputBufferLength;
	outbsize = pStack->Parameters.DeviceIoControl.OutputBufferLength;

	DbgPrint("    pinbuf=%p, inbsize=%d, poutbuf=%p, outbsize=%d\n", pinbuf, inbsize, poutbuf, outbsize);

	pIrp->IoStatus.Information = 0;
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(pIrp, 0);//close recuest
	return STATUS_SUCCESS; //STATUS_PENDING
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDrv, PUNICODE_STRING pRegPath)
{
	UNICODE_STRING DevName_b, DevName_d, DevName_n, LinklName_b, LinklName_d, LinklName_n;
	PDEVICE_OBJECT pDev_b, pDev_d, pDev_n;
	NTSTATUS status;

	DbgPrint("LAB1: in DriverEntry\n");

	//init unicode strings with device name
	RtlInitUnicodeString(&DevName_b, L"\\Device\\Lab3DevB");
	RtlInitUnicodeString(&LinklName_b, LINK_NAME_B);

	RtlInitUnicodeString(&DevName_d, L"\\Device\\Lab3DevD");
	RtlInitUnicodeString(&LinklName_d, LINK_NAME_D);

	RtlInitUnicodeString(&DevName_n, L"\\Device\\Lab3DevN");
	RtlInitUnicodeString(&LinklName_n, LINK_NAME_N);


	//creating device
	status = IoCreateDevice(pDrv, sizeof(DEV_EXT), &DevName_b, 0x8000, 0, FALSE, &pDev_b);
	if (STATUS_SUCCESS != status)
	{
		DbgPrint("LAB1: ERR: IoCreateDevice buffered\n");
		return status;
	}
	pDev_b->Flags |= DO_BUFFERED_IO;

	status = IoCreateDevice(pDrv, sizeof(DEV_EXT), &DevName_d, 0x8000, 0, FALSE, &pDev_d);
	if (STATUS_SUCCESS != status)
	{
		DbgPrint("LAB1: ERR: IoCreateDevice direct\n");
		IoDeleteDevice(pDev_b);
		return status;
	}
	pDev_d->Flags |= DO_DIRECT_IO;


	status = IoCreateDevice(pDrv, 0, &DevName_n, 0x8000, 0, FALSE, &pDev_n);
	if (STATUS_SUCCESS != status)
	{
		DbgPrint("LAB1: ERR: IoCreateDevice neither\n");
		IoDeleteDevice(pDev_b);
		IoDeleteDevice(pDev_d);
		return status;
	}

	//creating sumbol link
	IoCreateSymbolicLink(&LinklName_b, &DevName_b);
	IoCreateSymbolicLink(&LinklName_d, &DevName_b);
	IoCreateSymbolicLink(&LinklName_n, &DevName_n);

	//init functions

	pDrv->MajorFunction[IRP_MJ_CREATE] = OnCreateClose;
	pDrv->MajorFunction[IRP_MJ_CLOSE] = OnCreateClose;
	pDrv->MajorFunction[IRP_MJ_READ] = OnReadWrite;
	pDrv->MajorFunction[IRP_MJ_WRITE] = OnReadWrite;
	pDrv->MajorFunction[IRP_MJ_DEVICE_CONTROL] = OnDevIoCtl;
	pDrv->DriverUnload = OnUnload;


	return STATUS_SUCCESS;
}