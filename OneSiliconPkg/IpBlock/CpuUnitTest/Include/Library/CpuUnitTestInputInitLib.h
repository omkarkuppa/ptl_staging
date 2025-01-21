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

#ifndef _CPU_UNIT_TEST_INPUT_INIT_LIB_
#define _CPU_UNIT_TEST_INPUT_INIT_LIB_

/**
  Initialize the mock CPU using the capability index.
  If the CpuCapabilityIndex is NULL, return the default CPU capablility

  @param  CpuCapability       A pointer to a buffer containing CpuCapability. Caller need to allocate buffer.
  @param  CpuCapabilityIndex  The Index arry of the CpuCapability. Null means default CpuCapability.

  @return VOID
**/
VOID
EFIAPI
CpuUnitTestInitializeCpuCapability (
  OUT CPU_UNIT_TEST_CPU_CAPABILITY  *CpuCapability,
  IN  UINTN                         *CpuCapabilityIndex OPTIONAL
  );

#endif // _CPU_UNIT_TEST_INPUT_INIT_LIB_
