/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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
#ifndef _DXE_CPU_POLICY_UPDATE_LIB_H_
#define _DXE_CPU_POLICY_UPDATE_LIB_H_

#include <PiDxe.h>
#include <SetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/CpuPolicyProtocol.h>

/**

  This function prints the CPU DXE phase policy.

  @param[in] DxeCpuPolicy - CPU DXE Policy protocol

**/
VOID
CpuDxePrintPolicyProtocol (
  IN  DXE_CPU_POLICY_PROTOCOL  *DxeCpuPolicy
  );

/**

Routine Description:

  This function updates Dxe Cpu Policy Protocol

Arguments:

  @param[in] DxeCpuPolicy                 The Cpu Policy protocol instance

Returns:

  @retval EFI_SUCCESS                     Initialization complete.
  @retval EFI_UNSUPPORTED                 The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES            Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR                Device error, driver exits abnormally.

**/
EFI_STATUS
EFIAPI
UpdateDxeSiCpuPolicy (
  IN OUT  DXE_CPU_POLICY_PROTOCOL  *DxeCpuPolicy
  );

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
  );

#endif
