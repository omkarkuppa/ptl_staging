/** @file
  Internal file to access Foxville PCI IO.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
//
// Self-Module Foundation.
//
#include "Misc/FoxvillePciIoAccess.h"

/**
  Read a 32-bit data from PCI register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]   BarIndex          The BAR index of the register.
  @param[in]   Offset            The offset within the selected BAR.
  @param[out]  Value             Pointer to the variable that will store the read value.

  @retval  EFI_SUCCESS            Succeed to read the data from assigned PCI register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  Other                  Failed to read the data from assigned PCI register.

**/
EFI_STATUS
ReadPciRegUint32 (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN     UINT8                BarIndex,
  IN     UINT32               Offset,
     OUT UINT32               *Value
  )
{
  EFI_STATUS  Status;

  if ((PciIoProtocolPtr == NULL) || (Value == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  MemoryFence ();

  Status = PciIoProtocolPtr->Mem.Read (
                                   PciIoProtocolPtr,
                                   EfiPciIoWidthUint32,
                                   BarIndex,
                                   Offset,
                                   1,
                                   (VOID *)Value
                                   );

  MemoryFence ();

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read bar %d offset 0x%X - %r\n", BarIndex, Offset, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Write a 32-bit data to PCI register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  BarIndex          The BAR index of the register.
  @param[in]  Offset            The offset within the selected BAR.
  @param[in]  Value             Value that will write into the assigned offset.

  @retval  EFI_SUCCESS            Succeed to write the data into assigned PCI register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  Other                  Failed to write the data into assigned PCI register.

**/
EFI_STATUS
WritePciRegUint32 (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT8                BarIndex,
  IN UINT32               Offset,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MemoryFence ();

  Status = PciIoProtocolPtr->Mem.Write (
                                   PciIoProtocolPtr,
                                   EfiPciIoWidthUint32,
                                   BarIndex,
                                   Offset,
                                   1,
                                   (VOID *)&Value
                                   );

  MemoryFence ();

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to write bar %d offset 0x%X - %r\n", BarIndex, Offset, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Read a 32-bit data from PCI MAC register (BAR0).

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]   Offset            The offset within the selected BAR.
  @param[out]  Value             Pointer to the variable that will store the read value.

  @retval  EFI_SUCCESS            Succeed to read the data from PCI MAC register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  Other                  Failed to read the data from PCI MAC register.

**/
EFI_STATUS
EFIAPI
ReadMacRegUint32 (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN     UINT32               Offset,
     OUT UINT32               *Value
  )
{
  EFI_STATUS  Status;

  if ((PciIoProtocolPtr == NULL) || (Value == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadPciRegUint32 (
             PciIoProtocolPtr,
             MAC_REG_BAR_INDEX,
             Offset,
             Value
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Write a 32-bit data to PCI MAC register (BAR0).

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  BarIndex          The BAR index of the register.
  @param[in]  Offset            The offset within the selected BAR.
  @param[in]  Value             Value that will write into the assigned offset.

  @retval  EFI_SUCCESS            Succeed to write the data into PCI MAC register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  Other                  Failed to write the data into PCI MAC register.

**/
EFI_STATUS
EFIAPI
WriteMacRegUint32 (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Offset,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WritePciRegUint32 (
             PciIoProtocolPtr,
             MAC_REG_BAR_INDEX,
             Offset,
             Value
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Read a 32-bit data from PCI Flash register (BAR1).

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]   Offset            The offset within the selected BAR.
  @param[out]  Value             Pointer to the variable that will store the read value.

  @retval  EFI_SUCCESS            Succeed to read the data from PCI Flash register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  Other                  Failed to read the data from PCI Flash register.

**/
EFI_STATUS
EFIAPI
ReadFlashRegUint32 (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN     UINT32               Offset,
     OUT UINT32               *Value
  )
{
  EFI_STATUS  Status;

  if ((PciIoProtocolPtr == NULL) || (Value == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadPciRegUint32 (
             PciIoProtocolPtr,
             FLASH_REG_BAR_INDEX,
             Offset,
             Value
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}
