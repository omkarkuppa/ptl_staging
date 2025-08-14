/** @file

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
HiddenPciBusMemRead (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  HIDDEN_PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT8                    *Uint8Buffer;
  UINT16                   *Uint16Buffer;
  UINT32                   *Uint32Buffer;
  UINTN                    Index;

  PrivateData = (HIDDEN_PCI_DEVICE_PRIVATE_DATA*) This;

  //
  // Check offset alignment
  //
  if ((Offset & ((1 << (Width & 0x03)) - 1)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  if (PrivateData->BarAddress[BarIndex] == 0) {
    return EFI_UNSUPPORTED;
  }

  if (PrivateData->BarType[BarIndex] != PciBarTypeMem) {
    return EFI_UNSUPPORTED;
  }

  switch (Width) {
    case EfiPciIoWidthUint16:
      Uint16Buffer = (UINT16*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint16Buffer[Index] = MmioRead16 (PrivateData->BarAddress[BarIndex] + (UINT32)Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
    case EfiPciIoWidthUint32:
      Uint32Buffer = (UINT32*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint32Buffer[Index] = MmioRead32 (PrivateData->BarAddress[BarIndex] + (UINT32)Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
    case EfiPciIoWidthUint8:
    default:
      Uint8Buffer = (UINT8*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint8Buffer[Index] = MmioRead8 (PrivateData->BarAddress[BarIndex] + (UINT32)Offset + (Index * (UINTN)(1 << Width)));
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
HiddenPciBusMemWrite (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  HIDDEN_PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT8                    *Uint8Buffer;
  UINT16                   *Uint16Buffer;
  UINT32                   *Uint32Buffer;
  UINTN                    Index;

  PrivateData = (HIDDEN_PCI_DEVICE_PRIVATE_DATA*) This;

  //
  // Check offset alignment
  //
  if ((Offset & ((1 << (Width & 0x03)) - 1)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  if (PrivateData->BarAddress[BarIndex] == 0) {
    return EFI_UNSUPPORTED;
  }

  if (PrivateData->BarType[BarIndex] != PciBarTypeMem) {
    return EFI_UNSUPPORTED;
  }

  switch (Width) {
    case EfiPciIoWidthUint16:
      Uint16Buffer = (UINT16*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        MmioWrite16 (PrivateData->BarAddress[BarIndex] + (UINT32)Offset + (Index * (UINTN)(1 << Width)), Uint16Buffer[Index]);
      }
      break;
    case EfiPciIoWidthUint32:
      Uint32Buffer = (UINT32*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        MmioWrite32 (PrivateData->BarAddress[BarIndex] + (UINT32)Offset + (Index * (UINTN)(1 << Width)), Uint32Buffer[Index]);
      }
      break;
    case EfiPciIoWidthUint8:
    default:
      Uint8Buffer = (UINT8*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        MmioWrite8 (PrivateData->BarAddress[BarIndex] + (UINT32)Offset + (Index * (UINTN)(1 << Width)), Uint8Buffer[Index]);
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
HiddenPciBusIoRead (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  HIDDEN_PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT8                    *Uint8Buffer;
  UINT16                   *Uint16Buffer;
  UINT32                   *Uint32Buffer;
  UINTN                    Index;

  PrivateData = (HIDDEN_PCI_DEVICE_PRIVATE_DATA*) This;

  //
  // Check offset alignment
  //
  if ((Offset & ((1 << (Width & 0x03)) - 1)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  if (PrivateData->BarAddress[BarIndex] == 0) {
    return EFI_UNSUPPORTED;
  }

  if (PrivateData->BarType[BarIndex] != PciBarTypeIo) {
    return EFI_UNSUPPORTED;
  }

  switch (Width) {
    case EfiPciIoWidthUint16:
      Uint16Buffer = (UINT16*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint16Buffer[Index] = IoRead16 (PrivateData->BarAddress[BarIndex] + (UINT32)Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
    case EfiPciIoWidthUint32:
      Uint32Buffer = (UINT32*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint32Buffer[Index] = IoRead32 (PrivateData->BarAddress[BarIndex] + (UINT32)Offset + (Index * (UINTN)(1 << Width)));
      }
      break;
    case EfiPciIoWidthUint8:
    default:
      Uint8Buffer = (UINT8*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        Uint8Buffer[Index] = IoRead8 (PrivateData->BarAddress[BarIndex] + (UINT32)Offset + (Index * (UINTN)(1 << Width)));
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
HiddenPciBusIoWrite (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  HIDDEN_PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT8                    *Uint8Buffer;
  UINT16                   *Uint16Buffer;
  UINT32                   *Uint32Buffer;
  UINTN                    Index;

  PrivateData = (HIDDEN_PCI_DEVICE_PRIVATE_DATA*) This;

  //
  // Check offset alignment
  //
  if ((Offset & ((1 << (Width & 0x03)) - 1)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  if (PrivateData->BarAddress[BarIndex] == 0) {
    return EFI_UNSUPPORTED;
  }

  if (PrivateData->BarType[BarIndex] != PciBarTypeIo) {
    return EFI_UNSUPPORTED;
  }

  switch (Width) {
    case EfiPciIoWidthUint16:
      Uint16Buffer = (UINT16*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        IoWrite16 (PrivateData->BarAddress[BarIndex] + (UINT32)Offset + (Index * (UINTN)(1 << Width)), Uint16Buffer[Index]);
      }
      break;
    case EfiPciIoWidthUint32:
      Uint32Buffer = (UINT32*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        IoWrite32 (PrivateData->BarAddress[BarIndex] + (UINT32)Offset + (Index * (UINTN)(1 << Width)), Uint32Buffer[Index]);
      }
      break;
    case EfiPciIoWidthUint8:
    default:
      Uint8Buffer = (UINT8*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        IoWrite8 (PrivateData->BarAddress[BarIndex] + (UINT32)Offset + (Index * (UINTN)(1 << Width)), Uint8Buffer[Index]);
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
HiddenPciBusConfigRead (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT32                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  HIDDEN_PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT8                    *Uint8Buffer;
  UINT16                   *Uint16Buffer;
  UINT32                   *Uint32Buffer;
  UINTN                    Index;

  PrivateData = (HIDDEN_PCI_DEVICE_PRIVATE_DATA*) This;

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
        if (PrivateData->PciCfgAddress == 0) {
          Uint16Buffer[Index] = 0xFFFF;
        } else {
          Uint16Buffer[Index] = MmioRead16 (PrivateData->PciCfgAddress + Offset + (Index * (UINTN)(1 << Width)));
        }
      }
      break;
    case EfiPciIoWidthUint32:
      Uint32Buffer = (UINT32*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        if (PrivateData->PciCfgAddress == 0) {
          Uint32Buffer[Index] = 0xFFFFFFFF;
        } else {
          Uint32Buffer[Index] = MmioRead32 (PrivateData->PciCfgAddress + Offset + (Index * (UINTN)(1 << Width)));
        }
      }
      break;
    case EfiPciIoWidthUint8:
    default:
      Uint8Buffer = (UINT8*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        if (PrivateData->PciCfgAddress == 0) {
          Uint8Buffer[Index] = 0xFF;
        } else {
          Uint8Buffer[Index] = MmioRead8 (PrivateData->PciCfgAddress + Offset + (Index * (UINTN)(1 << Width)));
        }
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
HiddenPciBusConfigWrite (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT32                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  HIDDEN_PCI_DEVICE_PRIVATE_DATA  *PrivateData;
  UINT8                    *Uint8Buffer;
  UINT16                   *Uint16Buffer;
  UINT32                   *Uint32Buffer;
  UINTN                    Index;

  PrivateData = (HIDDEN_PCI_DEVICE_PRIVATE_DATA*) This;

  //
  // Check offset alignment
  //
  if ((Offset & ((1 << (Width & 0x03)) - 1)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  if (PrivateData->PciCfgAddress == 0) {
    return EFI_SUCCESS;
  }

  switch (Width) {
    case EfiPciIoWidthUint16:
      Uint16Buffer = (UINT16*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        MmioWrite16 (PrivateData->PciCfgAddress + Offset + (Index * (UINTN)(1 << Width)), Uint16Buffer[Index]);
      }
      break;
    case EfiPciIoWidthUint32:
      Uint32Buffer = (UINT32*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        MmioWrite32 (PrivateData->PciCfgAddress + Offset + (Index * (UINTN)(1 << Width)), Uint32Buffer[Index]);
      }
      break;
    case EfiPciIoWidthUint8:
    default:
      Uint8Buffer = (UINT8*) Buffer;
      for (Index = 0; Index < Count; Index++) {
        MmioWrite8 (PrivateData->PciCfgAddress + Offset + (Index * (UINTN)(1 << Width)), Uint8Buffer[Index]);
      }
      break;
  }
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
HiddenPciBusGetLocation (
  IN EFI_PCI_IO_PROTOCOL          *This,
  OUT UINTN                       *SegmentNumber,
  OUT UINTN                       *BusNumber,
  OUT UINTN                       *DeviceNumber,
  OUT UINTN                       *FunctionNumber
  )
{
  HIDDEN_PCI_DEVICE_PRIVATE_DATA  *PrivateData;

  PrivateData = (HIDDEN_PCI_DEVICE_PRIVATE_DATA *) This;

    if (This == NULL ||
      SegmentNumber == NULL ||
      BusNumber == NULL ||
      DeviceNumber == NULL ||
      FunctionNumber == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  
  *SegmentNumber  = RShiftU64 (PrivateData->PciCfgAddress, 32) & 0xFFFF;
  *BusNumber      = (PrivateData->PciCfgAddress >> 20) & 0xFF;
  *DeviceNumber   = (PrivateData->PciCfgAddress >> 15) & 0x1F;
  *FunctionNumber = (PrivateData->PciCfgAddress >> 12) & 0x7;

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
HiddenPciBusAttributes (
  IN EFI_PCI_IO_PROTOCOL                       *This,
  IN  EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION  Operation,
  IN  UINT64                                   Attributes,
  OUT UINT64                                   *Result OPTIONAL
  )
{
  
  if ((Operation == EfiPciIoAttributeOperationGet || Operation == EfiPciIoAttributeOperationSupported) && Result == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (Operation >= EfiPciIoAttributeOperationMaximum) {
    return EFI_INVALID_PARAMETER;
  }

  if (Operation == EfiPciIoAttributeOperationGet) {
    *Result = 0;
    return EFI_SUCCESS;
  }

  return EFI_UNSUPPORTED;
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
HiddenPciBusGetBarAtributes (
  IN EFI_PCI_IO_PROTOCOL             *This,
  IN  UINT8                          BarIndex,
  OUT UINT64                         *Supports, OPTIONAL
  OUT VOID                           **Resources OPTIONAL
  )
{
  
  if (Supports == NULL || Resources == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_UNSUPPORTED;
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
HiddenPciBusSetBarAttributes (
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
CreateHiddenPciDeviceIo (
  IN UINT64                    PciCfgAddress,
  IN UINT64                    NoOfBars,
  IN UINT64                    *Bar,
  IN PCI_BAR_TYPE              *BarTypes,
  OUT EDKII_PCI_DEVICE_PPI     **PciDevicePpi
  )
{
  HIDDEN_PCI_DEVICE_PRIVATE_DATA  *Private;
  UINT32                          Index;

  if (PciDevicePpi == NULL) {
    return FALSE;
  }

  Private = AllocateZeroPool (sizeof (HIDDEN_PCI_DEVICE_PRIVATE_DATA));
  if (Private == NULL) {
    return FALSE;
  }

  *PciDevicePpi = &Private->PciDevice;

  (*PciDevicePpi)->PciIo.PollMem = PciBusPollMem;
  (*PciDevicePpi)->PciIo.PollIo = PciBusPollIo;
  (*PciDevicePpi)->PciIo.Mem.Read = HiddenPciBusMemRead;
  (*PciDevicePpi)->PciIo.Mem.Write = HiddenPciBusMemWrite;
  (*PciDevicePpi)->PciIo.Io.Read = HiddenPciBusIoRead;
  (*PciDevicePpi)->PciIo.Io.Write = HiddenPciBusIoWrite;
  (*PciDevicePpi)->PciIo.Pci.Read = HiddenPciBusConfigRead;
  (*PciDevicePpi)->PciIo.Pci.Write = HiddenPciBusConfigWrite;
  (*PciDevicePpi)->PciIo.CopyMem = PciBusCopyMem;
  (*PciDevicePpi)->PciIo.Map = PciBusMap;
  (*PciDevicePpi)->PciIo.Unmap = PciBusUnmap;
  (*PciDevicePpi)->PciIo.AllocateBuffer = PciBusAllocateBuffer;
  (*PciDevicePpi)->PciIo.FreeBuffer = PciBusFreeBuffer;
  (*PciDevicePpi)->PciIo.Flush = PciBusFlush;
  (*PciDevicePpi)->PciIo.GetLocation = HiddenPciBusGetLocation;
  (*PciDevicePpi)->PciIo.Attributes = HiddenPciBusAttributes;
  (*PciDevicePpi)->PciIo.GetBarAttributes = HiddenPciBusGetBarAtributes;
  (*PciDevicePpi)->PciIo.SetBarAttributes = HiddenPciBusSetBarAttributes;

  Private->PciCfgAddress = PciCfgAddress;
  for (Index = 0; Index < NoOfBars && Index < PCI_MAX_HIDDEN_BAR; Index++) {
    Private->BarAddress[Index] = Bar[Index];
    Private->BarType[Index] = BarTypes[Index];
  }

  return TRUE;
}