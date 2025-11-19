/**@file
    Unit test case of the I2c Pss Monza Dxe driver.

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <I2cPssMonza.h>
#include "TestCasePssMonzaDeleteRequestPacket.h"
#include <Library/MemoryAllocationLib.h>

/**
  Delete I2C request packet.

  @param[in] Packet             I2C request packet that you want to delete.

**/
extern
VOID
PssMonzaDeleteRequestPacket (
  IN EFI_I2C_REQUEST_PACKET *Packet
  );
/**
  Mock PssMonzaDeleteRequestPacket

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
UnitTestPssMonzaDeleteRequestPacket (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_I2C_REQUEST_PACKET            *Packet;
  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test1: Check Packet is NULL will return NULL
  //
  Packet = NULL;
  PssMonzaDeleteRequestPacket (Packet);
  UT_ASSERT_EQUAL (Packet, NULL);
  //
  // Test2: Check Packet isn't NULL will free memory space.
  //
  Packet = AllocateZeroPool (sizeof (EFI_I2C_REQUEST_PACKET));
  // check value will free after PssMonzaDeleteRequestPacket.
  PssMonzaDeleteRequestPacket (Packet);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  return UNIT_TEST_PASSED;
}