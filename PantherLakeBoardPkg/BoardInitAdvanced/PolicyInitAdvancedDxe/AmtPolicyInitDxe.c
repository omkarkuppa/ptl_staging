/** @file
  This file is SampleCode for Intel AMT DXE Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#include "AmtPolicyInitDxe.h"

/**
  Initialize Intel AMT DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
AmtPolicyInitDxe (
  IN  EFI_HANDLE                  ImageHandle
  )
{
  EFI_STATUS                      Status;
  AMT_POLICY_PROTOCOL             *AmtPolicyProtocol;

  DEBUG ((DEBUG_INFO, "AmtPolicyInitDxe\n"));
  //
  // Call AmtCreatePolicyDefaults to initialize platform policy structure
  // and get all default policy settings.
  //
  Status = AmtCreatePolicyDefaults (&AmtPolicyProtocol);
  if (Status == EFI_UNSUPPORTED) {
    return EFI_SUCCESS;
  }

  if (mFirmwareConfiguration != FwConfigDefault) {
    UpdateDxeAmtPolicy (AmtPolicyProtocol);
  }

  //
  // Install AmtInstallPolicyProtocol.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = AmtInstallPolicyProtocol (ImageHandle, AmtPolicyProtocol);

  return Status;
}

