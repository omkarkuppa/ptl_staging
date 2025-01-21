/** @file
  This file defines the CNVi CONFIG HOB

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
#ifndef _CNVI_CONFIG_HOB_H_
#define _CNVI_CONFIG_HOB_H_

#include <Base.h>

extern EFI_GUID gCnviConfigHobGuid;
#pragma pack (push,1)

/**
  This HOB is used to pass CNVi related private information to DXE phase
**/
typedef struct {
  EFI_HOB_GUID_TYPE EfiHobGuidType; ///< GUID HOB type structure for gCnviConfigHobGuid
  UINT32 WifiCore                :  1; ///< 0: Disabled, <b>1: Enabled</b>
  UINT32 BtCore                  :  1; ///< 0: Disabled, <b>1: Enabled</b>
  UINT32 BtInterface             :  2; ///< 0: UART, <b>1: USB</b>, 2: PCI
  UINT32 BtAudioOffload          :  1; ///< <b>0: Disabled</b>, 1: Enabled
  UINT32 BtAudioOffloadInterface :  1; ///< <b>0: I2S</b>, 1: SoundWire
  UINT32 DdrRfim                 :  1; ///< 0: Disabled, <b>1: Enabled</b>
  UINT32 RsvdBits0               : 25; ///< Reserved bits
} CNVI_CONFIG_HOB;
#pragma pack (pop)

#endif // _CNVI_CONFIG_HOB_H_
