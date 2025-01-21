/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef __CPU_UNIT_TEST_FRU_H__
#define __CPU_UNIT_TEST_FRU_H__

#include <PiPei.h>
#include <Base.h>
#include <Register/Msr.h>
#include <Register/GenerationMsr.h>
#include <Register/Cpuid.h>
#include <Ppi/MpServices2.h>
#include <CpuUnitTestCapability.h>
#include <CpuRegs.h>

#include <CpuUnitTestRegisters.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/CpuPowerManagementLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuUnitTestInputInitLib.h>
#include <Library/CpuInitPreMem.h>
#include <Ppi/Wdt.h>
#include <Library/PeiOcLib.h>
#include <Library/PeiVrLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuUnitTestFruLib.h>
#include <Library/CpuUnitTestRegistersLib.h>

/**
  A clean up function of Unit Test test case.
  It will save unit test frame word data, and reset.
  Future boot will skip the test case.

  @param[in]  Context    [Optional] An optional parameter

**/
VOID
EFIAPI
CpuUnitTestReset (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Installs one or more config blocks into a ppi with the input guid.

  @param[in]  Guid              Guid of ppi where to install config blockes
  @param  ...                   A variable argument list containing pairs of function pointer of get config block size and add config block

  @retval EFI_SUCCESS           All the policy was installed.
  @retval EFI_INVALID_PARAMETER parameter is not legal.

**/
EFI_STATUS
EFIAPI
InitializeCpuUnitTestCpuPolicy (
  IN EFI_GUID  *Guid,
  ...
  );

typedef
UINT16
(EFIAPI *GET_CONFIG_BLOCK_SIZE)(
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *ADD_CONFIG_BLOCK)(
  IN     VOID      *ConfigBlockTableAddress
  );

/**
  Create UnitTest Suite for Core Count

  @param[in]   Framework  Unit test framework to add unit test suite to

  @retval  EFI_SUCCESS            The unit test suite was created.
**/
EFI_STATUS
EFIAPI
CreateUnitTestSuiteCoreCount (
  IN  UNIT_TEST_FRAMEWORK_HANDLE  Framework
  );

//
// Add more Test Case in the future....
//
#endif // __CPU_UNIT_TEST_FRU_H__
