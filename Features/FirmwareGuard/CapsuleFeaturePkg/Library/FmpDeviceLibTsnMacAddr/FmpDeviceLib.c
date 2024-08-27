/** @file
  Produce FMP instances for TSN MAC Address Sub Region

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <PiDxe.h>
#include <LastAttemptStatus.h>
#include <Guid/SystemResourceTable.h>
#include <Library/FmpDeviceLib.h>
#include <Library/SubRegionUpdateLib.h>

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/ComponentUpdateLib.h>

FLASH_SUB_REGION_INFO mTsnMacAddrSpiSubRegion = {
  &gTsnMacAddrSectionGuid,
  FixedPcdGet32 (PcdFlashFvTsnMacAddressOffset),
  FixedPcdGet32 (PcdFlashFvTsnMacAddressSize)
};

/**
  Used to pass the FMP install function to this lib.  This allows the library to
  have control of the handle that the FMP instance is installed on.  This allows
  the library to use DriverBinding protocol model to locate its device(s) in the
  system.

  @param[in] Func  Function pointer to FMP install function.

  @retval EFI_SUCCESS       Library has saved function pointer and will call
                            function pointer on each DriverBinding Start.
  @retval EFI_UNSUPPORTED   Library doesn't use driver binding and only supports
                            a single instance.
  @retval other error       Error occurred.  Don't install FMP

**/
EFI_STATUS
EFIAPI
RegisterFmpInstaller (
  IN FMP_DEVICE_LIB_REGISTER_FMP_INSTALLER Func
  )
{
  //
  // This is a system firmware update that does not use Driver Binding Protocol
  //
  return EFI_UNSUPPORTED;
}

/**
  Provide a function to uninstall the Firmware Management Protocol instance from a
  device handle when the device is managed by a driver that follows the UEFI
  Driver Model.  If the device is not managed by a driver that follows the UEFI
  Driver Model, then EFI_UNSUPPORTED is returned.

  @param[in] FmpUninstaller  Function that installs the Firmware Management
                             Protocol.

  @retval EFI_SUCCESS      The device is managed by a driver that follows the
                           UEFI Driver Model.  FmpUinstaller must be called on
                           each Driver Binding Stop().
  @retval EFI_UNSUPPORTED  The device is not managed by a driver that follows
                           the UEFI Driver Model.
  @retval other            The Firmware Management Protocol for this firmware
                           device is not installed.  The firmware device is
                           still locked using FmpDeviceLock().

**/
EFI_STATUS
EFIAPI
RegisterFmpUninstaller (
  IN FMP_DEVICE_LIB_REGISTER_FMP_UNINSTALLER  FmpUninstaller
  )
{
  //
  // This is a system firmware update that does not use Driver Binding Protocol
  //
  return EFI_UNSUPPORTED;
}

/**
  Set the device context for the FmpDeviceLib services when the device is
  managed by a driver that follows the UEFI Driver Model.  If the device is not
  managed by a driver that follows the UEFI Driver Model, then EFI_UNSUPPORTED
  is returned.  Once a device context is set, the FmpDeviceLib services
  operate on the currently set device context.

  @param[in]      Handle   Device handle for the FmpDeviceLib services.
                           If Handle is NULL, then Context is freed.
  @param[in, out] Context  Device context for the FmpDeviceLib services.
                           If Context is NULL, then a new context is allocated
                           for Handle and the current device context is set and
                           returned in Context.  If Context is not NULL, then
                           the current device context is set.

  @retval EFI_SUCCESS      The device is managed by a driver that follows the
                           UEFI Driver Model.
  @retval EFI_UNSUPPORTED  The device is not managed by a driver that follows
                           the UEFI Driver Model.
  @retval other            The Firmware Management Protocol for this firmware
                           device is not installed.  The firmware device is
                           still locked using FmpDeviceLock().

**/
EFI_STATUS
EFIAPI
FmpDeviceSetContext (
  IN EFI_HANDLE  Handle,
  IN OUT VOID    **Context
  )
{
  //
  // This is a system firmware update that does not use Driver Binding Protocol
  //
  return EFI_UNSUPPORTED;
}

/**
  Returns the size, in bytes, of the firmware image currently stored in the
  firmware device.  This function is used to by the GetImage() and
  GetImageInfo() services of the Firmware Management Protocol.  If the image
  size can not be determined from the firmware device, then 0 must be returned.

  @param[out] Size  Pointer to the size, in bytes, of the firmware image
                    currently stored in the firmware device.

  @retval EFI_SUCCESS            The size of the firmware image currently
                                 stored in the firmware device was returned.
  @retval EFI_INVALID_PARAMETER  Size is NULL.
  @retval EFI_UNSUPPORTED        The firmware device does not support reporting
                                 the size of the currently stored firmware image.
  @retval EFI_DEVICE_ERROR       An error occured attempting to determine the
                                 size of the firmware image currently stored in
                                 in the firmware device.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetSize (
  IN UINTN  *Size
  )
{
  if (Size == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *Size = mTsnMacAddrSpiSubRegion.FvSize;
  return EFI_SUCCESS;
}

/**
  Used to return a library supplied guid that will be the ImageTypeId guid of
  the FMP descriptor.  This is optional but can be used if at runtime the guid
  needs to be determined.

  @param[out] Guid  Double Guid Ptr that will be updated to point to guid.
                    This should be from static memory and will not be freed.

  @return EFI_UNSUPPORTED  Library instance doesn't need dynamic guid.
  @return Error            Any error will cause the wrapper to use the GUID
                           defined by PCD.
  @return EFI_SUCCESS      Guid ptr should be updated to point to static memeory
                           which contains a valid guid.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetImageTypeIdGuidPtr (
  OUT EFI_GUID  **Guid
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Returns values used to fill in the AttributesSupported and AttributesSettings
  fields of the EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the
  GetImageInfo() service of the Firmware Management Protocol.  The following
  bit values from the Firmware Management Protocol may be combined:
    IMAGE_ATTRIBUTE_IMAGE_UPDATABLE
    IMAGE_ATTRIBUTE_RESET_REQUIRED
    IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED
    IMAGE_ATTRIBUTE_IN_USE
    IMAGE_ATTRIBUTE_UEFI_IMAGE

  @param[out] Supported  Attributes supported by this firmware device.
  @param[out] Setting    Attributes settings for this firmware device.

  @retval EFI_SUCCESS            The attributes supported by the firmware
                                 device were returned.
  @retval EFI_INVALID_PARAMETER  Supported is NULL.
  @retval EFI_INVALID_PARAMETER  Setting is NULL.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetAttributes (
  IN OUT UINT64  *Supported,
  IN OUT UINT64  *Setting
  )
{
  if (Supported == NULL || Setting == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *Supported = (IMAGE_ATTRIBUTE_IMAGE_UPDATABLE         |
                IMAGE_ATTRIBUTE_RESET_REQUIRED          |
                IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED |
                IMAGE_ATTRIBUTE_IN_USE
                );
  *Setting   = (IMAGE_ATTRIBUTE_IMAGE_UPDATABLE         |
                IMAGE_ATTRIBUTE_RESET_REQUIRED          |
                IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED
                );

  if (PcdGet8 (PcdSystemFirmwareFmpSupportedMode) != 0) {
    //
    // Expose ESRT entry if not in Monolithic mode.
    //
    *Setting = *Setting | IMAGE_ATTRIBUTE_IN_USE;
  }

  return EFI_SUCCESS;
}

/**
  Gets the current Lowest Supported Version.

  This is a protection mechanism so that a previous version with known issue is
  not applied.  ONLY implement this if your running firmware has a method to
  return this at runtime.  If EFI_UNSUPPORTED is returned, then the Lowest
  Supported Version is stored in a UEFI Variable.

  @param[out] Version  On return this value represents the current Lowest
                       Supported Version (in same format as GetVersion).

  @retval EFI_SUCCESS       The Lowest Supported Version was correctly retrieved
  @retval EFI_UNSUPPORTED   Device firmware doesn't support reporting LSV
  @retval EFI_DEVICE_ERROR  Error occurred when trying to get the LSV
**/
EFI_STATUS
EFIAPI
FmpDeviceGetLowestSupportedVersion (
  IN OUT UINT32  *LowestSupportedVersion
  )
{
  //
  // Retrieve the lowest support version from a PCD
  // NOTE: This method of using a PCD can only be used for the system firmware
  //       FMP instance that is updated every time the system firmware is
  //       updated.  If system firmware updates support partial updates that
  //       would not include the system firmware FMP instance, then a PCD can
  //       not be used and the value must come from the currently running system
  //       firmware image.
  //
  return EFI_UNSUPPORTED;
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
FmpDeviceGetVersionString (
  OUT CHAR16  **VersionString
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Gets the current running version.

  ONLY implement this if your running firmware has a method to return this at
  runtime.

  @param[out] Version  On return this value represents the current running
                       version.

  @retval EFI_SUCCESS       The version was correctly retrieved.
  @retval EFI_UNSUPPORTED   Device firmware doesn't support reporting current
                            version.
  @retval EFI_DEVICE_ERROR  Error occurred when trying to get the version.
**/
EFI_STATUS
EFIAPI
FmpDeviceGetVersion (
  IN OUT UINT32  *Version
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Returns the value used to fill in the HardwareInstance field of the
  EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the GetImageInfo()
  service of the Firmware Management Protocol.  If EFI_SUCCESS is returned, then
  the firmware device supports a method to report the HardwareInstance value.
  If the value can not be reported for the firmware device, then EFI_UNSUPPORTED
  must be returned.  EFI_DEVICE_ERROR is returned if an error occurs attempting
  to retrieve the HardwareInstance value for the firmware device.

  @param[out] HardwareInstance  The hardware instance value for the firmware
                                device.

  @retval EFI_SUCCESS       The hardware instance for the current firmware
                            devide is returned in HardwareInstance.
  @retval EFI_UNSUPPORTED   The firmware device does not support a method to
                            report the hardware instance value.
  @retval EFI_DEVICE_ERROR  An error occurred attempting to retrieve the hardware
                            instance value.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetHardwareInstance (
  OUT UINT64  *HardwareInstance
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Retrieves a copy of the current firmware image of the device.

  This function allows a copy of the current firmware image to be created and
  saved.  The saved copy could later been used, for example, in firmware image
  recovery or rollback.

  @param[out] Image      Points to the buffer where the current image is copied
                         to.
  @param[out] ImageSize  On entry, points to the size of the buffer pointed to
                         by Image, in bytes.  On return, points to the length of
                         the image, in bytes.

  @retval EFI_SUCCESS            The image was successfully read from the device.
  @retval EFI_BUFFER_TOO_SMALL   The buffer specified by ImageSize is too small
                                 to hold the image. The current buffer size
                                 needed to hold the image is returned in
                                 ImageSize.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.
  @retval EFI_NOT_FOUND          The current image is not copied to the buffer.
  @retval EFI_UNSUPPORTED        The operation is not supported.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetImage (
  IN OUT VOID   *Image,
  IN OUT UINTN  *ImageSize
  )
{
  EFI_STATUS Status;

  Status  = SubRegionGetImage (
              &mTsnMacAddrSpiSubRegion,
              Image,
              ImageSize
              );
  return Status;
}

/**
  Updates the firmware image of the device.

  This function updates the hardware with the new firmware image.  This function
  returns EFI_UNSUPPORTED if the firmware image is not updatable.  If the
  firmware image is updatable, the function should perform the following minimal
  validations before proceeding to do the firmware image update.
    - Validate the image is a supported image for this device.  The function
      returns EFI_ABORTED if the image is unsupported.  The function can
      optionally provide more detailed information on why the image is not a
      supported image.
    - Validate the data from VendorCode if not null.  Image validation must be
      performed before VendorCode data validation.  VendorCode data is ignored
      or considered invalid if image validation failed.  The function returns
      EFI_ABORTED if the data is invalid.

  VendorCode enables vendor to implement vendor-specific firmware image update
  policy.  Null if the caller did not specify the policy or use the default
  policy.  As an example, vendor can implement a policy to allow an option to
  force a firmware image update when the abort reason is due to the new firmware
  image version is older than the current firmware image version or bad image
  checksum.  Sensitive operations such as those wiping the entire firmware image
  and render the device to be non-functional should be encoded in the image
  itself rather than passed with the VendorCode.  AbortReason enables vendor to
  have the option to provide a more detailed description of the abort reason to
  the caller.

  @param[in]  Image             Points to the new image.
  @param[in]  ImageSize         Size of the new image in bytes.
  @param[in]  VendorCode        This enables vendor to implement vendor-specific
                                firmware image update policy. Null indicates the
                                caller did not specify the policy or use the
                                default policy.
  @param[in]  Progress          A function used by the driver to report the
                                progress of the firmware update.
  @param[in]  CapsuleFwVersion  FMP Payload Header version of the image.
  @param[out] AbortReason       A pointer to a pointer to a null-terminated
                                string providing more details for the aborted
                                operation. The buffer is allocated by this
                                function with AllocatePool(), and it is the
                                caller's responsibility to free it with a call
                                to FreePool().

  @retval EFI_SUCCESS            The device was successfully updated with the
                                 new image.
  @retval EFI_ABORTED            The operation is aborted.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.
  @retval EFI_UNSUPPORTED        The operation is not supported.

**/
EFI_STATUS
EFIAPI
FmpDeviceSetImage (
  IN  CONST VOID                                     *Image,
  IN  UINTN                                          ImageSize,
  IN  CONST VOID                                     *VendorCode,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,
  IN  UINT32                                         CapsuleFwVersion,
  OUT CHAR16                                         **AbortReason
  )
{
  UINT32  LastAttemptStatus;

  return  FmpDeviceSetImageWithStatus (
            Image,
            ImageSize,
            VendorCode,
            Progress,
            CapsuleFwVersion,
            AbortReason,
            &LastAttemptStatus
            );
}

/**
  Updates a firmware device with a new firmware image.  This function returns
  EFI_UNSUPPORTED if the firmware image is not updatable.  If the firmware image
  is updatable, the function should perform the following minimal validations
  before proceeding to do the firmware image update.
    - Validate that the image is a supported image for this firmware device.
      Return EFI_ABORTED if the image is not supported.  Additional details
      on why the image is not a supported image may be returned in AbortReason.
    - Validate the data from VendorCode if is not NULL.  Firmware image
      validation must be performed before VendorCode data validation.
      VendorCode data is ignored or considered invalid if image validation
      fails.  Return EFI_ABORTED if the VendorCode data is invalid.

  VendorCode enables vendor to implement vendor-specific firmware image update
  policy.  Null if the caller did not specify the policy or use the default
  policy.  As an example, vendor can implement a policy to allow an option to
  force a firmware image update when the abort reason is due to the new firmware
  image version is older than the current firmware image version or bad image
  checksum.  Sensitive operations such as those wiping the entire firmware image
  and render the device to be non-functional should be encoded in the image
  itself rather than passed with the VendorCode.  AbortReason enables vendor to
  have the option to provide a more detailed description of the abort reason to
  the caller.

  @param[in]  Image             Points to the new firmware image.
  @param[in]  ImageSize         Size, in bytes, of the new firmware image.
  @param[in]  VendorCode        This enables vendor to implement vendor-specific
                                firmware image update policy.  NULL indicates
                                the caller did not specify the policy or use the
                                default policy.
  @param[in]  Progress          A function used to report the progress of
                                updating the firmware device with the new
                                firmware image.
  @param[in]  CapsuleFwVersion  The version of the new firmware image from the
                                update capsule that provided the new firmware
                                image.
  @param[out] AbortReason       A pointer to a pointer to a Null-terminated
                                Unicode string providing more details on an
                                aborted operation. The buffer is allocated by
                                this function with
                                EFI_BOOT_SERVICES.AllocatePool().  It is the
                                caller's responsibility to free this buffer with
                                EFI_BOOT_SERVICES.FreePool().
  @param[out] LastAttemptStatus A pointer to a UINT32 that holds the last attempt
                                status to report back to the ESRT table in case
                                of error. This value will only be checked when this
                                function returns an error.

                                The return status code must fall in the range of
                                LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_MIN_ERROR_CODE_VALUE to
                                LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_MAX_ERROR_CODE_VALUE.

                                If the value falls outside this range, it will be converted
                                to LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL.

  @retval EFI_SUCCESS            The firmware device was successfully updated
                                 with the new firmware image.
  @retval EFI_ABORTED            The operation is aborted.  Additional details
                                 are provided in AbortReason.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.
  @retval EFI_UNSUPPORTED        The operation is not supported.

**/
EFI_STATUS
EFIAPI
FmpDeviceSetImageWithStatus (
  IN  CONST VOID                                     *Image,
  IN  UINTN                                          ImageSize,
  IN  CONST VOID                                     *VendorCode,       OPTIONAL
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,          OPTIONAL
  IN  UINT32                                         CapsuleFwVersion,
  OUT CHAR16                                         **AbortReason,
  OUT UINT32                                         *LastAttemptStatus
  )
{
  EFI_STATUS                 Status;
  Status = SubRegionWrite (
             (VOID *) Image,
             ImageSize,
             &mTsnMacAddrSpiSubRegion,
             AbortReason
             );
  DEBUG((DEBUG_ERROR, "Tsn FmpDeviceSetImageWithStatus - SetImage Done\n"));
  if (EFI_ERROR (Status)) {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_MIN_ERROR_CODE_VALUE;
  }
  return Status;
}

/**
  Checks if the firmware image is valid for the device.

  This function allows firmware update application to validate the firmware image without
  invoking the SetImage() first.

  @param[in]  Image              Points to the new image.
  @param[in]  ImageSize          Size of the new image in bytes.
  @param[out] ImageUpdatable     Indicates if the new image is valid for update. It also provides,
                                 if available, additional information if the image is invalid.

  @retval EFI_SUCCESS            The image was successfully checked.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.

**/
EFI_STATUS
EFIAPI
FmpDeviceCheckImage (
  IN  CONST VOID  *Image,
  IN  UINTN       ImageSize,
  OUT UINT32      *ImageUpdateable
  )
{
  UINT32  LastAttemptStatus;

  return FmpDeviceCheckImageWithStatus (Image, ImageSize, ImageUpdateable, &LastAttemptStatus);
}

/**
  Checks if a new firmware image is valid for the firmware device.  This
  function allows firmware update operation to validate the firmware image
  before FmpDeviceSetImage() is called.

  @param[in]  Image               Points to a new firmware image.
  @param[in]  ImageSize           Size, in bytes, of a new firmware image.
  @param[out] ImageUpdatable      Indicates if a new firmware image is valid for
                                  a firmware update to the firmware device.  The
                                  following values from the Firmware Management
                                  Protocol are supported:
                                    IMAGE_UPDATABLE_VALID
                                    IMAGE_UPDATABLE_INVALID
                                    IMAGE_UPDATABLE_INVALID_TYPE
                                    IMAGE_UPDATABLE_INVALID_OLD
                                    IMAGE_UPDATABLE_VALID_WITH_VENDOR_CODE
  @param[out] LastAttemptStatus   A pointer to a UINT32 that holds the last attempt
                                  status to report back to the ESRT table in case
                                  of error. This value will only be checked when this
                                  function returns an error.

                                  The return status code must fall in the range of
                                  LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_MIN_ERROR_CODE_VALUE to
                                  LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_MAX_ERROR_CODE_VALUE.

                                  If the value falls outside this range, it will be converted
                                  to LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL.

  @retval EFI_SUCCESS            The image was successfully checked.  Additional
                                 status information is returned in
                                 ImageUpdatable.
  @retval EFI_INVALID_PARAMETER  Image is NULL.
  @retval EFI_INVALID_PARAMETER  ImageUpdatable is NULL.

**/
EFI_STATUS
EFIAPI
FmpDeviceCheckImageWithStatus (
  IN  CONST VOID  *Image,
  IN  UINTN       ImageSize,
  OUT UINT32      *ImageUpdatable,
  OUT UINT32      *LastAttemptStatus
  )
{
  EFI_STATUS   Status;

  Status = SubRegionCheckImage (
             &mTsnMacAddrSpiSubRegion,
             Image,
             ImageSize,
             ImageUpdatable
             );
  if (EFI_ERROR (Status)) {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_MIN_ERROR_CODE_VALUE;
  }
  return Status;
}

/**
  Device frimware should trigger lock mechanism so that device fw can not be
  updated or tampered with. This lock mechanism is generally only cleared by a
  full system reset (not just sleep state/low power mode)

  @retval EFI_SUCCESS           The device was successfully locked.
  @retval EFI_UNSUPPORTED       The hardware device/firmware doesn't support locking

**/
EFI_STATUS
EFIAPI
FmpDeviceLock (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "FmpDeviceLib(TsnMacAddr): FmpDeviceLock() for system FLASH\n"));
  // FLASH security is relied on gSkipBiosLockForSysFwUpdateGuid.
  return EFI_UNSUPPORTED;
}

/**
  The constructor function.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor successfully .
**/
EFI_STATUS
EFIAPI
FmpDeviceLibTsnMacAddrConstructor (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;
  UINTN                 VariableSize;

  //
  // Determine supported mode if PCD has not been initialized.
  //
  if (PcdGet8 (PcdSystemFirmwareFmpSupportedMode) == 0xFF) {
    VariableSize = 0;
    Status = gRT->GetVariable (
                    L"LastAttemptStatus",
                    &gEfiCallerIdGuid,
                    NULL,
                    &VariableSize,
                    NULL
                    );
    if (Status == EFI_BUFFER_TOO_SMALL) {
      //
      // Last attemp status exists. Switch to standalone mode.
      //
      PcdSet8S (PcdSystemFirmwareFmpSupportedMode, 1);
    }
  }

  return EFI_SUCCESS;
}
