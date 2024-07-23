/** @file
  ACPI EC SSDT table for Docking Indicator.

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

Device (DIND)  // Docking Indicators.
{
  External (\OSYS, IntObj)

  Name (_HID, "INT33D4")
  Name (_CID, "PNP0C70")
  Method (_STA, 0, Serialized)
  {
    If (LAnd (And (IUDE, 1), LGreaterEqual (\OSYS, 2012)))
    {
      Return (0x000F)
    }
    Return (0x00)
  }
}
