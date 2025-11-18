/** @file
  SoundWire topology definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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


Name(PDP0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdw-port-wordlength-configs", Package() {8,16,24}},   // Package
        Package(2) {"mipi-sdw-port-bra-mode-list", 0x1},                        // Bitmap
        Package(2) {"mipi-sdw-bra-flow-controlled", 1},                         // Boolean
        Package(2) {"mipi-sdw-bra-imp-def-response-supported", 1},              // Boolean
        Package(2) {"mipi-sdw-bra-role-supported", 1},                          // Boolean
        //Package(2) {"mipi-sdw-port-channelprepare-timeout", 0},                 // Integer
        Package(2) {"mipi-sdw-simplified-channelprepare-sm", 0},                // Boolean
        Package(2) {"mipi-sdw-imp-def-dp0-interrupts-supported", 0},            // Boolean
        Package(2) {"mipi-sdw-imp-def-bpt-supported", 0},                       // Boolean/
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdw-port-bra-mode-0", "BRA0"},                        // Package
    },
}) //End PDP0

Name(BRA0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        //Package(2) {"mipi-sdw-bra-mode-bus-frequency-configs", package() {}}, // Package
        Package(2) {"mipi-sdw-bra-mode-max-data-per-frame", 470},               // Integer
        //Package(2) {"mipi-sdw-bra-mode-min-us-between-transactions", 0},        // Integer
        //Package(2) {"mipi-sdw-bra-mode-max-bandwidth", 0},                      // Integer
        //Package(2) {"mipi-sdw-bra-mode-block-alignment", 4},                    // Integer: 4B alignment
    },
}) //End BRA0

Name(PDPN, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdw-port-wordlength-configs", Package() {8,16,24}},   // Package
        Package(2) {"mipi-sdw-data-port-type", 0},                              // Integer
        Package(2) {"mipi-sdw-channel-number-list", Package() {0, 1} },         // Integer: Allow channels 0 and 1, may need to update this on a per port basis.
        Package(2) {"mipi-sdw-channel-combination-list", Package() {0x3, 0x2, 0x1}},    // Package
        Package(2) {"mipi-sdw-port-encoding-type", 0x01},                       // Bitmap: PCM twos compliment
        Package(2) {"mipi-sdw-modes-supported", 0x01},                          // Bitmap: Isochronous mode only
        Package(2) {"mipi-sdw-max-async-buffer", 0},                            // Integer: Do not buffer
        Package(2) {"mipi-sdw-block-packing-mode ", 1},                         // Boolean: Block packing modes are supported
        Package(2) {"mipi-sdw-max-grouping-supported", 0},                      // Integer
        Package(2) {"mipi-sdw-simplified-channelprepare-sm", 0},                // Boolean
        //Package(2) {"mipi-sdw-port-channelprepare-timeout", 0},                 // Integer
        //Package(2) {"mipi-sdw-imp-def-dpn-interrupts-supported", 0x0},          // Bitmap
        //Package(2) {"mipi-sdw-block-packing-mode", 0},                          // Bitmap
    },
}) //End PDPN
