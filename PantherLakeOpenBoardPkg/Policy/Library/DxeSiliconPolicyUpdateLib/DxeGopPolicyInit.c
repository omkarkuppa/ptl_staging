/**@file
  This file initialises and Installs GopPolicy Protocol.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/GopPolicy.h>

GLOBAL_REMOVE_IF_UNREFERENCED GOP_POLICY_PROTOCOL        mGOPPolicy;

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
  UINTN                         FvProtocolCount;
  EFI_HANDLE                    *FvHandles;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *Fv;
  UINTN                         Index;
  UINT32                        AuthenticationStatus;
  UINT8                         *Buffer;
  UINTN                         VbtBufferSize;


  Status = EFI_NOT_FOUND;
  Fv = NULL;
  Buffer = 0;
  FvHandles = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &FvProtocolCount,
                  &FvHandles
                  );
  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < FvProtocolCount; Index++) {
      Status = gBS->HandleProtocol (
                      FvHandles[Index],
                      &gEfiFirmwareVolume2ProtocolGuid,
                      (VOID **) &Fv
                      );
      VbtBufferSize = 0;
      Status = Fv->ReadSection (
                     Fv,
                     PcdGetPtr (PcdIntelGraphicsVbtFileGuid),
                     EFI_SECTION_RAW,
                     0,
                     (VOID **) &Buffer,
                     &VbtBufferSize,
                     &AuthenticationStatus
                     );
      if (!EFI_ERROR (Status)) {
        *VbtAddress = (EFI_PHYSICAL_ADDRESS)Buffer;
        *VbtSize = (UINT32)VbtBufferSize;
        Status = EFI_SUCCESS;
        break;
      } else {
        Status = EFI_NOT_FOUND;
      }
    }
  } else {
    Status = EFI_NOT_FOUND;
  }

  if (FvHandles != NULL) {
    FreePool (FvHandles);
    FvHandles = NULL;
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
  EFI_STATUS         Status;

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
