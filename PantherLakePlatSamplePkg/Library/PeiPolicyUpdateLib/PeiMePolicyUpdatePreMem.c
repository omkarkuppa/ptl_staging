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

#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspmUpd.h>
#endif

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
  ME_PEI_PREMEM_CONFIG            *MePeiPreMemConfig;

  DEBUG ((DEBUG_INFO, "Update PeiMePolicyUpdate Pre-Mem Start\n"));

  MePeiPreMemConfig = NULL;

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
      Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
      ASSERT_EFI_ERROR (Status);

      COMPARE_AND_UPDATE_POLICY (MePeiPreMemConfig->HeciTimeouts, MeSetup.HeciTimeouts);

      //
      // Test policies
      //
      COMPARE_AND_UPDATE_POLICY (MePeiPreMemConfig->DidInitStat,               MeSetup.DidInitStat              );
      COMPARE_AND_UPDATE_POLICY (MePeiPreMemConfig->DisableCpuReplacedPolling, MeSetup.DisableCpuReplacedPolling);
      COMPARE_AND_UPDATE_POLICY (MePeiPreMemConfig->DisableMessageCheck,       MeSetup.DisableMessageCheck      );
      COMPARE_AND_UPDATE_POLICY (MePeiPreMemConfig->SkipMbpHob,                MeSetup.SkipMbpHob               );
      COMPARE_AND_UPDATE_POLICY (MePeiPreMemConfig->HeciCommunication,         MeSetup.HeciCommunication        );
      COMPARE_AND_UPDATE_POLICY (MePeiPreMemConfig->HeciCommunication2,        MeSetup.HeciCommunication2       );
      COMPARE_AND_UPDATE_POLICY (MePeiPreMemConfig->HeciCommunication3,        MeSetup.HeciCommunication3       );
      COMPARE_AND_UPDATE_POLICY (MePeiPreMemConfig->KtDeviceEnable,            MeSetup.KtDeviceEnable           );
    }
  }

  return Status;
}
