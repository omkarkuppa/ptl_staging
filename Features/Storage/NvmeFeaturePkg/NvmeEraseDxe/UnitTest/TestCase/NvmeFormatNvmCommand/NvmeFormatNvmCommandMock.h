/**@file
    Unit test case mock header for NvmeFormatNvmCommand.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _FORMAT_NVME_CMD_MOCK_H_
#define _FORMAT_NVME_CMD_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include "PreRequisite.h"

#include "NvmePassThruProtocolMock.h"

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreFormatNvmCommand_S (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreFormatNvmCommand_F (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test NvmeFormatNvmCommand CleanUp Function. Clean the parameter for mock

  @param[in]  Context    Input context for mock.

**/
VOID
CleanUpFormatNvmCommand (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Perform Nvme Format NVM command.

  @param[in] PassThruProtocol     The pointer to the instance of NVME_PASS_THRU_PROTOCOL.
  @param[in] NamespaceId          Namespace Id for an NVM Express namespace present on the NVM Express controller
  @param[in] SecureEraseSettings  Type of the Secure Erase operation

  @retval EFI_SUCCESS            Successfully performed secure erase operation.
  @retval EFI_NOT_READY          The NVM Express Command Packet could not be sent because the controller is not ready.
  @retval EFI_DEVICE_ERROR       A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER  NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid.
  @retval EFI_UNSUPPORTED        The command described by the NVM Express Command Packet is not supported by the NVM Express
  @retval EFI_TIMEOUT            A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
NvmeFormatNvmCommand (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *PassThruProtocol,
  IN UINT32                                NameSpaceId,
  IN UINT32                                SecureEraseSettings
  );

/**
  Unit test for NvmeFormatNvmCommand.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestFormatNvmCommand_S (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit test for NvmeFormatNvmCommand.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestFormatNvmCommand_F (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_FORMAT_NVME_CMD_MOCK_H_*/
