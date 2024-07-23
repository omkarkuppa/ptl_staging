/**@file
    Unit test case mock header for NvmeEraseBindingSupported.

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
#ifndef _NVME_ERASE_BINDING_SUPPORT_MOCK_H_
#define _NVME_ERASE_BINDING_SUPPORT_MOCK_H_

#include <setjmp.h>
#include <cmocka.h>

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include "PreRequisite.h"

#include <Protocol/NvmExpressPassthru.h>
#include <Protocol/BlockIo.h>
#include <Library/UefiLib.h>
#include "BootServicesMock.h"

//
// Define Input parameter for NvmeEraseBindingSupported Unit Test.
//
typedef struct {
  EFI_STATUS            Status;     // Status of opening BlockIoProtocol
  EFI_BLOCK_IO_PROTOCOL *BlockIO;   // Point to the BlockIoProtocol
} OPEN_BLOCKIO_MOCK;

typedef struct {
  OPEN_BLOCKIO_MOCK     OpenBlockIoMock;        // Mock of opening BlockIoProtocol
  EFI_STATUS            OpenNvmeEraseStsMock;   // Status of opening NvmeEraseBlockProtocol
} BINDING_SUPPORTED_MOCK;

/**
  Unit-Test Prerequisite Function pointer.

  gEfiBlockIoProtocolGuid does not support.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeEraseBindingSupported_1 (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  gEfiBlockIoProtocolGuid does support, gNvmeEraseBlockProtocolGuid has installed.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeEraseBindingSupported_2 (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  gEfiBlockIoProtocolGuid does support and it is Logical Partition
  gNvmeEraseBlockProtocolGuid does not install

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeEraseBindingSupported_3 (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  gEfiBlockIoProtocolGuid does support, it is not Logical Partition and not Nvme device,
  gNvmeEraseBlockProtocolGuid does not install

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeEraseBindingSupported_4 (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  gEfiBlockIoProtocolGuid does support, it is not Logical Partition and it is Nvme device
  gNvmeEraseBlockProtocolGuid does not install

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeEraseBindingSupported_5 (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test NvmeEraseBindingSupported CleanUp Function. Clean the parameter for mock

  @param[in]  Context    Input context for mock.

**/
VOID
EFIAPI
CleanUpNvmeEraseBindingSupported (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Tests to see if NvmeEraseDxe supports a given controller.

  This function checks to see if the controller contains an instance of the
  EFI_BLOCK_IO_PROTOCOL and returns EFI_SUCCESS if it does.

  @param[in]  This                  A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle      The Handle of the controller to test. This Handle
                                    must support a protocol interface that supplies
                                    an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  This parameter is ignored.

  @retval EFI_SUCCESS               The device contains required protocols
  @retval EFI_ALREADY_STARTED       The device specified by ControllerHandle and
                                    RemainingDevicePath is already being managed by the driver
                                    specified by This.
  @retval EFI_ACCESS_DENIED         The device specified by ControllerHandle and
                                    RemainingDevicePath is already being managed by a different
                                    driver or an application that requires exclusive access.
                                    Currently not implemented.
  @retval EFI_UNSUPPORTED           The device does not contain requires protocols

**/
EFI_STATUS
EFIAPI
NvmeEraseBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL*  This,
  IN EFI_HANDLE                    Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*     RemainingDevicePath
  );

/**
  Unit test for NvmeEraseBindingSupported.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestNvmeEraseBindingSupported (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_NVME_ERASE_BINDING_SUPPORT_MOCK_H_*/
