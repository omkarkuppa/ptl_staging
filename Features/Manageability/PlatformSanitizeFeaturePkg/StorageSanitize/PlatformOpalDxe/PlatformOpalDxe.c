/**@file
  Platform Opal Driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "PlatformOpalDxe.h"

PLATFORM_OPAL_DRIVER mPlatformOpalDriver;

//
// Globals
//
EFI_DRIVER_BINDING_PROTOCOL  gPlatformOpalDriverBinding = {
                               PlatformOpalEfiDriverBindingSupported,
                               PlatformOpalEfiDriverBindingStart,
                               PlatformOpalEfiDriverBindingStop,
                               0x1c,
                               NULL,
                               NULL
                               };

/**
  Provide interface to get the lock state on certain device

  @param[in]  This      Pointer of PLATFORM_OPAL_PROTOCOL.
  @param[in]  Handle    SSCP handle of the selected device.
  @param[out] IsEnabled TRUE/FALSE, TRUE: opal feature is enabled.
  @param[out] IsLocked  TRUE/FALSE, TRUE: device is locked.

  @retval EFI_SUCCESS  support Lock feature and return state.
  @retval Others       Failed due to some error.
**/
EFI_STATUS
EFIAPI
GetOpalState (
  IN  PLATFORM_OPAL_PROTOCOL *This,
  IN  EFI_HANDLE             Handle,
  OUT BOOLEAN                *IsEnabled OPTIONAL,
  OUT BOOLEAN                *IsLocked  OPTIONAL
  )
{
  PLATFORM_OPAL_DRIVER        *Private;
  PLATFORM_OPAL_DRIVER_DEVICE *Itr;
  BOOLEAN                     Found;
  BOOLEAN                     OpalEnabled;
  BOOLEAN                     DeviceLocked;

  Found = FALSE;
  OpalEnabled = FALSE;
  DeviceLocked = FALSE;

  if ((This == NULL) || (Handle == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Private  = PLATFORM_OPAL_PRIVATE_FROM_PROTOCOL (This);
  Itr = Private->DeviceList;
  while (Itr != NULL) {
    if (Handle == Itr->Handle) {
      Found = TRUE;
      OpalEnabled = OpalFeatureEnabled (&Itr->OpalDisk.SupportedAttributes, &Itr->OpalDisk.LockingFeature);
      if (OpalEnabled && Itr->OpalDisk.LockingFeature.Locked) {
        DeviceLocked = TRUE;
      }
      break;
    }
    Itr = Itr->Next;
  }

  DEBUG ((DEBUG_INFO, "%a: Found %d, EnableState: %d, LockState: %d \n" , __FUNCTION__, Found, OpalEnabled, DeviceLocked));
  if (!Found) {
    return EFI_UNSUPPORTED;
  }

  if (IsEnabled != NULL) {
    *IsEnabled = OpalEnabled;
  }
  if (IsLocked != NULL) {
    *IsLocked = DeviceLocked;
  }

  return EFI_SUCCESS;
}

/**
  Provide interface to get the block sid state on certain device

  @param[in]  This        Pointer of PLATFORM_OPAL_PROTOCOL.
  @param[in]  Handle      SSCP handle of the selected device.
  @param[out] BlockState  Blocked state.

  @retval EFI_SUCCESS  support block SID and return state.
  @retval Others       Failed due to some error.

**/
EFI_STATUS
EFIAPI
GetBlockSidState (
  IN PLATFORM_OPAL_PROTOCOL  *This,
  IN EFI_HANDLE              Handle,
  OUT UINT8                  *BlockState
  )
{
  PLATFORM_OPAL_DRIVER            *Private;
  PLATFORM_OPAL_DRIVER_DEVICE     *Dev;
  BOOLEAN                         Found;
  OPAL_SESSION                    Session;
  VOID                            *Buffer;
  TCG_LEVEL0_DISCOVERY_HEADER     *DiscoveryHeader;
  OPAL_LEVEL0_FEATURE_DESCRIPTOR  *Feat;
  UINTN                           Size;


  if ((This == NULL) || (Handle == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Private  = PLATFORM_OPAL_PRIVATE_FROM_PROTOCOL (This);
  Found = FALSE;
  Buffer = NULL;

  Dev = Private->DeviceList;
  while (Dev != NULL) {
    if (Handle == Dev->Handle) {
      Found = TRUE;
      break;
    }
    Dev = Dev->Next;
  }

  if (!Found) {
    return EFI_NOT_FOUND;
  }

  //
  // Prepare data for Opal session
  //
  ZeroMem(&Session, sizeof(Session));
  Session.Sscp = Dev->OpalDisk.Sscp;
  Session.MediaId = Dev->OpalDisk.MediaId;
  Session.OpalBaseComId = Dev->OpalDisk.OpalBaseComId;

  Buffer = AllocateZeroPool (sizeof (TCG_SUPPORTED_SECURITY_PROTOCOLS) + 1);
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  if (OpalRetrieveLevel0DiscoveryHeader (&Session, sizeof (TCG_SUPPORTED_SECURITY_PROTOCOLS), Buffer) == TcgResultFailure) {
    return EFI_NOT_FOUND;
  }

  DiscoveryHeader = (TCG_LEVEL0_DISCOVERY_HEADER *)Buffer;
  Size = 0;
  Feat = (OPAL_LEVEL0_FEATURE_DESCRIPTOR *)TcgGetFeature (DiscoveryHeader, TCG_FEATURE_BLOCK_SID, &Size);
  if ((Feat != NULL) && (Size >= sizeof (TCG_BLOCK_SID_FEATURE_DESCRIPTOR))) {
    *BlockState = Feat->BlockSid.SIDBlockedState;
    return EFI_SUCCESS;
  }

  return EFI_UNSUPPORTED;
}

/**
  Get password input from the popup window.

  @param[in]  PopUpString1  Pop up string 1.
  @param[out] PressEsc      Whether user escape function through Press ESC.

  @retval Password string if success. NULL if failed.

**/
CHAR8 *
OpalDriverPopUpPasswordInput (
  IN CHAR16                 *PopUpString1,
  OUT BOOLEAN               *PressEsc
  )
{
  EFI_INPUT_KEY             InputKey;
  UINTN                     InputLength;
  CHAR16                    Mask[OPAL_MAX_PASSWORD_SIZE + 1];
  CHAR16                    Unicode[OPAL_MAX_PASSWORD_SIZE + 1];
  CHAR8                     *Ascii;

  ZeroMem(Unicode, sizeof(Unicode));
  ZeroMem(Mask, sizeof(Mask));

  *PressEsc = FALSE;

  InputLength = 0;
  while (TRUE) {
    Mask[InputLength] = L'_';

      CreatePopUp (
        EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
        &InputKey,
        PopUpString1,
        L"-----------------------------------------------------",
        L"Enter admin revert password to unlock the device:",
        Mask,
        NULL
      );

    //
    // Check key.
    //
    if (InputKey.ScanCode == SCAN_NULL) {
      //
      // password finished
      //
      if (InputKey.UnicodeChar == CHAR_CARRIAGE_RETURN) {
        //
        // Add the null terminator.
        //
        Unicode[InputLength] = 0;
        Mask[InputLength] = 0;
        break;
      } else if ((InputKey.UnicodeChar == CHAR_NULL) ||
                 (InputKey.UnicodeChar == CHAR_TAB) ||
                 (InputKey.UnicodeChar == CHAR_LINEFEED)
                ) {
        continue;
      } else {
        //
        // delete last key entered
        //
        if (InputKey.UnicodeChar == CHAR_BACKSPACE) {
          if (InputLength > 0) {
            Unicode[InputLength] = 0;
            Mask[InputLength] = 0;
            InputLength--;
          }
        } else {
          //
          // add Next key entry
          //
          Unicode[InputLength] = InputKey.UnicodeChar;
          Mask[InputLength] = L'*';
          InputLength++;
          if (InputLength == OPAL_MAX_PASSWORD_SIZE) {
            //
            // Add the null terminator.
            //
            Unicode[InputLength] = 0;
            Mask[InputLength] = 0;
            break;
          }
        }
      }
    }

    //
    // exit on ESC
    //
    if (InputKey.ScanCode == SCAN_ESC) {
      *PressEsc = TRUE;
      break;
    }
  }

  if (InputLength == 0 || InputKey.ScanCode == SCAN_ESC) {
    ZeroMem (Unicode, sizeof (Unicode));
    return NULL;
  }

  Ascii = AllocateZeroPool (OPAL_MAX_PASSWORD_SIZE + 1);
  if (Ascii == NULL) {
    ZeroMem (Unicode, sizeof (Unicode));
    return NULL;
  }

  UnicodeStrToAsciiStrS (Unicode, Ascii, OPAL_MAX_PASSWORD_SIZE + 1);
  ZeroMem (Unicode, sizeof (Unicode));

  return Ascii;
}

/**
  Check if the device is Opal Locked and revert Opal feature.

  @param[in] Dev         The Opal device.
  @param[in] Password    Password received from FW.
  @param[in] DeviceName  Device Name

  @retval EFI_SUCCESS        Successfully performed operation.
  @retval EFI_DEVICE_ERROR   A device error occurred while attempting to send the Opal Command.
  @retval EFI_ACCESS_DENIED  Password is not correct.
**/
EFI_STATUS
OpalRevert (
  IN PLATFORM_OPAL_DRIVER_DEVICE  *Dev,
  IN CHAR8                        *HostPassWord,
  IN CHAR16                       *DeviceName
  )
{
  EFI_STATUS     Status;
  UINT8          Count;
  BOOLEAN        IsEnabled;
  CHAR8          *Password;
  UINT32         PasswordLength;
  OPAL_SESSION   Session;
  BOOLEAN        PressEsc;
  EFI_INPUT_KEY  Key;
  TCG_RESULT     Ret;
  BOOLEAN        PasswordFailed;
  CHAR16         PopUpString[100];

  Status = EFI_SUCCESS;
  Ret = TcgResultFailure;

  if (Dev == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a()\n", __FUNCTION__));

  if (DeviceName == NULL) {
    if (Dev->Name16 != NULL) {
      UnicodeSPrint (PopUpString, sizeof (PopUpString), Dev->Name16);
    } else {
      UnicodeSPrint (PopUpString, sizeof (PopUpString), L"Opal Disk");
    }
  } else {
    UnicodeSPrint (PopUpString, sizeof (PopUpString), L"%s", DeviceName);
  }

  IsEnabled = OpalFeatureEnabled (&Dev->OpalDisk.SupportedAttributes, &Dev->OpalDisk.LockingFeature);
  if (!IsEnabled) {
    return EFI_SUCCESS;
  }

  //
  // Prepare data for Opal session
  //
  ZeroMem(&Session, sizeof(Session));
  Session.Sscp = Dev->OpalDisk.Sscp;
  Session.MediaId = Dev->OpalDisk.MediaId;
  Session.OpalBaseComId = Dev->OpalDisk.OpalBaseComId;

  //
  // Unlock Opal device before perform Opal revert action
  //
  if (HostPassWord != NULL && AsciiStrLen (HostPassWord) != 0) {
    //
    // Use password from host
    //
    Password = AllocateZeroPool (OPAL_MAX_PASSWORD_SIZE + 1);
    if (Password != NULL) {
      AsciiStrCpyS (Password, OPAL_MAX_PASSWORD_SIZE, HostPassWord);
      PasswordLength = (UINT32) AsciiStrLen (Password);
      Ret = OpalUtilUpdateGlobalLockingRange (&Session, Password, PasswordLength, FALSE, FALSE);
    }
  } else {
    //
    // Use password from user input
    //
    for (Count = 0; Count < MAX_OPAL_PASSWORD_TRY_COUNT; Count ++) {
      Password = OpalDriverPopUpPasswordInput (PopUpString, &PressEsc);
      if (PressEsc) {
        break;
      }
      if (Password == NULL) {
        continue;
      }
      PasswordLength = (UINT32) AsciiStrLen(Password);
      Ret = OpalUtilUpdateGlobalLockingRange (&Session, Password, PasswordLength, FALSE, FALSE);
      if (Ret == TcgResultSuccess) {
        break;
      }
      if (Password != NULL) {
        ZeroMem (Password, PasswordLength);
        FreePool (Password);
        Password = NULL;
      }
      if (Ret == TcgResultFailureInvalidType) {
        Count = MAX_OPAL_PASSWORD_TRY_COUNT;
        break;
      }
      do {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"Failed to unlock the device, please try again.",
          L"Press [Enter] to continue",
          NULL
          );
      } while (Key.UnicodeChar != CHAR_CARRIAGE_RETURN);
    }
  }

  if (Ret != TcgResultSuccess) {
    DEBUG ((DEBUG_INFO, "OPAL: unlock device failed - 0x%x\n", Ret));
    if (Password != NULL) {
      ZeroMem (Password, PasswordLength);
      FreePool (Password);
    }
    return EFI_DEVICE_ERROR;
  }

  //
  // Perform Opal revert action
  //
  Ret = OpalUtilRevert(
          &Session,
          FALSE,
          Password,
          PasswordLength,
          &PasswordFailed,
          Dev->OpalDisk.Msid,
          Dev->OpalDisk.MsidLength
          );
  if (Ret != TcgResultSuccess) {
    DEBUG ((DEBUG_INFO, "OPAL: Revert failed - 0x%x, PasswordFailed = 0x%x\n", Ret, PasswordFailed));
    Status = EFI_DEVICE_ERROR;
  }

  if (Password != NULL) {
    ZeroMem (Password, PasswordLength);
    FreePool (Password);
  }
  return Status;
}

/**
  Update the device ownship

  @param[in] OpalDisk  The Opal device.

  @retval EFI_SUCCESS            Get ownership success.
  @retval EFI_ACCESS_DENIED      Has send BlockSID command, can't change ownership.
  @retval EFI_INVALID_PARAMETER  Not get Msid info before get ownership info.

**/
EFI_STATUS
OpalDiskUpdateOwnerShip (
  IN OPAL_DISK  *OpalDisk
  )
{
  OPAL_SESSION  Session;

  if (OpalDisk->MsidLength == 0) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&Session, sizeof (Session));
  Session.Sscp = OpalDisk->Sscp;
  Session.MediaId = OpalDisk->MediaId;
  Session.OpalBaseComId = OpalDisk->OpalBaseComId;

  OpalDisk->Owner = OpalUtilDetermineOwnership (&Session, OpalDisk->Msid, OpalDisk->MsidLength);
  return EFI_SUCCESS;
}

/**
  Update the device info.

  @param[in] OpalDisk  The Opal device.

  @retval EFI_SUCCESS            Initialize the device success.
  @retval EFI_DEVICE_ERROR       Get info from device failed.
  @retval EFI_INVALID_PARAMETER  Not get Msid info before get ownership info.
  @retval EFI_ACCESS_DENIED      Has send BlockSID command, can't change ownership.

**/
EFI_STATUS
OpalDiskUpdateStatus (
  IN OPAL_DISK  *OpalDisk
  )
{
  TCG_RESULT                  TcgResult;
  OPAL_SESSION                Session;

  ZeroMem (&Session, sizeof (Session));
  Session.Sscp = OpalDisk->Sscp;
  Session.MediaId = OpalDisk->MediaId;
  Session.OpalBaseComId = OpalDisk->OpalBaseComId;

  TcgResult = OpalGetLockingInfo (&Session, &OpalDisk->LockingFeature);
  if (TcgResult != TcgResultSuccess) {
    return EFI_DEVICE_ERROR;
  }

  return OpalDiskUpdateOwnerShip (OpalDisk);
}

/**
  Initialize the Opal disk base on the hardware info get from device.

  @param[in] Dev  The Opal device.

  @retval EFI_SUCCESS       Initialize the device success.
  @retval EFI_DEVICE_ERROR  Get info from device failed.

**/
EFI_STATUS
OpalDiskInitialize (
  IN PLATFORM_OPAL_DRIVER_DEVICE *Dev
  )
{
  TCG_RESULT                  TcgResult;
  OPAL_SESSION                Session;

  ZeroMem(&Dev->OpalDisk, sizeof(OPAL_DISK));
  Dev->OpalDisk.Sscp = Dev->Sscp;
  Dev->OpalDisk.MediaId = Dev->MediaId;

  ZeroMem(&Session, sizeof(Session));
  Session.Sscp = Dev->Sscp;
  Session.MediaId = Dev->MediaId;

  TcgResult = OpalGetSupportedAttributesInfo (&Session, &Dev->OpalDisk.SupportedAttributes, &Dev->OpalDisk.OpalBaseComId);
  if (TcgResult != TcgResultSuccess) {
    return EFI_DEVICE_ERROR;
  }
  Session.OpalBaseComId = Dev->OpalDisk.OpalBaseComId;

  TcgResult = OpalUtilGetMsid (&Session, Dev->OpalDisk.Msid, OPAL_MSID_LENGTH, &Dev->OpalDisk.MsidLength);
  if (TcgResult != TcgResultSuccess) {
    return EFI_DEVICE_ERROR;
  }

  return OpalDiskUpdateStatus (&Dev->OpalDisk);
}

/**
  Provide interface to perform Opal revert action on certain device

  @param[in] This        Pointer of OPAL_PROTOCOL.
  @param[in] Handle      SSCP handle of the selected device.
  @param[in] Password    Password for revert operation.
  @param[in] DeviceName  Device name.

  @retval EFI_SUCCESS            Perform opal revert successfully.
  @retval EFI_UNSUPPORTED        The Handle has no SSCP installed.
  @retval EFI_INVALID_PARAMETER  Parameter is invalid.
  @retval EFI_DEVICE_ERROR       Operation is failed due to some error.

**/
EFI_STATUS
EFIAPI
PerformOpalRevert (
  IN PLATFORM_OPAL_PROTOCOL  *This,
  IN EFI_HANDLE              Handle,
  IN CHAR8                   *Password OPTIONAL,
  IN CHAR16                  *DeviceName OPTIONAL
  )
{
  EFI_STATUS                  Status;
  PLATFORM_OPAL_DRIVER        *Private;
  PLATFORM_OPAL_DRIVER_DEVICE *Itr;
  BOOLEAN                     Found;

  if ((This == NULL) || (Handle == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_UNSUPPORTED;
  Private  = PLATFORM_OPAL_PRIVATE_FROM_PROTOCOL (This);
  Found = FALSE;

  Itr = Private->DeviceList;
  while (Itr != NULL) {
    if (Handle == Itr->Handle) {
      Found = TRUE;
      Status = OpalRevert (Itr, Password, DeviceName);
      break;
    }
    Itr = Itr->Next;
  }

  if (Found) {
    if (EFI_ERROR(Status)) {
      Status = EFI_DEVICE_ERROR;
    } else {
      Status = EFI_SUCCESS;
    }
  }

  return Status;
}

/**
  Add new device to the global device list.

  @param[in] Dev  New create device.

**/
VOID
AddDeviceToTail(
  IN PLATFORM_OPAL_DRIVER_DEVICE *Dev
  )
{
  PLATFORM_OPAL_DRIVER_DEVICE  *TmpDev;

  if (mPlatformOpalDriver.DeviceList == NULL) {
    mPlatformOpalDriver.DeviceList = Dev;
  } else {
    TmpDev = mPlatformOpalDriver.DeviceList;
    while (TmpDev->Next != NULL) {
      TmpDev = TmpDev->Next;
    }

    TmpDev->Next = Dev;
  }
}

/**
  Remove one device in the global device list.

  @param[in] Dev  The device need to be removed.

**/
VOID
RemoveDevice (
  IN PLATFORM_OPAL_DRIVER_DEVICE *Dev
  )
{
  PLATFORM_OPAL_DRIVER_DEVICE  *TmpDev;

  if (mPlatformOpalDriver.DeviceList == NULL) {
    return;
  }

  if (mPlatformOpalDriver.DeviceList == Dev) {
    mPlatformOpalDriver.DeviceList = Dev->Next;
    return;
  }

  TmpDev = mPlatformOpalDriver.DeviceList;
  while (TmpDev->Next != NULL) {
    if (TmpDev->Next == Dev) {
      TmpDev->Next = Dev->Next;
      break;
    }
  }
}

/**
  Stop this Controller.

  @param[in] Dev  The device need to be stopped.

**/
VOID
OpalDriverStopDevice (
  IN PLATFORM_OPAL_DRIVER_DEVICE  *Dev
  )
{
  //
  // remove OPAL_DRIVER_DEVICE from the list
  // it updates the controllerList pointer
  //
  RemoveDevice (Dev);

  //
  // close protocols that were opened
  //
  gBS->CloseProtocol (
         Dev->Handle,
         &gEfiStorageSecurityCommandProtocolGuid,
         gPlatformOpalDriverBinding.DriverBindingHandle,
         Dev->Handle
         );

  gBS->CloseProtocol (
         Dev->Handle,
         &gEfiBlockIoProtocolGuid,
         gPlatformOpalDriverBinding.DriverBindingHandle,
         Dev->Handle
         );

  FreePool (Dev);
}

/**
  Get device name through the component name protocol.

  @param[in] AllHandlesBuffer  The handle buffer for current system.
  @param[in] NumAllHandles     The number of handles for the handle buffer.
  @param[in] Dev               The device which need to get name.
  @param[in] UseComp1          Whether use component name or name2 protocol.

  @retval TRUE   Find the name for this device.
  @retval FALSE  Not found the name for this device.
**/
BOOLEAN
OpalDriverGetDeviceNameByProtocol (
  IN EFI_HANDLE                   *AllHandlesBuffer,
  IN UINTN                        NumAllHandles,
  IN PLATFORM_OPAL_DRIVER_DEVICE  *Dev,
  IN BOOLEAN                      UseComp1
  )
{
  EFI_HANDLE                    *ProtocolHandlesBuffer;
  UINTN                         NumProtocolHandles;
  EFI_STATUS                    Status;
  EFI_COMPONENT_NAME2_PROTOCOL  *Cnp1_2; // efi component name and componentName2 have same layout
  EFI_GUID                      Protocol;
  UINTN                         StrLength;
  UINTN                         Index1;
  UINTN                         Index2;
  EFI_HANDLE                    TmpHandle;
  CHAR16                        *DevName;
  EFI_DEVICE_PATH_PROTOCOL      *TmpDevPath;

  ProtocolHandlesBuffer = NULL;
  if ((Dev == NULL) || (AllHandlesBuffer == NULL) || (NumAllHandles == 0)) {
    return FALSE;
  }

  Protocol = UseComp1 ? gEfiComponentNameProtocolGuid : gEfiComponentName2ProtocolGuid;

  //
  // Find all EFI_HANDLES with protocol
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &Protocol,
                  NULL,
                  &NumProtocolHandles,
                  &ProtocolHandlesBuffer
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  //
  // Exit early if no supported devices
  //
  if (NumProtocolHandles == 0) {
    return FALSE;
  }

  //
  // Get printable name by iterating through all protocols
  // using the handle as the child, and iterate through all handles for the controller
  // exit loop early once found, if not found, then delete device
  // storage security protocol instances already exist, add them to internal list
  //
  Status = EFI_DEVICE_ERROR;
  for (Index1 = 0; Index1 < NumProtocolHandles; Index1++) {
    DevName = NULL;

    if (Dev->Name16 != NULL) {
      return TRUE;
    }

    TmpHandle = ProtocolHandlesBuffer[Index1];

    Status = gBS->OpenProtocol (
                    TmpHandle,
                    &Protocol,
                    (VOID **)&Cnp1_2,
                    gImageHandle,
                    NULL,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );
    if (EFI_ERROR (Status) || (Cnp1_2 == NULL)) {
      continue;
    }

    //
    // Use all handles array as controller handle
    //
    for (Index2 = 0; Index2 < NumAllHandles; Index2++) {
      Status = Cnp1_2->GetControllerName (
                         Cnp1_2,
                         AllHandlesBuffer[Index2],
                         Dev->Handle,
                         LANGUAGE_ISO_639_2_ENGLISH,
                         &DevName
                         );
      if (EFI_ERROR (Status)) {
        Status = Cnp1_2->GetControllerName (
                           Cnp1_2,
                           AllHandlesBuffer[Index2],
                           Dev->Handle,
                           LANGUAGE_RFC_3066_ENGLISH,
                           &DevName
                           );
      }

      if (!EFI_ERROR (Status) && (DevName != NULL)) {
        StrLength   = StrLen (DevName) + 1;     // Add one for NULL terminator
        Dev->Name16 = AllocateZeroPool (StrLength * sizeof (CHAR16));
        if (Dev->Name16 != NULL) {
          StrCpyS (Dev->Name16, StrLength, DevName);
        }
        //
        // Retrieve bridge BDF info and port number or namespace depending on type
        //
        TmpDevPath = NULL;
        Status     = gBS->OpenProtocol (
                            Dev->Handle,
                            &gEfiDevicePathProtocolGuid,
                            (VOID **)&TmpDevPath,
                            gImageHandle,
                            NULL,
                            EFI_OPEN_PROTOCOL_GET_PROTOCOL
                            );
        if (!EFI_ERROR (Status)) {
          FreePool (ProtocolHandlesBuffer);
          return TRUE;
        }

        if (Dev->Name16 != NULL) {
          FreePool (Dev->Name16);
          Dev->Name16 = NULL;
        }
      }
    }
  }
  FreePool (ProtocolHandlesBuffer);
  return FALSE;
}

/**
  Get device name through the component name protocol.

  @param[in]  Dev         The device which need to get name.

  @retval     TRUE        Find the name for this device.
  @retval     FALSE       Not found the name for this device.
**/
BOOLEAN
OpalDriverGetDriverDeviceName (
  IN PLATFORM_OPAL_DRIVER_DEVICE  *Dev
  )
{
  EFI_HANDLE  *AllHandlesBuffer;
  UINTN       NumAllHandles;
  EFI_STATUS  Status;

  AllHandlesBuffer = NULL;
  if (Dev == NULL) {
    DEBUG ((DEBUG_INFO | DEBUG_INIT, "OpalDriverGetDriverDeviceName Exiting, Dev=NULL\n"));
    return FALSE;
  }

  //
  // Iterate through ComponentName2 handles to get name, if fails, try ComponentName
  //
  if (Dev->Name16 == NULL) {
    DEBUG ((DEBUG_INFO | DEBUG_INIT, "Name is null, update it\n"));
    //
    // Find all EFI_HANDLES
    //
    Status = gBS->LocateHandleBuffer (
                    AllHandles,
                    NULL,
                    NULL,
                    &NumAllHandles,
                    &AllHandlesBuffer
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "LocateHandleBuffer for AllHandles failed %r\n", Status));
      return FALSE;
    }

    //
    // Try component Name2
    //
    if (!OpalDriverGetDeviceNameByProtocol (AllHandlesBuffer, NumAllHandles, Dev, FALSE)) {
      DEBUG ((DEBUG_INFO | DEBUG_INIT, "ComponentName2 failed to get device name, try ComponentName\n"));
      if (!OpalDriverGetDeviceNameByProtocol (AllHandlesBuffer, NumAllHandles, Dev, TRUE)) {
        DEBUG ((DEBUG_INFO | DEBUG_INIT, "ComponentName failed to get device name, skip device\n"));
        FreePool (AllHandlesBuffer);
        return FALSE;
      }
    }
    FreePool (AllHandlesBuffer);
  }

  return TRUE;
}

/**
  Main entry for this driver.

  @param[in] ImageHandle     Image Handle this driver.
  @param[in] SystemTable     Pointer to SystemTable.

  @retval EFI_SUCCESS    This function always complete successfully.
**/
EFI_STATUS
EFIAPI
PlatformOpalEfiDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE*  SystemTable
  )
{
  EFI_STATUS                     Status;
  EFI_HANDLE                     Handle;

  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gPlatformOpalDriverBinding,
             ImageHandle,
             &gPlatformOpalComponentName,
             &gPlatformOpalComponentName2
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Install protocols to Opal driver Handle failed\n"));
    return Status ;
  }

  //
  // Initialize Driver object
  //
  ZeroMem (&mPlatformOpalDriver, sizeof (mPlatformOpalDriver));
  mPlatformOpalDriver.Signature = PLATFORM_OPA_DRIVER_SIGNATURE;
  mPlatformOpalDriver.Handle = ImageHandle;

  //
  // Install OPAL_PROTOCOL
  //
  mPlatformOpalDriver.PlatformOpalProtocol.Revision = PLATFORM_OPAL_PROTOCOL_REVISION;
  mPlatformOpalDriver.PlatformOpalProtocol.PerformOpalRevert = PerformOpalRevert;
  mPlatformOpalDriver.PlatformOpalProtocol.GetBlockSidState  = GetBlockSidState;
  mPlatformOpalDriver.PlatformOpalProtocol.GetState          = GetOpalState;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gPlatformOpalProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPlatformOpalDriver.PlatformOpalProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: Failed to install Opal Protocol! Status = %r\n", __FUNCTION__, Status));
  }

  return Status;
}

/**
  Tests to see if this driver supports a given controller.

  This function checks to see if the controller contains an instance of the
  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL and the EFI_BLOCK_IO_PROTOCOL
  and returns EFI_SUCCESS if it does.

  @param[in]  This                  A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle      The Handle of the controller to test. This Handle
                                    must support a protocol interface that supplies
                                    an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath   This parameter is ignored.

  @retval EFI_SUCCESS               The device contains required protocols
  @retval EFI_ALREADY_STARTED       The device specified by ControllerHandle and
                                    RemainingDevicePath is already being managed by the driver
                                    specified by This.
  @retval EFI_ACCESS_DENIED         The device specified by ControllerHandle and
                                    RemainingDevicePath is already being managed by a different
                                    driver or an application that requires exclusive access.
                                    Currently not implemented.
  @retval EFI_UNSUPPORTED           The device does not contain requires protocols

**/
EFI_STATUS
EFIAPI
PlatformOpalEfiDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL* This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*    RemainingDevicePath
  )
{
  EFI_STATUS                              Status;
  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL*  SecurityCommand;

  if (!PcdGetBool (PcdRpeOpalDriverEnable)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Test EFI_STORAGE_SECURITY_COMMAND_PROTOCOL on controller Handle.
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiStorageSecurityCommandProtocolGuid,
                  ( VOID ** )&SecurityCommand,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (Status == EFI_ALREADY_STARTED) {
    return EFI_SUCCESS;
  }

  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Close protocol and reopen in Start call
  //
  gBS->CloseProtocol (
         Controller,
         &gEfiStorageSecurityCommandProtocolGuid,
         This->DriverBindingHandle,
         Controller
         );

  return EFI_SUCCESS;
}

/**
  Enables Opal Management on a supported device if available.

  The start function is designed to be called after the Opal UEFI Driver has confirmed the
  "controller", which is a child Handle, contains the EF_STORAGE_SECURITY_COMMAND protocols.
  This function will complete the other necessary checks, such as verifying the device supports
  the correct version of Opal.  Upon verification, it will add the device to the
  Opal HII list in order to expose Opal management options.

  @param[in]  This                  A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle      The Handle of the controller to start. This Handle
                                    must support a protocol interface that supplies
                                    an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath   A pointer to the remaining portion of a device path.  This
                                    parameter is ignored by device drivers, and is optional for bus
                                    drivers. For a bus driver, if this parameter is NULL, then handles
                                    for all the children of Controller are created by this driver.
                                    If this parameter is not NULL and the first Device Path Node is
                                    not the End of Device Path Node, then only the Handle for the
                                    child device specified by the first Device Path Node of
                                    RemainingDevicePath is created by this driver.
                                    If the first Device Path Node of RemainingDevicePath is
                                    the End of Device Path Node, no child Handle is created by this
                                    driver.

  @retval EFI_SUCCESS               Opal management was enabled.
  @retval EFI_DEVICE_ERROR          The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES      The request could not be completed due to a lack of resources.
  @retval Others                    The driver failed to start the device.

**/
EFI_STATUS
EFIAPI
PlatformOpalEfiDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL* This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*    RemainingDevicePath
  )
{
  EFI_STATUS                  Status;
  EFI_BLOCK_IO_PROTOCOL       *BlkIo;
  PLATFORM_OPAL_DRIVER_DEVICE *Dev;
  PLATFORM_OPAL_DRIVER_DEVICE *Itr;

  Itr = mPlatformOpalDriver.DeviceList;
  while (Itr != NULL) {
    if (Controller == Itr->Handle) {
      return EFI_SUCCESS;
    }
    Itr = Itr->Next;
  }

  DEBUG ((DEBUG_INFO, "%a() - enter\n", __FUNCTION__));

  //
  // Create internal device for tracking.
  //
  Dev = (PLATFORM_OPAL_DRIVER_DEVICE *)AllocateZeroPool (sizeof (PLATFORM_OPAL_DRIVER_DEVICE));
  if (Dev == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Dev->Handle = Controller;

  //
  // Open EFI_STORAGE_SECURITY_COMMAND_PROTOCOL to perform Opal supported checks
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiStorageSecurityCommandProtocolGuid,
                  (VOID **)&Dev->Sscp,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR(Status)) {
    FreePool (Dev);
    return Status;
  }

  //
  // Open EFI_BLOCK_IO_PROTOCOL on controller Handle, required by EFI_STORAGE_SECURITY_COMMAND_PROTOCOL
  // function APIs
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiBlockIoProtocolGuid,
                  (VOID **)&BlkIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR(Status)) {
    //
    // Block_IO not supported on handle
    //
    if(Status == EFI_UNSUPPORTED) {
      BlkIo = NULL;
    } else {
      //
      // Close storage security that was opened
      //
      gBS->CloseProtocol (
             Controller,
             &gEfiStorageSecurityCommandProtocolGuid,
             This->DriverBindingHandle,
             Controller
             );

      FreePool (Dev);
      return Status;
    }
  }

  //
  // Save mediaId
  //
  if(BlkIo == NULL) {
    // If no Block IO present, use defined MediaId value.
    Dev->MediaId = 0x0;
  } else {
    Dev->MediaId = BlkIo->Media->MediaId;

    gBS->CloseProtocol (
           Controller,
           &gEfiBlockIoProtocolGuid,
           This->DriverBindingHandle,
           Controller
           );
  }

  //
  // Acquire Ascii printable name of child, if not found, then ignore device
  //
  OpalDriverGetDriverDeviceName (Dev);

  Status = OpalDiskInitialize (Dev);
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  DEBUG ((DEBUG_INFO, "%a() -   add device %S to the list\n", __FUNCTION__, Dev->Name16));
  AddDeviceToTail (Dev);

  return EFI_SUCCESS;

Done:
  //
  // free device, close protocols and exit
  //
  gBS->CloseProtocol (
         Controller,
         &gEfiStorageSecurityCommandProtocolGuid,
         This->DriverBindingHandle,
         Controller
         );

  FreePool (Dev);
  return EFI_DEVICE_ERROR;
}

/**
  Stop this driver on Controller.

  @param[in] This              Protocol instance pointer.
  @param[in] Controller        Handle of device to stop driver on
  @param[in] NumberOfChildren  Number of Handles in ChildHandleBuffer. If number of
                               children is zero stop the entire bus driver.
  @param[in] ChildHandleBuffer List of Child Handles to Stop.

  @retval EFI_SUCCESS  This driver is removed Controller.
  @retval other        This driver could not be removed from this device.

**/
EFI_STATUS
EFIAPI
PlatformOpalEfiDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL*  This,
  IN EFI_HANDLE                    Controller,
  IN UINTN                         NumberOfChildren,
  IN EFI_HANDLE*                   ChildHandleBuffer
  )
{
  PLATFORM_OPAL_DRIVER_DEVICE     *Itr;

  Itr = mPlatformOpalDriver.DeviceList;

  //
  // does Controller match any of the devices we are managing for Opal
  //
  while (Itr != NULL) {
    if (Itr->Handle == Controller) {
      OpalDriverStopDevice (Itr);
      return EFI_SUCCESS;
    }
    Itr = Itr->Next;
  }

  return EFI_NOT_FOUND;
}

/**
  Unloads UEFI Driver.  Very useful for debugging and testing.

  @param[in] ImageHandle  Image Handle this driver.

  @retval EFI_SUCCESS            This function always complete successfully.
  @retval EFI_INVALID_PARAMETER  The input ImageHandle is not valid.
**/
EFI_STATUS
EFIAPI
PlatformOpalEfiDriverUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS                   Status;
  PLATFORM_OPAL_DRIVER_DEVICE  *Itr;

  Status = EFI_SUCCESS;

  if (ImageHandle != gImageHandle) {
    return (EFI_INVALID_PARAMETER);
  }

  //
  // Uninstall any interface added to each device by us
  //
  while (mPlatformOpalDriver.DeviceList) {
    Itr = mPlatformOpalDriver.DeviceList;
    //
    // Remove OPAL_DRIVER_DEVICE from the list
    // it updates the controllerList pointer
    //
    OpalDriverStopDevice (Itr);
  }

  return Status;
}

