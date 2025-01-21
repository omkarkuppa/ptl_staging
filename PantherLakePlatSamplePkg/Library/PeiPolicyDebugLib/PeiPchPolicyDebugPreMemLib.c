/** @file
  This file is SampleCode of the library for Intel PCH PEI Debug Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#include <Uefi.h>
#include <Library/SiPolicyLib.h>
#include <PiPei.h>
#include <SetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchInfoLib.h>
#include <PolicyUpdateMacro.h>
#include <SmbusConfig.h>
#include <DciConfig.h>
#include <HsioPcieConfig.h>
#include <HsioSataConfig.h>
#include <LpcConfig.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <WatchDogConfig.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspmUpd.h>
#endif

/**
  Update Smbus Debug Pre Mem Policy.

  @param[in] SiPreMemPolicyPpi    Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateSmbusDebugPreMemPolicy (
  IN SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi,
  IN PCH_SETUP                 *PchSetup
  )
{
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  PCH_SMBUS_PREMEM_CONFIG         *SmbusPreMemConfig;
  EFI_STATUS                      Status;
#endif

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SmbusSpdWriteDisable, SmbusPreMemConfig->SpdWriteDisable, PchSetup->SmbusSpdWriteDisable);
}

/**
  This function performs PCH PEI Debug Policy initialization.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS        The PPI is installed and initialized.
  @retval EFI ERRORS         The PPI is not successfully installed.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyDebugPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  PCH_SETUP                       PchSetup;

  DEBUG ((DEBUG_INFO, "Update PeiPchPolicyDebug Pre-Mem Start\n"));

  //
  // Retrieve Setup variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices       // PPI
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (PCH_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &PchSetup
                               );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  UpdateSmbusDebugPreMemPolicy (SiPreMemPolicyPpi, &PchSetup);

  return EFI_SUCCESS;
}
