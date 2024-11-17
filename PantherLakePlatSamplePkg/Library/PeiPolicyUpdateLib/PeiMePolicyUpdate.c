/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include <Library/ConfigBlockLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/PmcLib.h>
#include <Library/SiPolicyLib.h>
#include <Ppi/ReadOnlyVariable2.h>

#include <PolicyUpdateMacro.h>
#include <SetupVariable.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspsUpd.h>
#endif

/**
  Update the ME Policy Library

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS             Update complete.
  @retval Others                  Update unsuccessful.
**/
EFI_STATUS
EFIAPI
UpdatePeiMePolicy (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
  EFI_STATUS                      Status;
  ME_SETUP                        MeSetup;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *Variable;
  UINTN                           VariableSize;
#if FixedPcdGet8(PcdFspModeSelection) == 0
  ME_PEI_CONFIG                   *MePeiConfig;
#endif
  DEBUG ((DEBUG_INFO, "%a()\n", __FUNCTION__));
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspsUpd;
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#else
  MePeiConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
#endif

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &Variable
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  VariableSize = sizeof (ME_SETUP);
  Status = Variable->GetVariable (
                       Variable,
                       L"MeSetup",
                       &gMeSetupVariableGuid,
                       NULL,
                       &VariableSize,
                       &MeSetup
                       );
  if (!EFI_ERROR (Status)) {
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.EndOfPostMessage, MePeiConfig->EndOfPostMessage, MeSetup.EndOfPostMessage);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.MeUnconfigOnRtcClear, MePeiConfig->MeUnconfigOnRtcClear, MeSetup.MeUnconfigOnRtcClear);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.MctpBroadcastCycle, MePeiConfig->MctpBroadcastCycle, MeSetup.MctpBroadcastCycle);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.CseDataResilience, MePeiConfig->CseDataResilience, MeSetup.CseDataResilience);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.SseCommunication, MePeiConfig->SseCommunication, MeSetup.SseCommunication);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PseEomFlowEnable, MePeiConfig->PseEomFlowEnable, MeSetup.PseEomFlowEnable);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.DisableD0I3SettingForHeci, MePeiConfig->DisableD0I3SettingForHeci, MeSetup.DisableD0I3SettingForHeci);
  }
  if (!PmcIsRtcBatteryGood ()) {
    //
    // For non coin battery design, this can be skipped.
    //
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.MeUnconfigOnRtcClear, MePeiConfig->MeUnconfigOnRtcClear, 2);
  }

  return Status;
}

