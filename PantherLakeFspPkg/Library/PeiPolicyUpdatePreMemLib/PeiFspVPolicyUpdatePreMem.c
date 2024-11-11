/** @file
  This file is SampleCode of the library for Intel FspV Pre-Mem
  Platform Policy Update.

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

#include <Library/DebugLib.h>
#include <Ppi/SiPolicy.h>
#include <FspmUpd.h>
#include <Library/ConfigBlockLib.h>
#include <FspVConfig.h>

/**
  This function performs FspV PEI Policy update in pre-memory.

  @param[in out] SiPreMemPolicy    SI_PREMEM_POLICY_PPI
  @param[in]     FspmUpd           The pointer of FspmUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiFspVPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI   *SiPreMemPolicy,
  IN     FSPM_UPD               *FspmUpd
  )
{
  EFI_STATUS          Status;
  FSPV_PREMEM_CONFIG  *FspVPreMemConfig;

  DEBUG ((DEBUG_INFO , "FspUpdatePeiFspVPolicyPreMem\n"));

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gFspVPreMemConfigGuid, (VOID *) &FspVPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR ((Status))) {
    return EFI_NOT_FOUND;
  }

  FspVPreMemConfig = (FSPV_PREMEM_CONFIG *) (UINTN) FspmUpd->FspmConfig.FspmValidationPtr;

  DEBUG ((DEBUG_INFO, "FspVPreMemConfig->TestId = %x\n", FspVPreMemConfig->TestId));

  return EFI_SUCCESS;
}
