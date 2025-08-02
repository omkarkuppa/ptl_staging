/** @file
  UsbCPdBridgeUpdateLib instance to support Intel PD Bridge firmware update

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UsbCPdBridgeUpdateLib.h>
#include <Protocol/UsbCPdBridgeProtocol.h>
#include <LastAttemptStatus.h>
#include <FmpLastAttemptStatus.h>
#include <Library/UsbcCapsuleDebugLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>

UINT8 gVendorCommandData[72];

/**
  In the vendor command data, the first eight bytes represent the header and
  it is always the same. The header structure is as follows:

  | Byte Position | Description     |
  |---------------|-----------------|
  | 0             | Vendor Command  |
  | 1-3           | Reserved        |
  | 4-5           | Vendor ID       |
  | 6-7           | Product ID      |

  Initialize the vendor command data structure before sending the vendor command.

  @param[in] ProductId   The Product ID to be set in the vendor command data.

  @retval EFI_SUCCESS           The vendor command data was initialized successfully.
  @retval EFI_INVALID_PARAMETER The Product ID is invalid (zero).

**/
EFI_STATUS
InitialVendorCommandData (
  UINT16 ProductId
  )
{
  UINT16 VendorId;

  if (ProductId == 0) {
    return EFI_INVALID_PARAMETER;
  }

  VendorId = PD_BRIDGE_VENDOR_ID;

  ZeroMem (gVendorCommandData, sizeof (gVendorCommandData));
  CopyMem (&gVendorCommandData[VENDOR_SPECIFIC_CMD_VENDOR_ID_OFFSET], &VendorId, sizeof (VendorId));
  CopyMem (&gVendorCommandData[VENDOR_SPECIFIC_CMD_PRODUCT_ID_OFFSET], &ProductId, sizeof (ProductId));

  return EFI_SUCCESS;
}

/**
  The command to read data up to 32bytes from NVM.

  @param[in]  This            The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex        TCP index which PD Bridge is connected to.
  @param[in]  NvmOffset       This field contains the first address to be read relative to the base address of the
                              region being read. NVM Offset is specified in DWs.
  @param[in]  Length          Number of DoubleWords that shall be read starting from the NVM Offset field
                              value
  @param[out] OutputData      The Data read from NVM
  @param[out] OutputDataSize  The size of Data read from NVM

  @retval EFI_SUCCESS         Read data successfully
  @retval others              Read data failed

**/
EFI_STATUS
NvmRead (
  IN   USBC_PD_BRIDGE_PROTOCOL  *This,
  IN   UINT8                    TcpIndex,
  IN   UINT32                   NvmOffset,
  IN   UINT8                    Length,
  OUT  UINT8                    *OutputData,
  OUT  UINT8                    *OutputDataSize
  )
{
  EFI_STATUS             Status;
  NVM_READ_CMD_METADATA  ReadCmdData;
  UINT32                 DataSize;
  UINT8                  VendorCmd;

  if ((Length > VENDOR_SPECIFIC_CMD_VENDOR_CMD_DATA_MAX_BYTES) || (Length == 0)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_READ_LENGTH_INVALID, Length, 0);
    return EFI_INVALID_PARAMETER;
  }

  if ((OutputData == NULL) || (OutputDataSize == NULL)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_READ_OUTPUT_DATA_NULL, 0, 0);
    return EFI_INVALID_PARAMETER;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_READ_OFFSET_LENGTH, NvmOffset, (UINT32) Length);
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_READ_DATA_INDEX, (UINT32) TcpIndex, 0);

  ReadCmdData.Reserved1 = 0;
  ReadCmdData.NvmOffset = NvmOffset;
  ReadCmdData.Length    = Length;
  ReadCmdData.Reserved2 = 0;
  VendorCmd             = VENDOR_SPECIFIC_CMD_NVM_READ;
  DataSize              = sizeof (ReadCmdData) + VENDOR_SPECIFIC_CMD_HEADER_TOTAL_BYTES;
  CopyMem (&gVendorCommandData[VENDOR_SPECIFIC_CMD_VENDOR_CMD_OFFSET], &VendorCmd, sizeof (VendorCmd));
  CopyMem (&gVendorCommandData[VENDOR_SPECIFIC_CMD_DATA_OFFSET], &ReadCmdData, sizeof (ReadCmdData));

  Status = This->ExecuteVendorCmd (This, TcpIndex, VENDOR_SPECIFIC_CMD_NVM_READ, FALSE, (UINT8 *) &gVendorCommandData, (UINT8 *) &DataSize, (UINT8 *) OutputData, (UINT8 *) OutputDataSize);
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_VENDOR_CMD_FAIL, (UINT32) VENDOR_SPECIFIC_CMD_NVM_READ, (UINT32) Status);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  The command to set the first location in NVM to be written by the following NVM_WRITE command.
  This NVM offset is incremented by the CommandDataLen after each NVM_WRITE.
  The address is specified in DWs.

  @param[in]  This           The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex       TCP index which PD Bridge is connected to.
  @param[in]  NvmOffset      This field contains the first address to be written relative to the base address of the
                             region been written.

  @retval EFI_SUCCESS        Set Offset successfully
  @retval others             Read data failed

**/
EFI_STATUS
NvmSetOffset (
  IN  USBC_PD_BRIDGE_PROTOCOL  *This,
  IN  UINT8                    TcpIndex,
  IN  UINT32                   NvmOffset
  )
{
  EFI_STATUS                   Status;
  NVM_SET_OFFSET_CMD_METADATA  SetOffsetCmdData;
  UINT32                       DataSize;
  UINT8                        VendorCmd;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_SET_OFFSET_INDEX, NvmOffset, (UINT32) TcpIndex);

  SetOffsetCmdData.Reserved1 = 0;
  SetOffsetCmdData.NvmOffset = NvmOffset;
  SetOffsetCmdData.Reserved2 = 0;
  DataSize                   = sizeof (SetOffsetCmdData) + VENDOR_SPECIFIC_CMD_HEADER_TOTAL_BYTES;
  VendorCmd                  = VENDOR_SPECIFIC_CMD_NVM_SET_OFFSET;
  CopyMem (&gVendorCommandData[VENDOR_SPECIFIC_CMD_VENDOR_CMD_OFFSET], &VendorCmd, sizeof (VendorCmd));
  CopyMem (&gVendorCommandData[VENDOR_SPECIFIC_CMD_DATA_OFFSET], &SetOffsetCmdData, sizeof (SetOffsetCmdData));

  Status = This->ExecuteVendorCmd (This, TcpIndex, VENDOR_SPECIFIC_CMD_NVM_SET_OFFSET, FALSE, (UINT8 *) gVendorCommandData, (UINT8 *) &DataSize, NULL, NULL);
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_SET_OFFSET_VENDOR_CMD_FAIL, (UINT32) VENDOR_SPECIFIC_CMD_NVM_SET_OFFSET, (UINT32) Status);
    return Status;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_SET_OFFSET_SUCCESS, NvmOffset, (UINT32) TcpIndex);

  return EFI_SUCCESS;
}

/**
  The command writes (CommandDataLen - 1) bytes in NVM, starting at the address equal to the NVM_OFFSET.
  PD Bridge increases its NVM Offset value by (CommandDataLen - 1) after executing
  this command (even if the write operation fails).

  @param[in]  This           The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex       TCP index which PD Bridge is connected to.
  @param[in]  Buffer         Data need to be written to NVM.
  @param[in]  BufferSize     The size of Data need to be written to NVM.

  @retval EFI_SUCCESS        write data to NVM successfully
  @retval others             write data to NVM failed

**/
EFI_STATUS
NvmWrite (
  IN  USBC_PD_BRIDGE_PROTOCOL  *This,
  IN  UINT8                    TcpIndex,
  IN  UINT8                    *Buffer,
  IN  UINT8                    BufferSize
  )
{
  EFI_STATUS  Status;
  UINT32      DataSize;
  UINT8       VendorCmd;

  if (Buffer == NULL) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_WRITE_NULL, 0, 0);
    return EFI_INVALID_PARAMETER;
  }

  if ((BufferSize > VENDOR_SPECIFIC_CMD_VENDOR_CMD_DATA_MAX_BYTES) || (BufferSize == 0)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_WRITE_INVALID_BUFFER_SIZE, BufferSize, 0);
    return EFI_INVALID_PARAMETER;
  }

  DataSize  = BufferSize + VENDOR_SPECIFIC_CMD_HEADER_TOTAL_BYTES;
  VendorCmd = VENDOR_SPECIFIC_CMD_NVM_WRITE;
  CopyMem (&gVendorCommandData[VENDOR_SPECIFIC_CMD_VENDOR_CMD_OFFSET], &VendorCmd, sizeof (VendorCmd));
  CopyMem (&gVendorCommandData[VENDOR_SPECIFIC_CMD_DATA_OFFSET], Buffer, BufferSize);

  Status = This->ExecuteVendorCmd (This, TcpIndex, VENDOR_SPECIFIC_CMD_NVM_WRITE, FALSE, (UINT8 *) gVendorCommandData, (UINT8 *) &DataSize, NULL, NULL);
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_WRITE_VENDOR_CMD_FAIL, (UINT32) VENDOR_SPECIFIC_CMD_NVM_WRITE, (UINT32) Status);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  The command indicates to the PD Bridge that all data was sent to it.
  The PD Bridge performs an authentication check over the data written.

  @param[in]  This           The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex       TCP index which PD Bridge is connected to.

  @retval EFI_SUCCESS        send the command successfully
  @retval others             write data to NVM failed

**/
EFI_STATUS
NvmAuthenticateWrite (
  IN  USBC_PD_BRIDGE_PROTOCOL  *This,
  IN  UINT8                    TcpIndex
  )
{
  EFI_STATUS  Status;
  UINT32      DataSize;
  UINT8       VendorCmd;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_AUTH_CMD_INDEX, (UINT32) TcpIndex, 0);

  DataSize  = VENDOR_SPECIFIC_CMD_HEADER_TOTAL_BYTES;
  VendorCmd = VENDOR_SPECIFIC_CMD_NVM_AUTHENTICATE_WRITE;
  CopyMem (&gVendorCommandData[VENDOR_SPECIFIC_CMD_VENDOR_CMD_OFFSET], &VendorCmd, sizeof (VendorCmd));

  Status = This->ExecuteVendorCmd (This, TcpIndex, VENDOR_SPECIFIC_CMD_NVM_AUTHENTICATE_WRITE, FALSE, (UINT8 *) gVendorCommandData, (UINT8 *) &DataSize, NULL, NULL);
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_AUTH_VENDOR_CMD_FAIL, (UINT32) VENDOR_SPECIFIC_CMD_NVM_AUTHENTICATE_WRITE, (UINT32) Status);
    return Status;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_AUTH_WRITE_CMD_SUCCESS, (UINT32) TcpIndex, 0);

  return EFI_SUCCESS;
}

/**
  The commands indicates to the PD Bridge that the "second" PD Bridge performs NVM updates flow.
  After receiving this command, the PD Bridge performs a PD port reset and stalls any access to the NVM
  until it gets the RESET command.

  @param[in]  This           The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex       TCP index which PD Bridge is connected to.

  @retval EFI_SUCCESS        send the command successfully
  @retval others             write data to NVM failed

**/
EFI_STATUS
NvmStallNvmAccess (
  IN USBC_PD_BRIDGE_PROTOCOL  *This,
  IN UINT8                    TcpIndex
  )
{
  EFI_STATUS  Status;
  UINT32      DataSize;
  UINT8       VendorCmd;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_STALL_INDEX, (UINT32) TcpIndex, 0);

  DataSize  = VENDOR_SPECIFIC_CMD_HEADER_TOTAL_BYTES;
  VendorCmd = VENDOR_SPECIFIC_CMD_STALL_NVM_ACCESS;
  CopyMem (&gVendorCommandData[VENDOR_SPECIFIC_CMD_VENDOR_CMD_OFFSET], &VendorCmd, sizeof (VendorCmd));

  Status = This->ExecuteVendorCmd (This, TcpIndex, VENDOR_SPECIFIC_CMD_STALL_NVM_ACCESS, FALSE, (UINT8 *) gVendorCommandData, (UINT8 *) &DataSize, NULL, NULL);
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_STALL_VENDOR_CMD_FAIL, (UINT32) VENDOR_SPECIFIC_CMD_STALL_NVM_ACCESS, (UINT32) Status);
    return Status;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_STALL_CMD_SUCCESS, (UINT32) TcpIndex, 0);

  return EFI_SUCCESS;
}

/**
  The command indicates to the PD Bridge that the "second" PD Bridge completed the authentication check over the data written.
  After receiving this command, the PD Bridge performs a SW reset and "authenticate on boot" flow again, the reloads
  all the memories/data from the NVM.

  @param[in]  This           The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex       TCP index which PD Bridge is connected to.

  @retval EFI_SUCCESS        send the command successfully
  @retval others             write data to NVM failed

**/
EFI_STATUS
NvmReset (
  IN USBC_PD_BRIDGE_PROTOCOL  *This,
  IN UINT8                    TcpIndex
  )
{
  EFI_STATUS  Status;
  UINT32      DataSize;
  UINT8       VendorCmd;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_RESET_INDEX, (UINT32) TcpIndex, 0);

  DataSize  = VENDOR_SPECIFIC_CMD_HEADER_TOTAL_BYTES;
  VendorCmd = VENDOR_SPECIFIC_CMD_RESET;
  CopyMem (&gVendorCommandData[VENDOR_SPECIFIC_CMD_VENDOR_CMD_OFFSET], &VendorCmd, sizeof (VendorCmd));

  Status = This->ExecuteVendorCmd (This, TcpIndex, VENDOR_SPECIFIC_CMD_RESET, FALSE, (UINT8 *) gVendorCommandData, (UINT8 *) &DataSize, NULL, NULL);
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_RESET_VENDOR_CMD_FAIL, (UINT32) VENDOR_SPECIFIC_CMD_RESET, (UINT32) Status);
    return Status;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_RESET_CMD_SUCCESS, (UINT32) TcpIndex, 0);
  return EFI_SUCCESS;
}

/**
  The command indicates to the PD Bridge that the "second" PD Bridge starts NVM update flow.

  @param[in]  This      The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex  TCP index which PD Bridge is connected to.

  @retval EFI_SUCCESS   Send the command successfully
  @retval others        Failed to send the command

**/
EFI_STATUS
EnterNvmUpdateMode (
  IN USBC_PD_BRIDGE_PROTOCOL  *This,
  IN UINT8                    TcpIndex
  )
{
  EFI_STATUS  Status;
  UINT32      DataSize;
  UINT8       VendorCmd;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_ENTER_NVM_UPDATE_MODE_INDEX, (UINT32) TcpIndex, 0);

  DataSize  = VENDOR_SPECIFIC_CMD_HEADER_TOTAL_BYTES;
  VendorCmd = VENDOR_SPECIFIC_CMD_ENTER_NVM_UPDATE_MODE;
  CopyMem (&gVendorCommandData[VENDOR_SPECIFIC_CMD_VENDOR_CMD_OFFSET], &VendorCmd, sizeof (VendorCmd));

  Status = This->ExecuteVendorCmd (This, TcpIndex, VENDOR_SPECIFIC_CMD_ENTER_NVM_UPDATE_MODE, FALSE, (UINT8 *) gVendorCommandData, (UINT8 *) &DataSize, NULL, NULL);
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_ENTER_NVM_UPDATE_MODE_VENDOR_CMD_FAILED, (UINT32) VENDOR_SPECIFIC_CMD_ENTER_NVM_UPDATE_MODE, (UINT32) Status);
    return Status;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_ENTER_NVM_UPDATE_MODE_CMD_SUCCESS, (UINT32) TcpIndex, 0);
  return EFI_SUCCESS;
}

/**
  Update NVM Firmware on PD Bridge with given PD Bridge index.

  Flow to update PD Bridge NVM Firmware:
  1.  Send the NVM_SET_OFFSET command which set the first location in NVM to be written by the following
      NVM_WRITE command
  2.  Send a sequence of NVM_WRITE commands, each writing from 1 to 32 bytes of data to NVM. NVM offset will
      be incremented by the CommandDataLen of each NVM_WRITE
  3.a Shared Flash mode disable :
      i.   send NVM_AUTHENTICATE_WRITE command to indicate to PD Bridge that all data was sent.
  3.b Shared Flash mode enable :
      i.   send STALL_NVM_ACCESS to second PD Bridge to perform a PD port reset. PD Bridge will stall any access
           to NVM until get RESET command
      ii.  send NVM_AUTHENTICATE_WRITE command to indicate to PD Bridge that all data was sent.
      iii. send RESET command to second PD Bridge

  @param[in]  This                The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  PdBridgeImage       Pointer to an image buffer contains PD Bridge FW to be updated with.
  @param[in]  ImageSize           The size of PD Bridge image buffer.
  @param[in]  PrivateData         The pointer to the Firmware Private Data
  @param[in]  Progress            A function used to report the progress of updating the firmware
                                  device with the new firmware image.
  @param[in]  StartPercentage     The start completion percentage value that may be used to report progress
                                  during the flash write operation. The value is between 1 and 100.
  @param[in]  EndPercentage       The end completion percentage value that may be used to report progress
                                  during the flash write operation.
  @param[out] LastAttemptStatus   A pointer to a UINT32 that holds the last attempt status to report back
                                  to the ESRT table in case of error. This value will only be checked when
                                  this function returns an error.

  @retval EFI_SUCCESS             PD Bridge Firmware is successfully updated.
  @retval Others                  An error occurred attempting to access the PD Bridge firmware

**/
EFI_STATUS
EFIAPI
UpdatePdBridgeNvmFirmware (
  IN   USBC_PD_BRIDGE_PROTOCOL                        *This,
  IN   UINT8                                          *PdBridgeImage,
  IN   UINTN                                          ImageSize,
  IN   FIRMWARE_PRIVATE_DATA                          *PrivateData,
  IN   EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress, OPTIONAL
  IN   UINTN                                          StartPercentage,
  IN   UINTN                                          EndPercentage,
  OUT  UINT32                                         *LastAttemptStatus
  )
{
  EFI_STATUS  Status;
  UINT8       *BufferPointer;
  UINT32      Offset;
  UINT32      RecoveryCount;
  UINTN       DisplayLength;
  UINT8       WriteLength;
  UINT8       TcpIndex;

  if ((This == NULL) || (PdBridgeImage == NULL) || (EndPercentage < StartPercentage) || (PrivateData == NULL)) {
    ASSERT (This != NULL);
    ASSERT (PdBridgeImage != NULL);
    ASSERT (EndPercentage > StartPercentage);
    ASSERT (PrivateData != NULL);
    return EFI_INVALID_PARAMETER;
  }

  TcpIndex = PrivateData->PdBridge.TcpIndex;
  DisplayLength = EndPercentage - StartPercentage;

  BufferPointer  = (UINT8 *) PdBridgeImage;
  ImageSize     -= (*((UINT32 *)BufferPointer) & PD_BRIDGE_IMAGE_START_ADDRESS_MASK);
  BufferPointer += ((*((UINT32 *)BufferPointer) & PD_BRIDGE_IMAGE_START_ADDRESS_MASK));


  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_START, (UINT32) ImageSize, 0);

  for (RecoveryCount = 0; RecoveryCount < PD_BRIDGE_TOTAL_NUM_OF_RECOVERIES_DURING_IMAGE_WRITE; RecoveryCount++) {
    Status = NvmSetOffset (This, TcpIndex, PD_BRIDGE_NVM_OFFSET);
    if (EFI_ERROR (Status)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_SET_OFFSET_FAIL, (UINT32) Status, 0);
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_NVM_SET_OFFSET_FAILED;
      continue;
    }

    if (PrivateData->PdBridge.ShareFlashMode == SHARE_FLASH_MODE_ENABLE) {
      Status = EnterNvmUpdateMode (This, TcpIndex + 1);
      if (EFI_ERROR (Status)) {
        CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_ENTER_NVM_UPDATE_MODE_FAILED, (UINT32) Status, 0);
        *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_ENTER_NVM_UPDATE_MODE_FAILED;
        continue;
      }
    }

    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_TOTAL_SIZE, (UINT32) ImageSize, 0);

    for (Offset = 0; Offset < ImageSize; Offset += VENDOR_SPECIFIC_CMD_VENDOR_CMD_DATA_MAX_BYTES) {
      if ((Progress != NULL) & (DisplayLength > 0) && ((Offset & 0xFF) == 0)) {
        ///
        /// Display current progress
        ///
        Progress (StartPercentage + ((Offset * (DisplayLength)) / ImageSize));
      }

      if ((Offset % 3200) == 0) {
        CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_WRITTEN_SO_FAR, (UINT32) (Offset * 4), 0);
      }

      WriteLength = (ImageSize - Offset) >= VENDOR_SPECIFIC_CMD_VENDOR_CMD_DATA_MAX_BYTES ? VENDOR_SPECIFIC_CMD_VENDOR_CMD_DATA_MAX_BYTES :
                    (UINT8) (ImageSize - Offset);

      Status = NvmWrite (This, TcpIndex, (UINT8 *) (BufferPointer + Offset), WriteLength);
      if (EFI_ERROR (Status)) {
        CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_NVMWRITE_FAIL, (UINT32) Status, 0);
        *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_NVM_WRITE_FAILED;
        break;
      }
    }

    if (!EFI_ERROR (Status)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_WRITE_FINISH, 0, 0);
      Status = EFI_SUCCESS;
      break;
    } else {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_GOT_ERROR_RECOVERY, 0, 0);
    }
  }

  if (RecoveryCount >= PD_BRIDGE_TOTAL_NUM_OF_RECOVERIES_DURING_IMAGE_WRITE) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_DEV_ERROR, 0, 0);
    Status = EFI_DEVICE_ERROR;
    goto UpdateImageComplete;
  }

  if (PrivateData->PdBridge.ShareFlashMode == SHARE_FLASH_MODE_ENABLE) {
    ///
    /// For the shared flash mode, STALL_NVM_ACCESS is sent to the second PD Bridge to prevent any access to NVM.
    ///
    Status = NvmStallNvmAccess (This, TcpIndex + 1);
    if (EFI_ERROR (Status)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_STALLNVM_FAIL, (UINT32) Status, 0);
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_NVM_STALL_NVM_ACCESS_FAILED;
      return Status;
    }

    Status = NvmAuthenticateWrite (This, TcpIndex);
    if (EFI_ERROR (Status)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_AUTHWRITE_FAIL, (UINT32) Status, 0);
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_NVM_AUTH_WRITE_FAILED;
      return Status;
    }

    ///
    /// For the shared flash mode, RESET command is sent to the second PD Bridge to reload the data from NVM
    ///
    Status = NvmReset (This, TcpIndex + 1);
    if (EFI_ERROR (Status)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_NVMRESET_FAIL, (UINT32) Status, 0);
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_NVM_RESET_FAILED;
      return Status;
    }
  } else {
    Status = NvmAuthenticateWrite (This, TcpIndex);
    if (EFI_ERROR (Status)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_AUTHWRITE_FAIL, (UINT32) Status, 0);
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_DEVICE_LIBRARY_PD_BRIDGE_ERROR_NVM_AUTH_WRITE_FAILED;
      return Status;
    }
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_USBC_PD_BRIDGE_NVM_FW_UPDATE_SUCCESS, 0, 0);

  Status = EFI_SUCCESS;

UpdateImageComplete:

  if (Progress != NULL) {
    Progress (EndPercentage);
  }

  return Status;
}
