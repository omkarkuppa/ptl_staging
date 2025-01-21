/** @file
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

#include "PciIoLibInternal.h"

/**
  Enable a PCI driver to access PCI controller registers in the PCI memory or I/O space.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory or I/O operations.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for the memory or I/O operation to perform.
  @param  Offset                The offset within the selected BAR to start the memory or I/O operation.
  @param  Count                 The number of memory or I/O operations to perform.
  @param  Buffer                For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.
  @retval EFI_SUCCESS           The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED       BarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
**/
EFI_STATUS
EFIAPI
PciBusMemRead (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT8                    *Uint8Buffer;
  UINT16                   *Uint16Buffer;
  UINT32                   *Uint32Buffer;
  UINT32                   BarValue;
  UINTN                    Index;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = (PCI_DEVICE_PRIVATE_DATA*) This;

  BarValue = PciSegmentRead32 (PrivateData->PciCfgBase + (R_BASE_ADDRESS_OFFSET_0 + (0x4 * BarIndex)));
  BarValue &= 0xFFFFFFF0;

  //
  // Check offset alignment
  //
  if ((Offset & ((1 << (Width & 0x03)) - 1)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Width) {
    case EfiPciIoWidthUint16:
      Uint16Buffer = (UINT16*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint16Buffer[Index] = MmioRead16 (BarValue + (UINT32)Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
    case EfiPciIoWidthUint32:
      Uint32Buffer = (UINT32*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint32Buffer[Index] = MmioRead32 (BarValue + (UINT32)Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
    case EfiPciIoWidthUint8:
    default:
      Uint8Buffer = (UINT8*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint8Buffer[Index] = MmioRead8 (BarValue + (UINT32)Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
  }
  return EFI_SUCCESS;
}

/**
  Enable a PCI driver to access PCI controller registers in the PCI memory or I/O space.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory or I/O operations.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for the memory or I/O operation to perform.
  @param  Offset                The offset within the selected BAR to start the memory or I/O operation.
  @param  Count                 The number of memory or I/O operations to perform.
  @param  Buffer                For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.
  @retval EFI_SUCCESS           The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED       BarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
**/
EFI_STATUS
EFIAPI
PciBusMemWrite (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT8                    *Uint8Buffer;
  UINT16                   *Uint16Buffer;
  UINT32                   *Uint32Buffer;
  UINT32                   BarValue;
  UINTN                    Index;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = (PCI_DEVICE_PRIVATE_DATA*) This;

  BarValue = PciSegmentRead32 (PrivateData->PciCfgBase + (R_BASE_ADDRESS_OFFSET_0 + (0x4 * BarIndex)));
  BarValue &= 0xFFFFFFF0;

  //
  // Check offset alignment
  //
  if ((Offset & ((1 << (Width & 0x03)) - 1)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Width) {
    case EfiPciIoWidthUint16:
      Uint16Buffer = (UINT16*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        MmioWrite16 (BarValue + (UINT32)Offset + (Index * (UINTN)(1 << Width)), Uint16Buffer[Index]);
      }
      break;
    case EfiPciIoWidthUint32:
      Uint32Buffer = (UINT32*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        MmioWrite32 (BarValue + (UINT32)Offset + (Index * (UINTN)(1 << Width)), Uint32Buffer[Index]);
      }
      break;
    case EfiPciIoWidthUint8:
    default:
      Uint8Buffer = (UINT8*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        MmioWrite8 (BarValue + (UINT32)Offset + (Index * (UINTN)(1 << Width)), Uint8Buffer[Index]);
      }
      break;
  }
  return EFI_SUCCESS;
}

/**
  Enable a PCI driver to access PCI controller registers in the PCI memory or I/O space.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory or I/O operations.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for the memory or I/O operation to perform.
  @param  Offset                The offset within the selected BAR to start the memory or I/O operation.
  @param  Count                 The number of memory or I/O operations to perform.
  @param  Buffer                For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.
  @retval EFI_SUCCESS           The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED       BarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
**/
EFI_STATUS
EFIAPI
PciBusIoRead (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT8                    *Uint8Buffer;
  UINT16                   *Uint16Buffer;
  UINT32                   *Uint32Buffer;
  UINT32                   BarValue;
  UINTN                    Index;

  if (This == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = (PCI_DEVICE_PRIVATE_DATA*) This;

  BarValue = PciSegmentRead32 (PrivateData->PciCfgBase + (R_BASE_ADDRESS_OFFSET_0 + (0x4 * BarIndex)));
  BarValue &= 0xFFFFFFFC;

  //
  // Check offset alignment
  //
  if ((Offset & ((1 << (Width & 0x03)) - 1)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Width) {
    case EfiPciIoWidthUint16:
      Uint16Buffer = (UINT16*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint16Buffer[Index] = IoRead16 (BarValue + (UINT32)Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
    case EfiPciIoWidthUint32:
      Uint32Buffer = (UINT32*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint32Buffer[Index] = IoRead32 (BarValue + (UINT32)Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
    case EfiPciIoWidthUint8:
    default:
      Uint8Buffer = (UINT8*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint8Buffer[Index] = IoRead8 (BarValue + (UINT32)Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
  }
  return EFI_SUCCESS;
}

/**
  Enable a PCI driver to access PCI controller registers in the PCI memory or I/O space.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory or I/O operations.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for the memory or I/O operation to perform.
  @param  Offset                The offset within the selected BAR to start the memory or I/O operation.
  @param  Count                 The number of memory or I/O operations to perform.
  @param  Buffer                For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.
  @retval EFI_SUCCESS           The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED       BarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
**/
EFI_STATUS
EFIAPI
PciBusIoWrite (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT8                    *Uint8Buffer;
  UINT16                   *Uint16Buffer;
  UINT32                   *Uint32Buffer;
  UINT32                   BarValue;
  UINTN                    Index;

  if (This == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = (PCI_DEVICE_PRIVATE_DATA*) This;

  BarValue = PciSegmentRead32 (PrivateData->PciCfgBase + (R_BASE_ADDRESS_OFFSET_0 + (0x4 * BarIndex)));
  BarValue &= 0xFFFFFFFC;

  //
  // Check offset alignment
  //
  if ((Offset & ((1 << (Width & 0x03)) - 1)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Width) {
    case EfiPciIoWidthUint16:
      Uint16Buffer = (UINT16*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        IoWrite16 (BarValue + (UINT32)Offset + (Index * (UINTN)(1 << Width)), Uint16Buffer[Index]);
      }
      break;
    case EfiPciIoWidthUint32:
      Uint32Buffer = (UINT32*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        IoWrite32 (BarValue + (UINT32)Offset + (Index * (UINTN)(1 << Width)), Uint32Buffer[Index]);
      }
      break;
    case EfiPciIoWidthUint8:
    default:
      Uint8Buffer = (UINT8*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        IoWrite8 (BarValue + (UINT32)Offset + (Index * (UINTN)(1 << Width)), Uint8Buffer[Index]);
      }
      break;
  }
  return EFI_SUCCESS;
}

/**
  Reads from the memory space of a PCI controller. Returns either when the polling exit criteria is
  satisfied or after a defined duration.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory or I/O operations.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for the memory operation to perform.
  @param  Offset                The offset within the selected BAR to start the memory operation.
  @param  Mask                  Mask used for the polling criteria.
  @param  Value                 The comparison value used for the polling exit criteria.
  @param  Delay                 The number of 100 ns units to poll.
  @param  Result                Pointer to the last value read from the memory location.
  @retval EFI_SUCCESS           The last data returned from the access matched the poll exit criteria.
  @retval EFI_UNSUPPORTED       BarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       Offset is not valid for the BarIndex of this PCI controller.
  @retval EFI_TIMEOUT           Delay expired before a match occurred.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
**/
EFI_STATUS
EFIAPI
PciBusPollMem (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN  UINT8                        BarIndex,
  IN  UINT64                       Offset,
  IN  UINT64                       Mask,
  IN  UINT64                       Value,
  IN  UINT64                       Delay,
  OUT UINT64                       *Result
  )
{
  EFI_STATUS Status;

  if (This == NULL || Result == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = This->Mem.Read (This, Width, BarIndex, Offset, 1, Result);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (((*Result & Mask) == Value) || (Delay == 0)) {
    return EFI_SUCCESS;
  }

  do {
    MicroSecondDelay (10);
    Status = This->Mem.Read (This, Width, BarIndex, Offset, 1, Result);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if ((*Result & Mask) == Value) {
      return EFI_SUCCESS;
    }

    if (Delay <= 100) {
      return EFI_TIMEOUT;
    }
    Delay -= 100;
  } while (TRUE);
}

/**
  Reads from the I/O space of a PCI controller. Returns either when the polling exit criteria is
  satisfied or after a defined duration.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory or I/O operations.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for the memory operation to perform.
  @param  Offset                The offset within the selected BAR to start the memory operation.
  @param  Mask                  Mask used for the polling criteria.
  @param  Value                 The comparison value used for the polling exit criteria.
  @param  Delay                 The number of 100 ns units to poll.
  @param  Result                Pointer to the last value read from the memory location.
  @retval EFI_SUCCESS           The last data returned from the access matched the poll exit criteria.
  @retval EFI_UNSUPPORTED       BarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       Offset is not valid for the BarIndex of this PCI controller.
  @retval EFI_TIMEOUT           Delay expired before a match occurred.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
**/
EFI_STATUS
EFIAPI
PciBusPollIo (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN  UINT8                        BarIndex,
  IN  UINT64                       Offset,
  IN  UINT64                       Mask,
  IN  UINT64                       Value,
  IN  UINT64                       Delay,
  OUT UINT64                       *Result
  )
{
  EFI_STATUS Status;

  if (This == NULL || Result == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = This->Io.Read (This, Width, BarIndex, Offset, 1, Result);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (((*Result & Mask) == Value) || (Delay == 0)) {
    return EFI_SUCCESS;
  }

  do {
    MicroSecondDelay (10);
    Status = This->Io.Read (This, Width, BarIndex, Offset, 1, Result);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if ((*Result & Mask) == Value) {
      return EFI_SUCCESS;
    }

    if (Delay <= 100) {
      return EFI_TIMEOUT;
    }
    Delay -= 100;
  } while (TRUE);
}

/**
  Enable a PCI driver to access PCI controller registers in PCI configuration space.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory operations.
  @param  Offset                The offset within the PCI configuration space for the PCI controller.
  @param  Count                 The number of PCI configuration operations to perform.
  @param  Buffer                For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.
  @retval EFI_SUCCESS           The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI configuration header of the PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER Buffer is NULL or Width is invalid.
**/
EFI_STATUS
EFIAPI
PciBusConfigRead (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT32                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT8                    *Uint8Buffer;
  UINT16                   *Uint16Buffer;
  UINT32                   *Uint32Buffer;
  UINTN                    Index;

  if (This == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = (PCI_DEVICE_PRIVATE_DATA*) This;

  //
  // Check offset alignment
  //
  if ((Offset & ((1 << (Width & 0x03)) - 1)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Width) {
    case EfiPciIoWidthUint16:
      Uint16Buffer = (UINT16*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint16Buffer[Index] = PciSegmentRead16 (PrivateData->PciCfgBase + Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
    case EfiPciIoWidthUint32:
      Uint32Buffer = (UINT32*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint32Buffer[Index] = PciSegmentRead32 (PrivateData->PciCfgBase + Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
    case EfiPciIoWidthUint8:
    default:
      Uint8Buffer = (UINT8*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint8Buffer[Index] = PciSegmentRead8 (PrivateData->PciCfgBase + Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
  }
  return EFI_SUCCESS;
}

/**
  Enable a PCI driver to access PCI controller registers in PCI configuration space.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory operations.
  @param  Offset                The offset within the PCI configuration space for the PCI controller.
  @param  Count                 The number of PCI configuration operations to perform.
  @param  Buffer                For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.
  @retval EFI_SUCCESS           The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI configuration header of the PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER Buffer is NULL or Width is invalid.
**/
EFI_STATUS
EFIAPI
PciBusConfigWrite (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT32                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT8                    *Uint8Buffer;
  UINT16                   *Uint16Buffer;
  UINT32                   *Uint32Buffer;
  UINTN                    Index;

  if (This == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = (PCI_DEVICE_PRIVATE_DATA*) This;

  //
  // Check offset alignment
  //
  if ((Offset & ((1 << (Width & 0x03)) - 1)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Width) {
    case EfiPciIoWidthUint16:
      Uint16Buffer = (UINT16*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        PciSegmentWrite16 (PrivateData->PciCfgBase + Offset + (Index * (UINTN)(1 << Width)), Uint16Buffer[Index]);
      }
      break;
    case EfiPciIoWidthUint32:
      Uint32Buffer = (UINT32*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        PciSegmentWrite32 (PrivateData->PciCfgBase + Offset + (Index * (UINTN)(1 << Width)), Uint32Buffer[Index]);
      }
      break;
    case EfiPciIoWidthUint8:
    default:
      Uint8Buffer = (UINT8*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        PciSegmentWrite8 (PrivateData->PciCfgBase + Offset + (Index * (UINTN)(1 << Width)), Uint8Buffer[Index]);
      }
      break;
  }
  return EFI_SUCCESS;
}

/**
  Enables a PCI driver to copy one region of PCI memory space to another region of PCI
  memory space.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory operations.
  @param  DestBarIndex          The BAR index in the standard PCI Configuration header to use as the
                                base address for the memory operation to perform.
  @param  DestOffset            The destination offset within the BAR specified by DestBarIndex to
                                start the memory writes for the copy operation.
  @param  SrcBarIndex           The BAR index in the standard PCI Configuration header to use as the
                                base address for the memory operation to perform.
  @param  SrcOffset             The source offset within the BAR specified by SrcBarIndex to start
                                the memory reads for the copy operation.
  @param  Count                 The number of memory operations to perform. Bytes moved is Width
                                size * Count, starting at DestOffset and SrcOffset.
  @retval EFI_SUCCESS           The data was copied from one memory region to another memory region.
  @retval EFI_UNSUPPORTED       DestBarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       SrcBarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by DestOffset, Width, and Count
                                is not valid for the PCI BAR specified by DestBarIndex.
  @retval EFI_UNSUPPORTED       The address range specified by SrcOffset, Width, and Count is
                                not valid for the PCI BAR specified by SrcBarIndex.
  @retval EFI_INVALID_PARAMETER Width is invalid.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
**/
EFI_STATUS
EFIAPI
PciBusCopyMem (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        DestBarIndex,
  IN     UINT64                       DestOffset,
  IN     UINT8                        SrcBarIndex,
  IN     UINT64                       SrcOffset,
  IN     UINTN                        Count
  )
{
  EFI_STATUS  Status;
  UINTN       Index;
  UINTN       Stride;
  UINT64      Result;
  BOOLEAN     Forward;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Stride = (UINTN)(1 << Width);

  Forward = TRUE;
  if ((DestOffset > SrcOffset) &&
      (DestOffset < (SrcOffset + Count * Stride)))
  {
    Forward     = FALSE;
    SrcOffset  = SrcOffset + (Count - 1) * Stride;
    DestOffset = DestOffset + (Count - 1) * Stride;
  }

  for (Index = 0; Index < Count; Index++) {
    Status = This->Mem.Read (This, Width, SrcBarIndex, SrcOffset, 1, &Result);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = This->Mem.Write (This, Width, DestBarIndex, DestOffset, 1, &Result);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if (Forward) {
      SrcOffset  += Stride;
      DestOffset += Stride;
    } else {
      SrcOffset  -= Stride;
      DestOffset -= Stride;
    }
  }
  return EFI_SUCCESS;
}

/**
  Flushes all PCI posted write transactions from a PCI host bridge to system memory.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @retval EFI_SUCCESS           The PCI posted write transactions were flushed from the PCI host
                                bridge to system memory.
  @retval EFI_DEVICE_ERROR      The PCI posted write transactions were not flushed from the PCI
                                host bridge due to a hardware error.
**/
EFI_STATUS
EFIAPI
PciBusFlush (
  IN EFI_PCI_IO_PROTOCOL  *This
  )
{
  return EFI_SUCCESS;
}

/**
  Retrieves this PCI controller's current PCI bus number, device number, and function number.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  SegmentNumber         The PCI controller's current PCI segment number.
  @param  BusNumber             The PCI controller's current PCI bus number.
  @param  DeviceNumber          The PCI controller's current PCI device number.
  @param  FunctionNumber        The PCI controller's current PCI function number.
  @retval EFI_SUCCESS           The PCI controller location was returned.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
**/
EFI_STATUS
EFIAPI
PciBusGetLocation (
  IN EFI_PCI_IO_PROTOCOL          *This,
  OUT UINTN                       *SegmentNumber,
  OUT UINTN                       *BusNumber,
  OUT UINTN                       *DeviceNumber,
  OUT UINTN                       *FunctionNumber
  )
{
  PCI_DEVICE_PRIVATE_DATA  *PrivateData;

  if (This == NULL ||
      SegmentNumber == NULL ||
      BusNumber == NULL ||
      DeviceNumber == NULL ||
      FunctionNumber == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = (PCI_DEVICE_PRIVATE_DATA*) This;

  *SegmentNumber = PrivateData->Sbdf.Seg;
  *BusNumber = PrivateData->Sbdf.Bus;
  *DeviceNumber = PrivateData->Sbdf.Dev;
  *FunctionNumber = PrivateData->Sbdf.Func;
  return EFI_SUCCESS;
}

/**
  Performs an operation on the attributes that this PCI controller supports. The operations include
  getting the set of supported attributes, retrieving the current attributes, setting the current
  attributes, enabling attributes, and disabling attributes.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Operation             The operation to perform on the attributes for this PCI controller.
  @param  Attributes            The mask of attributes that are used for Set, Enable, and Disable
                                operations.
  @param  Result                A pointer to the result mask of attributes that are returned for the Get
                                and Supported operations.
  @retval EFI_SUCCESS           The operation on the PCI controller's attributes was completed.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_UNSUPPORTED       one or more of the bits set in
                                Attributes are not supported by this PCI controller or one of
                                its parent bridges when Operation is Set, Enable or Disable.
**/
EFI_STATUS
EFIAPI
PciBusAttributes (
  IN EFI_PCI_IO_PROTOCOL                       *This,
  IN  EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION  Operation,
  IN  UINT64                                   Attributes,
  OUT UINT64                                   *Result OPTIONAL
  )
{
  PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT16  Command;
  UINT64  Supports;
  UINT64  UpStreamAttributes;
  UINT16  CurrentCommand;
  EFI_STATUS Status;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = (PCI_DEVICE_PRIVATE_DATA*) This;
  Command = 0;

  switch (Operation) {
    case EfiPciIoAttributeOperationGet:
      if (Result == NULL) {
        return EFI_INVALID_PARAMETER;
      }

      *Result = PrivateData->Attributes;
      return EFI_SUCCESS;

    case EfiPciIoAttributeOperationSupported:
      if (Result == NULL) {
        return EFI_INVALID_PARAMETER;
      }

      *Result = PrivateData->Supports;
      return EFI_SUCCESS;

    case EfiPciIoAttributeOperationSet:
      Status = PrivateData->PciDevice.PciIo.Attributes (
                                               &(PrivateData->PciDevice.PciIo),
                                               EfiPciIoAttributeOperationEnable,
                                               Attributes,
                                               NULL
                                               );
      if (EFI_ERROR (Status)) {
        return EFI_UNSUPPORTED;
      }

      Status = PrivateData->PciDevice.PciIo.Attributes (
                                               &(PrivateData->PciDevice.PciIo),
                                               EfiPciIoAttributeOperationDisable,
                                               (~Attributes) & (PrivateData->Supports),
                                               NULL
                                               );
      if (EFI_ERROR (Status)) {
        return EFI_UNSUPPORTED;
      }

      return EFI_SUCCESS;

    case EfiPciIoAttributeOperationEnable:
    case EfiPciIoAttributeOperationDisable:
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  if ((Attributes & EFI_PCI_DEVICE_ENABLE) == EFI_PCI_DEVICE_ENABLE) {
    Attributes &= (PrivateData->Supports);
  }

  if ((Attributes & (EFI_PCI_IO_ATTRIBUTE_VGA_IO | EFI_PCI_IO_ATTRIBUTE_VGA_PALETTE_IO)) != 0) {
    if ((Attributes & (EFI_PCI_IO_ATTRIBUTE_VGA_IO_16 | EFI_PCI_IO_ATTRIBUTE_VGA_PALETTE_IO_16)) != 0) {
      return EFI_UNSUPPORTED;
    }
  }

  Supports = (PrivateData->Supports) & Attributes;
  if (Supports != Attributes) {
    return EFI_UNSUPPORTED;
  }

  if ((Attributes & EFI_PCI_IO_ATTRIBUTE_IO) != 0) {
    Command |= EFI_PCI_COMMAND_IO_SPACE;
  }

  if ((Attributes & EFI_PCI_IO_ATTRIBUTE_MEMORY) != 0) {
    Command |= EFI_PCI_COMMAND_MEMORY_SPACE;
  }

  if ((Attributes & EFI_PCI_IO_ATTRIBUTE_BUS_MASTER) != 0) {
    Command |= EFI_PCI_COMMAND_BUS_MASTER;
  }

  UpStreamAttributes = Attributes &
                       (~(EFI_PCI_IO_ATTRIBUTE_IO     |
                          EFI_PCI_IO_ATTRIBUTE_MEMORY |
                          EFI_PCI_IO_ATTRIBUTE_BUS_MASTER
                          )
                       );

  PrivateData->PciDevice.PciIo.Pci.Read (This, EfiPciIoWidthUint16, PCI_COMMAND_OFFSET, 1, &CurrentCommand);
  if (Operation == EfiPciIoAttributeOperationEnable) {
    CurrentCommand |= Command;
    Status = PrivateData->PciDevice.PciIo.Pci.Write (
                                                &(PrivateData->PciDevice.PciIo),
                                                EfiPciIoWidthUint16,
                                                PCI_COMMAND_OFFSET,
                                                1,
                                                &CurrentCommand
                                                );
    PrivateData->Attributes |= Attributes;
    if (PrivateData->Parent != NULL) {
      Status = PrivateData->Parent->PciDevice.PciIo.Attributes (
                                                      &(PrivateData->Parent->PciDevice.PciIo),
                                                      EfiPciIoAttributeOperationEnable,
                                                      UpStreamAttributes,
                                                      NULL
                                                      );
    }
  } else {
    CurrentCommand &= ~Command;
    Status = PrivateData->PciDevice.PciIo.Pci.Write (
                                                &(PrivateData->PciDevice.PciIo),
                                                EfiPciIoWidthUint16,
                                                PCI_COMMAND_OFFSET,
                                                1,
                                                &CurrentCommand
                                                );
    PrivateData->Attributes &= ~Attributes;
  }
  return Status;
}

/**
  Gets the attributes that this PCI controller supports setting on a BAR using
  SetBarAttributes(), and retrieves the list of resource descriptors for a BAR.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for resource range. The legal range for this field is 0..5.
  @param  Supports              A pointer to the mask of attributes that this PCI controller supports
                                setting for this BAR with SetBarAttributes().
  @param  Resources             A pointer to the resource descriptors that describe the current
                                configuration of this BAR of the PCI controller.
  @retval EFI_SUCCESS           If Supports is not NULL, then the attributes that the PCI
                                controller supports are returned in Supports. If Resources
                                is not NULL, then the resource descriptors that the PCI
                                controller is currently using are returned in Resources.
  @retval EFI_INVALID_PARAMETER Both Supports and Attributes are NULL.
  @retval EFI_UNSUPPORTED       BarIndex not valid for this PCI controller.
  @retval EFI_OUT_OF_RESOURCES  There are not enough resources available to allocate
                                Resources.
**/
EFI_STATUS
EFIAPI
PciBusGetBarAtributes (
  IN EFI_PCI_IO_PROTOCOL             *This,
  IN  UINT8                          BarIndex,
  OUT UINT64                         *Supports, OPTIONAL
  OUT VOID                           **Resources OPTIONAL
  )
{
  UINT32 BarValue;
  PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  PCI_BAR_TYPE BarType;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  *Descriptor;
  EFI_ACPI_END_TAG_DESCRIPTOR        *End;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = (PCI_DEVICE_PRIVATE_DATA*) This;
  BarValue = PciSegmentRead32 (PrivateData->PciCfgBase + (R_BASE_ADDRESS_OFFSET_0 + (0x4 * BarIndex)));

  if (Resources != NULL) {
    Descriptor = AllocateZeroPool (sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR));
    if (Descriptor == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    *Resources = Descriptor;

    Descriptor->Desc         = ACPI_ADDRESS_SPACE_DESCRIPTOR;
    Descriptor->Len          = (UINT16)(sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) - 3);

    if (BarValue & 0x1) {
      // IO bar
      BarValue &= 0xFFFFFFFC;
      if ((BarValue & 0xFFFF0000) != 0) {
        //
        // It is a IO32 bar
        //
        BarType   = PciBarTypeIo32;
        Descriptor->AddrLen    = (~(BarValue) + 1);
        Descriptor->AddrRangeMax = Descriptor->AddrLen - 1;
      } else {
        //
        // It is a IO16 bar
        //
        BarType   = PciBarTypeIo16;
        Descriptor->AddrLen    = 0x0000FFFF & (~(BarValue) + 1);
        Descriptor->AddrRangeMax = Descriptor->AddrLen - 1;
      }
    } else {
      BarValue &= 0xFFFFFFF0;
      if ((BarValue & 0x6) == 0) {
        // 32bit BAR
        if (((BarValue & 0x8) != 0)) {
          BarType = PciBarTypePMem32;
        } else {
          BarType = PciBarTypeMem32;
        }
      } else {
        if (((BarValue & 0x8) != 0)) {
          BarType = PciBarTypePMem64;
        } else {
          BarType = PciBarTypeMem64;
        }
      }
      Descriptor->AddrLen    = (~(BarValue) + 1);
      Descriptor->AddrRangeMax = Descriptor->AddrLen - 1;
    }
    Descriptor->AddrRangeMin = BarValue;

    switch (BarType) {
      case PciBarTypeIo16:
      case PciBarTypeIo32:
        //
        // Io
        //
        Descriptor->ResType = ACPI_ADDRESS_SPACE_TYPE_IO;
        break;

      case PciBarTypePMem32:
        //
        // prefetchable
        //
        Descriptor->SpecificFlag = EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_CACHEABLE_PREFETCHABLE;
      //
      // Fall through
      //
      case PciBarTypeMem32:
        //
        // Mem
        //
        Descriptor->ResType = ACPI_ADDRESS_SPACE_TYPE_MEM;
        //
        // 32 bit
        //
        Descriptor->AddrSpaceGranularity = 32;
        break;

      case PciBarTypePMem64:
        //
        // prefetchable
        //
        Descriptor->SpecificFlag = EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_CACHEABLE_PREFETCHABLE;
      //
      // Fall through
      //
      case PciBarTypeMem64:
        //
        // Mem
        //
        Descriptor->ResType = ACPI_ADDRESS_SPACE_TYPE_MEM;
        //
        // 64 bit
        //
        Descriptor->AddrSpaceGranularity = 64;
        break;

      default:
        break;
    }

    //
    // put the checksum
    //
    End           = (EFI_ACPI_END_TAG_DESCRIPTOR *)(Descriptor + 1);
    End->Desc     = ACPI_END_TAG_DESCRIPTOR;
    End->Checksum = 0;
  }
  return EFI_SUCCESS;
}

/**
  Sets the attributes for a range of a BAR on a PCI controller.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Attributes            The mask of attributes to set for the resource range specified by
                                BarIndex, Offset, and Length.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for resource range. The legal range for this field is 0..5.
  @param  Offset                A pointer to the BAR relative base address of the resource range to be
                                modified by the attributes specified by Attributes.
  @param  Length                A pointer to the length of the resource range to be modified by the
                                attributes specified by Attributes.
  @retval EFI_SUCCESS           The set of attributes specified by Attributes for the resource
                                range specified by BarIndex, Offset, and Length were
                                set on the PCI controller, and the actual resource range is returned
                                in Offset and Length.
  @retval EFI_INVALID_PARAMETER Offset or Length is NULL.
  @retval EFI_UNSUPPORTED       BarIndex not valid for this PCI controller.
  @retval EFI_OUT_OF_RESOURCES  There are not enough resources to set the attributes on the
                                resource range specified by BarIndex, Offset, and
                                Length.
**/
EFI_STATUS
EFIAPI
PciBusSetBarAttributes (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     UINT64                       Attributes,
  IN     UINT8                        BarIndex,
  IN OUT UINT64                       *Offset,
  IN OUT UINT64                       *Length
  )
{
  return EFI_SUCCESS;
}

/**
  Creates a EDKII_PCI_DEVICE_PPI from the PCI bus address and
  a parent device path. This function allocetes memory necessary
  for the EDKII_PCI_DEVICE_PPI.

  @param[in]   Segment           PCI Segment
  @param[in]   Bus               PCI bus
  @param[in]   Device            PCI device
  @param[in]   Function          PCI function
  @param[in]   ParentDevicePath  Pointer to the device path of a parent
  @param[out]  PciDevicePpi      On output a pointer to the valid EDKII_PCI_DEVICE_PPI
**/
BOOLEAN
CreatePciDeviceIoFromSbdf (
  IN UINT8                     Segment,
  IN UINT8                     Bus,
  IN UINT8                     Device,
  IN UINT8                     Function,
  IN EFI_DEVICE_PATH_PROTOCOL  *ParentDevicePath,
  OUT EDKII_PCI_DEVICE_PPI     **PciDevicePpi
  )
{
  PCI_DEVICE_PRIVATE_DATA  *Private;

  if (PciDevicePpi == NULL) {
    return FALSE;
  }

  Private = AllocateZeroPool (sizeof (PCI_DEVICE_PRIVATE_DATA));
  if (Private == NULL) {
    return FALSE;
  }

  *PciDevicePpi = &Private->PciDevice;

  (*PciDevicePpi)->PciIo.PollMem = PciBusPollMem;
  (*PciDevicePpi)->PciIo.PollIo = PciBusPollIo;
  (*PciDevicePpi)->PciIo.Mem.Read = PciBusMemRead;
  (*PciDevicePpi)->PciIo.Mem.Write = PciBusMemWrite;
  (*PciDevicePpi)->PciIo.Io.Read = PciBusIoRead;
  (*PciDevicePpi)->PciIo.Io.Write = PciBusIoWrite;
  (*PciDevicePpi)->PciIo.Pci.Read = PciBusConfigRead;
  (*PciDevicePpi)->PciIo.Pci.Write = PciBusConfigWrite;
  (*PciDevicePpi)->PciIo.CopyMem = PciBusCopyMem;
  (*PciDevicePpi)->PciIo.Map = PciBusMap;
  (*PciDevicePpi)->PciIo.Unmap = PciBusUnmap;
  (*PciDevicePpi)->PciIo.AllocateBuffer = PciBusAllocateBuffer;
  (*PciDevicePpi)->PciIo.FreeBuffer = PciBusFreeBuffer;
  (*PciDevicePpi)->PciIo.Flush = PciBusFlush;
  (*PciDevicePpi)->PciIo.GetLocation = PciBusGetLocation;
  (*PciDevicePpi)->PciIo.Attributes = PciBusAttributes;
  (*PciDevicePpi)->PciIo.GetBarAttributes = PciBusGetBarAtributes;
  (*PciDevicePpi)->PciIo.SetBarAttributes = PciBusSetBarAttributes;

  Private = (PCI_DEVICE_PRIVATE_DATA*) *PciDevicePpi;

  Private->PciCfgBase = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, 0);
  Private->Sbdf.Seg = Segment;
  Private->Sbdf.Bus = Bus;
  Private->Sbdf.Dev = Device;
  Private->Sbdf.Func = Function;

  Private->Supports = EFI_PCI_DEVICE_ENABLE;

  return TRUE;
}

/**
  Refreshes EDKII_PCI_DEVICE_PPI function pointers when callback is called

  @param  PciDevicePpi            Pointer to the EDKII_PCI_DEVICE_PPI

  @retval EFI_SUCCESS             Function pointers were refreshed successfully
  @retval EFI_INVALID_PARAMETER   PCI Device PPI is NULL
**/
EFI_STATUS
EFIAPI
RefreshPciDevicePointersPostMem (
  IN EDKII_PCI_DEVICE_PPI     **PciDevicePpi
  )
{
  if (PciDevicePpi == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Currently, post migration to RAM, refreshing function
  // pointers doesn't change addresses in most cases
  // It seems modules are not migrated at all from flash
  // Below code should be enough if migration is working as
  // expected
  //
  (*PciDevicePpi)->PciIo.PollMem = PciBusPollMem;
  (*PciDevicePpi)->PciIo.PollIo = PciBusPollIo;
  (*PciDevicePpi)->PciIo.Mem.Read = PciBusMemRead;
  (*PciDevicePpi)->PciIo.Mem.Write = PciBusMemWrite;
  (*PciDevicePpi)->PciIo.Io.Read = PciBusIoRead;
  (*PciDevicePpi)->PciIo.Io.Write = PciBusIoWrite;
  (*PciDevicePpi)->PciIo.Pci.Read = PciBusConfigRead;
  (*PciDevicePpi)->PciIo.Pci.Write = PciBusConfigWrite;
  (*PciDevicePpi)->PciIo.CopyMem = PciBusCopyMem;
  (*PciDevicePpi)->PciIo.Map = PciBusMap;
  (*PciDevicePpi)->PciIo.Unmap = PciBusUnmap;
  (*PciDevicePpi)->PciIo.AllocateBuffer = PciBusAllocateBuffer;
  (*PciDevicePpi)->PciIo.FreeBuffer = PciBusFreeBuffer;
  (*PciDevicePpi)->PciIo.Flush = PciBusFlush;
  (*PciDevicePpi)->PciIo.GetLocation = PciBusGetLocation;
  (*PciDevicePpi)->PciIo.Attributes = PciBusAttributes;
  (*PciDevicePpi)->PciIo.GetBarAttributes = PciBusGetBarAtributes;
  (*PciDevicePpi)->PciIo.SetBarAttributes = PciBusSetBarAttributes;

  return EFI_SUCCESS;
}