/** @file
  This file contains the IPU SSDT Table ASL code.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <Include/AcpiDebug.h>

DefinitionBlock (
  "IpuSsdt.aml",
  "SSDT",
  2,
  "INTEL ",
  "IpuSsdt",
  0x1000
  )
{
External (\_SB.PC00, DeviceObj)
External (\_SB.PC00.GFX0, DeviceObj)
External (\_SB.PC00.IPU0, DeviceObj)
ACPI_DEBUG_EXTERNAL_REFERENCE
External (IPTP)

Include ("Ipu.asl")
}
