/** @file
  This file initialises and Installs GopPolicy Protocol.

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

#include "GopPolicyInitDxe.h"
#include <Protocol/GopPolicy.h>
#include <Library/DxeServicesLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <PlatformBoardConfig.h>

GLOBAL_REMOVE_IF_UNREFERENCED GOP_POLICY_PROTOCOL   mGOPPolicy;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_PHYSICAL_ADDRESS  mVbtAddress = 0;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32                mVbtSize    = 0;
GLOBAL_REMOVE_IF_UNREFERENCED GPIOV2_PAD            mLidGpioPad;

//
// Function implementations
//

/**
  Get GPIO PAD of lid status on this platform and save it to module variable mLidGpioPad
  for later usage.

**/
VOID
EFIAPI
InitializePlatformLidGpioPad (
  VOID
  )
{
  VPD_GPIO_PAD  *GpioVpd;

  GpioVpd     = NULL;
  mLidGpioPad = 0;

  //
  // If the platform does not support a lid, set lid status GPIO PAD as 0
  //
  if ((PcdGet8 (PcdPlatformType) == TypeTrad) && (PcdGet8 (PcdPlatformFlavor) == FlavorDesktop)) {
    DEBUG ((DEBUG_INFO, "Lid status is unsupported to GOP for DT/AIO board\n"));
    return;
  }

  GpioVpd = PcdGetPtr (VpdPcdGpioLidStatus);
  if (GpioVpd != NULL) {
    mLidGpioPad = GpioVpd->GpioPad;
  }

  DEBUG ((DEBUG_INFO, "Lid status GPIO PAD = 0x%x\n", mLidGpioPad));

  return;
}

/**
  Get current lid status on the platform if mLidGpioPad is not zero.

  @param[out] CurrentLidStatus

  @retval     EFI_SUCCESS
  @retval     EFI_UNSUPPORTED

**/
EFI_STATUS
EFIAPI
GetPlatformLidStatus (
  OUT LID_STATUS  *CurrentLidStatus
  )
{
  EFI_STATUS        Status;
  GPIOV2_PAD_STATE  GpioLidStatus;

  DEBUG ((DEBUG_VERBOSE, "LidStatus Entry\n"));

  GpioLidStatus = GpioV2StateHigh;

  //
  // If the platform does not support a lid, the function must return EFI_UNSUPPORTED
  //
  if (mLidGpioPad == 0) {
    DEBUG ((DEBUG_VERBOSE, "Returning Lid status as unsupported\n"));
    return EFI_UNSUPPORTED;
  }

  Status = GpioV2GetRx (mLidGpioPad, &GpioLidStatus);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Invalid Lid GPIO: %x\n", mLidGpioPad));
    return EFI_UNSUPPORTED;
  }

  if (GpioLidStatus == GpioV2StateHigh) {
    *CurrentLidStatus = LidOpen;
  } else {
    *CurrentLidStatus = LidClosed;
  }

  DEBUG ((DEBUG_VERBOSE, "LidStatus Exit with LidStatus = %x\n", *CurrentLidStatus));

  return EFI_SUCCESS;
}

/**

  @param[out] CurrentDockStatus

  @retval     EFI_SUCCESS
  @retval     EFI_UNSUPPORTED

**/
EFI_STATUS
EFIAPI
GetPlatformDockStatus (
  OUT DOCK_STATUS  *CurrentDockStatus
  )
{
  return EFI_UNSUPPORTED;
}

/**

  @param[out] VbtAddress
  @param[out] VbtSize

  @retval     EFI_SUCCESS
  @retval     EFI_NOT_FOUND

**/
EFI_STATUS
EFIAPI
GetVbtData (
  OUT EFI_PHYSICAL_ADDRESS  *VbtAddress,
  OUT UINT32                *VbtSize
  )
{
  EFI_STATUS  Status;
  VOID        *Buffer;
  UINTN       VbtBufferSize;

  Status        = EFI_NOT_FOUND;
  Buffer        = NULL;
  VbtBufferSize = 0;

  if (mVbtAddress != 0) {
    *VbtAddress = mVbtAddress;
    *VbtSize    = mVbtSize ;
    return EFI_SUCCESS;
  }

  Status = GetSectionFromAnyFv (
             PcdGetPtr (PcdIntelGraphicsVbtFileGuid),
             EFI_SECTION_RAW,
             0,
             &Buffer,
             &VbtBufferSize
           );
  if (!EFI_ERROR(Status)) {
    *VbtAddress = (EFI_PHYSICAL_ADDRESS) Buffer;
    *VbtSize = (UINT32) VbtBufferSize;

    mVbtAddress = *VbtAddress;
    mVbtSize    = *VbtSize;
  } else {
    DEBUG ((DEBUG_WARN, "VBT is not found as part of GetVbtData()\n"));
    Status = EFI_NOT_FOUND;
  }
  return Status;
}

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
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS  Status;

  if (FALSE == IGpuIsSupported ()) {
    return EFI_SUCCESS;
  }

  //
  // Initialize the EFI Driver Library
  //
  SetMem (&mGOPPolicy, sizeof (GOP_POLICY_PROTOCOL), 0);

  mGOPPolicy.Revision              = GOP_POLICY_PROTOCOL_REVISION_03;
  mGOPPolicy.GetPlatformLidStatus  = GetPlatformLidStatus;
  mGOPPolicy.GetVbtData            = GetVbtData;
  mGOPPolicy.GetPlatformDockStatus = GetPlatformDockStatus;

  //
  // Initialize module variable for GetPlatformLidStatus() callback function.
  //
  InitializePlatformLidGpioPad ();

  //
  // Install protocol to allow access to this Policy.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gGopPolicyProtocolGuid,
                  &mGOPPolicy,
                  NULL
                  );

  return Status;
}
