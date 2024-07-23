/** @file
  Declaration of Intel Connectivity Variables Structures and guids for WLAN.

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

#ifndef __CNV_VARIABLES_WLAN_H__
#define __CNV_VARIABLES_WLAN_H__

//
// Include UEFI variables Revsion, Headers and Guids definition.
//
#include <CnvVariablesHeaders.h>
//
// Include all Common UEFI Variables Structures definition.
//
#include <CnvVariablesCommon.h>

#pragma pack(push, 1)

//
// Definition for Static Power Limit Configuration (SPLC)
//
#define CNV_UEFI_SPLC_VAR_NAME    L"UefiCnvWlanSPLC"
#define CNV_UEFI_SPLC_REVISION_0  0

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version.
**/
//
// The default Maximum Value per device, OEM is able to reduce the maximum power.
// Device        WiFi Max Power Limit[mW]
// ThP / JfP     2000
// HrP / CcP     2400
// TyP           2880
// GfP2          2660
// GfP4          5200
//
#define SPLC_NIC_CONTROL_NO_LIMIT  0xFFFFFFFF
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;              ///< Revision
  UINT32                 DefaultPowerLimit;   ///< mW
} CNV_UEFI_VAR_SPLC;

//
// Definition for Wireless Regulatory Domain Description (WRDD)
// Shall be kept for validation and test purpose using tools.
//
#define CNV_UEFI_WRDD_VAR_NAME    L"UefiCnvWlanWRDD"
#define CNV_UEFI_WRDD_REVISION_0  0

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version.
**/
//
// Defines the country code applied for this platform as defined in ISO 3166 - 1 A2
// Format Little Endian, Letter A --> Letter B
// https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes
// for Ex CN -  for China,
//              First Letter    = C
//              Second Letter   = N
//              in Hex 0x4e43
// CNV WLAN WRDD Support only China
//
#define MCC_CHINA  0x4e43

typedef struct {
  CNV_UEFI_VAR_HEADER    Header;           ///< Revision
  UINT32                 RegulatoryDomain; ///< Country identifier as defined in ISO/IEC 3166-1 Alpha 2 code. Supported only in China, only if targeted to China Set MCC_CHINA
} CNV_UEFI_VAR_WRDD;

//
// Definition for Wireless Regulatory Domain SAR (WRDS SAR)
//
#define CNV_UEFI_WRDS_VAR_NAME    L"UefiCnvWlanWRDS"
#define CNV_UEFI_WRDS_REVISION_2  2            // 2 - CDB

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version - Legacy.
  <b>Revision 1</b>:  - Add support for Ultra High Band.
  <b>Revision 2</b>:  - Add support for Concurrent Dual Band.
**/
//
// SAR Tx Power Limits - CDB
//
// TX POWER 6E UHB
enum TX_POWER_6E_UHB {
  From2400,        ///< WiFi SAR Tx Power Limit for 2400 MHz
  From5180To5320,  ///< WiFi SAR Tx Power Limit for 5180-5320 MHz
  From5340To5440,  ///< WiFi SAR Tx Power Limit for 5340-5440 MHz
  From5460To5700,  ///< WiFi SAR Tx Power Limit for 5460-5700 MHz
  From5720To5825,  ///< WiFi SAR Tx Power Limit for 5720-5825 MHz
  From5845To6135,  ///< WiFi SAR Tx Power Limit for 5845-6135 MHz
  From6155To6375,  ///< WiFi SAR Tx Power Limit for 6155-6375 MHz
  From6395To6495,  ///< WiFi SAR Tx Power Limit for 6395-6495 MHz
  From6515To6675,  ///< WiFi SAR Tx Power Limit for 6515-6675 MHz
  From6695To6835,  ///< WiFi SAR Tx Power Limit for 6695-6835 MHz
  From6855To7095,  ///< WiFi SAR Tx Power Limit for 6855-7095 MHz
  TX_POWER_LIMITS_UHB_MAX
};

typedef UINT8 TX_POWER_LIMITS_UHB;

typedef struct {
  union {
    struct {
      TX_POWER_LIMITS_UHB    ChainATxPowerLimits[TX_POWER_LIMITS_UHB_MAX];      ///< Chain Function A - UHB
      TX_POWER_LIMITS_UHB    ChainBTxPowerLimits[TX_POWER_LIMITS_UHB_MAX];      ///< Chain Function B - UHB
      TX_POWER_LIMITS_UHB    CdbChainATxPowerLimits[TX_POWER_LIMITS_UHB_MAX];   ///< Chain Function A - UHB in CDB
      TX_POWER_LIMITS_UHB    CdbChainBTxPowerLimits[TX_POWER_LIMITS_UHB_MAX];   ///< Chain Function B - UHB in CDB
    } SAR_VALUES_REV_2; // WRDS/EWRD - WIFI SAR - CDB Revision 2
  } SAR_VALUES_UNION;
} SAR_VALUES;

// Mode options
// Bit 0 Enabled
// Bits 1:31 Reserved shall set to zeros
// The Main structure of Rev 2
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;    ///< Revision
  UINT32                 Mode;      ///< WRDS Modes. Bit 0 Enabled (Value 0 - Disabled/ 1 - Enabled). Bits 1:31 reserved shall be set to zeros
  SAR_VALUES             SarValues; ///< SAR Values
} CNV_UEFI_VAR_WRDS_WIFI_SAR;

//
// Definition for Extended Wireless Regulatory Domain SAR (EWRD)
//
#define CNV_UEFI_EWRD_VAR_NAME    L"UefiCnvWlanEWRD"
#define CNV_UEFI_EWRD_REVISION_2  2                       // 2 - CDB

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version - Legacy.
  <b>Revision 1</b>:  - Add support for Ultra High Band.
  <b>Revision 2</b>:  - Add support for Concurrent Dual Band.
**/

/**
 * SAR Table is defined as follows :
 * 0 - Default OTP Table
 * 1 - WRDS object
 * 2-4 - EWRD Object
 * */
#define CNV_UEFI_EWRD_WIFI_SAR_VALUES_EXT  3
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;                                                     ///< Revision
  UINT32                 WifiDynamicSarMode;                                         ///< WiFi Dynamic SAR Enable/Disable
  UINT32                 WifiDynamicSarRangeSets;                                    ///< WiFi SAR Number of Optional added SAR table sets to be used: (0-3)
  SAR_VALUES             WifiDynamicSarValuesExt[CNV_UEFI_EWRD_WIFI_SAR_VALUES_EXT]; ///< Extended SAR Values (same definition as WRDS)
} CNV_UEFI_VAR_EWRD;

//
// Definition for Wireless Geo Delta Settings (WGDS)
//
#define CNV_UEFI_WGDS_VAR_NAME    L"UefiCnvWlanWGDS"
#define CNV_UEFI_WGDS_REVISION_3  3            // SGOM Minimum sets 3, Optional OEM sets up to 8. Supported Legacy bands and 6e.

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version - FCC, Japan/Europe, Rest of World.
  <b>Revision 1</b>:  - Add Korea support.
  <b>Revision 2</b>:  - Add Ultra High Band support 6E 3 sets.
  <b>Revision 3</b>:  - SGOM Min sets 3, Optional sets up to 8.
**/
#define CNV_UEFI_WGDS_SAR_DELTA_GROUP  8

typedef struct {
  UINT8    MaxAllowed; ///< 2400, 5200 , 6000-7000 MHZ Max Allowed
  UINT8    ChainA;     ///< Chain A Offset
  UINT8    ChainB;     ///< Chain B Offset
} GEO_SAR_OFFSET;

typedef struct {
  GEO_SAR_OFFSET    GeoSarOffset2400Mhz;
  GEO_SAR_OFFSET    GeoSarOffset5200Mhz;
  GEO_SAR_OFFSET    GeoSarOffset6000To7000Mhz;
} GEO_SAR_DELTA;

typedef struct {
  CNV_UEFI_VAR_HEADER    Header;                                            ///< Revision
  UINT8                  NumberOfGroups;                                    ///< The number of groups that supported
  GEO_SAR_DELTA          WifiSarPowerDelta[CNV_UEFI_WGDS_SAR_DELTA_GROUP];  ///< WiFi SAR Output Power Delta
} CNV_UEFI_VAR_WGDS;

//
// Definition for Per Platform Antenna Gain (PPAG)
//
#define CNV_UEFI_PPAG_VAR_NAME  L"UefiCnvWlanPPAG"
#define CNV_UEFI_PPAG_REVISION  3

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version - Legacy.
  <b>Revision 1</b>:  - Add support for Ultra High Band.
  <b>Revision 2</b>:  - Add support for Concurrent Dual Band.
  <b>Revision 3</b>:  - Add support for bit mode from bit 3 to bit 10 in ANT_BITS_MODES
**/
//
// To enable set all the relevant Bits (OR between the relevant bis)
// for Ex to support ETSI and China the value shall be set to 3
// bit-field used to unpack the st_mode in a field in a _stat structure
//
struct ANT_BITS_MODES {
  // Bit wise to revision CNV_UEFI_PPAG_REVISION_REV_3
  UINT32    PPAG_EU           : 1;  ///< Bit  0: Enable PPAG in LB/HB in EU countries
  UINT32    PPAG_CHINA        : 1;  ///< Bit  1: Enable PPAG in LB/HB in China Mainland
  UINT32    PPAG_EU_LPI_UHB   : 1;  ///< Bit  2: Enable PPAG in LPI-UHB in EU countries
  UINT32    PPAG_EU_VLP_UHB   : 1;  ///< Bit  3: Enable PPAG in VLP-UHB in EU countries
  UINT32    PPAG_EU_SP_UHB    : 1;  ///< Bit  4: Enable PPAG in SP-UHB in EU countries
  UINT32    PPAG_FCC_LPI_UHB  : 1;  ///< Bit  5: Enable PPAG in LPI-UHB in FCC countries
  UINT32    PPAG_FCC_VLP_UHB  : 1;  ///< Bit  6: Enable PPAG in VLP-UHB in FCC countries
  UINT32    PPAG_FCC_SP_UHB   : 1;  ///< Bit  7: Enable PPAG in SP-UHB in FCC countries
  UINT32    PPAG_ISED_LPI_UHB : 1;  ///< Bit  8: Enable PPAG in LPI-UHB in ISED countries
  UINT32    PPAG_ISED_VLP_UHB : 1;  ///< Bit  9: Enable PPAG in VLP-UHB in ISED countries
  UINT32    PPAG_ISED_SP_UHB  : 1;  ///< Bit 10: Enable PPAG in SP-UHB in ISED countries
  UINT32    Reserved          : 21; ///< Bit 11-31: resreved
};

typedef union _WIFI_ANT_GAIN_MODES {
  struct ANT_BITS_MODES    BitsModes;         ///< Etsi & China
  UINT32                   PpagModesU32;      ///< Represent content in UINT32
} WIFI_ANT_GAIN_MODES;

typedef struct {
  CNV_UEFI_VAR_HEADER    Header;                                     ///< Revision
  WIFI_ANT_GAIN_MODES    WifiAntGainMode;                            ///< Mode of Antenna Gain Control to be used
                                                                     ///< 00 - PPAG Disabled in ETSI & China, 01 - PPAG Enabled in ETSI & Disabled in China
                                                                     ///< 10 - PPAG Disabled in ETSI & Enabled in China,11 - PPAG Enabled in ETSI & China
  TX_POWER_LIMITS_UHB    AntennaGainChainA[TX_POWER_LIMITS_UHB_MAX]; ///< WiFi Antenna Gain Chain A
  TX_POWER_LIMITS_UHB    AntennaGainChainB[TX_POWER_LIMITS_UHB_MAX]; ///< WiFi Antenna Gain Chain B
} CNV_UEFI_VAR_PPAG;

//
// Definition for WiFi Time Average SAR (WTAS)
//
#define CNV_UEFI_WTAS_VAR_NAME    L"UefiCnvWlanWTAS"
#define CNV_UEFI_WTAS_REVISION_1  1             // 1 - Block Country Lists + More option in the Tas selection

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version - TAS Enable/Disable.
  <b>Revision 1</b>:  - Add IEC, NIC , UHB support.
**/
#define CNV_UEFI_WTAS_BLOCK_COUNTRY_LIST  16
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;                                              ///< Revision
  UINT32                 TASSelection;                                        ///< TAS feature please used Intel BIOS guide lines to specific bits.
  UINT8                  NumOfBlockedEntries;                                 ///< No. of blocked countries not approved by OEM to support this feature
  UINT16                 BlockedListEntry[CNV_UEFI_WTAS_BLOCK_COUNTRY_LIST];  ///< Blocked Country Code [CC1 -  CC16]
} CNV_UEFI_VAR_WTAS;

//
// Definition for WiFi General configuration simlar to legacy DSM
//
// This UEFI variable to 'small' Intel WLAN configurations, such modes on/off...etc.
#define CNV_UEFI_WLAN_GENERAL_CFG_NAME        L"UefiCnvWlanGeneralCfg"
#define CNV_UEFI_WLAN_GENERAL_CFG_REVISION_4  4

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version - ACPI _DSM.
  <b>Revision 1</b>:  - Used by UEFI driver.
  <b>Revision 2</b>:  - Used by UEFI driver.
  <b>Revision 3</b>:  - Used by UEFI driver.
  <b>Revision 4</b>:  - Migration from ACPI to UEFI Variable and consolidation with UEFI driver.
**/
#define CNV_UEFI_WLAN_GENERAL_CFG_MAX_FUNCTIONS  32
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;                                            ///< Revision
  UINT32                 Function[CNV_UEFI_WLAN_GENERAL_CFG_MAX_FUNCTIONS]; ///< Please see Intel CNV BIOS guidelines to specific function. 32bits payload for each function.
} WLAN_GENERAL_CONFIG_SET;

//
// Definition for WiFi OEM PNVM
//
#define CNV_UEFI_WLAN_OEM_PNVM_NAME        L"UefiCnvWlanOemSignedPnvm"
#define CNV_UEFI_WLAN_OEM_PNVM_REVISION_1  1

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version.
  <b>Revision 1</b>:  - Add support for 1-4 SKUs.
**/
#define OEM_MIN_NUM_OF_SKUS  1  //  the Min OEM SKUs
#define OEM_MAX_NUM_OF_SKUS  4  //  the max aggregated OEM SKUs
//
// The SKU Images must sign by Intel
//
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;                      ///< Revision
  UINT32                 TotalPackagesSizeInBytes;    ///< the Total sizeof of IntelSignedPnvmSkuList in Bytes
                                                      ///< 0 mean no SKUs (disable OEM PNVM SKU from BIOS).
  UINT8                  NubmberOfSkusPackages;       ///< The Number of SKUs Packages in
                                                      ///< IntelSignedPnvmSkuList, Max number of Elements as
                                                      ///< defined in OEM_MAX_NUM_OF_SKUS.
                                                      ///< 0 mean No SKUs (disable OEM PNVM SKU from BIOS).
  UINT32                 Reserved;
  UINT32                 Reserved2;
  UINT8                  IntelSignedPnvmSkuList[1];   ///< Payload of Intel signed SKUs.
                                                      ///< Payload zero-length arrays (the Length Part of the Intel Blob)
                                                      ///< In case more than one SKU of data in Continuous memory
                                                      ///< And update the Number Of SKU elements(more than 1).
} WLAN_OEM_PNVM;

//
// Definition for WiFi reduce power
//
// This UEFI variable to OEM PNVM WLAN
#define CNV_UEFI_WLAN_REDUCED_POWER_NAME       L"UefiCnvWlanReducedPower"
#define CNV_UEFI_WLAN_REDUCED_POWER_REVISON_1  1

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version.
  <b>Revision 1</b>:  - Add support for 1-4 SKUs.
**/
#define OEM_MIN_NUM_OF_SKUS  1  //  the Min OEM SKUs
#define OEM_MAX_NUM_OF_SKUS  4  //  the max aggregated OEM SKUs

//
// The OEM shall set the content from Intel Tool as one blob- TBD
//
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;                   ///< Revision
  UINT32                 TotalPackagesSizeInBytes; ///< the Total sizeof of IntelSignedPnvmSkuList in Bytes
                                                   ///< 0 mean no SKUs (disable OEM PNVM SKU from BIOS).
  UINT8                  NubmberOfSkusPackages;    ///< The Number of SKUs Packages in
                                                   ///< IntelSignedPnvmSkuList, Max number of Elements as
                                                   ///< defined in OEM_MAX_NUM_OF_SKUS.
                                                   ///< 0 mean No SKUs (disable OEM PNVM SKU from BIOS).
  UINT32                 Reserved;
  UINT32                 Reserved2;
  UINT8                  ReducedPowerPayload[1];  ///< Payload of the SKUs over Intel certified NIC
                                                  ///< Payload zero-length arrays (the Length Part of the Intel Blob)
                                                  ///< In case more than one SKU data in Continuous memory
                                                  ///< And update the Number Of SKU elements(more than 1).
} WLAN_TX_REDUCED_POWER;

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version.
  <b>Revision 1</b>:  - Add support for 1-4 SKUs.
**/
#define MAX_SUPPORTED_COUNTRIES  15

// The country code is based on two ASCII letters
// Country code based on https://en.wikipedia.org/wiki/ISO_3166-2
// the format
//   Memory little-endian
//   ASCII in capital letters
//   The First Letter in the first Byte and the Second letter in the Second Byte
// For example
//      Germany defined as 'DE' shall set in hex 0x4544
//      France defined as 'FR' shall set in hex 0x5246
//      All Europe without limitation shall be set to 'EU'  in hex '0x5545'

//
// This UEFI variable to SAR GEO Offset Mapping (SGOM)
//
#define CNV_UEFI_WLAN_SGOM_NAME        L"UefiCnvWlanSarGeoOffsetMapping"
#define CNV_UEFI_WLAN_SGOM_REVISION_1  1

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version.
  <b>Revision 1</b>:  - WGDS Revision supports 3.
**/

typedef struct _VERTICAL_LETTERS_COUNTRIES {
  UINT8    Country_A : 4;
  UINT8    Country_B : 4;
  UINT8    Country_C : 4;
  UINT8    Country_D : 4;
  UINT8    Country_E : 4;
  UINT8    Country_F : 4;
  UINT8    Country_G : 4;
  UINT8    Country_H : 4;
  UINT8    Country_I : 4;
  UINT8    Country_J : 4;
  UINT8    Country_K : 4;
  UINT8    Country_L : 4;
  UINT8    Country_M : 4;
  UINT8    Country_N : 4;
  UINT8    Country_O : 4;
  UINT8    Country_P : 4;
  UINT8    Country_Q : 4;
  UINT8    Country_R : 4;
  UINT8    Country_S : 4;
  UINT8    Country_T : 4;
  UINT8    Country_U : 4;
  UINT8    Country_V : 4;
  UINT8    Country_W : 4;
  UINT8    Country_X : 4;
  UINT8    Country_Y : 4;
  UINT8    Country_Z : 4;
} VERTICAL_LETTERS_COUNTRIES;

enum HORIZONTAL_COUNTRIES_LETTERS {
  HORIZONTAL_COUNTRIES_LETTER_A = 0,
  HORIZONTAL_COUNTRIES_LETTER_B,
  HORIZONTAL_COUNTRIES_LETTER_C,
  HORIZONTAL_COUNTRIES_LETTER_D,
  HORIZONTAL_COUNTRIES_LETTER_E,
  HORIZONTAL_COUNTRIES_LETTER_F,
  HORIZONTAL_COUNTRIES_LETTER_G,
  HORIZONTAL_COUNTRIES_LETTER_H,
  HORIZONTAL_COUNTRIES_LETTER_I,
  HORIZONTAL_COUNTRIES_LETTER_J,
  HORIZONTAL_COUNTRIES_LETTER_K,
  HORIZONTAL_COUNTRIES_LETTER_L,
  HORIZONTAL_COUNTRIES_LETTER_M,
  HORIZONTAL_COUNTRIES_LETTER_N,
  HORIZONTAL_COUNTRIES_LETTER_O,
  HORIZONTAL_COUNTRIES_LETTER_P,
  HORIZONTAL_COUNTRIES_LETTER_Q,
  HORIZONTAL_COUNTRIES_LETTER_R,
  HORIZONTAL_COUNTRIES_LETTER_S,
  HORIZONTAL_COUNTRIES_LETTER_T,
  HORIZONTAL_COUNTRIES_LETTER_U,
  HORIZONTAL_COUNTRIES_LETTER_V,
  HORIZONTAL_COUNTRIES_LETTER_W,
  HORIZONTAL_COUNTRIES_LETTER_X,
  HORIZONTAL_COUNTRIES_LETTER_Y,
  HORIZONTAL_COUNTRIES_LETTER_Z,
  HORIZONTAL_COUNTRIES_LETTER_MAX,
};

#define VERTICAL_COUNTRIES_LETTER_MAX  HORIZONTAL_COUNTRIES_LETTER_MAX

//
// Country code based on https://en.wikipedia.org/wiki/ISO_3166-2
// Each country defined two Letters [Horizontal][Vertical]
// Note: Some of the combinations are not relevant, and not mapped to countries such AA,AB,AC,AH....
// For Ex
// United States of America defined as 'US'
// U - Horizontal
// S - Vertical
// the Value '1' - point to WGDS set 1
//
// Japan defined as JP
// J -Horizontal
// P -Vertical
// the Value '2'point to WGDS set 2
//
// Israel defined as IL
// I -Horizontal
// L -Vertical
// the Value '0' Used the NIC/HW/FW definitions.

/*
    A   B   C   D   E   F   G   H   I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z
A   0   0   0   0   2   0   2   0   0   0   0   0   0   0   0   0   0   0   0   2   2   0   1   0   0   2
B   2   1   0   1   2   0   2   2   0   0   0   0   1   0   1   0   0   1   1   0   0   0   0   0   0   0
C   0   2   0   0   0   0   0   2   2   0   0   1   0   0   1   0   0   1   0   0   0   0   0   0   2   2
D   0   0   0   0   2   0   0   0   0   0   2   0   0   0   1   0   0   0   0   0   0   0   0   0   0   2
E   0   0   1   0   2   0   1   0   0   0   0   0   0   0   0   0   0   0   2   0   2   0   0   0   0   0
F   0   0   0   0   0   0   0   0   2   0   0   0   0   0   0   0   0   2   0   0   0   0   0   0   0   0
G   0   0   0   2   0   0   0   2   0   0   0   0   0   0   0   0   0   2   0   0   0   0   0   0   0   0
H   0   0   0   0   0   0   0   0   0   0   1   0   0   1   0   0   0   2   0   1   2   0   0   0   0   0
I   0   0   0   1   2   0   0   0   0   0   0   0   0   2   0   0   0   0   2   2   0   0   0   0   0   0
J   0   0   0   0   0   0   0   0   0   0   0   0   1   0   2   2   0   0   0   0   0   0   0   0   0   0
K   0   0   0   0   2   0   0   1   0   0   0   0   0   0   0   0   0   3   0   0   0   0   2   0   0   0
L   0   2   0   0   0   0   0   0   2   0   0   0   0   0   0   0   0   0   0   2   2   2   0   0   0   0
M   2   0   0   2   2   0   0   0   0   0   2   0   0   0   0   0   0   0   0   2   2   0   2   1   2   0
N   0   0   0   0   0   0   2   0   1   0   0   2   0   0   2   1   0   0   0   0   0   0   0   0   0   2
O   0   0   0   0   0   0   0   0   0   0   0   0   2   0   0   0   0   0   0   0   0   0   0   0   0   0
P   1   0   0   0   1   0   1   2   0   0   1   2   0   0   0   0   0   0   0   2   0   0   0   0   1   0
Q   2   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
R   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   2   0   2   0   2   0   0   0
S   2   0   0   0   0   0   1   0   2   0   2   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
T   0   0   0   0   0   0   0   2   0   0   0   0   0   2   0   0   0   2   0   1   0   0   1   0   0   1
U   2   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   1   0   0   0   0   0   1   0
V   0   0   0   0   1   0   0   0   0   0   0   0   0   2   0   0   0   0   0   0   0   0   0   0   0   0
W   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
X   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
Y   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
*/
typedef union {
  VERTICAL_LETTERS_COUNTRIES    HorizontalArray[HORIZONTAL_COUNTRIES_LETTER_MAX]; ///< Array of 26 HorizontalArray structure that contains the 26 vertical letters , each country size 4 bits.
  UINT8                         U8Raw[338];                                       ///< array raw unsigned char(one byte), Each Byte represents two countries each 4 bits
                                                                                  ///< started Vertical and then move Horzintal, Started from AA Till ZZ,
                                                                                  ///< started Vertical and then move Horizontal, Starting from AA Till ZZ, for example, AA BITS 0-3 AB BITS 4-7 AC 8 -11 ..etc.
} SGOM_COUNTRIES_VER1;

#define SGOM_COUNTRIES  SGOM_COUNTRIES_VER1
//
// UefiCnvWlanSarGeoOffsetMapping  Main Data Structure
//
typedef struct _CNV_UEFI_WLAN_SGOM_DATA {
  CNV_UEFI_VAR_HEADER    Header;        ///< Set CNV_UEFI_WLAN_SGOM_REV
  SGOM_COUNTRIES         CountriesSarOffsetMap;
} CNV_UEFI_WLAN_SGOM_DATA;

//
// Definition for WiFi Phy Filter Configuration (WPFC)
//
#define UEFI_CNV_WPFC_VAR_NAME    L"WPFC"
#define UEFI_CNV_WPFC_REVISION_0  0

typedef struct {
  UINT32    WifiPhyFilterConfigA;
  UINT32    WifiPhyFilterConfigB;
  UINT32    WifiPhyFilterConfigC;
  UINT32    WifiPhyFilterConfigD;
} WIFI_PHY_FILTER_CONFIG;

typedef struct {
  CNV_UEFI_VAR_HEADER       Header; // Revision
  WIFI_PHY_FILTER_CONFIG    Wpfc;
} UEFI_CNV_VAR_WPFC;

//
// This UEFI variable for MPCC
//
#define CNV_UEFI_WLAN_MPCC_NAME  L"UefiCnvWlanMPCC"
#define CNV_UEFI_WLAN_MPCC_REV   1
//
// UefiCnvWlanMpccMapping Main Data Structure
//
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;
  UINT16                 M21aAssembled;
  UINT16                 WifiPlatCurrentConsumption;
} WLAN_MPCC_CONFIG_SET;

//
// This UEFI variable for UATS (UHB AP Type Support)
//
#define CNV_UEFI_WLAN_UATS_NAME   L"CnvUefiWlanUATS"
#define CNV_UEFI_WLAN_UATS_REV_1  1
//
// CnvUefiWlanUATS Main Data Structure
//
typedef union {
  VERTICAL_LETTERS_COUNTRIES    HorizontalArray[HORIZONTAL_COUNTRIES_LETTER_MAX]; ///< Array of 26 HorizontalArray structure that contains the 26 vertical letters, each country size 4 bits.
  UINT8                         U8Raw[338];                                       ///< array raw unsigned char (one byte), Each Byte represents two countries each 4 bits.
                                                                                  ///< started Vertical and then move Horzintal, Started from AA Till ZZ,
                                                                                  ///< started Vertical and then move Horizontal, Starting from AA Till ZZ, for example, AA BITS 0-3 AB BITS 4-7 AC 8-11 ..etc.
} UHB_AP_TYPE_COUNTRIES_VER1;

#define UHB_AP_TYPE_COUNTRIES  UHB_AP_TYPE_COUNTRIES_VER1

typedef struct _CNV_UEFI_WLAN_UATS_DATA {
  CNV_UEFI_VAR_HEADER      Header;                        ///< Revision
  UHB_AP_TYPE_COUNTRIES    CountriesUhbApTypeSupportMap;  ///< COUNTRIES structure that contains the country codes based on the MCC, two-dimensional array 26 x 26
} CNV_UEFI_WLAN_UATS_DATA;

//
// Definition for Puncturing (UefiCnvWlanPuncturing)
//
#define UEFI_CNV_PUNC_VAR_NAME  L"UefiCnvWlanPuncturing"
#define UEFI_CNV_PUNC_REVISION  0

//
//  This UEFI variable helps OEM to enable puncturing in USA and Canada when they are certified for it.
//  bit0: PUNC_USA
//  bit1: PUNC_CANADA
//
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;
  UINT32                 Puncturing;
} CNV_UEFI_VAR_WIFI_PUNCTURING;

//
// Definition for WLAN Sensing feature
//
#define UEFI_CNV_WLAN_SENSING_NAME     L"UefiCnvWlanSensing"
#define UEFI_CNV_WLAN_SENSIN_REVISION  0

typedef struct {
  CNV_UEFI_VAR_HEADER    Header;
  UINT8                  SensingSettings;
  UINT8                  SensingConfiguration;
  UINT8                  NumOfPreferredChannels;
  UINT32                 PreferredChannels[16];
} UEFI_CNV_VAR_WIFI_SENSING;

//
// This UEFI variable for WBEM (Wi-Fi 320MHz Bandwidth Enablement per MCC)
//
#define CNV_UEFI_WLAN_WBEM_NAME    L"UefiCnvWlanWBEM"
#define CNV_UEFI_WLAN_WBEM_REV     0

//
// UefiCnvWlanWBEM Main Data Structure
// WiFi320MHzPerMcc – enablement of Wi-Fi 320MHz per MCC. Default is 0x0.
//                    Bit 0 - if set, Wi-Fi 320MHz is enabled for Japan if allowed at module level.
//                    Bit 1 - if set, Wi-Fi 320MHz is enabled for South Korea if allowed at module level.
//                    Bits 2-31 – Reserved (shall be set to 0).
//
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;
  UINT32                 WiFi320MHzPerMcc;
} CNV_UEFI_VAR_WIFI_320MHz_PER_MCC;

#pragma pack(pop)

#endif //__CNV_VARIABLES_WLAN_H__
