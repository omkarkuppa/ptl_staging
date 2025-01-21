/**@file
  Platform Opal Driver

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

@par Specification
**/

#ifndef _PLATFORM_OPAL_DRIVER_H_
#define _PLATFORM_OPAL_DRIVER_H_

#include <PiDxe.h>

#include <IndustryStandard/Pci.h>
#include <IndustryStandard/Atapi.h>

#include <Protocol/PciIo.h>
#include <Protocol/SmmCommunication.h>
#include <Protocol/BlockIo.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/StorageSecurityCommand.h>

#include <Guid/EventGroup.h>

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PciLib.h>
#include <Library/TcgStorageOpalLib.h>
#include <Protocol/PlatformOpalProtocol.h>

#define EFI_DRIVER_NAME_UNICODE L"Platform Opal Driver"

// UEFI 2.1
#define LANGUAGE_RFC_3066_ENGLISH ((CHAR8*)"en")

// UEFI/EFI < 2.1
#define LANGUAGE_ISO_639_2_ENGLISH ((CHAR8*)"eng")

extern EFI_DRIVER_BINDING_PROTOCOL   gPlatformOpalDriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL   gPlatformOpalComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL  gPlatformOpalComponentName2;

#define OPAL_MSID_LENGTH        128

#define MAX_OPAL_PASSWORD_TRY_COUNT  5

// PSID Length
#define PSID_CHARACTER_LENGTH   0x20
#define MAX_PSID_TRY_COUNT      5

//
// The max timeout value assume the user can wait for the revert action. The unit of this macro is second.
// If the revert time value bigger than this one, driver needs to popup a dialog to let user confirm the
// revert action.
//
#define MAX_ACCEPTABLE_REVERTING_TIME    10

#define OPAL_MAX_PASSWORD_SIZE      32

#pragma pack(1)
//
// Structure that is used to represent an OPAL_DISK.
//
typedef struct {
  UINT32                                   MsidLength;             // Byte length of MSID Pin for device
  UINT8                                    Msid[OPAL_MSID_LENGTH]; // MSID Pin for device
  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL    *Sscp;
  UINT32                                   MediaId;                // MediaId is used by Ssc Protocol.;
  UINT16                                   OpalBaseComId;          // Opal SSC 1 base com id.
  OPAL_OWNER_SHIP                          Owner;
  OPAL_DISK_SUPPORT_ATTRIBUTE              SupportedAttributes;
  TCG_LOCKING_FEATURE_DESCRIPTOR           LockingFeature;         // Locking Feature Descriptor retrieved from performing a Level 0 Discovery
  UINT8                                    PasswordLength;
  UINT8                                    Password[OPAL_MAX_PASSWORD_SIZE];
} OPAL_DISK;
#pragma pack()

//
// Device with block IO protocol
//
typedef struct _PLATFORM_OPAL_DRIVER_DEVICE PLATFORM_OPAL_DRIVER_DEVICE;

struct _PLATFORM_OPAL_DRIVER_DEVICE {
  PLATFORM_OPAL_DRIVER_DEVICE              *Next;              ///< Linked list pointer
  EFI_HANDLE                               Handle;             ///< Device handle which SSCP installed on
  OPAL_DISK                                OpalDisk;           ///< User context
  UINT32                                   MediaId;            ///< Required parameter for EFI_STORAGE_SECURITY_COMMAND_PROTOCOL, from BLOCK_IO_MEDIA
  CHAR16                                   *Name16;            ///< Allocated/freed by UEFI Filter Driver at device creation/removal
  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL    *Sscp;              /// Device protocols consumed
};

#define PLATFORM_OPA_DRIVER_SIGNATURE SIGNATURE_32 ('p', 'f', 'o', 'd')

//
// Opal Driver UEFI Driver Model
//
typedef struct {
  UINT32                       Signature;
  EFI_HANDLE                   Handle;              ///< Driver image handle
  PLATFORM_OPAL_PROTOCOL       PlatformOpalProtocol;
  PLATFORM_OPAL_DRIVER_DEVICE  *DeviceList;         ///< Linked list of controllers owned by this Driver
} PLATFORM_OPAL_DRIVER;

#define PLATFORM_OPAL_PRIVATE_FROM_PROTOCOL(a)  CR (a, PLATFORM_OPAL_DRIVER, PlatformOpalProtocol, PLATFORM_OPA_DRIVER_SIGNATURE)

/**
  Test to see if this driver supports Controller.

  @param[in] This                 Protocol instance pointer.
  @param[in] ControllerHandle     Handle of device to test
  @param[in] RemainingDevicePath  Optional parameter use to pick a specific child
                                  device to start.

  @retval EFI_SUCCESS          This driver supports this device.
  @retval EFI_ALREADY_STARTED  This driver is already running on this device.
  @retval other                This driver does not support this device.

**/
EFI_STATUS
EFIAPI
PlatformOpalEfiDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL*  This,
  IN EFI_HANDLE                    Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*     RemainingDevicePath
  );

/**
  Enables Opal Management on a supported device if available.

  The start function is designed to be called after the Opal UEFI Driver has confirmed the
  "controller", which is a child handle, contains the EF_STORAGE_SECURITY_COMMAND protocols.
  This function will complete the other necessary checks, such as verifying the device supports
  the correct version of Opal.  Upon verification, it will add the device to the
  Opal HII list in order to expose Opal management options.

  @param[in]  This                  A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle      The handle of the controller to start. This handle
                                    must support a protocol interface that supplies
                                    an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath   A pointer to the remaining portion of a device path.  This
                                    parameter is ignored by device drivers, and is optional for bus
                                    drivers. For a bus driver, if this parameter is NULL, then handles
                                    for all the children of Controller are created by this driver.
                                    If this parameter is not NULL and the first Device Path Node is
                                    not the End of Device Path Node, then only the handle for the
                                    child device specified by the first Device Path Node of
                                    RemainingDevicePath is created by this driver.
                                    If the first Device Path Node of RemainingDevicePath is
                                    the End of Device Path Node, no child handle is created by this
                                    driver.

  @retval EFI_SUCCESS               Opal management was enabled.
  @retval EFI_DEVICE_ERROR          The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES      The request could not be completed due to a lack of resources.
  @retval Others                    The driver failed to start the device.

**/
EFI_STATUS
EFIAPI
PlatformOpalEfiDriverBindingStart (
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
PlatformOpalEfiDriverBindingStop (
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

  @param[in]  This              A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param[in]  Language          A pointer to a Null-terminated ASCII string
                                array indicating the language. This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified
                                in RFC 4646 or ISO 639-2 language code format.

  @param[out]  DriverName       A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                driver specified by This in the language
                                specified by Language.

  @retval EFI_SUCCESS           The Unicode string for the Driver specified by
                                This and the language specified by Language was
                                returned in DriverName.

  @retval EFI_INVALID_PARAMETER  Language is NULL.

  @retval EFI_INVALID_PARAMETER  DriverName is NULL.

  @retval EFI_UNSUPPORTED        The driver specified by This does not support
                                 the language specified by Language.

**/
EFI_STATUS
EFIAPI
PlatformOpalEfiDriverComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
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

  @param[in]  This              A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param[in]  ControllerHandle  The handle of a controller that the driver
                                specified by This is managing.  This handle
                                specifies the controller whose name is to be
                                returned.

  @param[in]  ChildHandle       The handle of the child controller to retrieve
                                the name of.  This is an optional parameter that
                                may be NULL.  It will be NULL for device
                                drivers.  It will also be NULL for a bus drivers
                                that wish to retrieve the name of the bus
                                controller.  It will not be NULL for a bus
                                driver that wishes to retrieve the name of a
                                child controller.

  @param[in]  Language          A pointer to a Null-terminated ASCII string
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
PlatformOpalEfiDriverComponentNameGetControllerName (
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

  @param[in]  This              A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param[in]  Language          A pointer to a Null-terminated ASCII string
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
PlatformOpalEfiDriverComponentName2GetDriverName (
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

  @param[in]  This              A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param[in]  ControllerHandle  The handle of a controller that the driver
                                specified by This is managing.  This handle
                                specifies the controller whose name is to be
                                returned.

  @param[in]  ChildHandle       The handle of the child controller to retrieve
                                the name of.  This is an optional parameter that
                                may be NULL.  It will be NULL for device
                                drivers.  It will also be NULL for a bus drivers
                                that wish to retrieve the name of the bus
                                controller.  It will not be NULL for a bus
                                driver that wishes to retrieve the name of a
                                child controller.

  @param[in]  Language          A pointer to a Null-terminated ASCII string
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
PlatformOpalEfiDriverComponentName2GetControllerName (
  IN  EFI_COMPONENT_NAME2_PROTOCOL  *This,
  IN  EFI_HANDLE                    ControllerHandle,
  IN  EFI_HANDLE                    ChildHandle,
  IN  CHAR8                         *Language,
  OUT CHAR16                        **ControllerName
  );

#endif //_PLATFORM_OPAL_DRIVER_H_
