/** @file
  This file is SampleCode of the library for Intel Silicon PEI
  Platform Policy initialization.

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
#include <ConfigBlock/SiPreMemConfig.h>

/**
  This function performs Silicon PEI Policy initialization in pre-memory.

  @param[in, out] SiPreMemPolicy The Silicon Policy PPI instance
  @param[in]      FspmUpd        The pointer of FspmUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiSiPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI *SiPreMemPolicy,
  IN     FSPM_UPD             *FspmUpd
  )
{
  EFI_STATUS                  Status;
  SI_PREMEM_CONFIG            *SiPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSiPreMemConfigGuid, (VOID *) &SiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  SiPreMemConfig->PlatformDebugOption = FspmUpd->FspmConfig.PlatformDebugOption;
  SiPreMemConfig->SkipOverrideBootModeWhenFwUpdate = FspmUpd->FspmConfig.SiSkipOverrideBootModeWhenFwUpdate;
  SiPreMemConfig->StaticContentSizeAt4Gb = FspmUpd->FspmConfig.StaticContentSizeAt4Gb;

  return EFI_SUCCESS;
}