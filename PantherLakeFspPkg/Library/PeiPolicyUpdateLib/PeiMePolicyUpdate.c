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
#include <FspsUpd.h>
#include <Library/ConfigBlockLib.h>

/**
  This function performs ME PEI Policy update.

  @param[in] SiPolicy       The SI Policy PPI instance
  @param[in] FspsUpd        The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiMePolicy (
  IN OUT SI_POLICY_PPI    *SiPolicy,
  IN     FSPS_UPD         *FspsUpd
  )
{
  EFI_STATUS                      Status;
  ME_PEI_CONFIG                   *MePeiConfig;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiMePolicy\n"));

  Status = GetConfigBlock ((VOID *) SiPolicy, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);

  MePeiConfig->MeUnconfigOnRtcClear      = FspsUpd->FspsConfig.MeUnconfigOnRtcClear;
  MePeiConfig->CseDataResilience         = FspsUpd->FspsConfig.CseDataResilience;
  MePeiConfig->PseEomFlowEnable          = FspsUpd->FspsConfig.PseEomFlowEnable;

  //
  // Test policies
  //
  MePeiConfig->EndOfPostMessage          = FspsUpd->FspsConfig.EndOfPostMessage;
  MePeiConfig->DisableD0I3SettingForHeci = FspsUpd->FspsConfig.DisableD0I3SettingForHeci;
  MePeiConfig->MctpBroadcastCycle        = FspsUpd->FspsConfig.MctpBroadcastCycle;

  return EFI_SUCCESS;
}
