/** @file
 Implement the Lpss I2C Driver.

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

#include <Uefi.h>
#include <Base.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SerialIoI2cMasterCommonLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/SerialIoI2cPrivateLib.h>
#include <Library/SerialIoPrivateLib.h>
#include <Library/LpssI2cSocLib.h>
#include <Register/SerialIoRegs.h>
#include <Register/SerialIoI2cRegs.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>
#include <Protocol/I2cMaster.h>

typedef struct {
  UINTN                           Signature;
  EFI_I2C_MASTER_PROTOCOL         HostApi;
  EFI_EVENT                       RequestEvent;
  EFI_STATUS                      *RequestStatus;
  EFI_EVENT                       Timer;
  I2C_MASTER_CONTEXT              Host;
} I2C_DRIVER_CONTEXT;

//
// Locate I2C_DRIVER_CONTEXT from protocol
//
#define I2C_PORT_SIGNATURE      0x43324953 // "SI2C"
#define I2C_DRIVER_CONTEXT_FROM_HOST_PROTOCOL(a)  CR (a, I2C_DRIVER_CONTEXT, HostApi, I2C_PORT_SIGNATURE)

#define I2C_HOST_POLLING_PERIOD 500 //microseconds

GLOBAL_REMOVE_IF_UNREFERENCED CONST EFI_UNICODE_STRING_TABLE mControllerNameStringTable[] = {
  { "eng", L"SerialIo I2C X" }, //X - to be substituted with controller number
  { NULL , NULL }
};
GLOBAL_REMOVE_IF_UNREFERENCED CONST EFI_UNICODE_STRING_TABLE mDriverNameStringTable[] = {
  { "eng", L"SerialIo I2C" },
  { NULL , NULL }
};

/**
  Checks if Device with given PciDeviceId is one of Lpss I2C controllers
  If yes, its number is returned through I2cIndex parameter, otherwise I2cIndex is not updated

  @param[in]  PciDevId                  Device ID
  @param[out] I2cNumber                 Number of Lpss I2C controller

  @retval TRUE                          yes it is a Lpss I2C controller
  @retval FALSE                         no it isn't a Lpss I2C controller
**/
BOOLEAN
IsLpssI2cDeviceId (
  IN  UINT16    PciDevId,
  OUT UINT8     *I2cNumber
  )
{
  UINT8 Index;

  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    if (PciDevId == GetLpssI2cDeviceId (Index)) {
      *I2cNumber = Index;
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Get PCI Device ID from device pointed to by handle.
  If valid DeviceID can't be retrieved, 0xFFFF is returned

  @param[in] Handle   device handle

  @retval             PCI Device ID
**/
UINT16
STATIC
GetPciDevId (
  EFI_HANDLE Handle
  )
{
  EFI_STATUS          Status;
  EFI_PCI_IO_PROTOCOL *PciIo;
  UINT32              VenDevId;

  Status = gBS->OpenProtocol (
                  Handle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  Handle,
                  Handle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return 0xFFFF;
  }
  Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint32, 0, 1, &VenDevId);
  gBS->CloseProtocol (
         Handle,
         &gEfiPciIoProtocolGuid,
         Handle,
         Handle
         );
  if (EFI_ERROR (Status)) {
    return 0xFFFF;
  }
  return (VenDevId >> 16);
}

/**
  PerformTransfer. For synchronous transfer this function is called in a loop
  and for asynchronous transfers, as a timer callback. It writes data and/or
  read requests to hadrware, copies read data to destination buffers. When
  transfer completes, it cleans up Sw context and Hw registers in preparation
  for new transfer

  @param[in] Event     obligatory parameter for callback functions, not used here
  @param[in] Context   driver context
**/
VOID
EFIAPI
AsyncTransfer (
  IN EFI_EVENT          Event,
  IN VOID               *Context
  )
{
  I2C_DRIVER_CONTEXT    *DriverContext;

  DriverContext = (I2C_DRIVER_CONTEXT *) Context;
  PerformTransfer (&DriverContext->Host);
  if (!DriverContext->Host.TransferInProgress) {
    gBS->SetTimer (DriverContext->Timer, TimerCancel, 0);
    //set return values
    if (DriverContext->RequestStatus != NULL) {
      *(DriverContext->RequestStatus) = DriverContext->Host.TransferStatus;
    }
    if (DriverContext->RequestEvent != NULL) {
      gBS->SignalEvent (DriverContext->RequestEvent);
    }
  }
}

/**
  Set the I2C controller bus clock frequency.

  This routine must be called at or below TPL_NOTIFY.

  The software and controller do a best case effort of using the specified
  frequency for the I2C bus.  If the frequency does not match exactly then
  the controller will use lower frequency for the I2C to avoid exceeding
  the operating conditions for any of the I2C devices on the bus.
  For example if 400 KHz was specified and the controller's divide network
  only supports 402 KHz or 398 KHz then the controller would be set to 398
  KHz.

  @param[in] This           Address of an EFI_I2C_MASTER_PROTOCOL
                            structure
  @param[in] BusClockHertz  New I2C bus clock frequency in Hertz

  @retval EFI_SUCCESS       The bus frequency was set successfully.
  @retval EFI_UNSUPPORTED   The controller does not support this frequency.
**/

EFI_STATUS
EFIAPI
LpssI2cBusFrequencySet (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This,
  IN OUT UINTN                     *BusClockHertz
  )
{
  I2C_DRIVER_CONTEXT *Context;
  UINTN              Frequency = 0x00;

  Context = I2C_DRIVER_CONTEXT_FROM_HOST_PROTOCOL ( This );

  // Frequency in KHz

  Frequency = *BusClockHertz / 1000;
  if (Frequency != 0) {
    SerialIoI2cSetFrequency (Context->Host.MmioAddress, &Frequency, NULL);
    SerialIoI2cStartOperation (Context->Host.MmioAddress, &Frequency);
  }
  return EFI_SUCCESS;
}

/**
  Reset the I2C controller

  @param[in]     This       Address of an EFI_I2C_MASTER_PROTOCOL
                            structure
**/
EFI_STATUS
EFIAPI
LpssI2cReset (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This
  )
{
  I2C_DRIVER_CONTEXT *Context;

  Context = I2C_DRIVER_CONTEXT_FROM_HOST_PROTOCOL ( This );

  return I2cReset (Context->Host.MmioAddress);
}

/**
  Start an I2C operation on the controller

  This routine must be called at or below TPL_NOTIFY.  For synchronous
  requests this routine must be called at or below TPL_CALLBACK.

  The typical consumer of this API is the I2C host driver.
  Extreme care must be taken by other consumers of this API to
  prevent confusing the third party I2C drivers due to a state
  change at the I2C device which the third party I2C drivers did
  not initiate.  I2C platform drivers may use this API within
  these guidelines.

  This function initiates an I2C operation on the controller.

  The operation is performed by selecting the I2C device with its target
  address and then sending all data to the I2C device. If Event parameter is NULL,
  it means a synchronous transfer. In that case, function will return only after the
  transfer has finished. Otherwise, it's an asynchronous transfer. In that case
  function will start the transfer, set a timer event and return. Timer event will
  wake it up later so that it can continue or finish the transfer.

  @param[in] This               Address of an EFI_I2C_MASTER_PROTOCOL
                                structure
  @param[in] TargetAddress      Address of the device on the I2C bus.
  @param[in] Event              Event to set for asynchronous operations,
                                NULL for synchronous operations
  @param[in] RequestPacket      Address of an EFI_I2C_REQUEST_PACKET
                                structure describing the I2C operation
  @param[out] RequestStatus     Optional buffer to receive the I2C operation
                                completion status

  @retval EFI_SUCCESS           The requested synchronous operation completed successfully or
                                requested asynchronous operation was started.
  @retval EFI_ALREADY_STARTED   The controller is busy with another transfer
  @retval EFI_BAD_BUFFER_SIZE   Transfer size too big
  @retval EFI_DEVICE_ERROR      There was an I2C error (NACK) during the operation.
  @retval EFI_INVALID_PARAMETER RequestPacket is NULL, invalid Operation flags
  @retval EFI_NOT_FOUND         TargetAddress exceeds maximum address
  @retval EFI_NO_RESPONSE       The I2C device is not responding to the target address.
  @retval EFI_UNSUPPORTED       10bit i2c address or "ping" operation attempted (0-byte transfer, address byte not followed by any data)

**/
EFI_STATUS
EFIAPI
LpssI2cStartRequest (
  IN CONST EFI_I2C_MASTER_PROTOCOL  *This,
  IN UINTN                          TargetAddress,
  IN EFI_I2C_REQUEST_PACKET         *RequestPacket,
  IN EFI_EVENT                      Event          OPTIONAL,
  OUT EFI_STATUS                    *RequestStatus OPTIONAL
  )
{
  I2C_DRIVER_CONTEXT  *Context;
  EFI_STATUS          Status;

  Context = I2C_DRIVER_CONTEXT_FROM_HOST_PROTOCOL (This);

  Status = ValidateRequest (&Context->Host, RequestPacket);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "I2cDriver: ValidateRequest failed, %r\n", Status));
    return Status;
  }

  Context->RequestStatus = RequestStatus;
  Context->RequestEvent  = Event;

  Status = InitializeTransfer (&Context->Host, TargetAddress, RequestPacket);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Event == NULL) {
    gBS->SetTimer (Context->Timer, TimerCancel, 0); // make sure no other timers are set
    while (Context->Host.TransferInProgress) {
      AsyncTransfer (Context->Timer, Context);
    }
    return Context->Host.TransferStatus;
  } else {
    gBS->SetTimer (Context->Timer, TimerPeriodic, EFI_TIMER_PERIOD_MICROSECONDS (10));
    return EFI_SUCCESS;
  }
}

/**
  Verify the controller type

  This routine determines if an I2C controller is available.

  This routine is called by the UEFI driver framework during connect
  processing.

  @param [in] DriverBinding        Protocol instance pointer.
  @param [in] Controller           Handle of device to test.
  @param [in] RemainingDevicePath  Not used.

  @retval EFI_SUCCESS              This driver supports this device.
  @retval other                    This driver does not support this device.

**/
EFI_STATUS
EFIAPI
LpssI2cDriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL   *This,
  IN EFI_HANDLE                    Controller,
  IN EFI_DEVICE_PATH_PROTOCOL      *RemainingDevicePath
  )
{
  EFI_STATUS  Status;
  UINT8       Number;
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiI2cMasterProtocolGuid,
                  NULL,
                  NULL,
                  NULL,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    Status = EFI_ALREADY_STARTED;
    return Status;
  }
  if (IsLpssI2cDeviceId (GetPciDevId (Controller), &Number)) {
    return EFI_SUCCESS;
  }
  return EFI_UNSUPPORTED;
}

/**
  Connect to the I2C controller

  This routine initializes an instance of the I2C driver for this
  controller.

  This routine is called by the UEFI driver framework during connect
  processing if the controller passes the tests in SerialIoI2cDriverSupported.

  @param [in] DriverBinding        Protocol instance pointer.
  @param [in] Controller           Handle of device to work with.
  @param [in] RemainingDevicePath  Not used, always produce all possible children.

  @retval EFI_SUCCESS              This driver is added to Controller.
  @retval other                    This driver does not support this device.

**/
EFI_STATUS
EFIAPI
LpssI2cDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL *DriverBinding,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  )
{
  I2C_DRIVER_CONTEXT       *Context;
  EFI_STATUS               Status;
  UINT8                    ControllerNumber;
  UINT64                   PciCfgBase;

  if (!IsLpssI2cDeviceId (GetPciDevId (Controller), &ControllerNumber)) {
    return EFI_UNSUPPORTED;
  }
  if (ControllerNumber > GetMaxI2cInterfacesNum ()) {
    return EFI_UNSUPPORTED;
  }

  Context = AllocateZeroPool (sizeof (I2C_DRIVER_CONTEXT));
  if (NULL == Context) {
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "%a on I2C%d\n",__FUNCTION__, ControllerNumber));

  PciCfgBase  = LpssI2cPciCfgBase (ControllerNumber);

  Context->Host.ConfigAddress      = PciCfgBase;
  Context->Host.MmioAddress        = GetSerialIoBar (PciCfgBase);
  Context->Host.TransferInProgress = FALSE;

  Status = PrepareController (&Context->Host);
  if (EFI_ERROR (Status)) {
    FreePool (Context);
    return Status;
  }

  Context->Signature = I2C_PORT_SIGNATURE;

  Context->HostApi.SetBusFrequency                = LpssI2cBusFrequencySet;
  Context->HostApi.Reset                          = LpssI2cReset;
  Context->HostApi.StartRequest                   = LpssI2cStartRequest;
  Context->HostApi.I2cControllerCapabilities      = &(Context->Host.Capabilities);
  Context->Host.Capabilities.StructureSizeInBytes = sizeof (EFI_I2C_CONTROLLER_CAPABILITIES);
  Context->Host.Capabilities.MaximumReceiveBytes  = (UINT32) -1;
  Context->Host.Capabilities.MaximumTransmitBytes = (UINT32) -1;
  Context->Host.Capabilities.MaximumTotalBytes    = (UINT32) -1;

  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  AsyncTransfer,
                  (VOID*) Context,
                  &Context->Timer
                  );
  if (EFI_ERROR (Status)) {
    FreePool (Context);
    return Status;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Controller,
                  &gEfiI2cMasterProtocolGuid,
                  &Context->HostApi,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    FreePool (Context);
    return Status;
  }

  return EFI_SUCCESS;
}


/**
  Disconnect from the I2C port controller.

  This routine disconnects from the I2C controller.

  This routine is called by DriverUnload when the I2C port driver
  is being unloaded.

  @param [in] DriverBinding        Protocol instance pointer.
  @param [in] Controller           Handle of device to stop driver on.
  @param [in] NumberOfChildren     How many children need to be stopped.
  @param [in] ChildHandleBuffer    Not used.

  @retval EFI_SUCCESS              This driver is removed Controller.
  @retval EFI_DEVICE_ERROR         The device could not be stopped due to a device error.
  @retval other                    This driver was not removed from this device.

**/
EFI_STATUS
EFIAPI
LpssI2cDriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *DriverBinding,
  IN  EFI_HANDLE                   Controller,
  IN  UINTN                        NumberOfChildren,
  IN  EFI_HANDLE                   *ChildHandleBuffer
  )
{
  I2C_DRIVER_CONTEXT                 *I2cDriver;
  EFI_I2C_MASTER_PROTOCOL            *I2cMasterProtocol;
  EFI_PCI_IO_PROTOCOL                *I2cPciIo;
  EFI_STATUS                          Status;
  SERIAL_IO_PWRMG_CONTROL_AND_STATUS  PciPwrst;

  PciPwrst.Data32          = 0x0;
  //
  // Turn off I2c Controller
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &I2cPciIo,
                  DriverBinding->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    Status = I2cPciIo->Pci.Read (I2cPciIo, EfiPciIoWidthUint32, R_SERIAL_IO_CFG_PMECTRLSTATUS, 1, &(PciPwrst.Data32));
    if (!EFI_ERROR(Status)) {
      PciPwrst.Fields.PwrState = 0x3; // D3 State
      I2cPciIo->Pci.Write (I2cPciIo, EfiPciIoWidthUint32, R_SERIAL_IO_CFG_PMECTRLSTATUS, 1, &(PciPwrst.Data32));
      gBS->CloseProtocol (
             Controller,
             &gEfiPciIoProtocolGuid,
             DriverBinding->DriverBindingHandle,
             Controller
             );
    }
  }

  //
  //  Disconnect any connected drivers and locate the context
  //  structure
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiI2cMasterProtocolGuid,
                  (VOID**)&I2cMasterProtocol,
                  DriverBinding->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER | EFI_OPEN_PROTOCOL_EXCLUSIVE
                  );
  if ( !EFI_ERROR (Status)) {
    I2cDriver = I2C_DRIVER_CONTEXT_FROM_HOST_PROTOCOL (I2cMasterProtocol);

    //
    //  Done with the i2C port protocol
    //
    Status = gBS->CloseProtocol (
                    Controller,
                    &gEfiI2cMasterProtocolGuid,
                    DriverBinding->DriverBindingHandle,
                    Controller
                    );
    if (!EFI_ERROR (Status)) {
      //
      //  Remove the I2C port protocol
      //
      Status = gBS->UninstallMultipleProtocolInterfaces (
                      Controller,
                      &gEfiI2cMasterProtocolGuid,
                      I2cMasterProtocol,
                      NULL
                      );
      if (!EFI_ERROR (Status)) {
        FreePool (I2cDriver);
      } else {
        DEBUG ((DEBUG_ERROR, "ERROR - Failed to uninstall I2C master protocol, Status: %r\n", Status));
      }
    } else {
      DEBUG ((DEBUG_ERROR, "ERROR - Failed to close I2C master protocol, Status: %r\n", Status));
    }
  }

  return Status;
}


/**
  Driver binding protocol support
**/
EFI_DRIVER_BINDING_PROTOCOL gSerialIoI2cDriverBinding = {
  LpssI2cDriverSupported,
  LpssI2cDriverStart,
  LpssI2cDriverStop,
  0x10,
  NULL,
  NULL
};

/**
  This is DXE driver binding driver of I2C master protocol to comply the I2C driver stack of PI spec 1.3.
  The I2C host DXE driver installs a driver binding protocol and produces EFI_I2C_MASTER_PROTOCOL.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
**/
EFI_STATUS
EFIAPI
LpssI2cDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;

  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gSerialIoI2cDriverBinding,
             ImageHandle,
             NULL,
             NULL
             );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

