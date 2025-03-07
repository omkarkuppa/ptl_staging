/** @file
  This file is SampleCode for Intel Silicon DXE Platform Policy initialization.

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

  @par Specification
**/
#include <CpuPolicyInitDxe.h>

DXE_CPU_POLICY_PROTOCOL mCpuPolicyData;

/**
  Initilize Intel CPU DXE Platform Policy

  @param[in] ImageHandle        Image handle of this driver.

  @retval EFI_SUCCESS           Initialization complete.
  @exception EFI_UNSUPPORTED    The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
CpuPolicyInitDxe (
  IN EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS                Status;

  ZeroMem (&mCpuPolicyData, sizeof (DXE_CPU_POLICY_PROTOCOL));
  mCpuPolicyData.Revision = DXE_CPU_POLICY_PROTOCOL_REVISION;

  UpdateDxeSiCpuPolicy (&mCpuPolicyData);

  //
  // Install CpuInstallPolicyProtocol.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = CpuInstallPolicyProtocol (ImageHandle, &mCpuPolicyData);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
