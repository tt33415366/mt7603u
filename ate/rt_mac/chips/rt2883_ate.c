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
	rt2883_ate.c

	Abstract:
	Specific ATE funcitons and variables for RT2883

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/

#ifdef RT2883

#include "rt_config.h"

extern RTMP_RF_REGS RF2850RegTable[];
extern UCHAR NUM_OF_2850_CHNL;


/*
==========================================================================
    Description:

	AsicSwitchChannel() dedicated for RT2883 ATE.
    
==========================================================================
*/
VOID RT2883ATEAsicSwitchChannel(
    IN PRTMP_ADAPTER pAd)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	UINT32 Value = 0;
	CHAR TxPwer = 0, TxPwer2 = 0;
	UCHAR index = 0, BbpValue = 0, Channel = 0;
	UINT32 R2 = 0, R3 = DEFAULT_RF_TX_POWER, R4 = 0;
	RTMP_RF_REGS *RFRegTable = NULL;
#ifdef DOT11N_SS3_SUPPORT
	CHAR TxPwer3 = 0;
#endif /* DOT11N_SS3_SUPPORT */

	SYNC_CHANNEL_WITH_QA(pATEInfo, &Channel);

	/* fill Tx power value */
	TxPwer = pATEInfo->TxPower0;
	TxPwer2 = pATEInfo->TxPower1;
#ifdef DOT11N_SS3_SUPPORT
	TxPwer3 = pATEInfo->TxPower2;
#endif /* DOT11N_SS3_SUPPORT */

	RFRegTable = RF2850RegTable;

	switch (pAd->RfIcType)
	{
		case RFIC_2853:
			for (index = 0; index < NUM_OF_2850_CHNL; index++)
			{
				if (Channel == RFRegTable[index].Channel)
				{
					R2 = RFRegTable[index].R2;

					/* If TX path is 1, bit 14 = 1. */
					if (pAd->Antenna.field.TxPath == 1)
					{
						R2 |= 0x4000;	
					}

					if (pAd->Antenna.field.TxPath < 3)
					{
						R2 |= 0x2000;/* write 1 to bit 13 */
					}

					if (pAd->Antenna.field.TxPath == 3)
					{
						RTMP_IO_READ32(pAd, 0x1048, &Value);
						
						if (pATEInfo->TxAntennaSel == 1)
						{
							/* BBP, MAC control */
							RTMP_IO_READ32(pAd, 0x1048, &Value);
							
							Value &= ~0x000F0000;
							Value |= 0x00000000;    /* 0b00 for bit 16,17 */
							RTMP_IO_WRITE32(pAd, 0x1048, Value); 
							
							RTMP_IO_READ32(pAd, 0x1048, &Value);
							
							/* RF Reg2 bit[15:14:13]=0:1:1 */
							R2 &= ~(0x0000E000);
							R2 |= 0x00006000;
						} 
						else if (pATEInfo->TxAntennaSel == 2)
						{
							/* BBP, MAC control */
							RTMP_IO_READ32(pAd, 0x1048, &Value);
							Value &= ~0x000F0000;
        					Value |= 0x00010000;
							RTMP_IO_WRITE32(pAd, 0x1048, Value); 				
																	
							RTMP_IO_READ32(pAd, 0x1048, &Value);
							
							/* RF Reg2 bit[15:14:13]=1:0:1 */
							R2 &= ~(0x0000E000);
							R2 |= 0x0000A000;
						} 
						else if (pATEInfo->TxAntennaSel == 3)
						{
							/* BBP, MAC control */
							RTMP_IO_READ32(pAd, 0x1048, &Value);
							Value &= ~0x000F0000;
        					Value |= 0x00020000;										
							RTMP_IO_WRITE32(pAd, 0x1048, Value); 									
																		
							RTMP_IO_READ32(pAd, 0x1048, &Value);		
							
							/* RF Reg2 bit[15:14:13]=1:1:0 */
							R2 &= ~(0x0000E000);
							R2 |= 0x0000C000;
						} 
						else
						{
							/* BBP, MAC control */
							RTMP_IO_READ32(pAd, 0x1048, &Value);
							Value |= 0x00030000;
							RTMP_IO_WRITE32(pAd, 0x1048, Value);
							RTMP_IO_READ32(pAd, 0x1048, &Value);
								
							/* RF Reg2 bit[15:14:13]=0:0:0 */	
							R2 &= ~(0x0000E000);
							R2 |= 0x00000000;
						}
					}

					if (pAd->Antenna.field.TxPath == 2)
					{
						if (pATEInfo->TxAntennaSel == 1)
						{
							/* If TX Antenna select is 1 , bit 14 = 1; Disable Ant 2 */
							R2 |= 0x4000;	
							ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
							BbpValue &= 0xE7;		/* 11100111B */
							ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
						}
						else if (pATEInfo->TxAntennaSel == 2)
						{
							/* If TX Antenna select is 2 , bit 15 = 1; Disable Ant 1 */
							R2 |= 0x8000;	
							ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
							BbpValue &= 0xE7;	
							BbpValue |= 0x08;
							ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
						}
						else
						{
							ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
							BbpValue &= 0xE7;
							BbpValue |= 0x10;
							ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
						}
					}

					if (pAd->Antenna.field.RxPath == 2)
					{
						switch (pATEInfo->RxAntennaSel)
						{
							case 1:
								R2 |= 0x20040;
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
								BbpValue &= 0xE4;
								BbpValue |= 0x00;
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);								
								break;
							case 2:
								R2 |= 0x10040;
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
								BbpValue &= 0xE4;
								BbpValue |= 0x01;
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);									
								break;
							default:	
								R2 |= 0x40;
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
								BbpValue &= 0xE4;
								/* Only enable two Antenna to receive. */
								BbpValue |= 0x08;
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);								
								break;
						}
					}
					else if (pAd->Antenna.field.RxPath == 1)
					{
						/* write 1 to off RxPath */
						R2 |= 0x20040;	
					}

					if (pAd->Antenna.field.RxPath == 3)
					{
						switch (pATEInfo->RxAntennaSel)
						{
							case 1:
								R2 |= 0x20040;
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
								BbpValue &= 0xE4;
								BbpValue |= 0x00;
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);								
								break;
							case 2:
								R2 |= 0x10040;
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
								BbpValue &= 0xE4;
								BbpValue |= 0x01;
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);									
								break;
							case 3:	
								R2 |= 0x30000;
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
								BbpValue &= 0xE4;
								BbpValue |= 0x02;
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);
								break;								
							default:	
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
								BbpValue &= 0xE4;
								BbpValue |= 0x10;
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);								
								break;
						}
					}
					
					if (Channel > 14)
					{
						/* initialize R3, R4 */
						R3 = (RFRegTable[index].R3 & 0xffffc1ff);
						R4 = (RFRegTable[index].R4 & (~0x001f87c0)) | (pATEInfo->RFFreqOffset << 15);

						/*
							According the Rory's suggestion to solve the middle range issue.

							5.5G band power range : 0xF9~0X0F, TX0 Reg3 bit9/TX1 Reg4 bit6="0"
												means the TX power reduce 7dB.
						*/
						/* R3 */
						if ((TxPwer >= -7) && (TxPwer < 0))
						{
							TxPwer = (7+TxPwer);
							R3 |= (TxPwer << 10);
							DBGPRINT(RT_DEBUG_TRACE, ("ATEAsicSwitchChannel: TxPwer=%d \n", TxPwer));
						}
						else
						{
							TxPwer = (TxPwer > 0xF) ? (0xF) : (TxPwer);
							R3 |= (TxPwer << 10) | (1 << 9);
						}

						/* R4 */
						if ((TxPwer2 >= -7) && (TxPwer2 < 0))
						{
							TxPwer2 = (7+TxPwer2);
							R4 |= (TxPwer2 << 7);
							DBGPRINT(RT_DEBUG_TRACE, ("ATEAsicSwitchChannel: TxPwer2=%d \n", TxPwer2));
						}
						else
						{
							TxPwer2 = (TxPwer2 > 0xF) ? (0xF) : (TxPwer2);
							R4 |= (TxPwer2 << 7) | (1 << 6);
						}

						RFRegTable[index].R1 &= 0xff8fffcf; /* clear bit 4,5,20,21,22 */
						/* R1 */
						if ((TxPwer3 >= -7) && (TxPwer3 < 0))
						{
							TxPwer3 = (7+TxPwer3);
							RFRegTable[index].R1 |= (((TxPwer3 & 0xe) << 19) | ((TxPwer3 & 0x1) << 5));
						}
						else
						{
							TxPwer3 = (TxPwer3 > 0xF) ? (0xF) : (TxPwer3);
							RFRegTable[index].R1 |= (((TxPwer3 & 0xe) << 19) | ((TxPwer3 & 0x1) << 5)) | (1 << 4);
						}
						RFRegTable[index].R1 &= 0xfffffdff;
					}
					else
					{
						/* Set TX power0. */
						R3 = (RFRegTable[index].R3 & 0xffffc1ff) | (TxPwer << 9);
						/* Set frequency offset and TX power1. */
						R4 = (RFRegTable[index].R4 & (~0x001f87c0)) | (pATEInfo->RFFreqOffset << 15) | (TxPwer2 <<6);

						RFRegTable[index].R1 &= 0xff8fffcf; /* clear bit 4,5,20,21,22 */
						/* R1 */
						RFRegTable[index].R1 |= (((TxPwer3 & 0x1c) << 18) | ((TxPwer3 & 0x3) << 4));
						RFRegTable[index].R1 &= 0xfffffdff;
					}

					/* based on BBP current mode before changing RF channel */
					if (pATEInfo->TxWI.BW == BW_40)
					{
						R4 |=0x200000;
					}
					
					/* Update variables. */
					pAd->LatchRfRegs.Channel = Channel;
					pAd->hw_cfg.lan_gain = GET_LNA_GAIN(pAd);

					pAd->LatchRfRegs.R1 = RFRegTable[index].R1;
					pAd->LatchRfRegs.R2 = R2;
					pAd->LatchRfRegs.R3 = R3;
					pAd->LatchRfRegs.R4 = R4;

					RtmpRfIoWrite(pAd);
					
					break;
				}
			}
			break;

		default:
			break;
	}

	/* Change BBP setting during switch from a->g, g->a */
	if (Channel <= 14)
	{
		UINT32 TxPinCfg = 0x00050F0A;/* 2007.10.09 by Brian : 0x0005050A ==> 0x00050F0A */

		TxPinCfg = 0x32050F0A;/* Gary 2007/08/09 0x050A0A */

		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R62, (0x37 - pAd->hw_cfg.lan_gain));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R63, (0x37 - pAd->hw_cfg.lan_gain));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R64, (0x37 - pAd->hw_cfg.lan_gain));

		/* According the Rory's suggestion to solve the middle range issue. */
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R86, 0);	

		/* Rx High power VGA offset for LNA select */
		if (pAd->NicConfig2.field.ExternalLNAForG)
		{
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R82, 0x62);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R75, 0x46);
		}
		else
		{
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R82, 0x84);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R75, 0x50);
		}

		/* 2.4 G band selection PIN */
		rtmp_mac_set_band(pAd, BAND_24G);

		/* Turn off unused PA or LNA when only 1T or 1R. */
		if (pAd->Antenna.field.TxPath == 1)
		{
			TxPinCfg &= 0xFFFFFFF3;
		}

		if (pAd->Antenna.field.RxPath == 1)
		{
			TxPinCfg &= 0xFFFFF3FF;
		}

		/* calibration power unbalance issues */
		if (pAd->Antenna.field.TxPath == 2)
		{
			if (pATEInfo->TxAntennaSel == 1)
			{
				TxPinCfg &= 0xFFFFFFF7;
			}
			else if (pATEInfo->TxAntennaSel == 2)
			{
				TxPinCfg &= 0xFFFFFFFD;
			}
		}
		RTMP_IO_WRITE32(pAd, TX_PIN_CFG, TxPinCfg);
	}
	/* channel > 14 */
	else
	{
	    UINT32	TxPinCfg = 0x00050F05;/* 2007.10.09 by Brian : 0x00050505 ==> 0x00050F05 */

	    TxPinCfg = 0x31050F05;/* Gary 2007/08/09 0x050A0A */
		
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R62, (0x37 - pAd->hw_cfg.lan_gain));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R63, (0x37 - pAd->hw_cfg.lan_gain));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R64, (0x37 - pAd->hw_cfg.lan_gain));

		/* According the Rory's suggestion to solve the middle range issue. */
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R86, 0);        

		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R82, 0x62);
	}

	ATE_CHIP_RX_VGA_GAIN_INIT(pAd);

#ifdef RELEASE_EXCLUDE
	/*
		On 11A, We should delay and wait RF/BBP to be stable
		and the appropriate time should be 1000 micro seconds. 

		2005/06/05 - On 11G, We also need this delay time.
		Otherwise it's difficult to pass the WHQL.
	*/
#endif /* RELEASE_EXCLUDE */
	RtmpOsMsDelay(1);  

	if (Channel > 14)
	{
		/* When 5.5GHz band the LSB of TxPwr will be used to reduced 7dB or not. */
		DBGPRINT(RT_DEBUG_TRACE, ("RT2883:SwitchChannel#%d(RF=%d, %dT) to , R1=0x%08x, R2=0x%08x, R3=0x%08x, R4=0x%08x\n",
								  Channel, 
								  pAd->RfIcType, 
								  pAd->Antenna.field.TxPath,
								  pAd->LatchRfRegs.R1, 
								  pAd->LatchRfRegs.R2, 
								  pAd->LatchRfRegs.R3, 
								  pAd->LatchRfRegs.R4));
	}
	else
	{
		DBGPRINT(RT_DEBUG_TRACE, ("RT2883:SwitchChannel#%d(RF=%d, Pwr0=%u, Pwr1=%u, %dT) to , R1=0x%08x, R2=0x%08x, R3=0x%08x, R4=0x%08x\n",
								  Channel, 
								  pAd->RfIcType, 
								  (R3 & 0x00003e00) >> 9,
								  (R4 & 0x000007c0) >> 6,
								  pAd->Antenna.field.TxPath,
								  pAd->LatchRfRegs.R1, 
								  pAd->LatchRfRegs.R2, 
								  pAd->LatchRfRegs.R3, 
								  pAd->LatchRfRegs.R4));
    }
}    


INT RT2883ATETxPwrHandler(
	IN PRTMP_ADAPTER pAd,
	IN char index)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	ULONG R;
	CHAR TxPower = 0;
	UCHAR Bbp94 = 0;
	BOOLEAN bPowerReduce = FALSE;

#ifdef CONFIG_QA
	if ((pATEInfo->bQATxStart == TRUE) || (pATEInfo->bQARxStart == TRUE))
	{
#ifdef RELEASE_EXCLUDE
		/* 
			When QA is used for Tx, pATEInfo->TxPower0/1 and real tx power
			are not synchronized.
		*/
#endif /* RELEASE_EXCLUDE */
		return 0;
	}
	else
#endif /* CONFIG_QA */
	if (index == 0)
	{
		TxPower = pATEInfo->TxPower0;
	}
	else if (index == 1)
	{
		TxPower = pATEInfo->TxPower1;
	}
#ifdef DOT11N_SS3_SUPPORT
	else if (index == 2)
	{
		TxPower = pATEInfo->TxPower2;
	}
#endif /* DOT11N_SS3_SUPPORT */
	else
	{
#ifdef DOT11N_SS3_SUPPORT
		DBGPRINT_ERR(("%s : Only TxPower0, TxPower1, and TxPower2 are adjustable !\n", __FUNCTION__));
#else
		DBGPRINT_ERR(("%s : Only TxPower0 and TxPower1 are adjustable !\n", __FUNCTION__));
#endif /* DOT11N_SS3_SUPPORT */
		DBGPRINT_ERR(("%s : TxPower%d is out of range !\n", __FUNCTION__, index));
		return -1;
	}

	if (pATEInfo->Channel <= 14)
	{
		if (TxPower > 31)
		{
			/* R3, R4 can't large than 31 (0x24), 31 ~ 36 used by BBP 94 */
			R = 31;
			if (TxPower <= 36)
				Bbp94 = BBPR94_DEFAULT + (UCHAR)(TxPower - 31);		
		}
		else if (TxPower < 0)
		{
			/* R3, R4 can't less than 0, -1 ~ -6 used by BBP 94 */
			R = 0;
			if (TxPower >= -6)
				Bbp94 = BBPR94_DEFAULT + TxPower;
		}
		else
		{  
			/* 0 ~ 31 */
			R = (ULONG) TxPower;
			Bbp94 = BBPR94_DEFAULT;
		}

		DBGPRINT(RT_DEBUG_TRACE, ("%s : (TxPower=%d, R=%ld, BBP_R94=%d)\n", __FUNCTION__, TxPower, R, Bbp94));
	}
	else /* 5.5 GHz */
	{
		if (TxPower > 15)
		{
			/* R3, R4 can't large than 15 (0x0F) */
			R = 15;
		}
		else if (TxPower < 0)
		{
			/* R3, R4 can't less than 0 */
			/* -1 ~ -7 */
			ASSERT((TxPower >= -7));
			R = (ULONG)(TxPower + 7);
			bPowerReduce = TRUE;
		}
		else
		{  
			/* 0 ~ 15 */
			R = (ULONG) TxPower;
		}

		DBGPRINT(RT_DEBUG_TRACE, ("%s : (TxPower=%d, R=%lu)\n", __FUNCTION__, TxPower, R));
	}

	if (pATEInfo->Channel <= 14)
	{
		if (index == 0)
		{
			/* shift TX power control to correct RF(R3) register bit position */
			R = R << 9;		
			R |= (pAd->LatchRfRegs.R3 & 0xffffc1ff);
			pAd->LatchRfRegs.R3 = R;
		}
		else
		{
			/* shift TX power control to correct RF(R4) register bit position */
			R = R << 6;		
			R |= (pAd->LatchRfRegs.R4 & 0xfffff83f);
			pAd->LatchRfRegs.R4 = R;
		}
	}
	else /* 5.5GHz */
	{
		if (bPowerReduce == FALSE)
		{
			if (index == 0)
			{
				/* shift TX power control to correct RF(R3) register bit position */
				R = (R << 10) | (1 << 9);		
				R |= (pAd->LatchRfRegs.R3 & 0xffffc1ff);
				pAd->LatchRfRegs.R3 = R;
			}
			else
			{
				/* shift TX power control to correct RF(R4) register bit position */
				R = (R << 7) | (1 << 6);		
				R |= (pAd->LatchRfRegs.R4 & 0xfffff83f);
				pAd->LatchRfRegs.R4 = R;
			}
		}
		else
		{
			if (index == 0)
			{
				/* shift TX power control to correct RF(R3) register bit position */
				R = (R << 10);		
				R |= (pAd->LatchRfRegs.R3 & 0xffffc1ff);

				/* Clear bit 9 of R3 to reduce 7dB. */ 
				pAd->LatchRfRegs.R3 = (R & (~(1 << 9)));
			}
			else
			{
				/* shift TX power control to correct RF(R4) register bit position */
				R = (R << 7);		
				R |= (pAd->LatchRfRegs.R4 & 0xfffff83f);

				/* Clear bit 6 of R4 to reduce 7dB. */ 
				pAd->LatchRfRegs.R4 = (R & (~(1 << 6)));
			}
		}
	}
	RtmpRfIoWrite(pAd);

	return 0;
}


VOID RT2883ATERxVGAInit(
	IN PRTMP_ADAPTER		pAd)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	UCHAR R66;
	CHAR LNAGain = pAd->hw_cfg.lan_gain;
	
	if (pATEInfo->Channel <= 14)
	{
		/* BG band */
		R66 = (UCHAR)(0x2E + LNAGain);
	}
	else 
	{
		/* A band */
		if (pATEInfo->TxWI.BW == BW_20)
		{
			/* A band, BW == 20 */
			R66 = (UCHAR)(0x32 + (LNAGain*5)/3);
 		}
		else
		{
			/* A band, BW == 40 */
			R66 = (UCHAR)(0x3A + (LNAGain*5)/3);
		}
	}

	ATEBBPWriteWithRxChain(pAd, BBP_R66, R66, RX_CHAIN_ALL);

#ifdef RELEASE_EXCLUDE
	DBGPRINT(RT_DEBUG_INFO, 
				("%s :Ch=%d, BW=%d, LNAGain=0x%x, set R66 as 0x%x \n", 
				__FUNCTION__, pATEInfo->Channel, pATEInfo->TxWI.BW, 
				LNAGain, R66));
#endif /* RELEASE_EXCLUDE */

	return;
}


/* 
==========================================================================
    Description:
        Set RT2883 ATE RF BW
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	RT2883_Set_ATE_TX_BW_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	INT powerIndex;
	UCHAR value = 0;
	UCHAR BBPCurrentBW;
	
	BBPCurrentBW = simple_strtol(arg, 0, 10);

	if (BBPCurrentBW == 0)
	{
		pATEInfo->TxWI.BW = BW_20;
	}
	else
	{
		pATEInfo->TxWI.BW = BW_40;
 	}

#ifdef RELEASE_EXCLUDE
	/* Fix the error spectrum of CCK-40MHZ. */
	/* Turn on BBP 20MHz mode by request here. */
#endif /* RELEASE_EXCLUDE */
	if ((pATEInfo->TxWI.TXWI_O.PHYMODE == MODE_CCK) && (pATEInfo->TxWI.TXWI_O.BW == BW_40))
	{
		DBGPRINT_ERR(("Set_ATE_TX_BW_Proc!! Warning!! CCK only supports 20MHZ!!\n"));
		DBGPRINT_ERR(("Bandwidth switch to 20!!\n"));
		pATEInfo->TxWI.BW = BW_20;
	}

	if (pATEInfo->TxWI.TXWI_O.BW == BW_20)
	{
		if (pATEInfo->Channel <= 14)
		{
			/* BW=20;G band */
 			for (powerIndex=0; powerIndex<MAX_TXPOWER_ARRAY_SIZE; powerIndex++)
 			{
				if (pAd->Tx20MPwrCfgGBand[powerIndex] == 0xffffffff)
					continue;

				/* TX_PWR_CFG_0 ~ TX_PWR_CFG_6 */
				/* TX_PWR_CFG_0_EXT ~ TX_PWR_CFG_4_EXT */
				if (powerIndex == 5)
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_5, pAd->Tx20MPwrCfgGBand[powerIndex]);
				}
				else if (powerIndex == 6)
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_6, pAd->Tx20MPwrCfgGBand[powerIndex]);
				}
				else
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + powerIndex*4, pAd->Tx20MPwrCfgGBand[powerIndex]);	
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0_EXT + powerIndex*4, (pAd->Tx20MPwrCfgGBand[powerIndex] & 0xf0f0f0f0) >> 4);
				}
				RtmpOsMsDelay(5);				
			}
		}
		else
		{
			/* BW=20;A band */
 			for (powerIndex=0; powerIndex<MAX_TXPOWER_ARRAY_SIZE; powerIndex++)
 			{
				if (pAd->Tx20MPwrCfgABand[powerIndex] == 0xffffffff)
					continue;

				/* TX_PWR_CFG_0 ~ TX_PWR_CFG_6 */
				/* TX_PWR_CFG_0_EXT ~ TX_PWR_CFG_4_EXT */
				if (powerIndex == 5)
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_5, pAd->Tx20MPwrCfgABand[powerIndex]);
				}
				else if (powerIndex == 6)
 				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_6, pAd->Tx20MPwrCfgABand[powerIndex]);
				}
				else
 				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + powerIndex*4, pAd->Tx20MPwrCfgABand[powerIndex]);	
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0_EXT + powerIndex*4, (pAd->Tx20MPwrCfgABand[powerIndex] & 0xf0f0f0f0) >> 4);
 				}
 				RtmpOsMsDelay(5);				
 			}
		}

		/* set BW = 20 MHz */
		/* Set BBP R4 bit[4:3]=0:0 */
 		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &value);
 		value &= (~0x18);
 		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, value);

		/* Set BBP R66=0x3C */
		value = 0x3C;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, value);

		pAd->LatchRfRegs.R4 &= ~0x00200000;
		RtmpRfIoWrite(pAd);

		/* BW = 20 MHz */
		/* Set BBP R68=0x0B to improve Rx sensitivity. */
		value = 0x0B;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R68, value);
		/* Set BBP R69=0x16 */
		value = 0x16;
 		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, value);
		/* Set BBP R70=0x08 */
		value = 0x08;
 		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, value);
		/* Set BBP R73=0x11 */
	    value = 0x11;
 		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R73, value);

#ifdef RELEASE_EXCLUDE
	    /*
			If Channel=14, Bandwidth=20M and Mode=CCK, Set BBP R4 bit5=1
			(to set Japan filter coefficients).
			This segment of code will only works when ATETXMODE and ATECHANNEL
			were set to MODE_CCK and 14 respectively before ATETXBW is set to 0.
	    */
		/* 
			Please don't move this block backward.
			BBP_R4 should be overwritten for every chip if the condition matched.
		*/
#endif /* RELEASE_EXCLUDE */
		if (pATEInfo->Channel == 14)
		{
			INT TxMode = pATEInfo->TxWI.TXWI_O.PHYMODE;

			if (TxMode == MODE_CCK)
			{
				/* when Channel==14 && Mode==CCK && BandWidth==20M, BBP R4 bit5=1 */
 				ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &value);
				value |= 0x20; /* set bit5=1 */
 				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, value);				
			}
		}
	}
	/* If bandwidth = 40M, set RF Reg4 bit 21 = 0. */
	else if (pATEInfo->TxWI.BW == BW_40)
	{
		if (pATEInfo->Channel <= 14)
		{
			/* BW=40;G band */
			for (powerIndex=0; powerIndex<MAX_TXPOWER_ARRAY_SIZE; powerIndex++)
			{
				if (pAd->Tx40MPwrCfgGBand[powerIndex] == 0xffffffff)
					continue;

				/* TX_PWR_CFG_0 ~ TX_PWR_CFG_6 */
				/* TX_PWR_CFG_0_EXT ~ TX_PWR_CFG_4_EXT */
				if (powerIndex == 5)
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_5, pAd->Tx40MPwrCfgGBand[powerIndex]);
				}
				else if (powerIndex == 6)
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_6, pAd->Tx40MPwrCfgGBand[powerIndex]);
				}
				else
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + powerIndex*4, pAd->Tx40MPwrCfgGBand[powerIndex]);	
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0_EXT + powerIndex*4, (pAd->Tx40MPwrCfgGBand[powerIndex] & 0xf0f0f0f0) >> 4);
				}
				RtmpOsMsDelay(5);				
			}
		}
		else
		{
			/* BW=40;A band */
			for (powerIndex=0; powerIndex<MAX_TXPOWER_ARRAY_SIZE; powerIndex++)
			{
				if (pAd->Tx40MPwrCfgABand[powerIndex] == 0xffffffff)
					continue;

				/* TX_PWR_CFG_0 ~ TX_PWR_CFG_6 */
				if (powerIndex == 5)
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_5, pAd->Tx40MPwrCfgABand[powerIndex]);
				}
				else if (powerIndex == 6)
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_6, pAd->Tx40MPwrCfgABand[powerIndex]);
				}
				else
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + powerIndex*4, pAd->Tx40MPwrCfgABand[powerIndex]);	
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0_EXT + powerIndex*4, (pAd->Tx40MPwrCfgABand[powerIndex] & 0xf0f0f0f0) >> 4);
				}
				RtmpOsMsDelay(5);				
			}		

			if ((pATEInfo->TxWI.TXWI_O.PHYMODE >= 2) && (pATEInfo->TxWI.TXWI_O.MCS == 7))
			{
    			value = 0x28;
    			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R67, value);
			}
		}

		/* Set BBP R4 bit[4:3]=1:0 */
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &value);
		value &= (~0x18);
		value |= 0x10;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, value);

		/* Set BBP R66=0x3C */
		value = 0x3C;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, value);

		/* set BW = 40 MHz */
		pAd->LatchRfRegs.R4 |= 0x00200000;
		RtmpRfIoWrite(pAd);

		/* BW = 40 MHz */
		/* Set BBP R68=0x0C to improve Rx sensitivity. */
		value = 0x0C;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R68, value);
		/* Set BBP R69=0x1A */
		value = 0x1A;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, value);
		/* Set BBP R70=0x0A */
		value = 0x0A;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, value);
		/* Set BBP R73=0x16 */
	    value = 0x16;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R73, value);
	}

	return TRUE;
}


INT	RT2883_Set_ATE_TX_FREQ_OFFSET_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	ULONG R4 = 0;
	UCHAR RFFreqOffset = 0;

	RFFreqOffset = simple_strtol(arg, 0, 10);

	if (RFFreqOffset >= 64)
	{
		DBGPRINT_ERR(("Set_ATE_TX_FREQ_OFFSET_Proc::Out of range(0 ~ 63).\n"));
		return FALSE;
	}

	pATEInfo->RFFreqOffset = RFFreqOffset;

	/* shift TX power control to correct RF register bit position */
	R4 = pATEInfo->RFFreqOffset << 15;		
	R4 |= (pAd->LatchRfRegs.R4 & ((~0x001f8000)));
	pAd->LatchRfRegs.R4 = R4;
	
	RtmpRfIoWrite(pAd);

	return TRUE;
}

struct _ATE_CHIP_STRUCT RALINK2883 =
{
	/* functions */
	.ChannelSwitch = RT2883ATEAsicSwitchChannel,
	.TxPwrHandler = RT2883ATETxPwrHandler,
	.TssiCalibration = NULL,
	.ExtendedTssiCalibration = NULL,
	.RxVGAInit = RT2883ATERxVGAInit,
	.AsicSetTxRxPath = NULL,
	.AdjustTxPower = DefaultATEAsicAdjustTxPower,
	.AsicExtraPowerOverMAC = NULL,
	.TemperCompensation = NULL,
	
	/* command handlers */
	.Set_BW_Proc = RT2883_Set_ATE_TX_BW_Proc,
	.Set_FREQ_OFFSET_Proc = RT2883_Set_ATE_TX_FREQ_OFFSET_Proc,

	/* variables */
	.maxTxPwrCnt = 7,
	.bBBPStoreTXCARR = TRUE,
	.bBBPStoreTXCARRSUPP = TRUE,
	.bBBPStoreTXCONT = TRUE,
	.bBBPLoadATESTOP = TRUE,
};

#endif /* RT2883 */

