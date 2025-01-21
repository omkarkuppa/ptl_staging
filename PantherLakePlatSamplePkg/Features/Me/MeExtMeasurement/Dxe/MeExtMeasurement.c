/** @file
  CSME Extended Measurement driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include "MeExtMeasurement.h"

CSME_INFO_FLAGS  mCsmeInfoFlags;
BOOLEAN          mMeInvalidStateFlag = FALSE;

/**
  Perform measurement for HER register.

  @param[in] HerValue             The value of HECI Extend Register.
  @param[in] Index                HerValue Size.
  @param[in] HashAlgorithm        Hash Algorithm.

  @retval EFI_SUCCESS             Measurement performed.
**/
EFI_STATUS
MeasureHer (
  IN  UINT32                      *HerValue,
  IN  UINT8                       Index,
  IN  UINT8                       HashAlgorithm
  )
{
  EFI_STATUS              Status;
  UINTN                   HashDataLen;
  UINT8                   *TempEvent;
  UINT32                  EventLogSize;
  UINT8                   LogEntryIndex;
  UINT8                   RemainingEntries;
  UINT32                  RemainingBufSize;
  UINT32                  ReceivedLogSize;
  CSME_MEASUREMENT_EVENT  CsmeEvent;
  static CONST CHAR8      Signature[] = "IntelCSxEEvent01";
  BOOLEAN                 LogUnavailable = FALSE;

  ///
  /// Get ER Log using BUP_COMMON_GET_ER_LOG and pass that log to Tpm Measurement
  ///
  ZeroMem (&CsmeEvent, sizeof (CsmeEvent));
  CopyMem (&(CsmeEvent.Signature), Signature, AsciiStrLen (Signature));

  CsmeEvent.ERHashAlgorithm = HashAlgorithm;
  EventLogSize = 0;
  RemainingBufSize = MAX_EVENTLOG_SIZE;
  ReceivedLogSize = 0;
  LogEntryIndex = 0;
  TempEvent = CsmeEvent.Events;

  do {
    RemainingEntries = 0;
    Status = HeciGetErLog (
               TempEvent,
               RemainingBufSize,
               &ReceivedLogSize,
               &LogEntryIndex,
               &RemainingEntries);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to get ER Log data\n"));
      LogUnavailable = TRUE;
    } else {
      EventLogSize += ReceivedLogSize;
      if (RemainingEntries) {
        if (EventLogSize >= MAX_EVENTLOG_SIZE) {
          DEBUG ((DEBUG_ERROR, "Event Log is exceeding the Maximum allocated buffer\n"));
          return EFI_BUFFER_TOO_SMALL;
        }

        TempEvent += ReceivedLogSize;
        RemainingBufSize = MAX_EVENTLOG_SIZE - EventLogSize;
      }
    }
  } while (RemainingEntries != 0);

  EventLogSize += sizeof (CsmeEvent.Signature) + sizeof (CsmeEvent.ERHashAlgorithm);
  HashDataLen = (Index * sizeof (UINT32));

  mCsmeInfoFlags.LogUnavailable = (LogUnavailable == TRUE)? 1 : 0;

  //If ER Logs Unavailable then Events should be set to zero.
  if (mCsmeInfoFlags.LogUnavailable) {
    ZeroMem (&CsmeEvent, sizeof (CsmeEvent));
    CopyMem (&(CsmeEvent.Signature), Signature, AsciiStrLen (Signature));
    CsmeEvent.ERHashAlgorithm = HashAlgorithm;
  }

  //If Untrusted Measurement or FDO is Asserted then Events and HER values should be set to zero.
  if (mCsmeInfoFlags.UntrustedMeasurement || mCsmeInfoFlags.FdoInvalidMeasurement) {
    ZeroMem (&CsmeEvent, sizeof (CsmeEvent));
    CopyMem (&(CsmeEvent.Signature), Signature, AsciiStrLen (Signature));
    ZeroMem (HerValue, HashDataLen);
  }

  //
  // Measure to PCR[0] with event EV_NONHOST_INFO
  //
  Status = TpmMeasureAndLogData (
             0,
             EV_NONHOST_INFO,
             &CsmeEvent,
             EventLogSize,   // Actual received event log size + signature length + Algorithm size
             HerValue,
             HashDataLen
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Perform CSME Info measurement.

  @retval EFI_SUCCESS             Measurement performed
**/
EFI_STATUS
MeasureInfo (
  VOID
  )
{
  EFI_STATUS                    Status;
  CSME_INFO_MEASUREMENT         CsmeInfoEvent;
  static CONST CHAR8            Info_Signature[] = "IntelCSxEInfoEvent";

  ZeroMem (&CsmeInfoEvent, sizeof (CsmeInfoEvent));
  CopyMem (&(CsmeInfoEvent.Signature), Info_Signature, AsciiStrLen (Info_Signature));

  CsmeInfoEvent.Version  = CSME_INFO_VERSION;
  CsmeInfoEvent.VendorID = V_PCH_INTEL_VENDOR_ID;
  CsmeInfoEvent.DeviceID = PciSegmentRead16 (MeHeciPciCfgBase (HECI1) + PCI_DEVICE_ID_OFFSET);
  CsmeInfoEvent.Flags    = mCsmeInfoFlags;

  //
  // Measure to PCR[0] with event EV_NONHOST_INFO
  //
  Status = TpmMeasureAndLogData (
             0,
             EV_NONHOST_INFO,
             &CsmeInfoEvent,
             sizeof (CsmeInfoEvent),
             &CsmeInfoEvent,
             sizeof (CsmeInfoEvent)
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  This function gets sha384 CSME Measured data from HECI1 MMIO space (HECI1_HER1 - HECI1_HER12).

  @param[in, out]      Her             Pointer to receive HER Data buffer of 12 DWORDS
  @param[in]           HerOffset       Offset of first HER register in MMIO Space
  @param[in]           HerMaxRegCount  Max Register count to read HER registers

  @retval EFI_SUCCESS                  Event data read successfully
  @retval EFI_INVALID_PARAMETER        Input parameters are not valid
  @retval EFI_DEVICE_ERROR             HECI Device error, command aborts abnormally
  @retval EFI_NOT_READY                Device is not ready
**/
EFI_STATUS
HeciGetHerDataSha384 (
  IN OUT UINT32    *Her,
  IN     UINT32    HerOffset,
  IN     UINT8     HerMaxRegCount
  )
{
  UINT64              HeciBaseAddress;
  UINT64              MemBar;
  UINTN               HeciMemBar;
  UINT8               Index;

  if (Her == NULL) {
    DEBUG ((DEBUG_ERROR, "HeciGetHerDataSha384: Invalid parameter\n"));
    return EFI_INVALID_PARAMETER;
  }

  ///
  /// Get HECI Device MMIO address
  ///
  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  if (PciSegmentRead16 (HeciBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "HECI device is disabled\n"));
    return EFI_DEVICE_ERROR;
  }

  MemBar = PciSegmentRead32 (HeciBaseAddress + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;
  if ((PciSegmentRead32 (HeciBaseAddress + PCI_BASE_ADDRESSREG_OFFSET) & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
    MemBar += LShiftU64 (PciSegmentRead32 (HeciBaseAddress + (PCI_BASE_ADDRESSREG_OFFSET + 4)), 32);
  }

  if (MemBar == 0) {
    DEBUG ((DEBUG_WARN, "MMIO Bar for HECI device isn't programmed\n"));
    return EFI_DEVICE_ERROR;
  }
  HeciMemBar = (UINTN) MemBar;

  ///
  /// Check whether HECI device is in D0 State
  ///
  if ((MmioRead32 (HeciMemBar + R_ME_MEM_DEVIDLEC) & B_ME_MEM_DEVIDLEC_DEVIDLE) == B_ME_MEM_DEVIDLEC_DEVIDLE) {
    DEBUG ((DEBUG_ERROR, "HeciGetHerDataFromMmio Error: HECI Device is in D0I3 state, not recommended to read HER data\n"));
    return EFI_NOT_READY;
  }

  ///
  /// Read HER data from MMIO space
  ///
  for (Index = 0; Index < HerMaxRegCount; Index++) {
    *Her = MmioRead32 (HeciMemBar + (HerOffset + (Index * sizeof (UINT32))));
    Her++;
  }

  return EFI_SUCCESS;
}

/**
  Get Extended Measurement data from CSME Extended registers.

  @param[in] Her               Pointer to receive HER Data buffer
  @param[in] HashAlgorithm     Hash Algorithm
  @param[in] HerMaxRegCount    DWord registers count which has Hash data

  @retval EFI_SUCCESS             Successfully read Measurement data
  @retval EFI_INVALID_PARAMETER   Input parameters are not valid
  @retval EFI_UNSUPPORTED         Unsupported Hash Algorithm
**/
EFI_STATUS
HeciGetHerData (
  IN UINT32    *Her,
  IN UINT8     HashAlgorithm,
  IN UINT8     HerMaxRegCount
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  UINT32        HerOffset;
  UINT8         Index;

  if ((Her == NULL) || (HashAlgorithm == 0) || (HerMaxRegCount == 0)) {
    DEBUG ((DEBUG_ERROR, "HeciGetHerData: Invalid parameter\n"));
    return EFI_INVALID_PARAMETER;
  }

  HerOffset = (UINT32) R_ME_CFG_HER1;

  switch (HashAlgorithm) {
    case V_ME_SHA_256:
      ///
      /// Read HER data from PCI Config space 0xC0 - 0xDF (R_ME_CFG_HER1 - R_ME_CFG_HER8)
      ///
      for (Index = 0; Index < HerMaxRegCount; Index++) {
        *Her = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + (HerOffset + (Index * sizeof (UINT32))));
        Her++;
      }
      break;

  case V_ME_SHA_384:
    ///
    /// Read HER data from MMIO space 0xCC0 - 0xCDF (R_ME_MEM_HERS1 - R_ME_MEM_HERS12)
    ///
    HerOffset += R_ME_MEM_HER_BASE;
    Status = HeciGetHerDataSha384 (Her, HerOffset, HerMaxRegCount);
    break;

  default:
    DEBUG ((DEBUG_ERROR, "HeciGetHerData: Failed to get CSME Measurement data\n"));
    Status = EFI_UNSUPPORTED;
    break;
  }

  return Status;
}

/**
  End of DXE CallBack event to extend CSME Measurment to TPM-PCR[0] & AMT Config data to TPM-PCR[1]

  @param[in] Event      The event that triggered this notification function
  @param[in] Context    Pointer to the notification functions context

**/
VOID
EFIAPI
ExtendCsmeMeasurementToTpmPcr (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS    Status;
  UINT32        Her[HER_REG_COUNT_SHA384];
  UINT32        Hers;
  UINT32        EnhHers;
  UINT8         HashAlgorithm;
  UINT8         HerMaxRegCount;
  UINT64        HeciBaseAddress;
  UINTN         HeciMemBar;
  UINT8         TimeOut;

  gBS->CloseEvent (Event);
  ///
  /// Get HECI Device MMIO address
  ///
  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  if (PciSegmentRead16 (HeciBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "HECI device is disabled\n"));
    return;
  }

  ///
  /// Read HER Status register value
  ///
  Hers = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HERS);

  HeciMemBar = PciSegmentRead32 (HeciBaseAddress + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;
  if ((PciSegmentRead32 (HeciBaseAddress + PCI_BASE_ADDRESSREG_OFFSET) & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
    HeciMemBar += LShiftU64 (PciSegmentRead32 (HeciBaseAddress + (PCI_BASE_ADDRESSREG_OFFSET + 4)), 32);
  }

  if (HeciMemBar == 0) {
    DEBUG ((DEBUG_WARN, "MMIO Bar for HECI device isn't programmed\n"));
    return;
  }

  ///
  /// Enable HECI MSE
  ///
  PciSegmentOr8 (HeciBaseAddress + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  ///
  /// Check whether HECI device is in D0 State
  ///
  if ((MmioRead32 (HeciMemBar + R_ME_MEM_DEVIDLEC) & B_ME_MEM_DEVIDLEC_DEVIDLE) == B_ME_MEM_DEVIDLEC_DEVIDLE) {
    DEBUG ((DEBUG_ERROR, "HeciGetHerDataFromMmio Error: HECI Device is in D0I3 state, not recommended to read HER data\n"));
    return;
  }
  EnhHers = MmioRead32 (HeciMemBar + R_ME_MEM_HERS);

  ZeroMem (&mCsmeInfoFlags, sizeof (mCsmeInfoFlags));
  if (EnhHers & B_ME_EXTEND_FEAT_PRESENT) {
    mCsmeInfoFlags.HwRot = 1;
    if ((EnhHers & B_ME_EXTEND_COMPLETE) != B_ME_EXTEND_COMPLETE) {
      // If ERC bit is not set then BIOS shall poll the ERC bit status for 100 ms timeout value
      // with delay interval of 1 ms for each time.
      mCsmeInfoFlags.InvalidMeasurement = 1;
      for (TimeOut = 0; TimeOut < ERC_POLL_TIMEOUT; TimeOut++) {
        MicroSecondDelay (1000);
        EnhHers = MmioRead32 (HeciMemBar + R_ME_MEM_HERS);
        if ((EnhHers & B_ME_EXTEND_COMPLETE) == B_ME_EXTEND_COMPLETE) {
          mCsmeInfoFlags.InvalidMeasurement = 0;
          break;
        }
      }
      if (mCsmeInfoFlags.InvalidMeasurement) {
        DEBUG ((DEBUG_INFO, "CSME Extend Measurement Complete (ERC) bit is not set after polling for 100ms!\n"));
      }
    }
  } else {
    mCsmeInfoFlags.HwRot = 0;
    if ((Hers & B_ME_EXTEND_REG_VALID) != B_ME_EXTEND_REG_VALID) {
      mCsmeInfoFlags.InvalidMeasurement = 1;
      DEBUG ((DEBUG_INFO, "CSME Extend Measurement Valid bit is not set!\n"));
    }
  }

  ZeroMem (&Her, sizeof (Her));
  HashAlgorithm = 0;
  HerMaxRegCount = 0;

  if ((EnhHers & B_ME_EXTEND_REG_ALGORITHM) == V_ME_SHA_384) {
    HashAlgorithm = V_ME_SHA_384;
    HerMaxRegCount = (UINT8) HER_REG_COUNT_SHA384;
  } else if ((EnhHers & B_ME_EXTEND_REG_ALGORITHM) == V_ME_SHA_256) {
    HashAlgorithm = V_ME_SHA_256;
    HerMaxRegCount = (UINT8) HER_REG_COUNT_SHA256;
  }

  mCsmeInfoFlags.InvalidState = (mMeInvalidStateFlag == TRUE)? 1 : 0;
  mCsmeInfoFlags.UntrustedMeasurement = ((EnhHers & B_ME_DSP) == 0)? 0 : 1;

  if (SpiIsFlashDescriptorOverrideEnabled ()) {
    mCsmeInfoFlags.FdoInvalidMeasurement = 1;
    DEBUG ((DEBUG_INFO, "FDO is asserted\n"));
  } else {
    mCsmeInfoFlags.FdoInvalidMeasurement = 0;
  }

  if ((mCsmeInfoFlags.HwRot != 0) && (mCsmeInfoFlags.InvalidMeasurement != 1)) {
    Status = HeciGetHerData (Her, HashAlgorithm, HerMaxRegCount);
    if (!EFI_ERROR (Status)) {
      Status = MeasureHer (Her, HerMaxRegCount, HashAlgorithm);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Extending ME Measurement to TPM-PCR0 failed, Status is %r \n", Status));
      }
    }
  }

  Status = MeasureInfo ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Extending ME Info to TPM-PCR0 failed, Status is %r \n", Status));
  }

}

/**
  Performs Global Reset.
**/
VOID
MePerformGlobalReset (
  VOID
  )
{
  PCH_RESET_DATA             ResetData;

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  ZeroMem (&ResetData, sizeof (ResetData));
  CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
  StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);

  gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
  CpuDeadLoop ();
}

/**
  Return BIOS setup option status of Extended CSME Measured Boot.

  @retval FALSE      Extend CSME Measured Boot is disabled in Setup.
  @retval TRUE       Extend CSME Measured Boot is enabled in Setup.
**/
BOOLEAN
IsExtendedMeasuredBootEnabled (
  VOID
  )
{
  EFI_STATUS  Status;
  ME_SETUP    MeSetup;
  UINTN       Size;

  Size = sizeof (ME_SETUP);
  Status = gRT->GetVariable (L"MeSetup", &gMeSetupVariableGuid, NULL, &Size, &MeSetup);
  if (!EFI_ERROR (Status) && MeSetup.ExtendMeMeasuredBoot) {
    return TRUE;
  }

  return FALSE;
}

/**
  Entry point for Me Extended Measurement driver

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_UNSUPPORTED         ME policy library initialization failure.
  @retval Others                  Status code returned by CreateEventEx.
**/
EFI_STATUS
EFIAPI
MeExtMeasurementEntryPoint (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS              Status;
  BOOLEAN                 HostSetupMbEnabled;
  BOOLEAN                 CsmeMbState;
  MEFWCAPS_SKU            MeFwCapsSku;
  BOOLEAN                 IsAmtSupport;
  UINT8                   SetMeasureBoot;
  UINT8                   GetMeasureBoot;
  EFI_EVENT               Event;
  ME_BIOS_PAYLOAD_HOB     *MbpHob;
  AMT_PEI_CONFIG          *AmtPeiConfig;
  EFI_PEI_HOB_POINTERS    HobPtr;
  UINT64                  HeciBaseAddress;
  HECI_FWS_REGISTER       MeFwsts1;

  Status = EFI_SUCCESS;
  MbpHob = NULL;
  CsmeMbState = FALSE;
  IsAmtSupport = FALSE;
  HostSetupMbEnabled = FALSE;
  AmtPeiConfig = NULL;

  //
  // Get BIOS Setup value for Extended CSME Measurement
  //
  HostSetupMbEnabled = IsExtendedMeasuredBootEnabled ();

  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  if (PciSegmentRead16 (HeciBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "HECI device is disabled\n"));
    return EFI_UNSUPPORTED;
  }

  // Read ME status and check for CSME operation mode
  MeFwsts1.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS);

  // Check whether CSME is in Disabled mode
  if ((MeFwsts1.r.MeOperationMode != ME_OPERATION_MODE_NORMAL)) {
    DEBUG ((DEBUG_INFO, "%a: CSME is in not in Normal Mode.\n", __FUNCTION__));

    if (HostSetupMbEnabled == FALSE) {
      DEBUG ((DEBUG_INFO, "CSME Measured Boot is Disabled on Host Side. No Action Required, Continue to boot...\n"));
      return Status;
    }

    //
    // Extend CSME Measurement data to TPM PCR[0]
    //
    DEBUG ((DEBUG_INFO, "Register End Of Dxe event to extended CSME Measured data to TPM-PCR[0] for CSME not in Normal Mode\n"));
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    ExtendCsmeMeasurementToTpmPcr,
                    NULL,
                    &gEfiEndOfDxeEventGroupGuid,
                    &Event
                    );
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Get Mbp Data HOB
  //
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Mbp Hob not found\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  //
  // Read MBP Table and check for CSME Measured Boot Support (MBP_ITEM_ID_MEASURED_BOOT  Bit 0) & AMT Support (MBP_KERNEL_FW_CAPS_ITEM Bit 2)
  //
  if (MbpHob->MeBiosPayload.MeasuredBootSupport.Available == TRUE) {
    CsmeMbState = (MbpHob->MeBiosPayload.MeasuredBootSupport.MeasuredBootData.MeasuredBoot == 1) ? TRUE : FALSE;
  } else {
    DEBUG ((DEBUG_ERROR, "%a: Csme not supported Measured Boot\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  if (MbpHob->MeBiosPayload.FwCapsSku.Available == TRUE) {
    MeFwCapsSku.Data = MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Data;
    IsAmtSupport = (MeFwCapsSku.Fields.Amt == 1) ? TRUE : FALSE;
  }

  HobPtr.Guid = GetFirstGuidHob (&gAmtPolicyHobGuid);
  if (HobPtr.Guid != NULL) {
    AmtPeiConfig = (AMT_PEI_CONFIG *) GET_GUID_HOB_DATA (HobPtr.Guid);
    if (AmtPeiConfig->AmtEnabled == 0) {
      DEBUG ((DEBUG_INFO, "%a: BIOS - AMT Policy is not Enabled\n", __FUNCTION__));
    }
  }

  //
  // 1. If Measured Boot is enabled on both CSME & Host side then register End of DXE event to extend CSME Measurement from ER Registers to TPM-PCR[0]
  // 2. Else if Host side Measured Boot is not in sync with CSME Side Measurement Boot, then Send MEI Command to CSME to enable/disable as per Host
  //    side Measured Boot State.
  //    Note: If CSME enables Measured Boot then issue Global reset to capture Measured data in next restart,
  //          else if CSME disables Measured Boot then No need to issue Global reset as Host is not going to update TPM PCR.
  // 3. If Measured Boot is disabled on both CSME & Host side then No action is required continue to Boot.
  //
  if ((CsmeMbState & HostSetupMbEnabled) == TRUE) {
    //
    // Extend CSME Measured data to TPM PCR[0]
    //
    DEBUG ((DEBUG_INFO, "Register End Of Dxe event to extended CSME Measured data to TPM-PCR[0]\n"));
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    ExtendCsmeMeasurementToTpmPcr,
                    NULL,
                    &gEfiEndOfDxeEventGroupGuid,
                    &Event
                    );
    ASSERT_EFI_ERROR (Status);

  } else if (CsmeMbState != HostSetupMbEnabled) {
    if (HostSetupMbEnabled == TRUE) {
      //
      // Send Command to CSME to enable Extended Measured boot
      // Issue Global Reset
      //
      DEBUG ((DEBUG_INFO, "Send CSME command to Enable Measured Boot\n"));
      SetMeasureBoot = 1;
      Status = HeciSetMeasuredBootState (SetMeasureBoot);
      if (!EFI_ERROR (Status)) {
        GetMeasureBoot = 0;
        Status = HeciGetMeasuredBootState (&GetMeasureBoot);
        if (!EFI_ERROR (Status) && (GetMeasureBoot == 1)) {
          // CSME Successfully enabled Measured Boot. Issue Global Reset
          DEBUG ((DEBUG_INFO, "CSME Successfully Enable Measured Boot, Issuing Global Reset\n"));
          MePerformGlobalReset ();
        } else {
          DEBUG ((DEBUG_INFO, "CSME failed to HeciGetMeasuredBootState\n"));
          mMeInvalidStateFlag = TRUE;
        }
      } else {
        DEBUG ((DEBUG_INFO, "CSME failed for HeciSetMeasuredBootState to enable MB\n"));
        mMeInvalidStateFlag = TRUE;
      }
    } else {
      //
      // Send Command to CSME to disable Extended Measured boot
      // Issue Global Reset
      //
      DEBUG ((DEBUG_INFO, "Send CSME command to Disable Measured Boot\n"));
      SetMeasureBoot = 0;
      Status = HeciSetMeasuredBootState (SetMeasureBoot);
      if (!EFI_ERROR (Status)) {
        GetMeasureBoot = 1;
        Status = HeciGetMeasuredBootState (&GetMeasureBoot);
        if (!EFI_ERROR (Status) && (GetMeasureBoot == 0)) {
          // CSME Successfully disabled Measured Boot. Issue Global Reset.
          DEBUG ((DEBUG_INFO, "CSME Successfully disabled Measured Boot, Issuing Global Reset\n"));
          MePerformGlobalReset ();
        } else {
          DEBUG ((DEBUG_INFO, "CSME failed to HeciGetMeasuredBootState\n"));
        }
      } else {
        DEBUG ((DEBUG_INFO, "CSME failed for HeciSetMeasuredBootState to disable MB\n"));
      }
    }
  } else {
    //
    // MbCsmeSupport & MbHostSetupEnabled are FALSE that means Measured Boot is disabled on both CSME & Host side.
    // Continue to Boot
    //
    DEBUG ((DEBUG_INFO, "Extended CSME Measured Boot is disabled on both CSME & Host side, NO Action required, Continue to boot...\n"));
  }

  //
  // If AMT is supported, then get AMT Config data from CSME and push to TPM-PCR[1]
  //
  if ((AmtPeiConfig != NULL) && (AmtPeiConfig->AmtEnabled == 1) && (IsAmtSupport == TRUE)) {
    DEBUG ((DEBUG_INFO, "AMT is supported, push AMT Config to TPM-PCR[1]\n"));

    ///
    /// Create a Ready to Boot event.
    ///
    Status = EfiCreateEventReadyToBootEx (
               TPL_CALLBACK,
               AmtConfigReadyToBootEvent,
               (VOID *) &ImageHandle,
               &Event
               );
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "%a: Exit - Success.\n", __FUNCTION__));

  return Status;
}
