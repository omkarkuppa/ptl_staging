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
    DEBUG ((DEBUG_INFO, "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: Read data = 0x%x\n", Data));
    if (TBT_STATUS_ERR (TbtStatus)) {
       DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: ERROR! Reading register MSG_OUT_CMD is failed. \
                             TbtStatus is %d. d=%dExiting...\n", TbtStatus, DbgData));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    DEBUG ((DEBUG_INFO, "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: IECS transaction read data=%x\n", Data));
    gBS->Stall (TBT_WAIT_TIME_BEFORE_NEXT_MSG_OUT_ACCESS);
    AccessCnt++;
  } while ((Data & TBT_MSG_OUT_CMD_VALID) != 0 && AccessCnt < TBT_TOTAL_ACCESSES_WHILE_WAIT_FOR_MSG_OUT);

  if ((Data & TBT_MSG_OUT_CMD_VALID) != 0) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: ERROR! Local LC seems to be stuck. \
                          d=%d, Data=%x, m=%x AccessCnt=%d Exiting...\n", DbgData, Data, MsgOutCmdOffset, AccessCnt));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  if ((Data & TBT_MSG_OUT_TIMEOUT) != 0) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: IECS transaction was timeouted \
                          d=%d, Data=%x, m=%x AccessCnt=%d\n", DbgData, Data, MsgOutCmdOffset, AccessCnt));
  } else if ((Data & TBT_MSG_OUT_INVALID) != 0) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: IECS transaction was invalid \
                          d=%d, Data=%x, m=%x AccessCnt=%d\n", DbgData, Data, MsgOutCmdOffset, AccessCnt));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  } else {
    return TBT_STATUS_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: TBT_STATUS_RETRY, d=%d\n", DbgData));
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
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: \
                            ERROR when writing 0x%x data to local LC, TbtStatus - %d. \
                            Exiting...\n", DataPtr, TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    DEBUG ((DEBUG_INFO, "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: Write USB4 port cap = %x\n", USB4CapRegWrite));
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
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: \
                           ERROR when writing 0x%x data to local LC, TbtStatus - %d. \
                           Exiting...\n", DataPtr, TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    TbtStatus = WaitForMsgOutTxDone (RetimerPtr, TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1, 2);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: \
                            ERROR when writing 0x%x data to local LC, TbtStatus - %d. \
                            Exiting...\n", DataPtr, TbtStatus));
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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: \
                          ERROR when writing 0x%x cmd to local LC, TbtStatus - %d.\
                          Exiting...\n", Cmd, TbtStatus));
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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: \
                          ERROR when writing 0x%x cmd to local LC, TbtStatus - %d.\
                          Exiting...\n", Cmd, TbtStatus));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  TbtStatus = WaitForMsgOutTxDone (RetimerPtr, TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1, 2);
  if (TBT_STATUS_ERR (TbtStatus)) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: \
                          ERROR when writing 0x%x data to local LC, TbtStatus - %d. \
                          Exiting...\n", DataPtr, TbtStatus  ));
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
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: \
                            ERROR when writing 0x%x data to local LC, TbtStatus - %d. \
                            Exiting...\n", DataPtr, TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    TbtStatus = WaitForMsgOutTxDone (RetimerPtr, TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1, 2);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: \
                            ERROR when writing 0x%x data to local LC, TbtStatus - %d. \
                            Exiting...\n", DataPtr, TbtStatus));
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
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: \
                            ERROR when waiting for command 0x%x to complete, \
                            TbtStatus - %d. Exiting...\n", Cmd, TbtStatus));

      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    AccessCnt++;
  } while (USB4CapRegRead == Cmd && AccessCnt < TBT_TOTAL_ACCESSES_WHILE_WAIT_FOR_IECS);

  if (USB4CapRegRead == Cmd) {  // Timeouted
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: \
                          Local LC Couldn't perform 0x%x command - timeouted. \
                          Exiting...\n", Cmd));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  } else if (USB4CapRegRead != TBT_LC_CMD_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: \
                          Local LC reported error while performing 0x%x command - got error: 0x%x. \
                          Exiting...\n", Cmd, USB4CapRegRead));
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

  DEBUG ((DEBUG_INFO, "\nTbtNvmDrvRetimerThruHr::Sending ENUM to adapter %s of the HR\n",
    RetimerPtr->TbtPort == FIRST_MASTER_LANE ? "A" : "B"));

  for (Index = 0; Index < TBT_TOTAL_ENUM_ACCESSES; Index++) {
    // Doing this 4 times because there is no indication whether the operation succeeded or not
    Status = SendCommandToLocalLc (RetimerPtr, ADAPTER_CONFIG_SPACE, TBT_IECS_CMD_ENUM, NULL);
    gBS->Stall (TBT_WAIT_TIME_BETWEEN_ENUM_ACCESSES);
  }
  if (TBT_STATUS_ERR (Status)) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::Enum wasn't sent successfully, got error...\n"));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "TbtNvmDrvRetimerThruHr::Enum was sent successfully!\n"));

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

  DEBUG ((DEBUG_INFO, "\nTbtNvmDrvRetimerThruHr::Sending USUP to adapter %s of the HR, retimer index - %d.\
    The required operation is disable\n",
    RetimerPtr->TbtPort == FIRST_MASTER_LANE ? "A" : "B", (UINT32)RetimerPtr->CascadedRetimerIndex));

  Status = TbtNvmDrvSendCmd ((VOID *)RetimerPtr->Comm, TBT_IECS_CMD_USUP, FALSE);
  if (TBT_STATUS_ERR (Status)) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::USUP send is failed!\n"));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "TbtNvmDrvRetimerThruHr::USUP was sent successfully!\n"));

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

  DEBUG ((DEBUG_INFO, "\nTbtNvmDrvRetimerThruHr::Sending LSUP to adapter %s of the HR, retimer index - %d. \
         The required operation is enable\n",
    RetimerPtr->TbtPort == FIRST_MASTER_LANE ? "A" : "B", (UINT32)RetimerPtr->CascadedRetimerIndex));

  for (Index = 0; Index < TBT_TOTAL_LSUP_ACCESSES; Index++) {
    // Doing this 3 times because there might be no indication
    // whether the operation succeeded or not on the first or second executions
    Status = TbtNvmDrvSendCmd ((VOID *)RetimerPtr->Comm, TBT_IECS_CMD_LSUP, TRUE);
    if (Status == TBT_STATUS_SUCCESS) {
      DEBUG ((DEBUG_INFO, "TbtNvmDrvRetimerThruHr::LSUP command send had succeeded.\n"));
      goto Exit;
    }
    gBS->Stall (TBT_WAIT_TIME_BEFORE_NEXT_IECS_ACCESS);
  }
  if (TBT_STATUS_ERR (Status)) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::LSUP command send had failed\n"));
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

  DEBUG ((DEBUG_INFO, "\nTbtNvmDrvRetimerThruHr::Sending OfflineMode(%s) to adapter %s of the HR, retimer index - %d. \
        The required operation is enable\n",
  Data == OFFLINE_MODE_ENTRY ? L"0" : L"1",
  HrDevice->TbtPort == FIRST_MASTER_LANE ? "A" : "B",
  (UINT32)HrDevice->CascadedRetimerIndex));

  // Send LS Enable/Disable, to prevent LC to bring up the link
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_LSEN_CMD, (UINT32) Data, 0);
  Status = SendCommandToLocalLc (HrDevice, ADAPTER_CONFIG_SPACE, TBT_IECS_CMD_LSEN, &Data);
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_LSEN_CMD_FAIL, (UINT32) Data, (UINT32) Status);
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHrCtor: The retimer could not perform LS %s, \
                          Status %d. Exiting...\n", (Data == 1)? L"Enable" : L"Disable", Status));
  }
  return Status;
}
