/** @file
  This file is SampleCode for Intel Silicon DXE Platform Policy initialzation.

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
#include <SiliconPolicyInitDxe.h>
#include <Library/BaseLib.h>
#include <FspsUpd.h>
#include <IndustryStandard/FirmwareVersionInfo.h>

DXE_SI_POLICY_PROTOCOL mSiPolicyData  = { 0 };

/**
  Initilize Intel Cpu DXE Platform Policy

  @param[in] ImageHandle        Image handle of this driver.

  @retval EFI_SUCCESS           Initialization complete.
  @exception EFI_UNSUPPORTED    The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
SiliconPolicyInitDxe (
  IN EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS Status;
  #if FixedPcdGet8(PcdFspModeSelection) == 1
    VOID       *FspsUpd;
    FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
    ASSERT (FspsUpd != NULL);
    //
    // Update FspEventHandler to NULL to prevent locating PeiServices
    // after transition to DXE phase.
    //
    ((FSPS_UPD *) FspsUpd)->FspsArchUpd.FspEventHandler = 0;
  #endif

  mSiPolicyData.Revision                         = DXE_SI_POLICY_PROTOCOL_REVISION;
  mSiPolicyData.SmbiosOemTypeFirmwareVersionInfo = INTEL_FVI_SMBIOS_TYPE;
  ///
  /// Install the DXE_SI_POLICY_PROTOCOL interface
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gDxeSiPolicyProtocolGuid,
                  &mSiPolicyData,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  return Status;
}
