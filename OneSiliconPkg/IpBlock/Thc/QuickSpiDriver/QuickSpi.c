/** @file
  Touch Host Controller QuickSpi Driver

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

#include "QuickSpiPrivate.h"
#include "QuickSpiAbsPtr.h"
#include "QuickSpiHid.h"
#include "QuickSpiDriver.h"
#include <Uefi/UefiSpec.h>
#include <Library/HobLib.h>
#include <ThcConfigHob.h>

//
// Driver Support EFI Version Protocol instance
//
GLOBAL_REMOVE_IF_UNREFERENCED
EFI_DRIVER_SUPPORTED_EFI_VERSION_PROTOCOL gQuickSpiDriverSupportedEfiVersion = {
  sizeof (EFI_DRIVER_SUPPORTED_EFI_VERSION_PROTOCOL),
  EFI_2_40_SYSTEM_TABLE_REVISION
};

//
// NOTE: The module variables of policy here are only valid in post time, but not runtime
//

GLOBAL_REMOVE_IF_UNREFERENCED  UINT8    mThcMode[2] = {0xFF, 0xFF};

GLOBAL_REMOVE_IF_UNREFERENCED  THC_RESET mThcReset[2];

GLOBAL_REMOVE_IF_UNREFERENCED  THC_HID_OVER_SPI  mHidOverSpi[2];

typedef struct {
  UINT16 DeviceId1;
  UINT16 DeviceId2;
} QUICKSPI_DEVICEIDS;

#define V_INTEL_VENDOR_ID   0x8086
QUICKSPI_DEVICEIDS SupportedDeviceIds[] = {
  {0x67C9, 0x67CB},
  {0x7E49, 0x7E4B},
  {0x7749, 0x774B},
  {0x7F59, 0x7F5B},
  {0xA849, 0xA84B},
  {0xE349, 0xE34B},
  {0xE449, 0xE44B},
  {0x4D49, 0x4D4B},
};

/**
  Function to check for support by this THC driver

  @param[in]  VendorId        Vendor Id that will be compared to THCs VendorId
  @param[in]  DeviceId        Device Id that will be compared to THCs DeviceId
  @param[in]  Index           Device Index

  @retval  TRUE    Device ID belongs to THC
           FALSE   Device ID is not supported

**/
BOOLEAN
STATIC
QuickSpiIsDeviceIdSupported (
  IN  UINT16  VendorId,
  IN  UINT16  DeviceId,
  OUT UINT8   *Index
  )
{
  UINT8 IdIndex;
  *Index = 0;

  if (VendorId != V_INTEL_VENDOR_ID) {
    return FALSE;
  }

  for (IdIndex = 0; IdIndex < ARRAY_SIZE (SupportedDeviceIds); IdIndex++) {
    if (SupportedDeviceIds[IdIndex].DeviceId1 == DeviceId) {
      *Index = 0;
      return TRUE;
    } else if (SupportedDeviceIds[IdIndex].DeviceId2 == DeviceId) {
      *Index = 1;
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Function to unload QuickSpi Driver when gBS->ExitBootServices() called.

  @param[in]  Event             Pointer to this event
  @param[in]  Context           Event handler private data

**/
VOID
EFIAPI
QuickSpiExitEvent (
  IN EFI_EVENT     Event,
  IN VOID          *Context
  )
{
  QUICK_SPI_DEV     *QuickSpiDev;

  QuickSpiDev = Context;

  DEBUG ((DEBUG_INFO, "QuickSpiExitEvent Start \n"));

  //
  // Check if CleanUp is required
  //
  if (QuickSpiDev->Signature != QUICK_SPI_SIGNATURE) {
    DEBUG ((DEBUG_INFO, "QuickSpiExitEvent QuickSpi Structure already freed. End \n"));
    return;
  }

  QuickSpiStop (QuickSpiDev, &mHidOverSpi[QuickSpiDev->InstanceId], &mThcReset[QuickSpiDev->InstanceId]);

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  DEBUG ((DEBUG_INFO, "QuickSpiExitEvent End \n"));
}

/**
  Creates ExitEvent responsible for ExitBootServices clean up

  @param[in]  QuickSpiDev       Context of QuickSpi device

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
QuickSpiCreateExitEvents (
  IN QUICK_SPI_DEV *QuickSpiDev
  )
{
  EFI_STATUS Status;

  ///
  /// Create ExitBootServices for calling the QuickSpiDriverBindingStop procedure
  ///
  DEBUG ((DEBUG_INFO, "QuickSpiCreateExitEvents Create ExitBootServices Event\n"));
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  QuickSpiExitEvent,
                  QuickSpiDev,
                  &gEfiEventExitBootServicesGuid,
                  &QuickSpiDev->ExitEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Unloads an image.

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
  @retval EFI_INVALID_PARAMETER ImageHandle is not a valid image handle.

**/
EFI_STATUS
EFIAPI
QuickSpiUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  *HandleBuffer;
  UINTN       HandleCount;
  UINTN       Index;

  //
  // Retrieve array of all handles in the handle database
  //
  Status = gBS->LocateHandleBuffer (
                  AllHandles,
                  NULL,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Disconnect the current driver from handles in the handle database
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->DisconnectController (HandleBuffer[Index], ImageHandle, NULL);
    if (EFI_ERROR (Status)) {
      FreePool (HandleBuffer);
      return Status;
    }
  }

  //
  // Free the array of handles
  //
  FreePool (HandleBuffer);

  //
  // Uninstall the driver version protocol
  //
  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ImageHandle,
                  &gEfiDriverSupportedEfiVersionProtocolGuid,
                  &gQuickSpiDriverSupportedEfiVersion,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Uninstall protocols installed in the driver entry point
  //
  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ImageHandle,
                  &gEfiDriverBindingProtocolGuid,  &gQuickSpiDriverBinding,
                  &gEfiComponentNameProtocolGuid,  &gQuickSpiDriverComponentName,
                  &gEfiComponentName2ProtocolGuid, &gQuickSpiDriverComponentName2,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  return EFI_SUCCESS;
}

/**
  Starts Circular Buffer state Pointer polling on QuickSpi

  @param[in]  QuickSpiDev      Context of QuickSpi device

  @retval EFI_SUCCESS     Polling started
  @retval other           Error during initialization
**/
EFI_STATUS
QuickSpiStartPolling (
  IN QUICK_SPI_DEV        *QuickSpiDev
  )
{
  EFI_STATUS    Status;

  //
  // Periodic events run with NOTIFY priority to guarantee driver works fine when called from code that runs with CALLBACK priority
  //
  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  QuickSpiPolling,
                  (VOID*) QuickSpiDev,
                  &QuickSpiDev->PollingEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiStartPolling error, cannot create PollingEvent Status %r\n", Status));
    return Status;
  }

  Status = gBS->SetTimer (
                  QuickSpiDev->PollingEvent,
                  TimerPeriodic,
                  EFI_TIMER_PERIOD_MILLISECONDS (THC_POLLING_PERIOD)
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiStartPolling error, cannot enable PollingEvent timer, %r\n", Status));
    return Status;
  }

  return Status;
}

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
QuickSpiEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS               Status;
  VOID                     *HobPtr;
  THC_CONFIG_HOB           *ThcConfigHob;

  //
  // Install UEFI Driver Model protocol(s).
  //
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gQuickSpiDriverBinding,
             ImageHandle,
             &gQuickSpiDriverComponentName,
             &gQuickSpiDriverComponentName2
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Install Driver Supported EFI Version Protocol onto ImageHandle
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiDriverSupportedEfiVersionProtocolGuid, &gQuickSpiDriverSupportedEfiVersion,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Get THC HOB
  //
  HobPtr = GetFirstGuidHob (&gPchThcConfigHobGuid);
  if (HobPtr == NULL) {
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }
  ThcConfigHob = (THC_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr);

  mThcMode[0] = ThcConfigHob->ThcPort[0].Mode;
  mThcMode[1] = ThcConfigHob->ThcPort[1].Mode;

  CopyMem (&mHidOverSpi[0], &ThcConfigHob->ThcPort[0].HidOverSpi, sizeof (THC_HID_OVER_SPI));
  CopyMem (&mHidOverSpi[1], &ThcConfigHob->ThcPort[1].HidOverSpi, sizeof (THC_HID_OVER_SPI));

  CopyMem (&mThcReset[0], &ThcConfigHob->ThcPort[0].Reset, sizeof (THC_RESET));
  CopyMem (&mThcReset[1], &ThcConfigHob->ThcPort[1].Reset, sizeof (THC_RESET));

  return Status;
}

/**
  Tests to see if this driver supports a given controller. If a child device is provided,
  it further tests to see if this driver supports creating a handle for the specified child device.

  This function checks to see if the driver specified by This supports the device specified by
  ControllerHandle. Drivers will typically use the device path attached to
  ControllerHandle and/or the services from the bus I/O abstraction attached to
  ControllerHandle to determine if the driver supports ControllerHandle. This function
  may be called many times during platform initialization. In order to reduce boot times, the tests
  performed by this function must be very small, and take as little time as possible to execute. This
  function must not change the state of any hardware devices, and this function must be aware that the
  device specified by ControllerHandle may already be managed by the same driver or a
  different driver. This function must match its calls to AllocatePages() with FreePages(),
  AllocatePool() with FreePool(), and OpenProtocol() with CloseProtocol().
  Because ControllerHandle may have been previously started by the same driver, if a protocol is
  already in the opened state, then it must not be closed with CloseProtocol(). This is required
  to guarantee the state of ControllerHandle is not modified by this function.

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
QuickSpiDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  THC_PROTOCOL             *ThcProtocol;
  EFI_STATUS               Status;
  EFI_PCI_IO_PROTOCOL      *PciIo;
  UINT16                   DeviceId;
  UINT16                   VendorId;
  UINT8                    DeviceIndex;

  //
  // Verify that this driver is not already running on this device
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gThcProtocolGuid,
                  (VOID **) &ThcProtocol,
                  NULL,
                  NULL,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Status = PciIo->Pci.Read (
               PciIo,
               EfiPciIoWidthUint16,
               PCI_VENDOR_ID_OFFSET,
               1,
               &VendorId
               );

  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Status = PciIo->Pci.Read (
               PciIo,
               EfiPciIoWidthUint16,
               PCI_DEVICE_ID_OFFSET,
               1,
               &DeviceId
               );

  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Close the PCI I/O Protocol that was opened with PROTOCOL_BY_DRIVER
  //
  Status = gBS->CloseProtocol (ControllerHandle, &gEfiPciIoProtocolGuid, This->DriverBindingHandle, ControllerHandle);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  if (QuickSpiIsDeviceIdSupported (VendorId, DeviceId, &DeviceIndex)) {
    if (mThcMode[DeviceIndex] == HidOverSpi) {
      THC_LOCAL_DEBUG(L"QuickSpiDriverBindingSupported DeviceId: 0x%X DeviceIndex %d\n", DeviceId, DeviceIndex)
      DEBUG ((DEBUG_INFO, "QuickSpiDriverBindingSupported DeviceId: 0x%X DeviceIndex %d\n", DeviceId, DeviceIndex));
      return EFI_SUCCESS;
    }
  }

  return EFI_UNSUPPORTED;
}


/**
  Starts a device controller or a bus controller.

  The Start() function is designed to be invoked from the EFI boot service ConnectController().
  As a result, much of the error checking on the parameters to Start() has been moved into this
  common boot service. It is legal to call Start() from other locations,
  but the following calling restrictions must be followed, or the system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE.
  2. If RemainingDevicePath is not NULL, then it must be a pointer to a naturally aligned
     EFI_DEVICE_PATH_PROTOCOL.
  3. Prior to calling Start(), the Supported() function for the driver specified by This must
     have been called with the same calling parameters, and Supported() must have returned EFI_SUCCESS.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to start. This handle
                                   must support a protocol interface that supplies
                                   an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a device path.  This
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

  @retval EFI_SUCCESS              The device was started.
  @retval EFI_DEVICE_ERROR         The device could not be started due to a device error. Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a lack of resources.
  @retval Others                   The driver failed to start the device.

**/
EFI_STATUS
EFIAPI
QuickSpiDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  QUICK_SPI_DEV      *QuickSpiDev;
  EFI_STATUS         Status;
  UINT64             Supports;
  UINT64             OriginalPciAttributes;
  UINT16             VendorId;
  UINT16             DeviceId;

  OriginalPciAttributes  = 0;

  DEBUG ((DEBUG_INFO, "QuickSpiDriverBindingStart Start()\n"));
  THC_LOCAL_DEBUG(L"QuickSpiDriverBindingStart Start ()\n")
  //
  // Allocate the driver context
  //
  QuickSpiDev = AllocateZeroPool (sizeof (QUICK_SPI_DEV));
  if (QuickSpiDev == NULL) {
    THC_LOCAL_DEBUG(L"QuickSpiDriverBindingStart Failed to allocate driver context EFI_OUT_OF_RESOURCES \n")
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Save PciIo Protocol instance for DMA usage
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &QuickSpiDev->PciIo,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG(L"QuickSpiDriverBindingStart Failed to OpenProtocol PciIoProtocol\n")
    FreePool (QuickSpiDev);
    return Status;
  }

  //
  // Get BAR0
  //
  Status = QuickSpiDev->PciIo->Pci.Read (QuickSpiDev->PciIo, EfiPciIoWidthUint64, PCI_BASE_ADDRESSREG_OFFSET, 1, &QuickSpiDev->PciBar0);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG(L"QuickSpiDriverBindingStart ERROR - QuickSpi failed to read BAR0 Status: %r\n", Status);
    DEBUG ((DEBUG_ERROR, "ERROR - QuickSpi failed to BAR0 Status: %r\n", Status));
    FreePool (QuickSpiDev);
    return Status;
  }
  QuickSpiDev->PciBar0 = QuickSpiDev->PciBar0 & 0xFFFFFFFFFFFFFFF0;
  THC_LOCAL_DEBUG(L"QuickSpiDriverBindingStart BAR0 value: 0x%X\n", QuickSpiDev->PciBar0)

  //
  // Set D0
  //
  QuickSpiDev->PciPwrst = (UINT8)~(V_THC_CFG_PMD_PMCSRBSE_PMCSR_PWRST_D3);
  Status = QuickSpiDev->PciIo->Pci.Write (QuickSpiDev->PciIo, EfiPciIoWidthUint8, R_THC_CFG_PMD_PMCSRBSE_PMCSR, 1, &QuickSpiDev->PciPwrst);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG(L"QuickSpiDriverBindingStart ERROR - QuickSpi failed to set D0 state Status: %r\n", Status)
    DEBUG ((DEBUG_ERROR, "ERROR - QuickSpi failed to set D0 Status: %r\n", Status));
    FreePool (QuickSpiDev);
    return Status;
  }
  THC_LOCAL_DEBUG(L"QuickSpiDriverBindingStart QuickSpi D0 set\n");

  //
  // Get Instance Id
  //
  QuickSpiDev->PciIo->Pci.Read (QuickSpiDev->PciIo, EfiPciIoWidthUint16, PCI_VENDOR_ID_OFFSET, 1, &VendorId);
  QuickSpiDev->PciIo->Pci.Read (QuickSpiDev->PciIo, EfiPciIoWidthUint16, PCI_DEVICE_ID_OFFSET, 1, &DeviceId);
  QuickSpiIsDeviceIdSupported (VendorId, DeviceId, &QuickSpiDev->InstanceId);

  //
  // Save Controller's Pci Configuration state
  //
  Status = QuickSpiDev->PciIo->Attributes (QuickSpiDev->PciIo, EfiPciIoAttributeOperationGet, 0, &OriginalPciAttributes);
  if (EFI_ERROR (Status)) {
    FreePool (QuickSpiDev);
    return Status;
  }
  QuickSpiDev->PciAttributes = OriginalPciAttributes;

  //
  // Enable Controller
  //
  Status = QuickSpiDev->PciIo->Attributes (QuickSpiDev->PciIo, EfiPciIoAttributeOperationSupported, 0, &Supports);
  if (!EFI_ERROR (Status)) {
    Supports &= (UINT64) EFI_PCI_DEVICE_ENABLE;
    Status = QuickSpiDev->PciIo->Attributes (QuickSpiDev->PciIo, EfiPciIoAttributeOperationEnable, Supports, NULL);
  }
  if (EFI_ERROR (Status)) {
    FreePool (QuickSpiDev);
    return Status;
  }

  //
  // Initialize the driver context
  //
  QuickSpiDev->Signature                = QUICK_SPI_SIGNATURE;
  QuickSpiDev->ControllerHandle         = ControllerHandle;
  QuickSpiDev->Initialized              = FALSE;

  QuickSpiDev->ThcProtocol.Revision      = THC_PROTOCOL_REVISION;
  QuickSpiDev->ThcProtocol.DeviceStatus  = EFI_NOT_READY;

  //
  // HID
  //
  QuickSpiDev->ThcProtocol.HidGetFeature          = HidGetFeature;
  QuickSpiDev->ThcProtocol.HidSetFeature          = HidSetFeature;
  QuickSpiDev->ThcProtocol.HidEnableAfterReset    = HidEnableAfterReset;
  QuickSpiDev->ThcProtocol.HidGetInputReport      = HidGetInputReport;
  QuickSpiDev->ThcProtocol.HidSetOutputReport     = HidSetOutputReport;
  QuickSpiDev->ThcProtocol.HidGetDeviceDescriptor = HidGetDeviceDescriptor;
  QuickSpiDev->ThcProtocol.HidPerformReset        = HidPerformReset;
  QuickSpiDev->ThcProtocol.HidSetPowerState       = HidSetPowerState;
  QuickSpiDev->ThcProtocol.HidGetReportDescriptor = HidGetReportDescriptor;
  
  QuickSpiDev->HidDataAvailable          = FALSE;
  QuickSpiDev->HidActive                 = FALSE;
  QuickSpiDev->HidBuffer                 = NULL;

  QuickSpiDev->AbsPtrDataAvailable       = FALSE;
  QuickSpiDev->ReadDone                  = FALSE;

  QuickSpiDev->InputReportTable.Quantity = 0;
  QuickSpiDev->InputReportTable.Report   = NULL;

  AddUnicodeString2 ("eng", gQuickSpiDriverComponentName.SupportedLanguages,  &QuickSpiDev->ControllerNameTable, L"Intel Touch Host Controller QuickSpi Driver", TRUE);
  AddUnicodeString2 ("en",  gQuickSpiDriverComponentName2.SupportedLanguages, &QuickSpiDev->ControllerNameTable, L"Intel Touch Host Controller QuickSpi Driver", FALSE);

  //
  // Install only QuickSpi Protocol to avoid Binding Start getting invoked for second time.
  // Other protocols will be installed after QuickSpi successful Initialization
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ControllerHandle,
                  &gThcProtocolGuid,
                  &QuickSpiDev->ThcProtocol,
                  NULL,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - QuickSpi failed to install ThcProtocol Status: %r\n", Status));
    FreePool (QuickSpiDev);
    return Status;
  }

  Status = QuickSpiCreateExitEvents (QuickSpiDev);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - QuickSpi failed to create Exit Events Status: %r\n", Status));
    FreePool (QuickSpiDev);
    return Status;
  }

  Status = QuickSpiInitialize (QuickSpiDev, &mHidOverSpi[QuickSpiDev->InstanceId], &mThcReset[QuickSpiDev->InstanceId]);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - QuickSpi failed to Initialize Device Status: %r\n", Status));
    goto THC_ERROR_EXIT;
  }

  Status = QuickSpiReadDeviceDescriptor (QuickSpiDev);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - QuickSpi failed to Read Device Descriptor Status: %r\n", Status));
    goto THC_ERROR_EXIT;
  }

  QuickSpiPrintDeviceDescriptor (QuickSpiDev);

  Status = QuickSpiDmaInitialize (QuickSpiDev);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - QuickSpi failed to Initialize DMA Status: %r\n", Status));
    goto THC_ERROR_EXIT;
  }

  QuickSpiDev->ReadDone = TRUE;

  Status = QuickSpiStartPolling (QuickSpiDev);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - QuickSpi failed to StartPolling Status: %r\n", Status));
    goto THC_ERROR_EXIT;
  }

  Status = QuickSpiReadHidDescriptor (QuickSpiDev);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiDriverBindingStart failed to Read the HID descriptor Status: %r\n", Status));
    goto THC_ERROR_EXIT;
  }

  if (QuickSpiDev->HidSolutionFlag != ElanHidProtocol) {
    Status = QuickSpiTouchEnable (QuickSpiDev);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "QuickSpiDriverBindingStart failed to Enable Touch Status: %r\n", Status));
    }
  }

  Status = QuickSpiInstallAbsPtr (QuickSpiDev);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - QuickSpi failed to Install AbsPtr Protocol Status: %r\n", Status));
    goto THC_ERROR_EXIT;
  }

  Status = gBS->ConnectController (QuickSpiDev->ControllerHandle, NULL, NULL, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - QuickSpi failed to ConnectController Status: %r\n", Status));
    goto THC_ERROR_EXIT;
  }

  QuickSpiDev->Initialized = TRUE;
  QuickSpiDev->ThcProtocol.DeviceStatus = EFI_SUCCESS;

  THC_LOCAL_DEBUG(L"QuickSpiDriverBindingStart End()\n")
  DEBUG ((DEBUG_INFO, "QuickSpiDriverBindingStart End()\n"));
  if (!EFI_ERROR (Status)) {
    return Status;
  }

THC_ERROR_EXIT:
  //
  // QuickSpi Driver always returns EFI_SUCCESS from this point even if initialization fails
  // Driver Context, Events, Memory are cleared in Exit Boot Services Event
  // True device status is available on THC Protocol
  //
  QuickSpiDev->ThcProtocol.DeviceStatus = EFI_DEVICE_ERROR;
  DEBUG ((DEBUG_ERROR, "QuickSpiDriverBindingStart End() - Failed\n"));
  return EFI_SUCCESS;
}

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
QuickSpiDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer OPTIONAL
  )
{
  THC_PROTOCOL       *ThcProtocol;
  QUICK_SPI_DEV      *QuickSpiDev;
  EFI_STATUS         Status;

  DEBUG ((DEBUG_INFO, "QuickSpiDriverBindingStop Start\n"));

  //
  // Check if ThcProtocol is installed
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gThcProtocolGuid,
                  (VOID **) &ThcProtocol,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickSpiDriverBindingStop Failed to get ThcProtocol, Status: %r\n", Status));
    return EFI_UNSUPPORTED;
  }

  //
  // Get QuickSpiDevice context from Protocol
  //
  QuickSpiDev = QUICK_SPI_CONTEXT_FROM_THC_PROTOCOL (ThcProtocol);

  gBS->CloseEvent (QuickSpiDev->ExitEvent);

  QuickSpiStop (QuickSpiDev, &mHidOverSpi[QuickSpiDev->InstanceId], &mThcReset[QuickSpiDev->InstanceId]);

  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ControllerHandle,
                  &gThcProtocolGuid,
                  &QuickSpiDev->ThcProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiDriverBindingStop Failed to Uninstall ThcProtocol, Status: %r\n", Status));
  }

  DEBUG ((DEBUG_INFO, "QuickSpiDriverBindingStop End\n"));

  return Status;
}

///
/// Driver Binding Protocol instance
///
GLOBAL_REMOVE_IF_UNREFERENCED EFI_DRIVER_BINDING_PROTOCOL gQuickSpiDriverBinding = {
  QuickSpiDriverBindingSupported,
  QuickSpiDriverBindingStart,
  QuickSpiDriverBindingStop,
  TOUCH_DRIVER_VERSION,
  NULL,
  NULL
};

