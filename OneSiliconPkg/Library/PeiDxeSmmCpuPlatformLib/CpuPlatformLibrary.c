/** @file
  CPU Platform Lib implementation.

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
#include "CpuPlatformLibrary.h"
#include <Library/PciSegmentLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/MsrFruLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/BootGuardLib.h>
#include <Register/GenerationMsr.h>
#include <Register/CpuGenRegs.h>
#include <Pi/PiStatusCode.h>
#include <Library/CpuLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/CpuCommonLib.h>
#include <CpuPowerManagementPrivateDataHob.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/HobLib.h>
#include <Guid/CpuPmData.h>
#include <Pi/PiMultiPhase.h>
#include <Protocol/MpService.h>
#include <Register/B2pMailbox.h>
#include <Guid/MpInformation2.h>
#include <OcLimits.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>

#define SKIP_MICROCODE_CHECKSUM_CHECK 1

#define FIT_HEADER_ENTRY                                              0x0
#define FIT_SUCCESSFUL                                                0x0
#define FIT_SIZE_CHECK                                                0x1
#define FIT_RESERVED_FIELD_CHECK                                      0x2
#define FIT_VERSION_AND_TYPE_CHECK                                    0x3

#define FIT_MICROCODE_UPDATE_ENTRY                                    0x1
#define FIT_NO_MICROCODE_UPDATE                                       0x1
#define FIT_MICROCODE_UPDATE_FAIL                                     0x2

#define FIT_STARTUP_ACM_ENTRY                                         0x2
#define FIT_STARTUP_ACM_NOT_SUPPORTED                                 0x1
#define FIT_FATAL_ERROR_DURING_ACM                                    0x2
#define FIT_CPU_DOES_NOT_SUPPORT_LT                                   0x3
#define FIT_BIST_ERRORS                                               0x4
#define FIT_BEYOND_END_OF_FIT                                         0x5
#define FIT_NO_FIT_ACM_TYPE_MISMATCH                                  0x6
#define FIT_ACM_BASE_SIZE_AND_CHECKS                                  0x7

#define FIT_GENERAL_CHECKS                                            0x7F
#define FIT_DISABLED_BY_CPU                                           0x1
#define FIT_POINTER_ERROR                                             0x2
#define FIT_FIRST_FIT_ENTRY_MISMATCH                                  0x3

#define CPUID_1B                                                      0x1B
/**
  This function get MSR_FIT_BIOS_ERROR.

  @param[out]  FitEntryType
  @param[out]  FitErrorCode
**/
STATIC
VOID
MsrGetFitBiosError (
  OUT UINT8 *FitEntryType,
  OUT UINT8 *FitErrorCode
  )
{
  MSR_FIT_BIOS_ERROR_REGISTER FitData;
  FitData.Uint64 = AsmReadMsr64 (MSR_FIT_BIOS_ERROR);
  *FitEntryType = (UINT8) FitData.Bits.EntryType;
  *FitErrorCode = (UINT8) FitData.Bits.ErrorCode;
}

/**
  Return if CPU supports PFAT

  @retval TRUE             If CPU Supports
  @retval FALSE            If CPU doesn't Supports
**/
BOOLEAN
EFIAPI
IsPfatEnabled (
  VOID
  )
{
  MSR_PLATFORM_INFO_REGISTER  PlatformInfoMsr;
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  return (BOOLEAN) PlatformInfoMsr.Bits.PfatEnable;
}

/**
  Return CPU Sku

  @retval UINT8              CPU Sku
**/
UINT8
EFIAPI
GetCpuSku (
  VOID
  )
{
  UINT16                  CpuDid;
  UINT32                  CpuFamilyModel;
  CPUID_VERSION_INFO_EAX  Eax;

  ///
  /// Read the CPUID & DID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Eax.Uint32, NULL, NULL, NULL);
  CpuFamilyModel = Eax.Uint32 & CPUID_FULL_FAMILY_MODEL;
  CpuDid =  (UINT16) GetHostBridgeRegisterData (HostBridgeDeviceId, HostBridgeDeviceIdData);

  return GetCpuSkuInfo (CpuFamilyModel, CpuDid);

}

/**
  Returns the processor microcode revision of the processor installed in the system.

  @retval Processor Microcode Revision
**/
UINT32
EFIAPI
GetCpuUcodeRevision (
  VOID
  )
{
  MSR_IA32_BIOS_SIGN_ID_REGISTER  BiosSignMsr;

  AsmWriteMsr64 (MSR_IA32_BIOS_SIGN_ID, 0);
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, NULL, NULL);
  BiosSignMsr.Uint64 = AsmReadMsr64 (MSR_IA32_BIOS_SIGN_ID);

  return BiosSignMsr.Bits.MicrocodeUpdateSignature;
}

/**
  Detect if DLVR Phase SSC supported or not by reading it from PCODE mailbox.

  @retval TRUE - Supported
  @retval FALSE - Not Supported
**/
BOOLEAN
EFIAPI
IsDlvrPhaseSscSupported (
  VOID
  )
{
  UINT32                    MailboxStatus;
  MAILBOX_DATA_GET_DLVR_SOC MailboxData;
  EFI_STATUS                Status;
  PCODE_MAILBOX_INTERFACE   MailboxCommand;

  ///
  /// For DLVR Phase SSC, PCODE mailbox returns DLVR_PHASE_SSC_MODIFY_ALLOWED fuse in bit 0.
  ///
  MailboxCommand.InterfaceData = 0;
  MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_DLVR_SOC;
  MailboxCommand.Fields.Param1 = MAILBOX_PCODE_DLVR_PHASE_SSC_GET_SUBCOMMAND;
  MailboxData.Data32 = 0;
  Status = MailboxRead (MailboxCommand.InterfaceData, &MailboxData.Data32 , &MailboxStatus);
  if (Status != EFI_SUCCESS || MailboxStatus != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Mailbox read command failed unexpectedly, DLVR Phase SSC is not supported. MailboxStatus = %x , Mailbox command return status %r\n", MailboxStatus, Status));
    return FALSE;
  }

  return (BOOLEAN)MailboxData.Fields.PhaseSscSupported;
}

/**
  This function returns Number of enabled cores in the package.

  @retval Number of enabled cores in the package.
**/

UINT16
EFIAPI
GetEnabledCoreCount (
  VOID
  )
{
  MSR_CORE_THREAD_COUNT_REGISTER   MsrValue;

  MsrValue.Uint64 = AsmReadMsr64 (MSR_CORE_THREAD_COUNT);

  return (UINT16) MsrValue.Bits.Corecount;
}

/**
  This function returns Number of enabled Threads in the package.

  @retval Number of enabled threads in the package.
**/
UINT16
EFIAPI
GetEnabledThreadCount (
  VOID
  )
{
  MSR_CORE_THREAD_COUNT_REGISTER   MsrValue;

  MsrValue.Uint64 = AsmReadMsr64 (MSR_CORE_THREAD_COUNT);

  return (UINT16) MsrValue.Bits.Threadcount;
}

/**
  This function checks whether uCode loaded from FIT.

  @retval TRUE  - uCode loaded from FIT successful
  @retval FALSE - Failed on FIT to load uCode
**/
BOOLEAN
EFIAPI
IsValiduCodeEntry (
  VOID
  )
{
  UINT8                       FitEntryType;
  UINT8                       FitErrorCode;
  MSR_BIOS_SIGN_ID_REGISTER   MsrSignId;
  UINT32                      MsrValue;

  MsrSignId.Uint64 = AsmReadMsr64 (MSR_BIOS_SIGN_ID);
  if (MsrSignId.Bits.PatchId != 0) {
    DEBUG ((DEBUG_INFO, "Valid FIT uCode Found: 0x%08X\n", MsrSignId.Bits.PatchId));
    return TRUE;  // FIT_SUCCESSFUL
  } else {
    DEBUG ((DEBUG_WARN, "FIT uCode Not Loaded\n"));
    ///
    /// Read the FIT BIOS Error
    ///
    MsrGetFitBiosError (&FitEntryType, &FitErrorCode);
    MsrValue = (UINT32) AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO);
    if (FitErrorCode != FIT_SUCCESSFUL) {
      switch (FitEntryType) {
        case FIT_HEADER_ENTRY:
          DEBUG ((DEBUG_INFO, "Fit Entry Type: Fit Header Entry\n"));
          switch (FitErrorCode) {
            case FIT_SIZE_CHECK:
              DEBUG ((DEBUG_INFO, "Fit Error Code: Size check failed\n"));
              break;
            case FIT_RESERVED_FIELD_CHECK:
              DEBUG ((DEBUG_INFO, "Fit Error Code: Reserved field check failed\n"));
              break;
            case FIT_VERSION_AND_TYPE_CHECK:
              DEBUG ((DEBUG_INFO, "Fit Error Code: Version and Type Check failed\n"));
              break;
            default:
              DEBUG ((DEBUG_INFO, "Fit Error Code: Unknown\n"));
              break;
          }
          break;
        case FIT_MICROCODE_UPDATE_ENTRY:
          DEBUG ((DEBUG_INFO, "Fit Entry Type: Microcode Update Entry\n"));
          switch (FitErrorCode) {
            case FIT_NO_MICROCODE_UPDATE:
              DEBUG ((DEBUG_INFO, "Fit Error Code: No microcode update found\n"));
              REPORT_STATUS_CODE_EX (
                (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
                (EFI_COMPUTING_UNIT_HOST_PROCESSOR|EFI_CU_HP_EC_NO_MICROCODE_UPDATE),
                0,
                &gCpuFspErrorTypeCallerId,
                NULL,
                NULL,
                0
                );
              break;
            case FIT_MICROCODE_UPDATE_FAIL:
              DEBUG ((DEBUG_INFO, "Fit Error Code: Microcode update failure\n"));
              REPORT_STATUS_CODE_EX (
                (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
                (EFI_COMPUTING_UNIT_HOST_PROCESSOR|EFI_CU_HP_EC_MICROCODE_UPDATE),
                0,
                &gCpuFspErrorTypeCallerId,
                NULL,
                NULL,
                0
                );
              break;
            default:
              DEBUG ((DEBUG_INFO, "Fit Error Code: Unknown\n"));
              break;
          }
          break;
        case FIT_STARTUP_ACM_ENTRY:
          if (((MsrValue & B_BOOT_GUARD_SACM_INFO_NEM_ENABLED) != 0)) {
            DEBUG ((DEBUG_INFO, "Fit Entry Type: Startup ACM Entry\n"));
            switch (FitErrorCode) {
              case FIT_STARTUP_ACM_NOT_SUPPORTED:
                DEBUG ((DEBUG_INFO, "Fit Error Code: Startup ACM not supported by CPU\n"));
                break;
              case FIT_FATAL_ERROR_DURING_ACM:
                DEBUG ((DEBUG_INFO, "Fit Error Code: Fatal error during ACM in previous boot\n"));
                break;
              case FIT_CPU_DOES_NOT_SUPPORT_LT:
                DEBUG((DEBUG_INFO, "Fit Error Code: CPU doesn't support LT\n"));
                break;
             case FIT_BIST_ERRORS:
                DEBUG ((DEBUG_INFO, "Fit Error Code: BIST errors\n"));
                break;
             case FIT_BEYOND_END_OF_FIT:
                DEBUG ((DEBUG_INFO, "Fit Error Code: Beyond end of FIT\n"));
                break;
             case FIT_NO_FIT_ACM_TYPE_MISMATCH:
                DEBUG ((DEBUG_INFO, "Fit Error Code: No FIT ACM type mismatch\n"));
                break;
             case FIT_ACM_BASE_SIZE_AND_CHECKS:
                DEBUG ((DEBUG_INFO, "Fit Error Code: ACM base size and checks\n"));
                break;
             default:
                DEBUG ((DEBUG_INFO, "Fit Error Code: Unknown\n"));
                break;
            }
          }
          break;
        case FIT_GENERAL_CHECKS:
          DEBUG ((DEBUG_INFO, "Fit Entry Type: General Checks\n"));
          switch (FitErrorCode) {
            case FIT_DISABLED_BY_CPU:
              DEBUG ((DEBUG_INFO, "Fit Error Code: FIT disabled by CPU\n"));
              break;
            case FIT_POINTER_ERROR:
              DEBUG ((DEBUG_INFO, "Fit Error Code: FIT pointer error (> FFFFFFB0; < FF000000)\n"));
              break;
            case FIT_FIRST_FIT_ENTRY_MISMATCH:
              DEBUG ((DEBUG_INFO, "Fit Error Code: First FIT entry doesn't match \"_FIT_\"\n"));
              break;
            default:
              DEBUG ((DEBUG_INFO, "Fit Error Code: Unknown\n"));
              break;
          }
          break;
        default:
          DEBUG ((DEBUG_INFO, "Fit Entry Type: Unknown\n"));
          DEBUG ((DEBUG_INFO, "Fit Error Code: Unknown\n"));
          break;
      }
    }
  }
  return FALSE;
}

/**
  Is BIOS GUARD enabled.

  @retval TRUE   BIOS GUARD is supported and enabled.
  @retval FALSE  BIOS GUARD is disabled.
**/
BOOLEAN
EFIAPI
IsBiosGuardEnabled (
  VOID
  )
{
#if FixedPcdGetBool(PcdBiosGuardEnable) == 1
  MSR_PLAT_FRMW_PROT_CTRL_REGISTER  ProtCtrlMsr;
  if (IsPfatEnabled ()) {
    ProtCtrlMsr.Uint64 = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL);
    if(ProtCtrlMsr.Bits.PfatEnable) {
      return TRUE;
    }
  }
#endif
  return FALSE;
}

/**
  Determine if CPU supports Intel Turbo Boost Max Technology.

  @retval TRUE   ITBM is supported and enabled.
  @retval FALSE  ITBM is disabled.
**/
BOOLEAN
EFIAPI
IsItbmSupported (
  VOID
  )
{
  CPUID_THERMAL_POWER_MANAGEMENT_EAX PowerEax;

  AsmCpuidEx (
    CPUID_THERMAL_POWER_MANAGEMENT,
    0,
    &PowerEax.Uint32,
    NULL,
    NULL,
    NULL
    );
  return (BOOLEAN) (PowerEax.Bits.TurboBoostMaxTechnology30 != 0);
}

/**
  Determine if CPU supports Resource Priority Feature (RP)..
  @retval TRUE   Rp is supported and enabled.
  @retval FALSE  Rp is disabled.
**/
BOOLEAN
EFIAPI
IsResourcePriorityFeatureSupported (
  VOID
  )
{
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_EBX  Ebx;
  UINT32                                       CpuidSubLeafEbx;

  AsmCpuidEx (
    CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS,
    0,
    NULL,
    &Ebx.Uint32,
    NULL,
    NULL
    );

  AsmCpuidEx (
    CPUID_INTEL_RDT_ALLOCATION,
    CPUID_INTEL_RDT_ALLOCATION_ENUMERATION_SUB_LEAF,
    NULL,
    &CpuidSubLeafEbx,
    NULL,
    NULL
    );
  //(EAX = 07H, ECX = 0) : EBX[15] = 1 (RDT support) and CPUID.(EAX = 10H, ECX = 0) : EBX[6] = 1 (RP support)
  if ((Ebx.Bits.RDT_A == 1) && ((CpuidSubLeafEbx & BIT6) != 0)) {
    return TRUE;
  }
  return FALSE;
}
/**
  Determine if CPU supports Programmable Core Ratio Limit for the Turbo mode.

  @retval TRUE   Core Ratio Limit for the Turbo mode is supported and enabled.
  @retval FALSE  Core Ratio Limit for the Turbo mode is disabled.
**/
BOOLEAN
EFIAPI
IsCoreRatioLimitSupported (
  VOID
  )
{
  MSR_PLATFORM_INFO_REGISTER PlatformInfoMsr;
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  return PlatformInfoMsr.Bits.PrgTurboRatioEn != 0;
}

/**
  Determine if CPU supports Programmable TDC/TDP Limit for the Turbo mode.

  @retval TRUE   TDC/TDP Limit is supported and enabled.
  @retval FALSE  TDC/TDP Limit is disabled.
**/
BOOLEAN
EFIAPI
IsXETdcTdpLimitSupported (
  VOID
  )
{
  MSR_PLATFORM_INFO_REGISTER PlatformInfoMsr;
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  return PlatformInfoMsr.Bits.PrgTdpLimEn != 0;
}

/**
  Determine if CPU supports Turbo mode.

  @retval TRUE   Turbo mode is supported and enabled.
  @retval FALSE  Turbo mode is disabled.
**/
BOOLEAN
EFIAPI
IsTurboModeSupported (
  VOID
  )
{
  CPUID_THERMAL_POWER_MANAGEMENT_EAX   PowerEax;
  MSR_MISC_ENABLES_REGISTER            MiscMsr;

  AsmCpuid (
    CPUID_THERMAL_POWER_MANAGEMENT,
    &PowerEax.Uint32,
    NULL,
    NULL,
    NULL
    );

  MiscMsr.Uint64 = AsmReadMsr64 (MSR_IA32_MISC_ENABLE);
  return ((BOOLEAN) (PowerEax.Bits.TurboBoostTechnology) || (MiscMsr.Bits.TurboModeDisable != 0));
}

/**
  Determine if CPU supports PPIN (Protected Processor Inventory Number)

  @retval TRUE   PPIN feature is available.
  @retval FALSE  PPIN feature is not available.
**/
BOOLEAN
EFIAPI
IsPpinFeatureAvailable (
  VOID
  )
{
  MSR_PLATFORM_INFO_REGISTER PlatformInfoMsr;
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  return PlatformInfoMsr.Bits.PpinCap != 0;
}

/**
  Determine if CPU supports Hardware P-States.

  @retval TRUE   Hardware P-States is supported and enabled.
  @retval FALSE  Hardware P-States is disabled.
**/
BOOLEAN
EFIAPI
IsHwpSupported (
  VOID
  )
{
  MSR_MISC_PWR_MGMT_REGISTER  PwrMgmtMsr;
  PwrMgmtMsr.Uint64 = AsmReadMsr64 (MSR_MISC_PWR_MGMT);
  return (BOOLEAN)PwrMgmtMsr.Bits.EnableHwp;
}

/**
  Determine if CPU supports Mwait.

  @retval TRUE   Mwait is supported and enabled.
  @retval FALSE  Mwait is disabled.
**/
BOOLEAN
EFIAPI
IsTimedMwaitSupported (
  VOID
  )
{
  MSR_PLATFORM_INFO_REGISTER PlatformInfoMsr;
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  return PlatformInfoMsr.Bits.TimedMwaitEnable != 0;
}

/**
  Determine if CPU supports LPM.

  @retval TRUE   LPM is supported and enabled.
  @retval FALSE  LPM is disabled.
**/
BOOLEAN
EFIAPI
IsLpmSupported (
  VOID
  )
{
  MSR_PLATFORM_INFO_REGISTER  PlatformInfoMsr;
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  return (BOOLEAN) PlatformInfoMsr.Bits.LpmSupport;
}

/**
  Determine if CPU supports ConfigTdp.

  @retval TRUE   ConfigTdp is supported and enabled.
  @retval FALSE  ConfigTdp is disabled.
**/
BOOLEAN
EFIAPI
IsConfigTdpSupported (
  VOID
  )
{
  UINT8 CtdpLevels;

  CtdpLevels = GetConfigTdpLevels ();

  if ((CtdpLevels == 1) || (CtdpLevels == 2)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Determine if CPU supports Turbo mode.

  @retval TRUE   Efficiency Turbo mode is supported and enabled.
  @retval FALSE  Efficiency Turbo mode is disabled.
**/
BOOLEAN
EFIAPI
IsEnergyEfficientTurboSupported (
  VOID
  )
{
  CPUID_THERMAL_POWER_MANAGEMENT_ECX PowerEcx;
  AsmCpuid (
    CPUID_THERMAL_POWER_MANAGEMENT,
    NULL,
    NULL,
    &PowerEcx.Uint32,
    NULL
    );

  return (BOOLEAN) (PowerEcx.Bits.PerformanceEnergyBias);
}

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
  )
{
  MSR_CONFIG_TDP_LEVEL1_REGISTER     TdpLevel1;
  MSR_CONFIG_TDP_LEVEL2_REGISTER     TdpLevel2;

  *CtdpLevel1Supported = FALSE;
  *CtdpLevel2Supported = FALSE;

  TdpLevel1.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL1);
  if (TdpLevel1.Bits.TdpRatio !=0){
    *CtdpLevel1Supported = TRUE;
  }

  TdpLevel2.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL2);
  if (TdpLevel2.Bits.TdpRatio !=0){
    *CtdpLevel2Supported = TRUE;
  }

  DEBUG ((DEBUG_INFO, "CTDP: CtdpLevel1Supported = %d, CtdpLevel2Supported = %d\n", *CtdpLevel1Supported, *CtdpLevel2Supported));

  return;
}

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
  )
{
  AsmCpuid (
    CPUID_MONITOR_MWAIT,
    NULL,
    NULL,
    MwaitEcx,
    MwaitEdx
    );
  return;
}

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
  )
{
  return GetCpuFusedCoreCountFru (DieId, NumberOfFusedPCore, NumberOfFusedECore, NumberOfFusedLpECore);
}

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
  )
{
  return GetHubDieIdFru (HubDieId);
}

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
  )
{
  return GetFusedCDieIdsFru(NumberOfFusedCDie, CDieIds);
}

/**
  Function to compare 2 MP_INFORMATION2_HOB_DATA pointer based on ProcessorIndex.
  @param[in] Buffer1            pointer to MP_INFORMATION2_HOB_DATA poiner to compare
  @param[in] Buffer2            pointer to second MP_INFORMATION2_HOB_DATA pointer to compare
  @retval 0                     Buffer1 equal to Buffer2
  @retval <0                    Buffer1 is less than Buffer2
  @retval >0                    Buffer1 is greater than Buffer2
**/
INTN
EFIAPI
ComparatorOfMpInformation2Hob (
  IN  CONST VOID  *Buffer1,
  IN  CONST VOID  *Buffer2
  )
{
  return (*(MP_INFORMATION2_HOB_DATA **)Buffer1)->ProcessorIndex - (*(MP_INFORMATION2_HOB_DATA **)Buffer2)->ProcessorIndex;
}

/**
  Get processor information and core type from gMpInformation2HobGuid Hob.
  This function retrieves processor information and core type for all processors
  in the system.

  @param[out] ProcessorInfo               A pointer to an array of EFI_PROCESSOR_INFORMATION structures.
                                          One entry for each processor.
  @param[in, out]     NumberOfProcessors  As input, it is the max list count of ProcessorInfo.
                                          As outout, it is the number of ProcessorInfo

  @retval     EFI_SUCCESS                 Get the ProcessorInfo list successfully.
  @retval     EFI_INVALID_PARAMETER       Input parameter is invalid
  @retval     EFI_BUFFER_TOO_SMALL        Input list count is too small
**/
EFI_STATUS
GetProcessorInfoFromHob (
     OUT  EFI_PROCESSOR_INFORMATION  *ProcessorInfo,
  IN OUT  UINTN                      *NumberOfProcessors
  )
{
  EFI_HOB_GUID_TYPE         *GuidHob;
  MP_INFORMATION2_HOB_DATA  *MpInformation2HobData;
  UINTN                     HobCount;
  UINTN                     HobIndex;
  UINTN                     ProcessorIndex;
  UINT64                    PrevProcessorIndex;
  MP_INFORMATION2_ENTRY     *MpInformation2Entry;
  MP_INFORMATION2_HOB_DATA  **MpInfo2Hobs;
  UINTN                     SortBuffer;
  UINTN                     CountedNumberOfProcessors;

  GuidHob                   = NULL;
  MpInformation2HobData     = NULL;
  HobIndex                  = 0;
  HobCount                  = 0;
  CountedNumberOfProcessors = 0;
  GuidHob                   = GetFirstGuidHob (&gMpInformation2HobGuid);

  if (GuidHob == NULL) {
    //
    // gMpInformation2HobGuid is not installed, maybe MpService is not installed yet.
    //
    return EFI_NOT_STARTED;
  }
  ASSERT (GuidHob != NULL);

  // Count the number of processors and HOBs
  for (HobCount = 0; GuidHob != NULL; HobCount++) {
    MpInformation2HobData      = GET_GUID_HOB_DATA (GuidHob);
    CountedNumberOfProcessors += MpInformation2HobData->NumberOfProcessors;
    //
    // This is the last MpInformationHob in the HOB list.
    //
    if (MpInformation2HobData->NumberOfProcessors == 0) {
      ASSERT (HobCount != 0);
      break;
    }

    GuidHob = GetNextGuidHob (&gMpInformation2HobGuid, GET_NEXT_HOB (GuidHob));
  }

  if (*NumberOfProcessors < CountedNumberOfProcessors) {
    *NumberOfProcessors = CountedNumberOfProcessors;
    return EFI_BUFFER_TOO_SMALL;
  }

  *NumberOfProcessors = CountedNumberOfProcessors;
  if (ProcessorInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  MpInfo2Hobs = AllocatePages (EFI_SIZE_TO_PAGES(sizeof(MP_INFORMATION2_HOB_DATA *) * HobCount));
  ASSERT (MpInfo2Hobs != NULL);
  if (MpInfo2Hobs == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  GuidHob = GetFirstGuidHob (&gMpInformation2HobGuid);
  while (HobIndex < HobCount) {
    MpInfo2Hobs[HobIndex++] = GET_GUID_HOB_DATA (GuidHob);
    GuidHob                 = GetNextGuidHob (&gMpInformation2HobGuid, GET_NEXT_HOB (GuidHob));
  }

  // Sort MpInfo2Hobs
  QuickSort (MpInfo2Hobs, HobCount, sizeof (MP_INFORMATION2_HOB_DATA *), (BASE_SORT_COMPARE)ComparatorOfMpInformation2Hob, &SortBuffer);

  PrevProcessorIndex = 0;
  for (HobIndex = 0; HobIndex < HobCount; HobIndex++) {
    //
    // Make sure no overlap and no gap in the CPU range covered by each HOB
    //
    ASSERT (MpInfo2Hobs[HobIndex]->ProcessorIndex == PrevProcessorIndex);

    //
    // Cache each EFI_PROCESSOR_INFORMATION in order.
    //
    for (ProcessorIndex = 0; ProcessorIndex < MpInfo2Hobs[HobIndex]->NumberOfProcessors; ProcessorIndex++) {
      MpInformation2Entry = GET_MP_INFORMATION_ENTRY (MpInfo2Hobs[HobIndex], ProcessorIndex);
      if (ProcessorInfo != NULL) {
        CopyMem (
          &ProcessorInfo[PrevProcessorIndex + ProcessorIndex],
          &MpInformation2Entry->ProcessorInfo,
          sizeof (EFI_PROCESSOR_INFORMATION)
          );
      }
    }

    PrevProcessorIndex += MpInfo2Hobs[HobIndex]->NumberOfProcessors;
  }

  FreePages (MpInfo2Hobs, sizeof(MP_INFORMATION2_HOB_DATA*) * HobCount);
  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                 Status;
  UINTN                      NumberOfProcessors;
  EFI_PROCESSOR_INFORMATION  *ProcessorInfo;
  UINT8                      FusedCDieIds[MAX_CDIE];
  UINT8                      TempEnalbedCDieIds[MAX_CDIE];
  UINTN                      NumberOfFusedCDie;
  UINTN                      Index1;
  UINTN                      Index2;
  UINTN                      EnabledCDieIndex;

  if (NumberOfEnabledCDie == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  NumberOfFusedCDie = MAX_CDIE;
  Status = GetFusedCDieIds (&NumberOfFusedCDie, FusedCDieIds);
  if (EFI_ERROR(Status)){
    return Status;
  }

  NumberOfProcessors = 0;
  Status = GetProcessorInfoFromHob (NULL, &NumberOfProcessors);
  ASSERT (Status == EFI_BUFFER_TOO_SMALL);
  ProcessorInfo = AllocatePages (EFI_SIZE_TO_PAGES(sizeof(EFI_PROCESSOR_INFORMATION) * NumberOfProcessors));
  Status = GetProcessorInfoFromHob (ProcessorInfo, &NumberOfProcessors);
  ASSERT_EFI_ERROR(Status);

  EnabledCDieIndex = 0;
  for (Index1 = 0; Index1 < NumberOfFusedCDie; Index1++) {
    for (Index2 = 0; Index2 < NumberOfProcessors; Index2++) {
      if (FusedCDieIds[Index1] == ProcessorInfo[Index2].ExtendedInformation.Location2.Die) {
        TempEnalbedCDieIds[EnabledCDieIndex] = FusedCDieIds[Index1];
        EnabledCDieIndex++;
        break;
      }
    }
  }

  if (EnabledCDieIndex > *NumberOfEnabledCDie) {
    FreePages(ProcessorInfo, EFI_SIZE_TO_PAGES(sizeof(EFI_PROCESSOR_INFORMATION) * NumberOfProcessors));
    return EFI_BUFFER_TOO_SMALL;
  }

  *NumberOfEnabledCDie = EnabledCDieIndex;
  if (EnabledCDieIndex == 0){
    return EFI_SUCCESS;
  }

  if (CDieIds == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CopyMem (CDieIds, TempEnalbedCDieIds, sizeof(UINT8) * EnabledCDieIndex);

  FreePages(ProcessorInfo, EFI_SIZE_TO_PAGES(sizeof(EFI_PROCESSOR_INFORMATION) * NumberOfProcessors));
  return EFI_SUCCESS;
}

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
  )
{
  if ((NumberOfLpAtomCores == NULL) && (NumberOfLpAtomThreads == NULL)) {
    return EFI_INVALID_PARAMETER;
  } else {
    GetLpECoreThreadCountInfoFru (ProcessorInfo, NumberOfProcessors, NumberOfLpAtomCores, NumberOfLpAtomThreads);
    return EFI_SUCCESS;
  }
}

/**
  Detect Supported CPU Features

  @param[InOut] CpuEcx   Pointer to CpuEcx (CPUID_VERSION_INFO_ECX).
**/
VOID
EFIAPI
GetSupportedCpuFeatures (
  IN UINT32     *RegEcx
  )
{
  AsmCpuid (
    CPUID_VERSION_INFO,
    NULL,
    NULL,
    RegEcx,
    NULL
    );
  return;
}

/**
  Detect if Processor Trace supported or not

  @retval TRUE   IntelProcessorTrace is supported and enabled.
  @retval FALSE  IntelProcessorTrace is disabled.
**/
BOOLEAN
EFIAPI
IsIntelProcessorTraceSupported (
  VOID
  )
{
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_EBX Ebx;

  AsmCpuidEx (
    CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS,
    0,
    NULL,
    &Ebx.Uint32,
    NULL,
    NULL
    );

  return (BOOLEAN) (Ebx.Bits.IntelProcessorTrace);
}

/**
  Returns Generation string of the respective CPU

  @retval      Character pointer of Generation string
**/
CONST CHAR8*
EFIAPI
GetGenerationString (
  VOID
  )
{
  UINT32               CpuFamilyId;
  CpuFamilyId = GetCpuFamilyModel ();

  return GetFruGenerationString (CpuFamilyId);
}

/**
  Returns Revision Table

  @retval      Character pointer of Revision Table String
**/
CONST CHAR8*
EFIAPI
GetRevisionTable (
  VOID
  )
{
  CPUID_VERSION_INFO_EAX  Eax;

  AsmCpuid (CPUID_VERSION_INFO, &Eax.Uint32, NULL, NULL, NULL);

  return GetRevisionTableString (Eax.Uint32 & CPUID_FULL_FAMILY_MODEL_STEPPING);
}

/**
  Returns Sku String

  @retval      Character pointer of Sku String
**/
CONST CHAR8*
EFIAPI
GetSkuString (
  VOID
  )
{
  CPU_SKU                            CpuSku;
  CONST CHAR8                        *SkuString = NULL;

  CpuSku = GetCpuSku ();
  switch (CpuSku) {
    case EnumCpuUlt:
      SkuString = "ULT";
      break;

    case EnumCpuUlx:
      SkuString = "ULX";
      break;

    case EnumCpuTrad:
      SkuString = "DT";
      break;

    case EnumCpuHalo:
      SkuString = "Halo";
      break;


    default:
      SkuString = NULL;
      break;
  }
  return SkuString;
}

/**
  Returns the value of Brand Identity MSR if it is supported.

  @param[out] *MsrBrandIdentityValue       Brand Identity MSR value

  @retval     EFI_SUCCESS       Brand Identity MSR is supported
  @retval     EFI_UNSUPPORTED   Brand Identity MSR is not supported
**/
EFI_STATUS
GetMsrBrandIdentityValue (
  OUT UINT32 *MsrBrandIdentityValue
  )
{
  MSR_CORE_CAPABILITIES_REGISTER               CoreCapabilitiesMsr;
  MSR_BRAND_QUALIFIED_FEATURES_STATE_REGISTER  BrandIdentityMsr;

  CoreCapabilitiesMsr.Uint64 = AsmReadMsr64 (MSR_CORE_CAPABILITIES);
  DEBUG ((DEBUG_INFO, "Core Capabilities MSR (0xCF) value = 0x%x\n", CoreCapabilitiesMsr.Uint32));

  if (CoreCapabilitiesMsr.Bits.BrandQualifiedFeaturesStatePresent) {
    BrandIdentityMsr.Uint64 = AsmReadMsr64 (MSR_BRAND_QUALIFIED_FEATURES_STATE);
    *MsrBrandIdentityValue = (UINT32) BrandIdentityMsr.Uint32;
    DEBUG ((DEBUG_INFO, "Brand Identity MSR (0xAE) value = 0x%x\n", *MsrBrandIdentityValue));
    return EFI_SUCCESS;
  } else {
    DEBUG ((DEBUG_INFO, "Brand Identity MSR is not supported.\n"));
    return EFI_UNSUPPORTED;
  }
}

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
  )
{
  MSR_PLATFORM_INFO_REGISTER PlatformInfoMsr;

  if (MaxBusRatio != NULL || MinBusRatio != NULL) {
    PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
    if (MaxBusRatio != NULL) {
      *MaxBusRatio = (UINT8) PlatformInfoMsr.Bits.MaxNonTurboLimRatio;
    }
    if (MinBusRatio != NULL) {
      *MinBusRatio = (UINT8) PlatformInfoMsr.Bits.MaxEfficiencyRatio;
    }
  }

  return;
}

/**
  This function return max Efficiency Ratio.

  @retval Max efficiency ratio
**/
UINT8
EFIAPI
GetMaxEfficiencyRatio (
  VOID
  )
{
  MSR_PLATFORM_INFO_REGISTER PlatformInfoMsr;
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  return (UINT8) PlatformInfoMsr.Bits.MaxEfficiencyRatio;
}

/**
  This function return max Non-Turbo Ratio.

  @retval Max Non-Turbo Ratio
**/
UINT8
EFIAPI
GetMaxNonTurboRatio (
  VOID
  )
{
  MSR_PLATFORM_INFO_REGISTER PlatformInfoMsr;
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  return (UINT8) PlatformInfoMsr.Bits.MaxNonTurboLimRatio;
}

/**
  This function return the supported Config TDP Levels.

  @retval number of config TDP levels
**/
UINT8
EFIAPI
GetConfigTdpLevels (
  VOID
  )
{
  MSR_PLATFORM_INFO_REGISTER PlatformInfoMsr;
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);

  ASSERT ((UINT8) PlatformInfoMsr.Bits.ConfigTdpLevels < TDP_MAX_LEVEL);

  if ((UINT8) PlatformInfoMsr.Bits.ConfigTdpLevels < TDP_MAX_LEVEL) {
    return (UINT8) PlatformInfoMsr.Bits.ConfigTdpLevels;
  } else {
    return 0;
  }
}

/**
  This function returns the supported Physical Address Size

  @retval supported Physical Address Size.
**/
UINT8
EFIAPI
GetMaxPhysicalAddressSize (
  VOID
  )
{
  return GetMaxPhysicalAddressSizeFru ();
}

/**
  Return if Edram Enable

  @retval TRUE             If Edram Enable
  @retval FALSE            If Edram Disable
**/
BOOLEAN
EFIAPI
IsEdramEnable (
  VOID
  )
{
  MSR_PLATFORM_INFO_REGISTER  PlatformInfoMsr;
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  return PlatformInfoMsr.Bits.EdramEnable != 0;
}

/**
  This function return if TME is supported

  @retval TRUE             If TME is supported
  @retval FALSE            If TME is not supported
**/
BOOLEAN
EFIAPI
IsTmeSupported (
  VOID
  )
{
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_ECX  CpuidStructuredExtendedFeatureEcx;

  ///
  /// Verify TME supported through CPUID.7.0.ECX.13
  ///
  AsmCpuidEx (CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS,
              CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_SUB_LEAF_INFO,
              NULL,
              NULL,
              &CpuidStructuredExtendedFeatureEcx.Uint32,
              NULL
              );

  if ((CpuidStructuredExtendedFeatureEcx.Uint32 & BIT13) != BIT13) {
    return FALSE;
  }

  return TRUE;
}

/**
  This function return if PConfig is supported

  @retval TRUE             If PConfig is supported
  @retval FALSE            If PConfig is not supported
**/
BOOLEAN
EFIAPI
IsPConfigSupported  (
  VOID
  )
{
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_EDX  CpuidStructuredExtendedFeatureEdx;

  ///
  /// Verify TME supported through CPUID.7.0.EDX.18
  ///
  AsmCpuidEx (CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS,
              CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_SUB_LEAF_INFO,
              NULL,
              NULL,
              NULL,
              &CpuidStructuredExtendedFeatureEdx.Uint32
              );

  if ((CpuidStructuredExtendedFeatureEdx.Uint32 & BIT18) != BIT18) {
    return FALSE;
  }

  return TRUE;

}

/**
  This function return if MKTME PConfig is supported

  @retval TRUE             If MKTME PConfig is supported
  @retval FALSE            If MKTME PConfig is not supported
**/
BOOLEAN
EFIAPI
IsMkTmePConfigSupported  (
  VOID
  )
{
  UINT32                                       CpuidEax;

  ///
  /// Verify TME supported through CPUID.1B.0.EAX
  ///
  AsmCpuidEx (CPUID_1B,
              CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_SUB_LEAF_INFO,
              &CpuidEax,
              NULL,
              NULL,
              NULL
              );

  if ((CpuidEax) != 1) {
    return FALSE;
  }

  return TRUE;
}

/**
  This function return if SEAM Mode is supported

  @retval TRUE             If SEAM Mode is supported
  @retval FALSE            If SEAM Mode is not supported
**/
BOOLEAN
EFIAPI
IsSeamModeSupported (
  VOID
  )
{
  MSR_MTRRCAP_REGISTER           MtrrCap;
  MtrrCap.Uint64 = AsmReadMsr64 (MSR_MTRRCAP);
  return MtrrCap.Bits.Seamrr != 0;
}

/**
  Return if the processor is a preproduction sample

  @retval TRUE          If the processor is a preproduction sample
  @retval FALSE         If the part is intended for production
**/
BOOLEAN
EFIAPI
IsSamplePart (
  VOID
  )
{
  MSR_PLATFORM_INFO_REGISTER    PlatformInfoMsr;
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  return PlatformInfoMsr.Bits.SamplePart != 0;
}

/**
  Write a value to Special Chipset Usage MSR.

  @param[in] Data32   The 32-bit value to write to the MSR.

  @retval VOID
**/
VOID
EFIAPI
WriteSpclChipsetUsageMsr (
  IN UINT32 Data32
  )
{
  AsmWriteMsr32 (MSR_SPCL_CHIPSET_USAGE, Data32);
}

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
  )
{
  UINT8 CoreType;

  GetCoreType (&CoreType);

  return CoreType;
}

/**
  Returns Socket Type String

  @retval      Character pointer of Socket Type String
**/
CHAR8*
EFIAPI
GetSocketTypeString (
  VOID
  )
{
  UINT32 CpuFamilyId;

  CpuFamilyId = GetCpuFamilyModel ();
  return GetSocketTypeFru (CpuFamilyId);
}