/**@file
    Unit test case mock header for AtaPrepareSecureErase.

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
#ifndef _PREPARE_SECURE_ERASE_MOCK_H_
#define _PREPARE_SECURE_ERASE_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AhciCommandLib.h>

#include "AtaPassThruProtocolMock.h"

/**
  Sends prepare secure erase ATA command

  @param[in]       AtaDevice                  The pointer to the EFI_ATA_PASS_THRU_PROTOCOL.
  @param[in]       Port                       Port number on the ATA controller
  @param[in]       PortMultiplierPort         Port multiplier port number on the ATA controller

  @retval          EFI_SUCCESS
  @retval          EFI_BAD_BUFFER_SIZE        The ATA command was not executed. The number of bytes that could be transferred
                                              is returned in InTransferLength. For write and bi-directional commands,
                                              OutTransferLength bytes were transferred by OutDataBuffer.
  @retval          EFI_NOT_READY              The ATA command could not be sent because there are too many ATA commands
                                              already queued. The caller may retry again later.
  @retval          EFI_DEVICE_ERROR           A device error occurred while attempting to send the ATA command.
  @retval          EFI_INVALID_PARAMETER      Port, PortMultiplierPort, or the contents of Acb are invalid. The ATA
                                              command was not sent, so no additional status information is available.

**/
EFI_STATUS
SataPrepareSecureErase (
  IN EFI_ATA_PASS_THRU_PROTOCOL    *AtaDevice,
  IN UINT16                        Port,
  IN UINT16                        PortMultiplierPort
  );

/**
  Unit test for AtaPrepareSecureErase.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAtaPrepareSecureErase (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_PREPARE_SECURE_ERASE_MOCK_H_*/
