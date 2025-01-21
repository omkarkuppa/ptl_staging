/** @file
  This file contains functions to support FBM/BPM operations in FSP.

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

@par Specification Reference:
**/

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/FspFbmSupportLib.h>
#include <BpmDef.h>

VOID *
FindBpm (
  VOID
  );

VOID *
FindFbm (
  VOID
  );

/**
 Locate FBM from FIT.

  @retval FBM structure location or NULL if not found.

**/
FSP_BOOT_MANIFEST_STRUCTURE *
EFIAPI
LocateFbm (
  VOID
  ) {
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry;
  FSP_BOOT_MANIFEST_STRUCTURE     *Fbm;

  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *) FindFbm ();
  if (FitEntry == NULL) {
    DEBUG ((DEBUG_ERROR, "Fbm entry was not found inside the FIT!\n"));
    return NULL;
  }

  Fbm = (FSP_BOOT_MANIFEST_STRUCTURE *)(VOID *)(UINTN) FitEntry->Address;
  if (*((UINT64 *) Fbm->StructureId) == FSP_BOOT_MANIFEST_STRUCTURE_ID) {
    return Fbm;
  }

  return NULL;
}

/**
  Search BSPM from address 4G - 4K

  @retval BSPM structure location or NULL if not found.

**/
BSPM_ELEMENT*
EFIAPI
SearchBspmNear4G (
  VOID
  )
{
  UINTN         BspmAddress;
  BSPM_ELEMENT  *BspmElement;

  BspmAddress = BSSS_SEARCH_START_ADDRESS;

  DEBUG ((DEBUG_INFO, "BPM entry was not found inside the FIT, try to search BSPM from address 4G - 4K\n"));
  while (BspmAddress < BSSS_SEARCH_END_ADDRESS) {
    BspmElement = (BSPM_ELEMENT *)BspmAddress;
    if (*((UINT64 *) BspmElement->StructureID) == BSPM_ELEMENT_STRUCTURE_ID) {
      DEBUG ((DEBUG_INFO, "Found BSPM at address 0x%x\n", BspmAddress));
      return BspmElement;
    }
    //
    // Search for BSSS on every 32 bytes Boundary
    //
    BspmAddress += 0x20;
  }
  DEBUG ((DEBUG_ERROR, "BSPM not Found!\n"));
  return NULL;
}

/**
 Locate BSSS-BSPM structure from BPM. BSPM is one kind of BSSS format defined for FSP use.
 BPM layout is shown as below:

  BPM
    - BPM Header
    - IBBS
    - TXTS
    - PMDA
    - PCDS
      - PDRS
      - CNBS
      - BSIS
        - BSIS Header
        - BSSS Array
          - BSPM
          - Other BSSS format (Not Defined yet)
      - Other PCDE Elements
    - PMSG

  @retval Pointer for BSPM structure in BPM, or NULL if not found.
**/
BSPM_ELEMENT*
EFIAPI
LocateBspm (
  VOID
  )
{
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry;
  BOOT_POLICY_MANIFEST_HEADER     *Bpm;
  UINTN                           BpmSize;
  UINTN                           BpmEnd;
  BPM_ELEMENT_HEADER              *ElementHeader;
  PCDE_ELEMENT_HEADER             *PcdeElementHeader;
  BSIS_SEGMENT                    *BsisSegment;
  BSPM_ELEMENT                    *BspmElement;
  BOOLEAN                         IsBsisFound;

  DEBUG ((DEBUG_INFO, "Locate Bsss BSPM Element START ...\n"));

  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *) FindBpm ();
  if (FitEntry == NULL) {
    DEBUG ((DEBUG_ERROR, "BPM entry was not found inside the FIT!\n"));
    return NULL;
  }

  Bpm = (BOOT_POLICY_MANIFEST_HEADER *)(UINTN) FitEntry->Address;
  if (Bpm == NULL || *((UINT64 *) Bpm->StructureId) != BOOT_POLICY_MANIFEST_HEADER_STRUCTURE_ID) {
    DEBUG ((DEBUG_ERROR, "BPM entry is not valid!\n"));
    return SearchBspmNear4G ();
  }
  DEBUG ((DEBUG_INFO, "Bpm is found: 0x%x!\n", (UINTN) (VOID *) Bpm));

  BpmSize = FitEntry->Size[0] | (FitEntry->Size[1] << 8) | (FitEntry->Size[2] << 16);
  BpmEnd  = (UINTN) ((UINT8 *) Bpm + BpmSize);

  ElementHeader  = (BPM_ELEMENT_HEADER *) (Bpm + 1);
  IsBsisFound = FALSE;
  while ((UINTN) (VOID *) ElementHeader < BpmEnd) {

    if (*((UINT64 *) ElementHeader->StructureId) != BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT_STRUCTURE_ID &&
        *((UINT64 *) ElementHeader->StructureId) != BOOT_POLICY_MANIFEST_PLATFORM_CONFIG_DATA_ELEMENT_STRUCTURE_ID) {

      ElementHeader = (BPM_ELEMENT_HEADER *) ((UINT8 *) ElementHeader + ElementHeader->ElementSize);
    } else if (*((UINT64 *) ElementHeader->StructureId) == BOOT_POLICY_MANIFEST_PLATFORM_CONFIG_DATA_ELEMENT_STRUCTURE_ID) {

      PcdeElementHeader = (PCDE_ELEMENT_HEADER *) ((UINT8 *) ElementHeader + sizeof (PLATFORM_CONFIG_DATA_ELEMENT) - sizeof (UINT8 *));
      while ((UINTN) (VOID *) PcdeElementHeader < BpmEnd) {

        if (*((UINT64 *) PcdeElementHeader->StructureId) != BPM_BSIS_ELEMENT_STRUCTURE_ID) {
          PcdeElementHeader = (PCDE_ELEMENT_HEADER *) ((UINT8 *) PcdeElementHeader + sizeof (PCDE_ELEMENT_HEADER) + PcdeElementHeader->SizeOfData);
        } else {
          BsisSegment = (BSIS_SEGMENT *) PcdeElementHeader;
          IsBsisFound = TRUE;
          break;
        }
      }
      break;
    } else {
      //
      // We are not expecting to see this structure format!
      //
      DEBUG ((DEBUG_INFO, "Unexpected Element Format!\n"));
      ASSERT (FALSE);
    }
  }

  if (IsBsisFound) {

    BspmElement = (BSPM_ELEMENT *) BsisSegment->Data;
    if (*((UINT64 *) BspmElement->StructureID) == BSPM_ELEMENT_STRUCTURE_ID) {
      DEBUG ((DEBUG_INFO, "BspmElement is found: 0x%x!\n", (UINTN) (VOID *) BspmElement));
      return BspmElement;
    } else {
      DEBUG ((DEBUG_INFO, "Unexpected BSSS Format!\n"));
    }
  } else {
    DEBUG ((DEBUG_INFO, "No BSIS Data is found in BPM!\n"));
    return NULL;
  }

  DEBUG ((DEBUG_INFO, "No BSIS Data is found in BPM!\n"));
  return NULL;
}