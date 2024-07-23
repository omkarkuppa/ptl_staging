/** @file
  SoundWire topology definitions

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

//Name(_DSD, Package() // Do not uncomment this line - for demonstration purposes only
ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), /* Device Properties for _DSD */
Package()
{
	//Package(2) {"mipi-sdw-bank-delay-supported", 0},                        // Boolean
	Package(2) {"mipi-sdw-clock-stop-mode1-supported", 0},                  // Boolean
	//Package(2) {"mipi-sdw-clockstopprepare-hard-reset-behavior", 0x0},      // Bitmap
	//Package(2) {"mipi-sdw-clockstopprepare-timeout", 500},                  // Integer
	Package(2) {"mipi-sdw-commit-register-supported", 1},                   // Boolean
	Package(2) {"mipi-sdw-dp-0-supported", 1},                              // Boolean
	//Package(2) {"mipi-sdw-highPHY-capable", 0},                             // Boolean
	//Package(2) {"mipi-sdw-manager-list", 0x0},                              // Bitmap
	Package(2) {"mipi-sdw-paging-supported", 1},                            // Boolean SCP_AddrPage1/2 are implemented
	//Package(2) {"mipi-sdw-peripheral-channelprepare-timeout", 50},          // Integer
	//Package(2) {"mipi-sdw-port15-read-behavior", 0},                        // Boolean
	Package(2) {"mipi-sdw-sdca-interface-revision", 0x0901},                // Integer: SDCA 0.9r01
	Package(2) {"mipi-sdw-sdca-interrupt-register-list", 0xf},              // Bitmap
	Package(2) {"mipi-sdw-simplified-clockstopprepare-sm-supported", 0},    // Boolean
	Package(2) {"mipi-sdw-sink-port-list", 0x6},                            // Bitmap: IT 21 = DP 1 & IT 26 = DP 2
	Package(2) {"mipi-sdw-source-port-list", 0x18},                         // Bitmap: OT 25 = DP 4 & OT 24 = DP 3
	Package(2) {"mipi-sdw-sw-interface-revision", 0x00020000},              // Integer: v2.0
	Package(2) {"mipi-sdw-test-mode-supported", 0},                         // Boolean
	Package(2) {"mipi-sdw-wake-up-unavailable", 0},                         // Boolean
},

ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
Package()
{
	Package(2) {"mipi-sdw-dp-0-subproperties", "PDP0"},                 // Package Name
	Package(2) {"mipi-sdw-dp-1-sink-subproperties", "PDPN"},            // Package Name
	Package(2) {"mipi-sdw-dp-2-sink-subproperties", "PDPN"},            // Package Name
	Package(2) {"mipi-sdw-dp-3-source-subproperties", "PDPN"},          // Package Name
	Package(2) {"mipi-sdw-dp-4-source-subproperties", "PDPN"},          // Package Name
},
//}) // Do not uncomment this line - for demonstration purposes only
