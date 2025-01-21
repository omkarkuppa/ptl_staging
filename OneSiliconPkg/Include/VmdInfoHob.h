/** @file
  This file contains information about VMD assigned devices.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _VMD_INFO_HOB_H_
#define _VMD_INFO_HOB_H_

#include <Library/VmdInfoLib.h>

extern EFI_GUID gVmdInfoHobGuid;

#pragma pack (1)

/**
  PCIe device detection information.
  This information will be used to display BIOS setup option to users.
**/

typedef struct {
  UINT8         DeviceDetected;
  UINT16        DevId;
  UINT8         RpBus;
  UINT8         RpDev;
  UINT8         RpFunc;
  UINT8         PortEn;
  UINT8         RpIndex;
} PORT_INFO;

//
// VMD Port Info
//
typedef struct {
  PORT_INFO PortInfo[VMD_MAX_DEVICES];
} VMD_PORT_INFO;

//
// VMD Info Hob
//
typedef struct {
  EFI_HOB_GUID_TYPE      EfiHobGuidType;
  VMD_PORT_INFO          VmdPortInfo;
} VMD_INFO_HOB;

#pragma pack ()


#endif
