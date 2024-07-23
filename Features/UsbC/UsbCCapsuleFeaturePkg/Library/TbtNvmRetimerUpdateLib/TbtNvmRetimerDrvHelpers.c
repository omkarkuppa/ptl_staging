/** @file
  TbtNvmRetimerUpdateLib instance to support TBT Retimer firmware update

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
#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/TbtNvmDrvDef.h>
#include <Protocol/TbtNvmDrvDevice.h>
#include <Library/TbtNvmRetimerDrvHelpers.h>
#include <Library/TbtNvmDrvHr.h>
#include <Library/TbtNvmDrvRetimerThruHr.h>

extern UPDATE_TARGET_TYPE gUpdateTargetType;    // TARGET_TBT_HOST or TARGET_RETIMER

/**
  Waits for completion of IECS command by polling the IECS CMD remote register

  @param[in] DevCom Pointer to the device interface
  @param[in] Cmd IECS CMD which was send

  @retval TBT_STATUS_SUCCESS Command was successfully sent
  @retval TBT_STATUS_NON_RECOVERABLE_ERROR A device error has occured.
  @retval TBT_STATUS_RETRY LC reported error, the command might be retried
**/
TBT_STATUS
TbtNvmDrvWaitForCmdCpl (
  IN VOID          *DevCom,
  IN UINT32        Cmd
  )
{
  TBT_STATUS      TbtStatus;
  UINT32          ReadData;
  UINT32          AccessCount;
  TBT_RETIMER     *DevComRetimer;
  TBT_HOST_ROUTER *DevComHost;

  if (DevCom == NULL || Cmd == 0) {
    ASSERT (DevCom != NULL);
    ASSERT (Cmd != 0);
    return TBT_STATUS_INVALID_PARAM;
  }
  AccessCount = 0;
  DEBUG ((DEBUG_INFO, "TbtNvmDrvWaitForIecsCmdCpl: TbtNvmDrvWaitForCmdCpl() Called.\n"));

  if (gUpdateTargetType == TARGET_RETIMER) {
    DevComRetimer = (TBT_RETIMER *)DevCom;
    // Wait for command to be completed
    do {
      DEBUG ((DEBUG_INFO, "TbtNvmDrvWaitForIecsCmdCpl: Check reg for completion\n"));
      // Bypass the completion check for IECS_USUP_CMD,
      // because the operation kills the SBTx from the retimer side,
      // so it is most likely that we will not get response for this command
      // which still need to be considered as a success.
      if ((Cmd == TBT_IECS_CMD_USUP) && (AccessCount == 10)) {
        break;
      }
      TbtStatus = DevComRetimer->ReadIecsReg (DevComRetimer, IECS_CMD_ADDR, 1, &ReadData);
      if (TBT_STATUS_ERR (TbtStatus)) {
        DEBUG ((DEBUG_ERROR, "TbtNvmDrvWaitForIecsCmdCpl: Got an error while reading from IECS_CMD register, TbtStatus - %d, AccessCount = %d, Cmd = %x. Exiting...\n", TbtStatus, AccessCount, Cmd));
        return TBT_STATUS_NON_RECOVERABLE_ERROR;
      }
      AccessCount++;
    } while ((ReadData == Cmd) && (AccessCount < TBT_TOTAL_OF_ACCESSES_TO_CMD_UNTIL_TIMEOUT));

    if (ReadData == Cmd) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvWaitForIecsCmdCpl: Remote IECS CMD is timeouted. AccessCount = %d, ReadData = %x Exiting...\n", AccessCount, ReadData));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    if (ReadData == IECS_USB4_CMD_NOTSUPPORTED) {  // !CMD (command not supported)
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvWaitForIecsCmdCpl: Command not supported. AccessCount = %d, ReadData = %x, Cmd = %x\n", AccessCount, ReadData, Cmd));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    if (ReadData == IECS_USB4_CMD_ERR) { // ERR
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvWaitForIecsCmdCpl: Got error from remote LC while performing command = %x, AccessCount = %d, ReadData = %x\n", Cmd, AccessCount, ReadData));
      return TBT_STATUS_RETRY;
    }
  } else {
    DevComHost = (TBT_HOST_ROUTER *)DevCom;
    // Wait for command to be completed
    do
    {
      TbtStatus = DevComHost->ReadCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_CMD_ADDR, &ReadData);

      if (TBT_STATUS_ERR (TbtStatus)) {
        DEBUG ((DEBUG_ERROR, "TbtNvmDrvWaitForCmdCpl: Got an error while reading from CMD register, TbtStatus - %d, AccessCount = %d, Cmd = %x. Exiting...\n", TbtStatus, AccessCount, Cmd));
        return TBT_STATUS_NON_RECOVERABLE_ERROR;
      }

      AccessCount++;
    } while ((ReadData & TBT_CMD_VALID) != 0 && AccessCount < TBT_TOTAL_OF_ACCESSES_TO_CMD_UNTIL_TIMEOUT );

    if ((ReadData & TBT_CMD_VALID) != 0) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvWaitForCmdCpl: CMD is timeouted. AccessCount = %d, ReadData = %x Exiting...\n", AccessCount, ReadData));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    if ((ReadData & TBT_CMD_NOTSUPPORTED) != 0) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvWaitForCmdCpl: CMD not supported. AccessCount = %d, ReadData = %x Exiting...\n", AccessCount, ReadData));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    if ((ReadData != Cmd) != 0) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvWaitForCmdCpl: CMD 0x%x encountered an error. AccessCount = %d, ReadData = %x Exiting...\n", Cmd, AccessCount, ReadData));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
  }
  return TBT_STATUS_SUCCESS;
}

/**
  Sends IECS command and waits for completion.

  @param[in] DevCom Pointer to the device interface
  @param[in] Cmd IECS CMD to send
  @param[in] ResponseRequired Is response required

  @retval TBT_STATUS_SUCCESS Command was successfully sent
  @retval TBT_STATUS_NON_RECOVERABLE_ERROR A device error has occured.
  @retval TBT_STATUS_RETRY LC reported error, the command might be retried

**/
TBT_STATUS
TbtNvmDrvSendCmd (
  IN VOID          *DevCom,
  UINT32           Cmd,
  BOOLEAN          ResponseRequired
  )
{
  TBT_STATUS       TbtStatus;
  TBT_RETIMER      *DevComRetimer;
  TBT_HOST_ROUTER  *DevComHost;
  UINT32           Opcode;

  DEBUG ((DEBUG_VERBOSE, "Sending CMD 0x%x\n", Cmd));
  TbtStatus = TBT_STATUS_SUCCESS;

  if (DevCom == NULL || Cmd == 0) {
    return TBT_STATUS_INVALID_PARAM;
  }

  if (gUpdateTargetType == TARGET_RETIMER) {
    DevComRetimer = (TBT_RETIMER *)DevCom;
    // Write command
    TbtStatus = DevComRetimer->WriteIecsReg (DevComRetimer, IECS_CMD_ADDR, &Cmd, 1);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvSendCmd: Fail to write to IECS CMD reg, TbtStatus = %d.\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
  } else {
    DevComHost = (TBT_HOST_ROUTER *)DevCom;
    Opcode     = Cmd | TBT_CMD_VALID;
    TbtStatus = DevComHost->WriteCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_CMD_ADDR, 1, &Opcode);

    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvSendCmd: Fail to write to  CMD reg, TbtStatus = %d.\n", TbtStatus));
      DEBUG ((DEBUG_ERROR, "                  Command was 0x%x. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
  }

  if (ResponseRequired) {
    // Wait for command to be completed
    TbtStatus = TbtNvmDrvWaitForCmdCpl (DevCom, Cmd);
  }

  return TbtStatus;
}

/**
  Writes data block to the target device's NVM

  @param[in] DevCom Pointer to the device interface
  @param[in] Data to send
  @param[in] Length data length in DW

  @retval TBT_STATUS_SUCCESS Command was successfully sent
  @retval TBT_STATUS_NON_RECOVERABLE_ERROR A device error has occured.
  @retval TBT_STATUS_RETRY LC reported error, the command might be retried
**/
TBT_STATUS
TbtNvmDrvDeviceWrBlk (
  IN VOID           *DevCom,
  UINT32            *Data,
  UINT8             Length
  )
{
  TBT_STATUS       TbtStatus;
  TBT_RETIMER      *DevComRetimer;
  TBT_HOST_ROUTER  *DevComHost;

  if ((DevCom == NULL) || (Length == 0) || (Length > TBT_NVM_MAX_DWS_TO_WRITE) || (Data == NULL)) {
    ASSERT (DevCom != NULL);
    ASSERT (Length <= TBT_NVM_MAX_DWS_TO_WRITE);
    ASSERT (Length > 0);
    ASSERT (Data != NULL);
    return TBT_STATUS_INVALID_PARAM;
  }

  DEBUG ((DEBUG_VERBOSE, "Writing block with length - 0x%x\n", Length));
  TbtStatus = TBT_STATUS_SUCCESS;

  if (gUpdateTargetType == TARGET_RETIMER) {
    DevComRetimer = (TBT_RETIMER *)DevCom;
    // Write data
    TbtStatus = DevComRetimer->WriteIecsReg (DevComRetimer, IECS_MSG_OUT_RDATA, Data, Length);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceWrBlk: ERROR! The data to REMOTE_WDATA register wasn't written. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    // Send the command
    TbtStatus = TbtNvmDrvSendCmd (DevCom, IECS_BLKW_CMD, TRUE);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceWrBlk: ERROR! Sending IECS_BLKW_CMD command failed. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
  } else {
    DevComHost = (TBT_HOST_ROUTER *)DevCom;
    // Write data
    TbtStatus = DevComHost->WriteCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_DATA_ADDR, Length, Data);

    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceWrBlk: ERROR! The data to REMOTE_WDATA register wasn't written. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    // Send the command
    TbtStatus = TbtNvmDrvSendCmd (DevCom, OPCODE_NVM_WRITE_CMD, TRUE);

    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceWrBlk: ERROR! Sending OPCODE_NVM_WRITE_CMD command failed. TbtStatus - %d. Exiting...\n", TbtStatus));
    }
  }
  DEBUG ((DEBUG_VERBOSE, "Finished writing block with length - 0x%x. TbtStatus - %d\n", Length, TbtStatus));
  return TbtStatus;
}

/**
  Writes offset in the NVM, to start from it

  @param[in] DevCom Pointer to the device interface
  @param[in] OffsetInDW in NVM in DW resolution

  @retval TBT_STATUS_SUCCESS Command was successfully sent
  @retval TBT_STATUS_NON_RECOVERABLE_ERROR A device error has occured.
  @retval TBT_STATUS_RETRY LC reported error, the command might be retried
**/
TBT_STATUS
TbtNvmDrvDeviceWrOffset (
  IN VOID   *DevCom,
  IN UINT32 OffsetInDW
  )
{
  TBT_STATUS         TbtStatus;
  UINT32             OffsetInB;
  TBT_RETIMER        *DevComRetimer;
  TBT_HOST_ROUTER    *DevComHost;

  ASSERT (DevCom != NULL);
  if (DevCom == NULL) {
    return TBT_STATUS_INVALID_PARAM;
  }

  DEBUG ((DEBUG_VERBOSE, "Write device OffsetInDW - 0x%x\n", OffsetInDW));

  OffsetInB = OffsetInDW << 2;

  if (gUpdateTargetType == TARGET_RETIMER) {
    DevComRetimer = (TBT_RETIMER *)DevCom;
    // Write data
    TbtStatus = DevComRetimer->WriteIecsReg (DevComRetimer, IECS_METADATA_ADDR, &OffsetInB, 1);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceWrOffset: ERROR! Writing data to IECS_METADATA_ADDR register failed. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    // Send the command
    TbtStatus = TbtNvmDrvSendCmd (DevCom, IECS_BOPS_CMD, TRUE);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceWrOffset: ERROR! Sending command IECS_BOPS_CMD failed. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
  } else {
    DevComHost = (TBT_HOST_ROUTER *)DevCom;
    // Write data
    TbtStatus = DevComHost->WriteCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_METADATA_ADDR, 1, &OffsetInB); //metadata

    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceWrOffset: ERROR! The data to Metadata register wasn't written. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    // Send the command
    TbtStatus = TbtNvmDrvSendCmd (DevCom, OPCODE_NVM_SETOFFSET_CMD, TRUE);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceWrOffset: ERROR! The data to Metadata register wasn't written. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
  }
  DEBUG ((DEBUG_VERBOSE, "Finished writing device OffsetInDW - 0x%x (%d)\n", OffsetInDW, TbtStatus));
  return TbtStatus;
}

/**
  Initiate Auth at the device, wait for it to complete and check the status

  @param[in] DevCom Pointer to the device interface

  @retval TBT_STATUS_SUCCESS If auth passed
  @retval TBT_STATUS_TIMEOUT If device is not responding
  @retval TBT_STATUS_NON_RECOVERABLE_ERROR On any other communication error
  @retval TBT_STATUS_GENERIC_ERR If auth is failed

**/
TBT_STATUS
TbtNvmDrvDeviceExecAuth (
  IN VOID *DevCom
  )
{
  UINT32            ReadData;
  TBT_STATUS        TbtStatus;
  UINT32            WriteData;
  TBT_RETIMER       *DevComRetimer;
  RETIMER_THRU_HR   *RetimerPtr;
  TBT_HOST_ROUTER   *DevComHost;
  UINT32            Opcode;
  UINT32            Cmd;

  DEBUG ((DEBUG_INFO, "Sending Auth command\n"));
  if (DevCom == NULL) {
    return TBT_STATUS_INVALID_PARAM;
  }

  if (gUpdateTargetType == TARGET_RETIMER) {
    DevComRetimer = (TBT_RETIMER *)DevCom;
    RetimerPtr = (RETIMER_THRU_HR *)DevComRetimer->Impl;
    WriteData = 0;
    TbtStatus = DevComRetimer->WriteIecsReg (DevComRetimer, IECS_METADATA_ADDR, &WriteData, 1); // Turn Auth on
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceExecAuth: Got an error while writing to IECS_DATA register, TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    WriteData = IECS_AUTH_CMD;
    TbtStatus = DevComRetimer->WriteIecsReg (DevComRetimer, IECS_CMD_ADDR, &WriteData, 1); // Send Auth command
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceExecAuth: Got an error while writing to IECS_CMD register, TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    DEBUG ((DEBUG_INFO,
            "Waiting %d seconds for authentication to complete...\n",
            TBT_TIME_TO_WAIT_FOR_AUTH_US / 1000000));
    gBS->Stall (TBT_TIME_TO_WAIT_FOR_AUTH_US);

    // Enumerate retimers on a path to the target retimer
    TbtStatus = SendEnumCmd (RetimerPtr);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceExecAuth: The retimer could not enumerate, TbtStatus %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    TbtStatus = TbtNvmDrvWaitForCmdCpl (DevCom, IECS_AUTH_CMD);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR,
              "TbtNvmDrvDeviceExecAuth: Authentication failed!!! TbtStatus from LC - %d. Exiting...\n",
              TbtStatus));

      TbtStatus = DevComRetimer->ReadIecsReg (DevComRetimer, IECS_METADATA_ADDR, 1, &ReadData);
      if (TBT_STATUS_ERR (TbtStatus)) {
        DEBUG ((DEBUG_ERROR,
                "TbtNvmDrvDeviceExecAuth: Got an error while reading from IECS_DATA register, TbtStatus - %d. Exiting...\n",
                TbtStatus));
        return TBT_STATUS_NON_RECOVERABLE_ERROR;
      }
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceExecAuth: Auth has failed with metadata = %x. Exiting...\n", ReadData));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
  } else {
    DevComHost = (TBT_HOST_ROUTER *)DevCom;
    Cmd = OPCODE_NVM_AUTH_CMD;
    Opcode = Cmd | TBT_CMD_VALID;
    WriteData = 0x0;

    // Write command
    TbtStatus = DevComHost->WriteCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_METADATA_ADDR, 1, &WriteData); //turn auth on

    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceExecAuth: ERROR! The data to Metadata register wasn't written. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    TbtStatus = DevComHost->WriteCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_CMD_ADDR, 1, &Opcode);

    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDeviceExecAuth: Fail to write to  CMD reg, TbtStatus = %d.\n", TbtStatus));
      DEBUG ((DEBUG_ERROR, "                  Command was 0x%x. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    DEBUG ((DEBUG_INFO, "Waiting %d seconds for authentication to complete...\n", TBT_TIME_TO_WAIT_FOR_AUTH_US / 1000000));
    gBS->Stall (TBT_TIME_TO_WAIT_FOR_AUTH_US);
  }
  DEBUG ((DEBUG_INFO, "TbtNvmDrvDeviceExecAuth: Authentication success!\n"));
  return TBT_STATUS_SUCCESS;
}

/** Reading data from flash memory.

  @param[in]   DevCom     Pointer to the device interface
  @param[in]   Offset     TBT NVM offset
  @param[in]   DataLength How many DWORDS we want to read
  @param[out]  RdData     Pointer to read data

  @retval EFI_INVALID_PARAMETER
  @retval TBT_STATUS_NON_RECOVERABLE_ERROR
  @retval EFI_SUCCESS

**/
TBT_STATUS
TbtDrvReadDwFromNvm (
  IN  VOID          *DevCom,
  IN  UINT32        Offset,
  IN  UINT32        DataLength,
  OUT UINT32        *RdData
  )
{
  TBT_STATUS      TbtStatus;
  TBT_HOST_ROUTER *DevComHost;
  UINT32          WriteData;

  if (DevCom == NULL) {
    ASSERT (DevCom != NULL);
    return TBT_STATUS_INVALID_PARAM;
  }

  DEBUG ((DEBUG_INFO, "Reading DW from NVM, offset 0x%x\n", Offset));
  if (gUpdateTargetType == TARGET_RETIMER) {
    TBT_RETIMER *DevComRetimer = (TBT_RETIMER *)DevCom;

    // Write Offset to IECS_DATA register
    TbtStatus = DevComRetimer->WriteIecsReg (DevComRetimer, IECS_METADATA_ADDR, &Offset, 1);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtDrvReadDwFromNvm: ERROR! Writing data to IECS_METADATA_ADDR register failed. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    // Send the command
    TbtStatus = TbtNvmDrvSendCmd (DevCom, IECS_AFRR_CMD, TRUE);
    if (TBT_STATUS_ERR (TbtStatus)) {
     DEBUG ((DEBUG_ERROR, "TbtDrvReadDwFromNvm: ERROR! Sending command IECS_AFRR_CMD failed. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    // Read data from MSG_OUT
    TbtStatus = DevComRetimer->ReadIecsReg (DevComRetimer, IECS_MSG_OUT_RDATA, (DataLength<16?DataLength:16), RdData);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtDrvReadDwFromNvm: ERROR! Failed to read data from IECS_DATA register. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

  } else {
    DevComHost = (TBT_HOST_ROUTER *)DevCom;
    WriteData = TBT_METADATA_VALID | Offset;
    // Write Offset to Metadata register
    TbtStatus = DevComHost->WriteCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_METADATA_ADDR, 1, &WriteData);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtDrvReadDwFromNvm: ERROR! The data to DATA register wasn't written. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    // Send the command
    TbtStatus = TbtNvmDrvSendCmd (DevCom, OPCODE_NVM_READ_CMD, TRUE);


    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtDrvReadDwFromNvm: ERROR! The command wasn't send. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    // Read data from DATA register
    TbtStatus = DevComHost->ReadCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_DATA_ADDR, RdData);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtDrvReadDwFromNvm: ERROR! The read from RDATA has failed. TbtStatus - %d. Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

  }
  DEBUG ((DEBUG_INFO, "Finished reading DW from NVM offset - 0x%x\n", Offset));

  return TBT_STATUS_SUCCESS;
}


/** Reads NVM version from the target device

  @param[in]  DevCom Pointer to the device interface
  @param[out] Pointer where to put the read version

  @retval EFI_INVALID_PARAMETER
  @retval EFI_DEVICE_ERROR
  @retval EFI_SUCCESS

**/
EFI_STATUS
TbtDrvReadNvmVersion (
  IN  VOID         *DevCom,
  OUT UINT32       *Version
  )
{
  TBT_STATUS       TbtStatus;

  ASSERT (DevCom != NULL);
  if (DevCom == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "Reading nvm version\n"));

  TbtStatus = TbtDrvReadDwFromNvm (DevCom, TBT_NVM_VERSION_OFFSET_DW << 2, 1 /* in DWs */, Version);
  if (TBT_STATUS_ERR (TbtStatus)) {
    DEBUG ((DEBUG_ERROR, "TbtDrvReadNvmVersion: ERROR! Couldn't read from NVM. TbtStatus %d\n", TbtStatus));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Finished reading NVM version - 0x%x\n", *Version));

  return EFI_SUCCESS;
}
