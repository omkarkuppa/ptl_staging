/** @file
  This file is SampleCode for PCH DXE Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#include "PchPolicyInitDxe.h"

//
// Function implementations
//

/**
  Initialize PCH DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
PchPolicyInitDxe (
  IN EFI_HANDLE                   ImageHandle
  )
{
  EFI_STATUS               Status;
  PCH_POLICY_PROTOCOL      *PchPolicy;

  //
  // Call CreatePchDxeConfigBlocks to create & initialize platform policy structure
  // and get all Intel default policy settings.
  //
  Status = CreatePchDxeConfigBlocks (&PchPolicy);
  DEBUG ((DEBUG_INFO, "PchPolicy->TableHeader.NumberOfBlocks = 0x%x\n", PchPolicy->TableHeader.NumberOfBlocks));
  ASSERT_EFI_ERROR (Status);

  if (mFirmwareConfiguration != FwConfigDefault) {
    UpdateDxePchPolicy (PchPolicy);
  }

  //
  // Install PchInstallPolicyProtocol.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = PchInstallPolicyProtocol (ImageHandle, PchPolicy);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
