/**@file

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

#ifndef _I2C_PSS_MONZA_H_
#define _I2C_PSS_MONZA_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/I2cIo.h>
#include <Protocol/I2cPssProtocol.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/ComponentName.h>
#include <Include/MonzaPssChip.h>

//
// Global variables
//
extern EFI_DRIVER_BINDING_PROTOCOL  gPssMonzaDriverBinding;
extern EFI_COMPONENT_NAME2_PROTOCOL gPssMonzaDriverComponentName2;
extern EFI_COMPONENT_NAME_PROTOCOL  gPssMonzaDriverComponentName;


#define PSS_MONZA_SIGNATURE                  SIGNATURE_32 ('P', 'S', 'S', 'M')
#define PSS_MONZA_VERSION                    0x00000000
#define PSS_MONZA_CONTEXT_FROM_PROTOCOL(a)   CR (a, PSS_MONZA_PRIVATE_CONTEXT, PssProtocol, PSS_MONZA_SIGNATURE)

typedef struct {
  UINTN                                 Signature;
  EFI_HANDLE                            ControllerHandle;
  EFI_UNICODE_STRING_TABLE              *ControllerNameTable;
  EFI_I2C_IO_PROTOCOL                   *I2cIoProtocol;
  I2C_PLATFORM_SECURE_STORAGE_PROTOCOL  PssProtocol;
} PSS_MONZA_PRIVATE_CONTEXT;

/**
  Get 16 bytes Serial Number from PSS chip.

  @param[in]       *This              Pointer of I2C_PLATFORM_SECURE_STORAGE_PROTOCOL.
  @param[in, out]  Buffer             Data to store Serial Number from PSS chip

  @retval  EFI_SUCCESS                Get Serial Number from PSS chip successfully.
  @retval  EFI_INVALID_PARAMETER      Buffer is NULL or necessary protocol is not exist.
  @retval  EFI_DEVICE_ERROR           PSS chip detect failure.
  @retval  EFI_NOT_FOUND              No identify PSS chip is found.
**/
EFI_STATUS
EFIAPI
PssMonzaGetSerialNumber (
  IN     I2C_PLATFORM_SECURE_STORAGE_PROTOCOL *This,
  OUT    UINT8                                *Buffer
  );

/**
  Get 16 bytes Mac Address from PSS chip.
  @param[in]       *This              Pointer of I2C_PLATFORM_PROTOCOL.
  @param[in, out]  Buffer             Data to store Mac Address from PSS chip
  @retval  EFI_SUCCESS                Get Mac Address from PSS chip successfully.
  @retval  EFI_INVALID_PARAMETER      Buffer is NULL or necessary protocol is not exist.
  @retval  EFI_DEVICE_ERROR           PSS chip detect failure.
  @retval  EFI_NOT_FOUND              No identify PSS chip is found.
**/
EFI_STATUS
EFIAPI
PssMonzaGetMacAddress (
  IN     I2C_PLATFORM_SECURE_STORAGE_PROTOCOL *This,
  OUT    UINT16                                *Buffer
  );

/**
  Looks for controllers which declare the EFI_I2C_IO_PROTOCOL and match the device path
  supplied by the silicon vendor or third party I2C driver writer to the platform integrator.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to test. This handle
                                   must support a protocol interface that supplies
                                   an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a device path.  This
                                   parameter is ignored by device drivers, and is optional for bus
                                   drivers. For bus drivers, if this parameter is not NULL, then
                                   the bus driver must determine if the bus controller specified
                                   by ControllerHandle and the child controller specified
                                   by RemainingDevicePath are both supported by this
                                   bus driver.

  @retval EFI_SUCCESS              The device specified by ControllerHandle and
                                   RemainingDevicePath is supported by the driver specified by This.
  @retval EFI_ALREADY_STARTED      The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed by the driver
                                   specified by This.
  @retval EFI_ACCESS_DENIED        The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed by a different
                                   driver or an application that requires exclusive access.
                                   Currently not implemented.
  @retval EFI_UNSUPPORTED          The device specified by ControllerHandle and
                                   RemainingDevicePath is not supported by the driver specified by This.
**/
EFI_STATUS
EFIAPI
PssMonzaDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

/**
  Starts PSS Monza chip initialize by I2cIoProtocol.
  If initialization completes successfully, this driver will install gI2cPssProtocolGuid protocol
  on this device. This protocol can be further used to get serial number from PSS chip.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to start. This handle
                                   must support I2C protocol interface.
  @param[in]  RemainingDevicePath  ignored

  @retval EFI_SUCCESS              The device was started.
  @retval EFI_DEVICE_ERROR         The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a lack of resources.
  @retval Others                   The driver failded to start the device.

**/
EFI_STATUS
EFIAPI
PssMonzaDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

/**
  Stops a device controller or a bus controller.

  The Stop() function is designed to be invoked from the EFI boot service DisconnectController().
  As a result, much of the error checking on the parameters to Stop() has been moved
  into this common boot service. It is legal to call Stop() from other locations,
  but the following calling restrictions must be followed, or the system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE that was used on a previous call to this
     same driver's Start() function.
  2. The first NumberOfChildren handles of ChildHandleBuffer must all be a valid
     EFI_HANDLE. In addition, all of these handles must have been created in this driver's
     Start() function, and the Start() function must have called OpenProtocol() on
     ControllerHandle with an Attribute of EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.

  @param[in]  This              A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle  A handle to the device being stopped. The handle must
                                support a bus specific I/O protocol for the driver
                                to use to stop the device.
  @param[in]  NumberOfChildren  The number of child device handles in ChildHandleBuffer.
  @param[in]  ChildHandleBuffer An array of child handles to be freed. May be NULL
                                if NumberOfChildren is 0.

  @retval EFI_SUCCESS           The device was stopped.
  @retval EFI_DEVICE_ERROR      The device could not be stopped due to a device error.

**/
EFI_STATUS
EFIAPI
PssMonzaDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                  ControllerHandle,
  IN UINTN                       NumberOfChildren,
  IN EFI_HANDLE                  *ChildHandleBuffer OPTIONAL
  );

EFI_STATUS
EFIAPI
PssMonzaDriverComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL   *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  );

EFI_STATUS
EFIAPI
PssMonzaDriverComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL   *This,
  IN  EFI_HANDLE                    ControllerHandle,
  IN  EFI_HANDLE                    ChildHandle        OPTIONAL,
  IN  CHAR8                         *Language,
  OUT CHAR16                        **ControllerName
  );
#endif
