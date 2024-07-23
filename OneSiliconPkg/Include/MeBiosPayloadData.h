/** @file
  Interface definition details for MBP.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2006 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/
#ifndef _ME_BIOS_PAYLOAD_DATA_H_
#define _ME_BIOS_PAYLOAD_DATA_H_

#include <MkhiMsgs.h>

/**
  ME-BIOS Payload Data

  <b>Revision 1:</b>
  - Initial version.

**/

#define MBP_DATA_REVISION   1

#pragma pack (1)

#define MAX_MBP_ITEMS_SIZE  125 * sizeof (UINT32)

///
/// Common MBP types
///
typedef struct {
  UINT8  MbpSize;    ///< Byte 0 - MBP Size in DW including Header
  UINT8  NumEntries; ///< Byte 1 - Number of Entries (Data Items) in MBP
  UINT16 Reserved;   ///< Byte [3:2] - Reserved
} MBP_HEADER;

///
/// MBP items header
///
typedef union {
  UINT32 Data;
  struct {
    UINT32 AppId  : 8; ///< Byte 0 - Application ID
    UINT32 ItemId : 8; ///< Byte 1 - Item ID
    UINT32 Length : 8; ///< Byte 2 - Length in DW
    UINT32 Flags  : 8; ///< Byte 3 - Flags
  } Fields;
} MBP_ITEM_HEADER;

//
// Macro to build abstract MBP Item Id
//
#define MBP_ITEM_ID(ApplicationId, ItemId) ((ApplicationId << 4) | ItemId)

#define MBP_ITEM_FW_VERSION_NAME             MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdFwVerName)
#define MBP_ITEM_FW_CAPABILITIES             MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdFwCapabilities)
#define MBP_ITEM_FW_BIOS_PLATFORM_KEY        MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdBiosPlatformKey)
#define MBP_ITEM_CSE_PLATFORM_TYPE           MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdCsePlatformType)
#define MBP_ITEM_ID_MFS_INTEGRITY            MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdMsfIntegrity)
#define MBP_ITEM_ID_PERF_DATA                MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdPerformanceData)
#define MBP_ITEM_UNCONFIG_ON_RTC             MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdUnconfigOnRtc)
#define MBP_ITEM_SHIP_STATE                  MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdShipState)
#define MBP_ITEM_ID_FW_SECURITY_VER          MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdFwSecurityVer)
#define MBP_ITEM_ID_FW_ARB_SVN               MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdFwArbSvn)
#define MBP_ITEM_ID_MEASURED_BOOT            MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdMeasuredBoot)
#define MBP_ITEM_PERF_DATA_EX                MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdPerfDataEx)
#define MBP_ITEM_ID_KEY_REVOCATION           MBP_ITEM_ID(MbpAppIdKernel, MbpItemIdKeyRevocation)
#define MBP_ITEM_ID_PSR                      MBP_ITEM_ID(MbpAppIdPsr,    MbpItemIdPsr)
#define MBP_ITEM_OEM_NPHY_DATA               MBP_ITEM_ID(MbpAppIdIpl,    MbpItemIdOemNphyData)
#define MBP_ITEM_OEM_SPHY_DATA               MBP_ITEM_ID(MbpAppIdIpl,    MbpItemIdOemSphyData)
#define MBP_ITEM_HWA_MTU                     MBP_ITEM_ID(MbpAppIdHwa,    MbpItemIdHwaMtu)
#define MBP_ITEM_ID_ICC_PROFILE              MBP_ITEM_ID(MbpAppIdIcc,    MbpItemIdIccProfile)
#define MBP_ITEM_ID_ICC_NONCE                MBP_ITEM_ID(MbpAppIdIcc,    MbpItemIdIccNonce)
#define MBP_ITEM_CHIPSET_INIT_VER            MBP_ITEM_ID(MbpAppIdIcc,    MbpItemIdMphyData)
#define MBP_ITEM_OEM_PHY_DATA                MBP_ITEM_ID(MbpAppIdIcc,    MbpItemIdOemPhyData)
#define MBP_ITEM_ID_CURRENT_BOOT_MEDIA       MBP_ITEM_ID(MbpAppIdNvm,    MbpItemIdCurrentBootMedia)

//
// Enum for AppId
//
typedef enum {
  MbpAppIdKernel         = 1,
  MbpAppIdPsr            = 2,
  MbpAppIdIpl            = 3,
  MbpAppIdHwa            = 4,
  MbpAppIdIcc            = 5,
  MbpAppIdDnx            = 7,
  MbpAppIdNvm            = 8
} MBP_APPLICATION_ID;

//
// Enums for ItemIds
//
typedef enum {
  MbpItemIdFwVerName                  = 1,
  MbpItemIdFwCapabilities             = 2,
  MbpItemIdBiosPlatformKey            = 4,
  MbpItemIdCsePlatformType            = 5,
  MbpItemIdMsfIntegrity               = 6,
  MbpItemIdPerformanceData            = 7,
  MbpItemIdUnconfigOnRtc              = 8,
  MbpItemIdShipState                  = 9,
  MbpItemIdFwSecurityVer              = 10,
  MbpItemIdPerfDataEx                 = 13,
  MbpItemIdFwArbSvn                   = 14,
  MbpItemIdMeasuredBoot               = 15,
  MbpItemIdKeyRevocation              = 16
} MBP_KERNEL_ITEM_ID;

typedef enum {
  MbpItemIdPsr                        = 1,
} MBP_PSR_ITEM_ID;

typedef enum {
  MbpItemIdIsiConfig                  = 1,
  MbpItemIdOemNphyData                = 2,
  MbpItemIdOemSphyData                = 4,
} MBP_IPL_ITEM_ID;

typedef enum {
  MbpItemIdHwaMtu = 1
} MBP_HWA_ITEM_ID;

typedef enum {
  MbpItemIdIccProfile = 1,
  MbpItemIdIccNonce   = 2,
  MbpItemIdMphyData   = 3,
  MbpItemIdOemPhyData = 4
} MBP_ICC_ITEM_ID;

typedef enum {
  MbpItemIdCurrentBootMedia  = 1
} MBP_NVM_ITEM_ID;


//
// Enum for Flags
//
typedef enum {
  MbpItemFlagStoreToNvm = 0, ///< Item contains data to be stored in flash
  MbpItemFlagSensitive  = 1, ///< Item contains sensitive data.  Do not store in flash.
  MbpItemFlagRuntime    = 2, ///< Item is generated at runtime.  Do not store in flash.
} MBP_ITEM_FLAGS;

///
/// MBP items
///

///
/// Kernel AppId
///
typedef struct {
  UINT32  MajorVersion  : 16;
  UINT32  MinorVersion  : 16;
  UINT32  HotfixVersion : 16;
  UINT32  BuildVersion  : 16;
} MBP_FW_VERSION_NAME;

typedef struct {
  MEFWCAPS_SKU  FwCapabilities;
  BOOLEAN       Available;
  UINT8         Reserved[3];
} MBP_FW_CAPS_SKU;

typedef struct {
  PLATFORM_TYPE_RULE_DATA RuleData;
  BOOLEAN                 Available;
  UINT8                   Reserved[3];
} MBP_PLAT_TYPE;

typedef struct {
  UINT32 DisUnconfigOnRtcClearState : 1;
  UINT32 Reserved                   : 31;
} MBP_ME_UNCONF_ON_RTC_DATA;

typedef struct {
  MBP_ME_UNCONF_ON_RTC_DATA   UnconfigOnRtcClearData;
  BOOLEAN                     Available;
  UINT8                       Reserved[3];
} MBP_ME_UNCONF_ON_RTC_STATE;

typedef struct {
  MEFWCAPS_SKU  FwFeatures;
  BOOLEAN       Available;
  UINT8         Reserved[3];
} MBP_FW_FEATURES_STATE;


#define ARB_SVN_MISMATCH               BIT0
typedef struct {
  /*
    ARB SVN flags:
    BIT[0]: set if ARB SVN mismatch is found for any of ARB SVN components.
    BIT[1..6]: Reserved, should be 0
    BIT[7]: set if ARB_SVN exceeds 1 byte size
  */
  UINT8                       Flags;
  UINT8                       MinCseArbSvn;  ///< Minimal CSE ARB SVN.
  UINT8                       CurrCseArbSvn; ///< Current CSE ARB SVN.
  UINT8                       Reserved;      ///< Reserved, should be 0.
} MBP_ARB_SVN_DATA;

typedef struct {
  MBP_ARB_SVN_DATA            ArbSvnData;
  BOOLEAN                     Available;
  UINT8                       Reserved[3];
} MBP_ARB_SVN_STATE;

///
/// MBP IFWI DNX request structure containing IFWI Dnx request data
///
typedef struct {
  UINT32      EnterRecovery;
  BOOLEAN     Available;
  UINT8       Reserved[3];
} MBP_IFWI_DNX_REQUEST;

typedef struct {
  UINT32 MeasuredBoot   : 1;
  UINT32 Reserved       : 31;
} MBP_MEASURED_BOOT_DATA;

typedef struct {
  MBP_MEASURED_BOOT_DATA      MeasuredBootData;
  BOOLEAN                     Available;
  UINT8                       Reserved[3];
} MBP_MEASURED_BOOT_SUPPORT;

typedef struct {
  UINT32 TimeStamp0;
  UINT16 TimeStamp1;
  UINT16 TimeStamp2;
  UINT16 TimeStamp3;
  UINT16 TimeStamp4;
  UINT16 TimeStamp5;
  UINT16 TimeStamp6;
} MBP_PERF_DATA;

typedef struct {
  MBP_PERF_DATA Data;
  BOOLEAN       Available;
  UINT8         Reserved[3];
} MBP_PERF_DATA_EX;

///
/// HWA AppId
///
typedef union {
  UINT32  Raw;
  struct {
    UINT32  MediaTablePush : 1;
    UINT32  Reserved       : 31;
  } Fields;
} HWA_DATA;

typedef struct {
  HWA_DATA Data;
  BOOLEAN  Available;
  UINT8    Reserved[3];
} MBP_HWA_REQ;

///
/// ICC AppId
///
typedef struct {
  UINT32  ChipsetInitVer[3];
  BOOLEAN Available;
  UINT8   Reserved[3];
} MBP_MPHY_DATA;

typedef struct {
  UINT64  Data;
  BOOLEAN Available;
  UINT8   Reserved[3];
} MBP_OEM_SPHY_DATA;

typedef struct {
  UINT64  Data;
  BOOLEAN Available;
  UINT8   Reserved[3];
} MBP_OEM_NPHY_DATA;

//
// PSR data:
//
// BIT[0] Chassis intrusion event : 0 - Do not send the event from BIOS to CSME, 1 - Send the event from BIOS to CSME
// BIT[1] PSR state : 0- Logging not started, 1- Logging started
// BIT[2:31] Reserved for future use.
//
typedef struct {
  UINT32 ChassisIntrusionEvent : 1;  ///< Indicator to send the event from BIOS to CSME
  UINT32 PsrLogState           : 1;  ///< Indicator for PSR Log State
  UINT32 Reserved              : 30; ///< Reserved for future usages. Should be 0.
} MBP_PSR_DATA;

typedef struct {
  MBP_PSR_DATA             PsrData;
  UINT32                   PsrCapabilities;
  BOOLEAN                  Available;
  UINT8                    Reserved[3];
} MBP_PSR;

///
/// ME BIOS Payload structure containing insensitive data only
///
typedef struct {
  MBP_FW_VERSION_NAME        FwVersionName;
  MBP_FW_CAPS_SKU            FwCapsSku;
  MBP_FW_FEATURES_STATE      FwFeaturesState;
  MBP_PLAT_TYPE              FwPlatType;
  MBP_HWA_REQ                HwaRequest;
  MBP_ME_UNCONF_ON_RTC_STATE UnconfigOnRtcClearState;
  MBP_ARB_SVN_STATE          ArbSvnState;
  MBP_MPHY_DATA              MphyData;
  MBP_OEM_SPHY_DATA          OemSphyData;
  MBP_OEM_NPHY_DATA          OemNphyData;
  MBP_IFWI_DNX_REQUEST       IfwiDnxRequest;
  MBP_MEASURED_BOOT_SUPPORT  MeasuredBootSupport;
  MBP_PERF_DATA_EX           PerfDataEx;
  MBP_PSR                    Psr;
} ME_BIOS_PAYLOAD;

#pragma pack ()

#endif
