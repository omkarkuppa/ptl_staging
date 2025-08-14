/** @file
  @This file implements the functions to get NGU info.

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

#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/NguInfoLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Register/NocRegs.h>
#include <Register/HostBridgeRegs.h>
#include <Library/DebugLib.h>
#include <MemorySubSystemHob.h>
#include <Library/HobLib.h>
#include <Library/BaseLib.h>

/**
  GetImrPrmrrBaseOffset: Get IMR PRMRR Base Offset value

  @retval IMR PRMRR Base Offset
**/
UINT32
EFIAPI
GetImrPrmrrBaseOffset (
  VOID
  )
{
  return (UINT32) GET_NOC_EFFECTIVE_ADDRESS (IMR17BASE_IMPH_IOC_MCHBAR_REG);
}

/**
  GetImrPrmrrBaseOffset: Get IMR PRMRR Mask Offset value

  @retval IMR PRMRR Mask Offset
**/
UINT32
EFIAPI
GetImrPrmrrMaskOffset (
  VOID
  )
{
  return (UINT32) GET_NOC_EFFECTIVE_ADDRESS (IMR17MASK_IMPH_IOC_MCHBAR_REG);
}

/**
  GetImrGsmBaseOffset: Get IMR GSM Base Offset value

  @retval IMR GSM Base Offset
**/
UINT64
EFIAPI
GetImrGsmBaseOffset (
  VOID
  )
{
  UINT64                             ImrGsmBase;
  IMR1M2BASE_IMPH_IOC_MCHBAR_STRUCT GsmBaseStruct;

  GsmBaseStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M2BASE_IMPH_IOC_MCHBAR_REG))));
  ImrGsmBase = (GsmBaseStruct.Bits.imr_base_rw << IMR1M2BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);

  return ImrGsmBase;
}

/**
  GetImrDsmBaseOffset: Get IMR DSM Base Offset value

  @retval IMR DSM Base Offset
**/
UINT64
EFIAPI
GetImrDsmBaseOffset (
  VOID
  )
{
  UINT64                             ImrDsmBase;
  IMR1M3BASE_IMPH_IOC_MCHBAR_STRUCT DsmBaseStruct;

  DsmBaseStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M3BASE_IMPH_IOC_MCHBAR_REG))));
  ImrDsmBase = (DsmBaseStruct.Bits.imr_base_rw << IMR1M3BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);

  return ImrDsmBase;
}

/**
  GetImrTsegBaseOffset: Get IMR TSEG Base Offset value

  @retval IMR TSEG Base Offset
**/
UINT64
EFIAPI
GetImrTsegBaseOffset (
  VOID
  )
{
  UINT64                              ImrTsegBase;
  IMR1M10BASE_IMPH_IOC_MCHBAR_STRUCT TsegBaseStruct;

  TsegBaseStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M10BASE_IMPH_IOC_MCHBAR_REG))));
  ImrTsegBase = (TsegBaseStruct.Bits.imr_base_rw << IMR1M10BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);

  return ImrTsegBase;
}

/**
  GetImrCcsBase: Get IMR CCS Base address

  @retval IMR CCS Base address
**/
UINT64
EFIAPI
GetImrCcsBase (
  VOID
  )
{
  UINT64                         ImrCcsBase;
  IMR1M6BASE_IMPH_IOC_MCHBAR_STRUCT ImrCcsStruct;

  ImrCcsStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M6BASE_IMPH_IOC_MCHBAR_REG))));
  ImrCcsBase = LShiftU64 ((UINT64)ImrCcsStruct.Bits.imr_base_rw, IMR1M6BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);
  return ImrCcsBase;
}

/**
  GetImr1m10BaseOffset: Get IMR1M10 Base Offset value

  @retval IMR1M10 Base Offset
**/
UINT32
EFIAPI
GetImr1m10BaseOffset (
  VOID
  )
{
  return (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M10BASE_IMPH_IOC_MCHBAR_REG);
}

/**
  GetImr1m10MaskOffset: Get IMR1M10 Mask Offset

  @retval IMR1M10 Mask Offset
**/
UINT32
EFIAPI
GetImr1m10MaskOffset (
  VOID
  )
{
  return (UINT32) (GET_NOC_EFFECTIVE_ADDRESS(IMR1M10BASE_IMPH_IOC_MCHBAR_REG) + 0x8);
}

/**
  GetImrGsmMask: Get IMR GSM mask value

  @retval IMR GSM mask
**/
UINT64
EFIAPI
GetImrGsmMask (
  VOID
  )
{
  UINT64                              ImrGsmMask;
  IMR1M2LIMIT_IMPH_IOC_MCHBAR_STRUCT  GsmLimitStruct;

  GsmLimitStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M2LIMIT_IMPH_IOC_MCHBAR_REG))));
  ImrGsmMask = (GsmLimitStruct.Bits.imr_limit_rw << IMR1M2LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);

  return ImrGsmMask;
}

/**
  GetImrDsmMask: Get IMR DSM mask value

  @retval IMR DSM mask
**/
UINT64
EFIAPI
GetImrDsmMask (
  VOID
  )
{
  UINT64                              ImrDsmMask;
  IMR1M3LIMIT_IMPH_IOC_MCHBAR_STRUCT  DsmLimitStruct;

  DsmLimitStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M3LIMIT_IMPH_IOC_MCHBAR_REG))));
  ImrDsmMask = (DsmLimitStruct.Bits.imr_limit_rw << IMR1M3LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);

  return ImrDsmMask;
}

/**
  GetImrTsegMask: Get IMR TSEG mask value

  @retval IMR TSEG mask
**/
UINT64
EFIAPI
GetImrTsegMask (
  VOID
  )
{
  UINT64                               ImrTsegMask;
  IMR1M10LIMIT_IMPH_IOC_MCHBAR_STRUCT  TsegLimitStruct;

  TsegLimitStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M10LIMIT_IMPH_IOC_MCHBAR_REG))));
  ImrTsegMask = (TsegLimitStruct.Bits.imr_limit_rw << IMR1M10LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);

  return ImrTsegMask;
}

/**
  GetImrCcsMask: Get IMR CCS mask value

  @retval IMR CCS mask
**/
UINT64
EFIAPI
GetImrCcsMask (
  VOID
  )
{
  UINT64                             ImrCcsMask;
  IMR1M6LIMIT_IMPH_IOC_MCHBAR_STRUCT CcsLimitStruct;

  CcsLimitStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M6LIMIT_IMPH_IOC_MCHBAR_REG))));
  ImrCcsMask = (CcsLimitStruct.Bits.imr_limit_rw << IMR1M6LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);

  return ImrCcsMask;
}

/**
  GetImrDprBase: Get IMR DPR Base address

  @retval IMR DPR Base address
**/
UINT64
EFIAPI
GetImrDprBase (
  VOID
  )
{
  UINT64                             ImrDprBase;
  IMR1M9BASE_IMPH_IOC_MCHBAR_STRUCT  DprBaseStruct;

  DprBaseStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M9BASE_IMPH_IOC_MCHBAR_REG))));
  ImrDprBase = (DprBaseStruct.Bits.imr_base_rw << IMR1M9BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);

  return ImrDprBase;
}

/**
  GetImrDprMask: Get IMR DPR mask value

  @retval IMR DPR mask
**/
UINT64
EFIAPI
GetImrDprMask (
  VOID
  )
{
  UINT64                              ImrDprMask;
  IMR1M9LIMIT_IMPH_IOC_MCHBAR_STRUCT  DprLimitStruct;

  DprLimitStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M9LIMIT_IMPH_IOC_MCHBAR_REG))));
  ImrDprMask = (DprLimitStruct.Bits.imr_limit_rw << IMR1M9LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);

  return ImrDprMask;
}

/**
  This function returns the Remap Base value

  @retval Remap Base
**/
UINT64
EFIAPI
GetRemapBase (
  VOID
  )
{
  UINT32                        MchBar;

  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);

  return MmioRead64 (MchBar + GET_HBO_EFFECTIVE_ADDRESS (REMAP_BASE_HBO_MEM_REG));
}

/**
  This function returns the Remap Limit value

  @retval Remap Limit
**/
UINT64
EFIAPI
GetRemapLimit (
  VOID
  )
{
  UINT32                        MchBar;

  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  return MmioRead64 (MchBar + GET_HBO_EFFECTIVE_ADDRESS (REMAP_LIMIT_HBO_MEM_REG));
}

/**
  This function is provide Imr related details (Base, Mask, Limit).

  @param[in] ImrName     - IMR name or ID
  @param[in] ImrDataType - Requested data (Base, Mask, Limit)

  @retval UINT64           - Register Details based on DataType

**/
UINT64
EFIAPI
GetImrRegisterData (
  IN NGU_IMR              ImrName,
  IN NGU_IMR_DATATYPE     ImrDataType
  )
{

  UINT64                             MchBar;
  IMR15BASE_IMPH_IOC_MCHBAR_STRUCT   NpkBaseStruct;
  IMR15MASK_IMPH_IOC_MCHBAR_STRUCT   NpkMaskStruct;
  IMR17BASE_IMPH_IOC_MCHBAR_STRUCT   PrmrrBaseStruct;
  IMR17MASK_IMPH_IOC_MCHBAR_STRUCT   PrmrrMaskStruct;
  IMR21BASE_IMPH_IOC_MCHBAR_STRUCT   TseBaseStruct;
  IMR21MASK_IMPH_IOC_MCHBAR_STRUCT   TseMaskStruct;
  IMR25BASE_IMPH_IOC_MCHBAR_STRUCT   StreamBaseStruct;
  IMR25MASK_IMPH_IOC_MCHBAR_STRUCT   StreamMaskStruct;
  IMR26BASE_IMPH_IOC_MCHBAR_STRUCT   SeamrrBaseStruct;
  IMR26MASK_IMPH_IOC_MCHBAR_STRUCT   SeamrrMaskStruct;

  IMR1M2BASE_IMPH_IOC_MCHBAR_STRUCT   Imr1m2Base;
  IMR1M2LIMIT_IMPH_IOC_MCHBAR_STRUCT  Imr1m2Limit;
  IMR1M3BASE_IMPH_IOC_MCHBAR_STRUCT   Imr1m3Base;
  IMR1M3LIMIT_IMPH_IOC_MCHBAR_STRUCT  Imr1m3Limit;
  IMR1M6BASE_IMPH_IOC_MCHBAR_STRUCT   Imr1m6Base;
  IMR1M6LIMIT_IMPH_IOC_MCHBAR_STRUCT  Imr1m6Limit;
  IMR1M9BASE_IMPH_IOC_MCHBAR_STRUCT   Imr1m9Base;
  IMR1M9LIMIT_IMPH_IOC_MCHBAR_STRUCT  Imr1m9Limit;
  IMR1M10BASE_IMPH_IOC_MCHBAR_STRUCT  Imr1m10Base;
  IMR1M10LIMIT_IMPH_IOC_MCHBAR_STRUCT Imr1m10Limit;
  MEMORY_SUB_SYSTEM_DATA_HOB          *MemorySubSystemHob;

  //
  // Get the MCHBAR value to program IMRs.
  //
  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  switch (ImrName) {
  case NPK:
    if (ImrDataType == IMR_BASE) {
      NpkBaseStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR15BASE_IMPH_IOC_MCHBAR_REG));
      return (UINT64) NpkBaseStruct.Data;
    } else if (ImrDataType == IMR_MASK) {
      NpkMaskStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR15MASK_IMPH_IOC_MCHBAR_REG));
      return (UINT64) NpkMaskStruct.Bits.imr_mask_rw;
    } else if (ImrDataType == IMR_SIZE) {
      NpkMaskStruct.Data = (MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS(IMR15MASK_IMPH_IOC_MCHBAR_REG)));
      return (UINT64) (~(UINT64)NpkMaskStruct.Data) + 1;
    } else if (ImrDataType == IMR_BASE_BYTE) {
      NpkBaseStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR15BASE_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)NpkBaseStruct.Bits.imr_base_rw, IMR15BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);
    } else if (ImrDataType == IMR_SIZE_BYTE) {
      NpkMaskStruct.Data = (MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS(IMR15MASK_IMPH_IOC_MCHBAR_REG)));
      NpkMaskStruct.Bits.imr_mask_rw = ~(UINT64)NpkMaskStruct.Bits.imr_mask_rw;
      return LShiftU64 ((UINT64)(NpkMaskStruct.Bits.imr_mask_rw + 1), IMR15MASK_IMPH_IOC_MCHBAR_IMR_MASK_RW_LSB);
    } else {
      DEBUG ((DEBUG_INFO, "Invalid Imr Register Requested\n"));
      return MAX_UINT64;
    }
  case PRMRR:
    if (ImrDataType == IMR_BASE) {
      PrmrrBaseStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR17BASE_IMPH_IOC_MCHBAR_REG));
      return (UINT64) PrmrrBaseStruct.Data;
    } else if (ImrDataType == IMR_BASE_RAW) {
      PrmrrMaskStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR17MASK_IMPH_IOC_MCHBAR_REG));
      return (UINT64) PrmrrMaskStruct.Data;
    } else if (ImrDataType == IMR_MASK) {
      PrmrrMaskStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR17MASK_IMPH_IOC_MCHBAR_REG));
      return (UINT64) PrmrrMaskStruct.Bits.imr_mask_rw;
    } else if (ImrDataType == IMR_SIZE) {
      PrmrrMaskStruct.Data = (MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS(IMR17MASK_IMPH_IOC_MCHBAR_REG)));
      return (UINT64) (~(UINT64)PrmrrMaskStruct.Data) + 1;
    } else if (ImrDataType == IMR_BASE_BYTE) {
      PrmrrBaseStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR17BASE_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)PrmrrBaseStruct.Bits.imr_base_rw, IMR17BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);
    } else if (ImrDataType == IMR_MASK_BYTE) {
      PrmrrMaskStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR17MASK_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)PrmrrMaskStruct.Bits.imr_mask_rw, IMR17MASK_IMPH_IOC_MCHBAR_IMR_MASK_RW_LSB);
    } else if (ImrDataType == IMR_SIZE_BYTE) {
      PrmrrMaskStruct.Data = (MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS(IMR17MASK_IMPH_IOC_MCHBAR_REG)));
      PrmrrMaskStruct.Bits.imr_mask_rw = ~(UINT64) PrmrrMaskStruct.Bits.imr_mask_rw;
      return LShiftU64 ((UINT64)(PrmrrMaskStruct.Bits.imr_mask_rw + 1),IMR17MASK_IMPH_IOC_MCHBAR_IMR_MASK_RW_LSB);
    } else {
      DEBUG ((DEBUG_INFO, "Invalid Imr Register Requested\n"));
      return MAX_UINT64;
    }
  case TSE:
    if (ImrDataType == IMR_BASE) {
      TseBaseStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR21BASE_IMPH_IOC_MCHBAR_REG));
      return (UINT64) TseBaseStruct.Data;
    } else if (ImrDataType == IMR_MASK) {
      TseMaskStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR21MASK_IMPH_IOC_MCHBAR_REG));
      return (UINT64) TseMaskStruct.Bits.imr_mask_rw;
    } else if (ImrDataType == IMR_SIZE) {
      TseMaskStruct.Data = (MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS(IMR21MASK_IMPH_IOC_MCHBAR_REG)));
      return (UINT64) (~(UINT64)TseMaskStruct.Data) + 1;
    } else if (ImrDataType == IMR_BASE_BYTE) {
      TseBaseStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR21BASE_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)TseBaseStruct.Bits.imr_base_rw, IMR21BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);
    } else if (ImrDataType == IMR_SIZE_BYTE) {
      TseMaskStruct.Data = (MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS(IMR21MASK_IMPH_IOC_MCHBAR_REG)));
      TseMaskStruct.Bits.imr_mask_rw = ~(UINT64) TseMaskStruct.Bits.imr_mask_rw;
      return LShiftU64 ((UINT64)(TseMaskStruct.Bits.imr_mask_rw + 1), IMR21MASK_IMPH_IOC_MCHBAR_IMR_MASK_RW_LSB);
    } else {
      DEBUG ((DEBUG_INFO, "Invalid Imr Register Requested\n"));
      return MAX_UINT64;
    }
  case STREAM_TRACER:
    if (ImrDataType == IMR_BASE) {
      StreamBaseStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR25BASE_IMPH_IOC_MCHBAR_REG));
      return (UINT64) StreamBaseStruct.Data;
    } else if (ImrDataType == IMR_MASK) {
      StreamMaskStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR25MASK_IMPH_IOC_MCHBAR_REG));
      return (UINT64) StreamMaskStruct.Bits.imr_mask_rw;
    } else if (ImrDataType == IMR_SIZE) {
      StreamMaskStruct.Data = (MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS(IMR25MASK_IMPH_IOC_MCHBAR_REG)));
      return (UINT64) (~(UINT64)StreamMaskStruct.Data) + 1;
    } else if (ImrDataType == IMR_BASE_BYTE) {
      StreamBaseStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR25BASE_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)StreamBaseStruct.Bits.imr_base_rw, IMR25BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);
    } else if (ImrDataType == IMR_SIZE_BYTE) {
      StreamMaskStruct.Data = (MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS(IMR25MASK_IMPH_IOC_MCHBAR_REG)));
      StreamMaskStruct.Bits.imr_mask_rw = ~(UINT64) StreamMaskStruct.Bits.imr_mask_rw;
      return LShiftU64 ((UINT64)(StreamMaskStruct.Bits.imr_mask_rw + 1), IMR25MASK_IMPH_IOC_MCHBAR_IMR_MASK_RW_LSB);
    } else {
      DEBUG ((DEBUG_INFO, "Invalid Imr Register Requested\n"));
      return MAX_UINT64;
    }
  case SEAMRR:
    if (ImrDataType == IMR_BASE) {
      SeamrrBaseStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR26BASE_IMPH_IOC_MCHBAR_REG));
      return (UINT64) SeamrrBaseStruct.Data;
    } else if (ImrDataType == IMR_BASE_RAW) {
      SeamrrMaskStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR26MASK_IMPH_IOC_MCHBAR_REG));
      return (UINT64) SeamrrMaskStruct.Data;
    } else if (ImrDataType == IMR_MASK) {
      SeamrrMaskStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR26MASK_IMPH_IOC_MCHBAR_REG));
      return (UINT64) SeamrrMaskStruct.Bits.imr_mask_rw;
    } else if (ImrDataType == IMR_SIZE) {
      SeamrrMaskStruct.Data = (MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS(IMR26MASK_IMPH_IOC_MCHBAR_REG)));
      return (UINT64) (~(UINT64)SeamrrMaskStruct.Data) + 1;
    } else if (ImrDataType == IMR_BASE_BYTE) {
      SeamrrBaseStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR26BASE_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)SeamrrBaseStruct.Bits.imr_base_rw, IMR26BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);
    } else if (ImrDataType == IMR_MASK_BYTE) {
      SeamrrMaskStruct.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR26MASK_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)SeamrrMaskStruct.Bits.imr_mask_rw, IMR26MASK_IMPH_IOC_MCHBAR_IMR_MASK_RW_LSB);
    } else if (ImrDataType == IMR_SIZE_BYTE) {
      SeamrrMaskStruct.Data = (MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS(IMR26MASK_IMPH_IOC_MCHBAR_REG)));
      SeamrrMaskStruct.Bits.imr_mask_rw = ~(UINT64) SeamrrMaskStruct.Bits.imr_mask_rw;
      return LShiftU64 ((UINT64)(SeamrrMaskStruct.Bits.imr_mask_rw + 1), IMR26MASK_IMPH_IOC_MCHBAR_IMR_MASK_RW_LSB);
    } else {
      DEBUG ((DEBUG_INFO, "Invalid Imr Register Requested\n"));
      return MAX_UINT64;
    }
  case GSM:
    if (ImrDataType == IMR_BASE) {
      Imr1m2Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M2BASE_IMPH_IOC_MCHBAR_REG));
      return (UINT64) Imr1m2Base.Data;
    } else if (ImrDataType == IMR_LIMIT) {
      Imr1m2Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M2LIMIT_IMPH_IOC_MCHBAR_REG));
      return (UINT64) Imr1m2Limit.Data;
    } else if (ImrDataType == IMR_SIZE) {
      Imr1m2Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M2BASE_IMPH_IOC_MCHBAR_REG));
      Imr1m2Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M2LIMIT_IMPH_IOC_MCHBAR_REG));
      return (UINT64) (Imr1m2Limit.Data - Imr1m2Base.Data);
    } else if (ImrDataType == IMR_BASE_BYTE) {
      Imr1m2Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M2BASE_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)Imr1m2Base.Bits.imr_base_rw, IMR1M2BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);
    } else if (ImrDataType == IMR_LIMIT_BYTE) {
      Imr1m2Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M2LIMIT_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)(Imr1m2Limit.Bits.imr_limit_rw), IMR1M2LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB) - 1;
    } else if (ImrDataType == IMR_SIZE_BYTE) {
      Imr1m2Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M2BASE_IMPH_IOC_MCHBAR_REG));
      Imr1m2Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M2LIMIT_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)(Imr1m2Limit.Bits.imr_limit_rw - Imr1m2Base.Bits.imr_base_rw), IMR1M2LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);
    } else {
      DEBUG ((DEBUG_INFO, "Invalid Imr Register Requested\n"));
      return MAX_UINT64;
    }
  case DSM:
    if (ImrDataType == IMR_BASE) {
      Imr1m3Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M3BASE_IMPH_IOC_MCHBAR_REG));
      return (UINT64) Imr1m3Base.Data;
    } else if (ImrDataType == IMR_LIMIT) {
      Imr1m3Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M3LIMIT_IMPH_IOC_MCHBAR_REG));
      return (UINT64) Imr1m3Limit.Data;
    } else if (ImrDataType == IMR_SIZE) {
      Imr1m3Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M3BASE_IMPH_IOC_MCHBAR_REG));
      Imr1m3Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M3LIMIT_IMPH_IOC_MCHBAR_REG));
      return (UINT64) (Imr1m3Limit.Data - Imr1m3Base.Data);
    } else if (ImrDataType == IMR_BASE_BYTE) {
      Imr1m3Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M3BASE_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)Imr1m3Base.Bits.imr_base_rw, IMR1M3BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);
    } else if (ImrDataType == IMR_LIMIT_BYTE) {
      Imr1m3Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M3LIMIT_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)(Imr1m3Limit.Bits.imr_limit_rw), IMR1M3LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);
    } else if (ImrDataType == IMR_SIZE_BYTE) {
      Imr1m3Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M3BASE_IMPH_IOC_MCHBAR_REG));
      Imr1m3Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M3LIMIT_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)(Imr1m3Limit.Bits.imr_limit_rw - Imr1m3Base.Bits.imr_base_rw), IMR1M3LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);
    } else {
      DEBUG ((DEBUG_INFO, "Invalid Imr Register Requested\n"));
      return MAX_UINT64;
    }
  case CCS:
    if (ImrDataType == IMR_BASE) {
      Imr1m6Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M6BASE_IMPH_IOC_MCHBAR_REG));
      return (UINT64) Imr1m6Base.Data;
    } else if (ImrDataType == IMR_LIMIT) {
      Imr1m6Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M6LIMIT_IMPH_IOC_MCHBAR_REG));
      return (UINT64) Imr1m6Limit.Data;
    } else if (ImrDataType == IMR_SIZE) {
      Imr1m6Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M6BASE_IMPH_IOC_MCHBAR_REG));
      Imr1m6Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M6LIMIT_IMPH_IOC_MCHBAR_REG));
      return (UINT64) (Imr1m6Limit.Data - Imr1m6Base.Data);
    } else if (ImrDataType == IMR_BASE_BYTE) {
      Imr1m6Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M6BASE_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)Imr1m6Base.Bits.imr_base_rw, IMR1M6BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);
    } else if (ImrDataType == IMR_LIMIT_BYTE) {
      Imr1m6Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M6LIMIT_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)(Imr1m6Limit.Bits.imr_limit_rw), IMR1M6LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);
    } else if (ImrDataType == IMR_SIZE_BYTE) {
      Imr1m6Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M6BASE_IMPH_IOC_MCHBAR_REG));
      Imr1m6Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M6LIMIT_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)(Imr1m6Limit.Bits.imr_limit_rw - Imr1m6Base.Bits.imr_base_rw), IMR1M6LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);
    } else {
      DEBUG ((DEBUG_INFO, "Invalid Imr Register Requested\n"));
      return MAX_UINT64;
    }
  case DPR:
    if (ImrDataType == IMR_BASE) {
      Imr1m9Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M9BASE_IMPH_IOC_MCHBAR_REG));
      return (UINT64) Imr1m9Base.Data;
    } else if (ImrDataType == IMR_LIMIT) {
      Imr1m9Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M9LIMIT_IMPH_IOC_MCHBAR_REG));
      return (UINT64) Imr1m9Limit.Data;
    } else if (ImrDataType == IMR_SIZE) {
      Imr1m9Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M9BASE_IMPH_IOC_MCHBAR_REG));
      Imr1m9Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M9LIMIT_IMPH_IOC_MCHBAR_REG));
      return (UINT64) (Imr1m9Limit.Data - Imr1m9Base.Data);
    } else if (ImrDataType == IMR_BASE_BYTE) {
      Imr1m9Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M9BASE_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)Imr1m9Base.Bits.imr_base_rw, IMR1M9BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);
    } else if (ImrDataType == IMR_LIMIT_BYTE) {
      Imr1m9Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M9LIMIT_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64) (Imr1m9Limit.Bits.imr_limit_rw), IMR1M9LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);
    } else if (ImrDataType == IMR_SIZE_BYTE) {
      Imr1m9Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M9BASE_IMPH_IOC_MCHBAR_REG));
      Imr1m9Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M9LIMIT_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)(Imr1m9Limit.Bits.imr_limit_rw - Imr1m9Base.Bits.imr_base_rw), IMR1M9LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);
    } else {
      DEBUG ((DEBUG_INFO, "Invalid Imr Register Requested\n"));
      return MAX_UINT64;
    }
  case TSEG:
    if (ImrDataType == IMR_BASE) {
      Imr1m10Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M10BASE_IMPH_IOC_MCHBAR_REG));
      return (UINT64) Imr1m10Base.Data;
    } else if (ImrDataType == IMR_LIMIT) {
      Imr1m10Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M10LIMIT_IMPH_IOC_MCHBAR_REG));
      return (UINT64) Imr1m10Limit.Data;
    } else if (ImrDataType == IMR_SIZE) {
      Imr1m10Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M10BASE_IMPH_IOC_MCHBAR_REG));
      Imr1m10Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M10LIMIT_IMPH_IOC_MCHBAR_REG));
      return (UINT64) (Imr1m10Limit.Data - Imr1m10Base.Data);
    } else if (ImrDataType == IMR_BASE_BYTE) {
      Imr1m10Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M10BASE_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)(Imr1m10Base.Bits.imr_base_rw), IMR1M10BASE_IMPH_IOC_MCHBAR_IMR_BASE_RW_LSB);
    } else if (ImrDataType == IMR_LIMIT_BYTE) {
      Imr1m10Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M10LIMIT_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)(Imr1m10Limit.Bits.imr_limit_rw), IMR1M10LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);
    } else if (ImrDataType == IMR_SIZE_BYTE) {
      Imr1m10Base.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M10BASE_IMPH_IOC_MCHBAR_REG));
      Imr1m10Limit.Data = MmioRead64 (MchBar + (UINT64) GET_NOC_EFFECTIVE_ADDRESS (IMR1M10LIMIT_IMPH_IOC_MCHBAR_REG));
      return LShiftU64 ((UINT64)(Imr1m10Limit.Bits.imr_limit_rw - Imr1m10Base.Bits.imr_base_rw), IMR1M10LIMIT_IMPH_IOC_MCHBAR_IMR_LIMIT_RW_LSB);
    } else {
      DEBUG ((DEBUG_INFO, "Invalid Imr Register Requested\n"));
      return MAX_UINT64;
    }
  case CSE:
    MemorySubSystemHob = (MEMORY_SUB_SYSTEM_DATA_HOB *) GetFirstGuidHob (&gMemorySubSystemHobGuid);
    if (MemorySubSystemHob == NULL) {
      return 0x0;
    } else if (ImrDataType == IMR_BASE) {
      return (UINT64) MemorySubSystemHob->MeStolenBase;
    } else if (ImrDataType == IMR_SIZE) {
      return (UINT64) MemorySubSystemHob->MeStolenSize;
    } else if (ImrDataType == IMR_BASE_BYTE) {
      return LShiftU64 ((UINT64) MemorySubSystemHob->MeStolenBase, 20);
    } else if (ImrDataType == IMR_SIZE_BYTE) {
      return LShiftU64 ((UINT64) MemorySubSystemHob->MeStolenSize, 20);
    } else {
      DEBUG ((DEBUG_INFO, "Invalid Imr Register Requested\n"));
      return MAX_UINT64;
    }
  default:
    DEBUG ((DEBUG_INFO, "Invalid Imr Register Requested\n"));
    return MAX_UINT64;
  }
}
