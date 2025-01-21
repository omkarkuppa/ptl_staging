/** @file
  Platform hook to get EC information on platform or perfrom platform specific actions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <Uefi.h>
#include <Protocol/Spi.h>
#include <Library/PlatformEcUpdateHookLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <BootStateLib.h>

//
// EC REVISION string length is from PCD(PcdEcMajorRevision, PcdEcMinorRevision, PcdEcPlatformMajorRevision, PcdEcBuildRevision)
//
#define EC_REVISION_STRING_MAX_SIZE     (sizeof (L"###.###.###.###"))
#define EC_RECOVERY_IMG_OFFSET_DEFAULT  0x48000

/**
  Returns the offset of EC recovery image within EC image.

**/
UINT32
EFIAPI
PlatformGetEcRecoveryImageOffset (
  VOID
  )
{
  EFI_STATUS        Status;
  UINT32            Address;
  UINT32            Size;
  PCH_SPI_PROTOCOL  *SpiProtocol;

  SpiProtocol = NULL;
  Address     = 0;
  Size        = EC_RECOVERY_IMG_OFFSET_DEFAULT;

  //
  // Locate the SPI protocol.
  //
  Status = gBS->LocateProtocol (
                  &gPchSpiProtocolGuid,
                  NULL,
                  (VOID **)&SpiProtocol
                  );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    goto Exit;
  }

  //
  // Get the size of EC region.
  //
  Status = SpiProtocol->GetRegionAddress (
                          SpiProtocol,
                          FlashRegionEC,
                          &Address,
                          &Size
                          );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    goto Exit;
  }

Exit:
  if ((Size % 2) != 0) {
    ASSERT (FALSE);
    Size -= 1;
  }

  return (Size / 2);
}

/**
  Returns the Null-terminated Unicode string that is used to fill in the
  VersionName field of the EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is
  returned by the GetImageInfo() service of the Firmware Management Protocol.
  The returned string must be allocated using EFI_BOOT_SERVICES.AllocatePool().

  @note It is recommended that all firmware devices support a method to report
        the VersionName string from the currently stored firmware image.

  @param[out] VersionString  The version string retrieved from the currently
                             stored firmware image.

  @retval EFI_SUCCESS            The version string of currently stored
                                 firmware image was returned in Version.
  @retval EFI_INVALID_PARAMETER  VersionString is NULL.
  @retval EFI_UNSUPPORTED        The firmware device does not support a method
                                 to report the version string of the currently
                                 stored firmware image.
  @retval EFI_DEVICE_ERROR       An error occurred attempting to retrieve the
                                 version string of the currently stored
                                 firmware image.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources to allocate the
                                 buffer for the version string of the currently
                                 stored firmware image.

**/
EFI_STATUS
EFIAPI
PlatformFmpGetEcVersionString (
  OUT CHAR16  **VersionString
  )
{
  if (VersionString == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *VersionString = (CHAR16 *) AllocateZeroPool (EC_REVISION_STRING_MAX_SIZE);
  if (*VersionString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  UnicodeSPrint (
    *VersionString,
    EC_REVISION_STRING_MAX_SIZE,
    L"%d.%d.%d.%d",
    PcdGet8 (PcdEcMajorRevision),
    PcdGet8 (PcdEcMinorRevision),
    PcdGet8 (PcdEcPlatformMajorRevision),
    PcdGet8 (PcdEcBuildRevision)
    );

  return EFI_SUCCESS;
}

/**
  Returns EC FMP version on the platform

  Returns the value used to fill in the Version field of the
  EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the GetImageInfo()
  service of the Firmware Management Protocol.  If EFI_SUCCESS is returned, then
  the firmware device supports a method to report the Version value from the
  currently stored firmware image.  If the value can not be reported for the
  firmware image currently stored in the firmware device, then EFI_UNSUPPORTED
  must be returned.  EFI_DEVICE_ERROR is returned if an error occurs attempting
  to retrieve the LowestSupportedVersion value for the currently stored firmware
  image.

  @note It is recommended that all firmware devices support a method to report
        the Version value from the currently stored firmware image.

  @param[out] Version  The version value retrieved from the currently stored
                       firmware image.

  @retval EFI_SUCCESS       The version of currently stored firmware image was
                            returned in Version.
  @retval EFI_UNSUPPORTED   The firmware device does not support a method to
                            report the version of the currently stored firmware
                            image.
  @retval EFI_DEVICE_ERROR  An error occurred attempting to retrieve the version
                            of the currently stored firmware image.

**/
EFI_STATUS
EFIAPI
PlatformFmpGetEcVersion (
  OUT UINT32  *Version
  )
{
  *Version = (UINT32) ((
    (PcdGet8 (PcdEcMajorRevision)         << 24) +
    (PcdGet8 (PcdEcMinorRevision)         << 16) +
    (PcdGet8 (PcdEcPlatformMajorRevision) << 8)  +
    (PcdGet8 (PcdEcBuildRevision))
    ));

  return EFI_SUCCESS;
}

/**
  A function used to check if the EC image is compatible to current platform.

  @param[in]  Image           Points to a new EC image.
  @param[in]  ImageSize       Size, in bytes, of the new EC image.
  @param[out] ImageUpdatable  Indicates if a EC firmware image is valid for current platform.

**/
EFI_STATUS
EFIAPI
PlatformFmpCheckEcCompatibility (
  IN  CONST VOID  *EcImage,
  IN  UINTN       EcImageSize,
  OUT BOOLEAN     *EcImageUpdatable
  )
{
  return EFI_UNSUPPORTED;
}

/**
  A function used to perform platform related task before EC update.

  @retval EFI_SUCCESS           Platform required actions are perfromed successfully.
  @retval Others                Platform required actions are failed to execute.

**/
EFI_STATUS
EFIAPI
PlatformFmpEcUpdatePreHook (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  A function used to perform platform related task after EC update.

  @param[in]   UpdateStatus     The status of EC update.

  @retval EFI_SUCCESS           Platform required actions are perfromed successfully.
  @retval Others                Platform required actions are failed to execute.

**/
EFI_STATUS
EFIAPI
PlatformFmpEcUpdatePostHook (
  IN EFI_STATUS            UpdateStatus
  )
{
  if (UpdateStatus == EFI_SUCCESS) {
    //
    // Enforce booting with full configuration after flash
    //
    UnsetBootState();
  }

  return EFI_SUCCESS;
}
