/** @file
  Component Update Library
  Called by each FmpDeviceLib instance to do capsule update.

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

#include <IndustryStandard/Pci22.h>
#include <Protocol/MeFirmwareUpdate.h>
#include <Library/ComponentUpdateLib.h>

//
// Used in GbE update function to update checksum word:
//   The Checksum word (3Fh) should be calculated such that after adding all the words (00h-3Fh),
//   including the Checksum word itself, the sum should be BABAh.
//
#define GBE_NVM_ETHERNET_ADDRESS_OFFSET                       0
#define GBE_NVM_EEPROM_RESERVED_WORD_03_OFFSET                0x03
#define GBE_NVM_EEPROM_RESERVED_WORD_03_CHECKSUM_VALID_BIT    BIT0

#define GBE_NVM_EEPROM_CHECKSUM_WORD_OFFSET           0x3F
#define GBE_NVM_EEPROM_WORDS_SUM                      0xBABA


#define GBE_SHARED_INIT_CONTROL_WORD_OFFSET           0x13
#define GBE_SHARED_INIT_CONTROL_WORD_SIGN_BITS        (BIT14 | BIT15)
#define GBE_SHARED_INIT_CONTROL_WORD_SIGN_POSITION    14
#define GBE_SHARED_INIT_CONTROL_WORD_VALID            2

//
// Used in ME update function
//
#define FW_UPDATE_DISABLED                     0
#define FW_UPDATE_ENABLED                      1

//
// Use to display progress of sending ME FW
//
EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  mMeDisplayProgress = NULL;
UINTN                                          mMeSendProgressStartPercentage = 0;
UINTN                                          mMeSendProgressEndPercentage = 100;

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
  Display the ME update progress.

  @param BytesSent               Bytes already sent to ME
  @param BytesToBeSent           Bytes to be sent to ME

**/
VOID
DisplaySendMeFwStatus (
  UINT32                                BytesSent,
  UINT32                                BytesToBeSent
  )
{
  UINTN  Percentage;

  Percentage = mMeSendProgressStartPercentage + (BytesSent * (mMeSendProgressEndPercentage - mMeSendProgressStartPercentage)) / BytesToBeSent;

  if (mMeDisplayProgress != NULL) {
    mMeDisplayProgress (Percentage);
  }
}

/**
  Update ME Firmware by HECI interface.
  Use the ME FW Update API (FWUpdateLib.lib) provided by ME team to perform the update.

  @param[in] WriteReq                Request information for update flash.
  @param[in] Progress                A function used report the progress of the
                                     firmware update.  This is an optional parameter
                                     that may be NULL.
  @param[in] StartPercentage         The start completion percentage value that may
                                     be used to report progress during the flash
                                     write operation.
  @param[in] EndPercentage           The end completion percentage value that may
                                     be used to report progress during the flash
                                     write operation.

  @retval EFI_SUCCESS                ME FW is updated successfully.
  @retval Others                     The update operation fails.

**/
EFI_STATUS
UpdateMeByHeci (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  )
{
  EFI_STATUS                   Status;
  UINT32                       FwUpdateApiStatus;
  UINT16                       EnabledState;
  BOOLEAN                      InProgress;
  UINT32                       CurrentPercent;
  UINT32                       FwUpdateStatus;
  UINT32                       NeededResetType;
  UINT32                       UpdateTimer;
  UINT32                       PreviousPercent;
  EFI_GUID                     Uuid;
  ME_FIRMWARE_UPDATE_PROTOCOL  *MeFwu;

  DEBUG ((DEBUG_INFO, "UpdateMeByHeci: ME Image Length %d(%xh)\n", WriteReq->DataSize, WriteReq->DataSize));

  EnabledState    = 0;
  InProgress      = FALSE;
  CurrentPercent  = 0;
  FwUpdateStatus  = 0;
  NeededResetType = 0;
  UpdateTimer     = 0;
  PreviousPercent = 0;
  MeFwu           = NULL;

  Status = GetMeFwuProtocol (&MeFwu);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Check FWU enabled state
  //
  FwUpdateApiStatus = MeFwu->EnabledState (&EnabledState);
  if (FwUpdateApiStatus != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "UpdateMeByHeci: FwuEnabledState failed: %r.\n", FwUpdateApiStatus));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "UpdateMeByHeci: FwuEnabledState is 0x%x.\n", EnabledState));

  switch (EnabledState) {
    case FW_UPDATE_DISABLED:
      DEBUG ((DEBUG_ERROR, "UpdateMeByHeci: FWUpdate is disabled.\n"));
      return EFI_DEVICE_ERROR;

    case FW_UPDATE_ENABLED:
      break;

    default:
      break;
  }

  if (Progress != NULL) {
    mMeDisplayProgress = Progress;
    mMeSendProgressStartPercentage = StartPercentage;
    //
    // Assign 1/5 of progress bar to SendProgress
    //
    mMeSendProgressEndPercentage = StartPercentage + ((EndPercentage - StartPercentage) / 5);
  }

  //
  // Starting FWU full update. Send image to FW Update Client
  //
  DEBUG ((DEBUG_INFO, "UpdateMeByHeci: Executing Full FW Update.\n"));

  Status = MeFwu->OemId (&Uuid);
  DEBUG ((DEBUG_INFO, "Firmware OEM ID: %g [%r]\r\n", &Uuid, Status));
  if (!EFI_ERROR (Status) && !CompareGuid (&Uuid, &gZeroGuid)) {
    FwUpdateApiStatus = MeFwu->FullUpdateFromBuffer (WriteReq->Data, (UINT32)WriteReq->DataSize, &Uuid, &DisplaySendMeFwStatus);
  } else {
    FwUpdateApiStatus = MeFwu->FullUpdateFromBuffer (WriteReq->Data, (UINT32)WriteReq->DataSize, NULL, &DisplaySendMeFwStatus);
  }

  if (FwUpdateApiStatus != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "UpdateMeByHeci: FwuFullUpdateFromBuffer failed: %r.\n", FwUpdateApiStatus));
    return EFI_DEVICE_ERROR;
  }

  //
  // Image was sent to FW Update client
  // Poll the FW Update progress until finished
  //
  DEBUG ((DEBUG_INFO, "UpdateMeByHeci: Waiting for FW Update progress to be finished.\n"));
  do {
    FwUpdateApiStatus = MeFwu->CheckUpdateProgress (
                                 &InProgress,
                                 &CurrentPercent,
                                 &FwUpdateStatus,
                                 &NeededResetType
                                 );
    if (FwUpdateApiStatus != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "UpdateMeByHeci: FwuCheckUpdateProgress failed: %r.\n", FwUpdateApiStatus));
      break;
    }

    if (!InProgress) {
      DEBUG ((DEBUG_INFO, "UpdateMeByHeci: FWU Update finished successfully: %r.\n", FwUpdateApiStatus));
      if (Progress != NULL) {
        Progress (EndPercentage);
      }
      break;
    }

    // Update is in progress
    DEBUG ((DEBUG_INFO, "UpdateMeByHeci: Current percent: %d\n", CurrentPercent));
    gBS->Stall (250000); // wait 250 milliseconds before polling again

    // If 30 seconds passed
    if (UpdateTimer >= 30000) {
      // If percent didn't change in 30 seconds
      if (CurrentPercent == PreviousPercent) {
        DEBUG ((DEBUG_ERROR, "UpdateMeByHeci: FwuCheckUpdateProgress timeout.\n"));
        Status = EFI_TIMEOUT;
        break;
      }
      // Percent changed
      PreviousPercent = CurrentPercent;
      UpdateTimer = 0;
    } else {
      // Increase timer
      UpdateTimer += 250;
    }

    if (Progress != NULL) {
      Progress (mMeSendProgressEndPercentage + (CurrentPercent * (EndPercentage - mMeSendProgressEndPercentage)) / 100);
    }
  } while (TRUE);


  DEBUG ((DEBUG_INFO, "UpdateMeByHeci: FwUpdateApiStatus: 0x%x (%d).\n", FwUpdateApiStatus, FwUpdateApiStatus));
  DEBUG ((DEBUG_INFO, "UpdateMeByHeci: FwUpdateStatus   : 0x%x (%d).\n", FwUpdateStatus, FwUpdateStatus));
  DEBUG ((DEBUG_INFO, "UpdateMeByHeci: NeededResetType  : 0x%x.\n", NeededResetType));
  if (EFI_ERROR (Status) || (FwUpdateApiStatus != EFI_SUCCESS) || (FwUpdateStatus != EFI_SUCCESS)) {
    Status = EFI_DEVICE_ERROR;
  } else {
    Status = EFI_SUCCESS;
  }

  return Status;
}

/**
  Perform ME firmware update.

  @param[in] WriteReq                Request information for update flash.
  @param[in] Progress                A function used report the progress of the
                                     firmware update.  This is an optional parameter
                                     that may be NULL.
  @param[in] StartPercentage         The start completion percentage value that may
                                     be used to report progress during the flash
                                     write operation.
  @param[in] EndPercentage           The end completion percentage value that may
                                     be used to report progress during the flash
                                     write operation.

  @retval EFI_SUCCESS                ME FW is updated successfully.
  @retval Others                     The update operation fails.

**/
EFI_STATUS
EFIAPI
UpdateMeFirmware (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  )
{
  EFI_STATUS                      Status;

  DEBUG ((DEBUG_INFO, "UpdateMeFirmware: MeBinSize = %x\n", WriteReq->DataSize));
  DEBUG ((DEBUG_INFO, "UpdateMeFirmware: Update ME firmware with HECI interface\n"));
  //
  // Use the HECI method to perform Me update.
  //
  Status = UpdateMeByHeci (
             WriteReq,
             Progress,
             StartPercentage,
             EndPercentage
             );

  return Status;
}

/**
  Update BIOS region.

  @param[in] WriteReq                    Request information for update flash.
  @param[in] Progress                    A function used report the progress of the
                                         firmware update.  This is an optional parameter
                                         that may be NULL.
  @param[in] StartPercentage             The start completion percentage value that may
                                         be used to report progress during the flash
                                         write operation.
  @param[in] EndPercentage               The end completion percentage value that may
                                         be used to report progress during the flash
                                         write operation.

  @retval EFI_SUCCESS                    The flash region is updated successfully.
  @retval EFI_INVALID_PARAMETER          The input buffer is invalid.
  @retval Others                         The update operation fails.

**/
EFI_STATUS
EFIAPI
UpdateBiosFirmware (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  )
{
  EFI_STATUS          Status;

  DEBUG ((DEBUG_INFO, "UpdateBiosFirmware - 0x%x - 0x%x\n", (UINTN)WriteReq->FlashAddress, WriteReq->DataSize));
  if (WriteReq->BgupImage != NULL && WriteReq->BgupImageSize != 0) {
    if (Progress != NULL) {
      Progress (StartPercentage);
    }
  }

  Status = FlashUpdate(WriteReq, Progress, StartPercentage, EndPercentage);

  if (WriteReq->BgupImage != NULL && WriteReq->BgupImageSize != 0) {
    if (Progress != NULL) {
      Progress (EndPercentage);
    }
  }

  return Status;

}

/**
  Update EC region.

  @param[in] WriteReq                    Request information for update flash.
  @param[in] Progress                    A function used report the progress of the
                                         firmware update.  This is an optional parameter
                                         that may be NULL.
  @param[in] StartPercentage             The start completion percentage value that may
                                         be used to report progress during the flash
                                         write operation.
  @param[in] EndPercentage               The end completion percentage value that may
                                         be used to report progress during the flash
                                         write operation.

  @retval EFI_SUCCESS                    The flash region is updated successfully.
  @retval EFI_INVALID_PARAMETER          The input buffer is invalid.
  @retval Others                         The update operation fails.

**/
EFI_STATUS
EFIAPI
UpdateEcFirmware (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  )
{
  DEBUG ((DEBUG_INFO, "UpdateEcFirmware: Direct SPI.\n"));
  return FlashUpdate (WriteReq, Progress, StartPercentage, EndPercentage);
}

/**
  Update GbE region.

  @param[in] WriteReq                    Request information for update flash.
  @param[in] Progress                    A function used report the progress of the
                                         firmware update.  This is an optional parameter
                                         that may be NULL.
  @param[in] StartPercentage             The start completion percentage value that may
                                         be used to report progress during the flash
                                         write operation.
  @param[in] EndPercentage               The end completion percentage value that may
                                         be used to report progress during the flash
                                         write operation.

  @retval EFI_SUCCESS                    The flash region is updated successfully.
  @retval EFI_INVALID_PARAMETER          The input buffer is invalid.
  @retval Others                         The update operation fails.

**/
EFI_STATUS
EFIAPI
UpdateGbERegion (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  )
{
  EFI_STATUS                      Status;
  UINT8                           *GbEImageBuffer;
  UINT32                          BaseAddress;
  UINT32                          RegionSize;
  UINT16                          Sum16;
  UINT16                          CheckSum16;
  PCH_SPI_PROTOCOL                *SpiProtocol;
  FLASH_ACCESS_REQUEST            Request;
  UINT8                           *GbEBinPtr;
  UINTN                           GbEBinSize;
  UINT16                          Sector1SharedInitControlWord;
  UINT32                          ValidSectorOffset;

  GbEBinPtr   = WriteReq->Data;
  GbEBinSize  = WriteReq->DataSize;

  DEBUG ((DEBUG_INFO, "UpdateGbeRegion: GbEBinSize = %x\n", GbEBinSize));

  GbEImageBuffer = NULL;
  BaseAddress    = 0;
  RegionSize     = 0;
  Sum16          = 0;
  CheckSum16     = 0;
  Sector1SharedInitControlWord = 0;
  ValidSectorOffset            = 0;

  SpiProtocol = GetPchSpiProtocol ();
  if (SpiProtocol == NULL) {
    return EFI_NOT_READY;
  }

  Status = SpiProtocol->GetRegionAddress (SpiProtocol, FlashRegionGbE, &BaseAddress, &RegionSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "The region is not used\n"));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "UpdateGbeRegion: RegionSize = %x\n", RegionSize));

  if ((GbEBinSize < (sizeof (UINT16) * GBE_NVM_EEPROM_CHECKSUM_WORD_OFFSET)) ||
      (GbEBinSize != RegionSize)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read Shared Init Control Word in Sector 1
  //
  Status = SpiProtocol->FlashRead (
                          SpiProtocol,
                          FlashRegionGbE,
                          (UINT32) (GBE_SHARED_INIT_CONTROL_WORD_OFFSET * sizeof (UINT16)),
                          (UINT32) (sizeof (Sector1SharedInitControlWord)),
                          (UINT8 *) &Sector1SharedInitControlWord
                          );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read GbE Sector1SharedInitControlWord\n"));
    return Status;
  }

  //
  // Decide the valid MAC offset
  //
  if (((Sector1SharedInitControlWord & GBE_SHARED_INIT_CONTROL_WORD_SIGN_BITS) >> GBE_SHARED_INIT_CONTROL_WORD_SIGN_POSITION) == GBE_SHARED_INIT_CONTROL_WORD_VALID) {
    ValidSectorOffset = 0; // Sector 1
  } else {
    ValidSectorOffset = (RegionSize / 2); // Sector 2
  }

  GbEImageBuffer = AllocateCopyPool (GbEBinSize, GbEBinPtr);
  if (GbEImageBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate memory buffer\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Read MAC address from valid sector and overwrite GbE image buffer
  // (GbE image buffer should be a clean image that Sector 1 is valid and Sector 2 is empty)
  //
  Status = SpiProtocol->FlashRead (
                          SpiProtocol,
                          FlashRegionGbE,
                          (UINT32) (ValidSectorOffset + GBE_NVM_ETHERNET_ADDRESS_OFFSET),
                          (UINT32) 6,
                          GbEImageBuffer
                          );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read GbE valid MAC address\n"));
    FreePool (GbEImageBuffer);
    return Status;
  }

  //
  // Set checksum valid bit and calculate the checksum.
  //   The Checksum word (3Fh) should be calculated such that after adding all the words (00h-3Fh),
  //   including the Checksum word itself, the sum should be BABAh.
  //
  ((UINT16 *) GbEImageBuffer)[GBE_NVM_EEPROM_RESERVED_WORD_03_OFFSET] |= GBE_NVM_EEPROM_RESERVED_WORD_03_CHECKSUM_VALID_BIT; // indicate checksum field is valid

  Sum16 = CalculateSum16 (
            (UINT16 *)(UINTN) GbEImageBuffer,
            (GBE_NVM_EEPROM_CHECKSUM_WORD_OFFSET * sizeof (UINT16))
            ); // Calculate sum of words 00h-3Eh

  CheckSum16 = (UINT16)((0x10000 + GBE_NVM_EEPROM_WORDS_SUM) - Sum16);

  ((UINT16 *) GbEImageBuffer)[GBE_NVM_EEPROM_CHECKSUM_WORD_OFFSET] = CheckSum16;
  DEBUG ((DEBUG_INFO, "GbE Region: new checksum is: 0x%x\n", CheckSum16));

  ZeroMem (&Request, sizeof (Request));
  Request.FlashRegionType = FlashRegionGbE;
  Request.FlashAddress = 0;
  Request.Data = GbEImageBuffer;
  Request.DataSize = GbEBinSize;
  Status = FlashUpdate (&Request, Progress, StartPercentage, EndPercentage);

  FreePool (GbEImageBuffer);

  return Status;
}

/**
  Perform ISH PDT configuration update

  @param[in] WriteReq                Request information for update flash.
  @param[in] Progress                A function used report the progress of the
                                     firmware update.  This is an optional parameter
                                     that may be NULL.
  @param[in] StartPercentage         The start completion percentage value that may
                                     be used to report progress during the flash
                                     write operation.
  @param[in] EndPercentage           The end completion percentage value that may
                                     be used to report progress during the flash
                                     write operation.

  @retval EFI_SUCCESS                PDT is updated successfully.
  @retval Others                     The update operation fails.

**/
EFI_STATUS
EFIAPI
UpdateIshPdt (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  )
{
  EFI_STATUS                   Status;
  UINT8                        *PaddedPayload;
  UINTN                        PaddedPayloadSize;
  UINT32                       FwUpdateApiStatus;
  ME_FIRMWARE_UPDATE_PROTOCOL  *MeFwu;

  DEBUG ((DEBUG_INFO, "UpdateIshPdt: PdtBinSize = 0x%x\n", WriteReq->DataSize));
  ASSERT (WriteReq->DataSize <= SIZE_4KB);

  PaddedPayload = NULL;
  MeFwu         = NULL;

  Status = GetMeFwuProtocol (&MeFwu);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Payload size must be word aligned and padded with zero byte if size is not even.
  //
  PaddedPayloadSize = ALIGN_VALUE (WriteReq->DataSize, 2);
  DEBUG ((DEBUG_INFO, "UpdateIshPdt: Padded PdtBinSize = 0x%x\n", PaddedPayloadSize));

  PaddedPayload = AllocateZeroPool (PaddedPayloadSize);
  if (PaddedPayload == NULL) {
    DEBUG ((DEBUG_ERROR, "UpdateIshPdt: Ran out of memory resource.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (PaddedPayload, WriteReq->Data, WriteReq->DataSize);

  if (Progress != NULL) {
    Progress (StartPercentage);
  }

  FwUpdateApiStatus = MeFwu->SetIshConfig (PaddedPayload, (UINT32)PaddedPayloadSize);
  DEBUG ((DEBUG_INFO, "UpdateIshPdt FwuStatus = 0x%x\n", FwUpdateApiStatus));

  if (FwUpdateApiStatus != EFI_SUCCESS) {
    Status = EFI_DEVICE_ERROR;
  } else {
    Status = EFI_SUCCESS;
  }

  if (Progress != NULL) {
    Progress (EndPercentage);
  }

  FreePool (PaddedPayload);

  return Status;
}

/**
  The constructor function.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor successfully .
**/
EFI_STATUS
EFIAPI
ComponentUpdateLibConstructor (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  return EFI_SUCCESS;
}
