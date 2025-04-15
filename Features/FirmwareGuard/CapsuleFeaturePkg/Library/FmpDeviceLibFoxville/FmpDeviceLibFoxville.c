/** @file
  Library instance for FmpDeviceLib to support Foxville update in DXE.

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

  @par Specification Reference:
  Foxville Ethernet Controller External Architecture Specification, version 0.99

**/

//
// EDK-II Foundation.
//
#include <PiDxe.h>
#include <LastAttemptStatus.h>
#include <IndustryStandard/Pci.h>
#include <Guid/SystemResourceTable.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Protocol/PciIo.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/FmpDeviceLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleCommon.h"
#include "Defines/FoxvilleNvmMap.h"
#include "FoxvilleApi.h"
#include "FoxvilleDevice.h"
#include "FoxvilleUpdate.h"

#define FOXVILLE_DEVICE_VERSION_STRING_FORMAT  L"#.##-#"
#define FOXVILLE_DEVICE_VERSION_STRING_SIZE    (sizeof (FOXVILLE_DEVICE_VERSION_STRING_FORMAT))

//
// Module Variables.
//
GLOBAL_REMOVE_IF_UNREFERENCED STATIC EFI_EVENT             mFoxvilleOnPciEnumCompleteEvent = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED STATIC FOXVILLE_HW_INSTANCE  *mFoxvilleHwInstPtr             = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED STATIC EFI_PCI_IO_PROTOCOL   *mPciIoProtocolPtr              = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED STATIC FOXVILLE_DEVICE_INFO  *mFoxvilleDeviceInfoPtr         = NULL;

/**
  Initialize the Foxville device information.

  @param[out]  DeviceHandlePtr  Pointer to the device handle.

  @retval  EFI_SUCCESS            Succeed to initialize the Foxville device information.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_OUT_OF_RESOURCES   Failed to allocate needed memory buffer.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to initialize the Foxville device information.

**/
EFI_STATUS
EFIAPI
FoxvilleDeviceInit (
  OUT EFI_HANDLE  *DeviceHandlePtr
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;

  Status = EFI_ABORTED;
  Handle = NULL;

  if (DeviceHandlePtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  mFoxvilleHwInstPtr = AllocateZeroPool (sizeof (FOXVILLE_HW_INSTANCE));
  if (mFoxvilleHwInstPtr == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "AllocateZeroPool failed, Status = %r\n", Status));
    return Status;
  }

  Status = GetFoxvilleDeviceHandle (&Handle);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **)&mPciIoProtocolPtr
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate the PCI IO protocol, Status = %r\n", Status));
    return Status;
  }

  mFoxvilleHwInstPtr->PciIo = mPciIoProtocolPtr;

  //
  // Set the Foxville device PCI attributes.
  //
  Status = SetFoxvillePciAttributes (mPciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Initial the Foxville device information.
  //
  mFoxvilleDeviceInfoPtr = AllocateZeroPool (sizeof (FOXVILLE_DEVICE_INFO));
  if (mFoxvilleDeviceInfoPtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = InitFoxvilleDeviceInfo (mPciIoProtocolPtr, mFoxvilleDeviceInfoPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to initialize the Foxville device information.\n"));
    return EFI_DEVICE_ERROR;
  }

  mFoxvilleHwInstPtr->FlashSize = mFoxvilleDeviceInfoPtr->FlashSize;

  //
  // Config the Foxville register for device initialization.
  //
  Status = ConfigRegisterForDeviceInit (mPciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "%a: Flash Mode = %d\n", __func__, mFoxvilleDeviceInfoPtr->FlashMode));

  *DeviceHandlePtr = Handle;

  return Status;
}

/**
  Callback function for the gEfiPciEnumerationCompleteProtocolGuid protocol installed.

  @param[in]  Event    Event whose notification function is being invoked.
  @param[in]  Context  The pointer to the notification function's context,
                       which is implementation-dependent.

**/
VOID
EFIAPI
FoxvillePciEnumCompleteCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                             Status;
  FMP_DEVICE_LIB_REGISTER_FMP_INSTALLER  FmpInstaller;
  VOID                                   *InstPtr;
  EFI_HANDLE                             DeviceHandle;

  Status       = EFI_SUCCESS;
  FmpInstaller = (FMP_DEVICE_LIB_REGISTER_FMP_INSTALLER)Context;
  InstPtr      = NULL;
  DeviceHandle = NULL;

  if (FmpInstaller == NULL) {
    DEBUG ((DEBUG_ERROR, "Foxville FmpInstaller is invalid.\n"));
    return;
  }

  //
  // Ensure the PCI enumeration complete protocol is installed.
  //
  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  (VOID **)&InstPtr
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Retrieve the device handle for Foxville.
  //
  Status = FoxvilleDeviceInit (&DeviceHandle);
  if (Status == EFI_NOT_FOUND) {
    DEBUG ((DEBUG_INFO, "Not found supported Foxville Device for FMP capsule update.\n"));
    return;
  } else if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to detect Foxville device, Status = %r\n", Status));
    return;
  }

  //
  // Install the FMP protocol for correspond device handle.
  //
  Status = FmpInstaller (DeviceHandle);
  DEBUG ((DEBUG_INFO, "Create Foxville FMP device: %r\n", Status));

  gBS->CloseEvent (Event);

  return;
}

/**
  Provide a function to install the Firmware Management Protocol instance onto a
  device handle when the device is managed by a driver that follows the UEFI
  Driver Model.  If the device is not managed by a driver that follows the UEFI
  Driver Model, then EFI_UNSUPPORTED is returned.

  @param[in] FmpInstaller  Function that installs the Firmware Management
                           Protocol.

  @retval EFI_SUCCESS      The device is managed by a driver that follows the
                           UEFI Driver Model.  FmpInstaller must be called on
                           each Driver Binding Start().
  @retval EFI_UNSUPPORTED  The device is not managed by a driver that follows
                           the UEFI Driver Model.
  @retval other            The Firmware Management Protocol for this firmware
                           device is not installed.  The firmware device is
                           still locked using FmpDeviceLock().

**/
EFI_STATUS
EFIAPI
RegisterFmpInstaller (
  IN FMP_DEVICE_LIB_REGISTER_FMP_INSTALLER  Function
  )
{
  VOID  *Registration;

  mFoxvilleOnPciEnumCompleteEvent = EfiCreateProtocolNotifyEvent (
                                      &gEfiPciEnumerationCompleteProtocolGuid,
                                      TPL_CALLBACK,
                                      FoxvillePciEnumCompleteCallback,
                                      (VOID *)Function,
                                      &Registration
                                      );
  if (mFoxvilleOnPciEnumCompleteEvent == NULL) {
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}

/**
  Provide a function to uninstall the Firmware Management Protocol instance from a
  device handle when the device is managed by a driver that follows the UEFI
  Driver Model.  If the device is not managed by a driver that follows the UEFI
  Driver Model, then EFI_UNSUPPORTED is returned.

  @param[in] FmpUninstaller  Function that installs the Firmware Management
                             Protocol.

  @retval EFI_SUCCESS      The device is managed by a driver that follows the
                           UEFI Driver Model.  FmpUninstaller must be called on
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
  IN FMP_DEVICE_LIB_REGISTER_FMP_UNINSTALLER  Function
  )
{
  return EFI_SUCCESS;
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
  @retval EFI_DEVICE_ERROR       An error occurred attempting to determine the
                                 size of the firmware image currently stored in
                                 in the firmware device.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetSize (
  OUT UINTN  *Size
  )
{
  if (Size == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Size = 0;
  return EFI_SUCCESS;
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

  @retval EFI_SUCCESS      EFI_FIRMWARE_IMAGE_DESCRIPTOR ImageTypeId GUID is set
                           to the returned Guid value.
  @retval EFI_UNSUPPORTED  EFI_FIRMWARE_IMAGE_DESCRIPTOR ImageTypeId GUID is set
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

  @retval EFI_SUCCESS            The attributes supported by the firmware
                                 device were returned.
  @retval EFI_INVALID_PARAMETER  Supported is NULL.
  @retval EFI_INVALID_PARAMETER  Setting is NULL.

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

  *Setting   = (IMAGE_ATTRIBUTE_IMAGE_UPDATABLE         |
                IMAGE_ATTRIBUTE_RESET_REQUIRED          |
                IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED |
                IMAGE_ATTRIBUTE_IN_USE
                );

  return EFI_SUCCESS;
}

/**
  Returns the value used to fill in the LowestSupportedVersion field of the
  EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the GetImageInfo()
  service of the Firmware Management Protocol.  If EFI_SUCCESS is returned, then
  the firmware device supports a method to report the LowestSupportedVersion
  value from the currently stored firmware image.  If the value can not be
  reported for the firmware image currently stored in the firmware device, then
  EFI_UNSUPPORTED must be returned.  EFI_DEVICE_ERROR is returned if an error
  occurs attempting to retrieve the LowestSupportedVersion value for the
  currently stored firmware image.

  @note It is recommended that all firmware devices support a method to report
        the LowestSupportedVersion value from the currently stored firmware
        image.

  @param[out] LowestSupportedVersion  LowestSupportedVersion value retrieved
                                      from the currently stored firmware image.

  @retval EFI_SUCCESS       The lowest supported version of currently stored
                            firmware image was returned in LowestSupportedVersion.
  @retval EFI_UNSUPPORTED   The firmware device does not support a method to
                            report the lowest supported version of the currently
                            stored firmware image.
  @retval EFI_DEVICE_ERROR  An error occurred attempting to retrieve the lowest
                            supported version of the currently stored firmware
                            image.

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
  EFI_STATUS                    Status;
  FOXVILLE_DEV_STARTER_VERSION  NvmVersion;

  if (VersionString == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((mFoxvilleDeviceInfoPtr == NULL) || (!(mFoxvilleDeviceInfoPtr->IsValid))) {
    return EFI_DEVICE_ERROR;
  }

  *VersionString = (CHAR16 *)AllocateZeroPool (FOXVILLE_DEVICE_VERSION_STRING_SIZE);
  if (*VersionString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  NvmVersion.Uint16 = mFoxvilleDeviceInfoPtr->NvmVersion;
  Status            = UnicodeSPrint (
                        *VersionString,
                        FOXVILLE_DEVICE_VERSION_STRING_SIZE,
                        L"%X.%X%X-%X",
                        (UINT8)((NvmVersion.Bits.UpperByte & 0xF0) >> 4),
                        (UINT8)(NvmVersion.Bits.UpperByte & 0x0F),
                        (UINT8)((NvmVersion.Bits.LowerByte & 0xF0) >> 4),
                        (UINT8)(NvmVersion.Bits.LowerByte & 0x0F)
                        );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
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
FmpDeviceGetVersion (
  OUT UINT32  *Version
  )
{
  if (Version == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (mFoxvilleDeviceInfoPtr == NULL) {
    return EFI_DEVICE_ERROR;
  }

  if (!(mFoxvilleDeviceInfoPtr->IsValid)) {
    return EFI_DEVICE_ERROR;
  }

  *Version = (UINT32)(mFoxvilleDeviceInfoPtr->NvmVersion);

  return EFI_SUCCESS;
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
                            device is returned in HardwareInstance.
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
  Returns a copy of the firmware image currently stored in the firmware device.

  @note It is recommended that all firmware devices support a method to retrieve
        a copy currently stored firmware image.  This can be used to support
        features such as recovery and rollback.

  @param[out]     Image     Pointer to a caller allocated buffer where the
                            currently stored firmware image is copied to.
  @param[in, out] ImageSize Pointer the size, in bytes, of the Image buffer.
                            On return, points to the size, in bytes, of firmware
                            image currently stored in the firmware device.

  @retval EFI_SUCCESS            Image contains a copy of the firmware image
                                 currently stored in the firmware device, and
                                 ImageSize contains the size, in bytes, of the
                                 firmware image currently stored in the
                                 firmware device.
  @retval EFI_BUFFER_TOO_SMALL   The buffer specified by ImageSize is too small
                                 to hold the firmware image currently stored in
                                 the firmware device. The buffer size required
                                 is returned in ImageSize.
  @retval EFI_INVALID_PARAMETER  The Image is NULL.
  @retval EFI_INVALID_PARAMETER  The ImageSize is NULL.
  @retval EFI_UNSUPPORTED        The operation is not supported.
  @retval EFI_DEVICE_ERROR       An error occurred attempting to retrieve the
                                 firmware image currently stored in the firmware
                                 device.

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

  @retval EFI_SUCCESS            The image was successfully checked.  Additional
                                 status information is returned in
                                 ImageUpdatable.
  @retval EFI_INVALID_PARAMETER  Image is NULL.
  @retval EFI_INVALID_PARAMETER  ImageUpdatable is NULL.

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

  return FmpDeviceCheckImageWithStatus (
           Image,
           ImageSize,
           ImageUpdatable,
           &LastAttemptStatus
           );
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
                                  of error.

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
  IN     CONST VOID    *Image,
  IN           UINTN   ImageSize,
     OUT       UINT32  *ImageUpdatable,
     OUT       UINT32  *LastAttemptStatus
  )
{
  FOXVILLE_NVM_MAP  *FoxvilleNvmMapPtr;

  FoxvilleNvmMapPtr = NULL;

  //
  // Check the input parameters.
  //
  if ((Image == NULL) || (ImageSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((ImageUpdatable == NULL) || (LastAttemptStatus == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((ImageSize < SIZE_1MB) || (ImageSize > SIZE_2MB)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check the device information.
  //
  if ((mFoxvilleDeviceInfoPtr == NULL) || (!(mFoxvilleDeviceInfoPtr->IsValid))) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Check the image validity.
  //
  FoxvilleNvmMapPtr = (FOXVILLE_NVM_MAP *)Image;
  *ImageUpdatable   = IMAGE_UPDATABLE_VALID;

  if (IsBlankFlashDeviceId (mFoxvilleDeviceInfoPtr->DeviceId)) {
    return EFI_SUCCESS;
  }

  if (ImageSize > mFoxvilleDeviceInfoPtr->FlashSize) {
    DEBUG ((DEBUG_ERROR, "Image size is larger than flash size.\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
    return EFI_ABORTED;
  }

  if ((FoxvilleNvmMapPtr->DeviceId) != (mFoxvilleDeviceInfoPtr->DeviceId)) {
    DEBUG ((DEBUG_ERROR, "Update image is not for this device.\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID_TYPE;
    return EFI_ABORTED;
  }

  if ((FoxvilleNvmMapPtr->DevStarterVersion.Uint16) < (mFoxvilleDeviceInfoPtr->NvmVersion)) {
    DEBUG ((DEBUG_ERROR, "Update image version is lower than current, rollback is unsupported.\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID_OLD;
    return EFI_ABORTED;
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

  @retval EFI_SUCCESS            The firmware device was successfully updated
                                 with the new firmware image.
  @retval EFI_ABORTED            The operation is aborted.  Additional details
                                 are provided in AbortReason.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.
  @retval EFI_UNSUPPORTED        The operation is not supported.

**/
EFI_STATUS
EFIAPI
FmpDeviceSetImage (
  IN  CONST VOID                                     *Image,
  IN  UINTN                                          ImageSize,
  IN  CONST VOID                                     *VendorCode        OPTIONAL,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress           OPTIONAL,
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
  IN  CONST VOID                                     *VendorCode        OPTIONAL,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress           OPTIONAL,
  IN  UINT32                                         CapsuleFwVersion,
  OUT CHAR16                                         **AbortReason,
  OUT UINT32                                         *LastAttemptStatus
  )
{
  EFI_STATUS  Status;
  UINT8       *ImageCopy;

  ImageCopy = NULL;

  if (Progress == NULL) {
    DEBUG ((DEBUG_ERROR, "FmpDeviceSetImageWithStatus - Invalid progress callback\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_MIN_ERROR_CODE_VALUE;
    return EFI_INVALID_PARAMETER;
  }

  //
  // Test Progress Callback function.
  //
  Status = Progress (5);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FmpDeviceSetImageWithStatus - Progress Callback failed with Status %r.\n", Status));
  }

  DEBUG ((DEBUG_INFO, "Flashing new NVM image\n"));

  ImageCopy = (UINT8 *)AllocateZeroPool (ImageSize);
  if (ImageCopy == NULL) {
    return EFI_ABORTED;
  }

  CopyMem (ImageCopy, Image, ImageSize);

  if (mFoxvilleDeviceInfoPtr->FlashMode == FLASH_MODE_BLANK) {
    Status = FoxvilleUpdateInBlankMode (mPciIoProtocolPtr, ImageCopy, ImageSize);
    if (EFI_ERROR (Status)) {
      Status = EFI_ABORTED;
      goto Exit;
    }
  } else if (mFoxvilleDeviceInfoPtr->FlashMode == FLASH_MODE_PROTECTED) {
    //
    // It could implement the flow to write the non-protected region in the future.
    //
    DEBUG ((DEBUG_ERROR, "Device is in protected mode, abort to perform FW update.\n"));
    return EFI_ABORTED;
  } else {
    Status = FoxvilleUpdateInUnprotectedMode (mFoxvilleHwInstPtr, ImageCopy, ImageSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Flashing NVM FAILED\n"));
      Status = EFI_ABORTED;
      goto Exit;
    }

    //
    // Reload the GPHY FW to complete the update.
    //
    Status = ReloadGphyFw (mPciIoProtocolPtr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to reload the GPHY FW.\n"));
      Status = EFI_DEVICE_ERROR;
      goto Exit;
    }
  }

  Progress (90);

  DEBUG ((DEBUG_INFO, "NVM image was updated successfully\n"));

Exit:
  FreePool (ImageCopy);

  return EFI_SUCCESS;
}

/**
  Lock the firmware device that contains a firmware image.  Once a firmware
  device is locked, any attempts to modify the firmware image contents in the
  firmware device must fail.

  @note It is recommended that all firmware devices support a lock method to
        prevent modifications to a stored firmware image.

  @note A firmware device lock mechanism is typically only cleared by a full
        system reset (not just sleep state/low power mode).

  @retval  EFI_SUCCESS      The firmware device was locked.
  @retval  EFI_UNSUPPORTED  The firmware device does not support locking

**/
EFI_STATUS
EFIAPI
FmpDeviceLock (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}
