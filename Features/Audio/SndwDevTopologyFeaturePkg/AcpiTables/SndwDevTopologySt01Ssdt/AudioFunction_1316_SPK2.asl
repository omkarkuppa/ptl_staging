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
        { // Function Descriptor
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) {"mipi-sdw-sw-interface-revision", 0x00010001},
            Package(2) {"msft-sdca-spec-revision", 0x4},
            Package(2) {"mipi-sdca-function-topology-features", 0xB00},  // No secondary transducer, ultrasound render, DtoD stream
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x5, 0x6, 0x7, 0x8} },  // Function level controls
            Package(2) {"mipi-sdca-entity-id-list", Package() {0x1, 0x2, 0x3, 0x33, 0x22, 0x29, 0x54, 0x55, 0x31, 0x56, 0x51, 0x34, 0x24, 0x52, 0x53, 0x21, 0x40} },  // List of entities in this function
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-controlselector-0x4-subproperties", "C004" },
            Package(2) {"mipi-sdca-controlselector-0x5-subproperties", "C005"},
            Package(2) {"mipi-sdca-controlselector-0x6-subproperties", "C006"},
            Package(2) {"mipi-sdca-controlselector-0x7-subproperties", "C007"},
            Package(2) {"mipi-sdca-controlselector-0x8-subproperties", "C008"},
            Package(2) {"mipi-sdca-entity-id-0x1-subproperties", "E001"},
            Package(2) {"mipi-sdca-entity-id-0x2-subproperties", "E002"},
            Package(2) {"mipi-sdca-entity-id-0x3-subproperties", "E003"},
            Package(2) {"mipi-sdca-entity-id-0x33-subproperties", "E033"},
            Package(2) {"mipi-sdca-entity-id-0x22-subproperties", "E022"},
            Package(2) {"mipi-sdca-entity-id-0x29-subproperties", "E029"},
            Package(2) {"mipi-sdca-entity-id-0x54-subproperties", "E054"},
            Package(2) {"mipi-sdca-entity-id-0x55-subproperties", "E055"},
            Package(2) {"mipi-sdca-entity-id-0x31-subproperties", "E031"},
            Package(2) {"mipi-sdca-entity-id-0x56-subproperties", "E056"},
            Package(2) {"mipi-sdca-entity-id-0x51-subproperties", "E051"},
            Package(2) {"mipi-sdca-entity-id-0x34-subproperties", "E034"},
            Package(2) {"mipi-sdca-entity-id-0x24-subproperties", "E024"},
            Package(2) {"mipi-sdca-entity-id-0x52-subproperties", "E052"},
            Package(2) {"mipi-sdca-entity-id-0x53-subproperties", "E053"},
            Package(2) {"mipi-sdca-entity-id-0x21-subproperties", "E021"},
            Package(2) {"mipi-sdca-entity-id-0x40-subproperties", "E040"},
            Package(2) {"mipi-sdca-function-expansion-subproperties", "EXT0"},
          },
          ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
          Package()
          {
            Package(2) {"mipi-sdca-function-initialization-table","BUF0"},
            Package (2) {"mipi-sdca-function-supported-postures", "BUF1"},
          }
        }) //End AF04._DSD

        Name(BUF0, Buffer()
        {
          // ALC1316 SdW initial pattern
          // Version: 18
          // Date: 2021-01/21 (released)
          0x20, 0xC7, 0x00, 0x00, 0x17,  // 0x0000_C720 17
          0x21, 0xC7, 0x00, 0x00, 0x00,  // 0x0000_C721 00
          0x22, 0xC7, 0x00, 0x00, 0x3e,  // 0x0000_C722 3e
          0x23, 0xC7, 0x00, 0x00, 0x06,  // 0x0000_C723 06
          0x24, 0xC7, 0x00, 0x00, 0x80,  // 0x0000_C724 80
          0x25, 0xC7, 0x00, 0x00, 0x06,  // 0x0000_C725 06
          0x02, 0xC7, 0x00, 0x00, 0x0A,  // 0x0000_C702 0A
          0x03, 0xC7, 0x00, 0x00, 0x0A,  // 0x0000_C703 0A
          0x01, 0xC0, 0x00, 0x00, 0x45,  // 0x0000_C001 45
          0x03, 0xC0, 0x00, 0x00, 0x00,  // 0x0000_C003 00
          0x04, 0xC0, 0x00, 0x00, 0x11,  // 0x0000_C004 11
          0x05, 0xC0, 0x00, 0x00, 0x00,  // 0x0000_C005 00
          0x06, 0xC0, 0x00, 0x00, 0x00,  // 0x0000_C006 00
          0x06, 0xC1, 0x00, 0x00, 0x00,  // 0x0000_C106 00
          0x07, 0xC0, 0x00, 0x00, 0x11,  // 0x0000_C007 11
          0x08, 0xC0, 0x00, 0x00, 0x11,  // 0x0000_C008 11
          0x09, 0xC0, 0x00, 0x00, 0x00,  // 0x0000_C009 00
          0x10, 0xC7, 0x00, 0x00, 0x17,  // 0x0000_C710 17
          0x11, 0xC7, 0x00, 0x00, 0x80,  // 0x0000_C711 80
          0x12, 0xC7, 0x00, 0x00, 0x26,  // 0x0000_C712 26
          0x13, 0xC7, 0x00, 0x00, 0x06,  // 0x0000_C713 06
          0x14, 0xC7, 0x00, 0x00, 0x80,  // 0x0000_C714 80
          0x15, 0xC7, 0x00, 0x00, 0x06,  // 0x0000_C715 06
          0x30, 0xC7, 0x00, 0x00, 0x06,  // 0x0000_C730 06
          0x31, 0xC7, 0x00, 0x00, 0x00,  // 0x0000_C731 00
          0x32, 0xC7, 0x00, 0x00, 0x3E,  // 0x0000_C732 3E
          0x40, 0xC7, 0x00, 0x00, 0x13,  // 0x0000_C740 13
          0x41, 0xC7, 0x00, 0x00, 0x80,  // 0x0000_C741 80
          0x42, 0xC7, 0x00, 0x00, 0x30,  // 0x0000_C742 30
          0x43, 0xC7, 0x00, 0x00, 0x06,  // 0x0000_C743 06
          0x44, 0xC7, 0x00, 0x00, 0x05,  // 0x0000_C744 05
          0x45, 0xC7, 0x00, 0x00, 0x1E,  // 0x0000_C745 1E
          0x50, 0xC7, 0x00, 0x00, 0x17,  // 0x0000_C750 17
          0x51, 0xC7, 0x00, 0x00, 0x00,  // 0x0000_C751 00
          0x52, 0xC7, 0x00, 0x00, 0x2E,  // 0x0000_C752 2E
          0x53, 0xC7, 0x00, 0x00, 0x06,  // 0x0000_C753 06
          0x54, 0xC7, 0x00, 0x00, 0x80,  // 0x0000_C754 80
          0x55, 0xC7, 0x00, 0x00, 0x06,  // 0x0000_C755 06
          0x0A, 0x2F, 0x00, 0x00, 0x01,  // 0x0000_2F0A 01
          0x01, 0xD1, 0x00, 0x00, 0xF0,  // 0x0000_D101 F0
          0x03, 0xD1, 0x00, 0x00, 0x9B,  // 0x0000_D103 9B
          0x36, 0x2F, 0x00, 0x00, 0x8E,  // 0x0000_2F36 8E
          0x06, 0x32, 0x00, 0x00, 0x80,  // 0x0000_3206 80
          0x11, 0x32, 0x00, 0x00, 0x0B,  // 0x0000_3211 0B
          0x16, 0x32, 0x00, 0x00, 0x06,  // 0x0000_3216 06
          0x14, 0xC6, 0x00, 0x00, 0x20,  // 0x0000_C614 20
          0x15, 0xC6, 0x00, 0x00, 0x0A,  // 0x0000_C615 0A
          0x16, 0xC6, 0x00, 0x00, 0x02,  // 0x0000_C616 02
          0x17, 0xC6, 0x00, 0x00, 0x00,  // 0x0000_C617 00
          0x02, 0xC1, 0x00, 0x00, 0x00,  // 0x0000_C102 00
          0x0B, 0xC6, 0x00, 0x00, 0x10,  // 0x0000_C60B 10
          0x90, 0xC0, 0x00, 0x00, 0xB0,  // 0x0000_C090 B0
          0x0E, 0xC6, 0x00, 0x00, 0x05,  // 0x0000_C60E 05
          0x0F, 0xC0, 0x00, 0x00, 0x01,  // 0x0000_C00F 01
          0x9C, 0xC0, 0x00, 0x00, 0x7B,  // 0x0000_C09C 7B
          0x02, 0xC6, 0x00, 0x00, 0x07,  // 0x0000_C602 07
          0x03, 0xC6, 0x00, 0x00, 0x07,  // 0x0000_C603 07
          0xA3, 0xC0, 0x00, 0x00, 0x71,  // 0x0000_C0A3 71
          0x0B, 0xC0, 0x00, 0x00, 0x20,  // 0x0000_C00B 20
          0x93, 0xC0, 0x00, 0x00, 0x80,  // 0x0000_C093 80
          0x9D, 0xC0, 0x00, 0x00, 0x80,  // 0x0000_C09D 80
          0xB0, 0xC0, 0x00, 0x00, 0x77,  // 0x0000_C0B0 77
          0x50, 0xC0, 0x00, 0x00, 0x83,  // 0x0000_C050 83
          0x10, 0xC0, 0x00, 0x00, 0xA5,  // 0x0000_C010 A5
          0x32, 0x22, 0x00, 0x00, 0x80,  // 0x0000_2232 80
          0xB0, 0xC0, 0x00, 0x00, 0x77,  // 0x0000_C0B0 77
          0x11, 0xC0, 0x00, 0x00, 0x00,  // 0x0000_C011 00
          0x20, 0xC0, 0x00, 0x00, 0x00,  // 0x0000_C020 00
          0x23, 0xC0, 0x00, 0x00, 0x00,  // 0x0000_C023 00
          0x01, 0x31, 0x00, 0x00, 0x00,  // 0x0000_3101 00
          0x04, 0x30, 0x00, 0x00, 0xE4,  // 0x0000_3004 E4
          0x05, 0x30, 0x00, 0x00, 0xF5,  // 0x0000_3005 F5
          0x07, 0xC0, 0x00, 0x00, 0x44,  // 0x0000_C007 44
          0x08, 0xC0, 0x00, 0x00, 0x44,  // 0x0000_C008 44
          0x09, 0xC0, 0x00, 0x00, 0x33,  // 0x0000_C009 33
          0x07, 0xC0, 0x00, 0x00, 0x22,  // 0x0000_C007 22
          0x08, 0xC0, 0x00, 0x00, 0x22,  // 0x0000_C008 22
          0x09, 0xC0, 0x00, 0x00, 0x00,  // 0x0000_C009 00
          0x22, 0xC0, 0x00, 0x00, 0xD6,  // 0x0000_C022 D6
          0x25, 0xC0, 0x00, 0x00, 0xD6,  // 0x0000_C025 D6
          0x02, 0x32, 0x00, 0x00, 0x02,  // 0x0000_3202 02
          0x55, 0x2F, 0x00, 0x00, 0x03,  // 0x0000_2F55 03
          0x01, 0xD0, 0x00, 0x00, 0x03,  // 0x0000_D001 03
          0x02, 0xD0, 0x00, 0x00, 0xC6,  // 0x0000_D002 C6
          0x03, 0xD0, 0x00, 0x00, 0x03,  // 0x0000_D003 03
          0x04, 0xD0, 0x00, 0x00, 0xC6,  // 0x0000_D004 C6
          0x0F, 0xC6, 0x00, 0x00, 0x05,  // 0x0000_C60F 05
          0x9E, 0xC0, 0x00, 0x00, 0x01,  // 0x0000_C09E 01
        }) //End AF04.BUF0

        Name(BUF1, Buffer()
          {
                    0x0, 0x05,  // NotRotated, 0x05 = Right/Right
                    0x1, 0x09,  // 90DegreesCounterClockwise, 0x09 = Derived Center
                    0x2, 0x01,  // 180DegreesCounterClockwise, 0x01 = Left/Left
                    0x3, 0x09,  // 270DegreesCounterClockwise, 0x09 = Derived Center
        }) //End AF04.BUF1

        Name(C004, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            // Function_SDCA_Version = 0.6, Class, DC
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 0x6},
          }
        }) //End AF04.C004

        Name(C005, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            // Function_Topology = 0x1 Smart-Amp, Class, DC
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 0x1},
          }
        }) //End AF04.C005

        Name(C006, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 0x025d},
          }
        }) //End AF04.C006

        Name(C007, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 0x1316},
          }
        }) //End AF04.C007

        Name(C008, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            // Function_Version = 0x1, Class, DC
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 0x1},
          }
        }) //End AF04.C008

        Name(E001, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x2 },
            Package(2) {"mipi-sdca-entity-label", "IT 21"},
            Package(2) {"mipi-sdca-terminal-type", 0x0101}, // Generic Sink DataPort
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x8, 0x10, 0x11} },  // Latency, ClusterIndex, DataPortIndex
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-controlselector-0x8-subproperties", "C118" },
            Package(2) {"mipi-sdca-controlselector-0x10-subproperties", "C110"},
            Package(2) {"mipi-sdca-controlselector-0x11-subproperties", "C111"},
            Package(2) {"mipi-sdca-clock-origin", "E021"},
          },
          ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
          Package()
          {
            Package(2) {"mipi-sdca-terminal-dp-numbers","BUF2"},
          }
        }) //End AF04.E001

        Name(BUF2, Buffer()
          {
            0x0, 0x1, // DP_Index: 0x0 DP_Num: 0x1
        }) //End AF04.BUF2

        Name(C118, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // Latency, Class, RO, Interrupt at Bit 1
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-interrupt-position", 1},
            Package(2) {"mipi-sdca-control-selector-access-mode", 3},
          }
        }) //End AF04.C118

        Name(C110, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // ClusterIndex, Class, DC
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 1},
          }
        }) //End AF04.C110

        Name(C111, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // DataPortIndex, Class, DC
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 1},
          }
        }) //End AF04.C111

        Name(E002, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x21 },
            Package(2) {"mipi-sdca-entity-label", "UDMPU 21"},
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x6, 0x10} },  // Bypass, Latency, ClusterIndex
            Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-input-pin-1", "E001" },   // Input Pin 1 connected to IT21
            Package(2) {"mipi-sdca-controlselector-0x1-subproperties", "CS01"},
            Package(2) {"mipi-sdca-controlselector-0x6-subproperties", "LC00"},
            Package(2) {"mipi-sdca-controlselector-0x10-subproperties", "CS10"},
          }
        }) //End AF04.E002

        Name(CS01, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // Bypass, Class, RW
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 0},
          }
        }) //End AF04.CS01

        Name(LC00, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // Latency, Class, DC
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 0},
          }
        }) //End AF04.LC00

        Name(CS10, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // ClusterIndex, Class, DC
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 0x1},
          }
        }) //End AF04.CS10

        Name(E003, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x07 },
            Package(2) {"mipi-sdca-entity-label", "FU 21"},
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2, 0x10} },  // Mute, Volume, Latency
            Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-input-pin-1", "E002" }, // Input Pin 1 connected to UDMPU21
            Package(2) {"mipi-sdca-controlselector-0x1-subproperties", "C021"},
            Package(2) {"mipi-sdca-controlselector-0x2-subproperties", "C022"},
            Package(2) {"mipi-sdca-controlselector-0x10-subproperties", "LC00"},
          }
        }) //End AF04.E003

        Name(C021, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {  // Mute, User, RW
            Package(2) { "mipi-sdca-control-selector-access-layer", 1 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 0},
            Package(2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} },
          }
        }) //End AF04.C021

        Name(C022, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {  // Volume, User, RW
            Package(2) { "mipi-sdca-control-selector-access-layer", 1 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 0},
            Package(2) {"mipi-sdca-control-selector-default-value", 0xFA00},
            Package(2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} },
          },
          ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
          Package()
          {
            Package(2) {"mipi-sdca-control-number-range","BUF3"},
          }
        }) //End AF04.C022

        Name(BUF3, Buffer()
        {
          0x03, 0x00,  // Range type 0x0003 (Triples)
          0x01, 0x00,  // Count of ranges = 0x1
          0x60, 0xa0, 0x00, 0x00,  // Min =0x0000a060
          0x00, 0x00, 0x00, 0x00,  // Max =0x00000000
          0x60, 0x00, 0x00, 0x00,  // Step =0x00000060
        }) //End AF04.BUF3

        Name(E033, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x11 },
            Package(2) {"mipi-sdca-entity-label", "PDE 22"},
            Package(2) {"mipi-sdca-powerdomain-state-list", 0x9},
            Package(2)
            {
              "mipi-sdca-powerdomain-managed-list",
              Package()
              {
                0x22
              },
            },
            Package(2)
            {
              "mipi-sdca-powerdomain-transition-typical-delay",
              Package()
              {
                3, 0, 500000,
                0, 3, 500000,
              },
            },
            Package(2)
            {
              "mipi-sdca-powerdomain-transition-max-delay",
              Package()
              {
                3, 0, 800000,
                0, 3, 800000,
              },
            },
             Package(2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10} },  // Requested_PS, Actual_PS
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-controlselector-0x1-subproperties", "RPS0" },
            Package(2) {"mipi-sdca-controlselector-0x10-subproperties", "APS0"},
          }
        }) //End AF04.E033

        Name(RPS0, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {  // Requested_PS, Class, R/W
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 0},
          }
        }) //End AF04.RPS0

        Name(APS0, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {  // Actual_PS, Class, RO
             Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 3},
          }
        }) //End AF04.APS0

        Name(E022, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x0A },
            Package(2) {"mipi-sdca-entity-label", "XU 22"},
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2, 0x6 } },  // Bypass, ClusterIndex, Latency
            Package(2) {"mipi-sdca-input-pin-list", 0x6},  // Input Pin 1 & 2 connected
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-input-pin-1", "E003" },   // Input Pin 1 connected to FU21
            Package(2) {"mipi-sdca-input-pin-2", "E051"},   // Input Pin 2 connected to IT29
            Package(2) {"mipi-sdca-controlselector-0x1-subproperties", "CS11"},
            Package(2) {"mipi-sdca-controlselector-0x2-subproperties", "CS12"},
            Package(2) {"mipi-sdca-controlselector-0x6-subproperties", "LC00"},
          }
        }) //End AF04.E022

        Name(CS11, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // Bypass, Class, R/W
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 0},
          }
        }) //End AF04.CS11

        Name(CS12, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // ClusterIndex, Class, DC
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 1},
          }
        }) //End AF04.CS12

        Name(E029, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x24 },
            Package(2) {"mipi-sdca-entity-label", "SAPU 29"},
            Package(2) {"mipi-sdca-input-pin-list", 0x6}, // Input Pin 1 & 2 connected
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x5, 0x10, 0x11} }, // Latency, Protection_Mode, Protection_Status
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-input-pin-1", "E022" },  // Input Pin 1 connected to XU22
            Package(2) {"mipi-sdca-input-pin-2", "E051"},  // Input Pin 2 connected to IT29
            Package(2) {"mipi-sdca-controlselector-0x5-subproperties", "LC00"},
            Package(2) {"mipi-sdca-controlselector-0x10-subproperties", "CS22"},
            Package(2) {"mipi-sdca-controlselector-0x11-subproperties", "CS23"},
          }
        }) //End AF04.E029

        Name(CS22, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {   // Protection_Mode, Class, RO
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-interrupt-position", 0},  // SDCA Interrupt Bit 0
            Package(2) {"mipi-sdca-control-selector-access-mode", 3},
          }
        }) //End AF04.CS22

        Name(CS23, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {   // Protection_Status, Class, RO
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            //Package (2) {"mipi-sdca-control-selector-access-mode", 3},
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 0},
          }
        }) //End AF04.CS23

        Name(E054, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x21 },
            Package(2) {"mipi-sdca-entity-label", "UDMPU 23"},
            Package(2) {"mipi-sdca-input-pin-list", 0x2},  // Input Pin 1 connected
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x6} },  // Bypass, Latency
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-input-pin-1", "E029" },
            Package(2) {"mipi-sdca-controlselector-0x1-subproperties", "C101"},
            Package(2) {"mipi-sdca-controlselector-0x6-subproperties", "LC00"},
          }
        }) //End AF04.E054

        Name(C101, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 0},
          }
        }) //End AF04.C101

        Name(E055, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x07 },
            Package(2) {"mipi-sdca-entity-label", "FU 23"},
            Package(2) {"mipi-sdca-input-pin-list", 0x2},    // Input Pin 1 connected
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x10} },
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-input-pin-1", "E054" },
            Package(2) {"mipi-sdca-controlselector-0x10-subproperties", "LC00"},
          }
        }) //End AF04.E055

        Name(E031, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x11 },
            Package(2) {"mipi-sdca-entity-label", "PDE 23"},
            Package(2) {"mipi-sdca-powerdomain-state-list", 0x9},
            Package(2)
            {
              "mipi-sdca-powerdomain-managed-list",
              Package() {0x56},
            },
            Package(2)
            {
              "mipi-sdca-powerdomain-transition-typical-delay",
              Package()
              {
                3, 0, 500000,
                0, 3, 500000,
              },
            },
            Package(2)
            {
              "mipi-sdca-powerdomain-transition-max-delay",
              Package()
              {
                3, 0, 800000,
                0, 3, 800000,
              },
            },
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10} },
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-controlselector-0x1-subproperties", "RPS0" },
            Package(2) {"mipi-sdca-controlselector-0x10-subproperties", "APS0"},
          }
        }) //End AF04.E031

        Name(E056, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x03 },
            Package(2) {"mipi-sdca-entity-label", "OT 23"},
            Package(2) {"mipi-sdca-terminal-type", 0x0380},// Primary Full Speaker
            Package(2) {"mipi-sdca-input-pin-list", 0x2},
            Package(2) {"mipi-sdca-control-selector-list",
            Package() {0x4, 0x8} },  // Usage, Latency
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-input-pin-1", "E055" },
            Package(2) {"mipi-sdca-powerdomain-entity", "E031"},
            Package(2) {"mipi-sdca-controlselector-0x4-subproperties", "C054"},
            Package(2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
          }
        }) //End AF04.E056

        Name(C054, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 0x60},
          }
        }) //End AF04.C054

        Name(E051, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x02 },
            Package(2) {"mipi-sdca-entity-label", "IT 29"},
            Package(2) {"mipi-sdca-terminal-type", 0x0280},
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x8, 0x10} },
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-controlselector-0x8-subproperties", "LC00" },
            Package(2) {"mipi-sdca-controlselector-0x10-subproperties", "C610"},
          }
        }) //End AF04.E051

        Name(C610, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 1},
          }
        }) //End AF04.C610

        Name(E034, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x11 },
            Package(2) {"mipi-sdca-entity-label", "PDE 24"},
                    Package(2) {"mipi-sdca-powerdomain-state-list", 0x9},
            Package(2)
            {
              "mipi-sdca-powerdomain-managed-list",
              Package()
              {
                0x24
              },
            },
            Package(2)
            {
              "mipi-sdca-powerdomain-transition-typical-delay",
              Package()
              {
                3, 0, 500000,
                0, 3, 500000,
              },
            },
            Package(2)
            {
              "mipi-sdca-powerdomain-transition-max-delay",
              Package()
              {
                3, 0, 800000,
                0, 3, 800000,
              },
            },
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10} }, // Requested_PS, Actual_PS
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-controlselector-0x1-subproperties", "RPS0" },
            Package(2) {"mipi-sdca-controlselector-0x10-subproperties", "APS0"},
          }
        }) //End AF04.E034

        Name(E024, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x0A },
            Package(2) {"mipi-sdca-entity-label", "XU 24"},
            Package(2) {"mipi-sdca-input-pin-list", 0x2},
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2, 0x6} }, //Bypass, ClusterIndex, Latency
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-input-pin-1", "E051" },
            Package(2) {"mipi-sdca-powerdomain-entity", "E034"},
            Package(2) {"mipi-sdca-controlselector-0x1-subproperties", "C081"},
            Package(2) {"mipi-sdca-controlselector-0x2-subproperties", "C082"},
            Package(2) {"mipi-sdca-controlselector-0x6-subproperties", "LC00"},
          }
        }) //End AF04.E024

        Name(C081, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {  // Bypass, Class, R/W
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 0},
          }
        }) //End AF04.C081

        Name(C082, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {  // ClusterIndex, Class, DC
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 1},
          }
        }) //End AF04.C082

        Name(E052, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x03 },
            Package(2) {"mipi-sdca-entity-label", "OT 24"},
            Package(2) {"mipi-sdca-terminal-type", 0x0188},// IV Sense DP
            Package(2) {"mipi-sdca-input-pin-list", 0x2},
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x8, 0x11} },  // Latency, DataPortIndex
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-input-pin-1", "E024" },
            Package(2) {"mipi-sdca-clock-origin", "E053"},
            Package(2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
            Package(2) {"mipi-sdca-controlselector-0x11-subproperties", "C911"},
          },
          ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
          Package()
          {
            Package(2) {"mipi-sdca-terminal-dp-numbers","BUF4"},
          }
        }) //End AF04.E052

        Name(BUF4, Buffer()
        {
          0x0, 0x2, // DP_Index: 0x0 DP_Num: 0x2
        }) //End AF04.BUF4

        Name(C911, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // DataPortIndex, Class, DC
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 5},
            Package(2) {"mipi-sdca-control-number-dc-value", 1},
          }
        }) //End AF04.C911

        Name(E053, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x0b },
            Package(2) {"mipi-sdca-entity-label", "CS 24"},
            Package(2) {"mipi-sdca-cs-type", 0},
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x10} },  // Sample_Freq_Index
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-controlselector-0x10-subproperties", "C121" },
          }
        }) //End AF04.E053

        Name(C121, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // Sample_Freq_Index, Class, RW
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 0},
          },
          ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
          Package()
          {
            Package(2) {"mipi-sdca-control-number-range","BUF5"},
          }
        }) //End AF04.C121

        Name(BUF5, Buffer()
        {
          0x02, 0x00,  // Range type 0x0002 (Pairs)
          0x02, 0x00,  // Count of ranges = 0x2
          0x08, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00,0x00, // 0x00000008, 0x0000AC44(44100)
          0x09, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00,0x00, // 0x00000009, 0x0000BB80(48000)
        }) //End AF04.BUF5

        Name(E021, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x0b },
            Package(2) {"mipi-sdca-entity-label", "CS 21"},
            Package(2) {"mipi-sdca-cs-type", 0},
            Package(2) {"mipi-sdca-control-selector-list", Package() {0x2, 0x10} },  // Clock_Valid, Sample_Freq_Index
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-controlselector-0x2-subproperties", "C202" },
            Package(2) { "mipi-sdca-controlselector-0x10-subproperties", "C210" },
          }
        }) //End AF04.E021

        Name(C202, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // Clock_Valid, Class, RW
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 0},
          }
        }) //End AF04.C202

        Name(C210, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // Sample_Freq_Index, Class, RW
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 0},
          },
          ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
          Package()
          {
            Package(2) {"mipi-sdca-control-number-range","BUF5"},
          }
        }) //End AF04.C210

        Name(E040, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-entity-type", 0x30 },
            Package(2) {"mipi-sdca-entity-label", "TG 23"},
            Package(2)
            {
              "mipi-sdca-control-selector-list",
              Package() {0x10}
            },  // Class Divider
          },
          ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
          Package()
          {
            Package(2) { "mipi-sdca-controlselector-0x10-subproperties", "C910" },
            Package(2) {"mipi-sdca-clock-origin", "E021"},
          }
        }) //End AF04.E040

        Name(C910, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          { // Class Divider, Class, RW
            Package(2) { "mipi-sdca-control-selector-access-layer", 4 },
            Package(2) {"mipi-sdca-control-selector-access-mode", 0},
          }
        }) //End AF04.C910

        Name(EXT0, Package()
        {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package()
          {
            Package(2) { "mipi-sdca-function-expansion-subsystem-id", 0x1234 },
            Package(2) {"mipi-sdca-function-expansion-subsystem-revision-id", 0x1234},
            Package(2) {"mipi-rtk-part-smartamp-private-prop", 0x1234},
          }
        }) //End AF04.EXT0
