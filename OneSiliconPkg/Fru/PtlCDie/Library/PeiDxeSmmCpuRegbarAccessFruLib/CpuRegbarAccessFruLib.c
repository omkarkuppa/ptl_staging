/** @file
  CPU REGBAR ACCESS FRU library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuRegbarAccessLib.h>

/**
  Definition for REGBAR address
  The REGBAR address is used for the CPU IP's SB register access
**/
#define CPU_REGBAR_ADDRESS(Pid, CpuSbDeviceFid, Offset)    (PcdGet64(PcdRegBarBaseAddress) | ((UINT8)(Pid) << 19) | ((UINT8)(CpuSbDeviceFid) << 16) | (UINT16)(Offset))
/**
Definition for REGBAR address of NOC Range IPs
The REGBAR address is used for the CPU IP's SB register access of NOC Range IPs
**/
#define CPU_REGBAR_ADDRESS_NOC(Pid, Offset)    (PcdGet64(PcdRegBarBaseAddress) | ((UINT8)(Pid) << 19) | Offset)


/**
  Read REGBAR register.
  It returns REGBAR register and size in 8bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID

  @retval     UINT64              REGBAR register value.
**/
UINT64
CpuRegbarRead64Fru (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset
  )
{
  if (CpuSbDevicePid != INVALID_PID) {
    //
    // If IPs are in NOC range, then Offset would be of [18:0] otherwise Offset would be of [15:0]
    //
    if ((Offset & 0xFFFF0000) == 0) {
      return MmioRead64 (CPU_REGBAR_ADDRESS (CpuSbDevicePid, CpuSbDeviceFid, Offset));
    } else {
      return MmioRead64 (CPU_REGBAR_ADDRESS_NOC (CpuSbDevicePid, Offset));
    }
  } else {
    return INVALID_DATA_64;
  }
}


/**
  Read REGBAR register.
  It returns REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID

  @retval     UINT32              REGBAR register value.
**/
UINT32
CpuRegbarRead32Fru (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset
  )
{
  if (CpuSbDevicePid != INVALID_PID) {
    //
    // If IPs are in NOC range, then Offset would be of [18:0] otherwise Offset would be of [15:0]
    //
    if ((Offset & 0xFFFF0000) == 0) {
      return MmioRead32 ((CPU_REGBAR_ADDRESS (CpuSbDevicePid, CpuSbDeviceFid, Offset)));
    } else {
      return MmioRead32 (CPU_REGBAR_ADDRESS_NOC (CpuSbDevicePid, Offset));
    }
  } else {
    return INVALID_DATA_32;
  }
}

/**
  Read REGBAR register.
  It returns REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID

  @retval     UINT16              REGBAR register value.
**/
UINT16
CpuRegbarRead16Fru (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset
  )
{
  if (CpuSbDevicePid != INVALID_PID) {
    //
    // If IPs are in NOC range, then Offset would be of [18:0] otherwise Offset would be of [15:0]
    //
    if ((Offset & 0xFFFF0000) == 0) {
      return MmioRead16 (CPU_REGBAR_ADDRESS (CpuSbDevicePid, CpuSbDeviceFid, Offset));
    } else {
      return MmioRead16 (CPU_REGBAR_ADDRESS_NOC (CpuSbDevicePid, Offset));
    }
  } else {
    return INVALID_DATA_16;
  }
}

/**
  Read REGBAR register.
  It returns REGBAR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID

  @retval     UINT8               REGBAR regsiter value
**/
UINT8
CpuRegbarRead8Fru (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset
  )
{
  if (CpuSbDevicePid != INVALID_PID) {
    //
    // If IPs are in NOC range, then Offset would be of [18:0] otherwise Offset would be of [15:0]
    //
    if ((Offset & 0xFFFF0000) == 0) {
      return MmioRead8 (CPU_REGBAR_ADDRESS (CpuSbDevicePid, CpuSbDeviceFid, Offset));
    } else {
      return MmioRead8 (CPU_REGBAR_ADDRESS_NOC (CpuSbDevicePid, Offset));
    }
  } else {
    return INVALID_DATA_8;
  }
}


/**
  Write REGBAR register.
  It programs REGBAR register and size in 8bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID
  @param[in]  Data                Input Data. Must be the same size as Size parameter.

  @retval     UINT64              Value written to register
**/
UINT64
CpuRegbarWrite64Fru (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT64                            Data
  )
{
  if (CpuSbDevicePid != INVALID_PID) {
    if ((Offset & 0xFFFF0000) == 0) {
      //
      // If IPs are in NOC range, then Offset would be of [18:0] otherwise Offset would be of [15:0]
      //
      MmioWrite64 (CPU_REGBAR_ADDRESS (CpuSbDevicePid, CpuSbDeviceFid, Offset), Data);
      return Data;
    } else {
      MmioWrite64 (CPU_REGBAR_ADDRESS_NOC (CpuSbDevicePid, Offset), Data);
      return Data;
    }
  } else {
    return INVALID_DATA_64;
  }
}


/**
  Write REGBAR register.
  It programs REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID
  @param[in]  Data                Input Data. Must be the same size as Size parameter.

  @retval     UINT32              Value written to register
**/
UINT32
CpuRegbarWrite32Fru (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT32                            Data
  )
{
  if (CpuSbDevicePid != INVALID_PID) {
    if ((Offset & 0xFFFF0000) == 0) {
      //
      // If IPs are in NOC range, then Offset would be of [18:0] otherwise Offset would be of [15:0]
      //
      return MmioWrite32 (CPU_REGBAR_ADDRESS (CpuSbDevicePid, CpuSbDeviceFid, Offset), Data);
    } else {
      return MmioWrite32 (CPU_REGBAR_ADDRESS_NOC (CpuSbDevicePid, Offset), Data);
    }
  } else {
    return INVALID_DATA_32;
  }
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID
  @param[in]  Data                Input Data. Must be the same size as Size parameter.

  @retval     UINT16              Value written to register
**/
UINT16
CpuRegbarWrite16Fru (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT16                            Data
  )
{
  if (CpuSbDevicePid != INVALID_PID) {
    if ((Offset & 0xFFFF0000) == 0) {
      //
      // If IPs are in NOC range, then Offset would be of [18:0] otherwise Offset would be of [15:0]
      //
      return MmioWrite16 (CPU_REGBAR_ADDRESS (CpuSbDevicePid, CpuSbDeviceFid, Offset), Data);
    } else {
      return MmioWrite16 (CPU_REGBAR_ADDRESS_NOC (CpuSbDevicePid, Offset), Data);
    }
  } else {
    return INVALID_DATA_16;
  }
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID
  @param[in]  Data                Input Data. Must be the same size as Size parameter.

  @retval     UINT8               Value written to register
**/
UINT8
CpuRegbarWrite8Fru (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT8                             Data
  )
{
  if (CpuSbDevicePid != INVALID_PID) {
    if ((Offset & 0xFFFF0000) == 0) {
      //
      // If IPs are in NOC range, then Offset would be of [18:0] otherwise Offset would be of [15:0]
      //
      return MmioWrite8 (CPU_REGBAR_ADDRESS (CpuSbDevicePid, CpuSbDeviceFid, Offset), Data);
    } else {
      return MmioWrite8 (CPU_REGBAR_ADDRESS_NOC (CpuSbDevicePid, Offset), Data);
    }
  } else {
    return INVALID_DATA_8;
  }
}
