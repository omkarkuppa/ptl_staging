/** @file
  USB4 host router PCI service routines.

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

@par Specification
**/

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PciIo.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/Usb4Reg.h>
#include <Library/Usb4HrPci.h>
#include "HrPciSrv.h"

/**
  Dump PCI registers of the given device address

  @param[in] DevAddr - PCI Device Address
**/
VOID
HrSrvDumpPciDevRegs (
  IN UINT64 DevAddr
  )
{
  UINT32 Offset;

  DEBUG ((DEBUG_INFO, "Dev Addr: 0x%08x:\n", DevAddr));

  for (Offset = 0; Offset < 64; Offset += 4) {
    DEBUG ((DEBUG_INFO, "%08x ", PciSegmentRead32 (DevAddr + Offset)));
    if((Offset & 0x0F) == 0x0C) {
      DEBUG ((DEBUG_INFO, "\n"));
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

/**
  Dump MMIO data of the specified MMIO base

  @param[in] MmioBase - Base address of MMIO to be dumped
  @param[in] DwCount  - Count of DWORDs to be dumped
**/
VOID
HrSrvMmioDump (
  IN PHYSICAL_ADDRESS MmioBase,
  IN UINT32           DwCount
  )
{
  UINT32 Index;
  UINTN  MmioReg;

  MmioReg = (UINTN) MmioBase;
  DEBUG ((DEBUG_INFO, "MmioDump Base = 0x%016llx:\n", MmioReg));

  for (Index = 0; Index < DwCount; Index++) {
    DEBUG ((DEBUG_INFO, "%08x ", MmioRead32 (MmioReg)));
    if ((Index & 0x03) == 0x03) {
      DEBUG ((DEBUG_INFO, "\n"));
    }
    MmioReg += 4;
  }
}

/**
  Query PCI bridge information for the given bridge BDF.

  @param[in, out] PciBridge      - Pointer to PCI bridge information structure.

  @retval EFI_SUCCESS           - Query PCI bridge information successfully.
  @retval EFI_NOT_FOUND         - Device is not present.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrSrvPciBridgeQuery (
  IN OUT PCI_BRIDGE_INFO *PciBridge
  )
{
  EFI_STATUS    Status;
  UINT32        DevInfo;
  UINT64        DevAddr;

  if (PciBridge == NULL) {
    DEBUG ((DEBUG_ERROR, "HrSrvPciBridgeQuery: Null PciBridge\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, PciBridge->Bdf.Bus, PciBridge->Bdf.Dev, PciBridge->Bdf.Func, 0);
  DevInfo = PciSegmentRead32 (DevAddr);
  if(DevInfo == 0xFFFFFFFF)
  {
    DEBUG ((DEBUG_ERROR, "HrSrvPciBridgeQuery: Device at dev addr 0x%08x is not present\n", DevAddr));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  //
  // Query PCI bridge information
  //
  PciBridge->PriBus  = PciSegmentRead8 (DevAddr + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET);
  PciBridge->SecBus  = PciSegmentRead8 (DevAddr + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  PciBridge->SubBus  = PciSegmentRead8 (DevAddr + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);
  PciBridge->MemBase = PciSegmentRead16 (DevAddr + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase));
  DEBUG ((DEBUG_INFO, "PCI bridge (0x%08x): pri = 0x%x, sec = 0x%x, sub = 0x%x, mem_base = 0x%0x\n",
          DevAddr, PciBridge->PriBus, PciBridge->SecBus, PciBridge->SubBus, PciBridge->MemBase));
  Status = EFI_SUCCESS;

Exit:
  return Status;
}

/**
  Initialize PCI bridge device

  @param[in] DevAddr        - PCI Device address of the target bridge
  @param[in] MemoryBase     - Base address of MMIO range for Memory Base and Memory Limit register
  @param[in] PrimaryBus     - Primary bus number
  @param[in] SecondaryBus   - Secondary bus number
  @param[in] SubordinateBus - Subordinate bus number

  @retval EFI_SUCCESS   - Initialize PCI bridge success
  @retval EFI_NOT_FOUND - Device is not present
**/
EFI_STATUS
HrSrvPciBridgeInit (
  IN UINT64  DevAddr,
  IN UINT16  MemoryBase,
  IN UINT8   PrimaryBus,
  IN UINT8   SecondaryBus,
  IN UINT8   SubordinateBus
  )
{
  EFI_STATUS  Status;
  UINT32      DevInfo;
  UINT16      Data16;

  DevInfo = PciSegmentRead32 (DevAddr);
  if(DevInfo == 0xFFFFFFFF)
  {
    DEBUG ((DEBUG_ERROR, "HrSrvPciBridgeInit: Device at dev addr 0x%08x is not present\n", DevAddr));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  //
  // Set primary bus, secondary bus and subordinate bus of bridge
  // Set memory base and memory limit registers
  //
  DEBUG ((DEBUG_INFO, "HrSrvPciBridgeInit - dev_addr = 0x%08x, mem_base = 0x%08x\n", DevAddr, MemoryBase));
  DEBUG ((DEBUG_INFO, "pri = 0x%x, sec = 0x%x, sub = 0x%x\n", PrimaryBus, SecondaryBus, SubordinateBus));
  PciSegmentWrite8 (DevAddr + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, PrimaryBus);
  PciSegmentWrite8 (DevAddr + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, SecondaryBus);
  PciSegmentWrite8 (DevAddr + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, SubordinateBus);
  PciSegmentWrite16 (DevAddr + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), MemoryBase);
  PciSegmentWrite16 (DevAddr + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit), MemoryBase);

  //
  // Set CMD Memory Space Enable & Bus Master Enable
  //
  Data16 = PciSegmentRead16 (DevAddr + PCI_COMMAND_OFFSET);
  Data16 |= (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE);
  PciSegmentWrite16 (DevAddr + PCI_COMMAND_OFFSET, Data16);
  Status = EFI_SUCCESS;

Exit:
  return Status;
}

/**
  Un-Initialize PCI bridge device

  @param[in] DevAddr  - PCI Device address of the target bridge

  @retval EFI_SUCCESS   - Un-Initialize PCI bridge success
  @retval EFI_NOT_FOUND - Device is not present
**/
EFI_STATUS
HrSrvPciBridgeUninit (
  IN UINT64  DevAddr
  )
{
  EFI_STATUS  Status;
  UINT32      DevInfo;
  UINT16      Data16;

  DEBUG ((DEBUG_INFO, "HrSrvPciBridgeUninit - dev_addr = 0x%08x\n", DevAddr));
  DevInfo = PciSegmentRead32 (DevAddr);
  if(DevInfo == 0xFFFFFFFF)
  {
    DEBUG ((DEBUG_ERROR, "HrSrvPciBridgeUninit: Device at dev_addr 0x%08x is not present\n", DevAddr));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  //
  // Clear CMD Memory Space Enable & Bus Master Enable
  //
  Data16 = PciSegmentRead16 (DevAddr + PCI_COMMAND_OFFSET);
  Data16 &= (~(EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));
  PciSegmentWrite16 (DevAddr + PCI_COMMAND_OFFSET, Data16);

  //
  // Clear primary bus, secondary bus and subordinate bus of bridge
  // Clear memory base and memory limit registers
  //
  PciSegmentWrite8 (DevAddr + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, 0);
  PciSegmentWrite8 (DevAddr + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, 0);
  PciSegmentWrite8 (DevAddr + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, 0);
  PciSegmentWrite16 (DevAddr + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), 0);
  PciSegmentWrite16 (DevAddr + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit), 0);
  Status = EFI_SUCCESS;

Exit:
  return Status;
}

/**
  Initialize PCI configuration space of PCI device for MMIO access and DMA transfer.

  @param[in] DevAddr    - PCI device address
  @param[in] MmioAddr   - MMIO base address to be used by PCI device

  @retval EFI_SUCCESS   - Initialize NHI PCI configuration space successfully.
  @retval EFI_NOT_FOUND - Device is not present.
  @retval EFI_UNSUPPORTED - Fail to initialize NHI PCI configuration space.
**/
EFI_STATUS
HrSrvPciDevInit (
  IN UINT64              DevAddr,
  IN PHYSICAL_ADDRESS    MmioAddr
  )
{
  EFI_STATUS      Status;
  UINT32          DevInfo;
  UINT16          Data16;
  UINT32          Data32;
  UINT64          Data64;

  DEBUG ((DEBUG_INFO, "HrSrvPciDevInit entry - dev_addr = 0x%x, mmio_addr = 0x%016llX\n", DevAddr, MmioAddr));

  DevInfo = PciSegmentRead32 (DevAddr);
  if(DevInfo == 0xFFFFFFFF)
  {
    DEBUG ((DEBUG_ERROR, "HrSrvPciDevInit: PCI device 0x%x is not present\n", DevAddr));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  //
  // Configure MMIO base address
  //
  Data32 = PciSegmentRead32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET);
  Data32 &= 0x00000007;
  if (Data32 == BAR_TYPE_MEM_32) {
    //
    // 32 bit access space
    //
    PciSegmentWrite32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET, (UINT32) MmioAddr);
    DEBUG ((DEBUG_INFO, "PCI BAR0 (32bit) = 0x%08x\n", PciSegmentRead32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET)));
  } else if (Data32 == BAR_TYPE_MEM_64) {
    //
    // 64 bit access space
    //
    PciSegmentWrite32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET, (UINT32) (MmioAddr & 0xFFFFFFFF));
    PciSegmentWrite32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) RShiftU64 (MmioAddr, 32));
    Data64 = (UINT64) PciSegmentRead32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET + 4);
    Data64 = LShiftU64 (Data64, 32) | PciSegmentRead32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET);
    DEBUG ((DEBUG_INFO, "PCI BAR0 (64bit) = 0x%016llx\n", Data64));
  } else {
    DEBUG ((DEBUG_ERROR, "HrSrvPciDevInit: Invalid BAR type 0x%x\n", Data32));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Set CMD Memory Space Enable & Bus Master Enable
  //
  Data16 = PciSegmentRead16 (DevAddr + PCI_COMMAND_OFFSET);
  Data16 |= (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE);
  PciSegmentWrite16 (DevAddr + PCI_COMMAND_OFFSET, Data16);
  Status = EFI_SUCCESS;

Exit:
  DEBUG ((DEBUG_INFO, "HrSrvPciDevInit exit\n"));
  return Status;
}

/**
  Un-Initialize PCI configuration space of PCI device to clear MMIO and disable DMA transfer

  @param[in] DevAddr   - PCI device address of PCI device

  @retval EFI_SUCCESS     - Un-Initialize PCI configuration space success
  @retval EFI_NOT_FOUND   - Device is not present.
  @retval EFI_UNSUPPORTED - Fail to un-initialize NHI PCI configuration space.
**/
EFI_STATUS
HrSrvPciDevUninit (
  IN UINT64 DevAddr
  )
{
  EFI_STATUS      Status;
  UINT32          DevInfo;
  UINT32          Data32;
  UINT16          Data16;

  DEBUG ((DEBUG_INFO, "HrSrvPciDevUninit entry - dev_addr = 0x%08x\n", DevAddr));

  DevInfo = PciSegmentRead32 (DevAddr);
  if(DevInfo == 0xFFFFFFFF)
  {
    DEBUG ((DEBUG_ERROR, "HrSrvPciDevUninit: PCI device at dev addr 0x%08x is not available\n", DevAddr));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  //
  // Clear CMD Memory Space Enable & Bus Master Enable
  //
  Data16 = PciSegmentRead16 (DevAddr + PCI_COMMAND_OFFSET);
  Data16 &= (~(EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));
  PciSegmentWrite16 (DevAddr + PCI_COMMAND_OFFSET, Data16);

  //
  // Clear MMIO base address
  //
  Data32 = PciSegmentRead32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET);
  Data32 &= 0x00000007;
  if (Data32 == BAR_TYPE_MEM_32) {
    //
    // 32 bit access space
    //
    PciSegmentWrite32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET, 0);
  } else if (Data32 == BAR_TYPE_MEM_64) {
    //
    // 64 bit access space
    //
    PciSegmentWrite32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET, 0);
    PciSegmentWrite32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET + 4, 0);
  } else {
    DEBUG ((DEBUG_ERROR, "HrSrvPciDevUninit: Invalid BAR type 0x%x\n", Data32));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = EFI_SUCCESS;
Exit:
  DEBUG ((DEBUG_INFO, "HrSrvPciDevUninit exit\n"));
  return Status;
}

/**
  Query MMIO base address of PCI device.

  @param[in]  DevBdf   - PCI device location.
  @param[out] MmioAddr - Pointer to MMIO base address.

  @retval EFI_SUCCESS           - Query MMIO base address success.
  @retval EFI_UNSUPPORTED       - Fail to query MMIO base address.
  @retval EFI_NOT_READY         - MMIO is not assigned.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrSrvPciQueryMmio (
  IN  UINT64              DevAddr,
  OUT PHYSICAL_ADDRESS    *MmioAddr
  )
{
  EFI_STATUS          Status;
  PHYSICAL_ADDRESS    MmioBase;
  UINT16              Cmd;
  UINT32              Data32;

  DEBUG ((DEBUG_INFO, "HrSrvPciQueryMmio entry\n"));

  if (MmioAddr == NULL) {
    DEBUG ((DEBUG_ERROR, "HrSrvPciQueryMmio: Null MmioAddr\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Check CMD Memory Space Enable & Bus Master Enable
  //
  Cmd = PciSegmentRead16 (DevAddr + PCI_COMMAND_OFFSET);
  Data32 = (UINT32) Cmd & (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE);
  if (Data32 != (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE)) {
    DEBUG ((DEBUG_INFO, "HrSrvPciQueryMmio: PCI memory space & bus master are not enabled by default. CMD = 0x%0x\n", Cmd));
    //
    // Enable PCI memory space & bus master for Tx/Rx Ring access
    //
    Cmd |= (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE);
    PciSegmentWrite16 (DevAddr + PCI_COMMAND_OFFSET, Cmd);
  }

  //
  // Query MMIO base address
  //
  Data32 = PciSegmentRead32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET);
  if ((Data32 & 0x00000007) == BAR_TYPE_MEM_32) {
    //
    // 32 bit access space
    //
    MmioBase = (PHYSICAL_ADDRESS) (Data32 & 0xFFFFFFF0);
    DEBUG ((DEBUG_INFO, "HrSrvPciQueryMmio: BAR0 (32 bit) = 0x%08x\n", MmioBase));
  } else if ((Data32 & 0x00000007) == BAR_TYPE_MEM_64) {
    //
    // 64 bit access space
    //
    MmioBase = (PHYSICAL_ADDRESS) (Data32 & 0xFFFFFFF0);
    MmioBase += ((PHYSICAL_ADDRESS) PciSegmentRead32 (DevAddr + PCI_BASE_ADDRESSREG_OFFSET + 4)) << 32;
    DEBUG ((DEBUG_INFO, "HrSrvPciQueryMmio: BAR0 (64 bit) = 0x%016llx\n", MmioBase));
  } else {
    DEBUG ((DEBUG_ERROR, "HrSrvPciQueryMmio: Invalid BAR type 0x%x\n", (Data32 & 0x07)));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (MmioBase == 0) {
    DEBUG ((DEBUG_ERROR, "HrSrvPciQueryMmio: MMIO is not assigned for Tx/Rx Ring programming!\n"));
    Status = EFI_NOT_READY;
    goto Exit;
  }

  *MmioAddr = MmioBase;
  Status = EFI_SUCCESS;

Exit:
  DEBUG ((DEBUG_INFO, "HrSrvPciQueryMmio exit\n"));
  return Status;
}

/**
  Query PCI I/O protocol of PCI device.

  @param[in]  Bus           - Bus of PCI location.
  @param[in]  Dev           - Device of PCI location.
  @param[in]  Func          - Function of PCI location.
  @param[out] PciIoProtocol - Pointer of pointer to the associated PCI I/O protocol.

  @retval EFI_SUCCESS           - Query PCI I/O protocol success.
  @retval EFI_NOT_FOUND         - Can't find PCI I/O protocol for the given BDF.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrSrvPciQueryIoProtocol (
  IN  UINT8                  Bus,
  IN  UINT8                  Dev,
  IN  UINT8                  Func,
  OUT EFI_PCI_IO_PROTOCOL    **PciIoProtocol
  )
{
  EFI_STATUS            Status;
  UINTN                 PciHandleCount;
  EFI_HANDLE            *PciHandles;
  UINT32                Index;
  EFI_PCI_IO_PROTOCOL   *PciIo;
  UINTN                 SegNum;
  UINTN                 BusNum;
  UINTN                 DevNum;
  UINTN                 FuncNum;

  DEBUG ((DEBUG_INFO, "HrSrvPciQueryIoProtocol entry\n"));
  PciHandles = NULL;

  PciHandles = NULL;

  if (PciIoProtocol == NULL) {
    DEBUG ((DEBUG_ERROR, "HrSrvPciQueryIoProtocol: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = gBS->LocateHandleBuffer (
             ByProtocol,
             &gEfiPciIoProtocolGuid,
             NULL,
             &PciHandleCount,
             &PciHandles
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HrSrvPciQueryIoProtocol: Locate handle buffer failure, %r\n", Status));
    goto Exit;
  }

  //
  // Look for the PCI I/O protocol of the target device.
  //
  for (Index = 0; Index < PciHandleCount; Index++) {
    Status = gBS->HandleProtocol (PciHandles[Index], &gEfiPciIoProtocolGuid, (VOID **)&PciIo);
    if (EFI_ERROR (Status)) {
      continue;
    }
    Status = PciIo->GetLocation (PciIo, &SegNum, &BusNum, &DevNum, &FuncNum);
    if (EFI_ERROR (Status)) {
      continue;
    }
    if ((SegNum == 0) && (BusNum == Bus) && (DevNum == Dev) && (FuncNum == Func)) {
      break;
    }
  }
  if (Index >= PciHandleCount) {
    DEBUG ((DEBUG_INFO, "HrSrvPciQueryIoProtocol: Unable to find PCI I/O protocol for the target device\n"));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  *PciIoProtocol = PciIo;

Exit:
  gBS->FreePool (PciHandles);
  DEBUG ((DEBUG_INFO, "HrSrvPciQueryIoProtocol exit\n"));
  return Status;
}

/**
  Detect USB4 host router at the specified PCI device address.

  @param[in] DevAddr - PCI device address of USB4 host router to be checked.

  @retval EFI_SUCCESS   - USB4 host router is detected.
  @retval EFI_NOT_FOUND - USB4 host router is not found.
**/
EFI_STATUS
HrSrvPciDetectUsb4Hr (
  IN UINT64    DevAddr
  )
{
  UINT8         ProgInterface;
  UINT8         SubClassCode;
  UINT8         BaseCode;

  ProgInterface = PciSegmentRead8 (DevAddr + PCI_CLASSCODE_OFFSET);
  SubClassCode  = PciSegmentRead8 (DevAddr + PCI_CLASSCODE_OFFSET + 1);
  BaseCode      = PciSegmentRead8 (DevAddr + PCI_CLASSCODE_OFFSET + 2);

  if ((ProgInterface != PCI_IF_USB4) || (SubClassCode != PCI_CLASS_SERIAL_USB) || (BaseCode != PCI_CLASS_SERIAL)) {
    DEBUG ((DEBUG_ERROR, "Device 0x%llX is not USB4 host router, class code = (%0xh, %0xh, %0xh)\n", DevAddr, BaseCode, SubClassCode, ProgInterface));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "USB4 host router is detected at device address 0x%llx\n", DevAddr));

  return EFI_SUCCESS;
}