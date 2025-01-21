/** @file
  AMT specific functions implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#include <Defines/PcdPchBdfAssignment.h>
#include <IndustryStandard/UefiTcgPlatform.h>
#include "BdsPlatform.h"
#include "AmtSupport.h"
#include <AmtConfig.h>
#include <MeBiosPayloadHob.h>
#include <Library/DxeAsfLib.h>
#include <Library/DxeAmtSupportLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/DxeMeLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/TpmMeasurementLib.h>
#include <Protocol/HeciControlProtocol.h>
#include <Protocol/AmtPolicy.h>
#include <Guid/AuthenticatedVariableFormat.h>
#include <Protocol/OneClickRecoveryProtocol.h>
#include <OemOcrBootOptionList.h>

GLOBAL_REMOVE_IF_UNREFERENCED EFI_EVENT mHotkeyRegisteredEvent       = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED VOID      *mRegistration;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN   mModeInitialized             = FALSE;

//
// Boot video resolution and text mode.
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32    mBootHorizontalResolution    = 0;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32    mBootVerticalResolution      = 0;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32    mBootTextModeColumn          = 0;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32    mBootTextModeRow             = 0;
//
// BIOS setup video resolution and text mode.
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32    mSetupTextModeColumn         = 0;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32    mSetupTextModeRow            = 0;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32    mSetupHorizontalResolution   = 0;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32    mSetupVerticalResolution     = 0;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_GUID  mAmtRestoreSecureBootGuid    = {
  0x118b3c6f, 0x98d6, 0x4d05, {0x96, 0xb2, 0x90, 0xe4, 0xcb, 0xb7, 0x40, 0x34}
};

extern EFI_GUID mAmtConsoleVariableGuid;

/**
  Stop Asf watchdog timer.

  @param[in] Event    Event whose notification function is being invoked
  @param[in] Context  Context passed to this function.
**/
VOID
EFIAPI
AmtStopWatchdog (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  AsfStopWatchDog ();
}

/**
  This routine makes necessary Secure Boot change for Storage Redirection boot

  @param[in] ManageSecureBootMode  SecureBoot state is to be restored or disabled
**/
VOID
ManageSecureBootState (
  IN UINT8  ManageSecureBootMode
  )
{
  EFI_STATUS  Status;
  BOOLEAN     EnforceSecureBoot;
  UINT8       SecureBootState;
  BOOLEAN     IsUsbrBoot;
  UINTN       VarSize;
  UINT32      VarAttributes;
  UINT8       RestoreBootSettings;

  RestoreBootSettings = RESTORE_SECURE_BOOT_NONE;
  //
  // Get boot parameters (Storage Redirection boot?, EnforceSecureBoot flag set?, secure boot enabled?)
  //
  VarSize = sizeof (SecureBootState);
  Status  = gRT->GetVariable (
                   EFI_SECURE_BOOT_ENABLE_NAME,
                   &gEfiSecureBootEnableDisableGuid,
                   &VarAttributes,
                   &VarSize,
                   &SecureBootState
                   );
  if (EFI_ERROR (Status)) {
    return;
  }

  EnforceSecureBoot = AsfIsEnforceSecureBootEnabled ();
  IsUsbrBoot        = AsfIsStorageRedirectionEnabled ();

  if (SecureBootState == SECURE_BOOT_DISABLE) {
    //
    // We need to restore SecureBoot state if we change it for Storage Redirection boot.
    //
    VarSize = sizeof (RestoreBootSettings);
    Status  = gRT->GetVariable (
                     L"RestoreBootSettings",
                     &mAmtRestoreSecureBootGuid,
                     NULL,
                     &VarSize,
                     &RestoreBootSettings
                     );

    if (Status == EFI_SUCCESS &&
        RestoreBootSettings == RESTORE_SECURE_BOOT_ENABLED &&
        ManageSecureBootMode == RESTORE_SECURE_BOOT_STATE) {
      SecureBootState = SECURE_BOOT_ENABLE;
      Status          = gRT->SetVariable (
                               EFI_SECURE_BOOT_ENABLE_NAME,
                               &gEfiSecureBootEnableDisableGuid,
                               VarAttributes,
                               sizeof (SecureBootState),
                               &SecureBootState
                               );
      ASSERT_EFI_ERROR (Status);

      //
      // Delete RestoreBootSettings variable
      //
      Status = gRT->SetVariable (
                      L"RestoreBootSettings",
                      &mAmtRestoreSecureBootGuid,
                      0,
                      0,
                      NULL
                      );
      ASSERT_EFI_ERROR (Status);

      DEBUG ((DEBUG_INFO, "Secure Boot settings restored after Storage Redirection boot - Cold Reset!\n"));
      gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
    }
  } else {
    if (IsUsbrBoot && !EnforceSecureBoot && ManageSecureBootMode == DISABLE_SECURE_BOOT_STATE) {
      //
      // Secure boot needs to be disabled if we're doing Storage Redirection and EnforceSecureBoot not set
      //
      SecureBootState     = SECURE_BOOT_DISABLE;
      RestoreBootSettings = RESTORE_SECURE_BOOT_ENABLED;

      Status = gRT->SetVariable (
                      EFI_SECURE_BOOT_ENABLE_NAME,
                      &gEfiSecureBootEnableDisableGuid,
                      VarAttributes,
                      sizeof (SecureBootState),
                      &SecureBootState
                      );
      ASSERT_EFI_ERROR (Status);

      //
      // Set variable to restore previous secure boot state
      //
      Status = gRT->SetVariable (
                      L"RestoreBootSettings",
                      &mAmtRestoreSecureBootGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                      sizeof (RestoreBootSettings),
                      &RestoreBootSettings
                      );
      ASSERT_EFI_ERROR (Status);
      DEBUG ((DEBUG_INFO, "Secure Boot disabled for Storage Redirection boot\n"));

      //
      // When SecureBoot is disabled and a platform has a TPM enabled,
      // the platform SHALL extend an EV_EFI_ACTION event into PCR[7]
      //
      Status = TpmMeasureAndLogData (
                 7,
                 EV_EFI_ACTION,
                 SECURITY_EVENT_STRING,
                 (UINT32) (sizeof (SECURITY_EVENT_STRING) - 1),
                 SECURITY_EVENT_STRING,
                 sizeof (SECURITY_EVENT_STRING) - 1
                 );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "TpmMeasureAndLogData Status: %r\n", Status));
      }
    }
  }
}

/**
  Configure platform's Secure Boot State for AMT support and register the callback,
  which stops AMT Watchdog on Setup Entry and PXE boot.

  @retval EFI_SUCCESS           Initialization complete.
  @retval Others                Error occurred during the initialization.
**/
EFI_STATUS
AmtInitializePlatformForAmtSupport (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   PlatformSetupEvent;
  EFI_EVENT   PxeBootEvent;
  VOID        *Registration;
  VOID        *PxeRegistration;

  if (!AmtIsManageabilitySupportEnabled ()) {
    return EFI_UNSUPPORTED;
  }

  //
  // Restore SecureBoot state if it is changed for Storage Redirection boot
  //
  ManageSecureBootState (RESTORE_SECURE_BOOT_STATE);

  ///
  /// Whenever the user gets into BIOS setup, BIOS should stop the watchdog timer
  ///
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AmtStopWatchdog,
                  NULL,
                  &PlatformSetupEvent
                  );

  if (!EFI_ERROR (Status)) {
    Status = gBS->RegisterProtocolNotify (
                    &gSetupEnterGuid,
                    PlatformSetupEvent,
                    &Registration
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Failed to register callback on setup entry.\n"));
    }
  }

  ///
  /// When the user attempts to PXE boot, BIOS should stop the watchdog timer
  ///
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AmtStopWatchdog,
                  NULL,
                  &PxeBootEvent
                  );

  if (!EFI_ERROR (Status)) {
    Status = gBS->RegisterProtocolNotify (
                    &gEfiPxeBaseCodeCallbackProtocolGuid,
                    PxeBootEvent,
                    &PxeRegistration
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Failed to register callback on PXE callback protocol\n"));
    }
  }

  return Status;
}

/**
  Compare two device paths up to a size of Boot Option's Device Path

  @param[in] BootOptionDp     Device path acquired from BootXXXX EFI variable
  @param[in] FileSysDp        Device path acquired through EFI_SIMPLE_FILE_SYSTEM_PROTOCOL Handles buffer

  @retval TRUE                Both device paths point to the same device
  @retval FALSE               Device paths point to different devices
**/
BOOLEAN
CompareDevicePaths (
  IN  EFI_DEVICE_PATH_PROTOCOL *BootOptionDp,
  IN  EFI_DEVICE_PATH_PROTOCOL *FileSysDp
  )
{
  UINTN BootOptionDpSize;
  UINTN FileSysDpSize;

  if (BootOptionDp == NULL || FileSysDp == NULL) {
    return FALSE;
  }

  BootOptionDpSize = GetDevicePathSize (BootOptionDp) - END_DEVICE_PATH_LENGTH;
  FileSysDpSize    = GetDevicePathSize (FileSysDp) - END_DEVICE_PATH_LENGTH;

  if ((BootOptionDpSize <= FileSysDpSize) && (CompareMem (FileSysDp, BootOptionDp, BootOptionDpSize) == 0)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Get EFI device path through EFI_SIMPLE_FILE_SYSTEM_PROTOCOL Handles buffer. Acquired path must
  point to the same device as argument DevicePath passed to the function.

  @param[in] DevicePath              Device path acquired from BootXXXX EFI variable
  @param[in] EfiDeviceType           EFI device path type

  @retval EFI_DEVICE_PATH_PROTOCOL   Device path for booting
  @retval NULL                       No device path for booting
**/
EFI_DEVICE_PATH_PROTOCOL *
GetFullBootDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN UINTN                    EfiDeviceType
  )
{
  EFI_STATUS               Status;
  EFI_DEVICE_PATH_PROTOCOL *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL *ReturnDevicePath;
  UINTN                    HandleNum;
  EFI_HANDLE               *HandleBuf;
  UINTN                    Index;

  ReturnDevicePath = NULL;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  ((EfiDeviceType == MSG_MAC_ADDR_DP) ? &gEfiLoadFileProtocolGuid : &gEfiSimpleFileSystemProtocolGuid),
                  NULL,
                  &HandleNum,
                  &HandleBuf
                  );
  if ((EFI_ERROR (Status)) || (HandleBuf == NULL)) {
    return NULL;
  }

  for (Index = 0; Index < HandleNum; Index++) {
    TempDevicePath = DevicePathFromHandle (HandleBuf[Index]);

    if (CompareDevicePaths (DevicePath, TempDevicePath)) {
      ReturnDevicePath = DuplicateDevicePath (TempDevicePath);
      break;
    }
  }
  FreePool (HandleBuf);
  return ReturnDevicePath;
}

/**
  Translate ASF request type to EFI device path type

  @param[in] DeviceType     ASF request type

  @retval UINTN             Translated device type
**/
UINTN
GetBootDeviceType (
  IN UINTN    DeviceType
  )
{
  UINTN Type;

  Type = 0;

  switch (DeviceType) {
    case FORCE_PXE:
      Type = MSG_MAC_ADDR_DP;
      break;
    case FORCE_HARDDRIVE:
    case FORCE_SAFEMODE:
      Type = MEDIA_HARDDRIVE_DP;
      break;
    case FORCE_DIAGNOSTICS:
      Type = MEDIA_PIWG_FW_FILE_DP;
      break;
    case FORCE_CDDVD:
      Type = MEDIA_CDROM_DP;
      break;
    default:
      break;
  }

  return Type;
}

/**
  Build the Device Path for this boot selection

  @param[in] BootOptions   Boot Option data pointer
  @param[in] OptionCount   Boot Option count
  @param[in] DeviceType    Boot device whose device type
  @param[in] DeviceIndex   Boot device whose device index
  @param[in] UsbrBoot      If UsbrBoot is TRUE then check USBr device

  @retval BootDeviceIndex  Index of Boot Device
**/
UINTN
GetBootOptionIndex (
  IN EFI_BOOT_MANAGER_LOAD_OPTION *BootOptions,
  IN UINTN                        OptionCount,
  IN UINT16                       DeviceType,
  IN UINT16                       DeviceIndex,
  IN BOOLEAN                      UsbrBoot
  )
{
  EFI_DEVICE_PATH_PROTOCOL     *FullDevicePath;
  UINTN                        Index;
  UINTN                        TempIndex;
  EFI_DEVICE_PATH_PROTOCOL     *DevPathNode;
  EFI_DEVICE_PATH_PROTOCOL     *StorageRedirectionDevicePath;
  UINTN                        EfiDeviceType;
  BOOLEAN                      TypeMatched;
  INTN                         BootDeviceIndex;
  UINTN                        EfiNodeType;

  FullDevicePath   = NULL;
  TempIndex        = 1;
  EfiDeviceType    = GetBootDeviceType (DeviceType);
  TypeMatched      = FALSE;
  BootDeviceIndex  = -1;
  EfiNodeType      = (EfiDeviceType == MSG_MAC_ADDR_DP) ? MESSAGING_DEVICE_PATH : MEDIA_DEVICE_PATH;

  for (Index = 0; Index < OptionCount; Index++) {
    //
    // If this is EFI boot option, we need to get full device path from EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
    // to determine type of device
    //
    FullDevicePath = GetFullBootDevicePath (BootOptions[Index].FilePath, EfiDeviceType);
    if (FullDevicePath == NULL) {
      continue;
    }

    DevPathNode = FullDevicePath;

    //
    // Check if this is our requested boot device
    //
    while (!IsDevicePathEnd (DevPathNode)) {
      DEBUG ((DEBUG_INFO, "DevPathNode: %s\n", ConvertDevicePathToText (DevPathNode, FALSE, FALSE)));
      if (UsbrBoot) {
        StorageRedirectionDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AmtGetStorageRedirectionDevicePath ();
        if (StorageRedirectionDevicePath != NULL) {
          if (CompareDevicePaths (StorageRedirectionDevicePath, FullDevicePath)) {
            TypeMatched = TRUE;
          }
        }
      } else {
        //
        // EFI boot option
        //
        if (DevicePathType (DevPathNode) == EfiNodeType && DevicePathSubType (DevPathNode) == EfiDeviceType) {
          if (DeviceType == FORCE_DIAGNOSTICS) {
            //
            // If boot to EFI shell, find shell file by GUID
            //
            if (CompareMem (&((MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *)DevPathNode)->FvFileName, &gUefiShellFileGuid, sizeof (EFI_GUID))) {
              TypeMatched = TRUE;
            }
          } else {
            TypeMatched = TRUE;
          }
        } else if ((DeviceType == FORCE_HARDDRIVE) && (DevicePathSubType (DevPathNode) == MSG_USB_DP)) {
          DEBUG ((DEBUG_INFO, "Skip USB disk if system is forced to boot local hard drive\n"));
          break;
        }
      }

      if (TypeMatched) {
        //
        // Type matched, check for device index
        //
        if (!UsbrBoot && TempIndex < DeviceIndex) {
          TempIndex++;
          TypeMatched = FALSE;
          break;
        }

        BootDeviceIndex = Index;
        break;
      }

      DevPathNode = NextDevicePathNode (DevPathNode);
    }

    if (FullDevicePath != NULL) {
      FreePool (FullDevicePath);
    }

    if (BootDeviceIndex != -1) {
      break;
    }
  }

  return BootDeviceIndex;
}

/**
  Get ASF boot options index.

  @param[in] BootOptions               Boot Option data pointer
  @param[in] DeviceType                Boot device whose device type
  @param[in] OptionCount               Boot Option count

  @retval INTN                         Boot Option Index.
**/
INTN
GetAsfBootOptionIndex (
  IN EFI_BOOT_MANAGER_LOAD_OPTION *BootOptions,
  IN UINT8                        DeviceType,
  IN UINTN                        OptionCount
  )
{
  INTN                            BootOptionIndex;
  UINT8                           SpecialCmdParamHigh;
  UINT8                           SpecialCmdParamLow;

  BootOptionIndex     = -1;
  SpecialCmdParamHigh = AsfGetSpecialCmdParamHighByte ();
  SpecialCmdParamLow  = AsfGetSpecialCmdParamLowByte ();

  //
  // First we check ASF boot options Special Command
  //
  switch (DeviceType) {
    //
    // The Special Command Parameter can be used to specify a PXE
    // parameter. When the parameter value is 0, the system default PXE device is booted. All
    // other values for the PXE parameter are reserved for future definition by this specification.
    //
    case FORCE_PXE:
    case FORCE_DIAGNOSTICS:
      if ((SpecialCmdParamHigh != 0) || (SpecialCmdParamLow != 0)) {
        //
        // ASF spec says 0 currently is the only option
        //
        break;
      }

      //
      // We want everything connected up for PXE or EFI shell
      //
      EfiBootManagerConnectAll ();
      BootOptionIndex = GetBootOptionIndex (BootOptions, OptionCount, DeviceType, SpecialCmdParamHigh, FALSE);
      break;

    //
    // The Special Command Parameter identifies the boot-media index for
    // the managed client. When the parameter value is 0, the default hard-drive/optical drive is booted, when the
    // parameter value is 1, the primary hard-drive/optical drive is booted; when the value is 2, the secondary
    // hard-drive/optical drive is booted and so on.
    //
    case FORCE_HARDDRIVE:
    case FORCE_SAFEMODE:
    case FORCE_CDDVD:
      BootOptionIndex = GetBootOptionIndex (BootOptions, OptionCount, DeviceType, SpecialCmdParamHigh, FALSE);
      break;

    //
    // No additional special command is included; the Special Command Parameter has no meaning.
    //
    case NOP:
    default:
      break;
  }

  return BootOptionIndex;
}

/**
  Check Storage Redirection boot device and Asf boot device

  @param[in] BootOptions   Boot Option data pointer
  @param[in] OptionCount   Boot Option count

  @retval INTN             Boot Option Index.
**/
INTN
GetForcedBootOptionIndex (
  IN EFI_BOOT_MANAGER_LOAD_OPTION *BootOptions,
  IN UINTN                        OptionCount
  )
{
  INTN                            BootOptionIndex;
  UINT8                           SpecialCommand;

  BootOptionIndex = -1;
  SpecialCommand  = AsfGetSpecialCommand ();

  //
  // OEM command values; the interpretation of the Special Command and associated Special
  // Command Parameters is defined by the entity associated with the Enterprise ID.
  //
  if (AsfIsStorageRedirectionEnabled ()) {
    BootOptionIndex = GetBootOptionIndex (
                        BootOptions,
                        OptionCount,
                        FORCE_CDDVD,
                        0,
                        TRUE
                        );
  } else if (!AsfIsIndustryIanaId ()) {
    BootOptionIndex = GetAsfBootOptionIndex (
                        BootOptions,
                        SpecialCommand,
                        OptionCount
                        );
  }

  return BootOptionIndex;
}

/**
  Process ASF boot options and if available, attempt the boot

  @retval EFI_SUCCESS     The command completed successfully
  @retval EFI_NOT_FOUND   No boot options present
  @retval EFI_UNSUPPORTED No load option exist or not boot option index present
**/
EFI_STATUS
BdsBootViaAsf (
  VOID
  )
{
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOptions;
  UINTN                        OptionCount;
  INTN                         BootOptionIndex;
  EFI_INPUT_KEY                Key;
  UINTN                        EventIndex;
  EFI_TPL                      OldTpl;

  BootOptions = NULL;
  ZeroMem (&Key, sizeof (Key));

  //
  // Check if ASF Boot Options is present.
  //
  if (!AsfIsBootOptionsPresent ()) {
    return EFI_NOT_FOUND;
  }

  BootOptions = EfiBootManagerGetLoadOptions (&OptionCount, LoadOptionTypeBoot);
  if (BootOptions == NULL) {
    return EFI_UNSUPPORTED;
  }

  BootOptionIndex = GetForcedBootOptionIndex (BootOptions, OptionCount);
  //
  // If device path was set, the we have a boot option to use
  //
  if (BootOptionIndex == -1) {
    EfiBootManagerFreeLoadOptions (BootOptions, OptionCount);
    return EFI_UNSUPPORTED;
  }

  //
  // Manage secure boot before attempting Storage Redirection boot
  //
  ManageSecureBootState (DISABLE_SECURE_BOOT_STATE);

  //
  // If this is RCO/Storage Redirection EFI Boot, keep trying unless user cancels
  //
  while (!(Key.ScanCode == SCAN_ESC && Key.UnicodeChar == 0)) {
    EfiBootManagerBoot (&BootOptions[BootOptionIndex]);
    //
    // Returning from EfiBootManagerBoot means the boot failed
    // Display message to user before attempting another RCO/Storage Redirection boot
    //
    gST->ConOut->ClearScreen (gST->ConOut);
    gST->ConOut->OutputString (
                   gST->ConOut,
                   L"RCO/USBR boot failed. Press ENTER to try again or\r\nESC to return to regular boot or reset to restore secure boot state\r\n"
                   );
    Key.ScanCode    = 0;
    Key.UnicodeChar = 0;
    OldTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
    gBS->RestoreTPL (TPL_APPLICATION);
    while (!(Key.ScanCode == 0 && Key.UnicodeChar == L'\r')) {
      gBS->WaitForEvent (1, &(gST->ConIn->WaitForKey), &EventIndex);
      gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

      if (Key.ScanCode == SCAN_ESC && Key.UnicodeChar == 0) {
        break;
      }
    }

    if (OldTpl > TPL_APPLICATION) {
      gBS->RaiseTPL (OldTpl);
    }
  }

  EfiBootManagerFreeLoadOptions (BootOptions, OptionCount);
  ManageSecureBootState (RESTORE_SECURE_BOOT_STATE);
  return EFI_SUCCESS;
}

/**
  Function deleting ConIn Variable.
**/
VOID
DeleteConIn (
  VOID   **Variable,
  UINTN  *VariableSize
  )
{
  *Variable     = NULL;
  *VariableSize = 0;
}

/**
  Function updating ConIn Variable.
**/
VOID
AmtUpdateConsoleVariable (
  VOID
  )
{
  EFI_DEVICE_PATH_PROTOCOL *SolDevicePath;

  if (AsfIsConsoleLocked ()) {
    UpdateEfiGlobalVariable (L"ConIn", &mAmtConsoleVariableGuid, DeleteConIn);
  } else {
    UpdateEfiGlobalVariable (L"ConIn", &mAmtConsoleVariableGuid, NULL);
  }

  //
  // Add/Del AMT SOL device path to/from the ConIn, ConOut and StdErr variable when SOL is enabled/disabled.
  //
  SolDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AmtGetSolDevicePath ();
  if (SolDevicePath != NULL) {
    if (AsfIsSolEnabled ()) {
      EfiBootManagerUpdateConsoleVariable (ConIn, SolDevicePath, NULL);
      EfiBootManagerUpdateConsoleVariable (ConOut, SolDevicePath, NULL);
      EfiBootManagerUpdateConsoleVariable (ErrOut, SolDevicePath, NULL);
    } else {
      EfiBootManagerUpdateConsoleVariable (ConIn, NULL, SolDevicePath);
      EfiBootManagerUpdateConsoleVariable (ConOut, NULL, SolDevicePath);
      EfiBootManagerUpdateConsoleVariable (ErrOut, NULL, SolDevicePath);
    }
  }
}

/**
  Performs boot actions requested by AMT through Boot Options.
**/
VOID
AmtBdsBoot (
  VOID
  )
{
  UINTN  EventIndex;

  if (AsfIsPauseBootEnabled ()) {
    //
    // Wait until user presses any key when PauseBoot is requested
    //
    gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &EventIndex);
  } else if (AsfIsEnterSetupEnabled ()) {
    //
    // Enter to setup when SetupBoot is requested
    //
    BootUi ();
  } else if (AsfIsBootOptionsPresent ()) {
    BdsBootViaAsf ();
  }
}

/**
  AMT action when console is ready.
  Dynamically switch the output mode to 80x25 when SOL is enabled.
**/
VOID
AmtConsoleReady (
  VOID
  )
{
  EFI_STATUS              Status;
  INTN                    Index;
  UINTN                   Columns;
  UINTN                   Rows;
  INTN                    FailSafeMode;

  //
  // Set mode to 80 x 25
  //
  if (gST->ConOut == NULL) {
    return;
  }

  //
  // Add AMT SOL device path to the ConIn, ConOut and StdErr variable when SOL is enabled.
  //
  FailSafeMode = gST->ConOut->Mode->MaxMode;
  if (AsfIsSolEnabled ()) {
   for (Index = 0; Index < gST->ConOut->Mode->MaxMode; Index++) {
     Status = gST->ConOut->QueryMode (gST->ConOut, Index, &Columns, &Rows);
     if (!EFI_ERROR (Status) && (Columns == 80)) {
       if (Rows == 24) {
         break;
       }
       FailSafeMode = Index;
     }
   }

   if (Index == gST->ConOut->Mode->MaxMode) {
     //
     // When no 80x24 mode, use the 80x* mode.
     //
     Index = FailSafeMode;
   }

   if ((Index != gST->ConOut->Mode->MaxMode) && (Index != gST->ConOut->Mode->Mode)) {
     gST->ConOut->SetMode (gST->ConOut, (UINTN) Index);
   }
  }
}

/**
  Check if Remote Assistance Prompt String should be displayed.

  @retval TRUE              Remote Assistance prompt should be displayed.
  @retval FALSE             Remote Assistance prompt should not be displayed.
**/
BOOLEAN
AmtIsRemoteAssistancePromptSupported (
  VOID
  )
{
  return (AmtIsManageabilitySupportEnabled () && !AmtIsRemoteSessionEnabled () && IsCiraAvailable ());
}

/**
  Get Supported Hot Keys.

  @param[out] KeyCnt            Supported Hot Key Count.
  @param[out] KeyData           Supported Hot Key Data.

  @retval EFI_SUCCESS           Get Supported Hot Keys Successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of memory.
  @retval EFI_NOT_FOUND         No Supported Hot Keys.
**/
EFI_STATUS
AmtGetSupportedHotkeys (
  OUT  UINTN             *KeyCnt,
  OUT  EFI_KEY_DATA      **KeyData
  )
{
  EFI_KEY_DATA MeHotKeyTables[] = {
    //
    // Ctrl + Alt + F1
    //
    { {SCAN_F1, CHAR_NULL}, {EFI_SHIFT_STATE_VALID | EFI_RIGHT_CONTROL_PRESSED | EFI_RIGHT_ALT_PRESSED, 0} },
    { {SCAN_F1, CHAR_NULL}, {EFI_SHIFT_STATE_VALID | EFI_RIGHT_CONTROL_PRESSED | EFI_LEFT_ALT_PRESSED,  0} },
    { {SCAN_F1, CHAR_NULL}, {EFI_SHIFT_STATE_VALID | EFI_LEFT_CONTROL_PRESSED  | EFI_RIGHT_ALT_PRESSED, 0} },
    { {SCAN_F1, CHAR_NULL}, {EFI_SHIFT_STATE_VALID | EFI_LEFT_CONTROL_PRESSED  | EFI_LEFT_ALT_PRESSED,  0} }
  };

  *KeyCnt = 0;
  *KeyData = NULL;

  if (AmtIsManageabilitySupportEnabled () && !AsfIsConsoleLocked () && IsCiraAvailable ()) {
    //
    // CTRL + ALT + F1 hotkey
    //
    *KeyCnt = sizeof (MeHotKeyTables) / sizeof (MeHotKeyTables[0]);

    *KeyData = AllocateZeroPool (sizeof (EFI_KEY_DATA) * (*KeyCnt));
    if (*KeyData == NULL){
      return EFI_OUT_OF_RESOURCES;
    }

    CopyMem (*KeyData, MeHotKeyTables, sizeof (EFI_KEY_DATA) * (*KeyCnt));
    return EFI_SUCCESS;

  }
  return EFI_NOT_FOUND;
}

/**
  This function will change video resolution and text mode
  according to defined setup mode or defined boot mode

  @param[in]  IsSetupMode   Indicate mode is changed to setup mode or boot mode.

  @retval  EFI_SUCCESS      Mode is changed successfully.
  @retval  Others           Mode failed to be changed.
**/
EFI_STATUS
EFIAPI
BdsSetConsoleMode (
  IN BOOLEAN  IsSetupMode
  )
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *GraphicsOutput;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL       *SimpleTextOut;
  UINTN                                 SizeOfInfo;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *Info;
  UINT32                                MaxGopMode;
  UINT32                                MaxTextMode;
  UINT32                                ModeNumber;
  UINT32                                NewHorizontalResolution;
  UINT32                                NewVerticalResolution;
  UINT32                                NewColumns;
  UINT32                                NewRows;
  UINTN                                 HandleCount;
  EFI_HANDLE                            *HandleBuffer;
  EFI_STATUS                            Status;
  UINTN                                 Index;
  UINTN                                 CurrentColumn;
  UINTN                                 CurrentRow;

  MaxGopMode  = 0;
  MaxTextMode = 0;

  //
  // Get current video resolution and text mode
  //
  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID**)&GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
    GraphicsOutput = NULL;
  }

  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiSimpleTextOutProtocolGuid,
                  (VOID**)&SimpleTextOut
                  );
  if (EFI_ERROR (Status)) {
    SimpleTextOut = NULL;
  }

  if ((GraphicsOutput == NULL) || (SimpleTextOut == NULL)) {
    return EFI_UNSUPPORTED;
  }

  if (IsSetupMode) {
    //
    // The required resolution and text mode is setup mode.
    //
    NewHorizontalResolution = mSetupHorizontalResolution;
    NewVerticalResolution   = mSetupVerticalResolution;
    NewColumns              = mSetupTextModeColumn;
    NewRows                 = mSetupTextModeRow;
  } else {
    //
    // The required resolution and text mode is boot mode.
    //
    NewHorizontalResolution = mBootHorizontalResolution;
    NewVerticalResolution   = mBootVerticalResolution;
    NewColumns              = mBootTextModeColumn;
    NewRows                 = mBootTextModeRow;
  }

  if (GraphicsOutput != NULL) {
    MaxGopMode  = GraphicsOutput->Mode->MaxMode;
  }

  if (SimpleTextOut != NULL) {
    MaxTextMode = SimpleTextOut->Mode->MaxMode;
  }

  //
  // 1. If current video resolution is same with required video resolution,
  //    video resolution need not be changed.
  //    1.1. If current text mode is same with required text mode, text mode need not be changed.
  //    1.2. If current text mode is different from required text mode, text mode need be changed.
  // 2. If current video resolution is different from required video resolution, we need restart whole console drivers.
  //
  for (ModeNumber = 0; ModeNumber < MaxGopMode; ModeNumber++) {
    Status = GraphicsOutput->QueryMode (
                               GraphicsOutput,
                               ModeNumber,
                               &SizeOfInfo,
                               &Info
                               );
    if (!EFI_ERROR (Status)) {
      if ((Info->HorizontalResolution == NewHorizontalResolution) &&
          (Info->VerticalResolution == NewVerticalResolution)) {
        if ((GraphicsOutput->Mode->Info->HorizontalResolution == NewHorizontalResolution) &&
            (GraphicsOutput->Mode->Info->VerticalResolution == NewVerticalResolution)) {
          //
          // Current resolution is same with required resolution, check if text mode need be set
          //
          Status = SimpleTextOut->QueryMode (SimpleTextOut, SimpleTextOut->Mode->Mode, &CurrentColumn, &CurrentRow);
          ASSERT_EFI_ERROR (Status);
          if (CurrentColumn == NewColumns && CurrentRow == NewRows) {
            //
            // If current text mode is same with required text mode. Do nothing
            //
            FreePool (Info);
            return EFI_SUCCESS;
          } else {
            //
            // If current text mode is different from required text mode. Set new video mode
            //
            for (Index = 0; Index < MaxTextMode; Index++) {
              Status = SimpleTextOut->QueryMode (SimpleTextOut, Index, &CurrentColumn, &CurrentRow);
              if (!EFI_ERROR(Status)) {
                if ((CurrentColumn == NewColumns) && (CurrentRow == NewRows)) {
                  //
                  // Required text mode is supported, set it.
                  //
                  Status = SimpleTextOut->SetMode (SimpleTextOut, Index);
                  ASSERT_EFI_ERROR (Status);
                  //
                  // Update text mode PCD.
                  //
                  PcdSet32S (PcdConOutColumn, mSetupTextModeColumn);
                  PcdSet32S (PcdConOutRow, mSetupTextModeRow);
                  FreePool (Info);
                  return EFI_SUCCESS;
                }
              }
            }
            if (Index == MaxTextMode) {
              //
              // If required text mode is not supported, return error.
              //
              FreePool (Info);
              return EFI_UNSUPPORTED;
            }
          }
        } else {
          //
          // If current video resolution is not same with the new one, set new video resolution.
          // In this case, the driver which produces simple text out need be restarted.
          //
          Status = GraphicsOutput->SetMode (GraphicsOutput, ModeNumber);
          if (!EFI_ERROR (Status)) {
            FreePool (Info);
            break;
          }
        }
      }
      FreePool (Info);
    }
  }

  if (ModeNumber == MaxGopMode) {
    //
    // If the resolution is not supported, return error.
    //
    return EFI_UNSUPPORTED;
  }

  //
  // Set PCD to Inform GraphicsConsole to change video resolution.
  // Set PCD to Inform Consplitter to change text mode.
  //
  PcdSet32S (PcdVideoHorizontalResolution, NewHorizontalResolution);
  PcdSet32S (PcdVideoVerticalResolution, NewVerticalResolution);
  PcdSet32S (PcdConOutColumn, NewColumns);
  PcdSet32S (PcdConOutRow, NewRows);

  //
  // Video mode is changed, so restart graphics console driver and higher level driver.
  // Reconnect graphics console driver and higher level driver.
  // Locate all the handles with GOP protocol and reconnect it.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleTextOutProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < HandleCount; Index++) {
      gBS->DisconnectController (HandleBuffer[Index], NULL, NULL);
    }
    for (Index = 0; Index < HandleCount; Index++) {
      gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
    }
    if (HandleBuffer != NULL) {
      FreePool (HandleBuffer);
    }
  }

  return EFI_SUCCESS;
}

/**
  Event to restore original resolution.

  @param[in]    Event          Event whose notification function is being invoked
  @param[in]    Context        Context passed to this function.
**/
VOID
EFIAPI
AmtRestoreResolutionCallback (
  IN EFI_EVENT                    Event,
  IN VOID                         *Context
  )
{
  gBS->CloseEvent (Event);
  BdsSetConsoleMode (FALSE);
}


/**
  Update AMT DXE config block to enter remote assistance.
**/
VOID
RequestRemoteAssistanceEntry (
  VOID
  )
{
  EFI_STATUS             Status;
  AMT_POLICY_PROTOCOL    *AmtPolicy;
  AMT_DXE_CONFIG         *AmtDxeConfig;

  Status = gBS->LocateProtocol (&gDxeAmtPolicyGuid, NULL, (VOID **) &AmtPolicy);
  if (!EFI_ERROR (Status)) {
    Status = GetConfigBlock ((VOID *) AmtPolicy, &gAmtDxeConfigGuid, (VOID *) &AmtDxeConfig);
    ASSERT_EFI_ERROR (Status);
    AmtDxeConfig->CiraRequest = 1;
  }

  DEBUG ((DEBUG_INFO, "Entering Remote Assistance\n"));
}

/**
  Determine if CTRL key was pressed.

  @param[in] KeyShiftState  Key Data.

  @retval TRUE              CTRL key was pressed.
  @retval FALSE             CTRL key was not pressed.
**/
BOOLEAN
IsCtrlKey (
  IN UINT32 KeyShiftState
  )
{
  if ((KeyShiftState & EFI_SHIFT_STATE_VALID) && (KeyShiftState & (EFI_LEFT_CONTROL_PRESSED | EFI_RIGHT_CONTROL_PRESSED))) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Determine if ALT key was pressed.

  @param[in] KeyShiftState  Key Data.

  @retval TRUE              ALT key was pressed.
  @retval FALSE             ALT key was not pressed.
**/
BOOLEAN
IsAltKey (
  IN UINT32 KeyShiftState
  )
{
  if ((KeyShiftState & EFI_SHIFT_STATE_VALID) && (KeyShiftState & (EFI_RIGHT_ALT_PRESSED | EFI_LEFT_ALT_PRESSED))) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Determine if AMT hot key is pressed.
  Ctrl+ALT+F1  : Scan    = 0x0B, Shift = 0x3C

  @param[in] KeyData            Hot Key Data.

  @retval EFI_SUCCESS           AMT hot key is pressed.
  @retval EFI_NOT_FOUND         AMT hot key is not pressed.
**/
BOOLEAN
IsRemoteAssistanceHotKey (
  IN EFI_KEY_DATA                       *KeyData
  )
{
  DEBUG ((DEBUG_INFO, "<Scan=0x%X><Unicode=0x%X>\n", KeyData->Key.ScanCode, KeyData->Key.UnicodeChar));
  DEBUG ((DEBUG_INFO, "<ShiftState=0x%X><ToggleState=0x%X>\n", KeyData->KeyState.KeyShiftState, KeyData->KeyState.KeyToggleState));

  return ((KeyData->Key.ScanCode == SCAN_F1) &&
           IsCtrlKey (KeyData->KeyState.KeyShiftState) &&
           IsAltKey (KeyData->KeyState.KeyShiftState));
}

/**
  Notification function for keystrokes.

  @param[in] KeyData    The key that was pressed.

  @retval EFI_SUCCESS   The operation was successful.
  @retval Others        An error has occurred.
**/
EFI_STATUS
EFIAPI
AmtHotkeyCallback (
  IN EFI_KEY_DATA *KeyData
  )
{
  EFI_STATUS      Status;
  static BOOLEAN  IsRemoteAssistanceEntry = FALSE;
  EFI_EVENT       MeRestoreResolutionEvent;

  //
  // React to hotkey once per boot only
  //
  if (IsRemoteAssistanceEntry) {
    return EFI_NOT_FOUND;
  }

  //
  // Skip AMT hot key check if remote session is enabled.
  //
  if (AmtIsRemoteSessionEnabled ()) {
    return EFI_NOT_FOUND;
  }

  if (!IsRemoteAssistanceHotKey (KeyData)) {
    return EFI_NOT_FOUND;
  }

  IsRemoteAssistanceEntry = TRUE;
  RequestRemoteAssistanceEntry ();
  BdsSetConsoleMode (TRUE);

  //
  // Create callback function to restore boot resolution
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  AmtRestoreResolutionCallback,
                  NULL,
                  &gMePlatformReadyToBootGuid,
                  &MeRestoreResolutionEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Trigger ReadyToBoot event to launch MEBx,
  // If we change to launch MEBx not in ReadyToBoot event, we don't need to trigger the event here
  //
  EfiSignalEventReadyToBoot ();
  return EFI_SUCCESS;
}

/**
  Callback function for SimpleTextInEx protocol install events

  @param[in] Event           the event that is signalled.
  @param[in] Context         not used here.

**/
VOID
EFIAPI
PlatformBdsTxtInExCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                         Status;
  UINTN                              BufferSize;
  EFI_HANDLE                         Handle;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL  *TxtInEx;
  UINTN                              Count;
  UINTN                              Index;
  EFI_KEY_DATA                       *KeyData;
  EFI_HANDLE                         NotifyHandle;
  EFI_GRAPHICS_OUTPUT_PROTOCOL       *GraphicsOutput;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *SimpleTextOut;
  UINTN                              BootTextColumn;
  UINTN                              BootTextRow;

  if (!mModeInitialized) {
    //
    // At this moment, ConOut has been connected by BDS core, so get current video resolution
    // and text mode before launching setup at first time.
    //
    Status = gBS->HandleProtocol (
                    gST->ConsoleOutHandle,
                    &gEfiGraphicsOutputProtocolGuid,
                    (VOID**)&GraphicsOutput
                    );
    if (EFI_ERROR (Status)) {
      GraphicsOutput = NULL;
    }

    Status = gBS->HandleProtocol (
                    gST->ConsoleOutHandle,
                    &gEfiSimpleTextOutProtocolGuid,
                    (VOID**)&SimpleTextOut
                    );
    if (EFI_ERROR (Status)) {
      SimpleTextOut = NULL;
    }

    if (GraphicsOutput != NULL) {
      //
      // Get current video resolution and text mode.
      //
      mBootHorizontalResolution = GraphicsOutput->Mode->Info->HorizontalResolution;
      mBootVerticalResolution   = GraphicsOutput->Mode->Info->VerticalResolution;
    }

    if (SimpleTextOut != NULL) {
      SimpleTextOut->QueryMode (
                       SimpleTextOut,
                       SimpleTextOut->Mode->Mode,
                       &BootTextColumn,
                       &BootTextRow
                       );
      mBootTextModeColumn = (UINT32)BootTextColumn;
      mBootTextModeRow    = (UINT32)BootTextRow;
    }

    //
    // Get user defined text mode for setup.
    //
    mSetupHorizontalResolution = PcdGet32 (PcdSetupVideoHorizontalResolution);
    mSetupVerticalResolution   = PcdGet32 (PcdSetupVideoVerticalResolution);
    mSetupTextModeColumn       = PcdGet32 (PcdSetupConOutColumn);
    mSetupTextModeRow          = PcdGet32 (PcdSetupConOutRow);
    mModeInitialized           = TRUE;
  }

  Status = AmtGetSupportedHotkeys (&Count, &KeyData);
  if (EFI_ERROR (Status)) {
    return;
  }

  while (TRUE) {
    BufferSize = sizeof (EFI_HANDLE);
    Status = gBS->LocateHandle (
                    ByRegisterNotify,
                    NULL,
                    mRegistration,
                    &BufferSize,
                    &Handle
                    );
    if (EFI_ERROR (Status)) {
      //
      // If no more notification events exist
      //
      break;
    }

    Status = gBS->HandleProtocol (
                    Handle,
                    &gEfiSimpleTextInputExProtocolGuid,
                    (VOID **) &TxtInEx
                    );
    ASSERT_EFI_ERROR (Status);

    for (Index = 0; Index < Count; Index++) {
      Status = TxtInEx->RegisterKeyNotify (TxtInEx, &KeyData[Index], AmtHotkeyCallback, &NotifyHandle);
      DEBUG ((DEBUG_INFO, "[Bds]RegisterAmtKeyNotify: %04x/%04x %r\n", KeyData[Index].Key.ScanCode, KeyData[Index].Key.UnicodeChar, Status));
      if (EFI_ERROR (Status)) {
        continue;
      }
    }
  }

  if (KeyData != NULL) {
    FreePool (KeyData);
  }
}

/**
  Unregister Amt Hotkey.

  @param[in]  Event                 Event whose notification function is being invoked.
  @param[in]  Context               The pointer to the notification function's context,
                                    which is implementation-dependent.
**/
VOID
EFIAPI
AmtUnregisterHotKey (
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
  )
{
  EFI_STATUS                            Status;
  UINTN                                 Index;
  UINTN                                 KeyIndex;
  EFI_HANDLE                            *Handles;
  UINTN                                 HandleCount;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL     *TxtInEx;
  EFI_HANDLE                            NotifyHandle;
  EFI_KEY_DATA                          *KeyData;
  UINTN                                 Count;

  Status = AmtGetSupportedHotkeys (&Count, &KeyData);
  if (EFI_ERROR (Status)) {
    return;
  }

  gBS->LocateHandleBuffer (
         ByProtocol,
         &gEfiSimpleTextInputExProtocolGuid,
         NULL,
         &HandleCount,
         &Handles
         );
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (Handles[Index], &gEfiSimpleTextInputExProtocolGuid, (VOID **) &TxtInEx);
    if (EFI_ERROR (Status)) {
      continue;
    }
    for (KeyIndex = 0; KeyIndex < Count; KeyIndex++) {
      Status = TxtInEx->RegisterKeyNotify (
                          TxtInEx,
                          &KeyData[KeyIndex],
                          AmtHotkeyCallback,
                          &NotifyHandle
                          );
      if (!EFI_ERROR (Status)) {
        Status = TxtInEx->UnregisterKeyNotify (TxtInEx, NotifyHandle);
        if (EFI_ERROR (Status)) {
          continue;
        }
        DEBUG ((DEBUG_INFO, "[Bds]UnregisterAmtKeyNotify: %04x/%04x %r\n",
          KeyData[KeyIndex].Key.ScanCode, KeyData[KeyIndex].Key.UnicodeChar, Status));
      }
    }
  }

  //
  // Close Register and Unregister events as we're done with AMT hotkeys in this boot
  //
  if (KeyData != NULL) {
    FreePool (KeyData);
  }
  FreePool (Handles);
  gBS->CloseEvent (Event);
  gBS->CloseEvent (mHotkeyRegisteredEvent);
}

/**
  Register Amt Hotkey.
**/
VOID
AmtRegisterHotKey (
  VOID
  )
{
  VOID          *SetupRegistration;
  EFI_STATUS    Status;
  EFI_EVENT     Event;

  if (!AmtIsManageabilitySupportEnabled () || !IsCiraAvailable ()) {
    return;
  }

  //
  // Create event for registering the hotkeys on installed input consoles
  //
  mHotkeyRegisteredEvent = EfiCreateProtocolNotifyEvent (
                             &gEfiSimpleTextInputExProtocolGuid,
                             TPL_CALLBACK,
                             PlatformBdsTxtInExCallback,
                             NULL,
                             &mRegistration
                             );
  //
  // Register EnterSetup and ReadyToBoot events for removing AMT hotkeys
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AmtUnregisterHotKey,
                  NULL,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->RegisterProtocolNotify (
                  &gSetupEnterGuid,
                  Event,
                  &SetupRegistration
                  );
  ASSERT_EFI_ERROR (Status);

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             AmtUnregisterHotKey,
             NULL,
             &Event
             );
  ASSERT_EFI_ERROR (Status);
}


/**
  Check whether do full configuration enumeration for media asset table

  @param[in, out]   BootMode        A pointer to BootMode to be updated.

**/
VOID
AmtUpdateBootMode (
  IN OUT EFI_BOOT_MODE  *BootMode
  )
{
  EFI_STATUS            Status;
  ME_BIOS_PAYLOAD_HOB   *MbpHob;
  AMT_POLICY_PROTOCOL   *AmtPolicy;
  AMT_DXE_CONFIG        *AmtDxeConfig;

  Status = gBS->LocateProtocol (&gDxeAmtPolicyGuid, NULL, (VOID **) &AmtPolicy);
  if (!EFI_ERROR (Status)) {
    Status = GetConfigBlock ((VOID *) AmtPolicy, &gAmtDxeConfigGuid, (VOID *) &AmtDxeConfig);
    ASSERT_EFI_ERROR (Status);
    if ((BOOLEAN) AmtDxeConfig->UsbProvision == TRUE) {
      *BootMode = BOOT_WITH_FULL_CONFIGURATION;
      DEBUG ((DEBUG_INFO, "Boot exception raised for ME Usb Provision, do full media discovery.\n"));
      return;
    }
  }

  //
  // Get Mbp Hob
  //
  MbpHob = NULL;
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob != NULL) {
    //
    // During FastBoot, when the BIOS detects an Indication from the ME Firmware,the BIOS shall
    // enumerate all media devices and send all asset tables to the ME Firmware.
    //
    if ((MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType == IntelMeCorporateFw) &&
        (MbpHob->MeBiosPayload.HwaRequest.Available == TRUE) &&
        (MbpHob->MeBiosPayload.HwaRequest.Data.Fields.MediaTablePush == TRUE)) {
      *BootMode = BOOT_WITH_FULL_CONFIGURATION;
      DEBUG ((DEBUG_INFO, "Boot exception raised for ME MediaTablePush, do full media discovery.\n"));
      return;
    }
  }

  if (AsfIsBootOptionsPresent ()) {
    *BootMode = BOOT_WITH_FULL_CONFIGURATION;
  }
}

/**
  Restart ASF BIOS Watchdog after third party option ROM is complete.
**/
VOID
AmtStartWatchdog (
  VOID
  )
{
  AsfStartWatchDog (ASF_START_BIOS_WDT);
}

/**
  Checks if device is in a One Click Recovery boot flow. Will get the boot capabilities
  from ME setup data and pass too OCR_CONFIG use in OneClickRecovery feature. Then will
  call the OCR feature for operation, if in a recovery boot.
**/
VOID
AmtPerformOneClickRecoveryBoot (
  VOID
  )
{
  ONE_CLICK_RECOVERY_PROTOCOL   *OneClickRecoveryProtocol;
  UINT32                        OcrMeSetupAttr;
  ME_SETUP                      MeSetup;
  UINTN                         VarSize;
  EFI_STATUS                    Status;
  OCR_CONFIG                    OcrConfig;

  Status = gBS->LocateProtocol (&gOneClickRecoveryProtocolGuid, NULL, (VOID **)&OneClickRecoveryProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[%a] OCR protocol GUID not found with status %r\n", __FUNCTION__, Status));
    return;
  }

  // Send MeSetup data for OCR support to OCR setup data
  VarSize = sizeof (ME_SETUP);
  Status = gRT->GetVariable (
                  L"MeSetup",
                  &gMeSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &MeSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Get Me Setup variables for OCR with status %r\n", __FUNCTION__, Status));
    return;
  }

  VarSize = sizeof (OCR_CONFIG);
  Status = gRT->GetVariable (
                  OCR_CONFIG_VARIABLE_NAME,
                  &gOcrConfigVariableGuid,
                  &OcrMeSetupAttr,
                  &VarSize,
                  &OcrConfig
                  );
  if (EFI_ERROR (Status)) {
    OcrMeSetupAttr = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
  }

  OcrConfig.OcrConfigBootPba        = MeSetup.OcrBootPba;
  OcrConfig.OcrConfigBootWinRe      = MeSetup.OcrBootWinRe;
  OcrConfig.OcrConfigBootHttps      = MeSetup.OcrBootHttps;
  OcrConfig.OcrConfigAmtDisSecBoot  = MeSetup.OcrAmtDisSecBoot;

  Status = gRT->SetVariable (
                  OCR_CONFIG_VARIABLE_NAME,
                  &gOcrConfigVariableGuid,
                  OcrMeSetupAttr,
                  VarSize,
                  &OcrConfig
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to initiate/set OCR config variable with status %r\n", __FUNCTION__, Status));
    return;
  }

  // Set the Boot Options list from OEM to OCR
  Status = OneClickRecoveryProtocol->SetOcrBootOptionList ((BOOT_OPTIONS *) &OemBootOptionsList);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Start OCR boot flow
  Status = OneClickRecoveryProtocol->OcrEntry ();
  DEBUG ((DEBUG_INFO, "OneClickRecovery Status: %r\n", Status));
}
