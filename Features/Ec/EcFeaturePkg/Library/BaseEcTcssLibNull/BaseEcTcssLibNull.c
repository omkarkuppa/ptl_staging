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

#include <Base.h>
#include <Uefi.h>

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
  return EFI_SUCCESS;
}

/**
  Usbc Retimer Compliance Mode

  @param[in, out]   Data                  Recive 1 byte data from EC.

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_DEVICE_ERROR      Command error

**/
EFI_STATUS
EFIAPI
UsbcRetimerCompliancePDMessage  (
  IN OUT UINT8      *DataBuffer
  )
{
  return EFI_SUCCESS;
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
  return EFI_SUCCESS;
}

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
  OUT    UINT8      *DataBuffer,
  IN     UINT32     Timeout
  )
{
  return EFI_SUCCESS;
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
  return EFI_SUCCESS;
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
  return EFI_SUCCESS;
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
  return EFI_SUCCESS;
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
  return EFI_SUCCESS;
}

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
  )
{
  return EFI_SUCCESS;
}