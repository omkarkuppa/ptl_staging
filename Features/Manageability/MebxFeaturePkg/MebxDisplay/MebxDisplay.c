/** @file
  In this file there are implemented reference functions for Mebx Display Protocol.

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
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TimerLib.h>
#include <Library/HobLib.h>
#include <Library/MeUtilsLib.h>
#include <MeBiosPayloadHob.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/MebxDisplayProtocol.h>

#define MAX_STRING_LENGTH      256
#define DISPLAY_TEXT_DELAY     0
#define DISPLAY_ERROR_DELAY    2000000
#define DISPLAY_ERROR_ID_DELAY 4000000

typedef struct {
  MEBX_DISPLAY_PROTOCOL MebxDisplay;
  SCREEN_MODE           LastScreenMode;
  UINT32                BiosHorizontalResolution;
  UINT32                BiosVerticalResolution;
  UINT32                BiosColumns;
  UINT32                BiosRows;
} MEBX_DISPLAY_PRIVATE_PROTOCOL;

/**
  This function returns string which is related to specified message id.

  @param[in]  MsgId  The identifier of message.

  @retval NULL  The string specified by MsgId wasn't found.
  @retval Other The string was returned.
**/
STATIC
EFI_STRING
GetStringForMsgId (
  IN MEBX_MSG_ID    MsgId
  )
{
  CHAR16 *Text;

  Text = NULL;

  switch (MsgId) {
    case RecordInvalidError:
      Text = L"Provisioning data record header is invalid";
      break;
    case LoginFailedError:
      Text = L"Intel(R) ME login with current password failed";
      break;
    case ProvDataMissingError:
      Text = L"Provisioning data missing current Intel(R) ME password";
      break;
    case FailedEnabledFeaturesError:
      Text = L"Failed to get enabled features from FW";
      break;
    case FailedUpdateManageabilityModeError:
      Text = L"Failed to update manageability mode.";
      break;
    case AppFailedLoadError:
      Text = L"Application failed to load";
      break;
    case FwUpdateError:
      Text = L"Error applying FW Update";
      break;
    case FwUpdateDeprecatedError:
      Text = L"FW Update Password Protected State deprecated. Assuming disabled state";
      break;
    case FailedGetProvStatusError:
      Text = L"Failed to get provision status";
      break;
    case PkiDnsSuffixInvalidError:
      Text = L"Error: PKI DNS Suffix invalid";
      break;
    case PkiDnsSuffixError:
      Text = L"Error applying PKI DNS Suffix";
      break;
    case ConfigServerFqdnInvalidError:
      Text = L"Error: Configuration Server FQDN invalid";
      break;
    case RemoteConfigEnDisInvalidError:
      Text = L"Error: Remote Configuration en/dis invalid";
      break;
    case RemoteConfigEnDisError:
      Text = L"Error applying Remote Configuration En/Dis";
      break;
    case DefaultHashEnInvalidError:
      Text = L"Error: Default Hash enable invalid";
      break;
    case DefaultHashEnError:
      Text = L"Error applying Default Hash enable";
      break;
    case DefaultHashDisError:
      Text = L"Error applying Default Hash disable";
      break;
    case CustomHashConfigInvalidError:
      Text = L"Error: Custom Hash config invalid.";
      break;
    case CustomHashConfigError:
      Text = L"Error applying custom hash config";
      break;
    case DeleteHashError:
      Text = L"Error applying delete hash?";
      break;
    case CustomHashStateError:
      Text = L"Error applying custom hash state";
      break;
    case InvalidCustomHashError:
      Text = L"Error: Invalid Custom Hash Entry";
      break;
    case SolStorageRedirConfigInvalidError:
      Text = L"Error: SOL/Storage Redirection config invalid.";
      break;
    case SolStorageRedirDataError:
      Text = L"Error applying SOL/Storage Redirection data";
      break;
    case SolStorageRedirAuthProtError:
      Text = L"Error applying SOL/Storage Redirection Auth protocol. Setting no longer supported";
      break;
    case HostNameLargeError:
      Text = L"Error: Host Name is too large.";
      break;
    case DomainNameLargeError:
      Text = L"Error: Domain Name is too large.";
      break;
    case DhcpInvalidError:
      Text = L"Error: DHCP invalid.";
      break;
    case IdleTimeoutInvalidError:
      Text = L"Error: Idle timeout invalid.";
      break;
    case IdleTimeoutError:
      Text = L"Error applying Idle Timeout";
      break;
    case ProvServerInvalidError:
      Text = L"Error: Provisioning server address invalid.";
      break;
    case ProvServerPortInvalidError:
      Text = L"Error: Provisioning server port invalid.";
      break;
    case Ipv4ParamsInvalidError:
      Text = L"Error: IPV4 parameters invalid.";
      break;
    case PwdPolicyInvalidError:
      Text = L"Error: password policy data invalid.";
      break;
    case Ipv6DataInvalidError:
      Text = L"Error: IPV6 data invalid.";
      break;
    case Ipv6SettingError:
      Text = L"Error applying IPV6 Settings";
      break;
    case SharedFqdnInvalidError:
      Text = L"Error: Shared FQDN invalid.";
      break;
    case DdnsInvalidError:
      Text = L"Error: DDNS value invalid.";
      break;
    case KvmStateInvalidError:
      Text = L"Error: KVM state invalid.";
      break;
    case KvmStateError:
      Text = L"Error applying KVM State";
      break;
    case OptInDataInvalidError:
      Text = L"Error: Opt-in user data invalid.";
      break;
    case MeProvHaltDataInvalidError:
      Text = L"Error: ME provisioning Halt data invalid.";
      break;
    case MeProvHaltError:
      Text = L"ME Provisioning halt";
      break;
    case ManualSetupConfDataInvalidError:
      Text = L"Error: Manual setup and config data invalid.";
      break;
    case ProvServAddressError:
      Text = L"Error applying provisioning server address";
      break;
    case HashHandlesError:
      Text = L"Error applying hash handles";
      break;
    case HashEntriesError:
      Text = L"Error applying hash entries";
      break;
    case AddCustomHashError:
      Text = L"Error applying add custom hash";
      break;
    case MeProvActivateError:
      Text = L"Error applying ME Provisioning activate";
      break;
    case Ipv4ParametersError:
      Text = L"Error applying IPV4 parameters";
      break;
    case FqdnSettingError:
      Text = L"Error applying FQDN Settings";
      break;
    case OptinDataError:
      Text = L"Error applying opt-in data";
      break;
    case CompleteConfigFailedError:
      Text = L"Error: Complete configuration failed.";
      break;
    case NewPasswordError:
      Text = L"Error applying new password";
      break;
    case PowerPackagesError:
      Text = L"Error applying Power Packages";
      break;
    case ConinError:
      Text = L"CONIN_ERROR";
      break;
    case AmthiGetAmthiInterfaceVersionApiError:
      Text = L"Get Amthi Interface Version Error";
      break;
    case AmthiGetKvmStateApiError:
      Text = L"Get KVM State Error";
      break;
    case AmthiGetPolicyAmtRedirectionStateApiError:
      Text = L"Get Redirection State Policy Error";
      break;
    case AmthiGetOptinStateApiError:
      Text = L"Get Opt-In State Error";
      break;
    case AmthiGetConfigSvrDataApiError:
      Text = L"Get Configuration Server Data Error";
      break;
    case AmthiGetZtcConfigApiError:
      Text = L"Get ZTC Configuration Error";
      break;
    case AmthiGetPkiFqdnSuffixApiError:
      Text = L"Get PKI FQDN Suffix Error";
      break;
    case AmthiGetIpv4ParamsApiError:
      Text = L"Get IPV4 Parameters Error";
      break;
    case AmthiGetFqdnApiError:
      Text = L"Get FQDN Data Error";
      break;
    case AmthiGetIdleTimeoutApiError:
      Text = L"Get Idle Timeout Error";
      break;
    case AmthiGetProvisionStateApiError:
      Text = L"Get Provision State Error";
      break;
    case AmthiGetAuditRecordApiError:
      Text = L"Get Audit Record Error";
      break;
    case AmthiGetHashDataApiError:
      Text = L"Get Hash Data Error";
      break;
    case AmthiGetConnectionStatusApiError:
      Text = L"Get Connection Status Error";
      break;
    case AmthiGetIpv6LanIntfSettingsApiError:
      Text = L"Get IPv6 Lan Interface Settings Error";
      break;
    case AmthiGetPrivacyLevelApiError:
      Text = L"Get Privacy Level Error";
      break;
    case AmthiGetPowerPolicyApiError:
      Text = L"Get ME Power Policy Error";
      break;
    case AmthiSetMebxEnterStateApiError:
      Text = L"Set MEBx Enter State Error";
      break;
    case AmthiSetMebxExitStateApiError:
      Text = L"Set MEBx Exit State Error";
      break;
    case KvmActiveSessionMsg:
      Text = L"Active KVM Session - No MEBx";
      break;
    case AmthiSetIpv4ParamsApiError:
      Text = L"Set IPV4 Parameters Error";
      break;
    case AmthiSetPkiFqdnSuffixApiError:
      Text = L"Set PKI FQDN Suffix Error";
      break;
    case AmthiSetSolStorageRedirectionStateApiError:
      Text = L"Set Sol/Storage Redirection State Error";
      break;
    case AmthiSetKvmStateApiError:
      Text = L"Set KVM State Error";
      break;
    case AmthiSetPwdPolicyApiError:
      Text = L"Set Password Policy Error";
      break;
    case AmthiSetFqdnApiError:
      Text = L"Set FQDN Data Error";
      break;
    case AmthiSetOptinStateApiError:
      Text = L"Set Opt-In State Error";
      break;
    case AmthiSetIdleTimeoutApiError:
      Text = L"Set Idle Timeout Error";
      break;
    case AmthiSetConfigServerApiError:
      Text = L"Set Config Server Error";
      break;
    case AmthiPowerPolicyApiError:
      Text = L"Intel(R) ME Power Policy Update Error";
      break;
    case AmthiSetHashStateApiError:
      Text = L"Update Hash Active State Error";
      break;
    case AmthiSetZtcApiError:
      Text = L"Set Remote Configuration Error";
      break;
    case AmthiStopConfigUnprovisionApiError:
      Text = L"Halt Configuration - Get Unprovision State Error";
      break;
    case MeGetUserCapsApiError:
      Text = L"Get User Caps Error";
      break;
    case MeGetFwCapsApiError:
      Text = L"Get FW  Caps Error";
      break;
    case MeGetFwEnabledFeatureApiError:
      Text = L"Get FW Enabled Feature Error";
      break;
    case MeMeWaitFwFeatureAvailableApiError:
      Text = L"ME Wait FW Feature Available Error";
      break;
    case MeSetAmtStateApiError:
      Text = L"Set AMT State Error";
      break;
    case CoreUnconfigWoPassGetUnconfigStatusError:
      Text = L"Unconfigure Without Password - Get Unconfig Status Error";
      break;
    case CoreUsbProvError:
      Text = L"USB Provision flow Error";
      break;
    case AmthiCloseUserInitiatedConnApiError:
      Text = L"Close User Initiated Connection Error";
      break;
    case PressAnyKeyMsg:
      Text = L"Press any key to continue... ";
      break;
    case FoundUsbKeyMsg:
      Text = L"Found USB Key for provisioning.";
      break;
    case ContinueAutoProvMsg:
      Text = L"Continue with Auto Provisioning (Enter/Esc)?\r\n";
      break;
    case LoadingAmtMsg:
      Text = L"Loading Intel(R) AMT...";
      break;
    case DoneMsg:
      Text = L"Done. Please wait.";
      break;
    case AmtManageabilityUsbDataMissingMsg:
      Text = L"Intel(R) AMT is disabled and USB data is missing Manageability Feature Selection.\r\nNetwork and provisioning settings will not be applied.";
      break;
    case AmtManageabilityAlreadyProvMsg:
      Text = L"Intel(R) AMT is already provisioned.\r\nProvisioning related messages may fail.";
      break;
    case StringNotAppliedTooManyHashesMsg:
      Text = L"User defined certificate hashes not applied. Please delete existing user defined certificate hashes and try again.";
      break;
    case ConfigAppliedMsg:
      Text = L"Configuration Settings from the USB file were successfully applied.";
      break;
    case ContinueBootMsg:
      Text = L"Press any key to continue with system boot...";
      break;
    case CoreCautionMsg:
      Text = L"[CAUTION]";
      break;
    case CoreCpuReplacementMsg:
      Text = L"CPU Replacement Was Detected.";
      break;
    case CoreFeaturesDisabledMsg:
      Text = L"Some System Features Will Be Disabled.";
      break;
    case CoreFeaturesEnabledMsg:
      Text = L"Additional System Features Have Been Enabled.";
      break;
    case CoreConfirm1Msg:
      Text = L"Press Enter to Continue (System Will Go Through Reset to Complete CPU Change)";
      break;
    case CoreConfirm2Msg:
      Text = L"Otherwise Shutdown the Platform and Replace Original CPU.";
      break;
    case AmtManageabilityCilaMsg:
      Text = L"Within the Intranet, Intel(R) AMT will attempt to send an alert to your system\n\radministrator...\n\n\r";
      break;
    case CoreUnconfigWoPassMsg:
      Text = L"Found unconfigure of Intel(R) ME\r\nPlatform will continue with unconfiguration shortly...\r\n";
      break;
    case AmtManualConfigUnsupportedMsg:
      Text = L"Manual configuration no longer supported. USB configuration record has not been applied.\n";
      break;
    default:
      Text = NULL;
  }

  return Text;
}

/**
  This function prints MEBx information string to screen.

  @param[in,out] This      Pointer to MEBx Display Protocol.
  @param[in]     MsgId     The identifier of message.

  @retval EFI_SUCCESS      Text was displayed successfully
  @retval EFI_NOT_FOUND    The string specified by MsgId wasn't found.
**/
EFI_STATUS
EFIAPI
MebxDisplayText (
  IN OUT MEBX_DISPLAY_PRIVATE_PROTOCOL *This,
  IN     MEBX_MSG_ID                   MsgId
  )
{
  EFI_STRING    String;

  String = GetStringForMsgId (MsgId);

  if (String == NULL) {
    return EFI_NOT_FOUND;
  }

  gST->ConOut->OutputString (gST->ConOut, String);
  gST->ConOut->OutputString (gST->ConOut, L"\r\n");
  MicroSecondDelay (DISPLAY_TEXT_DELAY);

  return EFI_SUCCESS;
}

/**
  This function prints MEBx error string to screen and to console.

  @param[in,out] This      Pointer to MEBx Display Protocol.
  @param[in]     MsgId     The identifier of message.
  @param[in]     Delay     The amount of time for which the text will be displayed in microseconds.
                           If value is 0, the text is displayed in a fixed microseconds.

  @retval EFI_SUCCESS      Text was displayed successfully.
  @retval EFI_NOT_FOUND    The string specified by MsgId wasn't found.
**/
EFI_STATUS
EFIAPI
MebxDisplayError (
  IN OUT MEBX_DISPLAY_PRIVATE_PROTOCOL *This,
  IN     MEBX_MSG_ID                   MsgId,
  IN     UINT32                        Delay
  )
{
  EFI_STRING    String;
  CHAR16        TmpUniString[MAX_STRING_LENGTH];
  UINT8         ModuleId;
  UINT8         VariableId;
  UINT32        TimerDelay;

  if (MsgId >= CorruptedDataEntryStart && MsgId <= CorruptedDataEntryEnd) {
    ModuleId   = (MsgId >> 8) & 0xFF;
    VariableId = MsgId & 0xFF;

    UnicodeSPrint (
      TmpUniString,
      MAX_STRING_LENGTH,
      L"Corrupted data entry found (Module ID/Variable ID): %d/%d\r\n",
      (UINTN)ModuleId,
      (UINTN)VariableId
      );
    TimerDelay = DISPLAY_ERROR_ID_DELAY;
    gST->ConOut->OutputString (gST->ConOut, TmpUniString);
  } else {
    String = GetStringForMsgId (MsgId);

    if (String == NULL) {
      return EFI_NOT_FOUND;
    }

    DEBUG ((DEBUG_ERROR, "Error occurred: %s\n", String));

    gST->ConOut->OutputString (gST->ConOut, String);
    gST->ConOut->OutputString (gST->ConOut, L"\r\n");
    TimerDelay = DISPLAY_ERROR_DELAY;
  }
  if (Delay != 0) {
    TimerDelay = Delay;
  }
  MicroSecondDelay (TimerDelay);

  return EFI_SUCCESS;
}

/**
  This function sets specified graphics mode.

  @param[in,out] This      Pointer to MEBx Display Protocol
  @param[in]     Mode      Graphics mode to set:
                             TRUE  - BIOS mode
                             FALSE - MEBx mode

  @retval EFI_SUCCESS      New graphics mode set successfully
  @retval Others           Error has occurred
**/
EFI_STATUS
EFIAPI
MebxConfigureScreen (
  IN OUT MEBX_DISPLAY_PRIVATE_PROTOCOL *This,
  IN     SCREEN_MODE                   ScreenMode
  )
{
  EFI_STATUS                           Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL         *GraphicsOutput;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL      *SimpleTextOut;
  UINT32                               GraphicsMode;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *ModeInfo;
  UINTN                                ModeInfoSize;
  UINT32                               NewHorizontalResolution;
  UINT32                               NewVerticalResolution;
  UINTN                                CurrentColumn;
  UINTN                                CurrentRow;
  UINT32                               NewColumns;
  UINT32                               NewRows;
  UINTN                                Index;
  UINTN                                HandleCount;
  EFI_HANDLE                           *HandleBuffer;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gEfiGraphicsOutputProtocolGuid,
                  NULL,
                  (VOID**) &GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (
                  &gEfiSimpleTextOutProtocolGuid,
                  NULL,
                  (VOID**) &SimpleTextOut
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (This->LastScreenMode == ScreenMode) {
    //
    // Current mode is target mode, return directly.
    //
    return EFI_SUCCESS;
  }

  if (ScreenMode == ScreenModeMebx) {
    This->BiosHorizontalResolution = GraphicsOutput->Mode->Info->HorizontalResolution;
    This->BiosVerticalResolution   = GraphicsOutput->Mode->Info->VerticalResolution;

    Status = SimpleTextOut->QueryMode (SimpleTextOut, SimpleTextOut->Mode->Mode, &CurrentColumn, &CurrentRow);
    if (!EFI_ERROR (Status)) {
      This->BiosColumns            = (UINT32) CurrentColumn;
      This->BiosRows               = (UINT32) CurrentRow;
    }
  }

  if (ScreenMode == ScreenModeBios) {
    NewHorizontalResolution = This->BiosHorizontalResolution;
    NewVerticalResolution   = This->BiosVerticalResolution;
    NewColumns              = This->BiosColumns;
    NewRows                 = This->BiosRows;
  } else {
    NewHorizontalResolution = PcdGet32 (PcdMebxVideoHorizontalResolution);
    NewVerticalResolution   = PcdGet32 (PcdMebxVideoVerticalResolution);
    NewColumns              = PcdGet32 (PcdMebxConOutColumn);
    NewRows                 = PcdGet32 (PcdMebxConOutRow);
  }

  // set vendor-requested resolution for the user consent screen
  for (GraphicsMode = 0; GraphicsMode < GraphicsOutput->Mode->MaxMode; GraphicsMode++) {
    Status = GraphicsOutput->QueryMode (
                               GraphicsOutput,
                               GraphicsMode,
                               &ModeInfoSize,
                               &ModeInfo
                               );

    // make sure returned size of structure doesn't differ from the fixed size
    ASSERT (ModeInfoSize == sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));

    if (!EFI_ERROR (Status)) {
      if ((ModeInfo->HorizontalResolution == NewHorizontalResolution) &&
          (ModeInfo->VerticalResolution == NewVerticalResolution)) {
        if ((GraphicsOutput->Mode->Info->HorizontalResolution == NewHorizontalResolution) &&
            (GraphicsOutput->Mode->Info->VerticalResolution == NewVerticalResolution)) {

          Status = SimpleTextOut->QueryMode (SimpleTextOut, SimpleTextOut->Mode->Mode, &CurrentColumn, &CurrentRow);
          if (CurrentColumn == NewColumns && CurrentRow == NewRows) {
            //
            // If current text mode is same with required text mode. Do nothing
            //
            FreePool (ModeInfo);
            This->LastScreenMode = ScreenMode;
            return EFI_SUCCESS;
          } else {
            //
            // If current text mode is different from required text mode.  Set new video mode
            //
            for (Index = 0; Index < SimpleTextOut->Mode->MaxMode; Index++) {
              Status = SimpleTextOut->QueryMode (SimpleTextOut, Index, &CurrentColumn, &CurrentRow);
              if (!EFI_ERROR (Status)) {
                if ((CurrentColumn == NewColumns) && (CurrentRow == NewRows)) {
                  //
                  // Required text mode is supported, set it.
                  //
                  Status = SimpleTextOut->SetMode (SimpleTextOut, Index);
                  ASSERT_EFI_ERROR (Status);

                  FreePool (ModeInfo);
                  This->LastScreenMode = ScreenMode;
                  return EFI_SUCCESS;
                }
              }
            }
            if (Index == SimpleTextOut->Mode->MaxMode) {
              //
              // If required text mode is not supported, return error.
              //
              FreePool (ModeInfo);
              return EFI_UNSUPPORTED;
            }
          }
        } else {
          //
          // If current video resolution is not same with the new one, set new video resolution.
          // In this case, the driver which produces simple text out need be restarted.
          //
          Status = GraphicsOutput->SetMode (GraphicsOutput, GraphicsMode);
          if (!EFI_ERROR (Status)) {
            FreePool (ModeInfo);
            break;
          }
        }
      }
      FreePool (ModeInfo);
    }
  }

  if (GraphicsMode == GraphicsOutput->Mode->MaxMode) {
    //
    // If the resolution is not supported, return error.
    //
    return EFI_UNSUPPORTED;
  }

  //
  // Set PCD to Inform GraphicsConsole to change video resolution.
  // Set PCD to Inform Consplitter to change text mode.
  //
  Status = PcdSet32S (PcdVideoHorizontalResolution, NewHorizontalResolution);
  ASSERT_EFI_ERROR (Status);
  Status = PcdSet32S (PcdVideoVerticalResolution, NewVerticalResolution);
  ASSERT_EFI_ERROR (Status);
  Status = PcdSet32S (PcdConOutColumn, NewColumns);
  ASSERT_EFI_ERROR (Status);
  Status = PcdSet32S (PcdConOutRow, NewRows);
  ASSERT_EFI_ERROR (Status);

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

  This->LastScreenMode = ScreenMode;
  return EFI_SUCCESS;
}

/**
  This function draws image on the graphics screen.

  @param[in,out] This           Pointer to MEBx Display Protocol
  @param[in]     Bitmap         The data to transfer to the graphics screen
  @param[in]     ImageWidth     The width of image
  @param[in]     ImageHeight    The height of image

  @retval EFI_SUCCESS           Image was displayed successfully
  @retval EFI_INVALID_PARAMETER Invalid parameter
**/
EFI_STATUS
EFIAPI
MebxDisplayImage (
  IN OUT MEBX_DISPLAY_PRIVATE_PROTOCOL *This,
  IN     UINT8                         *Bitmap,
  IN     UINT32                        ImageWidth,
  IN     UINT32                        ImageHeight
  )
{
  EFI_STATUS                           Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL         *GraphicsOutput;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL        BlankPixel;
  UINT32                               ScreenWidth;
  UINT32                               ScreenHeight;
  UINT32                               DestX;
  UINT32                               DestY;

  ZeroMem (&BlankPixel, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

  if (This == NULL || Bitmap == NULL || ImageWidth == 0 || ImageHeight == 0) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gEfiGraphicsOutputProtocolGuid,
                  NULL,
                  (VOID**) &GraphicsOutput
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  ScreenHeight = GraphicsOutput->Mode->Info->VerticalResolution;
  ScreenWidth  = GraphicsOutput->Mode->Info->HorizontalResolution;

  Status = GraphicsOutput->Blt (GraphicsOutput, &BlankPixel, EfiBltVideoFill, 0, 0, 0, 0, ScreenWidth, ScreenHeight, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Clearing Screen, status = %r \n", __FUNCTION__, Status));
    return Status;
  }

  DestX = (ScreenWidth - ImageWidth) / 2;
  DestY = (ScreenHeight - ImageHeight) / 2;

  Status = GraphicsOutput->Blt (
                             GraphicsOutput,
                             (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*) Bitmap,
                             EfiBltBufferToVideo,
                             0,
                             0,
                             DestX,
                             DestY,
                             ImageWidth,
                             ImageHeight,
                             ImageWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Drawing Image, status = %r \n", __FUNCTION__, Status));
  }

  return Status;
}

/**
  This function reads user input.

  @param[in,out] This      Pointer to MEBx Display Protocol
  @param[out]    UserInput User input

  @retval EFI_SUCCESS      Successfully read user input or no input
  @retval Others           Error has occurred
**/
EFI_STATUS
EFIAPI
MebxGetUserInput (
  IN OUT MEBX_DISPLAY_PROTOCOL *This,
     OUT MEBX_USER_INPUT       *UserInput
  )
{
  EFI_STATUS     Status;
  EFI_INPUT_KEY  Key;

  Status = gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
  if (EFI_ERROR (Status) && Status != EFI_NOT_READY) {
    return Status;
  }

  if (Status == EFI_NOT_READY) {
    *UserInput = MebxUserInputNoInput;
  } else if (Key.ScanCode == SCAN_ESC) {
    *UserInput = MebxUserInputEsc;
  } else if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
    *UserInput = MebxUserInputEnter;
  } else {
    *UserInput = MebxUserInputOtherKey;
  }

  return EFI_SUCCESS;
}

/**
  This function gets information about current screen resolution.

  @param[in,out] This      Pointer to MEBx Display Protocol
  @param[out]    Width     Current screen width
  @param[out]    Height    Current screen height

  @retval EFI_SUCCESS      The function succeeded
  @retval Others           Error has occurred
**/
EFI_STATUS
EFIAPI
MebxGetCurrentResolution (
  IN OUT MEBX_DISPLAY_PROTOCOL  *This,
     OUT UINT16                 *Width,
     OUT UINT16                 *Height
  )
{
  EFI_STATUS                   Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput;

  if (Width == NULL || Height == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gEfiGraphicsOutputProtocolGuid,
                  NULL,
                  (VOID**) &GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *Height = (UINT16)GraphicsOutput->Mode->Info->VerticalResolution;
  *Width  = (UINT16)GraphicsOutput->Mode->Info->HorizontalResolution;

  return EFI_SUCCESS;
}

/**
  This function clears a screen's content.

  @param[in,out] This      Pointer to MEBx Display Protocol

  @retval EFI_SUCCESS      The function succeeded
  @retval Others           Error has occurred
**/
EFI_STATUS
EFIAPI
MebxClearScreen (
  IN OUT MEBX_DISPLAY_PROTOCOL *This
  )
{
  return gST->ConOut->ClearScreen (gST->ConOut);
}

/**
  This function installs platform implementation of MEBx display protocol.

  @param[in] ImageHandle         Handle on which the protocol will be installed.

  @retval EFI_SUCCESS            The protocol interface was installed.
  @retval EFI_OUT_OF_RESOURCES   Space for a new handle could not be allocated.
  @retval EFI_INVALID_PARAMETER  Handle or Protocol is NULL.
**/
EFI_STATUS
InstallMebxDisplayProtocol (
  IN EFI_HANDLE         ImageHandle
  )
{
  EFI_STATUS                    Status;
  MEBX_DISPLAY_PRIVATE_PROTOCOL *MebxDisplayProtocol;

  MebxDisplayProtocol = AllocateZeroPool (sizeof (MEBX_DISPLAY_PRIVATE_PROTOCOL));

  if (MebxDisplayProtocol == NULL) {
    ASSERT (MebxDisplayProtocol != NULL);
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }

  MebxDisplayProtocol->MebxDisplay.ConfigureScreen     = (MEBX_CONFIGURE_SCREEN) MebxConfigureScreen;
  MebxDisplayProtocol->MebxDisplay.DisplayText         = (MEBX_DISPLAY_TEXT) MebxDisplayText;
  MebxDisplayProtocol->MebxDisplay.DisplayError        = (MEBX_DISPLAY_ERROR) MebxDisplayError;
  MebxDisplayProtocol->MebxDisplay.DisplayImage        = (MEBX_DISPLAY_IMAGE) MebxDisplayImage;
  MebxDisplayProtocol->MebxDisplay.GetUserInput        = (MEBX_GET_USER_INPUT) MebxGetUserInput;
  MebxDisplayProtocol->MebxDisplay.GetScreenResolution = (MEBX_GET_SCREEN_RESOLUTION) MebxGetCurrentResolution;
  MebxDisplayProtocol->MebxDisplay.ClearScreen         = (MEBX_CLEAR_SCREEN) MebxClearScreen;
  MebxDisplayProtocol->LastScreenMode                  = ScreenModeBios;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gMebxDisplayProtocolGuid,
                  MebxDisplayProtocol,
                  NULL
                  );

  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  Driver's entry point. Checks if MEBx is supported, installs MEBx protocol and
  publishes setup formset into setup browser.

  @param[in] ImageHandle      The firmware allocated handle for the EFI image.
  @param[in] SystemTable      A pointer to the EFI System Table.

  @retval EFI_SUCCESS         MEBx driver initialized successfully
  @retval EFI_UNSUPPORTED     Feature is not supported by the FW
  @retval Others              MEBx driver initialization failed
**/
EFI_STATUS
EFIAPI
MebxDisplayEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE* SystemTable
  )
{
  EFI_STATUS            Status;

  if (!MeClientIsCorporate ()) {
    return EFI_UNSUPPORTED;
  }

  Status = InstallMebxDisplayProtocol (ImageHandle);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

