/** @file
  Header file for Reference code Firmware Version Info Init Lib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _SI_FVI_H_
#define _SI_FVI_H_

#include <IndustryStandard/SmBios.h>

///
/// Non-static SMBIOS table data to be filled later with a dynamically generated value
///
#define TO_BE_FILLED  0
#define TO_BE_FILLED_STRING  " "        ///< Initial value should not be NULL

///
/// String references in SMBIOS tables. This eliminates the need for pointers.
/// See the DMTF SMBIOS Specification v2.7.1, section 6.1.3.
///
#define NO_STRING_AVAILABLE  0
#define STRING_1  1
#define STRING_2  2
#define STRING_3  3
#define STRING_4  4
#define STRING_5  5
#define STRING_6  6
#define STRING_7  7

#define FVI_NUMBER_OF_STRINGS  2

#pragma pack(1)

///
/// The string number for ComponentName and VersionString is always calculated dynamically.
/// The initial value is ignored and should always be TO_BE_FILLED.
///
#define DEFAULT_FVI_HEADER_DATA(Data) \
{ \
  { \
    TO_BE_FILLED, \
    sizeof(FVI_HEADER) + sizeof (Data), \
    0, \
  }, \
  sizeof (Data) / sizeof (FVI_DATA) \
}

#define DEFAULT_FVI_DATA() \
{ \
  TO_BE_FILLED, \
  TO_BE_FILLED, \
  { \
    TO_BE_FILLED, \
    TO_BE_FILLED, \
    TO_BE_FILLED, \
    TO_BE_FILLED, \
  } \
}

#define FVI_VERSION(Version) \
{ \
  TO_BE_FILLED, \
  TO_BE_FILLED, \
  Version \
}

#define DEFAULT_FVI_STRINGS(Name) \
{ \
  Name ## _FVI_STRING, \
  NULL \
}

#define DEFAULT_FVI_VERSION \
{ \
  0xFF, 0xFF, 0xFF, 0xFFFF \
}

///
/// This structure contains the RC version details for FVI SMBIOS records.
///
typedef struct {
  UINT8   MajorVersion;
  UINT8   MinorVersion;
  UINT8   Revision;
  UINT16  BuildNum;
} RC_VERSION;

typedef struct {
  SMBIOS_STRUCTURE  SmbiosHeader;
  UINT8             Count;  ///< Number of (FVI_DATA) data blocks included
} FVI_HEADER;

///
/// The string number for ComponentName and VersionString is always calculated dynamically.
/// The initial value is ignored and should always be TO_BE_FILLED.
///
typedef struct {
  UINT8       ComponentName;
  UINT8       VersionString;
  RC_VERSION  Version;
} FVI_DATA;

typedef struct {
  CHAR8       *ComponentName;
  CHAR8       *VersionString;
} FVI_STRINGS;

#pragma pack()

#define RC_FVI_STRING               "Reference Code Version"
#define RC_FVI_DEFAULT_STRING       "FFFF.FFFF.FFFF.FFFF"

///
/// CPU Data definitions
///
#define UCODE_FVI_STRING            "uCode Version"
#define TXT_FVI_STRING              "TXT ACM version"

typedef enum {
  UCODE_VER = 0,
  TXT_VER
} CPU_FVI_INDEX;

///
/// ME Data definitions
///
#define ME_FW_FVI_STRING            "ME Firmware Version"
#define ME_FW_CONSUMER_SKU_STRING   "Consumer SKU"
#define ME_FW_CORPORATE_SKU_STRING  "Corporate SKU"
#define ME_FW_LITE_SKU_STRING       "Lite SKU"
#define SSE_FVI_STRING              "SSE Version"
#define SSE_FVI_DEFAULT_STRING      "FFFF.FFFF.FFFF.FFFF"

typedef enum {
  EnumMeFw = 0,     ///< ME FW Version
  EnumMeSse
} ME_FVI_INDEX;

///
/// PCH Data definitions
///
#define PCH_CRID_STATUS_STRING      "PCH-CRID Status"
#define PCH_CRID_ORIGINAL_VALUE     "PCH-CRID Original Value"
#define PCH_CRID_NEW_VALUE          "PCH-CRID New Value"
#define PCH_CRID_ENABLED            "Enabled "
#define PCH_CRID_DISABLED           "Disabled"
#define PCH_HSIO_STRING             "PCH Hsio Version"
#define RAID_FVI_STRING             "OPROM - RST - RAID"
#define RST_DRIVER_NAME1            L"Intel RST"
#define RST_DRIVER_NAME2            L"Intel(R) RST"

#define FVI_PCH_ICL_LP_AX_HSIO_VERSION \
  { \
    0x4, 0, 0, 0 \
  }

typedef enum {
  PCH_CRID_STATUS = 0,
  PCH_CRID_ORIGINAL,
  PCH_CRID_NEW,
  RAID_VER,
  HSIO_PCHHAX_VER,
  HSIO_PCHLPAX_VER
} PCH_FVI_INDEX;

///
/// SA Data definitions
///
#define SA_CRID_STATUS              "SA-CRID Status"
#define SA_CRID_ORIGINAL_VALUE      "SA-CRID Original Value"
#define SA_CRID_NEW_VALUE           "SA-CRID New Value"
#define SA_CRID_ENABLED             "Enabled "
#define SA_CRID_DISABLED            "Disabled"
#define VBIOS_FVI_STRING            "OPROM - VBIOS"
#define IOM_FVI_STRING              "IO Manageability Engine FW Version"
#define PHY_FVI_STRING              "PHY Build Version"
#define TBT_FVI_STRING              "Thunderbolt(TM) FW Version"
#define SAM_FVI_STRING              "System Agent Manageability Engine FW Version"

#define CRID_DATA                   0x69
#define CRID_LOCK                   0x17

typedef enum {
  CRID_STATUS = 0,
  CRID_ORIGINAL,
  CRID_NEW,
  VBIOS_VER,
  IOM_FW_VER,
  PHY_VER,
  TBT_FW_VER,
  SAM_FW_VER
} SA_FVI_INDEX;

#endif
