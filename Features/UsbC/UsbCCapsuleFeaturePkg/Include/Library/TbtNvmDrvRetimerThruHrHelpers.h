/** @file
  Declares the interface for TbtNvmDrvRetimerThruHr class.
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

#ifndef TBT_NVM_DRV_RETIMER_THRU_HR_HELPERS_
#define TBT_NVM_DRV_RETIMER_THRU_HR_HELPERS_

#include <Library/TbtNvmDrvUtils.h>
#include <Protocol/TbtNvmDrvRetimerThruHrProtocol.h>

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
  );

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
  );

/**
  Send enumeration command to local LC
  Need to repeate it for a number of times.
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

  @retval status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS - indicate operation success

**/
TBT_STATUS
SendEnumCmd (
  RETIMER_THRU_HR *RetimerPtr
  );

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
  );

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
  );

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
  );

#endif /* TBT_NVM_DRV_RETIMER_THRU_HR_HELPERS_ */
