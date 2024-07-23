/** @file
  This file contains functions to access BPM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <Uefi.h>
#include <BpmDef.h>

#include <Library/DebugLib.h>

/**
  Determines the Firmware Interface Table (FIT) base address.

  @param[out] FitBase         The FIT base address.

  @retval       EFI_SUCCESS       The operation completed successfully.
  @retval       EFI_NOT_FOUND     The gNemMapStructureHobGuid was not found
  @retval       EFI_NOT_FOUND     The FIT table could not be found (or is no longer available)
  @retval       EFI_DEVICE_ERROR  The GUID in the NEM map structure is invalid.
**/
EFI_STATUS
EFIAPI
GetFitBase (
  OUT UINTN    *FitBase
  )
{
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry;
  UINT64                          FitTableOffset;

  FitTableOffset = *(UINT64 *) (UINTN) (BASE_4GB - 0x40);
  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *) (UINTN) FitTableOffset;
  if (FitEntry[0].Address != *(UINT64 *) "_FIT_   ") {
    return EFI_NOT_FOUND;
  } else {
    *FitBase = (UINTN) ((VOID *) FitEntry);
  }
  return EFI_SUCCESS;
}

/**
  Find FIT Entry address data by type

  @param[in] Type   FIT Entry type

  @return FIT entry address data

**/
VOID *
FindFitEntryData (
  IN UINT8                        Type
  )
{
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry;
  UINT32                          EntryNum;
  UINT8                           BitField1;
  UINT16                          BitField2;
  UINT32                          Index;
  UINTN                           FitBaseAddress;
  EFI_STATUS                      Status;

  Status = GetFitBase (&FitBaseAddress);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return NULL;
  }
  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *) FitBaseAddress;
  if (FitEntry[0].Address != *(UINT64 *)"_FIT_   ") {
    DEBUG ((DEBUG_ERROR, "\tFIT signature was not found or is incorrect\n"));
    return NULL;
  }
  if (FitEntry[0].Type != FIT_TYPE_00_HEADER) {
    DEBUG ((DEBUG_ERROR, "\tFIT header type is incorrect\n"));
    return NULL;
  }
  BitField1 = *(UINT8 *)(&FitEntry[0].Size[0]);
  BitField2 = *(UINT16 *)(&FitEntry[0].Size[1]);
  EntryNum = (((UINT32)(BitField1)) | ((UINT32)(BitField2) << 8)) & 0xFFFFFF;
  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntry[Index].Type == Type) {
      DEBUG ((DEBUG_INFO, "\tFitEntry[%d]\t\t\t= 0x%08x\n", Index, &FitEntry[Index]));
      DEBUG ((DEBUG_INFO, "\tFitEntry[%d].Type= 0x%02x\n", Index, FitEntry[Index].Type));
      return &FitEntry[Index];
    }
  }
  DEBUG ((DEBUG_ERROR, "\tFIT entry type 0x%02x was not found\n", Type));

  return NULL;
}

/**
  Find BPM address

  @return BPM address

**/
VOID *
FindBpm (
  VOID
  )
{
  return FindFitEntryData (FIT_TYPE_0C_BOOT_POLICY_MANIFEST);
}

/**
  Find FBM address

  @return FBM address

**/
VOID *
FindFbm (
  VOID
  )
{
  return FindFitEntryData (FIT_TYPE_0D_FSP_BOOT_MANIFEST);
}