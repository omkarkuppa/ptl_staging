/** @file
  IMR_CONFIG HOB definition.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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


#ifndef _IMR_CONFIG_HOB_
#define _IMR_CONFIG_HOB_

extern EFI_GUID gImrConfigGuid;

#pragma pack(1)
typedef struct {
  UINT16            PcieImrSize;
  UINT32            BiosImrDisableMask0;
  UINT32            BiosImrDisableMask1;
  UINT8             ResizableBarSupport; ///< Resizable BAR Support
} IMR_CONFIG_HOB;
#pragma pack()

#endif
