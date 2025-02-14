/** @file
  PEIM Private Library to initialize PeiHostBridgeInitFruLib

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

#include <Ppi/SiPolicy.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/HobLib.h>
#include <Library/TimerLib.h>
#include <Library/IocLib.h>
#include <Library/PcdLib.h>
#include <Register/PcieSipRegs.h>
#include <Register/HacRegs.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Register/HostBridgeRegs.h>
#include <Register/MemSSRegs.h>
#include <IntelRcStatusCode.h>
#include <Library/NguInfoLib.h>
#include <HostBridgeDataHob.h>
#include <Library/TseDataHob.h>
#include <TraceHubDataHob.h>
#include <CMrcInterface.h>
#include <Library/PeiHostBridgeInitLib.h>
#include <TelemetryPeiConfig.h>
#include <Library/IGpuInfoLib.h>
#include <Library/TdxInfoLib.h>
#include <Library/PeiTdxLib.h>
#include <Library/PeiCmiInitLib.h>
#include <IGpuDataHob.h>
#include <Library/TdxFruLib.h>
#include <Library/TdxDataHob.h>
#include <Library/MemorySubSystemInfoLib.h>


//
// Time in microseconds BIOS will poll and wait for the MEM_CONFIG_DONE_ACK bit
// to be set
//
#define MEM_CONFIG_DONE_ACK_TIMEOUT_US 1000000

#define BYTES_TO_MB(a) (a >> 20)

typedef union {
  struct {
    UINT64 Lock                                    :  1;   // Bits 0:0
    UINT64 Rsvd1                                   :  9;   // Bits 9:1
    UINT64 ImrIaExcBase                            :  28;  // Bits 37:10
    UINT64 Rsvd2                                   :  26;  // Bits 63:38
  } Bits;
  UINT64 Data;
} IMRIAEXCBASE_MCHBAR_CBO_INGRESS_STRUCT;

typedef union {
  struct {
    UINT64 Lock                                    :  1;   // Bits 0:0
    UINT64 Rsvd1                                   :  9;   // Bits 9:1
    UINT64 ImrIaExcLimit                           :  28;  // Bits 37:10
    UINT64 Rsvd2                                   :  26;  // Bits 63:38
  } Bits;
  UINT64 Data;
} IMRIAEXCLIMIT_MCHBAR_CBO_INGRESS_STRUCT;

typedef union {
  struct {
    UINT64 Lock                                    :  1;   // Bits 0:0
    UINT64 Rsvd1                                   :  9;   // Bits 9:1
    UINT64 ImrIaExcBase1                           :  28;  // Bits 37:10
    UINT64 Rsvd2                                   :  26;  // Bits 63:38
  } Bits;
  UINT64 Data;
} IMRIAEXCBASE1_MCHBAR_CBO_INGRESS_STRUCT;

typedef union {
  struct {
    UINT64 Lock                                    :  1;   // Bits 0:0
    UINT64 Rsvd1                                   :  9;   // Bits 9:1
    UINT64 ImrIaExcLimit1                          :  28;  // Bits 37:10
    UINT64 Rsvd2                                   :  26;  // Bits 63:38
  } Bits;
  UINT64 Data;
} IMRIAEXCLIMIT1_MCHBAR_CBO_INGRESS_STRUCT;

/**
  Programs HostBridge Bars

**/
VOID
ProgramHostBridgeBars (
  VOID
  )
{
  HOST_BRIDGE_PREMEM_CONFIG     *HostBridgePreMemConfig;
  SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi;
  EFI_STATUS                    Status;
  ///
  /// Get policy settings through the SiPreMemPolicyPpi PPI
  ///
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if ((Status != EFI_SUCCESS) || (SiPreMemPolicyPpi == NULL)) {
    DEBUG ((DEBUG_INFO, "Fail to locate SiPreMemPolicyPpi\n"));
    return;
  }

  HostBridgePreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Program MchBar
  ///
  SetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseHigh, (UINT32)RShiftU64 (HostBridgePreMemConfig->MchBar, 32));
  SetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow, (UINT32)(HostBridgePreMemConfig->MchBar | BIT0));
  ///
  /// Program RegBar
  ///
  MmioWrite32 ((UINTN)(HostBridgePreMemConfig->MchBar) + REGBAR_NCU_CCF_IDP_MCHBAR_REG + 4, (UINT32)RShiftU64 (HostBridgePreMemConfig->RegBar, 32));
  MmioWrite32 ((UINTN)(HostBridgePreMemConfig->MchBar) + REGBAR_NCU_CCF_IDP_MCHBAR_REG, (UINT32)(HostBridgePreMemConfig->RegBar | BIT0));

  ///
  /// Program SafBar
  ///
  SetHostBridgeRegisterData (HostBridgeSafBar, SafBarCfgBaseHigh, (UINT32)RShiftU64 (HostBridgePreMemConfig->SafBar, 32));
  SetHostBridgeRegisterData (HostBridgeSafBar, SafBarCfgBaseLow, (UINT32)(HostBridgePreMemConfig->SafBar | BIT0));
  //
  // This function builds a HOB that describes a chunk of system memory.
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    HostBridgePreMemConfig->RegBar,
    V_SA_MCHBAR_REGBAR_SIZE
    );

  DEBUG ((DEBUG_INFO, "RegBar Programming Base = 0x%x Length = 0x%x\n", HostBridgePreMemConfig->RegBar, HostBridgePreMemConfig->RegBar + V_SA_MCHBAR_REGBAR_SIZE));

  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    HostBridgePreMemConfig->MchBar,
    FixedPcdGet32 (PcdMchMmioSize)
    );

  DEBUG ((DEBUG_INFO, "MchBar Programming Base = 0x%x Length = 0x%x\n", HostBridgePreMemConfig->MchBar, HostBridgePreMemConfig->MchBar + FixedPcdGet32 (PcdMchMmioSize)));

  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    HostBridgePreMemConfig->SafBar,
    V_SA_SAFBAR_REGBAR_SIZE
    );

  DEBUG ((DEBUG_INFO, "SafBar Programming Base = 0x%lx Length = 0x%lx\n", HostBridgePreMemConfig->SafBar, HostBridgePreMemConfig->SafBar + V_SA_SAFBAR_REGBAR_SIZE));

  //
  // This function builds a HOB for the memory allocation.
  //
  BuildMemoryAllocationHob (
    HostBridgePreMemConfig->RegBar,
    V_SA_MCHBAR_REGBAR_SIZE,
    EfiMemoryMappedIO
    );

  BuildMemoryAllocationHob (
    HostBridgePreMemConfig->MchBar,
    FixedPcdGet32 (PcdMchMmioSize),
    EfiMemoryMappedIO
    );

  BuildMemoryAllocationHob (
    HostBridgePreMemConfig->SafBar,
    V_SA_SAFBAR_REGBAR_SIZE,
    EfiMemoryMappedIO
    );
}


/**
  This function Program IA IMR Exclusion below 4GB

  @param[in] MeStolenBase - ME Stolen Base Address.
  @param[in] MeStolenSize - ME Stolen Size.
**/
VOID
SetIaImrExclusionBelow4Gb (
  IN UINT32     MeStolenBase,
  IN UINT32     MeStolenSize
  )
{
  UINT32                                   MchBar;
  IMRIAEXCBASE_MCHBAR_CBO_INGRESS_STRUCT   ImrExcBase;
  IMRIAEXCLIMIT_MCHBAR_CBO_INGRESS_STRUCT  ImrExcLimit;
  UINT32                                   BaseBelow4Gb;
  UINT32                                   LimitBelow4Gb;

  DEBUG ((DEBUG_INFO, "%a Enter\n", __FUNCTION__));

  BaseBelow4Gb  = MeStolenBase;
  if (MeStolenSize != 0) {
    LimitBelow4Gb = (MeStolenBase << 20) + (MeStolenSize << 20);
  } else {
    LimitBelow4Gb = 0;
  }

  DEBUG ((DEBUG_INFO, "ImrBase Low = 0x%x\n",BaseBelow4Gb));
  DEBUG ((DEBUG_INFO, "ImrLimit Low = 0x%x\n",LimitBelow4Gb));

  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);
  // Program IMR IA Exclusion Base and Limit below 4GB
  //    IMRIAEXCLBASE (MCHBAR + 0x6A40h[37:10]) should be programmed to the lowest address (base) of the IMR region.
  //    IMRIAEXCLIMIT (MCHBAR + 0x6A48h[37:10]) should be programmed to the highest address (base + IMR Size) of the IMR region.
  ImrExcBase.Data                     = 0;
  ImrExcLimit.Data                    = 0;
  ImrExcBase.Bits.ImrIaExcBase        = BaseBelow4Gb << 10;
  ImrExcLimit.Bits.ImrIaExcLimit      = LimitBelow4Gb >> 10;

  DEBUG ((DEBUG_INFO, "IAExcBase  below 4GB = 0x%lx\n", ImrExcBase.Data));
  DEBUG ((DEBUG_INFO, "IAExcLimit  below 4GB = 0x%lx\n", ImrExcLimit.Data));

  MmioWrite32 (MchBar + R_SA_MCHBAR_IMRIAEXCBASE_CBO_INGRESS_REG + 4, (UINT32) RShiftU64 (ImrExcBase.Data, 32));
  MmioWrite32 (MchBar + R_SA_MCHBAR_IMRIAEXCBASE_CBO_INGRESS_REG, (UINT32) ImrExcBase.Data);

  MmioWrite32 (MchBar + R_SA_MCHBAR_IMRIAEXCLIMIT_CBO_INGRESS_REG + 4, (UINT32) RShiftU64 (ImrExcLimit.Data, 32));
  MmioWrite32 (MchBar + R_SA_MCHBAR_IMRIAEXCLIMIT_CBO_INGRESS_REG, (UINT32) ImrExcLimit.Data);

   DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}

/**
  This function Program IA IMR Exclusion above 4GB
**/
VOID
SetIaImrExclusionAbove4Gb (
  VOID
  )
{
  UINT32                                   MchBar;
  IMRIAEXCBASE1_MCHBAR_CBO_INGRESS_STRUCT  ImrExcBase1;
  IMRIAEXCLIMIT1_MCHBAR_CBO_INGRESS_STRUCT ImrExcLimit1;
  UINT64                                   BaseAbove4Gb;
  UINT64                                   LimitAbove4Gb;
  IGPU_DATA_HOB                            *IGpuDataHob;

  IGpuDataHob = NULL;
  BaseAbove4Gb = 0;
  DEBUG ((DEBUG_INFO, "%a Enter\n", __FUNCTION__));

  IGpuDataHob = (IGPU_DATA_HOB *) GetFirstGuidHob (&gIGpuDataHobGuid);
  if (IGpuDataHob != NULL) {
    BaseAbove4Gb = RShiftU64 (IGpuDataHob->FlatCcsBaseAddr, 20);
  }
  if (IGpuGetFlatCcsSizeInMb () != 0) {
    LimitAbove4Gb = BaseAbove4Gb + IGpuGetFlatCcsSizeInMb ();
  } else {
    LimitAbove4Gb = 0;
  }

  DEBUG ((DEBUG_INFO, "ImrBase High= 0x%lx\n",BaseAbove4Gb));
  DEBUG ((DEBUG_INFO, "ImrLimit High= 0x%lx\n", LimitAbove4Gb));

  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);


  // Program IMR IA Exclusion Base and Limit above 4GB
  // IMRIAEXCLBASE1 (MCHBAR + 0x6A50h[37:10]) should be programmed to the lowest address (base) of the IMR region.
  // IMRIAEXCLIMIT1 (MCHBAR + 0x6A58h[37:10]) should be programmed to the highest address (base + IMR Size) of the IMR region
  ImrExcBase1.Data                      = 0;
  ImrExcLimit1.Data                     = 0;
  ImrExcBase1.Bits.ImrIaExcBase1        = LShiftU64 (BaseAbove4Gb, 10);
  ImrExcLimit1.Bits.ImrIaExcLimit1      = LShiftU64 (LimitAbove4Gb, 10);

  DEBUG ((DEBUG_INFO, "IAExcBase  Above 4GB = 0x%lx\n", ImrExcBase1.Data));
  DEBUG ((DEBUG_INFO, "IAExcLimit  Above 4GB = 0x%lx\n", ImrExcLimit1.Data));

  MmioWrite32 (MchBar + R_SA_MCHBAR_IMRIAEXCBASE1_CBO_INGRESS_REG + 4, (UINT32) RShiftU64 (ImrExcBase1.Data, 32));
  MmioWrite32 (MchBar + R_SA_MCHBAR_IMRIAEXCBASE1_CBO_INGRESS_REG, (UINT32) ImrExcBase1.Data);

  MmioWrite32 (MchBar + R_SA_MCHBAR_IMRIAEXCLIMIT1_CBO_INGRESS_REG + 4, (UINT32) RShiftU64 (ImrExcLimit1.Data, 32));
  MmioWrite32 (MchBar + R_SA_MCHBAR_IMRIAEXCLIMIT1_CBO_INGRESS_REG, (UINT32) ImrExcLimit1.Data);

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}

/**
  This function Lock IA IMR Exclusion
**/
VOID
SetIaImrExclusionLock (
  VOID
  )
{
  UINT32                                   MchBar;

  DEBUG ((DEBUG_INFO, "SetIaImrExclusionLock Enter\n"));
  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);

  // Lock IMR IA Exclusion Base and Limit
  MmioOr64 (MchBar + R_SA_MCHBAR_IMRIAEXCBASE_CBO_INGRESS_REG, BIT0);
  MmioOr64 (MchBar + R_SA_MCHBAR_IMRIAEXCLIMIT_CBO_INGRESS_REG, BIT0);
  MmioOr64 (MchBar + R_SA_MCHBAR_IMRIAEXCBASE1_CBO_INGRESS_REG, BIT0);
  MmioOr64 (MchBar + R_SA_MCHBAR_IMRIAEXCLIMIT1_CBO_INGRESS_REG, BIT0);

  DEBUG ((DEBUG_INFO, "SetIaImrExclusionLock End\n"));
}

/**
  This function Program NOC IMR Exclusion

  @param[in] SiPreMemPolicyPpi  Pointer to pre-mem SI policy.
  @param[in] MeStolenBase - ME Stolen Base Address.
  @param[in] MeStolenSize - ME Stolen Size.
  @param[in] PrmrrBase    - PRMRR Base Address.
  @param[in] PrmrrSize    - PRMRR Size.
  @param[in] TdxEnabled   - TDX Policy.
  @param[in] SeamrrSize   - Seamrr region size.

**/
VOID
SetNocImrExclusion (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi,
  IN UINT32                MeStolenBase,
  IN UINT32                MeStolenSize,
  IN UINT32                PrmrrBase,
  IN UINT32                PrmrrSize,
  IN UINT32                TdxEnabled,
  IN UINT32                SeamrrSize,
  IN UINT32                TotalPhysicalMemorySize
  )
{
  UINT64                                               MchBar;
  SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_H_IOC_MCHBAR_STRUCT  ImrExcBaseH;
  SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_H_IOC_MCHBAR_STRUCT ImrExcLimitH;
  SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_STRUCT  ImrExcBaseL;
  SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_STRUCT ImrExcLimitL;
  UINT64                                               BaseBelow4Gb;
  UINT64                                               BaseAbove4Gb;
  UINT64                                               LimitAbove4Gb;
  TSE_DATA_HOB                                         *TseDataHobPtr;
  TRACEHUB_DATA_HOB                                    *TraceHubDataHob;
  TDX_DATA_HOB                                         *TdxDataHobPtr;
  EFI_STATUS                                           Status;
  TELEMETRY_PEI_PREMEM_CONFIG                          *TelemetryPreMemConfig;
  UINT64                                               SizeAbove4Gb;
  UINT64                                               UpperBase;

  TseDataHobPtr          = NULL;
  TraceHubDataHob        = NULL;
  TelemetryPreMemConfig  = NULL;
  TdxDataHobPtr          = NULL;
  SizeAbove4Gb           = 0;
  UpperBase              = 0;

  DEBUG ((DEBUG_INFO, "SetNocImrExclusion Start\n"));

  TseDataHobPtr = (TSE_DATA_HOB *) GetFirstGuidHob (&gTseDataHobGuid);
  if (TseDataHobPtr == NULL) {
    return;
  }

  TraceHubDataHob = (TRACEHUB_DATA_HOB *) GetFirstGuidHob (&gTraceHubDataHobGuid);
  if (TraceHubDataHob == NULL) {
    return;
  }

  TdxDataHobPtr = (TDX_DATA_HOB *) GetFirstGuidHob (&gTdxDataHobGuid);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTelemetryPeiPreMemConfigGuid, (VOID *) &TelemetryPreMemConfig);
  if (Status != EFI_SUCCESS) {
    ASSERT_EFI_ERROR (Status);
  }
  if (TotalPhysicalMemorySize <= 0x1000) {
    if ((IsTdxSupported () == TRUE) && (TdxEnabled == TRUE)) {
      BaseBelow4Gb  = (UINT32) (TdxDataHobPtr->SeamRrBaseAddress);
    }
  } else if (TseDataHobPtr->TseDramMemBase != 0) {
    BaseBelow4Gb  = (UINT32) TseDataHobPtr->TseDramMemBase;
  } else if (MeStolenBase != 0) {
    BaseBelow4Gb = MeStolenBase;
  } else if (PrmrrBase != 0) {
    BaseBelow4Gb = PrmrrBase;
  } else {
    BaseBelow4Gb = 0;
  }

  if (PrmrrSize != 0) {
    UpperBase = PrmrrBase + PrmrrSize;
  }
  if (MeStolenSize != 0) {
    if (UpperBase == 0) {
      UpperBase = MeStolenBase + MeStolenSize;
    }
  }
  if (TseDataHobPtr != NULL) {
    if (TseDataHobPtr->TseDramMemSize != 0){
      if (UpperBase == 0) {
        UpperBase = (UINT32) TseDataHobPtr->TseDramMemBase + TseDataHobPtr->TseDramMemSize;
      }
    }
  }
  if (TotalPhysicalMemorySize <= 0x1000) {
    if ((IsTdxSupported () == TRUE) && (TdxEnabled == TRUE) && TdxDataHobPtr->SeamRrBaseAddress != 0) {
      if (UpperBase == 0) {
        UpperBase = (UINT32) (TdxDataHobPtr->SeamRrBaseAddress << 20 ) + (UINT32) (SeamrrSize << 20);
      }
    }
  }

  BaseAbove4Gb = BASE_4GB;
  if (TraceHubDataHob->TraceHubMemSize != 0) {
    SizeAbove4Gb += TraceHubDataHob->TraceHubMemSize;
  }

  if (TelemetryPreMemConfig != NULL) {
    if (TelemetryPreMemConfig->StreamTracerMode != StreamTracerDisabled) {
      SizeAbove4Gb += (TelemetryPreMemConfig->StreamTracerSize << 10);
    }
  }

  if ((IsTdxSupported () == TRUE) && (TdxEnabled == TRUE) && (TotalPhysicalMemorySize > 0x1000)) {
    SizeAbove4Gb += LShiftU64((UINT64)SeamrrSize, 20);
    SizeAbove4Gb = NaturalAlignment (SizeAbove4Gb, SIZE_32MB);
  }
  LimitAbove4Gb = BaseAbove4Gb + SizeAbove4Gb;

  DEBUG ((DEBUG_INFO, "ImrBaseBelow4Gb  = 0x%lx\n", BaseBelow4Gb));
  DEBUG ((DEBUG_INFO, "UpperBase = 0x%lx\n", UpperBase));
  DEBUG ((DEBUG_INFO, "ImrBaseAbove4Gb  = 0x%lx\n", BaseAbove4Gb));
  DEBUG ((DEBUG_INFO, "ImrLimitAbove4Gb = 0x%lx\n", LimitAbove4Gb));

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  //    Program IMR NOC Exclusion Base and Limit Below and Above 4GB
  //    IMRNOCEXCLBASEBELOW4GB (MCHBAR + 0x1B30h [41:20]) should be programmed to the lowest address (base) of the NOC IMR region.
  //    IMRIAEXCLIMITBELOW4GB  (MCHBAR + 0x1B38h [41:20]) should be programmed to the highest address (base + Size) of the IMR region.
  //    IMRNOCEXCLBASEABOVE4GB (MCHBAR + 0x1B20h [41:20]) should be programmed to the lowest address (base) of the IMR region.
  //    IMRIAEXCLIMITABOVE4GB  (MCHBAR + 0x1B28h [41:20]) should be programmed to the highest address (base + Size) of the IMR region.

  ImrExcBaseH.Data               = 0;
  ImrExcLimitH.Data              = 0;
  ImrExcBaseL.Data               = 0;
  ImrExcLimitL.Data              = 0;

  BaseBelow4Gb   = (UINT64) (BaseBelow4Gb << SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_IMR_BASE_RW_LSB);
  UpperBase  = (UINT64) (UpperBase << SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_IMR_LIMIT_RW_LSB);

  ImrExcBaseH.Data  = BaseAbove4Gb;
  ImrExcLimitH.Data = LimitAbove4Gb;
  ImrExcBaseL.Data  = BaseBelow4Gb;
  ImrExcLimitL.Data = UpperBase;

  DEBUG ((DEBUG_INFO, "ImrExcBase  below 4GB = 0x%lx\n", ImrExcBaseL.Data));
  DEBUG ((DEBUG_INFO, "ImrExcLimit below 4GB = 0x%lx\n", ImrExcLimitL.Data));
  DEBUG ((DEBUG_INFO, "ImrExcBase  Above 4GB = 0x%lx\n", ImrExcBaseH.Data));
  DEBUG ((DEBUG_INFO, "ImrExcLimit Above 4GB = 0x%lx\n", ImrExcLimitH.Data));

  MmioWrite64 ((UINTN) (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_REG)), ImrExcBaseL.Data);
  MmioWrite64 ((UINTN) (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_REG)), ImrExcLimitL.Data);

  DEBUG ((DEBUG_INFO, "ImrExcBase below 4GB Register After Write = 0x%lx\n", MmioRead64 ((UINTN) (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_REG)))));
  DEBUG ((DEBUG_INFO, "ImrExcLimit below 4GB Register After Write = 0x%lx\n", MmioRead64 ((UINTN) (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_L_IOC_MCHBAR_REG)))));

  MmioWrite64 ((UINTN) (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_H_IOC_MCHBAR_REG)), ImrExcBaseH.Data);
  MmioWrite64 ((UINTN) (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_H_IOC_MCHBAR_REG)), ImrExcLimitH.Data);

  DEBUG ((DEBUG_INFO, "ImrExcBase Above 4GB Register After Write = 0x%lx\n", MmioRead64 ((UINTN) (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_BASE_MEM_EXCLUDE_RANGE_H_IOC_MCHBAR_REG)))));
  DEBUG ((DEBUG_INFO, "ImrExcLimit Above 4GB Register After Write = 0x%lx\n", MmioRead64 ((UINTN) (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_LIMIT_MEM_EXCLUDE_RANGE_H_IOC_MCHBAR_REG)))));

  DEBUG ((DEBUG_INFO, "SetNocImrExclusion End\n"));
}

/**
  This function program Vga IMR Exclusion

**/
VOID
SetVgaImrExclusion (
  VOID
  )
{
  UINT64             MchBar;
  SAF_ADDR_BASE_MEM_VGA_EXCLUDE_RANGE_IOC_MCHBAR_STRUCT ImrExcBase;
  SAF_ADDR_MASK_MEM_VGA_EXCLUDE_RANGE_IOC_MCHBAR_STRUCT ImrExcMask;
  UINT64                                                ImrVgaSize;
  UINT64                                                ImrVgaMask;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  ImrVgaSize   = SIZE_128KB;
  ImrVgaMask   = (~ (ImrVgaSize - 1));
  ImrExcBase.Data = MmioRead64 (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_BASE_MEM_VGA_EXCLUDE_RANGE_IOC_MCHBAR_REG));
  ImrExcMask.Data = MmioRead64 (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_MASK_MEM_VGA_EXCLUDE_RANGE_IOC_MCHBAR_REG));
  ImrExcBase.Bits.disable0      = 0;
  ImrExcBase.Bits.imr_base_rw   = 0xA0000 >> 10; // imr_base_rw [41:10] is KB aligned so converting address to KB
  ImrExcMask.Bits.imr_mask_rw   = ImrVgaMask >> 10; // imr_mask_rw [41:10] is KB aligned so converting address to KB

  DEBUG ((DEBUG_INFO, "ImrExcBase = 0x%lx\n", ImrExcBase.Data));
  DEBUG ((DEBUG_INFO, "ImrExcMask = 0x%lx\n", ImrExcMask.Data));

  MmioWrite64 ((UINTN) (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_BASE_MEM_VGA_EXCLUDE_RANGE_IOC_MCHBAR_REG)), ImrExcBase.Data);
  MmioWrite64 ((UINTN) (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_MASK_MEM_VGA_EXCLUDE_RANGE_IOC_MCHBAR_REG)), ImrExcMask.Data);

  DEBUG ((DEBUG_INFO, "ImrExcBase Register After Write = 0x%lx\n", MmioRead64 ((UINTN) (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_BASE_MEM_VGA_EXCLUDE_RANGE_IOC_MCHBAR_REG)))));
  DEBUG ((DEBUG_INFO, "ImrExcMask Register After Write = 0x%lx\n", MmioRead64 ((UINTN) (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_ADDR_MASK_MEM_VGA_EXCLUDE_RANGE_IOC_MCHBAR_REG)))));

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}

/**
  Set and Acknowledge MEM_CONFIG_DONE.

  @retval EFI_SUCCESS       MEM_CONFIG_DONE bit is set and acknowledged.
  @retval EFI_TIMEOUT       Timeout reached while checking MEM_CONFIG_DONE_ACK
**/
EFI_STATUS
SetMemConfigDone (
  VOID
  )
{
  UINT64                                  MchBar;
  UINT32                                  Timeout;
  MEMSS_PMA_CR_INIT_STATE_MEMSS_STRUCT    MemSSCfg;

  //
  // Get MCHBAR value
  //
  MchBar  = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  //
  // Set MEM_CONFIG_DONE to 1
  //
  MemSSCfg.Data = MmioRead32((UINTN)MchBar + MEMSS_PMA_CR_INIT_STATE_MEMSS_REG);
  MemSSCfg.Bits.mem_config_done = TRUE;
  MmioWrite32 ((UINTN)MchBar + MEMSS_PMA_CR_INIT_STATE_MEMSS_REG, MemSSCfg.Data);
  DEBUG ((DEBUG_INFO, "MEMSS_PMA_CR_INIT_STATE = 0x%x\n", MmioRead32 ((UINTN)MchBar + MEMSS_PMA_CR_INIT_STATE_MEMSS_REG)));

  //
  // Poll MEM_CONFIG_DONE_ACK bit to be 1
  //
  Timeout = MEM_CONFIG_DONE_ACK_TIMEOUT_US;
  do {
    MemSSCfg.Data = MmioRead32((UINTN)MchBar + MEMSS_PMA_CR_INIT_STATE_MEMSS_REG);
    if (MemSSCfg.Bits.mem_config_done_ack) {
      DEBUG ((DEBUG_WARN, "MEM_CONFIG_DONE_ACK is set\n"));
      break;
    }
    MicroSecondDelay (1);
    Timeout--;
  } while (Timeout > 0);

  if (Timeout == 0) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/**
  This function DPR Lock Register

**/
VOID
LockDpr (
  )
{
  SetHostBridgeRegisterData (HostBridgeDprCfg, HostBridgeDprSetlock, 0);

}
/**
  This function is to Set BIOS_RESET_CPL bits.

  @retval None
**/
VOID
SetBiosResetCpl (
  VOID
)
{
  UINT64         MchBar;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  DEBUG ((DEBUG_INFO, "Set BIOS_RESET_CPL to indicate all configurations complete\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_SET_BIOS_RESET_CPL); //PostCode (0xA61)
  MmioOr8 ((UINTN) MchBar + BIOS_RESET_CPL_PCU_PUNIT_MCHBAR_REG, BIT0 | BIT1);
}

/**
  Enable VLW's

  @param[in] HostBridgePreMemConfig   Instance of HOST_BRIDGE_PREMEM_CONFIG

**/
VOID
VlwEnable (
  IN  HOST_BRIDGE_PREMEM_CONFIG     *HostBridgePreMemConfig
  )
{
  VLWCTL_IOC_MCHBAR_STRUCT VlwctlCfg;
  //
  // Enable VLW's by clearing bit0 DROP_VLW_CTL
  //
  DEBUG ((DEBUG_INFO, "Enable VLW \n"));
  VlwctlCfg.Data = MmioRead32 (HostBridgePreMemConfig->MchBar + VLWCTL_IOC_MCHBAR_REG);
  VlwctlCfg.Bits.drop_vlw_ctl = FALSE;
  MmioWrite32 (HostBridgePreMemConfig->MchBar + VLWCTL_IOC_MCHBAR_REG, VlwctlCfg.Data);

}

/**
  This function is to lock PAM region at End Of firmware.

**/
VOID
PamLockEndOfFirmware (
  VOID
  )
{
  HOST_BRIDGE_DATA_HOB     *HostBridgeDataHob;

  ///
  /// Get Host Bridge Data HOB
  ///
  HostBridgeDataHob = NULL;
  HostBridgeDataHob = (HOST_BRIDGE_DATA_HOB *) GetFirstGuidHob (&gHostBridgeDataHobGuid);
  if (HostBridgeDataHob != NULL) {
      if (HostBridgeDataHob->SkipPamLock == FALSE) {
        //
        // Lock PAM by removing POSTBOOT_SAI from the PAM_W_PG WAC/CP
        //
        DEBUG ((DEBUG_INFO, "PAM_LOCK!!\n"));
        SetHostBridgeRegisterData (HostBridgePamWpgWac, HostBridgePamWpgWacWriteData, 0);
        SetHostBridgeRegisterData (HostBridgePamWpgCp, HostBridgePamWpgCpWriteData, 0);
      }
  }
}

/**
  This function will dynamically calculate the MMIO Size Requirement
  @param[in] MmioSize             - Size of MMIO space reserved for devices
  @param[in] MmioSizeAdjustment   - MMIO size adjustment
  @retval RequiredMmio
**/
UINT32
EFIAPI
DynamicMmioCalculation (
  IN UINT32  MmioSize,
  IN UINT32  MmioSizeAdjustment
  )
{
  UINT32  AvailableMmio;
  UINT32  RequiredLowMmio;
  UINT32  ToludBase;
  UINT32  GttMmSize;
  UINT32  LMemBarSize;
  UINT32  AhciPeiSize;
  UINT32  NvmeHcPeiSize;
  UINT32  VmdCfgBarSize;
  UINT32  VmdCfgBar1Size;
  UINT32  VmdCfgBar2Size;
  UINT32  RequiredMmio;

  ToludBase = (UINT32) RShiftU64 (BASE_4GB, 20) - MmioSize;
  RequiredMmio = MmioSize;

  GttMmSize = BYTES_TO_MB(SIZE_16MB);
  LMemBarSize = BYTES_TO_MB(SIZE_256MB);
  VmdCfgBarSize = BYTES_TO_MB(SIZE_32MB);
  VmdCfgBar1Size = BYTES_TO_MB(SIZE_32MB);
  VmdCfgBar2Size = BYTES_TO_MB(SIZE_1MB);
  AhciPeiSize = BYTES_TO_MB(SIZE_16MB);
  NvmeHcPeiSize = BYTES_TO_MB(SIZE_16MB);

  AvailableMmio = (((UINT32) PcdGet64 (PcdSiPciExpressBaseAddress) >> 20) - ToludBase);
  RequiredLowMmio = GttMmSize + LMemBarSize + VmdCfgBarSize + VmdCfgBar1Size + VmdCfgBar2Size + AhciPeiSize + NvmeHcPeiSize + MmioSizeAdjustment;

  DEBUG ((DEBUG_INFO, "AvailableMmio = 0x%x\n", AvailableMmio));
  DEBUG ((DEBUG_INFO, "RequiredLowMmio = 0x%x\n", RequiredLowMmio));
  if (AvailableMmio < RequiredLowMmio) {
    RequiredMmio += (RequiredLowMmio - AvailableMmio);
  }

  DEBUG ((DEBUG_INFO, "RequiredMmio = 0x%x\n", RequiredMmio));

  return RequiredMmio;
}

/**
  This function overrides the default configurations in the UPD data region.

  @param   UpdDataRgn   A pointer to the UPD_DATA_REGION data strcture.

  @return  None
**/
VOID
PlatformLateInit (
  VOID
)
{
  ///* Set C0000-FFFFF to access RAM on both reads and writes */
  SetHostBridgeRegisterData (HostBridgePam0CfgRegLateInit, HostBridgePam0LateInitWriteData, 0);
  //
  //Offset 0x87 is LAC register and hence a DWORD access should be avoided.
  //
  SetHostBridgeRegisterData (HostBridgePam4CfgReg, HostBridgePam4WriteData, 0);
  SetHostBridgeRegisterData (HostBridgePam5CfgReg, HostBridgePam5WriteData, 0);
  SetHostBridgeRegisterData (HostBridgePam6CfgReg, HostBridgePam6WriteData, 0);

}

/**
  This function is to Set WAC/CP for IMRGLOBAL_BM
  @retval None
**/
VOID
SetImrGlobalSai (
  VOID
)
{
  UINT64                 MchBar;
  UINT64                 Wac;
  UINT64                 Cp;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  Wac = 0;
  Cp  = 0;
  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  Wac = B_PM_PCS_SAI | B_PM_IOSS_SAI | B_DFX_INTEL_MANUFACTURING_SAI | B_DFX_INTEL_PRODUCTION_SAI;
  Cp  = B_PM_PCS_SAI | B_PM_IOSS_SAI | B_DFX_INTEL_MANUFACTURING_SAI | B_DFX_INTEL_PRODUCTION_SAI;

  MmioWrite64 (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_IMRGLOBAL_WAC_IOC_MCHBAR_REG), (UINT64) Wac);  // WAC
  MmioWrite64 (MchBar + GET_NOC_EFFECTIVE_ADDRESS (SAF_IMRGLOBAL_CP_IOC_MCHBAR_REG), (UINT64) Cp);  // CP

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}
