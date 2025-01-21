/** @file
  Common IO interface.

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

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>

#include <RegisterAddressTranslation.h>

//
// Declare I/O Ports used to perform PCI Confguration Cycles
//
#define PCI_CONFIGURATION_ADDRESS_PORT  0xCF8
#define PCI_CONFIGURATION_DATA_PORT     0xCFC

#define MEMSS_POSTED_WRITE_MASK         BIT18

/**
  Read data from register via mmio interface.

  @param[in] Topology   Pointer to system topology data
  @param[in] Socket     Socket number
  @param[in] Address    Register address to read from
  @param[in] Size       Size of data to read

  @return Value of data read from given Address
**/
UINT64
EFIAPI
AddressMapMmioRead (
  IN SYSTEM_TOPOLOGY  *Topology,
  IN UINT8            Socket,
  IN UINT64           Address,
  IN UINTN            Size
  )
{
  UINT64    Value;

  switch (Size) {
  case RegisterWidthByte:
    Value = MmioRead8 ((UINTN)Address);
    break;

  case RegisterWidthWord:
    Value = MmioRead16 ((UINTN)Address);
    break;

  case RegisterWidthDWord:
    Value = MmioRead32 ((UINTN)Address);
    break;

  case RegisterWidthQWord:
    Value = MmioRead64 ((UINTN)Address);
    break;

  default:
    ASSERT (FALSE);
    Value = (UINT64)-1;
    break;
  }

  return Value;
}

/**
  Write data to register via mmio interface.

  @param[in] Topology   Pointer to system topology data
  @param[in] Socket     Socket number
  @param[in] Address    Register address to be written
  @param[in] Data       Data to be written to given address
  @param[in] Size       Size of data to write

  @return None
**/
VOID
EFIAPI
AddressMapMmioWrite (
  IN SYSTEM_TOPOLOGY  *Topology,
  IN UINT8            Socket,
  IN UINT64           Address,
  IN UINT64           Data,
  IN UINTN            Size
  )
{
  ASSERT (Topology != NULL);

  if (Topology->PerSocketData[Socket].PostedWrite) {
    Address |= MEMSS_POSTED_WRITE_MASK;
  }

  switch (Size) {
  case RegisterWidthByte:
    MmioWrite8 ((UINTN)Address, (UINT8)Data);
    break;

  case RegisterWidthWord:
    MmioWrite16 ((UINTN)Address, (UINT16)Data);
    break;

  case RegisterWidthDWord:
    MmioWrite32 ((UINTN)Address, (UINT32)Data);
    break;

  case RegisterWidthQWord:
    MmioWrite64 ((UINTN)Address, Data);
    break;

  default:
    ASSERT (FALSE);
    break;
  }

}

/**
  Read data from register via io interface.

  @param[in] Topology   Pointer to system topology data
  @param[in] Socket     Socket number
  @param[in] Port       Port address to read from
  @param[in] Size       Size of data to read

  @return Value of data read from given Address
**/
UINT64
EFIAPI
AddressMapIoRead (
  IN SYSTEM_TOPOLOGY  *Topology,
  IN UINT8            Socket,
  IN UINT64           Port,
  IN UINTN            Size
  )
{
  UINT64    Value;
  BOOLEAN   InterruptState;

  InterruptState = SaveAndDisableInterrupts ();
  IoWrite32 (PCI_CONFIGURATION_ADDRESS_PORT, (UINT32)(Port & 0xFFFFFFFC));

  switch (Size) {
  case RegisterWidthByte:
    Value = IoRead8 (PCI_CONFIGURATION_DATA_PORT + (UINT16)(Port & 3));
    break;

  case RegisterWidthWord:
    Value = IoRead16 (PCI_CONFIGURATION_DATA_PORT + (UINT16)(Port & 2));
    break;

  case RegisterWidthDWord:
    Value = IoRead32 (PCI_CONFIGURATION_DATA_PORT);
    break;

  default:
    ASSERT (FALSE);
    Value = (UINT64)-1;
    break;
  }

  SetInterruptState (InterruptState);

  return Value;
}

/**
  Write data to register via io interface.

  @param[in] Topology   Pointer to system topology data
  @param[in] Socket     Socket number
  @param[in] Port       Port address to be written
  @param[in] Data       Data to be written to given address
  @param[in] Size       Size of data to write

  @return None
**/
VOID
EFIAPI
AddressMapIoWrite (
  IN SYSTEM_TOPOLOGY  *Topology,
  IN UINT8            Socket,
  IN UINT64           Port,
  IN UINT64           Data,
  IN UINTN            Size
  )
{
  BOOLEAN   InterruptState;

  InterruptState = SaveAndDisableInterrupts ();
  IoWrite32 (PCI_CONFIGURATION_ADDRESS_PORT, (UINT32)(Port & 0xFFFFFFFC));

  switch (Size) {
  case RegisterWidthByte:
    IoWrite8 (PCI_CONFIGURATION_DATA_PORT + (UINT16)(Port & 3), (UINT8)Data);
    break;

  case RegisterWidthWord:
    IoWrite16 (PCI_CONFIGURATION_DATA_PORT + (UINT16)(Port & 2), (UINT16)Data);
    break;

  case RegisterWidthDWord:
    IoWrite32 (PCI_CONFIGURATION_DATA_PORT, (UINT32)Data);
    break;

  default:
    ASSERT (FALSE);
    break;
  }

  SetInterruptState (InterruptState);
}


