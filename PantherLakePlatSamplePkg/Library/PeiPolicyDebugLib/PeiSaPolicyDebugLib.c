/** @file
  This file is SampleCode of the library for Intel SA PEI Debug Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include "PeiPolicyDebug.h"
#include <CpuRegs.h>
#include <Library/CpuPlatformLib.h>
#include <Guid/AcpiVariable.h>
#include <Guid/MemoryTypeInformation.h>
#include <Library/HobLib.h>
#include <Platform.h>
#include <Library/PchInfoLib.h>
#include <PolicyUpdateMacro.h>
#include <FspmUpd.h>

/**
  This function performs SA PEI Debug PreMem Policy initialization.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyDebugPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  SA_SETUP                        SaSetup;
  UINTN                           VarSize;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  IGPU_PEI_PREMEM_CONFIG          *IGpuPreMemConfig;
#endif

  DEBUG ((DEBUG_INFO, "Update PeiSaPolicyDebug Pre-Mem Start\n"));

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  IGpuPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *) &IGpuPreMemConfig);
  ASSERT_EFI_ERROR (Status);
#endif
  //
  // Locate system configuration variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices       // PPI
             );
  ASSERT_EFI_ERROR ( Status);

  //
  // Get Setup SA variables
  //
  VarSize = sizeof (SA_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"SaSetup",
                               &gSaSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &SaSetup
                               );

  if (!EFI_ERROR (Status)) {
    //
    // Initialize the Graphics configuration
    //
    UPDATE_POLICY_V2 (((FSPM_UPD *)FspmUpd)->FspmConfig.PanelPowerEnable, IGpuPreMemConfig->PanelPowerEnable, SaSetup.PanelPowerEnable);
    //
    // Disable PanelPowerEnable if there is eDP present on DDI-A & B.
    //
#if FixedPcdGet8(PcdFspModeSelection) == 1
    if ((((FSPM_UPD *)FspmUpd)->FspmConfig.DdiPortAConfig != DdiPortEdp) && (((FSPM_UPD *)FspmUpd)->FspmConfig.DdiPortBConfig!= DdiPortEdp)) {
#else
    if ((IGpuPreMemConfig->DdiConfiguration.DdiPortAConfig != DdiPortEdp) && (IGpuPreMemConfig->DdiConfiguration.DdiPortBConfig != DdiPortEdp)) {
#endif
      UPDATE_POLICY_V2 (((FSPM_UPD *)FspmUpd)->FspmConfig.PanelPowerEnable, IGpuPreMemConfig->PanelPowerEnable, 0x0);
    }
  }

  return Status;
}