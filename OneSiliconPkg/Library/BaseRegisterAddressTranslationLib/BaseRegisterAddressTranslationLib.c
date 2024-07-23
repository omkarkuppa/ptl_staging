/** @file
  Base implementation of the Register Address Translation library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include "BaseRegisterAddressTranslationLib.h"


/**
  Returns the value in a BAR register

  @param[in] Topology   Pointer to system topology data
  @param[in] Socket     Which socket to check
  @param[in] BarIndex   Index to BAR in address map table
  @param[in] AllowCache Read the value from cache first if TRUE; otherwise, read
                        the value from registers in BAR descriptor

  @retval             The value of the given BAR for the given socket
**/
STATIC
UINT64
GetBarValueByIndex (
  IN SYSTEM_TOPOLOGY  *Topology,
  IN UINT8            Socket,
  IN UINTN            BarIndex,
  IN BOOLEAN          AllowCache
  )
{
  COMBINED_BAR      *BarData;
  UINT64            Value;
  UINTN             Index;
  UINTN             RegSize;
  UINT64            RegBase;
  UINTN             RegOff;
  UINT64            RegValue;
  PCI_CFG_ACCESSOR  PciCfg;
  BOOLEAN           ValidBar;

  if (AllowCache) {
    Value = SystemTopologyGetBarCachedValue (Topology, Socket, BarIndex);
    if (Value != (UINT64)-1) {
      return Value;
    }
  }

  BarData = SystemTopologyGetBarByIndex (Topology, Socket, BarIndex);
  if (BarData == NULL) {
    ASSERT (BarData != NULL);
    return (UINT64)-1;
  }

  Value     = 0;
  ValidBar  = FALSE;
  RegValue  = 0;
  for (Index = 0; Index < ARRAY_SIZE (BarData->Bar); ++Index) {
    RegSize = (BarData->Bar[Index].Mask >= 0x100000000ull)
              ? RegisterWidthQWord : RegisterWidthDWord;

    switch (BarData->Bar[Index].Type) {
    case RegisterSpaceCfg:
      PciCfg.Address = (UINT32)(BarData->Bar[Index].Register & 0xFFFFFFFF);
      PciCfg.Bdfo.Bus = Topology->PerSocketData[Socket].BusMap[PciCfg.Bdfo.Bus];

      RegBase = SystemTopologyGetMmcfgBase (Topology, Socket);
      RegBase += PciCfg.Address;

      RegValue = AddressMapMmioRead (Topology, Socket, RegBase, RegSize);
      break;

    case RegisterSpaceMsgCr:
      RegBase  = BarData->Bar[Index].Register & 0xFFFFFFFF;
      RegOff   = *((UINT32 *)(&BarData->Bar[Index].Register) + 1);
      RegOff  &= 0x0FFFFFFF; // 4-MSB are BAR type
      RegValue = AddressMapMsgRead (Topology, Socket, RegBase, RegOff, RegSize, RegisterSpaceMsgCr);
      break;

    case RegisterSpaceMemFixed:
    case RegisterSpaceLtMem:
      RegBase  = BarData->Bar[Index].Register & 0x0FFFFFFFFFFFFFFFull;
      RegValue = AddressMapMmioRead (Topology, Socket, RegBase, RegSize);
      break;

    case RegisterSpaceIoFixed:
      RegBase  = BarData->Bar[Index].Register & 0xFFFFFFFF;
      RegValue = AddressMapIoRead (Topology, Socket, RegBase, RegSize);
      break;

    default:
      continue;
    }

    if (RAT_INVALID_BASE (RegValue)) {
      ValidBar = FALSE;
      break;
    }

    ValidBar  = TRUE;
    RegValue  &= BarData->Bar[Index].Mask;
    if (BarData->Bar[Index].Shift > 0) {
      RegValue = LShiftU64 (RegValue, BarData->Bar[Index].Shift);
    }
    Value += RegValue;
  }

  if (!ValidBar) {
    Value = (UINT64)-1;
  }

  if (AllowCache && Value != (UINT64)-1) {
    SystemTopologySetBarCachedValue (Topology, Socket, BarIndex, Value);
  }

  return Value;
}

/**
  Get the register base address for given endpoint instance.

  @param[in] Topology   Pointer to system topology data
  @param[in] Socket     Which socket to check
  @param[in] Id         ID of endpoint
  @param[in] Instance   Index of endpoint instance
  @param[in] AccType    Type of register space
  @param[in] AccDesc    Pointer to accessor descriptor

  @return Base address of given endpoint instance
  @return NULL if given AccType is not supported
**/
STATIC
VOID*
GetIpBaseAddressInternal (
  IN SYSTEM_TOPOLOGY        *Topology,
  IN UINT8                  Socket,
  IN UINT64                 Id,
  IN UINT16                 Instance,
  IN REGISTER_SPACE_TYPE    AccType,
  IN ACCESSOR_DESCRIPTOR    *AccDesc
  )
{
  UINT64      Address;
  UINTN       Offset;

  switch (AccType) {
    case RegisterSpaceCfg:
      Address = SystemTopologyGetMmcfgBase (Topology, Socket);
      Offset  = AccDesc->PciCfg.Address;
      break;

    case RegisterSpaceMemFixed:
    case RegisterSpaceLtMem:
    case RegisterSpaceIoFixed:
      Address = AccDesc->Fixed.Address;
      Offset  = 0;
      break;

    case RegisterSpaceMemBar:
    case RegisterSpaceIoBar:
      Address = GetBarValueByIndex (Topology, Socket, AccDesc->Bar.BarIndex, TRUE);
      Offset  = AccDesc->Bar.Offset;
      break;

    case RegisterSpaceMsgCr:
    case RegisterSpaceMsgCfg:
    case RegisterSpaceMsgMem:
    case RegisterSpaceMsgIo:
      Address = AddressMapMsgGetBase (Topology, Socket, Instance, AccDesc->Sideband.Address, RegisterSpaceMsgCr);
      Offset  = 0;
      break;

    default:
      ASSERT (FALSE); // No other types supported at present
      return NULL;
  }

  if (RAT_INVALID_BASE (Address)) {
    return NULL;
  }

  return (VOID *)(UINTN)(Address + Offset);
}

/**
  Returns the base address of the requested endpoint instance in given register
  access space.

  @param[in] Socket     Socket ID or index
  @param[in] Id         Endpoint ID within SoC
  @param[in] Instance   Instance of endpoint given by Id
  @param[in] Type       Type of Register access space

  @retval               NULL if register address could not be determined,
                        valid address otherwise
**/
VOID*
EFIAPI
GetIpBaseAddress (
  IN UINT8                  Socket,
  IN UINT64                 Id,
  IN UINT16                 Instance,
  IN REGISTER_SPACE_TYPE    Type
  )
{
  SYSTEM_TOPOLOGY         *Topology;
  ACCESSOR_DESCRIPTOR     AccDesc;
  EFI_STATUS              Status;

  Topology = SystemTopologyGetDataPointer ();
  if (Topology == NULL) {
    ASSERT (Topology != NULL);
    return NULL;
  }

  Status = SystemTopologyGetAccessor (Topology, Id, Socket, Instance, &Type, &AccDesc, NULL);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  return GetIpBaseAddressInternal (Topology, Socket, Id, Instance, Type, &AccDesc);
}

/**
  Prints out register trace log before access, if enabled.

  @param[in] Socket             Socket ID or index
  @param[in] Id                 Endpoint ID within SoC
  @param[in] Instance           Instance of endpoint given by Id
  @param[in] AccType            Register access space type
  @param[in] Register           Register to write to
  @param[in] Size               Byte width of register
  @param[in] RegisterAddress    Register access address
  @param[in] RegisterAccessRw   Read or write operation

  @retval None
**/
STATIC
VOID
RatTracePreAccess (
  IN UINT8                    Socket,
  IN UINT64                   Id,
  IN UINT16                   Instance,
  IN REGISTER_SPACE_TYPE      AccType,
  IN UINT32                   Register,
  IN REGISTER_WIDTH_BYTES     Size,
  IN UINTN                    RegisterAddress,
  IN REGISTER_ACCESS_RW       RegisterAccessRw
  )
{
  SYSTEM_TOPOLOGY         *Topology;
  ACCESSOR_DESCRIPTOR     AccDesc;
  EFI_STATUS              Status;
  CHAR8                   *InstString;
  CHAR8                   *OpString;
  CHAR8                   DefaultInstString[16];

  Topology = SystemTopologyGetDataPointer ();
  if (Topology == NULL) {
    ASSERT (Topology != NULL);
    return;
  }

  Status = SystemTopologyGetAccessor (Topology, Id, Socket, Instance, &AccType, &AccDesc, &InstString);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  AsciiSPrint (DefaultInstString, sizeof(DefaultInstString), "inst.%d", Instance);
  OpString = (RegisterAccessRw == RegisterAccessRead) ? "read " : "write";

  switch (AccType) {
  case RegisterSpaceCfg:
    DEBUG ((
      DEBUG_INFO,
      "(RATrace) Cfg.%a %a register[%x/%x/%x + %x] @ %p:",
      OpString,
      (InstString == NULL) ? DefaultInstString : InstString,
      AccDesc.PciCfg.Bdfo.Bus,
      AccDesc.PciCfg.Bdfo.Device,
      AccDesc.PciCfg.Bdfo.Function,
      Register,
      RegisterAddress
      ));
    break;

  case RegisterSpaceMemBar:
    DEBUG ((
      DEBUG_INFO,
      "(RATrace) MemBar.%a %a register[BAR(%d) + %x + %x] @ %p:",
      OpString,
      (InstString == NULL) ? DefaultInstString : InstString,
      AccDesc.Bar.BarIndex,
      AccDesc.Bar.Offset,
      Register,
      RegisterAddress
      ));
    break;

  case RegisterSpaceMsgCfg:
    DEBUG ((
      DEBUG_INFO,
      "(RATrace) MsgCfg.%a %a @ [%x/%x/%x] ->\n",
      OpString,
      (InstString == NULL) ? DefaultInstString : InstString,
      AccDesc.Sideband.Bits.PortId,
      AccDesc.Sideband.Bits.SbFid,
      AccDesc.Sideband.Bits.SbBar
      ));

    AccType = RegisterSpaceCfg;
    Status = SystemTopologyGetAccessor (Topology, Id, Socket, Instance, &AccType, &AccDesc, &InstString);
    ASSERT_EFI_ERROR (Status);

    DEBUG ((
      DEBUG_INFO,
      "(RATrace)  ->Cfg.%a %a register[%x/%x/%x + %x] @ %p:",
      OpString,
      (InstString == NULL) ? DefaultInstString : InstString,
      AccDesc.PciCfg.Bdfo.Bus,
      AccDesc.PciCfg.Bdfo.Device,
      AccDesc.PciCfg.Bdfo.Function,
      Register,
      RegisterAddress
      ));

    break;

  case RegisterSpaceMsgMem:
    DEBUG ((
      DEBUG_INFO,
      "(RATrace) MsgMem.%a %a @ [%x/%x/%x] ->\n",
      OpString,
      (InstString == NULL) ? DefaultInstString : InstString,
      AccDesc.Sideband.Bits.PortId,
      AccDesc.Sideband.Bits.SbFid,
      AccDesc.Sideband.Bits.SbBar
      ));

    AccType = RegisterSpaceMemBar;
    Status = SystemTopologyGetAccessor (Topology, Id, Socket, Instance, &AccType, &AccDesc, &InstString);
    ASSERT_EFI_ERROR (Status);

    DEBUG ((
      DEBUG_INFO,
      "(RATrace) ->MemBar.%a %a register[BAR(%d)+ %x + %x] @ %p:",
      OpString,
      (InstString == NULL) ? DefaultInstString : InstString,
      AccDesc.Bar.BarIndex,
      AccDesc.Bar.Offset,
      Register,
      RegisterAddress
      ));
    break;

  case RegisterSpaceMsgIo:
    DEBUG ((
      DEBUG_INFO,
      "(RATrace) MsgMem.%a %a @ [%x/%x/%x] ->\n",
      OpString,
      (InstString == NULL) ? DefaultInstString : InstString,
      AccDesc.Sideband.Bits.PortId,
      AccDesc.Sideband.Bits.SbFid,
      AccDesc.Sideband.Bits.SbBar
      ));

    AccType = RegisterSpaceIoBar;
    Status = SystemTopologyGetAccessor (Topology, Id, Socket, Instance, &AccType, &AccDesc, &InstString);
    ASSERT_EFI_ERROR (Status);

    DEBUG ((
      DEBUG_INFO,
      "(RATrace) ->IoBar.%a %a register[BAR(%d)+ %x + %x] @ %p:",
      OpString,
      (InstString == NULL) ? DefaultInstString : InstString,
      AccDesc.Bar.BarIndex,
      AccDesc.Bar.Offset,
      Register,
      RegisterAddress
      ));
    break;

  case RegisterSpaceMsgCr:
    DEBUG ((
      DEBUG_INFO,
      "(RATrace) MsgCr.%a %a register[%x/%x/%x + %x] @ %p:",
      OpString,
      (InstString == NULL) ? DefaultInstString : InstString,
      AccDesc.Sideband.Bits.PortId,
      AccDesc.Sideband.Bits.SbFid,
      AccDesc.Sideband.Bits.SbBar,
      Register,
      RegisterAddress
      ));
    break;

  case RegisterSpaceIoBar:
    DEBUG ((
      DEBUG_INFO,
      "(RATrace) IoBar.%a %a register[BAR(%d) + %x + %x] @ %p:",
      OpString,
      (InstString == NULL) ? DefaultInstString : InstString,
      AccDesc.Bar.BarIndex,
      AccDesc.Bar.Offset,
      Register,
      RegisterAddress
      ));
    break;

  case RegisterSpaceIoFixed:
    DEBUG ((
      DEBUG_INFO,
      "(RATrace) FixedIo.%a %a register[%08x + %x] @ %p:",
      OpString,
      (InstString == NULL) ? DefaultInstString : InstString,
      AccDesc.Fixed.Address,
      Register,
      RegisterAddress
      ));
    break;

  case RegisterSpaceMemFixed:
  case RegisterSpaceLtMem:
    DEBUG ((
      DEBUG_INFO,
      "(RATrace) FixedMem.%a %a register[%08x + %x] @ %p:",
      OpString,
      (InstString == NULL) ? DefaultInstString : InstString,
      AccDesc.Fixed.Address,
      Register,
      RegisterAddress
      ));
    break;

  default:
    break;
  }
}

/**
  Prints out register trace log after access, if enabled.

  @param[in] Socket             Socket ID or index
  @param[in] Id                 Endpoint ID within SoC
  @param[in] Instance           Instance of endpoint given by Id
  @param[in] AccType            Register access space type
  @param[in] Register           Register to write to
  @param[in] Size               Byte width of register
  @param[in] RegisterAddress    Register access address
  @param[in] RegisterAccessRw   Read or write operation
  @param[in] Value              Value of register

  @retval None
**/
STATIC
VOID
RatTracePostAccess (
  IN UINT8                    Socket,
  IN UINT64                   Id,
  IN UINT16                   Instance,
  IN REGISTER_SPACE_TYPE      AccType,
  IN UINT32                   Register,
  IN REGISTER_WIDTH_BYTES     Size,
  IN UINTN                    RegisterAddress,
  IN REGISTER_ACCESS_RW       RegisterAccessRw,
  IN UINT64                   Value
  )
{
  switch (Size) {
  case RegisterWidthByte:
    DEBUG ((DEBUG_INFO, " %01x\n", Value));
    break;

  case RegisterWidthWord:
    DEBUG ((DEBUG_INFO, " %04x\n", Value));
    break;

  case RegisterWidthDWord:
    DEBUG ((DEBUG_INFO, " %08x\n", Value));
    break;

  case RegisterWidthQWord:
    DEBUG ((DEBUG_INFO, " %016x\n", Value));
    break;

  default:
    break;
  }

#if RAT_TRACE_WRITE_CONFIRM
  if (RegisterAccessRw == RegisterAccessWrite) {
    DEBUG ((
      DEBUG_INFO,
      "(RATrace)     write confirm @ 0x%p: %lx\n",
      RegisterAddress,
      AddressMapMmioRead (NULL, Socket, RegisterAddress, Size)
      ));
  }
#endif
}

/**
  Reads the value of the requested register within given endpoint instance.

  @param[in] Socket     Socket ID or index
  @param[in] Id         Endpoint ID within SoC
  @param[in] Instance   Instance of endpoint given by Id
  @param[in] Type       Register access space type
  @param[in] Register   Register to read from
  @param[in] Size       Byte width of register

  @retval             - Value read from register
**/
UINT64
EFIAPI
ReadIpRegister (
  IN UINT8                Socket,
  IN UINT64               Id,
  IN UINT16               Instance,
  IN REGISTER_SPACE_TYPE  Type,
  IN UINT32               Register,
  IN REGISTER_WIDTH_BYTES Size
  )
{
  EFI_STATUS            Status;
  UINTN                 RegisterAddress;
  UINT64                Value;
  BOOLEAN               AccessFiltered;
  SYSTEM_TOPOLOGY       *Topology;
  ACCESSOR_DESCRIPTOR   AccDesc;

  Topology = SystemTopologyGetDataPointer ();
  if (Topology == NULL) {
    ASSERT (Topology != NULL);
    return 0;
  }

  Value           = 0;
  AccessFiltered  = FALSE;
  RegisterAddress = 0;

  Status = SystemTopologyGetAccessor (Topology, Id, Socket, Instance, &Type, &AccDesc, NULL);
  if (!EFI_ERROR (Status)) {
    RegisterAddress = (UINTN)GetIpBaseAddressInternal (Topology, Socket, Id, Instance, Type, &AccDesc);
  }

  if ((VOID*)RegisterAddress == NULL && Type < RegisterSpaceMsgCfg) {
    return (UINT64)-1; // Register can't be read
  }

  RegisterAddress += Register;

  if (RAT_TRACE_ENABLED) {
    RatTracePreAccess (Socket, Id, Instance, Type, Register, Size,
                       RegisterAddress, RegisterAccessRead);
  }

  AccessFiltered = RatFilterAccess (Socket, Type, RegisterAddress, Size, RegisterAccessRead, &Value);

  if (!AccessFiltered) {
    switch (Type) {
    case RegisterSpaceMsgCr:
    case RegisterSpaceMsgCfg:
    case RegisterSpaceMsgMem:
    case RegisterSpaceMsgIo:
      Value = AddressMapMsgRead (Topology, Socket, AccDesc.Sideband.Address, Register, Size, Type);
      break;

    case RegisterSpaceIoBar:
    case RegisterSpaceIoFixed:
      Value = AddressMapIoRead (Topology, Socket, RegisterAddress, Size);
      break;

    default:
      Value = AddressMapMmioRead (Topology, Socket, RegisterAddress, Size);
      break;
    }
  }

  if (RAT_TRACE_ENABLED) {
    RatTracePostAccess (Socket, Id, Instance, Type, Register, Size,
                         RegisterAddress, RegisterAccessRead, Value);
  }

  return Value;
}

/**
  Writes a value to the requested register within given endpoint instance.

  @param[in] Socket     Socket ID or index
  @param[in] Id         Endpoint ID within SoC
  @param[in] Instance   Instance of endpoint given by Id
  @param[in] Type       Register access space type
  @param[in] Register   Register to write to
  @param[in] Value      Value to be written
  @param[in] Size       Byte width of register

  @retval               None
**/
VOID
EFIAPI
WriteIpRegister (
  IN UINT8                Socket,
  IN UINT64               Id,
  IN UINT16               Instance,
  IN REGISTER_SPACE_TYPE  Type,
  IN UINT32               Register,
  IN UINT64               Value,
  IN REGISTER_WIDTH_BYTES Size
  )
{
  EFI_STATUS            Status;
  UINTN                 RegisterAddress = 0;
  BOOLEAN               AccessFiltered;
  SYSTEM_TOPOLOGY       *Topology;
  ACCESSOR_DESCRIPTOR   AccDesc;

  Topology = SystemTopologyGetDataPointer ();
  if (Topology == NULL) {
    ASSERT (Topology != NULL);
    return;
  }

  Status = SystemTopologyGetAccessor (Topology, Id, Socket, Instance, &Type, &AccDesc, NULL);
  if (!EFI_ERROR (Status)) {
    RegisterAddress = (UINTN)GetIpBaseAddressInternal (Topology, Socket, Id, Instance, Type, &AccDesc);
  }

  if ((VOID*)RegisterAddress == NULL && Type < RegisterSpaceMsgCfg) {
    return; // Register can't be written
  }

  AccessFiltered  = FALSE;
  RegisterAddress = RegisterAddress + Register;

  if (RAT_TRACE_ENABLED) {
    RatTracePreAccess (Socket, Id, Instance, Type, Register, Size, RegisterAddress, RegisterAccessWrite);
  }

  AccessFiltered = RatFilterAccess (Socket, Type, RegisterAddress, Size, RegisterAccessWrite, &Value);

  if (!AccessFiltered) {
    switch (Type) {
    case RegisterSpaceMsgCr:
    case RegisterSpaceMsgCfg:
    case RegisterSpaceMsgMem:
    case RegisterSpaceMsgIo:
      AddressMapMsgWrite (Topology, Socket, AccDesc.Sideband.Address, Register, Value, Size, Type);
      break;

    case RegisterSpaceIoBar:
    case RegisterSpaceIoFixed:
      AddressMapIoWrite (Topology, Socket, RegisterAddress, Value, Size);
      break;

    default:
      AddressMapMmioWrite (Topology, Socket, RegisterAddress, Value, Size);
      break;
    }
  }

  if (RAT_TRACE_ENABLED) {
    RatTracePostAccess (Socket, Id, Instance, Type, Register, Size, RegisterAddress, RegisterAccessWrite, Value);
  }
}

