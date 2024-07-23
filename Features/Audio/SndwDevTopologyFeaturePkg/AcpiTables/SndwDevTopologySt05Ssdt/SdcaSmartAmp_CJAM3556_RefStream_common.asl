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

#ifndef CS35L56_FU_21_VOL_MIN
// Min = 0x00009A00 (-102.0 dB) in Q7.8 format
# define CS35L56_FU_21_VOL_MIN 0x00, 0x9A, 0x00, 0x00
#endif

#ifndef CS35L56_FU_21_VOL_MAX
// Max = 0x00000C00 (+12.0 dB) in Q7.8 format
# define CS35L56_FU_21_VOL_MAX 0x00, 0x0C, 0x00, 0x00
#endif

#ifndef CS35L56_FU_21_VOL_STEP
// Step = 0x00000020 (0.125 dB per step) in Q7.8 format
# define CS35L56_FU_21_VOL_STEP 0x20, 0x00, 0x00, 0x00
#endif

#ifndef CS35L56_FU_21_VOL_DEFAULT
// Default = 0x0 (0 dB) in Q7.8 format
# define CS35L56_FU_21_VOL_DEFAULT 0x0
#endif

#ifndef CS35L56_FU_23_VOL_MIN
// Min = 0x00009A00 (-102.0 dB) in Q7.8 format
# define CS35L56_FU_23_VOL_MIN 0x00, 0x9A, 0x00, 0x00
#endif

#ifndef CS35L56_FU_23_VOL_MAX
// Max = 0x00000C00 (+12.0 dB) in Q7.8 format
# define CS35L56_FU_23_VOL_MAX 0x00, 0x0C, 0x00, 0x00
#endif

#ifndef CS35L56_FU_23_VOL_STEP
// Step = 0x00000020 (0.125 dB per step) in Q7.8 format
# define CS35L56_FU_23_VOL_STEP 0x20, 0x00, 0x00, 0x00
#endif

#ifndef CS35L56_FU_23_VOL_DEFAULT
// Default = 0x0 (0 dB) in Q7.8 format
# define CS35L56_FU_23_VOL_DEFAULT 0x0
#endif

Name(_DSD, Package()
{ // Function Descriptor
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdw-sw-interface-revision", 0x00010001},
        Package(2) {"mipi-sdca-function-topology-features", 0x64},  // removed Sense Capture (OT24)
        Package(2) {"mipi-sdca-control-list", 0x1F0},  // Function level controls
        // Use below to add reference path
        //Package(2) {"mipi-sdca-entity-id-list", Package() {0x3, 0x2, 0x4, 0x1A, 0x8, 0x9, 0x16, 0x15, 0x17, 0x18, 0xA, 0xB, 0xE, 0xF, 0xD, 0x6, 0x10, 0x13, 0x14, 0x11, 0xC, 0x19} },  // List of entities in this function
        Package(2) {"mipi-sdca-entity-id-list", Package() {0x3, 0x2, 0x4, 0x1A, 0x8, 0x9, 0x16, 0x15, 0x17, 0x18, 0xA, 0xB, 0xE, 0xF, 0xD, 0x6, 0x10, 0xC, 0x19} },  // List of entities in this function
        // Use below to remove reference path
        //Package(2) {"mipi-sdca-entity-id-list", Package() {0x3, 0x2, 0x4, 0x1A, 0x8, 0x9, 0x16, 0x15, 0x17, 0x18, 0xA, 0xB, 0xE, 0xD, 0x6, 0x13, 0x14, 0x11, 0xC, 0x19} },  // List of entities in this function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-control-0x4-subproperties", "C004"},
        Package(2) {"mipi-sdca-control-0x5-subproperties", "C005"},
        Package(2) {"mipi-sdca-control-0x6-subproperties", "C006"},
        Package(2) {"mipi-sdca-control-0x7-subproperties", "C007"},
        Package(2) {"mipi-sdca-control-0x8-subproperties", "C008"},
        Package(2) {"mipi-sdca-entity-id-0x3-subproperties", "E003"},
        Package(2) {"mipi-sdca-entity-id-0x2-subproperties", "E002"},
        Package(2) {"mipi-sdca-entity-id-0x4-subproperties", "E004"},
        //Package(2) {"mipi-sdca-entity-id-0x5-subproperties", "E005"},  // FU 21 is removed.
        Package(2) {"mipi-sdca-entity-id-0x1A-subproperties", "E01A"},
        Package(2) {"mipi-sdca-entity-id-0x8-subproperties", "E008"},
        Package(2) {"mipi-sdca-entity-id-0x9-subproperties", "E009"},
        Package(2) {"mipi-sdca-entity-id-0x16-subproperties", "E016"},
        Package(2) {"mipi-sdca-entity-id-0x15-subproperties", "E015"},
        Package(2) {"mipi-sdca-entity-id-0x17-subproperties", "E017"},
        Package(2) {"mipi-sdca-entity-id-0x18-subproperties", "E018"},
        Package(2) {"mipi-sdca-entity-id-0xA-subproperties", "E00A"},
        Package(2) {"mipi-sdca-entity-id-0xB-subproperties", "E00B"},
        Package(2) {"mipi-sdca-entity-id-0xE-subproperties", "E00E"},
        Package(2) {"mipi-sdca-entity-id-0xD-subproperties", "E00D"},
        Package(2) {"mipi-sdca-entity-id-0xF-subproperties", "E00F"},
        Package(2) {"mipi-sdca-entity-id-0x10-subproperties", "E010"},
        Package(2) {"mipi-sdca-entity-id-0x6-subproperties", "E006"},
        //Package(2) {"mipi-sdca-entity-id-0x13-subproperties", "E013"},	// XU 24 is removed
        //Package(2) {"mipi-sdca-entity-id-0x14-subproperties", "E014"},	// OT 24 is removed
        //Package(2) {"mipi-sdca-entity-id-0x11-subproperties", "E011"},	// CS 24 is removed
        Package(2) {"mipi-sdca-entity-id-0xC-subproperties", "E00C"},
        Package(2) {"mipi-sdca-entity-id-0x19-subproperties", "E019"},
        Package(2) {"mipi-sdca-function-expansion-subproperties", "EXT0"},
    },
}) //End _DSD


Name(C004, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Class, DC, Function_SDCA_Version = 0.6
        Package(2) {"mipi-sdca-control-access-layer", 4},
        Package(2) {"mipi-sdca-control-access-mode", 5},
        Package(2) {"mipi-sdca-control-dc-value", 0x6},
    }
}) //End C004

Name(C005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Class, DC, Function_Type = 0x1 (Smart-Amp)
        Package(2) {"mipi-sdca-control-access-layer", 4},
        Package(2) {"mipi-sdca-control-access-mode", 5},
        Package(2) {"mipi-sdca-control-dc-value", 0x1},
    }
}) //End C005

Name(C006, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_Manufacturer_Id = 0x01FA
        Package(2) {"mipi-sdca-control-access-layer", 4},
        Package(2) {"mipi-sdca-control-access-mode", 5},
        Package(2) {"mipi-sdca-control-dc-value", 0x01FA},
    }
}) //End C006

Name(C007, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_Id = 0x3556
        Package(2) {"mipi-sdca-control-access-layer", 4},
        Package(2) {"mipi-sdca-control-access-mode", 5},
        Package(2) {"mipi-sdca-control-dc-value", 0x3556},
    }
}) //End C007

Name(C008, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Class, DC, Function_Version = 0x1
        Package(2) {"mipi-sdca-control-access-layer", 4},
        Package(2) {"mipi-sdca-control-access-mode", 5},
        Package(2) {"mipi-sdca-control-dc-value", 0x1},
    }
}) //End C008

Name(E003, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x2},
        Package(2) {"mipi-sdca-entity-label", "IT 21"},
        Package(2) {"mipi-sdca-terminal-type", 0x0101}, // Generic Sink DataPort
        Package(2) {"mipi-sdca-control-list", 0x20000}, // DataPortIndex
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-control-0x11-subproperties", "C021"},
        Package(2) {"mipi-sdca-terminal-clock-connection", "E002"},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-terminal-dp-numbers", "BUF3"},
    }
}) //End E003

Name(BUF3, Buffer()
{
    0x1, 0x1, // DP_Index: 0x1 DP_Num: 0x1
}) //End BUF3

Name(C021, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    { // DataPortIndex, Class, DC
        Package(2) {"mipi-sdca-control-access-layer", 4},
        Package(2) {"mipi-sdca-control-access-mode", 5},
        Package(2) {"mipi-sdca-control-dc-value", 1},
    }
}) //End C021

Name(E002, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x0b},
        Package(2) {"mipi-sdca-entity-label", "CS 21"},
        Package(2) {"mipi-sdca-cs-type", 0},
        Package(2) {"mipi-sdca-control-list", 0x10000},  // Sample_Freq_Index
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-control-0x10-subproperties", "C023"},
    }
}) //End E002

Name(C022, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    { // Clock_Valid, Class, RO
        Package(2) {"mipi-sdca-control-access-layer", 4},
        Package(2) {"mipi-sdca-control-access-mode", 3},
    }
}) //End C022

Name(C023, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    { // Sample_Freq_Index, Class, DC
        Package(2) {"mipi-sdca-control-access-layer", 0x4},
        Package(2) {"mipi-sdca-control-access-mode", 5},
        Package(2) {"mipi-sdca-control-dc-value", 0x3},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUF5"},
    }
}) //End C023

Name(BUF5, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x01, 0x00,  // Count of ranges = 0x1
    0x03, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00,0x00, // 0x00000003, 0x0000BB80(48000)  // V3: First byte now 0x03
}) //End BUF5

Name(E004, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x25},
        Package (2) {"mipi-sdca-entity-label", "PPU 21"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x00000},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E003"},
    }
}) //End E004

Name(E005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x07},
        Package(2) {"mipi-sdca-entity-label", "FU 21"},
        Package(2) {"mipi-sdca-control-list", 0x6},  // Mute, Volume
        Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-input-pin-1", "E004"},
        Package(2) {"mipi-sdca-control-0x1-subproperties", "C025"},
        Package(2) {"mipi-sdca-control-0x2-subproperties", "C026"},
    }
}) //End E005

Name(C025, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Mute, User, RW/Dual
        Package(2) {"mipi-sdca-control-access-layer", 1},
        Package(2) {"mipi-sdca-control-access-mode", 1},
        Package(2) {"mipi-sdca-control-cn-list", 0x6},
    }
}) //End C025

Name(C026, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Volume, User, RW/Dual
        Package(2) {"mipi-sdca-control-access-layer", 1},
        Package(2) {"mipi-sdca-control-access-mode", 1},
        Package(2) {"mipi-sdca-control-default-value", CS35L56_FU_21_VOL_DEFAULT},
        Package(2) {"mipi-sdca-control-cn-list", 0x6},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUF4"},
    }
}) //End C026

Name(BUF4, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    CS35L56_FU_21_VOL_MIN,
    CS35L56_FU_21_VOL_MAX,
    CS35L56_FU_21_VOL_STEP,
}) //End BUF4

Name(E01A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
        Package (2) {"mipi-sdca-entity-type", 0x22},
        Package (2) {"mipi-sdca-entity-label", "MFPU 21"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x2},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
        Package (2) {"mipi-sdca-input-pin-1", "E004"}, // Bypass FU21
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C027"},
    }
}) //End E01A

Name(C027, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 1},
    }
}) //End C027

Name(E008, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0xa},
        Package (2) {"mipi-sdca-entity-label", "XU 22"},
        Package (2) {"mipi-sdca-input-pin-list", 0x6},
        //Package (2) {"mipi-sdca-control-list", 0x7D0182},
        Package (2) {"mipi-sdca-control-list", 0x182},
        Package (2) {"mipi-sdca-firmware-dl-mode", 0},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-2", "E006"},
        Package (2) {"mipi-sdca-input-pin-1", "E01A"},
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C028"},
        Package (2) {"mipi-sdca-control-0x7-subproperties", "C029"},
        Package (2) {"mipi-sdca-control-0x8-subproperties", "C030"},
        Package (2) {"mipi-sdca-control-0x9-subproperties", "C031"},
        Package (2) {"mipi-sdca-control-0xA-subproperties", "C032"},
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C033"},
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C034"},
        Package (2) {"mipi-sdca-control-0x13-subproperties", "C035"},
        Package (2) {"mipi-sdca-control-0x14-subproperties", "C036"},
        Package (2) {"mipi-sdca-control-0x15-subproperties", "C037"},
        Package (2) {"mipi-sdca-control-0x16-subproperties", "C038"},
    }
}) //End E008

Name(C028, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C028

Name(C029, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 2}, // XU ID
    }
}) //End C029

Name(C030, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 1}, // XU Version
    }
}) //End C030

Name(C031, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) //End C031

Name(C032, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) //End C032

Name(C033, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-interrupt-position", 1},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 1},
    }
}) //End C033

Name(C034, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C034

Name(C035, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C035

Name(C036, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C036

Name(C037, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 3},
    }
}) //End C037

Name(C038, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C038

Name(E009, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x24},
        Package (2) {"mipi-sdca-entity-label", "SAPU 29"},
        Package (2) {"mipi-sdca-input-pin-list", 0x6},
        Package (2) {"mipi-sdca-control-list", 0x30000},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-2", "E006"},
        Package (2) {"mipi-sdca-input-pin-1", "E008"},
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C039"},
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C040"},
    }
}) //End E009

Name(C039, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 3},
    }
}) //End C039

Name(C040, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 3},
    }
}) //End C040

Name(E016, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x2},
        Package (2) {"mipi-sdca-entity-label", "IT 26"},
        Package (2) {"mipi-sdca-control-list", 0x20000},
        Package (2) {"mipi-sdca-terminal-type", 0x180},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C042"},
        Package (2) {"mipi-sdca-terminal-clock-connection", "E015"},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package() {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "BUF7"},
    }
}) //End E016

Name(BUF7, Buffer() {
    0x1, 0x2, // DP_Index, DP_Num
}) //End BUF7

Name(C042, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 2},
    }
}) //End C042

Name(E015, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0xb},
        Package (2) {"mipi-sdca-entity-label", "CS 26"},
        Package (2) {"mipi-sdca-cs-type", 0},
        Package (2) {"mipi-sdca-control-list", 0x10000}, // Sample_Freq_Index
        },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C043"},
    }
}) //End E015

Name(C043, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x3},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUF9"},
    }
}) //End C043

Name(BUF9, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x01, 0x00,  // Count of ranges = 0x1
    0x03, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x01, // 0x00000003, 0x00017700(96000)  // V3: First byte now 0x03
}) //End BUF9

Name(E017, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x25},
        Package (2) {"mipi-sdca-entity-label", "PPU 26"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x10000},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E016"},
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C044"},
    }
}) //End E017

Name(C044, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 1},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C044

Name(E018, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x22},
        Package (2) {"mipi-sdca-entity-label", "MFPU 26"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x00032},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E017"},
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C045"},
        Package (2) {"mipi-sdca-control-0x4-subproperties", "C046"},
        Package (2) {"mipi-sdca-control-0x5-subproperties", "C047"},
    }
}) //End E018

Name(C045, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) //End C045

Name(C046, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x283C},
    }
}) //End C046

Name(C047, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x283C},
    }
}) //End C047

Name(E00A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x5},
        Package (2) {"mipi-sdca-entity-label", "MU 26"},
        Package (2) {"mipi-sdca-input-pin-list", 0x6},
        Package (2) {"mipi-sdca-control-list", 0x2},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-2", "E019"},
        Package (2) {"mipi-sdca-input-pin-1", "E009"},
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C049"},
    }
}) //End E00A

Name(C049, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 0x8},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x1},
    }
}) //End C049

Name(E00B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x21},
        Package (2) {"mipi-sdca-entity-label", "UDMPU 23"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x10000},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E00A"},
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C050"},
    }
}) //End E00B

Name(C050, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},// Follow cluster definition of input pin 1
    }
}) //End C050

Name(E00E, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x03},
        Package(2) {"mipi-sdca-entity-label", "OT 23"},
        Package(2) {"mipi-sdca-terminal-type", 0x0380},// Primary Full Speaker
        Package(2) {"mipi-sdca-input-pin-list", 0x2},
        Package(2) {"mipi-sdca-control-list", 0x0},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-input-pin-1", "E00C"},
    }
}) //End E00E

Name(E00D, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x11},
        Package(2) {"mipi-sdca-entity-label", "PDE 23"},
        Package(2)
        {
            "mipi-sdca-powerdomain-managed-list", Package() {0xE},
        },
        Package(2) {"mipi-sdca-control-list", 0x10002}, // Requested_PS, Actual_PS
        Package(2)
        {
            "mipi-sdca-powerdomain-transition-typical-delay",
            Package()
            {
                3, 0, 500000,  // From_PS, To_PS, Delay in microseconds
                0, 3, 500000,
            },
        },
        Package(2)
        {
            "mipi-sdca-powerdomain-transition-max-delay",
            Package()
            {
                3, 0, 800000,  // From_PS, To_PS, Delay in microseconds
                0, 3, 800000,
            },
        },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-control-0x1-subproperties", "RPS0" },
        Package(2) {"mipi-sdca-control-0x10-subproperties", "APS0"},
    }
}) //End E00D

Name(RPS0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Requested_PS, Class, R/W 
        Package(2) { "mipi-sdca-control-access-layer", 4 },
        Package(2) { "mipi-sdca-control-access-mode", 0},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "PDER"},
    }
}) // End RPS0

Name(PDER, Buffer()
{
    0x01, 0x00,  // Range type 0x0001
    0x02, 0x00,  // Count of ranges = 0x2
    0x00, 0x00, 0x00, 0x00, // 0 --> PS0
    0x03, 0x00, 0x00, 0x00, // 3 --> PS3
}) // End PDER

Name(APS0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Actual_PS, Class, RO
         Package(2) { "mipi-sdca-control-access-layer", 4 },
         Package(2) {"mipi-sdca-control-access-mode", 3},
    }
}) //End APS0

Name(E00F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x21},
        Package (2) {"mipi-sdca-entity-label", "UDMPU 25"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x70000},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E00C"},
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C052"},
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C053"},
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C054"},
    }
}) //End E00F

Name(C052, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) //End C052

Name(C053, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 8},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) //End C053

Name(C054, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 8},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) //End C054

Name(E010, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x3},
        Package (2) {"mipi-sdca-entity-label", "OT 25"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x60000},
        Package (2) {"mipi-sdca-terminal-type", 0x188},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E00F"},
        Package (2) {"mipi-sdca-terminal-clock-connection", "E002"},
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C056"},
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C057"},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "BUF6"},
    }
}) //End E010

Name(BUF6, Buffer() {
    0x1, 0x4, // DP_Index, DP_Num
}) //End BUF6

Name(C056, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x4},
    }
}) //End C056

Name(C057, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x0},
    }
}) //End C057

Name(E006, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x2},
        Package (2) {"mipi-sdca-entity-label", "IT 29"},
        Package (2) {"mipi-sdca-terminal-type", 0x280},
        Package (2) {"mipi-sdca-control-list", 0x00000},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
    },
}) //End E006

Name(E013, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0xA},
        Package (2) {"mipi-sdca-entity-label", "XU 24"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x2},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E006"},
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C060"},
    }
}) //End E013

Name(C060, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C060

Name(E014, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x3},
        Package (2) {"mipi-sdca-entity-label", "OT 24"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x60000},
        Package (2) {"mipi-sdca-terminal-type", 0x189},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E013"},
        Package (2) {"mipi-sdca-terminal-clock-connection", "E011"},
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C062"},
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C063"},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "BUFA"},
    }
}) //End E014

Name(BUFA, Buffer() {
    0x1, 0x3, // DP_Index, DP_Num
}) //End BUFA

Name(C062, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x3},
    }
}) //End C062

Name(C063, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x0},
    }
}) //End C063

Name(E011, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0xb},
        Package (2) {"mipi-sdca-entity-label", "CS 24"},
        Package (2) {"mipi-sdca-cs-type", 0},
        Package (2) {"mipi-sdca-control-list", 0x10000}, // Sample_Freq_Index
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C064"},
    }
}) //End E011

Name(C064, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    { // Sample_Freq_Index, Class, DC
        Package (2) {"mipi-sdca-control-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x3},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUF8"},
    }
}) //End C064

Name(BUF8, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x01, 0x00,  // Count of ranges = 0x1
    0x03, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00,0x00, // 0x00000003, 0x0000BB80(48000)  // V3: First byte now 0x03
}) //End BUF8

Name(E00C, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x07},
        Package(2) {"mipi-sdca-entity-label", "FU 23"},
        Package(2) {"mipi-sdca-control-list", 0x6},  // Mute, Volume
        Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-input-pin-1", "E00B"},
        Package(2) {"mipi-sdca-control-0x1-subproperties", "C02A"},
        Package(2) {"mipi-sdca-control-0x2-subproperties", "C02B"},
    }
}) //End E00C

Name(C02A, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Mute, Platform, RW/Dual
        Package(2) {"mipi-sdca-control-access-layer", 0x8},
        Package(2) {"mipi-sdca-control-access-mode", 1},
        Package(2) {"mipi-sdca-control-cn-list", 0x6},
    }
}) //End C02A

Name(C02B, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Volume, Platform, RW/Dual
        Package(2) {"mipi-sdca-control-access-layer", 0x8},
        Package(2) {"mipi-sdca-control-access-mode", 1},
        Package(2) {"mipi-sdca-control-default-value", CS35L56_FU_23_VOL_DEFAULT},
        Package(2) {"mipi-sdca-control-cn-list", 0x6},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUFB"},
    }
}) //End C02B

Name(BUFB, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    CS35L56_FU_23_VOL_MIN,
    CS35L56_FU_23_VOL_MAX,
    CS35L56_FU_23_VOL_STEP,
}) //End BUFB

Name(E019, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x07},
        Package(2) {"mipi-sdca-entity-label", "FU 26"},
        Package(2) {"mipi-sdca-control-list", 0x6},  // Mute, Volume
        Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-input-pin-1", "E018"},
        Package(2) {"mipi-sdca-control-0x1-subproperties", "C02A"},
        Package(2) {"mipi-sdca-control-0x2-subproperties", "C02B"},
    }
}) //End E019

Name(EXT0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-function-expansion-subsystem-id", 0 },  // MIPI required, but not used by MSFT
        Package(2) { "01fa-chip-id", 0x3556 },
        Package(2) { "01fa-ssid-ex", 0x7 },
        Package(2) { "01fa-xu-features", (FEATURE_ENABLE_HWKWS | FEATURE_ENABLE_WT | FEATURE_ENABLE_KNCK)},
    }
}) //End EXT0
