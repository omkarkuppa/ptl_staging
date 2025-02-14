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
#include <Library/UsbcCapsuleDebugLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>

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
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_NVM_IECS_CALL, 0, 0);

  if (gUpdateTargetType == TARGET_RETIMER) {
    DevComRetimer = (TBT_RETIMER *)DevCom;
    // Wait for command to be completed
    do {
      CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_NVM_IECS_CHECK_REG, 0, 0);
      // Bypass the completion check for IECS_USUP_CMD,
      // because the operation kills the SBTx from the retimer side,
      // so it is most likely that we will not get response for this command
      // which still need to be considered as a success.
      if ((Cmd == TBT_IECS_CMD_USUP) && (AccessCount == 10)) {
        break;
      }
      TbtStatus = DevComRetimer->ReadIecsReg (DevComRetimer, IECS_CMD_ADDR, 1, &ReadData);
      if (TBT_STATUS_ERR (TbtStatus)) {
        CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_IECS_READ_GOT_ERROR, (UINT32) TbtStatus, 0);
        CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_IECS_READ_GOT_ERROR1, AccessCount, Cmd);
        return TBT_STATUS_NON_RECOVERABLE_ERROR;
      }
      AccessCount++;
    } while ((ReadData == Cmd) && (AccessCount < TBT_TOTAL_OF_ACCESSES_TO_CMD_UNTIL_TIMEOUT));

    if (ReadData == Cmd) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_IECS_CMD_TIMEOUT, AccessCount, ReadData);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    if (ReadData == IECS_USB4_CMD_NOTSUPPORTED) {  // !CMD (command not supported)
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_IECS_CMD_NOT_SUP, AccessCount, 0);
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_IECS_CMD_NOT_SUP1, ReadData, Cmd);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    if (ReadData == IECS_USB4_CMD_ERR) { // ERR
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_IECS_CMD_GOT_ERR, Cmd, 0);
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_IECS_CMD_GOT_ERR1, AccessCount, ReadData);
      return TBT_STATUS_RETRY;
    }
  } else {
    DevComHost = (TBT_HOST_ROUTER *)DevCom;
    // Wait for command to be completed
    do
    {
      TbtStatus = DevComHost->ReadCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_CMD_ADDR, &ReadData);

      if (TBT_STATUS_ERR (TbtStatus)) {
        CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_IECS_CMD_READ_GOT_ERR, (UINT32) TbtStatus, 0);
        CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_IECS_READ_GOT_ERROR1, AccessCount, Cmd);
        return TBT_STATUS_NON_RECOVERABLE_ERROR;
      }

      AccessCount++;
    } while ((ReadData & TBT_CMD_VALID) != 0 && AccessCount < TBT_TOTAL_OF_ACCESSES_TO_CMD_UNTIL_TIMEOUT );

    if ((ReadData & TBT_CMD_VALID) != 0) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_TBT_CMD_TIMEOUT, AccessCount, ReadData);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    if ((ReadData & TBT_CMD_NOTSUPPORTED) != 0) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_TBT_CMD_NOT_SUP, AccessCount, ReadData);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    if ((ReadData != Cmd) != 0) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_TBT_CMD_DATA_ERR, Cmd, 0);
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_TBT_CMD_DATA_ERR1, AccessCount, ReadData);
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

  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_NVM_SEND_IECS_CMD, (UINT32) Cmd, 0);
  TbtStatus = TBT_STATUS_SUCCESS;

  if (DevCom == NULL || Cmd == 0) {
    return TBT_STATUS_INVALID_PARAM;
  }

  if (gUpdateTargetType == TARGET_RETIMER) {
    DevComRetimer = (TBT_RETIMER *)DevCom;
    // Write command
    TbtStatus = DevComRetimer->WriteIecsReg (DevComRetimer, IECS_CMD_ADDR, &Cmd, 1);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_WRITE_IECS_CMD_FAIL, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
  } else {
    DevComHost = (TBT_HOST_ROUTER *)DevCom;
    Opcode     = Cmd | TBT_CMD_VALID;
    TbtStatus = DevComHost->WriteCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_CMD_ADDR, 1, &Opcode);

    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_WRITE_IECS_CMD_REG_FAIL, (UINT32) TbtStatus, 0);
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_WRITE_IECS_CMD, Cmd, 0);
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

  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_NVM_WRITE_BLK_LENGTH, (UINT32) Length, 0);
  TbtStatus = TBT_STATUS_SUCCESS;

  if (gUpdateTargetType == TARGET_RETIMER) {
    DevComRetimer = (TBT_RETIMER *)DevCom;
    // Write data
    TbtStatus = DevComRetimer->WriteIecsReg (DevComRetimer, IECS_MSG_OUT_RDATA, Data, Length);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_WRITE_BLK_WDATA_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    // Send the command
    TbtStatus = TbtNvmDrvSendCmd (DevCom, IECS_BLKW_CMD, TRUE);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_SEND_BLK_CMD_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
  } else {
    DevComHost = (TBT_HOST_ROUTER *)DevCom;
    // Write data
    TbtStatus = DevComHost->WriteCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_DATA_ADDR, Length, Data);

    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_WRITE_BLK_WDATA_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    // Send the command
    TbtStatus = TbtNvmDrvSendCmd (DevCom, OPCODE_NVM_WRITE_CMD, TRUE);

    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_SEND_BLK_OP_CMD_ERROR, (UINT32) TbtStatus, 0);
    }
  }
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_NVM_WRITE_BLK_FINISH, (UINT32) Length, (UINT32) TbtStatus);
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

  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_NVM_WRITE_OFFSET, OffsetInDW, 0);

  OffsetInB = OffsetInDW << 2;

  if (gUpdateTargetType == TARGET_RETIMER) {
    DevComRetimer = (TBT_RETIMER *)DevCom;
    // Write data
    TbtStatus = DevComRetimer->WriteIecsReg (DevComRetimer, IECS_METADATA_ADDR, &OffsetInB, 1);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_WRITE_DATA_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    // Send the command
    TbtStatus = TbtNvmDrvSendCmd (DevCom, IECS_BOPS_CMD, TRUE);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_SEND_CMD_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
  } else {
    DevComHost = (TBT_HOST_ROUTER *)DevCom;
    // Write data
    TbtStatus = DevComHost->WriteCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_METADATA_ADDR, 1, &OffsetInB); //metadata

    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_WRITE_RG_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    // Send the command
    TbtStatus = TbtNvmDrvSendCmd (DevCom, OPCODE_NVM_SETOFFSET_CMD, TRUE);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_WRITE_RG_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
  }
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_NVM_WRITE_DV_FINISH, OffsetInDW, (UINT32) TbtStatus);
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

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_SEND_AUTH_COMMAND, 0, 0);
  if (DevCom == NULL) {
    return TBT_STATUS_INVALID_PARAM;
  }

  if (gUpdateTargetType == TARGET_RETIMER) {
    DevComRetimer = (TBT_RETIMER *)DevCom;
    RetimerPtr = (RETIMER_THRU_HR *)DevComRetimer->Impl;
    WriteData = 0;
    TbtStatus = DevComRetimer->WriteIecsReg (DevComRetimer, IECS_METADATA_ADDR, &WriteData, 1); // Turn Auth on
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_IECS_DATA_GOT_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    WriteData = IECS_AUTH_CMD;
    TbtStatus = DevComRetimer->WriteIecsReg (DevComRetimer, IECS_CMD_ADDR, &WriteData, 1); // Send Auth command
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_IECS_CMD_GOT_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_WAIT_AUTH_SECONDS, (UINT32) (TBT_TIME_TO_WAIT_FOR_AUTH_US / 1000000), 0);
    gBS->Stall (TBT_TIME_TO_WAIT_FOR_AUTH_US);

    // Enumerate retimers on a path to the target retimer
    TbtStatus = SendEnumCmd (RetimerPtr);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_RT_ENUMERATE_ERROR, (UINT32) TbtStatus, 0);        
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    TbtStatus = TbtNvmDrvWaitForCmdCpl (DevCom, IECS_AUTH_CMD);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_AUTH_FAIL, (UINT32) TbtStatus, 0);   
      TbtStatus = DevComRetimer->ReadIecsReg (DevComRetimer, IECS_METADATA_ADDR, 1, &ReadData);
      if (TBT_STATUS_ERR (TbtStatus)) {
        CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_AUTH_IECS_DATA_GOT_ERROR, (UINT32) TbtStatus, 0);
        return TBT_STATUS_NON_RECOVERABLE_ERROR;
      }
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_AUTH_METADATA_FAIL, ReadData, 0);
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
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_WRITE_META_REG_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    TbtStatus = DevComHost->WriteCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_CMD_ADDR, 1, &Opcode);

    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_WRITE_CMD_REG_ERROR, (UINT32) TbtStatus, 0);
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_WRITE_IECS_CMD, Cmd, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_AUTH_WAIT_SECONDS, (UINT32) (TBT_TIME_TO_WAIT_FOR_AUTH_US / 1000000), 0);
    gBS->Stall (TBT_TIME_TO_WAIT_FOR_AUTH_US);
  }
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_AUTH_SUCCESS, 0, 0);
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

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_NVM_READ_DW_OFFSET, Offset, 0);
  if (gUpdateTargetType == TARGET_RETIMER) {
    TBT_RETIMER *DevComRetimer = (TBT_RETIMER *)DevCom;

    // Write Offset to IECS_DATA register
    TbtStatus = DevComRetimer->WriteIecsReg (DevComRetimer, IECS_METADATA_ADDR, &Offset, 1);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_WRITE_IECS_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    // Send the command
    TbtStatus = TbtNvmDrvSendCmd (DevCom, IECS_AFRR_CMD, TRUE);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_IECS_CMD_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    // Read data from MSG_OUT
    TbtStatus = DevComRetimer->ReadIecsReg (DevComRetimer, IECS_MSG_OUT_RDATA, (DataLength<16?DataLength:16), RdData);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_IECS_READ_REG_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

  } else {
    DevComHost = (TBT_HOST_ROUTER *)DevCom;
    WriteData = TBT_METADATA_VALID | Offset;
    // Write Offset to Metadata register
    TbtStatus = DevComHost->WriteCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_METADATA_ADDR, 1, &WriteData);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_IECS_WRITE_REG_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    // Send the command
    TbtStatus = TbtNvmDrvSendCmd (DevCom, OPCODE_NVM_READ_CMD, TRUE);


    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_SEND_CMD_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    // Read data from DATA register
    TbtStatus = DevComHost->ReadCioDevReg (DevComHost, DEVICE_CONFIG_SPACE, 0, ROUTER_DATA_ADDR, RdData);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_READ_DATA_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

  }
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_READ_DW_OFFSET, Offset, 0);

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

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_NVM_READ_VERSION, 0, 0);

  TbtStatus = TbtDrvReadDwFromNvm (DevCom, TBT_NVM_VERSION_OFFSET_DW << 2, 1 /* in DWs */, Version);
  if (TBT_STATUS_ERR (TbtStatus)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_NVM_READ_VERSION_FAIL, (UINT32) TbtStatus, 0);
    return EFI_DEVICE_ERROR;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_NVM_READ_VERSION_FINISH, *Version, 0);

  return EFI_SUCCESS;
}
