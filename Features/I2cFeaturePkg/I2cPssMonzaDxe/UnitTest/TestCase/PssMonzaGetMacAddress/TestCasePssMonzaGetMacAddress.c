/**@file
    Unit test case of the I2c Pss Monza Dxe driver.

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <I2cPssMonza.h>
#include "TestCasePssMonzaGetMacAddress.h"
#include "PrivateMock/PrivateMock.h"

const UINT8 UnknownPssMacAddress[] = {0, 0, 0, 0, 0, 0};
const UINT8 PssMacAddress[] = {144, 73, 250, 010, 36, 173}; // PSSMACAddress = 9049 FA0A 24AD
const UINT8 SizeofPssMacAddress = sizeof (PssMacAddress);
/**
  Get 16 bytes Mac Address from PSS chip.
  @param[in]       *This              Pointer of I2C_PLATFORM_SECURE_STORAGE_PROTOCOL.
  @param[in, out]  Buffer             Data to store Mac Address from PSS chip
  @retval  EFI_SUCCESS                Get Mac Address from PSS chip successfully.
  @retval  EFI_INVALID_PARAMETER      Buffer is NULL or necessary protocol is not exist.
  @retval  EFI_DEVICE_ERROR           PSS chip detect failure.
  @retval  EFI_NOT_FOUND              No identify PSS chip is found.
**/
extern
EFI_STATUS
EFIAPI
PssMonzaGetMacAddress (
  IN     I2C_PLATFORM_SECURE_STORAGE_PROTOCOL *This,
  IN OUT UINT16                                *Buffer
  );
/**
  Mock PssMonzaGetMacAddress
  @param[in]  Context                   [Optional] An optional parameter that enables:
                                        1) test-case reuse with varied parameters and
                                        2) test-case re-entry for Target tests that need a
                                        reboot.  This parameter is a VOID* and it is the
                                        responsibility of the test author to ensure that the
                                        contents are well understood by all test cases that may
                                        consume it.
  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestPssMonzaGetMacAddress (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PSS_MONZA_PRIVATE_CONTEXT                *This;
  UINT16                                    *Buffer;
  EFI_STATUS                                Status;
  UINT8                                     Index;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test1: Status will return EFI_INVALID_PARAMETER if parameter is NULL in the PssMonzaGetMacAddress().
  // Expected Result: EFI_INVALID_PARAMETER
  //

  // If This = NULL
  Buffer = AllocateZeroPool (sizeof (UINT16));
  Status = PssMonzaGetMacAddress (NULL, Buffer);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  // If This->I2cIoProtocol = NULL
  This = AllocateZeroPool (sizeof (PSS_MONZA_PRIVATE_CONTEXT));
  This->Signature = PSS_MONZA_SIGNATURE;
  Status = PssMonzaGetMacAddress (&(This->PssProtocol), Buffer);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  // If Buffer = NULL
  Buffer = NULL;
  This->I2cIoProtocol = AllocateZeroPool (sizeof (UINT16));
  Status = PssMonzaGetMacAddress (&(This->PssProtocol), Buffer);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
  //
  // Test2: If PSS Monza chip doesn't exist, Unit Test will return EFI_NOT_FOUND
  // Expected Result: EFI_NOT_FOUND
  //
  Buffer = AllocateZeroPool (sizeof (UINT16));
  This->I2cIoProtocol->QueueRequest = MockI2cBusQueueRequest;
  // Mock incorrect Pss chip Class Id [0x0, 0x0, 0x0, 0x0] to I2cIoProtocol->QueueRequest
  will_return (MockI2cBusQueueRequest, SizeofPssChipClassId);
  for (Index = 0; Index < SizeofPssChipClassId; Index++) {
    will_return (MockI2cBusQueueRequest, UnknownPssChipClassId[Index]);
  }
  will_return (MockI2cBusQueueRequest, EFI_UNSUPPORTED);
  Status = PssMonzaGetMacAddress (&(This->PssProtocol), Buffer);
  UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);
  //
  // Test3-1: If PSS Monza chip exist but PSS read Failed, Unit Test will return EFI_DEVICE_ERROR
  // Expected Result: EFI_DEVICE_ERROR
  //
  Buffer = AllocateZeroPool (sizeof (UINT16));
  // Mock correct Pss chip Class Id [0xE2, 0x80 , 0x11 , 0x50] to I2cIoProtocol->QueueRequest.
  will_return (MockI2cBusQueueRequest, SizeofPssChipClassId);
  for (Index = 0; Index < SizeofPssChipClassId; Index++) {
    will_return (MockI2cBusQueueRequest, PssChipClassId[Index]);
  }
  will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
  // Mock incorrect Pss Mac Address to I2cIoProtocol->QueueRequest.
  will_return (MockI2cBusQueueRequest, SizeofPssMacAddress);
  for (Index = 0; Index < SizeofPssMacAddress; Index++) {
    will_return (MockI2cBusQueueRequest, UnknownPssMacAddress[Index]);
  }
  will_return (MockI2cBusQueueRequest, EFI_UNSUPPORTED);
  Status = PssMonzaGetMacAddress (&(This->PssProtocol), Buffer);
  UT_ASSERT_EQUAL (Status, EFI_DEVICE_ERROR);
  //
  // Test3-2: If PSS read success but no valid Mac Address is found, Unit Test will return EFI_NOT_FOUND
  // Expected Result: EFI_NOT_FOUND
  //
  Buffer = AllocateZeroPool (sizeof (UINT16) * PSS_CHIP_MAC_ADDRESS_LENGTH);
  will_return (MockI2cBusQueueRequest, SizeofPssChipClassId);
  for (Index = 0; Index < SizeofPssChipClassId; Index++) {
    will_return (MockI2cBusQueueRequest, PssChipClassId[Index]);
  }
  will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
  // Mock Pss Mac Address = 0 to I2cIoProtocol->QueueRequest. It will still be true for PssMonzaGetMacAddress ().
  will_return (MockI2cBusQueueRequest, 0);
  will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
  Status = PssMonzaGetMacAddress (&(This->PssProtocol), Buffer);
  UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);
  //
  // Test4: Get 16 bytes Mac Address from PSS chip.
  // Expected Result: EFI_SUCCESS
  //
  Buffer = AllocateZeroPool (sizeof (UINT16) * PSS_CHIP_MAC_ADDRESS_LENGTH);
  // Mock correct Pss chip Class Id [0xE2, 0x80 , 0x11 , 0x50] to I2cIoProtocol->QueueRequest.
  will_return (MockI2cBusQueueRequest, SizeofPssChipClassId);
  for (Index = 0; Index < SizeofPssChipClassId; Index++) {
    will_return (MockI2cBusQueueRequest, PssChipClassId[Index]);
  }
  will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
  // Mock correct Pss Mac Address "9049 FA0A 24AD" to I2cIoProtocol->QueueRequest.
  will_return (MockI2cBusQueueRequest, SizeofPssMacAddress);
  for (Index = 0; Index < SizeofPssMacAddress; Index++) {
    will_return (MockI2cBusQueueRequest, PssMacAddress[Index]);
  }
  will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
  Status = PssMonzaGetMacAddress (&(This->PssProtocol), Buffer);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  FreePool (Buffer);
  FreePool (This);
  return UNIT_TEST_PASSED;
}