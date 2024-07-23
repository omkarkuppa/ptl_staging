/** @file
  DXE PSS Operation Library File

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

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PssLib.h>
#include <Library/SerialIoI2cLib.h>

/**
  Read data from PSS chip.

  @param[out]  Buffer
  @param[in]   Address
  @param[in]   Size

  @retval  EFI_STATUS
**/
EFI_STATUS
EFIAPI
ReadPssData (
  UINT8     *Buffer,
  UINT32    Address,
  UINT32    Size
  )
{
  EFI_STATUS      Status;
  UINT8           WriBuf [2] ={0, 0};
  UINT8           I2cNumber;
  UINT8           I2cDeviceAddress;

  if (Size == 0) {
    return EFI_SUCCESS;
  }

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((DEBUG_INFO, "ReadPssData ()\n"));

  I2cNumber         = PcdGet8 (VpdPcdPssI2cBusNumber);
  I2cDeviceAddress  = PcdGet8 (VpdPcdPssI2cDeviceAddress)

  WriBuf [0] = (UINT8) (Address >> 8);
  WriBuf [1] = (UINT8) (Address & 0xFF);

  Status = SerialIoI2cWrite (
             0,
             &I2cNumber,
             I2cDeviceAddress,
             2,
             WriBuf,
             1000000, //1sec
             400, //400kHz
             NULL,
             TRUE,
             TRUE
             );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  return SerialIoI2cRead (
             0,
             &I2cNumber,
             I2cDeviceAddress,
             Size,
             Buffer,
             1000000, //1sec
             400, //400kHz
             NULL,
             FALSE
             );
}

/**
  Write data to PSS chip.

  @param[in]  Buffer
  @param[in]  Address
  @param[in]  Size

  @retval  EFI_STATUS
**/
EFI_STATUS
EFIAPI
WritePssData (
  UINT8     *Buffer,
  UINT32    Address,
  UINTN     Size
  )
{
  EFI_STATUS      Status;
  UINT8           WriBuf [2] ={0, 0};
  UINT8           I2cNumber;
  UINT8           I2cDeviceAddress;

  if (Size == 0) {
    return EFI_SUCCESS;
  }

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((DEBUG_INFO, "WritePssData()\n"));

  I2cNumber         = PcdGet8 (VpdPcdPssI2cBusNumber);
  I2cDeviceAddress  = PcdGet8 (VpdPcdPssI2cDeviceAddress)

  WriBuf [0] = (UINT8) (Address >> 8);
  WriBuf [1] = (UINT8) (Address & 0xFF);

  Status = SerialIoI2cWrite (
             0,
             &I2cNumber,
             I2cDeviceAddress,
             2,
             WriBuf,
             1000000, //1sec
             400, //400kHz
             NULL,
             TRUE,
             TRUE
             );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  return SerialIoI2cRead (
             0,
             &I2cNumber,
             I2cDeviceAddress,
             (UINT32) Size,
             Buffer,
             1000000, //1sec
             400, //400kHz
             NULL,
             FALSE
             );
}

/**
  Check PSS chip status.

  @param[in]  none

  @retval  EFI_STATUS
**/
EFI_STATUS
EFIAPI
PssDetect (
  VOID
  )
{
  EFI_STATUS    Status;
  UINT8         PssData[4];

  //
  // Read the chip's Class ID from the TID bank, it should be 0xE2 (Gen2)
  // Check whether the PSS IC is Monza X-8K
  //
  ZeroMem (PssData, sizeof(PssData));
  Status = ReadPssData(&PssData[0], PSS_BASE_ADDR_CLASSID, 4);
  DEBUG((EFI_D_INFO, "Get PSS Class ID: %2X-%2X-%2X-%2X\n",\
         PssData[0], PssData[1], PssData[2], PssData[3]));
  if (!EFI_ERROR(Status)) {
    if ((PssData[0] == PSS_CHIP_CLASS_ID) &&
        ((PssData [2] & 0x0F) == PSS_CHIP_TID_MODEL_HIGH) &&
        (PssData [3] == PSS_CHIP_TID_MODEL_LOW)
    ) {
      DEBUG((EFI_D_INFO, "PSS (Monza X-8K) deteced\n"));
      return EFI_SUCCESS;  //Monza X-8K
    } else {
      return EFI_NOT_FOUND; // Other device, handled as not found.
    }
  } else {
    return Status;
  }
}

/**
  Get 16 bytes Serial Number from PSS chip.

  @param[in]  Buffer
  @param[in]  Size

  @retval  EFI_STATUS
**/
EFI_STATUS
EFIAPI
PssGetSerialNumber (
  UINT8     *Buffer,
  UINT32    Size
  )
{
  EFI_STATUS                    Status;
  UINT8                         PssSerialNumber[PSS_SN_LENGTH + 1];  // With null termintor for string usage

  if ((Size > PSS_SN_LENGTH) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (PssSerialNumber, PSS_SN_LENGTH + 1);

  Status = PssDetect ();
  if (!EFI_ERROR (Status)) {
    Status = ReadPssData ((UINT8 *) &PssSerialNumber [0], PSS_BASE_ADDR_USER + PSS_CHIP_BOARD_SERIAL_NUMBER_OFFSET, PSS_SN_LENGTH);
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "PSS Serial Number: %a, Data Length: 0x%X\n", PssSerialNumber, AsciiStrLen((CHAR8 *)PssSerialNumber)));
    } else {
      DEBUG ((DEBUG_ERROR, "PSS Read Failed : %r\n", Status));
      return Status;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "PSS detect Failed : %r\n", Status));
    return Status;
  }

  //
  // If no valid Serial number is found, skip filling Smbios Type1/2 Serial number information.
  //
  if (AsciiStrLen((CHAR8 *)PssSerialNumber) == 0) {
    DEBUG ((DEBUG_ERROR, "No valid serial number."));
    return EFI_NOT_FOUND;
  }

  ZeroMem (Buffer, Size);
  CopyMem (Buffer, PssSerialNumber, Size);
  return EFI_SUCCESS;
}
