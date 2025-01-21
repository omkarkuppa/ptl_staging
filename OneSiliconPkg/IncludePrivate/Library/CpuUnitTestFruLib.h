/** @file
  CPU Unit Test product code

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

@par Specification Reference:
**/

#ifndef __CPU_UNIT_TEST_PRODUCT_H__
#define __CPU_UNIT_TEST_PRODUCT_H__

#include <CpuUnitTestCapability.h>
#include <Library/UnitTestLib.h>
#include <CpuUnitTestFruData.h>

/**
  Modify CpuUnitTest CpuCapability CpuBasic based on product requirement.

  @param  CpuCapabilityCpuBasic    A pointer to a buffer containing CpuCapability.CpuBasic. Caller need to allocate buffer.
  @param  Size                     Size of the buffer.
  @param  Index                    The index of the CpuBasic Capability.

  @return VOID
**/
VOID
EFIAPI
InitializeCpuBasicCapabilityFru (
  IN OUT CPU_UNIT_TEST_BASIC  *CpuCapabilityCpuBasic,
  IN     UINTN                Size,
  IN     UINTN                Index
  );

/**
  Init registers such as mailbox, msr, mmio based on product requirement.

  @param[in]  CpuCapability    Cpu Capability
**/
EFI_STATUS
EFIAPI
CpuUnitTestInitializeRegistersFru (
  CONST CPU_UNIT_TEST_CPU_CAPABILITY  *CpuCapability
  );

/**
  Get product specific data via Fru

  @param  CpuFruData    A pointer to CPU_UNIT_TEST_FRU_DATA structure.

  @return VOID
**/
VOID
EFIAPI
CpuUnitTestGetFruData (
  OUT CPU_UNIT_TEST_FRU_DATA  *CpuFruData
  );

/**
  Create UnitTest Suite for specific product

  @param[in]   Framework  Unit test framework to add unit test suite to

  @retval  EFI_SUCCESS            The unit test suite was created.
**/
EFI_STATUS
EFIAPI
CreateCpuUnitTestSuiteFru (
  IN  UNIT_TEST_FRAMEWORK_HANDLE  Framework
  );

#endif
