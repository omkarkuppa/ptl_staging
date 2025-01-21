/**@file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#ifndef _AMT_PET_ALERT_H_
#define _AMT_PET_ALERT_H_

///
/// BAE device type defines
///
#define BAE_NETWORK_DEVICE    0x1
#define BAE_HDD_DEVICE        0x2
#define BAE_REMOVABLE_DEVICE  0x3
#define BAE_EMPTY_QUEUE       0x7F

#define NVME_GENERIC_TIMEOUT            EFI_TIMER_PERIOD_SECONDS (5)
#define NVME_CONTROLLER_ID              0
#define NVME_ALL_NAMESPACES             0xFFFFFFFF
#define NVME_IDENTIFY_CONTROLLER_STRUCT 1

///
/// ASF Event Data Codes for Boot Audit Entry Events
///
#define ASF_BAE_EVENT_DATA1                       0x40
#define ASF_BAE_EVENT_DATA1_EVENT_DATA_SET_BY_OEM 0xAA

#pragma pack(push,1)

typedef struct {
  UINT8 BootQueue[3];
  UINT8 BootHarddriveTag[64];
} PET_ALERT_CFG;

#pragma pack(pop)

#endif
