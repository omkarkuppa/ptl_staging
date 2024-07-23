/** @file
  This file is SampleCode of the library for Intel AMT PEI Platform Policy Update.

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

#include <Library/BaseMemoryLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/SiPolicy.h>
#include <IndustryStandardAsf.h>
#include <IntelAsfExtensions.h>
#include <MebxData.h>
#include <PolicyUpdateMacro.h>
#include <SetupVariable.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspsUpd.h>
#endif

//
// The ASF message types which will be push to PET Hob.
//
GLOBAL_REMOVE_IF_UNREFERENCED ASF_FRAMEWORK_MESSAGE_TYPE mAmtForcePushPetPolicy[] = {
  AsfMessageChassisIntrusion,
  AsfMessageUserAuthenticationInvalid,
  AsfMessageHddAuthenticationInvalid,
  AsfMessageTermination
};

/**
  Install the Active Management push PET policy
**/
VOID
InstallAmtForcePushPetPolicy (
  VOID
  )
{
  VOID        *Hob;

  Hob = BuildGuidDataHob (
          &gAmtForcePushPetPolicyGuid,
          &mAmtForcePushPetPolicy,
          sizeof (mAmtForcePushPetPolicy)
          );
  ASSERT (Hob != NULL);
  return;
}

/**
  Install the Active Management Policy Ppi Library

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS           Initialization complete.
  @retval EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
UpdatePeiAmtPolicy (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
#if FixedPcdGet8(PcdAmtEnable) == 1
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  ME_SETUP                        MeSetup;
  AMT_PEI_CONFIG                  *AmtPeiConfig;
  MEBX_DATA                       MebxData;

  DEBUG ((DEBUG_INFO, "Update PeiAmtPolicyUpdate Pos-Mem Start\n"));

  AmtPeiConfig = NULL;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gAmtPeiConfigGuid, (VOID *) &AmtPeiConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Locate Variable Ppi
  //
  Status = PeiServicesLocatePpi (&gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, (VOID **) &VariableServices);

  //
  // Make sure we have a PPI, if not, just return.
  //
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }
  //
  // Get Setup Variable
  //
  VariableSize = sizeof (ME_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"MeSetup",
                               &gMeSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &MeSetup
                               );

  if (!EFI_ERROR (Status)) {
    COMPARE_AND_UPDATE_POLICY (AmtPeiConfig->AmtEnabled,        MeSetup.Amt              );
    COMPARE_AND_UPDATE_POLICY (AmtPeiConfig->WatchDogEnabled,   MeSetup.WatchDogEnabled  );
    COMPARE_AND_UPDATE_POLICY (AmtPeiConfig->WatchDogTimerBios, MeSetup.WatchDogTimerBios);
    COMPARE_AND_UPDATE_POLICY (AmtPeiConfig->WatchDogTimerOs,   MeSetup.WatchDogTimerOs  );
  }

  VariableSize  = sizeof (MEBX_DATA);

  Status = VariableServices->GetVariable (
                               VariableServices,
                               MEBX_DATA_VARIABLE_NAME,
                               &gMebxDataGuid,
                               NULL,
                               &VariableSize,
                               &MebxData
                               );
  if (!EFI_ERROR (Status)) {
    UPDATE_POLICY (AmtPeiConfig->AmtSolEnabled, MebxData.AmtSol);
  } else {
    DEBUG ((DEBUG_WARN, "Failed to retrieve Variable:\"MebxData\", Status = %r\n", Status));
  }

  if (AmtPeiConfig->AmtEnabled == 0) {
    //
    // Disable below settings since they have dependency on AMT.
    //
    UPDATE_POLICY (AmtPeiConfig->WatchDogEnabled,   0);
    UPDATE_POLICY (AmtPeiConfig->WatchDogTimerBios, 0);
    UPDATE_POLICY (AmtPeiConfig->WatchDogTimerOs,   0);
    UPDATE_POLICY (AmtPeiConfig->AmtSolEnabled,     0);
  }

  InstallAmtForcePushPetPolicy ();
  return Status;
#else
  return EFI_SUCCESS;
#endif
}

