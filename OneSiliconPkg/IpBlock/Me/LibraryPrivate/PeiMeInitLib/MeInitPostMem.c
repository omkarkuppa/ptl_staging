/** @file
  The PEI Library Implements ME Init.

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

@par Specification Reference:
**/

#include <PiPei.h>
#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/PeiMeLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/HeciInitLib.h>
#include <Library/MeChipsetLib.h>
#include <Library/MeShowBufferLib.h>
#include <Library/MeFwStsLib.h>
#include <Library/MeInfoLib.h>
#include <Library/MeInitLib.h>
#include <Library/PmcLib.h>
#include <Library/PcdLib.h>
#include <Register/PseRegs.h>
#include <Register/HeciRegs.h>
#include <Register/PchRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <MeBiosPayloadHob.h>
#include <AmtConfig.h>
#include <MePeiConfig.h>
#include <PchPolicyCommon.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <MeHandle.h>
#include <MeFwHob.h>
#include <PciSbdf.h>

/**
  Trigger PSE End Of Manufacturing flow
  Note: PSE EOM write register is offset 4Ch, and read back from offset 40h.
**/
VOID
TriggerPseEom (
  VOID
  )
{
  EFI_STATUS     Status;
  UINT64         HeciBaseAddress;
  PSE_HFS        PseHfs;
  UINTN          Timeout;
  SI_POLICY_PPI  *SiPolicyPpi;
  ME_PEI_CONFIG  *MePeiConfig;
  ME_HANDLE      *MeHandle;

  DEBUG ((DEBUG_INFO, "%a () enter\n", __FUNCTION__));

  Status = GetMeHandle (&MeHandle);
  if (EFI_ERROR (Status)) {
    return;
  }

  HeciBaseAddress = MeHandle->Callbacks->GetPseHeciPciCfgBase (PseHeci1);

  if (HeciBaseAddress == MAX_UINT64) {
    DEBUG ((DEBUG_INFO, "%a () exit, PSE does not exist\n", __FUNCTION__));
    return;
  }

  DEBUG ((DEBUG_INFO, "PSE HFS  = 0x%08x\n", PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS)));
  DEBUG ((DEBUG_INFO, "PSE H_GS = 0x%08x\n", PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_H_GS)));

  Status = PeiServicesLocatePpi (&gSiPolicyPpiGuid, 0, NULL, (VOID **) &SiPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);

  PseHfs.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS);
  if (!PseHfs.r.EomHwBindingEnable || PseHfs.r.PseSocCl) {
    return;
  }

  if (PseHfs.r.EomFirstBoot || MePeiConfig->PseEomFlowEnable) {
    PciSegmentOr32 (HeciBaseAddress + R_ME_CFG_H_GS, B_ME_CFG_H_GS_PSE_EOM_TRIGGER);

    //
    // Polling timeout is 15 seconds, interval is 10 msec.
    //
    Timeout = PSE_MAX_WAIT_TIMEOUT * 100;
    do {
      PseHfs.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS);
      if (PseHfs.r.EomStatus == PSE_EOM_STATUS_SUCCESS) {
        DEBUG ((DEBUG_INFO, "PSE EOM Success!! PSE HFS: 0x%08x\n", PseHfs.ul));
        return;
      }

      MicroSecondDelay (ME_STATE_STALL_1_SECOND / 100);
      Timeout--;
    } while (Timeout > 0);

    DEBUG ((DEBUG_ERROR, "Timeout: PSE EOM fail after %ds. PSE HFS: 0x%08x, PSE H_GS = 0x%08x\n",
      PSE_MAX_WAIT_TIMEOUT,
      PseHfs.ul,
      PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_H_GS)
      ));
    return;
  }
}

/**
  Send Optional Heci Command.
**/
VOID
SendOptionalHeciCommand (
  VOID
  )
{
  EFI_STATUS                Status;
  SI_POLICY_PPI             *SiPolicyPpi;
  ME_PEI_CONFIG             *MePeiConfig;
  ME_BIOS_PAYLOAD_HOB       *MbpHob;
  UINT32                    UnconfigOnRtcClear;

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  SiPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate gSiPolicyPpiGuid fail. Status = %r\n", Status));
    return;
  }
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get ME PeiConfigBlock fail, Status = %r\n", Status));
    return;
  }

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob != NULL && MbpHob->MeBiosPayload.UnconfigOnRtcClearState.Available == 1) {
    UnconfigOnRtcClear = MbpHob->MeBiosPayload.UnconfigOnRtcClearState.UnconfigOnRtcClearData.DisUnconfigOnRtcClearState;
    if ((MePeiConfig->MeUnconfigOnRtcClear == 1) && (UnconfigOnRtcClear == DisableState)) {
      UnconfigOnRtcClear = EnableState;
    } else if ((MePeiConfig->MeUnconfigOnRtcClear == 0) && (UnconfigOnRtcClear == EnableState)) {
      UnconfigOnRtcClear = DisableState;
    } else {
      ///
      /// Platform policy same as feature state retrieved from MBP. No update needed.
      ///
      return;
    }
    Status = PeiHeciSetUnconfigOnRtcClearDisableMsg (UnconfigOnRtcClear);
  }
}

/**
  Save all policies in PEI phase to HOB.
**/
VOID
SavePolicyToHob (
  VOID
  )
{
  EFI_STATUS                Status;
  SI_POLICY_PPI             *SiPolicyPpi;
  SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi;
  ME_PEI_PREMEM_CONFIG      *MePeiPreMemConfig;
  ME_PEI_CONFIG             *MePeiConfig;
#if FixedPcdGetBool (PcdAmtEnable) == 1
  AMT_PEI_CONFIG            *AmtPeiConfig;
#endif
  UINTN                     HobSize;
  VOID                      *HobPtr;

  SiPreMemPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  SiPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);

#if FixedPcdGetBool (PcdAmtEnable) == 1
  AmtPeiConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gAmtPeiConfigGuid, (VOID *) &AmtPeiConfig);
  ASSERT_EFI_ERROR (Status);
#endif

  //
  // Create ME/AMT HOBs.
  //
  HobSize = MePeiPreMemConfig->Header.GuidHob.Header.HobLength;
  DEBUG ((DEBUG_INFO, "HobSize for MePeiPreMemConfig: %x\n", HobSize));
  HobPtr = BuildGuidDataHob (&gMePreMemPolicyHobGuid, MePeiPreMemConfig, HobSize);
  ASSERT (HobPtr != 0);

  HobSize = MePeiConfig->Header.GuidHob.Header.HobLength;
  DEBUG ((DEBUG_INFO, "HobSize for MePeiConfig: %x\n", HobSize));
  HobPtr = BuildGuidDataHob (&gMePolicyHobGuid, MePeiConfig, HobSize);
  ASSERT (HobPtr != 0);

#if FixedPcdGetBool (PcdAmtEnable) == 1
  if (AmtPeiConfig != NULL) {
    HobSize = AmtPeiConfig->Header.GuidHob.Header.HobLength;
    DEBUG ((DEBUG_INFO, "HobSize for AmtPeiConfig: %x\n", HobSize));
    HobPtr = BuildGuidDataHob (&gAmtPolicyHobGuid, AmtPeiConfig, HobSize);
    ASSERT (HobPtr != 0);
  }
#endif
}

/**
  Configure HECI devices on End Of Pei

  @param[in]  PeiServices        Pointer to PEI Services Table.
  @param[in]  NotifyDescriptor   Pointer to the descriptor for the Notification event that caused this function to execute.
  @param[in]  Ppi                Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS        The function completes successfully

**/
EFI_STATUS
EFIAPI
MeOnEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  UINT8                MeDevFunction;

  DEBUG ((DEBUG_INFO, "%a() enter\n", __FUNCTION__));

  if (GetBootModeHob () == BOOT_ON_S3_RESUME) {

    if (MeHeciD0I3Enabled () == FALSE) {
      return EFI_SUCCESS;
    }
    //
    // Set D0I3 bits if resuming from S3 for EDK2.
    // For FSP case, it will be set in MeEndOfFirmwareEvent.
    //
    DEBUG ((DEBUG_INFO, "Setting D0I3 bits for HECI devices on S3 resume path\n"));
    for (MeDevFunction = HECI1; MeDevFunction <= HECI4; MeDevFunction++) {
      if (!IsHeciDeviceFunction (MeDevFunction)) {
        continue;
      }
      SetD0I3Bit (MeDevFunction);
    }
  } else {
    SendOptionalHeciCommand ();
    TriggerPseEom ();
    //
    // Save ME/AMT policies in PEI phase to HOB for using in DXE.
    //
    SavePolicyToHob ();
  }

  DEBUG ((DEBUG_INFO, "%a() done.\n", __FUNCTION__));
  return EFI_SUCCESS;
}

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mMeEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) MeOnEndOfPei
};

/**
  Check if Firmware requests the platform to invoke Data Resiliency flow

  @param[in] SiPolicy        The Silicon Policy PPI instance

  @retval TRUE               CSME Data Resiliency is requested
  @retvak FALSE              CSME Data Resiliency is NOT requested
**/
BOOLEAN
IsDataResiliencyRequired (
  IN CONST SI_POLICY_PPI  *SiPolicy
  )
{
  UINT64                   HeciBaseAddress;
  HECI_GS_SHDW_REGISTER    MeFwSts2;
  EFI_STATUS               Status;
  ME_PEI_CONFIG            *MePeiConfig;

  Status = GetConfigBlock ((VOID*) SiPolicy, &gMePeiConfigGuid, (VOID*) &MePeiConfig);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  if (!MePeiConfig->CseDataResilience) {
    DEBUG ((DEBUG_INFO, "CSE Data Resilience support disabled\n"));
    return FALSE;
  }

  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  if (PciSegmentRead32 (HeciBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
    return FALSE;
  }
  MeFwSts2.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_2);
  if (MeFwSts2.r.MfsFailure == 0) {
    return FALSE;
  }

  return TRUE;
}

/**
  Invoke data clear for CSME
  If data clear command fails, BIOS should continue to boot;
  else BIOS send CSE reset and wait for CSE to finish data restore and trigger global reset.
**/
VOID
InvokeDataResiliency (
  VOID
  )
{
  EFI_STATUS    Status;
  UINT8         Timeout;

  DEBUG ((DEBUG_INFO, "%a () enter\n", __FUNCTION__));

  Status = PeiHeciDataClear ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Data Clear fail, Status = %r\n", Status));
    return;
  }

  PeiHeciSendCbmResetRequest (CBM_RR_REQ_ORIGIN_BIOS_POST, CBM_HRR_CSME_RESET);
  //
  // CSME will trigger global reset after data restore succeeds.
  //
  Timeout = ME_FORMAT_FILE_TIMEOUT;
  DEBUG ((DEBUG_INFO, "Waiting for formating file system and restoring data to start"));
  do {
    MicroSecondDelay (ME_STATE_STALL_1_SECOND);
    DEBUG ((DEBUG_INFO, "."));
  } while (Timeout--);

  DEBUG ((DEBUG_WARN, "\n Status polling timeout, CSME should trigger global reset."));
  CpuDeadLoop ();
}

/**
  Wait until CSME updates Operation Mode.

  @retval EFI_SUCCESS             Operation mode is updated.
  @retval EFI_DEVICE_ERROR        CSME device error.
  @retval EFI_TIMEOUT             Timeout exceeded.
**/
EFI_STATUS
WaitForOpModeUpdate (
  VOID
  )
{
  HECI_FWS_REGISTER        MeFirmwareStatus;
  UINTN                    Timeout;
  UINT64                   HeciBaseAddress;

  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  if (PciSegmentRead16 (HeciBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    return EFI_DEVICE_ERROR;
  }

  Timeout = MSG_MAX_WAIT_TIMEOUT * 10;
  do {
    ///
    /// Read ME status and check for operation mode
    ///
    MeFirmwareStatus.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS);
    if (MeFirmwareStatus.r.MeOperationMode != ME_OPERATION_MODE_NORMAL) {
      return EFI_SUCCESS;
    }

    MicroSecondDelay (ME_STATE_STALL_1_SECOND / 10);
    Timeout--;
  } while (Timeout > 0);

  DEBUG ((DEBUG_ERROR, "Timeout: CSME is in NORMAL mode after %ds. MeFirmwareStatus: %08x.\n", MSG_MAX_WAIT_TIMEOUT, MeFirmwareStatus.ul));
  return EFI_TIMEOUT;
}

/**
  Save FWSTS to ME FWSTS HOB, if the HOB is not existing, the HOB will be created and publish.
  If the HOB is existing, the data will be overrided.
**/
VOID
SaveFwStsToHob (
  VOID
  )
{
  ME_FW_HOB              *MeFwHob;
  UINT8                  Index;
  UINT8                  Count;
  EFI_STATUS             Status;
  HECI_GS_SHDW_REGISTER  Fwsts2;

  if (PmcIsDwrBootMode () == TRUE) {
    DEBUG ((DEBUG_INFO, "DWR detected : ME FW HOB not installed\n"));
    return;
  }

  MeFwHob = GetFirstGuidHob (&gMeFwHobGuid);
  if (MeFwHob == NULL) {
    //
    // Create HOB for ME Data
    //
    Status = PeiServicesCreateHob (
               EFI_HOB_TYPE_GUID_EXTENSION,
               sizeof (ME_FW_HOB),
               (VOID **) &MeFwHob
               );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return;
    }

    DEBUG ((DEBUG_INFO, "ME FW HOB installed\n"));

    //
    // Initialize default HOB data
    //
    ZeroMem (&(MeFwHob->Revision), (sizeof (ME_FW_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
    MeFwHob->EfiHobGuidType.Name = gMeFwHobGuid;
    MeFwHob->Revision = 1;
    MeFwHob->Count = MAX_HECI_FUNC;
  }

  Fwsts2.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS_2);
  if (Fwsts2.r.CseToDisabled) {
    //
    // Waiting for FWSTS to update the operation mode if CSE is on the way to be disabled.
    //
    WaitForOpModeUpdate ();
  }

  DEBUG ((DEBUG_INFO, "ME FW HOB data updated\n"));
  //
  // Save the FWSTS registers set for each MEI device.
  //
  for (Count = 0; Count < MeFwHob->Count; Count++) {
    for (Index = 0; Index < MAX_FWSTS_REG; Index++) {
      MeFwHob->Group[Count].Reg[Index] = PciSegmentRead32 (MeHeciPciCfgBase (gFwStsDeviceList[Count].HeciDev) + gFwStsOffsetTable[Index]);
    }
    MeFwHob->Group[Count].FunNumber = (UINT32)gFwStsDeviceList[Count].HeciDev;
  }

  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "Current ME FW HOB data printed - \n"));
  ShowBuffer ((UINT8 *) MeFwHob, sizeof (ME_FW_HOB));
  DEBUG_CODE_END ();
}

/**
 Configure CSME HECI device interrupts

 @param[in] SiPolicy      The Silicon Policy PPI instance
 @param[in] MeHandle      Pointer to ME Handle.
**/
STATIC
VOID
ConfigureCsmeHeciDeviceInterrupts (
  IN CONST SI_POLICY_PPI  *SiPolicy,
  IN       ME_HANDLE      *MeHandle
  )
{
  EFI_STATUS  Status;
  PCI_SBDF    HeciSbdf;
  HECI_DEVICE MeDev;

  for (MeDev = HECI1; MeDev <= HECI4; MeDev++) {
    DEBUG ((DEBUG_INFO, "%a () for MeDev = %d\n", __FUNCTION__, MeDev));

    HeciSbdf.PciSegmentLibAddress = MeHandle->Callbacks->GetCsmeHeciPciCfgBase (MeDev);
    if (PciSegmentRead16 (HeciSbdf.PciSegmentLibAddress + PCI_VENDOR_ID_OFFSET) == MAX_UINT16) {
      DEBUG ((DEBUG_INFO, "%a () exit (Device disabled)\n", __FUNCTION__));
      continue;
    }

    Status = MeHandle->Callbacks->SetCsmeHeciInterruptPin (MeHandle, SiPolicy, HeciSbdf);
    if (Status == EFI_ABORTED) {
      DEBUG ((DEBUG_INFO, "Desired interrupt pin is same as default, return directly\n"));
    } else if (Status == EFI_DEVICE_ERROR) {
      DEBUG ((DEBUG_ERROR, "%a () Error while setting interrupt pin, Status = %r\n", __FUNCTION__, Status));
    }
  }
}

/**
  Returns the mapping of CSME devices that need to be hidden for PLATFORM_POR.
  The mapping is based only on ME-Bios boot path taken. Additional devices
  might be hidden later based on other platform policies.

  @retval ME_DEV_EXPOSURE   Mapping of CSME devices to be hidden
                              @see ME_DEV_EXPOSURE for bit assignment
**/
ME_DEV_EXPOSURE
GetBootPathMeDevHidePolicy (
  VOID
  )
{
  ME_BIOS_BOOT_PATH MeBiosPath;
  ME_DEV_EXPOSURE   MeDevHidePolicy;

  MeBiosPath = CheckMeBootPath ();

  DEBUG ((DEBUG_INFO, "Current MeBiosBootPath - %d\n", MeBiosPath));

  switch (MeBiosPath) {
    case NormalPath:
      MeDevHidePolicy = (HIDE_MEI2 | HIDE_MEI3);
      break;
    case SecoverMeiMsgPath:
    case SwTempDisablePath:
    case SecoverJmprPath:
    case DebugModePath:
      MeDevHidePolicy = (HIDE_MEI2 | HIDE_MEI3 | HIDE_AMT_DEVICE);
      break;
    case ErrorPath:
    case RecoveryPath:
      MeDevHidePolicy = (HIDE_MEI2 | HIDE_MEI3 | HIDE_AMT_DEVICE);
      break;
    case ErrorWithoutDidMsgPath:
    default:
      MeDevHidePolicy = HIDE_ALL_ME_DEVICE;
  }

  return MeDevHidePolicy;
}

/**
  Disable ME Devices when needed

  @retval EFI_STATUS   Operation status
**/
EFI_STATUS
MeDeviceConfigure (
  IN ME_HANDLE         *MeHandle
  )
{
  EFI_STATUS           Status;
  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi;
  ME_PEI_PREMEM_CONFIG *MePeiPreMemConfig;
  ME_BIOS_BOOT_PATH    MeBiosPath;
  ME_DEV_EXPOSURE      MeDeviceExposure;
#if FixedPcdGetBool(PcdAmtEnable) == 1
  SI_POLICY_PPI        *SiPolicy;
  AMT_PEI_CONFIG       *AmtPeiConfig;
#endif

  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));

  ///
  /// Step 1. Determine which devices should be hidden depending on specific Me-Boot Path
  ///
  MeDeviceExposure = GetBootPathMeDevHidePolicy ();

  ///
  /// Step 2. Perform device disabling based on FW capability and feature state
  ///         in all boot mode if it is not listed by ME Bios Boot path.
  ///         Device enabling/disabling can be overridden by FORCE ENABLE/DISABLE policy.
  ///

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
  if (!EFI_ERROR (Status)) {
    if (MePeiPreMemConfig->HeciCommunication2 == FORCE_DISABLE) {
      MeDeviceExposure |= HIDE_MEI2;
    } else if (MePeiPreMemConfig->HeciCommunication2 == FORCE_ENABLE) {
      MeDeviceExposure &= ~HIDE_MEI2;
    }

    if (MePeiPreMemConfig->HeciCommunication3 == FORCE_DISABLE) {
      MeDeviceExposure |= HIDE_MEI3;
    } else if (MePeiPreMemConfig->HeciCommunication3 == FORCE_ENABLE) {
      MeDeviceExposure &= ~HIDE_MEI3;
    }

    if (MePeiPreMemConfig->HeciCommunication == FORCE_DISABLE) {
      MeDeviceExposure |= HIDE_MEI1;
    } else if (MePeiPreMemConfig->HeciCommunication == FORCE_ENABLE) {
      MeDeviceExposure &= ~HIDE_MEI1;
    }

    if (MeDeviceExposure & HIDE_MEI1) {
      MeDeviceExposure |= HIDE_MEI2;
    }

    switch (MePeiPreMemConfig->KtDeviceEnable) {
      case FORCE_DISABLE:
        MeDeviceExposure |= HIDE_SOL;
        break;
      case FORCE_ENABLE:
        MeDeviceExposure &= ~HIDE_SOL;
        break;
      case PLATFORM_POR:
      default:
        //
        // Since P2SB lock in EndOfPei phase, move all SOL disable check here.
        //
        MeDeviceExposure |= HIDE_SOL;
#if FixedPcdGetBool (PcdAmtEnable) == 1
        Status = PeiServicesLocatePpi (
                    &gSiPolicyPpiGuid,
                    0,
                    NULL,
                    (VOID **) &SiPolicy
                    );
        if (!EFI_ERROR (Status)) {
          Status = GetConfigBlock ((VOID *) SiPolicy, &gAmtPeiConfigGuid, (VOID *) &AmtPeiConfig);
          if (!EFI_ERROR (Status) && AmtPeiConfig->AmtSolEnabled == 1) {
            MeDeviceExposure &= ~HIDE_SOL;
          }
        }
#endif
      break;
    }
  }

  ///
  /// Step 3. Perform checking for specific boot mode with ME Bios Boot Path,
  ///         like MeDisableSecoverMeiMsgBiosPath. HECI1 shall be disabled
  ///         after HMRFPO_DISABLE_MSG sent in previous POST boot path
  ///
  if (GetBootModeHob () == BOOT_ON_S3_RESUME) {
    MeBiosPath = CheckMeBootPath ();
    if ((MeBiosPath == SecoverMeiMsgPath) || (MeBiosPath == SwTempDisablePath) || (MeBiosPath == SecoverJmprPath)) {
      MeDeviceExposure |= HIDE_MEI1;
    }
  }

  ///
  /// Save MEI FWSTS registers set before disable it. The MEI device access right will be removed
  /// by late POST, hence save current FWSTS before disable them for reference without further enabling
  /// MEI steps required. The HOB data might be updated if gets failure when send EOP message in PEI
  /// phase, then FWSTS registers will be updated to reflect the last status before disable rest MEI devices
  ///
  SaveFwStsToHob ();

  if (MeDeviceExposure & HIDE_MEI1) {
    MeHandle->Callbacks->SetCsmeHeciDeviceState (MeHandle, HECI1, Disabled);
  } else {
    MeHandle->Callbacks->SetCsmeHeciDeviceState (MeHandle, HECI1, Enabled);
  }

  if (MeDeviceExposure & HIDE_MEI2) {
    MeHandle->Callbacks->SetCsmeHeciDeviceState (MeHandle, HECI2, Disabled);
  } else {
    MeHandle->Callbacks->SetCsmeHeciDeviceState (MeHandle, HECI2, Enabled);
  }
  if (MeDeviceExposure & HIDE_MEI3) {
    MeHandle->Callbacks->SetCsmeHeciDeviceState (MeHandle, HECI3, Disabled);
  } else {
    MeHandle->Callbacks->SetCsmeHeciDeviceState (MeHandle, HECI3, Enabled);
  }
  if (MeDeviceExposure & HIDE_SOL) {
    MeHandle->Callbacks->SetCsmeHeciDeviceState (MeHandle, SOL, Disabled);
  } else {
    MeHandle->Callbacks->SetCsmeHeciDeviceState (MeHandle, SOL, Enabled);
  }

  ///
  /// Always disable IDE-r and HECI4 with CSME in all boot mode.
  ///
  MeHandle->Callbacks->SetCsmeHeciDeviceState (MeHandle, IDER, Disabled);
  MeHandle->Callbacks->SetCsmeHeciDeviceState (MeHandle, HECI4, Disabled);

  return EFI_SUCCESS;
}

/**
  This function performs basic initialization for ME in PEI phase after memory is initialized.

  @param[in] SiPolicy     The Silicon Policy PPI instance

**/
VOID
MePostMemInit (
  IN SI_POLICY_PPI *SiPolicy
  )
{
  EFI_STATUS Status;
  ME_HANDLE  *MeHandle;

  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));

  Status = GetMeHandle (&MeHandle);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Configure CSME devices
  //
  Status = MeDeviceConfigure (MeHandle);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Configure interrupts for CSME devices
  //
  ConfigureCsmeHeciDeviceInterrupts (SiPolicy, MeHandle);

  ///
  /// Check if CSME data resiliency is required before Core BIOS Done.
  ///
  if (IsDataResiliencyRequired (SiPolicy)) {
    InvokeDataResiliency ();
  }

  PeiServicesNotifyPpi (&mMeEndOfPeiNotifyList);

  DEBUG ((DEBUG_INFO, "%a () - End\n", __FUNCTION__));
}

/**
  Checks EndOfManufacturing (EOM)

  @retval TRUE   Out of Manufacturing Mode
  @retval FALSE  In Manufacturing Mode
**/
BOOLEAN
IsEom (
  VOID
  )
{
  HECI_FW_STS6_REGISTER          MeFirmwareStatus;

  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS_6);
  return ((MeFirmwareStatus.r.FpfSocConfigLock) ? TRUE : FALSE);
}

/**
  Check if MCTP (Management Component Transport) Protocol is supported by CSME and BIOS.

  @param[in] SiPolicy   Silicon Policy

  @retval    TRUE                     MCTP is supported by CSME or BIOS
  @retval    FALSE                    MCTP is not supported by CSME or BIOS
**/
BOOLEAN
IsMctpConfigurationSupported (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  EFI_STATUS             Status;
  MEFWCAPS_SKU           FwCapsSku;
  ME_BIOS_PAYLOAD_HOB    *MbpHob;
  ME_PEI_CONFIG          *MePeiConfig;

  Status = GetConfigBlock ((VOID*) SiPolicy, &gMePeiConfigGuid, (VOID*) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);

  MbpHob = NULL;
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);

  if (MePeiConfig->MctpBroadcastCycle == 0) {
    return FALSE;
  }

  if (MbpHob != NULL) {
    if ((MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Amt == 1) &&
        (MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.FullMng == 1))
      return TRUE;
  } else {
    //
    // When MBP is not available (e.g S3 resume), we need
    // to retrieve Feature Capabilities and State from CSME
    //
    Status = PeiHeciGetFwCapsSkuMsg (&FwCapsSku);
    if (!EFI_ERROR (Status) && FwCapsSku.Fields.Amt == 1) {
      Status = PeiHeciGetFwFeatureStateMsg (&FwCapsSku);
      if (!EFI_ERROR (Status) && FwCapsSku.Fields.FullMng == 1) {
        return TRUE;
      }
    }
  }
  return FALSE;
}