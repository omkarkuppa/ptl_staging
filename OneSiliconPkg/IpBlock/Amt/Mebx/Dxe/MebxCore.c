/** @file
  MEBx core functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PerformanceLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IntelRcStatusCode.h>
#include <Library/DxeAmtPolicyLib.h>
#include <Library/DxeMeLib.h>
#include <Library/HobLib.h>
#include <Library/MeUtilsLib.h>
#include <PchResetPlatformSpecific.h>
#include <Library/DxeMebxWrapperLib.h>
#include <Library/DxeMebxDisplayWrapperLib.h>
#include <Library/MeInfoLib.h>
#include <Protocol/AmtReadyToBoot.h>
#include <Protocol/MebxConfigProtocol.h>
#include <MeState.h>
#include <MeBiosPayloadHob.h>
#include <AmtConfig.h>
#include <Register/HeciRegs.h>
#include <MebxCore.h>
#include <MebxUserConsent.h>
#include <MebxRemoteAssistance.h>
#include <MebxUsbProvision.h>
#include <MebxData.h>

GLOBAL_REMOVE_IF_UNREFERENCED AMT_READY_TO_BOOT_PROTOCOL mAmtInvocationCodeReadyToBoot = {
  AMT_READY_TO_BOOT_PROTOCOL_REVISION,
  MebxInvocationCode
};

GLOBAL_REMOVE_IF_UNREFERENCED AMT_READY_TO_BOOT_PROTOCOL mAmtReadyToBoot = {
  AMT_READY_TO_BOOT_PROTOCOL_REVISION,
  MebxOnReadyToBoot
};

/**
  This function polls CSME for successful unconfiguration status.

  @param[in] Timeout                   Max polling time in seconds

  @retval    EFI_SUCCESS               Unconfiguration finished successfully
  @retval    EFI_TIMEOUT               Timeout has occurred, unconfiguration failed
  @retval    Others                    Unconfiguration failed
**/
EFI_STATUS
WaitForUnconfigFinish (
  IN UINT32 Timeout
  )
{
  EFI_STATUS Status;
  UINT8      UnconfigStatus;

  do {
    Status = HeciGetUnconfigureStatus (&UnconfigStatus);
    if (EFI_ERROR (Status)) {
      MebxDisplayError (CoreUnconfigWoPassGetUnconfigStatusError);
      return Status;
    }

    if (UnconfigStatus == ME_UNCONFIG_ERROR) {
      return EFI_ABORTED;
    } else if ((UnconfigStatus == ME_UNCONFIG_NOT_IN_PROGRESS) || (UnconfigStatus == ME_UNCONFIG_FINISHED)) {
      return EFI_SUCCESS;
    }
    MicroSecondDelay (1000000);
  } while (Timeout--);

  return EFI_TIMEOUT;

}

/**
  This function sends MEBx Entry/Exit messages to firmware

  @param[in]  MebxState    Informs if user enters or exits MEBx
                             TRUE  - MEBx Entry
                             FALSE - MEBx Exit

  @retval EFI_SUCCESS      MEBx Entry / Exit were signalled successfully
  @retval Others           Error has occurred during communication with AMT
**/
EFI_STATUS
MebxSignalEntryExit (
  IN AMT_MEBX_STATE MebxState
  )
{
  EFI_STATUS      Status;

  Status = MebxSetMebxState (MebxState);

  if (EFI_ERROR (Status)) {
    if (Status == EFI_ALREADY_STARTED) {
      MebxDisplayError (KvmActiveSessionMsg);
    } else if (MebxState == AmtMebxStateEnter) {
      MebxDisplayError (AmthiSetMebxEnterStateApiError);
    } else if (MebxState == AmtMebxStateExit) {
      MebxDisplayError (AmthiSetMebxExitStateApiError);
    }
  }

  return Status;
}

/**
  This function performs UsbProvisioning flow if required.

  @param[in] ProvisionRecord  Pointer to provision record
**/
VOID
PerformUsbProvisioningFlow (
  IN PROVISION_RECORD_HANDLE ProvisionRecord
  )
{
  EFI_STATUS Status;
  VOID       *UsbProvisionData;

  DEBUG ((DEBUG_INFO, "[MEBx] UsbProvisioningFlow start\n"));

  Status = MebxSignalEntryExit (AmtMebxStateEnter);
  if (EFI_ERROR (Status)) {
    return;
  }

  MebxConfigureScreen (ScreenModeMebx);
  MebxClearScreen ();

  UsbProvisionData = AllocateZeroPool (ProvisionRecord.ProvisionRecordSize);
  if (UsbProvisionData == NULL) {
    ASSERT (UsbProvisionData != NULL);
    return;
  }

  CopyMem (
    UsbProvisionData,
    ProvisionRecord.ProvisionRecordBuffer,
    ProvisionRecord.ProvisionRecordSize
    );

  Status = MebxOneTouchProvisioning (UsbProvisionData);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (CoreUsbProvError);
  }

  //
  // We remove the provision record after MEBx invocation
  // to account for the case when MEBx didn't consume the record
  // correctly. In such a case data record won't be removed.
  // MEBx returns information about data record successful consumption
  // by directly modifying data record buffer passed to it in MebxBiosParams
  //
  RemoveUsbProvisionRecord (&ProvisionRecord, UsbProvisionData);

  // clear screen after the usb provisioning flow output
  MebxClearScreen ();

  MebxSignalEntryExit (AmtMebxStateExit);
}

/**
  This function performs Remote Assistance flow if required.
**/
VOID
MebxRemoteAssistanceInit (
  VOID
  )
{
  EFI_STATUS Status;

  Status = MebxSignalEntryExit (AmtMebxStateEnter);
  if (EFI_ERROR (Status)) {
    return;
  }

  MebxConfigureScreen (ScreenModeMebx);
  ActivateRemoteAssistance ();
  MebxSignalEntryExit (AmtMebxStateExit);
}

/**
  Function checks if AMT support is enabled in CSME and current AMTHI
  Protocol version is supported.

  @retval     TRUE         AMT support enabled
  @retval     FALSE        AMT support disabled or incorrect AMTHI version.
**/
BOOLEAN
IsAmtSupportEnabled (
  VOID
  )
{
  ME_BIOS_PAYLOAD_HOB *MbpHob;

  MbpHob = NULL;
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL || MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.Amt == 0) {
    return FALSE;
  }

  return TRUE;
}

/**
  Unconfigure ME without password and try to wait for completion.
  Unconfiguration can take very long time, above a minute, so it is advised
  that BIOS should display some kind of warning before calling this function

  @param[in] Timeout   in seconds, how much time to wait for unconfiguration to finish.

  @retval EFI_ABORTED  Unconfiguration error
  @retval EFI_TIMEOUT  Unconfiguration is still in progress after time elapsed
  @retval EFI_SUCCESS  Unconfiguration finished successfully
**/
EFI_STATUS
UnconfigureWithoutPassword (
  IN UINT32 Timeout
  )
{
  EFI_STATUS Status;

  Status = HeciUnconfigWithoutPwd ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return WaitForUnconfigFinish (Timeout);
}

/**
  The function displays messages about CPU replacement and check whether an user accepts CPU change.

  @param[in] DisabledFeatures   Features which will be disabled
  @param[in] EnabledFeatures    Features which will be enabled

  @retval TRUE   An user accepts CPU change
  @retval FALSE  An user doesn't accept CPU change
**/
BOOLEAN
IsCpuChangeAccepted (
  IN UINT32 *DisabledFeatures,
  IN UINT32 *EnabledFeatures
  )
{
  UINT32          Index;
  BOOLEAN         FeaturesDisabled;
  BOOLEAN         FeaturesEnabled;
  EFI_STATUS      Status;
  MEBX_USER_INPUT UserInput;

  FeaturesEnabled  = FALSE;
  FeaturesDisabled = FALSE;

  //
  // Check all FeaturesDisabled/Enabled masks
  //
  for (Index = 0; Index < MAX_SKU_BITMAP; Index++) {
    if (DisabledFeatures[Index] != 0) {
      FeaturesDisabled = TRUE;
    }
    if (EnabledFeatures[Index] != 0) {
      FeaturesEnabled = TRUE;
    }
  }

  MebxDisplayText (CoreCpuReplacementMsg);
  if (FeaturesDisabled) {
    MebxDisplayText (CoreCautionMsg);
    MebxDisplayText (CoreFeaturesDisabledMsg);
  }
  if (FeaturesEnabled) {
    MebxDisplayText (CoreFeaturesEnabledMsg);
  }

  MebxDisplayText (CoreConfirm1Msg);
  MebxDisplayText (CoreConfirm2Msg);

  while (TRUE) {
    Status = MebxReadUserInput (&UserInput);
    if (EFI_ERROR (Status)) {
      return FALSE;
    }
    if (UserInput == MebxUserInputEnter) {
      break;
    }

    MicroSecondDelay (1000);
  }

  return TRUE;
}

/**
  This function performs Cpu Replacement flow if required.
  No other AMT specific action (autoprov, remote consent, etc) should be attempted, if this function wants reset.

  @param[in, out] ResetReason     Bitmask with reset reason
  @param[out]     SetDataSync     Informs if MEBx should set synchronization flag

  @retval     EFI_SUCCESS         Cpu replacement flow completed
  @retval     Others              Error during cpu replacement flow
**/
EFI_STATUS
CheckCpuReplacementFlow (
  IN OUT UINT32  *ResetReason,
     OUT BOOLEAN *SetDataSync
  )
{
  EFI_STATUS                   Status;
  GEN_GET_CPU_TYPE_CHANGE_DATA CpuTypeChangeInfo;
  BOOLEAN                      Consent;
  ME_BIOS_PAYLOAD_HOB          *MbpHob;
  MEFWCAPS_SKU                 FwCaps;

  ZeroMem (&CpuTypeChangeInfo, sizeof (GEN_GET_CPU_TYPE_CHANGE_DATA));

  Status = HeciGetCpuTypeChange (&CpuTypeChangeInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get CPU Type Change Error\n"));
    return Status;
  }

  if (CpuTypeChangeInfo.UserFeedback == USER_FEEDBACK_REQUESTED) {
    MebxConfigureScreen (ScreenModeMebx);
    Consent = IsCpuChangeAccepted (CpuTypeChangeInfo.FeaturesDisabled, CpuTypeChangeInfo.FeaturesEnabled);
    if (Consent) {
      Status = HeciSetCpuTypeChangeUserResponse (CPU_TYPE_CHANGE_ACCEPTED);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Set CPU Type Change Error\n"));
        return Status;
      }
      *SetDataSync = TRUE;
      *ResetReason |= MEBX_RESET_CPU_REPLACEMENT_USER_FEEDBACK;
    }
  }


  //
  // On workstation CSME is unaware whether the platform supports AMT during the first boot.
  // Send command to get capability and trigger 2nd GRST for data sync.
  //
  MbpHob = NULL;
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    return FALSE;
  }

  Status = HeciGetFwCapsSkuMsg (&FwCaps);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (MeGetFwCapsApiError);
    return Status;
  }

  if (CpuTypeChangeInfo.GlobalResetRequired ||
      (MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Amt != FwCaps.Fields.Amt)) {
    *ResetReason |= MEBX_RESET_CPU_REPLACEMENT_RESET_REQUIRED;
    *SetDataSync = TRUE;
  }

  return Status;
}

/**
  This function checks if Unconfigure flow, started on previous boot, has completed.

  @param[in]      Timeout               Timeout for checking if unconfig flow has finished
  @param[in, out] ResetReason           Bitmask with reset reason

  @retval     EFI_SUCCESS               Unconfigure flow completed
  @retval     EFI_TIMEOUT               Unconfigure flow exceeded allowed timeout
**/
EFI_STATUS
CheckUnconfigureFlow (
  IN     UINT32 Timeout,
  IN OUT UINT32 *ResetReason
  )
{
  EFI_STATUS Status;
  UINT8      UnconfigStatus;

  UnconfigStatus = 0;

  do {
    Status = HeciGetUnconfigureStatus (&UnconfigStatus);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    if (UnconfigStatus == ME_UNCONFIG_IN_PROGRESS) {
      MicroSecondDelay (1000000);
      Timeout--;
      continue;
    } else if (UnconfigStatus == ME_UNCONFIG_NOT_IN_PROGRESS) {
      return EFI_SUCCESS;
    } else if (UnconfigStatus == ME_UNCONFIG_FINISHED) {
      *ResetReason |= MEBX_RESET_UNCONFIG_FINISHED;
      return EFI_SUCCESS;
    } else if (UnconfigStatus >= ME_UNCONFIG_ERROR) {
      return EFI_DEVICE_ERROR;
    }
  } while (Timeout > 0);
  return EFI_TIMEOUT;
}

/**
  This function updates SOL/SRou state during the first MEBx invocation.

  @retval     EFI_SUCCESS           SoL state updates successfully
  @retval     Others                Error occurred during SoL state update
**/
EFI_STATUS
UpdateRedirectionState (
  VOID
  )
{
  EFI_STATUS                  Status;
  REDIRECTION_FEATURES_STATE  RedirectionFeaturesState;
  UINTN                       MebxDataSize;
  MEBX_DATA                   MebxData;
  MEBX_CONFIG_PROTOCOL        *MebxConfigProtocol;

  ZeroMem (&MebxData, sizeof (MEBX_DATA));

  Status = gBS->LocateProtocol (&gMebxConfigProtocolGuid, NULL, (VOID**) &MebxConfigProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Platform must implement this protocol to set/get Mebx settings\n"));
    return Status;
  }

  MebxDataSize = sizeof (MEBX_DATA);
  Status = MebxConfigProtocol->GetMebxConfig (&MebxDataSize, (VOID*) &MebxData);
  if (!EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "No AMT SOL settting present - need to retrieve initial SoL feature state\n"));

  // Retrieve Redirection enablement state
  Status = MebxGetRedirectionState (&RedirectionFeaturesState);
  DEBUG ((DEBUG_INFO, "MebxGetRedirectionState returns %r RedirectionFeaturesState = 0x%x\n", Status, RedirectionFeaturesState));
  if (!EFI_ERROR (Status)) {
    MebxData.AmtSol  = (UINT8) RedirectionFeaturesState.SolState;
    MebxData.AmtSrou = (UINT8) RedirectionFeaturesState.StorageRedirState;
  }

  DEBUG ((DEBUG_INFO, "Setting initial AmtSol / AmtSrou state  = %x / %x\n", MebxData.AmtSol, MebxData.AmtSrou));

  return MebxConfigProtocol->SetMebxConfig (MebxDataSize, (VOID*) &MebxData);
}

/**
  MEBX's invocation code flow.
  Compared to original implementation, it's missing autoprovisioning, setup and remote assistance

  @param[in]     AmtSupportEnabled Information if AMT support is enabled or not
  @param[in,out] ResetReason       Bitmask with reset reason

  @retval EFI_SUCCESS              MEBx has completed successfully
  @retval Others                   MEBx error has occurred
**/
EFI_STATUS
CheckFirmwareSpecialFlows (
  IN     BOOLEAN       AmtSupportEnabled,
  IN OUT UINT32        *ResetReason
  )
{
  EFI_STATUS           Status;
  BOOLEAN              SetDataSync;
  INVOCATION_CODE_BITS InvocationCode;

  SetDataSync = FALSE;
  ZeroMem (&InvocationCode, sizeof (INVOCATION_CODE_BITS));

  // Get Invocation Codes
  Status = HeciGetInvocationCode (&InvocationCode.Data);
  DEBUG ((DEBUG_INFO, "[MEBx] Invocation codes upon entry: InvocationCode.Data = 0x%x\n", InvocationCode.Data));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ME Invocation flow Error\n"));
  }

  if (InvocationCode.Fields.FwUnconfiguration) {
    DEBUG ((DEBUG_INFO, "Checking FW Unconfiguration Flow\n"));
    CheckUnconfigureFlow (UNCONFIGURE_TIMEOUT, ResetReason);
    SetDataSync = TRUE;
  }

#ifndef ME_WS_SUPPORT
  if (InvocationCode.Fields.CpuReplacement) {
#endif // ME_WS_SUPPORT
    DEBUG ((DEBUG_INFO, "Checking CPU Replacement Flow\n"));
    CheckCpuReplacementFlow (ResetReason, &SetDataSync);
#ifndef ME_WS_SUPPORT
    SetDataSync = TRUE;
  }
#endif // ME_WS_SUPPORT

  if (AmtSupportEnabled && InvocationCode.Fields.UserConsentFlow &&
      IsUserConsentRequired () && *ResetReason == MEBX_RESET_NO_RESET) {
    DEBUG ((DEBUG_INFO, "Performing User Consent Flow\n"));
    MebxConfigureScreen (ScreenModeMebx);
    PerformUserConsent ();
  }

  if (InvocationCode.Fields.WlanPowerConfigChange) {
    DEBUG ((DEBUG_ERROR, "WLAN Power Config Change\n"));
    *ResetReason |= MEBX_RESET_WLAN_PWR_CONFIG_CHANGE;
  }

  if (InvocationCode.Data != 0){
    Status = HeciClearInvocationCode (InvocationCode.Data);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "ME Invocation flow Error\n"));
    }
  }

  // If a sync is needed, set DataSyncConfirmation invocation code.
  // This code is needed to invoke MEBx on the next boot to verify
  // if there is no mismatch in synch structure due to CSE changes.
  if (SetDataSync) {
    DEBUG ((DEBUG_INFO, "Setting DATA_SYNC_CONFIRMATION invocation code\n"));
    Status = HeciSetInvocationCode (DATA_SYNC_CONFIRMATION);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "ME Invocation flow Error\n"));
    }
  }

  //
  // Make sure that we get the BIOS screen back
  //
  MebxConfigureScreen (ScreenModeBios);

  return EFI_SUCCESS;
}

/**
  Checks HFSTS2 register if CSME requested special flow support.

  @retval TRUE                    MEBx special flow support requested
  @retval FALSE                   MEBx special flow support not requested
**/
BOOLEAN
IsInvokeMebxBitSet (
  VOID
  )
{
  HECI_GS_SHDW_REGISTER MeFwsts2;

  MeFwsts2.ul = PciSegmentRead8 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS_2);

  if (MeFwsts2.ul != 0xFFFFFFFF) {
    return (MeFwsts2.r.InvokeMEBx == 1);
  }

  return FALSE;
}

/**
  Checks if Global Reset is needed by MEBx and sends adequate PET Alerts
  to indicate the event. If no reboot required issues additional PET Alert
  to indicate OS boot handoff.

  @param[in] ResetReason         Bitmask with reset reason
**/
VOID
CheckMebxResetAction (
  IN UINT32      ResetReason
  )
{
  PCH_RESET_DATA ResetData;

  if (ResetReason == MEBX_RESET_NO_RESET) {
    DEBUG ((DEBUG_INFO, "No MEBx Reset required. Proceeding with regular boot.\n"));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, EFI_SOFTWARE_UNSPECIFIED | EFI_SW_DXE_MEBX_OPROM_DONE);
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, EFI_SOFTWARE_UNSPECIFIED | EFI_SW_DXE_BS_PC_LEGACY_BOOT_EVENT);
  } else {
    REPORT_STATUS_CODE (EFI_ERROR_CODE, EFI_SOFTWARE_UNSPECIFIED | EFI_SW_DXE_MEBX_RESET_ACTION);
    DEBUG ((DEBUG_INFO, "MEBx returned Reset Reason: %x and requires Global Reset\n", ResetReason));
    CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
    StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
    gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
  }
}

/**
  Register MebxDisplay callback function for functions that need to interact with users.

  @param[in]  Event         The Event this notify function registered to.
  @param[in]  Context       Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
MebxDisplayCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS              Status;
  AMT_POLICY_PROTOCOL     *AmtPolicy;
  AMT_DXE_CONFIG          *AmtDxeConfig;
  static BOOLEAN          CallbackEntry = FALSE;

  if (CallbackEntry == TRUE) {
    return;
  }

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  gBS->CloseEvent (Event);
  CallbackEntry = TRUE;

  Status = gBS->LocateProtocol (&gDxeAmtPolicyGuid, NULL, (VOID **) &AmtPolicy);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = GetConfigBlock ((VOID *) AmtPolicy, &gAmtDxeConfigGuid, (VOID *) &AmtDxeConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  ///
  /// Check for Unconfigure Flow
  ///
  if (AmtDxeConfig->UnConfigureMe == 1) {
    DEBUG ((DEBUG_INFO, "[MEBx] Performing FW Unconfiguration Flow\n"));
    MebxClearScreen ();
    MebxConfigureScreen (ScreenModeMebx);
    MebxDisplayText (CoreUnconfigWoPassMsg);
    MicroSecondDelay(1000000);
    UnconfigureWithoutPassword (UNCONFIGURE_TIMEOUT);
    CheckMebxResetAction (MEBX_RESET_UNCONFIG_WITHOUT_PASSWORD);
    MebxConfigureScreen (ScreenModeBios);
  }
}

/**
  Check and perform MEBx specific actions.

  @param[out] ResetReason     Bitmask with reset reason
**/
VOID
CoreMebx (
  OUT UINT32          *ResetReason
  )
{
  EFI_STATUS              Status;
  AMT_POLICY_PROTOCOL     *AmtPolicy;
  AMT_DXE_CONFIG          *AmtDxeConfig;
  BOOLEAN                 AmtSupportEnabled;
  PROVISION_RECORD_HANDLE ProvisionRecord;

  Status = gBS->LocateProtocol (&gDxeAmtPolicyGuid, NULL, (VOID **) &AmtPolicy);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = GetConfigBlock ((VOID *) AmtPolicy, &gAmtDxeConfigGuid, (VOID *) &AmtDxeConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  ///
  /// If MebxDisplay protocol is not installed, we still need to perform the functions in callback.
  ///
  MebxDisplayCallback (NULL, NULL);

  AmtSupportEnabled = IsAmtSupportEnabled ();

  if (IsInvokeMebxBitSet ()) {
    CheckFirmwareSpecialFlows (AmtSupportEnabled, ResetReason);
    if (*ResetReason != MEBX_RESET_NO_RESET) {
      return;
    }
  }

  if (AmtSupportEnabled) {
    UpdateRedirectionState ();

    if (AmtDxeConfig->UsbProvision == 1 && GetUsbProvisionRecord (&ProvisionRecord)) {
      DEBUG ((DEBUG_INFO, "[MEBx] Performing USB Provisioning Flow\n"));
      PerformUsbProvisioningFlow (ProvisionRecord);
    } else if (AmtDxeConfig->CiraRequest == 1 && IsCiraAvailable ()) {
      DEBUG ((DEBUG_INFO, "[MEBx] Activating Remote Assistance\n"));
      MebxRemoteAssistanceInit ();
    }
  }
}

/**
  Signal an event for Amt Invocation Code ready to boot.
**/
VOID
EFIAPI
MebxInvocationCode (
  VOID
  )
{
  UINT32  ResetReason;

  DEBUG ((DEBUG_INFO, "%a - enter\n", __FUNCTION__));

  ResetReason = MEBX_RESET_NO_RESET;

  //
  // Set AmtSupportEnabled to FALSE and ignore User Consent Flow because gMebxDisplayProtocolGuid is not registered.
  //
  CheckFirmwareSpecialFlows (FALSE, &ResetReason);
  CheckMebxResetAction (ResetReason);
}

/**
  Signal an event for Amt ready to boot.
**/
VOID
EFIAPI
MebxOnReadyToBoot (
  VOID
  )
{
  UINT32                                        ResetReason;

  DEBUG ((DEBUG_INFO, "ME-BIOS: MEBx Entry.\n"));
  PERF_INMODULE_BEGIN ("MEBx");
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_MEBX_ENTRY); //PostCode (0xE07)

  ResetReason = MEBX_RESET_NO_RESET;
  CoreMebx (&ResetReason);
  CheckMebxResetAction (ResetReason);
  MebxConfigureScreen (ScreenModeBios);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_MEBX_EXIT); //PostCode (0xE27)
  PERF_INMODULE_END ("MEBx");
  DEBUG ((DEBUG_INFO, "ME-BIOS: MEBx Exit - Success.\n"));
}

/**
  This function checks if current CSME and BIOS configuration support MEBx.

  @retval  TRUE   CSME is ready for communication
  @retval  FALSE  CSME is not ready for communication
**/
BOOLEAN
IsMebxSupported (
  VOID
  )
{
  EFI_HOB_GUID_TYPE     *GuidHob;
  AMT_PEI_CONFIG        *AmtPeiConfig;

  //
  // Check if AMT support is enabled in the BIOS
  //
  GuidHob = NULL;
  GuidHob = GetFirstGuidHob (&gAmtPolicyHobGuid);
  if (GuidHob != NULL) {
    AmtPeiConfig = (AMT_PEI_CONFIG *) GET_GUID_HOB_DATA (GuidHob);
    if (AmtPeiConfig->AmtEnabled) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  This function checks if current CSME state supports MEBx.

  @retval  TRUE   CSME is ready for communication
  @retval  FALSE  CSME is not ready for communication
**/
BOOLEAN
IsFirmwareReady (
  VOID
  )
{
  EFI_STATUS    Status;
  UINTN         TimeoutCount;
  UINT32        MeMode;
  UINT32        MeStatus;

  TimeoutCount = 20;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || MeMode != ME_MODE_NORMAL) {
    DEBUG ((DEBUG_ERROR, "FW Status Error\n"));
    return FALSE;
  }

  Status = MeBiosGetMeStatus (&MeStatus);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FW Status Error\n"));
    return FALSE;
  }

  switch (ME_STATUS_ME_STATE_ONLY (MeStatus)) {

    case ME_READY:
      break;

    case ME_INITIALIZING:
    case ME_NOT_READY:
      do {
        Status = MeBiosGetMeStatus (&MeStatus);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "FW Status Error\n"));
          return FALSE;
        }
        MicroSecondDelay (500000);
        TimeoutCount--;
      } while (TimeoutCount != 0 && ME_STATUS_ME_STATE_ONLY (MeStatus) != ME_READY);

      if (ME_STATUS_ME_STATE_ONLY (MeStatus) != ME_READY) {
        DEBUG ((DEBUG_ERROR, "FW Status Initializing Error\n"));
        return FALSE;
      }
      break;
    case ME_DISABLE_WAIT:
    case ME_IN_RECOVERY_MODE:
    case ME_TRANSITION:
    default:
      DEBUG ((DEBUG_ERROR, "FW Status Error\n"));
      return FALSE;

  }

  return TRUE;
}

/**
  The driver entry point - MEBx Driver main body.

  @param[in] ImageHandle          Handle for this drivers loaded image protocol.
  @param[in] SystemTable          EFI system table.

  @retval EFI_SUCCESS             The driver installed without error.
  @retval EFI_UNSUPPORTED         The driver is unsupported in current configuration.
**/
EFI_STATUS
EFIAPI
MebxDriverEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_HANDLE            Handle;
  VOID                  *Registration;
  EFI_EVENT             Event;

  if (!MeClientIsCorporate () || !IsFirmwareReady ()) {
    return EFI_UNSUPPORTED;
  }

  if (!IsMebxSupported ()) {
    Status = EFI_UNSUPPORTED;
    if (IsInvokeMebxBitSet ()) {
      ///
      /// Install an Amt ReadyToBoot protocol to handle invocation cause when AmtPeiConfig->AmtEnabled is default disabled.
      ///
      Handle = NULL;
      Status = gBS->InstallMultipleProtocolInterfaces (
                      &Handle,
                      &gAmtReadyToBootProtocolGuid,
                      &mAmtInvocationCodeReadyToBoot,
                      NULL
                      );
      ASSERT_EFI_ERROR (Status);
    }

    return Status;
  }

  ///
  /// Install an Amt ready to boot protocol.
  ///
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gAmtReadyToBootProtocolGuid,
                  &mAmtReadyToBoot,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Register MebxDisplay callback function for functions that need to interact with users.
  ///
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  MebxDisplayCallback,
                  NULL,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->RegisterProtocolNotify (
                  &gMebxDisplayProtocolGuid,
                  Event,
                  &Registration
                  );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

