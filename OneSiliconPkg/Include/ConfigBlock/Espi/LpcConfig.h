/** @file
  Lpc policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _LPC_CONFIG_H_
#define _LPC_CONFIG_H_

#define PCH_LPC_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gLpcPreMemConfigGuid;

#pragma pack (push,1)

/**
  This structure contains the policies which are related to LPC.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    Enhance the port 8xh decoding.
    Original LPC only decodes one byte of port 80h, with this enhancement LPC can decode word or dword of port 80h-83h.
    @note: this will occupy one LPC generic IO range register. While this is enabled, read from port 80h always return 0x00.
    0: Disable, <b>1: Enable</b>
  **/
  UINT32    EnhancePort8xhDecoding      :  1;
  /**
   Hardware Autonomous Enable.
   When enabled, LPC will automatically engage power gating when it has reached its idle condition.
   0: Disable, <b>1: Enable</b>
  **/
  UINT32    LpcPmHAE                    :  1;
  UINT32    RsvdBits                    : 30;     ///< Reserved bits
} PCH_LPC_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _LPC_CONFIG_H_
