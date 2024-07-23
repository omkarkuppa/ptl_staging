/**@file
  Storage erase driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _STORAGE_ERASE_H_
#define _STORAGE_ERASE_H_

#include <PiDxe.h>

#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/StorageSanitizeNotifyProtocol.h>
#include <Protocol/PlatformOpalProtocol.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/UsbIo.h>
#include <Guid/EventGroup.h>
#include <Guid/PlatformSanitizeVariable.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PciLib.h>
#include <Library/StorageServiceLib.h>
#include <Library/EraseServiceLib.h>
#include <Library/StoragePlatformEventLib.h>
#include <Library/SanitizeUiLib.h>
#include <Library/RngLib.h>
#include <StorageSanitizeDefinitions.h>

#define EFI_DRIVER_NAME_UNICODE    L"Storage erase Driver"
#define REPORT_DATA_MAX_LENGTH     3000
#define REPORT_USB_MAX_ATTEPMT     1
#define MAX_DEVICE_IN_FILE         3
#define MAX_DEVICE_SUPPORT         15
#define MAX_NUM_OF_DOCUMENTATION   (MAX_DEVICE_SUPPORT / MAX_DEVICE_IN_FILE)
#define VERIFY_BLOCK_NUMBER        SIZE_1KB
#define MAX_BLOCK_SIZE             SIZE_4GB
#define ERASE_MESSAGE              L": Erasing..."
#define VERIFICATION_MESSAGE       L": Erase successful, Verifing ..."

// UEFI 2.1
#define LANGUAGE_RFC_3066_ENGLISH ((CHAR8*)"en")

// UEFI/EFI < 2.1
#define LANGUAGE_ISO_639_2_ENGLISH ((CHAR8*)"eng")

#define BUFF_SIZE            0x00001000  //4K
#define MAX_STRING_LEN       200

extern EFI_DRIVER_BINDING_PROTOCOL   gStorageEraseDriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL   gStorageEraseComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL  gStorageEraseComponentName2;

//
// CHILD_DEVICE is a linked list to store child device information.
//
#define PLATFORM_ERASE_DEVICE_SIGNATURE     SIGNATURE_32 ('S', 'E', 'C', 'D')
#define PLATFORM_ERASE_DEVICE_FROM_LINK(a)  CR(a, PLATFORM_ERASE_DEVICE, Link, PLATFORM_ERASE_DEVICE_SIGNATURE)

typedef struct _PLATFORM_ERASE_DEVICE {
  UINTN                            Signature;
  LIST_ENTRY                       Link;
  COMMAND_ID                       CmdId;
  EFI_HANDLE                       Handle;                 ///< Device handle
  EFI_HANDLE                       ParentHandle;           ///< Parent Device handle
  DEVICE_TYPE                      DeviceType;      //0: NVMe  1: UFS 2: SATA
  UINT8                            Selected;
  UINT8                            VmdControlled;
    //#define DEVICE_IS_RAID_VOLUME     BIT1
    #define DEVICE_IN_VMD_CONTROLLED  BIT0
  ERASE_INFO                       EraseInfo;     // Erase result status and device information which returned from erase block protocol
  UINT8                            Result;
  BOOLEAN                          Erased;
  CHAR8                            Password[PASSWORD_MAX_LENGTH + 1];
} PLATFORM_ERASE_DEVICE;

//
// Structure for storing data for documentation
//
#define DOC_INFO_SIGNATURE     SIGNATURE_32 ('D', 'C', 'I', 'F')
#define DOC_INFO_FROM_LINK(a)  CR(a, DOC_INFO, Link, DOC_INFO_SIGNATURE)

typedef struct {
  UINTN          Signature;
  LIST_ENTRY     Link;
  CHAR8          *ModelName;
  CHAR8          *SerialNumber;
  CHAR8          *MediaType;
  CHAR8          *EraseCat;
  CHAR8          *EraseType;
  EFI_STATUS      ValResult;
} DOC_INFO;

//
// Data structure of verify postpone
//
typedef struct {
  EFI_LBA Lba;
  UINT32  Size;
  VOID    *Data;
} VERIFY_DATA;

typedef struct {
  PLATFORM_ERASE_DEVICE *Dev;
  UINT64                Length;
  VERIFY_DATA           *BackupLba;
} VERIFY_INFO;

//
// Private structure of driver
//
#define STORAGE_ERASE_DRIVER_SIGNATURE     SIGNATURE_32 ('R', 'B', 'E', 'D')

typedef struct {
  UINTN                             Signature;
  EFI_HANDLE                        Handle;                   // Driver image handle
  LIST_ENTRY                        PlatformEraseDeviceList;  // Linked list of platform erase device

  BOOLEAN                           PerformUsbReport;
  BOOLEAN                           PerformVerify;

  LIST_ENTRY                        DocInfoList;
  ERASE_DOCUMENTATION               EraseDocumentation[MAX_NUM_OF_DOCUMENTATION];
  UINT8                             NumOfDocumentation;
  UINT8                             NumOfVerifyPostpone;
  VERIFY_INFO                       VerifyInfo[MAX_DEVICE_SUPPORT];
} STORAGE_ERASE_DRIVER;

extern STORAGE_ERASE_DRIVER  mStorageEraseDriver;
extern UINT8 mSanitizeOrder[SANITIZE_COUNT];

/**
  Tests to see if this driver supports a given controller.

  This function checks to see if the controller contains an instance of the
  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL and the EFI_BLOCK_IO_PROTOCOL
  and returns EFI_SUCCESS if it does.

  @param[in]  This                  A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle      The Handle of the controller to test. This Handle
                                    must support a protocol interface that supplies
                                    an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  This parameter is ignored.

  @retval EFI_SUCCESS               The device contains required protocols
  @retval EFI_ALREADY_STARTED       The device specified by ControllerHandle and
                                    RemainingDevicePath is already being managed by the driver
                                    specified by This.
  @retval EFI_ACCESS_DENIED         The device specified by ControllerHandle and
                                    RemainingDevicePath is already being managed by a different
                                    driver or an application that requires exclusive access.
                                    Currently not implemented.
  @retval EFI_UNSUPPORTED           The device does not contain requires protocols

**/
EFI_STATUS
EFIAPI
StorageEraseBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL*  This,
  IN EFI_HANDLE                    Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*     RemainingDevicePath
  );

/**
  Enables Storage erase Management on a supported device if available.

  The start function is designed to be called after the Opal UEFI Driver has confirmed the
  "controller", which is a child Handle, contains the EF_STORAGE_SECURITY_COMMAND protocols.
  This function will complete the other necessary checks, such as verifying the device supports
  the correct version of Opal.  Upon verification, it will add the device to the
  Opal HII list in order to expose Opal management options.

  @param[in] This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in] ControllerHandle     The Handle of the controller to start. This Handle
                                  must support a protocol interface that supplies
                                  an I/O abstraction to the driver.
  @param[in] RemainingDevicePath  A pointer to the remaining portion of a device path.  This
                                  parameter is ignored by device drivers, and is optional for bus
                                  drivers. For a bus driver, if this parameter is NULL, then handles
                                  for all the children of Controller are created by this driver.
                                  If this parameter is not NULL and the first Device Path Node is
                                  not the End of Device Path Node, then only the Handle for the
                                  child device specified by the first Device Path Node of
                                  RemainingDevicePath is created by this driver.
                                  If the first Device Path Node of RemainingDevicePath is
                                  the End of Device Path Node, no child Handle is created by this
                                  driver.

  @retval EFI_SUCCESS           Storage erase management was enabled.
  @retval EFI_DEVICE_ERROR      The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval Others                The driver failed to start the device.

**/
EFI_STATUS
EFIAPI
StorageEraseBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL*  This,
  IN EFI_HANDLE                    Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*     RemainingDevicePath
  );

/**
  Stop this driver on Controller.

  @param[in] This               Protocol instance pointer.
  @param[in] Controller         Handle of device to stop driver on
  @param[in] NumberOfChildren   Number of Handles in ChildHandleBuffer. If number of
                                children is zero stop the entire bus driver.
  @param[in] ChildHandleBuffer  List of Child Handles to Stop.

  @retval EFI_SUCCESS  This driver is removed Controller.
  @retval other        This driver could not be removed from this device.

**/
EFI_STATUS
EFIAPI
StorageEraseBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL*  This,
  IN EFI_HANDLE                    Controller,
  IN UINTN                         NumberOfChildren,
  IN EFI_HANDLE*                   ChildHandleBuffer
  );

/**
  Retrieves a Unicode string that is the user readable name of the driver.

  This function retrieves the user readable name of a driver in the form of a
  Unicode string. If the driver specified by This has a user readable name in
  the language specified by Language, then a pointer to the driver name is
  returned in DriverName, and EFI_SUCCESS is returned. If the driver specified
  by This does not support the language specified by Language,
  then EFI_UNSUPPORTED is returned.

  @param[in] This               A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param[in] Language           A pointer to a Null-terminated ASCII string
                                array indicating the language. This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified
                                in RFC 4646 or ISO 639-2 language code format.

  @param[out] DriverName        A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                driver specified by This in the language
                                specified by Language.

  @retval EFI_SUCCESS           The Unicode string for the Driver specified by
                                This and the language specified by Language was
                                returned in DriverName.

  @retval EFI_INVALID_PARAMETER Language is NULL.

  @retval EFI_INVALID_PARAMETER DriverName is NULL.

  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/
EFI_STATUS
EFIAPI
StorageEraseComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL     *This,
  IN  CHAR8                           *Language,
  OUT CHAR16                          **DriverName
  );

/**
  Retrieves a Unicode string that is the user readable name of the controller
  that is being managed by a driver.

  This function retrieves the user readable name of the controller specified by
  ControllerHandle and ChildHandle in the form of a Unicode string. If the
  driver specified by This has a user readable name in the language specified by
  Language, then a pointer to the controller name is returned in ControllerName,
  and EFI_SUCCESS is returned.  If the driver specified by This is not currently
  managing the controller specified by ControllerHandle and ChildHandle,
  then EFI_UNSUPPORTED is returned.  If the driver specified by This does not
  support the language specified by Language, then EFI_UNSUPPORTED is returned.

  @param[in] This               A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param[in] ControllerHandle   The handle of a controller that the driver
                                specified by This is managing.  This handle
                                specifies the controller whose name is to be
                                returned.

  @param[in] ChildHandle        The handle of the child controller to retrieve
                                the name of.  This is an optional parameter that
                                may be NULL.  It will be NULL for device
                                drivers.  It will also be NULL for a bus drivers
                                that wish to retrieve the name of the bus
                                controller.  It will not be NULL for a bus
                                driver that wishes to retrieve the name of a
                                child controller.

  @param[in] Language           A pointer to a Null-terminated ASCII string
                                array indicating the language.  This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified in
                                RFC 4646 or ISO 639-2 language code format.

  @param[out] ControllerName    A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                controller specified by ControllerHandle and
                                ChildHandle in the language specified by
                                Language from the point of view of the driver
                                specified by This.

  @retval EFI_SUCCESS           The Unicode string for the user readable name in
                                the language specified by Language for the
                                driver specified by This was returned in
                                DriverName.

  @retval EFI_INVALID_PARAMETER ControllerHandle is NULL.

  @retval EFI_INVALID_PARAMETER ChildHandle is not NULL and it is not a valid
                                EFI_HANDLE.

  @retval EFI_INVALID_PARAMETER Language is NULL.

  @retval EFI_INVALID_PARAMETER ControllerName is NULL.

  @retval EFI_UNSUPPORTED       The driver specified by This is not currently
                                managing the controller specified by
                                ControllerHandle and ChildHandle.

  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/
EFI_STATUS
EFIAPI
StorageEraseComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  EFI_HANDLE                   ControllerHandle,
  IN  EFI_HANDLE                   ChildHandle,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **ControllerName
  );

/**
  Retrieves a Unicode string that is the user readable name of the driver.

  This function retrieves the user readable name of a driver in the form of a
  Unicode string. If the driver specified by This has a user readable name in
  the language specified by Language, then a pointer to the driver name is
  returned in DriverName, and EFI_SUCCESS is returned. If the driver specified
  by This does not support the language specified by Language,
  then EFI_UNSUPPORTED is returned.

  @param[in] This               A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param[in] Language           A pointer to a Null-terminated ASCII string
                                array indicating the language. This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified
                                in RFC 4646 or ISO 639-2 language code format.

  @param[out] DriverName        A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                driver specified by This in the language
                                specified by Language.

  @retval EFI_SUCCESS           The Unicode string for the Driver specified by
                                This and the language specified by Language was
                                returned in DriverName.

  @retval EFI_INVALID_PARAMETER Language is NULL.

  @retval EFI_INVALID_PARAMETER DriverName is NULL.

  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/
EFI_STATUS
EFIAPI
StorageEraseComponentName2GetDriverName (
  IN  EFI_COMPONENT_NAME2_PROTOCOL  *This,
  IN  CHAR8                         *Language,
  OUT CHAR16                        **DriverName
  );

/**
  Retrieves a Unicode string that is the user readable name of the controller
  that is being managed by a driver.

  This function retrieves the user readable name of the controller specified by
  ControllerHandle and ChildHandle in the form of a Unicode string. If the
  driver specified by This has a user readable name in the language specified by
  Language, then a pointer to the controller name is returned in ControllerName,
  and EFI_SUCCESS is returned.  If the driver specified by This is not currently
  managing the controller specified by ControllerHandle and ChildHandle,
  then EFI_UNSUPPORTED is returned.  If the driver specified by This does not
  support the language specified by Language, then EFI_UNSUPPORTED is returned.

  @param[in] This               A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param[in] ControllerHandle   The handle of a controller that the driver
                                specified by This is managing.  This handle
                                specifies the controller whose name is to be
                                returned.

  @param[in] ChildHandle        The handle of the child controller to retrieve
                                the name of.  This is an optional parameter that
                                may be NULL.  It will be NULL for device
                                drivers.  It will also be NULL for a bus drivers
                                that wish to retrieve the name of the bus
                                controller.  It will not be NULL for a bus
                                driver that wishes to retrieve the name of a
                                child controller.

  @param[in] Language           A pointer to a Null-terminated ASCII string
                                array indicating the language.  This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified in
                                RFC 4646 or ISO 639-2 language code format.

  @param[out] ControllerName    A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                controller specified by ControllerHandle and
                                ChildHandle in the language specified by
                                Language from the point of view of the driver
                                specified by This.

  @retval EFI_SUCCESS           The Unicode string for the user readable name in
                                the language specified by Language for the
                                driver specified by This was returned in
                                DriverName.

  @retval EFI_INVALID_PARAMETER ControllerHandle is NULL.

  @retval EFI_INVALID_PARAMETER ChildHandle is not NULL and it is not a valid
                                EFI_HANDLE.

  @retval EFI_INVALID_PARAMETER Language is NULL.

  @retval EFI_INVALID_PARAMETER ControllerName is NULL.

  @retval EFI_UNSUPPORTED       The driver specified by This is not currently
                                managing the controller specified by
                                ControllerHandle and ChildHandle.

  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/
EFI_STATUS
EFIAPI
StorageEraseComponentName2GetControllerName(
  IN  EFI_COMPONENT_NAME2_PROTOCOL  *This,
  IN  EFI_HANDLE                    ControllerHandle,
  IN  EFI_HANDLE                    ChildHandle,
  IN  CHAR8                         *Language,
  OUT CHAR16                        **ControllerName
  );

/**
  Generate a report and write it to USB

  @retval EFI_SUCCESS  The function completes successfully
  @retval others       Operation is failed due to some error.

**/
EFI_STATUS
GetStorageEraseReport (
  VOID
  );

/**
  Write JSON file to external USB device.

**/
VOID
WriteToUsb (
  VOID
  );

/**
  Get device type

  @param[in] DevicePath  Device path.

  @retval DEVICE_TYPE   return device type

**/
DEVICE_TYPE
GetDeviceType (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  );

/**
  Perform storage erase on device.

  @param[in] Dev              The device to perform data erase.
  @param[in] Password         The password used to unlock device.
  @param[in] HasOutputConsole The output console connected or not.

  @retval EFI_SUCCESS  The function completes successfully
  @retval others       Operation is failed due to some error.
**/
EFI_STATUS
PerformDeviceErase (
  IN PLATFORM_ERASE_DEVICE  *Dev,
  IN CHAR8*                 Password,
  IN BOOLEAN                HasOutputConsole
  );

/**
  Perform unlock storage.

  @param[in] Dev              The device to perform data erase.
  @param[in] Password         The password used to unlock device.
  @param[in] HasOutputConsole The output console connected or not.

  @retval EFI_SUCCESS     Device is unlock.
  @retval Others          failed to unlock the device.
**/
EFI_STATUS
PerformUnlockStorage (
  IN PLATFORM_ERASE_DEVICE  *Dev,
  IN CHAR8*                 Password,
  IN BOOLEAN                HasOutputConsole
  );

/**
  Check on device if it need G3.

  @param[in] Dev            The device to perform data erase.

  @retval EFI_SUCCESS  The function completes successfully
  @retval others       Operation is failed due to some error.
**/
BOOLEAN
CheckG3Status (
  IN PLATFORM_ERASE_DEVICE  *Dev
  );

/**
  Convert result status to string.

  @param[in] Result         Secure erase result.
  @param[in] PerformVerify  Verify is enabled or not.

  @retval UINT16*           Result string.
**/
UINT16 *
ResultString (
  IN UINT8    Result,
  IN BOOLEAN  PerformVerify
  );

/**
  Print message for erasing

  @param[in] Name  Device name.
  @param[in] Type  Device type.
  @param[in] Info  Information shown in print message.

**/
VOID
PrintEraseMessage (
  IN CHAR8       *Name,
  IN DEVICE_TYPE Type,
  IN CHAR16      *Info
  );

/**
  Update message

  @param[in] Name  Device name.
  @param[in] Type  Device type.
  @param[in] Info  Information shown in print message.

**/
VOID
UpdateEraseMessage (
  IN CHAR8       *Name,
  IN DEVICE_TYPE Type,
  IN CHAR16      *Info
  );

/**
  Append time at end of message

  @param[in] Time  Time to be shown.

**/
VOID
AppendEraseTime (
  IN UINT64  Time
);

/**
  Verify all erased devcie, check if all data is erased.

**/
VOID
VerifyEraseResult (
  PLATFORM_ERASE_DEVICE  *Dev
  );

/**
  Check if it is the specific device, the data doesn't clear to zero.

  @param[in] Dev       Point to the platform erase list.

  @retval TRUE         it is the specific device.
  @retval FALSE        It is not the specific device.
**/
BOOLEAN
VerifySpecificDevice (
  IN PLATFORM_ERASE_DEVICE  *Dev
  );

/**
  Backup the Lba for late verification.

  @param[in] VerifyInfo  Point to the structure contains the backup Lba.
**/
VOID
BackupLba (
  IN VERIFY_INFO *VerifyInfo
  );

/**
  verify the Backup the Lba if it is erased and reallocate.

  @param[in] VerifyInfo  Point to the structure contains the backup Lba.
**/
VOID
VerifyBackupLba (
  IN VERIFY_INFO *VerifyInfo
  );

/**
  Update device command id, sata port id or nvme namespace id

  @param[in] DevicePath  Device path.
  @param[in] Dev         The device to update the command id.

**/
VOID
UpdateDeviceCmdId (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN PLATFORM_ERASE_DEVICE     *Dev
  );

/**
  Get the time for verify device.

  @param[in] Dev  The device to verify.

  @retval UINT64  Time need to verify
**/
UINT64
GetVerifyTime (
  IN PLATFORM_ERASE_DEVICE  *Dev
  );
#endif //_STORAGE_ERASE_H_
