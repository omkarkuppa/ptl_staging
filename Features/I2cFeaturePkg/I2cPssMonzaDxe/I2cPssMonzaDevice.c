/**@file

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

@par Specification
**/
#include "I2cPssMonza.h"


/**
  Create I2C request packet based on operations size.

  @param[in] Operations         Count of Operations that you want to executed

  @retval Packet                Pointer to EFI_I2C_REQUEST_PACKET
**/
EFI_I2C_REQUEST_PACKET*
PssMonzaNewRequestPacket (
  IN UINTN Operations
  )
{
  EFI_I2C_REQUEST_PACKET *NewPacket;

  if (Operations == 0) {
    return NULL;
  }
  NewPacket = AllocateZeroPool (sizeof (EFI_I2C_REQUEST_PACKET) + (Operations - 1) * sizeof (EFI_I2C_OPERATION));

  if (NewPacket != NULL) {
    NewPacket->OperationCount = Operations;
  }

  return NewPacket;
}

/**
  Delete I2C request packet.

  @param[in] Packet             I2C request packet that you want to delete.

**/
VOID
PssMonzaDeleteRequestPacket (
  IN EFI_I2C_REQUEST_PACKET *Packet
  )
{
  if (Packet != NULL) {
    FreePool (Packet);
  }
}

/**
  Read data from PSS chip.

  @param[in]   I2cIoProtocol           Pointer of EFI_I2C_IO_PROTOCOL.
  @param[out]  Buffer                  Return read data from PSS chip
  @param[in]   Address                 Address located in PSS chip
  @param[in]   Size                    Data Size

  @retval  EFI_DEVICE_ERROR            Fail to QueueRequest I2C device
  @retval  EFI_INVALID_PARAMETER       Buffer is NULL
  @retval  EFI_OUT_OF_RESOURCES        Don't have enough to create I2C request packet.
  @retval  EFI_SUCCESS                 Read data from PSS chip successfully
**/
EFI_STATUS
EFIAPI
PssMonzaReadData (
  IN  EFI_I2C_IO_PROTOCOL *I2cIoProtocol,
  OUT UINT8               *Buffer,
  IN  UINT32              Address,
  IN  UINTN               Size
  )
{
  UINT8                  WriteBuffer[2];
  EFI_I2C_REQUEST_PACKET *RequestPacket;
  EFI_STATUS             Status;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  RequestPacket = NULL;

  if (Size == 0) {
    Status = EFI_SUCCESS;
    goto End;
  }

  if (Buffer == NULL) {
    DEBUG ((DEBUG_INFO, "I2C check input Buffer Failed, Status = %r\n", EFI_INVALID_PARAMETER));
    Status = EFI_INVALID_PARAMETER;
    goto End;
  }

  WriteBuffer[0] = (UINT8) (Address >> 8);
  WriteBuffer[1] = (UINT8) (Address & 0xFF);

  RequestPacket = PssMonzaNewRequestPacket (2);
  if (RequestPacket == NULL) {
    DEBUG ((DEBUG_INFO, "I2C PssMonzaNewRequestPacket Failed, Status = %r\n", EFI_OUT_OF_RESOURCES));
    Status = EFI_OUT_OF_RESOURCES;
    goto End;
  }

  RequestPacket->Operation[0].Flags         = 0;
  RequestPacket->Operation[0].LengthInBytes = sizeof (WriteBuffer);
  RequestPacket->Operation[0].Buffer        = WriteBuffer;
  RequestPacket->Operation[1].Flags         = I2C_FLAG_READ;
  RequestPacket->Operation[1].LengthInBytes = (UINT32) Size;
  RequestPacket->Operation[1].Buffer        = Buffer;
  Status = I2cIoProtocol->QueueRequest (
                            I2cIoProtocol,
                            0,
                            NULL,
                            RequestPacket,
                            NULL
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "I2C QueueRequest Failed, Status = %r\n", Status));
    Status = EFI_DEVICE_ERROR;
    goto End;
  }

End:
  PssMonzaDeleteRequestPacket (RequestPacket);
  DEBUG ((DEBUG_INFO, "%a End, Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Check PSS Monza chip whether exists.

  @param[in] This                     Pointer of EFI_I2C_IO_PROTOCOL.

  @retval  TRUE                       Detect PSS Monza chip successfully.
  @retval  FALSE                      Unable to detect PSS Monza chip.
**/
BOOLEAN
EFIAPI
IsPssMonzaExist (
  IN EFI_I2C_IO_PROTOCOL *I2cIoProtocol
  )
{
  EFI_STATUS    Status;
  UINT8         PssData[4];
  BOOLEAN       IsChipDetect;

  IsChipDetect = FALSE;
  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  //
  // Read the chip's Class ID from the TID bank, it should be 0xE2 (Gen2)
  // Check whether the PSS IC is Monza X-8K
  //
  ZeroMem (PssData, sizeof (PssData));
  Status = PssMonzaReadData (
             I2cIoProtocol,
             &PssData[0],
             PSS_BASE_ADDR_CLASSID,
             sizeof (PssData)
             );
  DEBUG ((
    DEBUG_INFO,
    "Get PSS Class ID: %2X-%2X-%2X-%2X\n",
    PssData[0],
    PssData[1],
    PssData[2],
    PssData[3]
    ));

  if (!EFI_ERROR (Status)) {
    if ((PssData[0] == PSS_CHIP_CLASS_ID) &&
        ((PssData[2] & 0x0F) == PSS_CHIP_TID_MODEL_HIGH) &&
        (PssData[3] == PSS_CHIP_TID_MODEL_LOW)) {
      DEBUG ((DEBUG_INFO, "PSS (Monza X-8K) detected\n"));
      IsChipDetect = TRUE;
    }
  }
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return IsChipDetect;
}

/**
  Get 16 bytes Serial Number from PSS chip.

  @param[in]       *This              Pointer of I2C_PLATFORM_SECURE_STORAGE_PROTOCOL.
  @param[in, out]  Buffer             Data to store Serial Number from PSS chip

  @retval  EFI_SUCCESS                Get Serial Number from PSS chip successfully.
  @retval  EFI_INVALID_PARAMETER      Buffer is NULL or necessary protocol is not exist.
  @retval  EFI_DEVICE_ERROR           PSS chip detect failure.
  @retval  EFI_NOT_FOUND              No identify PSS chip is found.
**/
EFI_STATUS
EFIAPI
PssMonzaGetSerialNumber (
  IN     I2C_PLATFORM_SECURE_STORAGE_PROTOCOL *This,
  IN OUT UINT8                                *Buffer
  )
{
  PSS_MONZA_PRIVATE_CONTEXT     *PssMonzaContext;
  EFI_I2C_IO_PROTOCOL           *I2cIoProtocol;
  EFI_STATUS                    Status;
  UINT8                         PssSerialNumber[PSS_SN_LENGTH + 1];  // With null terminator for string usage

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  PssMonzaContext = PSS_MONZA_CONTEXT_FROM_PROTOCOL (This);

  if ((Buffer == NULL) || (PssMonzaContext->I2cIoProtocol == NULL)) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_INFO, "%a End, Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  I2cIoProtocol = PssMonzaContext->I2cIoProtocol;

  ZeroMem (PssSerialNumber, sizeof (PssSerialNumber));

  if (IsPssMonzaExist (I2cIoProtocol)) {
    Status = PssMonzaReadData (
               I2cIoProtocol,
               (UINT8 *) &PssSerialNumber[0],
               PSS_BASE_ADDR_USER + PSS_CHIP_BOARD_SERIAL_NUMBER_OFFSET,
               PSS_SN_LENGTH
               );
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "PSS Serial Number: %a, Data Length: 0x%X\n", PssSerialNumber, AsciiStrLen ((CHAR8 *)PssSerialNumber)));
    } else {
      Status = EFI_DEVICE_ERROR;
      DEBUG ((DEBUG_INFO, "PSS read Failed. %a End, Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
  } else {
    Status = EFI_NOT_FOUND;
    DEBUG ((DEBUG_INFO, "PSS not exist. %a End, Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // If no valid Serial number is found, skip filling Smbios Type1/2 Serial number information through PSS chip.
  //
  if (AsciiStrLen ((CHAR8 *) PssSerialNumber) == 0) {
    Status = EFI_NOT_FOUND;
    DEBUG ((DEBUG_INFO, "No valid serial number. %a End, Status = %r\n", __FUNCTION__, Status));
    return Status;
  } else {
    CopyMem (Buffer, PssSerialNumber, PSS_SN_LENGTH + 1);
    Status = EFI_SUCCESS;
    DEBUG ((DEBUG_INFO, "%a Get PSS Serial Number Status = %r , Serial Number = %a\n", __FUNCTION__, Status, PssSerialNumber));
    return Status;
  }
}

/**
  Get 16 bytes Mac Address from PSS chip.

  @param[in]       *This              Pointer of I2C_PLATFORM_SECURE_STORAGE_PROTOCOL.
  @param[in, out]  Buffer             Data to store Mac Address from PSS chip

  @retval  EFI_SUCCESS                Get Mac Addressr from PSS chip successfully.
  @retval  EFI_INVALID_PARAMETER      Buffer is NULL or necessary protocol is not exist.
  @retval  EFI_DEVICE_ERROR           PSS chip detect failure.
  @retval  EFI_NOT_FOUND              No identify PSS chip is found.
**/
EFI_STATUS
EFIAPI
PssMonzaGetMacAddress (
  IN     I2C_PLATFORM_SECURE_STORAGE_PROTOCOL *This,
  IN OUT UINT16                                *Buffer
  )
{
  PSS_MONZA_PRIVATE_CONTEXT     *PssMonzaContext;
  EFI_I2C_IO_PROTOCOL           *I2cIoProtocol;
  EFI_STATUS                    Status;
  UINT16                        PssMacAddress[PSS_CHIP_MAC_ADDRESS_LENGTH / 2];  // With null termintor for string usage
  UINT16                        PssMacAddress_normalorder[PSS_CHIP_MAC_ADDRESS_LENGTH / 2];

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  PssMonzaContext = PSS_MONZA_CONTEXT_FROM_PROTOCOL (This);

  if ((This == NULL) ||
      (Buffer == NULL) ||
      (PssMonzaContext->I2cIoProtocol == NULL)) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_INFO, "%a End, Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  I2cIoProtocol = PssMonzaContext->I2cIoProtocol;

  ZeroMem (PssMacAddress, sizeof (PssMacAddress));

  if (IsPssMonzaExist (I2cIoProtocol)) {
    Status = PssMonzaReadData (
               I2cIoProtocol,
               (UINT8 *) &PssMacAddress[0],
               PSS_BASE_ADDR_USER + PSS_CHIP_MAC_ADDRESS_OFFSET,
               PSS_CHIP_MAC_ADDRESS_LENGTH
               );
    //
    // Byte swapping
    //
    PssMacAddress_normalorder[0] = ((PssMacAddress[0] << 8) & 0xff00) | ((PssMacAddress[0] >> 8) & 0x00ff);
    PssMacAddress_normalorder[1] = ((PssMacAddress[1] << 8) & 0xff00) | ((PssMacAddress[1] >> 8) & 0x00ff);
    PssMacAddress_normalorder[2] = ((PssMacAddress[2] << 8) & 0xff00) | ((PssMacAddress[2] >> 8) & 0x00ff);

    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "PSS MAC Address in reverse order: %x %x %x, Data Length: 0x%X\n", PssMacAddress[0], PssMacAddress[1], PssMacAddress[2], sizeof (PssMacAddress)));
      DEBUG ((DEBUG_INFO, "PSS MAC Address: %x %x %x, Data Length: 0x%X\n", PssMacAddress_normalorder[0], PssMacAddress_normalorder[1], PssMacAddress_normalorder[2], sizeof (PssMacAddress_normalorder)));
    } else {
      Status = EFI_DEVICE_ERROR;
      DEBUG ((DEBUG_INFO, "PSS read Failed. %a End, Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
  } else {
    Status = EFI_NOT_FOUND;
    DEBUG ((DEBUG_INFO, "PSS not exist. %a End, Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // If no valid MAC Address is found, skip filling Smbios Type1/2 MAC Address information through PSS chip.
  //
  if (sizeof (PssMacAddress) == 0) {
    Status = EFI_NOT_FOUND;
    DEBUG ((DEBUG_INFO, "No valid Mac Address. %a End, Status = %r\n", __FUNCTION__, Status));
    return Status;
  }
  if (PssMacAddress[0] == 0 && PssMacAddress[1] == 0 && PssMacAddress[2] == 0) {
    Status = EFI_NOT_FOUND;
    DEBUG ((DEBUG_INFO, "No valid Mac Address. %a End, Status = %r\n", __FUNCTION__, Status));
    return Status;
  }
  if (PssMacAddress_normalorder[0] == 0 && PssMacAddress_normalorder[1] == 0 && PssMacAddress_normalorder[2] == 0) {
      //check if it is equal to 0
      Status = EFI_NOT_FOUND;
      DEBUG ((DEBUG_INFO, "The data read from PSS Chip is equal to 0\n"));
      return Status;
  }else {
    CopyMem (Buffer, PssMacAddress, PSS_CHIP_MAC_ADDRESS_LENGTH);
    Status = EFI_SUCCESS;
    DEBUG ((DEBUG_INFO, "%a Get PSS Mac Address Status = %r , MAC Address = %x %x %x\n", __FUNCTION__, Status, PssMacAddress[0], PssMacAddress[1], PssMacAddress[2]));
    return Status;
  }
}
