/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <PiPei.h>
#include <Uefi.h>
#include <IndustryStandard/Pci22.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/HeciAccessCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HeciUtilsLib.h>
#include <Register/HeciRegs.h>
#include <Register/PchRegs.h>
#include <Protocol/HeciAccessProtocol.h>
#include <Protocol/HeciTransportProtocol.h>
#include <HeciDeviceConfig.h>

#define HECI_ACCESS_NUM_DEBUG   "[HECI Access (%d)]"

GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN   mGotSmmReadyToLockEvent = FALSE;

/**
  Unsupported function

  @param  This              A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width             Signifies the width of the memory or I/O operations.
  @param  BarIndex          The BAR index of the standard PCI Configuration header to use as the
                            base address for the memory operation to perform.
  @param  Offset            The offset within the selected BAR to start the memory operation.
  @param  Mask              Mask used for the polling criteria.
  @param  Value             The comparison value used for the polling exit criteria.
  @param  Delay             The number of 100 ns units to poll.
  @param  Result            Pointer to the last value read from the memory location.

  @retval EFI_UNSUPPORTED   Function is unsupported
**/
EFI_STATUS
EFIAPI
PolMemIoUnsupported (
  IN  EFI_PCI_IO_PROTOCOL          *This,
  IN  EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN  UINT8                        BarIndex,
  IN  UINT64                       Offset,
  IN  UINT64                       Mask,
  IN  UINT64                       Value,
  IN  UINT64                       Delay,
  OUT UINT64                       *Result
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param [in]     This          A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param [in]     Width         Signifies the width of the memory operations.
  @param [in]     Offset        The offset within the PCI configuration space for the PCI controller.
  @param [in]     Count         The number of PCI configuration operations to perform.
  @param [in,out] Buffer        For read operations, the destination buffer to store the results. For write
                                operations, the source buffer to write data from.

  @retval EFI_UNSUPPORTED       Function is unsupported
**/
EFI_STATUS
EFIAPI
HeciPciIoUnsupported (
  IN     EFI_PCI_IO_PROTOCOL          *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT32                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

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

  @retval EFI_UNSUPPORTED       Function is unsupported
**/
EFI_STATUS
EFIAPI
HeciCopyMemUnsupported (
  IN  EFI_PCI_IO_PROTOCOL         *This,
  IN  EFI_PCI_IO_PROTOCOL_WIDTH   Width,
  IN  UINT8                       DestBarIndex,
  IN  UINT64                      DestOffset,
  IN  UINT8                       SrcBarIndex,
  IN  UINT64                      SrcOffset,
  IN  UINTN                       Count
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Operation             Indicates if the bus master is going to read or write to system memory.
  @param  HostAddress           The system memory address to map to the PCI controller.
  @param  NumberOfBytes         On input the number of bytes to map. On output the number of bytes
                                that were mapped.
  @param  DeviceAddress         The resulting map address for the bus master PCI controller to use to
                                access the hosts HostAddress.
  @param  Mapping               A resulting value to pass to Unmap().

  @retval EFI_UNSUPPORTED       Function is unsupported

**/
EFI_STATUS
EFIAPI
MapUnsupported (
  IN      EFI_PCI_IO_PROTOCOL             *This,
  IN      EFI_PCI_IO_PROTOCOL_OPERATION   Operation,
  IN      VOID                            *HostAddress,
  IN OUT  UINTN                           *NumberOfBytes,
  OUT     EFI_PHYSICAL_ADDRESS            *DeviceAddress,
  OUT     VOID                            **Mapping
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Mapping               The mapping value returned from Map().

  @retval EFI_DEVICE_ERROR      The data was not committed to the target system memory.

**/
EFI_STATUS
EFIAPI
UnMapUnsupported (
  IN  EFI_PCI_IO_PROTOCOL   *This,
  IN  VOID                  *Mapping
  )
{
   return EFI_DEVICE_ERROR;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Type                  This parameter is not used and must be ignored.
  @param  MemoryType            The type of memory to allocate, EfiBootServicesData or
                                EfiRuntimeServicesData.
  @param  Pages                 The number of pages to allocate.
  @param  HostAddress           A pointer to store the base system memory address of the
                                allocated range.
  @param  Attributes            The requested bit mask of attributes for the allocated range.

  @retval EFI_UNSUPPORTED       Function unsupported
**/
EFI_STATUS
EFIAPI
AllocateUnsupported (
  IN  EFI_PCI_IO_PROTOCOL   *This,
  IN  EFI_ALLOCATE_TYPE     Type,
  IN  EFI_MEMORY_TYPE       MemoryType,
  IN  UINTN                 Pages,
  OUT VOID                  **HostAddress,
  IN  UINT64                Attributes
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Pages                 The number of pages to free.
  @param  HostAddress           The base system memory address of the allocated range.

  @retval EFI_INVALID_PARAMETER Function unsupported
**/
EFI_STATUS
EFIAPI
FreeBufferUnsupported (
  IN  EFI_PCI_IO_PROTOCOL   *This,
  IN  UINTN                 Pages,
  IN  VOID                  *HostAddress
  )
{
  return EFI_INVALID_PARAMETER;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.

  @retval EFI_UNSUPPORTED       Function unsupported

**/
EFI_STATUS
EFIAPI
FlushUnsupported (
  IN EFI_PCI_IO_PROTOCOL  *This
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Operation             The operation to perform on the attributes for this PCI controller.
  @param  Attributes            The mask of attributes that are used for Set, Enable, and Disable
                                operations.
  @param  Result                A pointer to the result mask of attributes that are returned for the Get
                                and Supported operations.

  @retval EFI_UNSUPPORTED       Function unsupported
**/
EFI_STATUS
EFIAPI
AttributesUnsupported (
  IN  EFI_PCI_IO_PROTOCOL                       *This,
  IN  EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION   Operation,
  IN  UINT64                                    Attributes,
  OUT UINT64                                    *Result OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for resource range. The legal range for this field is 0..5.
  @param  Supports              A pointer to the mask of attributes that this PCI controller supports
                                setting for this BAR with SetBarAttributes().
  @param  Resources             A pointer to the resource descriptors that describe the current
                                configuration of this BAR of the PCI controller.

  @retval EFI_UNSUPPORTED       Function unsupported
**/
EFI_STATUS
EFIAPI
GetBarAttributesUnsupported (
  IN  EFI_PCI_IO_PROTOCOL   *This,
  IN  UINT8                 BarIndex,
  OUT UINT64                *Supports, OPTIONAL
  OUT VOID                  **Resources OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Unsupported function

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Attributes            The mask of attributes to set for the resource range specified by
                                BarIndex, Offset, and Length.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for resource range. The legal range for this field is 0..5.
  @param  Offset                A pointer to the BAR relative base address of the resource range to be
                                modified by the attributes specified by Attributes.
  @param  Length                A pointer to the length of the resource range to be modified by the
                                attributes specified by Attributes.

  @retval EFI_UNSUPPORTED       Function unsupported
**/
EFI_STATUS
EFIAPI
SetBarAttributesUnsupported (
  IN     EFI_PCI_IO_PROTOCOL    *This,
  IN     UINT64                 Attributes,
  IN     UINT8                  BarIndex,
  IN OUT UINT64                 *Offset,
  IN OUT UINT64                 *Length
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Return HECI interrupt mode based on HECI Handle

  @param[in]      This                Pointer to HECI Access device context structure
  @param[in, out] HeciInterruptMode   Pointer to returned HeciInterruptMode

  @retval EFI_SUCCESS                 Success
  @retval EFI_INVALID_PARAMETER       Wrong parameters passed
**/
EFI_STATUS
EFIAPI
GetHeciInterruptMode (
  IN      HECI_ACCESS_PRIVATE     *This,
  IN OUT  UINTN                   *HeciInterruptMode
  )
{
  if (This == NULL || HeciInterruptMode == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *HeciInterruptMode = This->DeviceConfig.HeciInterruptMode;

  return EFI_SUCCESS;
}

/**
  Returns HECI BAR.

  @param[in] This       HECI Access device context

  @retval BAR read from HECI PCI Config Space
**/
UINTN
GetHeciBar (
  IN HECI_ACCESS_PRIVATE    *This
  )
{
  HECI_MBAR   LocalBar;
  UINT64      HeciPciCfgAddr;
  UINT16      Command;
  UINTN       HeciNumber;

  if (This == NULL) {
    return (UINTN) 0;
  }

  LocalBar.QWord = 0;
  HeciPciCfgAddr = PCI_SEGMENT_LIB_ADDRESS (
                     This->DeviceConfig.PciSegment,
                     This->DeviceConfig.PciBus,
                     This->DeviceConfig.PciDevice,
                     This->DeviceConfig.PciFunction,
                     0
                     );

  if (PciSegmentRead16 (HeciPciCfgAddr + PCI_VENDOR_ID_OFFSET) != V_PCH_INTEL_VENDOR_ID) {
    return (UINTN) 0;
  }

  GetHeciNumber (This->DeviceConfig.PciFunction, &HeciNumber);

  LocalBar.Bits.DWordL = PciSegmentRead32 (HeciPciCfgAddr + PCI_BASE_ADDRESSREG_OFFSET);
  if (LocalBar.Bits.DWordL == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, HECI_ACCESS_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_ERROR, " ERROR: Couldn't read BAR (Device Disabled)\n"));
    return (UINTN) 0;
  }

  //
  // If BAR type [2:1] == 0x2, BAR is 64-bits wide
  //
  if (LocalBar.Bits.DWordL & BIT2) {
    LocalBar.Bits.DWordH = PciSegmentRead32 (HeciPciCfgAddr + PCI_BASE_ADDRESSREG_OFFSET + 4);
  }
  LocalBar.Bits.DWordL &= 0xFFFFFFF0; // Clear Space Indicator, Type and Prefetchable bits

  Command = PciSegmentRead16 (HeciPciCfgAddr + PCI_COMMAND_OFFSET);
  if ((Command & EFI_PCI_COMMAND_MEMORY_SPACE) == 0) {
    PciSegmentWrite16 (HeciPciCfgAddr + PCI_COMMAND_OFFSET, Command | EFI_PCI_COMMAND_MEMORY_SPACE);
  }

  return (UINTN) LocalBar.QWord;
}

/**
  Writes the BAR and enables it in HECI PCI Config Space

  @param[in] This     Pointer to HECI Access device context structure
  @param[in] Bar      BAR to set
**/
VOID
SetHeciBar (
  IN  HECI_ACCESS_PRIVATE   *This,
  IN  UINTN                 Bar
  )
{
  HECI_MBAR   LocalBar;
  UINT64      HeciPciCfgAddr;
  UINT16      Command;
  UINTN       HeciNumber;

  if (This == NULL) {
    return;
  }

  HeciPciCfgAddr = PCI_SEGMENT_LIB_ADDRESS (
                     This->DeviceConfig.PciSegment,
                     This->DeviceConfig.PciBus,
                     This->DeviceConfig.PciDevice,
                     This->DeviceConfig.PciFunction,
                     0
                     );

  if (PciSegmentRead16 (HeciPciCfgAddr + PCI_VENDOR_ID_OFFSET) != V_PCH_INTEL_VENDOR_ID) {
    GetHeciNumber (This->DeviceConfig.PciFunction, &HeciNumber);
    DEBUG ((DEBUG_WARN, HECI_ACCESS_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_WARN, "%a () - Couldn't set BAR (Device Disabled)\n", __FUNCTION__));
    return;
  }

  LocalBar.QWord = (UINT64) Bar;

  Command = PciSegmentRead16 (HeciPciCfgAddr + PCI_COMMAND_OFFSET);

  PciSegmentWrite16 (HeciPciCfgAddr + PCI_COMMAND_OFFSET, 0);
  PciSegmentWrite32 (HeciPciCfgAddr + PCI_BASE_ADDRESSREG_OFFSET, LocalBar.Bits.DWordL);
  PciSegmentWrite32 (HeciPciCfgAddr + PCI_BASE_ADDRESSREG_OFFSET + 4, LocalBar.Bits.DWordH);
  PciSegmentWrite16 (HeciPciCfgAddr + PCI_COMMAND_OFFSET, Command | EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  Write data to HECI PCI device.
  Count 1 and Width EfiPciIoWidthUint8/16/32 are supported only.

  @param [in]     This              A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param [in]     Width             Signifies the width of the memory operations.
  @param [in]     Offset            The offset within the PCI configuration space for the PCI controller.
  @param [in]     Count             The number of PCI configuration operations to perform.
  @param [in,out] Buffer            For read operations, the destination buffer to store the results. For write
                                    operations, the source buffer to write data from.


  @retval EFI_SUCCESS               The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED           The address range specified by Offset, Width, and Count is not
                                    valid for the PCI configuration header of the PCI controller.
  @retval EFI_DEVICE_ERROR          Written data differs from the one passed to write
  @retval EFI_INVALID_PARAMETER     Invalid parameters passed.

**/
EFI_STATUS
EFIAPI
HeciPciWrite (
  IN     EFI_PCI_IO_PROTOCOL          *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT32                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  HECI_ACCESS_PRIVATE *HeciAccess;
  UINT64              HeciPciCfgAddr;
  UINT32              WrittenData;

  if (This == NULL || Buffer == NULL || Count != 1) {
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = (HECI_ACCESS_PRIVATE *) This;
  HeciPciCfgAddr = PCI_SEGMENT_LIB_ADDRESS (
                     HeciAccess->DeviceConfig.PciSegment,
                     HeciAccess->DeviceConfig.PciBus,
                     HeciAccess->DeviceConfig.PciDevice,
                     HeciAccess->DeviceConfig.PciFunction,
                     Offset
                     );

  switch (Width) {
    case EfiPciIoWidthUint8:
      WrittenData = PciSegmentWrite8 (HeciPciCfgAddr, *((UINT8 *) Buffer));
      break;
    case EfiPciIoWidthUint16:
      WrittenData = PciSegmentWrite16 (HeciPciCfgAddr, *((UINT16 *) Buffer));
      break;
    case EfiPciIoWidthUint32:
      WrittenData = PciSegmentWrite32 (HeciPciCfgAddr, *((UINT32 *) Buffer));
      break;
    default:
      return EFI_UNSUPPORTED;
  }

  if (WrittenData != *((UINT32 *) Buffer)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Read data from HECI PCI device.
  Count 1 and Width EfiPciIoWidthUint8/16/32 are supported only.

  @param  This                    A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                   Signifies the width of the memory operations.
  @param  Offset                  The offset within the PCI configuration space for the PCI controller.
  @param  Count                   The number of PCI configuration operations to perform.
  @param  Buffer                  For read operations, the destination buffer to store the results. For write
                                  operations, the source buffer to write data from.

  @retval EFI_SUCCESS             The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED         The address range specified by Offset, Width, and Count is not
                                  valid for the PCI configuration header of the PCI controller.
  @retval EFI_INVALID_PARAMETER   Buffer is NULL or Width is invalid.

**/
EFI_STATUS
EFIAPI
HeciPciRead (
  IN     EFI_PCI_IO_PROTOCOL          *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT32                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  HECI_ACCESS_PRIVATE   *HeciAccess;
  UINT64                HeciPciCfgAddr;

  if (This == NULL || Buffer == NULL || Count != 1) {
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = (HECI_ACCESS_PRIVATE *) This;

  HeciPciCfgAddr = PCI_SEGMENT_LIB_ADDRESS (
                     HeciAccess->DeviceConfig.PciSegment,
                     HeciAccess->DeviceConfig.PciBus,
                     HeciAccess->DeviceConfig.PciDevice,
                     HeciAccess->DeviceConfig.PciFunction,
                     Offset
                     );

  switch (Width) {
    case EfiPciIoWidthUint8:
      *((UINT8 *) Buffer) = PciSegmentRead8 (HeciPciCfgAddr);
      break;
    case EfiPciIoWidthUint16:
      *((UINT16 *) Buffer) = PciSegmentRead16 (HeciPciCfgAddr);
      break;
    case EfiPciIoWidthUint32:
      *((UINT32 *) Buffer) = PciSegmentRead32 (HeciPciCfgAddr);
      break;
    default:
      return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  Enable a PCI driver to access HECI PCI controller registers in the PCI memory or I/O space.
  Bar 0, Count 1 and Width EfiPciIoWidthUint32 are supported only.

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
HeciMemRead (
  IN     EFI_PCI_IO_PROTOCOL          *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  HECI_ACCESS_PRIVATE   *HeciAccess;
  UINTN                 HeciBarSmm;
  UINTN                 HeciBar;

  if (This == NULL || Buffer == NULL || Width != EfiPciIoWidthUint32 || Count != 1 || BarIndex != 0) {
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = (HECI_ACCESS_PRIVATE *) This;
  HeciBar    = GetHeciBar (HeciAccess);
  HeciBarSmm = 0;

  //
  // Check if HECI BAR was changed after SmmReadyToLock event
  // If so, restore the saved one
  //
  if (mGotSmmReadyToLockEvent && HeciBar != HeciAccess->DeviceConfig.TempMemoryBar) {
    HeciBarSmm = HeciBar;
    HeciBar    = HeciAccess->DeviceConfig.TempMemoryBar;
    SetHeciBar (HeciAccess, HeciBar);
  }
  //
  // Set the BAR if it wasn't configured yet
  //
  if (HeciBar == 0) {
    HeciBar = HeciAccess->DeviceConfig.TempMemoryBar;
    SetHeciBar (HeciAccess, HeciBar);
  }

  *((UINT32 *) Buffer) = MmioRead32 (HeciBar + (UINTN) Offset);

  if (HeciBarSmm != 0) {
    SetHeciBar (HeciAccess, HeciBarSmm);
  }

  return EFI_SUCCESS;
}

/**
  Enables a PCI driver to access HECI PCI controller registers in the PCI memory or I/O space.
  Bar 0, Count 1 and  Width EfiPciIoWidthUint32 are supported only.

  @param  This                    A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                   Signifies the width of the memory or I/O operations.
  @param  BarIndex                The BAR index of the standard PCI Configuration header to use as the
                                  base address for the memory or I/O operation to perform.
  @param  Offset                  The offset within the selected BAR to start the memory or I/O operation.
  @param  Count                   The number of memory or I/O operations to perform.
  @param  Buffer                  For read operations, the destination buffer to store the results. For write
                                  operations, the source buffer to write data from.

  @retval EFI_SUCCESS             The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED         BarIndex not valid for this PCI controller.
  @retval EFI_DEVICE_ERROR        Written data differs from the one passed to write
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.

**/
EFI_STATUS
EFIAPI
HeciMemWrite (
  IN     EFI_PCI_IO_PROTOCOL          *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        BarIndex,
  IN     UINT64                       Offset,
  IN     UINTN                        Count,
  IN OUT VOID                         *Buffer
  )
{
  HECI_ACCESS_PRIVATE   *HeciAccess;
  UINT32                WrittenData;
  UINTN                 HeciBarSmm;
  UINTN                 HeciBar;

  if (This == NULL || Buffer == NULL || Width != EfiPciIoWidthUint32 || Count != 1 || BarIndex != 0) {
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = (HECI_ACCESS_PRIVATE *) This;
  HeciBar    = GetHeciBar (HeciAccess);
  HeciBarSmm = 0;

  //
  // Check if HECI BAR was changed after SmmReadyToLock event
  // If so, restore the saved one
  //
  if (mGotSmmReadyToLockEvent && HeciBar != HeciAccess->DeviceConfig.TempMemoryBar) {
    HeciBarSmm = HeciBar;
    HeciBar    = HeciAccess->DeviceConfig.TempMemoryBar;
    SetHeciBar (HeciAccess, HeciBar);
  }
  //
  // Set the BAR if it wasn't configured yet
  //
  if (HeciBar == 0) {
    HeciBar = HeciAccess->DeviceConfig.TempMemoryBar;
    SetHeciBar (HeciAccess, HeciBar);
  }

  WrittenData = MmioWrite32 (HeciBar + (UINTN) Offset, *((UINT32 *) Buffer));

  if (HeciBarSmm != 0) {
    SetHeciBar (HeciAccess, HeciBarSmm);
  }

  if (WrittenData != *((UINT32 *) Buffer)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Retrieves this PCI controller's current PCI bus number, device number, and function number.

  @param  This                    A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  SegmentNumber           The PCI controller's current PCI segment number.
  @param  BusNumber               The PCI controller's current PCI bus number.
  @param  DeviceNumber            The PCI controller's current PCI device number.
  @param  FunctionNumber          The PCI controller's current PCI function number.

  @retval EFI_SUCCESS             The PCI controller location was returned.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.

**/
EFI_STATUS
EFIAPI
HeciPciIoGetLocation (
  IN EFI_PCI_IO_PROTOCOL          *This,
  OUT UINTN                       *SegmentNumber,
  OUT UINTN                       *BusNumber,
  OUT UINTN                       *DeviceNumber,
  OUT UINTN                       *FunctionNumber
  )
{
  HECI_ACCESS_PRIVATE    *HeciAccess;

  if (This == NULL || SegmentNumber == NULL || BusNumber == NULL || DeviceNumber == NULL || FunctionNumber == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = (HECI_ACCESS_PRIVATE *) This;

  *SegmentNumber  = HeciAccess->DeviceConfig.PciSegment;
  *BusNumber      = HeciAccess->DeviceConfig.PciBus;
  *DeviceNumber   = HeciAccess->DeviceConfig.PciDevice;
  *FunctionNumber = HeciAccess->DeviceConfig.PciFunction;

  return EFI_SUCCESS;
}

/**
  Function sets HECI Access device PPI functions

  @param[in,out]  NewHeciDevice            Pointer to HECI Access device to be set
**/
VOID
SetHeciAccessFunctions (
  IN OUT HECI_ACCESS_PRIVATE     *NewHeciDevice
  )
{
  if (NewHeciDevice == NULL) {
    DEBUG ((DEBUG_ERROR, "%a () - Passed NULL pointer!\n", __FUNCTION__));
    return;
  }

   // Set Protocol/PPI functions
  NewHeciDevice->HeciAccess.GetHeciInterruptMode    = (HECI_ACCESS_GET_PARAMETER) GetHeciInterruptMode;
  NewHeciDevice->HeciAccess.GetNextHeciAccess       = (HECI_ACCESS_GET_NEXT_HECI) GetNextHeciAccess;

  NewHeciDevice->HeciAccess.PciIo.PollMem           = (EFI_PCI_IO_PROTOCOL_POLL_IO_MEM) PolMemIoUnsupported;
  NewHeciDevice->HeciAccess.PciIo.PollIo            = (EFI_PCI_IO_PROTOCOL_POLL_IO_MEM) PolMemIoUnsupported;
  NewHeciDevice->HeciAccess.PciIo.Mem.Read          = (EFI_PCI_IO_PROTOCOL_IO_MEM) HeciMemRead;
  NewHeciDevice->HeciAccess.PciIo.Mem.Write         = (EFI_PCI_IO_PROTOCOL_IO_MEM) HeciMemWrite;
  NewHeciDevice->HeciAccess.PciIo.Io.Read           = (EFI_PCI_IO_PROTOCOL_IO_MEM) HeciPciIoUnsupported;
  NewHeciDevice->HeciAccess.PciIo.Io.Write          = (EFI_PCI_IO_PROTOCOL_IO_MEM) HeciPciIoUnsupported;
  NewHeciDevice->HeciAccess.PciIo.Pci.Read          = (EFI_PCI_IO_PROTOCOL_CONFIG) HeciPciRead;
  NewHeciDevice->HeciAccess.PciIo.Pci.Write         = (EFI_PCI_IO_PROTOCOL_CONFIG) HeciPciWrite;
  NewHeciDevice->HeciAccess.PciIo.CopyMem           = (EFI_PCI_IO_PROTOCOL_COPY_MEM) HeciCopyMemUnsupported;
  NewHeciDevice->HeciAccess.PciIo.Map               = (EFI_PCI_IO_PROTOCOL_MAP) MapUnsupported;
  NewHeciDevice->HeciAccess.PciIo.Unmap             = (EFI_PCI_IO_PROTOCOL_UNMAP) UnMapUnsupported;
  NewHeciDevice->HeciAccess.PciIo.AllocateBuffer    = (EFI_PCI_IO_PROTOCOL_ALLOCATE_BUFFER) AllocateUnsupported;
  NewHeciDevice->HeciAccess.PciIo.FreeBuffer        = (EFI_PCI_IO_PROTOCOL_FREE_BUFFER) FreeBufferUnsupported;
  NewHeciDevice->HeciAccess.PciIo.Flush             = (EFI_PCI_IO_PROTOCOL_FLUSH) FlushUnsupported;
  NewHeciDevice->HeciAccess.PciIo.GetLocation       = (EFI_PCI_IO_PROTOCOL_GET_LOCATION) HeciPciIoGetLocation;
  NewHeciDevice->HeciAccess.PciIo.Attributes        = (EFI_PCI_IO_PROTOCOL_ATTRIBUTES) AttributesUnsupported;
  NewHeciDevice->HeciAccess.PciIo.GetBarAttributes  = (EFI_PCI_IO_PROTOCOL_GET_BAR_ATTRIBUTES) GetBarAttributesUnsupported;
  NewHeciDevice->HeciAccess.PciIo.SetBarAttributes  = (EFI_PCI_IO_PROTOCOL_SET_BAR_ATTRIBUTES) SetBarAttributesUnsupported;
}

/**
  Function sets new HECI Access device based on configuration record

  @param[in,out]  NewHeciDevice           HECI Access device to be set pointer
  @param[in]      HeciDeviceInitInfo      HECI Configuration record

  @retval EFI_SUCCESS                     Device info was set correctly
  @retval EFI_ABORTED                     HECI device is disabled or wrong config was passed
**/
EFI_STATUS
HeciSetNewDevice (
  IN OUT   HECI_ACCESS_PRIVATE    *NewHeciDevice,
  IN CONST HECI_DEVICE_CONFIG     *HeciDeviceInitInfo
  )
{
  UINT64      HeciPciCfgAddr;
  UINT16      PciVendorId;
  UINTN       HeciNumber;

  if ((NewHeciDevice == NULL) || (HeciDeviceInitInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  PciVendorId    = 0;
  HeciPciCfgAddr = PCI_SEGMENT_LIB_ADDRESS (
                     HeciDeviceInitInfo->PciSegment,
                     HeciDeviceInitInfo->PciBus,
                     HeciDeviceInitInfo->PciDevice,
                     HeciDeviceInitInfo->PciFunction,
                     0
                     );

  GetHeciNumber (HeciDeviceInitInfo->PciFunction, &HeciNumber);

  PciVendorId = PciSegmentRead16 (HeciPciCfgAddr + PCI_VENDOR_ID_OFFSET);

  if (PciVendorId == 0xFFFF) {
    DEBUG ((DEBUG_WARN, HECI_ACCESS_NUM_DEBUG, HeciNumber));
    DEBUG ((
      DEBUG_WARN,
      " WARNING: HECI%d [%d:%d:%d:%d] is disabled!\n",
      HeciNumber,
      HeciDeviceInitInfo->PciSegment,
      HeciDeviceInitInfo->PciBus,
      HeciDeviceInitInfo->PciDevice,
      HeciDeviceInitInfo->PciFunction
      ));

    return EFI_ABORTED;
  }

  NewHeciDevice->DeviceConfig.SecurityEngine     = HeciDeviceInitInfo->SecurityEngine;
  NewHeciDevice->DeviceConfig.PciSegment         = HeciDeviceInitInfo->PciSegment;
  NewHeciDevice->DeviceConfig.PciBus             = HeciDeviceInitInfo->PciBus;
  NewHeciDevice->DeviceConfig.PciDevice          = HeciDeviceInitInfo->PciDevice;
  NewHeciDevice->DeviceConfig.PciFunction        = HeciDeviceInitInfo->PciFunction;
  NewHeciDevice->DeviceConfig.HeciInterruptMode  = HeciDeviceInitInfo->HeciInterruptMode;
  NewHeciDevice->DeviceConfig.TempMemoryBar      = HeciDeviceInitInfo->TempMemoryBar;

  DEBUG ((DEBUG_INFO, HECI_ACCESS_NUM_DEBUG, HeciNumber));
  DEBUG ((
    DEBUG_INFO,
    " Set Heci Access Interface [%d:%d:%d:%d] (0x%02x:0x%02x)\n",
    NewHeciDevice->DeviceConfig.PciSegment,
    NewHeciDevice->DeviceConfig.PciBus,
    NewHeciDevice->DeviceConfig.PciDevice,
    NewHeciDevice->DeviceConfig.PciFunction,
    PciVendorId,
    PciSegmentRead16 (HeciPciCfgAddr + PCI_DEVICE_ID_OFFSET)
    ));

  SetHeciAccessFunctions (NewHeciDevice);

  NewHeciDevice->HeciAccess.PciIo.RomSize  = 0;
  NewHeciDevice->HeciAccess.PciIo.RomImage = NULL;

  return EFI_SUCCESS;
}
