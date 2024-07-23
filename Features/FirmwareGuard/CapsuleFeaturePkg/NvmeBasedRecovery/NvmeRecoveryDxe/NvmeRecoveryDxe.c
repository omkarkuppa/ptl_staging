/**@file
  NvmeRecovery Dxe driver, main entry of the Nvme recovery feature.

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

#include <Protocol/MeFirmwareUpdate.h>
#include <Library/HashSupportLib.h>
#include "NvmeRecoveryDxe.h"
#include "NvmeRecoveryHii.h"

extern PCH_SPI_PROTOCOL  *mSpiProtocol;

GLOBAL_REMOVE_IF_UNREFERENCED UINT8  mRpmbKey256[RPMB_WRAPPED_KEY_SIZE] = {0};

#define RPMB_MAX_PASSWORD_SIZE         32

UINT8  mKeyMigrationAction = NVME_RPMB_KEY_MIGRATION_VALUE_NO_ACTION;

/**
  Initialize PCH SpiProtocol interface module pointer

  @retval EFI_SUCCESS      PCH SpiProtocol interface module pointer is initialized successfully
  @retval Others           Fail to locate PCH SpiProtocol interface

**/
EFI_STATUS
InitializeSpiProtocolInterface (
  VOID
  )
{
  EFI_STATUS                   Status;

  Status = EFI_SUCCESS;

  if (mSpiProtocol == NULL) {
    Status = gBS->LocateProtocol (
                    &gPchSpiProtocolGuid,
                    NULL,
                    (VOID **)&mSpiProtocol
                    );
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  Return the first Protocol Interface that matches the Protocol GUID.

  Expect platform should only have one FWUpdateLib instance in a time.

  @param[in]  MeFwu  Pointer to firmware update protocol.

  @retval  EFI_SUCCESS            If a valid Interface is returned.
  @retval  EFI_INVALID_PARAMETER  Invalid parameter.
  @retval  EFI_NOT_FOUND          Protocol interface not found.

**/
EFI_STATUS
GetMeFwuProtocol (
  IN ME_FIRMWARE_UPDATE_PROTOCOL  **MeFwu
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  //
  // It should be only one FWUpdate instance driver could be installed.
  //
  Status = gBS->LocateProtocol (&gMeFwuProtocol, NULL, (void **)MeFwu);
  DEBUG ((DEBUG_INFO, "Locate MeFwuProtocol Status: %r\n", Status));

  return Status;
}

/**
  Make a check if it is a NVMe device by device path.

  @param  DevicePath       The NVMe device path.

  @retval TRUE     it is an NVMe device.
  @retval FALSE    it is not an NVMe device.

**/
BOOLEAN
IsNvmeDevice (
  IN  CONST EFI_DEVICE_PATH_PROTOCOL   *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL          *TempDevicePath;

  TempDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) DevicePath;
  while (!IsDevicePathEndType (TempDevicePath)) {
    if (DevicePathType (TempDevicePath) == MESSAGING_DEVICE_PATH &&
          DevicePathSubType (TempDevicePath) == MSG_NVME_NAMESPACE_DP) {
      return TRUE;
    }
    TempDevicePath = NextDevicePathNode (TempDevicePath);
  }
  return FALSE;
}

/**
  Check if this is the first boot for NVMe-based Recovery.

  @param  NvmeBackupInfo The NVMe backup info. It must not be NULL.

  @retval FALSE It's not the first boot for NVMe-based Recovery.
  @retval TRUE  It's the first boot for NVMe-based Recovery.
**/
BOOLEAN
IsNvmeRecoveryFirstBoot (
  IN OUT NVME_BACKUP_INFO  *NvmeBackupInfo
  )
{
  EFI_STATUS        Status;
  UINTN             VarSize;

  VarSize = sizeof (NVME_BACKUP_INFO);
  Status  = gRT->GetVariable (
                   NVME_BASED_RECOVERY_INFO_VARIABLE,
                   &gNvmeBasedRecoveryVarGuid,
                   NULL,
                   &VarSize,
                   NvmeBackupInfo
                   );
  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      DEBUG ((DEBUG_ERROR, "Failed to get the variable\n"));
      ASSERT_EFI_ERROR (Status);
    }
    //
    // Initial BackupStatus
    //
    NvmeBackupInfo->BackupStatus = BackupNotStarted;
    return TRUE;
  }
  if (VarSize != sizeof (NVME_BACKUP_INFO)) {
    DEBUG ((DEBUG_INFO, "NVMe Based Recovery variable size not aligned\n"));
  }
  if (NvmeBackupInfo->Revision != NVME_BACKUP_INFO_REVISION) {
    DEBUG ((DEBUG_INFO, "Nvme Backup Info structure has been changed\n"));
  }
  return FALSE;
}

/**
  Get password input from the popup window.

  @param[in]  PopUpString1  Pop up string 1.
  @param[in]  PopUpString2  Pop up string 2.
  @param[in]  PopUpString3  Pop up string 3.
  @param[out] PressEsc      Whether user escape function through Press ESC.

  @retval Password string if success. NULL if failed.

**/
EFI_STATUS
NvmeResiliencyPopUpPinKeyInput (
  IN CHAR16                                   *PopUpString1,
  IN CHAR16                                   *PopUpString2,
  IN CHAR16                                   *PopUpString3,
  IN OUT CHAR8                                *Pinkey
  )
{
  EFI_INPUT_KEY             InputKey;
  UINTN                     InputLength;
  CHAR16                    Mask[RPMB_MAX_PASSWORD_SIZE + 1];
  CHAR16                    Unicode[RPMB_MAX_PASSWORD_SIZE + 1];

  ZeroMem(Unicode, sizeof(Unicode));
  ZeroMem(Mask, sizeof(Mask));

  gST->ConOut->ClearScreen(gST->ConOut);

  InputLength = 0;
  while (TRUE) {
    Mask[InputLength] = L'_';
    if (PopUpString2 == NULL) {
      CreatePopUp (
        EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
        &InputKey,
        PopUpString1,
        L"---------------------",
        Mask,
        NULL
      );
    } else {
      if (PopUpString3 == NULL) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &InputKey,
          PopUpString1,
          PopUpString2,
          L"---------------------",
          Mask,
          NULL
        );
      } else {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &InputKey,
          PopUpString1,
          PopUpString2,
          PopUpString3,
          L"---------------------",
          Mask,
          NULL
        );
      }
    }

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
          if (InputLength == RPMB_MAX_PASSWORD_SIZE) {
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
      break;
    }
  }

  gST->ConOut->ClearScreen(gST->ConOut);

  if (InputLength == 0 || InputKey.ScanCode == SCAN_ESC) {
    ZeroMem (Unicode, sizeof (Unicode));
    return EFI_ABORTED;
  }

  UnicodeStrToAsciiStrS (Unicode, Pinkey, RPMB_MAX_PASSWORD_SIZE + 1);
  ZeroMem (Unicode, sizeof (Unicode));

  return EFI_SUCCESS;
}

/**
  Get RPMB data.

  @param[in]   Ssp                    The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param[in]   MediaId                Id of the media, changes every time the media is replaced.
  @param[out]  RpmbDataHeader         The pointer as RPMB data Header.
  @param[in]   RpmbDataHeaderSize     The size of RPMB data Header.
  @param[out]  WrappedInfoHeader      The pointer as wrapped key info list header.
  @param[in]   WrappedInfoHeaderSize  The size of wrapped key info list header.
  @param[out]  WrappedKeyList         The pointer as wrapped key list.
  @param[in]   WrappedKeyListSize     The size of wrapped key list.

  @retval  EFI_SUCCESS  Successfully get RPMB data.
  @retval  Others       Failed to get RPMB data.

**/
EFI_STATUS
GetRpmbData (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL  *Ssp,
  IN  UINT32                                 MediaId,
  OUT NVME_PRIVATE_DATA_IN_RPMB              *RpmbDataHeader,
  IN  UINT32                                 RpmbDataHeaderSize,
  OUT WRAPPED_KEY_INFO_LIST                  *WrappedInfoHeader,
  IN  UINT32                                 WrappedInfoHeaderSize,
  OUT AES256_WRAPPED_KEY                     *WrappedKeyList,
  IN  UINT32                                 WrappedKeyListSize
  )
{
  EFI_STATUS  Status;
  UINT8       RpmbTarget;
  UINT32      WrappedKeyBufferSize;
  UINT32      RpmbDataBufferSize;
  UINT8       *RpmbDataBuffer;
  UINT32      Offset;

  RpmbTarget         = 0;
  RpmbDataBufferSize = RpmbDataHeaderSize;
  RpmbDataBuffer     = AllocateZeroPool (RpmbDataBufferSize);
  if (RpmbDataBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Read the private data header in RPMB target.
  //
  Status = NvmeAuthenticatedDataRead (Ssp, MediaId, RpmbTarget, RpmbDataBuffer, RpmbDataBufferSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Can't get RPMB data header from RPMB target[%x], Status:[%x]. \n", RpmbTarget, Status));
    goto CleanUp;
  }

  CopyMem (RpmbDataHeader, RpmbDataBuffer, RpmbDataBufferSize);
  FreePool (RpmbDataBuffer);

  if (RpmbDataHeader->Signature != NVME_PRIVATE_DATA_IN_RPMB_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Wrong Signature of the private data struture.\n"));
    return EFI_UNSUPPORTED;
  }
  if (RpmbDataHeader->Revision != NVME_PRIVATE_DATA_IN_RPMB_REVISION) {
    DEBUG ((DEBUG_ERROR, "Private Data Header Changed, update RPMB data\n"));
    RpmbDataHeader->Revision = NVME_PRIVATE_DATA_IN_RPMB_REVISION;
  }

  RpmbDataBufferSize = RpmbDataHeader->TotalSize;
  RpmbDataBuffer     = AllocateZeroPool (RpmbDataHeader->TotalSize);
  if (RpmbDataBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Read RPMB data.
  //
  Status = NvmeAuthenticatedDataRead (Ssp, MediaId, RpmbTarget, RpmbDataBuffer, RpmbDataBufferSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Can't get WrappedKey info from RPMB target[%x], Status:[%x]. \n", RpmbTarget, Status));
    goto CleanUp;
  }

  //
  // Get the wrapped key info list
  //
  Offset = RpmbDataHeader->WrappedKeyInfoList;
  if ((Offset + WrappedInfoHeaderSize) > RpmbDataHeader->TotalSize) {
    Status = EFI_BUFFER_TOO_SMALL;
    goto CleanUp;
  }
  CopyMem (WrappedInfoHeader, RpmbDataBuffer + Offset, WrappedInfoHeaderSize);

  if (WrappedInfoHeader->Revision != WRAPPED_KEY_INFO_LIST_REVISION) {
    DEBUG ((DEBUG_ERROR, "Wrapped key info list Changed, update RPMB data\n"));
    WrappedInfoHeader->Revision = WRAPPED_KEY_INFO_LIST_REVISION;
  }

  //
  // Get the wrapped key list data
  //
  Offset = RpmbDataHeader->WrappedKeyInfoList + WrappedInfoHeader->HeaderSize;
  WrappedKeyBufferSize = sizeof (AES256_WRAPPED_KEY) * WrappedInfoHeader->Count;
  if (((Offset + WrappedKeyBufferSize) > RpmbDataHeader->TotalSize) || (WrappedKeyBufferSize > WrappedKeyListSize)) {
    Status = EFI_BUFFER_TOO_SMALL;
    goto CleanUp;
  }
  CopyMem (WrappedKeyList, RpmbDataBuffer + RpmbDataHeader->WrappedKeyInfoList + WrappedInfoHeader->HeaderSize, WrappedKeyBufferSize);

CleanUp:
  if (RpmbDataBuffer != NULL) {
    ZeroMem (RpmbDataBuffer, RpmbDataBufferSize);
    FreePool (RpmbDataBuffer);
  }
  DEBUG ((DEBUG_INFO, "Get RPMB data %r\n", Status));
  return Status;
}

/**
  Set RPMB data to SSD RPMB.

  @param[in]   Ssp                The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param[in]   MediaId            Id of the media, changes every time the media is replaced.
  @param[in]  RpmbDataHeader      The pointer as RPMB data Header.
  @param[in]  WrappedInfoHeader   The pointer as wrapped key info list header.
  @param[in]  WrappedKeyList      The pointer as wrapped key list.
  @param[in]  AccessSize          The number of written per RPMB access by Security Send or Security Receive commands
                                  for the controller as wrapped key list.

  @retval  EFI_SUCCESS  Successfully set RPMB data.
  @retval  Others       Failed to set RPMB data.

**/
EFI_STATUS
SetRpmbData (
  IN EFI_STORAGE_SECURITY_COMMAND_PROTOCOL  *Ssp,
  IN UINT32                                 MediaId,
  IN NVME_PRIVATE_DATA_IN_RPMB              *RpmbDataHeader,
  IN WRAPPED_KEY_INFO_LIST                  *WrappedInfoHeader,
  IN AES256_WRAPPED_KEY                     *WrappedKeyList,
  IN UINT8                                  AccessSize
  )
{
  EFI_STATUS  Status;
  UINT8       RpmbTarget;
  UINT8       *RpmbDataBuffer;
  UINT32      WrappedKeyListSize;

  RpmbTarget         = 0;
  WrappedKeyListSize = sizeof (AES256_WRAPPED_KEY) * WrappedInfoHeader->Count;
  RpmbDataBuffer     = NULL;

  //
  // Allocate the buffer to build the private data in RPMB
  //
  RpmbDataBuffer = AllocateZeroPool (RpmbDataHeader->TotalSize);
  if (RpmbDataBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Add RPMB data info
  //
  CopyMem (RpmbDataBuffer, RpmbDataHeader, RpmbDataHeader->HeaderSize);
  CopyMem (RpmbDataBuffer + RpmbDataHeader->WrappedKeyInfoList, WrappedInfoHeader, WrappedInfoHeader->HeaderSize);
  CopyMem (RpmbDataBuffer + RpmbDataHeader->WrappedKeyInfoList + WrappedInfoHeader->HeaderSize, WrappedKeyList, WrappedKeyListSize);
  //
  // Check the RPMB size of write access is enough for our data to be written in.
  //   (1) In here we have hard assumption,
  //       - Data is to be written from the beginning offset (0x0) of RPMB partition.
  //       - The whole partition of RPMB could be used.
  //   (2) Based on the NVMe Base Specification 2.0c section 5.17.2.1
  //       Write access is defined in 312-315 bytes and in bits 24-31.
  //       - If the value is not zero, then it is a multiple of 512B unit.
  //       - If the value is zero, then consider it is supported 512B unit only.
  //
  if (((AccessSize == 0) && (NVME_RPMB_TARGET_MIN_UNIT < RpmbDataHeader->TotalSize)) || \
      ((AccessSize != 0) && (AccessSize * NVME_RPMB_TARGET_MIN_UNIT) < RpmbDataHeader->TotalSize)) {
    Status = EFI_BUFFER_TOO_SMALL;
    return Status;
  }

  Status = NvmeAuthenticatedDataWrite (Ssp, MediaId, RpmbTarget, RpmbDataBuffer, RpmbDataHeader->TotalSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RPMB data programming is failed, Status:[%x]. \n", Status));
  } else {
    DEBUG ((DEBUG_ERROR, "RPMB data programming is succeed, Status:[%x]. \n", Status));
  }

  if (RpmbDataBuffer != NULL) {
    ZeroMem (RpmbDataBuffer, RpmbDataHeader->TotalSize);
    FreePool (RpmbDataBuffer);
  }

  return Status;
}

/**
  Initial RPMB area data when first program.

  //
  //   The layout of private data in RPMB
  //
  //   |--------------------------------------|
  //   |+Private data in RPMB header++++++++++|
  //   |--------------------------------------|
  //   |+Wrapped key list info header+++++++++|
  //   |--------------------------------------|
  //   |+Seed Wrapped Key data++++++++++++++++|
  //   |--------------------------------------|
  //   |+Pin Wrapped Key data(optional)+++++++|
  //   |--------------------------------------|
  //

  @param[in]  Ssp          The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param[in]  MediaId      Id of the media, changes every time the media is replaced.
  @param[in]  WrappingKey  The pointer as Wrapping key.
  @param[in]  AccessSize   The number of written per RPMB access by Security Send or Security Receive commands
                           for the controller as wrapped key list.
  @retval  EFI_SUCCESS  Successfully init RPMB data.
  @retval  Others       Failed init RPMB data.

**/
EFI_STATUS
InitRpmbData (
  IN EFI_STORAGE_SECURITY_COMMAND_PROTOCOL  *Ssp,
  IN UINT32                                 MediaId,
  IN UINT8                                  *WrappingKey,
  IN UINT8                                  AccessSize
  )
{
  EFI_STATUS                 Status;
  NVME_PRIVATE_DATA_IN_RPMB  RpmbDataHeader;
  WRAPPED_KEY_INFO_LIST      WrappedInfoHeader;
  AES256_WRAPPED_KEY         *WrappedKeyList;
  UINT32                     WrappedKeyListSize;

  Status             = EFI_UNSUPPORTED;
  WrappedKeyListSize = sizeof (AES256_WRAPPED_KEY);
  WrappedKeyList     = NULL;

  WrappedKeyList = AllocateZeroPool (WrappedKeyListSize);
  if (WrappedKeyList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initial private data header and wrapped key info list
  //
  RpmbDataHeader.Signature          = NVME_PRIVATE_DATA_IN_RPMB_SIGNATURE;
  RpmbDataHeader.Revision           = NVME_PRIVATE_DATA_IN_RPMB_REVISION;
  RpmbDataHeader.HeaderSize         = (UINT16)(sizeof (NVME_PRIVATE_DATA_IN_RPMB));
  RpmbDataHeader.WrappedKeyInfoList = RpmbDataHeader.HeaderSize;

  WrappedInfoHeader.Revision   = WRAPPED_KEY_INFO_LIST_REVISION;
  WrappedInfoHeader.HeaderSize = (UINT16)(sizeof (WRAPPED_KEY_INFO_LIST));
  WrappedInfoHeader.CryptoAlg  = AES_256_CBC;

  WrappedKeyList->WrappedKeySize = RPMB_WRAPPED_KEY_SIZE;
  WrappedKeyList->KeyTag         = SEED_WRAPPED;

  //
  // Get RPMB from random value which was saved to gWrappingKey256
  //
  if (AesCbcRequest (WrappedKeyList->WrappedKey, WrappingKey, mRpmbKey256, RPMB_WRAPPED_KEY_SIZE, WrappedKeyList->InitialVector, TRUE)) {
    WrappedInfoHeader.Count  = 1;
    RpmbDataHeader.TotalSize = (UINT16)(RpmbDataHeader.HeaderSize + WrappedInfoHeader.HeaderSize + (sizeof (AES256_WRAPPED_KEY)) * WrappedInfoHeader.Count);
    Status = SetRpmbData (Ssp, MediaId, &RpmbDataHeader, &WrappedInfoHeader, WrappedKeyList, AccessSize);
  }

  if (WrappedKeyList != NULL) {
    ZeroMem (WrappedKeyList, WrappedKeyListSize);
    FreePool (WrappedKeyList);
  }
  return Status;
}

/**
  Get wrapped key by KeyTag.

  @param[in]   KeyTag          The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param[in]   WrappedKeyList  The pointer to the wrapped key list.
  @param[out]  PointerKey      The pointer to the special key data.

  @retval  EFI_SUCCESS  Find wrapped key succeed.
  @retval  Others       Find wrapped key failed.

**/
EFI_STATUS
GetWrappedKeyByTag (
  IN  UINT32              KeyTag,
  IN  AES256_WRAPPED_KEY  *WrappedKeyList,
  OUT AES256_WRAPPED_KEY  *PointerKey
  )
{
  UINTN  Index;

  for (Index = 0; Index < AES256_WRAPPED_KEY_MAX_COUNT; Index++) {
    if (WrappedKeyList[Index].KeyTag == KeyTag) {
      CopyMem (PointerKey, &(WrappedKeyList[Index]), sizeof (AES256_WRAPPED_KEY));
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/**
  Replace wrapped key list data by KeyTag.

  @param[in]   KeyTag         KeyTag of the PointerKey.
  @param[out]  WrappedKeyLis  The pointer to the wrapped key list.
  @param[in]   PointerKey     The pointer to the special key data.

  @retval  EFI_SUCCESS  Find wrapped key succeed.
  @retval  Others       Find wrapped key failed.

**/
EFI_STATUS
ReplaceWrappedKeyByTag (
  IN  UINT32              KeyTag,
  OUT AES256_WRAPPED_KEY  *WrappedKeyList,
  IN  AES256_WRAPPED_KEY  *PointerKey
  )
{
  UINTN  Index;

  for (Index = 0; Index < AES256_WRAPPED_KEY_MAX_COUNT; Index++) {
    if (WrappedKeyList[Index].KeyTag == KeyTag) {
      CopyMem (&(WrappedKeyList[Index]), PointerKey, sizeof (AES256_WRAPPED_KEY));
      return EFI_SUCCESS;
    }
  }
  DEBUG ((DEBUG_ERROR, "Replace wrapped key fail.\n"));
  return EFI_NOT_FOUND;
}

/**
  Decrypt RPMB key from the pin wrapped key.

  @param[in]  PinWrappedKey  The pointer to the pin Wrapping key.

  @retval  EFI_SUCCESS  Successfully decrypt RPMB key.
  @retval  Others       Failed to decrypt RPMB key.

**/
EFI_STATUS
GetRpmbKeyFromPinKey (
  IN AES256_WRAPPED_KEY  *PinWrappedKey
  )
{
  UINT8       PinKey[RPMB_MAX_PASSWORD_SIZE + 1];
  EFI_STATUS  Status;

  ZeroMem (PinKey, RPMB_MAX_PASSWORD_SIZE + 1);

  //
  // Unwrapped PinWrappedKey
  //
  DEBUG ((DEBUG_INFO,"Unlock RPMB key from Pin Wrapped Key\n"));
  Status = NvmeResiliencyPopUpPinKeyInput (L"Get Migration Pin key", NULL, NULL, (CHAR8 *) PinKey);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Decrypt RPMB key with PinKey
  //
  if (AesCbcRequest (mRpmbKey256, PinKey, PinWrappedKey->WrappedKey, PinWrappedKey->WrappedKeySize, PinWrappedKey->InitialVector, FALSE)) {
    DEBUG ((DEBUG_ERROR, "Unlock Success\n"));
  } else {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "Unlock Fail\n"));
  }

  ZeroMem (PinKey, RPMB_MAX_PASSWORD_SIZE + 1);
  return Status;
}

/**
  Decrypt RPMB key from the data.

  @param[in]  WrappingKey     The pointer as Wrapping key.
  @param[in]  WrappedKeyList  The pointer as wrapped key list.

  @retval  EFI_SUCCESS  Successfully decrypt RPMB key.
  @retval  Others       Failed to decrypt RPMB key.

**/
EFI_STATUS
GetRpmbKey (
  IN UINT8               *WrappingKey,
  IN AES256_WRAPPED_KEY  *WrappedKeyList
  )
{
  EFI_STATUS          Status;
  AES256_WRAPPED_KEY  *PointerKey;

  PointerKey = NULL;
  PointerKey = AllocateZeroPool (sizeof (AES256_WRAPPED_KEY));
  if (PointerKey == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (mKeyMigrationAction == NVME_RPMB_KEY_MIGRATION_VALUE_UNLOCK_BY_PIN_KEY) {
    Status = GetWrappedKeyByTag (PIN_WRAPPED, WrappedKeyList, PointerKey);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Get pin wrapped key by tag fail.\n"));
      goto CleanUp;
    }
    Status = GetRpmbKeyFromPinKey (PointerKey);
    goto CleanUp;
  }

  Status = GetWrappedKeyByTag (SEED_WRAPPED, WrappedKeyList, PointerKey);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get seed wrapped key by tag fail.\n"));
    goto CleanUp;
  }
  if (AesCbcRequest (mRpmbKey256, WrappingKey, PointerKey->WrappedKey, PointerKey->WrappedKeySize, PointerKey->InitialVector, FALSE) == FALSE) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "RPMB Key Provision: AesCbcSha256Request is failed. \n"));
    ASSERT (FALSE);
  }

CleanUp:
  if (PointerKey != NULL) {
    ZeroMem (PointerKey, sizeof (AES256_WRAPPED_KEY));
    FreePool (PointerKey);
  }
  return Status;
}

/**
  Set WrappedKey2 with PinKey as pin wrapped key.

  @param[out]  RpmbDataHeader     The pointer as RPMB data Header.
  @param[out]  WrappedInfoHeader  The pointer as wrapped key info list header.
  @param[out]  WrappedKeyList     The pointer as wrapped key list.

  @retval  EFI_SUCCESS  Successfully save pin wrapped key to RPMB.
  @retval  Others       Failed to save pin wrapped key to RPMB.
**/
EFI_STATUS
SetPinWrappedKey (
  OUT NVME_PRIVATE_DATA_IN_RPMB  *RpmbDataHeader,
  OUT WRAPPED_KEY_INFO_LIST      *WrappedInfoHeader,
  OUT AES256_WRAPPED_KEY         *WrappedKeyList
  )
{
  EFI_STATUS          Status;
  AES256_WRAPPED_KEY  *PinWrappedKey;
  AES256_WRAPPED_KEY  PointerKey;
  UINT8               PinKey[RPMB_MAX_PASSWORD_SIZE + 1];

  ZeroMem (PinKey, RPMB_MAX_PASSWORD_SIZE + 1);
  ZeroMem (&PointerKey, sizeof (AES256_WRAPPED_KEY));
  PinWrappedKey = AllocateZeroPool (sizeof (AES256_WRAPPED_KEY));
  if (PinWrappedKey == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO,"Prepare for key migration \n"));
  Status = NvmeResiliencyPopUpPinKeyInput (L"Set Migration Pin key", NULL, NULL, (CHAR8 *) PinKey);
  if (EFI_ERROR (Status)) {
    goto CleanUp;
  }

  PinWrappedKey->KeyTag         = PIN_WRAPPED;
  PinWrappedKey->WrappedKeySize = RPMB_WRAPPED_KEY_SIZE;
  //
  // Encrypt RPMB key with PinKey
  //
  if (AesCbcRequest (PinWrappedKey->WrappedKey, PinKey, mRpmbKey256, RPMB_WRAPPED_KEY_SIZE, PinWrappedKey->InitialVector, TRUE) == FALSE) {
    Status = EFI_UNSUPPORTED;
    goto CleanUp;
  }

  Status = GetWrappedKeyByTag (PIN_WRAPPED, WrappedKeyList, &PointerKey);
  if (EFI_ERROR (Status)) {
    if ((WrappedInfoHeader->Count + 1) <= AES256_WRAPPED_KEY_MAX_COUNT) {
      // Create new wrapped key from pinkey + RPMB key
      (WrappedInfoHeader->Count)++;
      RpmbDataHeader->TotalSize += sizeof (AES256_WRAPPED_KEY);
      CopyMem (&(WrappedKeyList[WrappedInfoHeader->Count - 1]), PinWrappedKey, sizeof (AES256_WRAPPED_KEY));
      Status = EFI_SUCCESS;
    }
  } else {
    Status = ReplaceWrappedKeyByTag (PIN_WRAPPED, WrappedKeyList, PinWrappedKey);
  }

CleanUp:
  ZeroMem (PinKey, RPMB_MAX_PASSWORD_SIZE + 1);
  ZeroMem (&PointerKey, sizeof (AES256_WRAPPED_KEY));
  if (PinWrappedKey != NULL) {
    ZeroMem (PinWrappedKey, sizeof (AES256_WRAPPED_KEY));
    FreePool (PinWrappedKey);
  }
  DEBUG ((DEBUG_INFO, "Set Pin Wrapped Key %r\n", Status));
  return Status;
}

/**
  Set Wrapped Key by KeyTag after unlock RPMB key.

  @param[in]   KeyTag          KeyTag of the PointerKey.
  @param[in]   WrappingKey     The pointer as Wrapping key.
  @param[out]  WrappedKeyList  The pointer as wrapped key list.

  @retval  EFI_SUCCESS  Successfully save wrapped key.
  @retval  Others       Failed to save wrapped key.
**/
EFI_STATUS
SetWrappedKeybyKeyTag (
  IN  UINT32              KeyTag,
  IN  UINT8               *WrappingKey,
  OUT AES256_WRAPPED_KEY  *WrappedKeyList
  )
{
  EFI_STATUS          Status;
  AES256_WRAPPED_KEY  SeedWrappedKey;

  Status = EFI_UNSUPPORTED;
  ZeroMem (&SeedWrappedKey, sizeof (AES256_WRAPPED_KEY));
  //
  // Unwrapped PinWrappedKey
  //
  DEBUG ((DEBUG_INFO,"Start for key migration \n"));
  SeedWrappedKey.KeyTag         = KeyTag;
  SeedWrappedKey.WrappedKeySize = RPMB_WRAPPED_KEY_SIZE;
  //
  // Encrypt with wrapping key
  //
  if (AesCbcRequest (SeedWrappedKey.WrappedKey, WrappingKey, mRpmbKey256, RPMB_WRAPPED_KEY_SIZE, SeedWrappedKey.InitialVector, TRUE)) {
    Status = ReplaceWrappedKeyByTag (KeyTag, WrappedKeyList, &SeedWrappedKey);
    if (EFI_ERROR (Status)) {
      ZeroMem (&SeedWrappedKey, sizeof (AES256_WRAPPED_KEY));
      return Status;
    }
  }
  ZeroMem (&SeedWrappedKey, sizeof (AES256_WRAPPED_KEY));
  return Status;
}

/**
  Program RpmbKey and WrappedKey to Nvme Device, WrappedKey should be the generation
  by RpmbKey + AesCbc encryption.

  @param[in]  Ssp             The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param[in]  MediaId         Id of the media, changes every time the media is replaced.
  @param[in]  ControllerData  Input of current device's controller data.

  @retval  EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval  EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval  EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                        or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                        is non-zero.
  @retval  EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                        data from the device. The PayloadBuffer contains the truncated data.
  @retval  EFI_UNSUPPORTED              The given MediaId does not support security protocol commands, or Csme seed
                                        is not available.
  @retval  EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval  EFI_NO_MEDIA                 There is no media in the device.
  @retval  EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval  EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                        protocol command to execute.

**/
EFI_STATUS
NvmeBasedRecoveryGetSetRpmbKey (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN  UINT32                                  MediaId,
  IN  NVME_ADMIN_CONTROLLER_DATA              *ControllerData
  )
{
  EFI_STATUS                 Status;
  UINT8                      WrappingKey[AES256_KEY_BYTE_SIZE];
  EFI_HOB_GUID_TYPE          *GuidHob;
  GET_BIOS_SEED_RESPONSE     *BiosSeedContent;
  UINTN                      BlockCount;
  UINT64                     TempSeed[2];
  UINTN                      Index;
  NVME_PRIVATE_DATA_IN_RPMB  *RpmbDataHeader;
  UINT32                     RpmbDataHeaderSize;
  WRAPPED_KEY_INFO_LIST      *WrappedInfoHeader;
  UINT32                     WrappedInfoHeaderSize;
  AES256_WRAPPED_KEY         *WrappedKeyList;
  UINT32                     WrappedKeyListSize;
  UINT8                      AccessSize;

  BlockCount        = 0;
  Index             = 0;
  BiosSeedContent   = NULL;
  RpmbDataHeader    = NULL;
  WrappedInfoHeader = NULL;
  WrappedKeyList    = NULL;
  AccessSize        = (UINT8)(((ControllerData->Rpmbs) & NVME_RPMB_ACCESS_SIZE_MASK) >> NVME_RPMB_ACCESS_SIZE_SHIFT);

  BlockCount = sizeof (mRpmbKey256) / 16;
  while (Index < BlockCount) {
    if (GetRandomNumber128 (TempSeed) != TRUE) {
      ASSERT (FALSE);
      return EFI_UNSUPPORTED;
    }
    CopyMem (mRpmbKey256 + Index * 16, TempSeed, 16);
    Index++;
  }

  GuidHob = GetFirstGuidHob (&gCsmeSeedHobGuid);
  if (GuidHob != NULL) {
    DEBUG ((DEBUG_ERROR, "Wrapping Key is available. \n"));
    BiosSeedContent = (GET_BIOS_SEED_RESPONSE *) GET_GUID_HOB_DATA (GuidHob);
    CopyMem (WrappingKey, (VOID *)BiosSeedContent->BiosSeed, AES256_KEY_BYTE_SIZE);
    ZeroMem (BiosSeedContent, sizeof (GET_BIOS_SEED_RESPONSE));
  } else {
    DEBUG ((DEBUG_ERROR, "Wrapping Key is not available. \n"));
    return EFI_UNSUPPORTED;
  }

  RpmbDataHeaderSize = sizeof (NVME_PRIVATE_DATA_IN_RPMB);
  RpmbDataHeader = AllocateZeroPool (RpmbDataHeaderSize);
  if (RpmbDataHeader == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  WrappedInfoHeaderSize = sizeof (WRAPPED_KEY_INFO_LIST);
  WrappedInfoHeader = AllocateZeroPool (WrappedInfoHeaderSize);
  if (WrappedInfoHeader == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  WrappedKeyListSize = sizeof (AES256_WRAPPED_KEY) * AES256_WRAPPED_KEY_MAX_COUNT;
  WrappedKeyList = AllocateZeroPool (WrappedKeyListSize);
  if (WrappedKeyList == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = NvmeAuthenticatedKeyDataProgramming (Ssp, MediaId, mRpmbKey256, RPMB_WRAPPED_KEY_SIZE);
  if (EFI_ERROR (Status)) {
    //
    // ***** Could be already programmed before *****
    //
    DEBUG ((DEBUG_ERROR,"RPMB Key Programming is failed, Status[%x]. \n", Status));

    Status = GetRpmbData (
               Ssp,
               MediaId,
               RpmbDataHeader,
               RpmbDataHeaderSize,
               WrappedInfoHeader,
               WrappedInfoHeaderSize,
               WrappedKeyList,
               WrappedKeyListSize);
    if (EFI_ERROR (Status)) {
      goto Exit;
    }
    Status = GetRpmbKey(WrappingKey, WrappedKeyList);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,"Decrypt RPMB key Status[%x]. \n", Status));
      goto Exit;
    }
    //
    // Check Key migration request, update RPMB data
    //
    if (mKeyMigrationAction == NVME_RPMB_KEY_MIGRATION_VALUE_SET_PIN_KEY) {
      Status = SetPinWrappedKey (RpmbDataHeader, WrappedInfoHeader, WrappedKeyList);
      if (!EFI_ERROR (Status)) {
        Status = SetRpmbData (Ssp, MediaId, RpmbDataHeader, WrappedInfoHeader, WrappedKeyList, AccessSize);
      }
    } else if (mKeyMigrationAction == NVME_RPMB_KEY_MIGRATION_VALUE_UNLOCK_BY_PIN_KEY) {
      Status = SetWrappedKeybyKeyTag (SEED_WRAPPED, WrappingKey, WrappedKeyList);
      if (!EFI_ERROR (Status)) {
        Status = SetRpmbData (Ssp, MediaId, RpmbDataHeader, WrappedInfoHeader, WrappedKeyList, AccessSize);
      }
    }

  } else {
    DEBUG ((DEBUG_INFO,"RPMB Key Programming : Successful! \n"));
    Status = InitRpmbData (Ssp, MediaId, WrappingKey, AccessSize);
    goto Exit;
  }

Exit:

  ZeroMem (WrappingKey, AES256_KEY_BYTE_SIZE);

  if (RpmbDataHeader != NULL) {
    ZeroMem (RpmbDataHeader, RpmbDataHeaderSize);
    FreePool (RpmbDataHeader);
  }
  if (WrappedInfoHeader != NULL) {
    ZeroMem (WrappedInfoHeader, WrappedInfoHeaderSize);
    FreePool (WrappedInfoHeader);
  }
  if (WrappedKeyList != NULL) {
    ZeroMem (WrappedKeyList, WrappedKeyListSize);
    FreePool (WrappedKeyList);
  }
  return Status;
}

/**
  Get Bios offset/size by platform layout, it depends on the content should
  be stored in NVMe boot partition.

  @param  BiosSourceBuffer        A pointer return as bios buffer address
  @param  BiosSourceSize          A pointer return as bios buffer size.

  @retval EFI_SUCCESS      Successfully get the identify controller data.
  @retval EFI_DEVICE_ERROR Fail to get the identify controller data.

**/
EFI_STATUS
EFIAPI
GetBiosImageInfo (
  OUT VOID        **BiosSourceBuffer,
  OUT UINT32      *BiosSourceSize,
  OUT UINT32      *BiosAddress
  )
{
  EFI_STATUS               Status;
  *BiosSourceSize   = FixedPcdGet32 (PcdBiosSize);

  Status = mSpiProtocol->GetRegionAddress (mSpiProtocol, FlashRegionBios, BiosAddress, BiosSourceSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "The Bios region can not be read1, status == [%x] \n", Status));
    return Status;
  }

  *BiosSourceBuffer = AllocateCopyPool (
                        *BiosSourceSize,
                        (VOID *) (UINTN) FixedPcdGet32 (PcdBiosAreaBaseAddress)
                        );

  return EFI_SUCCESS;
}

/**
  Parse and print the Csme backup image header

  @param[in]  CsmeInputBuffer          A pointer to Csme backup image

**/
VOID
ParseCsmeImage(
  IN UINT8 *CsmeInputBuffer
  )
{
  NVME_IMAGE_STRUCT    *CsmeHeaderImage;
  NVME_RESTORE_ELEMENT *CsmeHeaderElement;
  UINT8                *TempCsmeParse;
  UINTN                Index = 0;

  TempCsmeParse = (UINT8 *) CsmeInputBuffer;
  CsmeHeaderImage = (NVME_IMAGE_STRUCT *)TempCsmeParse;
  DEBUG ((DEBUG_INFO, "<Csme Recovery Image Parse> CsmeHeaderImage->Type: 0x%lx.\n", CsmeHeaderImage->Type));
  DEBUG ((DEBUG_INFO, "<Csme Recovery Image Parse> CsmeHeaderImage->StructVersion: 0x%lx.\n", CsmeHeaderImage->StructVersion));
  DEBUG ((DEBUG_INFO, "<Csme Recovery Image Parse> CsmeHeaderImage->IpType: 0x%lx.\n", CsmeHeaderImage->IpType));
  DEBUG ((DEBUG_INFO, "<Csme Recovery Image Parse> CsmeHeaderImage->TotalSize: 0x%lx.\n", CsmeHeaderImage->TotalSize));
  DEBUG ((DEBUG_INFO, "<Csme Recovery Image Parse> CsmeHeaderImage->NumberOfElements: 0x%lx.\n", CsmeHeaderImage->NumberOfElements));
  DEBUG ((DEBUG_INFO, "<Csme Recovery Image Parse> CsmeHeaderImage->Reserved: 0x%lx.\n\n", CsmeHeaderImage->Reserved));

  TempCsmeParse = TempCsmeParse + sizeof (NVME_IMAGE_STRUCT) - sizeof (NVME_RESTORE_ELEMENT);

  for (Index = 0; Index < CsmeHeaderImage->NumberOfElements; Index++) {
    CsmeHeaderElement = (NVME_RESTORE_ELEMENT *) TempCsmeParse;
    DEBUG ((DEBUG_INFO, "<Csme Recovery Image Parse> Csme Element Number: %d.\n", Index));
    DEBUG ((DEBUG_INFO, "<Csme Recovery Image Parse> CsmeHeaderElement->RRestoreAction: 0x%lx.\n", CsmeHeaderElement->RestoreAction));
    DEBUG ((DEBUG_INFO, "<Csme Recovery Image Parse> CsmeHeaderElement->ElementType: 0x%lx.\n", CsmeHeaderElement->ElementType));
    DEBUG ((DEBUG_INFO, "<Csme Recovery Image Parse> CsmeHeaderElement->OffsetRecvImage: 0x%lx.\n", CsmeHeaderElement->OffsetRecvImage));
    DEBUG ((DEBUG_INFO, "<Csme Recovery Image Parse> CsmeHeaderElement->OffsetSpi: 0x%lx.\n", CsmeHeaderElement->OffsetSpi));
    DEBUG ((DEBUG_INFO, "<Csme Recovery Image Parse> CsmeHeaderElement->RElementSize: 0x%lx.\n\n", CsmeHeaderElement->ElementSize));
    TempCsmeParse = TempCsmeParse + sizeof (NVME_RESTORE_ELEMENT);
  }
}

/**
  Create a firmware descriptor which records the layout of every firmware components,
  EC will execute restore operation by detailed address mapping between SPI and NVME
  Boot Partition. Following up headers, the images of different IPs will be connected
  one by one.

  @param  RestoreImagebuffer            A pointer the buffer which contains the data need to
                                        backup to NVMe Boot Partition.
  @param  RestoreImagebufferLength      A pointer to buffer size

  @retval EFI_SUCCESS                   Successfully create descriptor before image backup.
  @retval RETURN_INVALID_PARAMETER      Fail to get the identify controller data.

**/
EFI_STATUS
EFIAPI
ConstructNvmeRecoveryDescriptorAndData (
  IN OUT UINT8  *RestoreImagebuffer,
  IN OUT UINTN  *RestoreImagebufferLength
  )
{
  EFI_STATUS                   Status;
  NVME_IMAGE_HEADER            NvmeHeader;
  NVME_IMAGE_STRUCT            RestoreStruct;
  UINT8                        *TempBuffHeader;
  VOID                         *TempBufferCsme;
  UINT32                       BPStoreLengh;
  VOID                         *BiosSourceBuffer;
  UINT32                       BiosSourceSize;
  UINT32                       BiosAddress;
  UINT32                       CsmeAddress;
  UINT32                       CsmeSize;
  UINT32                       ImageLength;
  UINTN                        CsmeRestoreSize;
  NVME_IMAGE_HEADER            *TempNvmeHeader;
  UINTN                        PciIoHandleCount;
  EFI_HANDLE                   *PciIoHandleBuffer;
  EFI_PCI_IO_PROTOCOL          *PciIo;
  UINTN                        Bus;
  UINTN                        Device;
  UINTN                        Function;
  UINTN                        Index;
  UINTN                        Segment;
  VOID                         *Mapping;
  EFI_PHYSICAL_ADDRESS         CsmeRestoreImage;
  BOOLEAN                      IsHeci1Found;
  UINT64                       Supports;
  ME_FIRMWARE_UPDATE_PROTOCOL  *MeFwu;
  UINT8                        Heci1BusNum;
  UINT8                        Heci1FuncNum;
  UINT8                        Heci1DeviceNum;
  UINT64                       HeciBaseAddress;

  TempBuffHeader        = RestoreImagebuffer;
  BPStoreLengh          = 0;
  BiosSourceBuffer      = NULL;
  ImageLength           = 0;
  TempNvmeHeader        = NULL;
  Mapping               = NULL;
  IsHeci1Found          = FALSE;
  TempBufferCsme        = NULL;
  CsmeRestoreSize       = 0;
  Supports              = 0;
  CsmeSize              = 0;
  PciIoHandleBuffer     = NULL;
  HeciBaseAddress       = MeHeciPciCfgBase (HECI1);
  Heci1FuncNum          = (HeciBaseAddress >> 12) & 0x07;
  Heci1DeviceNum        = (HeciBaseAddress >> 15) & 0x1F;
  Heci1BusNum           = (HeciBaseAddress >> 20) & 0xFF;
  DEBUG ((DEBUG_INFO, "Heci1FuncNum = [%x], Heci1DeviceNum = [%x], Heci1BusNum = [%x].\n", Heci1FuncNum, Heci1DeviceNum, Heci1BusNum));

  Status = InitializeSpiProtocolInterface ();
  if (EFI_ERROR (Status)) {
    return EFI_NOT_READY;
  }

  Status = GetMeFwuProtocol (&MeFwu);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Read Bios region
  //
  Status = GetBiosImageInfo (&BiosSourceBuffer, &BiosSourceSize, &BiosAddress);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "NVMe Descriptor: can't get bios info with Status. [%x].\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "<RecoveryDescriptor>, BiosSourceBuffer = [%x], BiosSourceSize = [%x], BiosAddress = [%x].\n", BiosSourceBuffer, BiosSourceSize, BiosAddress));

  Status = mSpiProtocol->GetRegionAddress (mSpiProtocol, FlashRegionMe, &CsmeAddress, &CsmeSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "The CSME region can not be read1, status = [%x] \n", Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "<RecoveryDescriptor>, CsmeSize = [%x], CsmeAddress = [%x].\n", CsmeSize, CsmeAddress));

  //
  //   The layout of restore image
  //
  //   |--------------------------------------|
  //   | Main header                          |
  //   |--------------------------------------|
  //   | Bios restore image header            |
  //   |--------------------------------------|
  //   | Bios restore element header          |
  //   |--------------------------------------|
  //   | Bios payload                         |
  //   |--------------------------------------|
  //   | Csme restore image header            |
  //   |--------------------------------------|
  //   | Csme restore element header(multi)   |
  //   |--------------------------------------|
  //   | Csme payload                         |
  //   |--------------------------------------|
  //
  ZeroMem (&NvmeHeader, sizeof (NVME_IMAGE_HEADER));
  ZeroMem (&RestoreStruct, sizeof (NVME_IMAGE_STRUCT));

  //
  // restore image Main header
  //
  NvmeHeader.NvmeImageHeaderStruct.Type             = NVME_RECOVERY_DESCRIPTOR_HEADER_SIGNATURE;
  NvmeHeader.NvmeImageHeaderStruct.StructVersion    = 1;
  NvmeHeader.NvmeImageHeaderStruct.IpType           = RECV_HEAD;                            // 0x00 means the header
  NvmeHeader.NvmeImageHeaderStruct.TotalSize        = 0;                                    // Wait for all region filled
  NvmeHeader.NvmeImageHeaderStruct.NumberOfElements = 2;                                    // includes Bios and CSME(code/data)
  NvmeHeader.HashType                               = RECV_SHA384;                          // SHA384
  NvmeHeader.NvmeImageHeaderStruct.Reserved         = CsmeAddress;
  CopyMem (TempBuffHeader, &NvmeHeader, sizeof (NVME_IMAGE_HEADER));
  TempBuffHeader = TempBuffHeader + sizeof (NVME_IMAGE_HEADER);
  BPStoreLengh   = BPStoreLengh + sizeof (NVME_IMAGE_HEADER);

  //
  // Bios restore image and element header
  //
  RestoreStruct.Type             = NVME_RECOVERY_DESCRIPTOR_ELEMENT_SIGNATURE;        // recv
  RestoreStruct.StructVersion    = 1;                                                 // Init version
  RestoreStruct.IpType           = RECV_BIOS;                                         // bios = 1, there will be an enum list in future.
  RestoreStruct.TotalSize        = BiosSourceSize + sizeof (NVME_IMAGE_STRUCT);
  RestoreStruct.NumberOfElements = 1;
  RestoreStruct.Reserved         = 0;

  RestoreStruct.RestoreElement.RestoreAction    = RESTORE_ELEMENT_ACTION_COPY;
  RestoreStruct.RestoreElement.ElementType      = 0;
  RestoreStruct.RestoreElement.OffsetRecvImage  = sizeof (NVME_IMAGE_STRUCT);
  RestoreStruct.RestoreElement.OffsetSpi        = BiosAddress;             // read bios offset in spi
  RestoreStruct.RestoreElement.ElementSize      = BiosSourceSize;
  CopyMem (TempBuffHeader, &RestoreStruct, sizeof (NVME_IMAGE_STRUCT));
  TempBuffHeader = TempBuffHeader + sizeof (NVME_IMAGE_STRUCT);
  BPStoreLengh   = BPStoreLengh + sizeof (NVME_IMAGE_STRUCT);

  //
  // Bios payload
  //
  CopyMem (TempBuffHeader, (UINT8 *)BiosSourceBuffer, BiosSourceSize);
  TempBuffHeader = TempBuffHeader + BiosSourceSize;
  BPStoreLengh   = BPStoreLengh + BiosSourceSize;

  //
  // Find HECI1 device PCIIO for map and unmap DMA buffer
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciIoHandleCount,
                  &PciIoHandleBuffer
                  );
  if (!EFI_ERROR (Status) &&
      (PciIoHandleBuffer != NULL) &&
      (PciIoHandleCount != 0)) {
    for (Index = 0; Index < PciIoHandleCount; Index++) {
      Status = gBS->HandleProtocol (
                      PciIoHandleBuffer[Index],
                      &gEfiPciIoProtocolGuid,
                      (VOID **) &PciIo
                      );
      ASSERT_EFI_ERROR (Status);
      PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
      if (Bus == Heci1BusNum && Device == Heci1DeviceNum && Function == Heci1FuncNum) {
        //
        // Find HECI1 device PCIIO, break.
        //
        IsHeci1Found = TRUE;
        break;
      }
    }

    if (IsHeci1Found) {
      Status = PciIo->Attributes (
                        PciIo,
                        EfiPciIoAttributeOperationSupported,
                        0,
                        &Supports
                        );

      if (!EFI_ERROR (Status)) {
        Supports &= (UINT64)EFI_PCI_DEVICE_ENABLE;
        Status = PciIo->Attributes (
                          PciIo,
                          EfiPciIoAttributeOperationEnable,
                          Supports,
                          NULL
                          );
        DEBUG ((DEBUG_INFO, "Program HECI PCI command register with value %x, (%r)\n", Supports, Status));
      }

      //
      // allocate 16m buffer for CSME restore image and aligned with 4K
      //
      Status = PciIo->AllocateBuffer (
                        PciIo,
                        AllocateAnyPages,
                        EfiBootServicesData,
                        EFI_SIZE_TO_PAGES ((UINTN) NVME_IFWI_IMAGE_SIZE / 2),
                        &TempBufferCsme,
                        0
                        );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "no memory for csme restore image\n"));
        goto Exit;
      }
      ZeroMem (TempBufferCsme, (UINTN)NVME_IFWI_IMAGE_SIZE / 2);
      CsmeRestoreSize = NVME_IFWI_IMAGE_SIZE / 2;
      Status = PciIo->Map (
                        PciIo,
                        EfiPciIoOperationBusMasterCommonBuffer,
                        TempBufferCsme,
                        &CsmeRestoreSize,
                        &CsmeRestoreImage,
                        &Mapping
                        );
      //
      // call CSME FWU API to get CSME restore image
      //
      DEBUG ((DEBUG_INFO, "allocated memory is 4k aligned? %016llX %016llX\n", (UINT64)TempBufferCsme, (UINT64) CsmeRestoreImage));
      CsmeRestoreImage = ALIGN_VALUE (CsmeRestoreImage, EFI_PAGE_SIZE);

      DEBUG ((DEBUG_INFO, "input memory to csme need 4k aligned phy: %llx %08X\n", (UINT64) CsmeRestoreImage, (UINT32)CsmeRestoreSize));
      Status = MeFwu->GetRecoveryImageToDmaBuffer (CsmeRestoreImage, (UINT32)CsmeRestoreSize, &ImageLength);
      if (Status != EFI_SUCCESS) {
        DEBUG ((DEBUG_ERROR, "NVMe Descriptor: can't get the recovery image from the FW with Status. [%x].\n", Status));
        Status = EFI_DEVICE_ERROR;
        goto Exit;
      }
      DEBUG ((DEBUG_INFO, "<RecoveryDescriptor> ImageLength = [%x].\n", ImageLength));

      //
      // Call Flush() to flush all write transactions to system memory
      //
      Status = PciIo->Flush (PciIo);

      //
      // Parse and print Csme backup image
      //
      ParseCsmeImage ((UINT8 *)CsmeRestoreImage);

      //
      // copy back from phy memory to image buffer
      //
      CopyMem (TempBuffHeader, (UINT8 *)CsmeRestoreImage, ImageLength);

      BPStoreLengh = BPStoreLengh + ImageLength;
      *RestoreImagebufferLength = BPStoreLengh;
      //
      // fill total size in image header since csme image size was get just now.
      //
      TempNvmeHeader = (NVME_IMAGE_HEADER *)RestoreImagebuffer;
      TempNvmeHeader->NvmeImageHeaderStruct.TotalSize = BPStoreLengh;    // fill the TotalSize in the structure of main header
    }
  }

Exit:
  if (BiosSourceBuffer != NULL) {
    FreePool (BiosSourceBuffer);
  }
  if (PciIoHandleBuffer != NULL) {
    FreePool (PciIoHandleBuffer);
  }
  if (Mapping != NULL) {
    PciIo->Unmap (PciIo, Mapping);
  }
  if (IsHeci1Found) {
    PciIo->FreeBuffer (
           PciIo,
           EFI_SIZE_TO_PAGES ((UINTN) NVME_IFWI_IMAGE_SIZE/2),
           TempBufferCsme
           );
  }

  return Status;
}

/**
  Implemented a series of Replay Protected Memory Block(RPMB) command to configure
  Device Configuration Block(DCB) value.

  @param  Passthru          A pointer to EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
  @param  NamespaceId       Current device name space ID.
  @param  Ssp               A pointer to EFI_STORAGE_SECURITY_COMMAND_PROTOCOL.
  @param  MediaId           The media ID that the write request is for.
  @param  WriteLength       Write boot partition size of per chunk.

  @retval EFI_SUCCESS           Successfully get the identify controller data.
  @retval EFI_DEVICE_ERROR      Fail to get the identify controller data.
  @retval EFI_OUT_OF_RESOURCES  There is not enough memory resource.

**/
EFI_STATUS
EFIAPI
ImageBackUpSpi2Nvme (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL     *Passthru,
  IN UINT32                                 NamespaceId,
  IN EFI_STORAGE_SECURITY_COMMAND_PROTOCOL  *Ssp,
  IN UINT32                                 MediaId,
  IN UINTN                                  WriteLength
  )
{
  EFI_STATUS  Status;
  UINT8       DCBDStru[sizeof (NVME_RPMB_DCB)];
  UINT8       *ReadReceiveBuffer;
  UINTN       NvmeRecBPSize;
  UINT8       *TempNvmeRecBPBuffer;
  UINTN       TempNvmeRecBPSize;
  UINT8       *NvmeRecBPBuffer;
  VOID        *HostMemoryBuffer;
  UINT8       *HashShaDigest;

  ReadReceiveBuffer   = NULL;
  TempNvmeRecBPBuffer = NULL;
  NvmeRecBPBuffer     = NULL;
  HostMemoryBuffer    = NULL;
  HashShaDigest       = NULL;
  NvmeRecBPSize       = 0;
  Status              = EFI_SUCCESS;

  //
  // Allocate a physically contiguous memory buffer in the host to store the contents of a Boot Partition.
  // 32MB is enough for recovery image and its header
  //
  TempNvmeRecBPSize   = NVME_IFWI_IMAGE_SIZE;
  TempNvmeRecBPBuffer = AllocateZeroPool (TempNvmeRecBPSize);
  if (TempNvmeRecBPBuffer == NULL) {
    DEBUG ((DEBUG_ERROR,"Warning: Run out memory. \n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  Status = ConstructNvmeRecoveryDescriptorAndData (TempNvmeRecBPBuffer, &TempNvmeRecBPSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Create NvmeRecovery image Fail [%r] \n", Status));
    goto Exit;
  }

  NvmeRecBPSize = TempNvmeRecBPSize % WriteLength == 0 ?
                  TempNvmeRecBPSize :
                  (TempNvmeRecBPSize / WriteLength + 1) * WriteLength ;
  NvmeRecBPBuffer = AllocateZeroPool (NvmeRecBPSize);
  if (NvmeRecBPBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  SetMem (NvmeRecBPBuffer, NvmeRecBPSize, 0xFF);
  CopyMem (NvmeRecBPBuffer, TempNvmeRecBPBuffer, TempNvmeRecBPSize);
  HashShaDigest = AllocateZeroPool (BACKUP_FILE_HASH_DIGEST_SIZE);
  if (HashShaDigest == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = CreateHashShaDigest (
             NvmeRecBPBuffer,
             TempNvmeRecBPSize,
             &HashShaDigest,
             BACKUP_FILE_HASH_DIGEST_SIZE
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to create digest, Status[%x]. \n", Status));
    goto Exit;
  }
  CopyMem (((NVME_IMAGE_HEADER *)NvmeRecBPBuffer)->Hash, (VOID *)HashShaDigest, SHA384_DIGEST_SIZE);

  ZeroMem (DCBDStru, NVME_RPMB_CONTENTS_SIZE);
  ReadReceiveBuffer = AllocateZeroPool (NVME_RPMB_DATA_TOTAL_SIZE);
  if (ReadReceiveBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = NvmeAuthenticatedDeviceConfigurationBlockRead (Ssp, MediaId, ReadReceiveBuffer, NVME_RPMB_DATA_FRAME_SIZE + sizeof (NVME_RPMB_DCB));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Warning: RPMB Configuration Block Read: something wrong happened, Status[%x]. \n", Status));
    goto Exit;
  } else {
    CopyMem (DCBDStru, ReadReceiveBuffer + NVME_RPMB_DATA_FRAME_SIZE, NVME_RPMB_CONTENTS_SIZE);
    DEBUG ((DEBUG_INFO, "DCBDS[BPPEnable]=[%x], DCBDS[BPLock]=[%x], DCBDS[NWPAuthCtrl]=[%x].\n", DCBDStru[BPPEnable], DCBDStru[BPLock], DCBDStru[NWPAuthCtrl]));
  }

  if ((DCBDStru[BPLock] & BIT0) != 0x00) { // so far, only partition 0 support image backup, BP2 will be ignored.
    DEBUG ((DEBUG_INFO,"  Boot Partition 0 is locked. \n"));

    DCBDStru[BPLock] = 0;
    Status = NvmeAuthenticatedDeviceConfigurationBlockWrite (Ssp, MediaId, DCBDStru, NVME_RPMB_CONTENTS_SIZE);   // unlock partition 0
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,"Boot Partition unlocking is failed, Status[%x]. \n", Status));
      goto Exit;
    } else {
      DEBUG ((DEBUG_INFO,"RPMB BP unlock is Successful! \n"));
    }

    Status = NvmeAuthenticatedDeviceConfigurationBlockRead (Ssp, MediaId, ReadReceiveBuffer, NVME_RPMB_DATA_FRAME_SIZE + sizeof (NVME_RPMB_DCB));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR," Warning: RPMB Configuration Block Read: something wrong happened, Status[%x]. \n", Status));
      goto Exit;
    } else {
      CopyMem (DCBDStru, ReadReceiveBuffer + NVME_RPMB_DATA_FRAME_SIZE, sizeof (NVME_RPMB_DCB));
      DEBUG ((DEBUG_INFO,"DCBDS[BPPEnable]=[%x], DCBDS[BPLock]=[%x], DCBDS[NWPAuthCtrl]=[%x].\n", DCBDStru[BPPEnable], DCBDStru[BPLock], DCBDStru[NWPAuthCtrl]));
    }

    Status = NvmeBootPartitionWrite (Passthru, NamespaceId, NvmeRecBPBuffer, NvmeRecBPSize, WriteLength);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,"DCBD write is failed with status:[%x].\n", Status));
      goto Exit;
    }
  } else {
    DEBUG ((DEBUG_INFO,"Boot Partition 0 is not locked.\n"));
    Status = NvmeBootPartitionWrite (Passthru, NamespaceId, NvmeRecBPBuffer, NvmeRecBPSize, WriteLength);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,"DCBD write is failed with status:[%x].\n", Status));
      goto Exit;
    }
  }
  //
  //******************************************Lock Partition********************************************************//
  //
  DCBDStru[BPPEnable] = 1;
  DCBDStru[BPLock]    = 1;
  Status = NvmeAuthenticatedDeviceConfigurationBlockWrite (Ssp, MediaId, DCBDStru, NVME_RPMB_CONTENTS_SIZE);   // lock partition 0
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,"  Boot Partition locking is failed, Status[%x]. \n", Status));
    goto Exit;
  } else {
    DEBUG ((DEBUG_INFO," RPMB BP lock is Successful. \n"));
  }

  Status = NvmeAuthenticatedDeviceConfigurationBlockRead (Ssp, MediaId, ReadReceiveBuffer, NVME_RPMB_DATA_FRAME_SIZE + sizeof (NVME_RPMB_DCB));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR," Warning: RPMB Configuration Block Read: something wrong happened, Status[%x]. \n", Status));
    goto Exit;
  } else {
    CopyMem (DCBDStru, ReadReceiveBuffer + NVME_RPMB_DATA_FRAME_SIZE, sizeof (NVME_RPMB_DCB));
    DEBUG ((DEBUG_INFO,"DCBDS[BPPEnable]=[%x], DCBDS[BPLock]=[%x], DCBDS[NWPAuthCtrl]=[%x].\n", DCBDStru[BPPEnable], DCBDStru[BPLock], DCBDStru[NWPAuthCtrl]));
  }

  //
  // Read back and do integrity check
  //
  Status = NvmeBootPartitionGetLogPageRead (Passthru, NamespaceId, &HostMemoryBuffer, NvmeRecBPSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "  Boot Partition Read is failed, Status[%x]. \n", Status));
  }

  if (CompareMem (HostMemoryBuffer, NvmeRecBPBuffer, NvmeRecBPSize) == 0) {
    DEBUG ((DEBUG_INFO, "  SPI2NVMe Backup Succeeded! \n"));
  } else {
    Status = EFI_CRC_ERROR;
    DEBUG ((DEBUG_ERROR, " Host memory and NVMe boot partion backup content are not align.\n"));
  }

Exit:
  if (TempNvmeRecBPBuffer != NULL) {
    FreePool(TempNvmeRecBPBuffer);
  }

  if (HashShaDigest != NULL) {
    FreePool (HashShaDigest);
  }

  if (HostMemoryBuffer != NULL) {
    FreePool (HostMemoryBuffer);
  }

  if (NvmeRecBPBuffer != NULL) {
    FreePool (NvmeRecBPBuffer);
  }

  if (ReadReceiveBuffer != NULL) {
    FreePool (ReadReceiveBuffer);
  }

  return Status;
}

/**
  Check current device is an VMD device or not.

  @param  PciIoProtocol        A pointer to pciio protocol

  @retval TRUE      vmd enabled on this device.
  @retval FALSE     vmd disabled on this device

**/

BOOLEAN
EFIAPI
IsVmdDevice(
  IN EFI_PCI_IO_PROTOCOL       *PciIoProtocol
  )
{
  BOOLEAN              IsVmd = FALSE;
  UINTN                Segment;
  UINTN                Bus;
  UINTN                Device;
  UINTN                Function;

  if (PciIoProtocol == NULL) {
    return IsVmd;
  }

  //
  // Now further check the PCI header
  //
  PciIoProtocol->GetLocation (PciIoProtocol, &Segment, &Bus, &Device, &Function);
  DEBUG ((DEBUG_INFO, "Bus Device and Function are: %x %x %x. \n", Bus, Device, Function));
  if ((GetVmdBusNumber () == Bus) && (GetVmdDevNumber () == Device) && (GetVmdFuncNumber () == Function)) {
    IsVmd = TRUE;
  }
  else
    IsVmd = FALSE;

  return IsVmd;
}

/**
  get current device's namespace ID

  @param  DevicePath        Current device's path

  @retval NamespaceId       Current device's namespace ID

**/

UINT32
EFIAPI
GetNamespaceId (
  IN  EFI_DEVICE_PATH_PROTOCOL                    *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL         *TempDevicePath;
  NVME_NAMESPACE_DEVICE_PATH       *Node;

  TempDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) DevicePath;
  while (!IsDevicePathEndType (TempDevicePath)) {
    if (DevicePathType (TempDevicePath) == MESSAGING_DEVICE_PATH &&
          DevicePathSubType (TempDevicePath) == MSG_NVME_NAMESPACE_DP) {
      Node  = (NVME_NAMESPACE_DEVICE_PATH *)TempDevicePath;
      return Node->NamespaceId;
    }
    TempDevicePath = NextDevicePathNode (TempDevicePath);
  }
  return NVME_ALL_NAMESPACEID;
}

/**
  get current device's protocol's interface

  @param  DevicePath        Current device's path
  @param  Protocol          Current protocol GUID

  @retval Interface       Current protocol GUID's content return as a pointer

**/

EFI_STATUS
EFIAPI
GetProtocolInterface (
  IN EFI_DEVICE_PATH_PROTOCOL      *DevicePath,
  IN EFI_GUID                      *Protocol,
  OUT VOID                         **Interface
  )
{
  EFI_STATUS                       Status;
  EFI_HANDLE                       Handle;
  EFI_DEVICE_PATH_PROTOCOL         *DevicePathTemp;

  if((DevicePath == NULL) || (Protocol == NULL) || (Interface == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  DevicePathTemp = DevicePath;
  Status = gBS->LocateDevicePath (
                  Protocol,
                  &DevicePathTemp,
                  &Handle
                  );
  if (!EFI_ERROR (Status)) {
    Status = gBS->HandleProtocol (
                  Handle,
                  Protocol,
                  (VOID**)Interface
                  );
    if(EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: HandleProtocol for interface is %r. \n", __FUNCTION__, Status));
      return EFI_NOT_FOUND;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a: LocateDevicePath for handle is %r. \n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}

/**
  read current device boot partition capability and information

  @param  CfgSpaceAddress        Current device's config space address.
  @param  PciIo                  Current device's pciio protocol pointer.
  @param  IsVmd                  VMD enabled flag.
  @param  NamespaceId            Current device's namespace ID

  @retval TRUE                   boot partition is supported on this device.
  @retval FALSE                  boot partition is not supported on this device.

**/

BOOLEAN
EFIAPI
ReadBpCapability (
  EFI_PHYSICAL_ADDRESS        CfgSpaceAddress,
  EFI_PCI_IO_PROTOCOL         *PciIo,
  BOOLEAN                     IsVmd,
  UINT32                      NamespaceId
  )
{
  EFI_STATUS      Status;
  BOOLEAN         IsBpSupport;
  NVME_BPINFO     *BPINFO;
  UINTN           TempdataSize;
  UINTN           TempCap;
  NVME_CAP        *Cap;
  EFI_PHYSICAL_ADDRESS MemSpaceAddress;
  UINT32          TempInfo;
  UINT32          Bar0Address;
  UINT32          Bar1Address;

  IsBpSupport = FALSE;

  if(PciIo == NULL) {
    return IsBpSupport;
  }

  if(IsVmd) {
    //
    // get mem space bar address
    //
    Bar0Address = MmioRead32 (CfgSpaceAddress + CFG_SPACE_BAR0_OFFSET);
    Bar1Address = MmioRead32 (CfgSpaceAddress + CFG_SPACE_BAR1_OFFSET);

    Bar0Address = Bar0Address & 0xFFFFFFF0;
    MemSpaceAddress = Bar1Address;
    MemSpaceAddress = LShiftU64 (MemSpaceAddress, 32);
    MemSpaceAddress += Bar0Address;

    TempCap = MmioRead64 (MemSpaceAddress + NVME_CAP_OFFSET);
    TempInfo = MmioRead32 (MemSpaceAddress + NVME_BPINFO_OFFSET);
    DEBUG ((DEBUG_INFO, "%a: Bar0Address = 0x%x, Bar1Address =0x%x\n", __FUNCTION__, Bar0Address, Bar1Address));
    DEBUG ((DEBUG_INFO, "%a: BpInfo = 0x%x, Capability =0x%lx\n", __FUNCTION__, TempInfo, TempCap));
  } else {

    Status = ReadNvmeController64 (PciIo, &TempCap, NVME_CAP_OFFSET);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Warning: Read Offset 0h: CAP, failed with status[%x]. \n", Status));
      return IsBpSupport;
    }
    //
    // Check if it was actived or not
    //
    Status = ReadNvmeController32 (PciIo, &TempInfo, NVME_BPINFO_OFFSET);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Warning: Read Offset 40h: BPINFO, failed with status[%x]. \n", Status));
      return IsBpSupport;
    }
  }

  Cap = (NVME_CAP *)(&TempCap);
  BPINFO = (NVME_BPINFO *)(&TempInfo);
  //
  // Check if support Boot Partition
  //
  if (Cap->Bps != 0x01) {
     DEBUG ((DEBUG_INFO, "The NVMe Controller doesn't support boot Partition. \n"));
     return IsBpSupport;
  }
  //
  // check key capabilities -- Read Partition size
  //
  if (BPINFO->Bpsz != 0x00) {
    TempdataSize = (BPINFO->Bpsz * 128) * 1024;
    DEBUG ((DEBUG_INFO,"  The size of NVMe Boot Partition is [%d] KB. \n" , TempdataSize / 1024));
  }

  if (BPINFO->Abpid == 0x00) {
    DEBUG ((DEBUG_INFO,"  Warning: NVMe Boot Partition is not actived. \n"));
  }
  IsBpSupport = TRUE;
  DEBUG ((DEBUG_INFO, "%a: Is Boot partition Support: %d. \n", __FUNCTION__, IsBpSupport));
  return IsBpSupport;
}

/**
  Get the bridge information from device path.

  @param[IN]  DevicePath            The NVMe device path.
  @param[OUT] BridgeDev             The device id of the bridge.
  @param[OUT] BridgeFun             The function id of the bridge.

  @return EFI_SUCCESS      Successfully get the bridge information.
  @return EFI_NOT_FOUND    Fail to find the bridge information.

**/
EFI_STATUS
EFIAPI
NvmeRecoveryGetDFFromDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL         *DevicePath,
  OUT UINTN                            *BridgeDev,
  OUT UINTN                            *BridgeFun
  )
{
  EFI_DEVICE_PATH_PROTOCOL          *TempDevicePath;
  PCI_DEVICE_PATH                   *PciNode;

  if((DevicePath == NULL) || (BridgeDev == NULL) || (BridgeFun == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  TempDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) DevicePath;
  while (!IsDevicePathEndType (TempDevicePath)) {
    if (DevicePathType (TempDevicePath) == HARDWARE_DEVICE_PATH &&
          DevicePathSubType (TempDevicePath) == HW_PCI_DP) {
      PciNode = (PCI_DEVICE_PATH*)TempDevicePath;
      *BridgeDev = PciNode->Device;
      *BridgeFun = PciNode->Function;
      return EFI_SUCCESS;
    }
    TempDevicePath = NextDevicePathNode (TempDevicePath);
  }

  return EFI_NOT_FOUND;
}

/**
  check current device is on the right port or not.

  @param[IN]  IsVmd                 VMD enabled or not.
  @param[IN] DevicePath             Current device path.
  @param[OUT] NameSpace             Current device name space ID.
  @param[OUT] CfgSpaceAddress       config space address with VMD enabled case

  @return TRUE                      This is the supported slot for image write
  @return FALSE                     This is not an supported slot

**/
BOOLEAN
EFIAPI
IsSupportedRootportDevice (
  IN  BOOLEAN                                 IsVmd,
  IN  EFI_DEVICE_PATH_PROTOCOL                *DevicePath,
  OUT UINT32                                  *NameSpace,
  OUT EFI_PHYSICAL_ADDRESS                    *CfgSpaceAddress
  )
{
  EFI_STATUS                                Status;
  EFI_RST_CONFIG_SPACE_ACCESS_PROTOCOL      *RstCfgSpace;
  UINT32                                    PortId;
  UINT32                                    NamespaceId;
  UINTN                                     Bus;
  UINTN                                     Device;
  UINTN                                     RpDevice;
  UINTN                                     Function;
  UINTN                                     RpFunction;
  UINTN                                     PortNumber;
  UINTN                                     RefPortNumber;
  UINT64                                    PciBase;
  UINT8                                     CapHeaderOffset;


  RstCfgSpace         = NULL;
  Bus                 = 0;
  Device              = 0;
  Function            = 0;
  NamespaceId         = NVME_ALL_NAMESPACEID;

  RefPortNumber = PcdGet32 (PcdNvmeRecoveryPrimarySlotPortNumber);

  if (RefPortNumber == 0) {
    DEBUG ((DEBUG_ERROR, " not supported RVP board\n"));
    return FALSE;
  }
  //
  // call rst nvme passthrough protocol to get cfg space, namespace and BDF data
  //
  if (IsVmd) {
    DEBUG ((DEBUG_INFO, "%a: try to get rst config space protocol\n", __FUNCTION__));
    Status = GetProtocolInterface (
               DevicePath,
               &gRstConfigSpaceAccessProtocolGuid,
               (VOID**)&RstCfgSpace
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: can't get RST config space protocol %r. \n", __FUNCTION__, Status));
      RstCfgSpace = NULL;
      return FALSE;
    }
    //
    // get namespaceID for current device with VMD enabled.
    //
    NamespaceId = GetNamespaceId (DevicePath);
    DEBUG ((DEBUG_INFO, "%a: NamespaceId is 0x%x. \n", __FUNCTION__, NamespaceId));
    NamespaceId = NamespaceId & 0x7FFFFFFF;
    PortId = 0;
    while (PortId < 31) {
      if ((NamespaceId & (1 << PortId)) != 0) {
        break;
      }
      PortId++;
    }
    NamespaceId = PortId + 1;
    //
    // get current device BDF for further clarify which device is righe one for backup image
    //
    Status = RstCfgSpace->GetBDF (RstCfgSpace, NamespaceId, &Bus, &Device, &Function);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "unable to get BDF from RST = %r.\n", Status));
    }
    DEBUG ((DEBUG_INFO, "VMD enabled Bus Device and Function are: %x %x %x. \n", Bus, Device, Function));
    //
    // call RST api to check Caps and BPINFO
    //
    Status = RstCfgSpace->GetAddress (RstCfgSpace, NamespaceId, CfgSpaceAddress);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "unable to get config space address from RST = %r.\n", Status));
    }
    //
    // port number started from index 0, so -1 here
    //
    GetPchPcieRpDevFun ((RefPortNumber - 1), &RpDevice, &RpFunction);
    *NameSpace = NamespaceId;
    DEBUG ((DEBUG_INFO, "VMD enabled calculated Bus Device and Function are: %x %x %x. \n", Bus, RpDevice, RpFunction));
    if ((Device != RpDevice) || (Function != RpFunction))
      return FALSE;
  } else {
    *NameSpace = NVME_CONTROLLER_ID;
    //
    // Get BDF from device path
    //
    Status = NvmeRecoveryGetDFFromDevicePath (DevicePath, &Device, &Function);
    PciBase = PCI_SEGMENT_LIB_ADDRESS (0, 0, Device, Function, 0);
    CapHeaderOffset = PcieFindCapId (0, 0, (UINT8)Device, (UINT8)Function, EFI_PCI_CAPABILITY_ID_PCIEXP);
    PortNumber = PciSegmentRead8 ((UINTN)PciBase + CapHeaderOffset + 0x0F);
    DEBUG ((DEBUG_INFO, "calculated d f port are: %d %d %d %d %lx\n", Device, Function, PortNumber, CapHeaderOffset, PciBase));
    if (PortNumber != RefPortNumber)
      return FALSE;
  }
  return TRUE;
}

/**

  It determines if current boot process is normal, then healthy image will
  be saved to NVMe Boot Partition.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
NvmeRecoveryCallBack (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                              Status;
  UINTN                                   HandleCount;
  EFI_HANDLE                              *HandleBuffer;
  UINTN                                   Index;
  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp;
  EFI_DEVICE_PATH_PROTOCOL                *DevicePath;
  EFI_BLOCK_IO_PROTOCOL                   *BlockIo;
  UINT32                                  MediaId;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS         *PreviousUpdateProgress;
  BOOLEAN                                 IsVmd;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL      *NvmeDevice;
  NVME_ADMIN_CONTROLLER_DATA              *ControllerData;
  EFI_PCI_IO_PROTOCOL                     *PciIo;
  UINT32                                  NamespaceId;
  BOOLEAN                                 BpSupport;
  BOOLEAN                                 IsPortSupported;
  EFI_PHYSICAL_ADDRESS                    CfgSpaceAddress;
  NVME_BASED_RECOVERY_HII_CONFIG          *RpmbHiiConfiguration;
  UINTN                                   RpmbHiiConfigurationSize;
  UINT32                                  RpmbHiiVarAttr;
  UINTN                                   WriteLength;
  NVME_BACKUP_INFO                        NvmeBackupInfo;
  BOOLEAN                                 IsNvmeBackupVarUpdate;
  BOOLEAN                                 IsFirstBoot;

  Ssp                 = NULL;
  Status              = EFI_SUCCESS;
  MediaId             = 0;
  NamespaceId         = NVME_ALL_NAMESPACEID;
  IsVmd               = FALSE;
  IsPortSupported     = FALSE;

  RpmbHiiConfiguration     = NULL;
  RpmbHiiVarAttr           = 0;
  RpmbHiiConfigurationSize = 0;
  HandleCount              = 0;
  HandleBuffer             = NULL;
  PreviousUpdateProgress   = NULL;
  IsNvmeBackupVarUpdate    = FALSE;
  IsFirstBoot              = FALSE;
  ZeroMem (&NvmeBackupInfo, sizeof (NVME_BACKUP_INFO));

  //
  // Disarm EC WDT first, since from RPL EC, WDT is armed as default, need disarm at the beginning.
  //
  Status = DisarmPlatformWdt ();
  DEBUG ((DEBUG_INFO, "DisarmPlatformWdt: %r\n", Status));

  Status = GetVariable3 (NVME_BASED_RECOVERY_VARIABLE_NAME, &gNvmeBasedRecoveryHiiGuid, (VOID **) &RpmbHiiConfiguration, &RpmbHiiConfigurationSize, &RpmbHiiVarAttr);
  if ((Status == EFI_SUCCESS) && (RpmbHiiConfiguration != NULL)) {
    mKeyMigrationAction = RpmbHiiConfiguration->KeyMigrationAction;
    //
    // Clean up action request
    //
    RpmbHiiConfiguration->KeyMigrationAction = NVME_RPMB_KEY_MIGRATION_VALUE_NO_ACTION;

    Status = gRT->SetVariable (
                    NVME_BASED_RECOVERY_VARIABLE_NAME,
                    &gNvmeBasedRecoveryHiiGuid,
                    RpmbHiiVarAttr,
                    RpmbHiiConfigurationSize,
                    RpmbHiiConfiguration
                    );
    ASSERT_EFI_ERROR (Status);
    if (RpmbHiiConfiguration != NULL) {
      FreePool (RpmbHiiConfiguration);
    }
  }

  //
  // First boot check
  //
  IsFirstBoot = IsNvmeRecoveryFirstBoot (&NvmeBackupInfo);
  if (IsFirstBoot) {
    //
    // First boot, variable need to get initialized.
    // Mark the IsNvmeBackupVarUpdate as True.
    //
    IsNvmeBackupVarUpdate = TRUE;
  }
  Status = GetVariable2 (SYSFW_UPDATE_PROGRESS_VARIABLE_NAME, &gSysFwUpdateProgressGuid, (VOID **) &PreviousUpdateProgress, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "<NVME debug>Get UpdateProgress variable = %r.\n", Status));
  }

  if (PreviousUpdateProgress != NULL) {
    if (PreviousUpdateProgress->Component == UpdatingResiliency) {
      DEBUG ((DEBUG_INFO, "<NVMe backup check> capsule update case, need backup. \n"));
      goto Backup;
    } else {
      DEBUG ((DEBUG_INFO, "<NVMe backup check> Resiliency/capsule update case, if not first boot, skip backup \n"));
      if (!IsFirstBoot) {
        goto Exit;
      }
    }
  }

  if ((!IsFirstBoot) && (mKeyMigrationAction == NVME_RPMB_KEY_MIGRATION_VALUE_NO_ACTION)) {
    DEBUG ((DEBUG_INFO, "<NVMe backup check> not first boot, no need backup. \n"));
    goto Exit;
  }

Backup:
  DEBUG ((DEBUG_INFO, "<NVMe backup check> This is the first boot. \n"));

  //
  // Locate all SSP protocol instances.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiStorageSecurityCommandProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status) || (HandleCount == 0) || (HandleBuffer == NULL)) {
    DEBUG ((DEBUG_ERROR, "Warning: gEfiStorageSecurityCommandProtocolGuid is not existed. \n"));
    ClearUpdateProgress ();
    return;
  }
  DEBUG ((DEBUG_INFO, "We found [%d] StorageSecurity instances totally. \n", HandleCount));

  for (Index = 0; Index < HandleCount; Index ++) {
    Status = gBS->HandleProtocol(
                    HandleBuffer[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **) &DevicePath
                    );

    if (EFI_ERROR (Status)) {
      continue;
    }
    DEBUG ((DEBUG_INFO, "%a: DevicePath: %s\n",
      __FUNCTION__, ConvertDevicePathToText (DevicePath, TRUE, TRUE)));

    if (!IsNvmeDevice (DevicePath)) {
       DEBUG ((DEBUG_ERROR, "It is not a NVMe device. \n"));
       continue;
    } else {
      //
      // try to get the PciIo interface.
      //
      Status = GetProtocolInterface (
                 DevicePath,
                 &gEfiPciIoProtocolGuid,
                 (VOID**)&PciIo
                 );
      if(EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: HandleProtocol for PciIo is %r. \n", __FUNCTION__, Status));
        PciIo = NULL;
      } else {
        IsVmd = IsVmdDevice (PciIo);
      }

      //
      // try to get the PassThrou interface.
      //
      Status = GetProtocolInterface (
                 DevicePath,
                 &gEfiNvmExpressPassThruProtocolGuid,
                 (VOID**)&NvmeDevice
                 );
      if(EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: HandleProtocol for Passthrough is %r. \n", __FUNCTION__, Status));
        NvmeDevice = NULL;
      }

      if((NvmeDevice == NULL) || (PciIo == NULL)) {
        DEBUG ((DEBUG_ERROR, "%a: passthrough or PciIo is NULL %r. \n", __FUNCTION__, Status));
        continue;
      }

      DEBUG ((DEBUG_INFO, "%a: NvmeDevice = 0x%x, PciIo =0x%x\n", __FUNCTION__, NvmeDevice, PciIo));

      IsPortSupported = IsSupportedRootportDevice (
                         IsVmd,
                         DevicePath,
                         &NamespaceId,
                         &CfgSpaceAddress
                         );
      DEBUG ((DEBUG_INFO, "%a: IsPortSupported = %d\n", __FUNCTION__, IsPortSupported));
      if (!IsPortSupported)
        continue;
      //
      // Found the NVMe controller, try to get the SSP interface.
      //
      Status = gBS->HandleProtocol(
                      HandleBuffer[Index],
                      &gEfiStorageSecurityCommandProtocolGuid,
                      (VOID **) &Ssp
                      );

      if (EFI_ERROR (Status)) {
        continue;
      } else {
        Status = gBS->HandleProtocol(
                    HandleBuffer[Index],
                    &gEfiBlockIoProtocolGuid,
                    (VOID **) &BlockIo
                    );

        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Warning: Not support BlockIo, continue to find another one. \n"));
          MediaId = 0;
        } else {
          MediaId = BlockIo->Media->MediaId;
        }
        DEBUG ((DEBUG_INFO, "NVMe: Current MediaID == [%x].\n", MediaId));

        ControllerData = (NVME_ADMIN_CONTROLLER_DATA *)AllocateZeroPool (sizeof (NVME_ADMIN_CONTROLLER_DATA));
        Status = DumpNvmeIdentifyControllerData (NvmeDevice, NamespaceId, ControllerData);
        if (EFI_ERROR (Status)) {
           DEBUG ((DEBUG_ERROR, "Warning: Dump RPMB data failed. \n"));
        }

        //
        // Check the capability if support Boot partition or not
        //
        BpSupport = ReadBpCapability (CfgSpaceAddress, PciIo, IsVmd, NamespaceId);
        if (BpSupport == FALSE) {
          continue;
        }

        //
        // ******* To be carefule here, don't programm key to your NVMe device without cosensus.
        //
        Status = NvmeBasedRecoveryGetSetRpmbKey (Ssp, MediaId, ControllerData);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR,"Nvme RPMB Key Provisioning is failed, Status[%x]. \n", Status));
        } else {
          DEBUG ((DEBUG_INFO,"Nvme RPMB Key Provisioning is Successful! \n"));
        }

        if ((mKeyMigrationAction == NVME_RPMB_KEY_MIGRATION_VALUE_NO_ACTION) || \
            (mKeyMigrationAction == NVME_RPMB_KEY_MIGRATION_VALUE_UNLOCK_BY_PIN_KEY)) {

          //
          // according to NVMe spec, define firmware download by WriteLength per chunk
          //
          if ((ControllerData->Fwug == 0) || (ControllerData->Fwug == 0xFF)) {
            WriteLength = NVME_4K_PORTION_WRITE_UNIT;
          } else {
            WriteLength = (ControllerData->Fwug) * NVME_4K_PORTION_WRITE_UNIT;
          }

          Status = ImageBackUpSpi2Nvme (NvmeDevice, NamespaceId, Ssp, MediaId, WriteLength);
          if (!EFI_ERROR (Status)) {
            NvmeBackupInfo.BackupStatus = BackupSuccess;
            IsNvmeBackupVarUpdate = TRUE;
          }
          DEBUG ((DEBUG_INFO,"  SPI2NVMe Backup is executed with [%x]! \n", Status));
        }
      }
    }
    if ((Ssp  == NULL)) {
      DEBUG ((DEBUG_ERROR, "NVMe does not exist. \n"));
    }
  }

Exit:
  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unknow Failure is found. \n"));
  }

  if (IsNvmeBackupVarUpdate) {
    DEBUG ((DEBUG_INFO, "Set NvmeBackupInfo.BackupStatus = %d\n", NvmeBackupInfo.BackupStatus));
    //
    // Initial the structure revision to align the latest number.
    // This is used to handle the case when revision get changed after capsule update.
    //
    NvmeBackupInfo.Revision = NVME_BACKUP_INFO_REVISION;
    Status  = gRT->SetVariable (
                     NVME_BASED_RECOVERY_INFO_VARIABLE,
                     &gNvmeBasedRecoveryVarGuid,
                     EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                     sizeof (NVME_BACKUP_INFO),
                     &NvmeBackupInfo
                     );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Set NVMe Based Recovery variable fail, Status = %r.\n", Status));
    }
  }

  //
  // Clear progress variable
  //
  ClearUpdateProgress ();

  ZeroMem (mRpmbKey256, sizeof (mRpmbKey256));
  DEBUG ((DEBUG_INFO, "NvmeRecoveryCallBack End.\n"));

  return;
}

/**
  NVMe Resiliency detect csme corruption

  To determine the CSE Image health and then trigger WDT timeout.
**/
VOID
MeNvmeResiliency
(
  VOID
)
{
  UINT64                 HeciBaseAddress;
  HECI_FWS_REGISTER      MeFwSts;
  HECI_GS_SHDW_REGISTER  MeFwSts2;
  UINT8                  WdtTimeOut;

  DEBUG ((DEBUG_INFO, "Me Nvme Resiliency detect start\n"));

  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  MeFwSts.ul      = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS);
  MeFwSts2.ul     = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_2);
  WdtTimeOut      = 3;

  if ((MeFwSts.ul == 0xFFFFFFFF) || (MeFwSts2.ul == 0xFFFFFFFF)) {
    DEBUG ((DEBUG_INFO, "ME is disabled, read status from MeFwHob\n"));
    MeFwSts.ul = GetMeStatusFromFwstsHob (R_ME_CFG_HFS);
    MeFwSts2.ul = GetMeStatusFromFwstsHob (R_ME_CFG_HFS_2);
  }

  DEBUG ((DEBUG_INFO, "MeFwSts = 0x%x!\n", MeFwSts.ul));
  DEBUG ((DEBUG_INFO, "MeFwSts2 = 0x%x!\n", MeFwSts2.ul));

  if ((MeFwSts.r.CurrentState == ME_STATE_RECOVERY) ||
      (MeFwSts.r.FtBupLdFlr == 1) ||
      (MeFwSts2.r.FwUpdIpu == 1)) {
      DEBUG ((DEBUG_INFO, "ME region is corrupted, trigger WDT timeout\n"));
      ArmPlatformWdt (WdtTimeOut);
      CpuDeadLoop ();
  }

  DEBUG ((DEBUG_INFO, "Me Nvme Resiliency end\n"));
  return;
}

/**
  Returns the SMBIOS table with the specified type.

  @param[in]  Type    The type of the SMBIOS table which is searched.
  @param[out] Record  If the functions finishes with success, then it will point the found SMBIOS Record.

  @retval EFI_SUCCESS            Record was found.
  @retval EFI_INVALID_PARAMETER  Table pointer is NULL.
  @retval EFI_NOT_FOUND          Record cannot be found.
**/
EFI_STATUS
GetSmbiosTable (
  IN  SMBIOS_TYPE              Type,
  OUT EFI_SMBIOS_TABLE_HEADER  **Table
  )
{
  EFI_STATUS               Status;
  EFI_SMBIOS_HANDLE        SmbiosHandle;
  EFI_SMBIOS_PROTOCOL      *Smbios;
  EFI_SMBIOS_TABLE_HEADER  *Record;

  if (Table == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Table = NULL;
  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID**) &Smbios
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  do {
    Status = Smbios->GetNext (
                       Smbios,
                       &SmbiosHandle,
                       NULL,
                       &Record,
                       NULL
                       );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }
  } while (Record->Type != Type);

  *Table = Record;

  return EFI_SUCCESS;
}

/**
  This function makes boot time changes to the content of the SMBIOS type 131.

  @param[in] Event    A pointer to the Event that triggered the callback.
  @param[in] Context  A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
UpdateSmbios131 (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                        Status;
  EFI_SMBIOS_TABLE_HEADER           *Record;
  SMBIOS_TABLE_TYPE131              *SmbiosTableType131Data;

  DEBUG ((DEBUG_INFO, "%a() entry \n", __FUNCTION__));

  Status = GetSmbiosTable (MESMBIOS_TYPE_131, &Record);
  if (EFI_ERROR (Status)) {
    return ;
  }

  SmbiosTableType131Data = (SMBIOS_TABLE_TYPE131*) Record;
  if (GetBiosResiliencyType () == SUPPORT_NVME_BASED_RECOVERY) {
    SmbiosTableType131Data->BiosCapabilities.NvmeRecoveryEnabled = 1;
  }

  gBS->CloseEvent (Event);

  return ;
}

/**
  Main entrypoint for NVMe Recovery Initialization.

  @param[in]  ImageHandle     The image handle.
  @param[in]  SystemTable     The system table.

  @retval EFI_SUCCESS            Command completed successfully.
  @retval EFI_INVALID_PARAMETER  Command usage error.
  @retval EFI_OUT_OF_RESOURCES   Not enough resources were available to run the command.
  @retval EFI_ABORTED            Aborted by user.
  @retval EFI_NOT_FOUND          The specified PCD is not found.
  @retval Others                 Error status returned from gBS->LocateProtocol.
**/
EFI_STATUS
EFIAPI
NvmeRecoveryDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_EVENT                 BeforeEndOfDxeEvent;
  EFI_EVENT                 EndofDxeEvent;

  Status                    = EFI_SUCCESS;
  BeforeEndOfDxeEvent       = NULL;
  EndofDxeEvent             = NULL;

  DEBUG ((DEBUG_INFO, "NvmeRecoveryDxeEntryPoint Start.\n"));

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  UpdateSmbios131,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndofDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  if (PcdGet8 (PcdNvmeRecoverySupported) == NVME_RECOVERY_FUNCTION_CSME_NOT_SUPPORT) {
    DEBUG ((DEBUG_INFO, "NVMe Recovery Function is not supported, reason csme not support.\n"));
    Status = EFI_SUCCESS;
    goto Exit;
  }

  // Initialize Nvme Recovery Hii Variable default
  Status = InitNvmeRecoveryHiiDefault ();
  DEBUG ((DEBUG_INFO, "HII variable initialization status:%r\n", Status));

  //
  // Install Hii packages.
  //
  Status = NvmeRecoveryHiiInstall ();
  DEBUG ((DEBUG_INFO, "HII install status:%r\n", Status));

  // If not support the NVMe-based recovery, skip to create the callback event of NvmeRecoveryCallBack.
  if (GetBiosResiliencyType () != SUPPORT_NVME_BASED_RECOVERY) {
    DEBUG ((DEBUG_INFO, "NVMe Recovery Function is not supported, NvmeRecoveryDxeEntryPoint End\n"));
    Status = EFI_SUCCESS;
    goto Exit;
  }

  // If SSD has backup image, try to detect csme health status
  if (IsBiosBackupValid () == TRUE) {
    MeNvmeResiliency();
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  NvmeRecoveryCallBack,
                  NULL,
                  &gPlatformBeforeEndOfDxeEventGroupGuid,
                  &BeforeEndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

Exit:
  DEBUG ((DEBUG_INFO, "NvmeRecoveryDxeEntryPoint End.\n"));
  return Status;
}
