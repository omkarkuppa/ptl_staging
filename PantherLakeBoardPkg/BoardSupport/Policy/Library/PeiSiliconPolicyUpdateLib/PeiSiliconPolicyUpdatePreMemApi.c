/** @file
  This file is SampleCode for Intel PEI Platform Policy initialization in pre-memory.
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
SiliconPolicyUpdatePreMemLoadDefaultUpd (
  VOID
  )
{
  FSP_INFO_HEADER              *FspmHeaderPtr;
  FSPM_UPD_COMMON              *FspmUpdDataPtr;
  UINTN                        *SourceData;

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Start in Pre-Memory...\n"));

  //
  // Copy default FSP-M UPD data from Flash
  //
  FspmHeaderPtr = (FSP_INFO_HEADER *) FspFindFspHeader (PcdGet32 (PcdFspmBaseAddress));
  if (FspmHeaderPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to get FspmHeaderPtr!\n"));
    ASSERT (FspmHeaderPtr != NULL);
    return NULL;
  }
  DEBUG ((DEBUG_INFO, "FspmHeaderPtr - 0x%x\n", FspmHeaderPtr));

  FspmUpdDataPtr = (FSPM_UPD_COMMON *) AllocateZeroPool ((UINTN) FspmHeaderPtr->CfgRegionSize);
  if (FspmUpdDataPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to allocate memory for FspmUpdData!\n"));
    ASSERT (FspmUpdDataPtr != NULL);
    return NULL;
  }
  SourceData = (UINTN *) ((UINTN) FspmHeaderPtr->ImageBase + (UINTN) FspmHeaderPtr->CfgRegionOffset);

  CopyMem (FspmUpdDataPtr, SourceData, (UINTN) FspmHeaderPtr->CfgRegionSize);

  PcdSet64S (PcdFspmUpdDataAddress64, (UINTN) FspmUpdDataPtr);
  DEBUG ((DEBUG_INFO, "FspmUpdDataPtr - 0x%x\n", (UINTN) FspmUpdDataPtr));

  return FspmUpdDataPtr;
}

VOID *
EFIAPI
SiliconPolicyUpdatePreMem (
  IN OUT VOID *Policy
  )
{
  Policy = NULL;

  SiliconPolicyUpdatePreMemLoadDefaultUpd ();
  //
  // Update essential policies by board configuration
  //
  UpdatePeiPolicyBoardConfigPreMem (Policy);

  //
  // Update advanced policies
  //
  SiliconPolicyUpdatePreMemAdvancedConfig (Policy);

  //
  // Update advanced policies
  //
  SiliconPolicyUpdatePreMemAdvancedConfigApi ();

  return Policy;
}
