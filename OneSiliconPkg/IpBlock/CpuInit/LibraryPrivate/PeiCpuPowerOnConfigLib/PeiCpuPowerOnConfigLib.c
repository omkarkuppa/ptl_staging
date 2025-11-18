/** @file
  CpuPowerOnConfig PEI Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include <Library/CpuPowerOnConfigLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiCpuInitFruLib.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <Library/ConfigBlockLib.h>
#include <IntelRcStatusCode.h>
#include <Library/PmcPrivateLib.h>
#include <Library/PmcSocLib.h>
#include <Library/CpuMailboxLib.h>
#include <Ppi/Spi.h>
#include <Register/GenerationMsr.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PcdLib.h>
#include <Register/B2pMailbox.h>
#include <Register/Ptl/Cpu/CpuStrapSetDefinition.h>

/**
  Check if VT is fused and disabled by Setup Option so reset is needed.

  @param[in] VmxEnable - VmxEnable CPU policy.
  @param[out] ColdResetFlag - Pointer to flag to indicate whether cold reset should be performed
  @param[out] WarmResetFlag - Pointer to flag to indicate whether warm reset should be performed

**/
VOID
CheckVmxIfNeedReset (
  IN UINT8            VmxEnable,
  OUT BOOLEAN         *ColdResetFlag,
  OUT BOOLEAN         *WarmResetFlag
  )
{
  MSR_IA32_FEATURE_CONTROL_REGISTER Msr;
  CPUID_VERSION_INFO_ECX            Ecx;

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);

  ///
  /// Check if VMX is supported
  ///
  if (Ecx.Bits.VMX == 0) {
    return ;
  }

  Msr.Uint64 = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);

  ///
  /// Need to reset only if locked and VMX state has to be changed.
  ///
  if ((Msr.Bits.Lock == 1) && (Msr.Bits.EnableVmxOutsideSmx != VmxEnable)) {
    *ColdResetFlag = TRUE;
  }
}

/**
  Set Cpu Strap and Set Early Power On Configuration setting for feature change.

  @param[in] SiPreMemPolicyPpi    - Address of the SI_PREMEM_POLICY_PPI.
**/
VOID
EFIAPI
SetCpuStrapAndEarlyPowerOnConfig (
  IN SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                    Status;
  BOOLEAN                       ColdResetFlag;
  BOOLEAN                       WarmResetFlag;
  UINT32                        CpuStrapSet1High;
  UINT32                        CpuStrapSet2Low;
  UINT32                        CpuStrapSet2High;
  UINT32                        CpuStrapSet3Low;
  EFI_BOOT_MODE                 BootMode;
  CPU_INIT_PREMEM_CONFIG        *CpuInitPreMemConfig;
  MSR_FLEX_RATIO_REGISTER       MsrFlexRatio;
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  OVERCLOCKING_PREMEM_CONFIG    *OverClockingConfig;
#endif

  ColdResetFlag      = FALSE;
  WarmResetFlag      = FALSE;
  CpuStrapSet1High   = 0;
  CpuStrapSet2Low    = 0;
  CpuStrapSet2High   = 0;
  CpuStrapSet3Low    = 0;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_SET_CPUSTRAP_AND_EARLYPOWERONCONFIG_START); //PostCode (0xC13)

  Status = PeiServicesGetBootMode (&BootMode);
  DEBUG ((DEBUG_INFO, "BootMode = %X\n", BootMode));
  if ((Status == EFI_SUCCESS) && (BootMode == BOOT_ON_S3_RESUME)) {
    PmcLockSetStrapMsgInterface ();
    return;
  }
  DEBUG ((DEBUG_INFO, "Set CPU Soft Straps Entry Point\n"));

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &CpuInitPreMemConfig);
  ASSERT_EFI_ERROR (Status);

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gOverclockingPreMemConfigGuid, (VOID *) &OverClockingConfig);
  ASSERT_EFI_ERROR (Status);
#endif

  ///
  /// Get Cpu Strap Set Data
  ///
  Status = GetCpuStrapData (&CpuStrapSet1High, &CpuStrapSet2Low, &CpuStrapSet2High, &CpuStrapSet3Low, NULL);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Current CPU Strap Set 1 High = 0x%08X\n", CpuStrapSet1High));
    DEBUG ((DEBUG_INFO, "Current CPU Strap Set 2 Low = 0x%08X\n", CpuStrapSet2Low));
    DEBUG ((DEBUG_INFO, "Current CPU Strap Set 2 High = 0x%08X\n", CpuStrapSet2High));
    DEBUG ((DEBUG_INFO, "Current CPU Strap Set 3 Low = 0x%08X\n", CpuStrapSet3Low));
    PeiCpuStrapSetActiveCores (&CpuStrapSet1High, (UINT8) CpuInitPreMemConfig->ActiveCoreCount, (UINT8) CpuInitPreMemConfig->ActiveSmallCoreCount, CpuInitPreMemConfig->ActiveLpAtomCoreCount, &ColdResetFlag, &WarmResetFlag);
    PeiCpuStrapBistEnableDisable (&CpuStrapSet1High, (UINT8) CpuInitPreMemConfig->BistOnReset, &ColdResetFlag, &WarmResetFlag);
    PeiCpuStrapReduceXecoresEnableDisable (&CpuStrapSet1High, (UINT8) CpuInitPreMemConfig->ReduceXecores, &ColdResetFlag, &WarmResetFlag);
    ///
    /// Perform Flex Ratio if processor is fused to perform Flex Ratio
    ///
    if (CpuInitPreMemConfig->CpuRatio != 0) {
      DEBUG ((DEBUG_INFO, "PeiCpuStrapProgramFlexMultiplier is getting programmend, CpuRatio from Policy is 0x%X\n", CpuInitPreMemConfig->CpuRatio));
      PeiCpuStrapProgramFlexMultiplier (&CpuStrapSet1High, (UINT8) CpuInitPreMemConfig->CpuRatio, &ColdResetFlag, &WarmResetFlag);
    } else {
      DEBUG ((DEBUG_INFO, "SKIPPING PeiCpuStrapProgramFlexMultiplier - CpuRatio is 0\n"));
      MsrFlexRatio.Uint64 = AsmReadMsr64 (MSR_FLEX_RATIO);
      DEBUG ((DEBUG_INFO, "Current MSR_FLEX_RATIO value: 0x%016llX\n", MsrFlexRatio.Uint64));
      DEBUG ((DEBUG_INFO, "Current CPU_STRAP_SET FlexRatio value: 0x%X\n", ((CPU_STRAP_SET *) &CpuStrapSet1High)->FlexRatio));
      if (MsrFlexRatio.Bits.FlexRatio != 0) {
        MsrFlexRatio.Bits.Enable = 1;
        MsrFlexRatio.Bits.FlexRatio = 0;
        AsmWriteMsr64 (MSR_FLEX_RATIO, MsrFlexRatio.Uint64);
        DEBUG ((DEBUG_INFO, "Updated MSR_FLEX_RATIO value: 0x%016llX\n", MsrFlexRatio.Uint64));
        ((CPU_STRAP_SET *) &CpuStrapSet1High)->FlexRatio = 0;
        DEBUG ((DEBUG_INFO, "Updated CPU_STRAP_SET FlexRatio value: 0x%X\n", ((CPU_STRAP_SET *) &CpuStrapSet1High)->FlexRatio));
        WarmResetFlag = TRUE;
      }
    } 
    PeiCpuStrapFastWakeupEnableDisable (&CpuStrapSet1High, (UINT8) CpuInitPreMemConfig->BootMaxFrequency, &ColdResetFlag, &WarmResetFlag);

#if FixedPcdGetBool(PcdOverclockEnable) == 1
    if (OverClockingConfig->OcSupport == 1) {
      PeiCpuStrapDlvrSetting (&CpuStrapSet1High, (UINT8) OverClockingConfig->CpuDlvrMode, &ColdResetFlag, &WarmResetFlag);
    }
#endif

    if (ColdResetFlag || WarmResetFlag) {
      DEBUG ((DEBUG_INFO, "New CPU Strap Set 1 High = 0x%08X\n", CpuStrapSet1High));
      DEBUG ((DEBUG_INFO, "New CPU Strap Set 2 Low = 0x%08X\n", CpuStrapSet2Low));
      DEBUG ((DEBUG_INFO, "New CPU Strap Set 2 High = 0x%08X\n", CpuStrapSet2High));
      DEBUG ((DEBUG_INFO, "New CPU Strap Set 3 Low = 0x%08X\n", CpuStrapSet3Low));
      Status = SetCpuStrapData (&CpuStrapSet1High, &CpuStrapSet2Low, &CpuStrapSet2High, &CpuStrapSet3Low, NULL);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "SetCpuStrapData: return %r\n", Status));
      }
    }
  } else {
    DEBUG ((DEBUG_ERROR, "GetCpuStrapData: return %r\n", Status));
  }

  CheckVmxIfNeedReset ((UINT8) CpuInitPreMemConfig->VmxEnable, &ColdResetFlag, &WarmResetFlag);

  if (ColdResetFlag || WarmResetFlag) {
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_SET_CPUSTRAP_AND_EARLYPOWERONCONFIG_RESET); //PostCode (0xC14)
    DEBUG ((DEBUG_INFO, "%a Reset\n", __FUNCTION__));
    if (ColdResetFlag) {
      DEBUG ((DEBUG_INFO, "Cpu straps changed, performing cold reset\n"));
      (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetCold, EFI_SUCCESS, 0, NULL);
    } else if (WarmResetFlag) {
      DEBUG ((DEBUG_INFO, "Cpu straps changed, performing warm reset\n"));
      (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetWarm, EFI_SUCCESS, 0, NULL);
    }
  }
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_SET_CPUSTRAP_AND_EARLYPOWERONCONFIG_DONE); //PostCode (0xC14)
}

