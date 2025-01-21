/** @file
  Header file for ACPI ADBG enable/disable.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _ACPI_DEBUG_H_
#define _ACPI_DEBUG_H_

#if FixedPcdGetBool(PcdAcpiDebugEnableFlag) == 1  //PcdAcpiDebugEnableFlag can be updated in BoardPkgPcdInit.dsc
  #define ADBG(x) ADBG(x)  //leave it as it is
  #define ASRT(x, y) ASRT(x, y)
  #define ACPI_DEBUG_EXTERNAL_REFERENCE External(\ADBG, MethodObj)   //additional define statement to cover ADBG external references
  #define ACPI_ASSERT_EXTERNAL_REFERENCE External(\_SB.ASRT, MethodObj)
#else
  #define ADBG(x)  //remove all ADBG and ASRT references
  #define ASRT(x, y)
  #define ACPI_DEBUG_EXTERNAL_REFERENCE //remove all external references for ADBG and ASRT
  #define ACPI_ASSERT_EXTERNAL_REFERENCE
#endif
#endif
