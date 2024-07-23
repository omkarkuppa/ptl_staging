/** @file
  This library initialize Silicon Policy for PostMemory.

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

#include <Ppi/PeiSiDefaultPolicy.h>
#include <Ppi/PeiSiPolicyPrint.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SiPolicyLib.h>

/**
  Performs silicon post-mem policy initialization.

  The returned data must be used as input data for SiliconPolicyDonePostMem (),
  and SiliconPolicyUpdateLib.SiliconPolicyUpdatePostMem ().

  @param[in, out] Policy       Pointer to policy.
  @return the initialized policy.
**/
VOID *
EFIAPI
SiliconPolicyInitPostMem (
  IN OUT VOID *Policy
  )
{
  EFI_STATUS                        Status;
  SI_POLICY_PPI                     *SiPolicyPpi;
  PEI_SI_DEFAULT_POLICY_INIT_PPI    *PeiSiDefaultPolicyInitPpi;
  PEI_SI_POLICY_PRINT_PPI           *PeiSiPolicyPrintPpi;

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Start in Post-Memory...\n"));

  ASSERT (Policy == NULL);
  SiPolicyPpi = NULL;
  PeiSiDefaultPolicyInitPpi = NULL;
  PeiSiPolicyPrintPpi = NULL;

  //
  // Locate Policy init PPI to install default silicon policy
  //
  Status = PeiServicesLocatePpi (
             &gSiDefaultPolicyInitPpiGuid,
             0,
             NULL,
             (VOID **) &PeiSiDefaultPolicyInitPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (PeiSiDefaultPolicyInitPpi != NULL) {
    Status = PeiSiDefaultPolicyInitPpi->PeiPolicyInit ();
    ASSERT_EFI_ERROR (Status);
    if (Status == EFI_SUCCESS) {
      Status = PeiServicesLocatePpi (
                 &gSiPolicyPpiGuid,
                 0,
                 NULL,
                 (VOID **) &SiPolicyPpi
                 );
      ASSERT_EFI_ERROR (Status);
    }
  }

  if (SiPolicyPpi == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to create default policy!\n"));
    return NULL;
  }

  if (PcdGetBool (PcdDumpDefaultSiliconPolicy)) {
    DEBUG ((DEBUG_INFO, "Print Default Silicon Policy...\n"));
    Status = PeiServicesLocatePpi (
               &gSiPolicyPrintPpiGuid,
               0,
               NULL,
               (VOID **) &PeiSiPolicyPrintPpi
               );
    ASSERT_EFI_ERROR (Status);
    if (PeiSiPolicyPrintPpi != NULL) {
      Status = PeiSiPolicyPrintPpi->PeiPolicyPrint ();
      ASSERT_EFI_ERROR (Status);
    }
  }

  return SiPolicyPpi;
}

/**
  The silicon post-mem policy is finalized.
  Silicon code can do initialization based upon the policy data.

  The input Policy must be returned by SiliconPolicyInitPostMem().

  @param[in] Policy       Pointer to policy.
  @retval RETURN_SUCCESS The policy is handled consumed by silicon code.
**/
RETURN_STATUS
EFIAPI
SiliconPolicyDonePostMem (
  IN VOID *Policy
  )
{
  EFI_STATUS                   Status;

  Status = SiInstallPolicyReadyPpi ();
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Done in Post-Memory\n"));

  return Status;
}
