/** @file
  Base EC library implementation for H/W layer.

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
#include <Library/EcLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/EcPrivateLib.h>
#include <EcCommands.h>

/**
  Send command to EC.

  @param[in]  Command               Command byte to send

  @retval     EFI_SUCCESS           Command success
  @retval     EFI_TIMEOUT           Command timeout
  @retval     Other                 Command failed
**/
EFI_STATUS
EFIAPI
SendEcCommand (
  IN UINT8          Command
  )
{
  return SendEcCommandTimeout (Command, EC_TIME_OUT);
}

/**
  Receive data from EC.

  @param[out] Data                    Data byte received

  @retval     EFI_SUCCESS             Read success
  @retval     EFI_INVALID_PARAMETER   Data should not be NULL.
  @retval     EFI_TIMEOUT             Command timeout
**/
EFI_STATUS
EFIAPI
ReceiveEcData (
  OUT UINT8         *Data
 )
{
  return ReceiveEcDataTimeout (Data, EC_TIME_OUT);
}


/**
  Send data to EC.

  @param[in]  Data                  Data byte to send

  @retval     EFI_SUCCESS           Read success
  @retval     EFI_TIMEOUT           Command timeout
**/
EFI_STATUS
EFIAPI
SendEcData (
  IN UINT8          Data
  )
{
  return SendEcDataTimeout (Data, EC_TIME_OUT);
}
