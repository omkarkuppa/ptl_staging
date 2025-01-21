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
#include <FspsUpd.h>
#include <Library/PeiServicesLib.h>

/**
  This function performs Silicon PEI Policy initialization.

  @param[in] SiPolicy       The Silicon Policy PPI instance
  @param[in] FspsUpd        The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiSiPolicy (
  IN OUT SI_POLICY_PPI  *SiPolicy,
  IN     FSPS_UPD       *FspsUpd
  )
{
  EFI_STATUS                   Status;
  SI_CONFIG                    *SiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
  ASSERT_EFI_ERROR (Status);

  SiConfig->SkipSsidProgramming      = FspsUpd->FspsConfig.SiSkipSsidProgramming;
  SiConfig->CustomizedSvid           = FspsUpd->FspsConfig.SiCustomizedSvid;
  SiConfig->CustomizedSsid           = FspsUpd->FspsConfig.SiCustomizedSsid;
  SiConfig->SsidTablePtr             = (UINT32*)(UINTN)FspsUpd->FspsConfig.SiSsidTablePtr;
  SiConfig->NumberOfSsidTableEntry   = FspsUpd->FspsConfig.SiNumberOfSsidTableEntry;
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  SiConfig->SkipBiosDoneWhenFwUpdate = FspsUpd->FspsConfig.SiSkipBiosDoneWhenFwUpdate;
#endif

  return EFI_SUCCESS;
}