/** @file
  This file is the library for CPU DXE Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Library/DxeCpuPolicyUpdateLib.h>
#include <Library/PcdLib.h>

/**
  This function prints the CPU DXE phase policy.

  @param[in] DxeCpuPolicy - CPU DXE Policy protocol
**/
VOID
CpuDxePrintPolicyProtocol (
  IN  DXE_CPU_POLICY_PROTOCOL  *DxeCpuPolicy
  )
{
  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "\n------------------------ CPU Policy (DXE) print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : %x\n", DxeCpuPolicy->Revision));
  ASSERT (DxeCpuPolicy->Revision == DXE_CPU_POLICY_PROTOCOL_REVISION);
  DEBUG ((DEBUG_INFO, "DgrEnable : %x\n", DxeCpuPolicy->DgrEnable));
  DEBUG ((DEBUG_INFO, "DgrSpaEnable : %x\n", DxeCpuPolicy->DgrSpaEnable));
  DEBUG ((DEBUG_INFO, "SmmMsrSaveStateEnable : %x\n", DxeCpuPolicy->SmmMsrSaveStateEnable));
  DEBUG ((DEBUG_INFO, "SmmUseDelayIndication : %x\n", DxeCpuPolicy->SmmUseDelayIndication));
  DEBUG ((DEBUG_INFO, "SmmUseBlockIndication : %x\n", DxeCpuPolicy->SmmUseBlockIndication));
  DEBUG ((DEBUG_INFO, "SmmUseSmmEnableIndication : %x\n", DxeCpuPolicy->SmmUseSmmEnableIndication));
  DEBUG ((DEBUG_INFO, "SmmProcTraceEnable : %x\n", DxeCpuPolicy->SmmProcTraceEnable));
  DEBUG ((DEBUG_INFO, "\n------------------------ CPU Policy (DXE) print END -----------------\n"));
  DEBUG_CODE_END ();
}

/**
  Get data for CPU policy from setup options.

  @param[in] DxeCpuPolicy              The pointer to get CPU Policy protocol instance

  @retval EFI_SUCCESS                  Operation success.

**/
EFI_STATUS
EFIAPI
UpdateDxeSiCpuPolicy (
  IN OUT  DXE_CPU_POLICY_PROTOCOL  *DxeCpuPolicy
  )
{
  UINTN                     VariableSize;
  CPU_SETUP                 CpuSetup;
  EFI_STATUS                Status;

#if FixedPcdGetBool (PcdPpamEnable) == 1
  DxeCpuPolicy->DgrEnable = 1;
#endif

  //
  // Setup options for below settings are removed as they are
  // not being used and hardcoded with default values.
  //
  DxeCpuPolicy->SmmMsrSaveStateEnable = 0;
  DxeCpuPolicy->SmmUseDelayIndication = 0;
  DxeCpuPolicy->SmmUseBlockIndication = 0;
  DxeCpuPolicy->SmmUseSmmEnableIndication = 0;

  VariableSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &CpuSetup
                  );

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  DxeCpuPolicy->SmmProcTraceEnable = CpuSetup.SmmProcTraceEnable;

  return EFI_SUCCESS;
}

/**
  CpuInstallPolicyProtocol installs CPU Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] DxeCpuPolicy               The pointer to CPU Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
CpuInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  DXE_CPU_POLICY_PROTOCOL     *DxeCpuPolicy
  )
{
  EFI_STATUS            Status;

  ///
  /// Print CPU DXE Policy
  ///
  CpuDxePrintPolicyProtocol (DxeCpuPolicy);

  ///
  /// Install the DXE_CPU_POLICY_PROTOCOL interface
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gDxeCpuPolicyProtocolGuid,
                  DxeCpuPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

