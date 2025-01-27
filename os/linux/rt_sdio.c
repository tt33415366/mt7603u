/*
 ***************************************************************************
 * MediaTek Inc. 
 *
 * All rights reserved. source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of MediaTek. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of MediaTek, Inc. is obtained.
 ***************************************************************************

	Module Name:
	rt_sdio.c
*/


#include "rt_config.h"

void InitSDIODevice(VOID *ad_src)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)ad_src;
	pAd->infType = RTMP_DEV_INF_SDIO;
	UINT32 value=0;

	pAd->BlockSize =512;   //shoud read block size from sdio cccr info

	RTMP_SDIO_READ32(pAd, WCIR,&value);
	DBGPRINT(RT_DEBUG_ERROR, ("%s(): WCIR1:%x\n",__FUNCTION__,value));

	pAd->ChipID = GET_CHIP_ID(value);

	if(IS_MT7636(pAd)){
	DBGPRINT(RT_DEBUG_ERROR, ("%s():chip is MT7636\n",__FUNCTION__));
	//return FALSE;
	}else{
	DBGPRINT(RT_DEBUG_ERROR, ("%s():chip is not MT7636\n",__FUNCTION__));
	}
#if 0   
      RTMP_SEM_EVENT_INIT(&(pAd->UsbVendorReq_semaphore), &pAd->RscSemMemList);
#ifdef RLT_MAC
      RTMP_SEM_EVENT_INIT(&(pAd->WlanEnLock), &pAd->RscSemMemList);
#endif /* RLT_MAC */
      RTMP_SEM_EVENT_INIT(&(pAd->reg_atomic), &pAd->RscSemMemList);
      RTMP_SEM_EVENT_INIT(&(pAd->hw_atomic), &pAd->RscSemMemList);
      RTMP_SEM_EVENT_INIT(&(pAd->cal_atomic), &pAd->RscSemMemList);
      os_alloc_mem(ad, (PUCHAR *)&ad->UsbVendorReqBuf, MAX_PARAM_BUFFER_SIZE - 1);
      
      if (ad->UsbVendorReqBuf == NULL) {
         DBGPRINT(RT_DEBUG_ERROR, ("Allocate vendor request temp buffer failed!\n"));
         return;
      }
#endif

#if 0
#ifdef RLT_MAC
      if (config->driver_info == RLT_MAC_BASE) {
         UINT32 value;
         RTMP_IO_READ32(ad, 0x00, &value);
         ad->ChipID = value;
#ifdef RT65xx
      if (IS_RT65XX(ad))
         rlt_wlan_chip_onoff(ad, TRUE, TRUE);
#endif
#ifdef MT76x2
       if (IS_MT76x2(ad))
         mt76x2_pwrOn(ad);
#endif
      }
#endif
#endif
#if 0
#ifdef MT_MAC
      {
         UINT32 value;
         RTMP_IO_READ32(ad, TOP_HVR, &value);
#ifdef MT7636
           RTMP_IO_READ32(ad, TOP_HVR+8, &value);
#endif
         ad->ChipID = value;
   
         if (IS_MT7603(ad))
         {
            RTMP_IO_READ32(ad, STRAP_STA, &value);
            ad->AntMode = (value >> 24) & 0x1;
         }
      }
#endif
#endif      
      RtmpRaDevCtrlInit(pAd, pAd->infType);

   
   return;
}

NDIS_STATUS	 RtmpMgmtTaskInit(RTMP_ADAPTER *pAd)
{



	return NDIS_STATUS_SUCCESS;
}


VOID RtmpMgmtTaskExit(RTMP_ADAPTER *pAd)
{




	return NDIS_STATUS_SUCCESS;
}


static VOID MTSdioWorker(struct work_struct *work)
{
	unsigned long flags;
	POS_COOKIE pObj = container_of(work, struct os_cookie, SdioWork); 
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *)pObj->pAd_va;
	SDIOWorkTask *CurTask, *TmpTask;

	RTMP_SPIN_LOCK_IRQSAVE(&pAd->SdioWorkTaskLock, &flags);
	DlListForEachSafe(CurTask, TmpTask,
					&pAd->SdioWorkTaskList, SDIOWorkTask, List)
	{
		RTMP_SPIN_UNLOCK_IRQRESTORE(&pAd->SdioWorkTaskLock, &flags);

		MTSDIODataWorkerTask(pAd);

		RTMP_SPIN_LOCK_IRQSAVE(&pAd->SdioWorkTaskLock, &flags);
			
		DlListDel(&CurTask->List);
		os_free_mem(NULL, CurTask);
	}

	RTMP_SPIN_UNLOCK_IRQRESTORE(&pAd->SdioWorkTaskLock, &flags);
}


NDIS_STATUS RtmpNetTaskInit(RTMP_ADAPTER *pAd)
{
	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie;
	unsigned long flags;

	INIT_WORK(&pObj->SdioWork, MTSdioWorker);

	pObj->SdioWq = create_singlethread_workqueue("mtk_wifi_sdio_wq");

	if (pObj->SdioWq == NULL)
	{
		DBGPRINT(RT_DEBUG_OFF, ("%s: create mtk_wifi_sdio_wq workqueue fail\n"));
		cancel_work_sync(&pObj->SdioWork);
	}
	
	NdisAllocateSpinLock(pAd, &pAd->SdioWorkTaskLock);
	NdisAllocateSpinLock(pAd, &pAd->IntStatusLock);
	NdisAllocateSpinLock(pAd, &pAd->TcCountLock);
	
	RTMP_SPIN_LOCK_IRQSAVE(&pAd->SdioWorkTaskLock, &flags);
	DlListInit(&pAd->SdioWorkTaskList);
	RTMP_SPIN_UNLOCK_IRQRESTORE(&pAd->SdioWorkTaskLock, &flags);

	
	return NDIS_STATUS_SUCCESS;
}


VOID RtmpNetTaskExit(IN RTMP_ADAPTER *pAd)
{
	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie;
	
	cancel_work_sync(&pObj->SdioWork);
	
	if (pObj->SdioWq)
		destroy_workqueue(pObj->SdioWq); 

	NdisFreeSpinLock(&pAd->SdioWorkTaskLock);
	NdisFreeSpinLock(&pAd->IntStatusLock);
	NdisFreeSpinLock(&pAd->TcCountLock);
}


VOID rt_sdio_interrupt(struct sdio_func *func)
{

	VOID* pAd;
	struct net_device *net_dev = sdio_get_drvdata(func);
   
	GET_PAD_FROM_NET_DEV(pAd, net_dev);

	DBGPRINT(RT_DEBUG_ERROR, ("%s()!!!!\n", __FUNCTION__));
	
	MTSDIODataIsr(pAd);
}

