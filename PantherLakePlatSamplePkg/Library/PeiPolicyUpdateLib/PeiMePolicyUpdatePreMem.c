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
#include <Library/SiPolicyLib.h>
#include <Ppi/ReadOnlyVariable2.h>

#include <PolicyUpdateMacro.h>
#include <SetupVariable.h>

#include <FspmUpd.h>

/**
  Update the ME Policy Library

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS           Update complete.
**/
EFI_STATUS
EFIAPI
UpdatePeiMePolicyPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  ME_SETUP                        MeSetup;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *Variable;
  UINTN                           VariableSize;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  ME_PEI_PREMEM_CONFIG            *MePeiPreMemConfig;
#endif

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#endif

  DEBUG ((DEBUG_INFO, "Update PeiMePolicyUpdate Pre-Mem Start\n"));

  Status = EFI_SUCCESS;
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &Variable
             );
  if (!EFI_ERROR (Status)) {
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
    #if FixedPcdGet8(PcdFspModeSelection) == 0
      MePeiPreMemConfig = NULL;
      Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
      ASSERT_EFI_ERROR (Status);
    #endif
      COMPARE_AND_UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.HeciTimeouts,  MePeiPreMemConfig->HeciTimeouts, MeSetup.HeciTimeouts);

      //
      // Test policies
      //
      COMPARE_AND_UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.DidInitStat, MePeiPreMemConfig->DidInitStat, MeSetup.DidInitStat);
      COMPARE_AND_UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableCpuReplacedPolling, MePeiPreMemConfig->DisableCpuReplacedPolling, MeSetup.DisableCpuReplacedPolling);
      COMPARE_AND_UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableMessageCheck, MePeiPreMemConfig->DisableMessageCheck, MeSetup.DisableMessageCheck);
      COMPARE_AND_UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.SkipMbpHob, MePeiPreMemConfig->SkipMbpHob, MeSetup.SkipMbpHob);
      COMPARE_AND_UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.HeciCommunication, MePeiPreMemConfig->HeciCommunication, MeSetup.HeciCommunication);
      COMPARE_AND_UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.HeciCommunication2, MePeiPreMemConfig->HeciCommunication2, MeSetup.HeciCommunication2);
      COMPARE_AND_UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.HeciCommunication3, MePeiPreMemConfig->HeciCommunication3, MeSetup.HeciCommunication3);
      COMPARE_AND_UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.KtDeviceEnable, MePeiPreMemConfig->KtDeviceEnable, MeSetup.KtDeviceEnable);
    }
  }

  return Status;
}
