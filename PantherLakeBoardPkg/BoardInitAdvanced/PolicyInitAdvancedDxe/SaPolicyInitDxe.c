/** @file
  This file is SampleCode for SA DXE Policy initialization.

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
#include "SaPolicyInitDxe.h"


//
// Function implementations
//

/**
  Initialize SA DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
SaPolicyInitDxe (
  IN EFI_HANDLE                   ImageHandle
  )
{
  EFI_STATUS               Status;
  IGPU_POLICY_PROTOCOL     *IGpuPolicy;

  //
  // Call CreateIGpuDxeConfigBlocks to create & initialize platform policy structure
  // and get all Intel default policy settings.
  //
  Status = CreateIGpuDxeConfigBlocks (&IGpuPolicy);
  DEBUG ((DEBUG_INFO, "IGpuPolicy->TableHeader.NumberOfBlocks = 0x%x\n ", IGpuPolicy->TableHeader.NumberOfBlocks));
  ASSERT_EFI_ERROR (Status);

  if (mFirmwareConfiguration != FwConfigDefault) {

    UpdateDxeSaPolicy (IGpuPolicy);
    GetGopVbtData();
  }

  //
  // Install IGpuInstallPolicyProtocol.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = IGpuInstallPolicyProtocol (ImageHandle, IGpuPolicy);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Function to register call back event to get GOP VBT data.

  @retval EFI_SUCCESS           Call back registered successfully.
  @retval EFI_UNSUPPORTED:      Call back registration fails.

**/
EFI_STATUS
GetGopVbtData (
  VOID
  )
{
  EFI_STATUS             Status;
  VOID                   *Registration;
  EFI_EVENT              Event;
  GOP_POLICY_PROTOCOL    *GopPolicy;

  ///
  /// If device 0:2:0 (Internal Graphics Device, or GT) is not present, skip it.
  ///
  if ((IGpuGetVendorId () == 0xFFFF)) {
    DEBUG((DEBUG_INFO, "IGD is not present, skip GetGopVbtData ()\n"));
    return EFI_SUCCESS;
  }

  Status = gBS->LocateProtocol (&gGopPolicyProtocolGuid, NULL, (VOID **) &GopPolicy);
  if (Status == EFI_NOT_FOUND) {
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    (EFI_EVENT_NOTIFY) UpdateSaPolicyGopVbtPointer,
                    NULL,
                    &Event
                    );
    ASSERT_EFI_ERROR (Status);
    Status = gBS->RegisterProtocolNotify (
                    &gGopPolicyProtocolGuid,
                    Event,
                    &Registration
                    );
    ASSERT_EFI_ERROR (Status);
  } else {
    UpdateSaPolicyGopVbtPointer (NULL, NULL);
  }

  return EFI_SUCCESS;
}


/**
  Callback function to Update SaPolicyGopVbtPointer and Size.

  @param[in] Event    - The triggered event.
  @param[in] Context  - Context for this event.

**/
VOID
UpdateSaPolicyGopVbtPointer (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_STATUS                       Status;
  IGPU_POLICY_PROTOCOL             *IGpuPolicy;
  GOP_POLICY_PROTOCOL              *GopPolicy;
  EFI_PHYSICAL_ADDRESS             VbtAddress;
  UINT32                           Size;
  IGPU_DXE_CONFIG                  *IGpuDxeConfig;

  Status = gBS->LocateProtocol (&gIGpuPolicyProtocolGuid, NULL, (VOID **) &IGpuPolicy);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = GetConfigBlock ((VOID *)IGpuPolicy, &gIGpuDxeConfigGuid, (VOID *)&IGpuDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Locate GopPolicy and GetVbtData\n"));
  ///
  /// Locate the GOP Policy Protocol.
  ///
  GopPolicy = NULL;
  Status = gBS->LocateProtocol (&gGopPolicyProtocolGuid, NULL, (VOID **) &GopPolicy);
  ASSERT (GopPolicy != NULL);
  if (EFI_ERROR (Status) || (GopPolicy == NULL)) {
    return;
  }

  ///
  /// Get VBT data
  ///
  VbtAddress      = 0;
  Size            = 0;
  DEBUG ((DEBUG_INFO, "GetVbtData\n"));
  Status          = GopPolicy->GetVbtData (&VbtAddress, &Size);
  if (EFI_SUCCESS != Status) {
    DEBUG ((DEBUG_WARN, "Not able to GetVbtData in UpdateSaPolicyGopVbtPointer, Status = %r \n", Status));
  }

  IGpuDxeConfig->VbtAddress = VbtAddress;
  IGpuDxeConfig->Size = Size;

  return;
}