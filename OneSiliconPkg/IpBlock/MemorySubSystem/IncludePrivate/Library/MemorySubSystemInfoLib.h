/** @file
  Header file for Memory Sub System Info Library

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

#ifndef _PEI_MEMORY_SUB_SYS_INFO_LIB_H_
#define _PEI_MEMORY_SUB_SYS_INFO_LIB_H_

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <MemorySubSystemHob.h>
#include <Library/HobLib.h>
#include <MemorySubSystemHandle.h>
#include <Defines/HostBridgeDefines.h>

#define BarNameLength 30
typedef enum {
  //
  // IGfx
  //
  FlatCcsBaseAddress,
  LMemBarBaseAddress,
  GttMmAdrBaseAddress,
  BdsmBaseAddress,
  GttBaseAddress,
  //
  // VMD
  //
  VmdMemBar2BaseAddress,
  VmdMemBar1BaseAddress,
  VmdCfgBarBaseAddress,
  //
  // Heci
  //
  Heci3BaseAddress,
  Heci2BaseAddress,
  Heci1BaseAddress,
  //
  // TraceHub
  //
  TraceHubBaseAddress,
  TraceHubRTITBaseAddress,
  TraceHubFWBaseAddress,
  TraceHubMTBBaseAddress,
  //
  // P2SB
  //
  SbRegBaseAddress,
  SbReg2BaseAddress,

  RemapBaseAddress,
  StreamTracerBaseAddress,
  BiosAreaBaseAddress,
  TemporaryRamBaseAddress,
  ApicLocalBaseAddress,
  IoApicBaseAddress,
  MchBaseAddress,
  TxtConfigBaseAddress,
  HpetBaseAddress,
  TempMemBaseAddress,
  eSpi2SEGmrBaseAddress,
  eSpi2SLGmrBaseAddress,
  SpiBarBaseAddress,
  SerialIoBarBaseAddress,
  PwrmBarBaseAddress,
  TraceHubSWBarBaseAddress,
  VtdMmioBaseAddress,
  SafBarBaseAddress,
  RegBarBaseAddress,
  PciExpressBaseAddress,
  NvmeHcPeiMmioBaseAddress,
  AhciPeiMmioBaseAddress,
  PrmrrBaseAddress,
  TsegBaseAddress,
  MeStolenBaseAddress,
  TseImrBaseAddress,
  SeamrrBaseAddress,

  MemoryMapTableLength         // Add new entries above it
} MEMORY_RANGES;

typedef struct {
  UINT64             Base;
  UINT64             Limit;
  CHAR16             BaseName[BarNameLength];
} MEMORY_RANGE_TABLE;

/**
  This function returns True if CCE0 Enable in MemSS PMA

  @retval TRUE  - if CCE0 Enable
  @retval FALSE - if CCE0 Disable
**/
BOOLEAN
EFIAPI
IsCce0Enable (
  VOID
  );

/**
  This function returns True if CCE1 Enable in MemSS PMA

  @retval TRUE  - if CCE1 Enable
  @retval FALSE - if CCE1 Disable
**/
BOOLEAN
EFIAPI
IsCce1Enable (
  VOID
  );

/**
  This function returns the Mc hash LSB

  @retval Mc Hash LSB
**/
UINT32
EFIAPI
GetMcHashLsb (
  VOID
  );

/**
  This function returns the Mc hash Mask

  @retval Mc Hash Mask
**/
UINT32
EFIAPI
GetMcHashMask (
  VOID
  );

/**
  This function returns Mc hash is enabled or not.
  @retval Mc Hash Enable
**/
BOOLEAN
EFIAPI
GetMcHashEnable (
  VOID
  );

/**
  Check whether the Base Address is Naturally aligned or not - Base needs to be multiple of size.
  If not then it will return the Naturally aligned Base Address

  @param[in]       EFI_PHYSICAL_ADDRESS MemBaseAddr   - Base Address
  @param[in]       UINT64               MemSize       - Size

  @retval          EFI_PHYSICAL_ADDRESS MemBaseAddr   -  Naturally Aligned Base Address
**/
EFI_PHYSICAL_ADDRESS
NaturalAlignment (
  IN  EFI_PHYSICAL_ADDRESS  MemBaseAddr,
  IN  UINT64                MemSize
  );

/**
  Reserve Memory for I/O Space
**/
VOID
PeiIOMemoryAllocation(
  VOID
  );

/**
  Reserve Memory Below PCIe
**/
VOID
ReserveMmioBelowPciConfig (
  VOID
  );

/**
  Collect memory mapping details and check for overlaps in it

**/
VOID
EFIAPI
CheckMemoryMapOverlap (
  );

#endif
