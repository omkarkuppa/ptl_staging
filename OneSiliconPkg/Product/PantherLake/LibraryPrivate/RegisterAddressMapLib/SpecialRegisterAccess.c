/** @file
  This file provides functions used to access registers via SoC specific way.

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

#include "SpecialRegisterAccess.h"
#include <Library/BaseLib.h>


/**
  Get the value of register given by index for the sake of sideband interface.

  The register must be known one in address map table.

  @param[in] Topology     Pointer to SYSTEM_TOPOLOGY data structure.
  @param[in] Socket       Socket number.
  @param[in] RegIndex     Index to the requested register.

  @return Value of given register; or
  @return -1 if failed to access the register.
**/
STATIC
UINT64
GetSbReg (
  IN SYSTEM_TOPOLOGY  *Topology,
  IN UINT8            Socket,
  IN UINTN            RegIndex
  )
{
  COMBINED_BAR      *RegInfo;
  UINTN             RegBase;
  UINT64            RegVal;
  PCI_CFG_ACCESSOR  PciCfg;
  UINT64            Value;
  UINTN             Index;

  RegInfo = SystemTopologyGetBarByIndex (Topology, Socket, RegIndex);
  if (RegInfo == NULL || RegInfo->Bar[0].Type != RegisterSpaceCfg) {
    ASSERT (RegInfo != NULL);
    ASSERT (RegInfo->Bar[0].Type == RegisterSpaceCfg);
    return (UINT32)-1;
  }

  Value   = 0;
  RegBase = (UINTN)SystemTopologyGetMmcfgBase (Topology, Socket);
  for (Index = 0; Index < ARRAY_SIZE (RegInfo->Bar); ++Index) {
    PciCfg.Address = (UINT32)(RegInfo->Bar[Index].Register & 0xFFFFFFFF);
    if (PciCfg.Address == (UINT32)-1) {
      continue;
    }

    PciCfg.Bdfo.Bus = Topology->PerSocketData[Socket].BusMap[PciCfg.Bdfo.Bus];
    RegVal = (UINT32)(MmioRead32 (RegBase + PciCfg.Address) & RegInfo->Bar[Index].Mask);
    if (RegInfo->Bar[Index].Shift > 0 && RegInfo->Bar[Index].Shift < 32) {
      RegVal = LShiftU64 (RegVal, RegInfo->Bar[Index].Shift);
    }

    Value |= RegVal;
  }
  SystemTopologySetBarCachedValue (Topology, Socket, RegIndex, Value);
  return Value;
}

/**
  Set value to the register given by index for the sake of sideband interface.

  The register must be known one in address map table.

  @param[in] Topology     Pointer to SYSTEM_TOPOLOGY data structure.
  @param[in] Socket       Socket number.
  @param[in] RegIndex     Index to the requested register.
  @param[in] Value        Value of register to be written.

  @return None
**/
STATIC
VOID
SetSbReg (
  IN SYSTEM_TOPOLOGY  *Topology,
  IN UINT8            Socket,
  IN UINTN            RegIndex,
  IN UINT64           Value
  )
{
  COMBINED_BAR      *RegInfo;
  UINTN             RegAddr;
  PCI_CFG_ACCESSOR  PciCfg;
  UINT8             Index;

  RegInfo = SystemTopologyGetBarByIndex (Topology, Socket, RegIndex);
  if (RegInfo == NULL || RegInfo->Bar[0].Type != RegisterSpaceCfg) {
    ASSERT (RegInfo != NULL);
    ASSERT (RegInfo->Bar[0].Type == RegisterSpaceCfg);
  }

  for (Index = 0; Index < ARRAY_SIZE (RegInfo->Bar); Index++) {
    PciCfg.Address  = (UINT32)(RegInfo->Bar[Index].Register & 0xFFFFFFFF);
    PciCfg.Bdfo.Bus = Topology->PerSocketData[Socket].BusMap[PciCfg.Bdfo.Bus];

    RegAddr = (UINTN)SystemTopologyGetMmcfgBase (Topology, Socket);
    RegAddr += PciCfg.Address;

    MmioWrite32 (RegAddr, (UINT32)RShiftU64(Value & RegInfo->Bar[Index].Mask, RegInfo->Bar[Index].Shift));
  }
}

/**
  Get Sideband access type by given socket number.

  @param[in] Socket       Socket number

  @return Pointer to Sideband access interface structure
**/
STATIC
P2SB_SIDEBAND_REGISTER_ACCESS*
GetP2SbSidebandAccess (
  IN UINT8                Socket
  )
{
  switch (Socket) {
  case 0:
    return (P2SB_SIDEBAND_REGISTER_ACCESS *)(UINTN)GetSystemInfoVar ()->SbiAccess;

  case 1:
    return (P2SB_SIDEBAND_REGISTER_ACCESS *)(UINTN)GetSystemInfoVar ()->PchSbiAccess;

  case 2:
    return (P2SB_SIDEBAND_REGISTER_ACCESS *)(UINTN)GetSystemInfoVar ()->CpuSbiAccess;

  default:
    break;
  }

  return NULL;
}

/**
  Read data from register via sideband interface.

  @param[in] Topology     Pointer to globl SYSTEM_TOPOLOGY
  @param[in] Socket       Socket number
  @param[in] Address      Register address
  @param[in] Offset       Register offset
  @param[in] Size         Size of data to read
  @param[in] Type         Type of sideband access method

  @return Value of data read from given Address
**/
UINT64
EFIAPI
AddressMapMsgRead (
  IN SYSTEM_TOPOLOGY      *Topology,
  IN UINT8                Socket,
  IN UINT64               Address,
  IN UINTN                Offset,
  IN UINTN                Size,
  IN REGISTER_SPACE_TYPE  Type
  )
{
  SIDEBAND_ACCESSOR               SbAcc;
  P2SB_SIDEBAND_REGISTER_ACCESS   *P2SbAccess;
  UINT64                          Value;

  P2SbAccess = GetP2SbSidebandAccess (Socket);
  P2SbAccess->AccessMethod  = P2SbMsgAccess;
  P2SbAccess->Fid           = (UINT16) SbAcc.Bits.SbFid;
  P2SbAccess->P2SbPid       = (UINT8) SbAcc.Bits.PortId;
  P2SbAccess->PostedWrites   = FALSE;

  switch (Type) {
  case RegisterSpaceMsgCfg:
    P2SbAccess->RegisterSpace = P2SbPciConfig;
    break;

  case RegisterSpaceMsgMem:
    P2SbAccess->RegisterSpace = P2SbMemory;
    break;

  case RegisterSpaceMsgCr:
    P2SbAccess->RegisterSpace = P2SbPrivateConfig;
    break;
  }

  switch (Size) {
  case RegisterWidthByte:
    Value = P2SbAccess->Access.Read8 (&P2SbAccess->Access, (UINT32) Offset);
    break;

  case RegisterWidthWord:
    Value = P2SbAccess->Access.Read16 (&P2SbAccess->Access, (UINT32) Offset);
    break;

  case RegisterWidthDWord:
    Value = P2SbAccess->Access.Read32 (&P2SbAccess->Access, (UINT32) Offset);
    break;

  default:
    ASSERT (Size < RegisterWidthQWord);
    Value = (UINT64)-1;
    break;
  }
  return Value;
}

/**
  Write data to register via sideband interface.

  @param[in] Topology     Pointer to globl SYSTEM_TOPOLOGY
  @param[in] Socket       Socket number
  @param[in] Address      Sideband address
  @param[in] Offset       Register offset
  @param[in] Data         Data to be written
  @param[in] Size         Size of data to write
  @param[in] Type         Type of sideband access method

  @return STATIC void
**/
VOID
EFIAPI
AddressMapMsgWrite (
  IN SYSTEM_TOPOLOGY      *Topology,
  IN UINT8                Socket,
  IN UINT64               Address,
  IN UINTN                Offset,
  IN UINT64               Data,
  IN UINTN                Size,
  IN REGISTER_SPACE_TYPE  Type
  )
{
  SIDEBAND_ACCESSOR               SbAcc;
  P2SB_SIDEBAND_REGISTER_ACCESS   *P2SbAccess;

  P2SbAccess = GetP2SbSidebandAccess (Socket);
  P2SbAccess->AccessMethod  = P2SbMsgAccess;
  P2SbAccess->Fid           = (UINT16) SbAcc.Bits.SbFid;
  P2SbAccess->P2SbPid       = (UINT8) SbAcc.Bits.PortId;
  P2SbAccess->PostedWrites   = FALSE;

  switch (Type) {
  case RegisterSpaceMsgCfg:
    P2SbAccess->RegisterSpace = P2SbPciConfig;
    break;

  case RegisterSpaceMsgMem:
    P2SbAccess->RegisterSpace = P2SbMemory;
    break;

  case RegisterSpaceMsgCr:
    P2SbAccess->RegisterSpace = P2SbPrivateConfig;
    break;
  }

  switch (Size) {
  case RegisterWidthByte:
    P2SbAccess->Access.Write8 (&P2SbAccess->Access, (UINT32) Offset, (UINT8)Data);
    break;

  case RegisterWidthWord:
    P2SbAccess->Access.Write16 (&P2SbAccess->Access, (UINT32) Offset, (UINT16)Data);
    break;

  case RegisterWidthDWord:
    P2SbAccess->Access.Write32 (&P2SbAccess->Access, (UINT32) Offset, (UINT32)Data);
    break;

  default:
    ASSERT (Size < RegisterWidthQWord);
    break;
  }
}

/**
  Returns the value in a Sideband BAR register

  @param[in] Topology   Pointer to system topology data
  @param[in] Socket     Which socket to check
  @param[in] Instance   Index of endpoint instance
  @param[in] Address    Sideband address
  @param[in] Type       Type of sideband access method

  @retval   The sideband base address of given endpoint instance
**/
UINT64
EFIAPI
AddressMapMsgGetBase (
  IN SYSTEM_TOPOLOGY      *Topology,
  IN UINT8                Socket,
  IN UINT16               Instance,
  IN UINT64               Address,
  IN REGISTER_SPACE_TYPE  Type
  )
{
  SIDEBAND_ACCESSOR               SbAcc;
  UINT64                          BaseAddress;
  UINT64                          Value;

  //
  // Publish PortId/SbBar/SbFid.
  //
  switch (Type) {
  case RegisterSpaceMsgCr:
    //
    // Retrieve sideband base.
    //
    Value = (UINTN)SystemTopologyGetBarCachedValue (Topology, Socket, TYPE_SBREG_BAR);
    if (Value == (UINTN)-1) {
      Value = GetSbReg (Topology, Socket, TYPE_SBREG_BAR);
    }
    Value += LShiftU64 (SbAcc.Bits.PortId, 16);
    BaseAddress = Value;

    break;

  case RegisterSpaceMsgCfg:
  case RegisterSpaceMsgMem:
  case RegisterSpaceMsgIo:
    BaseAddress = 0;
    break;

  default:
    ASSERT (FALSE);
    BaseAddress = 0;
  }

  return BaseAddress;
}

/**
  Update address map table with bus number assigned for given stack and socket.

  @param[in] Socket       Socket number
  @param[in] StackId      Stack ID
  @param[in] LogicalBus   Bus number assigned for StackId and Socket

  @return None
**/
VOID
EFIAPI
AddressMapUpdateBus (
  IN UINT8 Socket,
  IN UINT8 StackId,
  IN UINT8 LogicalBus
  )
{
}

