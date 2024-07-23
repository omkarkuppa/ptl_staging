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
  Package ()
  {
    Package (2) {"mipi-sdw-sw-interface-revision", 0x00010001}, // v1.1
    Package (2) {"mipi-sdca-interface-revision", 0x06}, // v0.6
    Package (2) {"msft-sdca-spec-revision", 0x4}, // r04
    Package (2) {"mipi-sdca-control-selector-list",  Package() {0x4, 0x5, 0x6, 0x7, 0x8} },  // Function level controls
    Package (2) {"mipi-sdca-entity-id-list",
      Package() {0x1, 0x2, 0x3, 0x42, 0x5, 0x45, 0x46, 0x47, 0x28, 0x6, 0x7, 0x48, 0x41,
             0x49, 0x29, 0x8, 0x9, 0x43, 0xA, 0xB, 0x44, 0xC, 0xD, 0xF, 0x10, 0x11} },// List of entities in this function
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-controlselector-0x4-subproperties", "C004"},
    Package (2) {"mipi-sdca-controlselector-0x5-subproperties", "C005"},
    Package (2) {"mipi-sdca-controlselector-0x6-subproperties", "C006"},
    Package (2) {"mipi-sdca-controlselector-0x7-subproperties", "C007"},
    Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "C008"},
    Package (2) {"mipi-sdca-entity-id-0x1-subproperties", "E001"},
    Package (2) {"mipi-sdca-entity-id-0x2-subproperties", "E002"},
    Package (2) {"mipi-sdca-entity-id-0x3-subproperties", "E003"},
    Package (2) {"mipi-sdca-entity-id-0x42-subproperties", "E042"},
    Package (2) {"mipi-sdca-entity-id-0x5-subproperties", "E005"},
    Package (2) {"mipi-sdca-entity-id-0x45-subproperties", "E045"},
    Package (2) {"mipi-sdca-entity-id-0x46-subproperties", "E046"},
    Package (2) {"mipi-sdca-entity-id-0x47-subproperties", "E047"},
    Package (2) {"mipi-sdca-entity-id-0x28-subproperties", "E028"},
    Package (2) {"mipi-sdca-entity-id-0x6-subproperties", "E006"},
    Package (2) {"mipi-sdca-entity-id-0x7-subproperties", "E007"},
    Package (2) {"mipi-sdca-entity-id-0x48-subproperties", "E048"},
    Package (2) {"mipi-sdca-entity-id-0x41-subproperties", "E041"},
    Package (2) {"mipi-sdca-entity-id-0x49-subproperties", "E049"},
    Package (2) {"mipi-sdca-entity-id-0x29-subproperties", "E029"},
    Package (2) {"mipi-sdca-entity-id-0x8-subproperties", "E008"},
    Package (2) {"mipi-sdca-entity-id-0x9-subproperties", "E009"},
    Package (2) {"mipi-sdca-entity-id-0x43-subproperties", "E043"},
    Package (2) {"mipi-sdca-entity-id-0xA-subproperties", "E00A"},
    Package (2) {"mipi-sdca-entity-id-0xB-subproperties", "E00B"},
    Package (2) {"mipi-sdca-entity-id-0x44-subproperties", "E044"},
    Package (2) {"mipi-sdca-entity-id-0xC-subproperties", "E00C"},
    Package (2) {"mipi-sdca-entity-id-0xD-subproperties", "E00D"},
    Package (2) {"mipi-sdca-entity-id-0xF-subproperties", "E00F"},
    Package (2) {"mipi-sdca-entity-id-0x10-subproperties", "E010"},
    Package (2) {"mipi-sdca-entity-id-0x11-subproperties", "E011"},
    Package (2) {"mipi-sdca-function-expansion-subproperties", "EXT0"},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package (2) {"mipi-sdca-function-initialization-table", "BUF0"},
  }
})// end of DSD

Name(BUF0, Buffer()
{
  0x11, 0x20, 0x00, 0x02, 0x04,  // 0x02002011, 0x04
  0x11, 0x00, 0x00, 0x02, 0x7a,  // 0x02000011, 0x7a

  0x08, 0x20, 0x00, 0x02, 0xa8,  // 0x02002008, 0xa8
  0x08, 0x00, 0x00, 0x02, 0x07,  // 0x02000008, 0x07

  0x09, 0x20, 0x00, 0x02, 0x30,  // 0x02002009, 0x30
  0x09, 0x00, 0x00, 0x02, 0x2b,  // 0x02000009, 0x2b

  0x47, 0x20, 0x00, 0x02, 0xa4,  // 0x02002047, 0xa4
  0x47, 0x00, 0x00, 0x02, 0x3e,  // 0x02000047, 0x3e

  0x5a, 0x2f, 0x00, 0x00, 0x05,  // 0x00002f5a, 0x05

  0x6a, 0x20, 0x00, 0x02, 0x02,  // 0x0200206a, 0x02
  0x6a, 0x00, 0x00, 0x02, 0x00,  // 0x0200006a, 0x00

  0x06, 0x20, 0x00, 0x02, 0x61,  // 0x02002006, 0x61
  0x06, 0x00, 0x00, 0x02, 0x73,  // 0x02000006, 0x73

  0x36, 0x20, 0x10, 0x06, 0x00,  // 0x06102036, 0x00
  0x36, 0x00, 0x10, 0x06, 0x09,  // 0x06100036, 0x09

  0x35, 0x20, 0x10, 0x06, 0x8c,  // 0x06102035, 0x8c
  0x35, 0x00, 0x10, 0x06, 0x60,  // 0x06100035, 0x60

  0x06, 0x20, 0x10, 0x06, 0x00,  // 0x06102006, 0x00
  0x06, 0x00, 0x10, 0x06, 0x00,  // 0x06100006, 0x00

  0x01, 0x20, 0xf0, 0x05, 0x00,  // 0x05f02001, 0x00
  0x01, 0x00, 0xf0, 0x05, 0x1A,  // 0x05f00001, 0x1a

  // 0x6f, 0x20, 0x00, 0x02, 0x25,  // 0x0200206f, 0x25
  // 0x6f, 0x00, 0x00, 0x02, 0x80,  // 0x0200006f, 0x80

  // 0x00, 0x20, 0x80, 0x05, 0x40,  // 0x05802000, 0x40
  // 0x00, 0x00, 0x80, 0x05, 0x08,  // 0x05800000, 0x08

  // 0x00, 0x20, 0x80, 0x05, 0xc0,  // 0x05802000, 0xc0
  // 0x00, 0x00, 0x80, 0x05, 0x08,  // 0x05800000, 0x08

  // 0x6f, 0x20, 0x00, 0x02, 0x25,  // 0x0200206f, 0x25
  // 0x6f, 0x00, 0x00, 0x02, 0xc0,  // 0x0200006f, 0xc0

  0x83, 0x20, 0x00, 0x02, 0x00,  // 0x02002083, 0x00
  0x83, 0x00, 0x00, 0x02, 0x00,  // 0x02000083, 0x00

  // 0x4a, 0x20, 0x00, 0x02, 0x20,  // 0x0200204a, 0x20
  // 0x4a, 0x00, 0x00, 0x02, 0x1b,  // 0x0200004a, 0x1b

  0x10, 0x20, 0xe0, 0x05, 0x00,  // 0x05e02010, 0x00
  0x10, 0x00, 0xe0, 0x05, 0x50,  // 0x05e00010, 0x50

  0x5D, 0x20, 0x00, 0x02, 0x06,  // 0x0200205D, 0x06
  0x5D, 0x00, 0x00, 0x02, 0x00,  // 0x0200005D, 0x00

  0x00, 0x03, 0x58, 0x40, 0x04,  // 0x40580300, 0x04

}) //End BUF0

Name(C004, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Function_SDCA_Version = 0.6, Class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 0x6},
  }
}) //End C004

Name(C005, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Function_Topology = 0x4 UAJ, Class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 0x4},
  }
}) //End C005

Name(C006, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Function_ManufacturerId = 0x025d, Class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 0x025d},
  }
}) //End C006

Name(C007, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Function_ID = 0x0711 (Device specific), Class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 0x0711},
  }
}) //End C007

Name(C008, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Function_Version = 0x1, Class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 0x1},
  }
}) //End C008

Name(E001, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x0b},
    Package (2) {"mipi-sdca-entity-label", "CS 41"},
    Package (2) {"mipi-sdca-cs-type", 2},
    Package (2) {"mipi-sdca-control-selector-list",  Package() {0x10} },  // Sample_Freq_Index
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "C110"},
  }
}) //End E001

Name(C110, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // Sample_Freq_Index, Class, RW
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package (2) {"mipi-sdca-control-number-range", "BUF1"},
  }
}) //End C110

Name(BUF1, Buffer()
{
  0x02, 0x00,  // Range type 0x0002
  0x04, 0x00,  // Count of ranges = 0x4
  0x08, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00,0x00, // 0x00000008, 0x0000AC44(44100)
  0x09, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00,0x00, // 0x00000009, 0x0000BB80(48000)
  0x0b, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01,0x00, // 0x0000000b, 0x00017700(96000)
  0x0d, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x02,0x00, // 0x0000000d, 0x0002EE00(192000)
}) //End BUF1

Name(E002, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x2},
    Package (2) {"mipi-sdca-entity-label", "IT 41"},
    Package (2) {"mipi-sdca-terminal-type", 0x0101},
    Package (2) {"mipi-sdca-control-selector-list",  Package() {0x8, 0x10, 0x11} },  // Latency, ClusterIndex, DataPortIndex
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "C210"},
    Package (2) {"mipi-sdca-controlselector-0x11-subproperties", "C211"},
    Package (2) {"mipi-sdca-clock-origin", "E001"},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package (2) {"mipi-sdca-terminal-dp-numbers", "BUF2"},
  }
}) //End E002

Name(BUF2, Buffer()
{
  0x0, 0x3, // DP_Index, DP_Num
}) //End BUF2

Name(LC00, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // Latency, Class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 0},
  }
}) //End LC00

Name(C210, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // ClusterIndex, Class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 1},
    Package (2) {"mipi-sdca-control-number-dc-value", 1},
  }
}) //End C210

Name(C211, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // DataPortIndex, Class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 1},
    Package (2) {"mipi-sdca-control-number-dc-value", 1},
  }
}) //End C211

Name(E003, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x0A},
    Package (2) {"mipi-sdca-entity-label", "XU 42"},
    Package (2) {"mipi-sdca-control-selector-interrupt-position", 6},// bit 6 in SDCA interrupt
    Package (2) {"mipi-sdca-control-selector-list",  Package() {0x1, 0x2, 0x6} },  // Bypass, ClusterIndex, latency
    Package (2) {"mipi-sdca-input-pin-list", 0x2},  // Input Pin 1
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    // Package (2) {"mipi-sdca-powerdomain-entity", "E050"},
    Package (2) {"mipi-sdca-input-pin-1", "E042"},   // Input Pin 1 connected to MU4 2
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "C301"},
    Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "C302"},
    Package (2) {"mipi-sdca-controlselector-0x6-subproperties", "LC00"},
  }
}) //End E003

Name(C301, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // Bypass, Class, R/W
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
  }
}) //End C301

Name(C302, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // ClusterIndex, Class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 1},
  }
}) //End C302

Name(E042, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x05},
    Package (2) {"mipi-sdca-entity-label", "MU 35"},
    Package (2) {"mipi-sdca-control-selector-list",  Package() {0x1,0x6} },  // mixer, latency
    Package (2) {"mipi-sdca-input-pin-list", 0x6}, // Input Pin 1 and 2 connected
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E002"},   // Input Pin 1 connected to IT41
    Package (2) {"mipi-sdca-input-pin-2", "E041"},   // Input Pin 1 connected to FU35
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "C421"},
    Package (2) {"mipi-sdca-controlselector-0x6-subproperties", "LC00"},
  }
}) //End E042

Name(C421, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // Bypass, Class, RW
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
  }
}) //End C421

Name(E005, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x07},
    Package (2) {"mipi-sdca-entity-label", "FU 42"},
    Package (2) {"mipi-sdca-control-selector-list",  Package() {0x1, 0x2, 0x10} },  // Mute, Volume, Latency
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E003"}, // Input Pin 1 connected to XU42
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "C501"},
    Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "C502"},
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "LC00"},//Latency
  }
}) //End E005

Name(C501, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // Mute, User, RW
    Package (2) {"mipi-sdca-control-selector-access-layer", 1},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} }, //channel 1 and 2
  }
}) //End C501

Name(C502, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // Volume, User, RW
    Package (2) {"mipi-sdca-control-selector-access-layer", 1},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} },
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package (2) {"mipi-sdca-control-number-range", "BUF3"},
  }
}) //End C502

Name(BUF3, Buffer()
{
  0x03, 0x00,  // Range type 0x0003
  0x01, 0x00,  // Count of ranges = 0x1
  0xc0, 0xbe, 0x00, 0x00,  // Min =0x0000bec0
  0x00, 0x00, 0x00, 0x00,  // Max =0x00000000
  0xc0, 0x00, 0x00, 0x00,  // Step =0x000000c0
}) //End BUF3

Name(E045, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x06},
    Package (2) {"mipi-sdca-entity-label", "SU 43"},
    //Package (2) {"mipi-sdca-group-entity", "E049"}, // No such property.
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2}},  // selctor, latency
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E005"}, // Input Pin 1 connected to FU42
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "C451"},
    Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "LC00"},
  }
}) //End E045

Name(C451, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // selector, Device, RO (SU controlled by GE)
    Package (2) {"mipi-sdca-control-selector-access-layer", 0x10},
    Package (2) {"mipi-sdca-control-selector-access-mode", 3},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} }, //channel 1 and 2
  }
}) //End C451

Name(E046, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x06},
    Package (2) {"mipi-sdca-entity-label", "SU 44"},
    //Package (2) {"mipi-sdca-group-entity", "E049"}, // No such property
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2} },  // selctor, latency
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E005"}, // Input Pin 1 connected to FU42
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "C461"},
    Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "LC00"},
  }
}) //End E046

Name(C461, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // selector, Device, RO (SU controlled by GE)
    Package (2) {"mipi-sdca-control-selector-access-layer", 0x10},
    Package (2) {"mipi-sdca-control-selector-access-mode", 3},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} }, //channel 1 and 2
  }
}) //End C461

Name(E047, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x06},
    Package (2) {"mipi-sdca-entity-label", "SU 45"},
    //Package (2) {"mipi-sdca-group-entity", "E049"}, // No such property
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2} },  // selctor, latency
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E005"}, // Input Pin 1 connected to FU42
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "C471"},
    Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "LC00"},
  }
}) //End E047

Name(C471, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // selector, Device, RO (SU controlled by GE)
    Package (2) {"mipi-sdca-control-selector-access-layer", 0x10},
    Package (2) {"mipi-sdca-control-selector-access-mode", 3},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} }, //channel 1 and 2
  }
}) //End C471

Name(E028, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x11},
    Package (2) {"mipi-sdca-entity-label", "PDE 47"},
    Package (2) {"mipi-sdca-powerdomain-state-list", 0xF},
    Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x6, 0x7, 0x48}},
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
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10}},  // Requested_PS, Actual_PS
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "RP28"},
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "AP28"},
  }
}) //End E028

Name(RP28, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // Requested_PS, Class, R/W
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
  }
}) //End RP28

Name(AP28, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // Actual_PS, Class, RO
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 3},
  }
}) //End AP28

Name(E006, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x03},
    Package (2) {"mipi-sdca-entity-label", "OT 43"},
    Package (2) {"mipi-sdca-terminal-type", 0x06C0},
    Package (2) {"mipi-sdca-reference-number", 0x1},
    Package (2) {"mipi-sdca-input-pin-list", 0x2},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x8} },  // Usage, Latency
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E045"},
    // Package (2) {"mipi-sdca-powerdomain-entity", "E028"},
    Package (2) {"mipi-sdca-controlselector-0x4-subproperties", "C604"},
    Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
  }
}) //End E006

Name(C604, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // Usage, Class, DC,
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 0x60}, // Full band
  }
}) //End C604

Name(E007, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x03},
    Package (2) {"mipi-sdca-entity-label", "OT 44"},
    Package (2) {"mipi-sdca-terminal-type", 0x0690},
    Package (2) {"mipi-sdca-reference-number", 0x1},
    Package (2) {"mipi-sdca-input-pin-list", 0x2},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x8} },  // Usage, Latency
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E046"},
    // Package (2) {"mipi-sdca-powerdomain-entity", "E028"},
    Package (2) {"mipi-sdca-controlselector-0x4-subproperties", "C704"},
    Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
  }
}) //End E007

Name(C704, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // Usage, Class, DC,
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 0x60}, // Full band
  }
}) //End C704

Name(E048, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x03},
    Package (2) {"mipi-sdca-entity-label", "OT 45"},
    Package (2) {"mipi-sdca-terminal-type", 0x06D0},
    Package (2) {"mipi-sdca-reference-number", 0x1},
    Package (2) {"mipi-sdca-input-pin-list", 0x2},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x8} },  // Usage, Latency
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E047"},
    // Package (2) {"mipi-sdca-powerdomain-entity", "E028"},
    Package (2) {"mipi-sdca-controlselector-0x4-subproperties", "C484"},
    Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
  }
}) //End E048

Name(C484, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // Usage, Class, DC,
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 0x60}, // Full band
  }
}) //End C484

Name(E049, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x12},
    Package (2) {"mipi-sdca-entity-label", "GE 35"},
    Package (2) {"mipi-sdca-ge-default-selectedmode", 3},// this is HP
        Package (2) {"msft-ge-mode-terminaltype-list",
      Package()
      {
        0x3, 0x06c0,   // Mode 3 maps to Headphone
        0x4, 0x0690,   // Mode 4 maps to Lineout stereo
        0x5, 0x06d0,   // Mode 5 maps to Headset
        0x6, 0x06a0,   // Mode 6 maps to Mic
        0x7, 0x0680,   // Mode 7 maps to LineIn stereo
      }},
        Package (2) {"msft-ge-managed-list",
      Package()
      {
        0x6,   // Entity Id of Headphone terminal
        0x7,   // Entity Id of Lineout stereo terminal
        0x48,   // Entity Id of Headset Speaker terminal
        0x8,   // Entity Id of to Mic terminal
        0x9,   // Entity Id of LineIn stereo terminal
        0x43,   // Entity Id of Headset Mic terminal
      }},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2}},  // seleted_mode, detected mode
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "GE01"},
    Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "GE02"}
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package (2) {"mipi-sdca-ge-selectedmode-controls-affected", "BUF4"},
  }
}) //End E049

Name(BUF4, Buffer()
{
  0x08, // Mode count

  0x00, // Mode 0, Unplugged
    0x04, // control count
      0xc, 0x1, 0x0, 0x0, // entity id, control selector, control number, value
      0x45, 0x1, 0x0, 0x0,
      0x46, 0x1, 0x0, 0x0,
      0x47, 0x1, 0x0, 0x0,
  0x01, // Mode 1, Unknown
    0x04, // control count
      0xc, 0x1, 0x0, 0x0,
      0x45, 0x1, 0x0, 0x0,
      0x46, 0x1, 0x0, 0x0,
      0x47, 0x1, 0x0, 0x0,
  0x02, // Mode 2, Undefined
    0x04, // control count
      0xc, 0x1, 0x0, 0x0,
      0x45, 0x1, 0x0, 0x0,
      0x46, 0x1, 0x0, 0x0,
      0x47, 0x1, 0x0, 0x0,
  0x03, // Mode 3, Headphone
    0x04, // control count
      0xc, 0x1, 0x0, 0x0,
      0x45, 0x1, 0x0, 0x1,
      0x46, 0x1, 0x0, 0x0,
      0x47, 0x1, 0x0, 0x0,
  0x04, // Mode 4, Lineout
    0x04, // control count
      0xc, 0x1, 0x0, 0x0,
      0x45, 0x1, 0x0, 0x0,
      0x46, 0x1, 0x0, 0x1,
      0x47, 0x1, 0x0, 0x0,
  0x05, // Mode 5, Headset
    0x04, // control count
      0xc, 0x1, 0x0, 0x3,
      0x45, 0x1, 0x0, 0x0,
      0x46, 0x1, 0x0, 0x0,
      0x47, 0x1, 0x0, 0x1,
  0x06, // Mode 6, Micin
    0x04, // control count
      0xc, 0x1, 0x0, 0x1,
      0x45, 0x1, 0x0, 0x0,
      0x46, 0x1, 0x0, 0x0,
      0x47, 0x1, 0x0, 0x0,
  0x07, // Mode 7, Linein
    0x04, // control count
      0xc, 0x1, 0x0, 0x2,
      0x45, 0x1, 0x0, 0x0,
      0x46, 0x1, 0x0, 0x0,
      0x47, 0x1, 0x0, 0x0,
}) //End BUF4

Name(GE01, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { //class, RW,
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0}
  }
})//End GE01

Name(GE02, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { //class, RO,
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-interrupt-position", 0},// bit 0 in SDCA interrupt
    Package (2) {"mipi-sdca-control-selector-access-mode", 3}
  }
})//End GE02

Name(E041, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x07},
    Package (2) {"mipi-sdca-entity-label", "FU 35"},
    // Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2, 0x10} },  // Mute, Volume, Latency
      Package (2) {"mipi-sdca-control-selector-list", Package() {0x10} },  // Latency Only. Sidetone Volume & Mute are disabled
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E00C"}, // Input Pin 1 connected to
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "C411"},
    Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "C412"},
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "LC00"},//Latency
  }
}) //End E041

Name(C411, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // Mute, User, RW
    Package (2) {"mipi-sdca-control-selector-access-layer", 1},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} }, //channel 1 and 2
  }
}) //End C411

Name(C412, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // Volume, User, RW
    Package (2) {"mipi-sdca-control-selector-access-layer", 1},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} },
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package (2) {"mipi-sdca-control-number-range", "BUF5"},
  }
}) //End C412

Name(BUF5, Buffer()
{
  0x03, 0x00,  // Range type 0x0003
  0x01, 0x00,  // Count of ranges = 0x1
  0xc0, 0xee, 0x00, 0x00,  // Min =0x0000eec0
  0x00, 0x1e, 0x00, 0x00,  // Max =0x00001e00
  0xc0, 0x00, 0x00, 0x00,  // Step =0x000000c0
}) //End BUF5

Name(E029, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x11},
    Package (2) {"mipi-sdca-entity-label", "PDE 34"},
    Package (2) {"mipi-sdca-powerdomain-state-list", 0xF},
    Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x8, 0x9, 0x43}},
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
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10}},  // Requested_PS, Actual_PS
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "RP29"},
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "AP29"},
  }
}) //End E029

Name(RP29, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // Requested_PS, Class, R/W
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
  }
}) //End RP29

Name(AP29, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // Actual_PS, Class, RO
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 3},
  }
}) //End AP29

Name(E008, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x02},
    Package (2) {"mipi-sdca-entity-label", "IT 31"},
    Package (2) {"mipi-sdca-terminal-type", 0x06A0},
    Package (2) {"mipi-sdca-reference-number", 0x1},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x8, 0x10}},  // TODO: Do we need Mic Bias control here? Class driver recognizes the control but does not do anything with it.
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    // Package (2) {"mipi-sdca-powerdomain-entity", "E029"},
    Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "C810"},
  }
}) //End E008

Name(C810, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  //cluster, class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 1},
  }
}) //End C810

Name(E009, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x02},
    Package (2) {"mipi-sdca-entity-label", "IT 32"},
    Package (2) {"mipi-sdca-terminal-type", 0x0680},
    Package (2) {"mipi-sdca-reference-number", 0x1},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x8, 0x10}},
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    // Package (2) {"mipi-sdca-powerdomain-entity", "E029"},
    Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "C910"},
  }
}) //End E009

Name(C910, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  //cluster, class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 1},
  }
}) //End C910

Name(E043, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x02},
    Package (2) {"mipi-sdca-entity-label", "IT 33"},
    Package (2) {"mipi-sdca-terminal-type", 0x06D0},
    Package (2) {"mipi-sdca-reference-number", 0x1},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x8, 0x10}},  // TODO: Do we need Mic Bias control here? Class driver recognizes the control but does not do anything with it.
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    // Package (2) {"mipi-sdca-powerdomain-entity", "E029"},
    Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "C431"},
  }
}) //End E043

Name(C431, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  //cluster, class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 1},
  }
}) //End C431

Name(E00A, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x07},
    Package (2) {"mipi-sdca-entity-label", "FU 31"},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0xb, 0x10} },  // boost gain, Latency
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E008"}, // Input Pin 1 connected to XU03
    Package (2) {"mipi-sdca-controlselector-0xb-subproperties", "CA0B"},
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "LC00"},//Latency
  }
}) //End E00A

Name(CA0B, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // boost gain, platform, RW
    Package (2) {"mipi-sdca-control-selector-access-layer", 8},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} },
  }
}) //End CA0B

Name(E00B, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x07},
    Package (2) {"mipi-sdca-entity-label", "FU 32"},
    Package (2) {"mipi-sdca-control-selector-list", Package() { 0xb, 0x10} },  // boost gain, Latency
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E009"}, // Input Pin 1 connected to XU03
    Package (2) {"mipi-sdca-controlselector-0xb-subproperties", "CB0B"},
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "LC00"},//Latency
  }
}) //End E00B

Name(CB0B, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // boost gain, platform, RW
    Package (2) {"mipi-sdca-control-selector-access-layer", 8},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} },
  }
}) //End CB0B

Name(E044, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x07},
    Package (2) {"mipi-sdca-entity-label", "FU 33"},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0xb, 0x10} },  // boost gain, Latency
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E043"}, // Input Pin 1 connected to XU03
    Package (2) {"mipi-sdca-controlselector-0xb-subproperties", "C44B"},
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "LC00"},//Latency
  }
}) //End E044

Name(C44B, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // gain, platform, RW
    Package (2) {"mipi-sdca-control-selector-access-layer", 8},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} },
  }
}) //End C44B

Name(E00C, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x06},
    Package (2) {"mipi-sdca-entity-label", "SU 35"},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2} },  // selctor, latency
    //Package (2) {"mipi-sdca-group-entity", "E049"},
    Package (2) {"mipi-sdca-input-pin-list", 0xE}, // Input Pin 1/2/3connected
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E00A"}, // Input Pin 1 connected to FU31
    Package (2) {"mipi-sdca-input-pin-2", "E00B"}, // Input Pin 1 connected to FU32
    Package (2) {"mipi-sdca-input-pin-3", "E044"}, // Input Pin 1 connected to FU33
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "CC01"},
    Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "LC00"},
  }
}) //End E00C

Name(CC01, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // selector, Device, RO (SU controlled by GE)
    Package (2) {"mipi-sdca-control-selector-access-layer",0x10},
    Package (2) {"mipi-sdca-control-selector-access-mode", 3},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} }, //channel 1 and 2
  }
}) //End CC01

Name(E00D, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x0A},
    Package (2) {"mipi-sdca-entity-label", "XU 36"},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2, 0x06} },  // Bypass, ClusterIndex, Latency
    Package (2) {"mipi-sdca-input-pin-list", 0x2},  // Input Pin 1
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    // Package (2) {"mipi-sdca-powerdomain-entity", "E051"},
    Package (2) {"mipi-sdca-input-pin-1", "E00C"},   // Input Pin 1 connected to MU4 2
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "CD01"},
    Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "CD02"},
    Package (2) {"mipi-sdca-controlselector-0x6-subproperties", "LC00"},
  }
}) //End E00D

Name(CD01, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // Bypass, Class, R/W
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
  }
}) //End CD01

Name(CD02, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // ClusterIndex, Class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 1},
  }
}) //End CD02

Name(E00F, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x07},
    Package (2) {"mipi-sdca-entity-label", "FU 36"},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2, 0x10} },  // Mute, Volume, Latency
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E00D"}, // Input Pin 1 connected to
    Package (2) {"mipi-sdca-controlselector-0x1-subproperties", "CF01"},
    Package (2) {"mipi-sdca-controlselector-0x2-subproperties", "CF02"},
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "LC00"},//Latency
  }
}) //End E00F

Name(CF01, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // Mute, User, RW
    Package (2) {"mipi-sdca-control-selector-access-layer", 1},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} }, //channel 1 and 2
  }
}) //End CF01

Name(CF02, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // Volume, User, RW
    Package (2) {"mipi-sdca-control-selector-access-layer", 1},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
    Package (2) {"mipi-sdca-control-number-list", Package() {0x1, 0x2} },
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package (2) {"mipi-sdca-control-number-range", "BUF5"},
  }
}) //End CF02

Name(E010, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x03},
    Package (2) {"mipi-sdca-entity-label", "OT 36"},
    Package (2) {"mipi-sdca-terminal-type", 0x0101},
    Package (2) {"mipi-sdca-input-pin-list", 0x2},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x8, 0x11}},  // Latency, DataPortIndex
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E00F"},
    Package (2) {"mipi-sdca-clock-origin", "E011"},
    Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "LC00"},
    Package (2) {"mipi-sdca-controlselector-0x11-subproperties", "C101"},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package (2) {"mipi-sdca-terminal-dp-numbers", "BUF6"},
  }
}) //End E010

Name(BUF6, Buffer()
{
  0x0, 0x2, // DP_Index, DP_Num
}) //End BUF6

Name(C101, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // DataPortIndex, Class, DC
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 5},
    Package (2) {"mipi-sdca-control-number-dc-value", 1},
  }
}) //End C101

Name(E011, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x0b},
    Package (2) {"mipi-sdca-entity-label", "CS 36"},
    Package (2) {"mipi-sdca-cs-type", 2},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x10} },  // Sample_Freq_Index
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "C111"},
  }
}) //End E011

Name(C111, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  { // Sample_Freq_Index, Class, RW
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 0},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package (2) {"mipi-sdca-control-number-range", "BUF7"},
  }
}) //End C111

Name(BUF7, Buffer()
{
  0x02, 0x00,  // Range type 0x0002
  0x04, 0x00,  // Count of ranges = 0x4
  0x08, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00,0x00, // 0x00000008, 0x0000AC44(44100)
  0x09, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00,0x00, // 0x00000009, 0x0000BB80(48000)
  0x0b, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01,0x00, // 0x0000000b, 0x00017700(96000)
  0x0d, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x02,0x00, // 0x0000000d, 0x0002EE00(192000)
}) //End BUF7

Name(EXT0, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {
    Package(2) {"mipi-sdca-function-expansion-subsystem-id", 0x1234 },
    Package(2) {"mipi-sdca-function-expansion-subsystem-revision-id", 0x1234},
    // both render and capture are supported
    Package(2) {"realtek-ge-supported-terminals", Package() {0x680, 0x6A0, 0x6D0, 0x690, 0x6C0} }, // LineIn Stereo, Mic on Jack, Headset, LineOut Stereo, Headphone on Jack
    // capture is disabled
    //Package(2) {"realtek-ge-supported-terminals", Package() {0x0, 0x0, 0x0, 0x690, 0x6C0} }, // LineOut Stereo, Headphone on Jack
    // render is disabled
    //Package(2) {"realtek-ge-supported-terminals", Package() {0x680, 0x6A0, 0x0, 0x0, 0x0} }, // LineIn Stereo, Mic on Jack
  }
}) //End EXT0
