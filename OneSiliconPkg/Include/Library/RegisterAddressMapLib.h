/** @file
  Abstraction of SoC dependent definitions for register access.

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

  This file contains Silicon register definitions.

  This is a generated file; please do not modify it directly.

@par Specification Reference:
**/

#ifndef __REGISTER_ADDRESS_MAP_LIB_INCLUDES__
#define __REGISTER_ADDRESS_MAP_LIB_INCLUDES__

#include <RegisterAddressTranslation.h>

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
  );

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
  );

/**
  Write data to register via sideband interface.

  @param[in] Topology     Pointer to globl SYSTEM_TOPOLOGY
  @param[in] Socket       Socket number
  @param[in] Address      Sideband address
  @param[in] Offset       Register offset
  @param[in] Data         Data to be written
  @param[in] Size         Size of data to write
  @param[in] Type         Type of sideband access method

  @return None
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
  );

/**
  Returns the value in a Sideband BAR register

  @param[in] Topology   Pointer to system topology data
  @param[in] Socket     Which socket to check
  @param[in] Instance   Index of endpoint instance
  @param[in] Address    Register address
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
  );

#define SOC_ADDRESS_MAP_STRING_TABLE_UNIT   (sizeof(CHAR8 *))

extern UINT8 gSocAddressMapTable[];
extern CHAR8 *gSocAddressMapStringTable[];

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
  );

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
  );

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
  );

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
  );

/**
  Initialize register address map table.

  Usually, this function should prepare initialization data and pass them to
  SystemTopologyInstallAddrMapData().

  @return EFI_UNSUPPORTED if this function is not supported or implemented
  @return EFI_SUCCESS     if the initialization is done successfully
**/
EFI_STATUS
EFIAPI
AddressMapTableInit (
  VOID
  );

/**
  Get pointer to SYSTEM_TOPOLOGY stored in different ways at different phases.

  @return The address of SYSTEM_TOPOLOGY structure stored globally
  @return NULL if error occurred
**/
SYSTEM_TOPOLOGY*
EFIAPI
AddressMapGetTopology (
  VOID
  );

/**
  Set or update the address of SYSTEM_TOPOLOGY data structure.

  Different phases (Pei/Dxe/Runtime/Smm) may have different ways to store this
  address.

  @param[in] Topology   (New) address of system topology data

  @return none
**/
VOID
EFIAPI
AddressMapSetTopology (
  IN SYSTEM_TOPOLOGY    *Topology
  );

#endif  //__REGISTER_ADDRESS_MAP_LIB_INCLUDES__

