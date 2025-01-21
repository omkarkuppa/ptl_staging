/** @file
  SMBUS DXE Driver

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
#include <Uefi.h>
#include <Protocol/SmbusHc.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/ComponentName.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <SmbusConfigHob.h>

#include "SmbusCommon.h"
#include "SmbusDxeSmm.h"

#define MICROSECOND 10
#define MILLISECOND (1000 * MICROSECOND)
#define ONESECOND   (1000 * MILLISECOND)

//
// Private Data Structures
//
typedef struct _SMBUS_NOTIFY_FUNCTION_LIST_NODE {
  UINT32                    Signature;
  LIST_ENTRY                Link;
  EFI_SMBUS_DEVICE_ADDRESS  DeviceAddress;
  UINTN                     Data;
  EFI_SMBUS_NOTIFY_FUNCTION NotifyFunction;
} SMBUS_NOTIFY_FUNCTION_LIST_NODE;

#define SMBUS_NOTIFY_FUNCTION_LIST_NODE_FROM_LINK(_node) \
  CR ( \
  _node, \
  SMBUS_NOTIFY_FUNCTION_LIST_NODE, \
  Link, \
  PCH_SMBUS_PRIVATE_DATA_SIGNATURE \
  )

EFI_STATUS
EFIAPI
SmbusComponentNameGetDriverName (
  IN EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN CHAR8                        *Language,
  OUT CHAR16                      **DriverName
  );

EFI_STATUS
EFIAPI
SmbusComponentNameGetControllerName (
  IN EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_HANDLE                   ChildHandle OPTIONAL,
  IN CHAR8                        *Language,
  OUT CHAR16                      **ControllerName
  );

GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME_PROTOCOL  gSmbusComponentName = {
  SmbusComponentNameGetDriverName,
  SmbusComponentNameGetControllerName,
  "eng"
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME2_PROTOCOL  gSmbusComponentName2 = {
  (EFI_COMPONENT_NAME2_GET_DRIVER_NAME)SmbusComponentNameGetDriverName,
  (EFI_COMPONENT_NAME2_GET_CONTROLLER_NAME)SmbusComponentNameGetControllerName,
  "en"
};

//
/// Driver Name Strings
///
GLOBAL_REMOVE_IF_UNREFERENCED EFI_UNICODE_STRING_TABLE  mSmbusDriverNameTable[] = {
  {
    "eng;en",
    (CHAR16 *)L"Intel SMBUS driver"
  },
  {
    NULL,
    NULL
  }
};

///
/// Controller Name Strings
///
GLOBAL_REMOVE_IF_UNREFERENCED EFI_UNICODE_STRING_TABLE  mSmbusControllerNameTable[] = {
  {
    "eng;en",
    (CHAR16 *)L"Intel SMBUS controller"
  },
  {
    NULL,
    NULL
  }
};

/**
  This is the callback function for PCI ENUMERATION COMPLETE.
**/
VOID
EFIAPI
SmbusPciEnumerationCompleteCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  SMBUS_PRIVATE_DATA_DXE  *Private;
  EFI_PCI_IO_PROTOCOL     *PciIo;
  UINT64                  EnabledPciAttributes;
  EFI_STATUS              Status;

  Private = (SMBUS_PRIVATE_DATA_DXE*) Context;
  PciIo = Private->PciIo;

  Status = PciIo->Attributes (
                  PciIo,
                  EfiPciIoAttributeOperationSupported,
                  0,
                  &EnabledPciAttributes
                  );
  if (EFI_ERROR (Status)) {
    return;
  } else {
    EnabledPciAttributes &= (UINT64)EFI_PCI_DEVICE_ENABLE;
    Status                = PciIo->Attributes (
                                    PciIo,
                                    EfiPciIoAttributeOperationEnable,
                                    EnabledPciAttributes,
                                    NULL
                                    );
    if (EFI_ERROR (Status)) {
      return;
    }
  }
}

/**
  Set device address for an Smbus device with a known UDID or perform a general
  ARP of all devices.

  @param[in] This                 Pointer to the instance of the EFI_SMBUS_HC_PROTOCOL.
  @param[in] ArpAll               If TRUE, do a full ARP. Otherwise, just ARP the specified UDID.
  @param[in] SmbusUdid            When doing a directed ARP, ARP the device with this UDID.
  @@param[in, out] DeviceAddress   Buffer to store new SMBus Device Address during directed ARP. On output,If
                                  ArpAlll == TRUE, this will contain the newly assigned Device address.

  @retval EFI_INVALID_PARAMETER   ArpAll == FALSE but SmbusUdid or DeviceAddress are NULL.
                                  Return value from SmbusFullArp() or SmbusDirectedArp().
**/
EFI_STATUS
EFIAPI
SmbusArpDevice (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN      BOOLEAN                       ArpAll,
  IN      EFI_SMBUS_UDID                * SmbusUdid, OPTIONAL
  IN OUT  EFI_SMBUS_DEVICE_ADDRESS      * DeviceAddress OPTIONAL
  )
{
  SMBUS_PRIVATE_DATA_DXE  *Private;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Private = SMBUS_PRIVATE_DATA_FROM_THIS(This);

  if (ArpAll) {
    return SmbusFullArp (Private->PciIo, &Private->DeviceEnumData);
  } else {
    if ((SmbusUdid == NULL) || (DeviceAddress == NULL)) {
      return EFI_INVALID_PARAMETER;
    }

    return SmbusDirectedArp (Private->PciIo, &Private->DeviceEnumData, SmbusUdid, DeviceAddress);
  }
}

/**
  Get a pointer to the assigned mappings of UDID's to Device Addresses.

  @param[in] This                 Pointer to the instance of the EFI_SMBUS_HC_PROTOCOL.
  @param[in, out] Length          Buffer to contain the lenght of the Device Map, it will be updated to
                                  contain the number of pairs of UDID's mapped to Device Addresses.
  @param[in, out] SmbusDeviceMap  Buffer to contian a pointer to the Device Map, it will be updated to
                                  point to the first pair in the Device Map

  @retval EFI_SUCCESS             Function completed successfully.
**/
EFI_STATUS
EFIAPI
SmbusGetArpMap (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN OUT  UINTN                         *Length,
  IN OUT  EFI_SMBUS_DEVICE_MAP          **SmbusDeviceMap
  )
{
  SMBUS_PRIVATE_DATA_DXE  *Private;

  Private = SMBUS_PRIVATE_DATA_FROM_THIS(This);

  *Length         = Private->DeviceEnumData.DeviceMapEntries * sizeof (EFI_SMBUS_DEVICE_MAP);
  *SmbusDeviceMap = Private->DeviceEnumData.DeviceMap;

  return EFI_SUCCESS;
}

/**
  Function to be called every time periodic event happens. This will check if
  the SMBus Host Controller has received a Host Notify command. If so, it will
  see if a notification has been reqested on that event and make any callbacks
  that may be necessary.

  @param[in] Event                The periodic event that occured and got us into this callback.
  @param[in] Context              Event context. Will be NULL in this case, since we already have our
                                  private data in a module global variable.

**/
VOID
EFIAPI
PollSmbusNotify (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  LIST_ENTRY                      *Link;
  EFI_SMBUS_DEVICE_ADDRESS        DeviceAddress;
  SMBUS_NOTIFY_FUNCTION_LIST_NODE *Node;
  UINT8                           SstsReg;
  UINTN                           Data;
  SMBUS_PRIVATE_DATA_DXE          *Private;

  DEBUG ((DEBUG_VERBOSE, "%a Start\n", __FUNCTION__));

  Private = (SMBUS_PRIVATE_DATA_DXE*) Context;

  SstsReg = SmbusIoRead (Private->PciIo, R_SMBUS_IO_SSTS);
  if (!(SstsReg & B_SMBUS_IO_SSTS_HNS)) {
    ///
    /// Host Notify has not been received
    ///
    return;
  }
  ///
  /// There was a Host Notify, see if any one wants to know about it
  ///
  DeviceAddress.SmbusDeviceAddress = (SmbusIoRead (Private->PciIo, R_SMBUS_IO_NDA)) >> 1;

  Link = GetFirstNode (&Private->NotifyFunctionList);

  while (!IsNull (&Private->NotifyFunctionList, Link)) {
    Node = SMBUS_NOTIFY_FUNCTION_LIST_NODE_FROM_LINK (Link);

    if (Node->DeviceAddress.SmbusDeviceAddress == DeviceAddress.SmbusDeviceAddress) {
      Data = (SmbusIoRead (Private->PciIo, R_SMBUS_IO_NDHB) << 8) + (SmbusIoRead (Private->PciIo, R_SMBUS_IO_NDLB));
      if ((UINT16) Node->Data == (UINT16) Data) {
        ///
        /// We have a match, notify the requested function
        ///
        Node->NotifyFunction (DeviceAddress, Data);
      }
    }

    Link = GetNextNode (&Private->NotifyFunctionList, &Node->Link);
  }
  ///
  /// Clear the Notify Status bit and exit.
  ///
  SmbusIoWrite (Private->PciIo, R_SMBUS_IO_SSTS, B_SMBUS_IO_SSTS_HNS);

  DEBUG ((DEBUG_VERBOSE, "%a End\n", __FUNCTION__));

  return;
}

/**
  Set up a periodic event so that we can check if any Smbus Device has sent a
  Notify ARP Smbus Host Controller command.

  @param Private  Pointer to SMB private data

  @retval EFI_SUCCESS             Periodic event successfully set up.
  @retval Other Errors            Failed to set up Periodic event.
                                  Error value from CreateEvent().
**/
EFI_STATUS
InitializePeriodicEvent (
  IN SMBUS_PRIVATE_DATA_DXE  *Private
  )
{
  EFI_STATUS  Status;

  Status = gBS->CreateEvent (
                  (EVT_TIMER | EVT_NOTIFY_SIGNAL),
                  TPL_CALLBACK,
                  PollSmbusNotify,
                  Private,
                  &Private->NotificationEvent
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->SetTimer (
                  Private->NotificationEvent,
                  TimerPeriodic,
                  ONESECOND
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Register a callback in the event of a Host Notify command being sent by a
  specified Device on bus.

  @param[in] This                 Pointer to the instance of the EFI_SMBUS_HC_PROTOCOL.
  @param[in] DeviceAddress        Address of the device whose Host Notify command we want to
                                  trap.
  @param[in] Data                 Data of the Host Notify command we want to trap.
  @param[in] NotifyFunction       Function to be called in the event the desired Host Notify
                                  command occurs.

  @exception EFI_UNSUPPORTED      Unable to create the event needed for notifications.
  @retval EFI_INVALID_PARAMETER   NotifyFunction was NULL.
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate space to register the notification.
  @retval EFI_SUCCESS             Function completed successfully
**/
EFI_STATUS
EFIAPI
SmbusNotify (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN      EFI_SMBUS_DEVICE_ADDRESS      DeviceAddress,
  IN      UINTN                         Data,
  IN      EFI_SMBUS_NOTIFY_FUNCTION     NotifyFunction
  )
{
  EFI_STATUS                      Status;
  SMBUS_NOTIFY_FUNCTION_LIST_NODE *NewNode;
  SMBUS_PRIVATE_DATA_DXE          *Private;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Private = SMBUS_PRIVATE_DATA_FROM_THIS(This);

  if (NotifyFunction == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  NewNode = (SMBUS_NOTIFY_FUNCTION_LIST_NODE *) AllocatePool (sizeof (SMBUS_NOTIFY_FUNCTION_LIST_NODE));
  if (NewNode == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate memory for NewNode! \n"));
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // If this is the first notification request, start an event to periodically
  // check for a Notify Smbus Host Controller command.
  //
  if (!Private->NotificationEvent) {
    Status = InitializePeriodicEvent (Private);
    if (EFI_ERROR (Status)) {
      FreePool (NewNode);
      return EFI_UNSUPPORTED;
    }
  }

  NewNode->Signature                        = PCH_SMBUS_PRIVATE_DATA_SIGNATURE;
  NewNode->DeviceAddress.SmbusDeviceAddress  = DeviceAddress.SmbusDeviceAddress;
  NewNode->Data = Data;
  NewNode->NotifyFunction = NotifyFunction;

  InsertTailList (&Private->NotifyFunctionList, &NewNode->Link);

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Execute an SMBUS operation

  @param[in] This                 The protocol instance
  @param[in] DeviceAddress        The address of the SMBUS device
  @param[in] Command              The SMBUS command
  @param[in] Operation            Which SMBus protocol will be issued
  @param[in] PecCheck             If Packet Error Code Checking is to be used
  @param[in, out] Length          Length of data
  @param[in, out] Buffer          Data buffer

  @retval EFI_SUCCESS             The SMBUS operation is successful
  @retval Other Values            Something error occurred
**/
EFI_STATUS
EFIAPI
SmbusExecute (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN      EFI_SMBUS_DEVICE_ADDRESS      DeviceAddress,
  IN      EFI_SMBUS_DEVICE_COMMAND      Command,
  IN      EFI_SMBUS_OPERATION           Operation,
  IN      BOOLEAN                       PecCheck,
  IN OUT  UINTN                         *Length,
  IN OUT  VOID                          *Buffer
  )
{
  SMBUS_PRIVATE_DATA_DXE  *Private;

  DEBUG ((DEBUG_VERBOSE, "SmbusExecute() Start, SmbusDeviceAddress=%x, Command=%x, Operation=%x\n", (DeviceAddress.SmbusDeviceAddress << 1), Command, Operation));

  Private = SMBUS_PRIVATE_DATA_FROM_THIS(This);

  return SmbusDispatchCommand (
           Private->PciIo,
           DeviceAddress,
           Command,
           Operation,
           PecCheck,
           Length,
           Buffer
           );
}

/**
  Supported function of Driver Binding protocol for this driver.
  Test to see if this driver supports ControllerHandle.

  @param This                   Protocol instance pointer.
  @param Controller             Handle of device to test.
  @param RemainingDevicePath    A pointer to the device path.
                                it should be ignored by device driver.

  @retval EFI_SUCCESS           This driver supports this device.
  @retval EFI_ALREADY_STARTED   This driver is already running on this device.
  @retval other                 This driver does not support this device.

**/
EFI_STATUS
EFIAPI
SmbusControllerSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  EFI_STATUS           Status;
  EFI_PCI_IO_PROTOCOL  *PciIo;

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **)&PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return SmbusSupported (PciIo);
}

/**
  This routine is called right after the .Supported() called and
  Start this driver on ControllerHandle.

  @param This                   Protocol instance pointer.
  @param Controller             Handle of device to bind driver to.
  @param RemainingDevicePath    A pointer to the device path.
                                it should be ignored by device driver.

  @retval EFI_SUCCESS           This driver is added to this device.
  @retval EFI_ALREADY_STARTED   This driver is already running on this device.
  @retval other                 Some error occurs when binding this driver to this device.

**/
EFI_STATUS
EFIAPI
SmbusControllerStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  EFI_STATUS               Status;
  EFI_PCI_IO_PROTOCOL      *PciIo;
  VOID                     *Hob;
  SMBUS_CONFIG_HOB         *SmbusConfigHob;
  SMBUS_PRIVATE_DATA_DXE   *SmbusPrivate;
  VOID                     *Registration;
  UINTN                    DataSize;
  VOID                     *Data;
  EFI_PEI_HOB_POINTERS     HobList;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  SmbusConfigHob = NULL;
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **)&PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a error. return status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = SmbusEnableHc (PciIo);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Hob = GetFirstGuidHob (&gPchSmbusConfigHobGuid);
  if (Hob != NULL) {
    SmbusConfigHob = (SMBUS_CONFIG_HOB *) GET_GUID_HOB_DATA (Hob);
  }

  SmbusPrivate = AllocateZeroPool (sizeof (SMBUS_PRIVATE_DATA_DXE));
  if (SmbusPrivate == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbusPrivate->Signature                  = PCH_SMBUS_PRIVATE_DATA_SIGNATURE;
  SmbusPrivate->SmbusController.Execute    = SmbusExecute;
  SmbusPrivate->SmbusController.ArpDevice  = SmbusArpDevice;
  SmbusPrivate->SmbusController.GetArpMap  = SmbusGetArpMap;
  SmbusPrivate->SmbusController.Notify     = SmbusNotify;
  SmbusPrivate->PciIo                      = PciIo;
  if (SmbusConfigHob != NULL) {
    SmbusPrivate->DeviceEnumData.PlatformNumRsvd            = SmbusConfigHob->NumRsvdSmbusAddresses;
    CopyMem (SmbusPrivate->DeviceEnumData.PlatformRsvdAddr, SmbusConfigHob->RsvdSmbusAddressTable, sizeof (UINT8) * PCH_MAX_SMBUS_RESERVED_ADDRESS);
  } else {
    SmbusPrivate->DeviceEnumData.PlatformNumRsvd            = 0;
  }
  ASSERT (SmbusPrivate->DeviceEnumData.PlatformNumRsvd <= PCH_MAX_SMBUS_RESERVED_ADDRESS);

  //
  // See if PEI already ARPed any devices, and if so, update our device map.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, (VOID **) &HobList.Raw);
  ASSERT_EFI_ERROR (Status);

  HobList.Raw = GetNextGuidHob (&gEfiSmbusArpMapGuid, HobList.Raw);
  if (HobList.Raw != NULL) {
    Data      = (VOID *) ((UINT8 *) (&HobList.Guid->Name) + sizeof (EFI_GUID));
    DataSize  = HobList.Header->HobLength - sizeof (EFI_HOB_GUID_TYPE);
    if (DataSize < MAX_SMBUS_DEVICES) {
      CopyMem (SmbusPrivate->DeviceEnumData.DeviceMap, Data, DataSize);
      SmbusPrivate->DeviceEnumData.DeviceMapEntries = (UINT8) (DataSize / sizeof (EFI_SMBUS_DEVICE_MAP));
    }
  }

  InitializeListHead (&SmbusPrivate->NotifyFunctionList);

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &SmbusPrivate->Handle,
                  &gEfiSmbusHcProtocolGuid,
                  &SmbusPrivate->SmbusController,
                  NULL
                  );

  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    SmbusPciEnumerationCompleteCallback,
    SmbusPrivate,
    &Registration
    );

  DEBUG ((DEBUG_INFO, "%a end with %r\n", __FUNCTION__, Status));

  return Status;
}

/**
  Stop this driver on ControllerHandle.

  @param This               A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param Controller         A handle to the device being stopped.
  @param NumberOfChildren   The number of child device handles in ChildHandleBuffer.
  @param ChildHandleBuffer  An array of child handles to be freed.

  @retval EFI_SUCCESS       This driver is removed from this device.
  @retval other             Some error occurs when removing this driver from this device.

**/
EFI_STATUS
EFIAPI
SmbusControllerStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer
  )
{
  return EFI_UNSUPPORTED;
}

EFI_DRIVER_BINDING_PROTOCOL  gSmbusDriverBinding = {
  SmbusControllerSupported,
  SmbusControllerStart,
  SmbusControllerStop,
  0xA,
  NULL,
  NULL
};

/**
  Retrieves a Unicode string that is the user readable name of the UEFI Driver.

  @param This                   A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
  @param Language               A pointer to a three character ISO 639-2 language identifier.
                                This is the language of the driver name that that the caller
                                is requesting, and it must match one of the languages specified
                                in SupportedLanguages.  The number of languages supported by a
                                driver is up to the driver writer.
  @param DriverName             A pointer to the Unicode string to return.  This Unicode string
                                is the name of the driver specified by This in the language
                                specified by Language.

  @retval EFI_SUCCESS           The Unicode string for the Driver specified by This
                                and the language specified by Language was returned
                                in DriverName.
  @retval EFI_INVALID_PARAMETER Language is NULL.
  @retval EFI_INVALID_PARAMETER DriverName is NULL.
  @retval EFI_UNSUPPORTED       The driver specified by This does not support the
                                language specified by Language.
**/
EFI_STATUS
EFIAPI
SmbusComponentNameGetDriverName (
  IN EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN CHAR8                        *Language,
  OUT CHAR16                      **DriverName
  )
{
  return LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           mSmbusDriverNameTable,
           DriverName,
           (BOOLEAN)(This == &gSmbusComponentName)
           );
}

/**
  Retrieves a Unicode string that is the user readable name of the controller
  that is being managed by an UEFI Driver.

  @param This                   A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
  @param ControllerHandle       The handle of a controller that the driver specified by
                                This is managing.  This handle specifies the controller
                                whose name is to be returned.
  @param ChildHandle OPTIONAL   The handle of the child controller to retrieve the name
                                of.  This is an optional parameter that may be NULL.  It
                                will be NULL for device drivers.  It will also be NULL
                                for a bus drivers that wish to retrieve the name of the
                                bus controller.  It will not be NULL for a bus driver
                                that wishes to retrieve the name of a child controller.
  @param Language               A pointer to a three character ISO 639-2 language
                                identifier.  This is the language of the controller name
                                that that the caller is requesting, and it must match one
                                of the languages specified in SupportedLanguages.  The
                                number of languages supported by a driver is up to the
                                driver writer.
  @param ControllerName         A pointer to the Unicode string to return.  This Unicode
                                string is the name of the controller specified by
                                ControllerHandle and ChildHandle in the language
                                specified by Language from the point of view of the
                                driver specified by This.

  @retval EFI_SUCCESS           The Unicode string for the user readable name in the
                                language specified by Language for the driver
                                specified by This was returned in DriverName.
  @retval EFI_INVALID_PARAMETER ControllerHandle is not a valid EFI_HANDLE.
  @retval EFI_INVALID_PARAMETER ChildHandle is not NULL and it is not a valid
                                EFI_HANDLE.
  @retval EFI_INVALID_PARAMETER Language is NULL.
  @retval EFI_INVALID_PARAMETER ControllerName is NULL.
  @retval EFI_UNSUPPORTED       The driver specified by This is not currently
                                managing the controller specified by
                                ControllerHandle and ChildHandle.
  @retval EFI_UNSUPPORTED       The driver specified by This does not support the
                                language specified by Language.
**/
EFI_STATUS
EFIAPI
SmbusComponentNameGetControllerName (
  IN EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_HANDLE                   ChildHandle OPTIONAL,
  IN CHAR8                        *Language,
  OUT CHAR16                      **ControllerName
  )
{
  EFI_STATUS  Status;

  //
  // Make sure this driver is currently managing ControllHandle
  //
  Status = EfiTestManagedDevice (
             ControllerHandle,
             gSmbusDriverBinding.DriverBindingHandle,
             &gEfiPciIoProtocolGuid
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (ChildHandle != NULL) {
    return EFI_UNSUPPORTED;
  }

  return LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           mSmbusControllerNameTable,
           ControllerName,
           (BOOLEAN)(This == &gSmbusComponentName)
           );
}

/**
  <b>SMBus DXE Module Entry Point</b>\n
  - <b>Introduction</b>\n
    The SMBus module is a DXE driver that provides a standard way to execute an SMBus command.

  - @pre
    - @link _PCH_POLICY PCH_POLICY_HOB @endlink
      - This module uses the parameters in SmbusConfig for platform reserved (non ARP capable) addresses.

  - @result
    The SMBus driver produces EFI_SMBUS_HC_PROTOCOL which is documented in the SMBus Host Controller
    Protocol Specification.

  @param[in] ImageHandle          ImageHandle of this module
  @param[in] SystemTable          EFI System Table

  @retval EFI_SUCCESS             Driver initializes successfully
  @retval Other values            Some error occurred
**/
EFI_STATUS
EFIAPI
SmbusEntryPoint (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS               Status;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Status = EfiLibInstallDriverBindingComponentName2 (
    ImageHandle,
    SystemTable,
    &gSmbusDriverBinding,
    ImageHandle,
    &gSmbusComponentName,
    &gSmbusComponentName2
  );

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return Status;
}