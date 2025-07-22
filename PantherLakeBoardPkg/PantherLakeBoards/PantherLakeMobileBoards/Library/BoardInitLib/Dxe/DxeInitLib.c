/** @file

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

#include <Uefi.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeUpdatePlatformInfoLib.h>
#include <Library/SetupInitLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BoardConfigLib.h>
#include <Library/IoLib.h>
#include <Library/PmcLib.h>
#include <Library/EcMiscLib.h>
#include <Library/PcdItssLib.h>
#include <MemInfoHob.h>
#include <PlatformBoardConfig.h>
#include <PlatformBoardId.h>
#include <Pins/GpioPinsVer2Lp.h>
#include <PcieRegs.h>
#include <Register/PchRegs.h>
#include <Register/PchRegs.h>
#include <Register/EspiRegs.h>
#include <Register/PmcRegs.h>
#include <Register/RtcRegs.h>
#include <Protocol/PlatformNvsArea.h>
#include <Library/MtrrLib.h>
#include <Protocol/MpService.h>
#include <Library/SiMtrrLib.h>
#include <Library/UefiLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/S3BootScriptLib.h>

EFI_STATUS
EFIAPI
RegisterDisplayDimmPopulationErrMsg(
  VOID
  );

VOID
EFIAPI
DisplayDimmPopulationErrMsg(
  IN EFI_EVENT Event,
  IN VOID* Context
  );

STATIC
EFI_STATUS
BoardHookPlatformSetup (
  VOID * Content
  )
{
  ((SETUP_VOLATILE_DATA *)Content)->PlatId = PcdGet16 (PcdBoardId);

  return RETURN_SUCCESS;
}

/**
  Register DisplayDIMMPopulationErrMsg Notification function on SimpleIn protocol Installation.
  @param[in] SystemTable    Pointer to the System Table
  @retval  EFI_SUCCESS      return EFI_STATUS as success
**/
EFI_STATUS
EFIAPI
RegisterDisplayDimmPopulationErrMsg(
  VOID
  )
{
  EFI_EVENT DimmPopultionErrMsgEvent;
  VOID* Registration;

  DimmPopultionErrMsgEvent = NULL;

  //
  // Create event for SimpleInProtocol Callback notification
  //
  gBS->CreateEvent(
        EVT_NOTIFY_SIGNAL,
        TPL_CALLBACK,
        DisplayDimmPopulationErrMsg,
        NULL,
        &DimmPopultionErrMsgEvent
        );
  gBS->RegisterProtocolNotify(
        &gBdsAllDriversConnectedProtocolGuid,
        DimmPopultionErrMsgEvent,
        &Registration
        );

  return EFI_SUCCESS;
}

/**
  Updates DIMM slots status for Desktop,server and workstation boards

**/
VOID
UpdateDimmPopulation(
  VOID
  )
{
  if (PcdGetBool(PcdDimmPopulationError)) {
    RegisterDisplayDimmPopulationErrMsg();
  }
}

/**
  Display DIMM population error message, while Wrong DIMM Population is found.
  @param[in] Event    A pointer to the Event that triggered the callback
  @param[in] Context  A pointer to private data registered with the callback function.
  @retval  EFI_SUCCESS     If Successfully exectuted else returns the error status
**/
VOID
EFIAPI
DisplayDimmPopulationErrMsg(
  IN EFI_EVENT Event,
  IN VOID* Context
  )
{
  EFI_STATUS                Status;
  EFI_INPUT_KEY             Key;

  DEBUG((DEBUG_ERROR, "\n DIMMPopulation Error msg\n"));

  if (gST->ConOut != NULL && gST->ConIn != NULL) {
    if (Event != NULL) {
      gBS->CloseEvent(Event);
    }
    //
    // Display the Error Message
    //
    gST->ConOut->OutputString(gST->ConOut, L"");

    gST->ConOut->OutputString(gST->ConOut, L"Error...! Invalid DIMM Population.");

    gST->ConOut->OutputString(gST->ConOut, L"Slot 0 can't be populated when Slot 1 is not populated on the same channel");

    gST->ConOut->OutputString(gST->ConOut, L"Press <Y> to Continue. <N> to Shutdown");

    //
    // Wait for User Input
    //
    do {
      Status = gBS->CheckEvent(gST->ConIn->WaitForKey);
      if (!EFI_ERROR(Status)) {
        gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
        if (Key.UnicodeChar == L'Y' || Key.UnicodeChar == L'y') {
          DEBUG((DEBUG_INFO, " Boot Continue\n"));
          break;
        } else if (Key.UnicodeChar == L'N' || Key.UnicodeChar == L'n') {
          DEBUG((DEBUG_ERROR, " System shutdown\n"));
          gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
        }
      }
    } while (1);
  }
}

/**
  Update memory space attributes.
**/
EFI_STATUS
EFIAPI
UpdateMemorySpaceAttributes(
  )
{
  EFI_STATUS                            Status;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR       GcdDescriptor;
  EFI_PHYSICAL_ADDRESS                  BaseAddress;
  UINT64                                Length;

  BaseAddress = PcdGet64(PcdPciExpressBaseAddress);;
  Length = PcdGet32(PcdPciExpressRegionLength);

  Status = gDS->GetMemorySpaceDescriptor(BaseAddress, &GcdDescriptor);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "UpdateMemorySpaceAttributes failed to get memory space descriptor.\n"));
  } else {
    Status = gDS->SetMemorySpaceAttributes(
                    BaseAddress,
                    Length,
                    GcdDescriptor.Attributes | EFI_MEMORY_RUNTIME
                    );
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "UpdateMemorySpaceAttributes failed to add EFI_MEMORY_RUNTIME attribute.\n"));
    }
  }

  return Status;
}

VOID
PmcDisableAndClearSmiWithS3BootScript (
  VOID
  )
{
  UINT16  ABase;
  UINT32  SmiEn;
  UINT16  Pm1Sts;

  ABase = PmcGetAcpiBase ();
  SmiEn = IoRead32 (ABase + R_ACPI_IO_SMI_EN);

  //
  // Disabled Periodic SMI and Software SMI Timer during S3 Resume.
  //
  SmiEn &= ~(B_ACPI_IO_SMI_EN_PERIODIC_EN | B_ACPI_IO_SMI_EN_SWSMI_TMR_EN);

  S3BootScriptSaveIoWrite (
    S3BootScriptWidthUint32,
    (UINTN) ABase + R_ACPI_IO_SMI_EN,
    1,
    &SmiEn
    );

  //
  // Clear Bus Master Status bit during S3 Resume
  //
  Pm1Sts = B_ACPI_IO_PM1_STS_BM;

  S3BootScriptSaveIoWrite (
    S3BootScriptWidthUint16,
    (UINTN) ABase + R_ACPI_IO_PM1_STS,
    1,
    &Pm1Sts
    );
}

/**
 Init Misc Platform Board Config Block.

 @param[in]  VOID

 @retval VOID
**/
VOID
PtlBoardMiscInit (
  VOID
)
{
  PcdSet64S (PcdFuncBoardHookPlatformSetupOverride, (UINT64) (UINTN) BoardHookPlatformSetup);

  //
  // Update DIMM funtionalities for desktop and server boards
  //
  UpdateDimmPopulation ();

  UpdateMemorySpaceAttributes ();

  PmcDisableAndClearSmiWithS3BootScript ();
}

VOID
PtlBoardSmbiosInit (
  VOID
  )
{
  PcdSet64S (PcdSmbiosFabBoardName, (UINTN) PcdGetPtr (PcdBoardName));
  //
  // @todo : Update Slot Entry Table for all the supported boards using below PCD.
  //  This PCD follows SYSTEM_SLOT_ENTRY structure in \Include\SmbiosPlatformInfoDefinition.h
  //PcdSet64S (PcdSmbiosMainSlotEntry, NULL);
}

VOID
PtlUpdateDimmPopulation (
  VOID
  )
{
  MEMORY_INFO_DATA_HOB    *MemInfo;
  UINT8                   Slot0;
  UINT8                   Slot1;
  UINT8                   Slot2;
  UINT8                   Slot3;
  CONTROLLER_INFO         *ControllerInfo;
  EFI_HOB_GUID_TYPE       *GuidHob;

  GuidHob = NULL;
  MemInfo = NULL;
  GuidHob = GetFirstGuidHob (&gSiMemoryInfoDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob != NULL) {
    MemInfo = (MEMORY_INFO_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
  }
  if (MemInfo != NULL) {
    if ( PcdGet8 (PcdPlatformFlavor) == FlavorDesktop ||
         PcdGet8 (PcdPlatformFlavor) == FlavorUpServer ||
         PcdGet8 (PcdPlatformFlavor) == FlavorWorkstation
        ) {
        ControllerInfo = &MemInfo->Controller[0];
        Slot0 = ControllerInfo->ChannelInfo[0].DimmInfo[0].Status;
        Slot1 = ControllerInfo->ChannelInfo[0].DimmInfo[1].Status;
        Slot2 = ControllerInfo->ChannelInfo[1].DimmInfo[0].Status;
        Slot3 = ControllerInfo->ChannelInfo[1].DimmInfo[1].Status;

      //
      // Channel 0          Channel 1
      // Slot0   Slot1      Slot0   Slot1      - Population            AIO board
      // 0          0          0          0          - Invalid        - Invalid
      // 0          0          0          1          - Valid          - Invalid
      // 0          0          1          0          - Invalid        - Valid
      // 0          0          1          1          - Valid          - Valid
      // 0          1          0          0          - Valid          - Invalid
      // 0          1          0          1          - Valid          - Invalid
      // 0          1          1          0          - Invalid        - Invalid
      // 0          1          1          1          - Valid          - Invalid
      // 1          0          0          0          - Invalid        - Valid
      // 1          0          0          1          - Invalid        - Invalid
      // 1          0          1          0          - Invalid        - Valid
      // 1          0          1          1          - Invalid        - Valid
      // 1          1          0          0          - Valid          - Valid
      // 1          1          0          1          - Valid          - Invalid
      // 1          1          1          0          - Invalid        - Valid
      // 1          1          1          1          - Valid          - Valid
      //
      if ((Slot0 && (Slot1 == 0)) || (Slot2 && (Slot3 == 0))) {
        PcdSetBoolS (PcdDimmPopulationError, TRUE);
      }
    }
  }
}

/**
  Enable Tier2 GPIO Sci wake capability.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
PtlTier2GpioWakeSupport (
  VOID
  )
{
  BOOLEAN Tier2GpioWakeEnable;

  Tier2GpioWakeEnable = FALSE;

  //
  // Root port #03: M.2 WLAN
  // @todo port as per board doc
//  if (IsPcieEndPointPresent (2)) {
//   Tier2GpioWakeEnable = TRUE;
//  }
  PcdSetBoolS (PcdGpioTier2WakeEnable, Tier2GpioWakeEnable);

  return EFI_SUCCESS;
}

/**
  A hook for board-specific initialization after PCI enumeration.

  @retval EFI_SUCCESS   The board initialization was successful.
**/
EFI_STATUS
EFIAPI
PtlBoardInitAfterPciEnumeration (
  VOID
  )
{
  PtlTier2GpioWakeSupport ();

  PtlBoardMiscInit();

  PtlBoardSmbiosInit ();

  UpdatePlatformInfo ();

  return EFI_SUCCESS;
}

/**
  A minimal wrapper function that allows MtrrSetAllMtrrs () to be passed to
  EFI_MP_SERVICES_PROTOCOL.StartupAllAPs () as Procedure.

  @param[in] Buffer  Pointer to an MTRR_SETTINGS object, to be passed to
                     MtrrSetAllMtrrs ().
**/
VOID
EFIAPI
SetMtrrsFromBuffer (
  IN VOID *Buffer
  )
{
  MtrrSetAllMtrrs (Buffer);
}

/**
  This function handles Update settings of MTRRs.

**/
VOID
EFIAPI
MtrrSettingsUpdate (
  VOID
  )
{
  EFI_STATUS                Status;
  MTRR_SETTINGS             MtrrSetting;
  EFI_MP_SERVICES_PROTOCOL  *MpService;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Status = MtrrTransfer2DefaultWB (&MtrrSetting);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Synchronize the update with all APs
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **)&MpService
                  );
  if (!EFI_ERROR (Status)) {
    MtrrGetAllMtrrs (&MtrrSetting);
    Status = MpService->StartupAllAPs (
                         MpService,          // This
                         SetMtrrsFromBuffer, // Procedure
                         FALSE,              // SingleThread
                         NULL,               // WaitEvent
                         0,                  // TimeoutInMicrosecsond
                         &MtrrSetting,       // ProcedureArgument
                         NULL                // FailedCpuList
                         );
    if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
      DEBUG ((DEBUG_ERROR, "SetMtrrsFromBuffer: StartupAllAps(): %r\n", Status));
      ASSERT (FALSE);
    }
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return;
}

/**
  A hook for board-specific functionality for the ReadyToBoot event.

  @retval EFI_SUCCESS   The board initialization was successful.
**/
EFI_STATUS
EFIAPI
PtlBoardInitReadyToBoot (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "PtlBoardInitReadyToBoot\n"));

  PtlUpdateDimmPopulation ();

  MtrrSettingsUpdate ();

  return EFI_SUCCESS;
}

/**
  Handler to enable ACPI mode

  Clear Port 80h

  Dispatched on reads from APM port with value EFI_ACPI_ENABLE_SW_SMI

  Disables the SW SMI Timer.
  ACPI events are disabled and ACPI event status is cleared.
  SCI mode is then enabled.

  Clear SLP SMI status
  Enable SLP SMI

  Disable SW SMI Timer

  Clear all ACPI event status and disable all ACPI events

  Disable PM sources except power button
  Clear status bits

  Disable GPE0 sources
  Clear status bits

  Disable GPE1 sources
  Clear status bits

  Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)

  Enable SCI

  Set 8254 timer clock gate enable if it's not set in early phase

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
EnableAcpiCallback (
  VOID
  )
{
  UINT32                              OutputValue;
  UINT32                              Pm1Cnt;
  EFI_STATUS                          Status;
  UINT8                               mEcPresent;
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  UINT8                               mPlatformFlavor = FlavorUnknown;
#endif
  UINT16                              mAcpiBaseAddr;
  UINTN                               DataSize;
  PLATFORM_NVS_AREA_PROTOCOL          *PlatformNvsAreaProtocol;
  PLATFORM_NVS_AREA                   *mPlatformNvsAreaPtr = 0;
  PCH_SETUP                           mPchSetup;

  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "EnableAcpiCallback entry\n"));
  //
  // Clear Port 80h
  //
  OutputValue = 0;
  IoWrite16 (0x80, (UINT16)OutputValue);

  DataSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  NULL,
                  &DataSize,
                  &mPchSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Get the ACPI Base Address
  //
  mAcpiBaseAddr = PmcGetAcpiBase ();
  //
  // Disable PM sources except power button
  //
  OutputValue = B_ACPI_IO_PM1_EN_PWRBTN;
  IoWrite16 (mAcpiBaseAddr + R_ACPI_IO_PM1_EN, (UINT16)OutputValue);

  //
  // Clear PM status except Power Button status for RapidStart Resume
  //
  OutputValue = 0xFEFF;
  IoWrite16 (mAcpiBaseAddr + R_ACPI_IO_PM1_STS, (UINT16)OutputValue);

  //
  // Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)
  //
  OutputValue = R_RTC_IO_REGISTER_D;
  IoWrite8 (R_RTC_IO_INDEX_ALT, (UINT8)OutputValue);
  OutputValue = 0x0;
  IoWrite8 (R_RTC_IO_TARGET_ALT, (UINT8)OutputValue);

  //
  // Enable SCI
  //
  Pm1Cnt = IoRead32 (mAcpiBaseAddr + R_ACPI_IO_PM1_CNT);
  Pm1Cnt |= B_ACPI_IO_PM1_CNT_SCI_EN;
  IoWrite32 (mAcpiBaseAddr + R_ACPI_IO_PM1_CNT, Pm1Cnt);

  mEcPresent      = PcdGetBool (PcdEcPresent);
  if (mEcPresent == 1) {
    //
    // Check the power state from EC and update it to global NVS area
    //
    //
    // Locate our shared data area
    //
    Status = gBS->LocateProtocol (&gPlatformNvsAreaProtocolGuid, NULL, (VOID **) &PlatformNvsAreaProtocol);
    ASSERT_EFI_ERROR (Status);
    mPlatformNvsAreaPtr = PlatformNvsAreaProtocol->Area;
    if (PowerStateIsAc ()) {
      mPlatformNvsAreaPtr->PowerState = 1;
    } else {
      mPlatformNvsAreaPtr->PowerState = 0;
    }

    //
    // Disable SMI mode
    //
    Status = DisableEcSmiMode ();
    ASSERT_EFI_ERROR (Status);

    //
    // Commands EC to begin reading Thermal Diode and comparing to Critical Temperature.
    // Shutdown will occur when current temp equals or exceeds Critical temperature.
    //
    Status = EnableEcAcpiMode (TRUE);
    ASSERT_EFI_ERROR (Status);
  }

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  //
  // Enable embedded specific stuff
  //
  if (mPlatformFlavor == FlavorEmbedded) {
    mPlatformNvsAreaPtr->PowerState = 0;
  }
#endif
  //
  // Write ALT_GPI_SMI_EN to disable GPI1 (SMC_EXTSMI#)
  //
  OutputValue = 0;
  OutputValue = IoRead32 (mAcpiBaseAddr + 0x38);
  OutputValue = OutputValue & ~(1 << (UINTN) PcdGet8 (PcdSmcExtSmiBitPosition));
  IoWrite32 (mAcpiBaseAddr + 0x38, OutputValue);

  //
  // Set 8254 timer clock gate enable if it's not set in early phase
  //
  if ((mPchSetup.Enable8254ClockGating != 1) && (mPchSetup.PchLegacyIoLowLatency != 1)) {
    Status = PtlPcdItssSet8254ClockGateState (TRUE);
    ASSERT_EFI_ERROR (Status);
  }
  DEBUG ((DEBUG_INFO, "EnableAcpiCallback exit\n"));
  return Status;
}

/**
  A hook for board-specific functionality for the ExitBootServices event.

  @retval EFI_SUCCESS   The board initialization was successful.
**/
EFI_STATUS
EFIAPI
PtlBoardInitEndOfFirmware (
  VOID
  )
{
  EFI_STATUS Status;
  DEBUG ((DEBUG_INFO, "PtlBoardInitEndOfFirmware\n"));
  Status = EnableAcpiCallback();

  return Status;
}