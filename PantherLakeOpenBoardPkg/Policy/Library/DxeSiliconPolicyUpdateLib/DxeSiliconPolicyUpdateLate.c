/**@file

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
#include <PiDxe.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

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
  );

/**
  Initialize PCH DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
PchPolicyInitDxe (
  IN EFI_HANDLE           ImageHandle
  );

/**
  Initialize GOP DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
GopPolicyInitDxe (
  IN EFI_HANDLE           ImageHandle
  );

/**
  Performs silicon late policy update.

  The meaning of Policy is defined by silicon code.
  It could be the raw data, a handle, a Protocol, etc.

  The input Policy must be returned by SiliconPolicyDoneLate().

  In FSP or non-FSP path, the board may use additional way to get
  the silicon policy data field based upon the input Policy.

  @param[in, out] Policy       Pointer to policy.

  @return the updated policy.
**/
VOID *
EFIAPI
SiliconPolicyUpdateLate (
  IN OUT VOID *Policy
  )
{
  EFI_STATUS               Status;

  if (PcdGetBool (PcdAdvancedFeatureEnable) == FALSE) {
    SaPolicyInitDxe (gImageHandle);
    PchPolicyInitDxe (gImageHandle);

    if (PcdGetBool (PcdIntelGopBinEnable)) {
      //
      // GOP Dxe Policy Initialization
      //
      Status = GopPolicyInitDxe (gImageHandle);
      DEBUG ((DEBUG_INFO, "GOP Dxe Policy Initialization done\n"));
      ASSERT_EFI_ERROR (Status);
    }
  }

  return Policy;
}

