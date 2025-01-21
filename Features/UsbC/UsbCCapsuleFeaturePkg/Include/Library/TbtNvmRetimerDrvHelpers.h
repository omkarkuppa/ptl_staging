/** @file
  Defines the device communication interface.

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

#include <Library/TbtNvmDrvUtils.h>

/**
  Sends IECS command and waits for completion.

  @param[in] DevCom Pointer to the device interface
  @param[in] Cmd IECS CMD to send
  @param[in] ResponseRequired Is response required

  @retval TBT_STATUS_SUCCESS Command was successfully sent
  @retval TBT_STATUS_NON_RECOVERABLE_ERROR A device error has occurred.
  @retval TBT_STATUS_RETRY LC reported error, the command might be retried

**/
TBT_STATUS
TbtNvmDrvSendCmd (
  IN VOID          *DevCom,
  UINT32           Cmd,
  BOOLEAN          ResponseRequired
  );

/**
  Writes data block to the target device's NVM

  @param[in] DevCom Pointer to the device interface
  @param[in] Data to send
  @param[in] Length data length in DW

  @retval TBT_STATUS_SUCCESS Command was successfully sent
  @retval TBT_STATUS_NON_RECOVERABLE_ERROR A device error has occurred.
  @retval TBT_STATUS_RETRY LC reported error, the command might be retried
**/
TBT_STATUS
TbtNvmDrvDeviceWrBlk (
  IN VOID           *DevCom,
  UINT32            *Data,
  UINT8             Length
  );

/**
  Writes offset in the NVM, to start from it

  @param[in] DevCom Pointer to the device interface
  @param[in] OffsetInDW in NVM in DW resolution

  @retval TBT_STATUS_SUCCESS Command was successfully sent
  @retval TBT_STATUS_NON_RECOVERABLE_ERROR A device error has occurred.
  @retval TBT_STATUS_RETRY LC reported error, the command might be retried
**/
TBT_STATUS
TbtNvmDrvDeviceWrOffset (
  IN VOID   *DevCom,
  IN UINT32 OffsetInDW
  );

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
  );

/** Reads NVM version from the target device

  @param[in]  DevCom Pointer to the device interface
  @param[out] Pointer where to put the read version

  @retval EFI_INVALID_PARAMETER
  @retval EFI_DEVICE_ERROR
  @retval EFI_SUCCESS

**/
EFI_STATUS
TbtDrvReadNvmVersion (
  IN VOID          *DevCom,
  OUT UINT32       *Version
  );
