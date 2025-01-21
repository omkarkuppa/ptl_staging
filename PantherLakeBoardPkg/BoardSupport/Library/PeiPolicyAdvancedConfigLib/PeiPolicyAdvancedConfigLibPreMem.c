/** @file
 Intel PEI Pre-Memory Advanced Policy Update

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include "PeiPolicyAdvancedConfig.h"

#include <SetupVariable.h>
#include <Ppi/ReadOnlyVariable2.h>

#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>

/**
  This function hook Pre-Memory Policy update by advanced configuration.

  @retval EFI_SUCCESS             The Policy is successfully updated.
  @retval Others                  The Policy is not successfully updated.
**/
VOID
EFIAPI
SiliconPolicyUpdatePreMemAdvancedConfig (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  SETUP_DATA                        SystemConfiguration;
  UINTN                             Size;
  UINT8                             FirmwareConfiguration;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "PeiServicesLocatePpi failed\n"));
    return;
  }
  FirmwareConfiguration = 0;
  Size = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &Size,
                               &SystemConfiguration
                               );
  if (!EFI_ERROR (Status)) {
    FirmwareConfiguration = SystemConfiguration.FirmwareConfiguration;
  }

  if (FirmwareConfiguration != FwConfigDefault) {
    //
    // Update and override all platform related and customized settings below.
    //
    UpdatePeiPciePolicyPreMem (SiPreMemPolicyPpi);
    UpdatePeiPchPolicyPreMem (SiPreMemPolicyPpi);
    UpdatePeiMePolicyPreMem (SiPreMemPolicyPpi);
    UpdatePeiSaPolicyPreMem (SiPreMemPolicyPpi);
    UpdatePeiCpuPolicyPreMem (SiPreMemPolicyPpi);
#if FixedPcdGetBool (PcdFspVEnable) == 1
    UpdatePeiFspVPolicyPreMem (SiPreMemPolicyPpi);
#endif

    if ((FirmwareConfiguration >= FwConfigTest) && (FirmwareConfiguration < FwConfigMax)) {
      // Update Debug Policies
      UpdatePeiPchPolicyDebugPreMem (SiPreMemPolicyPpi);
      UpdatePeiSaPolicyDebugPreMem (SiPreMemPolicyPpi);
      UpdatePeiCpuPolicyDebugPreMem (SiPreMemPolicyPpi);
    }

  }
}

/**
  This function hooks Pre-Memory Policy update by advanced configuration in FSP API mode.

  @retval EFI_SUCCESS             The Policy is successfully updated.
  @retval Others                  The Policy is not successfully updated.
**/
VOID
EFIAPI
SiliconPolicyUpdatePreMemAdvancedConfigApi (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  SETUP_DATA                        SystemConfiguration;
  UINTN                             Size;
  UINT8                             FirmwareConfiguration;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "PeiServicesLocatePpi failed\n"));
    return;
  }
  FirmwareConfiguration = 0;
  Size = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &Size,
                               &SystemConfiguration
                               );
  if (!EFI_ERROR (Status)) {
    FirmwareConfiguration = SystemConfiguration.FirmwareConfiguration;
  }

  if (FirmwareConfiguration != FwConfigDefault) {
    //
    // Update and override all platform related and customized settings below.
    //
    UpdatePeiSiPolicyPreMem ();
  }
}
