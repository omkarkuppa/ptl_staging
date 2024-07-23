/** @file
  Host Bridge Info Fru Lib implementation.

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
#include <Library/HostBridgeInfoLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/NguInfoLib.h>
#include <Register/HostBridgeRegs.h>
#include <Register/HacRegs.h>

/**
  Checks if SAM is enabled or not.

  @retval FALSE  SAM is disabled
  @retval TRUE   SAM is enabled
**/
BOOLEAN
EFIAPI
IsSamEnabled (
  VOID
  )
{
  return FALSE;
}

/**
  GetSamFwInfo - This function performs SAM ready state and get its FW version info.

  This function does the following steps:
  (1) Examines SAM_READY (Register SAM_STATUS_1, bit 30)
  (2) If 0, retry in SAM_READY_TIMEOUT ms (1ms timeout)
  (3) Return & print  FW version in success, else return 0x0.

  @retval UINT32       - SAM FW Version if SAM is ready, else 0x0.
**/
UINT32
EFIAPI
GetSamFwInfo (
  VOID
  )
{
  return 0x0;
}

/**
  Regbar size- This function return RegBar Size.

  @retval UINT32       - RegBar Size.
**/
UINT32
EFIAPI
GetRegBarSize (
  VOID
  )
{
  return V_SA_MCHBAR_REGBAR_SIZE;
}

/**
  This function will get 16-bit value of REG BAR from Device 0.

  @return The 16-bit value of REG BAR.
**/
UINT16
EFIAPI
GetRegBar (
  VOID
  )
{
  return REGBAR_NCU_CCF_IDP_MCHBAR_REG;
}

/**
  This function will check if Tseg base is consistent in different memory scopes.

  @retval TRUE          Tseg Base is consistent
  @retval FALSE         Tseg Base is not consistent
**/
BOOLEAN
IsTsegBaseAndImrMemoryConsistent (
  VOID
  )
{
  UINT64                       TsegMB;
  UINT64                       ImrTsegBase;

  ImrTsegBase = GetImrTsegBaseOffset();
  TsegMB = (UINT64) GetHostBridgeRegisterData (HostBridgeTsegMb, HostBridgeTsegMbBase);
  if (TsegMB == ImrTsegBase) {
    return TRUE;
  }

  DEBUG((DEBUG_WARN, "Tseg Base is not consistent\n"));
  return FALSE;
}

/**
  Checks whether MMIO above 4GB is enabled or not
  @return TRUE MMIO above 4GB is enabled
**/
BOOLEAN
IsAbove4GBMmioEnabled (
  VOID
  )
{
  return TRUE;
}

/**
  Checks whether Crid is enabled or not
  @return Crid is disabled
**/
BOOLEAN
IsCridEnabled (
  VOID
  )
{
  return FALSE;
}

/**
  This function will check if IMR BGSM range is enabled or not

  @retval TRUE          IMR BGSM range is enabled
  @retval FALSE         IMR BGSM range is disabled
**/
BOOLEAN
EFIAPI
IsImrBgsmRangeEnabled (
  VOID
  )
{
  IMR1M2BASE_IMPH_IOC_MCHBAR_STRUCT   GsmBaseStruct;
  IMR1M2LIMIT_IMPH_IOC_MCHBAR_STRUCT  GsmLimitStruct;

  GsmBaseStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M2BASE_IMPH_IOC_MCHBAR_REG))));
  GsmLimitStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M2LIMIT_IMPH_IOC_MCHBAR_REG))));

  if ((GsmBaseStruct.Bits.disable0 == 0) && (GsmBaseStruct.Bits.imr_base_rw != 0) && (GsmLimitStruct.Bits.imr_limit_rw != 0))
  {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  This function will check if IMR BDSM range is enabled or not

  @retval TRUE          IMR BDSM range is enabled
  @retval FALSE         IMR BDSM range is disabled
**/
BOOLEAN
EFIAPI
IsImrBdsmRangeEnabled (
  VOID
  )
{
  IMR1M3BASE_IMPH_IOC_MCHBAR_STRUCT  DsmBaseStruct;
  IMR1M3LIMIT_IMPH_IOC_MCHBAR_STRUCT DsmLimitStruct;

  DsmBaseStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M3BASE_IMPH_IOC_MCHBAR_REG))));
  DsmLimitStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M3LIMIT_IMPH_IOC_MCHBAR_REG))));

  if ((DsmBaseStruct.Bits.disable0 == 0) && (DsmBaseStruct.Bits.imr_base_rw != 0) && (DsmLimitStruct.Bits.imr_limit_rw != 0))
  {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  This function will check if IMR CCS range is enabled or not

  @retval TRUE          IMR CCS range is enabled
  @retval FALSE         IMR CCS range is disabled
**/
BOOLEAN
EFIAPI
IsImrCcsRangeEnabled (
  VOID
  )
{
  IMR1M6BASE_IMPH_IOC_MCHBAR_STRUCT   CcsBaseStruct;
  IMR1M6LIMIT_IMPH_IOC_MCHBAR_STRUCT  CcsLimitStruct;

  CcsBaseStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M6BASE_IMPH_IOC_MCHBAR_REG))));
  CcsLimitStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M6LIMIT_IMPH_IOC_MCHBAR_REG))));

  if ((CcsBaseStruct.Bits.disable0 == 0) && (CcsBaseStruct.Bits.imr_base_rw != 0) && (CcsLimitStruct.Bits.imr_limit_rw != 0))
  {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  This function will check if TSEG range is enabled or not

  @retval TRUE          IMR TSEG range is enabled
  @retval FALSE         IMR TSEG range is disabled
**/
BOOLEAN
EFIAPI
IsImrTsegRangeEnabled (
  VOID
  )
{
  IMR1M10BASE_IMPH_IOC_MCHBAR_STRUCT  TsegBaseStruct;
  IMR1M10LIMIT_IMPH_IOC_MCHBAR_STRUCT TsegLimitStruct;

  TsegBaseStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M10BASE_IMPH_IOC_MCHBAR_REG))));
  TsegLimitStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(IMR1M10LIMIT_IMPH_IOC_MCHBAR_REG))));

  if ((TsegBaseStruct.Bits.disable0 == 0) && (TsegBaseStruct.Bits.imr_base_rw != 0) && (TsegLimitStruct.Bits.imr_limit_rw != 0))
  {
    return TRUE;
  } else {
    return FALSE;
  }
}
/**
  This function is provide Imr related details (Base, Mask, Limit).

  @param[in] ExclusionRange     - IMR name or ID
  @param[in] ExclusionRangeType - Requested data (Base, Mask, Limit)

  @retval UINT64                - Register Details based on DataType

**/
UINT64
EFIAPI
GetExclusionRegisterOffset (
  IN IMR_EXCLUSION_OFFSET          ExclusionRange,
  IN IMR_EXCLUSION_OFFSET_TYPE     ExclusionRangeType
  )
{

  switch (ExclusionRange) {
    case NocExclusionRangeLow:
      if (ExclusionRangeType == ExclusionOffsetBase) {
        return (UINT64) GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_REG);
      } else if (ExclusionRangeType == ExclusionOffsetMask) {
        return (UINT64) GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_REG);
      } else {
        DEBUG ((DEBUG_INFO, "Invalid Exclusion Range Offset Requested\n"));
        return MAX_UINT64;
      }
    case NocExclusionRangeHigh:
      if (ExclusionRangeType == ExclusionOffsetBase) {
        return (UINT64) GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_H_IOC_MCHBAR_REG);
      } else if (ExclusionRangeType == ExclusionOffsetMask) {
        return (UINT64) GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_H_IOC_MCHBAR_REG);
      } else {
        DEBUG ((DEBUG_INFO, "Invalid Exclusion Range Offset Requested\n"));
        return MAX_UINT64;
      }
    case IaExclusionRange:
      if (ExclusionRangeType == ExclusionOffsetBase) {
        return (UINT64) R_SA_MCHBAR_IMRIAEXCBASE_CBO_INGRESS_REG;
      } else if (ExclusionRangeType == ExclusionOffsetLimit) {
        return (UINT64) R_SA_MCHBAR_IMRIAEXCLIMIT_CBO_INGRESS_REG;
      } else {
        DEBUG ((DEBUG_INFO, "Invalid Exclusion Range Offset Requested\n"));
        return MAX_UINT64;
      }
    case IaExclusionRange1:
      if (ExclusionRangeType == ExclusionOffsetBase) {
        return (UINT64) R_SA_MCHBAR_IMRIAEXCBASE1_CBO_INGRESS_REG;
      } else if (ExclusionRangeType == ExclusionOffsetLimit) {
        return (UINT64) R_SA_MCHBAR_IMRIAEXCLIMIT1_CBO_INGRESS_REG;
      } else {
        DEBUG ((DEBUG_INFO, "Invalid Exclusion Range Offset Requested\n"));
        return MAX_UINT64;
      }
    default:
      DEBUG ((DEBUG_INFO, "Invalid Exclusion Range Offset Requested\n"));
      ASSERT (FALSE);
      return MAX_UINT64;
  }
}
/**
  This function is provide Imr Size based on selected Imr

  @param[in] ExclusionRange     - IMR name or ID

  @retval UINT32                - IMR Size 

**/
UINT32
EFIAPI
GetExclusionRegisterSize (
  IN IMR_EXCLUSION_OFFSET          ExclusionRange
  )
{
  SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_STRUCT    NocExclusionRangeLowBase;
  SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_STRUCT   NocExclusionRangeLowLimit;
  SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_H_IOC_MCHBAR_STRUCT    NocExclusionRangeHighBase;
  SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_H_IOC_MCHBAR_STRUCT   NocExclusionRangeHighLimit;

  switch (ExclusionRange) {
    case NocExclusionRangeLow:
      NocExclusionRangeLowBase.Data = MmioRead64 ((GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_REG)));
      NocExclusionRangeLowLimit.Data = MmioRead64 ((GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_REG)));
      return (UINT32) (NocExclusionRangeLowLimit.Data - NocExclusionRangeLowBase.Data);
    case NocExclusionRangeHigh:
      NocExclusionRangeHighBase.Data = MmioRead64 ((GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_H_IOC_MCHBAR_REG)));
      NocExclusionRangeHighLimit.Data = MmioRead64 ((GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_H_IOC_MCHBAR_REG)));
      return (UINT32) (NocExclusionRangeHighLimit.Data - NocExclusionRangeHighBase.Data);
    case IaExclusionRange:
      return (UINT32) (MmioRead64 ((GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + R_SA_MCHBAR_IMRIAEXCLIMIT_CBO_INGRESS_REG)) -
              MmioRead64 ((GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + R_SA_MCHBAR_IMRIAEXCBASE_CBO_INGRESS_REG)));
    case IaExclusionRange1:
      return (UINT32) (MmioRead64 ((GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + R_SA_MCHBAR_IMRIAEXCLIMIT1_CBO_INGRESS_REG)) -
              MmioRead64 ((GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + R_SA_MCHBAR_IMRIAEXCBASE1_CBO_INGRESS_REG)));
    default:
      DEBUG ((DEBUG_INFO, "Invalid Exclusion Range Offset Requested\n"));
      ASSERT (FALSE);
      return MAX_UINT32;
  }
}