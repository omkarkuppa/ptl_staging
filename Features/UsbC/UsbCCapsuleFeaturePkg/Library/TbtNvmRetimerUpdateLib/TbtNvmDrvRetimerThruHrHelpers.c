/** @file
  Implements the interface for TbtNvmDrvRetimerThruHr class.
  This class is in charge of providing the way to access the retimer
  through TBT integrated HR.

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

#include <Protocol/TbtNvmDrvDef.h>
#include <Library/TbtNvmDrvRetimerThruHrHelpers.h>
#include <Library/TbtNvmRetimerDrvHelpers.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UsbcCapsuleDebugLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>

extern UINT16 TBT_USB4_PORT_CAPABILITY_OFFSET;

/**
  Wait for command to be completed
  Operation:
    Poll on MSG_OUT_CMD register to see when local LC completes the sending of a command to retimer

  @param[in] RetimerPtr        Pointer to the current implementation of TBT_RETIMER interface.
  @param[in] MsgOutCmdOffset   The MSG_OUT_CMD address in target space, different for each CIO port group
  @param[in] DbgData           Point to the caller of the function.

  @retval Status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_RETRY - the caller might retry
    TBT_STATUS_SUCCESS - indicate operation success
**/
TBT_STATUS
WaitForMsgOutTxDone (
  IN RETIMER_THRU_HR     *RetimerPtr,
  IN UINT16              MsgOutCmdOffset,
  IN UINT8               DbgData
  )
{
  TBT_STATUS  TbtStatus;
  UINT32      Data;
  UINT32      AccessCnt;

  AccessCnt = 0;
  do {
    TbtStatus = RetimerPtr->Hr->ReadCioDevReg (
                               RetimerPtr->Hr,
                               ADAPTER_CONFIG_SPACE,
                               (UINT8) RetimerPtr->TbtPort,
                               MsgOutCmdOffset,
                               &Data
                               );
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_WAIT_CMD_READ_DATA, Data, 0);

    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WAIT_CMD_READ_REG_FAIL, (UINT32) TbtStatus, (UINT32) DbgData);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_WAIT_CMD_READ_TRAN_DATA, Data, 0);
    gBS->Stall (TBT_WAIT_TIME_BEFORE_NEXT_MSG_OUT_ACCESS);
    AccessCnt++;
  } while ((Data & TBT_MSG_OUT_CMD_VALID) != 0 && AccessCnt < TBT_TOTAL_ACCESSES_WHILE_WAIT_FOR_MSG_OUT);

  if ((Data & TBT_MSG_OUT_CMD_VALID) != 0) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WAIT_CMD_LC_STUCK_ERROR, 0, 0);
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WAIT_CMD_LC_DATA, (UINT32) DbgData, Data);
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WAIT_CMD_LC_DATA2, (UINT32) MsgOutCmdOffset, AccessCnt);

    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  if ((Data & TBT_MSG_OUT_TIMEOUT) != 0) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WAIT_CMD_LC_TIMEOUT, 0, 0);
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WAIT_CMD_LC_DATA, (UINT32) DbgData, Data);
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WAIT_CMD_LC_DATA2, (UINT32) MsgOutCmdOffset, AccessCnt);
  } else if ((Data & TBT_MSG_OUT_INVALID) != 0) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WAIT_CMD_LC_INVALID, 0, 0);
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WAIT_CMD_LC_DATA, (UINT32) DbgData, Data);
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WAIT_CMD_LC_DATA2, (UINT32) MsgOutCmdOffset, AccessCnt);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  } else {
    return TBT_STATUS_SUCCESS;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_WAIT_CMD_RETRY_STATUS, (UINT32) DbgData, 0);
  return TBT_STATUS_RETRY;
}

/**
  Send command to the local LC
  Operation:
    If there is data to write it writes it to the LC_SW_FW_MAILBOX_DATA_IN1 register.
    Writes the command to the LC_SW_FW_MAILBOX_DATA_IN0
    Issues command indication to LC in TBT_LC_SW_FW_MAILBOX_IN
    Wait for command to be cleared in LC_SW_FW_MAILBOX_DATA_IN0
    Check the Status and return


  @param RetimerPtr         Pointer to the current implementation of TBT_RETIMER interface
  @param ConfigurationSpace TBT device config space.
  @param Cmd                The command to local LC
  @param DataPtr            Pointer to a buffer where the write data

  @retval Status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS - indicate operation success
**/
TBT_STATUS
SendCommandToLocalLc (
  IN  RETIMER_THRU_HR *RetimerPtr,
  IN  UINT8           ConfigurationSpace,
  IN  UINT32          Cmd,
  OUT UINT32          *DataPtr OPTIONAL
  )
{
  TBT_STATUS   TbtStatus;
  UINT32       AccessCnt;
  UINT32       USB4CapRegWrite;
  UINT32       USB4CapRegRead;

  USB4CapRegWrite = 0;
  USB4CapRegRead  = 0;
  if (DataPtr != NULL) {
    TbtStatus = RetimerPtr->Hr->WriteCioDevReg (
                                  RetimerPtr->Hr,
                                  ConfigurationSpace,
                                  RetimerPtr->TbtPort,
                                  TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
                                  1,
                                  DataPtr
                                  );
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_LOCAL_LC_WRITE_ERROR, *DataPtr, (UINT32) TbtStatus);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_LOCAL_LC_WRITE_USB4_CAP, USB4CapRegWrite, 0);
    USB4CapRegWrite = ((UINT32)1 << TBT_USB4_PORT_CAPABILITY_PND_OFFSET)
                    | ((UINT32)1 << TBT_USB4_PORT_CAPABILITY_WnR_OFFSET)
                    | ((UINT32)0 << TBT_USB4_PORT_CAPABILITY_TARGET_OFFSET)
                    | ((UINT32)4 << TBT_USB4_PORT_CAPABILITY_LENGTH_OFFSET)
                    | IECS_METADATA_ADDR;

    TbtStatus = RetimerPtr->Hr->WriteCioDevReg (
                                  RetimerPtr->Hr,
                                  ConfigurationSpace,
                                  RetimerPtr->TbtPort,
                                  TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
                                  1,
                                  &USB4CapRegWrite
                                  );
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_LOCAL_LC_WRITE_ERROR, *DataPtr, (UINT32) TbtStatus);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    TbtStatus = WaitForMsgOutTxDone (RetimerPtr, TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1, 2);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_LOCAL_LC_WRITE_ERROR, *DataPtr, (UINT32) TbtStatus);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
  }

  TbtStatus = RetimerPtr->Hr->WriteCioDevReg (
                                RetimerPtr->Hr,
                                ConfigurationSpace,
                                RetimerPtr->TbtPort,
                                TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
                                1,
                                &Cmd
                                );
  if (TBT_STATUS_ERR (TbtStatus)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_LOCAL_LC_WRITE_CMD_ERROR, Cmd, (UINT32) TbtStatus);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  USB4CapRegWrite = ((UINT32)1 << TBT_USB4_PORT_CAPABILITY_PND_OFFSET)
                  | ((UINT32)1 << TBT_USB4_PORT_CAPABILITY_WnR_OFFSET)
                  | ((UINT32)4 << TBT_USB4_PORT_CAPABILITY_LENGTH_OFFSET)
                  | IECS_CMD_ADDR;

  TbtStatus = RetimerPtr->Hr->WriteCioDevReg (
                                RetimerPtr->Hr,
                                ConfigurationSpace,
                                RetimerPtr->TbtPort,
                                TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
                                1,
                                &USB4CapRegWrite
                                );
  if (TBT_STATUS_ERR (TbtStatus)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_LOCAL_LC_WRITE_CMD_ERROR, Cmd, (UINT32) TbtStatus);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  TbtStatus = WaitForMsgOutTxDone (RetimerPtr, TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1, 2);
  if (TBT_STATUS_ERR (TbtStatus) && DataPtr != NULL) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_LOCAL_LC_WRITE_ERROR, *DataPtr, (UINT32) TbtStatus);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  // TODO improve this with timer
  AccessCnt = 0;
  do {
    gBS->Stall (TBT_WAIT_TIME_BEFORE_NEXT_IECS_ACCESS);
    USB4CapRegWrite = ((UINT32)1 << TBT_USB4_PORT_CAPABILITY_PND_OFFSET)
                    | ((UINT32)4 << TBT_USB4_PORT_CAPABILITY_LENGTH_OFFSET)
                    | IECS_CMD_ADDR;

    TbtStatus = RetimerPtr->Hr->WriteCioDevReg (
                                  RetimerPtr->Hr,
                                  ConfigurationSpace,
                                  RetimerPtr->TbtPort,
                                  TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
                                  1,
                                  &USB4CapRegWrite
                                  );
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_LOCAL_LC_WRITE_ERROR, *DataPtr, (UINT32) TbtStatus);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    TbtStatus = WaitForMsgOutTxDone (RetimerPtr, TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1, 2);
    if (TBT_STATUS_ERR (TbtStatus) && DataPtr != NULL) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_LOCAL_LC_WRITE_ERROR, *DataPtr, (UINT32) TbtStatus);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    TbtStatus = RetimerPtr->Hr->ReadCioDevReg (
                                  RetimerPtr->Hr,
                                  ConfigurationSpace,
                                  RetimerPtr->TbtPort,
                                  TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
                                  &USB4CapRegRead
                                  );
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_LOCAL_LC_WAIT_CMD_COMPLETE_ERROR, Cmd, (UINT32) TbtStatus);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    AccessCnt++;
  } while (USB4CapRegRead == Cmd && AccessCnt < TBT_TOTAL_ACCESSES_WHILE_WAIT_FOR_IECS);

  if (USB4CapRegRead == Cmd) {  // Timeouted
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_LOCAL_LC_CMD_TIMEOUT, Cmd, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  } else if (USB4CapRegRead != TBT_LC_CMD_SUCCESS) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_LOCAL_LC_REPORTED_ERROR, Cmd, USB4CapRegRead);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  return TBT_STATUS_SUCCESS;
}

/**
  Send enumeration command to local LC
  Need to repeat it for a number of times.
  Write to SW_FW_MAILBOX_IN0 corresponding to the port number the enum command.
  Then trigger cmd valid in SW_FW_MAILBOX_IN[0]

  Poll on the SW_FW_MAILBOX_IN0 register:
    if !ENUM -> Success
    if timeout exit
  Read SW_FW_MAILBOX_OUT0 reg
    if 0x1 error -> exit
    if 0x2 success
    else error -> exit

  @param[in] RetimerPtr pointer to the current implementation of TBT_RETIMER interface

  @retval Status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS - indicate operation success
**/
TBT_STATUS
SendEnumCmd (
  IN RETIMER_THRU_HR *RetimerPtr
  )
{
  TBT_STATUS Status;
  UINTN      Index;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_SEND_ENUM_THRU_CMD, (UINT32) (RetimerPtr->TbtPort == FIRST_MASTER_LANE ? 0xA : 0xB), 0);

  for (Index = 0; Index < TBT_TOTAL_ENUM_ACCESSES; Index++) {
    // Doing this 4 times because there is no indication whether the operation succeeded or not
    Status = SendCommandToLocalLc (RetimerPtr, ADAPTER_CONFIG_SPACE, TBT_IECS_CMD_ENUM, NULL);
    gBS->Stall (TBT_WAIT_TIME_BETWEEN_ENUM_ACCESSES);
  }
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_SEND_ENUM_THRU_CMD_FAIL, 0, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_SEND_ENUM_THRU_CMD_SUCCESS, 0, 0);

  return TBT_STATUS_SUCCESS;
}

/**
  Send LSUP disable command (USUP) to the target retimer

  @param[in] RetimerPtr Pointer to the current implementation of TBT_RETIMER interface

  @retval Status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS - indicate operation success
**/
TBT_STATUS
SendLsupCmdDis (
  IN RETIMER_THRU_HR *RetimerPtr
  )
{
  TBT_STATUS Status;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_SEND_USUP_RETIMER_INDEX, (UINT32) (RetimerPtr->TbtPort == FIRST_MASTER_LANE ? 0xA : 0xB), RetimerPtr->CascadedRetimerIndex);

  Status = TbtNvmDrvSendCmd ((VOID *)RetimerPtr->Comm, TBT_IECS_CMD_USUP, FALSE);
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_SEND_USUP_FAIL, 0, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_SEND_USUP_SUCCESS, 0, 0);

  return TBT_STATUS_SUCCESS;
}

/**
  Send LSUP enable command to the target retimer

  @param[in] RetimerPtr pointer to the current implementation of TBT_RETIMER interface

  @retval Status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS - indicate operation success
**/
TBT_STATUS
SendLsupCmdEn (
  IN RETIMER_THRU_HR *RetimerPtr
  )
{
  TBT_STATUS Status;
  UINTN      Index;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_SEND_LSUP_RETIMER_INDEX, (UINT32) (RetimerPtr->TbtPort == FIRST_MASTER_LANE ? 0xA : 0xB), RetimerPtr->CascadedRetimerIndex);

  for (Index = 0; Index < TBT_TOTAL_LSUP_ACCESSES; Index++) {
    // Doing this 3 times because there might be no indication
    // whether the operation succeeded or not on the first or second executions
    Status = TbtNvmDrvSendCmd ((VOID *)RetimerPtr->Comm, TBT_IECS_CMD_LSUP, TRUE);
    if (Status == TBT_STATUS_SUCCESS) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_SEND_LSUP_SUCCESS, 0, 0);
      goto Exit;
    }
    gBS->Stall (TBT_WAIT_TIME_BEFORE_NEXT_IECS_ACCESS);
  }
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_SEND_LSUP_FAIL, 0, 0);
    goto Exit;
  }

  Exit:
  return Status;
}

/**
  Send OFFLINE_MODE to the specified HR and specified port

  @param[in]  HrDevice  Pointer to to the current implementation of Host Router device - RETIMER_THRU_HR.
  @param[in]  Data      0 - OFFLINE_MODE_ENTRY
                        1 - OFFLINE_MODE_EXIT

  @retval EFI_SUCCESS   indicate operation success
  @retval Others        Fail to send RETIMER_OFFLINE_MODE to the specified port on the HR.
**/
EFI_STATUS
EFIAPI
SendOfflineMode (
  IN  RETIMER_THRU_HR   *HrDevice,
  IN  UINT32            Data
  )
{
  TBT_STATUS       Status;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_SEND_LSEN_CMD, (UINT32) (Data == OFFLINE_MODE_ENTRY ? 0x0 : 0x1), (UINT32) (HrDevice->TbtPort == FIRST_MASTER_LANE ? 0xA : 0xB));
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_SEND_LSEN_CMD1, HrDevice->CascadedRetimerIndex, 0);

  // Send LS Enable/Disable, to prevent LC to bring up the link
  Status = SendCommandToLocalLc (HrDevice, ADAPTER_CONFIG_SPACE, TBT_IECS_CMD_LSEN, &Data);
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_SEND_LSEN_CMD_FAIL, (UINT32) (Data == 1 ? 0x1 : 0x0), (UINT32) Status);
  }
  return Status;
}
