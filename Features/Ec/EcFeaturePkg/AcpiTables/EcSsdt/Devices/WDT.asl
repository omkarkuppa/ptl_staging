/** @file
  ACPI EC SSDT table for WDT device.

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

Device (WDT0)  // WDT Device Resource Consumption
{
  Name (_HID, EISAID ("PNP0C02"))

  Name (_UID, 3)

  Name (_CRS, ResourceTemplate ()
  {
    IO (Decode16, 0x6A4, 0x6A4, 0x1, 0x1)  // 1 Byte EC Prv Intfc.
    IO (Decode16, 0x6A0, 0x6A0, 0x1, 0x1)  // 1 Byte EC Prv Intfc.
  })
}
