/** @file
  This file is the implementation for PeiCpuInitFru library.

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

@par Specification
**/

#include <Library/PeiCpuInitFruLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/CpuPlatformLib.h>
#include <Register/GenerationMsr.h>
#include <Register/Cpuid.h>
#include <Library/CpuMailboxLib.h>
#include <Library/MsrFruLib.h>
#include <Library/PeiNguInitLib.h>
#include <Library/PeiImrInitLib.h>
#include <Register/CpuGenRegs.h>
#include <Register/B2pMailbox.h>

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
  )
{
  EFI_STATUS                      Status;
  CPU_STRAP_SET                   *CpuStrapSet;
  MSR_CORE_THREAD_COUNT_REGISTER  MsrCoreThreadCount;
  UINT32                          NumberOfActiveCores;
  UINT32                          NumberOfActiveThreads;
  UINT8                           AllCoreCount;
  UINT8                           AllSmallCoreCount;
  UINT8                           AllLpAtomCores;

  AllCoreCount      = 0;
  AllSmallCoreCount = 0;
  AllLpAtomCores    = 0;
  CpuStrapSet       = (CPU_STRAP_SET *) CpuStrapSetData;

  /* CORE_THREAD_COUNT - msr 0x35
     Symbol      Name        MSB:LSB  Description
     CoreCount   CoreCount   31:16    The Core Count reflects the enabled cores based
                                      on the above thread count and the value of threads_
                                      available (to determine if HT is on) at reset time.

     ThreadCount ThreadCount 15:0     The Thread Count reflects the enabled threads based
                                      on the factory-configured thread count and the value
                                      of the PCH Soft Reset Data register for Client processors
                                      at reset time.

     Read MSR for Active Core and Thread Count.
  */
  MsrCoreThreadCount.Uint64 = AsmReadMsr64 (MSR_CORE_THREAD_COUNT);
  NumberOfActiveCores       = (UINT32)MsrCoreThreadCount.Bits.Corecount;
  NumberOfActiveThreads     = (UINT32)MsrCoreThreadCount.Bits.Threadcount;

  DEBUG ((DEBUG_INFO, "Number of Active Cores / Threads = 0x%x / 0x%x\n", NumberOfActiveCores, NumberOfActiveThreads));

  //
  // Mapping {ALL} into {0} before CPU strap setting since the pCode default value is {0}.
  //
  Status = GetCpuFusedCoreCount (0, &AllCoreCount, &AllSmallCoreCount, &AllLpAtomCores);
  if (EFI_ERROR (Status)) {
    return ;
  }

  if (ActiveLpAtomCoreCount == 0xFF) {
    ActiveLpAtomCoreCount = AllLpAtomCores;
  }

  if (ActiveCoreCount == 0xFF) {
    ActiveCoreCount = AllCoreCount;
  }

  if (ActiveSmallCoreCount == 0xFF) {
    ActiveSmallCoreCount = AllSmallCoreCount;
  }

  if (ActiveCoreCount == AllCoreCount && ActiveSmallCoreCount == AllSmallCoreCount && ActiveLpAtomCoreCount == AllLpAtomCores) {
    ActiveCoreCount       = 0;
    ActiveSmallCoreCount  = 0;
    ActiveLpAtomCoreCount = 0;
  }


  ///
  /// Check if the configuration of "Active Big Cores" matches the BIOS Setup option.
  ///
  if (CpuStrapSet->NumberOfActiveBigCores != ActiveCoreCount) {
    DEBUG ((
      DEBUG_INFO,
      "Number Of active big cores doesn't match the value from Setup = %x / %x\n",
      CpuStrapSet->NumberOfActiveBigCores,
      ActiveCoreCount
      ));
    CpuStrapSet->NumberOfActiveBigCores = ActiveCoreCount;
    *ColdResetFlag = TRUE;
  }

  ///
  /// Check if the configuration of "Active Ecores on ring" matches the BIOS Setup option.
  ///
  if (CpuStrapSet->NumberOfNonLpAtomCores != ActiveSmallCoreCount) {
    DEBUG ((
      DEBUG_INFO,
      "Number Of active atom cores doesn't match the value from Setup = %x / %x\n",
      CpuStrapSet->NumberOfNonLpAtomCores,
      ActiveSmallCoreCount
      ));
    CpuStrapSet->NumberOfNonLpAtomCores = ActiveSmallCoreCount;
    *ColdResetFlag = TRUE;
  }

  ///
  /// Check if the configuration of "Active LP ECores off ring" matches the BIOS Setup option.
  ///
  if (CpuStrapSet->NumberOfLpAtomCores != ActiveLpAtomCoreCount) {
    DEBUG ((
      DEBUG_INFO,
      "Number Of active LP atom cores doesn't match the value from Setup = %x / %x\n",
      CpuStrapSet->NumberOfLpAtomCores,
      ActiveLpAtomCoreCount
      ));
    CpuStrapSet->NumberOfLpAtomCores = ActiveLpAtomCoreCount;
    *ColdResetFlag = TRUE;
  }
}

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
  )
{
  CPU_STRAP_SET     *CpuStrapSet;

  CpuStrapSet = (CPU_STRAP_SET *) CpuStrapSetData;

  if (CpuStrapSet->Bist == BistOnReset) {
    return ;
  } else {
    CpuStrapSet->Bist = BistOnReset;
    DEBUG ((DEBUG_INFO, "BIST configuration doesn't match the setup value\n"));
    *WarmResetFlag = TRUE;
    return ;
  }
}

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
  )
{
  CPU_STRAP_SET             *CpuStrapSet;
  MSR_FLEX_RATIO_REGISTER   MsrFlexRatio;

  CpuStrapSet = (CPU_STRAP_SET *) CpuStrapSetData;

  DEBUG ((DEBUG_INFO, "Ratio from Policy is 0x%X\n", CpuRatio));

  ///
  /// Read and save current Flex Ratio data; disregard enable bit (MSR 194h [15:8])
  ///
  MsrFlexRatio.Uint64 = AsmReadMsr64 (MSR_FLEX_RATIO);
  DEBUG ((DEBUG_INFO, "Current Flex Ratio from MSR is 0x%X\n", MsrFlexRatio.Bits.FlexRatio));
  DEBUG ((DEBUG_INFO, "Current Flex Ratio from CPU Strap: 0x%X\n", CpuStrapSet->FlexRatio));
  ///
  /// Check and set Flex Ratio to requested ratio if possible
  ///
  if (CpuRatio == CpuStrapSet->FlexRatio) {
    ///
    /// Do nothing, ratio is already set to requested value and enabled
    ///
    DEBUG ((DEBUG_INFO, "No need to set Flex Ratio.\n"));
  } else {
    ///
    /// Set Flex Ratio to user selected value
    ///
    MsrFlexRatio.Bits.FlexRatio = CpuRatio;
    MsrFlexRatio.Bits.Enable = 1;
    AsmWriteMsr64 (MSR_FLEX_RATIO, MsrFlexRatio.Uint64);

    ///
    /// Set Soft Reset Data for Flex Ratio
    ///
    CpuStrapSet->FlexRatio = CpuRatio;

    ///
    /// Set RESET flag
    ///
    DEBUG ((DEBUG_INFO, "Setting Flex Ratio to 0x%X\n", CpuStrapSet->FlexRatio));
    *WarmResetFlag = TRUE;
  }
}

/**
  Compare the boot max frequency setup option against the boot ratio strap setting
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
  )
{
  CPU_STRAP_SET       *CpuStrapSet;

  CpuStrapSet = (CPU_STRAP_SET *) CpuStrapSetData;

  ///
  /// Check if the configuration of FastWakeup from Bit12 of strap setting is not aligned with the BootFrequency setup option.
  ///
  if (CpuStrapSet->FastWakeup != BootMaxFrequency) {
    DEBUG ((
      DEBUG_INFO,
      "Boot Ratio strap setting of %x does not match the BootMaxFrequency policy %x\n",
      CpuStrapSet->FastWakeup,
      BootMaxFrequency
      ));
    CpuStrapSet->FastWakeup = BootMaxFrequency;
    *WarmResetFlag = TRUE;
  }
}

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
  )
{
  return;
}

/**
  Return Total Memory Encryption (TME) default policy setting

  @retval Total Memory Encryption (TME) default policy setting
**/
UINT32
EFIAPI
PeiCpuTmeDefaultSetting (
  VOID
  )
{
  return 0;
}

/**
  Disable or enable CPU Crashlog dump feature by Mailbox.

  @param[in] BOOLEAN      Policy for CrashLog
**/
VOID
EFIAPI
PeiCpuCrashLogMailboxEnable (
  IN  BOOLEAN  CrashLogEnable
  )
{

}

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
  )
{
  RETURN_STATUS Status;
  Status = SetPrmrrMsr (PrmrrBase, PrmrrSize);
  if (!RETURN_ERROR (Status)) {
    Status = SetImr (IMR_PRMRR, PrmrrBase, PrmrrSize);
  }
  return Status;
}

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
  )
{
  RETURN_STATUS Status;
  DEBUG ((DEBUG_INFO, "[TDX] Inside %a \n", __FUNCTION__));
  Status = SetSeamrrMsr (SeamrrBase, SeamrrSize);
  return Status;
}


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
  )
{
  RETURN_STATUS Status;
  MSR_BIOS_SE_SVN_REGISTER BiosSeSvnMsr;

  BiosSeSvnMsr.Uint64 = 0;
  BiosSeSvnMsr.Bits.SeamldrSeSvn = SeamldrSeSvn;

  Status = SetBiosSeSvnMsr (BiosSeSvnMsr.Uint64);
  return Status;
}

/**
  Configure DFD.

  @param[in] DfdEnable. TRUE, DFD enable; FALSE, DFD disable

  @retval EFI_UNSUPPORTED           Command unsupported
  @retval EFI_SUCCESS               Command succeeded
**/
EFI_STATUS
EFIAPI
PeiCpuDfdEnable (
  IN BOOLEAN               DfdEnable
  )
{
  UINT32                   MailboxData;
  UINT32                   MailboxStatus;
  EFI_STATUS               Status;
  PCODE_MAILBOX_INTERFACE  MailboxCommand;

  MailboxCommand.InterfaceData = 0;
  MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_BIOS_WRITE_DFD_POLICY;
  if (DfdEnable) {
    MailboxData = V_MAILBOX_DFD_EN;
  } else {
    MailboxData = V_MAILBOX_DFD_DIS;
  }
  Status = MailboxWrite (MailboxCommand.InterfaceData, MailboxData, &MailboxStatus);

  if (Status != EFI_SUCCESS || MailboxStatus != PCODE_MAILBOX_CC_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Mailbox write command 0x%x failed. MailboxStatus = %x, Mailbox command return status %x \n", MailboxCommand.Fields.Command, MailboxStatus, Status));
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**
Returns MCU's 2nd Patch load Status.
@retval TRUE 2nd Patch Load was success
@retval FALSE 2nd Patch Load was fail
**/
BOOLEAN
EFIAPI
MsrReadMcu2ndPatchStatusFru (
  VOID
  )
{
  MSR_BIOS_DEBUG_REGISTER    BiosDebugMsr;
  BiosDebugMsr.Uint64 = AsmReadMsr64 (MSR_BIOS_DEBUG);
  return BiosDebugMsr.Bits.PartialSuccess == 0;
}
