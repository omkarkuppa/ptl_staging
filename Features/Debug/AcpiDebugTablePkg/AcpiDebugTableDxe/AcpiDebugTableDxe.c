/** @file
  Install ACPI Debug Table

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include "AcpiDebugTableDxe.h"

/**
  Install Debug Port ACPI Table

  @param[in] OsDebugPort                    Parameter to tell which debug port is used by the OS
  @param[in] SerialIoDebugUartPowerGating   If TRUE, Update Serial Io Power Gating
**/
STATIC
VOID
InstallDebugPortAcpiTable (
  IN  UINT8                                 OsDebugPort,
  IN  BOOLEAN                               SerialIoDebugUartPowerGating
  )
{
  BOOLEAN                                       HasDebugPort;
  UINTN                                         AcpiTableKey;
  EFI_ACPI_TABLE_PROTOCOL                       *AcpiTable;
  EFI_ACPI_DEBUG_PORT_DESCRIPTION_TABLE         *DebugPortTable;
  EFI_ACPI_DEBUG_PORT_2_DESCRIPTION_TABLE       *DebugPortTable2;
  EFI_ACPI_DBG2_DEBUG_DEVICE_INFORMATION_STRUCT *Dbg2DeviceTablePtr;
  EFI_ACPI_6_5_GENERIC_ADDRESS_STRUCTURE        *Dbg2DeviceGasPtr;
  UINT32                                        *Dbg2DeviceAdrSizePtr;
  UINT8                                         *Dbg2DeviceNameSpacePtr;
  CHAR8                                         *Dbg2DeviceNameSpaceStr;
  UINT16                                        Dbg2DeviceNameSpaceStrSize;
  UINT32                                        DebugPortTable2Size;
  UINT32                                        Dbg2DevNum;
  UINT16                                        DebugDevInfoStructSize;
  EFI_STATUS                                    Status;

  DebugPortTable    = NULL;
  AcpiTableKey      = 0;

  //
  // Locate ACPI support protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  ASSERT_EFI_ERROR (Status);

  //
  // Allocate Memory for Debug Port Table
  //
  DebugPortTable = AllocateZeroPool (sizeof(EFI_ACPI_DEBUG_PORT_DESCRIPTION_TABLE));
  ASSERT (DebugPortTable != NULL);

  if (DebugPortTable == NULL) return;

  DebugPortTable->Header.Signature       = EFI_ACPI_6_5_DEBUG_PORT_TABLE_SIGNATURE;
  DebugPortTable->Header.Length          = sizeof(EFI_ACPI_DEBUG_PORT_DESCRIPTION_TABLE);
  DebugPortTable->Header.Revision        = EFI_ACPI_DEBUG_PORT_TABLE_REVISION;
  DebugPortTable->Header.Checksum        = 0x00;
  CopyMem (DebugPortTable->Header.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (DebugPortTable->Header.OemId));
  DebugPortTable->Header.OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  DebugPortTable->Header.OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  DebugPortTable->Header.CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  DebugPortTable->Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);
  DebugPortTable->InterfaceType          = EFI_ACPI_DBGP_INTERFACE_TYPE_FULL_16550;

  //
  // Initialize the number of device for DBG2
  //
  Dbg2DevNum = 1;

  //
  // Initialize the DBG2 debug device's name space string and size
  //
  switch (OsDebugPort) {
    case 1:
      Dbg2DeviceNameSpaceStr = ACPI_DBG2_SERIALIO_UART0_NAME_SPACE;
      break;
    case 2:
      Dbg2DeviceNameSpaceStr = ACPI_DBG2_SERIALIO_UART1_NAME_SPACE;
      break;
    case 3:
      Dbg2DeviceNameSpaceStr = ACPI_DBG2_SERIALIO_UART2_NAME_SPACE;
      break;
    default:
      Dbg2DeviceNameSpaceStr = ACPI_DBG2_DEFAULT_NAME_SPACE;
      break;
  }
  Dbg2DeviceNameSpaceStrSize = (UINT16) AsciiStrSize(Dbg2DeviceNameSpaceStr);

  //
  // Initialize the DBG2 total size
  //
  DebugDevInfoStructSize = (UINT16) ((sizeof(EFI_ACPI_DBG2_DEBUG_DEVICE_INFORMATION_STRUCT) +       // Debug Device Information Structure
                                     sizeof(EFI_ACPI_6_5_GENERIC_ADDRESS_STRUCTURE) +               // Base Address Register - Assume 1 BAR
                                     4 +                                                            // Address Size
                                     Dbg2DeviceNameSpaceStrSize +                                   // Name Space String
                                     0) *                                                           // Oem Data - Assume none
                                     Dbg2DevNum);
  DebugPortTable2Size = sizeof (EFI_ACPI_DEBUG_PORT_2_DESCRIPTION_TABLE) + DebugDevInfoStructSize;  // DBG2 table size

  //
  // Allocate memory for DBG2
  //
  DebugPortTable2 = AllocatePool (DebugPortTable2Size);
  ASSERT (DebugPortTable2 != NULL);

  if (DebugPortTable2 == NULL) {
    FreePool (DebugPortTable);
    return;
  }

  //
  // Populate data for DBG2
  //
  DebugPortTable2->Header.Signature       = EFI_ACPI_6_5_DEBUG_PORT_2_TABLE_SIGNATURE;
  DebugPortTable2->Header.Length          = DebugPortTable2Size;
  DebugPortTable2->Header.Revision        = EFI_ACPI_DEBUG_PORT_2_TABLE_REVISION;
  DebugPortTable2->Header.Checksum        = 0x00;
  CopyMem (DebugPortTable2->Header.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (DebugPortTable2->Header.OemId));
  DebugPortTable2->Header.OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  DebugPortTable2->Header.OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  DebugPortTable2->Header.CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  DebugPortTable2->Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);
  DebugPortTable2->OffsetDbgDeviceInfo    = sizeof(EFI_ACPI_DEBUG_PORT_2_DESCRIPTION_TABLE);
  DebugPortTable2->NumberDbgDeviceInfo    = Dbg2DevNum;

  //
  // Set pointer to DBG2 Device Info Structure Offset
  //
  Dbg2DeviceTablePtr = (EFI_ACPI_DBG2_DEBUG_DEVICE_INFORMATION_STRUCT *) ((UINT8 *)DebugPortTable2 + DebugPortTable2->OffsetDbgDeviceInfo);

  //
  // Populate DBG2 Device Info Structure
  //
  Dbg2DeviceTablePtr[0].Revision                        = 0x00;
  Dbg2DeviceTablePtr[0].Length                          = DebugDevInfoStructSize;
  Dbg2DeviceTablePtr[0].NumberofGenericAddressRegisters = 1;
  Dbg2DeviceTablePtr[0].NameSpaceStringLength           = Dbg2DeviceNameSpaceStrSize;
  Dbg2DeviceTablePtr[0].NameSpaceStringOffset           = sizeof (EFI_ACPI_DBG2_DEBUG_DEVICE_INFORMATION_STRUCT) + sizeof (EFI_ACPI_6_5_GENERIC_ADDRESS_STRUCTURE) + 4;
  Dbg2DeviceTablePtr[0].OemDataLength                   = 0;
  Dbg2DeviceTablePtr[0].OemDataOffset                   = 0;
  Dbg2DeviceTablePtr[0].PortType                        = EFI_ACPI_DBG2_PORT_TYPE_SERIAL;
  Dbg2DeviceTablePtr[0].PortSubtype                     = EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_16550_SUBSET_COMPATIBLE_WITH_MS_DBGP_SPEC;
  Dbg2DeviceTablePtr[0].Reserved[0]                     = 0;
  Dbg2DeviceTablePtr[0].Reserved[1]                     = 0;
  Dbg2DeviceTablePtr[0].BaseAddressRegisterOffset       = sizeof (EFI_ACPI_DBG2_DEBUG_DEVICE_INFORMATION_STRUCT);
  Dbg2DeviceTablePtr[0].AddressSizeOffset               = sizeof (EFI_ACPI_DBG2_DEBUG_DEVICE_INFORMATION_STRUCT) + sizeof (EFI_ACPI_6_5_GENERIC_ADDRESS_STRUCTURE);

  //
  // Set pointer to DBG2 Device Base Address Offset
  //
  Dbg2DeviceGasPtr = (EFI_ACPI_6_5_GENERIC_ADDRESS_STRUCTURE *)((UINT8 *)Dbg2DeviceTablePtr + Dbg2DeviceTablePtr[0].BaseAddressRegisterOffset);

  //
  // Set pointer to DBG2 Device Address Size Offset and populate value
  //
  Dbg2DeviceAdrSizePtr = (UINT32 *)((UINT8 *)Dbg2DeviceTablePtr + Dbg2DeviceTablePtr[0].AddressSizeOffset);
  Dbg2DeviceAdrSizePtr[0] = 32;

  //
  // Set pointer to DBG2 Device Name Space Offset and populate value
  //
  Dbg2DeviceNameSpacePtr = (UINT8 *)((UINT8 *)Dbg2DeviceTablePtr + Dbg2DeviceTablePtr[0].NameSpaceStringOffset);
  CopyMem (Dbg2DeviceNameSpacePtr, Dbg2DeviceNameSpaceStr, Dbg2DeviceNameSpaceStrSize);

  //
  // Check which port is used as Debug Port based on policy
  // case 0 : Legacy UART
  // case 1 : Serial IO UART 0
  // case 2 : Serial IO UART 1
  // case 3 : Serial IO UART 2
  //
  HasDebugPort      = FALSE;
  switch (OsDebugPort) {
    case 0:
      DebugPortTable->BaseAddress.AddressSpaceId    = EFI_ACPI_6_5_SYSTEM_IO;
      DebugPortTable->BaseAddress.RegisterBitWidth  = 8;
      DebugPortTable->BaseAddress.RegisterBitOffset = 0;
      DebugPortTable->BaseAddress.Reserved          = 0;
      DebugPortTable->BaseAddress.Address           = PcdGet64 (PcdSerialRegisterBase);

      //
      // Update DBG2 Device Base Address
      //
      Dbg2DeviceGasPtr[0].AddressSpaceId    = EFI_ACPI_6_5_SYSTEM_IO;
      Dbg2DeviceGasPtr[0].RegisterBitWidth  = 8;
      Dbg2DeviceGasPtr[0].RegisterBitOffset = 0;
      Dbg2DeviceGasPtr[0].AccessSize        = EFI_ACPI_6_5_UNDEFINED;
      Dbg2DeviceGasPtr[0].Address           = PcdGet64 (PcdSerialRegisterBase);
      HasDebugPort                          = TRUE;
      break;
    case 1:
    case 2:
    case 3:
      DebugPortTable->BaseAddress.AddressSpaceId    = EFI_ACPI_6_5_SYSTEM_MEMORY;
      DebugPortTable->BaseAddress.RegisterBitWidth  = 8;
      DebugPortTable->BaseAddress.RegisterBitOffset = 0;
      DebugPortTable->BaseAddress.Reserved          = 0;
      DebugPortTable->BaseAddress.Address           = PcdGet64 (PcdAcpiDebugPciCfgBase);

      //
      // Update DBG2 Device Base Address
      //
      Dbg2DeviceGasPtr[0].AddressSpaceId    = EFI_ACPI_6_5_SYSTEM_MEMORY;
      Dbg2DeviceGasPtr[0].RegisterBitWidth  = 8;
      Dbg2DeviceGasPtr[0].RegisterBitOffset = 0;
      Dbg2DeviceGasPtr[0].AccessSize        = EFI_ACPI_6_5_UNDEFINED;
      Dbg2DeviceGasPtr[0].Address           = DebugPortTable->BaseAddress.Address;
      HasDebugPort                          = TRUE;

      //
      // Update Serial Io Power Gating support for Kernel Debug S0iX capability
      //
      if (SerialIoDebugUartPowerGating) {
        Dbg2DeviceAdrSizePtr[0]           = SERIAL_IO_UART_MEM_SIZE;
        Dbg2DeviceTablePtr[0].PortSubtype = EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_INTEL_LPSS;
      }
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  if (HasDebugPort) {
    Status = AcpiTable->InstallAcpiTable (AcpiTable, DebugPortTable, DebugPortTable->Header.Length, &AcpiTableKey);
    ASSERT_EFI_ERROR (Status);

    //
    // Publish DBG2
    //
    Status = AcpiTable->InstallAcpiTable (AcpiTable, DebugPortTable2, DebugPortTable2->Header.Length, &AcpiTableKey);
    ASSERT_EFI_ERROR (Status);
  }

  FreePool (DebugPortTable);
  FreePool (DebugPortTable2);
}

/**
  ACPI debug table driver installation function.

  @param[in] ImageHandle     Handle for this drivers loaded image protocol.
  @param[in] SystemTable     EFI system table.

  @retval EFI_SUCCESS        The driver installed without error.
**/
EFI_STATUS
EFIAPI
InstallDebugAcpiTable (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  //
  // If there is no SerialIo Uart setting to DBG2. EC Uart is used instead.
  //
  InstallDebugPortAcpiTable (PcdGet8 (PcdAcpiDebugOsDebugPort), PcdGet8 (PcdAcpiDebugSerialIoDebugUartPowerGating));

  return EFI_SUCCESS;
}
