/**@file
  Ata storage device library file to support secure feature

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include "AtaSecure.h"

/**
  Get password input from the popup window.

  @param[in]  PopUpString1  Pop up string 1.
  @param[out] PressEsc      Whether user escape function through Press ESC.

  @retval Password string if success. NULL if failed.

**/
EFI_STATUS
PopUpHddPasswordInput (
  IN CHAR16                 *PopUpString1,
  OUT CHAR8                 Password[ATA_PASSWORD_MAX_LENGTH]
  )
{
  EFI_INPUT_KEY             InputKey;
  UINTN                     InputLength;
  CHAR16                    Mask[ATA_PASSWORD_MAX_LENGTH + 1];
  CHAR16                    Unicode[ATA_PASSWORD_MAX_LENGTH + 1];
  CHAR8                     Ascii[ATA_PASSWORD_MAX_LENGTH + 1];

  ZeroMem(Unicode, sizeof(Unicode));
  ZeroMem(Ascii, sizeof(Ascii));
  ZeroMem(Mask, sizeof(Mask));

  InputLength = 0;
  while (TRUE) {
    Mask[InputLength] = L'_';

      CreatePopUp (
        EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
        &InputKey,
        PopUpString1,
        L"-----------------------------------------------------",
        L"Enter hard drives user password to erase the device:",
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
          if (InputLength == ATA_PASSWORD_MAX_LENGTH) {
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
      return EFI_ABORTED;
    }
  }
  if (InputLength == 0) {
    return EFI_NOT_READY;
  }
  UnicodeStrToAsciiStrS (Unicode, Ascii, ATA_PASSWORD_MAX_LENGTH + 1);
  CopyMem (Password, Ascii, ATA_PASSWORD_MAX_LENGTH);
  ZeroMem (Unicode, sizeof (Unicode));
  ZeroMem (Ascii, sizeof (Ascii));
  return EFI_SUCCESS;
}
/**
  Parse security status according to identify data.

  @param[in] AtaPassThru         The pointer to the ATA_PASS_THRU protocol.
  @param[in] Port                The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                 If there is no port multiplier, then specify 0xFFFF.
  @param[in, out] SecurityStatus IFR data to hold security status.

**/
EFI_STATUS
GetHddPasswordSecurityStatus (
  IN EFI_ATA_PASS_THRU_PROTOCOL        *AtaPassThru,
  IN UINT16                            Port,
  IN UINT16                            PortMultiplierPort,
  IN OUT ATA_PASSWORD_SECURITY_STATUS  *SecurityStatus
  )
{
  EFI_STATUS                Status;
  ATA_IDENTIFY_DATA         IdentifyData;

  if (AtaPassThru == NULL || SecurityStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&IdentifyData, sizeof (ATA_IDENTIFY_DATA));
  Status = GetAtaIdentifyData (AtaPassThru, Port, PortMultiplierPort, &IdentifyData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SecurityStatus->Supported = (IdentifyData.command_set_supported_82 & BIT1) ? 1 : 0;
  SecurityStatus->Enabled   = (IdentifyData.security_status & BIT1) ? 1 : 0;
  SecurityStatus->Locked    = (IdentifyData.security_status & BIT2) ? 1 : 0;
  SecurityStatus->Frozen    = (IdentifyData.security_status & BIT3) ? 1 : 0;
  SecurityStatus->CountExpired    = (IdentifyData.security_status & BIT4) ? 1 : 0;
  SecurityStatus->UserPasswordStatus   = SecurityStatus->Enabled;
  SecurityStatus->MasterPasswordStatus = SecurityStatus->Supported;

  DEBUG ((DEBUG_INFO, "SecurityStatus.Supported            = %x\n", SecurityStatus->Supported));
  DEBUG ((DEBUG_INFO, "SecurityStatus.Enabled              = %x\n", SecurityStatus->Enabled));
  DEBUG ((DEBUG_INFO, "SecurityStatus.Locked               = %x\n", SecurityStatus->Locked));
  DEBUG ((DEBUG_INFO, "SecurityStatus.Frozen               = %x\n", SecurityStatus->Frozen));
  DEBUG ((DEBUG_INFO, "SecurityStatus.CountExpired         = %x\n", SecurityStatus->CountExpired));
  DEBUG ((DEBUG_INFO, "SecurityStatus.UserPasswordStatus   = %x\n", SecurityStatus->UserPasswordStatus));
  DEBUG ((DEBUG_INFO, "SecurityStatus.MasterPasswordStatus = %x\n", SecurityStatus->MasterPasswordStatus));

  return EFI_SUCCESS;
}

/**
  Disable ATA security on device

  @param[in] AtaPassThru         The pointer to the ATA_PASS_THRU protocol.
  @param[in] Port                The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                 If there is no port multiplier, then specify 0xFFFF.
  @param[in] HostPassword        Password received from host
  @param[in] DeviceName          Device name

  @retval EFI_SUCCESS  ATA security was disabled successfully
  @retval Others       An error occurred due to some error
**/
EFI_STATUS
DisableAtaSecurityInternal (
  IN EFI_ATA_PASS_THRU_PROTOCOL  *AtaPassThru,
  IN UINT16                      Port,
  IN UINT16                      PortMultiplier,
  IN CHAR8                       *HostPassword,
  IN CHAR16                      *DeviceName
  )
{
  EFI_STATUS                           Status;
  CHAR8                                Password[ATA_PASSWORD_MAX_LENGTH];
  ATA_PASSWORD_SECURITY_STATUS         SecurityStatus;
  CHAR16                               PopUpString[100];
  UINT32                               Count;
  EFI_INPUT_KEY                        Key;
  BOOLEAN                              Retry;

  Status = EFI_DEVICE_ERROR;
  Retry  = TRUE;

  if (DeviceName == NULL) {
    UnicodeSPrint (PopUpString, sizeof (PopUpString), L"SATA Disk");
  } else {
    UnicodeSPrint (PopUpString, sizeof (PopUpString), L"%s", DeviceName);
  }

  // Get identify data
  Status = GetHddPasswordSecurityStatus (AtaPassThru, Port, PortMultiplier, &SecurityStatus);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "GetHddPasswordSecurityStatus: failed, Status = %r\n", Status));
    return Status;
  }

  //
  // Device is frozen, disable command is access denied
  //
  if (SecurityStatus.Frozen) {
    return EFI_ACCESS_DENIED;
  }
  //
  // Check the device security status and disable security feature
  //
  if (SecurityStatus.Supported && SecurityStatus.Enabled) {
    Status = EFI_DEVICE_ERROR;
    Count  = 0;
    while (!SecurityStatus.CountExpired && Count < MAX_PASSWORD_TRY_COUNT) {
      ZeroMem (Password, sizeof (Password));
      if (HostPassword != NULL && AsciiStrLen (HostPassword) != 0) {
        //
        // Use password from host, no need retry
        //
        AsciiStrCpyS (Password, ATA_PASSWORD_MAX_LENGTH, HostPassword);
        Retry = FALSE;
      } else {
        Status = PopUpHddPasswordInput (PopUpString, Password);
        if (EFI_ERROR (Status)) {
          break;
        }
      }
      if (AsciiStrLen(Password) != 0) {
        if (SecurityStatus.Locked) {
          Status = UnlockAtaPassword (AtaPassThru, Port, PortMultiplier, ATA_SECURITY_ERASE_IDENTIFY_USER_PASSWORD, Password);
        }
        if (!EFI_ERROR (Status)) {
          Status = DisableAtaPassword (AtaPassThru, Port, PortMultiplier, ATA_SECURITY_ERASE_IDENTIFY_USER_PASSWORD, Password);
        }
        if (!EFI_ERROR (Status)) {
          break;
        }
      }
      if (Retry) {
        Count ++;
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
  }
  return Status;
}

/**
  Get the instance of EFI_ATA_PASS_THRU_PROTOCOL.

  @param[in] Handle              Device Handle
  @param[in] ParentHandle        Parent Device Handle

  @retval EFI_ATA_PASS_THRU_PROTOCOL  Point to the EFI_ATA_PASS_THRU_PROTOCOL
**/
EFI_ATA_PASS_THRU_PROTOCOL *
GetPassThruFromHandle (
  IN EFI_HANDLE  Handle,
  IN EFI_HANDLE  ParentHandle
  )
{
  EFI_STATUS                  Status;
  EFI_ATA_PASS_THRU_PROTOCOL  *AtaPassThru;
  EFI_HANDLE                  AtaPassThruHandle;
  EFI_DEVICE_PATH_PROTOCOL    *DevicePath;

  AtaPassThru = NULL;
  if (Handle == NULL) {
    return NULL;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID**) &DevicePath
                  );
  if (!EFI_ERROR (Status)) {
    Status = gBS->LocateDevicePath (
                    &gEfiAtaPassThruProtocolGuid,
                    &DevicePath,
                    &AtaPassThruHandle
                    );
    if (!EFI_ERROR (Status)) {
      Status = gBS->HandleProtocol (
                      AtaPassThruHandle,
                      &gEfiAtaPassThruProtocolGuid,
                      (VOID**) &AtaPassThru
                      );
    }
  } else if (ParentHandle != NULL) {
    Status = gBS->HandleProtocol (
                    ParentHandle,
                    &gEfiAtaPassThruProtocolGuid,
                    (VOID**) &AtaPassThru
                    );
  }

  if(EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: Failed to locate AtaPassThruProtocol %r. \n", __FUNCTION__, Status));
    return NULL;
  } else {
    return AtaPassThru;
  }
}

/**
  Disable ATA security on device by port id.

  @param[in] Handle              Device Handle
  @param[in] ParentHandle        Parent Device Handle
  @param[in] Port                The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                 If there is no port multiplier, then specify 0xFFFF.
  @param[in] HostPassword        Password received from host
  @param[in] DeviceName          Device name

  @retval EFI_SUCCESS  ATA security was disabled successfully
  @retval Others       An error occurred due to some error
**/
EFI_STATUS
EFIAPI
DisableAtaSecurityWithPortId (
  IN EFI_HANDLE  Handle,
  IN EFI_HANDLE  ParentHandle,
  IN UINT16      Port,
  IN UINT16      PortMultiplier,
  IN CHAR8       *HostPassword,
  IN CHAR16      *DeviceName
  )
{
  EFI_STATUS                  Status;
  EFI_ATA_PASS_THRU_PROTOCOL  *AtaPassThru;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  AtaPassThru = GetPassThruFromHandle (Handle, ParentHandle);
  if(AtaPassThru == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = DisableAtaSecurityInternal (AtaPassThru, Port, PortMultiplier, HostPassword, DeviceName);
  DEBUG ((DEBUG_INFO, "%a end(%r)\n", __FUNCTION__, Status));
  return Status;
}

/**
  Get ATA frozen state on device by port id.

  @param[in] Handle              Device Handle
  @param[in] ParentHandle        Parent Device Handle
  @param[in] Port                The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                 If there is no port multiplier, then specify 0xFFFF.
  @param[out] FrozenState        Device is frozen or not

  @retval EFI_SUCCESS  ATA security was disabled successfully
  @retval Others       An error occurred due to some error
**/
EFI_STATUS
EFIAPI
GetAtaFrozenStateWithPortId (
  IN EFI_HANDLE  Handle,
  IN EFI_HANDLE  ParentHandle,
  IN UINT16      Port,
  IN UINT16      PortMultiplier,
  OUT UINT8      *FrozenState
  )
{
  EFI_STATUS                   Status;
  EFI_ATA_PASS_THRU_PROTOCOL   *AtaPassThru;
  ATA_PASSWORD_SECURITY_STATUS SecurityStatus;

  AtaPassThru = GetPassThruFromHandle (Handle, ParentHandle);
  if(AtaPassThru == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get identify data
  Status = GetHddPasswordSecurityStatus (AtaPassThru, Port, PortMultiplier, &SecurityStatus);
  DEBUG ((DEBUG_INFO, "%a Status(%r), frozen: %d\n", __FUNCTION__, Status, SecurityStatus.Frozen));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *FrozenState = SecurityStatus.Frozen;
  return Status;
}

/**
  Get ATA security state on device by port id.

  @param[in] Handle              Device Handle
  @param[in] ParentHandle        Parent Device Handle
  @param[in] Port                The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                 If there is no port multiplier, then specify 0xFFFF.

  @retval TRUE/FALSE   TRUE: ATA device was enabled security feature
**/
BOOLEAN
EFIAPI
GetAtaSecurityStateWithPortId (
  IN EFI_HANDLE  Handle,
  IN EFI_HANDLE  ParentHandle,
  IN UINT16      Port,
  IN UINT16      PortMultiplier
  )
{
  EFI_STATUS                   Status;
  EFI_ATA_PASS_THRU_PROTOCOL   *AtaPassThru;
  ATA_PASSWORD_SECURITY_STATUS SecurityStatus;
  BOOLEAN                      SecurityEnabled;

  SecurityEnabled = FALSE;
  AtaPassThru = GetPassThruFromHandle (Handle, ParentHandle);
  if(AtaPassThru == NULL) {
    return SecurityEnabled;
  }

  // Get identify data
  Status = GetHddPasswordSecurityStatus (AtaPassThru, Port, PortMultiplier, &SecurityStatus);
  if (EFI_ERROR (Status)) {
    return SecurityEnabled;
  }

  SecurityEnabled = SecurityStatus.Supported && SecurityStatus.Enabled;
  return SecurityEnabled;
}
