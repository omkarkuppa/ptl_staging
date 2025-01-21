/** @file
 OEM Specific Setup Variables and Structures

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _OEMSETUP_H_
#define _OEMSETUP_H_

#include <Library/PchLimits.h>

#ifdef __cplusplus
extern "C" {
#endif

// OS profile
#define OS_PROFILE_GUID \
 { 0xE59376D7, 0x2DD9, 0x42A3, { 0x9E, 0xC8, 0x1D, 0x71, 0xD5, 0xE3, 0xC1, 0xEC }}

// Currently enumerated OSes
#define OsNone                   0x00
#define OsAndroid                0x01
#define OsWinCs                  0x02
#define OsWinNonCs               0x03
#define OsMax                    0x04

#define PCH_MOBILE_SKU          0
#define PCH_DESKTOP_SKU         1
#define PCH_SERVER_SKU          2

#define PCH_RP_AVAILABLE       0
#define PCH_RP_FUSED_OFF       1
#define PCH_RP_MERGED          2
#define PCH_RP_DIFFERENT_BUS   3
#define PCH_RP_NOT_IMPLEMENTED 5

#define PCH_TSN_GBE_LANE8      0
#define PCH_TSN_GBE_LANE9      1
#define PCH_TSN_GBE_LANE11     2
#define PCH_TSN_GBE_LANE10     3

#pragma pack(1)

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Remove Item EnableDebugSerialIoUartNumber
  <b>Revision 3</b>:  - Added Item PSOnSupported
  <b>Revision 4</b>:  - Move Board related variables to SETUP_DATA
  <b>Revision 5</b>:  - Add Crashlog Capability
  <b>Revision 6</b>:  - Added OcrBootHttpsSupported, OcrBootPbaSupported, OcrBootWinReSupported, and OcrAmtDisSecBootSupported
  <b>Revision 7</b>:  - Added TcssPdType
  <b>Revision 10</b>: - Added AudioDspFusedOut
  <b>Revision 11</b>: - Added MemorySizeIn32MB, McCount
  <b>Revision 12</b>: - Added TotalNumberOfTypeCPortsSupported
  <b>Revision 13</b>: - Added IsS3Supported
**/

#define SETUP_VOLATILE_DATA_REVISION 15

typedef struct _SETUP_VOLATILE_DATA
{
  UINT16 PlatId;
  UINT8  PlatformFlavor;
  UINT8  PlatformType;
  UINT8  BoardType;
  UINT8  VTdAvailable;
  UINT8  PchGeneration;
  UINT8  GbeSupportByPch;            // Gbe is supported on current PCH
  UINT8  GbeAvailable;               // Gbe is configured and usable
  UINT8  ThcSupported;
  UINT8  ThcAvailable;
  UINT8  IshAvailable;
  UINT8  PchSeries;
  UINT8  DimmPresent[16];
  UINT8  MemoryType;                // 0 - DDR4, 1 - DDR5, 2 - LPDDR5, 3 - LPDDR4, 4 - UnKnown
  UINT8  PciePortCfg[MAX_PCIE_ROOT_PORTS];
  UINT8  PchUsb2PortCount;
  UINT8  PchUsb3PortCount;
  UINT8  UfsSupported[PCH_MAX_UFS_DEV_NUM];
  UINT32 AcpiDebugAddress;
  UINT8  SataMpsPresent[8];
  UINT8  DockAttached;
  UINT8  AudioHdaLinkSupported;
  UINT8  AudioDmicLinkSupported[PCH_MAX_HDA_DMIC_LINK_NUM];
  UINT8  AudioSspLinkSupported[PCH_MAX_HDA_SSP_LINK_NUM];
  UINT8  AudioSndwLinkSupported[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8  AudioSndwMultilaneSupported[PCH_MAX_HDA_SNDW_MULTILANE_NUM];
  UINT8  PchCridSupport;
  UINT8  IpuSupport;
  UINT8  TsnSupportByPch;
  UINT8  TsnAvailable;
  UINT8  PSOnSupported;
  UINT8  NpuSupport;
  UINT32  CrashlogOptionControl;
  UINT8  OcrBootHttpsSupported;
  UINT8  OcrBootPbaSupported;
  UINT8  OcrBootWinReSupported;
  UINT8  OcrAmtDisSecBootSupported;
  UINT8  OcrOptionExposure;
  UINT8  OcrOptionGrayOut;
  UINT8  PreviousOcrState[4];
  BOOLEAN OcrStateChanged;
  UINT8  TcssPdType;
  UINT8  AudioDspFusedOut;
  UINT8  IoeSupport;
  UINT8  MtlPchSupport;
  UINT8  IbeccSupport;
  UINT8  IaxSupport;
  UINT8  IsDisplayPresent;
  UINT8  IsGtPresent;
  UINT8  IsMediaPresent;
  UINT32 MemorySizeIn32MB;
  UINT8  McCount;
  UINT8  TsnGbeLaneNum;
  UINT16 TypeCPortsSupportedMap;
  BOOLEAN IsS3Supported;
  UINT32 TpmLocation;
} SETUP_VOLATILE_DATA;

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/

#define SETUP_CPU_FEATURES_REVISION 1

//This structure is used for setup.
typedef struct {
  UINT8 TurboModeAvailable;
  UINT8 XETdcTdpLimitAvailable;
  UINT8 CoreRatioLimitAvailable;
  UINT8 MultiCoreAvailable;
  UINT8 VTAvailable;
  UINT8 AESAvailable;
  UINT8 TXTAvailable;
  UINT8 CxAvailable;
  UINT8 C1Available;
  UINT8 C3Available;
  UINT8 C6Available;
  UINT8 C7Available;
  UINT8 C8Available;
  UINT8 C9Available;
  UINT8 C10Available;
  UINT8 EISTAvailable;
  UINT8 HwpAvailable;
  UINT8 NumCores;
  UINT8 MultiSocketAvailable;
  UINT8 MultiSocketPopulated;
  UINT8 LocalX2ApicAvailable;
  UINT8 EnergyEfficientTurboAvailable;
  UINT8 Reserved;
  UINT8 ApicIdFlag;
  UINT8 ConfigTdpAvailable;
  UINT8 PpinFeatureAvailable;
  UINT8 LpmAvailable;
  UINT8 TimedMwaitSupported;
  UINT8 DebugInterfaceSupported;
  UINT8 ProcTraceSupported;
  UINT8 HwpInterruptSupported;
  UINT8 FclkSupported;
  UINT8 PerCoreHtDisableSupported;
  UINT8 TmeSupported;
  BOOLEAN CtdpLevel1Supported;
  BOOLEAN CtdpLevel2Supported;
  UINT8 DlvrPhaseSscSupported;
  UINT8 TseSupported;
  UINT8 RpSupported;
  UINT8 NumSmallCoresCLuster;
  UINT8 TmeBypassSupported;
  UINT8 TdxSupported;
} SETUP_CPU_FEATURES;

typedef struct {
  UINT8   OsProfile;
  UINT8   PrevOs;
} OS_PROFILE;

#pragma pack()

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
