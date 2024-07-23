/** @file
  This file is SampleCode of the library for Intel CPU PEI Debug Pre Memory Policy initialization.

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
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PcdLib.h>


/**
  This function performs CPU PEI Debug Pre-Memory Policy initialization.

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyDebugPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  CPU_SETUP                       CpuSetup;

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
  VariableSize = sizeof (CPU_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"CpuSetup",
                               &gCpuSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &CpuSetup
                               );
  ASSERT_EFI_ERROR (Status);

  //
  // If Monitor Mwait Disabled. ApIdleManner should not be MWait Loop.
  // Set ApIdleManner to Halt Loop as workaround.
  //
  if ((CpuSetup.MonitorMwait == 0) && (CpuSetup.ApIdleManner == 2)) {
    CpuSetup.ApIdleManner = 1;
  }
#if (FixedPcdGet8(PcdFspModeSelection) == 1)
  PcdSet8S (PcdCpuApLoopMode, (UINT8) CpuSetup.ApIdleManner);
#else
  PcdSetEx8S (&gUefiCpuPkgTokenSpaceGuid, PcdCpuApLoopMode, (UINT8) CpuSetup.ApIdleManner);
#endif

  return EFI_SUCCESS;
}