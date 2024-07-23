/** @file
  Private data structures

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#ifndef _DXE_FIRMWARE_VERSION_INFO_LIB_H_
#define _DXE_FIRMWARE_VERSION_INFO_LIB_H_

#define PLATFORM_FVI_STATUS_DISABLED "Disabled"
#define PLATFORM_FVI_STATUS_ENABLED  "Enabled "

#define PLATFORM_FVI_SETUP_NAME      L"PlatformFviSetupDataVar"

//
// Ensure proper structure formats
//
#pragma pack(1)

typedef struct {
  UINT8  LanPhyVersion;
  UINT16 IccMajorVersion;
  UINT16 IccMinorVersion;
  UINT16 IccHotfixVersion;
  UINT16 IccBuildVersion;
} PLATFORM_FVI_SETUP;

#pragma pack()


#define SENSOR_FW_FVI_STRING  "Sensor Firmware Version"
#define SENSOR_FW_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define LAN_PHY_FVI_STRING  "Lan Phy Version"
#define LAN_PHY_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define DEBUG_MODE_STATUS  "Debug Mode Status"
#define DEBUG_MODE_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define PERFORMANCE_MODE_STATUS  "Performance Mode Status"
#define PERFORMANCE_MODE_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define DEBUG_USE_USB_STATUS  "Debug Use USB(Disabled:Serial)"
#define DEBUG_USE_USB_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define ICC_OVERLOCKING_FVI_STRING  "ICC Overclocking Version"
#define ICC_OVERLOCKING_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define UNDI_FVI_STRING  "UNDI Version"
#define UNDI_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define EC_FVI_STRING  "EC FW Version"
#define EC_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define GOP_FVI_STRING  "GOP Version"
#define GOP_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define ROYALPARK_FVI_STRING  "Royal Park Version"
#define ROYALPARK_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define PLATFORM_FVI_STRING  "Platform Version"
#define PLATFORM_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

enum {
  LAN_PHY_VER = 0,
  SENSOR_FW_VER,
  DEBUG_MODE_VER,
  PERFORMANCE_MODE_VER,
  DEBUB_USE_USB_VER,
  ICC_OVERLOCKING_VER,
  UNDI_VER,
  EC_VER,
  GOP_VER,
  ROYALPARK_VER,
  PLATFORM_VER,
  USBC_PD0_VER,
  USBC_PD1_VER,
  USBC_RETIMER0_VER,
  USBC_RETIMER1_VER,
  USBC_RETIMER2_VER,
} PLATFORM_FVI_INDEX;

#define USBC_PD0_FVI_STRING  "USBC PD0 Version"
#define USBC_PD0_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define USBC_PD1_FVI_STRING  "USBC PD1 Version"
#define USBC_PD1_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define USBC_RETIMER0_FVI_STRING  "USBC Retimer0 Version"
#define USBC_RETIMER0_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define USBC_RETIMER1_FVI_STRING  "USBC Retimer1 Version"
#define USBC_RETIMER1_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define USBC_RETIMER2_FVI_STRING  "USBC Retimer2 Version"
#define USBC_RETIMER2_VERSION \
  { \
    0xFF, 0xFF, 0xFF, 0xFFFF \
  }

#define SENSOR_HUB_FW_VERSION_OFFSET 0x1000
#define SENSOR_HUB_FW_VERSION_LENGTH 0x04

//
// LAN PHY Revision definitions
//
#define LAN_PHY_REV_TC 0xA0
#define LAN_PHY_REV_A0 0xA1
#define LAN_PHY_REV_A1 0xA2
#define LAN_PHY_REV_A2 0xA3
#define LAN_PHY_REV_A3 0xA4
#define LAN_PHY_REV_B1 0xA5


#endif
