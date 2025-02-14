/** @file
  FmpDeviceLib instance to support Thunderbolt Retimer Firmware update

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

**/

#include <PiDxe.h>
#include <LastAttemptStatus.h>
#include <Guid/SystemResourceTable.h>
#include <Library/FmpDeviceLib.h>

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/TbtNvmRetimerUpdateLib.h>
#include <Library/TimerLib.h>
#include <Protocol/UsbCRetimerProtocol.h>
#include <Protocol/UsbCProgressCodeProtocol.h>
#include <UsbCRetimerSetup.h>
#include <FmpLastAttemptStatus.h>
#include <TbtNvmRetimer.h>
#include <Library/UsbcCapsuleDebugLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>

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
  OUT UINT64    *Supported,
  OUT UINT64    *Setting
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
  if (PcdGet8 (PcdUsbCCapsuleDebugLevel) ==  USBC_CAPSULE_DBG_DISABLED) {
    *Setting   = (IMAGE_ATTRIBUTE_IMAGE_UPDATABLE         |
                  IMAGE_ATTRIBUTE_RESET_REQUIRED          |
                  IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED |
                  IMAGE_ATTRIBUTE_IN_USE
                  );
  } else {
    *Setting   = (IMAGE_ATTRIBUTE_IMAGE_UPDATABLE         |
                  IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED |
                  IMAGE_ATTRIBUTE_IN_USE
                  );
  }

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
  if (VersionString == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_UNSUPPORTED;
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
  EFI_STATUS                            Status;
  USBC_RETIMER_SETUP                    UsbCRetimerSetup;
  UINTN                                 VarSize;

  if (Version == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // @ToDo: Enable GetVersion after ensuring manipulation of multiple Retimer firmwares is working
  //        and there is no significant POST time impact.
  //
  ZeroMem (&UsbCRetimerSetup, sizeof (USBC_RETIMER_SETUP));
  VarSize = sizeof (USBC_RETIMER_SETUP);
  Status = gRT->GetVariable (
                  USBC_RETIMER_NAME,
                  &gUsbCRetimerSetupGuid,
                  NULL,
                  &VarSize,
                  &UsbCRetimerSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get UsbCRetimer variable with return Status = (%r).\n", Status));
    return EFI_UNSUPPORTED;
  }
  *Version = UsbCRetimerSetup.UsbCRetimer0Version;
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
  PAYLOAD_HEADER            *RetimerPayloadHeader;
  RETIMER_ITEM              *RetimerPayloadItem;
  UINTN                     Index;

  RetimerPayloadHeader = NULL;
  RetimerPayloadItem   = NULL;

  if (ImageUpdatable == NULL) {
    DEBUG ((DEBUG_ERROR, "CheckImage - ImageUpdatable Pointer Parameter is NULL.\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_IMAGE_TABLE_NOT_PROVIDED;
    return EFI_INVALID_PARAMETER;
  }

  //
  //Set to valid and then if any tests fail it will update this flag.
  //
  *ImageUpdatable = IMAGE_UPDATABLE_VALID;

  if (Image == NULL) {
    DEBUG ((DEBUG_ERROR, "CheckImage - Image Pointer Parameter is NULL.\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_IMAGE_PARAMETER_IS_NULL;
    return EFI_INVALID_PARAMETER;
  }

  if ((ImageSize < sizeof (PAYLOAD_HEADER)) || \
      (*(UINT32 *)Image != RETIMER_PAYLOAD_HEADER_SIGNATURE)) {
    DEBUG ((DEBUG_ERROR, "CheckImage - Retimer payload signature is not detected.\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_SIGNATURE_IS_NOT_DETECTED;
    return EFI_INVALID_PARAMETER;
  }

  RetimerPayloadHeader = (PAYLOAD_HEADER *) Image;

  if (RetimerPayloadHeader->PayloadCount == 0) {
    DEBUG ((DEBUG_ERROR, "CheckImage - RetimerCount in header is 0, nothing to update.\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_RETIMER_COUNT_HEADER_IS_ZERO;
    return EFI_INVALID_PARAMETER;
  }

  if (ImageSize < (sizeof (PAYLOAD_HEADER) + \
      (RetimerPayloadHeader->PayloadCount * sizeof (RETIMER_ITEM)))) {
    DEBUG ((DEBUG_ERROR, "CheckImage - Retimer payload size too small\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_PAYLOAD_SIZE_TOO_SMALL;
    return EFI_INVALID_PARAMETER;
  }

  RetimerPayloadItem = (RETIMER_ITEM *)(RetimerPayloadHeader + 1);

  for (Index = 0; Index < RetimerPayloadHeader->PayloadCount; Index++, RetimerPayloadItem++) {
    if ((RetimerPayloadItem->ImageOffset + RetimerPayloadItem->ImageSize) > ImageSize) {
      DEBUG ((DEBUG_ERROR, "CheckImage - Retimer payload is out of bounds\n"));
      *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_PAYLOAD_IS_OUT_OF_BOUNDS;
      return EFI_INVALID_PARAMETER;
    }
  }

  return EFI_SUCCESS;
}

/**
  Construct the Retimer instances through the Retimer Payload header and item.
  Also construct the Host Router instances for sending LSEN.

  @param[in]  Image                         Points to the new firmware image.
  @param[in]  ImageSize                     Size, in bytes, of the new firmware image.
  @param[out] LastAttemptStatus             A pointer to a UINT32 that holds the last attempt
                                            status to report back to the ESRT table in case
                                            of error. This value will only be checked when this
                                            function returns an error.
                                            The return status code must fall in the range of
                                            LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_MIN_ERROR_CODE_VALUE to
                                            LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_MAX_ERROR_CODE_VALUE.
                                            If the value falls outside this range, it will be converted
                                            to LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL.
  @param[out] RetimerDeviceInstances        A pointer to the Retimer device instances base on RETIMER_DEV_INSTANCE.
  @param[out] RetimerDeviceInstancesCount   A pointer to count the number of Retimer device instances.
  @param[out] HrDeviceInstances             A pointer to the Host Router device instances base on HR_DEV_INSTANCE.
  @param[out] HrDeviceInstancesCount        A pointer to count the number of Host Router device instances.

  @retval EFI_SUCCESS             The firmware device was successfully updated
                                  with the new firmware image.
  @retval EFI_ABORTED             The operation is aborted.  Additional details
                                  are provided in AbortReason.
  @retval EFI_INVALID_PARAMETER   The Image was NULL.
  @retval EFI_UNSUPPORTED         The operation is not supported.
**/
EFI_STATUS
EFIAPI
ConstructRetimerInstances (
  IN  CONST VOID                      *Image,
  IN  UINTN                           ImageSize,
  OUT UINT32                          *LastAttemptStatus,
  OUT RETIMER_DEV_INSTANCE            *RetimerDeviceInstances,
  OUT UINT32                          *RetimerDeviceInstancesCount,
  OUT HR_DEV_INSTANCE                 *HrDeviceInstances,
  OUT UINT32                          *HrDeviceInstancesCount
  )
{
  EFI_STATUS                          Status;
  PAYLOAD_HEADER                      *RetimerPayloadHeader;
  RETIMER_ITEM                        *RetimerPayloadItem;
  UINTN                               Index;
  RETIMER_DEV_INSTANCE                RetimerDevice;

  //
  // Boundary check is supposed to be taken case by FmpDeviceCheckImage above already.
  //
  RetimerPayloadHeader = (PAYLOAD_HEADER *) Image;
  RetimerPayloadItem = (RETIMER_ITEM *) (RetimerPayloadHeader + 1);

  DEBUG ((DEBUG_INFO, "Total of Retimer Payload = %d\n", RetimerPayloadHeader->PayloadCount));

  //
  // Start to Construct Retimer Instances
  //
  for (Index = 0; Index < RetimerPayloadHeader->PayloadCount; Index++, RetimerPayloadItem++) {
    if (*RetimerDeviceInstancesCount >= MAX_RETIMER_INSTANCES) {
      DEBUG ((DEBUG_ERROR, "MAX_RETIMER_INSTANCES(%d) too small\n", MAX_RETIMER_INSTANCES));
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_TOO_FEW_RETIMER_INSTANCES;
      Status = EFI_INVALID_PARAMETER;
      return Status;
    }

    if ((RetimerPayloadItem->ImageOffset + RetimerPayloadItem->ImageSize) > ImageSize) {
      DEBUG ((DEBUG_ERROR, "Retimer payload is out of bounds\n"));
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_PAYLOAD_IS_OUT_OF_BOUNDS_2;
      Status = EFI_INVALID_PARAMETER;
      return Status;
    }

    if (RetimerPayloadItem->FirmwareType == INTEGRATED_TBT_RETIMER) {
      DEBUG ((
        DEBUG_INFO,
        "PayLoad - Integrated TBT retimer address B%d D%d F%d, Tbt Port %d, Retimer Index %d\n",
        RetimerPayloadItem->RetimerDevAddress.Bus,
        RetimerPayloadItem->RetimerDevAddress.Device,
        RetimerPayloadItem->RetimerDevAddress.Function,
        RetimerPayloadItem->RetimerDevAddress.Port,
        RetimerPayloadItem->RetimerDevAddress.CascadedRetimerIndex
        ));
    } else if (RetimerPayloadItem->FirmwareType == DISCRETE_TBT_RETIMER) {
      DEBUG ((
        DEBUG_INFO,
        "PayLoad - Discrete TBT retimer's Pcie root port type value %x, root port number %d, TBT Port %d, Retimer Index %d\n",
        RetimerPayloadItem->PcieRpType,
        RetimerPayloadItem->PcieRootPort,
        RetimerPayloadItem->RetimerDevAddress.Port,
        RetimerPayloadItem->RetimerDevAddress.CascadedRetimerIndex
        ));
    } else {
      DEBUG ((DEBUG_ERROR, "PayLoad - Update failed on Un-Support FirmwareType value %d\n",
        RetimerPayloadItem->FirmwareType));
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_UNSUPPORT_FIRMWARE_TYPE;
      continue;
    }

    //
    // Construct Retimer device
    //
    RetimerDevice = NULL;
    Status = CreateRetimerDevInstance (
               RetimerPayloadItem->FirmwareType,
               RetimerPayloadItem->PcieRpType,
               RetimerPayloadItem->PcieRootPort,
               &RetimerPayloadItem->RetimerDevAddress,
               &RetimerDevice);
    if (EFI_ERROR (Status) || (RetimerDevice == NULL)) {
      DEBUG ((DEBUG_ERROR, "CreateRetimerDevInstance failed (%r) at instance index %d\n", Status, Index));
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_INITIALIZATION_FAILED;
      return Status;
    }
    //
    // Add to Retimer list
    //
    RetimerDeviceInstances[*RetimerDeviceInstancesCount] = RetimerDevice;
    ++(*RetimerDeviceInstancesCount);
  }

  //
  // Create Host Router device instances for sending OFFLINE_MODE(EXIT) to the specified HR and specified port
  //
  Status = CreateHrDevInstance (
             RetimerDeviceInstances,
             RetimerDeviceInstancesCount,
             HrDeviceInstances,
             HrDeviceInstancesCount
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateHrDevInstance failed (%r) at Host Router instance index %d\n", Status, Index));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_INITIALIZATION_FAILED;
    return Status;
  }

  Status = EFI_SUCCESS;
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
  IN  CONST VOID                                     *VendorCode,       OPTIONAL
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,          OPTIONAL
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
  EFI_STATUS                          Status;
  EFI_STATUS                          RestoreStatus;
  USBC_RETIMER_PROTOCOL               *UsbCRetimerProtocol;
  EFI_HANDLE                          *DeviceHandleBuffer;
  UINTN                               DeviceHandleCount;
  UINTN                               HandleIndex;
  PAYLOAD_HEADER                      *RetimerPayloadHeader;
  RETIMER_ITEM                        *RetimerPayloadItem;
  UINTN                               Index;
  RETIMER_DEV_INSTANCE                RetimerDevice;
  RETIMER_DEV_INSTANCE                RetimerDeviceInstances[MAX_RETIMER_INSTANCES];
  UINT32                              RetimerDeviceInstancesCount;
  HR_DEV_INSTANCE                     HrDeviceInstances[MAX_RETIMER_INSTANCES];
  UINT32                              HrDeviceInstancesCount;
  UINT32                              RetimerVersion;
  USBC_PROGRESS_CODE_PROTOCOL         *UsbCProgressCodeProtocol;
  UINTN                               WaitForRetimerReadyToUpdate;

  DEBUG ((DEBUG_INFO, "%a (UsbC Retimer) - Start\n", __FUNCTION__));

  if (Image == NULL) {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_IMAGE_NOT_PROVIDED;
    return EFI_INVALID_PARAMETER;
  }

  if (Progress == NULL) {
    DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - Invalid progress callback\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_PROGRESS_CALLBACK_ERROR;
    return EFI_INVALID_PARAMETER;
  }

  //
  // Test Progress Callback function
  //
  Status = Progress (5);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - Progress Callback failed with Status %r.\n", Status));
  }

  //
  // Enforce all TBT ports into TBT mode
  //
  Status = gBS->LocateProtocol (&gUsbCRetimerProtocolGuid, NULL, (VOID**) &UsbCRetimerProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - Failed to locate UsbCRetimerProtocol (%r).\n", Status));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_USBC_RETIMER_PROTOCOL_NOT_FOUND;
    return Status;
  }

  //
  // Locate UsbC Capsule Debug Progress Code Protocol
  //
  Status = gBS->LocateProtocol (&gUsbCCapsuleDebugProgressCodeProtocolGuid, NULL, (VOID**) &UsbCProgressCodeProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - Failed to locate UsbCProgressCodeProtocol (%r).\n", Status));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_USBC_PROGRESS_CODE_PROTOCOL_NOT_FOUND;
    return Status;
  }

  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_START);
  RetimerDeviceInstancesCount = 0;
  HrDeviceInstancesCount = 0;
  DeviceHandleBuffer = NULL;

  Status = ConstructRetimerInstances (
             Image,
             ImageSize,
             LastAttemptStatus,
             RetimerDeviceInstances,
             &RetimerDeviceInstancesCount,
             HrDeviceInstances,
             &HrDeviceInstancesCount
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - Failed to ConstructRetimerInstances(%r). LastAttemptStatus(%d)\n", Status, *LastAttemptStatus));
    goto FreeInstances;
  }

  //
  // Boundary check is supposed to be taken case by FmpDeviceCheckImage above already.
  //
  DeviceHandleBuffer   = NULL;
  DeviceHandleCount    = 0;
  RetimerPayloadHeader = (PAYLOAD_HEADER *) Image;
  RetimerPayloadItem   = (RETIMER_ITEM *) (RetimerPayloadHeader + 1);

  for (Index = 0; Index < RetimerPayloadHeader->PayloadCount; Index++, RetimerPayloadItem++) {
    if (RetimerPayloadItem->FirmwareType == DISCRETE_TBT_RETIMER) {
      //
      // Locate all Usb protocols handles.
      //
      Status = gBS->LocateHandleBuffer (
                      ByProtocol,
                      &gEfiUsb2HcProtocolGuid,
                      NULL,
                      &DeviceHandleCount,
                      &DeviceHandleBuffer
                      );
      if (EFI_ERROR (Status)) {
        *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_USB2HC_PROTOCOL_NOT_FOUND;
        goto ConnectHc;
      }
      DEBUG ((DEBUG_INFO, "UsbC Retimer Capsule - DisconnectController gEfiUsb2HcProtocolGuid\n"));
      //
      // Disconnect the driver from all the devices in the handle database.
      // This for avoid Chip Hardware Reset wait until the Controller Not Ready (CNR) flag
      // in the USBSTS is '0' before writing any xHC Operational or Runtime registers.
      //
      if (DeviceHandleBuffer != NULL) {
        for (HandleIndex = 0; HandleIndex < DeviceHandleCount; HandleIndex++) {
          DEBUG ((DEBUG_INFO, "UsbC Retimer Capsule - DisconnectController gEfiUsb2HcProtocolGuid HandleIndex %x\n", HandleIndex));
          gBS->DisconnectController (
                  (DeviceHandleBuffer)[HandleIndex],
                  NULL,
                  NULL
                  );
        }
      }
    }
  }

  //
  // Send OFFLINE_MODE_ENTRY to the specified HR and specified port
  //
  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_OFFLINE_MODE_ENTRY);
  for (Index = 0; Index < HrDeviceInstancesCount; Index++) {
    Status = TbtSendOfflineMode (HrDeviceInstances[Index], OFFLINE_MODE_ENTRY);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - TbtSendOfflineMode failed (%r) on HR instance index %d\n", Status, Index));
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_SEND_OFFLINE_MODE_FAILED;
      goto OfflineModeExit;
    }
  }

  DEBUG ((DEBUG_INFO, "UsbC Retimer Capsule - Drive all TBT ports to TBT mode...\n"));

  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_PD_DRIVE);
  Status = DriveToFwUpdateMode (UsbCRetimerProtocol, gAllTbtRetimerDeviceGuid);
  WaitForRetimerReadyToUpdate = (UINTN) PcdGet32 (PcdTcssWaitRetimerDeviceReady);

  if (EFI_ERROR (Status)) {
    if (Status != EFI_ALREADY_STARTED) {
      DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - Failed to drive ports to TBT mode (%r).\n", Status));
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_DRIVE_TBT_MODE_FAILED;
      goto RestorePdPowerMode;
    }
  } else {
    //
    // Time delay for Retimer device ready.
    //
    DEBUG ((DEBUG_INFO, "Time delay for Retimer device ready = %d(MicroSecond)\n", WaitForRetimerReadyToUpdate));
    MicroSecondDelay (WaitForRetimerReadyToUpdate);
  }

  //
  // BIOS should assert retimer ForcePower GPIO after completing BIOS EC-PD command to avoid retimer entering sleep mode
  //
  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_RETIMER_FP_GPIO_ASSERT);
  if (UsbCRetimerProtocol->RetimerFP != NULL) {
    Status = UsbCRetimerProtocol->RetimerFP (RETIMER_FORCE_POWER_GPIO_HIGH);
    DEBUG ((DEBUG_INFO, "UsbC Retimer Capsule - Assert Retimer ForcePower GPIO\n"));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - Failed to assert Retimer Force Power GPIO (%r).\n", Status));
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_ASSERT_RETIMER_FORCE_POWER_FAILED;
      goto DeAssertRetimerForcePowerGpio;
    }
  }

  DEBUG ((DEBUG_INFO, "UsbC Retimer Capsule - Update TBT Retimer - Start\n"));

  DEBUG ((DEBUG_INFO, "UsbC Retimer Capsule - Total of Retimer Payload = %d\n", RetimerPayloadHeader->PayloadCount));

  RetimerPayloadItem = (RETIMER_ITEM *) (RetimerPayloadHeader + 1);
  //
  // Start to update Retimer firmware
  //
  for (Index = 0; Index < RetimerPayloadHeader->PayloadCount && Index < RetimerDeviceInstancesCount; Index++, RetimerPayloadItem++) {

    DEBUG ((DEBUG_INFO, "UsbC Retimer Capsule - Update Retimer Payload = %d\n", Index + 1));
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_UPDATE_RETIMER_PAYLOAD, (UINT32) (Index + 1), 0);
    DEBUG ((DEBUG_INFO, "UsbC Retimer Capsule - ImageOffset=0x%x, size=0x%x\n", RetimerPayloadItem->ImageOffset, RetimerPayloadItem->ImageSize));
    RetimerDevice = RetimerDeviceInstances[Index];

    Status = InitRetimerHW (RetimerDevice);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - InitRetimerHW Fail (%r)\n", Status));
      continue;
    }

    if ((RetimerPayloadItem->ImageOffset + RetimerPayloadItem->ImageSize) > ImageSize) {
      DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - Retimer payload is out of bounds\n"));
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_PAYLOAD_IS_OUT_OF_BOUNDS_2;
      Status = EFI_INVALID_PARAMETER;
      continue;
    }

    UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_UPDATE_RETIMER_NVM_FW);
    Status = UpdateRetimerNvmFirmware (
               RetimerDevice,
               (UINT8 *) Image + RetimerPayloadItem->ImageOffset,
               RetimerPayloadItem->ImageSize,
               Progress,
               6 + (90 / RetimerPayloadHeader->PayloadCount) * Index,
               6 + (90 / RetimerPayloadHeader->PayloadCount) * (Index + 1)
               );
    if (!EFI_ERROR (Status)) {
      RetimerVersion = 0;
      Status = ReadRetimerNvmVersion (RetimerDevice, &RetimerVersion);
      DEBUG ((DEBUG_INFO, "UsbC Retimer Capsule - Retimer version after the update is 0x%x (%r) at image index %d\n", RetimerVersion, Status, Index));
      if (!EFI_ERROR (Status)) {
        UpdateRetimerNvmInformation (
        RetimerPayloadItem->FirmwareType,
        RetimerPayloadItem->PcieRpType,
        RetimerPayloadItem->PcieRootPort,
        &RetimerPayloadItem->RetimerDevAddress,
        RetimerVersion);
      }
    }

    //
    // Send LSUP disable command (USUP) to the target retimer
    //
    Status = TerminateRetimerHW (RetimerDeviceInstances[Index]);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - TerminateRetimerHW fail :%r\n", Status));
    }
  }

DeAssertRetimerForcePowerGpio:
  //
  // De-assert Retimer ForcePower GPIO before sending restore command to EC
  //
  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_RETIMER_FP_GPIO_DEASSERT);
  if (UsbCRetimerProtocol->RetimerFP != NULL) {
    DEBUG ((DEBUG_INFO, "UsbC Retimer Capsule - De-assert Retimer ForcePower GPIO\n"));
    Status = UsbCRetimerProtocol->RetimerFP (RETIMER_FORCE_POWER_GPIO_LOW);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - Failed to de-assert Retimer Force Power GPIO (%r).\n", Status));
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_DEASSERT_RETIMER_FORCE_POWER_FAILED;
    }
  }
RestorePdPowerMode:
  //
  // Brings all TBT ports back to the original mode
  //
  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_PD_RESTORE);
  RestoreStatus = RestoreToOriginalMode (UsbCRetimerProtocol, gAllTbtRetimerDeviceGuid);
  if (EFI_ERROR (RestoreStatus)) {
    DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - Restore to TBT ports to original mode (%r).\n", RestoreStatus));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_RESTORE_ORIGINAL_MODE_FAILED;
  }

OfflineModeExit:
  //
  // Send OFFLINE_MODE_EXIT to the specified HR and specified port
  //
  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_OFFLINE_MODE_EXIT);
  for (Index = 0; Index < HrDeviceInstancesCount; Index++) {
    Status = TbtSendOfflineMode (HrDeviceInstances[Index], OFFLINE_MODE_EXIT);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UsbC Retimer Capsule - TbtSendOfflineMode failed (%r) on HR instance index %d\n", Status, Index));
    }
  }

ConnectHc:

  if (DeviceHandleBuffer != NULL) {
    DEBUG ((DEBUG_INFO, "UsbC Retimer Capsule - ConnectController gEfiUsb2HcProtocolGuid\n"));
    for (HandleIndex = 0; HandleIndex < DeviceHandleCount; HandleIndex++) {
      gBS->ConnectController (DeviceHandleBuffer[HandleIndex], NULL, NULL, TRUE);
    }
    // Free the buffer containing the list of handles from the handle database
    gBS->FreePool (DeviceHandleBuffer);
  }

FreeInstances:
  //
  // Free up resources
  //
  for (Index = 0; Index < RetimerDeviceInstancesCount; Index++) {
    DEBUG ((DEBUG_INFO, "UsbC Retimer Capsule - DestroyRetimerDevInstance: Retimer Instance %x\n", Index));
    DestroyRetimerDevInstance (RetimerDeviceInstances[Index]);
  }

  Progress (100);

  DEBUG ((DEBUG_INFO, "%a (UsbC Retimer) - End\n", __FUNCTION__));
  if (EFI_ERROR (Status)) {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_TBT_RETIMER_ERROR_UPDATE_FAILED;
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

  @retval  EFI_SUCCESS      The firmware device was locked.
  @retval  EFI_UNSUPPORTED  The firmware device does not support locking

**/
EFI_STATUS
EFIAPI
FmpDeviceLock (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "FmpDeviceLib(TBT Retimer): FmpDeviceLock() for system FLASH\n"));
  return EFI_UNSUPPORTED;
}
