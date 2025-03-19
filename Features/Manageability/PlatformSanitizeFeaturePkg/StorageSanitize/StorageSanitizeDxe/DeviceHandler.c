/**@file
  Storage erase driver

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

#include "StorageErase.h"

/**
  Dump the data from given buffer

  @param[in] Buffer  The buffer which content will be exposed.
  @param[in] Size    Size of buffer
**/
VOID
DumpBuffer (
  IN UINT8   *Buffer,
  IN UINT32  Size
  )
{
  UINT64  row = 0;
  UINT8   col = 0;
  UINT64  Offset = 0;

  if((Buffer == NULL) || (Size == 0)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "Dump data from storage\n"));
  DEBUG ((DEBUG_INFO, "========================\n"));
  for (row = 0; row <= 0xf; row++) {
    DEBUG ((DEBUG_INFO, "%.8xh: ", Offset));
    for (col = 0; col <= 0xf; col++) {
      DEBUG ((DEBUG_INFO, "%.2x ", *(Buffer+Offset)));
      Offset++;
      if (Offset >= Size)
        break;
    }
    DEBUG ((DEBUG_INFO, "\n"));
    if (Offset >= Size)
      break;
  }
  DEBUG ((DEBUG_INFO, " ========================\n"));
}

/**
  Convert result status to string.

  @param[in] Result         Secure erase result.
  @param[in] PerformVerify  Verify is enabled or not.

  @retval UINT16*           Result string.
**/
UINT16 *
ResultString (
  IN UINT8    Result,
  IN BOOLEAN  PerformVerify
  )
{
  if ((Result & ERASE_RESULT_VERIFICATION_FAILED) != 0) {
    return L": Verify failed";
  } else if ((Result & ERASE_RESULT_DISABLE_SECURITY_FAILED) != 0) {
    return L": Access Denied";;
  } else if (Result != ERASE_RESULT_SUCCESS) {
    return L": Device Error";
  } else {
    if (PerformVerify) {
      return L": Erase and verify successful";
    } else {
      return L": Erase successful";
    }
  }
}

/**
  Add the device name in the begining of the message

  @param[in] Message  The message to be shown in UI .
  @param[in] Type     Device type.
  @param[in] Name     Device name.

**/
VOID
AddMessageHeader (
  IN CHAR16      Message[MAX_STRING_LEN],
  IN DEVICE_TYPE Type,
  IN CHAR8       *Name
  )
{
  ZeroMem (Message, sizeof (Message[0]) * MAX_STRING_LEN);
  if (Name == NULL) {
    if (Type == NVME) {
      StrCatS (Message, MAX_STRING_LEN, L"Nvme Disk");
    } else if (Type == SATA) {
      StrCatS (Message, MAX_STRING_LEN, L"Sata Disk");
    } else if (Type == UFS) {
      StrCatS (Message, MAX_STRING_LEN, L"Ufs Disk");
    } else {
      StrCatS (Message, MAX_STRING_LEN, L"Unknow Disk");
    }
  } else {
    UnicodeSPrintAsciiFormat(Message, MAX_STRING_LEN, "%a", Name);
  }
}

/**
  Print message for erasing

  @param[in] Name  Device name.
  @param[in] Type  Device type.
  @param[in] Info  Information shown in print message.

**/
VOID
PrintEraseMessage (
  IN CHAR8       *Name,
  IN DEVICE_TYPE Type,
  IN CHAR16      *Info
  )
{
  CHAR16 Message[MAX_STRING_LEN];

  AddMessageHeader (Message, Type, Name);
  if (Info != NULL) {
    StrCatS (Message, MAX_STRING_LEN, Info);
  }
  OutputStringAtNext (Message, EFI_BACKGROUND_BLACK | EFI_WHITE);
}

/**
  Update message

  @param[in] Name  Device name.
  @param[in] Type  Device type.
  @param[in] Info  Information shown in print message.

**/
VOID
UpdateEraseMessage (
  IN CHAR8       *Name,
  IN DEVICE_TYPE Type,
  IN CHAR16      *Info
  )
{
  CHAR16 Message[MAX_STRING_LEN];

  AddMessageHeader (Message, Type, Name);
  if (Info != NULL) {
    StrCatS (Message, MAX_STRING_LEN, Info);
  }
  UpdateString (Message, EFI_BACKGROUND_BLACK | EFI_WHITE);
}

/**
  Convert the time from second to Hour : Minute : Second

  @param[in] Time    Time in second.
  @param[in] Hour    Hour.
  @param[in] Minute  Minute.
  @param[in] Second  Second.
**/
VOID
FormatTime (
  IN  UINT32  Time,
  OUT UINT32  *Hour,
  OUT UINT32  *Minute,
  OUT UINT32  *Second
  )
{
  if (Second != NULL) {
    *Second = Time % 60;
  }
  if (Hour != NULL) {
    *Hour   = Time / 3600;
  }
  if (Minute != NULL) {
    *Minute = (Time / 60) % 60;
  }
}

/**
  Convert the time from UINT64 to String

  @param[in]  Time     Time.
  @param[out] Message  Output the string for time in format Hour : Minute : Second or Second.
**/
CHAR16 *
TimeToString (
  IN  UINT64  Time,
  OUT CHAR16  Message[MAX_STRING_LEN]
  )
{
  UINT32  Minute;
  UINT32  Hour;
  UINT32  Second;
  UINT32  MaxTime;
  UINT32  MinTime;

  ZeroMem (Message, sizeof (Message[0]) * MAX_STRING_LEN);

  if (Time <= 0) {
    return NULL;
  }

  MaxTime = (UINT32) RShiftU64 (Time, 32);
  MinTime = (UINT32)Time;
  if (MaxTime == 0) {
    UnicodeSPrintAsciiFormat (Message, MAX_STRING_LEN, " (About %dS)", MinTime);
  } else {
    FormatTime (MinTime, &Hour, &Minute, &Second);
    UnicodeSPrintAsciiFormat (Message, MAX_STRING_LEN, " (%d:%d:%d - ", Hour, Minute, Second);
    FormatTime (MaxTime, &Hour, &Minute, &Second);
    UnicodeSPrintAsciiFormat (Message, MAX_STRING_LEN, "%s%d:%d:%d)", Message, Hour, Minute, Second);
  }
  DEBUG ((DEBUG_INFO, "%a: Time: %S \n" , __FUNCTION__, Message));
  return Message;
}

/**
  Append time at end of message

  @param[in] Time  Time to be shown.

**/
VOID
AppendEraseTime (
  IN UINT64  Time
  )
{
  CHAR16 TimeMessage[MAX_STRING_LEN];

  if (NULL != TimeToString (Time, TimeMessage)) {
    AppendString (TimeMessage, EFI_BACKGROUND_BLACK | EFI_WHITE);
  }
}

/**
  Get device type

  @param[in] DevicePath  Device path.

  @retval DEVICE_TYPE   return device type

**/
DEVICE_TYPE
GetDeviceType (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  if (EFI_SUCCESS == GetNamespaceId (DevicePath, NULL)) {
    return NVME;
  } else if (EFI_SUCCESS == GetPortNum (DevicePath, NULL, NULL)) {
    return SATA;
  } else if (EFI_SUCCESS == GetUfsDevice (DevicePath)) {
    return UFS;
  } else {
    return NOT_DEFINED;
  }
}

/**
  Update device command id, sata port id or nvme namespace id

  @param[in] DevicePath  Device path.
  @param[in] Dev         The device to update the command id.

**/
VOID
UpdateDeviceCmdId (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN PLATFORM_ERASE_DEVICE     *Dev
  )
{
  EFI_STATUS Status;
  UINT32     Nsid;
  UINT16     PortId;
  UINT16     PortMultiplier;

  if ((DevicePath == NULL) || (Dev == NULL))
    return;

  if (Dev->DeviceType == NVME) {
    Status = GetNamespaceId (DevicePath, &Nsid);
    if (!EFI_ERROR (Status)) {
      Dev->CmdId.NameSpaceId = Nsid;
    }
  } else if (Dev->DeviceType == SATA) {
    Status = GetPortNum (DevicePath, &PortId, &PortMultiplier);
    if (!EFI_ERROR (Status)) {
      Dev->CmdId.Sata.Port = PortId;
      Dev->CmdId.Sata.PortMultiplier = PortMultiplier;
    }
  }
}

/**
  Perform storage erase on NVMe, SATA, UFS device.

  @param[in] Dev              The device to perform data erase.
  @param[in] Password         The password used to unlock device.
  @param[in] HasOutputConsole The output console connected or not.

  @retval EFI_SUCCESS  The function completes successfully
  @retval others       Operation is failed due to some error.
**/
EFI_STATUS
PerformErase (
  IN PLATFORM_ERASE_DEVICE  *Dev,
  IN CHAR8                  *Password,
  IN BOOLEAN                HasOutputConsole
  )
{
  EFI_STATUS              Status;
  EFI_BLOCK_IO_PROTOCOL   *BlkIo;
  UINT32                  Index;
  UINTN                   Size;
  UINT32                  MediaId;
  UINT64                  Time;

  if (Dev == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status  = EFI_SUCCESS;
  BlkIo   = NULL;
  Size    = 0;
  MediaId = 0;

  if (Dev->Handle != NULL) {
    Status = gBS->HandleProtocol (Dev->Handle, &gEfiBlockIoProtocolGuid, (VOID **)&BlkIo);
    if (!EFI_ERROR (Status) && (BlkIo != NULL)) {
      Size = (BlkIo->Media->LastBlock + 1) * (BlkIo->Media->BlockSize);
      MediaId = BlkIo->Media->MediaId;
      // if the device doesn't have media, no need to erase
      if (Size == 0) {
        Dev->Result |= ERASE_RESULT_SUCCESS;
        return EFI_SUCCESS;
      }
    }
  }

  // erase storage, try all the sanitize type
  for (Index = 0; Index < SANITIZE_COUNT; Index ++) {
    if (HasOutputConsole) {
      if (Dev->VmdControlled != 0) {
        Time = GetRstEraseEstimateTime (Dev->CmdId, Dev->DeviceType, mSanitizeOrder[Index]);
      } else {
        Time = GetEraseEstimateTime (Dev->Handle, Dev->DeviceType, mSanitizeOrder[Index]);
      }
      AppendEraseTime (Time);
    }
    if (Dev->VmdControlled != 0) {
      Status = PerformRstEraseService (
                 Dev->CmdId,
                 Dev->DeviceType,
                 mSanitizeOrder[Index],
                 Size
                 );
    } else {
      Status = PerformEraseService (
                 Dev->Handle,
                 Dev->DeviceType,
                 mSanitizeOrder[Index],
                 MediaId,
                 Size
                 );
    }
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  DEBUG ((DEBUG_INFO, "%a: Erase, Status: %r \n" , __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    Dev->Result |= ERASE_RESULT_ERASE_OPERATION_FAILED;
  }

  if (Dev->VmdControlled != 0) {
    Status = GetRstEraseInfo (Dev->CmdId, Dev->DeviceType, &Dev->EraseInfo);
  } else {
    Status = GetEraseInfo (Dev->Handle, Dev->DeviceType, &Dev->EraseInfo);
  }
  if (EFI_ERROR (Status)) {
    Dev->Result |= ERASE_RESULT_ERASE_OPERATION_FAILED;
  } else {
    Dev->Result |= ERASE_RESULT_SUCCESS;
  }

  if (Dev->Result != ERASE_RESULT_SUCCESS) {
    Status = EFI_DEVICE_ERROR;
  } else {
    Status = EFI_SUCCESS;
  }
  return Status;
}

/**
  Perform storage erase on device.

  @param[in] Dev              The device to perform data erase.
  @param[in] Password         The password used to unlock device.
  @param[in] HasOutputConsole The output console connected or not.

  @retval EFI_SUCCESS     Storage erase is successful.
  @retval Others          failed to erase all devices.
**/
EFI_STATUS
PerformDeviceErase (
  IN PLATFORM_ERASE_DEVICE  *Dev,
  IN CHAR8*                 Password,
  IN BOOLEAN                HasOutputConsole
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  if ((Dev->DeviceType == NVME)
    || (Dev->DeviceType == UFS)
    || (Dev->DeviceType == SATA)) {
    Status = PerformErase (Dev, Password, HasOutputConsole);
  }

  return Status;
}

/**
  Perform unlock storage.

  @param[in] Dev              The device to perform data erase.
  @param[in] Password         The password used to unlock device.
  @param[in] HasOutputConsole The output console connected or not.

  @retval EFI_SUCCESS     Device is unlock.
  @retval Others          failed to unlock the device.
**/
EFI_STATUS
PerformUnlockStorage (
  IN PLATFORM_ERASE_DEVICE  *Dev,
  IN CHAR8*                 Password,
  IN BOOLEAN                HasOutputConsole
  )
{
  EFI_STATUS              Status;
  PLATFORM_OPAL_PROTOCOL  *PlatformOpalProtocol;
  BOOLEAN                 TrustedSupport;
  BOOLEAN                 IsEnabled;
  BOOLEAN                 HasPassword;
  CHAR16                  ModelName[MN_MAX_LEN + 1];

  if (Dev == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status      = EFI_SUCCESS;
  IsEnabled   = FALSE;
  HasPassword = FALSE;
  TrustedSupport = FALSE;

  ZeroMem (ModelName, sizeof (ModelName));
  UnicodeSPrintAsciiFormat(ModelName, sizeof (ModelName), "%a", Dev->EraseInfo.ModelName);
  if ((Password != NULL) && (AsciiStrLen (Password) != 0)) {
    HasPassword = TRUE;
  }

  Status = gBS->LocateProtocol (
                  &gPlatformOpalProtocolGuid,
                  NULL,
                  (VOID **) &PlatformOpalProtocol
                );
  if (!EFI_ERROR (Status)) {
    //
    // postpone erase when device is lock
    // but no password delivery and no output console to input password
    //
    Status = PlatformOpalProtocol->GetState (PlatformOpalProtocol, Dev->Handle, &IsEnabled, NULL);
    if (!EFI_ERROR (Status) && IsEnabled) {
      TrustedSupport = TRUE;
      if (!HasPassword && !HasOutputConsole) {
        Dev->Erased = FALSE;
        return EFI_NOT_READY;
      } else {
        Status = PlatformOpalProtocol->PerformOpalRevert (PlatformOpalProtocol, Dev->Handle, Password, ModelName);
        if (HasOutputConsole) {
          ClearPopupScreen ();
        }
        if (EFI_ERROR (Status) && (Status != EFI_UNSUPPORTED)) {
          // Revert failed
          Dev->Result |= ERASE_RESULT_DISABLE_SECURITY_FAILED;
          return EFI_ACCESS_DENIED;
        }
      }
    }
  }
  if ((Dev->DeviceType == SATA) && !TrustedSupport) { // SATA
    //
    // postpone erase when security feature is enabled
    // but no password delivery and no output console to input
    //
    IsEnabled = GetAtaSecurityStateWithPortId (Dev->Handle, Dev->ParentHandle, Dev->CmdId.Sata.Port, Dev->CmdId.Sata.PortMultiplier);
    if (IsEnabled) {
      if (!HasPassword && !HasOutputConsole) {
        Dev->Erased = FALSE;
        return EFI_NOT_READY;
      } else {
        Status = DisableAtaSecurityWithPortId (Dev->Handle, Dev->ParentHandle, Dev->CmdId.Sata.Port, Dev->CmdId.Sata.PortMultiplier, Password, ModelName);
        if (HasOutputConsole) {
          ClearPopupScreen ();
        }
        if (EFI_ERROR (Status)) {
          Dev->Result |= ERASE_RESULT_DISABLE_SECURITY_FAILED;
          return EFI_ACCESS_DENIED;
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Check if the Sid is blocked or not.

  @param[in] Dev            The device to perform data erase.

  @retval EFI_SUCCESS  The function completes successfully
  @retval others       Operation is failed due to some error.
**/
BOOLEAN
CheckSidBlockedStatus (
  IN PLATFORM_ERASE_DEVICE  *Dev
  )
{
  EFI_STATUS Status;
  UINT8      BlockSidState;
  PLATFORM_OPAL_PROTOCOL  *PlatformOpalProtocol;

  BlockSidState = 0;
  if (Dev == NULL) {
    return FALSE;
  }

  Status = gBS->LocateProtocol (&gPlatformOpalProtocolGuid, NULL, (VOID **) &PlatformOpalProtocol);
  if (!EFI_ERROR (Status)) {
    Status = PlatformOpalProtocol->GetBlockSidState (PlatformOpalProtocol, Dev->Handle, &BlockSidState);
    if (!EFI_ERROR (Status) && (BlockSidState == 1)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Check the SATA device if it is frozen or not.

  @param[in] Dev            The device to perform data erase.

  @retval EFI_SUCCESS  The function completes successfully
  @retval others       Operation is failed due to some error.
**/
BOOLEAN
CheckSataFrozenStatus (
  IN PLATFORM_ERASE_DEVICE  *Dev
  )
{
  EFI_STATUS Status;
  UINT8      FrozenState;

  FrozenState = 0;
  if (Dev == NULL) {
    return FALSE;
  }

  Status = GetAtaFrozenStateWithPortId (Dev->Handle, Dev->ParentHandle, Dev->CmdId.Sata.Port, Dev->CmdId.Sata.PortMultiplier, &FrozenState);
  if (!EFI_ERROR (Status) && (FrozenState == 1)) {
    return TRUE;
  } else {
    DEBUG ((DEBUG_INFO, "%a: Get ATA frozen state %r. \n", __FUNCTION__, Status));
  }
  return FALSE;
}

/**
  Check on device if it need G3.

  @param[in] Dev            The device to perform data erase.

  @retval EFI_SUCCESS  The function completes successfully
  @retval others       Operation is failed due to some error.
**/
BOOLEAN
CheckG3Status (
  IN PLATFORM_ERASE_DEVICE  *Dev
  )
{
  if (Dev == NULL) {
    return FALSE;
  }

  if (Dev->DeviceType == SATA) {
    if (CheckSataFrozenStatus (Dev)) {
      return TRUE;
    }
  }

  return CheckSidBlockedStatus (Dev);
}

/**
  Get the time for verify device.

  @param[in] Dev  The device to verify.

  @retval UINT64  Time need to verify
**/
UINT64
GetVerifyTime (
  IN PLATFORM_ERASE_DEVICE  *Dev
  )
{
  UINT64 Time;
  UINT32 MaxTime;
  UINT32 MinTime;

  if (Dev->VmdControlled != 0) {
    Time = GetRstEraseEstimateTime (Dev->CmdId, Dev->DeviceType, SANITIZE_CLEAR);
  } else {
    Time = GetEraseEstimateTime (Dev->Handle, Dev->DeviceType, SANITIZE_CLEAR);
  }
  if (Time != 0) {
    MaxTime = (UINT32) RShiftU64 (Time, 32);
    MinTime = (UINT32)Time;
    Time = 0;
    Time += (MaxTime / 10);
    Time = LShiftU64 (Time, 32);
    Time += (MinTime / 10);
  }
  return Time;
}
