/** @file
  Intel ACPI Reference Code for CNV Guid Lock Status ACPI Indicator

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

@par Specification Reference:
**/

Name (GLAX, Package ()
{
  0,                      // Revision, DWordConst
  Package ()              // Configuration for WiFi
  {
    0x07,                 // DomainType, 0x7:WiFi
    0,                    // Value:  0
  },
  Package ()               // Configuration for Bluetooth
  {
    0x12,                 // DomainType, 0x12:Bluetooth Core
    0,                    // Value:  0
  }
})
// GLAI (CNV Guid Lock Status ACPI Indicator)
//
// The general purpose configuration method allows a general purpose configuration/communication
// between BIOS and CNV Core's drivers. It allows OEM specific & platform specific configuration
// notifications to Core's drivers.
//
// Arguments: (0)
//   None
// Return Value:
//
// Name (GLAX, Package()
// {
//   0,                      // Revision, DWordConst
//   Package()               // Configuration for WiFi or BT
//   {
//     DomainType,           // 0x7:WiFi  Core, 0x12:Bluetooth Core
//     Value,                // The value/data of the type
//   },
//   Package()               // Configuration for WiFi or BT
//   {
//     DomainType,           // 0x7:WiFi  Core, 0x12:Bluetooth Core
//     Value,                // The value/data of the type
//   },
// })
//
Method (GLAI, 0, Serialized)
{
  ADBG (Concatenate ("GLAI method. CGLS = ", ToHexString (\CGLS)))
  Store (\CGLS, Index (DeRefOf (Index (GLAX, 1)), 1))
  Store (\CGLS, Index (DeRefOf (Index (GLAX, 2)), 1))

  Return (GLAX)
}