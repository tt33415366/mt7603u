/*++

Copyright (c) 2000 Microsoft Corporation

Module Name:

    bulkwmi.c

Abstract:

Environment:

    Kernel mode

Notes:

    Copyright (c) 2000 Microsoft Corporation.  
    All Rights Reserved.

--*/
#include "config.h"
#include <Wmistr.h>
#include <Wmilib.h>

/*#include "bulkusb.h"
#include "bulkpwr.h"
#include "bulkpnp.h"
#include "bulkdev.h"
#include "bulkrwr.h"
#include "bulkwmi.h"
#include "bulkusr.h"*/
/*
#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, BulkUsb_WmiRegistration)
#pragma alloc_text(PAGE, BulkUsb_WmiDeRegistration)
#pragma alloc_text(PAGE, BulkUsb_DispatchSysCtrl)
#pragma alloc_text(PAGE, BulkUsb_QueryWmiRegInfo)
#pragma alloc_text(PAGE, BulkUsb_QueryWmiDataBlock)
#pragma alloc_text(PAGE, BulkUsb_SetWmiDataItem)
#pragma alloc_text(PAGE, BulkUsb_SetWmiDataBlock)
#endif
*/

#define MOFRESOURCENAME L"MofResourceName"

#define WMI_BULKUSB_DRIVER_INFORMATION 0


WMIGUIDREGINFO BulkWmiGuidList[1] = { {

        &GUID_QA_MT7603U, 1, 0 // driver information
    }
};

NTSTATUS
BulkUsb_WmiRegistration(
    IN OUT RTMP_ADAPTER *DeviceExtension
    )
/*++

Routine Description:

    Registers with WMI as a data provider for this
    instance of the device

Arguments:

Return Value:

--*/
{
    NTSTATUS ntStatus;
    
    PAGED_CODE();

    DeviceExtension->WmiLibInfo.GuidCount = 
          sizeof (BulkWmiGuidList) / sizeof (WMIGUIDREGINFO);

    DeviceExtension->WmiLibInfo.GuidList           = BulkWmiGuidList;
    DeviceExtension->WmiLibInfo.QueryWmiRegInfo    = BulkUsb_QueryWmiRegInfo;
    DeviceExtension->WmiLibInfo.QueryWmiDataBlock  = BulkUsb_QueryWmiDataBlock;
    DeviceExtension->WmiLibInfo.SetWmiDataBlock    = BulkUsb_SetWmiDataBlock;
    DeviceExtension->WmiLibInfo.SetWmiDataItem     = BulkUsb_SetWmiDataItem;
    DeviceExtension->WmiLibInfo.ExecuteWmiMethod   = NULL;
    DeviceExtension->WmiLibInfo.WmiFunctionControl = NULL;

    //
    // Register with WMI
    //
    
    ntStatus = IoWMIRegistrationControl(DeviceExtension->FunctionalDeviceObject,
                                        WMIREG_ACTION_REGISTER);

    return ntStatus;
    
}

NTSTATUS
BulkUsb_WmiDeRegistration(
    IN OUT RTMP_ADAPTER *DeviceExtension
    )
/*++

Routine Description:

     Inform WMI to remove this DeviceObject from its 
     list of providers. This function also 
     decrements the reference count of the deviceobject.

Arguments:

Return Value:

--*/
{

    PAGED_CODE();

    return IoWMIRegistrationControl(DeviceExtension->FunctionalDeviceObject,
                                    WMIREG_ACTION_DEREGISTER);

}

NTSTATUS
BulkUsb_DispatchSysCtrl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    )
/*++
 
Routine Description:

Arguments:

Return Value:

--*/
{
    RTMP_ADAPTER *deviceExtension;
    SYSCTL_IRP_DISPOSITION  disposition;
    NTSTATUS                ntStatus;
    PIO_STACK_LOCATION      irpStack;
    
    PAGED_CODE();

    irpStack = IoGetCurrentIrpStackLocation (Irp);
    deviceExtension = (RTMP_ADAPTER *) DeviceObject->DeviceExtension;
    DBGPRINT(RT_DEBUG_TRACE,("USB: BulkUsb_DispatchSysCtrl==>\n"));
    //BulkUsb_DbgPrint(3, ("%s",WMIMinorFunctionString(irpStack->MinorFunction)));

    if(USBRemoved == deviceExtension->DeviceState) {

        ntStatus = STATUS_DELETE_PENDING;

        Irp->IoStatus.Status = ntStatus;
        Irp->IoStatus.Information = 0;

        IoCompleteRequest(Irp, IO_NO_INCREMENT);

        return ntStatus;
    }

    //BulkUsb_DbgPrint(3, ("BulkUsb_DispatchSysCtrl::"));
    BulkUsb_IoIncrement(deviceExtension);

    ntStatus = WmiSystemControl(&deviceExtension->WmiLibInfo, 
                                DeviceObject, 
                                Irp,
                                &disposition);

    switch(disposition) {

        case IrpProcessed: 
        {
            //
            // This irp has been processed and may be completed or pending.
            //

            break;
        }
        
        case IrpNotCompleted:
        {
            //
            // This irp has not been completed, but has been fully processed.
            // we will complete it now
            //

            IoCompleteRequest(Irp, IO_NO_INCREMENT);                

            break;
        }
        
        case IrpForward:
        case IrpNotWmi:
        {
            //
            // This irp is either not a WMI irp or is a WMI irp targeted
            // at a device lower in the stack.
            //

            IoSkipCurrentIrpStackLocation (Irp);

            ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, 
                                    Irp);

            break;
        }
                                    
        default:
        {
            //
            // We really should never get here, but if we do just forward....
            //

            ASSERT(FALSE);

            IoSkipCurrentIrpStackLocation (Irp);

            ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, 
                                  Irp);
            break;
        }        
    }

    //BulkUsb_DbgPrint(3, ("BulkUsb_DispatchSysCtrl::"));
    BulkUsb_IoDecrement(deviceExtension);

    return ntStatus;
}

NTSTATUS
BulkUsb_QueryWmiRegInfo(
    IN  PDEVICE_OBJECT  DeviceObject,
    OUT ULONG           *RegFlags,
    OUT PUNICODE_STRING InstanceName,
    OUT PUNICODE_STRING *RegistryPath,
    OUT PUNICODE_STRING MofResourceName,
    OUT PDEVICE_OBJECT  *Pdo	    
    )
/*++

Routine Description:

    This routine is a callback into the driver to retrieve the list of
    guids or data blocks that the driver wants to register with WMI. This
    routine may not pend or block. Driver should NOT call
    WmiCompleteRequest.

Arguments:

    DeviceObject is the device whose data block is being queried

    *RegFlags returns with a set of flags that describe the guids being
        registered for this device. If the device wants enable and disable
        collection callbacks before receiving queries for the registered
        guids then it should return the WMIREG_FLAG_EXPENSIVE flag. Also the
        returned flags may specify WMIREG_FLAG_INSTANCE_PDO in which case
        the instance name is determined from the PDO associated with the
        device object. Note that the PDO must have an associated devnode. If
        WMIREG_FLAG_INSTANCE_PDO is not set then Name must return a unique
        name for the device.

    InstanceName returns with the instance name for the guids if
        WMIREG_FLAG_INSTANCE_PDO is not set in the returned *RegFlags. The
        caller will call ExFreePool with the buffer returned.

    *RegistryPath returns with the registry path of the driver

    *MofResourceName returns with the name of the MOF resource attached to
        the binary file. If the driver does not have a mof resource attached
        then this can be returned as NULL.

    *Pdo returns with the device object for the PDO associated with this
        device if the WMIREG_FLAG_INSTANCE_PDO flag is returned in 
        *RegFlags.

Return Value:

    status

--*/
{
    RTMP_ADAPTER *deviceExtension;

    PAGED_CODE();

    UNREFERENCED_PARAMETER( InstanceName );

    //BulkUsb_DbgPrint(3, ("BulkUsb_QueryWmiRegInfo - begins\n"));

    deviceExtension = (RTMP_ADAPTER *)DeviceObject->DeviceExtension;

    *RegFlags     = WMIREG_FLAG_INSTANCE_PDO;
    //*RegistryPath = &Globals.BulkUsb_RegistryPath;
    *Pdo          = deviceExtension->PhysicalDeviceObject;
    RtlInitUnicodeString(MofResourceName, MOFRESOURCENAME);

    //BulkUsb_DbgPrint(3, ("BulkUsb_QueryWmiRegInfo - ends\n"));
    
    return STATUS_SUCCESS;
}

NTSTATUS
BulkUsb_QueryWmiDataBlock(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp,
    IN ULONG          GuidIndex,
    IN ULONG          InstanceIndex,
    IN ULONG          InstanceCount,
    IN OUT PULONG     InstanceLengthArray,
    IN ULONG          OutBufferSize,
    OUT PUCHAR        Buffer
    )
/*++

Routine Description:

    This routine is a callback into the driver to query for the contents of
    a data block. When the driver has finished filling the data block it
    must call WmiCompleteRequest to complete the irp. The driver can
    return STATUS_PENDING if the irp cannot be completed immediately.

Arguments:

    DeviceObject is the device whose data block is being queried

    Irp is the Irp that makes this request

    GuidIndex is the index into the list of guids provided when the
        device registered

    InstanceIndex is the index that denotes which instance of the data block
        is being queried.
            
    InstanceCount is the number of instances expected to be returned for
        the data block.
            
    InstanceLengthArray is a pointer to an array of ULONG that returns the 
        lengths of each instance of the data block. If this is NULL then
        there was not enough space in the output buffer to fulfill the request
        so the irp should be completed with the buffer needed.        
            
    OutBufferSize has the maximum size available to write the data
        block.

    Buffer on return is filled with the returned data block


Return Value:

    status

--*/
{
    RTMP_ADAPTER *deviceExtension;
    NTSTATUS          ntStatus;
    ULONG             size;
    WCHAR             modelName[] = L"Aishverya\0\0";
    USHORT            modelNameLen;

    PAGED_CODE();

    //BulkUsb_DbgPrint(3, ("BulkUsb_QueryWmiDataBlock - begins\n"));

    size = 0;
    modelNameLen = (USHORT)((wcslen(modelName) + 1) * sizeof(WCHAR));

    //
    // Only ever registers 1 instance per guid
    //

    ASSERT((InstanceIndex == 0) &&
           (InstanceCount == 1));
    
    deviceExtension = (RTMP_ADAPTER *) DeviceObject->DeviceExtension;

    switch (GuidIndex) {

    case WMI_BULKUSB_DRIVER_INFORMATION:

        size = sizeof(ULONG) + modelNameLen + sizeof(USHORT);

        if (OutBufferSize < size ) {

            //BulkUsb_DbgPrint(3, ("OutBuffer too small\n"));

            ntStatus = STATUS_BUFFER_TOO_SMALL;

            break;
        }

        //* (PULONG) Buffer = DebugLevel;

        Buffer += sizeof(ULONG);

        //
        // put length of string ahead of string
        //

        *((PUSHORT)Buffer) = modelNameLen;

        Buffer = (PUCHAR)Buffer + sizeof(USHORT);

        RtlCopyBytes((PVOID)Buffer, (PVOID)modelName, modelNameLen);

        *InstanceLengthArray = size ;

        ntStatus = STATUS_SUCCESS;

        break;

    default:

        ntStatus = STATUS_WMI_GUID_NOT_FOUND;
    }

    ntStatus = WmiCompleteRequest(DeviceObject,
                                Irp,
                                ntStatus,
                                size,
                                IO_NO_INCREMENT);

    //BulkUsb_DbgPrint(3, ("BulkUsb_QueryWmiDataBlock - ends\n"));

    return ntStatus;
}


NTSTATUS
BulkUsb_SetWmiDataItem(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp,
    IN ULONG          GuidIndex,
    IN ULONG          InstanceIndex,
    IN ULONG          DataItemId,
    IN ULONG          BufferSize,
    IN PUCHAR         Buffer
    )
/*++

Routine Description:

    This routine is a callback into the driver to set for the contents of
    a data block. When the driver has finished filling the data block it
    must call WmiCompleteRequest to complete the irp. The driver can
    return STATUS_PENDING if the irp cannot be completed immediately.

Arguments:

    DeviceObject is the device whose data block is being queried

    Irp is the Irp that makes this request

    GuidIndex is the index into the list of guids provided when the
        device registered

    InstanceIndex is the index that denotes which instance of the data block
        is being queried.
            
    DataItemId has the id of the data item being set

    BufferSize has the size of the data item passed

    Buffer has the new values for the data item


Return Value:

    status

--*/
{
    RTMP_ADAPTER *deviceExtension;
    NTSTATUS          ntStatus;
    ULONG             info;
    
    PAGED_CODE();

	UNREFERENCED_PARAMETER( InstanceIndex );

    //BulkUsb_DbgPrint(3, ("BulkUsb_SetWmiDataItem - begins\n"));

    deviceExtension = (RTMP_ADAPTER *) DeviceObject->DeviceExtension;
    info = 0;

    switch(GuidIndex) {
    
    case WMI_BULKUSB_DRIVER_INFORMATION:

        if(DataItemId == 1) {

            if(BufferSize == sizeof(ULONG)) {

                //DebugLevel = *((PULONG)Buffer);

                ntStatus = STATUS_SUCCESS;

                info = sizeof(ULONG);
            }
            else {

                ntStatus = STATUS_INFO_LENGTH_MISMATCH;
            }
        }
        else {

            ntStatus = STATUS_WMI_READ_ONLY;
        }

        break;

    default:

        ntStatus = STATUS_WMI_GUID_NOT_FOUND;
    }

    ntStatus = WmiCompleteRequest(DeviceObject,
                                Irp,
                                ntStatus,
                                info,
                                IO_NO_INCREMENT);

    //BulkUsb_DbgPrint(3, ("BulkUsb_SetWmiDataItem - ends\n"));

    return ntStatus;
}

NTSTATUS
BulkUsb_SetWmiDataBlock(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp,
    IN ULONG          GuidIndex,
    IN ULONG          InstanceIndex,
    IN ULONG          BufferSize,
    IN PUCHAR         Buffer
    )
/*++

Routine Description:

    This routine is a callback into the driver to set the contents of
    a data block. When the driver has finished filling the data block it
    must call WmiCompleteRequest to complete the irp. The driver can
    return STATUS_PENDING if the irp cannot be completed immediately.

Arguments:

    DeviceObject is the device whose data block is being queried

    Irp is the Irp that makes this request

    GuidIndex is the index into the list of guids provided when the
        device registered

    InstanceIndex is the index that denotes which instance of the data block
        is being queried.
            
    BufferSize has the size of the data block passed

    Buffer has the new values for the data block


Return Value:

    status

--*/
{
    RTMP_ADAPTER *deviceExtension;
    NTSTATUS          ntStatus;
    ULONG             info;

    PAGED_CODE();

	UNREFERENCED_PARAMETER( InstanceIndex );
	
    deviceExtension = (RTMP_ADAPTER *)DeviceObject->DeviceExtension;
    info = 0;

    //BulkUsb_DbgPrint(3, ("BulkUsb_SetWmiDataBlock - begins\n"));

    switch(GuidIndex) {
    
    case WMI_BULKUSB_DRIVER_INFORMATION:

        if(BufferSize == sizeof(ULONG)) {

            //DebugLevel = *(PULONG) Buffer;
                    
            ntStatus = STATUS_SUCCESS;

            info = sizeof(ULONG);
        }
        else {

            ntStatus = STATUS_INFO_LENGTH_MISMATCH;
        }

        break;

    default:

        ntStatus = STATUS_WMI_GUID_NOT_FOUND;
    }

    ntStatus = WmiCompleteRequest(DeviceObject,
                                Irp,
                                ntStatus,
                                info,
                                IO_NO_INCREMENT);

    //BulkUsb_DbgPrint(3, ("BulkUsb_SetWmiDataBlock - ends\n"));

    return ntStatus;
}

PCHAR
WMIMinorFunctionString (
    UCHAR MinorFunction
    )
/*++
 
Routine Description:

Arguments:

Return Value:

--*/
{
    switch (MinorFunction) {

        case IRP_MN_CHANGE_SINGLE_INSTANCE:
            return "IRP_MN_CHANGE_SINGLE_INSTANCE\n";

        case IRP_MN_CHANGE_SINGLE_ITEM:
            return "IRP_MN_CHANGE_SINGLE_ITEM\n";

        case IRP_MN_DISABLE_COLLECTION:
            return "IRP_MN_DISABLE_COLLECTION\n";

        case IRP_MN_DISABLE_EVENTS:
            return "IRP_MN_DISABLE_EVENTS\n";

        case IRP_MN_ENABLE_COLLECTION:
            return "IRP_MN_ENABLE_COLLECTION\n";

        case IRP_MN_ENABLE_EVENTS:
            return "IRP_MN_ENABLE_EVENTS\n";

        case IRP_MN_EXECUTE_METHOD:
            return "IRP_MN_EXECUTE_METHOD\n";

        case IRP_MN_QUERY_ALL_DATA:
            return "IRP_MN_QUERY_ALL_DATA\n";

        case IRP_MN_QUERY_SINGLE_INSTANCE:
            return "IRP_MN_QUERY_SINGLE_INSTANCE\n";

        case IRP_MN_REGINFO:
            return "IRP_MN_REGINFO\n";

        default:
            return "IRP_MN_?????\n";
    }
}

