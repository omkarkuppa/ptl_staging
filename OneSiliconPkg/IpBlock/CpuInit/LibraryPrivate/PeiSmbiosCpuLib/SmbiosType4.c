/** @file
  SMBIOS Processor Info HOB data.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
  System Management BIOS (SMBIOS) Reference Specification v3.0.0
  dated 2015-Feb-12 (DSP0134)
  http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_3.0.0.pdf
**/

#include <Register/GenerationMsr.h>
#include <Library/CpuPlatformLib.h>
#include "SmbiosCpu.h"
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/PcdLib.h>
#include <OcLimits.h>
#if FixedPcdGetBool(PcdOverclockEnable) == 1
#include <Library/OcMailboxLib.h>
#endif
#include <IndustryStandard/SmBios.h>

///
/// Processor Information (Type 4)
///
GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_PROCESSOR_INFO SmbiosProcessorInfoData = {
  TO_BE_FILLED,             ///< TotalNumberOfSockets
  TO_BE_FILLED,             ///< CurrentSocketNumber
  CentralProcessor,         ///< ProcessorType
  TO_BE_FILLED,             ///< ProcessorFamily
  STRING_1,                 ///< ProcessorManufacturerStrIndex
  TO_BE_FILLED,             ///< ProcessorId
  STRING_2,                 ///< ProcessorVersionStrIndex
  TO_BE_FILLED,             ///< Voltage
  100,                      ///< ExternalClockInMHz
  TO_BE_FILLED,             ///< MaxSpeedInMHz
  TO_BE_FILLED,             ///< CurrentSpeedInMHz
  TO_BE_FILLED,             ///< Status
  TO_BE_FILLED,             ///< ProcessorUpgrade
  TO_BE_FILLED,             ///< CoreCount
  TO_BE_FILLED,             ///< EnabledCoreCount
  TO_BE_FILLED,             ///< ThreadCount
  TO_BE_FILLED,             ///< ProcessorCharacteristics
};
GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_PROCESSOR_INFO_STRING_ARRAY SmbiosProcessorInfoStrings = {
  INTEL_CORPORATION_STRING, ///< ProcessorManufacturer
  TO_BE_FILLED_STRING,      ///< ProcessorVersion
};

typedef struct _PROCESSOR_FAMILY_FIELD {
  CHAR8  ProcessorFamily[48];
  UINT16 ProcessorEnum;
} PROCESSOR_FAMILY_FIELD;

GLOBAL_REMOVE_IF_UNREFERENCED PROCESSOR_FAMILY_FIELD ProcessorFamilyField[] = {
  { "Intel(R) Core(TM) 3",        ProcessorFamilyIntelCore3 },
  { "Intel(R) Core(TM) 5",        ProcessorFamilyIntelCore5 },
  { "Intel(R) Core(TM) 7",        ProcessorFamilyIntelCore7 },
  { "Intel(R) Core(TM) 9",        ProcessorFamilyIntelCore9 },
  { "Intel(R) Core(TM) Ultra 3",  ProcessorFamilyIntelCoreUltra3 },
  { "Intel(R) Core(TM) Ultra 5",  ProcessorFamilyIntelCoreUltra5 },
  { "Intel(R) Core(TM) Ultra 7",  ProcessorFamilyIntelCoreUltra7 },
  { "Intel(R) Core(TM) Ultra 9",  ProcessorFamilyIntelCoreUltra9 },
};

/**
  This function returns the maximum number of cores supported in this physical processor package
  by leverging CPUID_CACHE_PARAMS with offset 26.

  @retval Maximum number of supported cores in the package.
**/
UINT8
GetMaxSupportedCoreCount (
  VOID
  )
{
  CPUID_CACHE_PARAMS_EAX  CpuidCacheParamsEax;

  AsmCpuidEx (
    CPUID_CACHE_PARAMS,
    0,
    &CpuidCacheParamsEax.Uint32,
    NULL,
    NULL,
    NULL
    );

  return (UINT8) (CpuidCacheParamsEax.Bits.MaximumAddressableIdsForProcessorCores) + 1;
}

/**
  This function returns the supported number of threads per core,
  and supported total threads in the physical processor package
  by leverging CPUID_EXTENDED_TOPOLOGY with index 1 & 2.

  @param[out] *ThreadsPerCore    - variable that will store Maximum enabled threads per core.
  @param[out] *Threads           - variable that will store supported total threads.
**/
VOID
GetSupportedThreadCount (
  OUT UINT8 *ThreadsPerCore,  OPTIONAL
  OUT UINT8 *Threads          OPTIONAL
  )
{
  CPUID_EXTENDED_TOPOLOGY_EBX  CpuidExtendedTopologyEbx;

  AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 0, NULL, &CpuidExtendedTopologyEbx.Uint32, NULL, NULL);

  if (ThreadsPerCore != NULL) {
    *ThreadsPerCore = (UINT8) CpuidExtendedTopologyEbx.Uint32;
  }

  if (Threads != NULL) {
    AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, NULL, &CpuidExtendedTopologyEbx.Uint32, NULL, NULL);
    *Threads = (UINT8) CpuidExtendedTopologyEbx.Uint32;
  }
}

///
/// Processor-specific routines
///
/**
  Returns the processor family and processor version of the processor installed in the system.

  @param[out] Processor Version - Returns the procesor version string installed in the system

  @retval                       - The Processor Family enum value
**/
UINT16
GetProcessorFamilyAndVersion (
  OUT CHAR8 **ProcessorVersion
  )
{
  UINTN              Index;
  UINT32             CpuExtendedSupportEax;
  UINT16             ProcessorFamily;
  UINTN              StrOffset;
  UINTN              ProcessorVersionStrLen;
  UINTN              ProcessorFamilyStrLen;
  CPUID_BRAND_STRING_DATA      BrandStringEax;
  CPUID_BRAND_STRING_DATA      BrandStringEbx;
  CPUID_BRAND_STRING_DATA      BrandStringEcx;
  CPUID_BRAND_STRING_DATA      BrandStringEdx;

  ProcessorFamily = ProcessorFamilyOther;

  ///
  /// Get Brand string
  ///
  AsmCpuid (
    CPUID_EXTENDED_FUNCTION,
    &CpuExtendedSupportEax,
    NULL,
    NULL,
    NULL
    );

  ///
  /// Check if Brand ID String is supported as per SDM Vol. 2A Chapter 3 Instruction Set Reference - CPUID
  ///
  if (CpuExtendedSupportEax >= 0x80000004) {
    AsmCpuid (CPUID_BRAND_STRING1, &BrandStringEax.Uint32, &BrandStringEbx.Uint32, &BrandStringEcx.Uint32, &BrandStringEdx.Uint32);
    CopyMem (*ProcessorVersion,       &BrandStringEax.BrandString, 4);
    CopyMem (*ProcessorVersion + 4,   &BrandStringEbx.BrandString, 4);
    CopyMem (*ProcessorVersion + 8,   &BrandStringEcx.BrandString, 4);
    CopyMem (*ProcessorVersion + 12,  &BrandStringEdx.BrandString, 4);

    AsmCpuid (CPUID_BRAND_STRING2, &BrandStringEax.Uint32, &BrandStringEbx.Uint32, &BrandStringEcx.Uint32, &BrandStringEdx.Uint32);
    CopyMem (*ProcessorVersion + 16,  &BrandStringEax.BrandString, 4);
    CopyMem (*ProcessorVersion + 20,  &BrandStringEbx.BrandString, 4);
    CopyMem (*ProcessorVersion + 24,  &BrandStringEcx.BrandString, 4);
    CopyMem (*ProcessorVersion + 28,  &BrandStringEdx.BrandString, 4);

    AsmCpuid (CPUID_BRAND_STRING3, &BrandStringEax.Uint32, &BrandStringEbx.Uint32, &BrandStringEcx.Uint32, &BrandStringEdx.Uint32);
    CopyMem (*ProcessorVersion + 32,  &BrandStringEax.BrandString, 4);
    CopyMem (*ProcessorVersion + 36,  &BrandStringEbx.BrandString, 4);
    CopyMem (*ProcessorVersion + 40,  &BrandStringEcx.BrandString, 4);
    CopyMem (*ProcessorVersion + 44,  &BrandStringEdx.BrandString, 4);

    *(*ProcessorVersion + MAXIMUM_CPU_BRAND_STRING_LENGTH) = 0;

    ///
    /// Remove preceeding spaces
    ///
    while (**ProcessorVersion == 0x20) {
      (*ProcessorVersion) ++;
    }
  } else {
    *ProcessorVersion = NULL;
    return ProcessorFamily;
  }

  ProcessorVersionStrLen = AsciiStrLen (*ProcessorVersion);
  for (Index = 0; Index < sizeof (ProcessorFamilyField) / sizeof (PROCESSOR_FAMILY_FIELD); Index++) {
    ProcessorFamilyStrLen = AsciiStrLen (ProcessorFamilyField[Index].ProcessorFamily);
    //
    // Detect if it matches the substring in ProcessorVersion.
    //
    for (StrOffset = 0; StrOffset < ProcessorVersionStrLen; StrOffset ++) {
      if ((ProcessorVersionStrLen - StrOffset) < ProcessorFamilyStrLen) {
        break;
      }
      if (AsciiStrnCmp (
            (*ProcessorVersion) + StrOffset,
            ProcessorFamilyField[Index].ProcessorFamily,
            (AsciiStrLen (ProcessorFamilyField[Index].ProcessorFamily))
            ) == 0) {
        ProcessorFamily = ProcessorFamilyField[Index].ProcessorEnum;
        break;
      }
    }
  }
  return ProcessorFamily;
}

/**
  Get processor status for socket base only

  @retval PROCESSOR_STATUS_DATA for socket base only
**/
UINT8
GetProcessorStatus (
  VOID
  )
{
  PROCESSOR_STATUS_DATA ProcessorStatus;

  ProcessorStatus.Bits.Reserved1 = 0;
  ProcessorStatus.Bits.Reserved2 = 0;
  ProcessorStatus.Bits.SocketPopulated = TRUE;

  ///
  /// For BSP, processor is always enabled.
  ///
  ProcessorStatus.Bits.CpuStatus = CpuStatusEnabled;

  return *(UINT8 *) &ProcessorStatus;
}

/**
  Returns the processor characteristics based on the processor's capabilities.

  @retval Processor Characteristics
**/
UINT16
GetProcessorCharacteristics (
  VOID
  )
{
  UINT16                     ProcessorCharacteristics;
  UINT32                     RegEax;
  CPUID_VERSION_INFO_ECX     VersionInfoEcx;
  CPUID_EXTENDED_CPU_SIG_EDX CpuSigEdx;
  UINT8                      ThreadsPerCore;

  ProcessorCharacteristics = 0;

  ///
  /// Check 64-bit and Execute Protection
  ///
  AsmCpuid (CPUID_EXTENDED_FUNCTION, &RegEax, NULL, NULL, NULL);
  if (RegEax >= CPUID_EXTENDED_CPU_SIG) {
    AsmCpuid (CPUID_EXTENDED_CPU_SIG, NULL, NULL, NULL, &CpuSigEdx.Uint32);
    //
    // Check Intel 64-bit capable (EDX[29])
    //
    if (CpuSigEdx.Bits.LM == 1) {
      ProcessorCharacteristics |= SMBIOS_TYPE4_64BIT_CAPABLE;
    }

    //
    // Execute Protection
    //
    if (CpuSigEdx.Bits.NX == 1) {
      ProcessorCharacteristics |= SMBIOS_TYPE4_EXECUTE_PROTECTION;
    }
  }

  ///
  /// Multi-Core and HT capabilities
  ///
  if (GetMaxSupportedCoreCount() > 1) {
    ProcessorCharacteristics |= SMBIOS_TYPE4_MULTI_CORE;
  }
  GetSupportedThreadCount (&ThreadsPerCore, NULL);
  if (ThreadsPerCore > 1) {
    ProcessorCharacteristics |= SMBIOS_TYPE4_HARDWARE_THREAD;
  }

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &VersionInfoEcx.Uint32, NULL);

  ///
  /// Enhanced Virtualization
  ///
  if (VersionInfoEcx.Bits.VMX == 1) {
    ProcessorCharacteristics |= SMBIOS_TYPE4_ENHANCED_VIRTUALIZATION;
  }

  ///
  /// Power/Performance Control
  ///
  if (VersionInfoEcx.Bits.EIST == 1) {
    ProcessorCharacteristics |= SMBIOS_TYPE4_POWER_PERF_CONTROL;
  }

  return ProcessorCharacteristics;
}

/**
  Add SMBIOS strings to the end of the HOB, then call the BuildGuidDataHob function.
  Add each non-null string, plus the terminating (double) null.

  @param[in]  FixedDataRegion       The data for the fixed portion of the HOB.
  @param[in]  FixedRegionSize       The size of the fixed portion of the HOB.
  @param[in]  TableStrings          Set of string pointers to append onto the full record.
                                    If TableStrings is null, no strings are appended. Null strings
                                    are skipped.
  @param[in]  NumberOfStrings       Number of TableStrings to append, null strings are skipped.
  @param[in]  HobGuid               The GUID to pass to the BuildGuidDataHob function.
**/
VOID
AddSmbiosStringsAndBuildGuidDataHob (
  IN  VOID          *FixedDataRegion,
  IN  UINT8         FixedRegionSize,
  IN  CHAR8         **TableStrings,
  IN  UINT8         NumberOfStrings,
  IN  EFI_GUID      *HobGuid
  )
{
  CHAR8                   *StringPtr;
  UINTN                   Size;
  UINTN                   HobSize;
  UINTN                   CurrentStringSize;
  UINTN                   Index;
  VOID                    *HobData;

  Size = FixedRegionSize;

  ///
  /// Add the size of each non-null string
  ///
  if (TableStrings != NULL) {
    for (Index = 0; Index < NumberOfStrings; Index++) {
      if ((TableStrings[Index] != NULL) && (*TableStrings[Index] != '\0')) {
        Size += AsciiStrSize (TableStrings[Index]);
      }
    }
  }

  ///
  /// Add the size of the terminating double null
  /// If there were any strings added, just add the second null
  ///
  if (Size == FixedRegionSize) {
    Size += 2;
  } else {
    Size += 1;
  }
  HobSize = Size;

  ///
  /// Initialize the full record
  ///
  HobData = AllocateZeroPool (HobSize);
  if (HobData == NULL) {
    return;
  }
  CopyMem (HobData, FixedDataRegion, FixedRegionSize);

  ///
  /// Copy the strings to the end of the record
  ///
  StringPtr = ((CHAR8 *) HobData) + FixedRegionSize;
  if (TableStrings != NULL) {
    for (Index = 0; Index < NumberOfStrings; Index++) {
      if ((TableStrings[Index] != NULL) && (*TableStrings[Index] != '\0')) {
        AsciiStrnCpyS (StringPtr, MAXIMUM_SIZE_OF_CPU_SMBIOS_STRING, TableStrings[Index], Size - FixedRegionSize - 1);
        CurrentStringSize = AsciiStrSize (TableStrings[Index]);
        StringPtr += CurrentStringSize;
        Size -= CurrentStringSize;
      }
    }
  }

  BuildGuidDataHob (
    HobGuid,
    HobData,
    HobSize
    );

  return;
}

/**
  Get the SMBIOS TYPE 4 max speed field value

  @retval Max Speed support by system or CPU.
**/
STATIC
UINT16
GetSmbiosType4MaxSpeed (
  VOID
  )
{
  UINT16                             MaxSpeed;
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  EFI_STATUS                         Status;
  UINT32                             OcMaxSpeed;
  OC_MAILBOX_INTERFACE               OcMbxCmd;
  UINT32                             MailboxStatus;
#endif

  MaxSpeed = 0;

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  //
  // Fill up Max Speed from OCMB Command 0x01 - Get Overclocking Capabilities
  //
  OcMbxCmd.InterfaceData = 0;
  OcMbxCmd.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_OC_CAPABILITIES; // Get Overclocking Capabilities
  Status = OcMailboxRead (OcMbxCmd.InterfaceData, &OcMaxSpeed, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
    MaxSpeed = (UINT16) ((OcMaxSpeed & 0xFF) * 100);
    DEBUG ((DEBUG_INFO, "OC MaxSpeed %d MHz\n", MaxSpeed));
  } else {
#endif
      if (IsTurboModeSupported ()) {
        MaxSpeed = (UINT16) (GetMaxTurboRatio () * 100);
        DEBUG ((DEBUG_INFO, "Turbo MaxSpeed %d MHz\n", MaxSpeed));
      } else {
        MaxSpeed = (UINT16) (GetMaxNonTurboRatio () * 100);
        DEBUG ((DEBUG_INFO, "HFM MaxSpeed %d MHz\n", MaxSpeed));
      }
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  }
#endif
  return MaxSpeed;
}


/**
  This function produces an SMBIOS Processor Info HOB.

**/
VOID
InitializeSmbiosProcessorInfoHob (
  VOID
  )
{
  EFI_STATUS                        Status;
  CHAR8                             BrandIdString[MAXIMUM_CPU_BRAND_STRING_LENGTH + 1];
  UINT64                            MchBar;
  UINT32                            BclkFrequency;
  PROCESSOR_ID_DATA                 ProcessorId;
  UINT8                             NumberOfFusedCores;
  UINT8                             NumberOfFusedAtomCores;
  UINT8                             NumberOfFusedLpAtomCores;
  UINT8                             NumberOfFusedCoresPerDie;
  UINT8                             NumberOfFusedAtomCoresPerDie;
  UINT8                             NumberOfFusedLpAtomCoresPerDie;
  MSR_PERF_STATUS_REGISTER          MsrPerfStatus;
  UINT8                             DieIds[MAX_CDIE + 1];
  UINTN                             NumberOfFusedDie;
  UINTN                             Index;

  DEBUG ((DEBUG_INFO, "InitializeSmbiosProcessorInfoHob() - Start\n"));

  SmbiosProcessorInfoData.TotalNumberOfSockets = 1;
  SmbiosProcessorInfoData.CurrentSocketNumber = 0;

  SmbiosProcessorInfoStrings.ProcessorVersion = (CHAR8 *) &BrandIdString;

  SmbiosProcessorInfoData.ProcessorFamily = GetProcessorFamilyAndVersion (&SmbiosProcessorInfoStrings.ProcessorVersion);
  if (SmbiosProcessorInfoStrings.ProcessorVersion == NULL) {
    AsciiStrnCpyS ((CHAR8 *) &BrandIdString, MAXIMUM_CPU_BRAND_STRING_LENGTH + 1, (CHAR8 *) &BRAND_STRING_UNSUPPORTED, (sizeof (BrandIdString) / sizeof (CHAR8)) - 1);
    SmbiosProcessorInfoStrings.ProcessorVersion = (CHAR8 *) &BrandIdString;
  }

  AsmCpuid (CPUID_VERSION_INFO, (UINT32 *) &ProcessorId.Signature, NULL, NULL, (UINT32 *) &ProcessorId.FeatureFlags);
  SmbiosProcessorInfoData.ProcessorId = *(UINT64 *) &ProcessorId;

  SmbiosProcessorInfoData.Voltage = 0;

  //
  // BCLK value is in kHz units. Convert to MHz for SMBIOS table usage.
  //
  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);
  BclkFrequency = (UINT32) (MmioRead64(MchBar + PCU_CR_BCLK_FREQ_MCHBAR));
  DEBUG ((DEBUG_INFO, "(SMBIOS) BCLK frequency = %d\n", BclkFrequency));

  //
  // Command returns BCLK in KHz, divide by 1000 for MHz
  //
  BclkFrequency = BclkFrequency / 1000;

  MsrPerfStatus.Uint64 = AsmReadMsr64 (MSR_PERF_STATUS);
  SmbiosProcessorInfoData.CurrentSpeedInMHz = (UINT16) (BclkFrequency * MsrPerfStatus.Bits.CoreRatio100mhz);

  SmbiosProcessorInfoData.Status = GetProcessorStatus ();

  SmbiosProcessorInfoData.MaxSpeedInMHz = GetSmbiosType4MaxSpeed ();
  //
  //  Programs Processor Upgrade for type 4 SMBIOS Processor Info HOB in Fru.
  //
  SmbiosProcessorInfoData.ProcessorUpgrade = ProcessorUpgradeInvalid;

  //
  // Get all CDIE IDs, and fill them them into the DieIds list
  //
  NumberOfFusedDie = MAX_CDIE;
  Status = GetFusedCDieIds (&NumberOfFusedDie, DieIds);
  ASSERT (Status != EFI_BUFFER_TOO_SMALL);
  ASSERT_EFI_ERROR (Status);

  //
  // Get HubDieId, and if there is HubDie, add it in the DieIds list
  //
  ASSERT (NumberOfFusedDie <= MAX_CDIE);
  Status = GetHubDieId (&DieIds[NumberOfFusedDie]);
  if (!EFI_ERROR(Status)) {
    NumberOfFusedDie++;
  }

  NumberOfFusedCores       = 0;
  NumberOfFusedAtomCores   = 0;
  NumberOfFusedLpAtomCores = 0;

  for (Index = 0; Index < NumberOfFusedDie; Index++){
    Status = GetCpuFusedCoreCount(DieIds[Index], &NumberOfFusedCoresPerDie, &NumberOfFusedAtomCoresPerDie, &NumberOfFusedLpAtomCoresPerDie);
    //ASSERT_EFI_ERROR (Status);
    NumberOfFusedCores += NumberOfFusedCoresPerDie;
    NumberOfFusedAtomCores += NumberOfFusedAtomCoresPerDie;
    NumberOfFusedLpAtomCores += NumberOfFusedLpAtomCoresPerDie;
  }

  SmbiosProcessorInfoData.CoreCount = NumberOfFusedCores + NumberOfFusedAtomCores + NumberOfFusedLpAtomCores;
  SmbiosProcessorInfoData.EnabledCoreCount = GetEnabledCoreCount ();
  SmbiosProcessorInfoData.ThreadCount = GetEnabledThreadCount ();

  SmbiosProcessorInfoData.ProcessorCharacteristics = GetProcessorCharacteristics ();

  AddSmbiosStringsAndBuildGuidDataHob (
    (VOID *) &SmbiosProcessorInfoData,
    sizeof (SMBIOS_PROCESSOR_INFO),
    (CHAR8 **) &SmbiosProcessorInfoStrings,
    SMBIOS_PROCESSOR_INFO_NUMBER_OF_STRINGS,
    &gSmbiosProcessorInfoHobGuid
    );

  //
  // ProcessorVersion pointer is no longer valid.
  //
  SmbiosProcessorInfoStrings.ProcessorVersion = NULL;

  DEBUG ((DEBUG_INFO, "InitializeSmbiosProcessorInfoHob() - End\n"));

  return;
}
