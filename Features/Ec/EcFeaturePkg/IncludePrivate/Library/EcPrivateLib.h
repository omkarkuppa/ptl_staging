/** @file
  EC private library functions and definitions.

  This library provides private EC interface.

  There may be different libraries for different environments (PEI, BS, RT, SMM).
  Make sure you meet the requirements for the library (protocol dependencies, use
  restrictions, etc).

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


#ifndef __EC_PRIVATE_LIB_H__
#define __EC_PRIVATE_LIB_H__

//
// Include files
//
#include <Base.h>
#include <Uefi.h>

typedef enum {
  LpcInterface = 0,
  SpiInterface,
  EspiInterface,
  I2cInterface,
  EcInterfaceMax
} EC_INTERFACE_TYPE;

typedef enum {
  EcId0 = 0,
  EcIdMax
} EC_ID;

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
  );

/**
  Receives status from EC.

  @param[out]       EcStatus                Status byte to receive

  @retval           EFI_SUCCESS             Command success.
  @retval           EFI_INVALID_PARAMETER   Parameter invalid.

**/
EFI_STATUS
EFIAPI
ReceiveEcStatus (
  OUT UINT8         *EcStatus
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );


#endif

