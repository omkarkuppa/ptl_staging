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

GLOBAL_REMOVE_IF_UNREFERENCED GOP_POLICY_PROTOCOL        mGOPPolicy;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_PHYSICAL_ADDRESS       gVbtAddress = 0;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32                     gVbtSize = 0;

//
// Function implementations
//

/**

  @param[out] CurrentLidStatus

  @retval     EFI_SUCCESS
  @retval     EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
GetPlatformLidStatus (
  OUT LID_STATUS *CurrentLidStatus
  )
{
  EFI_STATUS              Status;
  GPIOV2_PAD_STATE        GpioLidStatus;
  VPD_GPIO_PAD            *GpioVpd;

  GpioLidStatus = GpioV2StateHigh;
  GpioVpd = NULL;

  DEBUG ((DEBUG_INFO, "LidStatus Entry\n"));

  //
  // If the platform does not support a lid, the function must return EFI_UNSUPPORTED
  //
  if (PcdGet8 (PcdPlatformType) == TypeTrad && PcdGet8 (PcdPlatformFlavor) == FlavorDesktop) {
    DEBUG ((DEBUG_INFO, "Returning Lid status as unsupported to GOP for DT/AIO board\n"));
    return EFI_UNSUPPORTED;
  } else {
    GpioVpd = PcdGetPtr(VpdPcdGpioLidStatus);
    if (GpioVpd->GpioPad != 0x0) {
      Status = GpioV2GetRx (GpioVpd->GpioPad, &GpioLidStatus);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Invalid Lid GPIO: %x\n", GpioVpd->GpioPad));
        return EFI_UNSUPPORTED;
      }
      if (GpioLidStatus == GpioV2StateHigh) {
        *CurrentLidStatus = LidOpen;
      } else {
        *CurrentLidStatus = LidClosed;
      }
      DEBUG ((DEBUG_INFO, "LidStatus Exit\n"));
      return EFI_SUCCESS;
    }
  }

  DEBUG ((DEBUG_INFO, "LidStatus UnSupported\n"));
  return EFI_UNSUPPORTED;
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
  OUT EFI_PHYSICAL_ADDRESS *VbtAddress,
  OUT UINT32               *VbtSize
  )
{
  EFI_STATUS                    Status;
  VOID                          *Buffer;
  UINTN                         VbtBufferSize;

  Status = EFI_NOT_FOUND;
  Buffer    = NULL;
  VbtBufferSize = 0;

  if (gVbtAddress != 0) {
    *VbtAddress = gVbtAddress;
    *VbtSize = gVbtSize ;
    return EFI_SUCCESS;
  }
  Status = GetSectionFromAnyFv (
             PcdGetPtr(PcdIntelGraphicsVbtFileGuid),
             EFI_SECTION_RAW,
             0,
             &Buffer,
             &VbtBufferSize
           );
  if (!EFI_ERROR(Status)) {
    *VbtAddress = (EFI_PHYSICAL_ADDRESS) Buffer;
    *VbtSize = (UINT32) VbtBufferSize;

    gVbtAddress = *VbtAddress;
    gVbtSize = *VbtSize;
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
  IN EFI_HANDLE       ImageHandle
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

  mGOPPolicy.Revision                = GOP_POLICY_PROTOCOL_REVISION_03;
  mGOPPolicy.GetPlatformLidStatus    = GetPlatformLidStatus;
  mGOPPolicy.GetVbtData              = GetVbtData;
  mGOPPolicy.GetPlatformDockStatus   = GetPlatformDockStatus;

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
