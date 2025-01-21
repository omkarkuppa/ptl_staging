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

   Name(_DSD, Package() {  // Function Descriptor
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdw-sw-interface-revision", 0x00010001},
           Package (2) {"msft-sdca-spec-revision", 0x4},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x4, 0x5, 0x6, 0x7, 0x8} },  // Function level controls
           Package (2) {"mipi-sdca-entity-id-list",
            Package() {0x1, 0x2, 0x4, 0x5, 0x6 ,0x8, 0x9, 0xA, 0xE, 0xF, 0x10, 0x11, 0x12,
            0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x36 } },  // List of entities in this function
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-controlselector-0x4-subproperties", "C004"},
           Package (2) {"mipi-sdca-controlselector-0x5-subproperties", "C005"},
           Package (2) {"mipi-sdca-controlselector-0x6-subproperties", "C006"},
           Package (2) {"mipi-sdca-controlselector-0x7-subproperties", "C007"},
           Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "C008"},
           Package (2) {"mipi-sdca-entity-id-0x1-subproperties", "E001"},
           Package (2) {"mipi-sdca-entity-id-0x2-subproperties", "E002"},
           Package (2) {"mipi-sdca-entity-id-0x4-subproperties", "E004"},
           Package (2) {"mipi-sdca-entity-id-0x5-subproperties", "E005"},
           Package (2) {"mipi-sdca-entity-id-0x6-subproperties", "E006"},
           Package (2) {"mipi-sdca-entity-id-0x8-subproperties", "E008"},
           Package (2) {"mipi-sdca-entity-id-0x9-subproperties", "E009"},
           Package (2) {"mipi-sdca-entity-id-0xA-subproperties", "E00A"},
           Package (2) {"mipi-sdca-entity-id-0xE-subproperties", "E00E"},
           Package (2) {"mipi-sdca-entity-id-0xF-subproperties", "E00F"},
           Package (2) {"mipi-sdca-entity-id-0x10-subproperties", "E010"},
           Package (2) {"mipi-sdca-entity-id-0x11-subproperties", "E011"},
           Package (2) {"mipi-sdca-entity-id-0x12-subproperties", "E012"},
           Package (2) {"mipi-sdca-entity-id-0x13-subproperties", "E013"},
           Package (2) {"mipi-sdca-entity-id-0x14-subproperties", "E014"},
           Package (2) {"mipi-sdca-entity-id-0x15-subproperties", "E015"},
           Package (2) {"mipi-sdca-entity-id-0x16-subproperties", "E016"},
           Package (2) {"mipi-sdca-entity-id-0x17-subproperties", "E017"},
           Package (2) {"mipi-sdca-entity-id-0x18-subproperties", "E018"},
           Package (2) {"mipi-sdca-entity-id-0x19-subproperties", "E019"},
           Package (2) {"mipi-sdca-entity-id-0x1A-subproperties", "E01A"},
           Package (2) {"mipi-sdca-entity-id-0x1B-subproperties", "E01B"},
           Package (2) {"mipi-sdca-entity-id-0x36-subproperties", "E036"},
           Package (2) {"mipi-sdca-function-expansion-subproperties", "EXT0"},
        },
        ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
        Package() {
           Package (2) {"mipi-sdca-function-initialization-table", "BUF0"},
        }
   }) //End AF02._DSD

    Name(BUF0, Buffer() {
        0x44, 0x20, 0x00, 0x02, 0x02,  // 0x02002044, 0x02
        0x44, 0x00, 0x00, 0x02, 0x03,  // 0x02000044, 0x03
        0x00, 0x20, 0x10, 0x06, 0x67,  // 0x06102000, 0x67
        0x00, 0x00, 0x10, 0x06, 0x45,  // 0x06100000, 0x45
        0x13, 0x20, 0x10, 0x06, 0x12,  // 0x06102013, 0x12
        0x13, 0x00, 0x10, 0x06, 0x34,  // 0x06100013, 0x34
        0x12, 0x20, 0x10, 0x06, 0x56,  // 0x06102012, 0x56
        0x12, 0x00, 0x10, 0x06, 0x78,  // 0x06100012, 0x78
        0x08, 0x20, 0x00, 0x02, 0x00,  // 0x02002008, 0x00
        0x08, 0x00, 0x00, 0x02, 0x33,  // 0x02000008, 0x33
        0x0A, 0x20, 0x00, 0x02, 0x05,  // 0x0200200A, 0x05
        0x0A, 0x00, 0x00, 0x02, 0x33,  // 0x0200000A, 0x33
        0x39, 0x20, 0x00, 0x02, 0xAA,  // 0x02002039, 0xAA
        0x39, 0x00, 0x00, 0x02, 0xC1,  // 0x02000039, 0xC1
        0x36, 0x20, 0x00, 0x02, 0x15,  // 0x02002036, 0x15
        0x36, 0x00, 0x00, 0x02, 0x54,  // 0x02000036, 0x54
        0x24, 0x20, 0xE0, 0x05, 0x00,  // 0x05E02024, 0x00
        0x24, 0x00, 0xE0, 0x05, 0x00,  // 0x05E00024, 0x00
        0x1B, 0x20, 0xE0, 0x05, 0xC0,  // 0x05E0201B, 0xC0
        0x1B, 0x00, 0xE0, 0x05, 0x50,  // 0x05E0001B, 0x50
    }) //End AF02.BUF0

   Name(C004, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           // Function_SDCA_Version = 0.6, Class, DC
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 5},
           Package (2) {"mipi-sdca-control-number-dc-value", 0x6},
        }
   }) //End AF02.C004

   Name(C005, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           // Function_Topology = 0x2 Smart-Mic, Class, DC
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 5},
           Package (2) {"mipi-sdca-control-number-dc-value", 0x2},
        }
   }) //End AF02.C005

   Name(C006, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 5},
           Package (2) {"mipi-sdca-control-number-dc-value", 0x025d},
        }
   }) //End AF02.C006

   Name(C007, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           // Function_ID = 0x0714 (Device specific), Class, DC
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 5},
           Package (2) {"mipi-sdca-control-number-dc-value", 0x0714},
        }
   }) //End AF02.C007

   Name(C008, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           // Function_Version = 0x1, Class, DC
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 5},
           Package (2) {"mipi-sdca-control-number-dc-value", 0x1},
        }
   }) //End AF02.C008

   Name(E011, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x11},
           Package (2) {"mipi-sdca-entity-label", "PDE 18"},
           Package (2) {"mipi-sdca-powerdomain-state-list", 0x19},
           Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x12} },
           Package (2) {"mipi-sdca-powerdomain-transition-typical-delay",
               Package()
               {
                  0x3, 0x0, 200000,
                  0x0, 0x3, 200000,
                  0x2, 0x0, 200000,
                  0x0, 0x2, 200000,
                  0x1, 0x0, 0,
                  0x0, 0x1, 0,
                  0x0, 0x0, 0,
               }},
           Package (2) {"mipi-sdca-powerdomain-transition-max-delay",
               Package()
               {
                  0x3, 0x0, 300000,
                  0x0, 0x3, 300000,
                  0x2, 0x0, 300000,
                  0x0, 0x2, 300000,
                  0x1, 0x0, 300000,
                  0x0, 0x1, 300000,
                  0x0, 0x0, 300000,
               }},
           Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10} },  // Requested_PS, Actual_PS
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "RPS0"},
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "APS0"},
        }
   }) //End AF02.E011

   Name(RPS0, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {  // Requested_PS, Class, R/W
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        }
   }) //End AF02.RPS0

   Name(APS0, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {  // Actual_PS, Class, RO
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        }
   }) //End AF02.APS0

   Name(E012, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x0b},
           Package (2) {"mipi-sdca-entity-label", "CS 18"},
           Package (2) {"mipi-sdca-cs-type", 1},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x2} },  // clock_valid
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "C202"},
        }
   }) //End AF02.E012

   Name(C202, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // Clock_Valid, Class, R
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 3},
           Package (2) {"mipi-sdca-control-selector-interrupt-position", 19},
        }
    }) //End AF02.C202

   Name(E013, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x0b},
           Package (2) {"mipi-sdca-entity-label", "CS 11"},
           Package (2) {"mipi-sdca-cs-type", 0},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x2} },  // clock_valid
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "C203"},
        }
   }) //End AF02.E013

   Name(C203, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // Clock_Valid, Class, R
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 3},
           Package (2) {"mipi-sdca-control-selector-interrupt-position", 18},
        }
    }) //End AF02.C203

   Name(E010, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x0c},
           Package (2) {"mipi-sdca-entity-label", "CX 11"},
           Package (2) {"mipi-sdca-input-pin-list", 0x6},  // Input Pin 1 & 2 connected
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x1} },  // clock_select
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E013"},   // Input Pin 1 connected to CS11
           Package (2) {"mipi-sdca-input-pin-2", "E012"},   // Input Pin 2 connected to CS18
           Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "C101"},
        }
   }) //End AF02.E010

   Name(C101, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // clock_select, Class, RW
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        }
    }) //End AF02.C101

   Name(E018, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x11},
           Package (2) {"mipi-sdca-entity-label", "PDE 11"},
           Package (2) {"mipi-sdca-powerdomain-state-list", 0x19},
           Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0xf} },
           Package (2) {"mipi-sdca-powerdomain-transition-typical-delay",
               Package()
               {
                  0x3, 0x0, 200000,
                  0x0, 0x3, 200000,
                  0x2, 0x0, 200000,
                  0x0, 0x2, 200000,
                  0x1, 0x0, 0,
                  0x0, 0x1, 0,
                  0x0, 0x0, 0,
               }},
           Package (2) {"mipi-sdca-powerdomain-transition-max-delay",
               Package()
               {
                  0x3, 0x0, 300000,
                  0x0, 0x3, 300000,
                  0x2, 0x0, 300000,
                  0x0, 0x2, 300000,
                  0x1, 0x0, 300000,
                  0x0, 0x1, 300000,
                  0x0, 0x0, 300000,
               }},
           Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10} },  // Requested_PS, Actual_PS
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "RPS0"},
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "APS0"},
        }
   }) //End AF02.E018

   Name(E00F, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x02},
           Package (2) {"mipi-sdca-entity-label", "IT 11"},
           Package (2) {"mipi-sdca-terminal-type", 0x0201}, // Mic transducer
           Package (2) {"mipi-sdca-microphone-snr", 0x00410000},           // 65dB, in 16.16 fixed point format
           Package (2) {"mipi-sdca-microphone-sensitivity", 0xFFE60000},   // -26dB
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x4} }, // Usage
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-clock-origin", "E010"},
           Package (2) {"mipi-sdca-controlselector-0x4-subproperties", "CF04"},
        },
        ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
        Package() {
           Package (2) {"mipi-sdca-microphone-array-geometry", "MGEO"},
        }
   }) //End AF02.E00F

   Name(MGEO, Buffer()
   {
        0x00, 0x01,       // Version
        0x00, 0x00,       // Mic Array Type, KSMICARRAY_MICARRAYTYPE_LINEAR=0
        0xEA, 0xDD,       // VerticalAngleBegin, 0xDDEA = -8726*180/31416 = -50 degree
        0x16, 0x22,       // VerticalAngleEnd, 0x2216 = 8726*180/31416 = 50 degree
        0x19, 0xD7,       // HorizontalAngleBegin, 0xD719 = -10471*180/31416 = -60 degree
        0xE7, 0x28,       // HorizontalAngleEnd, 0x28E7 = 10471*180/31416 = 60 degree
        0x64, 0x00,       // FrequencyBandLo, 100Hz
        0x4C, 0x1D,       // FrequencyBandHi, 7500Hz
        0x02, 0x00,       // NumberOfMicrophones, 2 mic elements

            0x00, 0x00,   // Type, KSMICARRAY_MICTYPE_OMNIDIRECTIONAL=0
            0x00, 0x00,   // XCoord
            0xD7, 0xFF,   // YCoord, -41mm, nagtive to user's left hand side
            0x00, 0x00,   // ZCoord
            0x00, 0x00,   // VerticalAngle
            0x00, 0x00,   // HorizontalAngle

            0x00, 0x00,   // Type, KSMICARRAY_MICTYPE_OMNIDIRECTIONAL=0
            0x00, 0x00,   // XCoord
            0x29, 0x00,   // YCoord, 41mm, positive to user's right hand side
            0x00, 0x00,   // ZCoord
            0x00, 0x00,   // VerticalAngle
            0x00, 0x00,   // HorizontalAngle
   }) //End MGEO

   Name(CF04, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {  // Usage, class, DC
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 5},
           Package (2) {"mipi-sdca-control-number-dc-value", 1},
        }
   }) //End AF02.CF04

   // FU 0x00E should be platform access only
   Name(E00E, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x07},
           Package (2) {"mipi-sdca-entity-label", "FU 11"},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0xb} },  // gain
           Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E00F"}, // Input Pin 1 connected to IT0f
           Package (2) {"mipi-sdca-controlselector-0xb-subproperties", "CF0B"},
        }
   }) //End AF02.E00E

   Name(CF0B, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {  // gain, platform, RW
           Package (2) {"mipi-sdca-control-selector-access-layer", 8},
           Package (2) {"mipi-sdca-control-selector-access-mode", 0},
           Package (2) {"mipi-sdca-control-number-list",
            Package() {0x1, 0x2, 0x3, 0x4} },
        }
   }) //End AF02.CF0B

   Name(E036, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x11},
           Package (2) {"mipi-sdca-entity-label", "PDE 12"},
           Package (2) {"mipi-sdca-powerdomain-state-list", 0x19},
           Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x17} },
           Package (2) {"mipi-sdca-powerdomain-transition-typical-delay",
               Package()
               {
                  0x3, 0x0, 200000,
                  0x0, 0x3, 200000,
                  0x2, 0x0, 200000,
                  0x0, 0x2, 200000,
                  0x1, 0x0, 0,
                  0x0, 0x1, 0,
                  0x0, 0x0, 0,
               }},
           Package (2) {"mipi-sdca-powerdomain-transition-max-delay",
               Package()
               {
                  0x3, 0x0, 300000,
                  0x0, 0x3, 300000,
                  0x2, 0x0, 300000,
                  0x0, 0x2, 300000,
                  0x1, 0x0, 300000,
                  0x0, 0x1, 300000,
                  0x0, 0x0, 300000,
               }},
           Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10} },  // Requested_PS, Actual_PS
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "RPS0"},
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "APS0"},
        }
   }) //End AF02.E036

   Name(E017, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x0A},
           Package (2) {"mipi-sdca-entity-label", "XU 12"},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x1, 0x2} },  // Bypass, ClusterIndex
           Package (2) {"mipi-sdca-input-pin-list", 0x2},  // Input Pin 1
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E00E"},  // Input Pin 1 connected to FU-0e
           Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "C701"},
           Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "C702"},
        }
   }) //End AF02.E017

   Name(C701, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // Bypass, Class, R/W
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        }
   }) //End AF02.C701

   Name(C702, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // ClusterIndex, Class, DC
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 5},
           Package (2) {"mipi-sdca-control-number-dc-value", 1},
        }
   }) //End AF02.C702

   Name(E01B, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x20},
           Package (2) {"mipi-sdca-entity-label", "CRU 14"},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x6, 0x10} },  // Latency, ClusterIndex
           Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E017"},   // Input Pin 1 connected to XU17
           Package (2) {"mipi-sdca-controlselector-0x6-subproperties", "LC00"},
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "CS10"},
        }
   }) //End AF02.E01B

   Name(LC00, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // Latency, Class, DC
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 5},
           Package (2) {"mipi-sdca-control-number-dc-value", 0},
        }
   }) //End AF02.LC00

   Name(CS10, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // ClusterIndex, Class, DC
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 5},
           Package (2) {"mipi-sdca-control-number-dc-value", 0x1},
        }
   }) //End AF02.CS10

   Name(E00A, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x07},
           Package (2) {"mipi-sdca-entity-label", "FU 14"},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() { 0x1, 0x2, 0x10} },  // Mute, Volume, Latency
           Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E01B"}, // Input Pin 1 connected to CRU-1B
           Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "CA01"},
           Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "CA02"},
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "LC00"},
        }
   }) //End AF02.E00A

   Name(CA01, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {  // mute, application, RW
           Package (2) {"mipi-sdca-control-selector-access-layer", 2}, // layer is application
           Package (2) {"mipi-sdca-control-selector-access-mode", 0},
           Package (2) {"mipi-sdca-control-number-list",
            Package() {0x1, 0x2} },
        }
   }) //End AF02.CA01

   Name(CA02, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {  // Volume, application, RW
           Package (2) {"mipi-sdca-control-selector-access-layer", 2}, // layer is application
           Package (2) {"mipi-sdca-control-selector-access-mode", 0},
           Package (2) {"mipi-sdca-control-selector-fixed-value", 0x17d4},
           Package (2) {"mipi-sdca-control-number-list",
            Package() {0x1, 0x2} },
        },
        ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
        Package() {
            Package (2) {"mipi-sdca-control-number-range", "BUF1"},
        }
   }) //End AF02.CA02

   Name(BUF1, Buffer() {
        0x03, 0x00,  // Range type 0x0003
        0x01, 0x00,  // Count of ranges = 0x1
        0xc0, 0xee, 0x00, 0x00,  // Min =0x0000eec0
        0x00, 0x1e, 0x00, 0x00,  // Max =0x00001e00
        0x60, 0x00, 0x00, 0x00,  // Step =0x00000060
   }) //End AF02.BUF1

   Name(E009, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x23},
           Package (2) {"mipi-sdca-entity-label", "SMPU 14"},
           Package (2) {"mipi-sdca-input-pin-list", 0x2},  // Input Pin 1 connected
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x10, 0x11} },
            // trigger_enable, trigger_status
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E00A"}, // Input Pin 1 connected to FU 0xA
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "C510"},  // trigger_enable
           Package (2) {"mipi-sdca-controlselector-0x11-subproperties", "C511"},  // trigger_status
        }
   }) //End AF02.E009

   Name(C510, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // trigger_enable, Class, RW
            Package (2) {"mipi-sdca-control-selector-access-layer", 4},
            Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        }
   }) //End AF02.C510

    Name(C511, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // trigger_status, Class, RW1C
            Package (2) {"mipi-sdca-control-selector-access-layer", 4},
            Package (2) {"mipi-sdca-control-selector-interrupt-position", 17},
            Package (2) {"mipi-sdca-control-selector-access-mode", 2},  // RW1C
        }
    }) //End AF02.C511

   Name(E008, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x0b},
           Package (2) {"mipi-sdca-entity-label", "CS 14"},
           Package (2) {"mipi-sdca-cs-type", 0},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x10} },  // Sample_Freq_Index
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "C410"},
        }
   }) //End AF02.E008

   Name(C410, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // Sample_Freq_Index, Class, RW
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        },
        ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
        Package() {
            Package(2) {"mipi-sdca-control-number-range", "BUF2"},
        }
   }) //End AF02.C410

   Name(BUF2, Buffer() {
        0x02, 0x00,  // Range type 0x0002
        0x05, 0x00,  // Count of ranges = 0x5
        0x04, 0x00, 0x00, 0x00, 0x80, 0x3E, 0x00,0x00, // 0x00000004, 0x00003E80(16000)
        0x08, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00,0x00, // 0x00000008, 0x0000AC44(44100)
        0x09, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00,0x00, // 0x00000009, 0x0000BB80(48000)
        0x0b, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01,0x00, // 0x0000000b, 0x00017700(96000)
        0x0d, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x02,0x00, // 0x0000000d, 0x0002EE00(192000)
   }) //End AF02.BUF2

   Name(E016, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x03},
           Package (2) {"mipi-sdca-entity-label", "OT 14"},
           Package (2) {"mipi-sdca-terminal-type", 0x0180}, // Ultrasonic DP
           Package (2) {"mipi-sdca-input-pin-list", 0x2},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x8, 0x11} },  // Latency, DataPortIndex
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E009"},
           Package (2) {"mipi-sdca-clock-origin", "E008"},
           Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
           Package (2) {"mipi-sdca-controlselector-0x11-subproperties", "C611"},
        },
        ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
        Package() {
           Package(2) {"mipi-sdca-terminal-dp-numbers", "BUF3"},
        }
   }) //End AF02.E016

   Name(BUF3, Buffer() {
        0x0, 0x2, // DP_Index, DP_Num
   }) //End AF02.BUF3

   Name(C611, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // DataPortIndex, Class, DC
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 5},
           Package (2) {"mipi-sdca-control-number-dc-value", 0},
        }
   }) //End AF02.C611

   Name(E019, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x20},
           Package (2) {"mipi-sdca-entity-label", "CRU 13"},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x6, 0x10} },  // Latency, ClusterIndex
           Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E017"},  // Input Pin 1 connected to XU17
           Package (2) {"mipi-sdca-controlselector-0x6-subproperties", "LC00"},
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "CS10"},
        }
   }) //End AF02.E019

   Name(E002, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x07},
           Package (2) {"mipi-sdca-entity-label", "FU 13"},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() { 0x1, 0x2, 0x10} },  // Mute, Volume, Latency
           Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E019"}, // Input Pin 1 connected to CRU-19
           Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "CA11"},
           Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "CA12"},
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "LC00"},
        }
   }) //End AF02.E002

   Name(CA11, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {  // mute, application, RW
           Package (2) {"mipi-sdca-control-selector-access-layer", 2},  // layer is application
           Package (2) {"mipi-sdca-control-selector-access-mode", 0},
           Package (2) {"mipi-sdca-control-number-list",
            Package() {0x1, 0x2} },
        }
   }) //End AF02.CA11

   Name(CA12, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {  // Volume, application, RW
           Package (2) {"mipi-sdca-control-selector-access-layer", 2},  // layer is application
           Package (2) {"mipi-sdca-control-selector-access-mode", 0},
           Package (2) {"mipi-sdca-control-selector-default-value", 0x1B00},
           Package (2) {"mipi-sdca-control-number-list",
            Package() {0x1, 0x2} },
        },
        ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
        Package() {
            Package(2) {"mipi-sdca-control-number-range", "BUF1"},
        }
   }) //End AF02.CA12

   Name(E001, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x0b},
           Package (2) {"mipi-sdca-entity-label", "CS 13"},
           Package (2) {"mipi-sdca-cs-type", 0},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x10} },  // Sample_Freq_Index
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "C410"},
        }
   }) //End AF02.E001

   Name(E014, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x03},
           Package (2) {"mipi-sdca-entity-label", "OT 13"},
           Package (2) {"mipi-sdca-terminal-type", 0x0181}, // Raw PCM Capture
           Package (2) {"mipi-sdca-input-pin-list", 0x2},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x8, 0x11} },  // Latency, DataPortIndex
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E002"},
           Package (2) {"mipi-sdca-clock-origin", "E001"},
           Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
           Package (2) {"mipi-sdca-controlselector-0x11-subproperties", "C611"},
        },
        ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
        Package() {
           Package(2) {"mipi-sdca-terminal-dp-numbers", "BUF4"},
        }
   }) //End AF02.E014

   Name(BUF4, Buffer() {
        0x0, 0x6, // DP_Index, DP_Num
   }) //End AF02.BUF4

   Name(E01A, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x20},
           Package (2) {"mipi-sdca-entity-label", "CRU 17"},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x6, 0x10} },  // Latency, ClusterIndex
           Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E017"},   // Input Pin 1 connected to XU17
           Package (2) {"mipi-sdca-controlselector-0x6-subproperties", "LC00"},
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "CS10"},
        }
   }) //End AF02.E01A

   Name(E006, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x07},
           Package (2) {"mipi-sdca-entity-label", "FU 02"},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() { 0x1, 0x2, 0x10} },  // Mute, Volume, Latency
           Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E01A"}, // Input Pin 1 connected to CRU-1A
           Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "CA01"},
           Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "CA02"},
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "LC00"},
        }
   }) //End AF02.E006

   Name(E005, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x23},
           Package (2) {"mipi-sdca-entity-label", "SMPU 17"},
           Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x10, 0x11} },
            // trigger_enable, trigger_status
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E006"}, // Input Pin 1 connected to FU 0xA
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "C910"}, // trigger_enable
           Package (2) {"mipi-sdca-controlselector-0x11-subproperties", "C911"}, // trigger_status
        }
   }) //End AF02.E005

   Name(C910, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // trigger_enable, Class, RW
           Package (2) {"mipi-sdca-control-selector-access-layer", 4},
           Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        }
   }) //End AF02.C910

    Name(C911, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // trigger_status, Class, RW1C
            Package (2) {"mipi-sdca-control-selector-access-layer", 4},
            Package (2) {"mipi-sdca-control-selector-interrupt-position", 16},
            Package (2) {"mipi-sdca-control-selector-access-mode", 2},  // RW1C
        }
    }) //End AF02.C911

   Name(E004, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x0b},
           Package (2) {"mipi-sdca-entity-label", "CS 17"},
           Package (2) {"mipi-sdca-cs-type", 0},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x10} },  // Sample_Freq_Index
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "C410"},
        }
   }) //End AF02.E004

   Name(E015, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x03},
           Package (2) {"mipi-sdca-entity-label", "OT 17"},
           Package (2) {"mipi-sdca-terminal-type", 0x0185}, // Buffered Capture
           Package (2) {"mipi-sdca-input-pin-list", 0x2},
           Package (2) {"mipi-sdca-control-selector-list",
            Package() {0x8, 0x11} },  // Latency, DataPortIndex
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-input-pin-1", "E005"},
           Package (2) {"mipi-sdca-clock-origin", "E004"},
           Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
           Package (2) {"mipi-sdca-controlselector-0x11-subproperties", "C611"},
        },
        ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
        Package() {
           Package(2) {"mipi-sdca-terminal-dp-numbers", "BUF5"},
        }
   }) //End AF02.E015

   Name(BUF5, Buffer() {
        0x0, 0x4, // DP_Index, DP_Num
   }) //End AF02.BUF5

   Name(EXT0, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-function-expansion-subsystem-id", 0x1234},
           Package (2) {"mipi-sdca-function-expansion-subsystem-revision-id", 0x1234},
        }
   }) //End AF02.EXT0
