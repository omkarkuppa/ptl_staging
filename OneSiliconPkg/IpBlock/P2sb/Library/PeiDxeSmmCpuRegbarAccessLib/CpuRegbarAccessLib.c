/** @file
  CPU REGBAR ACCESS library.
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

#include <Library/CpuRegbarAccessLib.h>

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
CpuRegbarRead64 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset
  )
{
  return CpuRegbarRead64Fru (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset);
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
CpuRegbarRead32 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset
  )
{
  return CpuRegbarRead32Fru (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset);
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
CpuRegbarRead16 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset
  )
{
  return CpuRegbarRead16Fru (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset);
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
CpuRegbarRead8 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset
  )
{
  return CpuRegbarRead8Fru (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset);
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
CpuRegbarWrite64 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT64                            Data
  )
{
  return CpuRegbarWrite64Fru (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, Data);
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
CpuRegbarWrite32 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT32                            Data
  )
{
  return CpuRegbarWrite32Fru (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, Data);
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
CpuRegbarWrite16 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT16                            Data
  )
{
  return CpuRegbarWrite16Fru (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, Data);
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
CpuRegbarWrite8 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT8                             Data
  )
{
  return CpuRegbarWrite8Fru (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, Data);
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID
  @param[in]  OrData              OR Data. Must be the same size as Size parameter.

  @retval     UINT32              Value written to register

**/
UINT32
CpuRegbarOr32 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT32                            OrData
  )
{
  return CpuRegbarWrite32 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, CpuRegbarRead32 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset) | OrData);
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID
  @param[in]  OrData              OR Data. Must be the same size as Size parameter.

  @retval     UINT16              Value written to register

**/
UINT16
CpuRegbarOr16 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT16                            OrData
  )
{
  return CpuRegbarWrite16 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, CpuRegbarRead16 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset) | OrData);
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID
  @param[in]  OrData              OR Data. Must be the same size as Size parameter.

  @retval     UINT8               Value written to register

**/
UINT8
CpuRegbarOr8 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT8                             OrData
  )
{
  return CpuRegbarWrite8 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, CpuRegbarRead8 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset) | OrData);
}

/**
  Performs a bitwise AND of a 32-bit data.
  It programs REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID
  @param[in]  AndData             And Data. Must be the same size as Size parameter.

  @retval     UINT32              Value written to register

**/
UINT32
CpuRegbarAnd32 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT32                            AndData
  )
{
  return CpuRegbarWrite32 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, CpuRegbarRead32 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset) & AndData);
}

/**
  Performs a bitwise AND of a 16-bit data.
  It programs REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID
  @param[in]  AndData             And Data. Must be the same size as Size parameter.

  @retval     UINT16              Value written to register

**/
UINT16
CpuRegbarAnd16 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT16                            AndData
  )
{
  return CpuRegbarWrite16 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, CpuRegbarRead16 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset) & AndData);
}

/**
Performs a bitwise AND of a 8-bit data.
It programs REGBAR register and size in 1byte.
The Offset should not exceed 0xFFFF and must be aligned with size.

@param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
@param[in]  CpuSbDevicePid      CPU SB Device Port ID
@param[in]  CpuSbDeviceFid      CPU SB Device Function ID
@param[in]  Offset              Register offset of this Port ID
@param[in]  AndData             And Data. Must be the same size as Size parameter.

@retval     UINT8               Value written to register

**/
UINT8
CpuRegbarAnd8 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT8                             AndData
  )
{
  return CpuRegbarWrite8 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, CpuRegbarRead8 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset) & AndData);
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID
  @param[in]  AndData             AND Data. Must be the same size as Size parameter.
  @param[in]  OrData              OR Data. Must be the same size as Size parameter.

  @retval     UINT32              Value written to register

**/
UINT32
CpuRegbarAndThenOr32 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  )
{
  return CpuRegbarWrite32(CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, (CpuRegbarRead32(CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset) & AndData) | OrData);
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID
  @param[in]  AndData             AND Data. Must be the same size as Size parameter.
  @param[in]  OrData              OR Data. Must be the same size as Size parameter.

  @retval     UINT16              Value written to register

**/
UINT16
CpuRegbarAndThenOr16 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT16                            AndData,
  IN  UINT16                            OrData
  )
{
  return CpuRegbarWrite16 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, (CpuRegbarRead16 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset) & AndData) | OrData);
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDeviceDie      CPU SB Device Port Connected Die (0 - SOC Die, 1 - BASE Die)
  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  CpuSbDeviceFid      CPU SB Device Function ID
  @param[in]  Offset              Register offset of this Port ID
  @param[in]  AndData             AND Data. Must be the same size as Size parameter.
  @param[in]  OrData              OR Data. Must be the same size as Size parameter.

  @retval     UINT8               Value written to register

**/
UINT8
CpuRegbarAndThenOr8 (
  IN  CPU_SB_DEVICE_DIE                 CpuSbDeviceDie,
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  CPU_SB_DEVICE_FID                 CpuSbDeviceFid,
  IN  UINT32                            Offset,
  IN  UINT8                             AndData,
  IN  UINT8                             OrData
  )
{
  return CpuRegbarWrite8 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDeviceFid, Offset, (CpuRegbarRead8 (CpuSbDeviceDie, CpuSbDevicePid, CpuSbDevicePid, Offset) & AndData) | OrData);
}
