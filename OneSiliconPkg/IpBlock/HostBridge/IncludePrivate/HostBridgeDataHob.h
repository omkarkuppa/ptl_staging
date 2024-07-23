/** @file
  The GUID definition for Host Bridge Data Hob

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _HOST_BRIDGE_DATA_HOB_H_
#define _HOST_BRIDGE_DATA_HOB_H_

#include <Base.h>

extern EFI_GUID gHostBridgeDataHobGuid;
#pragma pack (push,1)

///
/// Host Bridge Data Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;                       ///< GUID Hob type structure for gHostBridgeDataHobGuid
  UINT8                    EnableAbove4GBMmio;                   ///< 0=Disable above 4GB MMIO resource support, 1=Enable above 4GB MMIO resource support
  BOOLEAN                  SkipPamLock;                          ///< 0=All PAM registers will be locked in System Agent code, 1=Do not lock PAM registers in System Agent code.
  BOOLEAN                  CridEnable;                           ///< This field inidicates if CRID is enabled or disabled (to support Intel(R) SIPP)
  UINT8                    Rsvd1[2];                             ///< Reserved for future use
} HOST_BRIDGE_DATA_HOB;
#pragma pack (pop)
#endif
