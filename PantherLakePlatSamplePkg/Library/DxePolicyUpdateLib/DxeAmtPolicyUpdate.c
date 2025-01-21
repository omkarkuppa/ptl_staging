/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AmtPolicy.h>
#include <Protocol/AmtDisplayMsgProtocol.h>
#include <AmtConfig.h>

/**
  Function updates AMT related options in ME setup.

  @param[in] Event         A pointer to the Event that triggered the callback.
  @param[in] Context       A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
UpdateAmtSetupCallback (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  EFI_STATUS                      Status;
  ME_SETUP                        MeSetup;
  UINTN                           Size;
  UINT32                          SetupAttr;
  VOID                            *VariableWrite;

  Status = gBS->LocateProtocol (&gEfiVariableWriteArchProtocolGuid, NULL, &VariableWrite);
  if (EFI_ERROR (Status)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  SetupAttr = 0;
  Size = sizeof (ME_SETUP);
  Status = gRT->GetVariable (L"MeSetup", &gMeSetupVariableGuid, &SetupAttr, &Size, &MeSetup);

  if (!EFI_ERROR (Status)) {
    //
    // UnConfigureMe / AmtCiraRequest Setup Options are only valid for one boot
    //
    MeSetup.UnConfigureMe  = 0;
    MeSetup.AmtCiraRequest = 0;

    Status = gRT->SetVariable (L"MeSetup", &gMeSetupVariableGuid, SetupAttr, sizeof (ME_SETUP), &MeSetup);
    ASSERT_EFI_ERROR (Status);
  }

  gBS->CloseEvent (Event);
}

/**
  This function is used to display message strings in local language in the console.
  To display the message, the routine will check the message ID and ConOut
  the message strings.

  @param[in] MsgId                AMT message ID for displaying on screen message
**/
VOID
EFIAPI
DisplayAmtMessage (
  IN AMT_MSG_ID                  MsgId
  )
{
  gST->ConOut->ClearScreen (gST->ConOut);

  switch (MsgId) {
    case MsgKvmTimesUp:
      gST->ConOut->OutputString (gST->ConOut, L"Error!! Times up and the KVM session was cancelled!!");
      break;

    case MsgKvmRejected:
      gST->ConOut->OutputString (gST->ConOut, L"Error!! The request has rejected and the KVM session was cancelled!!");
      break;

    case MsgKvmWait:
      gST->ConOut->OutputString (gST->ConOut, L"Waiting Up to 8 Minutes For KVM FW.....");
      break;

    default:
      DEBUG ((DEBUG_ERROR, "This Message Id hasn't been defined yet, MsgId = %x\n", MsgId));
      break;
  }

  gBS->Stall (AMT_MSG_DISPLAY);
}

/**
  This function installs platform implementation of AMT Display Message protocol.

  @param[in] Event       Pointer to the Event that triggered the callback.
  @param[in] Context     Pointer to private data registered with the callback function.
**/
VOID
EFIAPI
AmtDisplayMsgConsoleOutCallback (
  IN  EFI_EVENT       Event,
  IN  VOID            *Context
  )
{
  EFI_STATUS                Status;
  VOID                      *Interface;
  EFI_HANDLE                Handle;
  AMT_DISPLAY_MSG_PROTOCOL  *DisplayMsgProtocol;

  Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, &Interface);
  if (EFI_ERROR (Status)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  DisplayMsgProtocol = AllocateZeroPool (sizeof (AMT_DISPLAY_MSG_PROTOCOL));
  if (DisplayMsgProtocol == NULL) {
    return;
  }

  DisplayMsgProtocol->DisplayAmtMsg = DisplayAmtMessage;

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gAmtDisplayMsgProtocolGuid,
                  DisplayMsgProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Install AmtDisplayMsgProtocol Status = %r\n", Status));
  }

  gBS->CloseEvent (Event);
}

/**
  Install DxeAmtPolicy with setup values

  @param[in, out] DxeAmtPolicy  DXE Amt Policy

  @retval EFI_SUCCESS             Initialization complete.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
UpdateDxeAmtPolicy (
  IN OUT  AMT_POLICY_PROTOCOL   *DxeAmtPolicy
  )
{
  EFI_STATUS     Status;
  AMT_DXE_CONFIG *AmtDxeConfig;
  ME_SETUP       MeSetup;
  UINTN          Size;
  VOID           *MsgRegistration;
  VOID           *SetupRegistration;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  //
  // Get Amt configuration from Me Setup
  //
  ZeroMem (&MeSetup, sizeof (ME_SETUP));
  Size = sizeof (ME_SETUP);
  Status = gRT->GetVariable (L"MeSetup", &gMeSetupVariableGuid, NULL, &Size, &MeSetup);

  if (!EFI_ERROR (Status)) {
    Status = GetConfigBlock ((VOID *) DxeAmtPolicy, &gAmtDxeConfigGuid, (VOID *) &AmtDxeConfig);
    ASSERT_EFI_ERROR (Status);

    AmtDxeConfig->CiraRequest   = MeSetup.AmtCiraRequest;
    AmtDxeConfig->UnConfigureMe = MeSetup.UnConfigureMe;
    AmtDxeConfig->UsbProvision  = MeSetup.UsbProvision;
  }

  EfiCreateProtocolNotifyEvent (
    &gEfiGraphicsOutputProtocolGuid,
    TPL_CALLBACK,
    AmtDisplayMsgConsoleOutCallback,
    NULL,
    &MsgRegistration
    );

  EfiCreateProtocolNotifyEvent (
    &gEfiVariableWriteArchProtocolGuid,
    TPL_CALLBACK,
    UpdateAmtSetupCallback,
    NULL,
    &SetupRegistration
    );

  DEBUG ((DEBUG_INFO, "%a done\n", __FUNCTION__));
  return Status;
}
