/** @file
  This file contains generation specific interfaces for PEI CPU initialization.
  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#ifndef _PEI_CPU_INIT_FRU_LIB_H_
#define _PEI_CPU_INIT_FRU_LIB_H_

#include <ConfigBlock.h>
#include <CpuPowerMgmtBasicConfig.h>
#include <CpuPowerMgmtVrConfig.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuCommonLib.h>

/**
  Compare the number of active cores setup option against the CPU strap setting
  and in case of mismatch request a reset.

  @param[in] CpuStrapSetData             - The current strap setting.
  @param[in] ActiveCoreCount             - Active big core count.
  @param[in] ActiveSmallCoreCount        - Active small core count.
  @param[in] ActiveLpAtomCoreCount       - Active LP atom core count.
  @param[out] ColdResetFlag              - Pointer to flag to indicate whether cold reset should be performed
  @param[out] WarmResetFlag              - Pointer to flag to indicate whether warm reset should be performed

**/
VOID
EFIAPI
PeiCpuStrapSetActiveCores (
  IN VOID              *CpuStrapSetData,
  IN UINT8             ActiveCoreCount,
  IN UINT8             ActiveSmallCoreCount,
  IN UINT8             ActiveLpAtomCoreCount,
  OUT BOOLEAN          *ColdResetFlag,
  OUT BOOLEAN          *WarmResetFlag
  );

/**
  Compare the BIST setup option against the CPU strap setting
  and in case of mismatch request a reset.

  @param[in] CpuStrapSetData    - The current strap setting.
  @param[in] BistOnReset        - BistOnReset CPU Test Config Policy.
  @param[out] ColdResetFlag     - Pointer to flag to indicate whether cold reset should be performed
  @param[out] WarmResetFlag     - Pointer to flag to indicate whether warm reset should be performed

**/
VOID
EFIAPI
PeiCpuStrapBistEnableDisable (
  IN VOID              *CpuStrapSetData,
  IN UINT8             BistOnReset,
  OUT BOOLEAN          *ColdResetFlag,
  OUT BOOLEAN          *WarmResetFlag
  );

/**
  Compare the flex multiplier setup options against the CPU strap settings
  and in case of mismatch request a reset.

  @param[in] CpuStrapSetData    - The current strap setting.
  @param[in] CpuRatio           - CpuRatio CPU policy.
  @param[out] ColdResetFlag     - Pointer to flag to indicate whether cold reset should be performed
  @param[out] WarmResetFlag     - Pointer to flag to indicate whether warm reset should be performed

**/
VOID
EFIAPI
PeiCpuStrapProgramFlexMultiplier (
  IN VOID                        *CpuStrapSetData,
  IN UINT8                       CpuRatio,
  OUT BOOLEAN                    *ColdResetFlag,
  OUT BOOLEAN                    *WarmResetFlag
  );

/**
  Compare the boot frequency setup option against the boot ratio strap setting
  and in case of mismatch request a reset.

  @param[in] CpuStrapSetData    - The current strap setting.
  @param[in] BootMaxFrequency   - BootMaxFrequency CPU policy.
  @param[out] ColdResetFlag     - Pointer to flag to indicate whether cold reset should be performed
  @param[out] WarmResetFlag     - Pointer to flag to indicate whether warm reset should be performed

**/
VOID
EFIAPI
PeiCpuStrapFastWakeupEnableDisable (
  IN VOID                *CpuStrapSetData,
  IN BOOLEAN             BootMaxFrequency,
  OUT BOOLEAN            *ColdResetFlag,
  OUT BOOLEAN            *WarmResetFlag
  );
/**
  Compare the Dlvr Power Gate Mode setup option against the CPU strap setting
  and in case of mismatch request a reset.

  @param[in] CpuStrapSetData    - The current strap setting.
  @param[in] CpuDlvrMode        - CpuDlvrMode Config Policy.
  @param[out] ColdResetFlag     - Pointer to flag to indicate whether cold reset should be performed
  @param[out] WarmResetFlag     - Pointer to flag to indicate whether warm reset should be performed

**/
VOID
EFIAPI
PeiCpuStrapDlvrSetting (
  IN VOID              *CpuStrapSetData,
  IN UINT8             CpuDlvrMode,
  OUT BOOLEAN          *ColdResetFlag,
  OUT BOOLEAN          *WarmResetFlag
  );

/**
  Return Total Memory Encryption (TME) default policy setting

  @retval Total Memory Encryption (TME) default policy setting
**/
UINT32
EFIAPI
PeiCpuTmeDefaultSetting (
  VOID
  );

/**
  Disable or enable CPU Crashlog dump feature by Mailbox.

  @param[in] BOOLEAN      Policy for CrashLog

**/
VOID
EFIAPI
PeiCpuCrashLogMailboxEnable (
  IN  BOOLEAN  CrashLogEnable
  );

/**
  Program the BIOS Parameters required for MCHECK module.
**/
VOID
EFIAPI
ProgramBiosParamsForMcheck (
  VOID
  );

/**
  Set the PRMRR region.

  @param PrmrrBase PRMRR base address
  @param PrmrrSize PRMRR size

  @retval RETURN_SUCCESS       PRMRR region is set successfully.
  @retval RETURN_UNSUPPORTED   PRMRR is not supported.
  @retval RETURN_ACCESS_DENIED PRMRR region is already locked and cannot be changed.
**/
RETURN_STATUS
EFIAPI
PeiCpuSetPrmrrRegion (
  UINT64 PrmrrBase,
  UINT32 PrmrrSize
  );

/**
  Set the SEAMRR MSRs.
  @param SeamrrBase SEAMRR base address
  @param SeamrrSize SEAMRR size
  @retval RETURN_SUCCESS       SEAMRR MSRs are set successfully.
  @retval RETURN_UNSUPPORTED   SEAMRR is not supported.
  @retval RETURN_ACCESS_DENIED SEAMRR MSRs are already locked and cannot be changed.
*/
RETURN_STATUS
EFIAPI
PeiCpuSetSeamrrRegion (
  UINT64 SeamrrBase,
  UINT32 SeamrrSize
  );

/**
  Set Seamldr SVN in BIOS_SE_SVN MSR.

  @param SeamldrSeSvn SeamldrSvn value to be set.

  @retval RETURN_SUCCESS            MSR is set successfully.
  @retval RETURN_SECURITY_VIOLATION SeamldrSeSvn is lower than value in the MSR
*/
RETURN_STATUS
EFIAPI
PeiCpuSetSeamldrSeSvn (
  UINT8 SeamldrSeSvn
  );

/**
  Configure DFD.

  @param[in] DfdEnable. TRUE, DFD enabled; FALSE, DFD disabled

  @retval EFI_UNSUPPORTED           Command unsupported
  @retval EFI_SUCCESS               Command succeeded
**/
EFI_STATUS
EFIAPI
PeiCpuDfdEnable (
  IN BOOLEAN               DfdEnable
  );

/**
  Returns MCU's 2nd Patch load Status.

  @retval TRUE 2nd Patch Load was success
  @retval FALSE 2nd Patch Load was fail.
**/
BOOLEAN
EFIAPI
MsrReadMcu2ndPatchStatusFru (
  VOID
);

/**
Update CDie Bar IA Exclusion.

**/
VOID
EFIAPI
PeiCpuProgramIaExclusion (
  VOID
  );

#endif // _PEI_CPU_INIT_FRU_LIB_H_
