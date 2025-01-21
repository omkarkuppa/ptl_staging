/** @file
  This file is the library for ME DXE Policy initialization.

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

#include <PiDxe.h>
#include <SetupVariable.h>
#include <Setup/MeSetup.h>
#include <Guid/EventGroup.h>
#include <Protocol/MePolicy.h>
#include <Protocol/MeDisplayMsgProtocol.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeMeLib.h>
#include <Library/HobLib.h>
#include <Library/MeFwStsLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/UefiLib.h>
#include <MePeiConfig.h>
#include <MeBiosPayloadHob.h>

/**
  This function is used to display message strings in local language in the console.
  To display the message, the routine will check the message ID and ConOut
  the message strings.

  @param[in] MsgId                ME message ID for displaying on screen message
**/
VOID
EFIAPI
DisplayMeMessage (
  IN ME_MSG_ID                  MsgId
  )
{
  UINTN    MsgDisplayTime;

  MsgDisplayTime = ME_MSG_DISPLAY;
  gST->ConOut->ClearScreen (gST->ConOut);

  switch (MsgId) {
    case MsgEopError:
      gST->ConOut->OutputString (gST->ConOut, L"Error sending End Of Post message to ME: HECI disabled, proceeding with boot!\n");
      break;

    case MsgHmrfpoLockFailure:
      gST->ConOut->OutputString (gST->ConOut, L"(A7) Me FW Downgrade - Request MeSpiLock Failed\n");
      break;

    case MsgHmrfpoUnLockFailure:
      gST->ConOut->OutputString (gST->ConOut, L"(A7) Me FW Downgrade - Request MeSpiEnable Failed\n");
      break;

    case MsgIllegalCpuPluggedIn:
      gST->ConOut->OutputString (gST->ConOut, L"\n\n\rAn unsupported CPU/PCH configuration has been identified.\n");
      gST->ConOut->OutputString (gST->ConOut, L"\rPlease refer to the Platform Validation Matrix\n\rfor supported CPU/PCH combinations.");
      break;

    case MsgPlatDisableWait:
      gST->ConOut->OutputString (gST->ConOut, L"WARNING! Firmware encountered errors and will reboot the platform in 30 minutes.");
      MsgDisplayTime = 5 * ME_MSG_DISPLAY;
      break;

    default:
      DEBUG ((DEBUG_ERROR, "This Message Id hasn't been defined yet, MsgId = %x\n", MsgId));
      break;
  }

  gBS->Stall (MsgDisplayTime);
}

/**
  This function installs platform implementation of ME Display Message protocol.

  @param[in] Event       Pointer to the Event that triggered the callback.
  @param[in] Context     Pointer to private data registered with the callback function.
**/
VOID
EFIAPI
MeDisplayMsgConsoleOutCallback (
  IN  EFI_EVENT       Event,
  IN  VOID            *Context
  )
{
  EFI_STATUS               Status;
  VOID                     *Interface;
  EFI_HANDLE               Handle;
  ME_DISPLAY_MSG_PROTOCOL  *DisplayMsgProtocol;

  Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, &Interface);
  if (EFI_ERROR (Status)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  DisplayMsgProtocol = AllocateZeroPool (sizeof (ME_DISPLAY_MSG_PROTOCOL));
  if (DisplayMsgProtocol == NULL) {
    return;
  }

  DisplayMsgProtocol->DisplayMeMsg = DisplayMeMessage;

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gMeDisplayMsgProtocolGuid,
                  DisplayMsgProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Install MeDisplayMsgProtocol Status = %r\n", Status));
  }

  gBS->CloseEvent (Event);
}

/**
  Function performs HECI exchange with FW to update MePolicy settings.

  @param[in] Event         A pointer to the Event that triggered the callback.
  @param[in] Context       A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
UpdateMeSetupCallback (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  EFI_STATUS                      Status;
  ME_SETUP                        MeSetup;
  ME_INFO_SETUP_DATA              MeInfoSetupData;
  ME_BIOS_PAYLOAD_HOB             *MbpHob;
  UINTN                           Size;
  UINT32                          SetupAttr;
  UINT32                          MeMode;
  VOID                            *VariableWrite;
  EFI_HOB_GUID_TYPE               *GuidHob;
  ME_PEI_CONFIG                   *MePeiConfig;

  Status = gBS->LocateProtocol (&gEfiVariableWriteArchProtocolGuid, NULL, &VariableWrite);
  if (EFI_ERROR (Status)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  MeMode  = ME_MODE_FAILED;
  MbpHob  = NULL;
  GuidHob = NULL;
  ZeroMem (&MeInfoSetupData, sizeof (MeInfoSetupData));

  //
  // Update MeInfoSetup data
  //
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob != NULL) {
    MeInfoSetupData.MeMajor   = (UINT32) MbpHob->MeBiosPayload.FwVersionName.MajorVersion;
    MeInfoSetupData.MeMinor   = (UINT32) MbpHob->MeBiosPayload.FwVersionName.MinorVersion;
    MeInfoSetupData.MeHotFix  = (UINT32) MbpHob->MeBiosPayload.FwVersionName.HotfixVersion;
    MeInfoSetupData.MeBuildNo = (UINT32) MbpHob->MeBiosPayload.FwVersionName.BuildVersion;
  }

  Status = gRT->SetVariable (L"MeInfoSetup", &gMeInfoSetupGuid, EFI_VARIABLE_BOOTSERVICE_ACCESS, sizeof (MeInfoSetupData), &MeInfoSetupData);
  ASSERT_EFI_ERROR (Status);

  //
  // Update MeSetup data
  //
  SetupAttr = 0;
  Size = sizeof (ME_SETUP);
  Status = gRT->GetVariable (L"MeSetup", &gMeSetupVariableGuid, &SetupAttr, &Size, &MeSetup);
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    Status = MeBiosGetMeMode (&MeMode);
    if (EFI_ERROR (Status)) {
      MeMode = GetMeModeFromFwstsHob ();
    }

    MeSetup.OperationMode          = (UINT8) MeMode;
    MeSetup.MeImageType            = (MbpHob == NULL) ? 0 : (UINT8) MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType;
    MeSetup.UnconfigOnRtcAvailable = (MbpHob == NULL) ? 0 : (UINT8) MbpHob->MeBiosPayload.UnconfigOnRtcClearState.Available;
    if (MeSetup.UnconfigOnRtcAvailable) {
      GuidHob = GetFirstGuidHob (&gMePolicyHobGuid);
      if (GuidHob != NULL) {
        MePeiConfig = GET_GUID_HOB_DATA (GuidHob);
        if (MePeiConfig->MeUnconfigOnRtcClear == 2) {
          //
          // ME Unconfig item in setup menu could be destroyed because of Secondary NvRam Init.
          // If item is not valid, restore it from DisUnconfigOnRtcClearState.
          //
          MeSetup.MeUnconfigOnRtcClear = (MbpHob->MeBiosPayload.UnconfigOnRtcClearState.UnconfigOnRtcClearData.DisUnconfigOnRtcClearState == EnableState) ? 1 : 0;
        }
      }
    }

    //
    // If FwCapSku is available, then check for capabilities field whether
    // TBT Dock is enabled or not and update TBT Dock support accordingly
    //
    if (MbpHob != NULL && MbpHob->MeBiosPayload.FwCapsSku.Available == TRUE) {
      MeSetup.vProTbtDockSupported = (UINT8) MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.TbtDock;
    } else {
      MeSetup.vProTbtDockSupported = 0;
    }

    //
    // FW Downgrade Setup Option is only valid for one boot
    //
    MeSetup.MeFwDowngrade = 0;

    Status = gRT->SetVariable (L"MeSetup", &gMeSetupVariableGuid, SetupAttr, sizeof (ME_SETUP), &MeSetup);
    ASSERT_EFI_ERROR (Status);
  }

  gBS->CloseEvent (Event);
}

/**
  Update the ME Policy Library

  @param[in, out] DxeMePolicy     ME Policy in DXE phase

  @retval EFI_SUCCESS             Initialization complete.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
UpdateDxeMePolicy (
  IN OUT  ME_POLICY_PROTOCOL *DxeMePolicy
  )
{
  EFI_STATUS    Status;
  ME_DXE_CONFIG *MeDxeConfig;
  ME_SETUP      MeSetup;
  UINTN         Size;
  VOID          *SetupRegistration;
  VOID          *MsgRegistration;

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  ZeroMem (&MeSetup, sizeof (ME_SETUP));
  Size = sizeof (ME_SETUP);
  Status = gRT->GetVariable (L"MeSetup", &gMeSetupVariableGuid, NULL, &Size, &MeSetup);
  if (!EFI_ERROR (Status)) {
    Status = GetConfigBlock ((VOID *) DxeMePolicy, &gMeDxeConfigGuid, (VOID *) &MeDxeConfig);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    MeDxeConfig->MeFwDowngrade       = MeSetup.MeFwDowngrade;
    MeDxeConfig->CoreBiosDoneEnabled = MeSetup.CoreBiosDoneEnabled;
    MeDxeConfig->AutoArbSvnCommit    = MeSetup.AutoArbSvnCommit;
  }

  EfiCreateProtocolNotifyEvent (
    &gEfiVariableWriteArchProtocolGuid,
    TPL_CALLBACK,
    UpdateMeSetupCallback,
    NULL,
    &SetupRegistration
    );

  EfiCreateProtocolNotifyEvent (
    &gEfiGraphicsOutputProtocolGuid,
    TPL_CALLBACK,
    MeDisplayMsgConsoleOutCallback,
    NULL,
    &MsgRegistration
    );

  DEBUG ((DEBUG_INFO, "%a () done\n", __FUNCTION__));
  return Status;
}
