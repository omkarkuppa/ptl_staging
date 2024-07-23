/** @file
  This file contains functions for accessing FIT and its entries

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include "FitAccess.h"
#include <Library/DebugLib.h>

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
  UINT32                          Index;
  UINTN                           FitBaseAddress;
  EFI_STATUS                      Status;

  Status = GetFitBase (&FitBaseAddress);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return NULL;
  }

  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FitBaseAddress;
  if (FitEntry[0].Address != *(UINT64 *)"_FIT_   ") {
    DEBUG ((DEBUG_ERROR, "\tFIT signature was not found or is incorrect\n"));
    return NULL;
  }
  if (FitEntry[0].Type != FIT_TYPE_00_HEADER) {
    DEBUG ((DEBUG_ERROR, "\tFIT header type is incorrect\n"));
    return NULL;
  }
  EntryNum = *(UINT32 *)(&FitEntry[0].Size[0]) & 0x00FFFFFF;
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
  This functions finds the OBB Hash stored inside the Boot Policy Manifest (BPM)

  @param  BootMode              Current boot mode.

  @retval OBB Hash              A struture pointer where the OBB Hash is located inside the BPM.
  @retval NULL                  BPM was not found or is invalid

**/
VOID *
FindObbHash (
  EFI_BOOT_MODE                   BootMode
  )
{
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry;
  BPM_HEADER                      *Bpm;
  BPM_ELEMENT_HEADER              *Element;
  BPM_ELEMENT_HEADER              *ElementEnd;
  IBB_ELEMENT                     *Ibb;
  UINT32                          *IbbEntryPoint;
  HASH_LIST                       *DigestList;
  UINTN                           BpmSize;

  DEBUG ((DEBUG_INFO, "OBB:: FindObbHash START\n"));

  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FindBpm ();
  if (FitEntry == NULL) {
    DEBUG ((DEBUG_ERROR, "\tBPM entry was not found inside the FIT\n"));
    return NULL;
  }
  Bpm = (BPM_HEADER *)(UINTN)FitEntry->Address;
  if (Bpm == NULL || Bpm->StructureID != BPM_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "\tBPM entry is not valid\n"));
    return NULL;
  }
  BpmSize = FitEntry->Size[0] | (FitEntry->Size[1] << 8) | (FitEntry->Size[2] << 16);
  DEBUG ((DEBUG_INFO, "\tBpm[0x%016lx]\n", (UINTN)Bpm));
  DEBUG ((DEBUG_INFO, "\tBpmSize\t\t\t\t\t= 0x%08x\n", (UINT32)BpmSize));
  Element = (BPM_ELEMENT_HEADER *)((UINT8 *)Bpm + Bpm->HdrSize);
  ElementEnd = (BPM_ELEMENT_HEADER *)((UINT8 *)Bpm + BpmSize);
  while (Element < ElementEnd) {
    if (Element->StructureId == IBB_SIGNATURE) {
      Ibb = (IBB_ELEMENT *)Element;
      if ((BootMode == BOOT_ON_S3_RESUME && Ibb->SetType == 1) ||
          (BootMode != BOOT_ON_S3_RESUME && Ibb->SetType == 0)) {
        IbbEntryPoint = (UINT32 *)(((UINT8 *)&Ibb->PostIbbHash)
                                   + sizeof (Ibb->PostIbbHash)
                                   + Ibb->PostIbbHash.Size);
        DigestList = (HASH_LIST *)(IbbEntryPoint + 1);
        return (UINT8 *)DigestList + DigestList->Size;
      }
    }
    Element = (BPM_ELEMENT_HEADER *)((UINT8 *)Element + Element->ElementSize);
  }

  DEBUG ((DEBUG_INFO, "OBB:: FindObbHash END\n"));

  return NULL;
}

/**
  This functions finds the Post IBB Hash stored inside the Boot Policy Manifest (BPM).

  @param  BootMode[in]  Current boot mode.

  @retval  POST IBB Hash  A struture pointer where the POST IBB Hash is located inside the BPM.
  @retval  NULL           POST IBB Hash was not found or is invalid.

**/
VOID *
FindPostIbbHash (
  IN EFI_BOOT_MODE  BootMode
  )
{
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry;
  BPM_HEADER                      *Bpm;
  BPM_ELEMENT_HEADER              *Element;
  BPM_ELEMENT_HEADER              *ElementEnd;
  IBB_ELEMENT                     *Ibb;
  UINTN                           BpmSize;

  DEBUG ((DEBUG_INFO, "OBB:: FindPostIbbHash START\n"));

  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FindBpm ();
  if (FitEntry == NULL) {
    DEBUG ((DEBUG_ERROR, "\tBPM entry was not found inside the FIT\n"));
    return NULL;
  }

  Bpm = (BPM_HEADER *)(UINTN)FitEntry->Address;
  if ((Bpm == NULL) || (Bpm->StructureID != BPM_SIGNATURE)) {
    DEBUG ((DEBUG_ERROR, "\tBPM entry is not valid\n"));
    return NULL;
  }

  BpmSize = FitEntry->Size[0] | (FitEntry->Size[1] << 8) | (FitEntry->Size[2] << 16);
  DEBUG ((DEBUG_INFO, "\tBpm[0x%08x]\n", (UINTN)Bpm));
  DEBUG ((DEBUG_INFO, "\tBpmSize\t\t\t\t\t= 0x%08x\n", (UINT32)BpmSize));

  Element    = (BPM_ELEMENT_HEADER *)((UINT8 *)Bpm + Bpm->HdrSize);
  ElementEnd = (BPM_ELEMENT_HEADER *)((UINT8 *)Bpm + BpmSize);
  while (Element < ElementEnd) {
    if (Element->StructureId == IBB_SIGNATURE) {
      Ibb = (IBB_ELEMENT *)Element;
      if ((BootMode != BOOT_ON_S3_RESUME) && (Ibb->SetType == 0)) {
        //
        // Post-IBB and OBB FVs HASH FV get record within BPM.
        // When doing the seamless recovery/resiliency restore, the OBB is not trustable.
        // Need to check the Post-IBB HASH value within here only.
        // OBB FVs digest would be checked by capsule recovery flow.
        //
        return (UINT8 *)&Ibb->PostIbbHash;
      }
    }

    Element = (BPM_ELEMENT_HEADER *)((UINT8 *)Element + Element->ElementSize);
  }

  DEBUG ((DEBUG_INFO, "OBB:: FindPostIbbHash END\n"));

  return NULL;
}
