/** @file
  Performs PCI and Media device inventory.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2005 Intel Corporation.

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

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MeInitLib.h>
#include <Library/VmdInfoLib.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/IdeControllerInit.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/NvmExpressPassthru.h>
#include <Protocol/AtaPassThru.h>
#include <Protocol/BlockIo.h>
#include <Protocol/UsbIo.h>
#include <Protocol/MebxConfigProtocol.h>
#include <IndustryStandard/AlertStandardFormatTable.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Pci.h>
#include <IndustryStandard/SmBios.h>
#include <IndustryStandard/Nvme.h>
#include <MeSmbios.h>
#include <MebxData.h>
#include <MeHandle.h>
#include <MeDefines.h>
#include <PciSbdf.h>
#include "Inventory.h"

/**
  Try to get the controller's USB description.

  @param[in]  Handle                USB Controller handle.
  @param[out] UsbModelNo            USB Model Number.
  @param[out] UsbSerialNo           USB Serial Number.

  @return EFI_SUCCESS               Successfully get the USB description.
  @return EFI_INVALID_PARAMETER     Invalid parameter.
  @return EFI_OUT_OF_RESOURCES      Could not allocate required resource.
  @return EFI_NOT_FOUND             Fail to get the USB description.
**/
EFI_STATUS
GetUsbDescription (
  IN  EFI_HANDLE             Handle,
  OUT CHAR8                  *UsbModelNo,
  OUT CHAR8                  *UsbSerialNo
  )
{
  EFI_STATUS                   Status;
  EFI_USB_IO_PROTOCOL          *UsbIo;
  CHAR16                       NullChar;
  CHAR16                       *Manufacturer;
  CHAR16                       *Product;
  CHAR16                       *SerialNumber;
  CHAR16                       *ModelName;
  EFI_USB_DEVICE_DESCRIPTOR    DevDesc;
  UINTN                        ModelNameSize;
  UINT16                       UsbLangId;
  CHAR8                        AsciiBuffer[256];

  DEBUG ((DEBUG_INFO, "%a () enter\n", __FUNCTION__));

  if ((UsbModelNo == NULL) || (UsbSerialNo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiUsbIoProtocolGuid,
                  (VOID **) &UsbIo
                  );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Manufacturer = NULL;
  Product      = NULL;
  SerialNumber = NULL;
  ModelName    = NULL;
  UsbLangId    = 0x0409; // set language as English
  NullChar     = L'\0';

  Status = UsbIo->UsbGetDeviceDescriptor (UsbIo, &DevDesc);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = UsbIo->UsbGetStringDescriptor (
                    UsbIo,
                    UsbLangId,
                    DevDesc.StrManufacturer,
                    &Manufacturer
                    );
  if (EFI_ERROR (Status)) {
    Manufacturer = &NullChar;
  }

  Status = UsbIo->UsbGetStringDescriptor (
                    UsbIo,
                    UsbLangId,
                    DevDesc.StrProduct,
                    &Product
                    );
  if (EFI_ERROR (Status)) {
    Product = &NullChar;
  }

  Status = UsbIo->UsbGetStringDescriptor (
                    UsbIo,
                    UsbLangId,
                    DevDesc.StrSerialNumber,
                    &SerialNumber
                    );
  if (EFI_ERROR (Status)) {
    SerialNumber = &NullChar;
  }

  if ((Manufacturer == &NullChar) || (Product == &NullChar) || (SerialNumber == &NullChar)) {
    return EFI_NOT_FOUND;
  }

  ModelNameSize = StrSize (Manufacturer) + StrSize (Product);
  ModelName = AllocateZeroPool (ModelNameSize);
  if (ModelName == NULL) {
    if (Manufacturer != &NullChar) {
      FreePool (Manufacturer);
    }
    if (Product != &NullChar) {
      FreePool (Product);
    }
    if (SerialNumber != &NullChar) {
      FreePool (SerialNumber);
    }
    return EFI_OUT_OF_RESOURCES;
  }

  StrCatS (ModelName, ModelNameSize / sizeof (CHAR16), Manufacturer);
  StrCatS (ModelName, ModelNameSize / sizeof (CHAR16), L" ");
  StrCatS (ModelName, ModelNameSize / sizeof (CHAR16), Product);

  //
  // UnicodeStrToAsciiStrS might fail if USB description model name and serial number are larger than ModelNo and SerialNo in HWA_FRU_MEDIA_DEVICES.
  // Use AsciiBuffer as a converged buffer.
  //
  ZeroMem (AsciiBuffer, sizeof (AsciiBuffer));
  UnicodeStrToAsciiStrS (ModelName, AsciiBuffer, sizeof (AsciiBuffer));
  CopyMem (UsbModelNo, AsciiBuffer, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH);

  ZeroMem (AsciiBuffer, sizeof (AsciiBuffer));
  UnicodeStrToAsciiStrS (SerialNumber, AsciiBuffer, sizeof (AsciiBuffer));
  CopyMem (UsbSerialNo, AsciiBuffer, MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH);

  if (Manufacturer != &NullChar) {
    FreePool (Manufacturer);
  }
  if (Product != &NullChar) {
    FreePool (Product);
  }
  if (SerialNumber != &NullChar) {
    FreePool (SerialNumber);
  }
  if (ModelName != NULL) {
    FreePool (ModelName);
  }

  return EFI_SUCCESS;
}

/**
  This function gets the protocol interface from the given handle, and
  obtains its address space descriptors.

  @param[in]  Handle         The PCI_ROOT_BRIDIGE_IO_PROTOCOL handle.
  @param[out] IoDev          Handle used to access configuration space of PCI device.
  @param[out] Descriptors    Points to the address space descriptors.

  @retval EFI_SUCCESS     The command completed successfully
**/
EFI_STATUS
PciGetProtocolAndResource (
  IN  EFI_HANDLE                         Handle,
  OUT EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL    **IoDev,
  OUT EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  **Descriptors
  )
{
  EFI_STATUS  Status;

  //
  // Get inferface from protocol
  //
  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiPciRootBridgeIoProtocolGuid,
                  (VOID **)IoDev
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Call Configuration() to get address space descriptors
  //
  Status = (*IoDev)->Configuration (*IoDev, (VOID **)Descriptors);
  if (Status == EFI_UNSUPPORTED) {
    *Descriptors = NULL;
    return EFI_SUCCESS;
  } else {
    return Status;
  }
}

/**
  This function get the next bus range of given address space descriptors.
  It also moves the pointer backward a node, to get prepared to be called
  again.

  @param[in, out] Descriptors Points to current position of a serial of address space
                              descriptors.
  @param[out] MinBus          The lower range of bus number.
  @param[out] MaxBus          The upper range of bus number.
  @param[out] IsEnd           Meet end of the serial of descriptors.

  @retval EFI_SUCCESS     The command completed successfully.
**/
EFI_STATUS
PciGetNextBusRange (
  IN OUT EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  **Descriptors,
  OUT    UINT16                             *MinBus,
  OUT    UINT16                             *MaxBus,
  OUT    BOOLEAN                            *IsEnd
  )
{
  *IsEnd = FALSE;

  //
  // When *Descriptors is NULL, Configuration() is not implemented, so assume
  // range is 0~PCI_MAX_BUS
  //
  if ((*Descriptors) == NULL) {
    *MinBus = 0;
    *MaxBus = PCI_MAX_BUS;
    return EFI_SUCCESS;
  }

  //
  // *Descriptors points to one or more address space descriptors, which
  // ends with a end tagged descriptor. Examine each of the descriptors,
  // if a bus typed one is found and its bus range covers bus, this handle
  // is the handle we are looking for.
  //

  while ((*Descriptors)->Desc != ACPI_END_TAG_DESCRIPTOR) {
    if ((*Descriptors)->ResType == ACPI_ADDRESS_SPACE_TYPE_BUS) {
      *MinBus = (UINT16)(*Descriptors)->AddrRangeMin;
      *MaxBus = (UINT16)(*Descriptors)->AddrRangeMax;
      (*Descriptors)++;
      return (EFI_SUCCESS);
    }

    (*Descriptors)++;
  }

  if ((*Descriptors)->Desc == ACPI_END_TAG_DESCRIPTOR) {
    *IsEnd = TRUE;
  }

  return EFI_SUCCESS;
}

/**
  Create the FRU table to send to AMT FW

  @param[in] AssetTableData  Buffer of all Asset tables to send to FW
  @param[in] TableOffset     Offset to the beginning of HWA PCI Fru Table

  @return    Size            Size of the HWA PCI Fru Table
**/
UINT16
BuildHwaPciFruTable (
  IN HWA_TABLE                    *AssetTableData,
  IN UINT16                       TableOffset
  )
{
  HWA_FRU_TABLE                      *PciFruTable;
  HWA_PCI_FRU_DATA                   *PciFru;
  EFI_STATUS                         Status;
  ME_HANDLE                          *MeHandle;
  PCI_SBDF                           HeciSbdf;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL    *IoDev;
  UINT16                             Bus;
  UINT16                             Device;
  UINT16                             Func;
  UINT64                             Address;
  PCI_DEVICE_INDEPENDENT_REGION      PciHeader;
  UINT16                             Length;
  UINTN                              HandleBufSize;
  EFI_HANDLE                         *HandleBuf;
  UINTN                              HandleCount;
  UINTN                              Index;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  *Descriptors;
  UINT16                             MinBus;
  UINT16                             MaxBus;
  BOOLEAN                            IsEnd;

  DEBUG ((DEBUG_INFO, "%a() Entry\n", __FUNCTION__));

  PciFruTable = (HWA_FRU_TABLE*)&AssetTableData->TableData[TableOffset];
  PciFru      = &PciFruTable->Data[0];
  IoDev       = NULL;
  HandleBuf   = NULL;
  Descriptors = NULL;
  ZeroMem (&PciHeader, sizeof (PCI_DEVICE_INDEPENDENT_REGION));

  //
  // Get all instances of PciRootBridgeIo. Allocate space for 1 EFI_HANDLE and
  // call LibLocateHandle(), if EFI_BUFFER_TOO_SMALL is returned, allocate enough
  // space for handles and call it again.
  //
  HandleBufSize = sizeof (EFI_HANDLE);
  HandleBuf     = (EFI_HANDLE *)AllocateZeroPool (HandleBufSize);
  Status = gBS->LocateHandle (
                  ByProtocol,
                  &gEfiPciRootBridgeIoProtocolGuid,
                  NULL,
                  &HandleBufSize,
                  HandleBuf
                  );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    HandleBuf = ReallocatePool (sizeof (EFI_HANDLE), HandleBufSize, HandleBuf);
    if (HandleBuf == NULL) {
      return 0;
    }

    Status = gBS->LocateHandle (
                    ByProtocol,
                    &gEfiPciRootBridgeIoProtocolGuid,
                    NULL,
                    &HandleBufSize,
                    HandleBuf
                    );
  }
  if (EFI_ERROR (Status)) {
    return 0;
  }

  HandleCount = HandleBufSize / sizeof (EFI_HANDLE);

  Status = GetMeHandle (&MeHandle);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  HeciSbdf.PciSegmentLibAddress = MeHandle->Callbacks->GetCsmeHeciPciCfgBase (HECI1);

  //
  // For each handle, which decides a segment and a bus number range,
  // enumerate all devices on it.
  //
  for (Index = 0; Index < HandleCount; Index++) {
    //
    // Locate the PCI IO protocol
    //
    Status = PciGetProtocolAndResource (
               HandleBuf[Index],
               &IoDev,
               &Descriptors
               );
    if (EFI_ERROR (Status)) {
      return 0;
    }

    //
    // No document say it's impossible for a RootBridgeIo protocol handle
    // to have more than one address space descriptors, so find out every
    // bus range and for each of them do device enumeration.
    //
    while (TRUE) {
      Status = PciGetNextBusRange (&Descriptors, &MinBus, &MaxBus, &IsEnd);
      if (EFI_ERROR (Status)) {
        return 0;
      }

      if (IsEnd) {
        break;
      }

      //
      // Break the function loop if the device is not a multi - function device.
      // Only report the add-on devices installed in the system.
      //
      for (Bus = MinBus; Bus <= MaxBus; Bus++) {
        //
        // For each devices, enumerate all functions it contains
        //
        for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
          //
          // For each function, read its configuration space and print summary
          //
          for (Func = 0; Func <= PCI_MAX_FUNC; Func++) {
            Address = EFI_PCI_ADDRESS (Bus, Device, Func, 0);
            IoDev->Pci.Read (IoDev, EfiPciWidthUint16, Address, 1, &PciHeader.VendorId);

            //
            // If VendorId = 0xffff, there does not exist a device at this
            // location. For each device, if there is any function on it,
            // there must be 1 function at Function 0. So if Func = 0, there
            // will be no more functions in the same device, so we can break
            // loop to deal with the next device.
            //
            if ((PciHeader.VendorId == 0xffff) && (Func == 0)) {
              break;
            }

            if (PciHeader.VendorId != 0xffff) {
              IoDev->Pci.Read (IoDev, EfiPciWidthUint32, Address, sizeof (PciHeader) / sizeof (UINT32), &PciHeader);
              if (Bus != 0 && Bus != HeciSbdf.Fields.Bus) {
                //
                // List PCI devices not on SOC die and discrete PCH
                //
                PciFru->SmbiosType   = 0;
                PciFru->Length       = sizeof (HWA_PCI_FRU_DATA);
                PciFru->SmbiosHandle = 0;
                PciFru->FruType      = HWA_FRU_TYPE_PCI;
                PciFru->VendorId     = PciHeader.VendorId;
                PciFru->DeviceId     = PciHeader.DeviceId;
                PciFru->RevisionId   = PciHeader.RevisionID;
                PciFru->ClassCode    = ((UINT32) PciHeader.ClassCode[0]) | ((UINT32) PciHeader.ClassCode[1] << 0x8) | ((UINT32) PciHeader.ClassCode[2] << 0x10);
                //
                // Read SVID and SID
                //
                IoDev->Pci.Read (IoDev, EfiPciWidthUint16, Address + PCI_SVID_OFFSET, 2, &PciFru->SubsystemVendorId);
                PciFruTable->StructureCount++;
                PciFru++;
              }

              if (Func == 0 && ((PciHeader.HeaderType & HEADER_TYPE_MULTI_FUNCTION) == 0x00)) {
                break;
              }
            }
          }
        }
      }

      //
      // If Descriptor is NULL, Configuration() returns EFI_UNSUPPRORED,
      // we assume the bus range is 0~PCI_MAX_BUS. After enumerated all
      // devices on all bus, we can leave loop.
      //
      if (Descriptors == NULL) {
        break;
      }
    }
  }

  PciFruTable->TableByteCount = PciFruTable->StructureCount * sizeof (HWA_PCI_FRU_DATA);

  Length = PciFruTable->TableByteCount + sizeof (PciFruTable->StructureCount)
           + sizeof (PciFruTable->TableByteCount);

  return Length;
}

/**
  Transfer each other while being front and back.

  @param[in] Data                 The address of data
  @param[in] Size                 Size of data

**/
VOID
SwapEntries (
  IN CHAR8                        *Data,
  IN UINT8                        Size
  )
{
  UINT16                          Index;
  CHAR8                           Temp8;

  Index = 0;
  while (Data[Index] != 0 && Data[Index + 1] != 0) {
    Temp8           = Data[Index];
    Data[Index]     = Data[Index + 1];
    Data[Index + 1] = Temp8;
    Index += 2;
    if (Index >= Size) {
      break;
    }
  }

  return;
}

/**
  Check if the given device under VMD controlled

  @param[in] DevicePath  Device path.

  @retval TRUE           Device is under VMD controlled.
  @retval FALSE          Device is not under VMD controlled.
**/
BOOLEAN
EFIAPI
IsVmdControlled (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;
  PCI_DEVICE_PATH           *PciDevicePath;

  DevicePathNode = DevicePath;
  while (!IsDevicePathEndType (DevicePathNode)) {
    if (DevicePathNode->Type == HARDWARE_DEVICE_PATH &&
        DevicePathNode->SubType == HW_PCI_DP) {
      PciDevicePath = (PCI_DEVICE_PATH *) DevicePathNode;
      if (PciDevicePath->Device == GetVmdDevNumber () &&
          PciDevicePath->Function == GetVmdFuncNumber ()) {
        return TRUE;
      }
    }
    DevicePathNode = NextDevicePathNode (DevicePathNode);
  }

  return FALSE;
}

/**
  Get identify controller data.

  @param[in]   NvmeDevice        The pointer to the NVME_PASS_THRU_DEVICE data structure.
  @param[in]   NamespaceId       NamespaceId for an NVM Express namespace present on the NVM Express controller
  @param[out]  Buffer            The buffer used to store the identify controller data.

  @return EFI_SUCCESS            Successfully get the identify controller data.
  @return EFI_DEVICE_ERROR       Fail to get the identify controller data.
**/
EFI_STATUS
NvmeIdentifyController (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmeDevice,
  IN  UINT32                                NamespaceId,
  OUT VOID                                  *Buffer
  )
{
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;
  EFI_STATUS                                Status;

  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;
  CommandPacket.TransferBuffer = Buffer;
  CommandPacket.TransferLength = sizeof (NVME_ADMIN_CONTROLLER_DATA);
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;
  Command.Cdw0.Opcode          = NVME_ADMIN_IDENTIFY_CMD;
  //
  // According to Nvm Express 1.1 spec Figure 38, When not used, the field shall be cleared to 0h.
  // For the Identify command, the Namespace Identifier is only used for the Namespace data structure.
  //
  Command.Nsid                 = 0;
  //
  // Set bit 0 (Cns bit) to 1 to identify a controller
  //
  Command.Cdw10                = 1;
  Command.Flags                = CDW10_VALID;

  Status = NvmeDevice->PassThru (NvmeDevice, NamespaceId, &CommandPacket, NULL);

  DEBUG ((DEBUG_INFO, "%a Status %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Get specified identify namespace data.

  @param[in]   NvmeDevice        The pointer to the NVME_PASS_THRU_DEVICE data structure.
  @param[in]   NamespaceId       NamespaceId for an NVM Express namespace present on the NVM Express controller
  @param[out]  Buffer            The buffer used to store the identify controller data.

  @return EFI_SUCCESS            Successfully get the identify namespace data.
  @return EFI_DEVICE_ERROR       Fail to get the identify namespace data.
**/
EFI_STATUS
NvmeIdentifyNamespace (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmeDevice,
  IN  UINT32                                NamespaceId,
  OUT VOID                                  *Buffer
  )
{
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;
  EFI_STATUS                                Status;

  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;
  CommandPacket.TransferBuffer = Buffer;
  CommandPacket.TransferLength = sizeof (NVME_ADMIN_NAMESPACE_DATA);
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;
  Command.Cdw0.Opcode          = NVME_ADMIN_IDENTIFY_CMD;
  Command.Nsid                 = NamespaceId;
  //
  // Set bit 0 (Cns bit) to 0 to identify a namespace
  //
  Command.Cdw10                = 0;
  Command.Flags                = CDW10_VALID;

  Status = NvmeDevice->PassThru (NvmeDevice, NamespaceId, &CommandPacket, NULL);

  DEBUG ((DEBUG_INFO, "%a Status %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Checks if the given device is a USBr device.

  @param[in]   DevicePath         A pointer to a device path data structure.

  @retval      True               Current device is a USBr device.
  @return      False              Current device is not a USBr device.
**/
BOOLEAN
IsUsbrDevice (
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath
  )
{
  EFI_STATUS                   Status;
  EFI_DEVICE_PATH_PROTOCOL     *DevicePathNode;
  ME_HANDLE                    *MeHandle;

  Status = GetMeHandle (&MeHandle);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  DevicePathNode  = DevicePath;

  while (!IsDevicePathEnd (DevicePathNode)) {
    if ((DevicePathType (DevicePathNode) == MESSAGING_DEVICE_PATH) && (DevicePathSubType (DevicePathNode) == MSG_USB_DP)) {
      if (((USB_DEVICE_PATH *)DevicePathNode)->ParentPortNumber == MeHandle->UsbrStoragePortNum) {
        return TRUE;
      }
    }
    DevicePathNode = NextDevicePathNode (DevicePathNode);
  }
  return FALSE;
}


/**
  Searches the device path for the handle that has a pass thru protocol installed.

  @param[in]   DevicePath         A pointer to a device path data structure.
  @param[in]   ProtocolGuid       Guid of a protocol that is being searched.

  @retval      True               Pass Thru Protocol was located.
  @return      False              Failed to locate Pass Thru Protocol.
**/
BOOLEAN
IsPassThruProtocolAvailable (
  IN  EFI_DEVICE_PATH_PROTOCOL          *DevicePath,
  IN  EFI_GUID                          ProtocolGuid
  )
{
  EFI_STATUS                            Status;
  EFI_HANDLE                            Handle;

  Status = gBS->LocateDevicePath (
                  &ProtocolGuid,
                  &DevicePath,
                  &Handle
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Get Sata HDD identify data.

  @param[in]          AtaDevice           The pointer to the EFI_ATA_PASS_THRU_PROTOCOL.
  @param[in]          Port                Port number on the ATA controller
  @param[in]          PortMultiplierPort  Port multiplier port number on the ATA controller
  @param[out]         ControllerData      The buffer used to store the identify controller data.

  @return             EFI_SUCCESS         Successfully get the identify controller data.
  @return             EFI_DEVICE_ERROR    Fail to get the identify controller data.
**/
EFI_STATUS
GetHddIdentifyData (
  IN EFI_ATA_PASS_THRU_PROTOCOL             *AtaDevice,
  IN UINT16                                 Port,
  IN UINT16                                 PortMultiplierPort,
  OUT ATA_IDENTIFY_DATA                     *ControllerData
  )
{
  EFI_STATUS                               Status;
  EFI_ATA_PASS_THRU_COMMAND_PACKET         Packet;
  EFI_ATA_COMMAND_BLOCK                    Acb;
  EFI_ATA_STATUS_BLOCK                     Asb;

  ZeroMem (&Packet, sizeof(EFI_ATA_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Acb, sizeof(EFI_ATA_COMMAND_BLOCK));
  ZeroMem (&Asb, sizeof(EFI_ATA_STATUS_BLOCK));

  Acb.AtaCommand             = ATA_CMD_IDENTIFY_DRIVE;

  Packet.Protocol            = EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_IN;
  Packet.Acb                 = &Acb;
  Packet.Asb                 = &Asb;
  Packet.InDataBuffer        = ControllerData;
  Packet.InTransferLength    = sizeof (ATA_IDENTIFY_DATA);
  Packet.Length              = EFI_ATA_PASS_THRU_LENGTH_BYTES | EFI_ATA_PASS_THRU_LENGTH_SECTOR_COUNT;
  Packet.Timeout             = EFI_TIMER_PERIOD_SECONDS (3);

  Status = AtaDevice->PassThru (AtaDevice, Port, PortMultiplierPort, &Packet, NULL);

  return Status;

}

/**
  Get device identify data using Disk Info Protocol.

  @param[in]   Handle             Handle to the device.
  @param[out]  IdentifyDriveInfo  The buffer used to store the device identify data.

  @return EFI_SUCCESS             Successfully got the device identify data.
  @return Other                   Failed to get the identify controller data.
**/
EFI_STATUS
GetIdentifyDataFromDiskInfo (
  IN  EFI_HANDLE                        Handle,
  OUT EFI_IDENTIFY_DATA                 *IdentifyDriveInfo
  )
{
  EFI_STATUS                           Status;
  EFI_DISK_INFO_PROTOCOL               *DiskInfo;
  UINT32                               BufferSize;

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiDiskInfoProtocolGuid,
                  (VOID **) &DiskInfo
                  );

  if (EFI_ERROR (Status)) {
   return Status;
  }

  BufferSize = sizeof (EFI_IDENTIFY_DATA);
  ZeroMem (IdentifyDriveInfo, sizeof (EFI_IDENTIFY_DATA));

  Status = DiskInfo->Identify (
                       DiskInfo,
                       IdentifyDriveInfo,
                       &BufferSize
                       );
  ASSERT_EFI_ERROR (Status);

  return Status;
}


/**
  Detects all Ata Devices with Ata Pass Thru Protocol installed and adds them to
  media table.

  @param[in] MediaEntryPtr     A pointer to current Media Table tail.

  @return DeviceCount          Number of detected ATA Devices.
**/
UINT16
DetectAtaDevices (
  IN HWA_MEDIA_ENTRY         *MediaEntryPtr
  )
{
  EFI_STATUS                 Status;
  UINTN                      HandleNum;
  EFI_HANDLE                 *AtaPassThruHandles;
  UINTN                      Index;
  UINTN                      WordOffset;
  EFI_ATA_PASS_THRU_PROTOCOL *AtaDevice;
  EFI_ATA_IDENTIFY_DATA      AtaIdentifyData;
  UINT16                     Port;
  UINT16                     PortMultiplierPort;
  UINT64                     DriveSize;
  UINT16                     DeviceCount;

  DeviceCount = 0;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiAtaPassThruProtocolGuid,
                  NULL,
                  &HandleNum,
                  &AtaPassThruHandles
                  );
  if (EFI_ERROR (Status)) {
    return DeviceCount;
  }

  DEBUG ((DEBUG_INFO, "%a - Number of detected ATA Handles: %d\n", __FUNCTION__, HandleNum));

  for (Index = 0; Index < HandleNum; Index++) {

    Status = gBS->HandleProtocol (
                    AtaPassThruHandles[Index],
                    &gEfiAtaPassThruProtocolGuid,
                    (VOID **) &AtaDevice
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "gBS->Handle Protocol : Status = %r\n", Status));
      gBS->FreePool (AtaPassThruHandles);
      return DeviceCount;
    }

    //
    // Go through all of the ports and portmultiplierports and get identify data
    //
    Port = 0xFFFF;

    while (TRUE) {

      Status = AtaDevice->GetNextPort (AtaDevice, &Port);
      if (EFI_ERROR (Status)) {
        if (Status == EFI_NOT_FOUND) {
          break;
        } else {
          gBS->FreePool (AtaPassThruHandles);
          return DeviceCount;
        }
      }

      PortMultiplierPort = 0xFFFF;

      while (TRUE) {
        Status = AtaDevice->GetNextDevice (AtaDevice, Port, &PortMultiplierPort);
        if (EFI_ERROR (Status)) {
          if (Status == EFI_NOT_FOUND) {
            break;
          } else {
            gBS->FreePool (AtaPassThruHandles);
            return DeviceCount;
          }
        }

        ZeroMem (&AtaIdentifyData, sizeof(ATA_IDENTIFY_DATA));
        Status = GetHddIdentifyData (AtaDevice, Port, PortMultiplierPort, &AtaIdentifyData);
        if (EFI_ERROR (Status)) {
          continue;
        }

        DriveSize = AtaIdentifyData.maximum_lba_for_48bit_addressing[0];
        ///
        /// Lower byte goes first: word[100] is the lowest word, word[103] is highest
        ///
        for (WordOffset = 1; WordOffset < 4; WordOffset++) {
          DriveSize |= LShiftU64 (AtaIdentifyData.maximum_lba_for_48bit_addressing[WordOffset], 16 * WordOffset);
        }
        DriveSize = MultU64x32 (DriveSize, 512);

        MediaEntryPtr->SmbiosType                    = 0;
        MediaEntryPtr->Length                        = sizeof (HWA_MEDIA_ENTRY);
        MediaEntryPtr->SmbiosHandle                  = 0;
        MediaEntryPtr->MediaData.StructSize          = sizeof (HWA_FRU_MEDIA_DEVICES);
        MediaEntryPtr->MediaData.Interface           = HWA_MEDIA_IN_SATA;
        MediaEntryPtr->MediaData.DevType             = HWA_MEDIA_DT_HDD;
        SwapEntries ((CHAR8 *) AtaIdentifyData.ModelName, (UINT8)MEDIA_DEVICE_MODEL_NO_MAX_LENGTH);
        SwapEntries ((CHAR8 *) AtaIdentifyData.SerialNo, (UINT8)MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH);
        CopyMem (&MediaEntryPtr->MediaData.SerialNo, AtaIdentifyData.SerialNo, MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH);
        CopyMem (&MediaEntryPtr->MediaData.VersionNo, AtaIdentifyData.FirmwareVer, MEDIA_DEVICE_VERSION_NO_MAX_LENGTH);
        CopyMem (&MediaEntryPtr->MediaData.ModelNo, AtaIdentifyData.ModelName, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH);
        CopyMem (&MediaEntryPtr->MediaData.SupportedCmdSets, &AtaIdentifyData.command_set_supported_82, sizeof (MediaEntryPtr->MediaData.SupportedCmdSets));
        MediaEntryPtr->MediaData.MaxMediaSize = DriveSize;
        DeviceCount++;
        MediaEntryPtr++;
      }
    }
  }

  gBS->FreePool (AtaPassThruHandles);
  return DeviceCount;
}


/**
  Detects all Nvm Express Devices with Nvm Express Pass Thru Protocol installed and adds them to
  media table.

  @param[in] MediaEntryPtr     A pointer to current Media Table tail.

  @return DeviceCount          Number of detected NVMe Devices.
**/
UINT16
DetectNvmeDevices (
  IN OUT HWA_MEDIA_ENTRY             *MediaEntryPtr
  )
{
  EFI_STATUS                         Status;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL *NvmeDevice;
  NVME_ADMIN_CONTROLLER_DATA         ControllerData;
  NVME_ADMIN_NAMESPACE_DATA          NamespaceData;
  UINT32                             NamespaceId;
  UINTN                              HandleNum;
  EFI_HANDLE                         *NvmePassThruHandles;
  UINTN                              Index;
  UINT16                             DeviceCount;
  EFI_DEVICE_PATH_PROTOCOL           *DevicePath;
  BOOLEAN                            IsUnderVmdControl;
  UINT8                              SerialNo[MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH + 1];
  UINT8                              ModelNo[MEDIA_DEVICE_MODEL_NO_MAX_LENGTH + 1];

  DeviceCount       = 0;
  IsUnderVmdControl = 0;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiNvmExpressPassThruProtocolGuid,
                  NULL,
                  &HandleNum,
                  &NvmePassThruHandles
                  );
  if (EFI_ERROR (Status)) {
    return DeviceCount;
  }

  DEBUG ((DEBUG_INFO, "%a - Number of detected NVMe Handles: %d\n", __FUNCTION__, HandleNum));

  ZeroMem (SerialNo, sizeof (SerialNo));
  ZeroMem (ModelNo, sizeof (ModelNo));

  for (Index = 0; Index < HandleNum; Index++) {
    Status = gBS->HandleProtocol (
                    NvmePassThruHandles[Index],
                    &gEfiNvmExpressPassThruProtocolGuid,
                    (VOID **) &NvmeDevice
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Handle NvmExpressPassThru Protocol Error, Status = %r\n", Status));
      gBS->FreePool (NvmePassThruHandles);
      return DeviceCount;
    }

    Status = gBS->HandleProtocol (
                    NvmePassThruHandles[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **) &DevicePath
                    );
    if (!EFI_ERROR (Status) && DevicePath != NULL) {
      IsUnderVmdControl = IsVmdControlled (DevicePath);
      DEBUG ((DEBUG_INFO, "Is under VMD control: %d\n", IsUnderVmdControl));
    }

    NamespaceId = NVME_ALL_NAMESPACES;

    while (TRUE) {
      Status = NvmeDevice->GetNextNamespace (NvmeDevice, &NamespaceId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_WARN, "Get Next Namespace Status = %r\n", Status));
        if (Status == EFI_NOT_FOUND) {
          break;
        } else {
          gBS->FreePool (NvmePassThruHandles);
          return DeviceCount;
        }
      }

      DEBUG ((DEBUG_INFO, "[%d]NamespaceId: 0x%x\n", Index, NamespaceId));

      ZeroMem (&ControllerData, sizeof (NVME_ADMIN_CONTROLLER_DATA));
      ZeroMem (&NamespaceData, sizeof (NVME_ADMIN_NAMESPACE_DATA));

      //
      // Get NVMe namespace data and check Namespace Capacity to make sure it is valid.
      //
      Status = NvmeIdentifyNamespace (NvmeDevice, NamespaceId, (VOID *) &NamespaceData);
      if (EFI_ERROR (Status) || NamespaceData.Ncap == 0) {
        DEBUG ((DEBUG_WARN, "Nvme Identify Namespace Status = %r, Namespace Capacity = 0x%x\n", Status, NamespaceData.Ncap));
        continue;
      }

      //
      // Get NVMe controller data
      //
      if (IsUnderVmdControl) {
        Status = NvmeIdentifyController (NvmeDevice, NamespaceId, (VOID *) &ControllerData);
      } else {
        Status = NvmeIdentifyController (NvmeDevice, NVME_CONTROLLER_ID, (VOID *) &ControllerData);
      }
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_WARN, "Nvme Identify Controller Error, Status = %r.\n", Status));
        continue;
      }

      MediaEntryPtr->SmbiosType                    = 0;
      MediaEntryPtr->Length                        = sizeof (HWA_MEDIA_ENTRY);
      MediaEntryPtr->SmbiosHandle                  = 0;
      MediaEntryPtr->MediaData.StructSize          = sizeof (HWA_FRU_MEDIA_DEVICES);
      MediaEntryPtr->MediaData.Interface           = HWA_MEDIA_IN_PCIE;
      MediaEntryPtr->MediaData.DevType             = HWA_MEDIA_DT_HDD;
      CopyMem (&MediaEntryPtr->MediaData.ModelNo,   ControllerData.Mn, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH);
      CopyMem (&MediaEntryPtr->MediaData.SerialNo,  ControllerData.Sn, MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH);
      CopyMem (&MediaEntryPtr->MediaData.VersionNo, ControllerData.Fr, MEDIA_DEVICE_VERSION_NO_MAX_LENGTH);
      MediaEntryPtr->MediaData.SupportedCmdSets[0] = ControllerData.Oacs;
      MediaEntryPtr->MediaData.SupportedCmdSets[1] = ControllerData.Oncs;
      MediaEntryPtr->MediaData.MaxMediaSize        = MultU64x32 (NamespaceData.Ncap, 512);
      DeviceCount++;
      MediaEntryPtr++;

      DEBUG_CODE (
        CopyMem (SerialNo, ControllerData.Sn, MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH);
        SerialNo[MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH] = 0;
        CopyMem (ModelNo, ControllerData.Mn, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH);
        ModelNo[MEDIA_DEVICE_MODEL_NO_MAX_LENGTH] = 0;

        DEBUG ((DEBUG_INFO, " == NVME IDENTIFY DATA ==\n"));
        DEBUG ((DEBUG_INFO, "    SN        : %a   \n",   SerialNo));
        DEBUG ((DEBUG_INFO, "    MN        : %a   \n",   ModelNo));
        DEBUG ((DEBUG_INFO, "    FR        : 0x%x \n",   *((UINT64 *)ControllerData.Fr)));
        DEBUG ((DEBUG_INFO, "    MediaSize : 0x%x \n\n", MultU64x32 (NamespaceData.Ncap, 512)));
      );
    }
  }

  gBS->FreePool (NvmePassThruHandles);
  return DeviceCount;
}

/**
  Get last device before end of device path node.

  @param[in]   DevicePath         A pointer to a device path data structure.
  @param[out]  Type               The Type field of the device path node specified by Node.
  @param[out]  SubType            The SubType field of the device path node specified by Node.
**/
VOID
GetLastDeviceFromDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL         *DevicePath,
  OUT UINT8                            *Type,
  OUT UINT8                            *Subtype
  )
{
  EFI_DEVICE_PATH_PROTOCOL             *DevicePathNode;

  DevicePathNode  = DevicePath;

  while (!IsDevicePathEnd (DevicePathNode)) {
    *Type = DevicePathType (DevicePathNode);
    *Subtype = DevicePathSubType (DevicePathNode);
    DevicePathNode = NextDevicePathNode (DevicePathNode);
  }
}

/**
  Detects Block I/O devices without ATA and NVMe PassThru Protocol installed and adds them to
  media table.

  @param[in] MediaEntryPtr     A pointer to current Media Table tail.

  @return DeviceCount          Number of detected Block I/O Devices.
**/
UINT16
DetectBlockIoDevices (
  IN HWA_MEDIA_ENTRY       *MediaEntryPtr
  )
{
  EFI_STATUS               Status;
  UINTN                    Index;
  UINTN                    HandleCount;
  EFI_HANDLE               *HandleBuffer;
  EFI_BLOCK_IO_PROTOCOL    *BlkIo;
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
  EFI_IDENTIFY_DATA        IdentifyDriveInfo;
  UINT8                    Type;
  UINT8                    Subtype;
  UINT16                   DeviceCount;
  CHAR8                    UsbModelNo[MEDIA_DEVICE_MODEL_NO_MAX_LENGTH + 1];
  CHAR8                    UsbSerialNo[MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH + 1];

  DeviceCount = 0;

  ///
  /// Locate all media devices connected.
  /// We look for the Block I/O protocol to be attached to the device.
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiBlockIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  DEBUG ((DEBUG_INFO, "%a - Status = %r, HandleCount = %d\n", __FUNCTION__, Status, HandleCount));
  if (EFI_ERROR (Status) || HandleCount == 0) {
    return DeviceCount;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiBlockIoProtocolGuid,
                    (VOID **) &BlkIo
                    );
    ASSERT_EFI_ERROR (Status);

    ///
    /// We want to remove any Block I/O instances that refer to Logical partitions.
    /// A Block I/O instance is added to the raw device and any partition discovered on
    /// the media. This prevents duplications in our table.
    ///
    if (BlkIo->Media->LogicalPartition) {
      continue;
    }

    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID *) &DevicePath
                    );
    ASSERT_EFI_ERROR (Status);

    GetLastDeviceFromDevicePath (DevicePath, &Type, &Subtype);

    if (Type == MESSAGING_DEVICE_PATH) {
      switch (Subtype) {
        case MSG_ATAPI_DP:
          MediaEntryPtr->MediaData.MaxMediaSize = MultU64x32 (BlkIo->Media->LastBlock + 1, BlkIo->Media->BlockSize);
          MediaEntryPtr->MediaData.Interface = HWA_MEDIA_IN_ATAPI;
          if (BlkIo->Media->RemovableMedia) {
            MediaEntryPtr->MediaData.DevType = HWA_MEDIA_DT_DVD;
          } else {
            MediaEntryPtr->MediaData.DevType = HWA_MEDIA_DT_HDD;
          }

          Status = GetIdentifyDataFromDiskInfo (HandleBuffer[Index], &IdentifyDriveInfo);
          if (!EFI_ERROR (Status)) {
            SwapEntries ((CHAR8 *) &IdentifyDriveInfo.AtapiData.ModelName, (UINT8)MEDIA_DEVICE_MODEL_NO_MAX_LENGTH);
            SwapEntries ((CHAR8 *) &IdentifyDriveInfo.AtapiData.SerialNo, (UINT8)MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH);
            CopyMem (&MediaEntryPtr->MediaData.SerialNo, IdentifyDriveInfo.AtapiData.SerialNo, MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH);
            CopyMem (&MediaEntryPtr->MediaData.VersionNo, IdentifyDriveInfo.AtapiData.FirmwareVer, MEDIA_DEVICE_VERSION_NO_MAX_LENGTH);
            CopyMem (&MediaEntryPtr->MediaData.ModelNo, IdentifyDriveInfo.AtapiData.ModelName, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH);
            CopyMem (&MediaEntryPtr->MediaData.SupportedCmdSets, &IdentifyDriveInfo.AtapiData.cmd_set_support_82, sizeof (MediaEntryPtr->MediaData.SupportedCmdSets));
          } else {
            AsciiStrCpyS ((CHAR8*)&MediaEntryPtr->MediaData.ModelNo, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH, "ATAPI");
          }
          break;

        case MSG_USB_DP:
          if (IsUsbrDevice (DevicePath) == TRUE) {
            continue;
          }
          MediaEntryPtr->MediaData.MaxMediaSize = MultU64x32 (BlkIo->Media->LastBlock + 1, BlkIo->Media->BlockSize);
          MediaEntryPtr->MediaData.Interface = HWA_MEDIA_IN_ATA;
          MediaEntryPtr->MediaData.DevType   = HWA_MEDIA_DT_HDD;
          ZeroMem (UsbModelNo, sizeof (UsbModelNo));
          ZeroMem (UsbSerialNo, sizeof (UsbSerialNo));
          Status = GetUsbDescription (HandleBuffer[Index], UsbModelNo, UsbSerialNo);
          UsbModelNo[MEDIA_DEVICE_MODEL_NO_MAX_LENGTH] = 0;
          UsbSerialNo[MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH] = 0;
          DEBUG ((DEBUG_INFO, "UsbModelNo = %a\n", UsbModelNo));
          DEBUG ((DEBUG_INFO, "UsbSerialNo = %a\n", UsbSerialNo));
          if (!EFI_ERROR (Status)) {
            CopyMem (&MediaEntryPtr->MediaData.ModelNo, UsbModelNo, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH);
            CopyMem (&MediaEntryPtr->MediaData.SerialNo, UsbSerialNo, MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH);
          } else {
            AsciiStrCpyS ((CHAR8*)&MediaEntryPtr->MediaData.ModelNo, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH, "USB");
          }
          break;

        case MSG_SATA_DP:
          //
          // If this is a Removable Media it means the device is a ODD
          //
          if (BlkIo->Media->RemovableMedia) {
            MediaEntryPtr->MediaData.MaxMediaSize = MultU64x32 (BlkIo->Media->LastBlock + 1, BlkIo->Media->BlockSize);
            MediaEntryPtr->MediaData.Interface    = HWA_MEDIA_IN_SATA;
            MediaEntryPtr->MediaData.DevType      = HWA_MEDIA_DT_DVD;

            Status = GetIdentifyDataFromDiskInfo (HandleBuffer[Index], &IdentifyDriveInfo);
            if (!EFI_ERROR (Status)) {
              SwapEntries ((CHAR8*)&IdentifyDriveInfo.AtaData.ModelName, (UINT8)MEDIA_DEVICE_MODEL_NO_MAX_LENGTH);
              SwapEntries ((CHAR8*)&IdentifyDriveInfo.AtaData.SerialNo, (UINT8)MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH);
              CopyMem (&MediaEntryPtr->MediaData.SerialNo, IdentifyDriveInfo.AtaData.SerialNo, MEDIA_DEVICE_SERIAL_NO_MAX_LENGTH);
              CopyMem (&MediaEntryPtr->MediaData.VersionNo, IdentifyDriveInfo.AtaData.FirmwareVer, MEDIA_DEVICE_VERSION_NO_MAX_LENGTH);
              CopyMem (&MediaEntryPtr->MediaData.ModelNo, IdentifyDriveInfo.AtaData.ModelName, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH);
              CopyMem (&MediaEntryPtr->MediaData.SupportedCmdSets, &IdentifyDriveInfo.AtaData.command_set_supported_82, sizeof (MediaEntryPtr->MediaData.SupportedCmdSets));
            } else {
              AsciiStrCpyS ((CHAR8*)&MediaEntryPtr->MediaData.ModelNo, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH, "DVD Disk");
            }
            break;
          }
          //
          // If Protocol is available it means that the device was already identified and we should not increase
          // media device count.
          //
          if (IsPassThruProtocolAvailable (DevicePath, gEfiAtaPassThruProtocolGuid)) {
            continue;
          }
          MediaEntryPtr->MediaData.MaxMediaSize = MultU64x32 (BlkIo->Media->LastBlock + 1, BlkIo->Media->BlockSize);
          MediaEntryPtr->MediaData.Interface = HWA_MEDIA_IN_SATA;
          if (BlkIo->Media->RemovableMedia) {
            MediaEntryPtr->MediaData.DevType = HWA_MEDIA_DT_DVD;
          } else {
            MediaEntryPtr->MediaData.DevType = HWA_MEDIA_DT_HDD;
          }
          AsciiStrCpyS ((CHAR8*)&MediaEntryPtr->MediaData.ModelNo, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH, "Unknown SATA Disk");
          break;

        case MSG_NVME_NAMESPACE_DP:
          if (IsPassThruProtocolAvailable (DevicePath, gEfiNvmExpressPassThruProtocolGuid)) {
            continue;
          }
          MediaEntryPtr->MediaData.MaxMediaSize = MultU64x32 (BlkIo->Media->LastBlock + 1, BlkIo->Media->BlockSize);
          MediaEntryPtr->MediaData.Interface    = HWA_MEDIA_IN_PCIE;
          MediaEntryPtr->MediaData.DevType      = HWA_MEDIA_DT_HDD;
          AsciiStrCpyS ((CHAR8*)MediaEntryPtr->MediaData.ModelNo, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH, "Unknown NVMe Disk");
          break;

        default:
          MediaEntryPtr->MediaData.Interface = HWA_MEDIA_IN_ATA;
          MediaEntryPtr->MediaData.DevType   = HWA_MEDIA_DT_MO;
          AsciiStrCpyS ((CHAR8*)&MediaEntryPtr->MediaData.ModelNo, MEDIA_DEVICE_MODEL_NO_MAX_LENGTH, "Unknown");
          break;
      }

      //
      // Add a common preface for each entry, increase number of entries
      // and forward the entry pointer
      //
      MediaEntryPtr->SmbiosType           = 0;
      MediaEntryPtr->Length               = sizeof (HWA_MEDIA_ENTRY);
      MediaEntryPtr->SmbiosHandle         = 0;
      MediaEntryPtr->MediaData.StructSize = sizeof (HWA_FRU_MEDIA_DEVICES);
      DeviceCount++;
      MediaEntryPtr++;
    }
  }

  FreePool (HandleBuffer);

  return DeviceCount;
}


/**
  Create the Media table to send to AMT FW

  @param[in] AssetTableData            Buffer of all Asset tables to send to FW
  @param[in] TableOffset               Offset to the beginning of Media Table

  @return    Size                      Size of the HWA Media Table
*/
UINT16
BuildHwaMediaTable (
  IN HWA_TABLE             *AssetTableData,
  IN UINT16                TableOffset
  )
{
  HWA_MEDIA_TABLE          *MediaHeaderPtr;
  HWA_MEDIA_ENTRY          *MediaEntryPtr;
  UINT16                   Length;
  UINT16                   DeviceCount;

  MediaHeaderPtr = (HWA_MEDIA_TABLE *) &AssetTableData->TableData[TableOffset];
  MediaEntryPtr = &MediaHeaderPtr->Data[0];

  ///
  /// 1. Detect connected ATA and NVMe devices using latest corresponding protocols.
  ///    The purpose is to detect devices in RAID configuration, which cannot be detected
  ///    using Block I/O.
  ///    Add ATA and NVMe devices using previous versions of protocols will be detected later on.
  /// 2. For other media devices use Block I/O to find them
  ///
  DeviceCount = DetectAtaDevices (MediaEntryPtr);
  MediaEntryPtr += DeviceCount;
  MediaHeaderPtr->StructureCount += DeviceCount;

  DeviceCount = DetectNvmeDevices (MediaEntryPtr);
  MediaEntryPtr += DeviceCount;
  MediaHeaderPtr->StructureCount += DeviceCount;

  DeviceCount = DetectBlockIoDevices (MediaEntryPtr);
  MediaEntryPtr += DeviceCount;
  MediaHeaderPtr->StructureCount += DeviceCount;

  MediaHeaderPtr->TableByteCount = MediaHeaderPtr->StructureCount * sizeof (HWA_MEDIA_ENTRY);
  Length = sizeof (MediaHeaderPtr->TableByteCount) + sizeof (MediaHeaderPtr->StructureCount) + MediaHeaderPtr->TableByteCount;
  return Length;
}

/**
  Get the full size of SMBIOS structure including optional strings that follow the formatted structure.

  @param[in] Head               Pointer to the beginning of SMBIOS structure.

  @return Size                  The returned size.
**/
UINTN
GetSmbiosStructureSize (
  IN   SMBIOS_STRUCTURE *Head
  )
{
  UINTN  Size;
  UINTN  StrLen;
  CHAR8  *CharInStr;

  CharInStr = (CHAR8*)Head + Head->Length;
  Size = Head->Length;
  StrLen = 0;
  //
  // look for the two consecutive zeros, check the string limit by the way.
  //
  while (*CharInStr != 0 || *(CharInStr+1) != 0) {
    if (*CharInStr == 0) {
      Size += 1;
      CharInStr++;
    }

    for (StrLen = 0 ; StrLen < SMBIOS_3_0_TABLE_MAX_LENGTH; StrLen++) {
      if (*(CharInStr+StrLen) == 0) {
        break;
      }
    }

    if (StrLen == SMBIOS_3_0_TABLE_MAX_LENGTH) {
      return 0;
    }

    //
    // forward the pointer
    //
    CharInStr += StrLen;
    Size += StrLen;
  }

  //
  // count ending two zeros.
  //
  Size += 2;

  return Size;
}

/**
  Update ME SMBIOS Type 130 content.
  Some contents are not up-to-date in SMBIOS 130 in the very first boot after reflashing.
  This function is to update AmtEnabled and SolEnabled status before pushing it to CSME.

  @param[in] SmbiosHeaderPtr       Pointer to the header of SMBIOS structure.

**/
VOID
UpdateType130Content (
  IN SMBIOS_STRUCTURE         *SmbiosHeaderPtr
  )
{
  EFI_STATUS                 Status;
  VOID                       *ProtocolPointer;
  SMBIOS_TABLE_TYPE130       *SmbiosTableType130Data;
  MEBX_CONFIG_PROTOCOL       *MebxConfigProtocol;
  UINTN                       MebxDataSize;
  MEBX_DATA                   MebxData;

  DEBUG ((DEBUG_INFO, "%a() enter\n", __FUNCTION__));

  ZeroMem (&MebxData, sizeof (MEBX_DATA));

  Status = gBS->LocateProtocol (&gUpdateSmbiosHwAssetTableGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = gBS->LocateProtocol (&gMebxConfigProtocolGuid, NULL, (VOID**) &MebxConfigProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Locate gMebxConfigProtocolGuid Status = %r\n", Status));
    return;
  }

  MebxDataSize = sizeof (MEBX_DATA);
  Status = MebxConfigProtocol->GetMebxConfig (&MebxDataSize, (VOID*) &MebxData);
  if (EFI_ERROR (Status)) {
    return;
  }

  SmbiosTableType130Data = (SMBIOS_TABLE_TYPE130*) SmbiosHeaderPtr;
  if (SmbiosTableType130Data->AmtSupported) {
    SmbiosTableType130Data->SolEnabled  = MebxData.AmtSol;
    SmbiosTableType130Data->SrouEnabled = MebxData.AmtSrou;
  }
}

/**
  Create the SMBIOS table to send to AMT FW

  @param[in] AssetTableData   Buffer of all Asset tables to send to FW
  @param[in] TableOffset      Offset to the beginning of HWA SMBIOS Table

  @return    Size             Size of the HWA Asf Table
**/
UINT16
BuildHwaSmbiosTable (
  IN HWA_TABLE             *AssetTableData,
  IN UINT16                TableOffset
  )
{
  EFI_STATUS               Status;
  VOID                     *SmbiosTablePointer;
  SMBIOS_TABLE_ENTRY_POINT SmbiosEntryPointTable;
  SMBIOS_STRUCTURE         *SmbiosHeaderPtr;
  UINTN                    StructureLength;
  BOOLEAN                  IsEndOfTable;
  UINT16                   Length;

  IsEndOfTable = FALSE;

  ///
  /// 1. Locate SMBIOS table entry point structure and verify the anchor string ('_SM_')
  /// 2. Go through all SMBIOS tables and copy the required ones into Asset Table
  ///
  Status = EfiGetSystemConfigurationTable (&gEfiSmbiosTableGuid, (VOID **) &SmbiosTablePointer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Error getting SMBIOS Table GUID %g -> %r\n", gEfiSmbiosTableGuid, Status));
    return 0;
  }
  CopyMem (&SmbiosEntryPointTable, SmbiosTablePointer, sizeof (SMBIOS_TABLE_ENTRY_POINT));

  if (CompareMem (&SmbiosEntryPointTable.AnchorString, "_SM_", 4) == 1) {
    DEBUG ((DEBUG_WARN, "CalulateSmbiosTable Error:  Not SMBIOS Signature\n"));
    return 0;
  }

  if (SmbiosEntryPointTable.TableLength == 0) {
    DEBUG ((DEBUG_WARN, "CalulateSmbiosTable Error: SMBIOS Table Size == 0\n"));
    return 0;
  } else if (SmbiosEntryPointTable.TableLength > (MAX_ASSET_TABLE_ALLOCATED_SIZE - TableOffset)) {
    DEBUG ((DEBUG_WARN, "SMBIOS Tables Are Larger Than 0x%x\n", (MAX_ASSET_TABLE_ALLOCATED_SIZE - TableOffset)));
    return 0;
  }

  SmbiosHeaderPtr = (SMBIOS_STRUCTURE*)(UINTN)(SmbiosEntryPointTable.TableAddress);

  while (!IsEndOfTable) {
    StructureLength = GetSmbiosStructureSize (SmbiosHeaderPtr);
    switch (SmbiosHeaderPtr->Type) {
      case 13:
      case 15:
      case 25:
      case 32:
        ///
        /// Not needed by AMT - forward the SMBIOS table pointer but don't copy the data
        ///
        SmbiosHeaderPtr = (SMBIOS_STRUCTURE*)((UINT8*)SmbiosHeaderPtr + StructureLength);
        continue;
        break;

      case 127:
        IsEndOfTable = TRUE;
        break;

      case MESMBIOS_TYPE_130:
        UpdateType130Content (SmbiosHeaderPtr);
      default:
        break;
    }
    //
    // Copy the data and forward the pointers
    //
    CopyMem (&AssetTableData->TableData[TableOffset], (VOID *)(UINTN)SmbiosHeaderPtr, StructureLength);
    TableOffset += (UINT16)StructureLength;
    SmbiosHeaderPtr = (SMBIOS_STRUCTURE*)((UINT8*)SmbiosHeaderPtr + StructureLength);
  }

  Length = TableOffset - AssetTableData->Table[HWA_TABLE_TYPE_INDEX_SMBIOS].Offset;
  return Length;
}

/**
  Create the ASF table to send to AMT FW

  @param[in] AssetTableData   Buffer of all Asset tables to send to FW
  @param[in] TableOffset      Offset to the beginning of HWA ASF Table

  @return    Size             Size of the HWA Asf Table
**/
UINT16
BuildHwaAsfTable (
  IN HWA_TABLE                                 *AssetTableData,
  IN UINT16                                    TableOffset
  )
{
  EFI_STATUS                                   Status;
  EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER *Rsdp;
  EFI_ACPI_DESCRIPTION_HEADER                  *Rsdt;
  EFI_ACPI_DESCRIPTION_HEADER                  *TempAcpiTable;
  UINT32                                       Index;
  UINT32                                       Address;


  ///
  /// 1. Locate RSDP and RSDT.
  /// 2. Locate ASF! Table and copy it into Asset Table
  ///
  /// ACPI 2.0 or newer tables use gEfiAcpiTableGuid.
  ///
  Status = EfiGetSystemConfigurationTable (&gEfiAcpiTableGuid, (VOID *) &Rsdp);
  if (EFI_ERROR (Status) || (Rsdp == NULL)) {
    DEBUG ((DEBUG_WARN, "Error getting ACPI Table -> %r\n", Status));
    return 0;
  }

  Rsdt = (EFI_ACPI_DESCRIPTION_HEADER*)(UINTN)Rsdp->RsdtAddress;

  if (Rsdt->Signature != EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    DEBUG ((DEBUG_WARN, "ACPI RSDT Signature Does Not Match\n"));
    return 0;
  }

  for (Index = sizeof (EFI_ACPI_DESCRIPTION_HEADER); Index < Rsdt->Length; Index = Index + sizeof (UINT32)) {
    Address       = *(UINT32*)((UINT8*) Rsdt + Index);
    TempAcpiTable = (EFI_ACPI_DESCRIPTION_HEADER*)(UINTN)Address;
    if (TempAcpiTable->Signature == EFI_ACPI_ASF_DESCRIPTION_TABLE_SIGNATURE) {
      CopyMem (&AssetTableData->TableData[TableOffset], (VOID *) (UINTN) TempAcpiTable, TempAcpiTable->Length);
      return (UINT16)TempAcpiTable->Length;
    }
  }

  return 0;

}

/**
  Constructs each of the lower level asset tables

  @param[in]  AssetTablesData           Buffer of all Asset tables to send to FW
  @param[in]  IsMediaTablePushRequested Determines if CSME has requested Media Table in this boot

  @return     TableSize                 Total Size of the HWA Table
**/
UINT16
BuildHwaTable (
  IN  HWA_TABLE           *AssetTableData,
  IN  BOOLEAN             IsMediaTablePushRequested
  )
{
  UINT16 TableSize;

  TableSize = 0;

  AssetTableData->Table[HWA_TABLE_TYPE_INDEX_FRU_DEVICE].Offset = 0;
  AssetTableData->Table[HWA_TABLE_TYPE_INDEX_FRU_DEVICE].Length = BuildHwaPciFruTable (AssetTableData, TableSize);
  TableSize += AssetTableData->Table[HWA_TABLE_TYPE_INDEX_FRU_DEVICE].Length;
  DEBUG ((DEBUG_INFO, "HWA Table offset after PCI FRU: %d,\n", TableSize));

  ///
  /// Build Media List if Firmware requests it or full BIOS boot path:
  /// If there are no media devices in the system - HWA Media table size is 4 bytes
  ///
  AssetTableData->Table[HWA_TABLE_TYPE_INDEX_MEDIA_DEVICE].Offset = TableSize;
  if (GetBootModeHob () != BOOT_WITH_MINIMAL_CONFIGURATION || IsMediaTablePushRequested) {
    AssetTableData->Table[HWA_TABLE_TYPE_INDEX_MEDIA_DEVICE].Length = BuildHwaMediaTable (AssetTableData, TableSize);
  } else {
    AssetTableData->Table[HWA_TABLE_TYPE_INDEX_MEDIA_DEVICE].Length = sizeof (HWA_MEDIA_TABLE);
  }
  TableSize += AssetTableData->Table[HWA_TABLE_TYPE_INDEX_MEDIA_DEVICE].Length;
  DEBUG ((DEBUG_INFO, "HWA Table offset after Media Table: %d,\n", TableSize));

  AssetTableData->Table[HWA_TABLE_TYPE_INDEX_SMBIOS].Offset = TableSize;
  AssetTableData->Table[HWA_TABLE_TYPE_INDEX_SMBIOS].Length = BuildHwaSmbiosTable (AssetTableData, TableSize);
  TableSize += AssetTableData->Table[HWA_TABLE_TYPE_INDEX_SMBIOS].Length;
  DEBUG ((DEBUG_INFO, "HWA Table offset after SMBIOS Table: %d,\n", TableSize));

  AssetTableData->Table[HWA_TABLE_TYPE_INDEX_ASF].Offset = TableSize;
  AssetTableData->Table[HWA_TABLE_TYPE_INDEX_ASF].Length = BuildHwaAsfTable (AssetTableData, TableSize);
  TableSize += AssetTableData->Table[HWA_TABLE_TYPE_INDEX_ASF].Length;
  DEBUG ((DEBUG_INFO, "Complete HWA Table Size: %d,\n", TableSize));

  return TableSize;
}

