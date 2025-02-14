/** @file
  EC TCSS library functions.

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

#ifndef _EC_TCSS_LIB_H_
#define _EC_TCSS_LIB_H_

//
// Include files
//
#include <Base.h>
#include <Uefi.h>

#define HOST_USBC_UCSI_STATUS BIT0

/**
  USBC IOM Ready Notify

  @param[in, out]   DataBuffer

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_DEVICE_ERROR      Command error

**/
EFI_STATUS
EFIAPI
UsbcIomReadyNotify (
  IN OUT UINT8     *DataBuffer
  );

/**
  Usbc Retimer Compliance Mode

  @param[in, out]   Data                  Recive 1 byte data from EC.

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_DEVICE_ERROR      Command error

**/
EFI_STATUS
EFIAPI
UsbcRetimerCompliancePDMessage  (
  IN OUT UINT8     *DataBuffer
  );

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
  IN OUT UINT8     *DataBuffer,
  IN     UINT32    Timeout
  );

/**
  USBC Sx Exit Wait

  @param[out]      DataBuffer
  @param[in]       Timeout

  @retval         EFI_SUCCESS         Command success
  @retval         EFI_DEVICE_ERROR    Command error

**/
EFI_STATUS
EFIAPI
UsbcSxExit (
  OUT   UINT8      *DataBuffer,
  IN    UINT32     Timeout
  );

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
  IN     UINT8     PdControllerMode
  );

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
  OUT    UINT8     *PdControllerModeBuffer,
  OUT    UINT8     *EcPdTempBuffer,
  IN     UINT8     TotalCountOfPdController
  );

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
  );

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
  );

/**
  Notify EC controller to restore of PL4 offset values.

  @retval         EFI_SUCCESS       Command success
  @retval         Others            Command error:
                                    EFI_TIMEOUT,
                                    EFI_INVALID_PARAMETER,
                                    EFI_UNSUPPORTED,
                                    EFI_BUFFER_TOO_SMALL, etc.
**/
EFI_STATUS
EFIAPI
NotifyEcToRestorePL4Value (
  VOID
  );

/**
  Execute the PD Vendor Command via EC private port

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
  );

/**
  Lock or unlock the EC PD I2c target

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
  );

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
  );

#endif // _EC_TCSS_LIB_H_
