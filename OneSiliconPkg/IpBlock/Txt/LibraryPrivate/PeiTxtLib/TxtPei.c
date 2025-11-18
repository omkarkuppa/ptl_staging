/** @file
  This is the main PEIM file for TXT. It represents an abstract outline of the
  steps required during PEI for enabling TXT. Each individual step is further
  abstracted behind a function call interface. This is intended to minimize
  the need to modify this file when porting TXT to future platforms.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include <Ppi/EndOfPeiPhase.h>
#include <DprInfoHob.h>
#include <Library/TxtPeiLib.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>
#include <TxtInfoHob.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Register/GenerationMsr.h>
#include <Library/MpInitLib.h>
#include <Register/PchRegs.h>
#include <Register/PmcRegs.h>
#include <Library/SpiAccessLib.h>
#include <Library/PmcLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <TxtConfig.h>
#include <Ppi/SiPolicy.h>

EFI_STATUS
EFIAPI
ConfigureTxtPart1 (
  IN  EFI_PEI_SERVICES      **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );
STATIC EFI_PEI_NOTIFY_DESCRIPTOR mNotifyDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  ConfigureTxtPart1
};

EFI_STATUS
EFIAPI
ConfigureTxtPart2 (
  IN  EFI_PEI_SERVICES      **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

//
// Below event must be executed after S3 Boot Script execution to ensure
// silicon registers are restored. However, the event notification between
// FSP vs. non-FSP build defer. Therefore, align the below event appropriately.
//
STATIC EFI_PEI_NOTIFY_DESCRIPTOR mNotifyDesc2 = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEventReadyToBootGuid,    // in FSP build ReadyToBoot event occurs after S3 Boot Script
  ConfigureTxtPart2
};

extern EFI_GUID gDprInfoHobGuid;

/**
  TPM detected callback module, must have TPM hardware initialized of detected none

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The memory discovered PPI.  Not used.

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
TxtInit (
  VOID
  )
{
  EFI_STATUS            Status;
  TXT_INFO_HOB          *TxtInfoHob;
  BOOLEAN               TxtEnvInitFail;
  UINT32                Data32;
  EFI_BOOT_MODE         BootMode;
  SI_PREMEM_POLICY_PPI  *SiPreMemPolicy;
  TXT_PREMEM_CONFIG     *TxtPreMemConfig;

  Data32         = 0;
  TxtEnvInitFail = FALSE;
  SiPreMemPolicy = NULL;
  TxtPreMemConfig = NULL;

  ///
  /// Locate TXT policy ppi
  ///
  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicy);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TXTPEI: Failed to locate SiPreMemPolicy PPI\n"));
    return Status;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gTxtPreMemConfigGuid, (VOID *) &TxtPreMemConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TXTPEI: Failed to get TXT PreMem config block\n"));
    return Status;
  }

  ///
  /// Initialize the TXT PEI Lib functions
  ///
  Status = InitializeTxtPeiLib (
             &TxtInfoHob
             );

  if (EFI_ERROR (Status)) {
    TxtEnvInitFail = TRUE;
    DEBUG ((DEBUG_WARN, "PEI Lib initialization failure\n"));
  }

  /**
  Simics Environment reports RTC Battery failure on first boot, the check is added
  to make sure that the clear secrets leaf isn't executed when running in the Simics
  Environment
  **/
  if (!IsSimicsEnvironment ()) {
    if (!PmcIsRtcBatteryGood ()) {
      if (IsTxtSecretsSet () && !IsTxtEnabledCmos(TxtPreMemConfig)) {
        if (TxtInfoHob->Data.TxtMode == 1) {
          DEBUG ((DEBUG_INFO, "TXTPEI::RTC Failure Detected, Restoring FIT A Content\n"));
          UpdateTxtStatusCmos(TRUE, TxtPreMemConfig);
        }
        DEBUG ((DEBUG_INFO, "RTC Failure Detected & Secrets is set, Executing ClearSecretsBit\n"));
        ClearSecretsBit ();
      }
    }
  }
  /**
  Determine TPM presence. If TPM is not present - disabling TXT through TxtInfoHob by setting TxtMode=0
  In case TXT had been enabled but TPM was removed suddenly. Although TPM presence is precondition of this module
  since all commands executed by BIOS ACM don't depend on TPM state.
  TPM_NV_read will be successfully executed even if TPM is disabled
  and/or deactivated because all indices defined for BIOS ACM
  usage don't require authorization. TPM_ResetEstablishmentBit
  doesn't depend on TPM state at all and can
  be executed with disabled/deactivated TPM always.
  Case when TPM is completely not functional is not considered.
  **/
  Status = IsTpmPresent (TxtInfoHob);
  if (EFI_ERROR (Status)) {
    ///
    /// If TPM is not present / not supported, set TxtMode=0 in case TPM was removed after TXT enabled
    ///
    if (Status == EFI_UNSUPPORTED) {
      DEBUG ((DEBUG_WARN, "TPM Support is Disabled in BIOS! Disabling TXT! TxtMode=%x\n", TxtInfoHob->Data.TxtMode));
    } else {
      DEBUG ((DEBUG_WARN, "TPM is not present! Disabling TXT! TxtMode=%x\n", TxtInfoHob->Data.TxtMode));
    }

    TxtEnvInitFail = TRUE;
  }
  ///
  /// Detect TXT capable Processor & PCH
  ///
  if (!IsTxtChipset ()) {
    DEBUG ((DEBUG_WARN, "Platform or PCH is not TXT capable\n"));
    goto Done;
  } else if (!IsTxtProcessor ()) {
    DEBUG ((DEBUG_WARN, "Processor is not TXT capable\n"));
    goto Done;
  } else {
    DEBUG ((DEBUG_INFO, "Processor, PCH & Platform is TXT capable\n"));

    ///
    /// If TXT Lib or TPM  initialization unsuccessful, disable TXT
    ///
    if (TxtEnvInitFail) {
      TxtUnlockMemory (TxtInfoHob);
      TxtInfoHob->Data.TxtMode = 0;
      goto Done;
    }
  }

  // Handle BIOS knob Memory Powerdown Request
  if (TxtInfoHob->Data.TxtPowerdownRequest == 1) {
    SetPowerDownRequest (2);
  }

  Data32 = MmioRead32 (TXT_PUBLIC_BASE + TXT_CRASHCODE_REG_OFF);
  DEBUG ((DEBUG_INFO, "Crash Code Register=%x\n", Data32));

  ///
  /// Memory is supposed to lock if system is TXT capable.
  /// Check if we need to run SCLEAN. TXT BIOS spec Section 6.2.5
  ///
  if (TxtIsEstablishmentBitAsserted (TxtInfoHob) && IsTxtWakeError ()) {

    DEBUG ((DEBUG_INFO, "EstablishmentBit is set\n"));
    ///
    /// If TXTRESET is set , we must clean TXTRESET bit otherwise SCLEAN
    /// will fail
    ///
    if (IsTxtResetSet ()) {
      IssueGlobalReset ();
    }
    ///
    /// Setup and Launch SCLEAN
    ///
    DEBUG ((DEBUG_INFO, "Entering SCLEAN\n"));

  } else {
    ///
    /// Unlock memory, and then continue running
    ///
    DEBUG ((DEBUG_INFO, "EstablishmentBit not asserted - Unlock Memory\n"));
    AsmWriteMsr64 (MSR_LT_UNLOCK_MEMORY, 0);
  }

  //
  // Get Boot Mode
  //
  Status = PeiServicesGetBootMode (&BootMode);
  if (EFI_ERROR (Status)) {
    TxtInfoHob->Data.TxtMode = 0;
    goto Done;
  }

  Status = PeiServicesNotifyPpi (&mNotifyDesc);
  ASSERT_EFI_ERROR (Status);

  if (BootMode == BOOT_ON_S3_RESUME) {
    Status = PeiServicesNotifyPpi (&mNotifyDesc2);
    ASSERT_EFI_ERROR (Status);
  }

Done:
  ///
  /// If S-ACM reports TXT failure , disable TXT
  ///

  if (IsBptTxtPassed () == FALSE || SpiIsFlashDescriptorOverrideEnabled ()) {
    DEBUG ((DEBUG_WARN, "TXT failed reported by S-ACM\n"));
    TxtInfoHob->Data.TxtMode = 0;
  }

  return EFI_SUCCESS;
}


/**
  This is AliasCheck entry point for MRC call.  The routine checks for an TXT
  capable platform and determines whether ACHECK should be run.  If so,
  it launches the BIOS ACM to run ACHECK (which will reset the platform).
  The AliasCheck routine is in peitxtlib.

  @exception EFI_UNSUPPORTED  - If the platform is not TXT capable.
  @retval EFI_SUCCESS         - In all other cases not listed above.
**/
EFI_STATUS
EFIAPI
AliasCheck (
  VOID
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  TXT_INFO_HOB        *TxtInfoHob = NULL;

  DEBUG ((DEBUG_INFO, "New Alias Check Begin\n"));

  TxtInfoHob = GetFirstGuidHob (&gTxtInfoHobGuid);
  if (TxtInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "TxtInfoHob not found\n"));
    return EFI_NOT_FOUND;
  }

  if (IsTxtEnabled (TxtInfoHob)) {
    Status = IsTpmPresent (TxtInfoHob);
    if (!EFI_ERROR (Status)) {
      if (IsTxtChipset ()) {
        if (IsTxtProcessor ()) {
          DEBUG ((DEBUG_INFO, "TXT enabled platform\n"));
          if (((MmioRead32 (TXT_PUBLIC_BASE + TXT_SPAD_REG_OFF) & B_TXT_SPAD_ALIAS_CHECK) == B_TXT_SPAD_ALIAS_CHECK)
              || ((MmioRead32 (TXT_PUBLIC_BASE + TXT_CRASHCODE_REG_OFF) & 0xFE007FF0) == 0xC00040B0)
              || (IsAcheckRequested (TxtInfoHob))
              ) {
            DEBUG ((DEBUG_INFO, "DoAlias\n"));

            DoAcmLaunch (TxtInfoHob, TXT_LAUNCH_ACHECK);
            ///
            /// Reset platform - performed by DoAcheck, should not return to execute the following dead looping
            ///
            CpuDeadLoop ();
          }  // if special pattern
        } // IsTxtProcessor
      } // IsTxtChipset
    } // IsTpmPresent
  } // IsTxtEnabled
  DEBUG ((DEBUG_INFO, "New Alias Check End\n"));

  return EFI_UNSUPPORTED;
}

/**
  Perform TXT configuration part 1

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The memory discovered PPI.  Not used.

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
ConfigureTxtPart1 (
  IN EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  TXT_INFO_HOB        *TxtInfoHob;

  DEBUG ((DEBUG_INFO, "ConfigureTxtPart1\n"));

  TxtInfoHob = (TXT_INFO_HOB *) GetFirstGuidHob (&gTxtInfoHobGuid);
  if (TxtInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "TxtInfoHob not found.... Unloading\n"));
    return EFI_NOT_FOUND;
  }

  ///
  /// If TXT is enabled, configure platform appropriately.
  /// Code assumes that if TXT is enabled by CPU driver than all checks
  /// are passed, i.e. TPM is present, CPU and CS are TXT capable.
  ///
  ///
  /// Add to check CPU TXT capable in case CPU drivers do not check additional requirements
  ///
  if ((TxtInfoHob->Data.ChipsetIsTxtCapable) && (TxtInfoHob->Data.ProcessorIsTxtCapable) && (IsTxtEnabled (TxtInfoHob))) {
    DEBUG ((DEBUG_INFO, "TXT Enabled\n"));

    //
    // Update DPR Memory
    //
    Status = DprUpdate (TxtInfoHob);
    if (EFI_ERROR (Status)) {
      TxtInfoHob->Data.TxtMode = 0;
      return Status;
    }
  }

  return Status;
}

/**
  Perform TXT configuration part 2

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The memory discovered PPI.  Not used.

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
ConfigureTxtPart2 (
  IN EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  TXT_INFO_HOB        *TxtInfoHob;

  DEBUG ((DEBUG_INFO, "ConfigureTxtPart2\n"));

  TxtInfoHob = (TXT_INFO_HOB *) GetFirstGuidHob (&gTxtInfoHobGuid);
  if (TxtInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "TxtInfoHob not found.... Unloading\n"));
    return EFI_NOT_FOUND;
  }

    ///
  /// If ESTS.TXTRESET bit is set, skip all other functions since
  /// attempt to execute GETSEC will hang system. Skipping allows to
  /// boot to OS and let MLE assess situation.
    ///
  if (IsTxtResetSet ()) {
    DEBUG ((DEBUG_ERROR, "TXT_RESET bit is set.... Unloading\n"));
    return EFI_UNSUPPORTED;
  }

      ///
  /// If TXT is enabled, configure platform appropriately.
  /// Code assumes that if TXT is enabled by CPU driver than all checks
  /// are passed, i.e. TPM is present, CPU and CS are TXT capable.
      ///
    ///
  /// Add to check CPU TXT capable in case CPU drivers do not check additional requirements
    ///
  if ((TxtInfoHob->Data.ChipsetIsTxtCapable) && (TxtInfoHob->Data.ProcessorIsTxtCapable) && (IsTxtEnabled (TxtInfoHob))) {
    DEBUG ((DEBUG_INFO, "TXT Enabled\n"));

    //
    // Invoke TXT LockConfig during S3 resume
    //
    Status = DoPeiLaunchAcm (PeiServices, TxtInfoHob, TXT_LAUNCH_LOCK_CONFIG, S3_RESUME_PATH);
    if (EFI_ERROR (Status)) {
      TxtInfoHob->Data.TxtMode = 0;
      return Status;
    }
  }

  return Status;
}

/**
  Fix up pointers since they are located in real memory now.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The memory discovered PPI.  Not used.

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
DprUpdate (
  TXT_INFO_HOB        *TxtInfoHob
  )
{
  DPR_INFO_HOB       *DprInfoHob;
  DPR_DIRECTORY_ENTRY *DprDirectory;
  UINT16              Index;

  DprInfoHob  = NULL;
  DprDirectory = NULL;
  Index        = 0;

  DEBUG ((DEBUG_INFO, "DprUpdate\n"));

  //
  // Get TxtInfoHob
  //
  if (TxtInfoHob == NULL) {
    return EFI_NOT_FOUND;
  }

  TxtInfoHob->Data.TxtDprMemoryBase = 0;

  //
  // Get DprInfoHob
  //
  DprInfoHob = (DPR_INFO_HOB *) GetFirstGuidHob (&gDprInfoHobGuid);
  if (DprInfoHob == NULL) {
    return EFI_NOT_FOUND;
  }

  DprDirectory = (DPR_DIRECTORY_ENTRY *)&(DprInfoHob->DprDirectory[0]);

  //
  // Find TXT DPR Directory
  //
  for (Index = 0; Index < DPR_DIRECTORY_MAX; Index++) {
    if (DprDirectory[Index].Type == DPR_DIRECTORY_TYPE_TXT) {
      TxtInfoHob->Data.TxtDprMemoryBase = (EFI_PHYSICAL_ADDRESS) DprDirectory[Index].PhysBase;
      break;
    }
  }

  return EFI_SUCCESS;
}


/**
  This is ClearSecretsBit entry point for MRC call.

  @exception EFI_UNSUPPORTED  - If the platform is not TXT capable.
  @retval EFI_SUCCESS         - In all other cases not listed above.
**/
EFI_STATUS
EFIAPI
ClearSecretsBit (
  VOID
  )
{
  EFI_STATUS          Status      = EFI_SUCCESS;
  TXT_INFO_HOB        *TxtInfoHob = NULL;

  DEBUG ((DEBUG_INFO, "ClearSecretsBit\n"));

  TxtInfoHob = GetFirstGuidHob (&gTxtInfoHobGuid);
  if (TxtInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "TxtInfoHob not found\n"));
    return EFI_NOT_FOUND;
  }

  ///
  /// Invoke ACM
  ///
  DoAcmLaunch (TxtInfoHob, TXT_LAUNCH_CLEAR_SECRETS);

  ///
  /// Reset platform
  ///
  IssueGlobalReset ();

  return Status;
}

/**
  Set Power Down Request.

  @param[in] PdrSrc             Power down type
                            0 - No Power Down Request
                            1 - MRC requested Power Down due to scrubbing failure
                            2 - BIOS setup requested power down
                            3 - Power down through MOR interface

  @exception EFI_UNSUPPORTED  - If the platform is not TXT capable.
  @retval EFI_SUCCESS         - In all other cases not listed above.
**/
EFI_STATUS
EFIAPI
SetPowerDownRequest (
  UINT8 PdrSrc
  )
{
  EFI_STATUS                     Status      = EFI_SUCCESS;
  TXT_INFO_HOB                   *TxtInfoHob = NULL;

  DEBUG ((DEBUG_INFO, "SetPowerDownRequest\n"));

  TxtInfoHob = GetFirstGuidHob (&gTxtInfoHobGuid);
  if (TxtInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "TxtInfoHob not found\n"));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "PdrSrc = %d\n", PdrSrc));

  //
  // Set signal in TxtInfoHob to be consumed by request handler.
  //
  TxtInfoHob->Data.TxtPowerdownRequest = 1;
  DEBUG ((DEBUG_INFO, "TxtInfoHob->Data.TxtPowerdownRequest = %d\n", TxtInfoHob->Data.TxtPowerdownRequest));

  return Status;
}


/**
  This routine will find the FIT A Record
  @retval Pointer to FIT Record Type A
**/
VOID*
EFIAPI
GetFitAPtr (
  VOID
)
{
  UINT64                                 FitPointer;
  FIRMWARE_INTERFACE_TABLE_ENTRY         *FitEntry;
  UINT32                                 EntryNum;
  UINTN                                  Index;
  FitPointer = *(UINT64 *) (UINTN) FIT_POINTER_ADDRESS;
  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *) (UINTN) FitPointer;
  EntryNum = (FitEntry[0].Size[0] | (FitEntry[0].Size[1] << 8) | (FitEntry[0].Size[2] << 16));

  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntry[Index].Type == FIT_TYPE_0A_TXT_POLICY) {
      return (VOID *) FitEntry[Index].Address;
    }
  }

  return NULL;
}
