/** @file
  FspV PEI Policy Update & initialization.

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
#include <Library/PeiServicesLib.h>
#include <SetupVariable.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <PiPei.h>
#include <Ppi/SiPolicy.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiPlatformLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/SiPolicyLib.h>
#include <FspVConfig.h>
#include <PolicyUpdateMacro.h>
#include <FspVSetup.h>
#include <Library/MemoryAllocationLib.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspmUpd.h>
#include <FspsUpd.h>
#endif
#include <Library/FspCommonLib.h>


/**
  This function performs FspV Post-Mem PEI Policy initialization.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiFspVPolicy (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
  EFI_STATUS                       Status;
  UINTN                            VariableSize;
  FSPV_SETUP                       FspVSetup;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;
  FSPV_POSTMEM_CONFIG              *FspVUpdateConfig;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                             *FspsUpd;
#endif
  DEBUG ((DEBUG_INFO, "Update PeiFspVPolicyUpdate Post-Mem Start\n"));
  FspVUpdateConfig                 = NULL;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
#else

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gFspVConfigGuid, (VOID *) &FspVUpdateConfig);
  ASSERT_EFI_ERROR (Status);
#endif

  //
  // Make sure ReadOnlyVariablePpi is available
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Get Setup Variable
  //
  VariableSize = sizeof (SETUP_DATA);
  if (Status == EFI_SUCCESS) {
    Status = VariableServices->GetVariable (
                               VariableServices,
                               FSPV_SETUP_NAME,
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &FspVSetup
                               );
  }
  if (EFI_ERROR ((Status))) {
    ASSERT_EFI_ERROR (Status);
    return EFI_NOT_FOUND;
  }

#if FixedPcdGet8(PcdFspModeSelection) == 1
  ((FSPS_UPD *)FspsUpd)->FspsConfig.FspsValidationPtr = (UINTN) (AllocateZeroPool (sizeof (FSPV_POSTMEM_CONFIG)));
  FspVUpdateConfig = (FSPV_POSTMEM_CONFIG *) (UINTN) ((FSPS_UPD *)FspsUpd)->FspsConfig.FspsValidationPtr;
  if (FspVUpdateConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
#endif

  UPDATE_POLICY_V2 (FspVUpdateConfig->TestId, FspVUpdateConfig->TestId, FspVSetup.FspSVTestCaseId);

  return EFI_SUCCESS;
}

/**
  This function performs FspV Pre-Mem PEI Policy initialization.

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiFspVPolicyPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                       Status;
  UINTN                            VariableSize;
  FSPV_SETUP                       FspVSetup;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;
  FSPV_PREMEM_CONFIG              *FspVUpdatePreMemConfig;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#endif
  DEBUG ((DEBUG_INFO, "Update PeiFspVPolicyUpdate Pre-Mem Start\n"));
  FspVUpdatePreMemConfig          = NULL;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
#else

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gFspVPreMemConfigGuid, (VOID *) &FspVUpdatePreMemConfig);
  ASSERT_EFI_ERROR (Status);
#endif

  //
  // Make sure ReadOnlyVariablePpi is available
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Get Setup Variable
  //
  VariableSize = sizeof (FSPV_SETUP);
  if (Status == EFI_SUCCESS) {
    Status = VariableServices->GetVariable (
                               VariableServices,
                               FSPV_SETUP_NAME,
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &FspVSetup
                               );
  }

  if (EFI_ERROR ((Status))) {
    ASSERT_EFI_ERROR (Status);
    return EFI_NOT_FOUND;
  }

#if FixedPcdGet8(PcdFspModeSelection) == 1
  ((FSPM_UPD *)FspmUpd)->FspmConfig.FspmValidationPtr = (UINTN) (AllocateZeroPool (sizeof (FSPV_PREMEM_CONFIG)));
  FspVUpdatePreMemConfig = (FSPV_PREMEM_CONFIG *) (UINTN) ((FSPM_UPD *)FspmUpd)->FspmConfig.FspmValidationPtr;
  if (FspVUpdatePreMemConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
#endif

  UPDATE_POLICY_V2 (FspVUpdatePreMemConfig->TestId, FspVUpdatePreMemConfig->TestId, FspVSetup.FspMVTestCaseId);

  return EFI_SUCCESS;
}
