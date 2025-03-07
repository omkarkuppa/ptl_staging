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

    Name(_DSD, Package()
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package()
        {
        Package(2) {"mipi-sdw-sw-interface-revision", 0x00020001}, // v2.1

            // Peripheral Device configuration
        Package(2) {"mipi-sdw-sdca-interface-revision", 0x1000}, // v1.0r00

            Package(2) {"mipi-sdw-sdca-interrupt-register-list", 0xF},            // Bitmap
            Package(2) {"mipi-sdw-commit-register-supported", 1},                 // Boolean

            Package(2) {"mipi-sdw-wake-up-unavailable", 0}, // support wakeup
            Package(2) {"mipi-sdw-test-mode-supported", 1}, // Integer/Boolean
            Package(2) {"mipi-sdw-clock-stop-mode1-supported", 1}, // Integer/Boolean
            Package(2) {"mipi-sdw-simplified-clockstopprepare-sm-supported", 1}, //Integer/Boolean
            Package(2) {"mipi-sdw-clockstopprepare-hard-reset-behavior", 0}, // Integer
            Package(2) {"mipi-sdw-highPHY-capable", 0}, // Integer/Boolean
            Package(2) {"mipi-sdw-paging-supported", 1}, // Integer/Boolean
            Package(2) {"mipi-sdw-bank-delay-supported", 0}, // Integer/Boolean
            Package(2) {"mipi-sdw-port15-read-behavior", 1}, // Integer
            Package(2) {"mipi-sdw-manager-list", 0x0}, // Integer
      // Lane mapping
      // Package (2) {"mipi-sdw-lane-1-mapping", "mipi-sdw-manager-lane-1"},
            // Data Port configuration
            Package(2) {"mipi-sdw-source-port-list", 0x554}, // Bitmap: DP2, DP4, DP6, DP8, DP10
            Package(2) {"mipi-sdw-sink-port-list", 0x22}, // Bitmap: DP1, DP5
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Extension
        Package()
        {
            Package(2) {"mipi-sdw-dp-1-sink-subproperties", "P1SP"},
            Package(2) {"mipi-sdw-dp-5-sink-subproperties", "P5SP"},
            Package(2) {"mipi-sdw-dp-2-source-subproperties", "P2SP"},
            Package(2) {"mipi-sdw-dp-4-source-subproperties", "P4SP"},
            Package(2) {"mipi-sdw-dp-6-source-subproperties", "P6SP"},
            Package(2) {"mipi-sdw-dp-8-source-subproperties", "P8SP"},
            Package(2) {"mipi-sdw-dp-10-source-subproperties", "PASP"},
        }
    })  // End _DSD

    Name(P1SP, Package()
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package()
        {
            //Package(2) {"mipi-sdw-port-max-wordlength", 24},
            //Package(2) {"mipi-sdw-port-min-wordlength", 16},
            Package(2) {"mipi-sdw-port-wordlength-configs", Package() {16, 20, 24}},
            Package(2) {"mipi-sdw-data-port-type", 0},
            Package(2) {"mipi-sdw-max-grouping-supported", 0},
            Package(2) {"mipi-sdw-simplified-channelprepare-sm", 1},
            Package(2) {"mipi-sdw-min-channel-number", 0x0},
            Package(2) {"mipi-sdw-max-channel-number", 0x1},
            Package(2) {"mipi-sdw-channel-combination-list", Package() {0x3}}, // only stereo channel are used
            Package(2) {"mipi-sdw-modes-supported", 0x0F},
            Package(2) {"mipi-sdw-max-async-buffer", 0x08 }, // need to confirm with designers
            Package(2) {"mipi-sdw-port-encoding-type", 0x02},
      // Package(2) {"mipi-sdw-lane-list", Package() {0, 1}},
        }
    }) // End P1SP

    Name(P5SP, Package()
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package()
        {
            //Package(2) {"mipi-sdw-port-max-wordlength", 24},
            //Package(2) {"mipi-sdw-port-min-wordlength", 16},
            Package(2) {"mipi-sdw-port-wordlength-configs", Package() {16, 20, 24}},
            Package(2) {"mipi-sdw-data-port-type", 0},
            Package(2) {"mipi-sdw-max-grouping-supported", 0},
            Package(2) {"mipi-sdw-simplified-channelprepare-sm", 1},
            Package(2) {"mipi-sdw-min-channel-number", 0x0},
            Package(2) {"mipi-sdw-max-channel-number", 0x1},
            Package(2) {"mipi-sdw-channel-combination-list", Package() {0x3}}, // only stereo channel are used
            Package(2) {"mipi-sdw-modes-supported", 0x0F},
            Package(2) {"mipi-sdw-max-async-buffer", 0x08 }, // need to confirm with designers
            Package(2) {"mipi-sdw-port-encoding-type", 0x02},
      // Package(2) {"mipi-sdw-lane-list", Package() {0, 1}},
        }
    }) // End P5SP

    Name(P2SP, Package()
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package()
        {
            //Package(2) {"mipi-sdw-port-max-wordlength", 24},
            //Package(2) {"mipi-sdw-port-min-wordlength", 16},
            Package(2) {"mipi-sdw-port-wordlength-configs", Package() {16, 20, 24}},
            Package(2) {"mipi-sdw-data-port-type", 0},
            Package(2) {"mipi-sdw-max-grouping-supported", 0},
            Package(2) {"mipi-sdw-simplified-channelprepare-sm", 1},
            Package(2) {"mipi-sdw-min-channel-number", 0x0},
            Package(2) {"mipi-sdw-max-channel-number", 0x1},
            Package(2) {"mipi-sdw-channel-combination-list", Package() {0x3}}, // only stereo channel are used
            Package(2) {"mipi-sdw-modes-supported", 0x0F},
            Package(2) {"mipi-sdw-max-async-buffer", 0x08 }, // need to confirm with designers
            Package(2) {"mipi-sdw-port-encoding-type", 0x02},
      // Package(2) {"mipi-sdw-lane-list", Package() {0, 1}},
        }
    }) // End P2SP

    Name(P4SP, Package()
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package()
        {
            //Package(2) {"mipi-sdw-port-max-wordlength", 24},
            //Package(2) {"mipi-sdw-port-min-wordlength", 16},
            Package(2) {"mipi-sdw-port-wordlength-configs", Package() {16, 20, 24}},
            Package(2) {"mipi-sdw-data-port-type", 0},
            Package(2) {"mipi-sdw-max-grouping-supported", 0},
            Package(2) {"mipi-sdw-simplified-channelprepare-sm", 1},
            Package(2) {"mipi-sdw-min-channel-number", 0x0},
            Package(2) {"mipi-sdw-max-channel-number", 0x1},
            Package(2) {"mipi-sdw-channel-combination-list", Package() {0x3}}, // only stereo channel are used
            Package(2) {"mipi-sdw-modes-supported", 0x0F},
            Package(2) {"mipi-sdw-max-async-buffer", 0x08 }, // need to confirm with designers
            Package(2) {"mipi-sdw-port-encoding-type", 0x02},
      // Package(2) {"mipi-sdw-lane-list", Package() {0, 1}},
        }
    }) // End P4SP

    Name(P6SP, Package()
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package()
        {
            //Package(2) {"mipi-sdw-port-max-wordlength", 24},
            //Package(2) {"mipi-sdw-port-min-wordlength", 16},
            Package(2) {"mipi-sdw-port-wordlength-configs", Package() {16, 20, 24}},
            Package(2) {"mipi-sdw-data-port-type", 0},
            Package(2) {"mipi-sdw-max-grouping-supported", 0},
            Package(2) {"mipi-sdw-simplified-channelprepare-sm", 1},
            Package(2) {"mipi-sdw-min-channel-number", 0x0},
            Package(2) {"mipi-sdw-max-channel-number", 0x1},
            Package(2) {"mipi-sdw-channel-combination-list", Package() {0x3}}, // only stereo channel are used
            Package(2) {"mipi-sdw-modes-supported", 0x0F},
            Package(2) {"mipi-sdw-max-async-buffer", 0x08 }, // need to confirm with designers
            Package(2) {"mipi-sdw-port-encoding-type", 0x02},
      // Package(2) {"mipi-sdw-lane-list", Package() {0, 1}},
        }
    }) // End P6SP

    Name(P8SP, Package()
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package()
        {
            //Package(2) {"mipi-sdw-port-max-wordlength", 24},
            //Package(2) {"mipi-sdw-port-min-wordlength", 16},
            Package(2) {"mipi-sdw-port-wordlength-configs", Package() {16, 20, 24}},
            Package(2) {"mipi-sdw-data-port-type", 0},
            Package(2) {"mipi-sdw-max-grouping-supported", 0},
            Package(2) {"mipi-sdw-simplified-channelprepare-sm", 1},
            Package(2) {"mipi-sdw-min-channel-number", 0x0},
            Package(2) {"mipi-sdw-max-channel-number", 0x1},
            Package(2) {"mipi-sdw-channel-combination-list", Package() {0x3}}, // only stereo channel are used
            Package(2) {"mipi-sdw-modes-supported", 0x0F},
            Package(2) {"mipi-sdw-max-async-buffer", 0x08 }, // need to confirm with designers
            Package(2) {"mipi-sdw-port-encoding-type", 0x02},
      // Package(2) {"mipi-sdw-lane-list", Package() {1, 0}},
        }
    }) // End P8SP

    Name(PASP, Package()
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package()
        {
            //Package(2) {"mipi-sdw-port-max-wordlength", 24},
            //Package(2) {"mipi-sdw-port-min-wordlength", 16},
            Package(2) {"mipi-sdw-port-wordlength-configs", Package() {16, 20, 24}},
            Package(2) {"mipi-sdw-data-port-type", 0},
            Package(2) {"mipi-sdw-max-grouping-supported", 0},
            Package(2) {"mipi-sdw-simplified-channelprepare-sm", 1},
            Package(2) {"mipi-sdw-min-channel-number", 0x0},
            Package(2) {"mipi-sdw-max-channel-number", 0x1},
            Package(2) {"mipi-sdw-channel-combination-list", Package() {0x3}}, // only stereo channel are used
            Package(2) {"mipi-sdw-modes-supported", 0x0F},
            Package(2) {"mipi-sdw-max-async-buffer", 0x08 }, // need to confirm with designers
            Package(2) {"mipi-sdw-port-encoding-type", 0x02},
      // Package(2) {"mipi-sdw-lane-list", Package() {1, 0}},
        }
    }) // End PASP

    Device(AF04)
    {
        Name (_ADR, 0x4)
        Include ("AudioFunction_1320VBC_SPK1_ADR01.asl")
    }
