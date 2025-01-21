/** @file
  Intel ACPI Sample Code for Connectivity Modules

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

#include <CnvBaseDef.h>

DefinitionBlock (
  "CnvSsdt.aml",
  "SSDT",
  2,
  "INTEL ",
  "Cnv_Ssdt",
  0x1000
  )
{
  External (ADBG, MethodObj)
  #define ADBG(x) \
  If (CondRefOf(ADBG)) {\
    ADBG(x) \
  }

  Include ("CnvPkgNvs.asl")
#if FixedPcdGetBool (PcdCnvIntegratedSupport) == 1
  Include ("Connectivity.asl")      // CNVi WiFi and PCI BT
#endif
#if FixedPcdGetBool (PcdCnvDiscreteSupport) == 1
  Include ("WifiRpPxsxWrapper.asl") // CNVd WiFi
#endif
#if FixedPcdGetBool (PcdBtUsbInterfaceSupport) == 1
  Include ("BtUsbPortWrapper.asl")  // CNVi/d USB BT
#endif
}