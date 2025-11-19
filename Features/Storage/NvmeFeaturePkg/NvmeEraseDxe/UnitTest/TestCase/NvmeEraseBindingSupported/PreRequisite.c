/**@file
    Unit test case Prerequisite of NvmeEraseBindingSupported.

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

#include "NvmeEraseBindingSupportedMock.h"
#include "TestCase/IsNvmeDevice/IsNvmeDeviceMock.h"

/**
  Unit-Test Mock Function.

  @param[in]  Context    Input context for mock.
**/
VOID
PreNvmeEraseBindingSupported_Mock (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_BOOT_SERVICES  *mBS;

  BINDING_SUPPORTED_MOCK *Parameter = (BINDING_SUPPORTED_MOCK*)Context;

  //
  // Mock boot service OpenProtocol
  //
  mBS = BootServiceConstructorMock ();
  mBS->OpenProtocol = MockBootServiceOpenProtocol;

  will_return (MockBootServiceOpenProtocol, Parameter->OpenBlockIoMock.Status);
  will_return (MockBootServiceOpenProtocol, Parameter->OpenBlockIoMock.BlockIO);
  if (Parameter->OpenBlockIoMock.Status == EFI_SUCCESS) {
    will_return (MockBootServiceOpenProtocol, Parameter->OpenNvmeEraseStsMock);
  }
}

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
  )
{
  EFI_STATUS             Status;
  EFI_STATUS             *Output;
  BINDING_SUPPORTED_MOCK *Parameter;

  //
  // Mock Input for OpenProtocol
  //
  Parameter = AllocateZeroPool (sizeof (BINDING_SUPPORTED_MOCK));
  Parameter->OpenBlockIoMock.Status  = EFI_UNSUPPORTED;
  Parameter->OpenBlockIoMock.BlockIO = NULL;

  //
  // Expect Output
  //
  Status = EFI_UNSUPPORTED;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    Parameter,                          // Mock context
    PreNvmeEraseBindingSupported_Mock,  // Mock function
    CleanUpNvmeEraseBindingSupported,   // CleanUp function
    (VOID *) Output                     // Expect Output
    );

  return UNIT_TEST_PASSED;
}

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
  )
{
  EFI_STATUS             Status;
  EFI_STATUS             *Output;
  EFI_BLOCK_IO_PROTOCOL  *BlockIo;
  BINDING_SUPPORTED_MOCK *Parameter;

  //
  // Mock Input for OpenProtocol
  //
  BlockIo = AllocateZeroPool (sizeof (EFI_BLOCK_IO_PROTOCOL));
  BlockIo->Media = AllocateZeroPool (sizeof (EFI_BLOCK_IO_MEDIA));
  BlockIo->Media->LogicalPartition = TRUE;

  Parameter = AllocateZeroPool (sizeof (BINDING_SUPPORTED_MOCK));
  Parameter->OpenBlockIoMock.Status  = EFI_SUCCESS;
  Parameter->OpenBlockIoMock.BlockIO = BlockIo;
  Parameter->OpenNvmeEraseStsMock    = EFI_SUCCESS;

  //
  // Expect Output
  //
  Status = EFI_ALREADY_STARTED;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    Parameter,                          // Mock context
    PreNvmeEraseBindingSupported_Mock,  // Mock function
    CleanUpNvmeEraseBindingSupported,   // CleanUp function
    (VOID *) Output                     // Expect Output
    );

  return UNIT_TEST_PASSED;
}

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
  )
{
  EFI_STATUS             Status;
  EFI_STATUS             *Output;
  EFI_BLOCK_IO_PROTOCOL  *BlockIo;
  BINDING_SUPPORTED_MOCK *Parameter;

  //
  // Mock Input for OpenProtocol
  //
  BlockIo = AllocateZeroPool (sizeof (EFI_BLOCK_IO_PROTOCOL));
  BlockIo->Media = AllocateZeroPool (sizeof (EFI_BLOCK_IO_MEDIA));
  BlockIo->Media->LogicalPartition = TRUE;

  Parameter = AllocateZeroPool (sizeof (BINDING_SUPPORTED_MOCK));
  Parameter->OpenBlockIoMock.Status  = EFI_SUCCESS;
  Parameter->OpenBlockIoMock.BlockIO = BlockIo;
  Parameter->OpenNvmeEraseStsMock    = EFI_UNSUPPORTED;

  //
  // Expect Output
  //
  Status = EFI_UNSUPPORTED;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    Parameter,                          // Mock context
    PreNvmeEraseBindingSupported_Mock,  // Mock function
    CleanUpNvmeEraseBindingSupported,   // CleanUp function
    (VOID *) Output                     // Expect Output
    );

  return UNIT_TEST_PASSED;
}

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
  )
{
  EFI_STATUS             Status;
  EFI_STATUS             *Output;
  EFI_BLOCK_IO_PROTOCOL  *BlockIo;
  BINDING_SUPPORTED_MOCK *Parameter;

  //
  // Mock Input for OpenProtocol
  //
  BlockIo = AllocateZeroPool (sizeof (EFI_BLOCK_IO_PROTOCOL));
  BlockIo->Media = AllocateZeroPool (sizeof (EFI_BLOCK_IO_MEDIA));
  BlockIo->Media->LogicalPartition = FALSE;

  Parameter = AllocateZeroPool (sizeof (BINDING_SUPPORTED_MOCK));
  Parameter->OpenBlockIoMock.Status  = EFI_SUCCESS;
  Parameter->OpenBlockIoMock.BlockIO = BlockIo;
  Parameter->OpenNvmeEraseStsMock    = EFI_UNSUPPORTED;

  PreRequisiteSetMock (
    Parameter,                          // Mock context
    PreNvmeEraseBindingSupported_Mock,  // Mock function
    CleanUpNvmeEraseBindingSupported,   // CleanUp function
    NULL                                // Expect Output
    );

  //
  // Mock IsNvmeDevice is FALSE
  //
  PreIsNvmeDevice_False (Context);

  //
  // Expect Output
  //
  Status = EFI_UNSUPPORTED;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));
  PreRequisiteSetOutput ((VOID*)Output);

  return UNIT_TEST_PASSED;
}

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
  )
{
  EFI_STATUS             Status;
  EFI_STATUS             *Output;
  EFI_BLOCK_IO_PROTOCOL  *BlockIo;
  BINDING_SUPPORTED_MOCK *Parameter;

  //
  // Mock Input for OpenProtocol
  //
  BlockIo = AllocateZeroPool (sizeof (EFI_BLOCK_IO_PROTOCOL));
  BlockIo->Media = AllocateZeroPool (sizeof (EFI_BLOCK_IO_MEDIA));
  BlockIo->Media->LogicalPartition = FALSE;

  Parameter = AllocateZeroPool (sizeof (BINDING_SUPPORTED_MOCK));
  Parameter->OpenBlockIoMock.Status  = EFI_SUCCESS;
  Parameter->OpenBlockIoMock.BlockIO = BlockIo;
  Parameter->OpenNvmeEraseStsMock    = EFI_UNSUPPORTED;

  PreRequisiteSetMock (
    Parameter,                          // Mock context
    PreNvmeEraseBindingSupported_Mock,  // Mock function
    CleanUpNvmeEraseBindingSupported,   // CleanUp function
    NULL                                // Expect Output
    );

  //
  // Mock IsNvmeDevice is TRUE
  //
  PreIsNvmeDevice_True (Context);

  //
  // Expect Output
  //
  Status = EFI_SUCCESS;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));
  PreRequisiteSetOutput ((VOID*)Output);

  return UNIT_TEST_PASSED;
}
