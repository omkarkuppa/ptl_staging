/** @file
  FmpDeviceLib instance to support PD Bridge Firmware update

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
#include <FmpLastAttemptStatus.h>
#include <UsbCPdBridgeRetimer.h>
#include <Library/FmpDeviceLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UsbCPdBridgeUpdateLib.h>
#include <Guid/SystemResourceTable.h>
#include <Protocol/UsbCPdBridgeProtocol.h>
#include <Library/UsbcCapsuleDebugLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>

/**
  Used to pass the FMP install function to this lib.  This allows the library to
  have control of the handle that the FMP instance is installed on.  This allows
  the library to use DriverBinding protocol model to locate its device(s) in the
  system.

  @param[in] Func           Function pointer to FMP install function.

  @retval EFI_UNSUPPORTED   Library doesn't use driver binding and only supports
                            a single instance.

**/
EFI_STATUS
EFIAPI
RegisterFmpInstaller (
  IN FMP_DEVICE_LIB_REGISTER_FMP_INSTALLER  Func
  )
{
  ///
  /// This is a system firmware update that does not use Driver Binding Protocol
  ///
  return EFI_UNSUPPORTED;
}

/**
  Provide a function to uninstall the Firmware Management Protocol instance from a
  device handle when the device is managed by a driver that follows the UEFI
  Driver Model.  If the device is not managed by a driver that follows the UEFI
  Driver Model, then EFI_UNSUPPORTED is returned.

  @param[in] FmpUninstaller  Function that installs the Firmware Management
                             Protocol.

  @retval EFI_UNSUPPORTED    The device is not managed by a driver that follows
                             the UEFI Driver Model.

**/
EFI_STATUS
EFIAPI
RegisterFmpUninstaller (
  IN FMP_DEVICE_LIB_REGISTER_FMP_UNINSTALLER  FmpUninstaller
  )
{
  ///
  /// This is a system firmware update that does not use Driver Binding Protocol
  ///
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

  @retval EFI_UNSUPPORTED  The device is not managed by a driver that follows
                           the UEFI Driver Model.

**/
EFI_STATUS
EFIAPI
FmpDeviceSetContext (
  IN EFI_HANDLE  Handle,
  IN OUT VOID    **Context
  )
{
  ///
  /// This is a system firmware update that does not use Driver Binding Protocol
  ///
  return EFI_UNSUPPORTED;
}

/**
  Returns the size, in bytes, of the firmware image currently stored in the
  firmware device. This function is used to by the GetImage() and
  GetImageInfo() services of the Firmware Management Protocol.  If the image
  size can not be determined from the firmware device, then 0 must be returned.

  @param[out] Size         Pointer to the size, in bytes, of the firmware image
                           currently stored in the firmware device.

  @retval EFI_UNSUPPORTED  The firmware device does not support reporting
                           the size of the currently stored firmware image.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetSize (
  OUT UINTN  *Size
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Returns the GUID value used to fill in the ImageTypeId field of the
  EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the GetImageInfo()
  service of the Firmware Management Protocol.  If EFI_UNSUPPORTED is returned,
  then the ImageTypeId field is set to gEfiCallerIdGuid.  If EFI_SUCCESS is
  returned, then ImageTypeId is set to the Guid returned from this function.

  @param[out] Guid  Double pointer to a GUID value that is updated to point to
                    to a GUID value.  The GUID value is not allocated and must
                    not be modified or freed by the caller.

  @retval EFI_UNSUPPORTED        EFI_FIRMWARE_IMAGE_DESCRIPTOR ImageTypeId GUID is set
                                 to gEfiCallerIdGuid.

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

  @retval EFI_SUCCESS            The attributes supported by the firmware device were returned.
  @retval EFI_INVALID_PARAMETER  Supported or Setting is NULL.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetAttributes (
  OUT UINT64  *Supported,
  OUT UINT64  *Setting
  )
{
  if ((Supported == NULL) || (Setting == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *Supported = (IMAGE_ATTRIBUTE_IMAGE_UPDATABLE         |
                IMAGE_ATTRIBUTE_RESET_REQUIRED          |
                IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED |
                IMAGE_ATTRIBUTE_IN_USE
                );
  if (PcdGet8 (PcdUsbCCapsuleDebugLevel) ==  USBC_CAPSULE_DBG_DISABLED) {
    *Setting = (IMAGE_ATTRIBUTE_IMAGE_UPDATABLE         |
                IMAGE_ATTRIBUTE_RESET_REQUIRED          |
                IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED |
                IMAGE_ATTRIBUTE_IN_USE
                );
  } else {
    *Setting = (IMAGE_ATTRIBUTE_IMAGE_UPDATABLE         |
                IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED |
                IMAGE_ATTRIBUTE_IN_USE
                );
  }
  return EFI_SUCCESS;
}

/**
  Returns the value used to fill in the LowestSupportedVersion field of the
  Gets the current Lowest Supported Version.

  This is a protection mechanism so that a previous version with known issue is
  not applied.  ONLY implement this if your running firmware has a method to
  return this at runtime.  If EFI_UNSUPPORTED is returned, then the Lowest
  Supported Version is stored in a UEFI Variable.

  @param[out] LowestSupportedVersion  On return this value represents the current Lowest
                                      Supported Version (in same format as GetVersion).

  @retval EFI_UNSUPPORTED             The firmware device does not support a method to
                                      report the lowest supported version of the currently
                                      stored firmware image.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetLowestSupportedVersion (
  OUT UINT32  *LowestSupportedVersion
  )
{
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

  @retval EFI_UNSUPPORTED    The firmware device does not support a method
                             to report the version string of the currently
                             stored firmware image.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetVersionString (
  OUT CHAR16  **VersionString
  )
{
  if (VersionString == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_UNSUPPORTED;
}

/**
  Gets the current running version.

  ONLY implement this if your running firmware has a method to return this at
  runtime.

  @param[out] Version            On return this value represents the current running
                                 version.

  @retval EFI_SUCCESS            Variable retrieval or initialization successful.
  @retval EFI_INVALID_PARAMETER  Version is NULL.
  @retval Others                 Variable retrieval or initialization failed.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetVersion (
  OUT UINT32  *Version
  )
{
  EFI_STATUS     Status;
  UINT32         PdBridgeVersion;
  UINT32         VarAttributes;
  UINTN          VarSize;

  if (Version == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  VarSize = sizeof (PdBridgeVersion);
  Status = gRT->GetVariable (
                  USBC_PD_BRIDGE_VERSION_NAME,
                  &gUsbCPdBridgeVersionGuid,
                  &VarAttributes,
                  &VarSize,
                  &PdBridgeVersion
                  );
  if (!EFI_ERROR (Status) || Status == EFI_NOT_FOUND ) {
    VarAttributes = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE;
    PdBridgeVersion = USBC_PD_BRIDGE_VIRTUAL_VERSION;
    Status = gRT->SetVariable (
                    USBC_PD_BRIDGE_VERSION_NAME,
                    &gUsbCPdBridgeVersionGuid,
                    VarAttributes,
                    VarSize,
                    &PdBridgeVersion
                    );
    ASSERT_EFI_ERROR (Status);
  }

  ///
  /// Use the version in the gUsbCPdBridgeVersionGuid
  /// Initial the value at the first boot, and update the value once capsule update successfully.
  ///
  *Version = (UINT32) PdBridgeVersion;

  return Status;
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

  @retval EFI_UNSUPPORTED       The firmware device does not support a method to
                                report the hardware instance value.

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

  @param[out] Image        Points to the buffer where the current image is copied to.
  @param[out] ImageSize    On entry, points to the size of the buffer pointed to
                           by Image, in bytes.  On return, points to the length of
                           the image, in bytes.

  @retval EFI_UNSUPPORTED  The operation is not supported.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetImage (
  OUT    VOID   *Image,
  IN OUT UINTN  *ImageSize
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Checks if a new firmware image is valid for the firmware device.  This
  function allows firmware update operation to validate the firmware image
  before FmpDeviceSetImage() is called.

  @param[in]  Image           Points to a new firmware image.
  @param[in]  ImageSize       Size, in bytes, of a new firmware image.
  @param[out] ImageUpdatable  Indicates if a new firmware image is valid for
                              a firmware update to the firmware device.  The
                              following values from the Firmware Management
                              Protocol are supported:
                                IMAGE_UPDATABLE_VALID
                                IMAGE_UPDATABLE_INVALID
                                IMAGE_UPDATABLE_INVALID_TYPE
                                IMAGE_UPDATABLE_INVALID_OLD
                                IMAGE_UPDATABLE_VALID_WITH_VENDOR_CODE

  @retval EFI_SUCCESS         The image was successfully checked.  Additional
                              status information is returned in
                              ImageUpdatable.
  @retval Others              Return value from FmpDeviceCheckImageWithStatus ()

**/
EFI_STATUS
EFIAPI
FmpDeviceCheckImage (
  IN  CONST VOID  *Image,
  IN  UINTN       ImageSize,
  OUT UINT32      *ImageUpdatable
  )
{
  UINT32  LastAttemptStatus;

  return FmpDeviceCheckImageWithStatus (Image, ImageSize, ImageUpdatable, &LastAttemptStatus);
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

  @retval EFI_SUCCESS            The image was successfully checked. Additional status information
                                 is returned in ImageUpdatable.
  @retval EFI_INVALID_PARAMETER  The parameter is NULL or invalid.

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
  PAYLOAD_HEADER          *PdBridgePayloadHeader;
  PD_BRIDGE_PAYLOAD_ITEM  *PdBridgePayloadItem;
  UINTN                   Index;

  PdBridgePayloadHeader = NULL;
  PdBridgePayloadItem   = NULL;

  if (ImageUpdatable == NULL) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_CHECKIMAGE, 0, 0);
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_IMAGE_TABLE_NOT_PROVIDED;
    return EFI_INVALID_PARAMETER;
  }

  ///
  /// Set to valid and then if any tests fail it will update this flag.
  ///
  *ImageUpdatable = IMAGE_UPDATABLE_VALID;

  if (Image == NULL) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_CHECKIMAGE_POINTER_NULL, 0, 0);
    *ImageUpdatable    = IMAGE_UPDATABLE_INVALID;
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_IMAGE_PARAMETER_IS_NULL;
    return EFI_INVALID_PARAMETER;
  }

  if ((ImageSize < sizeof (PAYLOAD_HEADER)) || \
      (*(UINT32 *) Image != PD_BRIDGE_PAYLOAD_HEADER_SIGNATURE))
  {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_SIGNATURE_NOT_DETECTED, 0, 0);
    *ImageUpdatable    = IMAGE_UPDATABLE_INVALID;
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_SIGNATURE_IS_NOT_DETECTED;
    return EFI_INVALID_PARAMETER;
  }

  PdBridgePayloadHeader = (PAYLOAD_HEADER *) Image;

  if (PdBridgePayloadHeader->PayloadCount == 0) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_ZERO, 0, 0);
    *ImageUpdatable    = IMAGE_UPDATABLE_INVALID;
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_PAYLOAD_COUNT_IS_ZERO;
    return EFI_INVALID_PARAMETER;
  }

  if (ImageSize < (sizeof (PAYLOAD_HEADER) + \
                   (PdBridgePayloadHeader->PayloadCount * sizeof (PD_BRIDGE_PAYLOAD_ITEM))))
  {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_SIZE_SMALL, 0, 0);
    *ImageUpdatable    = IMAGE_UPDATABLE_INVALID;
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_PAYLOAD_SIZE_TOO_SMALL;
    return EFI_INVALID_PARAMETER;
  }

  PdBridgePayloadItem = (PD_BRIDGE_PAYLOAD_ITEM *) (PdBridgePayloadHeader + 1);

  for (Index = 0; Index < PdBridgePayloadHeader->PayloadCount; Index++, PdBridgePayloadItem++) {
    if ((PdBridgePayloadItem->ImageOffset + PdBridgePayloadItem->ImageSize) > ImageSize) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_OUT_BOUNDS1, 0, 0);
      *ImageUpdatable    = IMAGE_UPDATABLE_INVALID;
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_PAYLOAD_IS_OUT_OF_BOUNDS;
      return EFI_INVALID_PARAMETER;
    }
  }

  return EFI_SUCCESS;
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
  force a firmware image update when the abort reason zxc due to the new firmware
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

  @retval EFI_SUCCESS            The firmware device was successfully updated
                                 with the new firmware image.
  @retval Others                 Return value from FmpDeviceSetImageWithStatus ()

**/
EFI_STATUS
EFIAPI
FmpDeviceSetImage (
  IN  CONST VOID                                     *Image,
  IN  UINTN                                          ImageSize,
  IN  CONST VOID                                     *VendorCode, OPTIONAL
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress, OPTIONAL
  IN  UINT32                                         CapsuleFwVersion,
  OUT CHAR16                                         **AbortReason
  )
{
  UINT32  LastAttemptStatus;

  return FmpDeviceSetImageWithStatus (
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
  Update FW version to gUsbCPdBridgeVersionGuid to support FmpDeviceGetVersion () when the capsule update
  is successful.

  @param[in]  FwVersion    PD Bridge version

  @retval EFI_SUCCESS      Variable is successfully updated.
  @retval EFI_UNSUPPORTED  Failed to get variable
  @retval Others           Variable updated failed.

**/
EFI_STATUS
EFIAPI
UpdatePdBridgeVersion (
   IN  UINT32  FwVersion
  )
{
  EFI_STATUS     Status;
  UINT32         PdBridgeVersion;
  UINT32         VarAttributes;
  UINTN          VarSize;

  VarSize = sizeof (PdBridgeVersion);
  Status = gRT->GetVariable (
                  USBC_PD_BRIDGE_VERSION_NAME,
                  &gUsbCPdBridgeVersionGuid,
                  &VarAttributes,
                  &VarSize,
                  &PdBridgeVersion
                  );
  if (!EFI_ERROR (Status) || Status == EFI_NOT_FOUND ) {
    VarAttributes   = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE;
    PdBridgeVersion = FwVersion;
    Status = gRT->SetVariable (
                    USBC_PD_BRIDGE_VERSION_NAME,
                    &gUsbCPdBridgeVersionGuid,
                    VarAttributes,
                    VarSize,
                    &PdBridgeVersion
                    );
    ASSERT_EFI_ERROR (Status);
  } else {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_GET_VERSION_VARIABLE_FAILED, (UINT32) Status, 0);
    return EFI_UNSUPPORTED;
  }

  return Status;
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
  @retval EFI_INVALID_PARAMETER  The Image or Progress was NULL.
  @retval Others                 The firmware device updated failed

**/
EFI_STATUS
EFIAPI
FmpDeviceSetImageWithStatus (
  IN  CONST VOID                                     *Image,
  IN  UINTN                                          ImageSize,
  IN  CONST VOID                                     *VendorCode, OPTIONAL
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress, OPTIONAL
  IN  UINT32                                         CapsuleFwVersion,
  OUT CHAR16                                         **AbortReason,
  OUT UINT32                                         *LastAttemptStatus
  )
{
  EFI_STATUS               Status;
  PAYLOAD_HEADER           *PdBridgePayloadHeader;
  PD_BRIDGE_PAYLOAD_ITEM   *PdBridgePayloadItem;
  UINTN                    Index;
  USBC_PD_BRIDGE_PROTOCOL  *PdBridgeProtocol;
  UINT64                   PdBridgeVersion;
  UINT32                   FwVersion;
  UINT32                   SubFwVersion;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_FMP_DEV_PD_BRIDGE_SET_IMAGE_START, 0, 0);

  if (Image == NULL) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_IMAGE_NULL, 0, 0);
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_IMAGE_NOT_PROVIDED;
    return EFI_INVALID_PARAMETER;
  }

  if (Progress == NULL) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_PROGRESS_IS_NULL, 0, 0);
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_PROGRESS_CALLBACK_ERROR;
    return EFI_INVALID_PARAMETER;
  }

  ///
  /// Test Progress Callback function
  ///
  Status = Progress (5);
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_PROGRESS_CALLBACK_FAILED, (UINT32) Status, 0);
    goto Exit;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_FMP_DEV_PD_BRIDGE_UPDATE_START, 0, 0);

  Status = gBS->LocateProtocol (&gUsbCPdBridgeProtocolGuid, NULL, (VOID **) &PdBridgeProtocol);
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_LOCATE_PROTOCOL_FAILED, (UINT32) Status, 0);
    goto Exit;
  }

  ///
  /// Boundary check is supposed to be taken care by FmpDeviceCheckImage above already.
  ///
  PdBridgePayloadHeader = (PAYLOAD_HEADER *) Image;
  PdBridgePayloadItem   = (PD_BRIDGE_PAYLOAD_ITEM *) (PdBridgePayloadHeader + 1);

  ///
  /// Send Lock command to EC to stop EC-PD regular communication.
  ///
  Status = PdBridgeProtocol->Lock (PdBridgeProtocol, LOCK_I2C);
  if (EFI_ERROR (Status)) {
    goto UnLockEcPdCommunication;
  }

  ///
  /// Start to update PD Bridge firmware
  ///
  for (Index = 0; Index < PdBridgePayloadHeader->PayloadCount; Index++, PdBridgePayloadItem++) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_COUNT, (UINT32) (Index + 1), PdBridgePayloadHeader->PayloadCount);
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_IMAGE_OFFSET_SIZE, PdBridgePayloadItem->ImageOffset, PdBridgePayloadItem->ImageSize);
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_CNTRL_INDEX_SHARE_FLASH_MODE, (UINT32) PdBridgePayloadItem->PrivateData.PdBridge.PdCntrlIndex, (UINT32) PdBridgePayloadItem->PrivateData.PdBridge.ShareFlashMode);
    if ((PdBridgePayloadItem->ImageOffset + PdBridgePayloadItem->ImageSize) > ImageSize) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_PAYLOAD_OUT_BOUNDS2, 0, 0);
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_PAYLOAD_IS_OUT_OF_BOUNDS_2;
      Status = EFI_INVALID_PARAMETER;
      goto UnLockEcPdCommunication;
    }
    if (PdBridgePayloadItem->FirmwareType != PD_BRIDGE) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_UNSUPPORT_FW_TYPE, (UINT32) PdBridgePayloadItem->FirmwareType, 0);
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_UNSUPPORT_FIRMWARE_TYPE;
      continue;
    }

    if (PdBridgePayloadItem->PrivateData.PdBridge.ShareFlashMode > SHARE_FLASH_MODE_ENABLE) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_INVALID_SHARE_FLASH_MODE_VALUE, (UINT32) PdBridgePayloadItem->PrivateData.PdBridge.ShareFlashMode, 0);
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_INVALID_SHARE_FLASH_MODE_VALUE;
      continue;
    }

    Status = UpdatePdBridgeNvmFirmware (
               PdBridgeProtocol,
               (UINT8 *) Image + PdBridgePayloadItem->ImageOffset,
               PdBridgePayloadItem->ImageSize,
               (PdBridgePayloadItem->PrivateData.PdBridge.PdCntrlIndex),
               PdBridgePayloadItem->PrivateData.PdBridge.ShareFlashMode,
               Progress,
               START_UPDATE_PROCESS + (END_UPDATE_PROCESS / PdBridgePayloadHeader->PayloadCount) * Index,
               START_UPDATE_PROCESS + (END_UPDATE_PROCESS / PdBridgePayloadHeader->PayloadCount) * (Index + 1)
               );
    if (!EFI_ERROR (Status)) {
      PdBridgeVersion = 0;
      ///
      /// The PdCntrlIndex for GetPDFwVersion is 1-based.
      ///
      Status = PdBridgeProtocol->GetVersion (PdBridgeProtocol, PdBridgePayloadItem->PrivateData.PdBridge.PdCntrlIndex + 1, &PdBridgeVersion);
      if (!EFI_ERROR (Status)) {
        FwVersion    = (UINT32) (PdBridgeVersion & 0xFFFFFFFF);
        SubFwVersion = (UINT32) ((PdBridgeVersion >> 32) & 0xFFFFFFFF);
        CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_FMP_DEV_PD_BRIDGE_FW_VERSION_INDEX, (UINT32) Index, 0);
        CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_FMP_DEV_PD_BRIDGE_FW_VERSION, (UINT32) FwVersion, SubFwVersion);
        ///
        /// Update the version to gUsbCPdBridgeVersionGuid
        ///
        UpdatePdBridgeVersion (FwVersion);
      }
    } else {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_FMP_DEV_PD_BRIDGE_UPDATE_FAILED_AT_INDEX, (UINT32) Status, (UINT32) Index);
    }
  }

UnLockEcPdCommunication:
  ///
  /// Send UnLock command to EC to restore EC-PD regular communication.
  ///
  Status = PdBridgeProtocol->Lock (PdBridgeProtocol, UNLOCK_I2C);

Exit:
  Progress (100);

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_FMP_DEV_PD_BRIDGE_SET_IMAGE_END, 0, 0);
  if (EFI_ERROR (Status)) {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_UPDATE_FAILED;
  }

  return Status;
}

/**
  Lock the firmware device that contains a firmware image.  Once a firmware
  device is locked, any attempts to modify the firmware image contents in the
  firmware device must fail.

  @note It is recommended that all firmware devices support a lock method to
        prevent modifications to a stored firmware image.

  @note A firmware device lock mechanism is typically only cleared by a full
        system reset (not just sleep state/low power mode).

  @retval  EFI_UNSUPPORTED  The firmware device does not support locking

**/
EFI_STATUS
EFIAPI
FmpDeviceLock (
  VOID
  )
{
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_FMP_DEV_PD_BRIDGE_LOCK, 0, 0);
  return EFI_UNSUPPORTED;
}
