/** @file
  Component information file for base EC library.

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
#include <Base.h>
#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <EcCommands.h>
#include <Library/EcLib.h>
#include <Library/EcPrivateLib.h>

#if FixedPcdGetBool(PcdEcZeroTimeout) == 0
#ifndef STALL_ONE_MICRO_SECOND
#define STALL_ONE_MICRO_SECOND  1
#endif
#ifndef STALL_ONE_MILLI_SECOND
#define STALL_ONE_MILLI_SECOND  1000
#endif
#else
#ifndef STALL_ONE_MICRO_SECOND
#define STALL_ONE_MICRO_SECOND  0
#endif
#ifndef STALL_ONE_MILLI_SECOND
#define STALL_ONE_MILLI_SECOND  0
#endif
#endif

#ifndef EC_INIT_TIME_OUT
#define EC_INIT_TIME_OUT       0x200
#endif

typedef struct {
  UINT8   CommandNumber;
  UINT8   NumberOfSendData;
  UINT8   NumberOfReceiveData;
  BOOLEAN CommandImplemented;
} EC_COMMAND_TABLE;

EC_COMMAND_TABLE mEcCommand[] = {
  // EC Command,                    Send data size, Receive data size,    Implemented
  {EC_C_STC_CONFIG,                 5,              0,                    TRUE},
  {EC_C_SMC_GET_MODE,               0,              4,                    TRUE},
  {EC_C_SWITCH_STATUS,              0,              1,                    TRUE},
  {GET_HW_PERIPHERALS_STS,          0,              2,                    TRUE},
  {EC_C_LAST_SHUTDOWN_STATUS,       0,              1,                    TRUE},
  {EC_C_BOARD_INFO,                 0,              2,                    TRUE},
  {EC_C_RESTORATION_PL4_VALUES,     0,              0,                    TRUE},
  {EC_C_PLN_ENABLE,                 1,              0,                    TRUE},
  {EC_C_SET_SYS_CHANGING_CFG,       0,              0,                    TRUE},
  {EC_C_SET_CHARGING_METHOD,        1,              0,                    TRUE},
  {EC_C_PWR_BOSS_OK,                0,              0,                    TRUE},
  {EC_C_UPDATE_UVTH,                0,              0,                    TRUE},
  {EC_C_TDP_WATTS,                  1,              0,                    TRUE},
  {EC_C_UPDATE_PWM,                 0,              0,                    TRUE},
  {EC_C_SET_PD_FW_UPDATE_MODE,      1,              0,                    TRUE},
  {EC_C_GET_PD_FW_UPDATE_MODE,      0,              1,                    TRUE},
  {EC_C_USBC_RETIMER,               1,              0,                    TRUE},
  {EC_C_ENABLE_POWER_BTN,           0,              0,                    TRUE},
  {EC_C_DISABLE_POWER_BTN,          0,              0,                    TRUE},
  {EC_C_CS_LOW_POWER_ENABLE,        1,              0,                    TRUE},
  {EC_C_SET_DEEP_SLEEP,             1,              0,                    TRUE},
  {EC_C_SET_DSW_MODE,               1,              0,                    TRUE},
  {EC_C_GET_DSW_MODE,               0,              1,                    TRUE},
  {EC_C_LOW_POWER_ENTRY,            0,              0,                    TRUE},
  {EC_C_LOW_POWER_EXIT,             0,              0,                    TRUE},
  {EC_C_P_G3_SET_MODE,              1,              0,                    TRUE},
  {EC_C_SET_BAT_THRESHOLD,          1,              0,                    TRUE},
  {EC_C_GET_WAKE_STATUS,            0,              1,                    TRUE},
  {EC_C_CLEAR_WAKE_STATUS,          0,              0,                    TRUE},
  {EC_C_CHARGE_RATE_CHANGE_NOTIFY,  0,              0,                    TRUE},
  {EC_C_BTN_SCI_CTL,                1,              0,                    TRUE},
  {EC_C_FAIL_SAFE_FAN_CTRL,         2,              0,                    TRUE},
  {EC_C_SET_PECI_MODE,              1,              0,                    TRUE},
  {EC_C_SET_TMP_THRESHOLD,          0,              0,                    TRUE},
  {EC_C_DIS_PCH_DTS_READ,           1,              0,                    TRUE},
  {EC_C_SET_CRITICAL_TEMP,          1,              0,                    TRUE},
  {EC_C_START_PECI_TEST,            0,              0,                    TRUE},
  {EC_C_STOP_PECI_TEST,             0,              0,                    TRUE},
  {EC_C_ACPI_PORT_WRT_PECI_DATA,    0,              0,                    TRUE},
  {EC_C_ACPI_PORT_RD_PECI_DATA,     0,              0,                    TRUE},
  {EC_C_ACPI_PORT_GET_ERR_COUNT,    0,              0,                    TRUE},
  {EC_C_ACPI_PORT_RD_CMD_STATUS,    0,              1,                    TRUE},
  {EC_C_SMI_QUERY,                  0,              1,                    TRUE},
  {EC_C_EN_PWR_BTN_NOTIFY,          0,              0,                    TRUE},
  {EC_C_DIS_PWR_BTN_NOTIFY,         0,              0,                    TRUE},
  {EC_C_READ_MEM,                   1,              1,                    TRUE},
  {EC_C_WRITE_MEM,                  2,              0,                    TRUE},
  {EC_C_ACPI_TURST_CMD,             0,              0,                    TRUE},
  {EC_C_ACPI_NORMAL_EC,             0,              0,                    TRUE},
  {EC_C_ACPI_QUERY_EC,              0,              0,                    TRUE},
  {EC_C_EC_REVISION,                0,              4,                    TRUE},
  {EC_C_ACPI_ENABLE,                0,              0,                    TRUE},
  {EC_C_ACPI_DISABLE,               0,              0,                    TRUE},
  {EC_C_SX_ENTER,                   0,              0,                    TRUE},
  {EC_C_SX_EXIT,                    0,              0,                    TRUE},
  {EC_C_SMI_DISABLE,                0,              0,                    TRUE},
  {EC_C_SMI_ENABLE,                 0,              0,                    TRUE},
  {EC_C_MODULAR_IO_INFO_CMD,        0,              8,                    TRUE},
  {EC_C_PD_VERSION_INFO_CMD,        1,              8,                    TRUE},
  {EC_C_PD_VERSION_INFO_CMD_4BYTE,  1,              4,                    TRUE},
  {EC_C_UCSI_CMD_HNDLR,             0,              0,                    TRUE},
  {EC_C_UCSI_READ_VERSION,          0,              2,                    TRUE},
  {EC_C_USBC_HOST_FLAGS,            1,              0,                    TRUE},
  {EC_C_USBC_SX_EXIT_WAIT,          0,              1,                    TRUE},
  {EC_C_WDT_WRITE,                  2,              0,                    TRUE},
  {EC_C_WDT_READ,                   0,              1,                    TRUE},
  {EC_C_READ_ACPI_SPACE,            0,              0,                    TRUE},
  {EC_C_WRITE_ACPI_SPACE,           0,              0,                    TRUE},
  {EC_C_BATTERY_SHIP_MODE,          0,              0,                    TRUE},
  {EC_C_SET_G3TOS5,                 1,              0,                    TRUE},
  {EC_C_BIOS_FAN_CONTROL,           0,              0,                    TRUE},
  {EC_C_RESET_KSC,                  1,              0,                    TRUE},
};

/**
  Send data and command to EcId0

  @param[in]        Command                 Command value to send to the EcId0
  @param[in, out]   DataSize                Size of data to send to the EcId0.
                                            If the command retuned data - size of buffer returned by the EcId0.
                                            Be aware of the DataSize must euqal to size of DataBuffer and cannot smaller
                                            than number of send data or number of receive data, whichever is the grater.
  @param[in, out]   DataBuffer              Pointer to the data buffer including data to be sent to the EcId0.
                                            If the command returned data - pointer to the buffer including the data.
                                            The buffer size should be the max of receive and transmit data.

  @retval           EFI_SUCCESS             Command success.
  @retval           EFI_TIMEOUT             EC is busy.
  @retval           EFI_INVALID_PARAMETER   Parameter invalid.
  @retval           EFI_UNSUPPORTED         The command is not found in mEcCommand.
  @retval           EFI_BUFFER_TOO_SMALL    The DataBuffer is too small to Read/Write data with EC FW.

**/
EFI_STATUS
EcId0Interface (
  IN UINT8          Command,
  IN OUT UINT8      *DataSize,
  IN OUT UINT8      *DataBuffer
  )
{
  EFI_STATUS        Status;
  UINT8             NumberOfEcCommands;
  UINT8             Index;
  UINT8             TxDataIndex;
  UINT8             RxDataIndex;
  UINT8             MaxValue;

  Status = EFI_SUCCESS;

  //
  // Ec Debug info print
  //
  DEBUG ((DEBUG_INFO, "Ec Command(Hex):       %02X\n", Command));
  DEBUG ((DEBUG_INFO, "Ec DataSize Address:   0x%08X\n", DataSize));
  DEBUG ((DEBUG_INFO, "Ec DataBuffer Address: 0x%08X\n", DataBuffer));
  if (DataSize != NULL) {
    DEBUG ((DEBUG_INFO, "Ec DataSize value(Hex): %x\n", *DataSize));
    if (DataBuffer == NULL) {
      DEBUG ((DEBUG_ERROR , "Invalid NULL DataBuffer!\n"));
      Status = EFI_INVALID_PARAMETER;
    } else {
      DEBUG ((DEBUG_INFO, "EC DataBuffer(Hex):\n"));
      for (Index = 0; Index < *DataSize; Index++) {
        DEBUG ((DEBUG_INFO, "%02x ", DataBuffer[Index]));
      }
      DEBUG ((DEBUG_INFO, "\n"));
    }
  } else {
    if (DataBuffer != NULL) {
      DEBUG ((DEBUG_ERROR, "Invalid input DataSize NULL with DataBuffer not NULL!\n"));
      Status = EFI_INVALID_PARAMETER;
    }
  }

  if (EFI_ERROR (Status)) {
    return Status;
  }

  NumberOfEcCommands = sizeof (mEcCommand) / sizeof (EC_COMMAND_TABLE);

  for (Index = 0; Index < NumberOfEcCommands; Index++) {
    if (Command != mEcCommand[Index].CommandNumber) {
      continue;
    }
    if (mEcCommand[Index].CommandImplemented != TRUE) {
      Status = EFI_UNSUPPORTED;
      break;
    }

    if ((mEcCommand[Index].NumberOfSendData != 0) || (mEcCommand[Index].NumberOfReceiveData != 0)) {
      if (DataSize == NULL || DataBuffer == NULL) {
        Status = EFI_INVALID_PARAMETER;
        break;
      } else {
        MaxValue = MAX (mEcCommand[Index].NumberOfSendData, mEcCommand[Index].NumberOfReceiveData);
        if (*DataSize < MaxValue) {
          Status = EFI_BUFFER_TOO_SMALL;
          *DataSize = MaxValue;
          break;
        }
      }
    }
    Status = SendEcCommand (Command);
    if (EFI_ERROR (Status)) {
      break;
    }
    if (mEcCommand[Index].NumberOfSendData != 0) {
     for (TxDataIndex = 0; TxDataIndex < mEcCommand[Index].NumberOfSendData; TxDataIndex++) {
        Status = SendEcData (DataBuffer[TxDataIndex]);
        if (EFI_ERROR (Status)) {
          break;
        }
      }
    }
    if (mEcCommand[Index].NumberOfReceiveData != 0) {
      for (RxDataIndex = 0; RxDataIndex < mEcCommand[Index].NumberOfReceiveData; RxDataIndex++) {
        Status = ReceiveEcData (&DataBuffer[RxDataIndex]);
        if (EFI_ERROR (Status)) {
          break;
        }
        *DataSize = RxDataIndex + 1;
      }
    }
    break;
  }
  if (Index == NumberOfEcCommands) {
    DEBUG ((DEBUG_ERROR, "Command %x not defined in mEcCommand list.\n", Command));
  } else if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "EcId0Interface Status %r.\n", Status));
  }
  return Status;
}

/**
  Sends command to EC.

  @param[in]        Command                 Command byte to send
  @param[in]        Timeout                 Timeout in microseonds

  @retval           EFI_SUCCESS             Command success.
  @retval           EFI_TIMEOUT             EC is busy.

**/
EFI_STATUS
EFIAPI
SendEcCommandTimeout (
  IN UINT8          Command,
  IN UINT32         Timeout
  )
{
  UINTN             Index;
  UINT8             EcStatus;

  Index       = 0;
  EcStatus    = 0;
  //
  // Wait for EC to be ready (with a timeout)
  //
  ReceiveEcStatus (&EcStatus);
  //
  // Check if output buffer bit(OBF) is set.
  // Read and discard the output buffer data so that next BIOS-EC cmd is in sync
  // OBF is cleared by h/w after all data in output buffer is read by BIOS.
  //
  while (((EcStatus & EC_S_OBF) != 0) && (Index < Timeout)) {
    //
    // Read EC data
    //
    MicroSecondDelay (10 * STALL_ONE_MICRO_SECOND);
    IoRead8 (EC_D_PORT);
    ReceiveEcStatus (&EcStatus);
    Index+=10;
  }
  DEBUG ((DEBUG_INFO, "Ec OBF check, EcStatus: %x\n", EcStatus));

  if (Index >= Timeout) {
    return EFI_TIMEOUT;
  }

  Index = 0;

  while (((EcStatus & EC_S_IBF) != 0) && (Index < Timeout)) {
    MicroSecondDelay (10 * STALL_ONE_MICRO_SECOND);
    ReceiveEcStatus (&EcStatus);
    Index+=10;
  }
  DEBUG ((DEBUG_INFO, "Ec IBF check, EcStatus: %x\n", EcStatus));

  if (Index >= Timeout) {
    return EFI_TIMEOUT;
  }

  //Printing EC Command Sent
  DEBUG ((DEBUG_INFO, "Sending EC Command: %02X\n", Command));

  //
  // Send the EC command
  //
  IoWrite8 (EC_C_PORT, Command);

  DEBUG ((DEBUG_INFO, "SendEcCommandTimeout sent %x \n", Command));

  return EFI_SUCCESS;
}

/**
  Receives status from EC.

  @param[out]       EcStatus                Status byte to receive

  @retval           EFI_SUCCESS             Command success.
  @retval           EFI_INVALID_PARAMETER   Parameter invalid.

**/
EFI_STATUS
EFIAPI
ReceiveEcStatus (
  OUT UINT8                 *EcStatus
  )
{
  if (EcStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read and return the status
  //
  *EcStatus = IoRead8 (EC_C_PORT);

  return EFI_SUCCESS;
}

/**
  Receive data from EC.

  @param[out]       Data                    Data byte received
  @param[in]        Timeout                 Timeout in microseonds

  @retval           EFI_SUCCESS             Read data success.
  @retval           EFI_TIMEOUT             EC is busy.
  @retval           EFI_INVALID_PARAMETER   Parameter invalid.

**/
EFI_STATUS
EFIAPI
ReceiveEcDataTimeout (
  OUT UINT8         *Data,
  IN  UINT32        Timeout
  )
{
  UINTN             Index;
  UINT8             EcStatus;

  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Index = 0;

  //
  // Wait for EC to be ready (with a timeout)
  //
  ReceiveEcStatus (&EcStatus);
  while (((EcStatus & EC_S_OBF) == 0) && (Index < Timeout)) {
    MicroSecondDelay (15 * STALL_ONE_MICRO_SECOND);
    ReceiveEcStatus (&EcStatus);
    Index++;
  }
  DEBUG ((DEBUG_INFO, "Ec OBF check, EcStatus: %x\n", EcStatus));

  if (Index >= Timeout) {
    return EFI_TIMEOUT;
  }
  //
  // Read EC data and return
  //
  *Data = IoRead8 (EC_D_PORT);

  //Printing EC Data Received
  DEBUG ((DEBUG_INFO, "Receiving EC Data: %02X\n", *Data));

  return EFI_SUCCESS;
}

/**
  Send data from EC.

  @param[out]       Data                    Data byte received
  @param[in]        Timeout                 Timeout in microseonds

  @retval           EFI_SUCCESS             Send data success.
  @retval           EFI_TIMEOUT             EC is busy.

**/
EFI_STATUS
EFIAPI
SendEcDataTimeout (
  OUT UINT8         Data,
  IN  UINT32        Timeout
  )
{
  UINTN             Index;
  UINT8             EcStatus;

  Index = 0;

  //
  // Wait for EC to be ready (with a timeout)
  //
  ReceiveEcStatus (&EcStatus);
  while (((EcStatus & EC_S_IBF) != 0) && (Index < Timeout)) {
    MicroSecondDelay (15);
    ReceiveEcStatus (&EcStatus);
    Index++;
  }
  DEBUG ((DEBUG_INFO, "Ec IBF check, EcStatus: %x\n", EcStatus));

  if (Index >= Timeout) {
    return EFI_TIMEOUT;
  }

  //
  // Send the data and return
  //
  IoWrite8 (EC_D_PORT, Data);

  //
  // Printing EC Data Sent
  //
  DEBUG ((DEBUG_INFO, "Sent EC Data: %02X sent\n", Data));
  return EFI_SUCCESS;
}

/**
  Sends data to the Embedded Controler.
    Four use-cases:
    1.Send command to EC (DataSize and DataBuffer can be NULL)
    2.Send command to EC and send data to EC
    3.Send command to EC and receive data from EC
    4.Send command to EC, send data to EC and receive data from EC

  @param[in]        EcId                    Embedded Controller identification - in case there is more then one.
  @param[in]        Interface               Interface used to access the EC HW
  @param[in]        Command                 Command value to send to the EC
  @param[in, out]   DataSize                Size of data to send to the EC.
                                            If the command retuned data - size of buffer returned by the EC.
                                            Be aware of the DataSize must euqal to size of DataBuffer and cannot smaller
                                            than number of send data or number of receive data, whichever is the grater.
  @param[in, out]   DataBuffer              Pointer to the data buffer including data to be sent to the EC.
                                            If the command returned data - pointer to the buffer including the data.
                                            The buffer size should be the max of receive and transmit data.

  @retval           EFI_SUCCESS             Command success.
  @retval           EFI_TIMEOUT             EC is busy.
  @retval           EFI_INVALID_PARAMETER   Parameter invalid.
  @retval           EFI_UNSUPPORTED         EcId is not EcId0 or the command is not found in mEcCommand.
  @retval           EFI_BUFFER_TOO_SMALL    The DataBuffer is too small to Read/Write data with EC FW.

**/
EFI_STATUS
EFIAPI
EcInterface (
  IN EC_ID          EcId,
  IN UINT8          Command,
  IN OUT UINT8      *DataSize,
  IN OUT UINT8      *DataBuffer
  )
{
  EFI_STATUS        Status;

  if (EcId >= EcIdMax) {
    return EFI_INVALID_PARAMETER;
  }
  Status = EFI_SUCCESS;

  switch (EcId) {
    case EcId0:
      Status = EcId0Interface (Command, DataSize, DataBuffer);
      DEBUG ((DEBUG_INFO, "Ec Command Status: %r\n", Status));
      break;

    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  return Status;
}


/**
  Read Data from EC Memory from location pointed by DataBuffer.

  @param[in, out]   DataBuffer              Buffer use to communicate with EC RAM
  @param[in]        DataSize                Buffer size of DataBuffer

  @retval           EFI_SUCCESS             Command success
  @retval           Others                  Command error:
                                            EFI_TIMEOUT,
                                            EFI_INVALID_PARAMETER,
                                            EFI_UNSUPPORTED,
                                            EFI_BUFFER_TOO_SMALL, etc.

**/
EFI_STATUS
EFIAPI
ReadEcRam (
  IN OUT  UINT8       *DataBuffer,
  IN      UINT8       DataSize
  )
{
  return (EcInterface (EcId0, EC_C_READ_MEM, &DataSize, DataBuffer));
}

/**
  Write Data to EC memory at location pointed by DataBuffer.

  @param[in, out]   DataBuffer              Buffer use to communicate with EC RAM
  @param[in]        DataSize                Buffer size of DataBuffer

  @retval           EFI_SUCCESS             Command success
  @retval           Others                  Command error:
                                            EFI_TIMEOUT,
                                            EFI_INVALID_PARAMETER,
                                            EFI_UNSUPPORTED,
                                            EFI_BUFFER_TOO_SMALL, etc.

**/
EFI_STATUS
EFIAPI
WriteEcRam (
  IN OUT  UINT8       *DataBuffer,
  IN      UINT8       DataSize
  )
{
  return (EcInterface (EcId0, EC_C_WRITE_MEM, &DataSize, DataBuffer));
}

/**
  Send Exit Low Power Mode command to EC

  @retval           EFI_SUCCESS             Command success
  @retval           Others                  Command error:
                                            EFI_TIMEOUT,
                                            EFI_INVALID_PARAMETER,
                                            EFI_UNSUPPORTED,
                                            EFI_BUFFER_TOO_SMALL, etc.

**/
EFI_STATUS
EFIAPI
ExitLowPowerMode (
  VOID
  )
{
  return (EcInterface (EcId0, EC_C_LOW_POWER_EXIT, NULL, NULL));
}

/**
  Send Update PWM command to EC

  @retval           EFI_SUCCESS             Command success
  @retval           Others                  Command error:
                                            EFI_TIMEOUT,
                                            EFI_INVALID_PARAMETER,
                                            EFI_UNSUPPORTED,
                                            EFI_BUFFER_TOO_SMALL, etc.

**/
EFI_STATUS
EFIAPI
UpdatePwm (
  VOID
  )
{
  return (EcInterface (EcId0, EC_C_UPDATE_PWM, NULL, NULL));
}

/**
  EC wdt write command

  @param[in]        DataBuffer              2 bytes write for ECWDT through 0x66
  @param[in]        DataSize                Data size of DataBuffer

  @retval           EFI_SUCCESS             Command success
  @retval           Others                  Command error:
                                            EFI_TIMEOUT,
                                            EFI_INVALID_PARAMETER,
                                            EFI_UNSUPPORTED,
                                            EFI_BUFFER_TOO_SMALL, etc.

**/
EFI_STATUS
EFIAPI
BiosEcWdtWriteCommand (
  IN UINT8          *DataBuffer,
  IN UINT8          DataSize
  )
{
  return (EcInterface (EcId0, EC_C_WDT_WRITE, &DataSize, DataBuffer));
}