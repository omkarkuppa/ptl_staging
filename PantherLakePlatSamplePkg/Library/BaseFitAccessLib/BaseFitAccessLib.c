/** @file
  This file contains functions for accessing FIT and its entries

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/
#include <Uefi.h>
#include <Library/BaseFitAccessLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>

/**
  Find FIT Entry address data by type

  @param[in] Type   FIT Entry type

  @return FIT entry address data
**/
VOID *
FindFitEntryData (
  IN UINT8   Type
  )
{
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry;
  UINT32                          EntryNum;
  UINT64                          FitTableOffset;
  UINT32                          Index;

  FitTableOffset = *(UINT64 *)(UINTN)(BASE_4GB - 0x40);
  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)(UINTN)FitTableOffset;
  if ((FitEntry == NULL) || (FitEntry[0].Address != *(UINT64 *)"_FIT_   ")) {
    return NULL;
  }
  if (FitEntry[0].Type != FIT_TYPE_00_HEADER) {
    return NULL;
  }
  EntryNum = *(UINT32 *)(&FitEntry[0].Size[0]) & 0xFFFFFF;
  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntry[Index].Type == Type) {
      return (VOID *)(UINTN)FitEntry[Index].Address;
    }
  }

  return NULL;
}