/** @file
  This file is SampleCode of the library for Intel ME PEI
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
#include <Library/FspCommonLib.h>
#include <FspmUpd.h>
#include <Library/ConfigBlockLib.h>

/**
  This function performs ME PEI Policy update in pre-memory.

  @param[in out] SiPreMemPolicy    SI_PREMEM_POLICY_PPI
  @param[in]     FspmUpd           The pointer of FspmUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiMePolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI   *SiPreMemPolicy,
  IN     FSPM_UPD               *FspmUpd
  )
{
  EFI_STATUS                      Status;
  ME_PEI_PREMEM_CONFIG            *MePeiPreMemConfig;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FspUpdatePeiMePolicyPreMem\n"));

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  MePeiPreMemConfig->HeciTimeouts               = FspmUpd->FspmConfig.HeciTimeouts;

  // <Test policies>
  MePeiPreMemConfig->DidInitStat                = FspmUpd->FspmConfig.DidInitStat;
  MePeiPreMemConfig->DisableCpuReplacedPolling  = FspmUpd->FspmConfig.DisableCpuReplacedPolling;
  MePeiPreMemConfig->DisableMessageCheck        = FspmUpd->FspmConfig.DisableMessageCheck;
  MePeiPreMemConfig->SkipMbpHob                 = FspmUpd->FspmConfig.SkipMbpHob;
  MePeiPreMemConfig->HeciCommunication2         = FspmUpd->FspmConfig.HeciCommunication2;
  MePeiPreMemConfig->KtDeviceEnable             = FspmUpd->FspmConfig.KtDeviceEnable;
  MePeiPreMemConfig->SkipCpuReplacementCheck    = FspmUpd->FspmConfig.SkipCpuReplacementCheck;

  MePeiPreMemConfig->HeciCommunication          = FspmUpd->FspmConfig.HeciCommunication;
  MePeiPreMemConfig->HeciCommunication3         = FspmUpd->FspmConfig.HeciCommunication3;

  return EFI_SUCCESS;
}

