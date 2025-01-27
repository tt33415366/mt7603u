/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	oid.h

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Name		Date			Modification logs
*/
#ifndef _OID_H_
#define _OID_H_

/*#include <linux/wireless.h> */

#ifdef RELEASE_EXCLUDE
/*
	DON'T USE OID 0x2000 ~ 0x2ffff 
	It's reserved for iNIC using.
 */
#endif // RELEASE_EXCLUDE //

#if 0				/* os abl move to rtmp_comm.h */
#ifndef TRUE
#define TRUE				1
#endif
#ifndef FALSE
#define FALSE				0
#endif
/* */
/* IEEE 802.11 Structures and definitions */
/* */
#define MAX_TX_POWER_LEVEL              100	/* mW */
#define MAX_RSSI_TRIGGER                -10	/* dBm */
#define MIN_RSSI_TRIGGER                -200	/* dBm */
#define MAX_FRAG_THRESHOLD              2346	/* byte count */
#define MIN_FRAG_THRESHOLD              256	/* byte count */
#define MAX_RTS_THRESHOLD               2347	/* byte count */
#endif /* 0 */

/* new types for Media Specific Indications */
/* Extension channel offset */
#define EXTCHA_NONE			0
#define EXTCHA_ABOVE		0x1
#define EXTCHA_BELOW		0x3

/* BW */
#define BAND_WIDTH_20		0
#define BAND_WIDTH_40		1
#define BAND_WIDTH_80		2
#define BAND_WIDTH_BOTH	3
#define BAND_WIDTH_10		4	/* 802.11j has 10MHz. This definition is for internal usage. doesn't fill in the IE or other field. */


/* SHORTGI */
#define GAP_INTERVAL_400	1	/* only support in HT mode */
#define GAP_INTERVAL_800	0
#define GAP_INTERVAL_BOTH	2

#define NdisMediaStateConnected			1
#define NdisMediaStateDisconnected		0

#define NdisApMediaStateConnected			1
#define NdisApMediaStateDisconnected		0


#define NDIS_802_11_LENGTH_SSID         32

#define MAC_ADDR_LEN			6
#define IEEE80211_ADDR_LEN		6	/* size of 802.11 address */
#define IEEE80211_NWID_LEN		32

#define NDIS_802_11_LENGTH_RATES        8
#define NDIS_802_11_LENGTH_RATES_EX     16

#define OID_P2P_DEVICE_NAME_LEN	32
/*#define MAX_NUM_OF_CHS					49 */ /* 14 channels @2.4G +  12@UNII + 4 @MMAC + 11 @HiperLAN2 + 7 @Japan + 1 as NULL terminationc */
/*#define MAX_NUM_OF_CHS             		54 */ /* 14 channels @2.4G +  12@UNII(lower/middle) + 16@HiperLAN2 + 11@UNII(upper) + 0 @Japan + 1 as NULL termination */
#define MAX_NUMBER_OF_EVENT				10	/* entry # in EVENT table */

#ifdef BB_SOC
#define MAX_NUMBER_OF_MAC				8 // if MAX_MBSSID_NUM is 8, this value can't be larger than 211
#else
#if defined(MT7603_FPGA) || defined(MT7628_FPGA) || defined(MT7636_FPGA)
#define MAX_NUMBER_OF_MAC				4	/* if MAX_MBSSID_NUM is 8, this value can't be larger than 211 */
#elif defined(MT7603)
#ifdef MAC_REPEATER_SUPPORT
#ifdef MULTI_APCLI_SUPPORT
#define MAX_NUMBER_OF_MAC				(75 - (16 + 1) * 2)
#else
#define MAX_NUMBER_OF_MAC				(75 - (16 + 1) * 1)
#endif /* MULTI_APCLI_SUPPORT */
#else
#define MAX_NUMBER_OF_MAC            	75
#endif /* MAC_REPEATER_SUPPORT */
#elif defined(MT76x2)
#ifdef MAC_REPEATER_SUPPORT				//((MAX_EXT_MAC_ADDR_SIZE + 1) * MAC_APCLI_NUM)
#define MAX_NUMBER_OF_MAC               (116 - ((16 + 1) * 1))
#else
#define MAX_NUMBER_OF_MAC			   	 116
#endif /* MAC_REPEATER_SUPPORT */
#else
#define MAX_NUMBER_OF_MAC				32	/* if MAX_MBSSID_NUM is 8, this value can't be larger than 211 */
#endif /* defined(MT7603_FPGA) || defined(MT7628_FPGA) */
#endif /* BB_SOC */

#define MAX_NUMBER_OF_ACL				64
#define MAX_LENGTH_OF_SUPPORT_RATES		12	/* 1, 2, 5.5, 11, 6, 9, 12, 18, 24, 36, 48, 54 */
#define MAX_NUMBER_OF_DLS_ENTRY			4

#ifdef MESH_SUPPORT
#define MAX_NEIGHBOR_NUM                64
#define MAX_MESH_LINK_NUM               4
#define MAX_HOST_NAME_LENGTH            64
#define MAX_MESH_ID_LENGTH              32
#define MESH_MAX_FORWARD_ENTRY_NUM      48
#endif /* MESH_SUPPORT */

#define RT_QUERY_SIGNAL_CONTEXT				0x0402
#define RT_SET_IAPP_PID                 	0x0404
#define RT_SET_APD_PID						0x0405
#define RT_SET_DEL_MAC_ENTRY				0x0406
#define RT_QUERY_EVENT_TABLE            	0x0407
#ifdef DOT11R_FT_SUPPORT
#define RT_SET_FT_STATION_NOTIFY			0x0408
#define RT_SET_FT_KEY_REQ					0x0409
#define RT_SET_FT_KEY_RSP					0x040a
#define RT_FT_KEY_SET						0x040b
#define RT_FT_DATA_ENCRYPT					0x040c
#define RT_FT_DATA_DECRYPT					0x040d
#define RT_FT_NEIGHBOR_REPORT				0x040e
#define RT_FT_NEIGHBOR_REQUEST				0x040f
#define RT_FT_NEIGHBOR_RESPONSE				0x0410
#define RT_FT_ACTION_FORWARD				0x0411
#endif /* DOT11R_FT_SUPPORT */
/* */
/* IEEE 802.11 OIDs */
/* */
#define	OID_GET_SET_TOGGLE			0x8000
#define	OID_GET_SET_FROM_UI			0x4000

#define	OID_802_11_NETWORK_TYPES_SUPPORTED			0x0103
#define	OID_802_11_NETWORK_TYPE_IN_USE				0x0104
#define	OID_802_11_RSSI_TRIGGER						0x0107
#define	RT_OID_802_11_RSSI							0x0108	/* rt2860 only */
#define	RT_OID_802_11_RSSI_1						0x0109	/* rt2860 only */
#define	RT_OID_802_11_RSSI_2						0x010A	/* rt2860 only */
#define	OID_802_11_NUMBER_OF_ANTENNAS				0x010B
#define	OID_802_11_RX_ANTENNA_SELECTED				0x010C
#define	OID_802_11_TX_ANTENNA_SELECTED				0x010D
#define	OID_802_11_SUPPORTED_RATES					0x010E
#define	OID_802_11_ADD_WEP							0x0112
#define	OID_802_11_REMOVE_WEP						0x0113
#define	OID_802_11_DISASSOCIATE						0x0114
#define	OID_802_11_PRIVACY_FILTER					0x0118
#define	OID_802_11_ASSOCIATION_INFORMATION			0x011E
#define	OID_802_11_TEST								0x011F

#ifdef WMM_ACM_SUPPORT
#define RT_OID_WMM_ACM_TSPEC                        0x0450
#define RT_OID_WMM_ACM_BandWidth                          0x0788
#endif /* WMM_ACM_SUPPORT */

#define	RT_OID_802_11_COUNTRY_REGION				0x0507
#define	OID_802_11_BSSID_LIST_SCAN					0x0508
#define	OID_802_11_SSID								0x0509
#define	OID_802_11_BSSID							0x050A
#define	RT_OID_802_11_RADIO							0x050B
#define	RT_OID_802_11_PHY_MODE						0x050C
#define	RT_OID_802_11_STA_CONFIG					0x050D
#define	OID_802_11_DESIRED_RATES					0x050E
#define	RT_OID_802_11_PREAMBLE						0x050F
#define	OID_802_11_WEP_STATUS						0x0510
#define	OID_802_11_AUTHENTICATION_MODE				0x0511
#define	OID_802_11_INFRASTRUCTURE_MODE				0x0512
#define	RT_OID_802_11_RESET_COUNTERS				0x0513
#define	OID_802_11_RTS_THRESHOLD					0x0514
#define	OID_802_11_FRAGMENTATION_THRESHOLD			0x0515
#define	OID_802_11_POWER_MODE						0x0516
#define	OID_802_11_TX_POWER_LEVEL					0x0517
#define	RT_OID_802_11_ADD_WPA						0x0518
#define	OID_802_11_REMOVE_KEY						0x0519
#define	RT_OID_802_11_QUERY_PID						0x051A
#define	RT_OID_802_11_QUERY_VID						0x051B
#define	OID_802_11_ADD_KEY							0x0520
#define	OID_802_11_CONFIGURATION					0x0521
#define	OID_802_11_TX_PACKET_BURST					0x0522
#define	RT_OID_802_11_QUERY_NOISE_LEVEL				0x0523
#define	RT_OID_802_11_EXTRA_INFO					0x0524
#define	RT_OID_802_11_HARDWARE_REGISTER				0x0525
#define OID_802_11_ENCRYPTION_STATUS            OID_802_11_WEP_STATUS
#define OID_802_11_DEAUTHENTICATION                 0x0526
#define OID_802_11_DROP_UNENCRYPTED                 0x0527
#define OID_802_11_MIC_FAILURE_REPORT_FRAME         0x0528
#define OID_802_11_EAP_METHOD						0x0529
#define OID_802_11_ACL_LIST							0x052A

/* For 802.1x daemin using */
#ifdef DOT1X_SUPPORT
#define OID_802_DOT1X_CONFIGURATION					0x0540
#define OID_802_DOT1X_PMKID_CACHE					0x0541
#define OID_802_DOT1X_RADIUS_DATA					0x0542
#define OID_802_DOT1X_WPA_KEY						0x0543
#define OID_802_DOT1X_STATIC_WEP_COPY				0x0544
#define OID_802_DOT1X_IDLE_TIMEOUT					0x0545
#define OID_802_DOT1X_RADIUS_ACL_NEW_CACHE          0x0546
#define OID_802_DOT1X_RADIUS_ACL_DEL_CACHE          0x0547
#define OID_802_DOT1X_RADIUS_ACL_CLEAR_CACHE        0x0548
#define OID_802_DOT1X_QUERY_STA_AID                 0x0549
#endif /* DOT1X_SUPPORT */

#define	RT_OID_DEVICE_NAME							0x0607
#define	RT_OID_VERSION_INFO							0x0608
#define	OID_802_11_BSSID_LIST						0x0609
#define	OID_802_3_CURRENT_ADDRESS					0x060A
#define	OID_GEN_MEDIA_CONNECT_STATUS				0x060B
#define	RT_OID_802_11_QUERY_LINK_STATUS				0x060C
#define	OID_802_11_RSSI								0x060D
#define	OID_802_11_STATISTICS						0x060E
#define	OID_GEN_RCV_OK								0x060F
#define	OID_GEN_RCV_NO_BUFFER						0x0610
#define	RT_OID_802_11_QUERY_EEPROM_VERSION			0x0611
#define	RT_OID_802_11_QUERY_FIRMWARE_VERSION		0x0612
#define	RT_OID_802_11_QUERY_LAST_RX_RATE			0x0613
#define	RT_OID_802_11_TX_POWER_LEVEL_1				0x0614
#define	RT_OID_802_11_QUERY_PIDVID					0x0615
/*for WPA_SUPPLICANT_SUPPORT */
#define OID_SET_COUNTERMEASURES                     0x0616
#define OID_802_11_SET_IEEE8021X                    0x0617
#define OID_802_11_SET_IEEE8021X_REQUIRE_KEY        0x0618
#define OID_802_11_PMKID                            0x0620
#define RT_OID_WPA_SUPPLICANT_SUPPORT               0x0621
#define RT_OID_WE_VERSION_COMPILED                  0x0622
#define RT_OID_NEW_DRIVER                           0x0623
#define	OID_AUTO_PROVISION_BSSID_LIST				0x0624
#define RT_OID_WPS_PROBE_REQ_IE						0x0625

#define	RT_OID_802_11_SNR_0							0x0630
#define	RT_OID_802_11_SNR_1							0x0631
#define	RT_OID_802_11_QUERY_LAST_TX_RATE			0x0632
#define	RT_OID_802_11_QUERY_HT_PHYMODE				0x0633
#define	RT_OID_802_11_SET_HT_PHYMODE				0x0634
#define	OID_802_11_RELOAD_DEFAULTS					0x0635
#define	RT_OID_802_11_QUERY_APSD_SETTING			0x0636
#define	RT_OID_802_11_SET_APSD_SETTING				0x0637
#define	RT_OID_802_11_QUERY_APSD_PSM				0x0638
#define	RT_OID_802_11_SET_APSD_PSM					0x0639
#define	RT_OID_802_11_QUERY_DLS						0x063A
#define	RT_OID_802_11_SET_DLS						0x063B
#define	RT_OID_802_11_QUERY_DLS_PARAM				0x063C
#define	RT_OID_802_11_SET_DLS_PARAM					0x063D
#define RT_OID_802_11_QUERY_WMM              		0x063E
#define RT_OID_802_11_SET_WMM      					0x063F
#define RT_OID_802_11_QUERY_IMME_BA_CAP				0x0640
#define RT_OID_802_11_SET_IMME_BA_CAP				0x0641
#define RT_OID_802_11_QUERY_BATABLE					0x0642
#define RT_OID_802_11_ADD_IMME_BA					0x0643
#define RT_OID_802_11_TEAR_IMME_BA					0x0644
#define RT_OID_DRIVER_DEVICE_NAME                   0x0645
#define RT_OID_802_11_QUERY_DAT_HT_PHYMODE          0x0646
#define RT_OID_QUERY_MULTIPLE_CARD_SUPPORT          0x0647
#define OID_802_11_SET_PSPXLINK_MODE				0x0648
/*+++ add by woody +++*/
#define OID_802_11_SET_PASSPHRASE				0x0649
#define RT_OID_802_11_QUERY_TX_PHYMODE                          0x0650
#define RT_OID_802_11_QUERY_MAP_REAL_TX_RATE                          0x0678
#define RT_OID_802_11_QUERY_MAP_REAL_RX_RATE                          0x0679
#define	RT_OID_802_11_SNR_2							0x067A
#define RT_OID_802_11_PER_BSS_STATISTICS			0x067D
#if defined(RT2883) || defined(RT3883) || defined(RT3593)
#define	RT_OID_802_11_STREAM_SNR					0x067B
#endif /* defined(RT2883) || defined(RT3883) || defined(RT3593) */
#define OID_802_11_MBSS_GET_STA_COUNT					0x067E

#ifdef TXBF_SUPPORT
#define RT_OID_802_11_QUERY_TXBF_TABLE				0x067C
#endif

#ifdef RTMP_RBUS_SUPPORT
#define OID_802_11_QUERY_WirelessMode				0x0718
#endif /* RTMP_RBUS_SUPPORT */

#ifdef HOSTAPD_SUPPORT
#define SIOCSIWGENIE	0x8B30
#define OID_HOSTAPD_SUPPORT               0x0661

#define HOSTAPD_OID_STATIC_WEP_COPY   0x0662
#define HOSTAPD_OID_GET_1X_GROUP_KEY   0x0663

#define HOSTAPD_OID_SET_STA_AUTHORIZED   0x0664
#define HOSTAPD_OID_SET_STA_DISASSOC   0x0665
#define HOSTAPD_OID_SET_STA_DEAUTH   0x0666
#define HOSTAPD_OID_DEL_KEY   0x0667
#define HOSTAPD_OID_SET_KEY   0x0668
#define HOSTAPD_OID_SET_802_1X   0x0669
#define HOSTAPD_OID_GET_SEQ   0x0670
#define HOSTAPD_OID_GETWPAIE                 0x0671
#define HOSTAPD_OID_COUNTERMEASURES 0x0672
#define HOSTAPD_OID_SET_WPAPSK 0x0673
#define HOSTAPD_OID_SET_WPS_BEACON_IE 0x0674
#define HOSTAPD_OID_SET_WPS_PROBE_RESP_IE 0x0675

#define	RT_HOSTAPD_OID_HOSTAPD_SUPPORT				(OID_GET_SET_TOGGLE |	OID_HOSTAPD_SUPPORT)
#define	RT_HOSTAPD_OID_STATIC_WEP_COPY				(OID_GET_SET_TOGGLE |	HOSTAPD_OID_STATIC_WEP_COPY)
#define	RT_HOSTAPD_OID_GET_1X_GROUP_KEY				(OID_GET_SET_TOGGLE |	HOSTAPD_OID_GET_1X_GROUP_KEY)
#define	RT_HOSTAPD_OID_SET_STA_AUTHORIZED			(OID_GET_SET_TOGGLE |	HOSTAPD_OID_SET_STA_AUTHORIZED)
#define	RT_HOSTAPD_OID_SET_STA_DISASSOC				(OID_GET_SET_TOGGLE |	HOSTAPD_OID_SET_STA_DISASSOC)
#define	RT_HOSTAPD_OID_SET_STA_DEAUTH				(OID_GET_SET_TOGGLE |	HOSTAPD_OID_SET_STA_DEAUTH)
#define	RT_HOSTAPD_OID_DEL_KEY						(OID_GET_SET_TOGGLE |	HOSTAPD_OID_DEL_KEY)
#define	RT_HOSTAPD_OID_SET_KEY						(OID_GET_SET_TOGGLE |	HOSTAPD_OID_SET_KEY)
#define	RT_HOSTAPD_OID_SET_802_1X						(OID_GET_SET_TOGGLE |	HOSTAPD_OID_SET_802_1X)
#define	RT_HOSTAPD_OID_COUNTERMEASURES				(OID_GET_SET_TOGGLE |	HOSTAPD_OID_COUNTERMEASURES)
#define	RT_HOSTAPD_OID_SET_WPAPSK				(OID_GET_SET_TOGGLE |	HOSTAPD_OID_SET_WPAPSK)
#define	RT_HOSTAPD_OID_SET_WPS_BEACON_IE				(OID_GET_SET_TOGGLE |	HOSTAPD_OID_SET_WPS_BEACON_IE)
#define	RT_HOSTAPD_OID_SET_WPS_PROBE_RESP_IE				(OID_GET_SET_TOGGLE |	HOSTAPD_OID_SET_WPS_PROBE_RESP_IE)

#define IEEE80211_IS_MULTICAST(_a) (*(_a) & 0x01)
#define	IEEE80211_KEYBUF_SIZE	16
#define	IEEE80211_MICBUF_SIZE	(8 + 8)	/* space for both tx+rx keys */
#define IEEE80211_TID_SIZE		17	/* total number of TIDs */

#define	IEEE80211_MLME_ASSOC		    1	/* associate station */
#define	IEEE80211_MLME_DISASSOC		    2	/* disassociate station */
#define	IEEE80211_MLME_DEAUTH		    3	/* deauthenticate station */
#define	IEEE80211_MLME_AUTHORIZE	    4	/* authorize station */
#define	IEEE80211_MLME_UNAUTHORIZE	    5	/* unauthorize station */
#define IEEE80211_MLME_CLEAR_STATS	    6	/* clear station statistic */
#define IEEE80211_1X_COPY_KEY        	7	/* copy static-wep unicast key */

#define	IEEE80211_MAX_OPT_IE	256
#define IWEVEXPIRED	0x8C04

struct ieee80211req_mlme {
	UINT8 im_op;		/* operation to perform */
	UINT8 im_ssid_len;	/* length of optional ssid */
	UINT16 im_reason;	/* 802.11 reason code */
	UINT8 im_macaddr[IEEE80211_ADDR_LEN];
	UINT8 im_ssid[IEEE80211_NWID_LEN];
};

struct ieee80211req_key {
	UINT8 ik_type;		/* key/cipher type */
	UINT8 ik_pad;
	UINT16 ik_keyix;	/* key index */
	UINT8 ik_keylen;	/* key length in bytes */
	UINT8 ik_flags;
	UINT8 ik_macaddr[IEEE80211_ADDR_LEN];
	UINT64 ik_keyrsc;	/* key receive sequence counter */
	UINT64 ik_keytsc;	/* key transmit sequence counter */
	UINT8 ik_keydata[IEEE80211_KEYBUF_SIZE + IEEE80211_MICBUF_SIZE];
	int txkey;
};

struct ieee80211req_del_key {
	UINT8 idk_keyix;	/* key index */
	UINT8 idk_macaddr[IEEE80211_ADDR_LEN];
};

struct default_group_key {
	UINT16 ik_keyix;	/* key index */
	UINT8 ik_keylen;	/* key length in bytes */
	UINT8 ik_keydata[IEEE80211_KEYBUF_SIZE + IEEE80211_MICBUF_SIZE];
};

struct ieee80211req_wpaie {
	UINT8 wpa_macaddr[IEEE80211_ADDR_LEN];
	UINT8 rsn_ie[IEEE80211_MAX_OPT_IE];
};

struct hostapd_wpa_psk {
	struct hostapd_wpa_psk *next;
	int group;
	UCHAR psk[32];
	UCHAR addr[6];
};

#endif /*HOSTAPD_SUPPORT */

#define RT_OID_802_11_QUERY_TDLS_PARAM			0x0676
#define	RT_OID_802_11_QUERY_TDLS				0x0677

/* Ralink defined OIDs */
/* Dennis Lee move to platform specific */

#define	RT_OID_802_11_BSSID					  (OID_GET_SET_TOGGLE |	OID_802_11_BSSID)
#define	RT_OID_802_11_SSID					  (OID_GET_SET_TOGGLE |	OID_802_11_SSID)
#define	RT_OID_802_11_INFRASTRUCTURE_MODE	  (OID_GET_SET_TOGGLE |	OID_802_11_INFRASTRUCTURE_MODE)
#define	RT_OID_802_11_ADD_WEP				  (OID_GET_SET_TOGGLE |	OID_802_11_ADD_WEP)
#define	RT_OID_802_11_ADD_KEY				  (OID_GET_SET_TOGGLE |	OID_802_11_ADD_KEY)
#define	RT_OID_802_11_REMOVE_WEP			  (OID_GET_SET_TOGGLE |	OID_802_11_REMOVE_WEP)
#define	RT_OID_802_11_REMOVE_KEY			  (OID_GET_SET_TOGGLE |	OID_802_11_REMOVE_KEY)
#define	RT_OID_802_11_DISASSOCIATE			  (OID_GET_SET_TOGGLE |	OID_802_11_DISASSOCIATE)
#define	RT_OID_802_11_AUTHENTICATION_MODE	  (OID_GET_SET_TOGGLE |	OID_802_11_AUTHENTICATION_MODE)
#define	RT_OID_802_11_PRIVACY_FILTER		  (OID_GET_SET_TOGGLE |	OID_802_11_PRIVACY_FILTER)
#define	RT_OID_802_11_BSSID_LIST_SCAN		  (OID_GET_SET_TOGGLE |	OID_802_11_BSSID_LIST_SCAN)
#define	RT_OID_802_11_WEP_STATUS			  (OID_GET_SET_TOGGLE |	OID_802_11_WEP_STATUS)
#define	RT_OID_802_11_RELOAD_DEFAULTS		  (OID_GET_SET_TOGGLE |	OID_802_11_RELOAD_DEFAULTS)
#define	RT_OID_802_11_NETWORK_TYPE_IN_USE	  (OID_GET_SET_TOGGLE |	OID_802_11_NETWORK_TYPE_IN_USE)
#define	RT_OID_802_11_TX_POWER_LEVEL		  (OID_GET_SET_TOGGLE |	OID_802_11_TX_POWER_LEVEL)
#define	RT_OID_802_11_RSSI_TRIGGER			  (OID_GET_SET_TOGGLE |	OID_802_11_RSSI_TRIGGER)
#define	RT_OID_802_11_FRAGMENTATION_THRESHOLD (OID_GET_SET_TOGGLE |	OID_802_11_FRAGMENTATION_THRESHOLD)
#define	RT_OID_802_11_RTS_THRESHOLD			  (OID_GET_SET_TOGGLE |	OID_802_11_RTS_THRESHOLD)
#define	RT_OID_802_11_RX_ANTENNA_SELECTED	  (OID_GET_SET_TOGGLE |	OID_802_11_RX_ANTENNA_SELECTED)
#define	RT_OID_802_11_TX_ANTENNA_SELECTED	  (OID_GET_SET_TOGGLE |	OID_802_11_TX_ANTENNA_SELECTED)
#define	RT_OID_802_11_SUPPORTED_RATES		  (OID_GET_SET_TOGGLE |	OID_802_11_SUPPORTED_RATES)
#define	RT_OID_802_11_DESIRED_RATES			  (OID_GET_SET_TOGGLE |	OID_802_11_DESIRED_RATES)
#define	RT_OID_802_11_CONFIGURATION			  (OID_GET_SET_TOGGLE |	OID_802_11_CONFIGURATION)
#define	RT_OID_802_11_POWER_MODE			  (OID_GET_SET_TOGGLE |	OID_802_11_POWER_MODE)
#define RT_OID_802_11_SET_PSPXLINK_MODE		  (OID_GET_SET_TOGGLE |	OID_802_11_SET_PSPXLINK_MODE)
#define RT_OID_802_11_EAP_METHOD			  (OID_GET_SET_TOGGLE | OID_802_11_EAP_METHOD)
#define RT_OID_802_11_SET_PASSPHRASE		  (OID_GET_SET_TOGGLE | OID_802_11_SET_PASSPHRASE)

#ifdef DOT1X_SUPPORT
#define RT_OID_802_DOT1X_PMKID_CACHE		(OID_GET_SET_TOGGLE | OID_802_DOT1X_PMKID_CACHE)
#define RT_OID_802_DOT1X_RADIUS_DATA		(OID_GET_SET_TOGGLE | OID_802_DOT1X_RADIUS_DATA)
#define RT_OID_802_DOT1X_WPA_KEY			(OID_GET_SET_TOGGLE | OID_802_DOT1X_WPA_KEY)
#define RT_OID_802_DOT1X_STATIC_WEP_COPY	(OID_GET_SET_TOGGLE | OID_802_DOT1X_STATIC_WEP_COPY)
#define RT_OID_802_DOT1X_IDLE_TIMEOUT		(OID_GET_SET_TOGGLE | OID_802_DOT1X_IDLE_TIMEOUT)
#endif /* DOT1X_SUPPORT */

#define RT_OID_802_11_SET_TDLS_PARAM			(OID_GET_SET_TOGGLE | RT_OID_802_11_QUERY_TDLS_PARAM)
#define RT_OID_802_11_SET_TDLS				(OID_GET_SET_TOGGLE | RT_OID_802_11_QUERY_TDLS)

#ifdef SUPPORT_ACS_ALL_CHANNEL_RANK
#define OID_GET_ACS_RANK_LIST           0x06B7 /* Get AutoChannelSelection Rank list */
#endif

#ifdef WAPI_SUPPORT
#define OID_802_11_WAPI_PID					0x06A0
#define OID_802_11_PORT_SECURE_STATE		0x06A1
#define OID_802_11_UCAST_KEY_INFO			0x06A2
#define OID_802_11_MCAST_TXIV				0x06A3
#define OID_802_11_MCAST_KEY_INFO			0x06A4
#define OID_802_11_WAPI_CONFIGURATION		0x06A5
#define OID_802_11_WAPI_IE					0x06A6

#define RT_OID_802_11_WAPI_PID				(OID_GET_SET_TOGGLE | OID_802_11_WAPI_PID)
#define RT_OID_802_11_PORT_SECURE_STATE		(OID_GET_SET_TOGGLE | OID_802_11_PORT_SECURE_STATE)
#define RT_OID_802_11_UCAST_KEY_INFO		(OID_GET_SET_TOGGLE | OID_802_11_UCAST_KEY_INFO)
#define RT_OID_802_11_MCAST_TXIV			(OID_GET_SET_TOGGLE | OID_802_11_MCAST_TXIV)
#define RT_OID_802_11_MCAST_KEY_INFO		(OID_GET_SET_TOGGLE | OID_802_11_MCAST_KEY_INFO)
#define RT_OID_802_11_WAPI_CONFIGURATION	(OID_GET_SET_TOGGLE | OID_802_11_WAPI_CONFIGURATION)
#define RT_OID_802_11_WAPI_IE				(OID_GET_SET_TOGGLE | OID_802_11_WAPI_IE)
#endif /* WAPI_SUPPORT */


typedef enum _NDIS_802_11_STATUS_TYPE {
	Ndis802_11StatusType_Authentication,
	Ndis802_11StatusType_MediaStreamMode,
	Ndis802_11StatusType_PMKID_CandidateList,
	Ndis802_11StatusTypeMax	/* not a real type, defined as an upper bound */
} NDIS_802_11_STATUS_TYPE, *PNDIS_802_11_STATUS_TYPE;

typedef UCHAR NDIS_802_11_MAC_ADDRESS[6];

typedef struct _NDIS_802_11_STATUS_INDICATION {
	NDIS_802_11_STATUS_TYPE StatusType;
} NDIS_802_11_STATUS_INDICATION, *PNDIS_802_11_STATUS_INDICATION;

/* mask for authentication/integrity fields */
#define NDIS_802_11_AUTH_REQUEST_AUTH_FIELDS        0x0f

#define NDIS_802_11_AUTH_REQUEST_REAUTH             0x01
#define NDIS_802_11_AUTH_REQUEST_KEYUPDATE          0x02
#define NDIS_802_11_AUTH_REQUEST_PAIRWISE_ERROR     0x06
#define NDIS_802_11_AUTH_REQUEST_GROUP_ERROR        0x0E

typedef struct _NDIS_802_11_AUTHENTICATION_REQUEST {
	ULONG Length;		/* Length of structure */
	NDIS_802_11_MAC_ADDRESS Bssid;
	ULONG Flags;
} NDIS_802_11_AUTHENTICATION_REQUEST, *PNDIS_802_11_AUTHENTICATION_REQUEST;

/*Added new types for PMKID Candidate lists. */
typedef struct _PMKID_CANDIDATE {
	NDIS_802_11_MAC_ADDRESS BSSID;
	ULONG Flags;
} PMKID_CANDIDATE, *PPMKID_CANDIDATE;

typedef struct _NDIS_802_11_PMKID_CANDIDATE_LIST {
	ULONG Version;		/* Version of the structure */
	ULONG NumCandidates;	/* No. of pmkid candidates */
	PMKID_CANDIDATE CandidateList[1];
} NDIS_802_11_PMKID_CANDIDATE_LIST, *PNDIS_802_11_PMKID_CANDIDATE_LIST;

/*Flags for PMKID Candidate list structure */
#define NDIS_802_11_PMKID_CANDIDATE_PREAUTH_ENABLED	0x01

/* Added new types for OFDM 5G and 2.4G */
typedef enum _NDIS_802_11_NETWORK_TYPE {
	Ndis802_11FH,
	Ndis802_11DS,
	Ndis802_11OFDM5,
	Ndis802_11OFDM24,
	Ndis802_11Automode,
	Ndis802_11OFDM5_N,
	Ndis802_11OFDM24_N,
	Ndis802_11NetworkTypeMax	/* not a real type, defined as an upper bound */
} NDIS_802_11_NETWORK_TYPE, *PNDIS_802_11_NETWORK_TYPE;

typedef struct _NDIS_802_11_NETWORK_TYPE_LIST {
	UINT NumberOfItems;	/* in list below, at least 1 */
	NDIS_802_11_NETWORK_TYPE NetworkType[1];
} NDIS_802_11_NETWORK_TYPE_LIST, *PNDIS_802_11_NETWORK_TYPE_LIST;

typedef enum _NDIS_802_11_POWER_MODE {
	Ndis802_11PowerModeCAM,
	Ndis802_11PowerModeMAX_PSP,
	Ndis802_11PowerModeFast_PSP,
	Ndis802_11PowerModeLegacy_PSP,
	Ndis802_11PowerModeMax	/* not a real mode, defined as an upper bound */
} NDIS_802_11_POWER_MODE, *PNDIS_802_11_POWER_MODE;

typedef ULONG NDIS_802_11_TX_POWER_LEVEL;	/* in milliwatts */

/* */
/* Received Signal Strength Indication */
/* */
typedef LONG NDIS_802_11_RSSI;	/* in dBm */

typedef struct _NDIS_802_11_CONFIGURATION_FH {
	ULONG Length;		/* Length of structure */
	ULONG HopPattern;	/* As defined by 802.11, MSB set */
	ULONG HopSet;		/* to one if non-802.11 */
	ULONG DwellTime;	/* units are Kusec */
} NDIS_802_11_CONFIGURATION_FH, *PNDIS_802_11_CONFIGURATION_FH;

typedef struct _NDIS_802_11_CONFIGURATION {
	ULONG Length;		/* Length of structure */
	ULONG BeaconPeriod;	/* units are Kusec */
	ULONG ATIMWindow;	/* units are Kusec */
	ULONG DSConfig;		/* Frequency, units are kHz */
	NDIS_802_11_CONFIGURATION_FH FHConfig;
} NDIS_802_11_CONFIGURATION, *PNDIS_802_11_CONFIGURATION;

typedef struct _NDIS_802_11_STATISTICS {
	ULONG Length;		/* Length of structure */
	LARGE_INTEGER TransmittedFragmentCount;
	LARGE_INTEGER MulticastTransmittedFrameCount;
	LARGE_INTEGER FailedCount;
	LARGE_INTEGER RetryCount;
	LARGE_INTEGER MultipleRetryCount;
	LARGE_INTEGER RTSSuccessCount;
	LARGE_INTEGER RTSFailureCount;
	LARGE_INTEGER ACKFailureCount;
	LARGE_INTEGER FrameDuplicateCount;
	LARGE_INTEGER ReceivedFragmentCount;
	LARGE_INTEGER MulticastReceivedFrameCount;
	LARGE_INTEGER FCSErrorCount;
	LARGE_INTEGER TransmittedFrameCount;
	LARGE_INTEGER WEPUndecryptableCount;
	LARGE_INTEGER TKIPLocalMICFailures;
	LARGE_INTEGER TKIPRemoteMICErrors;
	LARGE_INTEGER TKIPICVErrors;
	LARGE_INTEGER TKIPCounterMeasuresInvoked;
	LARGE_INTEGER TKIPReplays;
	LARGE_INTEGER CCMPFormatErrors;
	LARGE_INTEGER CCMPReplays;
	LARGE_INTEGER CCMPDecryptErrors;
	LARGE_INTEGER FourWayHandshakeFailures;
} NDIS_802_11_STATISTICS, *PNDIS_802_11_STATISTICS;

typedef struct _MBSS_STATISTICS {
	LONG TxCount;
	ULONG RxCount;
	ULONG ReceivedByteCount;
	ULONG TransmittedByteCount;
	ULONG RxErrorCount;
	ULONG RxDropCount;
	ULONG TxErrorCount;
	ULONG TxDropCount;
	ULONG ucPktsTx;
	ULONG ucPktsRx;
	ULONG mcPktsTx;
	ULONG mcPktsRx;
	ULONG bcPktsTx;
	ULONG bcPktsRx;
} MBSS_STATISTICS, *PMBSS_STATISTICS;

typedef ULONG NDIS_802_11_KEY_INDEX;
typedef ULONGLONG NDIS_802_11_KEY_RSC;

#ifdef DOT1X_SUPPORT
#define MAX_RADIUS_SRV_NUM			2	/* 802.1x failover number */

/* The dot1x related structure. 
   It's used to communicate with DOT1X daemon */
typedef struct GNU_PACKED _RADIUS_SRV_INFO {
	UINT32 radius_ip;
	UINT32 radius_port;
	UCHAR radius_key[64];
	UCHAR radius_key_len;
} RADIUS_SRV_INFO, *PRADIUS_SRV_INFO;

typedef struct GNU_PACKED _DOT1X_BSS_INFO {
	UCHAR radius_srv_num;
	RADIUS_SRV_INFO radius_srv_info[MAX_RADIUS_SRV_NUM];
	UCHAR ieee8021xWEP;	/* dynamic WEP */
	UCHAR key_index;
	UCHAR key_length;	/* length of key in bytes */
	UCHAR key_material[13];
	UCHAR nasId[IFNAMSIZ];
	UCHAR nasId_len;
} DOT1X_BSS_INFO, *PDOT1X_BSS_INFO;

typedef struct GNU_PACKED _DOT1X_CMM_CONF {
	UINT32 Length;		/* Length of this structure */
	UCHAR mbss_num;		/* indicate multiple BSS number */
	UINT32 own_ip_addr;
	UINT32 retry_interval;
	UINT32 session_timeout_interval;
	UINT32 quiet_interval;
	UCHAR EAPifname[8][IFNAMSIZ];
	UCHAR EAPifname_len[8];
	UCHAR PreAuthifname[8][IFNAMSIZ];
	UCHAR PreAuthifname_len[8];
	DOT1X_BSS_INFO Dot1xBssInfo[8];
} DOT1X_CMM_CONF, *PDOT1X_CMM_CONF;

typedef struct GNU_PACKED _DOT1X_IDLE_TIMEOUT {
	UCHAR StaAddr[6];
	UINT32 idle_timeout;
} DOT1X_IDLE_TIMEOUT, *PDOT1X_IDLE_TIMEOUT;

typedef struct GNU_PACKED _DOT1X_QUERY_STA_AID {
	UCHAR StaAddr[MAC_ADDR_LEN];
	UINT aid;
} DOT1X_QUERY_STA_AID, *PDOT1X_QUERY_STA_AID;
#endif /* DOT1X_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
typedef struct _NDIS_AP_802_11_KEY {
	UINT Length;		/* Length of this structure */
	UCHAR addr[6];
	UINT KeyIndex;
	UINT KeyLength;		/* length of key in bytes */
	UCHAR KeyMaterial[1];	/* variable length depending on above field */
} NDIS_AP_802_11_KEY, *PNDIS_AP_802_11_KEY;
#endif /* CONFIG_AP_SUPPORT */

#ifdef APCLI_SUPPORT
#ifdef WPA_SUPPLICANT_SUPPORT
typedef struct _NDIS_APCLI_802_11_KEY
{
    UINT           Length;             
    UINT           KeyIndex;           
    UINT           KeyLength;         
    NDIS_802_11_MAC_ADDRESS BSSID;
    NDIS_802_11_KEY_RSC KeyRSC;
	UCHAR           KeyMaterial[];
} NDIS_APCLI_802_11_KEY, *PNDIS_APCLI_802_11_KEY;
#endif/* WPA_SUPPLICANT_SUPPORT */
#endif /* APCLI_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
/* Key mapping keys require a BSSID */
typedef struct _NDIS_802_11_KEY {
	UINT Length;		/* Length of this structure */
	UINT KeyIndex;
	UINT KeyLength;		/* length of key in bytes */
	NDIS_802_11_MAC_ADDRESS BSSID;
	NDIS_802_11_KEY_RSC KeyRSC;
	UCHAR KeyMaterial[];	/* variable length depending on above field */
} NDIS_802_11_KEY, *PNDIS_802_11_KEY;

typedef struct _NDIS_802_11_PASSPHRASE {
	UINT KeyLength;		/* length of key in bytes */
	NDIS_802_11_MAC_ADDRESS BSSID;
	UCHAR KeyMaterial[1];	/* variable length depending on above field */
} NDIS_802_11_PASSPHRASE, *PNDIS_802_11_PASSPHRASE;
#endif /* CONFIG_STA_SUPPORT */

typedef struct _NDIS_802_11_REMOVE_KEY {
	UINT Length;		/* Length of this structure */
	UINT KeyIndex;
	NDIS_802_11_MAC_ADDRESS BSSID;
} NDIS_802_11_REMOVE_KEY, *PNDIS_802_11_REMOVE_KEY;

typedef struct _NDIS_802_11_WEP {
	UINT Length;		/* Length of this structure */
	UINT KeyIndex;		/* 0 is the per-client key, 1-N are the */
	/* global keys */
	UINT KeyLength;		/* length of key in bytes */
	UCHAR KeyMaterial[1];	/* variable length depending on above field */
} NDIS_802_11_WEP, *PNDIS_802_11_WEP;


/* Add new authentication modes */
typedef enum _NDIS_802_11_AUTHENTICATION_MODE {
	Ndis802_11AuthModeOpen,
	Ndis802_11AuthModeShared,
	Ndis802_11AuthModeAutoSwitch,
	Ndis802_11AuthModeWPA,
	Ndis802_11AuthModeWPAPSK,
	Ndis802_11AuthModeWPANone,
	Ndis802_11AuthModeWPA2,
	Ndis802_11AuthModeWPA2PSK,
	Ndis802_11AuthModeWPA1WPA2,
	Ndis802_11AuthModeWPA1PSKWPA2PSK,
#ifdef WAPI_SUPPORT
	Ndis802_11AuthModeWAICERT,	/* WAI certificate authentication */
	Ndis802_11AuthModeWAIPSK,	/* WAI pre-shared key */
#endif				/* WAPI_SUPPORT */
	Ndis802_11AuthModeMax	/* Not a real mode, defined as upper bound */
} NDIS_802_11_AUTHENTICATION_MODE, *PNDIS_802_11_AUTHENTICATION_MODE;

typedef UCHAR NDIS_802_11_RATES[NDIS_802_11_LENGTH_RATES];	/* Set of 8 data rates */
typedef UCHAR NDIS_802_11_RATES_EX[NDIS_802_11_LENGTH_RATES_EX];	/* Set of 16 data rates */

typedef struct GNU_PACKED _NDIS_802_11_SSID {
	UINT SsidLength;	/* length of SSID field below, in bytes; */
	/* this can be zero. */
	UCHAR Ssid[NDIS_802_11_LENGTH_SSID];	/* SSID information field */
} NDIS_802_11_SSID, *PNDIS_802_11_SSID;

typedef struct GNU_PACKED _NDIS_WLAN_BSSID {
	ULONG Length;		/* Length of this structure */
	NDIS_802_11_MAC_ADDRESS MacAddress;	/* BSSID */
	UCHAR Reserved[2];
	NDIS_802_11_SSID Ssid;	/* SSID */
	ULONG Privacy;		/* WEP encryption requirement */
	NDIS_802_11_RSSI Rssi;	/* receive signal strength in dBm */
	NDIS_802_11_NETWORK_TYPE NetworkTypeInUse;
	NDIS_802_11_CONFIGURATION Configuration;
	NDIS_802_11_NETWORK_INFRASTRUCTURE InfrastructureMode;
	NDIS_802_11_RATES SupportedRates;
} NDIS_WLAN_BSSID, *PNDIS_WLAN_BSSID;

typedef struct GNU_PACKED _NDIS_802_11_BSSID_LIST {
	UINT NumberOfItems;	/* in list below, at least 1 */
	NDIS_WLAN_BSSID Bssid[1];
} NDIS_802_11_BSSID_LIST, *PNDIS_802_11_BSSID_LIST;

typedef struct {
	BOOLEAN bValid;		/* 1: variable contains valid value */
	USHORT StaNum;
	UCHAR ChannelUtilization;
	USHORT RemainingAdmissionControl;	/* in unit of 32-us */
} QBSS_LOAD_UI, *PQBSS_LOAD_UI;

/* Added Capabilities, IELength and IEs for each BSSID */
typedef struct GNU_PACKED _NDIS_WLAN_BSSID_EX {
	ULONG Length;		/* Length of this structure */
	NDIS_802_11_MAC_ADDRESS MacAddress;	/* BSSID */
	UCHAR WpsAP; /* 0x00: not support WPS, 0x01: support normal WPS, 0x02: support Ralink auto WPS, 0x04: support Samsung WAC */
	CHAR MinSNR;
	NDIS_802_11_SSID Ssid;	/* SSID */
	UINT Privacy;		/* WEP encryption requirement */
	NDIS_802_11_RSSI Rssi;	/* receive signal */
	/* strength in dBm */
	NDIS_802_11_NETWORK_TYPE NetworkTypeInUse;
	NDIS_802_11_CONFIGURATION Configuration;
	NDIS_802_11_NETWORK_INFRASTRUCTURE InfrastructureMode;
	NDIS_802_11_RATES_EX SupportedRates;
	ULONG IELength;
	UCHAR IEs[];
} NDIS_WLAN_BSSID_EX, *PNDIS_WLAN_BSSID_EX;

typedef struct GNU_PACKED _NDIS_802_11_BSSID_LIST_EX {
	UINT NumberOfItems;	/* in list below, at least 1 */
	NDIS_WLAN_BSSID_EX Bssid[1];
} NDIS_802_11_BSSID_LIST_EX, *PNDIS_802_11_BSSID_LIST_EX;

typedef struct GNU_PACKED _NDIS_802_11_FIXED_IEs {
	UCHAR Timestamp[8];
	USHORT BeaconInterval;
	USHORT Capabilities;
} NDIS_802_11_FIXED_IEs, *PNDIS_802_11_FIXED_IEs;

typedef struct _NDIS_802_11_VARIABLE_IEs {
	UCHAR ElementID;
	UCHAR Length;		/* Number of bytes in data field */
	UCHAR data[1];
} NDIS_802_11_VARIABLE_IEs, *PNDIS_802_11_VARIABLE_IEs;

typedef ULONG NDIS_802_11_FRAGMENTATION_THRESHOLD;

typedef ULONG NDIS_802_11_RTS_THRESHOLD;

typedef ULONG NDIS_802_11_ANTENNA;

typedef enum _NDIS_802_11_PRIVACY_FILTER {
	Ndis802_11PrivFilterAcceptAll,
	Ndis802_11PrivFilter8021xWEP
} NDIS_802_11_PRIVACY_FILTER, *PNDIS_802_11_PRIVACY_FILTER;

/* Added new encryption types */
/* Also aliased typedef to new name */
typedef enum _NDIS_802_11_WEP_STATUS {
	Ndis802_11WEPEnabled,
	Ndis802_11Encryption1Enabled = Ndis802_11WEPEnabled,
	Ndis802_11WEPDisabled,
	Ndis802_11EncryptionDisabled = Ndis802_11WEPDisabled,
	Ndis802_11WEPKeyAbsent,
	Ndis802_11Encryption1KeyAbsent = Ndis802_11WEPKeyAbsent,
	Ndis802_11WEPNotSupported,
	Ndis802_11EncryptionNotSupported = Ndis802_11WEPNotSupported,
	Ndis802_11TKIPEnable,
	Ndis802_11Encryption2Enabled = Ndis802_11TKIPEnable,
	Ndis802_11Encryption2KeyAbsent,
	Ndis802_11AESEnable,
	Ndis802_11Encryption3Enabled = Ndis802_11AESEnable,
	Ndis802_11Encryption3KeyAbsent,
	Ndis802_11TKIPAESMix,
	Ndis802_11Encryption4Enabled = Ndis802_11TKIPAESMix,	/* TKIP or AES mix */
	Ndis802_11Encryption4KeyAbsent,
	Ndis802_11GroupWEP40Enabled,
	Ndis802_11GroupWEP104Enabled,
#ifdef WAPI_SUPPORT
	Ndis802_11EncryptionSMS4Enabled,	/* WPI SMS4 support */
#endif /* WAPI_SUPPORT */
} NDIS_802_11_WEP_STATUS, *PNDIS_802_11_WEP_STATUS, NDIS_802_11_ENCRYPTION_STATUS, *PNDIS_802_11_ENCRYPTION_STATUS;

typedef enum _NDIS_802_11_RELOAD_DEFAULTS {
	Ndis802_11ReloadWEPKeys
} NDIS_802_11_RELOAD_DEFAULTS, *PNDIS_802_11_RELOAD_DEFAULTS;

#define NDIS_802_11_AI_REQFI_CAPABILITIES      1
#define NDIS_802_11_AI_REQFI_LISTENINTERVAL    2
#define NDIS_802_11_AI_REQFI_CURRENTAPADDRESS  4

#define NDIS_802_11_AI_RESFI_CAPABILITIES      1
#define NDIS_802_11_AI_RESFI_STATUSCODE        2
#define NDIS_802_11_AI_RESFI_ASSOCIATIONID     4

typedef struct _NDIS_802_11_AI_REQFI {
	USHORT Capabilities;
	USHORT ListenInterval;
	NDIS_802_11_MAC_ADDRESS CurrentAPAddress;
} NDIS_802_11_AI_REQFI, *PNDIS_802_11_AI_REQFI;

typedef struct _NDIS_802_11_AI_RESFI {
	USHORT Capabilities;
	USHORT StatusCode;
	USHORT AssociationId;
} NDIS_802_11_AI_RESFI, *PNDIS_802_11_AI_RESFI;

typedef struct _NDIS_802_11_ASSOCIATION_INFORMATION {
	ULONG Length;
	USHORT AvailableRequestFixedIEs;
	NDIS_802_11_AI_REQFI RequestFixedIEs;
	ULONG RequestIELength;
	ULONG OffsetRequestIEs;
	USHORT AvailableResponseFixedIEs;
	NDIS_802_11_AI_RESFI ResponseFixedIEs;
	ULONG ResponseIELength;
	ULONG OffsetResponseIEs;
} NDIS_802_11_ASSOCIATION_INFORMATION, *PNDIS_802_11_ASSOCIATION_INFORMATION;

typedef struct _NDIS_802_11_AUTHENTICATION_EVENT {
	NDIS_802_11_STATUS_INDICATION Status;
	NDIS_802_11_AUTHENTICATION_REQUEST Request[1];
} NDIS_802_11_AUTHENTICATION_EVENT, *PNDIS_802_11_AUTHENTICATION_EVENT;

/*        
typedef struct _NDIS_802_11_TEST
{
    ULONG Length;
    ULONG Type;
    union
    {
        NDIS_802_11_AUTHENTICATION_EVENT AuthenticationEvent;
        NDIS_802_11_RSSI RssiTrigger;
    };
} NDIS_802_11_TEST, *PNDIS_802_11_TEST;
 */

/* 802.11 Media stream constraints, associated with OID_802_11_MEDIA_STREAM_MODE */
typedef enum _NDIS_802_11_MEDIA_STREAM_MODE {
	Ndis802_11MediaStreamOff,
	Ndis802_11MediaStreamOn,
} NDIS_802_11_MEDIA_STREAM_MODE, *PNDIS_802_11_MEDIA_STREAM_MODE;

/* PMKID Structures */
typedef UCHAR NDIS_802_11_PMKID_VALUE[16];

#if defined(CONFIG_STA_SUPPORT) || defined(WPA_SUPPLICANT_SUPPORT)
typedef struct _BSSID_INFO {
	NDIS_802_11_MAC_ADDRESS BSSID;
	NDIS_802_11_PMKID_VALUE PMKID;
} BSSID_INFO, *PBSSID_INFO;

typedef struct _NDIS_802_11_PMKID {
	UINT Length;
	UINT BSSIDInfoCount;
	BSSID_INFO BSSIDInfo[1];
} NDIS_802_11_PMKID, *PNDIS_802_11_PMKID;
#endif /* defined(CONFIG_STA_SUPPORT) || defined(WPA_SUPPLICANT_SUPPORT) */

#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT
#ifdef WPA_SUPPLICANT_SUPPORT
typedef struct _NDIS_APCLI_802_11_PMKID
{
    UINT    Length;
    UINT    BSSIDInfoCount;
	BSSID_INFO BSSIDInfo[];
} NDIS_APCLI_802_11_PMKID, *PNDIS_APCLI_802_11_PMKID;
#endif/*WPA_SUPPLICANT_SUPPORT*/
#endif /* APCLI_SUPPORT */

typedef struct _AP_BSSID_INFO {
	NDIS_802_11_MAC_ADDRESS MAC;
	NDIS_802_11_PMKID_VALUE PMKID;
	UCHAR PMK[32];
	ULONG RefreshTime;
	BOOLEAN Valid;
} AP_BSSID_INFO, *PAP_BSSID_INFO;

#define MAX_PMKID_COUNT		8
typedef struct _NDIS_AP_802_11_PMKID {
	AP_BSSID_INFO BSSIDInfo[MAX_PMKID_COUNT];
} NDIS_AP_802_11_PMKID, *PNDIS_AP_802_11_PMKID;
#endif /* CONFIG_AP_SUPPORT */

typedef struct _NDIS_802_11_AUTHENTICATION_ENCRYPTION {
	NDIS_802_11_AUTHENTICATION_MODE AuthModeSupported;
	NDIS_802_11_ENCRYPTION_STATUS EncryptStatusSupported;
} NDIS_802_11_AUTHENTICATION_ENCRYPTION, *PNDIS_802_11_AUTHENTICATION_ENCRYPTION;

typedef struct _NDIS_802_11_CAPABILITY {
	ULONG Length;
	ULONG Version;
	ULONG NoOfPMKIDs;
	ULONG NoOfAuthEncryptPairsSupported;
	NDIS_802_11_AUTHENTICATION_ENCRYPTION
	    AuthenticationEncryptionSupported[1];
} NDIS_802_11_CAPABILITY, *PNDIS_802_11_CAPABILITY;

#if 0				/* os abl move to include/os/rt_os.h */
#ifdef LINUX
#if WIRELESS_EXT <= 11
#ifndef SIOCDEVPRIVATE
#define SIOCDEVPRIVATE                              0x8BE0
#endif
#define SIOCIWFIRSTPRIV								SIOCDEVPRIVATE
#endif
#endif /* LINUX */

#ifdef CONFIG_STA_SUPPORT
#define RT_PRIV_IOCTL								(SIOCIWFIRSTPRIV + 0x01)
#define RTPRIV_IOCTL_SET							(SIOCIWFIRSTPRIV + 0x02)

#ifdef DBG
#define RTPRIV_IOCTL_BBP                            (SIOCIWFIRSTPRIV + 0x03)
#define RTPRIV_IOCTL_MAC                            (SIOCIWFIRSTPRIV + 0x05)

#ifdef RTMP_RF_RW_SUPPORT
#define RTPRIV_IOCTL_RF                             (SIOCIWFIRSTPRIV + 0x13)	/* edit by johnli, fix read rf register problem */
#endif /* RTMP_RF_RW_SUPPORT */
#define RTPRIV_IOCTL_E2P                            (SIOCIWFIRSTPRIV + 0x07)
#endif /* DBG */

#define RTPRIV_IOCTL_ATE							(SIOCIWFIRSTPRIV + 0x08)

#define RTPRIV_IOCTL_STATISTICS                     (SIOCIWFIRSTPRIV + 0x09)
#define RTPRIV_IOCTL_ADD_PMKID_CACHE                (SIOCIWFIRSTPRIV + 0x0A)
#define RTPRIV_IOCTL_RADIUS_DATA                    (SIOCIWFIRSTPRIV + 0x0C)
#define RTPRIV_IOCTL_GSITESURVEY					(SIOCIWFIRSTPRIV + 0x0D)
#define RT_PRIV_IOCTL_EXT							(SIOCIWFIRSTPRIV + 0x0E)	/* Sync. with RT61 (for wpa_supplicant) */
#define RTPRIV_IOCTL_GET_MAC_TABLE					(SIOCIWFIRSTPRIV + 0x0F)
#define RTPRIV_IOCTL_GET_MAC_TABLE_STRUCT					(SIOCIWFIRSTPRIV + 0x1F)	/* modified by Red@Ralink, 2009/09/30 */

#define RTPRIV_IOCTL_SHOW							(SIOCIWFIRSTPRIV + 0x11)

enum {
#ifdef MAT_SUPPORT
	SHOW_IPV4_MAT_INFO = 1,
	SHOW_IPV6_MAT_INFO = 2,
	SHOW_ETH_CLONE_MAC = 3,
#endif /* MAT_SUPPORT */
	SHOW_CONN_STATUS = 4,
	SHOW_DRVIER_VERION = 5,
	SHOW_BA_INFO = 6,
	SHOW_DESC_INFO = 7,
#ifdef RTMP_MAC_USB
	SHOW_RXBULK_INFO = 8,
	SHOW_TXBULK_INFO = 9,
#endif /* RTMP_MAC_USB */
	RAIO_OFF = 10,
	RAIO_ON = 11,
#ifdef MESH_SUPPORT
	SHOW_MESH_INFO = 12,
	SHOW_NEIGHINFO_INFO = 13,
	SHOW_MESH_ROUTE_INFO = 14,
	SHOW_MESH_ENTRY_INFO = 15,
	SHOW_MULPATH_INFO = 16,
	SHOW_MCAST_AGEOUT_INFO = 17,
	SHOW_MESH_PKTSIG_INFO = 18,
	SHOW_MESH_PROXY_INFO = 19,
#endif /* MESH_SUPPORT */
#ifdef QOS_DLS_SUPPORT
	SHOW_DLS_ENTRY_INFO = 20,
#endif /* QOS_DLS_SUPPORT */
	SHOW_CFG_VALUE = 21,
	SHOW_ADHOC_ENTRY_INFO = 22,
#ifdef WMM_ACM_SUPPORT
	SHOW_ACM_BADNWIDTH = 23,
	SHOW_ACM_STREAM = 24,
#endif /* WMM_ACM_SUPPORT */
#ifdef DOT11Z_TDLS_SUPPORT
	SHOW_TDLS_ENTRY_INFO = 25,
#endif /* DOT11Z_TDLS_SUPPORT */
};

#ifdef WSC_STA_SUPPORT
#define RTPRIV_IOCTL_SET_WSC_PROFILE_U32_ITEM       (SIOCIWFIRSTPRIV + 0x14)
#define RTPRIV_IOCTL_SET_WSC_PROFILE_STRING_ITEM    (SIOCIWFIRSTPRIV + 0x16)

enum {
	WSC_CREDENTIAL_COUNT = 1,
	WSC_CREDENTIAL_SSID = 2,
	WSC_CREDENTIAL_AUTH_MODE = 3,
	WSC_CREDENTIAL_ENCR_TYPE = 4,
	WSC_CREDENTIAL_KEY_INDEX = 5,
	WSC_CREDENTIAL_KEY = 6,
	WSC_CREDENTIAL_MAC = 7,
	WSC_SET_DRIVER_CONNECT_BY_CREDENTIAL_IDX = 8,
	WSC_SET_DRIVER_AUTO_CONNECT = 9,
	WSC_SET_CONF_MODE = 10,	/* Enrollee or Registrar */
	WSC_SET_MODE = 11,	/* PIN or PBC */
	WSC_SET_PIN = 12,
	WSC_SET_SSID = 13,
	WSC_START = 14,
	WSC_STOP = 15,
	WSC_GEN_PIN_CODE = 16,
	WSC_AP_BAND = 17,
	WSC_SET_BSSID = 18,
};
#endif /* WSC_STA_SUPPORT */
#endif /* CONFIG_STA_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
/* Ralink defined OIDs */
#define RT_PRIV_IOCTL								(SIOCIWFIRSTPRIV + 0x01)
#define RTPRIV_IOCTL_SET							(SIOCIWFIRSTPRIV + 0x02)

#ifdef DBG
#define RTPRIV_IOCTL_BBP                            (SIOCIWFIRSTPRIV + 0x03)
#define RTPRIV_IOCTL_MAC                            (SIOCIWFIRSTPRIV + 0x05)

#ifdef RTMP_RF_RW_SUPPORT
#define RTPRIV_IOCTL_RF                             (SIOCIWFIRSTPRIV + 0x13)
#endif /* RTMP_RF_RW_SUPPORT */

/*
	When use private ioctl oid get/set the configuration, we can use following flags to provide specific rules when handle the cmd
 */
#define RTPRIV_IOCTL_FLAG_UI			0x0001	/* Notidy this private cmd send by UI. */
#define RTPRIV_IOCTL_FLAG_NODUMPMSG	0x0002	/* Notify driver cannot dump msg to stdio/stdout when run this private ioctl cmd */
#define RTPRIV_IOCTL_FLAG_NOSPACE		0x0004	/* Notify driver didn't need copy msg to caller due to the caller didn't reserve space for this cmd */

#endif /* DBG */
#define RTPRIV_IOCTL_E2P                            (SIOCIWFIRSTPRIV + 0x07)

#define RTPRIV_IOCTL_ATE							(SIOCIWFIRSTPRIV + 0x08)

#define RTPRIV_IOCTL_STATISTICS                     (SIOCIWFIRSTPRIV + 0x09)
#define RTPRIV_IOCTL_ADD_PMKID_CACHE                (SIOCIWFIRSTPRIV + 0x0A)
#define RTPRIV_IOCTL_RADIUS_DATA                    (SIOCIWFIRSTPRIV + 0x0C)
#define RTPRIV_IOCTL_GSITESURVEY					(SIOCIWFIRSTPRIV + 0x0D)
#define RTPRIV_IOCTL_ADD_WPA_KEY                    (SIOCIWFIRSTPRIV + 0x0E)
#define RTPRIV_IOCTL_GET_MAC_TABLE					(SIOCIWFIRSTPRIV + 0x0F)
#define RTPRIV_IOCTL_GET_MAC_TABLE_STRUCT	(SIOCIWFIRSTPRIV + 0x1F)	/* modified by Red@Ralink, 2009/09/30 */
#define RTPRIV_IOCTL_STATIC_WEP_COPY                (SIOCIWFIRSTPRIV + 0x10)

#define RTPRIV_IOCTL_SHOW							(SIOCIWFIRSTPRIV + 0x11)
#define RTPRIV_IOCTL_WSC_PROFILE                    (SIOCIWFIRSTPRIV + 0x12)
#define RTPRIV_IOCTL_QUERY_BATABLE                  (SIOCIWFIRSTPRIV + 0x16)
#ifdef INF_AR9
#define RTPRIV_IOCTL_GET_AR9_SHOW   (SIOCIWFIRSTPRIV + 0x17)
#endif /* INF_AR9 */
#define RTPRIV_IOCTL_SET_WSCOOB	(SIOCIWFIRSTPRIV + 0x19)
#endif /* CONFIG_AP_SUPPORT */
#endif /* 0 */

#if 0				/* os abl move to rtmp_cmd.h (used in sta_ioctl.c) */
enum {
#ifdef MAT_SUPPORT
	SHOW_IPV4_MAT_INFO = 1,
	SHOW_IPV6_MAT_INFO = 2,
	SHOW_ETH_CLONE_MAC = 3,
#endif /* MAT_SUPPORT */
	SHOW_CONN_STATUS = 4,
	SHOW_DRVIER_VERION = 5,
	SHOW_BA_INFO = 6,
	SHOW_DESC_INFO = 7,
#ifdef RTMP_MAC_USB
	SHOW_RXBULK_INFO = 8,
	SHOW_TXBULK_INFO = 9,
#endif /* RTMP_MAC_USB */
	RAIO_OFF = 10,
	RAIO_ON = 11,
#ifdef MESH_SUPPORT
	SHOW_MESH_INFO = 12,
	SHOW_NEIGHINFO_INFO = 13,
	SHOW_MESH_ROUTE_INFO = 14,
	SHOW_MESH_ENTRY_INFO = 15,
	SHOW_MULPATH_INFO = 16,
	SHOW_MCAST_AGEOUT_INFO = 17,
	SHOW_MESH_PKTSIG_INFO = 18,
	SHOW_MESH_PROXY_INFO = 19,
#endif /* MESH_SUPPORT */
#ifdef QOS_DLS_SUPPORT
	SHOW_DLS_ENTRY_INFO = 20,
#endif /* QOS_DLS_SUPPORT */
	SHOW_CFG_VALUE = 21,
	SHOW_ADHOC_ENTRY_INFO = 22,
#ifdef WMM_ACM_SUPPORT
	SHOW_ACM_BADNWIDTH = 23,
	SHOW_ACM_STREAM = 24,
#endif /* WMM_ACM_SUPPORT */
#ifdef DOT11Z_TDLS_SUPPORT
	SHOW_TDLS_ENTRY_INFO = 25,
#endif /* DOT11Z_TDLS_SUPPORT */
};
#endif /* 0 */

#ifdef DBG
/*
	When use private ioctl oid get/set the configuration, we can use following flags to provide specific rules when handle the cmd
 */
#define RTPRIV_IOCTL_FLAG_UI			0x0001	/* Notidy this private cmd send by UI. */
#define RTPRIV_IOCTL_FLAG_NODUMPMSG	0x0002	/* Notify driver cannot dump msg to stdio/stdout when run this private ioctl cmd */
#define RTPRIV_IOCTL_FLAG_NOSPACE		0x0004	/* Notify driver didn't need copy msg to caller due to the caller didn't reserve space for this cmd */
#endif /* DBG */

#ifdef MESH_SUPPORT
/* mesh extension OID */
#define OID_802_11_MESH_SECURITY_INFO	0x0651
#define OID_802_11_MESH_ID				0x0652
#define OID_802_11_MESH_AUTO_LINK		0x0653
#define OID_802_11_MESH_LINK_STATUS		0x0654
#define OID_802_11_MESH_LIST			0x0655
#define OID_802_11_MESH_ROUTE_LIST		0x0656
#define OID_802_11_MESH_ADD_LINK		0x0657
#define OID_802_11_MESH_DEL_LINK		0x0658
#define OID_802_11_MESH_MAX_TX_RATE		0x0659
#define OID_802_11_MESH_CHANNEL			0x065A
#define OID_802_11_MESH_HOSTNAME		0x065B
#define OID_802_11_MESH_ONLY_MODE		0x065C
#define OID_802_11_MESH_DEVICENAME		0x065d
#define OID_802_11_MESH_CHANNEL_BW		0x065e
#define OID_802_11_MESH_CHANNEL_OFFSET	0x065f
#define OID_802_11_MESH_FORWARD			0x0660

#define RT_OID_802_11_MESH_SECURITY_INFO	(OID_GET_SET_TOGGLE + OID_802_11_MESH_SECURITY_INFO)
#define RT_OID_802_11_MESH_ID				(OID_GET_SET_TOGGLE + OID_802_11_MESH_ID)
#define RT_OID_802_11_MESH_AUTO_LINK		(OID_GET_SET_TOGGLE + OID_802_11_MESH_AUTO_LINK)
#define RT_OID_802_11_MESH_ADD_LINK		(OID_GET_SET_TOGGLE + OID_802_11_MESH_ADD_LINK)
#define RT_OID_802_11_MESH_DEL_LINK		(OID_GET_SET_TOGGLE + OID_802_11_MESH_DEL_LINK)
#define RT_OID_802_11_MESH_MAX_TX_RATE	(OID_GET_SET_TOGGLE + OID_802_11_MESH_MAX_TX_RATE)
#define RT_OID_802_11_MESH_CHANNEL		(OID_GET_SET_TOGGLE + OID_802_11_MESH_CHANNEL)
#define RT_OID_802_11_MESH_HOSTNAME		(OID_GET_SET_TOGGLE + OID_802_11_MESH_HOSTNAME)
#define RT_OID_802_11_MESH_ONLY_MODE	(OID_GET_SET_TOGGLE + OID_802_11_MESH_ONLY_MODE)
#define RT_OID_802_11_MESH_DEVICENAME	(OID_GET_SET_TOGGLE + OID_802_11_MESH_DEVICENAME)
#define RT_OID_802_11_MESH_FORWARD			(OID_GET_SET_TOGGLE + 0x0660)
#endif /* MESH_SUPPORT */

#ifdef SNMP_SUPPORT
/*SNMP ieee 802dot11 , 2008_0220 */
/* dot11res(3) */
#define RT_OID_802_11_MANUFACTUREROUI			0x0700
#define RT_OID_802_11_MANUFACTURERNAME			0x0701
#define RT_OID_802_11_RESOURCETYPEIDNAME		0x0702

/* dot11smt(1) */
#define RT_OID_802_11_PRIVACYOPTIONIMPLEMENTED	0x0703
#define RT_OID_802_11_POWERMANAGEMENTMODE		0x0704
#define OID_802_11_WEPDEFAULTKEYVALUE			0x0705	/* read , write */
#define OID_802_11_WEPDEFAULTKEYID				0x0706
#define RT_OID_802_11_WEPKEYMAPPINGLENGTH		0x0707
#define OID_802_11_SHORTRETRYLIMIT				0x0708
#define OID_802_11_LONGRETRYLIMIT				0x0709
#define RT_OID_802_11_PRODUCTID					0x0710
#define RT_OID_802_11_MANUFACTUREID				0x0711

/* //dot11Phy(4) */
#define OID_802_11_CURRENTCHANNEL				0x0712

#endif /* SNMP_SUPPORT */

/*dot11mac */
#define RT_OID_802_11_MAC_ADDRESS				0x0713
#define OID_802_11_BUILD_CHANNEL_EX				0x0714
#define OID_802_11_GET_CH_LIST					0x0715
#define OID_802_11_GET_COUNTRY_CODE				0x0716
#define OID_802_11_GET_CHANNEL_GEOGRAPHY		0x0717

/*#define RT_OID_802_11_STATISTICS              (OID_GET_SET_TOGGLE | OID_802_11_STATISTICS) */

#ifdef WIDI_SUPPORT
#define RT_OID_INTEL_WIDI                     	0x0720
#define RT_OID_WSC_GEN_PIN_CODE                 0x0721
#endif /* WIDI_SUPPORT */

#ifdef DPA_T
#define RT_OID_WSC_SET_VENDOR_EXT               0x0722
#define RT_OID_WSC_SET_UUID						0x0723
#endif /* DPA_T */

#ifdef WSC_INCLUDED
#define RT_OID_WAC_REQ								0x0736
#define	RT_OID_WSC_AUTO_PROVISION_WITH_BSSID		0x0737
#define	RT_OID_WSC_AUTO_PROVISION					0x0738
#ifdef WSC_LED_SUPPORT
/*WPS LED MODE 10 for Dlink WPS LED */
#define RT_OID_LED_WPS_MODE10						0x0739
#endif /* WSC_LED_SUPPORT */
#endif /* WSC_INCLUDED */
#ifdef CONFIG_STA_SUPPORT
#define RT_OID_WSC_SET_PASSPHRASE                   0x0740	/* passphrase for wpa(2)-psk */
#define RT_OID_WSC_DRIVER_AUTO_CONNECT              0x0741
#define RT_OID_WSC_QUERY_DEFAULT_PROFILE            0x0742
#define RT_OID_WSC_SET_CONN_BY_PROFILE_INDEX        0x0743
#define RT_OID_WSC_SET_ACTION                       0x0744
#define RT_OID_WSC_SET_SSID                         0x0745
#define RT_OID_WSC_SET_PIN_CODE                     0x0746
#define RT_OID_WSC_SET_MODE                         0x0747	/* PIN or PBC */
#define RT_OID_WSC_SET_CONF_MODE                    0x0748	/* Enrollee or Registrar */
#define RT_OID_WSC_SET_PROFILE                      0x0749
#endif /* CONFIG_STA_SUPPORT */
#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT
#define RT_OID_APCLI_WSC_PIN_CODE					0x074A
#endif /* APCLI_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */
#define	RT_OID_WSC_FRAGMENT_SIZE					0x074D
#define	RT_OID_WSC_V2_SUPPORT						0x074E
#define	RT_OID_WSC_CONFIG_STATUS					0x074F
#define RT_OID_802_11_WSC_QUERY_PROFILE				0x0750
/* for consistency with RT61 */
#define RT_OID_WSC_QUERY_STATUS						0x0751
#define RT_OID_WSC_PIN_CODE							0x0752
#define RT_OID_WSC_UUID								0x0753
#define RT_OID_WSC_SET_SELECTED_REGISTRAR			0x0754
#define RT_OID_WSC_EAPMSG							0x0755
#define RT_OID_WSC_MANUFACTURER						0x0756
#define RT_OID_WSC_MODEL_NAME						0x0757
#define RT_OID_WSC_MODEL_NO							0x0758
#define RT_OID_WSC_SERIAL_NO						0x0759
#define RT_OID_WSC_READ_UFD_FILE					0x075A
#define RT_OID_WSC_WRITE_UFD_FILE					0x075B
#define RT_OID_WSC_QUERY_PEER_INFO_ON_RUNNING		0x075C
#define RT_OID_WSC_MAC_ADDRESS						0x0760

#ifdef LLTD_SUPPORT
/* for consistency with RT61 */
#define RT_OID_GET_PHY_MODE                         0x761
#ifdef CONFIG_AP_SUPPORT
#define RT_OID_GET_LLTD_ASSO_TABLE                  0x762
#ifdef APCLI_SUPPORT
#define RT_OID_GET_REPEATER_AP_LINEAGE				0x763
#endif /* APCLI_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */
#endif /* LLTD_SUPPORT */

#ifdef NINTENDO_AP
/*#define RT_OID_NINTENDO                             0x0D010770 */
#define RT_OID_802_11_NINTENDO_GET_TABLE			0x0771	/*((RT_OID_NINTENDO + 0x01) & 0xffff) */
#define RT_OID_802_11_NINTENDO_SET_TABLE			0x0772	/*((RT_OID_NINTENDO + 0x02) & 0xffff) */
#define RT_OID_802_11_NINTENDO_CAPABLE				0x0773	/*((RT_OID_NINTENDO + 0x03) & 0xffff) */
#endif /* NINTENDO_AP */

#ifdef DOT11R_FT_SUPPORT
#define OID_802_11R_SUPPORT							0x0780
#define OID_802_11R_MDID							0x0781
#define OID_802_11R_R0KHID							0x0782
#define OID_802_11R_RIC								0x0783
#define OID_802_11R_OTD								0x0784
#define OID_802_11R_INFO							0x0785

#define	RT_OID_802_11R_SUPPORT					  	(OID_GET_SET_TOGGLE | OID_802_11R_SUPPORT)
#define RT_OID_802_11R_MDID							(OID_GET_SET_TOGGLE | OID_802_11R_MDID)
#define RT_OID_802_11R_R0KHID						(OID_GET_SET_TOGGLE | OID_802_11R_R0KHID)
#define	RT_OID_802_11R_RIC					  		(OID_GET_SET_TOGGLE | OID_802_11R_RIC)
#define RT_OID_802_11R_OTD							(OID_GET_SET_TOGGLE | OID_802_11R_OTD)
#define RT_OID_802_11R_INFO							(OID_GET_SET_TOGGLE | OID_802_11R_INFO)
#endif /* DOT11R_FT_SUPPORT */

#ifdef EASY_CONFIG_SETUP
#define OID_PIN_OF_ENROLLEE							0x0800
#endif /* EASY_CONFIG_SETUP */

#ifdef WSC_NFC_SUPPORT
#define RT_OID_NFC_STATUS							0x0930
#endif /* WSC_NFC_SUPPORT */

/* New for MeetingHouse Api support */
#define OID_MH_802_1X_SUPPORTED               0xFFEDC100

/* MIMO Tx parameter, ShortGI, MCS, STBC, etc.  these are fields in TXWI. Don't change this definition!!! */
typedef union _HTTRANSMIT_SETTING {
#ifdef RT_BIG_ENDIAN
	struct {
		USHORT MODE:3;	/* Use definition MODE_xxx. */
		USHORT iTxBF:1;
		USHORT eTxBF:1;
		USHORT STBC:1;	/* only support in HT/VHT mode with MCS0~7 */
		USHORT ShortGI:1;
		USHORT BW:2;	/* channel bandwidth 20MHz/40/80 MHz */
		USHORT ldpc:1;
		USHORT MCS:6;	/* MCS */
	} field;
#else
	struct {
		USHORT MCS:6;
		USHORT ldpc:1;
		USHORT BW:2;
		USHORT ShortGI:1;
		USHORT STBC:1;
		USHORT eTxBF:1;
		USHORT iTxBF:1;
		USHORT MODE:3;
	} field;
#endif
	USHORT word;
} HTTRANSMIT_SETTING, *PHTTRANSMIT_SETTING;

typedef enum _RT_802_11_PREAMBLE {
	Rt802_11PreambleLong,
	Rt802_11PreambleShort,
	Rt802_11PreambleAuto
} RT_802_11_PREAMBLE, *PRT_802_11_PREAMBLE;

typedef enum _RT_802_11_PHY_MODE {
	PHY_11BG_MIXED = 0,
	PHY_11B = 1,
	PHY_11A = 2,
	PHY_11ABG_MIXED = 3,
	PHY_11G = 4,
#ifdef DOT11_N_SUPPORT
	PHY_11ABGN_MIXED = 5,	/* both band   5 */
	PHY_11N_2_4G = 6,		/* 11n-only with 2.4G band      6 */
	PHY_11GN_MIXED = 7,		/* 2.4G band      7 */
	PHY_11AN_MIXED = 8,		/* 5G  band       8 */
	PHY_11BGN_MIXED = 9,	/* if check 802.11b.      9 */
	PHY_11AGN_MIXED = 10,	/* if check 802.11b.      10 */
	PHY_11N_5G = 11,		/* 11n-only with 5G band                11 */
#endif /* DOT11_N_SUPPORT */
#ifdef DOT11_VHT_AC
	PHY_11VHT_N_ABG_MIXED = 12, /* 12 -> AC/A/AN/B/G/GN mixed */
	PHY_11VHT_N_AG_MIXED = 13, /* 13 -> AC/A/AN/G/GN mixed  */
	PHY_11VHT_N_A_MIXED = 14, /* 14 -> AC/AN/A mixed in 5G band */
	PHY_11VHT_N_MIXED = 15, /* 15 -> AC/AN mixed in 5G band */
#endif /* DOT11_VHT_AC */
	PHY_MODE_MAX,
} RT_802_11_PHY_MODE;

#ifdef DOT11_VHT_AC
#define PHY_MODE_IS_5G_BAND(__Mode)	\
	((__Mode == PHY_11A) ||			\
	(__Mode == PHY_11ABG_MIXED) ||	\
	(__Mode == PHY_11ABGN_MIXED) ||	\
	(__Mode == PHY_11AN_MIXED) ||	\
	(__Mode == PHY_11AGN_MIXED) ||	\
	(__Mode == PHY_11N_5G) ||\
	(__Mode == PHY_11VHT_N_MIXED) ||\
	(__Mode == PHY_11VHT_N_A_MIXED))
#elif defined(DOT11_N_SUPPORT)
#define PHY_MODE_IS_5G_BAND(__Mode)	\
	((__Mode == PHY_11A) ||			\
	(__Mode == PHY_11ABG_MIXED) ||	\
	(__Mode == PHY_11ABGN_MIXED) ||	\
	(__Mode == PHY_11AN_MIXED) ||	\
	(__Mode == PHY_11AGN_MIXED) ||	\
	(__Mode == PHY_11N_5G))
#else

#define PHY_MODE_IS_5G_BAND(__Mode)	\
	((__Mode == PHY_11A) ||			\
	(__Mode == PHY_11ABG_MIXED))
#endif /* DOT11_N_SUPPORT */

/* put all proprietery for-query objects here to reduce # of Query_OID */
typedef struct _RT_802_11_LINK_STATUS {
	ULONG CurrTxRate;	/* in units of 0.5Mbps */
	ULONG ChannelQuality;	/* 0..100 % */
	ULONG TxByteCount;	/* both ok and fail */
	ULONG RxByteCount;	/* both ok and fail */
	ULONG CentralChannel;	/* 40MHz central channel number */
} RT_802_11_LINK_STATUS, *PRT_802_11_LINK_STATUS;

#ifdef SYSTEM_LOG_SUPPORT
typedef struct _RT_802_11_EVENT_LOG {
	LARGE_INTEGER SystemTime;	/* timestammp via NdisGetCurrentSystemTime() */
	UCHAR Addr[MAC_ADDR_LEN];
	USHORT Event;		/* EVENT_xxx */
} RT_802_11_EVENT_LOG, *PRT_802_11_EVENT_LOG;

typedef struct _RT_802_11_EVENT_TABLE {
	ULONG Num;
	ULONG Rsv;		/* to align Log[] at LARGE_INEGER boundary */
	RT_802_11_EVENT_LOG Log[MAX_NUMBER_OF_EVENT];
} RT_802_11_EVENT_TABLE, *PRT_802_11_EVENT_TABLE;
#endif /* SYSTEM_LOG_SUPPORT */

/* MIMO Tx parameter, ShortGI, MCS, STBC, etc.  these are fields in TXWI. Don't change this definition!!! */
typedef union _MACHTTRANSMIT_SETTING {
	struct {
		USHORT MCS:7;	/* MCS */
		USHORT BW:1;	/*channel bandwidth 20MHz or 40 MHz */
		USHORT ShortGI:1;
		USHORT STBC:2;	/*SPACE */
		USHORT rsv:3;
		USHORT MODE:2;	/* Use definition MODE_xxx. */
	} field;
	USHORT word;
} MACHTTRANSMIT_SETTING, *PMACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
	UCHAR ApIdx;
	UCHAR Addr[MAC_ADDR_LEN];
	UCHAR Aid;
	UCHAR Psm;		/* 0:PWR_ACTIVE, 1:PWR_SAVE */
	UCHAR MimoPs;		/* 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled */
	CHAR AvgRssi0;
	CHAR AvgRssi1;
	CHAR AvgRssi2;
	UINT32 ConnectedTime;
	MACHTTRANSMIT_SETTING TxRate;
#ifdef DPA_S
	UCHAR	DeviceName[OID_P2P_DEVICE_NAME_LEN];
	UINT32	DeviceNameLen;
#endif /* DPA_S */
	UINT32 LastRxRate;
#ifdef RTMP_RBUS_SUPPORT
	SHORT StreamSnr[3];				/* BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed */
	SHORT SoundingRespSnr[3];			/* SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed */
/*	SHORT TxPER;	*/					/* TX PER over the last second. Percent */
/*	SHORT reserved;*/
#endif /* RTMP_RBUS_SUPPORT */
} RT_802_11_MAC_ENTRY, *PRT_802_11_MAC_ENTRY;

typedef struct _RT_802_11_MAC_TABLE {
	ULONG Num;
	RT_802_11_MAC_ENTRY Entry[MAX_NUMBER_OF_MAC];
} RT_802_11_MAC_TABLE, *PRT_802_11_MAC_TABLE;

#ifdef DOT11_N_SUPPORT
#ifdef TXBF_SUPPORT
typedef
    struct {
	ULONG TxSuccessCount;
	ULONG TxRetryCount;
	ULONG TxFailCount;
	ULONG ETxSuccessCount;
	ULONG ETxRetryCount;
	ULONG ETxFailCount;
	ULONG ITxSuccessCount;
	ULONG ITxRetryCount;
	ULONG ITxFailCount;
} RT_COUNTER_TXBF;

typedef
    struct {
	ULONG Num;
	RT_COUNTER_TXBF Entry[MAX_NUMBER_OF_MAC];
} RT_802_11_TXBF_TABLE;
#endif /* TXBF_SUPPORT */
#endif /* DOT11_N_SUPPORT */

/* structure for query/set hardware register - MAC, BBP, RF register */
typedef struct _RT_802_11_HARDWARE_REGISTER {
	ULONG HardwareType;	/* 0:MAC, 1:BBP, 2:RF register, 3:EEPROM */
	ULONG Offset;		/* Q/S register offset addr */
	ULONG Data;		/* R/W data buffer */
} RT_802_11_HARDWARE_REGISTER, *PRT_802_11_HARDWARE_REGISTER;

typedef struct _RT_802_11_AP_CONFIG {
	ULONG EnableTxBurst;	/* 0-disable, 1-enable */
	ULONG EnableTurboRate;	/* 0-disable, 1-enable 72/100mbps turbo rate */
	ULONG IsolateInterStaTraffic;	/* 0-disable, 1-enable isolation */
	ULONG HideSsid;		/* 0-disable, 1-enable hiding */
	ULONG UseBGProtection;	/* 0-AUTO, 1-always ON, 2-always OFF */
	ULONG UseShortSlotTime;	/* 0-no use, 1-use 9-us short slot time */
	ULONG Rsv1;		/* must be 0 */
	ULONG SystemErrorBitmap;	/* ignore upon SET, return system error upon QUERY */
} RT_802_11_AP_CONFIG, *PRT_802_11_AP_CONFIG;

/* structure to query/set STA_CONFIG */
typedef struct _RT_802_11_STA_CONFIG {
	ULONG EnableTxBurst;	/* 0-disable, 1-enable */
	ULONG EnableTurboRate;	/* 0-disable, 1-enable 72/100mbps turbo rate */
	ULONG UseBGProtection;	/* 0-AUTO, 1-always ON, 2-always OFF */
	ULONG UseShortSlotTime;	/* 0-no use, 1-use 9-us short slot time when applicable */
	ULONG AdhocMode;	/* 0-11b rates only (WIFI spec), 1 - b/g mixed, 2 - g only */
	ULONG HwRadioStatus;	/* 0-OFF, 1-ON, default is 1, Read-Only */
	ULONG Rsv1;		/* must be 0 */
	ULONG SystemErrorBitmap;	/* ignore upon SET, return system error upon QUERY */
} RT_802_11_STA_CONFIG, *PRT_802_11_STA_CONFIG;

/* */
/*  For OID Query or Set about BA structure */
/* */
typedef struct _OID_BACAP_STRUC {
	UCHAR RxBAWinLimit;
	UCHAR TxBAWinLimit;
	UCHAR Policy;		/* 0: DELAY_BA 1:IMMED_BA  (//BA Policy subfiled value in ADDBA frame)   2:BA-not use. other value invalid */
	UCHAR MpduDensity;	/* 0: DELAY_BA 1:IMMED_BA  (//BA Policy subfiled value in ADDBA frame)   2:BA-not use. other value invalid */
	UCHAR AmsduEnable;	/*Enable AMSDU transmisstion */
	UCHAR AmsduSize;	/* 0:3839, 1:7935 bytes. UINT  MSDUSizeToBytes[]        = { 3839, 7935}; */
	UCHAR MMPSmode;		/* MIMO power save more, 0:static, 1:dynamic, 2:rsv, 3:mimo enable */
	BOOLEAN AutoBA;		/* Auto BA will automatically */
} OID_BACAP_STRUC, *POID_BACAP_STRUC;

typedef struct _RT_802_11_ACL_ENTRY {
	UCHAR Addr[MAC_ADDR_LEN];
	USHORT Rsv;
} RT_802_11_ACL_ENTRY, *PRT_802_11_ACL_ENTRY;

typedef struct GNU_PACKED _RT_802_11_ACL {
	ULONG Policy;		/* 0-disable, 1-positive list, 2-negative list */
	ULONG Num;
	RT_802_11_ACL_ENTRY Entry[MAX_NUMBER_OF_ACL];
} RT_802_11_ACL, *PRT_802_11_ACL;

typedef struct _RT_802_11_WDS {
	ULONG Num;
	NDIS_802_11_MAC_ADDRESS Entry[24 /*MAX_NUM_OF_WDS_LINK */ ];
	ULONG KeyLength;
	UCHAR KeyMaterial[32];
} RT_802_11_WDS, *PRT_802_11_WDS;

typedef struct _RT_802_11_TX_RATES_ {
	UCHAR SupRateLen;
	UCHAR SupRate[MAX_LENGTH_OF_SUPPORT_RATES];
	UCHAR ExtRateLen;
	UCHAR ExtRate[MAX_LENGTH_OF_SUPPORT_RATES];
} RT_802_11_TX_RATES, *PRT_802_11_TX_RATES;

/* Definition of extra information code */
#define	GENERAL_LINK_UP			0x0	/* Link is Up */
#define	GENERAL_LINK_DOWN		0x1	/* Link is Down */
#define	HW_RADIO_OFF			0x2	/* Hardware radio off */
#define	SW_RADIO_OFF			0x3	/* Software radio off */
#define	AUTH_FAIL				0x4	/* Open authentication fail */
#define	AUTH_FAIL_KEYS			0x5	/* Shared authentication fail */
#define	ASSOC_FAIL				0x6	/* Association failed */
#define	EAP_MIC_FAILURE			0x7	/* Deauthencation because MIC failure */
#define	EAP_4WAY_TIMEOUT		0x8	/* Deauthencation on 4-way handshake timeout */
#define	EAP_GROUP_KEY_TIMEOUT	0x9	/* Deauthencation on group key handshake timeout */
#define	EAP_SUCCESS				0xa	/* EAP succeed */
#define	DETECT_RADAR_SIGNAL		0xb	/* Radar signal occur in current channel */
#define EXTRA_INFO_MAX			0xb	/* Indicate Last OID */

#define EXTRA_INFO_CLEAR		0xffffffff

#define SME_CONNECTING 0
#define SME_IDLE 1

/* This is OID setting structure. So only GF or MM as Mode. This is valid when our wirelss mode has 802.11n in use. */
typedef struct {
	RT_802_11_PHY_MODE PhyMode;	/* */
	UCHAR TransmitNo;
	UCHAR HtMode;		/*HTMODE_GF or HTMODE_MM */
	UINT8 ExtOffset;	/*extension channel above or below */
	UCHAR MCS;
	UCHAR BW;
	UCHAR STBC;
	UCHAR SHORTGI;
	UCHAR rsv;
} OID_SET_HT_PHYMODE, *POID_SET_HT_PHYMODE;

#ifdef NINTENDO_AP
#define NINTENDO_MAX_ENTRY 16
#define NINTENDO_SSID_NAME_LN 8
#define NINTENDO_SSID_NAME "NWCUSBAP"
#define NINTENDO_PROBE_REQ_FLAG_MASK 0x03
#define NINTENDO_PROBE_REQ_ON 0x01
#define NINTENDO_PROBE_REQ_SIGNAL 0x02
#define NINTENDO_PROBE_RSP_ON 0x01
#define NINTENDO_SSID_NICKNAME_LN 20

#define NINTENDO_WEPKEY_LN 13

typedef struct _NINTENDO_SSID {
	UCHAR NINTENDOFixChar[NINTENDO_SSID_NAME_LN];
	UCHAR zero1;
	UCHAR registe;
	UCHAR ID;
	UCHAR zero2;
	UCHAR NICKname[NINTENDO_SSID_NICKNAME_LN];
} RT_NINTENDO_SSID, *PRT_NINTENDO_SSID;

typedef struct _NINTENDO_ENTRY {
	UCHAR NICKname[NINTENDO_SSID_NICKNAME_LN];
	UCHAR DS_Addr[MAC_ADDR_LEN];
	UCHAR registe;
	UCHAR UserSpaceAck;
} RT_NINTENDO_ENTRY, *PRT_NINTENDO_ENTRY;

/*RTPRIV_IOCTL_NINTENDO_GET_TABLE */
/*RTPRIV_IOCTL_NINTENDO_SET_TABLE */
typedef struct _NINTENDO_TABLE {
	UINT number;
	RT_NINTENDO_ENTRY entry[NINTENDO_MAX_ENTRY];
} RT_NINTENDO_TABLE, *PRT_NINTENDO_TABLE;

/*RTPRIV_IOCTL_NINTENDO_SEED_WEPKEY */
typedef struct _NINTENDO_SEED_WEPKEY {
	UCHAR seed[NINTENDO_SSID_NICKNAME_LN];
	UCHAR wepkey[16];	/*use 13 for 104 bits wep key */
} RT_NINTENDO_SEED_WEPKEY, *PRT_NINTENDO_SEED_WEPKEY;
#endif /* NINTENDO_AP */

#ifdef LLTD_SUPPORT
typedef struct _RT_LLTD_ASSOICATION_ENTRY {
	UCHAR Addr[MAC_ADDR_LEN];
	unsigned short MOR;	/* maximum operational rate */
	UCHAR phyMode;
} RT_LLTD_ASSOICATION_ENTRY, *PRT_LLTD_ASSOICATION_ENTRY;

typedef struct _RT_LLTD_ASSOICATION_TABLE {
	unsigned int Num;
	RT_LLTD_ASSOICATION_ENTRY Entry[MAX_NUMBER_OF_MAC];
} RT_LLTD_ASSOICATION_TABLE, *PRT_LLTD_ASSOICATION_TABLE;
#endif /* LLTD_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
#ifdef QOS_DLS_SUPPORT
/*rt2860, 2007-0118 */
/* structure for DLS */
typedef struct _RT_802_11_DLS_UI {
	USHORT TimeOut;		/* unit: second , set by UI */
	USHORT CountDownTimer;	/* unit: second , used by driver only */
	NDIS_802_11_MAC_ADDRESS MacAddr;	/* set by UI */
	UCHAR Status;		/* 0: none , 1: wait STAkey, 2: finish DLS setup , set by driver only */
	BOOLEAN Valid;		/* 1: valid , 0: invalid , set by UI, use to setup or tear down DLS link */
} RT_802_11_DLS_UI, *PRT_802_11_DLS_UI;

typedef struct _RT_802_11_DLS_INFO {
	RT_802_11_DLS_UI Entry[MAX_NUMBER_OF_DLS_ENTRY];
	UCHAR num;
} RT_802_11_DLS_INFO, *PRT_802_11_DLS_INFO;

typedef enum _RT_802_11_DLS_MODE {
	DLS_NONE,
	DLS_WAIT_KEY,
	DLS_FINISH
} RT_802_11_DLS_MODE;
#endif /* QOS_DLS_SUPPORT */

#ifdef DOT11Z_TDLS_SUPPORT
typedef struct _RT_802_11_TDLS_UI {
	USHORT TimeOut;		/* unit: second , set by UI */
	USHORT CountDownTimer;	/* unit: second , used by driver only */
	NDIS_802_11_MAC_ADDRESS MacAddr;	/* set by UI */
	UCHAR Status;		/* 0: none , 1: wait STAkey, 2: finish DLS setup , set by driver only */
	BOOLEAN Valid;		/* 1: valid , 0: invalid , set by UI, use to setup or tear down DLS link */
} RT_802_11_TDLS_UI, *PRT_802_11_TDLS_UI;
#endif /* DOT11Z_TDLS_SUPPORT */

#if 0				/* os abl move to rtmp_cmd.h */
#ifdef WPA_SUPPLICANT_SUPPORT
#ifndef NATIVE_WPA_SUPPLICANT_SUPPORT
#define	RT_ASSOC_EVENT_FLAG                         0x0101
#define	RT_DISASSOC_EVENT_FLAG                      0x0102
#define	RT_REQIE_EVENT_FLAG                         0x0103
#define	RT_RESPIE_EVENT_FLAG                        0x0104
#define	RT_ASSOCINFO_EVENT_FLAG                     0x0105
#define RT_PMKIDCAND_FLAG                           0x0106
#define RT_INTERFACE_DOWN                           0x0107
#define RT_INTERFACE_UP                             0x0108
#endif /* NATIVE_WPA_SUPPLICANT_SUPPORT */
#endif /* WPA_SUPPLICANT_SUPPORT */
#endif /* 0 */
#endif /* CONFIG_STA_SUPPORT */

#ifdef WSC_INCLUDED
#define RT_WSC_UPNP_EVENT_FLAG		0x109
#endif /* WSC_INCLUDED */

#ifdef WMM_ACM_SUPPORT
typedef struct _RT_WMM_ACM_CMD {
	UINT CmdID;
	UINT16 DialogToken;
	UCHAR TspecCmd[128];
	UCHAR TclasCmd[128];
} RT_WMM_ACM_CMD, *PRT_WMM_ACM_CMD;
#endif /* WMM_ACM_SUPPORT */

#ifdef MESH_SUPPORT
typedef enum _MESH_KEY_SELECTION {
	MESH_KEY_NONE,
	MESH_KEY_PMKMA_PEER,
	MESH_KEY_PMKMA_LOCAL
} MESH_KEY_SELECTION, *PMESH_KEY_SELECTION;

typedef enum _MESH_SECURITY_TYPE {
	ENCRYPT_OPEN_NONE,
	ENCRYPT_OPEN_WEP,
	ENCRYPT_WPANONE_TKIP,
	ENCRYPT_WPANONE_AES
} MESH_SECURITY_TYPE, *PMESH_SECURITY_TYPE;

typedef struct GNU_PACKED _MESH_SECURITY_INFO {
	ULONG Length;		/* Length of this structure */
	UCHAR EncrypType;	/* indicate Encryption Type */
	UCHAR KeyIndex;		/* Default Key Index */
	UCHAR KeyLength;	/* length of key in bytes */
	UCHAR KeyMaterial[64];	/* the key Material */
} MESH_SECURITY_INFO, *PMESH_SECURITY_INFO;

typedef struct _MESH_LINK_ENTRY_INFO {
	UCHAR Status;
	UCHAR LinkType;
	CHAR Rssi;
	HTTRANSMIT_SETTING CurTxRate;
	UCHAR PeerMacAddr[MAC_ADDR_LEN];
} MESH_LINK_ENTRY_INFO, *PMESH_LINK_ENTRY_INFO;

typedef struct _MESH_LINK_INFO {
	MESH_LINK_ENTRY_INFO Entry[MAX_MESH_LINK_NUM];
} MESH_LINK_INFO, *PMESH_LINK_INFO;

typedef struct _MESH_NEIGHBOR_ENTRY_INFO {
	char Rssi;
	UCHAR HostName[MAX_HOST_NAME_LENGTH + 1];
	UCHAR MacAddr[MAC_ADDR_LEN];
	UCHAR MeshId[MAX_MESH_ID_LENGTH + 1];
	UCHAR Channel;
	UCHAR Status;		/*0:idle, 1:connected. */
	UCHAR MeshEncrypType;
} MESH_NEIGHBOR_ENTRY_INFO, *PMESH_NEIGHBOR_ENTRY_INFO;

typedef struct _MESH_NEIGHBOR_INFO {
	MESH_NEIGHBOR_ENTRY_INFO Entry[MAX_NEIGHBOR_NUM];
	UCHAR num;
} MESH_NEIGHBOR_INFO, *PMESH_NEIGHBOR_INFO;

typedef struct _RT_MESH_ROUTE_ENTRY {
	UCHAR MeshDA[MAC_ADDR_LEN];
	ULONG Dsn;
	UCHAR NextHop[MAC_ADDR_LEN];
	ULONG Metric;
} RT_MESH_ROUTE_ENTRY, *PRT_MESH_ROUTE_ENTRY;

typedef struct _RT_MESH_ROUTE_TABLE {
	UCHAR Num;
	RT_MESH_ROUTE_ENTRY Entry[MESH_MAX_FORWARD_ENTRY_NUM];
} RT_MESH_ROUTE_TABLE, *PRT_MESH_ROUTE_TABLE;
#endif /* MESH_SUPPORT */

/*#define MAX_CUSTOM_LEN 128 */

#ifdef CONFIG_STA_SUPPORT
typedef enum _RT_802_11_D_CLIENT_MODE {
	Rt802_11_D_None,
	Rt802_11_D_Flexible,
	Rt802_11_D_Strict,
} RT_802_11_D_CLIENT_MODE, *PRT_802_11_D_CLIENT_MODE;
#endif /* CONFIG_STA_SUPPORT */

typedef struct _RT_CHANNEL_LIST_INFO {
	UCHAR ChannelList[MAX_NUM_OF_CHS];	/* list all supported channels for site survey */
	UCHAR ChannelListNum;	/* number of channel in ChannelList[] */
} RT_CHANNEL_LIST_INFO, *PRT_CHANNEL_LIST_INFO;

#ifdef IWSC_SUPPORT
#define IWSC_MAX_SUB_MASK_LIST_COUNT	3
#endif /* IWSC_SUPPORT */

/* WSC configured credential */
typedef struct _WSC_CREDENTIAL {
	NDIS_802_11_SSID SSID;	/* mandatory */
	USHORT AuthType;	/* mandatory, 1: open, 2: wpa-psk, 4: shared, 8:wpa, 0x10: wpa2, 0x20: wpa2-psk */
	USHORT EncrType;	/* mandatory, 1: none, 2: wep, 4: tkip, 8: aes */
	UCHAR Key[64];		/* mandatory, Maximum 64 byte */
	USHORT KeyLength;
	UCHAR MacAddr[MAC_ADDR_LEN];	/* mandatory, AP MAC address */
	UCHAR KeyIndex;		/* optional, default is 1 */
	UCHAR bFromUPnP;	/* TRUE: This credential is from external UPnP registrar */
	UCHAR Rsvd[2];		/* Make alignment */
#ifdef IWSC_SUPPORT
	USHORT				IpConfigMethod;
	UINT32				RegIpv4Addr;
	UINT32				Ipv4SubMask;
	UINT32				EnrIpv4Addr;
	UINT32				AvaIpv4SubmaskList[IWSC_MAX_SUB_MASK_LIST_COUNT];
#endif /* IWSC_SUPPORT */
} WSC_CREDENTIAL, *PWSC_CREDENTIAL;

/* WSC configured profiles */
typedef struct _WSC_PROFILE {
	UINT ProfileCnt;
	UINT ApplyProfileIdx;	/* add by johnli, fix WPS test plan 5.1.1 */
	WSC_CREDENTIAL Profile[8];	/* Support up to 8 profiles */
} WSC_PROFILE, *PWSC_PROFILE;

#ifdef WAPI_SUPPORT
typedef enum _WAPI_PORT_SECURE_STATE {
	WAPI_PORT_NOT_SECURED,
	WAPI_PORT_SECURED,
} WAPI_PORT_SECURE_STATE, *PWAPI_PORT_SECURE_STATE;

typedef struct _WAPI_PORT_SECURE_STRUCT {
	UCHAR Addr[MAC_ADDR_LEN];
	USHORT state;
} WAPI_PORT_SECURE_STRUCT, *PWAPI_PORT_SECURE_STRUCT;

typedef struct _WAPI_UCAST_KEY_STRUCT {
	UCHAR Addr[MAC_ADDR_LEN];
	USHORT key_id;
	UCHAR PTK[64];		/* unicast and additional key */
} WAPI_UCAST_KEY_STRUCT, *PWAPI_UCAST_KEY_STRUCT;

typedef struct _WAPI_MCAST_KEY_STRUCT {
	UINT32 key_id;
	UCHAR m_tx_iv[16];
	UCHAR key_announce[16];
	UCHAR NMK[16];		/* notify master key */
} WAPI_MCAST_KEY_STRUCT, *PWAPI_MCAST_KEY_STRUCT;

typedef struct _WAPI_WIE_STRUCT {
	UCHAR addr[6];
	UINT32 wie_len;
	UCHAR wie[90];		/* wapi information element */
} WAPI_WIE_STRUCT, *PWAPI_WIE_STRUCT;

#endif /* WAPI_SUPPORT */

#ifdef DOT11R_FT_SUPPORT
typedef struct _FT_CONFIG_INFO {
	UCHAR MdId[2];
	UCHAR R0KHId[49];
	UCHAR R0KHIdLen;
	BOOLEAN FtSupport;
	BOOLEAN FtRicSupport;
	BOOLEAN FtOtdSupport;
} FT_CONFIG_INFO, *PFT_CONFIG_INFO;
#endif /* DOT11R_FT_SUPPORT */


#ifdef APCLI_SUPPORT
#ifdef WPA_SUPPLICANT_SUPPORT
#define	RT_ASSOC_EVENT_FLAG                         0x0101
#define	RT_DISASSOC_EVENT_FLAG                      0x0102
#define	RT_REQIE_EVENT_FLAG                         0x0103
#define	RT_RESPIE_EVENT_FLAG                        0x0104
#define	RT_ASSOCINFO_EVENT_FLAG                     0x0105
#define RT_PMKIDCAND_FLAG                           0x0106
#define RT_INTERFACE_DOWN                           0x0107
#define RT_INTERFACE_UP                             0x0108
#endif /* WPA_SUPPLICANT_SUPPORT */
#endif /* APCLI_SUPPORT */


#ifdef P2P_SUPPORT
/* RT_P2P_SPECIFIC_WIRELESS_EVENT */
#define RT_P2P_DEVICE_FIND							0x010A
#define RT_P2P_RECV_PROV_REQ							0x010B
#define RT_P2P_RECV_PROV_RSP							0x010C
#define RT_P2P_RECV_INVITE_REQ							0x010D
#define RT_P2P_RECV_INVITE_RSP							0x010E
#define RT_P2P_RECV_GO_NEGO_REQ							0x010F
#define RT_P2P_RECV_GO_NEGO_RSP							0x0110
#define RT_P2P_GO_NEG_COMPLETED						0x0111
#define RT_P2P_GO_NEG_FAIL						0x0112
#define RT_P2P_WPS_COMPLETED						0x0113
#define RT_P2P_CONNECTED							0x0114
#define RT_P2P_DISCONNECTED							0x0115
#define RT_P2P_CONNECT_FAIL							0x0116
#define RT_P2P_LEGACY_CONNECTED					0x0117
#define RT_P2P_LEGACY_DISCONNECTED					0x0118
#define RT_P2P_AP_STA_CONNECTED					0x0119
#define RT_P2P_AP_STA_DISCONNECTED					0x011A
#define RT_P2P_DEVICE_TABLE_ITEM_DELETE				0x011B
#define RT_P2P_GO_NEGO_FAIL_INTENT					0x011C
/* RT_P2P_SPECIFIC_WIRELESS_EVENT */

#define OID_802_11_P2P_MODE	0x0801
#define OID_802_11_P2P_DEVICE_NAME			0x0802
#define OID_802_11_P2P_LISTEN_CHANNEL		0x0803
#define OID_802_11_P2P_OPERATION_CHANNEL		0x0804
#define OID_802_11_P2P_DEV_ADDR		0x0805
#define OID_802_11_P2P_SCAN_LIST		0x0806
#define OID_802_11_P2P_GO_INT		0x080c

#define OID_802_11_P2P_CTRL_STATUS		0x0807
#define OID_802_11_P2P_DISC_STATUS		0x0808
#define OID_802_11_P2P_GOFORM_STATUS		0x0809
#define OID_P2P_WSC_PIN_CODE		0x080a
#define OID_802_11_P2P_CLEAN_TABLE		0x080b
#define OID_802_11_P2P_SCAN		0x080d
#define OID_802_11_P2P_WscMode		0x080e
#define OID_802_11_P2P_WscConf		0x080f
/* 0x0810 ~ 0x0814 Reserved for iNIC USERDEF_GPIO_SUPPORT */
/* 0x0820 ~ 0x0822 Reserved for iNIC USERDEF_GPIO_SUPPORT */
#define OID_802_11_P2P_Link								0x0830
#define OID_802_11_P2P_Connected_MAC					0x0831
#define OID_P2P_OFFSET						0x0000
#ifdef WIDI_SUPPORT
#undef OID_P2P_OFFSET
#define OID_P2P_OFFSET						0x0010
#define OID_802_11_P2P_PERSISTENT_TABLE		0x0832
#define RT_OID_INTEL_P2P_EDID				0x0833
#define OID_GAS_INIT_REQ_DATA				0x0834
#define OID_GAS_INIT_RSP_DATA				0x0835
#define OID_GAS_COMEBACK_REQ_DATA			0x0836
#define OID_GAS_COMEBACK_RSP_DATA			0x0837
#define OID_SEND_SRV_DISC					0x0838
#define OID_WFD_IE_IN_BEACON				0x0839
#define OID_WFD_IE_IN_PROBE_REQ				0x083a
#define OID_WFD_IE_IN_PROBE_RSP				0x083b
#endif /* WIDI_SUPPORT */
#define OID_802_11_P2P_RESET				(0x0832 + OID_P2P_OFFSET)
#define OID_802_11_P2P_SIGMA_ENABLE			(0x0833 + OID_P2P_OFFSET)
#define OID_802_11_P2P_SSID					(0x0834 + OID_P2P_OFFSET)
#define OID_802_11_P2P_CONNECT_ADDR			(0x0835 + OID_P2P_OFFSET)
#define OID_802_11_P2P_CONNECT_STATUS		(0x0836 + OID_P2P_OFFSET)
#define OID_802_11_P2P_PEER_GROUP_ID		(0x0837 + OID_P2P_OFFSET)
#define OID_802_11_P2P_ENTER_PIN					(0x0838 + OID_P2P_OFFSET)
#define OID_802_11_P2P_PROVISION					(0x0839 + OID_P2P_OFFSET)
#define OID_802_11_P2P_DEL_CLIENT					(0x083a + OID_P2P_OFFSET)
#define OID_802_11_P2P_PASSPHRASE					(0x0840 + OID_P2P_OFFSET)
#define OID_802_11_P2P_ASSOCIATE_TAB				(0x0841 + OID_P2P_OFFSET)
#define OID_802_11_P2P_PROVISION_MAC				(0x0842 + OID_P2P_OFFSET)
#define OID_802_11_P2P_LINK_DOWN						(0x0843 + OID_P2P_OFFSET)
#define OID_802_11_P2P_PRI_DEVICE_TYPE				(0x0844 + OID_P2P_OFFSET)
#define OID_802_11_P2P_INVITE							(0x0845 + OID_P2P_OFFSET)
#define OID_802_11_P2P_PERSISTENT_TABLE				(0x0846 + OID_P2P_OFFSET)
#define OID_DELETE_PERSISTENT_TABLE          				(0x0847 + OID_P2P_OFFSET)
/* If p2p0 is Go, please use following OID to trigger WPS with None-P2P STA */
#define OID_802_11_P2P_TRIGGER_WSC						(0x0848 + OID_P2P_OFFSET)
#define OID_802_11_P2P_WSC_CONF_MODE						(0x0849 + OID_P2P_OFFSET)
#define OID_802_11_P2P_PERSISTENT_ENABLE            			(0x084a + OID_P2P_OFFSET)
#define OID_802_11_P2P_WSC_CANCEL					(0x084b + OID_P2P_OFFSET)
#define OID_802_11_P2P_WSC_MODE						(0x0850 + OID_P2P_OFFSET)
#define OID_802_11_P2P_PIN_CODE						(0x0851 + OID_P2P_OFFSET)
#define OID_802_11_P2P_AUTO_ACCEPT					(0x0852 + OID_P2P_OFFSET)
#define OID_802_11_P2P_CHECK_PEER_CHANNEL			(0x0853 + OID_P2P_OFFSET)
#define OID_DELETE_PERSISTENT_ENTRY          				(0x0854 + OID_P2P_OFFSET)

#ifdef WFD_SUPPORT
#define OID_802_11_WFD_ENABLE						(0x0859 + OID_P2P_OFFSET)
#define OID_802_11_WFD_DEVICE_TYPE				(0x0860 + OID_P2P_OFFSET)
#define OID_802_11_WFD_SOURCE_COUPLED			(0x0861 + OID_P2P_OFFSET)
#define OID_802_11_WFD_SINK_COUPLED				(0x0862 + OID_P2P_OFFSET)
#define OID_802_11_WFD_SESSION_AVAILABLE 		(0x0863 + OID_P2P_OFFSET)
#define OID_802_11_WFD_RTSP_PORT					(0x0864 + OID_P2P_OFFSET)
#define OID_802_11_WFD_MAX_THROUGHPUT			(0x0865 + OID_P2P_OFFSET)
#define OID_802_11_WFD_SESSION_ID				(0x0866 + OID_P2P_OFFSET)
#define OID_802_11_WFD_PEER_RTSP_PORT			(0x0867 + OID_P2P_OFFSET)
#define RT_OID_802_11_QUERY_WFD_TDLS_CONNECT_STATUS       (0x0868 + OID_P2P_OFFSET)
#define RT_OID_802_11_QUERY_WFD_TDLS_PEER_IP_ADDR    (0x0869 + OID_P2P_OFFSET)
#define OID_802_11_WFD_CONTENT_PROTECT			(0x086a + OID_P2P_OFFSET)

#define OID_802_11_WFD_DEV_LIST						(0x0870 + OID_P2P_OFFSET)
#ifdef RT_CFG80211_SUPPORT
#define OID_802_11_WFD_IE_INSERT					(0x0871 + OID_P2P_OFFSET)
#endif /* RT_CFG80211_SUPPORT */
#endif /* WFD_SUPPORT */

#define RT_OID_802_11_P2P_MODE	(OID_GET_SET_TOGGLE + OID_802_11_P2P_MODE)
#define RT_OID_802_11_P2P_DEVICE_NAME		(OID_GET_SET_TOGGLE + OID_802_11_P2P_DEVICE_NAME)
#define RT_OID_802_11_P2P_LISTEN_CHANNEL		(OID_GET_SET_TOGGLE + OID_802_11_P2P_LISTEN_CHANNEL)
#define RT_OID_802_11_P2P_OPERATION_CHANNEL		(OID_GET_SET_TOGGLE + OID_802_11_P2P_OPERATION_CHANNEL)
#define RT_OID_802_11_P2P_DEV_ADDR	(OID_GET_SET_TOGGLE + OID_802_11_P2P_DEV_ADDR)
#define RT_OID_802_11_P2P_SCAN_LIST	(OID_GET_SET_TOGGLE + OID_802_11_P2P_SCAN_LIST)
#define RT_OID_802_11_P2P_CTRL_STATUS	(OID_GET_SET_TOGGLE + OID_802_11_P2P_CTRL_STATUS)
#define RT_OID_802_11_P2P_DISC_STATUS	(OID_GET_SET_TOGGLE + OID_802_11_P2P_DISC_STATUS)
#define RT_OID_802_11_P2P_GOFORM_STATUS	(OID_GET_SET_TOGGLE + OID_802_11_P2P_GOFORM_STATUS)
#define RT_OID_P2P_WSC_PIN_CODE	(OID_GET_SET_TOGGLE + OID_P2P_WSC_PIN_CODE)
#define RT_OID_802_11_P2P_CLEAN_TABLE	(OID_GET_SET_TOGGLE + OID_802_11_P2P_CLEAN_TABLE)
#define RT_OID_802_11_P2P_GO_INT	(OID_GET_SET_TOGGLE + OID_802_11_P2P_GO_INT)
#define RT_OID_802_11_P2P_SCAN	(OID_GET_SET_TOGGLE + OID_802_11_P2P_SCAN)
#define RT_OID_802_11_P2P_WscMode	(OID_GET_SET_TOGGLE + OID_802_11_P2P_WscMode)
#define RT_OID_802_11_P2P_WscConf	(OID_GET_SET_TOGGLE + OID_802_11_P2P_WscConf)
#define RT_OID_802_11_P2P_Link	(OID_GET_SET_TOGGLE + OID_802_11_P2P_Link)
#define RT_OID_802_11_P2P_Connected_MAC	(OID_GET_SET_TOGGLE + OID_802_11_P2P_Connected_MAC)
#define RT_OID_802_11_P2P_RESET	(OID_GET_SET_TOGGLE + OID_802_11_P2P_RESET)


#define IWEVP2PSHOWPIN 	0x8C05
#define IWEVP2PKEYPIN 	0x8C06

#ifdef DPA_S
/*
	P2P Status related with all events or status 
*/
typedef enum {
	P2P_NOTIF_NONE = 0,	
	/* ---------- Discovery --------------------------------------------- */
	/* Started the initial 802.11 scan phase */
	P2P_NOTIF_DISCOVER_START_80211_SCAN = 0x1001,
	
	/* Started the subsequent search-listen phase */
	P2P_NOTIF_DISCOVER_START_SEARCH_LISTEN,
	
	/* Sent on each iteration of the search-listen phase. */
	P2P_NOTIF_DISCOVER_SEARCH_LISTEN_ITERATION,

	/* Have results from the initial 802.11 scan */
	P2P_NOTIF_DISCOVER_FOUND_P2P_GROUPS,

	/* Have results from subsequent search-listen phase */
	P2P_NOTIF_DISCOVER_FOUND_PEERS,
	P2P_NOTIF_DISCOVER_CANCEL,
	P2P_NOTIF_DISCOVER_FAIL,
	P2P_NOTIF_DISCOVER_COMPLETE,
	
	/* Discovery suspended due to start of GO Negotiation */
	P2P_NOTIF_DISCOVER_SUSPENDED,

	/** Discovery resumed after GO Negotiation failure */
	P2P_NOTIF_DISCOVER_RESUMED,

	/** Started discovery in listen-only state */
	P2P_NOTIF_DISCOVER_START_LISTEN_ONLY,

	/* ---------- Provision Discovery ----------------------------------- */
	P2P_NOTIF_PROVISION_DISCOVERY_REQUEST = 0x2101,
	P2P_NOTIF_PROVISION_DISCOVERY_RESPONSE,
	P2P_NOTIF_PROVISION_DISCOVERY_TIMEOUT,

	/* ---------- Group Owner Negotiation ------------------------------- */
	/** Sent GON request	pNotificationData: NULL	*/
	P2P_NOTIF_GROUP_OWNER_NEGOTIATION_START = 0x2201,

	/** pNotificationData: PP2P_DISCOVER_ENTRY */
	P2P_NOTIF_GROUP_OWNER_NEGOTIATION_AP_ACK,

	/** pNotificationData: PP2P_DISCOVER_ENTRY */
	P2P_NOTIF_GROUP_OWNER_NEGOTIATION_STA_ACK,

	/** pNotificationData: NULL */
	P2P_NOTIF_GROUP_OWNER_NEGOTIATION_COMPLETE,

	/** pNotificationData: NULL */
	P2P_NOTIF_GROUP_OWNER_NEGOTIATION_FAIL,

	/** pNotificationData: PP2P_DISCOVER_ENTRY */
	/* Deprecated, use P2P_NOTIF_GROUP_OWNER_NEGOTIATION_NO_PROV_INFO. */
	P2P_NOTIF_GROUP_OWNER_NEGOTIATION_NO_PIN,

	/** Peer has no provisioning info.	  pNotificationData: PP2P_DISCOVER_ENTRY	  */
	P2P_NOTIF_GROUP_OWNER_NEGOTIATION_INFO_UNAVAIL,

	/** pNotificationData: NULL */
	P2P_NOTIF_GROUP_OWNER_NEGOTIATION_FAIL_INTENT,

	/** We have no provisioning info.	  pNotificationData: PP2P_DISCOVER_ENTRY	  */
	P2P_NOTIF_GROUP_OWNER_NEGOTIATION_NO_PROV_INFO,

	/** We are already in an existing P2P connection.	  pNotificationData: NULL	  */
	P2P_NOTIF_GROUP_OWNER_NEGOTIATION_ALREADY_CONNECTED,

	/** Received GON request	pNotificationData: PP2P_DISCOVER_ENTRY	*/
	P2P_NOTIF_GROUP_OWNER_NEGOTIATION_REQUEST_RECEIVED,

	/* ---------- P2P Invite -------------------------------------------- */
	/** pNotificationData: PP2P_INVITE_PARAM */
	P2P_NOTIF_P2P_INVITE_REQ = 0x2301,
	P2P_NOTIF_P2P_INVITE_RSP,

	/* ---------- P2P Presence------------------------------------------- */
	P2P_NOTIF_P2P_PRESENCE_REQ = 0x2401,
	P2P_NOTIF_P2P_PRESENCE_RSP,

	/* ---------- P2P Device Discoverability ---------------------------- */
	P2P_NOTIF_DEV_DISCOVERABILITY_REQ = 0x2501,
	P2P_NOTIF_GO_DISCOVERABILITY_REQ,
	P2P_NOTIF_DEV_DISCOVERABILITY_RSP,

	/* ---------- Link Creation ------------------------------------------ */
	P2P_NOTIF_CREATE_LINK_START = 0x3001,
	P2P_NOTIF_CREATE_LINK_CANCEL,
	P2P_NOTIF_CREATE_LINK_TIMEOUT,
	P2P_NOTIF_CREATE_LINK_AUTH_FAIL,
	P2P_NOTIF_CREATE_LINK_FAIL,

	/* ---------- SoftAP ------------------------------------------ */
	/** SoftAP-creation process starts */
	P2P_NOTIF_SOFTAP_START,

	/** SoftAP is ready to provide the service */
	P2P_NOTIF_SOFTAP_READY,	

	/** SoftAP has stopped */
	P2P_NOTIF_SOFTAP_STOP,

	/** SoftAP-creation failed */
	P2P_NOTIF_SOFTAP_FAIL,
	P2P_NOTIF_DHCP_START,
	P2P_NOTIF_DHCP_STOP,

	/** Successful P2P connection - pNotificationData: P2P_PERSISTENT */
	P2P_NOTIF_CREATE_LINK_COMPLETE,
	P2P_NOTIF_SOFTAP_STA_ASSOC,
	P2P_NOTIF_SOFTAP_STA_DISASSOC,

	/** P2P GC loss of link */
	P2P_NOTIF_LINK_LOSS,
/** add by woody */
	P2P_NOTIF_CONNECT_TIMEOUT,

	/* ---------- WPS status -------------------------------------------- */
	P2P_NOTIF_WPS_START = 0x4001,
	P2P_NOTIF_WPS_STATUS_SCANNING,
	P2P_NOTIF_WPS_STATUS_SCANNING_OVER,
	P2P_NOTIF_WPS_STATUS_ASSOCIATING,
	P2P_NOTIF_WPS_STATUS_ASSOCIATED,
	P2P_NOTIF_WPS_STATUS_WPS_MSG_EXCHANGE,
	P2P_NOTIF_WPS_STATUS_DISCONNECTING,
	P2P_NOTIF_WPS_COMPLETE,
	P2P_NOTIF_WPS_PROTOCOL_FAIL,
	P2P_NOTIF_WPS_WRONG_PIN,
	P2P_NOTIF_WPS_TIMEOUT,	
	P2P_NOTIF_WPS_SESSION_OVERLAP,
	P2P_NOTIF_WPS_FAIL,

	/* generic errors */
	/* ---------- Service Discovery-------------------------------------- */
	/** Service Response received - pNotificationData: PP2P_SVC_LIST */
	P2P_NOTIF_SVC_RESP_RECEIVED = 0x5001,

	/** Service Request received. */
	P2P_NOTIF_SVC_REQ_RECEIVED,

	/** Failed to decode service frame */
	P2P_NOTIF_SVC_FAIL_TO_DECODE,

	/** Service Comeback Response received. */
	P2P_NOTIF_SVC_COMEBACK_RESP_RECEIVED,

	/** Service Comeback Request received. */
	P2P_NOTIF_SVC_COMEBACK_REQ_RECEIVED,

	/** Session to request service completed */
	P2P_NOTIF_SVC_REQ_COMPLETED,

	/** Session to respond service completed */
	P2P_NOTIF_SVC_RSP_COMPLETED,

	/* ---------- Miscellaneous ----------------------------------------- */
	/* Primary Interface disconnected */
	P2P_NOTIF_PRIMARY_IF_DISCONNECTED = 0x6001

}P2PNotificationStatus;
#endif /* DPA_S */
#endif /* P2P_SUPPORT */

#ifdef WAC_QOS_PRIORITY
#define WAC_DEVICE_TYPE_TV			0x0
#define WAC_DEVICE_TYPE_PHONE		0x1
#define WAC_DEVICE_TYPE_PC			0x2
#define WAC_DEVICE_TYPE_CAMERA		0x3
#define WAC_DEVICE_TYPE_CAMCORDER	0x4
#define WAC_DEVICE_TYPE_PRINTER		0x5
#define WAC_DEVICE_TYPE_TC			0x6
#define WAC_DEVICE_TYPE_AP			0x7
#define WAC_DEVICE_TYPE_BD			0x8
#endif /* WAC_QOS_PRIORITY */

#ifdef RELEASE_EXCLUDE
/*
	DON'T USE OID 0x2000 ~ 0x2ffff 
	It's reserved for iNIC using.
 */
#endif // RELEASE_EXCLUDE //

#ifdef IWSC_SUPPORT
#define RT_OID_IWSC_SELF_IPV4				0x0900
#define RT_OID_IWSC_REGISTRAR_IPV4			0x0901
#define RT_OID_IWSC_SMPBC_ENROLLEE_COUNT	0x0902
#endif // IWSC_SUPPORT //

enum {
	OID_WIFI_TEST_BBP = 0x1000,
	OID_WIFI_TEST_RF = 0x1001,
	OID_WIFI_TEST_RF_BANK = 0x1002,
	OID_WIFI_TEST_MEM_MAP_INFO = 0x1003,
	OID_WIFI_TEST_BBP_NUM = 0x1004,
	OID_WIFI_TEST_RF_NUM = 0x1005,
	OID_WIFI_TEST_RF_BANK_OFFSET = 0x1006,
	OID_WIFI_TEST_MEM_MAP_NUM = 0x1007,
	OID_WIFI_TEST_BBP32 = 0x1008,
	OID_WIFI_TEST_MAC = 0x1009,
	OID_WIFI_TEST_MAC_NUM = 0x1010,
	OID_WIFI_TEST_E2P = 0x1011,
	OID_WIFI_TEST_E2P_NUM = 0x1012,
	OID_WIFI_TEST_PHY_MODE = 0x1013,
	OID_WIFI_TEST_RF_INDEX = 0x1014,
	OID_WIFI_TEST_RF_INDEX_OFFSET = 0x1015,
};

struct bbp_info {
	UINT32 bbp_start;
	UINT32 bbp_end;
	UINT8 bbp_value[0];
};

struct bbp32_info {
	UINT32 bbp_start;
	UINT32 bbp_end;
	UINT32 bbp_value[0];
};

struct rf_info {
	UINT16 rf_start;
	UINT16 rf_end;
	UINT8 rf_value[0];
};

struct rf_bank_info {
	UINT8 rf_bank;
	UINT16 rf_start;
	UINT16 rf_end;
	UINT8 rf_value[0];
};

struct rf_index_info {
	UINT8 rf_index;
	UINT16 rf_start;
	UINT16 rf_end;
	UINT32 rf_value[0];
};

struct mac_info {
	UINT32 mac_start;
	UINT32 mac_end;
	UINT32 mac_value[0];
};

struct mem_map_info {
	UINT32 base;
	UINT16 mem_map_start;
	UINT16 mem_map_end;
	UINT32 mem_map_value[0];
};

struct e2p_info {
	UINT16 e2p_start;
	UINT16 e2p_end;
	UINT16 e2p_value[0];
};

struct phy_mode_info {
	int data_phy;
	UINT8 data_bw;
	UINT8 data_ldpc;
	UINT8 data_mcs;
	UINT8 data_gi;
	UINT8 data_stbc;
};

struct anqp_req_data {
	UINT32 ifindex;
	UCHAR peer_mac_addr[6];
	UINT32 anqp_req_len;
	UCHAR anqp_req[0];
};

struct anqp_rsp_data {
	UINT32 ifindex;
	UCHAR peer_mac_addr[6];
	UINT16 status;
	UINT32 anqp_rsp_len;
	UCHAR anqp_rsp[0];
};

struct hs_onoff {
	UINT32 ifindex;
	UCHAR hs_onoff;
	UCHAR event_trigger;
	UCHAR event_type;
};

struct hs_param_setting {
	UINT32 param;
	UINT32 value;
};


struct btm_req_data {
	UINT32 ifindex;
	UCHAR peer_mac_addr[6];
	UINT32 btm_req_len;
	UCHAR btm_req[0];
};

struct btm_query_data {
	UINT32 ifindex;
	UCHAR peer_mac_addr[6];
	UINT32 btm_query_len;
	UCHAR btm_query[0];
};

struct btm_rsp_data {
	UINT32 ifindex;
	UCHAR peer_mac_addr[6];
	UINT32 btm_rsp_len;
	UCHAR btm_rsp[0];
};

struct proxy_arp_entry {
	UINT32 ifindex;
	UCHAR ip_type;
	UCHAR from_ds;
	UCHAR IsDAD;
	UCHAR source_mac_addr[6];
	UCHAR target_mac_addr[6];
	UCHAR ip_addr[0];
};


struct security_type {
	UINT32 ifindex;
	UINT8 auth_mode;
	UINT8 encryp_type;
};

struct wnm_req_data {
	UINT32 ifindex;
	UCHAR peer_mac_addr[6];
	UINT32 type;
	UINT32 wnm_req_len;
	UCHAR wnm_req[0];
};

struct qosmap_data {
	UINT32 ifindex;
	UCHAR peer_mac_addr[6];
	UINT32 qosmap_len;
	UCHAR qosmap[0];
};

#define OID_802_11_WIFI_VER                     0x0920
#define OID_802_11_HS_TEST                      0x0921
#define OID_802_11_HS_IE                        0x0922
#define OID_802_11_HS_ANQP_REQ                  0x0923
#define OID_802_11_HS_ANQP_RSP                  0x0924
#define OID_802_11_HS_ONOFF                     0x0925
#define OID_802_11_HS_PARAM_SETTING             0x0927
#define OID_802_11_WNM_BTM_REQ                  0x0928
#define OID_802_11_WNM_BTM_QUERY                0x0929
#define OID_802_11_WNM_BTM_RSP                  0x093a
#define OID_802_11_WNM_PROXY_ARP                0x093b
#define OID_802_11_WNM_IPV4_PROXY_ARP_LIST      0x093c
#define OID_802_11_WNM_IPV6_PROXY_ARP_LIST      0x093d
#define OID_802_11_SECURITY_TYPE                0x093e
#define OID_802_11_HS_RESET_RESOURCE            0x093f
#define OID_802_11_HS_AP_RELOAD                 0x0940
#define OID_802_11_HS_BSSID                     0x0941
#define OID_802_11_HS_OSU_SSID                  0x0942
//#define OID_802_11_HS_OSU_NONTX               0x0944
#define OID_802_11_HS_SASN_ENABLE               0x0943
#define OID_802_11_WNM_NOTIFY_REQ               0x0944
#define OID_802_11_QOSMAP_CONFIGURE             0x0945
#define OID_802_11_GET_STA_HSINFO             	0x0946
#define OID_802_11_BSS_LOAD			           	0x0947
#endif /* _OID_H_ */

