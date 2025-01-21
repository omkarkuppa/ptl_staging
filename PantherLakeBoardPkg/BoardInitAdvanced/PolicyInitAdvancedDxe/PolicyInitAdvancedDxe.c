/** @file
  This file is a wrapper for Platform Policy Advanced driver. Get Setup
  Value to initialize Intel DXE Platform Policy.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#include "PolicyInitAdvancedDxe.h"
#include <Library/PreSiliconEnvDetectLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT8            mFirmwareConfiguration = 0;

/**
  Initialize  DXE Platform Policy

  @param[in] ImageHandle       Image handle of this driver.
  @param[in] SystemTable       Global system service table.

  @retval EFI_SUCCESS           Initialization complete.
  @exception EFI_UNSUPPORTED    The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
PolicyInitAdvancedDxeEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS           Status;
  UINTN                VarDataSize;
  SETUP_DATA           *SystemConfiguration = NULL;


  if (IsSimicsEnvironment () || IsHfpgaEnvironment () ) {
    Status = PcdSetBoolS (PcdResetOnMemoryTypeInformationChange, FALSE);
  } else {
    Status = PcdSetBoolS (PcdResetOnMemoryTypeInformationChange, TRUE);
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: PcdSetBoolS failed @ line %d\n",__LINE__));
    ASSERT_EFI_ERROR (Status);
  } else {
    DEBUG ((DEBUG_INFO, "Info: PcdResetOnMemoryTypeInformationChange is now False\n"));
  }

  VarDataSize = sizeof (SETUP_DATA);

  SystemConfiguration = (SETUP_DATA *) AllocateZeroPool (VarDataSize);

  mFirmwareConfiguration = 0;
  if (SystemConfiguration == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to allocate memory to get System Configuration!\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  } else {
    Status = gRT->GetVariable (
                    L"Setup",
                    &gSetupVariableGuid,
                    NULL,
                    &VarDataSize,
                    SystemConfiguration
                    );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Fail to get System Configuration to identify the initial policy mode!\n"));
    } else {
      mFirmwareConfiguration = SystemConfiguration->FirmwareConfiguration;
    }
  }
  //
  // SystemAgent Dxe Platform Policy Initialization
  //
  Status = SaPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "SystemAgent Dxe Platform Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

  //
  // PCH Dxe Platform Policy Initialization
  //
  Status = PchPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "PCH Dxe Platform Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

  //
  // ME DXE Policy Initialization
  //
  Status = MePolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "ME DXE Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

  //
  // AMT DXE Policy Initialization
  //
#if FixedPcdGetBool (PcdAmtEnable) == 1
  Status = AmtPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "AMT DXE Policy Initialization done\n"));
#endif

  //
  // Silicon Dxe Platform Policy Initialization
  //
  Status = SiliconPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "Silicon Dxe Platform Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

  //
  // CPU DXE Platform Policy Initialization
  //
  Status = CpuPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "Cpu Dxe Platform Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);


  if (PcdGetBool (PcdIntelGopBinEnable)) {
    //
    // GOP Dxe Policy Initialization
    //
    Status = GopPolicyInitDxe (ImageHandle);
    DEBUG ((DEBUG_INFO, "GOP Dxe Policy Initialization done\n"));
    ASSERT_EFI_ERROR (Status);
  }

  //
  // TBT Dxe Platform Policy Initialization
  //
  Status = TbtPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "TBT Dxe Platform Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

  return Status;
}
