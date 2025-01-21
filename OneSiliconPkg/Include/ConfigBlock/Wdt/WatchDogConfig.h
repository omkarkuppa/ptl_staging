/** @file
  WatchDog policy

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
#ifndef _WATCH_DOG_CONFIG_H_
#define _WATCH_DOG_CONFIG_H_

#define PCH_WDT_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gWatchDogPreMemConfigGuid;

#pragma pack (push,1)

/**
  This policy clears status bits and disable watchdog, then lock the
  WDT registers.
  while WDT is designed to be disabled and locked by Policy,
  bios should not enable WDT by WDT PPI. In such case, bios shows the
  warning message but not disable and lock WDT register to make sure
  WDT event trigger correctly.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  UINT32    DisableAndLock    :  1;     ///< <b>(Test)</b> Set 1 to clear WDT status, then disable and lock WDT registers. <b>0: Disable</b>; 1: Enable.
  UINT32    RsvdBits          : 31;
} PCH_WDT_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _WATCH_DOG_CONFIG_H_
