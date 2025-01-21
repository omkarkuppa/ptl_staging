/**@file
  ME Setup Implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2008 Intel Corporation.

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

#include <SetupPrivate.h>
#include <MeSetup.h>
#include <Register/PchRegs.h>
#include <Register/HeciRegs.h>
#include <IndustryStandard/Pci30.h>
#include <Protocol/OneClickRecoveryProtocol.h>
#include <Library/DxeMeLib.h>
#include <Library/DxeAmtSupportLib.h>
#include <Library/MeFwStsLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/MeInfoLib.h>
#include <Library/MeUtilsLib.h>
#include <MeBiosPayloadHob.h>

GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN            mMeReset = FALSE;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_HII_HANDLE     mHiiHandle;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_EVENT          mRemoteSessionEvent;

/**
  Initialize KVM by sending HECI message to ME

  @param[in] Event                The event registered.
  @param[in] Context              Event context. Not used in this event handler.
**/
VOID
EFIAPI
RemoteSessionDetection (
  IN EFI_EVENT                    Event,
  IN VOID                         *Context
  )
{
  ME_STORAGE_EXPOSURE  MeStorageExposure;
  EFI_STRING           RequestString;
  EFI_TPL              OldTpl;

  DEBUG ((DEBUG_INFO, "%a () enter\n", __FUNCTION__));
  RequestString = NULL;
  OldTpl = EfiGetCurrentTpl ();

  gBS->RestoreTPL (TPL_APPLICATION);

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (ME_STORAGE_EXPOSURE, RemoteSessionActive), sizeof (MeStorageExposure.RemoteSessionActive));
  if (HiiGetBrowserData (&gMeSetupVariableGuid, L"MeStorageExposure", sizeof (ME_STORAGE_EXPOSURE), (UINT8 *) &MeStorageExposure)) {
    if ((MeStorageExposure.RemoteSessionActive == 0) && AmtIsRemoteSessionEnabled ()) {
      CreatePopUp (
        EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
        NULL,
        L"",
        L"Remote Session is open in PCH-FW Configuration.",
        L"System needs to reset to set up item attribute.",
        L"Reset system in 5 seconds...",
        L"",
        NULL
        );
      gBS->Stall (5000000);
      gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
    }
  }
  FreePool (RequestString);

  gBS->RaiseTPL (OldTpl);
  DEBUG ((DEBUG_INFO, "%a () exit\n", __FUNCTION__));

  return;
}

/**
  Check remote session status.

  If remote session is open, keep it as it is.
  If remote session is not open, because it might be open in runtime,
  BIOS needs to take care of the scenario accordingly.

  @param[in] Action          Specifies the type of action taken by the browser.
  @param[in] KeyValue        A unique value which is sent to the original exporting driver
                             so that it can identify the type of data to expect.
**/
VOID
CheckRemoteSession (
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue
  )
{
  EFI_STATUS           Status;
  EFI_STRING           RequestString;
  ME_STORAGE_EXPOSURE  MeStorageExposure;
  VOID                 *AfterConsolInNotifyReg;
  static BOOLEAN       IsConsoleEventCreated = FALSE;

  DEBUG ((DEBUG_INFO, "%a () enter, Action = 0x%x\n", __FUNCTION__, Action));

  RequestString = NULL;
  Status        = EFI_SUCCESS;

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (ME_STORAGE_EXPOSURE, RemoteSessionActive), sizeof (MeStorageExposure.RemoteSessionActive));
  if (RequestString == NULL) {
    return;
  }
if (!HiiGetBrowserData (&gMeSetupVariableGuid, L"MeStorageExposure", sizeof (ME_STORAGE_EXPOSURE), (UINT8 *)&MeStorageExposure)) {
    return;
  }

  //
  // If remote session is opened or closed after entering setup page, BIOS can update RemoteSessionActive value here.
  //
  if (Action == EFI_BROWSER_ACTION_FORM_OPEN) {
    MeStorageExposure.RemoteSessionActive = AmtIsRemoteSessionEnabled ();
    if (!HiiSetBrowserData (&gMeSetupVariableGuid, L"MeStorageExposure", sizeof (ME_STORAGE_EXPOSURE), (UINT8 *)&MeStorageExposure, RequestString)) {
      return;
    }
    FreePool (RequestString);
  }

  if (MeStorageExposure.RemoteSessionActive == 1) {
    return;
  }

  //
  // Remote session could be opened/closed after entering PCH-FW configuration page, BIOS should create event to update RemoteSessionActive in below conditions:
  //   1. Create event to detect remote session open when entering PCH-FW or AMT Configuration
  //   2. Close event only when exiting PCH-FW configuration.
  //
  if (Action == EFI_BROWSER_ACTION_FORM_OPEN &&
      (KeyValue == ME_FORM_ACTION_KEY || KeyValue == AMT_FORM_ACTION_KEY)) {
    //
    // If remote session is opened after entering "PCH-FW Configuration", the "grayoutif" capability can't be changed in real time.
    // Below event is used to detect this scenario and trigger warm reset.
    //
    if ((MeStorageExposure.RemoteSessionActive == 0) && !IsConsoleEventCreated) {
      IsConsoleEventCreated = TRUE;
      DEBUG ((DEBUG_INFO, "Create RemoteSessionDetection event\n"));
      Status = gBS->CreateEvent (
                      EVT_NOTIFY_SIGNAL,
                      TPL_NOTIFY,
                      RemoteSessionDetection,
                      NULL,
                      &mRemoteSessionEvent
                      );
      ASSERT_EFI_ERROR (Status);

      Status = gBS->RegisterProtocolNotify (
                      &gEfiConsoleInDeviceGuid,
                      mRemoteSessionEvent,
                      &AfterConsolInNotifyReg
                      );
      ASSERT_EFI_ERROR (Status);
    }
  } else if (Action == EFI_BROWSER_ACTION_FORM_CLOSE && KeyValue == ME_FORM_ACTION_KEY && IsConsoleEventCreated) {
    DEBUG ((DEBUG_INFO, "Close RemoteSessionDetection event\n"));
    IsConsoleEventCreated = FALSE;
    Status = gBS->CloseEvent (mRemoteSessionEvent);
  }

  return;
}

/**
  Converts the low nibble of a byte to hex unicode character.

  @param[in] Nibble      Lower nibble of a byte

  @retval CHAR16         Hex unicode character
**/
CHAR16
NibbleToHexChar (
  IN UINT8      Nibble
  );

/**
  Converts retrieved byte to string adding a hyphen after every 2nd byte.

  @param[out] String     Unicode String
  @param[in]  Buffer     Hash in hex format
  @param[in]  Length     Max length of hash to convert
**/
VOID
BinaryToHexString (
  OUT UINT16 *String,
  IN UINT8   *Buffer,
  IN UINTN   Length
  )
{
  UINTN   Index;
  UINT8   Byte;

  for (Index = 0; Index < Length; Index++) {

    if ((Index % 2 == 0) && (Index != 0)) {
      *String = '-';
      String++;
    }
    Byte = Buffer[Index];
    *String = NibbleToHexChar ((UINT8)(Byte >> 4));
    *(String + 1) = NibbleToHexChar (Byte);
    String += 2;
  }

  *String = '\0';
}

/**
  Sets One Click Recovery Setup Options are Supported
**/
VOID
SetOcrSetupSupported (
  VOID
  )
{
  EFI_STATUS                  Status;
  UINTN                       VariableSize;
  UINT32                      SetupVolVarAttr;
  SETUP_VOLATILE_DATA         VolatileData;
  ONE_CLICK_RECOVERY_PROTOCOL *OneClickRecovery;
  ONE_CLICK_RECOVERY_CAP      OcrCap;
  ME_BIOS_PAYLOAD_HOB         *MbpHob;
  BOOLEAN                     VariableUpdate;

  Status = gBS->LocateProtocol (&gOneClickRecoveryProtocolGuid, NULL, (VOID **)&OneClickRecovery);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[%a] OCR protocol GUID not found with status %r\n", __FUNCTION__, Status));
    return;
  }

  OcrCap = OneClickRecovery->OcrCap ();
  VariableUpdate = FALSE;

  VariableSize = sizeof (SETUP_VOLATILE_DATA);
  Status = gRT->GetVariable (
                  L"SetupVolatileData",
                  &gSetupVariableGuid,
                  &SetupVolVarAttr,
                  &VariableSize,
                  &VolatileData
                  );
  ASSERT_EFI_ERROR (Status);

  // Set supported OCR boot options
  VolatileData.OcrBootHttpsSupported     = OcrCap.Bits.OcrBootHttps;
  VolatileData.OcrBootPbaSupported       = OcrCap.Bits.OcrBootPba;
  VolatileData.OcrBootWinReSupported     = OcrCap.Bits.OcrBootWinRe;
  VolatileData.OcrAmtDisSecBootSupported = OcrCap.Bits.OcrAmtDisSecBoot;

  // If Brand ID OCR enabled make OCR options visible
  VolatileData.OcrOptionExposure = 0;
  VolatileData.OcrStateChanged           = FALSE;
  VolatileData.PreviousOcrState[0]       = VolatileData.OcrBootHttpsSupported;
  VolatileData.PreviousOcrState[1]       = VolatileData.OcrBootPbaSupported;
  VolatileData.PreviousOcrState[2]       = VolatileData.OcrBootWinReSupported;
  VolatileData.PreviousOcrState[3]       = VolatileData.OcrAmtDisSecBootSupported;
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob != NULL) {
    if (MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Amt == 1 && MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.OCR == 1) {
      if (MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.Amt) {
        if (VolatileData.OcrOptionExposure == 0) {
          VolatileData.OcrOptionExposure = 1;
          VariableUpdate = TRUE;
          if (VolatileData.OcrStateChanged) {
            VolatileData.OcrBootHttpsSupported     = VolatileData.PreviousOcrState[0];
            VolatileData.OcrBootPbaSupported       = VolatileData.PreviousOcrState[1];
            VolatileData.OcrBootWinReSupported     = VolatileData.PreviousOcrState[2];
            VolatileData.OcrAmtDisSecBootSupported = VolatileData.PreviousOcrState[3];
            VolatileData.OcrStateChanged = FALSE;
          }
        }
      } else if (VolatileData.OcrOptionExposure == 1 || VolatileData.OcrBootHttpsSupported == 1 || VolatileData.OcrBootPbaSupported == 1 || VolatileData.OcrBootWinReSupported == 1 || VolatileData.OcrAmtDisSecBootSupported == 1) {
        VolatileData.OcrOptionExposure = 0;
        if (VolatileData.OcrStateChanged == FALSE) {
          VolatileData.PreviousOcrState[0]     = VolatileData.OcrBootHttpsSupported;
          VolatileData.PreviousOcrState[1]     = VolatileData.OcrBootPbaSupported;
          VolatileData.PreviousOcrState[2]     = VolatileData.OcrBootWinReSupported;
          VolatileData.PreviousOcrState[3]     = VolatileData.OcrAmtDisSecBootSupported;
          VolatileData.OcrStateChanged = TRUE;
        }
        VolatileData.OcrBootHttpsSupported     = 0;
        VolatileData.OcrBootPbaSupported       = 0;
        VolatileData.OcrBootWinReSupported     = 0;
        VolatileData.OcrAmtDisSecBootSupported = 0;
        VariableUpdate = TRUE;
      }
    } else if (VolatileData.OcrOptionExposure == 1 || VolatileData.OcrBootHttpsSupported == 1 || VolatileData.OcrBootPbaSupported == 1 || VolatileData.OcrBootWinReSupported == 1 || VolatileData.OcrAmtDisSecBootSupported == 1) {
        VolatileData.OcrOptionExposure = 0;
        VolatileData.OcrBootHttpsSupported     = 0;
        VolatileData.OcrBootPbaSupported       = 0;
        VolatileData.OcrBootWinReSupported     = 0;
        VolatileData.OcrAmtDisSecBootSupported = 0;
        VariableUpdate = TRUE;
    }
  }

  // If remote KVM session active then gray out OCR options
  VolatileData.OcrOptionGrayOut = 0;
  if (AmtIsRemoteSessionEnabled ()) {
    VolatileData.OcrOptionGrayOut = 1;
    VariableUpdate = TRUE;
  }

  if (VariableUpdate) {
    Status = gRT->SetVariable (
                  L"SetupVolatileData",
                  &gSetupVariableGuid,
                  SetupVolVarAttr,
                  sizeof (SETUP_VOLATILE_DATA),
                  &VolatileData
                  );
    ASSERT_EFI_ERROR (Status);
  }
}

/**
  Initialize ME strings.

  @param[in]  HiiHandle          Hii Handle.
  @param[in]  Class              Setup form class.
**/
VOID
InitMeInfo (
  IN EFI_HII_HANDLE HiiHandle,
  IN UINT16         Class
  )
{
  EFI_STATUS            Status;
  UINTN                 VariableSize;
  UINT8                 *MeFwSkuValue;
  CHAR16                String[0x100];
  UINTN                 StringLength;
  static  EFI_STRING_ID MeMode[] = {
    STRING_TOKEN(STR_ME_FW_MODE_NORMAL_MODE),
    STRING_TOKEN(STR_ME_FW_MODE_FAILED),
    STRING_TOKEN(STR_ME_FW_MODE_ALT_DISABLED),
    STRING_TOKEN(STR_ME_FW_MODE_TEMP_DISABLED),
    STRING_TOKEN(STR_ME_FW_MODE_SECOVER),
    STRING_TOKEN(STR_ME_FW_MODE_FAILED),
    STRING_TOKEN(STR_ME_FW_MODE_FAILED),
  };
  static  UINT8         *FwMeSku[] = {
     (UINT8*) ME_FW_IMAGE_TYPE_CONSUMER_STRING,
     (UINT8*) ME_FW_IMAGE_TYPE_CORPORATE_STRING,
     (UINT8*) ME_FW_IMAGE_TYPE_UNIDENTIFIED_STRING,
  };
  ME_INFO_SETUP_DATA    MeInfoSetupData;
  UINT64                HeciBaseAddress;
  UINT32                MeFwStatus;
  UINT32                MeFwStatus2;
  UINT32                MeFwStatus3;
  UINT32                MeFwStatus4;
  UINT32                MeFwStatus5;
  UINT32                MeFwStatus6;

  if ((Class != MAIN_FORM_SET_CLASS) && (Class != ADVANCED_FORM_SET_CLASS)) {
    return;
  }

  mHiiHandle      = HiiHandle;
  HeciBaseAddress = 0;
  MeFwStatus      = 0;
  MeFwStatus2     = 0;
  MeFwStatus3     = 0;
  MeFwStatus4     = 0;
  MeFwStatus5     = 0;
  MeFwStatus6     = 0;

  DEBUG ((DEBUG_INFO, "%a() enter\n", __FUNCTION__));

  //
  // Set default string size assumption at no more than 256 bytes
  //
  StringLength = 0x100;

  VariableSize = sizeof (ME_INFO_SETUP_DATA);
  Status = gRT->GetVariable (
                  L"MeInfoSetup",
                  &gMeInfoSetupGuid,
                  NULL,
                  &VariableSize,
                  &MeInfoSetupData
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  InitString (
    mHiiHandle,
    STRING_TOKEN (STR_ME_FW_VERSION_VALUE),
    L"%d.%d.%d.%d",
    MeInfoSetupData.MeMajor,
    MeInfoSetupData.MeMinor,
    MeInfoSetupData.MeHotFix,
    MeInfoSetupData.MeBuildNo
  );

  VariableSize = sizeof (ME_SETUP);
  Status = gRT->GetVariable (
                  L"MeSetup",
                  &gMeSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &mMeSetup
                  );
  ASSERT_EFI_ERROR (Status);

  if ((mMeSetup.OperationMode & ME_MODE_MASK) < sizeof (MeMode) / sizeof (EFI_STRING_ID)) {
    StringLength = sizeof (String);
    HiiLibGetString (mHiiHandle, MeMode[(mMeSetup.OperationMode & ME_MODE_MASK)], String, &StringLength);
    InitString (mHiiHandle, STRING_TOKEN (STR_ME_FW_MODE_VALUE), L"%s", String);
  }

  if (mMeSetup.MeImageType == ME_IMAGE_CONSUMER_SKU_FW) {
    MeFwSkuValue = FwMeSku[0];
  } else if (mMeSetup.MeImageType == ME_IMAGE_CORPORATE_SKU_FW) {
    MeFwSkuValue = FwMeSku[1];
  } else {
    MeFwSkuValue = FwMeSku[2];
  }
  InitString (
    mHiiHandle,
    STRING_TOKEN (STR_ME_FW_SKU_VALUE),
    L"%a",
    MeFwSkuValue
    );

#if FixedPcdGet8 (PcdOneClickRecoveryEnable) == 0x1
  SetOcrSetupSupported ();
#endif

  //
  // Check if Heci device is enabled
  //
  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  if (PciSegmentRead32 (HeciBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_INFO, "ME is disabled, read status from MeFwHob\n"));
    MeFwStatus  = GetMeStatusFromFwstsHob (R_ME_CFG_HFS);
    MeFwStatus2 = GetMeStatusFromFwstsHob (R_ME_CFG_HFS_2);
    MeFwStatus3 = GetMeStatusFromFwstsHob (R_ME_CFG_HFS_3);
    MeFwStatus4 = GetMeStatusFromFwstsHob (R_ME_CFG_HFS_4);
    MeFwStatus5 = GetMeStatusFromFwstsHob (R_ME_CFG_HFS_5);
    MeFwStatus6 = GetMeStatusFromFwstsHob (R_ME_CFG_HFS_6);
  } else {
    MeFwStatus  = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS);
    MeFwStatus2 = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_2);
    MeFwStatus3 = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_3);
    MeFwStatus4 = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_4);
    MeFwStatus5 = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_5);
    MeFwStatus6 = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_6);
  }

  InitString (
    mHiiHandle,
    STRING_TOKEN (STR_ME_FW_STATUS_1_VALUE),
    L"0x%08x",
    MeFwStatus
    );

  InitString (
    mHiiHandle,
    STRING_TOKEN (STR_ME_FW_STATUS_2_VALUE),
    L"0x%08x",
    MeFwStatus2
    );

  InitString (
    mHiiHandle,
    STRING_TOKEN (STR_ME_FW_STATUS_3_VALUE),
    L"0x%08x",
    MeFwStatus3
    );

  InitString (
    mHiiHandle,
    STRING_TOKEN (STR_ME_FW_STATUS_4_VALUE),
    L"0x%08x",
    MeFwStatus4
    );

  InitString (
    mHiiHandle,
    STRING_TOKEN (STR_ME_FW_STATUS_5_VALUE),
    L"0x%08x",
    MeFwStatus5
    );

  InitString (
    mHiiHandle,
    STRING_TOKEN (STR_ME_FW_STATUS_6_VALUE),
    L"0x%08x",
    MeFwStatus6
    );

  //
  // Update PSE FW Status
  //
  HeciBaseAddress = MePseHeciPciCfgBase (PseHeci1);
  if (HeciBaseAddress != MAX_UINT64) {
    InitString (
      mHiiHandle,
      STRING_TOKEN (STR_PSE_FW_STATUS_1_VALUE),
      L"0x%08x",
      PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS)
      );
  } else {
    InitString (
      mHiiHandle,
      STRING_TOKEN (STR_PSE_FW_STATUS_1_VALUE),
      L"0x%08x",
      MAX_UINT32
      );
  }
}

/**
  Update ME setup configuration and sent command to CSME after data changed.
**/
VOID
MeRouteConfig (
  VOID
  )
{
  ME_SETUP_STORAGE        MeSetupStorage;
  ME_SETUP_STORAGE        MeBackupStorage;
  ME_STORAGE_EXPOSURE     MeStorageExposure;
  UINTN                   Size;
  EFI_STATUS              Status;
  UINT32                  EnableBitmap;
  UINT32                  DisableBitmap;
  EFI_STRING              RequestString;

  DEBUG ((DEBUG_INFO, "%a () enter\n", __FUNCTION__));

  RequestString = NULL;
  ZeroMem (&MeSetupStorage, sizeof (ME_SETUP_STORAGE));
  ZeroMem (&MeBackupStorage, sizeof (ME_SETUP_STORAGE));
  ZeroMem (&MeStorageExposure, sizeof (ME_STORAGE_EXPOSURE));

  Size = sizeof (ME_SETUP_STORAGE);
  Status = gRT->GetVariable (L"MeSetupStorage", &gMeSetupVariableGuid, NULL, &Size, &MeSetupStorage);
  if (EFI_ERROR (Status)) {
    return;
  }

  Size = sizeof (ME_SETUP_STORAGE);
  Status = gRT->GetVariable (L"MeBackupStorage", &gMeSetupVariableGuid, NULL, &Size, &MeBackupStorage);
  if (EFI_ERROR (Status)) {
    return;
  }

  Size = sizeof (ME_SETUP);
  Status = gRT->GetVariable (L"MeSetup", &gMeSetupVariableGuid, NULL, &Size, &mMeSetup);
  if (EFI_ERROR (Status)) {
    return;
  }

  Size = sizeof (ME_STORAGE_EXPOSURE);
  Status = gRT->GetVariable (L"MeStorageExposure", &gMeSetupVariableGuid, NULL, &Size, &MeStorageExposure);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (MeSetupStorage.InvokeArbSvnCommit != MeBackupStorage.InvokeArbSvnCommit) {
    if ((mMeSetup.AutoArbSvnCommit == 0) && (MeSetupStorage.InvokeArbSvnCommit == 1)) {
      HeciArbSvnCommitMsg ();
    }
  }

  if (MeSetupStorage.UpidState != MeBackupStorage.UpidState) {
    HeciSetUpidFeatureState (UpidFeaturePlatId, MeSetupStorage.UpidState);
  }

  //
  // @Note: Commands listed below are blocked in post EndOfPost
  //
  if (!MeIsAfterEndOfPost ()) {

    if (MeSetupStorage.MeStateControl != MeBackupStorage.MeStateControl) {
      mMeReset = TRUE;

      if (MeSetupStorage.MeStateControl == 0) {
        HeciSetMeDisableMsg ();
      } else {
        HeciSetMeEnableMsg ();
      }

      //
      // Gray out the item to prevent ME State Control been switched back after command is sent.
      //
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (ME_STORAGE_EXPOSURE, MeState), sizeof (MeStorageExposure.MeState));
      if (RequestString != NULL) {
        MeStorageExposure.MeState = 0;
        Status = HiiSetBrowserData (&gMeSetupVariableGuid, L"MeStorageExposure", sizeof (ME_STORAGE_EXPOSURE), (UINT8 *)&MeStorageExposure, RequestString);
        FreePool (RequestString);
        if (EFI_ERROR (Status)) {
          return;
        }
      }
    }

    if (MeSetupStorage.FwUpdEnabled != MeBackupStorage.FwUpdEnabled) {
      HeciSetFwUpdateState (MeSetupStorage.FwUpdEnabled);
    }

    if (MeSetupStorage.MngState != MeBackupStorage.MngState) {
      mMeReset = TRUE;
      if (MeSetupStorage.MngState == 1) {
        //
        // User wants to enable Manageability Features
        //
        EnableBitmap  = AMT_BITMASK;
        DisableBitmap = CLEAR_FEATURES_BITMASK;
      } else {
        //
        // User wants to disable Manageability Features
        //
        EnableBitmap  = CLEAR_FEATURES_BITMASK;
        DisableBitmap = AMT_BITMASK;
      }
      HeciFwFeatureStateOverride (EnableBitmap, DisableBitmap);
    }

    if (MeSetupStorage.DelayedAuthenticationMode != MeBackupStorage.DelayedAuthenticationMode) {
      mMeReset = TRUE;
      HeciSetDamState (MeSetupStorage.DelayedAuthenticationMode);
    }

    if (MeSetupStorage.FipsModeSelect != MeBackupStorage.FipsModeSelect) {
      HeciSetFipsMode (MeSetupStorage.FipsModeSelect);
    }

    if (MeSetupStorage.UpidOsControlState != MeBackupStorage.UpidOsControlState) {
      HeciSetUpidOsControlState (UpidFeaturePlatId, MeSetupStorage.UpidOsControlState);
    }

    if (MeSetupStorage.ActiveLanInterface != MeBackupStorage.ActiveLanInterface) {
      mMeReset = TRUE;
      AmtSetActiveLanInterface (MeSetupStorage.ActiveLanInterface);
    }
  }

  //
  // Remember current FW's state to avoid sending unnecessary messages next time
  //
  Status = gRT->SetVariable (
                  L"MeBackupStorage",
                  &gMeSetupVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (ME_SETUP_STORAGE),
                  &MeSetupStorage
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gRT->SetVariable (
                  L"MeStorageExposure",
                  &gMeSetupVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (ME_STORAGE_EXPOSURE),
                  &MeStorageExposure
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  Setup callback executed when user selects PCH-FW configuration from the BIOS UI.
  Changes strings and settings of certain options.

  @param[in] This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action          Specifies the type of action taken by the browser.
  @param[in] KeyValue        A unique value which is sent to the original exporting driver
                             so that it can identify the type of data to expect.
  @param[in] Type            The type of value for the question.
  @param[in] Value           A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest  On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS        The callback successfully handled the action
  @retval EFI_UNSUPPORTED    The specified action is not supported by the callback
**/
EFI_STATUS
EFIAPI
MeFormCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  EFI_STATUS           Status;
  ME_STORAGE_EXPOSURE  MeStorageExposure;
  UINTN                Size;
  UINT32               VariableAttr;

  DEBUG ((DEBUG_INFO, "%a () enter, Action = 0x%x, KeyValue = 0x%x\n", __FUNCTION__, Action, KeyValue));

  if (Action >= EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
    switch (KeyValue) {
      case MNG_STATE_KEY:
      case DAM_STATE_KEY:
        return EFI_SUCCESS;

     default:
       return EFI_UNSUPPORTED;
    }
  }

  //
  // If it is corporate image and both RemoteSessionActive/AfterEoP are 0, BIOS needs to check remote session status in runtime.
  //
  ZeroMem (&MeStorageExposure, sizeof (ME_STORAGE_EXPOSURE));
  Size = sizeof (ME_STORAGE_EXPOSURE);
  Status = gRT->GetVariable (L"MeStorageExposure", &gMeSetupVariableGuid, &VariableAttr, &Size, &MeStorageExposure);
  if (!EFI_ERROR (Status) &&
      (mMeSetup.MeImageType == ME_IMAGE_CORPORATE_SKU_FW) &&
      (MeStorageExposure.RemoteSessionActive == 0) &&
      (MeStorageExposure.AfterEoP == 0)) {
    CheckRemoteSession (Action, KeyValue);
  }

  return EFI_UNSUPPORTED;
}

/**
  Extract CSME setting and update ME setup configuration.
**/
VOID
MeExtractConfig (
  VOID
  )
{
  EFI_STATUS              Status;
  ME_SETUP_STORAGE        MeSetupStorage;
  ME_SETUP_STORAGE        MeBackupStorage;
  ME_STORAGE_EXPOSURE     MeStorageExposure;
  UINTN                   Size;
  UINT32                  VariableAttr;
  CHAR16                  String[80]; // Fit 64 chars of UPID String + 15 dashes + Null char
  UINTN                   StringLength;
  EFI_STRING_ID           FipsMode[] = {
    STRING_TOKEN(STR_DISABLED),
    STRING_TOKEN(STR_ENABLED)
  };
  GET_FIPS_MODE_DATA      FipsModeData;
  UINT32                  NumOfEntries;
  ARB_SVN_INFO_ENTRY      *ArbSvnInfo;
  MEFWCAPS_SKU            CurrentFeatures;
  MEFWCAPS_SKU            UserCapabilities;
  UINT32                  FwUpdateState;
  UINT32                  PlatformState;
  UINT32                  Index;
  UINT32                  OemPlatformIdType;
  UINT8                   OemPlatformId[MAX_PLAT_ID_SIZE];
  UINT8                   CsmePlatformId[MAX_PLAT_ID_SIZE];
  ME_BIOS_PAYLOAD_HOB     *MbpHob;

  MbpHob = NULL;

  DEBUG ((DEBUG_INFO, "%a () enter\n", __FUNCTION__));

  VariableAttr          = 0;
  NumOfEntries          = MFT_KEY_USAGE_INDEX_UNDEFINED_MANIFEST;
  ArbSvnInfo            = NULL;
  Index                 = 0;
  CurrentFeatures.Data  = 0;
  UserCapabilities.Data = 0;
  FwUpdateState         = 0;
  PlatformState         = 0;
  ZeroMem (&MeSetupStorage, sizeof (ME_SETUP_STORAGE));
  ZeroMem (&MeBackupStorage, sizeof (ME_SETUP_STORAGE));
  ZeroMem (&MeStorageExposure, sizeof (ME_STORAGE_EXPOSURE));

  ArbSvnInfo = AllocateZeroPool (NumOfEntries * sizeof (ARB_SVN_INFO_ENTRY));
  if (ArbSvnInfo != NULL) {
    Status = HeciArbSvnGetInfoMsg (&NumOfEntries, ArbSvnInfo);

    if (!EFI_ERROR (Status)) {
      for (Index = 0; Index < NumOfEntries; Index++) {
        //
        // BIOS should always display the entry with MFT_KEY_USAGE_INDEX_CSE_RBE_MANIFEST
        //
        if (ArbSvnInfo[Index].UsageId == MFT_KEY_USAGE_INDEX_CSE_RBE_MANIFEST) {
          InitString (mHiiHandle, STRING_TOKEN (STR_ARB_SVN_MIN_VALUE), L"%d", ArbSvnInfo[Index].MinAllowedSvn);
          InitString (mHiiHandle, STRING_TOKEN (STR_ARB_SVN_CURR_VALUE), L"%d", ArbSvnInfo[Index].ExecutingSvn);
          break;
        }
      }
    }
  }

  Size = sizeof (ME_SETUP_STORAGE);
  Status = gRT->GetVariable (L"MeSetupStorage", &gMeSetupVariableGuid, &VariableAttr, &Size, &MeSetupStorage);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "MeSetupStorage does not exist. Assuming default values.\n"));
    VariableAttr = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
    MeSetupStorage.MngState = 1;
    MeSetupStorage.FwUpdEnabled = 1;
    MeSetupStorage.MeStateControl = 0;
    MeSetupStorage.DelayedAuthenticationMode = 0;
    MeSetupStorage.FipsModeSelect = 0;
    MeSetupStorage.AmtState = 1;
    MeSetupStorage.UpidSupport = 0;
    MeSetupStorage.UpidState = 0;
    MeSetupStorage.UpidOsControlState = 0;
    MeSetupStorage.ActiveLanInterface = 0;
  }

  Size = sizeof (ME_STORAGE_EXPOSURE);
  Status = gRT->GetVariable (L"MeStorageExposure", &gMeSetupVariableGuid, NULL, &Size, &MeStorageExposure);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "MeStorageExposure does not exist. Assuming default values.\n"));
    MeStorageExposure.MeState = 1;
    MeStorageExposure.AfterEoP = 0;
    MeStorageExposure.RemoteSessionActive = 0;
  }

  //
  // Always disable InvokeArbSvnCommit setup option
  //
  MeSetupStorage.InvokeArbSvnCommit = 0;

  HeciGetDamState (&MeSetupStorage.DelayedAuthenticationMode);

  if (MeIsAfterEndOfPost ()) {
    MeStorageExposure.AfterEoP = 1;
  } else {
    MeStorageExposure.AfterEoP = 0;
  }

  Status = HeciGetPlatformStateMsg (&PlatformState);
  if (EFI_ERROR (Status)) {
    PlatformState = 0;
  }

  if (MeStorageExposure.MeState) {
    //
    // Meaning the command for MeStateControl has NOT been saved and sent.
    //
    MeSetupStorage.MeStateControl = (UINT8) PlatformState;
  }

  MeStorageExposure.RemoteSessionActive = AmtIsRemoteSessionEnabled ();

  Status = HeciGetFwFeatureStateMsg (&CurrentFeatures);
  if (!EFI_ERROR (Status)) {
    MeSetupStorage.MngState   = !!CurrentFeatures.Fields.Amt;
  }

  Status = HeciGetUserCapabilitiesState (&UserCapabilities);
  if (!EFI_ERROR (Status)) {
    MeSetupStorage.AmtState   = !!UserCapabilities.Fields.Amt;
  }

  Status = HeciGetFwUpdateState (&FwUpdateState);
  if (!EFI_ERROR (Status)) {
    MeSetupStorage.FwUpdEnabled = (UINT8) FwUpdateState;
  }

  Size = sizeof (ME_SETUP);
  Status = gRT->GetVariable (L"MeSetup", &gMeSetupVariableGuid, NULL, &Size, &mMeSetup);
  ASSERT_EFI_ERROR (Status);

  if (mMeSetup.MeImageType == IntelMeCorporateFw) {
    //
    // FIPS feature
    //
    Status = HeciGetFipsMode (&FipsModeData);
    if (!EFI_ERROR (Status)) {
      StringLength = sizeof (String);
      HiiLibGetString (mHiiHandle, FipsMode[!!FipsModeData.FipsMode], String, &StringLength);
      InitString (mHiiHandle, STRING_TOKEN (STR_ME_FW_FIPS_CURRENT_MODE_VALUE), L"%s", String);
      InitString (
        mHiiHandle,
        STRING_TOKEN (STR_ME_FW_FIPS_CRYPTO_VER_VALUE),
        L"%d.%d.%d.%d",
        FipsModeData.CryptoVersion.Major,
        FipsModeData.CryptoVersion.Minor,
        FipsModeData.CryptoVersion.Build,
        FipsModeData.CryptoVersion.Hotfix
      );
    }

    //
    // UPID Platform Id feature
    //

    MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
    if (MbpHob != NULL) {
      if (MbpHob->MeBiosPayload.FwCapsSku.Available == TRUE) {
        if (MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Upid == 1) {
          MeSetupStorage.UpidSupport = 1;
          Status = HeciGetUpidFeatureState (UpidFeaturePlatId, &MeSetupStorage.UpidState);
          if (!EFI_ERROR (Status) && MeSetupStorage.UpidState == 1) {
            Status = HeciGetUpidPlatformId (&OemPlatformIdType, OemPlatformId, CsmePlatformId);
            if (!EFI_ERROR (Status)) {
              //
              // Update CSME Platform ID
              //
              ZeroMem (String, sizeof (String));
              BinaryToHexString (String, CsmePlatformId, MAX_PLAT_ID_SIZE);
              InitString (mHiiHandle, STRING_TOKEN (STR_UPID_CSME_PLAT_ID_VALUE), L"%s", String);

              //
              // Update OEM Platform ID
              //
              ZeroMem (String, sizeof (String));
              if (OemPlatformIdType == OemPlatIdTypePrintableString) {
                AsciiStrToUnicodeStrS ((CHAR8 *)OemPlatformId, String, MAX_PLAT_ID_SIZE);
              } else if (OemPlatformIdType == OemPlatIdTypeBinary) {
                BinaryToHexString (String, OemPlatformId, MAX_PLAT_ID_SIZE);
              }
              if (OemPlatformIdType != OemPlatIdTypeNotSet) {
                InitString (mHiiHandle, STRING_TOKEN (STR_UPID_OEM_PLAT_ID_VALUE), L"%s", String);
              }
            }
          }
          HeciGetUpidOsControlState (UpidFeaturePlatId, &MeSetupStorage.UpidOsControlState);
        }
      }
    }
    //
    // Dynamic Lan Switch feature
    //
    AmtGetActiveLanInterface (&MeSetupStorage.ActiveLanInterface);
  }

  if (ArbSvnInfo != NULL) {
    FreePool (ArbSvnInfo);
  }
  Status = gRT->SetVariable (
                  L"MeSetupStorage",
                  &gMeSetupVariableGuid,
                  VariableAttr,
                  sizeof (ME_SETUP_STORAGE),
                  &MeSetupStorage
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gRT->SetVariable (
                  L"MeBackupStorage",
                  &gMeSetupVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (ME_SETUP_STORAGE),
                  &MeSetupStorage
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gRT->SetVariable (
                  L"MeStorageExposure",
                  &gMeSetupVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (ME_STORAGE_EXPOSURE),
                  &MeStorageExposure
                  );
  ASSERT_EFI_ERROR (Status);

  // Since DEBUG() only allow max 12 format parameters, so call it twice.
  DEBUG ((DEBUG_INFO, "%a %d %d %d %d %d %d %d %d %d %d",
    __FUNCTION__,
    MeSetupStorage.MngState,
    MeSetupStorage.FwUpdEnabled,
    MeSetupStorage.MeStateControl,
    MeSetupStorage.DelayedAuthenticationMode,
    MeSetupStorage.FipsModeSelect,
    MeSetupStorage.AmtState,
    MeSetupStorage.UpidSupport,
    MeSetupStorage.UpidState,
    MeSetupStorage.UpidOsControlState,
    MeSetupStorage.ActiveLanInterface
    ));
  DEBUG ((DEBUG_INFO, " %d %d %d\n",
    MeStorageExposure.MeState,
    MeStorageExposure.AfterEoP,
    MeStorageExposure.RemoteSessionActive
    ));

}
