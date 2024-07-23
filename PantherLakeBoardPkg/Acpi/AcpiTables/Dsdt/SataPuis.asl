/** @file
  ACPI Sata PUIS Enablement

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

Name (GTF0, Buffer (0x7) {0x00,0x00,0x00,0x00,0x00,0xA0,0x00})

//
// Set Device Data Method
// Arg0 - A Buffer containing an ATA drive identify block, contents described by the ATA
//
Method (_SDD, 0x1, NotSerialized) {
  CreateByteField (GTF0, 0x0, FEAT)
  CreateByteField (GTF0, 0x6, CMMD)
  CreateWordField (Arg0, 0xA6, W083)    // Identify Device Data Word 83
  CreateWordField (Arg0, 0xAC, W086)    // Identify Device Data Word 86

  If (LEqual (SizeOf (Arg0), 0x200)) {  // 256 WORD Data Block by OSPM to send 0xEC Command to Device
    // Check if PUIS feature is supported (Word 83 Bit5)
    If (LEqual (And (W083, 0x0020), 0x0020)) {
      // Check enable state (Word 86 Bit5)
      If (LEqual (And (W086, 0x0020), 0)) {
        // Issue Set Feature EFh, subcommand 06h to enable PUIS
        Store (0x06, FEAT)
        Store (0xEF, CMMD)
      }
    }
  }
}

//
// Get Task File Method
//
// @return 0 - A Buffer containing a byte stream of ATA commands for the drive
//
Method (_GTF, 0, NotSerialized) {
  return (GTF0)

}
