/** @file
  Source code file for the Board Init Advanced DXE module

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
#include "BoardInitAdvancedDxe.h"
#include <Guid/EventGroup.h>
#include <Library/BootGuardLib.h>
#include <Protocol/CpuInfo.h>
#include <Protocol/PlatformNvsArea.h>
#include <Protocol/VariableLock.h>
#include <Library/CpuPlatformLib.h>
#include <Guid/TpmInstance.h>
#include <Library/Tpm2CommandLib.h>
#include <IntelRcStatusCode.h>
#include <Register/Cpuid.h>
#include <Protocol/Tcg2Protocol.h>
#include <Library/TpmMeasurementLib.h>
#include <Library/UefiLib.h>
#include <Protocol/ResetNotification.h>
#include <Register/GenerationMsr.h>
#include <Library/DxeServicesTableLib.h>

#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 1
#include <MipiCamConfig.h>
#endif

GLOBAL_REMOVE_IF_UNREFERENCED EFI_EVENT                         EndOfDxeEvent;
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_NVS_AREA_PROTOCOL        *mPlatformNvsAreaProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_NVS_AREA                 *mPlatformNvsAreaPtr;

#define TBT_SECURITY_EVENT_STRING      "DMA Protection Disabled"
#define TBT_SECURITY_EVENT_STRING_LEN  (sizeof(TBT_SECURITY_EVENT_STRING) - 1)

VOID
EFIAPI
PlatformResetNotificationCallback (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  );

/**
  This function handles PlatformInit task at the end of DXE

  @param[in]  Event     The Event this notify function registered to.
  @param[in]  Context   Pointer to the context data registered to the
                        Event.
**/
VOID
EFIAPI
EndofDxeCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  MSR_ANC_SACM_INFO_REGISTER        AncSacmInfoMsr;

  static BOOLEAN                    S3DataSaved = FALSE;

  gBS->CloseEvent (Event);

  if (S3DataSaved) {
    return ;
  }

  S3DataSaved = TRUE;


  ///
  /// Verify if Boot Guard is supported
  ///
  if (IsBootGuardSupported ()) {
    AncSacmInfoMsr.Uint64 = AsmReadMsr64 (MSR_ANC_SACM_INFO);
    ///
    /// Identify if Revocation is requested by Boot Guard ACM
    ///
    if (AncSacmInfoMsr.Bits.SacmData & BIT7) {
      BootGuardOemRevocationHook ();
    }
  }
}

/**
  Registers callback for.PlatformInitEndOfDxe

**/
VOID
RegisterEndOfDxeCallbacks (
  VOID
  )
{
  EFI_STATUS                     Status;

  ///
  /// Performing PlatformInitEndOfDxe after the gEfiEndOfDxeEventGroup is signaled.
  ///
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  EndofDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

}

/**
  Call back function for reset notification.

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.
**/
VOID
EFIAPI
PlatformResetNotificationCallback (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  EFI_STATUS                  Status;

  DEBUG ((DEBUG_INFO, "PlatformResetNotificationCallback\n"));

  Status = Tpm2Shutdown (TPM_SU_CLEAR);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tpm2Shutdown (TPM_SU_CLEAR) failed: %r\n", Status));
  }
}


/**
  Hook to reset notification protocol to properly reset function with TPM.
  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context
**/
VOID
EFIAPI
OnResetNotifyInstall (
  IN EFI_EVENT                      Event,
  IN VOID                           *Context
  )
{

  EFI_STATUS  Status;
  EFI_RESET_NOTIFICATION_PROTOCOL   *ResetNotify;

  Status = gBS->LocateProtocol (&gEfiResetNotificationProtocolGuid, NULL, (VOID **) &ResetNotify);
  if (!EFI_ERROR (Status)) {
    ResetNotify->RegisterResetNotify (ResetNotify, PlatformResetNotificationCallback);
    if(Event) gBS->CloseEvent (Event);
  }
}

/**
  Security EndOfDxe CallBack Function
  If the firmware/BIOS has an option to enable and disable DMA protections via a VT-d switch in BIOS options, then the shipping configuration must be with VT-d protection enabled.
  On every boot where VT-d/DMA protection is disabled, or will be disabled, or configured to a lower security state, and a platform has a TPM enabled, then the platform SHALL
  extend an EV_EFI_ACTION event into PCR[7] before enabling external DMA
  The event string SHALL be "DMA Protection Disabled". The platform firmware MUST log this measurement in the event log using the string "DMA Protection Disabled" for the Event Data.
  Measure and log launch of TBT Security, and extend the measurement result into a specific PCR.
  Extend an EV_EFI_ACTION event into PCR[7] before enabling external DMA. The event string SHALL be "DMA Protection Disabled". The platform firmware MUST log this measurement
  in the event log using the string "DMA Protection Disabled" for the Event Data.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
ExtendPCR7CallBack (
  IN EFI_EVENT          Event,
  IN VOID               *Context
  )
{
  UINTN                 Status;
  UINT64                HashDataLen;

  DEBUG ((DEBUG_INFO, "ExtendPCR7CallBack START\n"));

  //
  // When VT-d/DMA protection is disabled and a platform has a TPM enabled,
  // the platform SHALL extend an EV_EFI_ACTION event into PCR[7]
  //
  HashDataLen = TBT_SECURITY_EVENT_STRING_LEN;

  Status = TpmMeasureAndLogData (
             7,
             EV_EFI_ACTION,
             TBT_SECURITY_EVENT_STRING,
             (UINT32) HashDataLen,
             TBT_SECURITY_EVENT_STRING,
             HashDataLen
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TpmMeasureAndLogData Status: %d\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "TpmMeasureAndLogData Successfully\n"));
  }

  DEBUG ((DEBUG_INFO, "ExtendPCR7CallBack END\n"));
}

/**
  Register an End of DXE event for extended a TPM log to PCR[7] when vtd is diable
  This feature is introduced by TBT Security requirment
**/
VOID
RegisterExtendPCR7CallBack (
  VOID
  )
{
  EFI_STATUS                Status;
  UINTN                     DataSize;
  SA_SETUP                  *SaSetup;

  Status                    = EFI_SUCCESS;
  SaSetup                   = NULL;
  DataSize                  = 0;


  DataSize = sizeof (SA_SETUP);
  SaSetup = AllocateZeroPool (DataSize);
  if (SaSetup == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate SaSetup size\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = gRT->GetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  NULL,
                  &DataSize,
                  SaSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get SaSetup Status: %d\n", Status));
    goto Exit;
  }

  if (SaSetup->EnableVtd == FALSE) {
    //
    // Register an End of DXE event for extended a TPM log to PCR[7]
    //
    DEBUG ((DEBUG_ERROR, "Register an End of DXE event for extended a TPM log to PCR[7] when Vtd is disable\n"));
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    ExtendPCR7CallBack,
                    NULL,
                    &gEfiEndOfDxeEventGroupGuid,
                    &EndOfDxeEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to Register an End of DXE event for extended a TPM log to PCR[7], Status: %d\n", Status));
      gBS->CloseEvent (EndOfDxeEvent);
      goto Exit;
    }
  }

Exit:
  if (SaSetup != NULL) {
    FreePool (SaSetup);
  }

}

#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 1
/**
  Update Lch specific setup configuration
**/
VOID
UpdateLchSetupConfig (
  VOID
  )
{
  UINTN                         VarSize;
  UINTN                         MipiVarSize;
  UINT32                        VariableAttr;
  UINT32                        MipiVariableAttr;
  SETUP_DATA                    SetupData;
  MIPICAM_CONFIG                MipiSetupData;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "UpdateLchSetupConfig () - Start\n"));

  ZeroMem (&SetupData, sizeof (SETUP_DATA));
  VariableAttr = 0;
  VarSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  PLATFORM_SETUP_VARIABLE_NAME,
                  &gSetupVariableGuid,
                  &VariableAttr,
                  &VarSize,
                  &SetupData
                  );
  if (EFI_ERROR(Status)){
    DEBUG ((DEBUG_ERROR, "UpdateLchSetupConfig () : SetupData was not found.\n"));
    return;
  }

  ZeroMem (&MipiSetupData, sizeof (MIPICAM_CONFIG));
  MipiVariableAttr = 0;
  MipiVarSize = sizeof (MIPICAM_CONFIG);
  Status = gRT->GetVariable (
                  MIPICAM_CONFIG_VARIABLE_NAME,
                  &gMipiCamConfigVariableGuid,
                  &MipiVariableAttr,
                  &MipiVarSize,
                  &MipiSetupData
                  );
  if (EFI_ERROR(Status)){
    DEBUG ((DEBUG_ERROR, "UpdateLchSetupConfig () : MipiSetupData was not found.\n"));
    return;
  }

  if (SetupData.LchSupport) {
    MipiSetupData.MipiCam_ControlLogic0_Type = 1;
    MipiSetupData.MipiCam_ControlLogic0_CrdVersion = 0x20;
    MipiSetupData.MipiCam_ControlLogic0_InputClock = 3;
    MipiSetupData.MipiCam_ControlLogic0_PchClockSource = 0;
    MipiSetupData.MipiCam_Link0_SensorModel = 4;
    MipiSetupData.MipiCam_Link0_DriverData_CrdVersion = 0x20;
    MipiSetupData.MipiCam_Link0_CameraPhysicalLocation = 0x61;
    MipiSetupData.MipiCam_Link0_DriverData_FlashSupport = 2;
    CopyMem (MipiSetupData.MipiCam_Link0_ModuleName, L"KAFE799", sizeof (L"KAFE799"));
    MipiSetupData.MipiCam_Link0_DriverData_LinkUsed = 0;
    MipiSetupData.MipiCam_Link0_DriverData_EepromType = 0x14;
    MipiSetupData.MipiCam_Link0_DriverData_VcmType = 0x0;
    MipiSetupData.MipiCam_Link0_I2cDevicesEnabled = 0x2;
    MipiSetupData.MipiCam_Link0_I2cChannel = 0x1;
    MipiSetupData.MipiCam_Link0_I2cAddress[1] = 0x50;
    MipiSetupData.MipiCam_Link0_I2cDeviceType[1] = 0x2;

    Status = gRT->SetVariable (
                    MIPICAM_CONFIG_VARIABLE_NAME,
                    &gMipiCamConfigVariableGuid,
                    MipiVariableAttr,
                    MipiVarSize,
                    &MipiSetupData
                    );
    if (EFI_ERROR(Status)){
      DEBUG ((DEBUG_ERROR, "UpdateLchSetupConfig () : Failed to update MipiSetupData.\n"));
      return;
    }
    DEBUG ((DEBUG_INFO, "Lch(CVS) specific MipiSetupData configuration Updated\n"));
  }

  DEBUG ((DEBUG_INFO, "UpdateLchSetupConfig () - End\n"));
}
#endif

/**
  Entry point for the driver.

  @param[in]  ImageHandle  Image Handle.
  @param[in]  SystemTable  EFI System Table.

  @retval     EFI_SUCCESS  Function has completed successfully.
  @retval     Others       All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
EFIAPI
BoardInitAdvancedDxeEntryPoint (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
{
#if FixedPcdGetBool (PcdSinitAcmBinEnable) == 1
  EFI_STATUS                  Status;
#endif
  VOID                        *Registration;
  EFI_GUID                    *TpmInstanceGuid;

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_BOARD_INIT_DXE_ENTRY); //PostCode (BOARD_INIT_DXE_ENTRY);

#if FixedPcdGetBool (PcdSinitAcmBinEnable) == 1
  Status = EFI_SUCCESS;
  Status = TxtSinitAcmLoad ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "TXTDXE: Error finding SINIT binary in FVs, Status = %r\n", Status));
  } else {
      DEBUG ((DEBUG_INFO, "TXTDXE: Found SINIT ACM In FV\n"));
  }
#endif

  //
  // Initialize System Agent platform settings
  //
  SaPlatformInitDxe ();

#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 1
  //
  //Update Lch specific setup configuration
  //
  UpdateLchSetupConfig ();
#endif

  RegisterEndOfDxeCallbacks ();

  //
  // Check if TPM2.0 exist.
  //
  TpmInstanceGuid = PcdGetPtr (PcdTpmInstanceGuid);
  if (TpmInstanceGuid != NULL) {
    if (CompareGuid (TpmInstanceGuid , &gEfiTpmDeviceInstanceTpm20DtpmGuid)) {
      //
      // Register Pch reset callback to shutdown TPM
      //
      EfiCreateProtocolNotifyEvent (
        &gEfiResetNotificationProtocolGuid,
        TPL_CALLBACK,
        OnResetNotifyInstall,
        NULL,
        &Registration
        );
    }
  }

  //
  // Inform OS by TPM PCR7 when Vtd is disable
  //
  RegisterExtendPCR7CallBack ();

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_BOARD_INIT_DXE_EXIT); //PostCode (BOARD_INIT_DXE_EXIT);

  return EFI_SUCCESS;
}

#if FixedPcdGetBool(PcdSinitAcmBinEnable) == 1
/**

  This function looks for SINIT ACM in FVs and updates TXT HOB
  with SINIT ACM Base and Size.

  @retval EFI_SUCCESS     - SINIT ACM found and copied.
  @retval EFI_NOT_FOUND   - If TxtInfoHob is not found
  @retval EFI_UNSUPPORTED - TXT Device memory not available.

**/
EFI_STATUS
TxtSinitAcmLoad (
  VOID
  )
{
  EFI_STATUS                    Status;
  TXT_INFO_HOB                  *HobList;
  TXT_INFO_DATA                 *TxtInfoData;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  UINTN                         Size;
  EFI_FV_FILETYPE               FileType;
  UINT32                        FvStatus;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  UINT64                        TxtHeapMemoryBase;
  UINT64                        TxtSinitMemoryBase;
  UINT64                        *Ptr64;
  EFI_PHYSICAL_ADDRESS          TopAddr;
  BIOS_OS_DATA_REGION           *BiosOsDataRegion;

  HandleBuffer = NULL;
  NumberOfHandles = 0;
  FwVol = NULL;
  FileType = 0;
  Attributes = 0;
  BiosOsDataRegion = NULL;

  ///
  /// Find TXT HOB
  ///
  HobList = (TXT_INFO_HOB *) GetFirstGuidHob (&gTxtInfoHobGuid);
  if (HobList == NULL) {
    return EFI_NOT_FOUND;
  }

  TxtInfoData = &HobList->Data;
  ///
  /// Check TXT mode
  ///
  if (TxtInfoData->TxtMode == 0) {
    return EFI_UNSUPPORTED;
  }

  if ((TxtInfoData == 0) ||
      (TxtInfoData->TxtDprMemoryBase == 0) ||
      (TxtInfoData->TxtDprMemorySize == 0) ||
      (TxtInfoData->TxtHeapMemorySize == 0) ||
      (TxtInfoData->SinitMemorySize == 0)
      ) {
    return EFI_UNSUPPORTED;
  } else {
    ///
    /// Use address passed from PEI
    ///
    TopAddr             = TxtInfoData->TxtDprMemoryBase + TxtInfoData->TxtDprMemorySize;

    TxtHeapMemoryBase   = (UINT64) (TopAddr - TxtInfoData->TxtHeapMemorySize);

    TxtSinitMemoryBase  = TxtHeapMemoryBase - TxtInfoData->SinitMemorySize;
  }
  ///
  /// Start looking for SINIT ACM in FVs
  ///
  DEBUG ((DEBUG_INFO, "TXTDXE: Looking for SINIT ACM in FVs\n"));

  Status = gBS->LocateHandleBuffer (
    ByProtocol,
    &gEfiFirmwareVolume2ProtocolGuid,
    NULL,
    &NumberOfHandles,
    &HandleBuffer
    );
  ASSERT_EFI_ERROR (Status);
  ///
  /// Looking for FV with SinitAC binary
  ///
  for (UINTN i = 0; i < NumberOfHandles; i++) {
    ///
    /// Get the protocol on this handle
    ///
    Status = gBS->HandleProtocol (
      HandleBuffer[i],
      &gEfiFirmwareVolume2ProtocolGuid,
      &FwVol
      );
    ASSERT_EFI_ERROR (Status);
    ///
    /// See if it has the SinitACM file
    ///
    Size = 0;
    FvStatus = 0;
    Status = FwVol->ReadFile (
      FwVol,
      &gSinitModuleGuid,
      NULL,
      &Size,
      &FileType,
      &Attributes,
      &FvStatus
      );
    ///
    /// If the binary was located, then break
    ///
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "TXTDXE: Found SINIT ACM In FV\n"));
      break;
    }
  }
  FreePool (HandleBuffer);
  ///
  /// Sanity check
  ///
  if ( !(FwVol == NULL || Status != EFI_SUCCESS || Size >= TxtInfoData->SinitMemorySize ) ) {
    ///
    /// Read Sinit ACM from the storage file.
    ///
    Ptr64 = (UINT64 *)TxtSinitMemoryBase;
    Status = FwVol->ReadFile (
      FwVol,
      &gSinitModuleGuid,
      &Ptr64,
      &Size,
      &FileType,
      &Attributes,
      &FvStatus
    );
    ASSERT_EFI_ERROR (Status);
    TxtInfoData->SinitAcmSize = (UINT64) Size;

    Ptr64 = (UINT64 *) TxtHeapMemoryBase;
    ///
    /// BiosOsDataSize plus size of data size field itself
    ///
    BiosOsDataRegion                = (BIOS_OS_DATA_REGION *) (Ptr64 + 1);
    BiosOsDataRegion->BiosSinitSize = (UINT32) TxtInfoData->SinitAcmSize;
    DEBUG ((DEBUG_INFO, "TXTDXE: Copy SINIT ACM Done\n"));
  }
  return Status;
}
#endif
