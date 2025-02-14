/** @file
  BaseEcTcss library implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Library/EcLib.h>
#include <Library/EcTcssLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/EcPrivateLib.h>
#include <Library/PcdLib.h>
#include <EcCommands.h>
#include <EcCommon.h>
#include <EcPdCommands.h>
#include <EcPdRamOffset.h>

/**
  USBC EC Timeout Retry Message. Used to send messages to EC with predefined
  timeout and retry times.

  @param[in, out]   DataBuffer
  @param[in]        Command
  @param[in]        Timeout
  @param[in]        RetryTime

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_TIMEOUT           Command timeout
  @retval           EFI_DEVICE_ERROR      Command error

**/
EFI_STATUS
UsbcEcTimeoutRetryMessage (
  IN OUT UINT8      *DataBuffer,
  IN     UINT8      Command,
  IN     UINT32     Timeout,
  IN     UINT32     RetryTime
  )
{
  UINT8             DataSize;
  UINT8             DataBufferIn;
  UINT32            NumRetries;
  EFI_STATUS        Status;

  DataSize = 1;
  NumRetries = Timeout / RetryTime;
  DataBufferIn = *DataBuffer;

  DEBUG ((DEBUG_INFO, "\nUsbcEcTimeoutRetryMessage: %x, Timeout: %u, RetryTime: %u\n", Command, Timeout, RetryTime));
  do {
    //
    // Send the command to EC
    //
    Status = EcInterface (EcId0Ch0, Command, &DataSize, DataBuffer);
    if (Status != EFI_SUCCESS) {
      return Status;
    }

    //
    // If USB connection status is successful (0x00) or failed (0x02) then return
    // If in progress (0x01) then retry after 10 ms
    //
    if (*DataBuffer == 0x00 || *DataBuffer == 0x02) {
      return EFI_SUCCESS;
    } else if (*DataBuffer == 0x01) {
      *DataBuffer = DataBufferIn;
      MicroSecondDelay(RetryTime * 1000);
      NumRetries--;
    } else {
      return EFI_DEVICE_ERROR;
    }
  } while (NumRetries != 0);

  DEBUG ((DEBUG_INFO, "\nUsbcEcTimeoutRetryMessage %x timed out\n", Command));
  return EFI_TIMEOUT;
}

/**
  USBC IOM Ready Notify

  @param[in, out]   DataBuffer

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_DEVICE_ERROR      Command error

**/
EFI_STATUS
EFIAPI
UsbcIomReadyNotify (
  IN OUT UINT8      *DataBuffer
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Usbc Retimer Compliance Mode

  @param[in, out]   Data                  Recive 1 byte data from EC.

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_DEVICE_ERROR      Command error

**/
EFI_STATUS
EFIAPI
UsbcRetimerCompliancePDMessage (
  IN OUT UINT8      *Data
  )
{
  UINT8             DataSize;
  UINT8             Data8[1];
  EFI_STATUS        Status;

  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DataSize = sizeof (Data8);
  Data8[0] = *Data;
  Status = EcInterface (EcId0Ch0, EC_C_USBC_RETIMER, &DataSize, Data8);
  if (!EFI_ERROR (Status)) {
    *Data = Data8[0];
  }

  return Status;
}

/**
  USBC Get USB Connection Status

  @param[in, out]   DataBuffer
  @param[in]        Timeout

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_TIMEOUT           Command timeout
  @retval           EFI_DEVICE_ERROR      Command error

**/
EFI_STATUS
EFIAPI
UsbcGetUsbConnStatus (
  IN OUT UINT8      *DataBuffer,
  IN     UINT32     Timeout
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Set PD Controller Mode

  @param[in]        PdControllerMode

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_TIMEOUT           Command timeout
  @retval           EFI_DEVICE_ERROR      Command error
  @retval           EFI_INVALID_PARAMETER Command usage error.

**/
EFI_STATUS
EFIAPI
SetPdControllerMode (
  IN     UINT8      PdControllerMode
  )
{
  UINT8             DataSize;
  EFI_STATUS        Status;
  UINT32            Count;
  UINT8             Data8[1];

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "\nSetPdControllerMode of PD Controller to Retimer FW Update Mode = %s\n",
        ((PdControllerMode == RetimerFirmWareUpdateEnableMode) ? L"RetimerFirmWareUpdateEnableMode" : L"RetimerFirmWareUpdateDisableMode" )));

  if (PdControllerMode >= InvalidRetimerFirmWareUpdateMode) {
    DEBUG ((DEBUG_INFO, "\nSetPdControllerMode Returning as Invalid Pd Controller Mode\n"));
    return EFI_INVALID_PARAMETER;
  }
  //
  // Currently EC Implementation Does not support PD Controller Number as
  // Parameter. It Change/Set the mode for all PD Controller. So currently
  // we are only passing one parameter to EC i.e. PD Controller Mode.
  //
  DataSize = 1; // This should be set to two if we are passing 2 Parameter to EC
  Data8[0] = PdControllerMode;

  // Send EC Command to Set PD Controller Mode
  Status = (EcInterface (EcId0Ch0, EC_C_SET_PD_FW_UPDATE_MODE, &DataSize, Data8));

  // Wait (EC_SET_PD_MODE_WAIT_TIME_IN_FACTOR_OF_50_MS * 50)ms time before exiting
  for (Count = 0; Count < EC_SET_PD_MODE_WAIT_TIME_IN_FACTOR_OF_50_MS; Count++) {
    MicroSecondDelay(50 * 1000); // 50ms Delay
  }

  DEBUG ((DEBUG_INFO, "\nSetPdControllerMode Returning Status =%r\n", Status));
  return Status;
}

/**
  Get PD Controller Mode

  @param[out]       PdControllerModeBuffer
  @param[out]       EcPdTempBuffer
  @param[in]        TotalCountOfPdController

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_TIMEOUT           Command timeout
  @retval           EFI_DEVICE_ERROR      Command error
  @retval           EFI_INVALID_PARAMETER Command usage error.

**/
EFI_STATUS
EFIAPI
GetPdControllerMode (
  OUT    UINT8      *PdControllerModeBuffer,
  OUT    UINT8      *EcPdTempBuffer,
  IN     UINT8      TotalCountOfPdController
  )
{
  UINT8             DataSize;
  UINT8             PdI2cCommandSuccess;
  UINT8             PdRetimerFwMode;
  UINT8             TempBuffer[1];
  EFI_STATUS        Status;
  UINT8             Index;
  UINT8             MaxPdController;

  if (PdControllerModeBuffer == NULL) {
    DEBUG ((DEBUG_INFO, "\nGetPdControllerMode Returning as Null Buffer Passed\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (EcPdTempBuffer == NULL) {
    DEBUG ((DEBUG_INFO, "\nEcPdTempBuffer Returning as Null Buffer Passed\n"));
    return EFI_INVALID_PARAMETER;
  }

  DataSize = 1;
  *PdControllerModeBuffer = 0;
  *EcPdTempBuffer = 0;
  TempBuffer[0] = 0;
  Status = EFI_SUCCESS;
  MaxPdController = (UINT8)(GET_PD_MODE_STATUS_BIT_WIDTH_TOTAL / GET_PD_MODE_STATUS_BIT_WIDTH_PER_CONTROLLER);

  // Initialize the parameter for Bit0 -> PdI2cCommandSuccess.
  PdI2cCommandSuccess = 0;
  // Initialize the parameter for Bit1 -> PdRetimerFwMode.
  PdRetimerFwMode = 0;

  // Initialize PD Controller Mode Buffer.
  // By default it is RetimerFirmWareUpdateDisableMode,
  // and will remain in that state unless if one PD reports in TBT mode.
  *PdControllerModeBuffer = RetimerFirmWareUpdateDisableMode;

  DEBUG ((DEBUG_INFO, "\nGetPdControllerMode with TotalCountOfPdController =%d\n", TotalCountOfPdController));

  Status = EcInterface (EcId0Ch0, EC_C_GET_PD_FW_UPDATE_MODE, &DataSize, TempBuffer);

  *EcPdTempBuffer = TempBuffer[0];

  if (Status == EFI_SUCCESS) {
    // Check the PD0/1 status after the EC send 1E command.
    for (Index = 0; ((Index < MaxPdController) && (Index < TotalCountOfPdController)); Index ++) {

      // Get PdI2cCommandSuccess (Bit0) information.
      PdI2cCommandSuccess = (UINT8)((TempBuffer[0] >> (Index * GET_PD_MODE_STATUS_BIT_WIDTH_PER_CONTROLLER)) & I2C_COMMAND_SUCCESSFUL);
      // Get PdRetimerFwMode (Bit1) information.
      PdRetimerFwMode     = (UINT8)((TempBuffer[0] >> (Index * GET_PD_MODE_STATUS_BIT_WIDTH_PER_CONTROLLER)) & RETIMER_FW_UPDATE_MODE);

      DEBUG ((DEBUG_INFO, "GetPdMode - PD%d Bit0 = %d\n", Index + 1, PdI2cCommandSuccess));
      DEBUG ((DEBUG_INFO, "GetPdMode - PD%d Bit1 = %d\n", Index + 1, PdRetimerFwMode));

      // Bit0 = 0 (PD I2C Command fail)
      if (PdI2cCommandSuccess != 0) {
        DEBUG ((DEBUG_INFO, "PD%d I2C command successful.\n", Index + 1));
      }
      // Bit1 = 0 (Not in force TBT mode)
      if (PdRetimerFwMode != 0) {
        DEBUG ((DEBUG_INFO, "PD%d in Retimer FW update mode.\n", Index + 1));
      }
      // Set PdControllerModeBuffer to RetimerFirmWareUpdateEnableMode if PdI2cCommandSuccess and PdRetimerFwMode not 0.
      if ((PdI2cCommandSuccess != 0) && (PdRetimerFwMode != 0)) {
        *PdControllerModeBuffer = RetimerFirmWareUpdateEnableMode;
      }
    }
  }

  DEBUG ((DEBUG_INFO, "\nGetPdControllerMode of PD controller current Retimer FW Update Mode = %s With return Status = %r\n", \
        ((*PdControllerModeBuffer == RetimerFirmWareUpdateEnableMode) ? L"RetimerFirmWareUpdateEnableMode" : L"RetimerFirmWareUpdateDisableMode" ), Status));
  return Status;
}

/**
  Get Modular USBC IO Config

  @param[in, out]   DataBuffer
  @param[in]        BufferSize

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_TIMEOUT           Command timeout
  @retval           EFI_DEVICE_ERROR      Command error

**/
EFI_STATUS
EFIAPI
GetModularUsbCIoConfig (
  IN OUT UINT64     *DataBuffer,
  IN     UINT8      BufferSize
  )
{
  if (BufferSize < sizeof (UINT64)) {
    return EFI_BUFFER_TOO_SMALL;
  }
  return EcInterface (EcId0Ch0, EC_C_MODULAR_IO_INFO_CMD, &BufferSize, (UINT8 *) DataBuffer);
}

/**
  Update USBC host flags to EC

  @param[in]        UsbCHostFlags         Send USBC host flags to EC.

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_INVALID_PARAMETER UsbCHostFlags is NULL

**/
EFI_STATUS
EFIAPI
UpdateUsbCHostFlagsToEc (
  IN UINT8          *UsbCHostFlags
  )
{
  EFI_STATUS        Status;
  UINT8             DataSize;
  UINT8             Data8;

  if (UsbCHostFlags == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DataSize = sizeof (Data8);
  Data8 = *UsbCHostFlags;
  Status = EcInterface (EcId0Ch0, EC_C_USBC_HOST_FLAGS, &DataSize, &Data8);
  DEBUG((DEBUG_INFO, "EC_C_USBC_HOST_FLAGS Command status %r \n ", Status));

  return EFI_SUCCESS;
}

/**
  Execute the PD Vendor Command via 0x0C command with EC private port

  @param[in]  PdCntrlIndex       PD controller index (0-based).
  @param[in]  VendorCmd          PD Vendor command data
  @param[in]  Lock               Need to Lock the EC PD I2C target or not
  @param[in]  InputData          A pointer to input data
  @param[in]  InputDataSize      A pointer to input data size
  @param[out] OutputData         A pointer to out data
  @param[out] OutputDataSize     A pointer to out data size

  @retval EFI_SUCCESS            Command success.
  @retval EFI_TIMEOUT            EC is busy.
  @retval EFI_ACCESS_DENIED      EC PD I2C target is lock.
  @retval EFI_INVALID_PARAMETER  Parameter invalid.
  @retval EFI_UNSUPPORTED        Unsupported EC channel or the command is not found in mEcCommand.
  @retval EFI_BUFFER_TOO_SMALL   The DataBuffer is too small to Read/Write data with EC FW.

**/
EFI_STATUS
EFIAPI
EcPdExecuteVendorCommand (
  IN  UINT8    PdCntrlIndex,
  IN  UINT8    VendorCmd,
  IN  BOOLEAN  Lock,
  IN  UINT8    *InputData,
  IN  UINT8    *InputDataSize,
  OUT UINT8    *OutputData OPTIONAL,
  OUT UINT8    *OutputDataSize OPTIONAL
  )
{
  EFI_STATUS  Status;
  UINT8       DataBuffer[2];
  UINT8       Index;
  UINT16      EcPdCmdMaxRetry;
  UINT16      EcPdCmdWaitStallInUs;
  UINT16      Time;

  if (Lock) {
    ///
    /// Lock the Ec Pd communication if needed
    ///
    Status = LockEcPdI2cTarget (EC_PD_LOCK_I2C);
    if (EFI_ERROR (Status)) {
      goto ERROR_EXIT;
    }
  }

  ///
  /// Write the PD controller index
  ///
  DataBuffer[1] = PdCntrlIndex;
  DataBuffer[0] = PD_CONTROLLER_INDEX;
  Status        = WriteEcRam (EcId0Ch1, DataBuffer, sizeof (DataBuffer));
  if (EFI_ERROR (Status)) {
    goto ERROR_EXIT;
  }

  ///
  /// Write the PD Vendor command data
  ///
  DataBuffer[1] = VendorCmd;
  DataBuffer[0] = VENDOR_REGISTER_OR_COMMAND;
  Status        = WriteEcRam (EcId0Ch1, DataBuffer, sizeof (DataBuffer));
  if (EFI_ERROR (Status)) {
    goto ERROR_EXIT;
  }

  ///
  /// Write the data size
  ///
  DataBuffer[1] = *InputDataSize;
  DataBuffer[0] = DATA_SIZE;
  Status        = WriteEcRam (EcId0Ch1, DataBuffer, sizeof (DataBuffer));
  if (EFI_ERROR (Status)) {
    goto ERROR_EXIT;
  }

  ///
  /// Write the data
  ///
  for (Index = 0; Index < *InputDataSize; Index++) {
    DataBuffer[1] = InputData[Index];
    DataBuffer[0] = DATA_00 + Index;
    Status        = WriteEcRam (EcId0Ch1, DataBuffer, sizeof (DataBuffer));
    if (EFI_ERROR (Status)) {
      goto ERROR_EXIT;
    }
  }

  ///
  /// Write the EC PD command 0x0C to execute PD vendor command
  ///
  DataBuffer[1] = EC_PRIVATE_CMD_EXECUTE;
  DataBuffer[0] = BIOS_EC_COMMAND;
  Status        = WriteEcRam (EcId0Ch1, DataBuffer, sizeof (DataBuffer));
  if (EFI_ERROR (Status)) {
    goto ERROR_EXIT;
  }

  EcPdCmdMaxRetry      = PcdGet16 (PcdEcPdCmdMaxRetry);
  EcPdCmdWaitStallInUs = PcdGet16 (PcdEcPdCmdWaitStallInUs);

  for (Time = 0; Time < EcPdCmdMaxRetry; Time++) {
    DataBuffer[0] = COMMAND_EXECUTION_STATUS;

    Status = ReadEcRam (EcId0Ch1, DataBuffer, sizeof (DataBuffer));
    if (EFI_ERROR (Status)) {
      goto ERROR_EXIT;
    }

    if (DataBuffer[0] == EC_PRIVATE_CMD_STATUS_SUCCESS) {
      break;
    } else if (DataBuffer[0] == EC_PRIVATE_CMD_STATUS_IN_PROGRESS) {
      MicroSecondDelay (EcPdCmdWaitStallInUs);
    } else if ((DataBuffer[0] == EC_PRIVATE_CMD_STATUS_NO_ACK) || (DataBuffer[0] == EC_PRIVATE_CMD_STATUS_FAILED)) {
      DEBUG ((DEBUG_ERROR, "Command execution failed, Status:%x \n", DataBuffer[0]));
      Status = EFI_DEVICE_ERROR;
      goto ERROR_EXIT;
    }
  }

  if (DataBuffer[0] != EC_PRIVATE_CMD_STATUS_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Command execution failed, Status:%x \n", DataBuffer[0]));
    Status = EFI_TIMEOUT;
    goto ERROR_EXIT;
  }

  if (OutputData != NULL) {
    ///
    /// Read the return data size
    ///
    DataBuffer[0] = DATA_SIZE;
    Status        = ReadEcRam (EcId0Ch1, DataBuffer, sizeof (DataBuffer));
    if (EFI_ERROR (Status)) {
      goto ERROR_EXIT;
    }

    *OutputDataSize = DataBuffer[0];
    DEBUG ((DEBUG_INFO, "DataSize %d \n", *OutputDataSize));

    ///
    /// Read the return data
    ///
    for (Index = 0; Index < *OutputDataSize; Index++) {
      DataBuffer[0] = DATA_00 + Index;
      Status        = ReadEcRam (EcId0Ch1, DataBuffer, sizeof (DataBuffer));
      if (EFI_ERROR (Status)) {
        goto ERROR_EXIT;
      }

      OutputData[Index] = DataBuffer[0];
    }
  }

  return Status;

ERROR_EXIT:
  DEBUG ((DEBUG_ERROR, "%a: Read/Write EcRam fail, Status: %r\n", __FUNCTION__, Status));
  if (Lock) {
    LockEcPdI2cTarget (EC_PD_UNLOCK_I2C);
  }

  return Status;
}

/**
  Lock or unlock the EC PD I2C target

  @param[in] Lock                Lock(0x01) or unlock(0x00).

  @retval EFI_SUCCESS            Command success.
  @retval EFI_TIMEOUT            EC is busy.
  @retval EFI_INVALID_PARAMETER  Parameter invalid.
  @retval EFI_UNSUPPORTED        Unsupported EC channel or the command is not found in mEcCommand.
  @retval EFI_BUFFER_TOO_SMALL   The DataBuffer is too small to Read/Write data with EC FW.

**/
EFI_STATUS
EFIAPI
LockEcPdI2cTarget (
  IN UINT8  Lock
  )
{
  EFI_STATUS  Status;
  UINT8       LockStatus;
  UINT8       DataBuffer[2];

  ///
  /// Check the lock status before lock the I2C communication
  ///
  Status = GetEcPdLockStatus (&LockStatus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (LockStatus != Lock) {
    DEBUG ((DEBUG_INFO, "%a: EC PD %s the I2C target\n", __FUNCTION__, Lock == EC_PD_LOCK_I2C ? L"Lock" : L"Unlock"));
    DataBuffer[1] = Lock;
    DataBuffer[0] = I2C_COMMUNICATION_LOCK;
    Status        = WriteEcRam (EcId0Ch1, DataBuffer, sizeof (DataBuffer));
  }

  return Status;
}

/**
  Check the EC PD lock status

  @param[in, out] LockStatus     Lock status

  @retval EFI_SUCCESS            Command success.
  @retval EFI_TIMEOUT            EC is busy.
  @retval EFI_INVALID_PARAMETER  Parameter invalid.
  @retval EFI_UNSUPPORTED        Unsupported EC channel or the command is not found in mEcCommand.
  @retval EFI_BUFFER_TOO_SMALL   The DataBuffer is too small to Read/Write data with EC FW.

**/
EFI_STATUS
EFIAPI
GetEcPdLockStatus (
  IN OUT UINT8  *LockStatus
  )
{
  EFI_STATUS  Status;
  UINT8       DataBuffer[1];

  DataBuffer[0] = I2C_COMMUNICATION_LOCK;
  Status        = ReadEcRam (EcId0Ch1, DataBuffer, sizeof (DataBuffer));

  if (!EFI_ERROR (Status)) {
    *LockStatus = DataBuffer[0];
    DEBUG ((DEBUG_INFO, "%a: EC PD lock status: 0x%2X\n", __FUNCTION__, *LockStatus));
  }

  return Status;
}
