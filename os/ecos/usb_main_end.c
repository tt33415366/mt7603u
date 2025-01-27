/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
	usb_main_end.c

    Abstract:
   
    Revision History:
    Who         When            What
    --------    ----------      ------------------------------------------
*/
#include "rt_config.h"


static USB_DEVICE_ID RT_USB_DEVICE_ID[] =
{
	{0x148F, 0x2770}, /* Ralink */
	{0x148F, 0x2870}, /* Ralink */
	{0x0B05, 0x1731}, /* Asus */
	{0x0B05, 0x1732}, /* Asus */
	{0x0B05, 0x1742}, /* Asus */
	{0x0DF6, 0x0017}, /* Sitecom */
	{0x0DF6, 0x002B}, /* Sitecom */
	{0x0DF6, 0x002C}, /* Sitecom */
	{0x0DF6, 0x002D}, /* Sitecom */
	{0x14B2, 0x3C06}, /* Conceptronic */
	{0x14B2, 0x3C28}, /* Conceptronic */
	{0x2019, 0xED06}, /* Planex Communications, Inc. */
	{0x07D1, 0x3C09}, /* D-Link */
	{0x07D1, 0x3C11}, /* D-Link */
	{0x14B2, 0x3C07}, /* AL */
	{0x050D, 0x8053}, /* Belkin */
	{0x14B2, 0x3C23}, /* Airlink */
	{0x14B2, 0x3C27}, /* Airlink */
	{0x07AA, 0x002F}, /* Corega */
	{0x07AA, 0x003C}, /* Corega */
	{0x07AA, 0x003F}, /* Corega */
	{0x1044, 0x800B}, /* Gigabyte */
	{0x15A9, 0x0006}, /* Sparklan */
	{0x083A, 0xB522}, /* SMC */
	{0x083A, 0xA618}, /* SMC */
	{0x083A, 0x7522}, /* Arcadyan */
	{0x0CDE, 0x0022}, /* ZCOM */
	{0x0586, 0x3416}, /* Zyxel */
	{0x0CDE, 0x0025}, /* Zyxel */
	{0x1740, 0x9701}, /* EnGenius */
	{0x1740, 0x9702}, /* EnGenius */
	{0x0471, 0x200f}, /* Philips */
	{0x14B2, 0x3C25}, /* Draytek */
	{0x13D3, 0x3247}, /* AzureWave */
	{0x083A, 0x6618}, /* Accton */
	{0x15c5, 0x0008}, /* Amit */
	{0x0E66, 0x0001}, /* Hawking */
	{0x0E66, 0x0003}, /* Hawking */
	{0x129B, 0x1828}, /* Siemens */
	{0x157E, 0x300E}, /* U-Media */
	{0x5A57, 0x0284}, /* Zinwell 3572 */
};


static USBHST_DEVICE_DRIVER rtusb_driver[sizeof(RT_USB_DEVICE_ID) / sizeof(USB_DEVICE_ID)];

static void * USBEndCookie;

/*
  * USBDevConfigInit - Configure ralink ralink wireless USB device
  * @uDeviceHandle: USB Device handle
  * @uInterfaceNumber: only support one interface, so alwayes will index 0
  * @pAd: RTMP_ADAPTER
  */
NDIS_STATUS USBDevConfigInit(pUSB_DEVICE_INFO USBDev, UINT8 uInterfaceNumber, RTMP_ADAPTER *pAd)
{
	ULONG BulkOutIdx = 0;
	UINT32 i;
	UINT16	uLength = 0;		/* Config Descr length */
	UINT16 uTotalLength = 0; 	/* Config descriptor length */
	UINT32	uDescSize = 0;     	/* Descriptor size */
    UCHAR	*pBuffer = NULL;	/* Config descr buffer */
	UINT8 uCurEndpointAddress;	/* Current Endpoint address */
	UINT16 uCurMaxPacketSize;  	/* Current Endpoint number */
	UINT32	nStatus;       /* Function return status */
	pUSBHST_DEVICE_DESCRIPTOR pDeviceDesc = NULL; /* Ptr to device descr */
	pUSBHST_CONFIG_DESCRIPTOR pConfigDesc = NULL;	/* Ptr to configuration descr */
	pUSBHST_INTERFACE_DESCRIPTOR pInterfaceDesc = NULL;
	pUSBHST_ENDPOINT_DESCRIPTOR pEndpointDesc = NULL; /* Endpoint descriptor */
	pUSBHST_DESCRIPTOR_HEADER   pHeader = NULL;     /* First 2 bytes of descr */
	UINT8                       uInterfaceCount = 0; /* Interface counter */
	UINT8                       uEndpointCount = 0; /* Endpoint counter */


	/* Allocate memory to pBuffer to get the device descriptor */
    pBuffer = (UCHAR* )OS_MALLOC(sizeof(USBHST_DEVICE_DESCRIPTOR));

	if(!pBuffer)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("USBDevConfigInit: Allocate USB device descriptor failed!!\n"));
		return NDIS_STATUS_FAILURE;
	}

	/* Fill the buffer memory area to store device descriptor with 0 */
    OS_MEMSET(pBuffer, 0, sizeof(USBHST_DEVICE_DESCRIPTOR));

	/* Set the number of bytes to get from device descriptor */
    uDescSize = 18;

	/*
     * Obtain the first 8 bytes of the USB Device Descriptor
     * If the function fails then
     * release the USBDeviceInfo memory
     */
    nStatus = usbHstGetDescriptor(USBDev->uDeviceHandle,
                                   USBHST_DEVICE_DESC,
                                   0,
                                   0,
                                   &uDescSize,
                                   pBuffer);

	/* Check the status returned */
    if (USBHST_SUCCESS != nStatus)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("USBDevConfigInit(): usbHstGetDescriptor Failed!!\n"));
		
        /* Free the memory allocated to get first 18 bytes descriptor */
        OS_FREE(pBuffer);
        return NDIS_STATUS_FAILURE;
     }

	 /* Convert the buffer to CPU endian */
    OS_BUFFER_LE_TO_CPU(pBuffer,uDescSize);

	pDeviceDesc = (pUSBHST_DEVICE_DESCRIPTOR)pBuffer;

	USBDev->VendorID= OS_UINT16_CPU_TO_LE(pDeviceDesc->idVendor);
	USBDev->ProductID = OS_UINT16_CPU_TO_LE(pDeviceDesc->idProduct);
	
	OS_FREE(pBuffer);
	
	/* Allocate memory to get the configuration descriptor */
    pBuffer = (UCHAR *)OS_MALLOC(sizeof(USBHST_CONFIG_DESCRIPTOR));

	if (!pBuffer)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("USBDevConfigInit: Allocate USB configuration descriptor failed!!\n"));
		return NDIS_STATUS_FAILURE;
	}

	/* Fill the configuration descriptor memory area with 0 */
    OS_MEMSET(pBuffer, 0, sizeof(USBHST_CONFIG_DESCRIPTOR));

	/* Store the size of descriptor to be read */
    uDescSize = 9;

	/*
     * Obtain the first 9 bytes of the USB Configuration Descriptor
     * If the function fails or pBuffer returned is NULL then
     * release the pBuffer memory
     */
    nStatus = usbHstGetDescriptor(USBDev->uDeviceHandle,
                                  USBHST_CONFIG_DESC,
                                  0,
                                  0,
                                  &uDescSize,
                                  pBuffer);

	/* Check the status returned */
    if (USBHST_SUCCESS != nStatus)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("USBDevConfigInit(): usbHstGetDescriptor Failed!!\n"));
		
        /* Free the memory allocated to get first 9 bytes descriptor */
        OS_FREE(pBuffer);
        return NDIS_STATUS_FAILURE;
    }

	/* Convert the buffer into CPU endian */
    OS_BUFFER_LE_TO_CPU(pBuffer , uDescSize);

	/* Allocate memory to get the configuration descriptor */
    pConfigDesc = (pUSBHST_CONFIG_DESCRIPTOR)pBuffer;

	/* Update the uLength with CPU endian conversion */
    uLength = 	OS_UINT16_LE_TO_CPU(pConfigDesc->wTotalLength);

	/* Release the memory allocated for the configuration descriptor buffer */
    OS_FREE(pBuffer);

	/* Allocate memory to get the entire configuration descriptor */
    pBuffer = (UCHAR *)OS_MALLOC(uLength);

	if(!pBuffer)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("USBDevConfigInit(): Allocate pBuffer for entire configuration descriptor failed!!\n"));
		return NDIS_STATUS_FAILURE;	
	}

	/* Fill the configuration descriptor memory area with 0 */
    OS_MEMSET(pBuffer, 0, uLength);

	/* Store the total size of descriptor to be read */
    uDescSize = uLength;

	/* Get the configuration descriptor */
    nStatus = usbHstGetDescriptor(USBDev->uDeviceHandle,
                                  USBHST_CONFIG_DESC,
                                  0,
                                  0,
                                  &uDescSize,
                                  pBuffer);


	/* Check the status returned */
    if (USBHST_SUCCESS != nStatus)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("USBDevConfigInit(): usbHstGetDescriptor Failed!!\n"));
		
        /* Free the memory allocated to store entire configuration descriptor */
        OS_FREE(pBuffer);
		return NDIS_STATUS_FAILURE;
	}

	/*
     * Convert the buffer into CPU endian dont convert the length
     * since that will be taken care by USBD_ParseConfiguration()
     */
     OS_BUFFER_LE_TO_CPU(pBuffer , uDescSize);

	/* Initialize the configuration descriptor pointer */
    pConfigDesc = (pUSBHST_CONFIG_DESCRIPTOR)pBuffer;

	/* update the total length with CPU endian conversion */
    uTotalLength = OS_UINT16_LE_TO_CPU(pConfigDesc->wTotalLength);

	/* Move the pointer to the interface */
    uLength = pConfigDesc->bLength;

	/*
     * Gets all the interfaces with alternate setting zero and the
     * corresponding endpoints for the interface
     */
    while (uInterfaceCount < pConfigDesc->bNumInterfaces)
    {
    	/*
         * Check if whole configuration descriptor buffer is read before the
         * expected information is fetched.
         */
		if (uLength >= uTotalLength)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("Invalid configuration descriptor\n"));

			return NDIS_STATUS_FAILURE;
		}

		/* Copy the descriptor header from the buffer */
	    pHeader = (pUSBHST_DESCRIPTOR_HEADER)
	                  (pBuffer + uLength);

		while (pHeader->uDescriptorType != USBHST_INTERFACE_DESC)
		{
			/* Move the pointer to read the next descriptor */
	 		uLength = uLength +  pHeader->uLength;

			/*
             * Check if whole configuration descriptor buffer is read before the
             * expected information is fetched.
             */
            if (uLength >= uTotalLength)
            {
				DBGPRINT(RT_DEBUG_ERROR, ("Invalid configuration descriptor\n"));

				return NDIS_STATUS_FAILURE;
            }

			/* Copy the next descriptor header from the buffer */
	        pHeader = (pUSBHST_DESCRIPTOR_HEADER)
	                      (pBuffer + uLength);
		} /* end while */

		/* Copy the interface info from the buffer */
        pInterfaceDesc = (pUSBHST_INTERFACE_DESCRIPTOR)(pHeader);

		 /*
         * if this interface's uAlternateSetting is >0  get the endpointnumber
         * and  skip the memory regios for end points continue
         */
        if (pInterfaceDesc->bAlternateSetting > 0)
		{
            /* Skip the size of interface desc size */
            uLength = uLength +  sizeof(USBHST_INTERFACE_DESCRIPTOR);
            
            continue;
		}

		/* Move the buffer pointer to point to the endpoint descriptor */
        uLength = uLength + sizeof(USBHST_INTERFACE_DESCRIPTOR);

		/* Check if memory is allocated to store interface info */
		if (uLength >= uTotalLength)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("Invalid configuration descriptor\n"));

			return NDIS_STATUS_FAILURE;
		}


		/* Get # of endpoints */
		pAd->NumberOfPipes = pInterfaceDesc->bNumEndpoints;
		
        /*
         * For this interface read all the corresponding endpoint descriptors
         * and store them as endpoint info in the respective interface
         */

        /* Initialize the endpoint count to zero */
        uEndpointCount = 0;
		
		while (uEndpointCount < pInterfaceDesc->bNumEndpoints)
		{
			/* Copy the descriptor header from the buffer */
            pHeader = (pUSBHST_DESCRIPTOR_HEADER)
                      (pBuffer + uLength);

			 /* Check if the descriptor is endpoint */
            while (pHeader->uDescriptorType != USBHST_ENDPOINT_DESC)
            {
				/* Move the pointer to read the next descriptor */
                uLength = uLength +  pHeader->uLength;

				 /*
                 * Check if whole configuration descriptor buffer is read before
                 * the expected information is fetched.
                 */
                if (uLength >= uTotalLength)
                {
                	DBGPRINT(RT_DEBUG_ERROR, ("Invalid configuration descriptor\n"));

					return NDIS_STATUS_FAILURE;
				}

				 /* Copy the next descriptor header from the buffer */
                pHeader = (pUSBHST_DESCRIPTOR_HEADER)
                          (pBuffer + uLength);
  			} /* end while */
  			
			/* Copy the endpoint descriptor from the buffer */
            pEndpointDesc = (pUSBHST_ENDPOINT_DESCRIPTOR)
                            (pBuffer+ uLength);

			uCurEndpointAddress = pEndpointDesc->bEndpointAddress;
			uCurMaxPacketSize = OS_UINT16_LE_TO_CPU(pEndpointDesc->wMaxPacketSize);

			/* Get RX Control I/O Information (EP1 IN))*/
			if( (uCurEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_IN){
				pAd->BulkInEpAddr = uCurEndpointAddress;
				pAd->BulkInMaxPacketSize = uCurMaxPacketSize; 
				DBGPRINT(RT_DEBUG_TRACE, ("BulkIN Endpointaddress : %x\n", uCurEndpointAddress));

			/* Get EDCA Tx information for EP1~EP4 OUT, HCCA Tx information for  EP5 Out, and reserved for EP6 OUT */
			}else if((uCurEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_OUT){
				pAd->BulkOutEpAddr[BulkOutIdx++] = uCurEndpointAddress;
				pAd->BulkOutMaxPacketSize = uCurMaxPacketSize;
				DBGPRINT(RT_DEBUG_TRACE, ("BulkOUT Endpointaddress : %x\n", uCurEndpointAddress));
			}

			/* Move the buffer pointer to point to the next descriptor */
            uLength = uLength + sizeof(USBHST_ENDPOINT_DESCRIPTOR);
			
			/* Increment the endpoint count */
            uEndpointCount = uEndpointCount + 1;
			 
		} /* End of WHILE loop for endpoint */

	/* Increment uInterfaceCount by 1 */
	uInterfaceCount = uInterfaceCount + 1;

	} /* End of WHILE loop for interface */

	/* Create USB Pipe for URB transfer */
	usbHstSetConfiguration(USBDev->uDeviceHandle, 0);

	return NDIS_STATUS_SUCCESS;
}


/*
  * rtusb_probe - load(create) ralink wireless USB END device
  * This function is dynamic loaded when ralink wireless USB device attached.
  * @uDeivceHandle: wireless USB device handle
  * @uInterfaceNumber: wireless USB interface number, ralink USB device only support one interface
  * @uSpeed: USB speed, ralink wireless USB is HIGH speed
  * @pContext: not used by ralink driver
  * 
  * RETURNS: USBHST_SUCCESS or USBHST_FAILURE
  */
USBHST_STATUS rtusb_probe 
    (
    UINT32 uDeviceHandle,
    UINT8 uInterfaceNumber,
    UINT8 uSpeed,
    void ** pContext
    )
{

	INT32 unitNum = 0;
	char usbEndParam[60] ={0};

	/* Cookie returned by muxDevLoad() and can be passed to muxDevStart() or NULL if device can not be load*/
	USBEndCookie = NULL;
	

	/* 
	 * sysRtmpEndLoad initString for ralink wireless usb interface used by 
	 * RtmpVxUsbEndLoad when transformed to RTMP_VX_INIT_STRING, the most meaning 
	 * token needed by ralink usb driver is Interrupt(no meaning for usb) field that is filled 
	 * by uDeviceHandle.
	 */
	
	/* devNum:InterfaceType:Interrupt vector:deviceType:privSize:csrAddr:deviceName */
	sprintf(usbEndParam, "0:2:%d:0:0:0:ra", uDeviceHandle); /* note : we use uDeviceHandle to instrad Interrupt vector definition */
	USBEndCookie = muxDevLoad(unitNum, sysRtmpEndLoad, usbEndParam, 0, NULL);

	if (USBEndCookie == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("muxDevLoad() failed for ralink USB wireless device\n"));
		return USBHST_FAILURE;
	} else {
		if (muxDevStart(USBEndCookie) == ERROR)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("muxDevStart() failed for ralink USB wireless device\n"));
			return USBHST_FAILURE;
		}
	}
	
	return USBHST_SUCCESS;
}


static void rtusb_disconnect(UINT32    hDevice,
                                    void *    pDriverData)
{
	END_OBJ *pEndDev = (END_OBJ *)USBEndCookie;
	PRTMP_ADAPTER pAd = RTMP_OS_NETDEV_GET_PRIV(pEndDev);
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST);
	RtmpPhyNetDevExit(pAd, pEndDev);
	RtmpOSNetDevFree(pEndDev);
	RtmpVxNetPoolFree(pAd->OS_Cookie);
	RtmpRaDevCtrlExit(pAd);
}

static void rtusb_resume(UINT32 uDeviceHandle, void *pContext)
{


}


static void rtusb_suspend(UINT32 uDeviceHandle, void *pContext)
{


}

	

/*
========================================================================
Routine Description:
    Check the chipset vendor/product ID.

Arguments:
    _dev_p				Point to the PCI or USB device

Return Value:
    TRUE				Check ok
	FALSE				Check fail

Note:
========================================================================
*/
BOOLEAN RT28XXChipsetCheck(
	IN void *_dev_p)
{
	return TRUE;
}


END_OBJ *RtmpVxUsbEndLoad(
	IN RTMP_VX_INIT_STRING *initToken)
{
	END_OBJ *pENDDev;
	
	RTMP_ADAPTER *pAd = NULL;
	INT32 status, rv;
	POS_COOKIE handle;
	pUSB_DEVICE_INFO pUSBDev = NULL;

	DBGPRINT(RT_DEBUG_TRACE, ("===>RtmpVxUsbEndLoad()!\n"));

	/* Allocate RTMP_ADAPTER adapter structure */
	handle = kmalloc(sizeof(*handle), GFP_KERNEL);

	if (!handle)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("RtmpVxUsbEndLoad(): Allocate memory for os handle failed!!\n"));
		goto ERROR0;
	}

	 /* Fill the os_cookie memory area with zero */
	NdisZeroMemory(handle, sizeof(*handle));

	/* Alocate USB Device Info */
	pUSBDev = kmalloc(sizeof(*pUSBDev), GFP_KERNEL);

	if (!pUSBDev)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("RtmpVxUsbEndLoad() : Allocate memory for USB dev info failed!!\n"));
		goto ERROR1;
	}

	/* Fill USBDev memory area with zero */
	NdisZeroMemory(pUSBDev, sizeof(*pUSBDev));

	((POS_COOKIE)handle)->pUsb_Dev = pUSBDev;

	/* Store USB Device Handle for URB transfer */
	((POS_COOKIE)handle)->pUsb_Dev->uDeviceHandle = initToken->irqNum;
	
	rv = RTMPAllocAdapterBlock(handle, &pAd);
	
	if (rv != NDIS_STATUS_SUCCESS) 
	{
		DBGPRINT(RT_DEBUG_ERROR, ("RtmpVxUsbEndLoad(): Allocate RtmpAdapter Block Failed!!\n"));
		goto ERROR2;
	}
	
/*USBDevInit============================================== */
	status = USBDevConfigInit(((POS_COOKIE)handle)->pUsb_Dev, 0, pAd);
	if (status == NDIS_STATUS_FAILURE)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("RtmpVxUsbEndLoad(): USBDevConfigInit Failed!!\n"));
		goto ERROR3;
	}

	RtmpRaDevCtrlInit(pAd, RTMP_DEV_INF_USB);
	
/*NetDevInit============================================== */
	/*it's time to register the net device to vxworks kernel. */
	pENDDev = (END_OBJ *)kmalloc(sizeof(END_OBJ), GFP_KERNEL);

	if (pENDDev == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, (" malloc failed for main physical net device!\n"));
		goto ERROR3;
	}

	NdisZeroMemory(pENDDev, sizeof(*pENDDev));
	pAd->net_dev = pENDDev;

	status = RtmpVxNetPoolInit(handle);
	
	if (status == NDIS_STATUS_FAILURE)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Allocate NetPool failed!\n"));
		goto ERROR4;
	}
	
	/* We only assign the Tx pool for netdevice due to we use the Rx pool internal. */
	pENDDev->pNetPool = handle->pNetPool[RTMP_NETPOOL_TX];

#ifdef CONFIG_AP_SUPPORT
	pAd->ApCfg.MBSSID[MAIN_MBSSID].MSSIDDev = pENDDev;
#endif /* CONFIG_AP_SUPPORT */

	if ((END_OBJ_INIT(pENDDev, (DEV_OBJ *)pAd, &initToken->devName[0],
	              initToken->phyNum, &RtmpMAINEndFuncTable, "ra0") == ERROR)) 
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Can not init net device %s!\n", "ra0"));
		goto ERROR5;
	}
	
	if ((END_MIB_INIT(pENDDev, M2_ifType_ethernet_csmacd,
	              &ZERO_MAC_ADDR[0],  6, ETHERMTU, END_SPEED) == ERROR))
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Can not init MIB of net device %s!\n", "ra0"));
		goto ERROR5;
	}

	/*END_OBJ_READY(pENDDev, IFF_NOTRAILERS |IFF_SIMPLEX |IFF_BROADCAST |IFF_ALLMULTI); */
	END_OBJ_READY(pENDDev, IFF_NOTRAILERS |IFF_BROADCAST |IFF_MULTICAST);

	DBGPRINT(RT_DEBUG_TRACE, ("<===RtmpVxUsbEndLoad()!\n"));
	
	return pENDDev;

ERROR5:
	RtmpVxNetPoolFree(handle);
	
ERROR4:
	RtmpOSNetDevFree(pENDDev);

ERROR3:
	/* free RTMP_ADAPTER strcuture and os_cookie*/
	RTMPFreeAdapter(pAd);

ERROR2:
	kfree(pUSBDev);
		
ERROR1:
	kfree(handle);
	
ERROR0:
	return NULL;
}


/* Ralink USB driver register API, must call after USB Core initialization */
void RtmpVxUsbDriverRegister()
{

	USBHST_STATUS Result = USBHST_FAILURE;
	UINT16 index = 0;

	/* Get RTUSB DEVICE ID Table, and register each USB DEVICE ID for kernel probe when attached */
	for (index = 0; index < sizeof(RT_USB_DEVICE_ID) / sizeof(USB_DEVICE_ID); index++ )
	{
		
		/* State that we are using class specific matching */
		rtusb_driver[index].bFlagVendorSpecific = 1; /* Means the driver is vendor specific */
	
		rtusb_driver[index].uVendorIDorClass = RT_USB_DEVICE_ID[index].uVendorIDorClass;

		rtusb_driver[index].uProductIDorSubClass = RT_USB_DEVICE_ID[index].uProductIDorSubClass;
	
    	rtusb_driver[index].uBCDUSBorProtocol    = 0x0101;

		rtusb_driver[index].addDevice = rtusb_probe;

		rtusb_driver[index].removeDevice = rtusb_disconnect;

		rtusb_driver[index].resumeDevice = rtusb_resume;

		rtusb_driver[index].suspendDevice = rtusb_suspend;

		Result = usbHstDriverRegister(&rtusb_driver[index], NULL);

		if(USBHST_SUCCESS != Result)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("RtmpVxUsbDriverRegister(): Failed\n"));
			break;
		}
	}

}

/*---------------------------------------------------------------------	*/
/* function declarations												*/
/*---------------------------------------------------------------------	*/


/*
========================================================================
Routine Description:
    USB command kernel thread.

Arguments:
	*Context			the pAd, driver control block pointer

Return Value:
    0					close the thread

Note:
========================================================================
*/
INT RTUSBCmdThread(
	IN void * Context)
{
	RTMP_ADAPTER *pAd;
	RTMP_OS_TASK *pTask;
	int status;
	
	pTask = (RTMP_OS_TASK *)Context;
	pAd = (PRTMP_ADAPTER)pTask->priv;
	
	RtmpOSTaskCustomize(pTask);

	NdisAcquireSpinLock(&pAd->CmdQLock);
	pAd->CmdQ.CmdQState = RTMP_TASK_STAT_RUNNING;
	NdisReleaseSpinLock(&pAd->CmdQLock);
	
	while (pAd->CmdQ.CmdQState == RTMP_TASK_STAT_RUNNING)
	{
		/* lock the device pointers */
		RTMP_SEM_EVENT_WAIT(&(pTask->taskSema), status);

		if (pAd->CmdQ.CmdQState == RTMP_TASK_STAT_STOPED)
			break;
		
		if (status != 0)
		{
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);
			break;
		}
		/* lock the device pointers , need to check if required*/
		/*down(&(pAd->usbdev_semaphore)); */

		if (!pAd->PM_FlgSuspend)
			CMDHandler(pAd);
	}

	if (!pAd->PM_FlgSuspend)
	{	/* Clear the CmdQElements. */
		CmdQElmt	*pCmdQElmt = NULL;

		NdisAcquireSpinLock(&pAd->CmdQLock);
		pAd->CmdQ.CmdQState = RTMP_TASK_STAT_STOPED;
		while(pAd->CmdQ.size)
		{
			RTThreadDequeueCmd(&pAd->CmdQ, &pCmdQElmt);
			if (pCmdQElmt)
			{
				if (pCmdQElmt->CmdFromNdis == TRUE)
				{
					if (pCmdQElmt->buffer != NULL)
						os_free_mem(pAd, pCmdQElmt->buffer);
					os_free_mem(pAd, (PUCHAR)pCmdQElmt);
				}
				else
				{
					if ((pCmdQElmt->buffer != NULL) && (pCmdQElmt->bufferlength != 0))
						os_free_mem(pAd, pCmdQElmt->buffer);
					os_free_mem(pAd, (PUCHAR)pCmdQElmt);
				}
			}
		}

		NdisReleaseSpinLock(&pAd->CmdQLock);
	}
	/* notify the exit routine that we're actually exiting now 
	 *
	 * complete()/wait_for_completion() is similar to up()/down(),
	 * except that complete() is safe in the case where the structure
	 * is getting deleted in a parallel mode of execution (i.e. just
	 * after the down() -- that's necessary for the thread-shutdown
	 * case.
	 *
	 * complete_and_exit() goes even further than this -- it is safe in
	 * the case that the thread of the caller is going away (not just
	 * the structure) -- this is necessary for the module-remove case.
	 * This is important in preemption kernels, which transfer the flow
	 * of execution immediately upon a complete().
	 */
	DBGPRINT(RT_DEBUG_TRACE,( "<---RTUSBCmdThread\n"));

	return 0;
}

/*Nobody uses it currently*/
#if 0
VOID RTUSBWatchDog(IN RTMP_ADAPTER *pAd)
{
	PHT_TX_CONTEXT		pHTTXContext;
	int 					idx;
	ULONG				irqFlags;
	PURB		   		pUrb;
	BOOLEAN				needDumpSeq = FALSE;
	UINT32          	MACValue;

#ifdef RLT_MAC
	if (pAd->chipCap.hif_type == HIF_RLT) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s(): FixME!!\n", __FUNCTION__));
		return;
	}
#endif /* RLT_MAC */

#ifdef RTMP_MAC
	idx = 0;
	RTMP_IO_READ32(pAd, TXRXQ_PCNT, &MACValue);
	if ((MACValue & 0xff) !=0 )
	{
		DBGPRINT(RT_DEBUG_TRACE, ("TX QUEUE 0 Not EMPTY(Value=0x%0x). !!!!!!!!!!!!!!!\n", MACValue));
		RTMP_IO_WRITE32(pAd, PBF_CFG, 0xf40012);
		while((MACValue &0xff) != 0 && (idx++ < 10))
		{
		        RTMP_IO_READ32(pAd, TXRXQ_PCNT, &MACValue);
		        RtmpusecDelay(1);
		}
		RTMP_IO_WRITE32(pAd, PBF_CFG, 0xf40006);
	}

	idx = 0;
	if ((MACValue & 0xff00) !=0 )
	{
		DBGPRINT(RT_DEBUG_TRACE, ("TX QUEUE 1 Not EMPTY(Value=0x%0x). !!!!!!!!!!!!!!!\n", MACValue));
		RTMP_IO_WRITE32(pAd, PBF_CFG, 0xf4000a);
		while((MACValue &0xff00) != 0 && (idx++ < 10))
		{
			RTMP_IO_READ32(pAd, TXRXQ_PCNT, &MACValue);
			RtmpusecDelay(1);
		}
		RTMP_IO_WRITE32(pAd, PBF_CFG, 0xf40006);
	}

	
	if (pAd->watchDogRxOverFlowCnt >= 2)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("Maybe the Rx Bulk-In hanged! Cancel the pending Rx bulks request!\n"));
		if ((!RTMP_TEST_FLAG(pAd, (fRTMP_ADAPTER_RESET_IN_PROGRESS |
									fRTMP_ADAPTER_BULKIN_RESET |
									fRTMP_ADAPTER_HALT_IN_PROGRESS |
									fRTMP_ADAPTER_NIC_NOT_EXIST))))
		{
			DBGPRINT(RT_DEBUG_TRACE, ("Call CMDTHREAD_RESET_BULK_IN to cancel the pending Rx Bulk!\n"));
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_BULKIN_RESET);
			RTEnqueueInternalCmd(pAd, CMDTHREAD_RESET_BULK_IN, NULL, 0);
			needDumpSeq = TRUE;
		}
		pAd->watchDogRxOverFlowCnt = 0;
	}


	for (idx = 0; idx < NUM_OF_TX_RING; idx++)
	{
		pUrb = NULL;
		
		RTMP_IRQ_LOCK(&pAd->BulkOutLock[idx], irqFlags);
		if ((pAd->BulkOutPending[idx] == TRUE) && pAd->watchDogTxPendingCnt)
		{
			pAd->watchDogTxPendingCnt[idx]++;

			if ((pAd->watchDogTxPendingCnt[idx] > 2) && 
				 (!RTMP_TEST_FLAG(pAd, (fRTMP_ADAPTER_RESET_IN_PROGRESS | fRTMP_ADAPTER_HALT_IN_PROGRESS | fRTMP_ADAPTER_NIC_NOT_EXIST | fRTMP_ADAPTER_BULKOUT_RESET)))
				)
			{
				/* FIXME: Following code just support single bulk out. If you wanna support multiple bulk out. Modify it! */
				pHTTXContext = (PHT_TX_CONTEXT)(&pAd->TxContext[idx]);
				if (pHTTXContext->IRPPending)
				{	/* Check TxContext. */
					pUrb = pHTTXContext->pUrb;
				}
				else if (idx == MGMTPIPEIDX)
				{
					PTX_CONTEXT pMLMEContext, pNULLContext, pPsPollContext;
					
					/*Check MgmtContext. */
					pMLMEContext = (PTX_CONTEXT)(pAd->MgmtRing.Cell[pAd->MgmtRing.TxDmaIdx].AllocVa);
					pPsPollContext = (PTX_CONTEXT)(&pAd->PsPollContext);
					pNULLContext = (PTX_CONTEXT)(&pAd->NullContext);
					
					if (pMLMEContext->IRPPending)
					{
						ASSERT(pMLMEContext->IRPPending);
						pUrb = pMLMEContext->pUrb;
					}
					else if (pNULLContext->IRPPending)
					{	
						ASSERT(pNULLContext->IRPPending);
						pUrb = pNULLContext->pUrb;
					}
					else if (pPsPollContext->IRPPending)
					{	
						ASSERT(pPsPollContext->IRPPending);
						pUrb = pPsPollContext->pUrb;
					}
				}
				
				RTMP_IRQ_UNLOCK(&pAd->BulkOutLock[idx], irqFlags);
				
				DBGPRINT(RT_DEBUG_TRACE, ("Maybe the Tx Bulk-Out hanged! Cancel the pending Tx bulks request of idx(%d)!\n", idx));
				if (pUrb)
				{
					DBGPRINT(RT_DEBUG_TRACE, ("Unlink the pending URB!\n"));
					/* unlink it now */
					RTUSB_UNLINK_URB(pUrb);
					/* Sleep 200 microseconds to give cancellation time to work */
					RtmpusecDelay(200);
					needDumpSeq = TRUE;
				}
				else
				{
					DBGPRINT(RT_DEBUG_ERROR, ("Unkonw bulkOut URB maybe hanged!!!!!!!!!!!!\n"));
				}
			}
			else
			{
				RTMP_IRQ_UNLOCK(&pAd->BulkOutLock[idx], irqFlags);
			}
		}
		else
		{
			RTMP_IRQ_UNLOCK(&pAd->BulkOutLock[idx], irqFlags);
		}
	}

#ifdef DOT11_N_SUPPORT
	/* For Sigma debug, dump the ba_reordering sequence. */
	if((needDumpSeq == TRUE) /*&& (pAd->CommonCfg.bDisableReordering == 0)*/)
	{
		USHORT				Idx;
		PBA_REC_ENTRY		pBAEntry = NULL;
		UCHAR				count = 0;
		struct reordering_mpdu *mpdu_blk;
					
		Idx = pAd->MacTab.Content[BSSID_WCID].BARecWcidArray[0];

		pBAEntry = &pAd->BATable.BARecEntry[Idx];
		if((pBAEntry->list.qlen > 0) && (pBAEntry->list.next != NULL))
		{
			DBGPRINT(RT_DEBUG_TRACE, ("NICUpdateRawCounters():The Queueing pkt in reordering buffer:\n"));
			NdisAcquireSpinLock(&pBAEntry->RxReRingLock);
			mpdu_blk = pBAEntry->list.next;
			while (mpdu_blk)
			{
				DBGPRINT(RT_DEBUG_TRACE, ("\t%d:Seq-%d, bAMSDU-%d!\n", count, mpdu_blk->Sequence, mpdu_blk->bAMSDU));
				mpdu_blk = mpdu_blk->next;
				count++;
			}

			DBGPRINT(RT_DEBUG_TRACE, ("\npBAEntry->LastIndSeq=%d!\n", pBAEntry->LastIndSeq));
			NdisReleaseSpinLock(&pBAEntry->RxReRingLock);
		}
	}
#endif /* DOT11_N_SUPPORT */
#endif /* RTMP_MAC */
}
#endif
