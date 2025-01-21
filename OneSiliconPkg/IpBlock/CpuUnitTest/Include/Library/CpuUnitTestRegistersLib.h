/** @file

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

@par Specification Reference:
**/

#ifndef _CPU_UNIT_TEST_REGISTERS_LIB_H_
#define _CPU_UNIT_TEST_REGISTERS_LIB_H_

#include <Uefi.h>
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <CpuUnitTestRegisters.h>
#include <CpuUnitTestCapability.h>

/**
  Init registers such as mailbox, msr, mmio base on input CpuCapability.

  @param[in]  CpuCapability    Cpu Capability
**/
EFI_STATUS
EFIAPI
CpuUnitTestInitializeRegisters (
  CONST CPU_UNIT_TEST_CPU_CAPABILITY  *CpuCapability
  );

/**
  Get the CpuUnitTest register global varaible

  @return      A pointer to register global varaible
**/
CPU_UNIT_TEST_REGISTERS *
EFIAPI
CpuUnitTestGetRegisters (
  VOID
  );

#endif
