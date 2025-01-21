/** @file
  Common SMBUS functions between PEI and DXE

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

#ifndef _SMBUS_COMMON_H_
#define _SMBUS_COMMON_H_

#include <Library/DebugLib.h>
#include <Register/SmbusRegs.h>
#include <Library/ConfigBlockLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Uefi/UefiSpec.h>
#include <IndustryStandard/Pci.h>
#include <Library/SmbusLib.h>
#include <Library/TimerLib.h>
#include <ConfigBlock.h>
#include <SmbusConfig.h>
#include <Ppi/Smbus2.h>
#include <Protocol/PciIo.h>
#include <Uefi.h>

///
/// Max number of SMBus devices
/// (7 bit address yields 128 combinations but 21 of those are reserved)
/// Due to limited resources, we only allow 8 in PEI.
///
#define MAX_SMBUS_DEVICES       8

typedef struct {
  UINT8                                DeviceMapEntries;
  EFI_SMBUS_DEVICE_MAP                 DeviceMap[MAX_SMBUS_DEVICES];
  UINT8                                PlatformNumRsvd;
  UINT8                                PlatformRsvdAddr[PCH_MAX_SMBUS_RESERVED_ADDRESS];
  BOOLEAN                              ArpEnable;
} SMBUS_DEVICE_ENUMERATION_DATA;

/**
  This function provides a standard way to read PCH Smbus IO registers.

  @param[in] PciIo                PCI_IO for SMB controller
  @param[in] Offset               Register offset from Smbus base IO address.

  @retval UINT8                   Returns data read from IO.
**/
UINT8
SmbusIoRead (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT8               Offset
  );

/**
  This function provides a standard way to write PCH Smbus IO registers.

  @param[in] PciIo                PCI_IO for SMB controller
  @param[in] Offset               Register offset from Smbus base IO address.
  @param[in] Data                 Data to write to register.

**/
VOID
SmbusIoWrite (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN      UINT8           Offset,
  IN      UINT8           Data
  );

/**
  Do a directed Arp procress to assign the SMBus Device address of a single ARP-capable device.

  @param[in] PciIo                PCI_IO for SMB controller
  @param[in] DeviceEnumData       Pointer to device enumeartion data
  @param[in] SmbusUdid            When doing a directed ARP, ARP the device with this UDID.
  @param[in, out] DeviceAddress   Buffer to store new SMBus Device Address during directed ARP.

  @retval EFI_OUT_OF_RESOURCES    DeviceMapEntries is more than Max number of SMBus devices.
                                  Or there is no available address to assign
  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SmbusDirectedArp (
  IN     EFI_PCI_IO_PROTOCOL            *PciIo,
  IN     SMBUS_DEVICE_ENUMERATION_DATA  *DeviceEnumData,
  IN     EFI_SMBUS_UDID                 *SmbusUdid,
  IN OUT EFI_SMBUS_DEVICE_ADDRESS       *DeviceAddress
  );

/**
  Do a fully (general) Arp procress to assign the SMBus Device address of all ARP-capable device.
  This function will issue issue the "Prepare to ARP", "Get UDID" and "Assign Address" commands.

  @param[in] PciIo                PCI_IO for SMB controller
  @param[in] DeviceEnumData       Pointer to device enumeartion data

  @retval EFI_OUT_OF_RESOURCES    No available address to assign
  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SmbusFullArp (
  IN EFI_PCI_IO_PROTOCOL            *PciIo,
  IN SMBUS_DEVICE_ENUMERATION_DATA  *DeviceEnumData
  );

/**
  Dispatches SMBUS  command based on the input parameters.

  @param[in] PciIo                PCI_IO for SMB controller
  @param[in] DeviceAddress        Smbus device address
  @param[in] Command              Command to be sent
  @param[in] Operation            Which SMBus PPI will be used
  @param[in] PecCheck             Defines if Packet Error Code Checking is to be used
  @param[in, out] Length          How many bytes to read/write. Must be 1 <= Length <= 32 depending on the Operation
  @param[in, out] Buffer          Data buffer

  @return Status of the operation
**/
EFI_STATUS
SmbusDispatchCommand (
  IN EFI_PCI_IO_PROTOCOL     *PciIo,
  IN EFI_SMBUS_DEVICE_ADDRESS DeviceAddress,
  IN EFI_SMBUS_DEVICE_COMMAND Command,
  IN EFI_SMBUS_OPERATION      Operation,
  IN BOOLEAN                  PecCheck,
  IN OUT UINTN                *Length,
  IN OUT VOID                 *Buffer
  );

/**
  Enables host controler to make it ready to process commands.

  @param[in] PciIo  Pointer to a PCI_IO_PROTOCOL

  @retval EFI_SUCCESS  Controller initialized
  @retval Other        Failed to initialize controller
**/
EFI_STATUS
SmbusEnableHc (
  IN EFI_PCI_IO_PROTOCOL  *PciIo
  );

/**
  Checks if PciIo protocol is connected with a supported
  PCI_IO_PROTOCOL.

  @param[in] PciIo  Pointer to a PCI_IO_PROTOCOL to check

  @retval EFI_SUCCESS      Supported SMB device present
  @retval EFI_UNSUPPORTED  Unsupported device
**/
EFI_STATUS
SmbusSupported (
  IN EFI_PCI_IO_PROTOCOL  *PciIo
  );

#endif
