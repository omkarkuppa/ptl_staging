/** @file
  Declaration of Intel Connectivity Variables Structures and guids for BT.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

  @par Specification Reference: 559910_Intel_Connectivity_Platforms_BIOS_Guidelines_Rev6_6.pdf

**/

#ifndef __CNV_VARIABLES_BT_H__
#define __CNV_VARIABLES_BT_H__
//
// Include UEFI variables Revision, Headers and Guids definition.
//
#include <CnvVariablesHeaders.h>
//
// Include all Common UEFI Variables Structures definition.
//
#include <CnvVariablesCommon.h>

#pragma pack(push, 1)

//
// SAR Power Limits struct for BRDS, EBRD
//
typedef struct {
  UINT8   subBand2G4;      // Bluetooth SAR power restriction in sub-band 2G4
  UINT8   subBand5G2;      // Bluetooth SAR power restriction in sub-band 5G2
  UINT8   subBand5G85G9;   // Bluetooth SAR power restriction in sub-band 5G8 and 5G9
  UINT8   subBand6G1;      // Bluetooth SAR power restriction in sub-band 6G1
  UINT8   subBand6G3;      // Bluetooth SAR power restriction in sub-band 6G3
} BT_SAR_POWER_LIMITS;

//
// Definition for Bluetooth wireless Regulatory Domain SAR (BRDS) Revision 2
//
#define CNV_UEFI_BRDS_VAR_NAME    L"UefiCnvBtBRDS"
#define CNV_UEFI_BRDS_REVISION_2  2

typedef struct {
  CNV_UEFI_VAR_HEADER  Header;
  UINT32               Mode;                // Defines the BRDS Modes. Bit 0 Enabled (Value 0 - Disabled/ 1 - Enabled). Bits 1:31 reserved shall be set to zeros.
  UINT32               IncreasedPowerMode;  // Defines Bluetooth Sar limitation feature disablement. Default : 0-enabled, 1-Disable
  BT_SAR_POWER_LIMITS  BtSarValuesChainA;   // Defines the BT SAR Values for chain A.
  BT_SAR_POWER_LIMITS  BtSarValuesChainB;   // Defines the BT SAR Values for chain B.
} CNV_UEFI_VAR_BRDS;

//
// Definition for Bluetooth Tile Configuration (BTLC)
//
#define CNV_UEFI_BTLC_VAR_NAME    L"UefiCnvBtBTLC"
#define CNV_UEFI_BTLC_REVISION_1  1

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version.
  <b>Revision 1</b>:  - Migration from ACPI to UEFI Variable.
**/

/*
  | Tile Mode     | Bit #   |  CNVi/CNVd/Both   | S0    | S0ix  | S4    | S5    |
  -------------------------------------------------------------------------------
  | Find My PC    | 0       | Both              | V     | V     | V     | V     |
  | Ring My PC    | 1:2     | Both              | V     | V     | V     | V     |
  -------------------------------------------------------------------------------

  Bit 0  - Find my PC
  -------------------
  0x0 : Tile Locate (Find) is disabled in this system state
  0x1 : Tile Locate (Find) is enabled in this system state

  Bits 1-2 Ring My PC
  -------------------
  0x0 : Tile Ring is disabled in this system state
  0x1 : Tile Ring is enabled in this system state. BT_LED is used for Ring by EC/BIOS
  0x2, 0x3 - Reserved

*/
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;           ///< Revision
  UINT8                  Mode;             ///< Defines the mode of Tile Feature to be used. 0 - Tile disabled | 1 Tile Enabled.
  UINT8                  S0;               ///< Defines the activity in case of Tile activated and Triggered in S0
  UINT8                  S0ix;             ///< Defines the activity in case of Tile activated and Triggered in S0ix
  UINT8                  S4;               ///< Defines the activity in case of Tile activated and Triggered in S4
  UINT8                  S5;               ///< Defines the activity in case that Tile activated and Triggered in S5
  UINT8                  LedConfig;        ///< Bluetooth SAR LE 2Mhz Modulation. Bit 0 : BT_LED Connected to EC Bits 7:1 reserved
  UINT8                  LedPulseDuration; ///< BT_LED Pulse Duration in milliseconds. Min 1ms (0x01 Hex) Max 255 ms(0xFF Hex)
  UINT8                  LedPulseInterval; ///< BT_LED Pulse Interval in tens of milliseconds Min 10 ms (0x01 Hex) Max 2550 ms(0xFF Hex)
  UINT8                  TileInAPM;        ///< APM Tile Mode
  UINT8                  Reserved;         ///< Shall set to zero
} CNV_UEFI_VAR_BTLC;

#if (FixedPcdGet16 (PcdCnvUefiVarVersion) == 4)
//
// Definition for Bluetooth Audio offload (AOLD)
//
#define CNV_UEFI_AOLD_VAR_NAME    L"UefiCnvBtAOLD"
#define CNV_UEFI_AOLD_REVISION_0  0

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version.
**/

typedef struct {
  CNV_UEFI_VAR_HEADER    Header;                ///< Revision
  UINT32                 AudioOffloadSelection; ///< Set the audio offload selection. 0 : Audio Offload is Disabled 1 : Audio Offload is Enabled
} CNV_UEFI_VAR_AOLD;
#endif

//
// Definition for Bluetooth BiQuad Filter Bypass (BBFB)
//
#define UEFI_CNV_BBFB_VAR_NAME  L"IntelUefiCnvBtBiQuadFilterBypass"
#define UEFI_CNV_BBFB_VAR_REV   1
//
// IntelUefiCnvBtBiQuadFilterBypass Main Data Structure
//
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;
  UINT32                 BiQuadBypass;
} ICNV_BT_BIQUAD_BYPASS_REV_1;

//
// Definition for BT PPAG support
//
#define CNV_UEFI_BT_PPAG_VAR_NAME      L"IntelUefiCnvBtPpagSupport"
#define CNV_UEFI_BT_PPAG_VAR_REVISION  2
//
// IntelUefiCnvBtPpagSupport Main Data Structure
// ppagModesU32 - provides the ability to control PPAG feature feature enablement. If set to zero, the feature is disabled. Default is zero.
//                Bit 0 If set 1 , enables PPAG for Bluetooth core in EU countries in LB/HB
//                Bit 1 If set 1 , enables PPAG for Bluetooth core  in China Mainland in LB/HB
//                Bit 2 If set 1 , enables PPAG for Bluetooth core  in EU UHB
//                Bit 3 If set 1 , enables PPAG for Bluetooth core  in FCC UHB
//                Bit 4 If set 1 , enables PPAG for Bluetooth core  in ISED UHB
//                Bits 5:31 reserved and shall be zeroed.
//
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;
  UINT32                 ppagModesU32;
} BT_PPAG_CONFIG_SET;

//
// Definition for Extended Bluetooth Descriptor (EBRD)
//
#define CNV_UEFI_EBRD_VAR_NAME           L"UefiCnvBtEBRD"
#define CNV_UEFI_EBRD_REVISION_1         1
#define CNV_UEFI_EBRD_BT_SAR_VALUES_EXT  3

typedef struct {
  UINT8   BtSarPowerAntA;
  UINT8   BtSarPowerAntB;
} BT_SAR_LIMITS_EXT_DCMB;

typedef struct {
  CNV_UEFI_VAR_HEADER      Header;
  UINT32                   BtDynamicSarMode;                                              // BtDynamicSarMode - Defines the Bluetooth Dynamic SAR Enable/Disable.
  UINT32                   BtDynamicSarRangeSets;                                         // BtDynamicSarMode - Defines the Bluetooth SAR Number of Optional added SAR table sets to be used: (0-3).
  BT_SAR_LIMITS_EXT_DCMB   BtDynamicSarValues2G4Ext[CNV_UEFI_EBRD_BT_SAR_VALUES_EXT];     // BtDynamicSarValues2G4Ext - Defines the Extended 2G4 SAR Values
  BT_SAR_LIMITS_EXT_DCMB   BtDynamicSarValues5G2Ext[CNV_UEFI_EBRD_BT_SAR_VALUES_EXT];     // BtDynamicSarValues5G2Ext - Defines the Extended 5G2 SAR Values.
  BT_SAR_LIMITS_EXT_DCMB   BtDynamicSarValues5G85G9Ext[CNV_UEFI_EBRD_BT_SAR_VALUES_EXT];  // BtDynamicSarValues5G85G9Ext - Defines the Extended 5G85G9 SAR Values.
  BT_SAR_LIMITS_EXT_DCMB   BtDynamicSarValues6G1Ext[CNV_UEFI_EBRD_BT_SAR_VALUES_EXT];     // BtDynamicSarValues6G1Ext - Defines the Extended 6G1 SAR Values
  BT_SAR_LIMITS_EXT_DCMB   BtDynamicSarValues6G3Ext[CNV_UEFI_EBRD_BT_SAR_VALUES_EXT];     // BtDynamicSarValues6G3Ext - Defines the Extended 6G3 SAR Values.
} CNV_UEFI_VAR_EBRD;

//
// Definition for BT Bands Selection Mode
//
#define CNV_UEFI_BT_BANDS_SEL_VAR_NAME    L"IntelUefiCnvBtBandsSelection"
#define CNV_UEFI_BT_BANDS_SEL_REVISION_1  1

typedef struct {
  CNV_UEFI_VAR_HEADER    Header;         ///< Revision
  UINT32                 BandsSelection; ///< Defines the Bluetooth Bands Selection disablement
} ICNV_BT_BANDS_SELECTION_MODE_REV_1;

//
// Definition for BT UHB Country Select bit
//
#define CNV_UEFI_BT_UHB_COUNTRY_SELECT_VAR_NAME  L"IntelUefiCnvBtUHBCountrySelect"
#define CNV_UEFI_BT_UHB_COUNTRY_SEL_REV_1        1

typedef struct {
  CNV_UEFI_VAR_HEADER    Header;                 ///< Revision
  UINT32                 CountrySelectionBitmap; ///< Defines the Bluetooth High band or Ultra High Band country specific disablement per structure.
} ICNV_BT_UHB_COUNTRY_SELECT_REV_1;

//
// Definition for BT Dual Band Mode Configuartion
//
#define CNV_UEFI_BT_DUAL_CHAIN_MODE_NAME        L"IntelUefiCnvBtDualChainMode"
#define CNV_UEFI_BT_DUAL_CHAIN_MODE_REVISION_1  1

typedef struct {
  CNV_UEFI_VAR_HEADER    Header;        ///< Revision
  UINT32                 DualChainMode; ///< Defines the Dual Chain Mode (Chain A only, or both Chain A and Chain B)
} ICNV_BT_DUAL_CHAIN_MODE_REV_1;

//
// Definition for BT Dual Mac Mode Configuartion
//
#define CNV_UEFI_BT_DUAL_MAC_MODE_NAME    L"IntelUefiCnvBtDualMacMode"
#define ICNV_BT_DUAL_MAC_MODE_REVISION_1  1

typedef struct {
  CNV_UEFI_VAR_HEADER    Header;        ///< Revision
  UINT8                  DualMacMode;   ///< Defines the Enablement/Disablement of Dual Mac Mode
} ICNV_BT_DUAL_MAC_REV_1;

#pragma pack(pop)

#endif //__CNV_VARIABLES_BT_H__
