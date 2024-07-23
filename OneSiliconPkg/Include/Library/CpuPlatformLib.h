/** @file
  Header file for CpuPlatform Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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

#ifndef _CPU_PLATFORM_LIB_H_
#define _CPU_PLATFORM_LIB_H_

#include <Uefi.h>
#include <CpuRegs.h>
#include <Register/CpuGenInfoRegs.h>
#include <CpuPowerMgmt.h>
#include <Pi/PiMultiPhase.h>
#include <Protocol/MpService.h>

#define ATOM_QUANTITY_IN_CLUSTER              4

/**
  Return CPU Sku

  @retval UINT8              CPU Sku
**/
UINT8
EFIAPI
GetCpuSku (
  VOID
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
GetHubDieId (
  OUT UINT8 *HubDieId
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
GetFusedCDieIds (
  IN OUT UINTN                    *NumberOfFusedCDie,
     OUT UINT8                    *CDieIds
  );

/**
  Detect if DLVR Phase Modulation SSC supported or not by reading it from PCODE mailbox.

  @retval TRUE - Supported
  @retval FALSE - Not Supported
**/
BOOLEAN
EFIAPI
IsDlvrPhaseSscSupported (
  VOID
  );

/**
  This function returns Number of enabled cores in the package.

  @retval Number of enabled cores in the package.
**/
UINT16
EFIAPI
GetEnabledCoreCount (
  VOID
  );

/**
  This function returns Number of enabled Threads in the package.

  @retval Number of enabled threads in the package.
**/
UINT16
EFIAPI
GetEnabledThreadCount (
  VOID
  );

/**
  This function checks whether uCode loaded from FIT.

  @retval TRUE  - uCode loaded from FIT successful
  @retval FALSE - Failed on FIT to load uCode
**/
BOOLEAN
EFIAPI
IsValiduCodeEntry (
  VOID
  );

/**
  Return if CPU supports PFAT

  @retval TRUE             If CPU Supports
  @retval FALSE            If CPU doesn't Supports
**/
BOOLEAN
EFIAPI
IsPfatEnabled (
  VOID
  );

/**
  Is BIOS GUARD enabled.

  @retval TRUE   BIOS GUARD is supported and enabled.
  @retval FALSE  BIOS GUARD is disabled.
**/
BOOLEAN
EFIAPI
IsBiosGuardEnabled (
  VOID
  );

/**
  Determine if CPU supports Intel Turbo Boost Max Technology 3.0 (ITBM).

  @retval TRUE   ITBM is supported and enabled.
  @retval FALSE  ITBM is disabled.
**/
BOOLEAN
EFIAPI
IsItbmSupported (
  VOID
);

/**
  Determine if CPU supports Resource Priority Feature (RP).

  @retval TRUE   Rp is supported and enabled.
  @retval FALSE  Rp is disabled.
**/
BOOLEAN
EFIAPI
IsResourcePriorityFeatureSupported (
  VOID
);

/**
  Determine if CPU supports Programmable Core Ratio Limit for the Turbo mode.

  @retval TRUE   Core Ratio Limit for the Turbo mode is supported and enabled.
  @retval FALSE  Core Ratio Limit for the Turbo mode is disabled.
**/
BOOLEAN
EFIAPI
IsCoreRatioLimitSupported (
  VOID
);

/**
  Determine if CPU supports Programmable TDC/TDP Limit for the Turbo mode.

  @retval TRUE   TDC/TDP Limit is supported and enabled.
  @retval FALSE  TDC/TDP Limit is disabled.
**/
BOOLEAN
EFIAPI
IsXETdcTdpLimitSupported (
  VOID
);

/**
  Determine if CPU supports Turbo mode.

  @retval TRUE   Turbo mode is supported and enabled.
  @retval FALSE  Turbo mode is disabled.
**/
BOOLEAN
EFIAPI
IsTurboModeSupported (
  VOID
);

/**
  Determine if CPU supports PPIN (Protected Processor Inventory Number)

  @retval TRUE   PPIN feature is available.
  @retval FALSE  PPIN feature is not available.
**/
BOOLEAN
EFIAPI
IsPpinFeatureAvailable (
  VOID
  );

/**
  Determine if CPU supports Hardware P-States.

  @retval TRUE   Hardware P-States is supported and enabled.
  @retval FALSE  Hardware P-States is disabled.
**/
BOOLEAN
EFIAPI
IsHwpSupported (
  VOID
  );

/**
  Determine if CPU supports Mwait.

  @retval TRUE   Mwait is supported and enabled.
  @retval FALSE  Mwait is disabled.
**/
BOOLEAN
EFIAPI
IsTimedMwaitSupported (
  VOID
  );

/**
  Determine if CPU supports LPM.

  @retval TRUE   LPM is supported and enabled.
  @retval FALSE  LPM is disabled.
**/
BOOLEAN
EFIAPI
IsLpmSupported (
  VOID
  );

/**
  Determine if CPU supports ConfigTdp.

  @retval TRUE   ConfigTdp is supported and enabled.
  @retval FALSE  ConfigTdp is disabled.
**/
BOOLEAN
EFIAPI
IsConfigTdpSupported (
  VOID
  );

/**
  Determine if CPU supports Turbo mode.

  @retval TRUE   Efficiency Turbo mode is supported and enabled.
  @retval FALSE  Efficiency Turbo mode is disabled.
**/
BOOLEAN
EFIAPI
IsEnergyEfficientTurboSupported (
  VOID
  );

/**
  Determines what Ctdp levels are supported on the silicon.

  @param[in] CtdpLevel1Supported     - Pointer to CtdpLevel1Supported
  @param[in] CtdpLevel2Supported     - Pointer to CtdpLevel2Supported

  @retval VOID
**/
VOID
EFIAPI
GetConfigTdpLevelsSupported (
  IN BOOLEAN     *CtdpLevel1Supported,
  IN BOOLEAN     *CtdpLevel2Supported
  );

/**
  Determines what Ctdp levels are supported on the silicon.

  @param[in] MwaitEcx     - Pointer to MwaitEcx (CPUID_MONITOR_MWAIT_ECX)
  @param[in] MwaitEdx     - Pointer to MwaitEdx (CPUID_MONITOR_MWAIT_EDX)

  @retval VOID
**/
VOID
EFIAPI
GetSubCStateSupported (
  IN UINT32     *MwaitEcx,
  IN UINT32     *MwaitEdx
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
GetCpuFusedCoreCount (
  IN  UINT8                     DieId,
  OUT UINT8                     *NumberOfFusedPCore,     OPTIONAL
  OUT UINT8                     *NumberOfFusedECore,     OPTIONAL
  OUT UINT8                     *NumberOfFusedLpECore    OPTIONAL
  );

/**
  Get the enabled CDIE list.
  Caller need to prepare the buffer and pass the list count in the first parameter

  @param[in,out]  NumberOfEnabledCDie - As input, it is the max list count of CDieIds.
                                      - As outout, it is the number of enabled CDIES
  @param[out]     *CDieIds            - Output CDie ID list.

  @retval     EFI_SUCCESS             Get the enabled CDIE list successfully.
  @retval     EFI_INVALID_PARAMETER   Input parameter is invalid
  @retval     EFI_BUFFER_TOO_SMALL    Input list count is too small
  @retval     EFI_UNSUPPORTED         Doesn't support to get the CDIE list.
**/
EFI_STATUS
EFIAPI
GetEnabledCDieIds (
  IN OUT UINTN                    *NumberOfEnabledCDie,
     OUT UINT8                    *CDieIds
  );

/**
  This function is used to get enabled LP Atom cores and threads off ring

  @param[in]  *ProcessorInfo         - Pointer to EFI_PROCESSOR_INFORMATION
  @param[in]  NumberOfProcessors     - Number of processors
  @param[out] *NumberOfLpAtomCores   - variable that will store enabled cores off ring
  @param[out] *NumberOfLpAtomThreads - variable that will store enabled threads off ring

  @retval     EFI_SUCCESS             Get enabled LP Atom cores and threads.
  @retval     EFI_INVALID_PARAMETER   Doesn't support to get enabled LP Atom cores and threads.
**/
EFI_STATUS
EFIAPI
GetLpECoreThreadCountInfo (
  IN  CONST EFI_PROCESSOR_INFORMATION *ProcessorInfo,
  IN  UINTN                           NumberOfProcessors,
  OUT UINT8                           *NumberOfLpAtomCores,   OPTIONAL
  OUT UINT8                           *NumberOfLpAtomThreads  OPTIONAL
  );

/**
  Detect Supported CPU Features

  @param[InOut] CpuEcx   Pointer to CpuEcx (CPUID_VERSION_INFO_ECX).
**/
VOID
EFIAPI
GetSupportedCpuFeatures (
  IN UINT32     *CpuEcx
  );

/**
  Detect if Processor Trace supported or not

  @retval TRUE   IntelProcessorTrace is supported and enabled.
  @retval FALSE  IntelProcessorTrace is disabled.
**/
BOOLEAN
EFIAPI
IsIntelProcessorTraceSupported (
  VOID
  );


/**
  Returns Generation string of the respective CPU

  @retval      Character pointer of Generation string
**/
CONST CHAR8*
EFIAPI
GetGenerationString (
  VOID
  );

/**
  Returns Revision Table

  @retval      Character pointer of Revision Table String
**/
CONST CHAR8*
EFIAPI
GetRevisionTable (
  VOID
  );

/**
  Returns Sku String

  @retval      Character pointer of Sku String
**/
CONST CHAR8*
EFIAPI
GetSkuString (
  VOID
  );

/**
  Returns the value of Brand Identity MSR if it is supported.

  @param[out] *MsrBrandIdentityValue       Brand Identity MSR value

  @retval     EFI_SUCCESS       Brand Identity MSR is supported
  @retval     EFI_UNSUPPORTED   Brand Identity MSR is not supported
**/
EFI_STATUS
GetMsrBrandIdentityValue (
  OUT UINT32 *MsrBrandIdentityValue
  );

/**
  This function return max and min bus ratio.

  @param[out]  MaxBusRatio
  @param[out]  MinBusRatio
**/
VOID
EFIAPI
GetBusRatio (
  OUT UINT8 *MaxBusRatio, OPTIONAL
  OUT UINT8 *MinBusRatio OPTIONAL
  );

/**
  This function return max Efficiency Ratio.

  @retval Max efficiency ratio
**/
UINT8
EFIAPI
GetMaxEfficiencyRatio (
  VOID
  );

/**
  This function return max Non-Turbo Ratio.

  @retval Max Non-Turbo Ratio
**/
UINT8
EFIAPI
GetMaxNonTurboRatio (
  VOID
  );

/**
  This function return the supported Config TDP Levels.

  @retval number of config TDP levels
**/
UINT8
EFIAPI
GetConfigTdpLevels (
  VOID
  );

/**
  This function returns the supported Physical Address Size

  @retval supported Physical Address Size.
**/
UINT8
EFIAPI
GetMaxPhysicalAddressSize (
  VOID
  );

/**
  Return if Edram Enable

  @retval TRUE             If Edram Enable
  @retval FALSE            If Edram Disable
**/
BOOLEAN
EFIAPI
IsEdramEnable (
  VOID
  );

/**
  This function return if TME is supported

  @retval TRUE             If TME is supported
  @retval FALSE            If TME is not supported
**/
BOOLEAN
EFIAPI
IsTmeSupported (
  VOID
  );

/**
  This function return if PConfig is supported

  @retval TRUE             If PConfig is supported
  @retval FALSE            If PConfig is not supported
**/
BOOLEAN
EFIAPI
IsPConfigSupported (
  VOID
  );

/**
  This function return if MKTME PConfig is supported

  @retval TRUE             If MKTME PConfig is supported
  @retval FALSE            If MKTME PConfig is not supported
**/
BOOLEAN
EFIAPI
IsMkTmePConfigSupported (
  VOID
  );

/**
  This function return if SEAM Mode is supported

  @retval TRUE             If SEAM Mode is supported
  @retval FALSE            If SEAM Mode is not supported
**/
BOOLEAN
EFIAPI
IsSeamModeSupported (
  VOID
  );

/**
  Return if the processor is a preproduction sample

  @retval TRUE          If the processor is a preproduction sample
  @retval FALSE         If the part is intended for production
**/
BOOLEAN
EFIAPI
IsSamplePart (
  VOID
  );

/**
  Write a value to Special Chipset Usage MSR.

  @param[in] Data32   The 32-bit value to write to the MSR.

  @retval VOID
**/
VOID
EFIAPI
WriteSpclChipsetUsageMsr (
  IN UINT32 Data32
  );

/**
  Detect the type of core, whether it is Big/Small Core.

  @param[out]    CoreType      Output pointer that get CPUID_NATIVE_MODEL_ID_INFO data
                 10h - Quark
                 20h - Atom
                 30H - Knights
                 40H - Core
**/
UINT8
EFIAPI
CpuGetCoreType (
  VOID
  );
#endif
