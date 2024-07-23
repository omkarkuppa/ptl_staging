/** @file
  RTC policy

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
#ifndef _RTC_CONFIG_H_
#define _RTC_CONFIG_H_

#include <ConfigBlock.h>

#define RTC_CONFIG_REVISION 1
extern EFI_GUID gRtcConfigGuid;

#pragma pack (push,1)

/**
  The RTC_CONFIG block describes the expected configuration of RTC configuration.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    When set, prevents RTC TS (BUC.TS) from being changed.
    This BILD bit has different function compared to LPC/eSPI, SPI.
    0: Disabled; <b>1: Enabled</b>
  **/
  UINT32  BiosInterfaceLock       :  1;
  /**
    When set, bytes 38h-3Fh in the upper 128bytes bank of RTC RAM are locked
    and cannot be accessed.
    Writes will be droipped and reads will not return any guaranteed data.
    0: Disabled; <b>1: Enabled</b>
  **/
  UINT32  MemoryLock              :  1;
  UINT32  RsvdBits0               : 30;
} RTC_CONFIG;

#pragma pack (pop)

#endif // _RTC_CONFIG_H_
