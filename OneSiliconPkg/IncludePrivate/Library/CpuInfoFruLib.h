 /** @file
  This file contains Cpu Information for specific generation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _CPU_INFO_FRU_LIB_H_
#define _CPU_INFO_FRU_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <CpuRegs.h>
#include <Register/CpuGenInfoRegs.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MmPciLib.h>
#include <Library/PcdLib.h>
#include <Pi/PiMultiPhase.h>
#include <Protocol/MpService.h>

///
/// Override table structure for cTDP and non-cTDP skus.
/// Non-cTDP parts would have '0' data for TDP level information.
///
typedef struct {
  UINT32  MsrPowerLimit1;
  UINT32  MsrPowerLimit2;
  UINT32  CtdpUpPowerLimit2;
  UINT32  CtdpNominalPowerLimit2;
  UINT32  CtdpDownPowerLimit2;
  UINT32  MsrPowerLimit4;
  UINT32  MsrPowerLimit4DisableFvm;
  UINT8  MsrPowerLimit1TimeWindow;
  UINT8  IsysCurrentL1Tau;
  UINT16 IsysCurrentLimitL1;
  UINT16 IsysCurrentLimitL2;
  UINT16 VsysMax;
  UINT16 PsysPmax;
} PPM_PLX_PROFILE;

typedef struct {
  UINT32          CpuIdentifier;
  PPM_PLX_PROFILE *Plx;
} PPM_OVERRIDE_TABLE;

/**
  Return CPU Sku

  @param[in]  UINT32             CpuFamilyModel
  @param[in]  UINT16             CpuDid

  @retval     UINT8              CPU Sku
**/
UINT8
EFIAPI
GetCpuSkuInfo (
  IN UINT32    CpuFamilyModel,
  IN UINT16    CpuDid
  );

/**
  This function returns the HUB DIE ID.

  @param[out] *HubDieId             - variable that will store the DIE ID for HUB DIE.

  @retval     EFI_SUCCESS           Get the Hub Die Id successfully.
  @retval     EFI_NOT_FOUND         No Hub DIE in current product
  @retval     EFI_INVALID_PARAMETER Input pointer is NULL.
**/
EFI_STATUS
EFIAPI
GetHubDieIdFru (
  OUT UINT8 *HubDieId
);

/**
  This function returns the possible CDIE ID list.
  Do not free the buffer of PossibleCDieIds

  @param[out]  *NumberOfFusedCDie  - Output list count of PossibleCDieIds.
  @param[out]  *PossibleCDieIds    - Output possible CDIE ID list.

  @retval     EFI_SUCCESS           Get the possible CDIE ID list successfully.
  @retval     EFI_NOT_FOUND         No possible CDIE in current product
**/
EFI_STATUS
EFIAPI
GetPossibleCDieIdsFru (
  OUT UINTN                    *NumberOfPossibleCDie,
  OUT UINT8                    **PossibleCDieIds
);

/**
  This function returns the fused cores count for specific DIE.

  @param[in]  DieId                        - DIE ID.
  @param[out] *NumberOfSupportedCores      - variable that will store fuse P cores in current DIE.
  @param[out] *NumberOfSupportedAtomCores  - variable that will store fuse E cores in current DIE.
  @param[out] *NumberOfSupportedAtomCores  - variable that will store fuse LP cores in current DIE.

  @retval     EFI_SUCCESS      Get the fused core count successfully.
  @retval     EFI_NOT_FOUND    Current DIE ID doesn't exist in fuse config
  @retval     EFI_UNSUPPORTED  Doesn't support to get the fused core count.
**/
EFI_STATUS
EFIAPI
GetCpuFusedCoreCountFru (
  IN  UINT8                     DieId,
  OUT UINT8                     *NumberOfFusedPCore,     OPTIONAL
  OUT UINT8                     *NumberOfFusedECore,     OPTIONAL
  OUT UINT8                     *NumberOfFusedLpECore    OPTIONAL
  );

/**
  Get the fused CDIE list.
  Caller need to prepare the buffer and pass the list count in the first parameter

  @param[in,out]  NumberOfFusedCDie   - As input, it is the max list count of CDieIds.
                                      - As outout, it is the number of fused CDIES
  @param[out]     *CDieIds            - Output CDie ID list.

  @retval     EFI_SUCCESS             Get the fused CDIE list successfully.
  @retval     EFI_INVALID_PARAMETER   Input parameter is invalid
  @retval     EFI_BUFFER_TOO_SMALL    Input list count is too small
  @retval     EFI_UNSUPPORTED         Doesn't support to get the CDIE list.
**/
EFI_STATUS
EFIAPI
GetFusedCDieIdsFru (
  IN OUT UINTN                    *NumberOfFusedCDie,
     OUT UINT8                    *CDieIds
  );

/**
  This function is used to get enabled LP Atom cores and threads off ring

  @param[in]  *ProcessorInfo         - Pointer to EFI_PROCESSOR_INFORMATION
  @param[in]  NumberOfProcessors     - Number of processors
  @param[out] *NumberOfLpAtomCores   - variable that will store enabled cores off ring
  @param[out] *NumberOfLpAtomThreads - variable that will store enabled threads off ring
**/
VOID
EFIAPI
GetLpECoreThreadCountInfoFru (
  IN  CONST EFI_PROCESSOR_INFORMATION *ProcessorInfo,
  IN  UINTN                           NumberOfProcessors,
  OUT UINT8                           *NumberOfLpAtomCores,   OPTIONAL
  OUT UINT8                           *NumberOfLpAtomThreads  OPTIONAL
  );

/**
  Returns Generation string of the respective CPU

  @param[in]   CpuFamilyId

  @retval      Character pointer of Generation string
**/
CONST CHAR8*
EFIAPI
GetFruGenerationString (
  IN   UINT32   CpuFamilyId
  );

/**
  Return MCP stepping based on compute die SRID
  @retval  MCP stepping
**/
UINT8
McpGetStepping (
  VOID
  );

/**
  Returns Revision Table string

  @param[in]   CpuFamilyId

  @retval      Character pointer of Revision Table string
**/
CONST CHAR8*
EFIAPI
GetRevisionTableString (
  UINT32                CpuId
  );

/**
  Clear all MCA bank status.
**/
VOID
EFIAPI
McaBankStatusClear (
  VOID
  );

/**
  Returns power limits Table

  @param[in]      NoOfOverrides

  @retval         override table pointer of power limits Table
**/
PPM_OVERRIDE_TABLE*
EFIAPI
GetFruPowerLimits (
  IN UINTN        *NoOfOverrides
  );

/**
  This function is used to Patch SmmSupovrStateLock.

  @retval This corresponds to bit 2 of MSR_SMM_SUPOVR_STATE_LOCK_REGISTER. When set, prevents WRMSR to IA32_SMM_MONITOR_CTL (aka MSEG) MSR.

**/
BOOLEAN
EFIAPI
SmmSupovrStateLockPatch (
  VOID
  );

/**
  This function returns the supported Physical Address Size

  @retval returns the supported Physical Address Size.
**/
UINT8
EFIAPI
GetMaxPhysicalAddressSizeFru (
  VOID
  );

/**
  Return IPC scaling * 100 per processor.

  @param[in]  ProcessorInfo      Pointer to EFI_PROCESSOR_INFORMATION
  @param[out] IpcScaling         Pointer to each processor's IpcScaling
  @param[in]  NumberOfProcessors Number of processors
**/
VOID
EFIAPI
GetIpcScalingFru (
  IN  CONST EFI_PROCESSOR_INFORMATION  *ProcessorInfo,
  OUT       UINT8                      *IpcScaling,
  IN        UINTN                      NumberOfProcessors
  );

/**
  Get Pm Logic ID. If not support Pm Logic ID, return -1.

  @retval Pm Logic ID
**/
UINT64
EFIAPI
GetPmLogicId (
  VOID
  );

/**
  Return the per processor P1 ratio for each processor.

  @param[in]  ProcessorInfo      Pointer to EFI_PROCESSOR_INFORMATION
  @param[out] P1Ratio            Pointer to each processor's P1 ratio
  @param[in]  NumberOfProcessors Number of processors
**/
VOID
GetP1RatioFru (
  IN  EFI_PROCESSOR_INFORMATION  *ProcessorInfo,
  OUT UINT8                      *P1Ratio,
  IN  UINTN                      NumberOfProcessors
  );

/**
  Check Overclocking Enabled or Disabled

  @retval TRUE   OC is enabled.
  @retval FALSE  OC is disabled.
**/
BOOLEAN
EFIAPI
IsOcEnabled (
  VOID
  );


/**
  Function is used to check and verify PrmrrSize.
  1. if PrmrrSize is zero, return silicon recommended value
  2. if PrmrrSize is non-zero and valid, return this valid PrmrrSize
  3. if PrmrrSize is non-zero and invalid, return 0

  @param[in]  TdxEnable    If Tdx is enabled
  @param[in]  PrmrrSize    Size of Prmrr

  @retval Recommended PrmrrSize
**/
UINT32
EFIAPI
CpuGetAndVerifyPrmrrSize (
  IN BOOLEAN  TdxEnable,
  IN UINT32   PrmrrSize
  );

/**
  Returns Socket Type String

  @param[in]  CpuFamilyId

  @retval     Character pointer of Socket Type String
**/
CHAR8*
EFIAPI
GetSocketTypeFru (
  IN UINT32  CpuFamilyId
  );

#endif // _CPU_INFO_FRU_LIB_H_
