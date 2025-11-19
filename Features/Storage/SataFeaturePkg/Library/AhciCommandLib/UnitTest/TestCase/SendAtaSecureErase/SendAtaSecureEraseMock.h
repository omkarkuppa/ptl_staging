/**@file
    Unit test case mock header for SendAtaSecureErase.

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

@par Specification
**/
#ifndef _SEND_ATA_SECURE_ERASE_MOCK_H_
#define _SEND_ATA_SECURE_ERASE_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AhciCommandLib.h>

#include "AtaPassThruProtocolMock.h"

/**
  Performs every action required to securely erase a given device

  @param[in] AtaDevice                  The pointer to the EFI_ATA_PASS_THRU_PROTOCOL.
  @param[in] Port                       Port number on the ATA controller
  @param[in] PortMultiplierPort         Port multiplier port number on the ATA controller
  @param[in] Identifier                 The identifier to set user or master password.
  @param[in] EraseMode                  Erase mode: normal or enhanced.
  @param[in] Timeout                    Time to wait in 100ns units
  @param[in] Password                   Drive password

  @retval    EFI_SUCCESS
  @retval    EFI_DEVICE_ERROR           The command has failed
  @retval    EFI_INVALID_PARAMETER      Port, PortMultiplierPort, or the contents of Acb are invalid. The ATA
                                        command was not sent, so no additional status information is available.
  @retval    EFI_NOT_READY              The ATA command could not be sent because there are too many ATA commands
                                        already queued. The caller may retry again later.
  @retval    EFI_TIMEOUT                Secure erase command takes longer to complete
                                        then the specified timeout
**/
EFI_STATUS
EFIAPI
SendAtaSecureErase (
  IN EFI_ATA_PASS_THRU_PROTOCOL        *AtaPassThru,
  IN UINT16                            Port,
  IN UINT16                            PortMultiplierPort,
  IN CHAR8                             Identifier,
  IN UINT8                             EraseMode,
  IN UINT64                            Timeout,
  IN CHAR8                             Password[ATA_PASSWORD_MAX_LENGTH]
  );

/**
  Unit test for SendAtaSecureErase.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSendAtaSecureErase (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_SEND_ATA_SECURE_ERASE_MOCK_H_*/
