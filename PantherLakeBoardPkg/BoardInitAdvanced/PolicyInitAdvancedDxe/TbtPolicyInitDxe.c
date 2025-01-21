/** @file
  This file is SampleCode for TBT DXE Policy initialization.

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
#include "TbtPolicyInitDxe.h"
#if FixedPcdGetBool (PcdDTbtEnable) == 1
#include <Library/DxeDTbtPolicyLib.h>
#endif


//
// Function implementations
//

/**
  Initialize TBT DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
TbtPolicyInitDxe (
  IN EFI_HANDLE                   ImageHandle
  )
{
  EFI_STATUS               Status = EFI_SUCCESS;
  ITBT_POLICY_PROTOCOL     *ITbtPolicy;
  ITbtPolicy = NULL;

  //
  // Call CreateITbtDxeConfigBlocks to create & initialize platform policy structure
  // and get all Intel default policy settings.
  //
  Status = CreateITbtDxeConfigBlocks (&ITbtPolicy);
  if ((Status == EFI_SUCCESS) && (ITbtPolicy != NULL)) {
    DEBUG ((DEBUG_INFO, "ITbtPolicy->TableHeader.NumberOfBlocks = 0x%x\n ", ITbtPolicy->TableHeader.NumberOfBlocks));
  } else {
    DEBUG ((DEBUG_INFO, "Create iTBT Dxe config block failed\n"));
  }

#if FixedPcdGetBool (PcdDTbtEnable) == 1
  //
  // Install DTBT Policy
  //
  Status = DTbtInstallPolicyProtocol (ImageHandle);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Install DTbt Policy done\n"));
  }
#endif

  if (mFirmwareConfiguration != FwConfigDefault) {
    UpdateDxeTbtPolicy (ITbtPolicy);
  }
  //
  // Install ITbtInstallPolicyProtocol.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  if (ITbtPolicy != NULL) {
    Status = ITbtInstallPolicyProtocol (ImageHandle, ITbtPolicy);
  }
  return Status;
}
