/** @file
  SoundWire topology definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

    Name(_DSD, Package()
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package()
        {
            Package(2) {"mipi-sdw-sw-interface-revision", 0x00010002 }, // v 1.2.
            Package(2) {"mipi-sdca-interface-revision", 0x06}, // v 0.6
            Package(2) {"mipi-sdw-wake-up-unavailable", 0}, // support wakeup
            Package(2) {"mipi-sdw-test-mode-supported", 1}, // Integer/Boolean
            Package(2) {"mipi-sdw-clock-stop-mode1-supported", 1}, // Integer/Boolean
            Package(2) {"mipi-sdw-simplified-clockstopprepare-sm-supported", 1}, //Integer/Boolean
            Package(2) {"mipi-sdw-clockstopprepare-hard-reset-behavior", 0}, // Integer
            Package(2) {"mipi-sdw-highPHY-capable", 0}, // Integer/Boolean
            Package(2) {"mipi-sdw-paging-support", 1}, // Integer/Boolean
            Package(2) {"mipi-sdw-bank-delay-support", 0}, // Integer/Boolean
            Package(2) {"mipi-sdw-port15-read-behavior", 1}, // Integer
            Package(2) {"mipi-sdw-master-count", 0x0}, // Integer
            Package(2) {"mipi-sdw-source-port-list", 4}, // Integer, DP2
            Package(2) {"mipi-sdw-sink-port-list", 2}, // Integer, DP1
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Extension
        Package()
        {
            Package(2) {"mipi-sdw-dp-1-sink-subproperties", "P1SP"},
            Package(2) {"mipi-sdw-dp-2-source-subproperties", "P2SP"},
        }
    })  // End _DSD

    Name(P1SP, Package()
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package()
        {
            Package(2) {"mipi-sdw-port-max-wordlength", 24},
            Package(2) {"mipi-sdw-port-min-wordlength", 8},
            Package(2) {"mipi-sdw-port-wordlength-configs", Package() {0x8, 0x16, 0x20, 0x24} },
            Package(2) {"mipi-sdw-data-port-type", 0},
            Package(2) {"mipi-sdw-max-grouping-supported", 0},
            Package(2) {"mipi-sdw-simplified-channelprepare-sm", 0},
            Package(2) {"mipi-sdw-min-channel-number", 0x1},
            Package(2) {"mipi-sdw-max-channel-number", 0x2},
            Package(2) {"mipi-sdw-channel-combination-list", 0x3}, // only stereo channel are used
            Package(2) {"mipi-sdw-modes-supported", 0x0F},
            Package(2) {"mipi-sdw-max-async-buffer", 0x08 }, // need to confirm with designers
            Package(2) {"mipi-sdw-port-encoding-type", 0x02}
        }
    }) // End P1SP

    Name(P2SP, Package()
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package()
        {
            Package(2) { "mipi-sdw-port-max-wordlength", 24 },
            Package(2) {"mipi-sdw-port-min-wordlength", 8},
            Package(2) {"mipi-sdw-port-wordlength-configs", Package() {0x8, 0x16, 0x20, 0x24} },
            Package(2) {"mipi-sdw-data-port-type", 0},
            Package(2) {"mipi-sdw-max-grouping-supported", 0},
            Package(2) {"mipi-sdw-simplified-channelprepare-sm", 0},
            Package(2) {"mipi-sdw-min-channel-number", 0x1},
            Package(2) {"mipi-sdw-max-channel-number", 0x2},
            Package(2) {"mipi-sdw-channel-combination-list", 0x3}, // only stereo channel are used
            Package(2) {"mipi-sdw-modes-supported", 0x0F},
            Package(2) {"mipi-sdw-max-async-buffer", 0x08 }, // need to confirm with designers
            Package(2) {"mipi-sdw-port-encoding-type", 0x02}
        }
    }) // End P2SP

    Device(AF04)
    {
        Name (_ADR, 0x4)
        Include ("AudioFunction_1316_SPK1.asl")
    }