/** @file
  This file is SampleCode for Intel PEI Platform Policy initialization in post-memory.
  For FSP API mode

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include "PeiPolicy.h"
#include <Library/PeiPolicyBoardConfigLib.h>
#include <Library/PeiPolicyAdvancedConfigLib.h>
#include <Library/PeiServicesLib.h>
#include <FspEas.h>
#include <Library/FspCommonLib.h>
#include <Library/FspWrapperApiLib.h>

VOID *
EFIAPI
SiliconPolicyUpdatePostMemLoadDefaultUpd (
  VOID
  )
{
  FSP_INFO_HEADER              *FspsHeaderPtr;
  EFI_PHYSICAL_ADDRESS         FspsUpdDataAddress;
  UINTN                        *SourceData;
  EFI_STATUS                   Status;

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Start in Post-Memory...\n"));

  //
  // Copy default FSP-S UPD data from Flash
  //

  FspsHeaderPtr = (FSP_INFO_HEADER *) FspFindFspHeader (PcdGet32 (PcdFspsBaseAddress));
  if (FspsHeaderPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to get FspsHeaderPtr!\n"));
    ASSERT (FspsHeaderPtr != NULL);
    return NULL;
  }
  DEBUG ((DEBUG_INFO, "FspsHeaderPtr - 0x%x\n", FspsHeaderPtr));

  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES (FspsHeaderPtr->CfgRegionSize),
             &FspsUpdDataAddress
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to allocate memory for FspsUpdData!\n"));
    ASSERT (!EFI_ERROR (Status));
    return NULL;
  }
  SourceData = (UINTN *) ((UINTN) FspsHeaderPtr->ImageBase + (UINTN) FspsHeaderPtr->CfgRegionOffset);

  CopyMem ((VOID *)(UINTN) FspsUpdDataAddress, SourceData, (UINTN) FspsHeaderPtr->CfgRegionSize);

  PcdSet64S (PcdFspsUpdDataAddress64, FspsUpdDataAddress);
  DEBUG ((DEBUG_INFO, "FspsUpdDataAddress - 0x%lx\n", FspsUpdDataAddress));

  return (VOID *)(UINTN) FspsUpdDataAddress;
}

VOID *
EFIAPI
SiliconPolicyUpdatePostMem (
  IN OUT VOID *Policy
  )
{
  Policy = NULL;

  SiliconPolicyUpdatePostMemLoadDefaultUpd ();

  //
  // Update essential policies by board configuration
  //
  UpdatePeiPolicyBoardConfigApi ();

  //
  // Update advanced policies
  //
  SiliconPolicyUpdatePostMemAdvancedConfigApi ();

  return Policy;
}
