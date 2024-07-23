/**@file
  This file initialises and Installs GopPolicy Protocol.

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

@par Specification
**/
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Protocol/IGpuPolicy.h>

/**
  Initialize SA DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
SaPolicyInitDxe (
  IN EFI_HANDLE           ImageHandle
  )
{
  EFI_STATUS             Status;
  EFI_HANDLE             SaHandle;
  IGPU_POLICY_PROTOCOL   *IGpuPolicy;

  //
  // Call CreateIGpuDxeConfigBlocks to create & initialize platform policy structure
  // and get all Intel default policy settings.
  //
  Status = CreateIGpuDxeConfigBlocks (&IGpuPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // Install IGpuInstallPolicyProtocol.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  SaHandle = NULL;
  Status = IGpuInstallPolicyProtocol (SaHandle, IGpuPolicy);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
