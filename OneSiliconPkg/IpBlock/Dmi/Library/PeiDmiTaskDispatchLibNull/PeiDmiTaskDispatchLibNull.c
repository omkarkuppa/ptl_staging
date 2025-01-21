/**@file
  PEI DMI Task Dispatch library NULL.

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

#include <Library/PeiDmiTaskDispatchLib.h>

/**
  Covert the current EFI_BOOT_MODE to DMI_BOOT_MODE

  @retval  DMI_BOOT_MODE                Mode to be used to execute corresponding DMI tasks.
**/
DMI_BOOT_MODE
DmiGetBootMode (
  VOID
  )
{
  return DMI_NULL;
}

/**
  DmiTaskDispatch: Dispatch the DMI tasks according to IP_DMI3_FEAT_ID_DISPATCH_ENTRY

  @param[in] IP_DMI3_FEAT_ID_DISPATCH_ENTRY       DmiCallTable
  @param[in] VOID                       PeiDmiConfig
**/
VOID
EFIAPI
DmiTaskDispatch (
  IN IP_DMI3_FEAT_ID_DISPATCH_ENTRY     *DmiCallTable,
  IN VOID                               *PeiDmiConfig
)
{
}