/** @file
  Implementation for functions to support AMT.

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

#include <PiDxe.h>
#include <IndustryStandard/Pci30.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/DxeAmtSupportLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MeInitLib.h>
#include <Protocol/PciIo.h>
#include <Register/UsbHostRegs.h>
#include <Register/PchRegs.h>
#include <PcieRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <MeBiosPayloadHob.h>
#include <MeSmbios.h>
#include <MeDefines.h>
#include <MeHandle.h>
#include <PciSbdf.h>

GLOBAL_REMOVE_IF_UNREFERENCED EFI_DEVICE_PATH_PROTOCOL *mUsbrStorageDevicePath = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_DEVICE_PATH_PROTOCOL *mSerialOverLanDevicePath = NULL;

/**
  Check if USB-R port for remote session is enabled in runtime.

  @retval TRUE         USB-R Port for Remote Console is Enabled.
  @retval FALSE        USB-R Port for Remote Console is NOT Enabled.
**/
BOOLEAN
STATIC
IsUsbrPortEnabled (
  VOID
  )
{
  EFI_STATUS Status;
  UINT64     XhciMmioBase;
  UINT64     UsbrMmioBase;
  UINT8      OrgCmd;
  BOOLEAN    IsPortEnabled;
  ME_HANDLE  *MeHandle;

  Status = GetMeHandle (&MeHandle);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  IsPortEnabled = FALSE;
  if (PciSegmentRead32 (MeHandle->XhciPciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
    return FALSE;
  }
  OrgCmd       = PciSegmentRead8 (MeHandle->XhciPciCfgBase + PCI_COMMAND_OFFSET);
  XhciMmioBase = PciSegmentRead32 (MeHandle->XhciPciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & ~(UINT32)(0xFFFF);
  if ((PciSegmentRead32 (MeHandle->XhciPciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
    XhciMmioBase += LShiftU64 ((UINT64) PciSegmentRead32 (MeHandle->XhciPciCfgBase + (PCI_BASE_ADDRESSREG_OFFSET + 4)), 32);
  }

  if (XhciMmioBase == 0) {
    return FALSE;
  }

  if ((OrgCmd & EFI_PCI_COMMAND_MEMORY_SPACE) == 0) {
    PciSegmentWrite8 (MeHandle->XhciPciCfgBase + PCI_COMMAND_OFFSET, OrgCmd | EFI_PCI_COMMAND_MEMORY_SPACE);
  }

  //
  // Check if KVM Port is Enabled and Connected
  //
  UsbrMmioBase = XhciMmioBase + R_XHCI_MEM_PORTSC1 + (MeHandle->UsbrKvmPortNum * S_XHCI_MEM_PORTSC_PORT_SPACING);
  if ((MmioRead32 (UsbrMmioBase) & (B_XHCI_MEM_PORTSC1_PED | B_XHCI_MEM_PORTSC1_CCS)) == (B_XHCI_MEM_PORTSC1_PED | B_XHCI_MEM_PORTSC1_CCS)) {
    IsPortEnabled = TRUE;
  }

  if ((OrgCmd & EFI_PCI_COMMAND_MEMORY_SPACE) == 0) {
    PciSegmentWrite8 (MeHandle->XhciPciCfgBase + PCI_COMMAND_OFFSET, OrgCmd);
  }

  DEBUG ((DEBUG_INFO, "IsPortEnabled = %x\n", IsPortEnabled));
  return IsPortEnabled;
}

/**
  Checks if Manageability is supported.

  @retval True           Manageability is supported.
  @retval False          Manageability is not supported.
**/
BOOLEAN
AmtIsManageabilitySupportEnabled (
  VOID
  )
{
  ME_BIOS_PAYLOAD_HOB   *MbpHob;

  MbpHob = NULL;

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    DEBUG ((DEBUG_ERROR, "No MBP Data Protocol available\n"));
    return FALSE;
  }

  if (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType != IntelMeCorporateFw ||
      MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.Amt != 1 ||
      MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Amt != 1 ) {
    return FALSE;
  }

  return TRUE;
}

/**
  Checks if KVM session is active

  @retval True                    KVM session is active.
  @retval False                   No KVM session in progress.
**/
BOOLEAN
AmtIsKvmSessionEnabled (
  VOID
  )
{
  return AsfIsKvmEnabled () || IsUsbrPortEnabled ();
}

/**
  Checks if there is any remote session active

  @retval True                    Remote session active.
  @retval False                   No remote session in progress.
**/
BOOLEAN
AmtIsRemoteSessionEnabled (
  VOID
  )
{
  return AmtIsKvmSessionEnabled () || AsfIsSolEnabled () || AsfIsStorageRedirectionEnabled ();
}

/**
  Get Storage Redirection Device Path

  @retval DevicePath   Pointer to Storage Redirection Device Path
  @retval NULL         Storage Redirection Device Path Not Found
**/
VOID *
AmtGetStorageRedirectionDevicePath (
  VOID
  )
{
  EFI_STATUS                   Status;
  UINTN                        HandleNum;
  UINTN                        Index;
  EFI_HANDLE                   *PciIoHandles;
  EFI_PCI_IO_PROTOCOL          *PciIo;
  UINTN                        Segment;
  UINTN                        Bus;
  UINTN                        Device;
  UINTN                        Function;
  EFI_DEVICE_PATH_PROTOCOL     *UsbrStorageDP;
  USB_DEVICE_PATH              UsbDP;
  ME_HANDLE                    *MeHandle;
  PCI_SBDF                     XhciSbdf;

  ZeroMem (&UsbDP, sizeof (USB_DEVICE_PATH));

  if (mUsbrStorageDevicePath != NULL) {
    return (VOID *) mUsbrStorageDevicePath;
  }

  Status = GetMeHandle (&MeHandle);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  UsbrStorageDP = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleNum,
                  &PciIoHandles
                  );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  for (Index = 0; Index < HandleNum; Index++) {
    Status = gBS->HandleProtocol (
                    PciIoHandles[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
    if (EFI_ERROR (Status)) {
      continue;
    }
    //
    // The XHCI controller of USBr port is on same bus of CSME.
    //
    XhciSbdf.PciSegmentLibAddress = MeHandle->XhciPciCfgBase;
    if (Bus == XhciSbdf.Fields.Bus && Device == XhciSbdf.Fields.Dev && Function == XhciSbdf.Fields.Func) {
      UsbrStorageDP = DevicePathFromHandle (PciIoHandles[Index]);
      break;
    }
  }

  gBS->FreePool (PciIoHandles);
  if (UsbrStorageDP == NULL) {
    return NULL;
  }

  //
  // Append USB Device Path Node
  //
  UsbDP.Header.Type      = MESSAGING_DEVICE_PATH;
  UsbDP.Header.SubType   = MSG_USB_DP;
  SetDevicePathNodeLength (&UsbDP, sizeof (USB_DEVICE_PATH));

  UsbDP.InterfaceNumber  = AsfGetStorageRedirectionBootDevice ();
  UsbDP.ParentPortNumber = MeHandle->UsbrStoragePortNum;
  UsbrStorageDP = AppendDevicePathNode (UsbrStorageDP, (EFI_DEVICE_PATH_PROTOCOL *) &UsbDP);

  mUsbrStorageDevicePath = DuplicateDevicePath (UsbrStorageDP);
  return (VOID *) mUsbrStorageDevicePath;
}

/**
  Get Serial Over Lan Device Path

  @retval DevicePath   Pointer to Serial Over Lan Device Path
  @retval NULL         Serial Over Lan Device Path Not Found
**/
VOID *
AmtGetSolDevicePath (
  VOID
  )
{
  EFI_STATUS                   Status;
  UINTN                        HandleNum;
  UINTN                        Index;
  EFI_HANDLE                   *PciIoHandles;
  EFI_PCI_IO_PROTOCOL          *PciIo;
  EFI_DEVICE_PATH_PROTOCOL     *SolDP;
  UART_DEVICE_PATH             UartDP;
  VENDOR_DEVICE_PATH           VendorDP;
  UINTN                        Segment;
  UINTN                        Bus;
  UINTN                        Device;
  UINTN                        Function;
  ME_HANDLE                    *MeHandle;
  PCI_SBDF                     SolSbdf;

  ZeroMem (&UartDP, sizeof (USB_DEVICE_PATH));
  ZeroMem (&VendorDP, sizeof (USB_DEVICE_PATH));
  if (mSerialOverLanDevicePath != NULL) {
    return (VOID *) mSerialOverLanDevicePath;
  }

  Status = GetMeHandle (&MeHandle);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  SolDP = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleNum,
                  &PciIoHandles
                  );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  for (Index = 0; Index < HandleNum; Index++) {
    Status = gBS->HandleProtocol (
                    PciIoHandles[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
    if (EFI_ERROR (Status)) {
      continue;
    }

    SolSbdf.PciSegmentLibAddress = MeHandle->Callbacks->GetCsmeHeciPciCfgBase (SOL);

    if (Bus == SolSbdf.Fields.Bus &&  Device == SolSbdf.Fields.Dev && Function == SolSbdf.Fields.Func) {
      SolDP = DevicePathFromHandle (PciIoHandles[Index]);
      break;
    }
  }

  gBS->FreePool (PciIoHandles);
  if (SolDP == NULL) {
    return NULL;
  }

  //
  // Append UART Device Path Node
  //
  UartDP.Header.Type    = MESSAGING_DEVICE_PATH;
  UartDP.Header.SubType = MSG_UART_DP;
  SetDevicePathNodeLength (&UartDP, sizeof (UART_DEVICE_PATH));

  UartDP.Reserved       = 0;
  UartDP.BaudRate       = 115200;
  UartDP.DataBits       = 8;
  UartDP.Parity         = 1;
  UartDP.StopBits       = 1;
  SolDP = AppendDevicePathNode (SolDP, (EFI_DEVICE_PATH_PROTOCOL *) &UartDP);

  //
  // Append Terminal Type Device Path Node
  //
  VendorDP.Header.Type    = MESSAGING_DEVICE_PATH;
  VendorDP.Header.SubType = MSG_VENDOR_DP;

  SetDevicePathNodeLength (&VendorDP, sizeof (VENDOR_DEVICE_PATH));

  switch (PcdGet8 (PcdSolTerminalType)) {
    case OEM_CAPS_2_TERMINAL_EMULATION_VT100PLUS:
      CopyGuid (&VendorDP.Guid, &gEfiVT100PlusGuid);
      break;

    case OEM_CAPS_2_TERMINAL_EMULATION_PCANSI:
      CopyGuid (&VendorDP.Guid, &gEfiPcAnsiGuid);
      break;

    case OEM_CAPS_2_TERMINAL_EMULATION_VTUTF8:
    default:
      CopyGuid (&VendorDP.Guid, &gEfiVTUTF8Guid);
      break;
  }
  SolDP = AppendDevicePathNode (SolDP, (EFI_DEVICE_PATH_PROTOCOL *) &VendorDP);

  mSerialOverLanDevicePath = DuplicateDevicePath (SolDP);
  return (VOID *) mSerialOverLanDevicePath;
}
