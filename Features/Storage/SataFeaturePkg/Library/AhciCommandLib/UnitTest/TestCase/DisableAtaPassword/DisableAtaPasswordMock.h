/**@file
    Unit test case mock header for DisableAtaPassword.

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
#ifndef _DISABLE_ATA_PASSWORD_MOCK_H_
#define _DISABLE_ATA_PASSWORD_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AhciCommandLib.h>

#include "AtaPassThruProtocolMock.h"

/**
  Send disable password cmd through Ata Pass Thru Protocol.

  @param[in] AtaPassThru         The pointer to the ATA_PASS_THRU protocol.
  @param[in] Port                The port number of the ATA device to send the command.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                 If there is no port multiplier, then specify 0xFFFF.
  @param[in] Identifier          The identifier to set user or master password.
  @param[in] Password            The hdd password of attached ATA device.

  @retval EFI_SUCCESS            Successful to disable hdd password cmd.
  @retval EFI_INVALID_PARAMETER  The parameter passed-in is invalid.
  @retval EFI_OUT_OF_RESOURCES   Not enough memory to disable hdd password cmd.
  @retval EFI_DEVICE_ERROR       Can not disable hdd password cmd.

**/
EFI_STATUS
EFIAPI
DisableAtaPassword (
  IN EFI_ATA_PASS_THRU_PROTOCOL     *AtaPassThru,
  IN UINT16                         Port,
  IN UINT16                         PortMultiplierPort,
  IN CHAR8                          Identifier,
  IN CHAR8                          Password[ATA_PASSWORD_MAX_LENGTH]
  );

/**
  Unit test for DisableAtaPassword.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestDisableAtaPassword (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_DISABLE_ATA_PASSWORD_MOCK_H_*/
