/** @file
  This file is SampleCode of the library for Intel AMT PEI Platform Policy Update.

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
#include <Library/ConfigBlockLib.h>

/**
  This function performs Active Management PEI Policy update.

  @param[in, out] SiPolicyPpi   The Si Policy PPI instance
  @param[in]      FspsUpd       The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiAmtPolicy (
  IN OUT SI_POLICY_PPI    *SiPolicyPpi,
  IN     FSPS_UPD         *FspsUpd
  )
{
  EFI_STATUS              Status;
  AMT_PEI_CONFIG          *AmtPeiConfig;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiAmtPolicy\n"));

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gAmtPeiConfigGuid, (VOID *) &AmtPeiConfig);
  ASSERT_EFI_ERROR (Status);

  AmtPeiConfig->AmtEnabled        = FspsUpd->FspsConfig.AmtEnabled;
  AmtPeiConfig->WatchDogEnabled   = FspsUpd->FspsConfig.WatchDogEnabled;
  AmtPeiConfig->WatchDogTimerOs   = FspsUpd->FspsConfig.WatchDogTimerOs;
  AmtPeiConfig->WatchDogTimerBios = FspsUpd->FspsConfig.WatchDogTimerBios;
  AmtPeiConfig->AmtSolEnabled     = FspsUpd->FspsConfig.AmtSolEnabled;

  return EFI_SUCCESS;
}
