/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

        Module Name:

        Abstract:

        Revision History:
        Who             When                    What
        --------        ----------              ----------------------------------------------
*/



#include "rt_config.h"



#ifdef RTMP_BBP
#ifdef RTMP_MAC_PCI
VOID RTMP_BBP_IO_READ8(
	PRTMP_ADAPTER pAd,
	UCHAR bbp_id,
	UINT8 *pValue,
	BOOLEAN bViaMCU)
{
	BBP_CSR_CFG_STRUC BbpCsr;
	int _busyCnt, _secCnt, _regID;
	ULONG __IrqFlags = 0;

	// TODO: shiang-7603
	if (pAd->chipCap.hif_type == HIF_MT) {
		DBGPRINT(RT_DEBUG_OFF, ("%s(%d): Not support for HIF_MT yet!\n",
							__FUNCTION__, __LINE__));
		return;
	}


#if defined(RT65xx) || defined(MT7603) || defined(MT7628)
	if (IS_RT65XX(pAd) || IS_MT7603(pAd) || IS_MT7628(pAd)) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s(): This chip(0x%x) should not call this function!\n",
					__FUNCTION__, pAd->MACVersion));

		return;
	}
#endif /* defined(RT65xx) || defined(MT7603) */

#ifdef MT7601
	if (IS_MT7601(pAd))
	{
		if (pAd->WlanFunCtrl.field.WLAN_EN == 0) {
			DBGPRINT_ERR(("%s():Not allow to read BBP 0x%x : fail\n",
							__FUNCTION__, bbp_id));
			return;
		}

		// ForMT7601, not go through MCU
		bViaMCU = FALSE;
	}
#endif /* MT7601 */

	if ((bViaMCU) == TRUE)
		RTMP_MAC_SHR_MSEL_PROTECT_LOCK(pAd, __IrqFlags);

	_regID = ((bViaMCU) == TRUE ? H2M_BBP_AGENT : BBP_CSR_CFG);
	for (_busyCnt=0; _busyCnt<MAX_BUSY_COUNT; _busyCnt++)
	{
		RTMP_IO_READ32(pAd, _regID, &BbpCsr.word);
		if (BbpCsr.field.Busy == BUSY)
			continue;
		BbpCsr.word = 0;
		BbpCsr.field.fRead = 1;
		BbpCsr.field.BBP_RW_MODE = 1;
		BbpCsr.field.Busy = 1;
		BbpCsr.field.RegNum = bbp_id;
		RTMP_IO_WRITE32(pAd, _regID, BbpCsr.word);
		if ((bViaMCU) == TRUE)
		{
			AsicSendCommandToMcuBBP(pAd, 0x80, 0xff, 0x0, 0x0, FALSE);
			/*RtmpusecDelay(1000);*/
		}
		for (_secCnt=0; _secCnt<MAX_BUSY_COUNT; _secCnt++)
		{
			RTMP_IO_READ32(pAd, _regID, &BbpCsr.word);
			if (BbpCsr.field.Busy == IDLE)
				break;
		}
		if ((BbpCsr.field.Busy == IDLE) && (BbpCsr.field.RegNum == bbp_id))
		{
			*pValue = (UCHAR)BbpCsr.field.Value;
			break;
		}
	}

	if (BbpCsr.field.Busy == BUSY)
	{
		DBGPRINT_ERR(("BBP(viaMCU=%d) read R%d fail\n", bViaMCU, bbp_id));
		*pValue = pAd->BbpWriteLatch[bbp_id];
		if (bViaMCU == TRUE)
		{
			RTMP_IO_READ32(pAd, _regID, &BbpCsr.word);
			BbpCsr.field.Busy = 0;
			RTMP_IO_WRITE32(pAd, _regID, BbpCsr.word);
		}
	}

	if (bViaMCU == TRUE)
		RTMP_MAC_SHR_MSEL_PROTECT_UNLOCK(pAd, __IrqFlags);
}


VOID RTMP_BBP_IO_WRITE8(
	RTMP_ADAPTER *pAd,
	UCHAR bbp_id,
	UINT8 Value,
	BOOLEAN bViaMCU)
{
	BBP_CSR_CFG_STRUC BbpCsr;
	int _busyCnt=0, _regID;
	BOOLEAN brc;
	ULONG __IrqFlags = 0;

	// TODO: shiang-7603
	if (pAd->chipCap.hif_type == HIF_MT) {
		DBGPRINT(RT_DEBUG_OFF, ("%s(%d): Not support for HIF_MT yet!\n",
							__FUNCTION__, __LINE__));
		return;
	}

#if defined(RT65xx) || defined(MT7603) || defined(MT7628)
	if (IS_RT65XX(pAd) || IS_MT7603(pAd) || IS_MT7628(pAd)) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s(): This chip(0x%x) should not call this function!\n",
					__FUNCTION__, pAd->MACVersion));

		return;
	}
#endif /* defined(RT65xx) || defined(MT7603) */

#ifdef MT7601
	if (IS_MT7601(pAd))
	{
		if (pAd->WlanFunCtrl.field.WLAN_EN == 0) {
			DBGPRINT_ERR(("%s():Not allow to write BBP 0x%x : fail\n",
						__FUNCTION__, bbp_id));
			return;
		}

		// ForMT7601, not go through MCU
		bViaMCU = FALSE;
	}
#endif /* MT7601 */

	if (bViaMCU == TRUE)
		RTMP_MAC_SHR_MSEL_PROTECT_LOCK(pAd, __IrqFlags);

	_regID = (bViaMCU == TRUE ? H2M_BBP_AGENT : BBP_CSR_CFG);
	for (_busyCnt=1; _busyCnt<MAX_BUSY_COUNT; _busyCnt++)
	{
		RTMP_IO_READ32((pAd), _regID, &BbpCsr.word);
		if (BbpCsr.field.Busy == BUSY)
		{
				if ( (bViaMCU == TRUE) && ((_busyCnt % 20) == 0))
				{
					BbpCsr.field.Busy = IDLE;
					RTMP_IO_WRITE32(pAd, H2M_BBP_AGENT, BbpCsr.word);
				}
			continue;
		}
		BbpCsr.word = 0;
		BbpCsr.field.fRead = 0;
		BbpCsr.field.BBP_RW_MODE = 1;
		BbpCsr.field.Busy = 1;
		BbpCsr.field.Value = Value;
		BbpCsr.field.RegNum = bbp_id;
		RTMP_IO_WRITE32((pAd), _regID, BbpCsr.word);
		if (bViaMCU == TRUE)
		{
			brc = AsicSendCommandToMcuBBP(pAd, 0x80, 0xff, 0x0, 0x0, FALSE);
			if (pAd->OpMode == OPMODE_AP)
				RtmpusecDelay(1000);
			if (brc == FALSE)
			{
				BbpCsr.field.Busy = IDLE;
				RTMP_IO_WRITE32(pAd, H2M_BBP_AGENT, BbpCsr.word);
			}
		}
		pAd->BbpWriteLatch[bbp_id] = Value;
		break;
	}
	if (_busyCnt == MAX_BUSY_COUNT)
	{
		DBGPRINT_ERR(("BBP write R%d fail\n", bbp_id));
		if(bViaMCU == TRUE)
		{
			RTMP_IO_READ32(pAd, H2M_BBP_AGENT, &BbpCsr.word);
			BbpCsr.field.Busy = 0;
			RTMP_IO_WRITE32(pAd, H2M_BBP_AGENT, BbpCsr.word);
		}
	}
	if (bViaMCU == TRUE)
		RTMP_MAC_SHR_MSEL_PROTECT_UNLOCK(pAd, __IrqFlags);
		
}
#endif /* RTMP_MAC_PCI */


NTSTATUS RTMP_BBP_IO_READ8_BY_REG_ID(
	RTMP_ADAPTER *pAd,
	UINT32 Offset,
	UINT8 *pValue)
{
	// TODO: shiang-7603
	if (pAd->chipCap.hif_type == HIF_MT) {
		DBGPRINT(RT_DEBUG_OFF, ("%s(%d): Not support for HIF_MT yet!\n",
							__FUNCTION__, __LINE__));
		return STATUS_UNSUCCESSFUL;
	}

#if defined(RT65xx) || defined(MT7603) || defined(MT7628)
	if (IS_RT65XX(pAd) || IS_MT7603(pAd) || IS_MT7628(pAd)) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s(): This chip(0x%x) should not call this function!\n",
					__FUNCTION__, pAd->MACVersion));

		return STATUS_UNSUCCESSFUL;
	}
#endif /* defined(RT65xx) || defined(MT7603) */

#ifdef RTMP_MAC_USB
	return RTUSBReadBBPRegister(pAd, Offset, pValue);
#endif /* RTMP_MAC_USB */

#ifdef RTMP_MAC_PCI
/*
	This marco used for the BBP read operation which need via MCU.
	But for some chipset which didn't have mcu (e.g., RBUS based chipset), we
	will use this function too and didn't access the bbp register via the MCU.
*/
	if ((pAd)->bPCIclkOff == FALSE)
	{
		if ((pAd)->infType == RTMP_DEV_INF_RBUS)
			RTMP_BBP_IO_READ8((pAd), (Offset), (pValue), FALSE);
		else	
			if(IS_SUPPORT_PCIE_PS_L3((pAd)))
				RTMP_PCIE_PS_L3_BBP_IO_READ8((pAd), (Offset), (pValue), TRUE);
			else
				RTMP_BBP_IO_READ8((pAd), (Offset), (pValue), TRUE);
	}

	return STATUS_SUCCESS;
#endif /* RTMP_MAC_PCI */
}


NTSTATUS RTMP_BBP_IO_WRITE8_BY_REG_ID(
	RTMP_ADAPTER *pAd,
	UINT32 Offset,
	UINT8 Value)
{
	// TODO: shiang-7603
	if (pAd->chipCap.hif_type == HIF_MT) {
		DBGPRINT(RT_DEBUG_OFF, ("%s(%d): Not support for HIF_MT yet!\n",
							__FUNCTION__, __LINE__));
		return STATUS_UNSUCCESSFUL;
	}

#if defined(RT65xx) || defined(MT7603) || defined(MT7628)
	if (IS_RT65XX(pAd) || IS_MT7603(pAd) || IS_MT7628(pAd)) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s(): This chip(0x%x) should not call this function!\n",
					__FUNCTION__, pAd->MACVersion));

		return STATUS_UNSUCCESSFUL;
	}
#endif /* defined(RT65xx) || defined(MT7603) */

#ifdef RTMP_MAC_USB
	return RTUSBWriteBBPRegister(pAd, Offset, Value);
#endif /* RTMP_MAC_USB */

#ifdef RTMP_MAC_PCI
/*
	This marco used for the BBP write operation which need via MCU.
	But for some chipset which didn't have mcu (e.g., RBUS based chipset), we
	will use this function too and didn't access the bbp register via the MCU.
*/
	if ((pAd)->bPCIclkOff == FALSE)
	{
		if ((pAd)->infType == RTMP_DEV_INF_RBUS)
			RTMP_BBP_IO_WRITE8(pAd, Offset, Value, FALSE);
		else
			if(IS_SUPPORT_PCIE_PS_L3(pAd))
				RTMP_PCIE_PS_L3_BBP_IO_WRITE8(pAd, Offset, Value, TRUE);
			else
			RTMP_BBP_IO_WRITE8(pAd, Offset, Value, TRUE);
	}

	return STATUS_SUCCESS;
#endif /* RTMP_MAC_PCI */
}


/* BBP register initialization set*/
REG_PAIR   BBPRegTable[] = {
	{BBP_R65,		0x2C},		/* fix rssi issue*/
	{BBP_R66,		0x38},	/* Also set this default value to pAd->BbpTuning.R66CurrentValue at initial*/
	{BBP_R68,		0x0B},  /* improve Rx sensitivity. */
	{BBP_R69,		0x12},
	{BBP_R70,		0xa},	/* BBP_R70 will change to 0x8 in ApStartUp and LinkUp for rt2860C, otherwise value is 0xa*/
	{BBP_R73,		0x10},
	{BBP_R81,		0x37},
	{BBP_R82,		0x62},
	{BBP_R83,		0x6A},
	{BBP_R84,		0x99},	/* 0x19 is for rt2860E and after. This is for extension channel overlapping IOT. 0x99 is for rt2860D and before*/
	{BBP_R86,		0x00},	/* middle range issue, Rory @2008-01-28 	*/
	{BBP_R91,		0x04},	/* middle range issue, Rory @2008-01-28*/
	{BBP_R92,		0x00},	/* middle range issue, Rory @2008-01-28*/
	{BBP_R103,		0x00}, 	/* near range high-power issue, requested from Gary @2008-0528*/
#if 0 /* def RTMP_RBUS_SUPPORT */ /* move to code segment */
	{BBP_R105,		0x01},/*kurtis:0x01 ori 0x05 is for rt2860E to turn on FEQ control. It is safe for rt2860D and before, because Bit 7:2 are reserved in rt2860D and before.*/
#else
	{BBP_R105,		0x05},	/* 0x05 is for rt2860E to turn on FEQ control. It is safe for rt2860D and before, because Bit 7:2 are reserved in rt2860D and before.*/
#endif /* RTMP_RBUS_SUPPORT */
#ifdef DOT11_N_SUPPORT
	{BBP_R106,		0x35},	/* Optimizing the Short GI sampling request from Gray @2009-0409*/
#endif /* DOT11_N_SUPPORT */
};
#define	NUM_BBP_REG_PARMS	(sizeof(BBPRegTable) / sizeof(REG_PAIR))


static INT rtmp_bbp_is_ready(struct _RTMP_ADAPTER *pAd)
{
	INT idx = 0;
	UCHAR val;
	
	do 
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R0, &val);
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))			
			return FALSE;
	} while ((++idx < 20) && ((val == 0xff) || (val == 0x00)));

	if (!((val == 0xff) || (val == 0x00)))
	{
		DBGPRINT(RT_DEBUG_TRACE, ("BBP version = %x\n", val));
	}

	return (((val == 0xff) || (val == 0x00)) ? FALSE : TRUE);
}


static INT rtmp_bbp_init(RTMP_ADAPTER *pAd)
{
	INT Index = 0;
	
	/* Read BBP register, make sure BBP is up and running before write new data*/
	if (rtmp_bbp_is_ready(pAd)== FALSE)
		return FALSE;

	Index = 0;

	/* Initialize BBP register to default value*/
	for (Index = 0; Index < NUM_BBP_REG_PARMS; Index++)
	{
#ifdef RTMP_RBUS_SUPPORT
		if (pAd->infType == RTMP_DEV_INF_RBUS)
		{
			if (Index == BBP_R105)
			{
				/*
					kurtis:0x01 ori 0x05 is for rt2860E to turn on FEQ control. 
							It is safe for rt2860D and before, because Bit 7:2 
							are reserved in rt2860D and before.
				*/
				BBPRegTable[Index].Value=0x01;
				DBGPRINT(RT_DEBUG_TRACE, 
					("RBUS:BBP[%d] = %x\n",(INT)Index, 
					BBPRegTable[Index].Value));
			}
		}
#endif /* RTMP_RBUS_SUPPORT */

#ifdef MICROWAVE_OVEN_SUPPORT
#ifdef MT7601
		if (IS_MT7601(pAd)) {
			if (	BBPRegTable[Index].Register == BBP_R65)
			{
				/* Backup BBP_R65 and B5.R6 and B5.R7 */	
				pAd->CommonCfg.MO_Cfg.Stored_BBP_R65 = BBPRegTable[Index].Value;
				DBGPRINT(RT_DEBUG_TRACE, ("Stored_BBP_R65=%x @%s \n",
							pAd->CommonCfg.MO_Cfg.Stored_BBP_R65, __FUNCTION__));
			}
		}
#endif /* MT7601 */
#endif /* MICROWAVE_OVEN_SUPPORT */

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd,
				BBPRegTable[Index].Register,
				BBPRegTable[Index].Value);
	}

	// TODO: shiang, fix this for some chips which has side-effect (ex: 5572/3572, etc.) 
	if (pAd->chipCap.hif_type == HIF_RTMP)
	{
		for (Index = EEPROM_BBP_ARRAY_OFFSET; Index < NUM_EEPROM_BBP_PARMS; Index++)
		{
			UCHAR BbpRegIdx, BbpValue;
	
			if ((pAd->EEPROMDefaultValue[Index] != 0xFFFF) &&
				(pAd->EEPROMDefaultValue[Index] != 0))
			{
				BbpRegIdx = (UCHAR)(pAd->EEPROMDefaultValue[Index] >> 8);
				BbpValue  = (UCHAR)(pAd->EEPROMDefaultValue[Index] & 0xff);
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BbpRegIdx, BbpValue);
			}
		}
	}

	/* re-config specific BBP registers for individual chip */
	if (pAd->chipCap.pBBPRegTable)
	{
		REG_PAIR *reg_list = pAd->chipCap.pBBPRegTable;
		
		for (Index = 0; Index < pAd->chipCap.bbpRegTbSize; Index++)
		{
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd,
					reg_list[Index].Register,
					reg_list[Index].Value);
			DBGPRINT(RT_DEBUG_TRACE, ("BBP_R%d=0x%x\n", 
					reg_list[Index].Register, 
					reg_list[Index].Value));
		}
	}

	if (pAd->chipOps.AsicBbpInit != NULL)
		pAd->chipOps.AsicBbpInit(pAd);


#ifdef RT3593
	if (IS_RT3593(pAd))
	{
		/* SNR mapping */
		RT3593_SNR_MAPPING_INIT(pAd);
	}
#endif /* RT3593 */

	/*
		For rt2860E and after, init BBP_R84 with 0x19. This is for extension channel overlapping IOT.
		RT3090 should not program BBP R84 to 0x19, otherwise TX will block.
		3070/71/72,3090,3090A( are included in RT30xx),3572,3390
	*/
#if !defined(RT5350)
	if (((pAd->MACVersion & 0xffff) != 0x0101) &&
		!(IS_RT30xx(pAd)|| IS_RT3572(pAd) || IS_RT5390(pAd) || IS_RT5392(pAd) || IS_RT3290(pAd) || IS_MT7601(pAd) || IS_RT6352(pAd) || IS_MT76x2(pAd)))
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R84, 0x19);
#endif /* RT5350 */

#if defined(RT30xx) || defined(RT5390) || defined(RT5392)
	/* RF power sequence setup*/
	if (IS_RT30xx(pAd) || IS_RT3572(pAd) || IS_RT5390(pAd) || IS_RT5392(pAd))
	{	/*update for RT3070/71/72/90/91/92,3572,3390.*/
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R79, 0x13);		
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R80, 0x05);	
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R81, 0x33);	
	}
#endif /* defined(RT30xx) || defined(RT5390) || defined(RT5392) */

	if (pAd->MACVersion == 0x28600100)
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, 0x16);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R73, 0x12);
	}

	return TRUE;
	
}


static INT rtmp_bbp_get_temp(struct _RTMP_ADAPTER *pAd, CHAR *temp_val)
{
#if defined(RTMP_INTERNAL_TX_ALC) || defined(RTMP_TEMPERATURE_COMPENSATION) 
	BBP_R49_STRUC bbp_val;

	bbp_val.byte = 0;
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, &bbp_val.byte);
	*temp_val = (CHAR)bbp_val.byte;

	pAd->curr_temp = (bbp_val.byte & 0xff);
#endif

	return TRUE;
}


static INT rtmp_bbp_tx_comp_init(RTMP_ADAPTER *pAd, INT adc_insel, INT tssi_mode)
{
	UCHAR bbp_val, rf_val;

	
	/* Set BBP_R47 */
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R47, &bbp_val);
	bbp_val &= 0xe7;
	bbp_val |= ((tssi_mode << 3) & 0x18);
	bbp_val |= 0x80;
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R47, bbp_val);

	/*  Set RF_R27 */
	RT30xxReadRFRegister(pAd, RF_R27, &rf_val);
	rf_val &= 0x3f;
	rf_val |= ((adc_insel << 6) & 0xc0);
	RT30xxWriteRFRegister(pAd, RF_R27, rf_val);
	DBGPRINT(RT_DEBUG_TRACE, ("[temp. compensation] Set RF_R27 to 0x%x\n", rf_val));

	return TRUE;
}


static INT rtmp_bbp_set_txdac(struct _RTMP_ADAPTER *pAd, INT tx_dac)
{
	UCHAR val, old_val = 0;

	
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &old_val);
	val = old_val & (~0x18);
	switch (tx_dac)
	{
		case 2:
			val |= 0x10;
			break;
		case 1:
			val |= 0x08;
			break;
		case 0:
		default:
			break;
	}

	if (val != old_val) {
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, val);
	}

	return TRUE;
}


static INT rtmp_bbp_set_rxpath(struct _RTMP_ADAPTER *pAd, INT rxpath)
{
	UCHAR val = 0;

	/* Receiver Antenna selection, write to BBP R3(bit4:3) */	
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &val);
	val &= (~0x18);
	if(rxpath == 3)
		val |= (0x10);
	else if(rxpath == 2)
		val |= (0x8);
	else if(rxpath == 1)
		val |= (0x0);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, val);

#ifdef CONFIG_STA_SUPPORT
#ifdef RTMP_MAC_PCI
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
		pAd->StaCfg.BBPR3 = val;
	}
#endif /* RTMP_MAC_PCI */
#endif /* CONFIG_STA_SUPPORT */
	return TRUE;
}


static INT rtmp_bbp_set_ctrlch(struct _RTMP_ADAPTER *pAd, UINT8 ext_ch)
{
	UCHAR val, old_val = 0;

	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &old_val);
	val = old_val;
	switch (ext_ch)
	{
		case EXTCHA_BELOW:
			val |= (0x20);
			break;
		case EXTCHA_ABOVE:
		case EXTCHA_NONE:
			val &= (~0x20);
			break;
	}

	if (val != old_val)
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, val);

#ifdef CONFIG_STA_SUPPORT
#ifdef RTMP_MAC_PCI
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
		pAd->StaCfg.BBPR3 = val;
	}
#endif /* RTMP_MAC_PCI */
#endif /* CONFIG_STA_SUPPORT */

	return TRUE;
}


static INT rtmp_bbp_set_bw(struct _RTMP_ADAPTER *pAd, UINT8 bw)
{
	UCHAR val, old_val = 0;
	BOOLEAN bstop = FALSE;
	UINT32 MTxCycle, macStatus;


	if (bw != pAd->CommonCfg.BBPCurrentBW)
		bstop = TRUE;

	if (bstop)
	{
		/* Disable MAC Tx/Rx */
		AsicSetMacTxRx(pAd, ASIC_MAC_TXRX, FALSE);
		/* Check MAC Tx/Rx idle */
		for (MTxCycle = 0; MTxCycle < 10000; MTxCycle++)
		{
			RTMP_IO_READ32(pAd, MAC_STATUS_CFG, &macStatus);
			if (macStatus & 0x3)
				RtmpusecDelay(50);
			else
				break;
		}
	}

	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &old_val);
	val = (old_val & (~0x18));
	switch (bw)
	{
		case BW_20:
			val &= (~0x18);
			break;
		case BW_40:
			val |= (0x10);
			break;
		case BW_10:
			val |= 0x08;
			break;	
	}

	if (val != old_val) {
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, val);
	}

	/* Enable MAC Tx/Rx */
	if (bstop)
		AsicSetMacTxRx(pAd, ASIC_MAC_TXRX, TRUE);

	pAd->CommonCfg.BBPCurrentBW = bw;
	
	return TRUE;
}


static INT rtmp_bbp_set_mmps(struct _RTMP_ADAPTER *pAd, BOOLEAN ReduceCorePower)
{
	UCHAR bbp_val, org_val;
	
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &org_val);
	bbp_val = org_val;
	if (ReduceCorePower)
		bbp_val |= 0x04;
	else
		bbp_val &= ~0x04;

	if (bbp_val != org_val)
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, bbp_val);

#ifdef RT6352
	if (IS_RT6352(pAd))
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R95, &org_val);
		bbp_val = org_val;
		if (ReduceCorePower) {
			bbp_val &= ~(0x80); /* bit 7 */
		} else {
			if (pAd->Antenna.field.RxPath > 1)
				bbp_val |= 0x80;
		}
		if (bbp_val != org_val)
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R95, bbp_val);
	}
#endif /* RT6352*/

	return TRUE;
}


static NDIS_STATUS AsicBBPWriteWithRxChain(
	IN RTMP_ADAPTER *pAd,
	IN UCHAR bbpId,
	IN CHAR bbpVal,
	IN RX_CHAIN_IDX rx_ch_idx)
{
	UCHAR idx = 0, val = 0;

	if (((pAd->MACVersion & 0xffff0000) <= 0x30900000) || 
		(pAd->Antenna.field.RxPath == 1))
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, bbpId, bbpVal);
		return NDIS_STATUS_SUCCESS;
	}
	
	while (rx_ch_idx != 0)
	{
		if (idx >= pAd->Antenna.field.RxPath)
			break;
		
		if (rx_ch_idx & 0x01)
		{
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R27, &val);
			val = (val & (~0x60)) | (idx << 5);
#ifdef RTMP_MAC_USB
			if ((IS_USB_INF(pAd)) &&
			    (RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, val) == STATUS_SUCCESS))
			{
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, bbpId, bbpVal);
			}
#endif /* RTMP_MAC_USB */

#ifdef RTMP_MAC_PCI
			if (IS_PCI_INF(pAd) || IS_RBUS_INF(pAd))
			{
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, val);
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, bbpId, bbpVal);
			}
#endif /* RTMP_MAC_PCI */

			DBGPRINT(RT_DEBUG_INFO, 
					("%s(Idx):Write(R%d,val:0x%x) to Chain(0x%x, idx:%d)\n",
						__FUNCTION__, bbpId, bbpVal, rx_ch_idx, idx));
		}
		rx_ch_idx >>= 1;
		idx++;
	}

	return NDIS_STATUS_SUCCESS;
}


static NDIS_STATUS AsicBBPReadWithRxChain(
	IN RTMP_ADAPTER *pAd, 
	IN UCHAR bbpId, 
	IN CHAR *pBbpVal,
	IN RX_CHAIN_IDX rx_ch_idx)
{
	UCHAR idx, val;

	if (((pAd->MACVersion & 0xffff0000) <= 0x30900000) || 
		(pAd->Antenna.field.RxPath == 1))
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, bbpId, pBbpVal);
		return NDIS_STATUS_SUCCESS;
	}

	idx = 0;
	while(rx_ch_idx != 0)
	{
		if (idx >= pAd->Antenna.field.RxPath)
			break;

		if (rx_ch_idx & 0x01)
		{
			val = 0;
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R27, &val);
			val = (val & (~0x60)) | (idx << 5);
#ifdef RTMP_MAC_USB
			if ((IS_USB_INF(pAd)) && 
			    (RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, val) == STATUS_SUCCESS))
			{
				RTMP_BBP_IO_READ8_BY_REG_ID(pAd, bbpId, pBbpVal);
			}
#endif /* RTMP_MAC_USB */

#ifdef RTMP_MAC_PCI
			if (IS_PCI_INF(pAd) || IS_RBUS_INF(pAd))
			{
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, val);
				RTMP_BBP_IO_READ8_BY_REG_ID(pAd, bbpId, pBbpVal);
			}
#endif /* RTMP_MAC_PCI */
			break;
		}
		rx_ch_idx >>= 1;
		idx++;
	}

	return NDIS_STATUS_SUCCESS;
}


static INT rtmp_bbp_get_agc(struct _RTMP_ADAPTER *pAd, CHAR *agc, RX_CHAIN_IDX idx)
{
	return AsicBBPReadWithRxChain(pAd, BBP_R66, agc, idx);
}


static INT rtmp_bbp_set_agc(struct _RTMP_ADAPTER *pAd, UCHAR agc, RX_CHAIN_IDX idx)
{
	return AsicBBPWriteWithRxChain(pAd, BBP_R66, agc, idx);
}


static INT rtmp_bbp_set_filter_coefficient_ctrl(RTMP_ADAPTER *pAd, UCHAR Channel)
{
	UCHAR bbp_val = 0, org_val = 0;

	if (Channel == 14)
	{
		/* when Channel==14 && Mode==CCK && BandWidth==20M, BBP R4 bit5=1 */
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &org_val);
		bbp_val = org_val;
		if (WMODE_EQUAL(pAd->CommonCfg.PhyMode, WMODE_B))
			bbp_val |= 0x20;
		else
			bbp_val &= (~0x20);

		if (bbp_val != org_val)
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, bbp_val);
	}

	return TRUE;
}


static UCHAR rtmp_bbp_get_random_seed(RTMP_ADAPTER *pAd)
{
	UCHAR value1, value2, value3, value4, value5;

	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R50, &value1);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R51, &value2);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R52, &value3);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R53, &value4);
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R54, &value5);

	return (value1^value2^value3^value4^value5);
}


#ifdef DYNAMIC_VGA_SUPPORT
INT rtmp_dynamic_vga_enable(RTMP_ADAPTER *pAd)
{
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x83);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x70);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x86);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x70);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x9c);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x27);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x9d);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x27);
	return TRUE;
}


INT rtmp_dynamic_vga_disable(RTMP_ADAPTER *pAd)
{
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x83);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x32);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x86);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x19);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x9c);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x3d);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, 0x9d);
	if (pAd->CommonCfg.BBPCurrentBW == BW_20)
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x40);
	else
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, 0x2F);

	bbp_set_agc(pAd, pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0, RX_CHAIN_ALL);
	return TRUE;
}


INT rtmp_dynamic_vga_adjust(RTMP_ADAPTER *pAd)
{
	if ((pAd->CommonCfg.lna_vga_ctl.bDyncVgaEnable) &&
		(pAd->chipCap.dynamic_vga_support)
#ifdef RT6352
		&& (pAd->bCalibrationDone && IS_RT6352(pAd))
#endif
		)
	{
		UCHAR BbpReg = 0;

		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R66, &BbpReg);

		DBGPRINT(RT_DEBUG_TRACE,
				("one second False CCA=%d, fixed R66 at 0x%x\n", pAd->RalinkCounters.OneSecFalseCCACnt, BbpReg));

		if (pAd->RalinkCounters.OneSecFalseCCACnt > pAd->CommonCfg.lna_vga_ctl.nFalseCCATh)
		{
			if (BbpReg < (pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0 + 0x10))
			{
				BbpReg += 4;
				bbp_set_agc(pAd, BbpReg, RX_CHAIN_ALL);
			}
		}
		else if (pAd->RalinkCounters.OneSecFalseCCACnt < pAd->CommonCfg.lna_vga_ctl.nLowFalseCCATh)
		{
			if (BbpReg > pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0)
			{
				BbpReg -= 4;
				bbp_set_agc(pAd, BbpReg, RX_CHAIN_ALL);
			}
		}
	}
	
	return TRUE;
}
#endif /* DYNAMIC_VGA_SUPPORT */


static struct phy_ops rtmp_phy_ops = {
	.bbp_init = rtmp_bbp_init,
	.bbp_is_ready = rtmp_bbp_is_ready,
	.get_random_seed_by_phy = rtmp_bbp_get_random_seed,
	.filter_coefficient_ctrl = rtmp_bbp_set_filter_coefficient_ctrl,
	.bbp_set_agc = rtmp_bbp_set_agc,
	.bbp_get_agc = rtmp_bbp_get_agc,
	.bbp_set_bw = rtmp_bbp_set_bw,
	.bbp_set_ctrlch = rtmp_bbp_set_ctrlch,
	.bbp_set_rxpath = rtmp_bbp_set_rxpath,
	.bbp_set_txdac = rtmp_bbp_set_txdac,
	.bbp_set_mmps = rtmp_bbp_set_mmps,
	.bbp_tx_comp_init = rtmp_bbp_tx_comp_init,
	.bbp_get_temp = rtmp_bbp_get_temp,
#ifdef DYNAMIC_VGA_SUPPORT
	.dynamic_vga_enable = rtmp_dynamic_vga_enable,
	.dynamic_vga_disable = rtmp_dynamic_vga_disable,
	.dynamic_vga_adjust = rtmp_dynamic_vga_adjust,
#endif /* DYNAMIC_VGA_SUPPORT */
};


INT rtmp_phy_probe(RTMP_ADAPTER *pAd)
{
	pAd->phy_op = &rtmp_phy_ops;

	return TRUE;
}


#ifdef PRE_ANT_SWITCH

#if defined (RT2883) || defined (RT3883)
#ifdef CONFIG_STA_SUPPORT
/* STASelectPktDetAntenna - Selects antenna with best RSSI to be used for packet detection */
VOID STASelectPktDetAntenna(RTMP_ADAPTER *pAd)
{
	UCHAR rxAnts = ((pAd->StaCfg.BBPR3 >> 3) & 0x3) + 1;
	UCHAR antValue;

	/* Use antenna with best RSSI. Last RSSI is negative number. Find largest value */
	if (pAd->CommonCfg.PreAntSwitch != 0 &&  pAd->Antenna.field.RxPath>1)
	{
		if (pAd->StaCfg.RssiSample.LastRssi[0] > pAd->CommonCfg.PreAntSwitchRSSI ||
			(pAd->MacTab.Size != 1) ||
#ifdef MESH_SUPPORT
			MESH_ON(pAd) ||
#endif /* MESH_SUPPORT */
#ifdef SMART_ANTENNA
			pAd->smartAntEnable ||
#endif /* SMART_ANTENNA */
			rxAnts==1)
			antValue = 0;
		else if (pAd->StaCfg.RssiSample.LastRssi[1] > pAd->StaCfg.RssiSample.LastRssi[0])
				antValue = (rxAnts==3 && pAd->StaCfg.RssiSample.LastRssi[2]>pAd->StaCfg.RssiSample.LastRssi[1])? 2: 1;
		else
			antValue = (rxAnts==3 && pAd->StaCfg.RssiSample.LastRssi[2]>pAd->StaCfg.RssiSample.LastRssi[0])? 2: 0;

		if ((pAd->StaCfg.BBPR3 & 0x03) != antValue) {
			pAd->StaCfg.BBPR3 = (pAd->StaCfg.BBPR3 & ~0x03) | antValue;
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, pAd->StaCfg.BBPR3);
		}
	}
}
#endif /* CONFIG_STA_SUPPORT */


#ifdef CONFIG_AP_SUPPORT
INT rtmp_pre_ant_switch(RTMP_ADAPTER *pAd, MAC_TABLE_ENTRY *pEntry)
{
	/*
		Use antenna with best RSSI for packet detection. 
		If PreAntSwitch==0 then don't modify BBP R3 in case it was set manually
	*/
	if (IS_RT3883(pAd) || IS_RT2883(pAd))
	{
		if (pAd->CommonCfg.PreAntSwitch!=0 && pAd->Antenna.field.RxPath>1)
		{
			UCHAR antValue=0, r3Value;

			// Last RSSI is negative number. Find largest value
			if (pEntry->RssiSample.LastRssi[0] > pAd->CommonCfg.PreAntSwitchRSSI)
				antValue = 0;
			else if (pEntry->RssiSample.LastRssi[1] > pEntry->RssiSample.LastRssi[0])
				antValue = (pAd->Antenna.field.RxPath==3 && pEntry->RssiSample.LastRssi[2]>pEntry->RssiSample.LastRssi[1])? 2: 1;
			else
				antValue = (pAd->Antenna.field.RxPath==3 && pEntry->RssiSample.LastRssi[2]>pEntry->RssiSample.LastRssi[0])? 2: 0;

			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &r3Value);
			if ((r3Value & 0x03) != antValue)
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, (r3Value & ~0x03) | antValue);
		}
	}

	return TRUE;
}
#endif /* CONFIG_AP_SUPPORT */

#endif /* defined (RT2883) || defined (RT3883) */
#endif /* PRE_ANT_SWITCH */


#ifdef CFO_TRACK
#ifdef CONFIG_AP_SUPPORT
INT rtmp_cfo_track(RTMP_ADAPTER *pAd, MAC_TABLE_ENTRY *pEntry, INT lastClient)
{
	/* CFO Tracking */
	if (IS_RT3883(pAd))
	{
		if (pAd->MacTab.Size !=1 || pAd->CommonCfg.CFOTrack==0)
		{
			/* Set to default */
			RT3883_AsicSetFreqOffset(pAd, pAd->RfFreqOffset);
		}
		else if ((lastClient < MAX_LEN_OF_MAC_TABLE) && (lastClient >=1) && 
			pAd->CommonCfg.CFOTrack < 8 && 
			pEntry->freqOffsetValid)
		{
			/* Track CFO */
			SHORT foValue, offset = pEntry->freqOffset;
			UCHAR RFValue;

			RT30xxReadRFRegister(pAd, RF_R17, (PUCHAR)&RFValue);
			RFValue &= 0x7F;

			if (offset > 32)
				offset = 32;
			else if (offset < -32)
				offset = -32;

			foValue = RFValue - (offset/16);
			if (foValue < 0)
				foValue = 0;
			else if (foValue > 0x5F)
				foValue = 0x5F;

			if (foValue != RFValue)
				RT3883_AsicSetFreqOffset(pAd, foValue);

			/* If CFOTrack!=1 then keep updating until CFOTrack==8 */
			if (pAd->CommonCfg.CFOTrack != 1)
				pAd->CommonCfg.CFOTrack++;

			pEntry->freqOffsetValid = FALSE;
		}
	}

	return TRUE;
}
#endif /* CONFIG_AP_SUPPORT */
#endif /* CFO_TRACK */

#endif /* RTMP_BBP */

