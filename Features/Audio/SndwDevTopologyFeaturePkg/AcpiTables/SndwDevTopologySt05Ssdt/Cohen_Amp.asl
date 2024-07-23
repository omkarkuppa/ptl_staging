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

#ifndef CS42L43_FU_21_VOL_MIN
// Min = 0x0000C000 (-64.0 dB) in Q7.8 format
# define CS42L43_FU_21_VOL_MIN 0x00, 0xC0, 0x00, 0x00
#endif

#ifndef CS42L43_FU_21_VOL_MAX
// Max = 0x00000000 (0.0 dB) in Q7.8 format
# define CS42L43_FU_21_VOL_MAX 0x00, 0x00, 0x00, 0x00
#endif

#ifndef CS42L43_FU_21_VOL_STEP
// Step = 0x00000008 (1/32 dB per step) in Q7.8 format
# define CS42L43_FU_21_VOL_STEP 0x08, 0x00, 0x00, 0x00
#endif

#ifndef CS42L43_FU_21_VOL_DEFAULT
// Default = 0x0 (0 dB) in Q7.8 format
# define CS42L43_FU_21_VOL_DEFAULT 0x0
#endif

Name(_DSD, Package()
{
    // Device properties UUID
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package() {
        Package(2) { "mipi-sdw-sw-interface-revision", 0x00010001 },
        Package(2) { "mipi-sdca-function-topology-features", 0x0},
        Package(2) { "mipi-sdca-control-list", 0x1F0 }, // Function-Level Controls (0x4, 0x5, 0x6, 0x7, 0x8)
        // Not included: Security/Privacy signaling, TG_23
        //Package(2) { "mipi-sdca-entity-id-list", Package() {0x8, 0x6, 0x4, 0x5, 0x10, 0x1, 0x7, 0x9, 0xA, 0xB, 0x2, 0xC, 0x11, 0x12}},
        Package(2) { "mipi-sdca-entity-id-list", Package() {0x8, 0x6, 0x4, 0x10, 0x1, 0x7, 0x9, 0xA, 0xB, 0x2, 0xC, 0x11, 0x12}},
        Package(2) { "mipi-sdca-function-busy-max-delay", 5000},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        // Function-level
        Package(2) { "mipi-sdca-control-0x4-subproperties", "C004"},
        Package(2) { "mipi-sdca-control-0x5-subproperties", "C005"},
        Package(2) { "mipi-sdca-control-0x6-subproperties", "C006"},
        Package(2) { "mipi-sdca-control-0x7-subproperties", "C007"},
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C008"},

        // Entity-level
        Package(2) { "mipi-sdca-entity-id-0x1-subproperties", "E001"},
        Package(2) { "mipi-sdca-entity-id-0x2-subproperties", "E002"},
        Package(2) { "mipi-sdca-entity-id-0x3-subproperties", "E003"},
        Package(2) { "mipi-sdca-entity-id-0x4-subproperties", "E004"},
        //Package(2) { "mipi-sdca-entity-id-0x5-subproperties", "E005"},
        Package(2) { "mipi-sdca-entity-id-0x6-subproperties", "E006"},
        Package(2) { "mipi-sdca-entity-id-0x7-subproperties", "E007"},
        Package(2) { "mipi-sdca-entity-id-0x8-subproperties", "E008"},
        Package(2) { "mipi-sdca-entity-id-0x9-subproperties", "E009"},
        Package(2) { "mipi-sdca-entity-id-0xA-subproperties", "E00A"},
        Package(2) { "mipi-sdca-entity-id-0xB-subproperties", "E00B"},
        Package(2) { "mipi-sdca-entity-id-0xC-subproperties", "E00C"},
        Package(2) { "mipi-sdca-entity-id-0xD-subproperties", "E00D"},
        Package(2) { "mipi-sdca-entity-id-0xE-subproperties", "E00E"},
        Package(2) { "mipi-sdca-entity-id-0xF-subproperties", "E00F"},
        Package(2) { "mipi-sdca-entity-id-0x10-subproperties", "E010"},
        Package(2) { "mipi-sdca-entity-id-0x11-subproperties", "E011"},
        Package(2) { "mipi-sdca-entity-id-0x12-subproperties", "E012"},

        // Vendor-specific
        Package(2) { "mipi-sdca-function-expansion-subproperties", "EXT0"},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        // Class driver will print warnings if these are not defined.
        // To not pollute the class driver logs, simply do not define these if we are not using them.
        //Package(2) { "mipi-sdca-function-initialization-table", "BUF0"},
        //Package(2) { "msft-stream-start-table", "BUF1"},
        //Package(2) { "msft-stream-stop-table", "BUF2"},
    }
}) // End _DSD




// +====================================+
// |    Function level controls         |
// +====================================+


// Function initialization-table
Name(BUF0, Buffer()
{
}) // End BUF0

// Stream start table
Name(BUF1, Buffer()
{
}) // End BUF1

// Stream stop table
Name(BUF2, Buffer()
{
}) // End BUF2

Name(EXT0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-function-expansion-subsystem-id", 0 },  // MIPI required, but not used by MSFT
        Package(2) { "01fa-chip-id", 0x4243 },
        Package(2) { "01fa-ssid-ex", 0x7 },
        //
        // This property is used to select HWKWS FW.
        // Since the audio function performing the FDL is selected at random,
        // all audio functions involved with FDL must have it defined.
        Package(2) {"01fa-xu-features", (FEATURE_ENABLE_HWKWS | FEATURE_ENABLE_WT | FEATURE_ENABLE_KNCK)},
#ifdef COHEN_TWEETER_JAMERSON_WOOFER
        Include ("Sidecar_Cohen_Tweeter_Jamerson_Woofer.asl")
#endif
    }
}) // End EXT0


// Function-level control definitions
Name(C004, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Function_SDCA_Version: Class, DC = 0.6
        Package(2) { "mipi-sdca-control-access-layer", 4 },
        Package(2) { "mipi-sdca-control-access-mode", 5 },
        Package(2) { "mipi-sdca-control-dc-value", 0x6 },
    }
}) // End C004

Name(C005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Function_Type: Class, DC = 0x1 (Smart Amp)
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x1},
    }
}) // End C005

Name(C006, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Function_Manufacturer_Id: Class, DC = 0x01FA
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x01FA},
    }
}) // End C006

Name(C007, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Function_Id: Class, DC = 0x4243
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x4243},
    }
}) // End C007

Name(C008, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Shows up as VER in HWID
        // Function_Version: Class, DC = 0x1
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x1},
    }
}) // End C008

// DataPortMap1
Name(DPM1, Buffer() {
    0x1, 0x5, // {DP_Index, DP_Num}
}) // End DPM1

// DataPortMap2
Name(DPM2, Buffer() {
    0x2, 0x3, // {DP_Index, DP_Num}
}) // End DPM2



// +====================================+
// |        Entity Definitions          |
// +====================================+


// +------------------------------------+
// |                IT_21               |
// +------------------------------------+
Name(E006, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x02},
        Package(2) { "mipi-sdca-entity-label", "IT 21"},
        Package(2) { "mipi-sdca-terminal-type", 0x101},  // Speaker render stream. Generic Source / Sink Data Port
        Package(2) { "mipi-sdca-control-list", 0x20100}, // Latency, DataPortSelector
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C108"},  // Reuse
        Package(2) { "mipi-sdca-control-0x11-subproperties", "C111"}, // DataPortSelector
        Package(2) { "mipi-sdca-terminal-clock-connection", "E008" }, // CS_21
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "DPM1"},
    }
}) // End E006


Name(C108, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Latency: Class, DC = 0x4 (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x4},
    }
}) // End C108


// DataPortSelector for IT_21
Name(C111, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // DataPortSelector: Class, DC, *index* = 1 (see {DP_Index, DP_Num} map in DPM1)
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    }
}) // End C111



// +------------------------------------+
// |               PPU_21               |
// +------------------------------------+
Name(E004, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x25},
        Package (2) {"mipi-sdca-entity-label", "PPU 21"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x00000 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E006"}, // Input Pin 1 connected to IT_21
    }
}) // End E004



// +------------------------------------+
// |                CS_21               |
// +------------------------------------+
Name(E008, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0xB},
        Package(2) { "mipi-sdca-entity-label", "CS 21"},
        Package(2) { "mipi-sdca-cs-type", 0}, // External
        Package(2) { "mipi-sdca-control-list", 0x10000 },  // SampleRateIndex
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C210"},
    }
}) // End E008

Name(C210, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // SampleRateIndex: Class, RW
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 0},
        //Package(2) { "mipi-sdca-control-dc-value", 0},
        Package(2) { "mipi-sdca-control-deferrable", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "B210"},
    }
}) // End C210

// {SampleRateIndex, SampleRate} mapping.
Name(B210, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x01, 0x00,  // Count of ranges = 0x2
    0x00, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000000, SampleRate = 0x0000BB80 (48000)
    //0x01, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // SampleRateIndex = 0x00000001, SampleRate = 0x0000AC44 (44100)
    //0x02, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // SampleRateIndex = 0x00000002, SampleRate = 96000 (0x00017700)
    //0x03, 0x00, 0x00, 0x00, 0x80, 0x3e, 0x00, 0x00, // SampleRateIndex = 0x00000003, SampleRate = 16000 (0x00003e80)
}) // End B210





// +------------------------------------+
// |                FU_21               |
// +------------------------------------+

Name(E005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 21"},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
        Package(2) { "mipi-sdca-control-list", 0x10006 }, // Mute, Channel Volume, Latency
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C601"}, // Mute
        Package(2) { "mipi-sdca-control-0x2-subproperties", "C602"}, // Volume
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C610"}, // Latency
        Package(2) { "mipi-sdca-input-pin-1", "E004"}, // Input Pin 1 connected to PPU_21
    }
}) // End E010


// FU_21 Volume map
Name(VMAP, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    CS42L43_FU_21_VOL_MIN,
    CS42L43_FU_21_VOL_MAX,
    CS42L43_FU_21_VOL_STEP,
}) // End VMAP


Name(C601, Package()
{
    // Mute
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Mute: User, Dual, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", 1},
        Package(2) { "mipi-sdca-control-access-mode", 1},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6 }, // 0x1, 0x2
    }
}) // End C601

Name(C602, Package()
{
    // Volume
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Channel Volume: User, Dual, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", 1},
        Package(2) { "mipi-sdca-control-access-mode", 1},
        Package(2) { "mipi-sdca-control-default-value", CS42L43_FU_21_VOL_DEFAULT},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6 }, // 0x1, 0x2
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "VMAP"},
    }
}) // End C602

Name(C610, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Value = 0x8
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x8}, // Dummy value for test
    }
}) // End C610




// +------------------------------------+
// |                MFPU_21             |
// +------------------------------------+
Name(E010, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x22},
        Package(2) { "mipi-sdca-entity-label", "MFPU 21"},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
        Package(2) { "mipi-sdca-control-list", 0x1 }, // Bypass
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C501"},
        //Package(2) { "mipi-sdca-input-pin-1", "E005"}, // Input Pin 1 connected to FU_21
        Package(2) { "mipi-sdca-input-pin-1", "E004"}, // Input Pin 1 connected to PPU_21
    }
}) // End E010

Name(C501, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Bypass: Class, DC = 1
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x1},
    }
}) // End C501





// +------------------------------------+
// |                XU_22               |
// +------------------------------------+
Name(E001, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x0A},
        Package(2) { "mipi-sdca-entity-label", "XU 22"},
        Package(2) { "mipi-sdca-input-pin-list", 0x6}, // Pin1 and Pin2 connected
        Package(2) { "mipi-sdca-control-list", 0x182 },  // no FDL Controls 0x1, 0x7, 0x8
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C901"}, // Bypass
        Package(2) { "mipi-sdca-control-0x7-subproperties", "C907"}, // XU_ID
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C908"}, // XU_Version
        Package(2) { "mipi-sdca-input-pin-1", "E010"}, // Input Pin 1 connected to MFPU_21
        Package(2) { "mipi-sdca-input-pin-2", "E007"}, // Input Pin 2 connected to IT_29
    }
}) // End E001

Name(C901, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Bypass, Class, RW.
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 0},
        Package(2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End C901

Name(C907, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // XU_ID: Class, DC = 1
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    }
}) // End C907

Name(C908, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // XU_Version: Class, DC = 0x10
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x10},
    }
}) // End C908



// +------------------------------------+
// |               SAPU_29              |
// +------------------------------------+
Name(E009, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x24},
        Package (2) {"mipi-sdca-entity-label", "SAPU 29"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x30000 }, // Protection_Mode, Protection_Status
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C039"},
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C040"},
        Package (2) {"mipi-sdca-input-pin-1", "E001"}, // Input Pin 1 connected to XU_22
    }
}) // End E009


Name(C039, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Protection_Mode: Class, RO
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 3},
        Package (2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End C039


Name(C040, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Protection_Status: Class, RO
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 3},
        Package (2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End C040



// +------------------------------------+
// |               UDMPU_23             |
// +------------------------------------+
Name(E00A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x21},
        Package (2) {"mipi-sdca-entity-label", "UDMPU 23"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x00000 }
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E009"}, // Input Pin 1 connected to SAPU_29
    }
}) // End E00A



// +------------------------------------+
// |                FU_23               |
// +------------------------------------+

Name(E00B, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 23"},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
        Package(2) { "mipi-sdca-control-list", 0x800 }, // Channel Gain
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0xb-subproperties", "C400"},
        Package(2) { "mipi-sdca-input-pin-1", "E00A"}, // Input Pin 1 connected to UDMPU_23
    }
}) // End E00B


Name(C400, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // ### Changed to Application because with Mic topo occasionally the write to the Mute control #2 would fail. This should be revisited.
        // Channel Gain: Application, RW = 0
        Package(2) { "mipi-sdca-control-access-layer", 2}, // Class ***Platform***
        Package(2) { "mipi-sdca-control-access-mode", 0}, // RW
        Package(2) { "mipi-sdca-control-cn-list", 0x1 } // Master Gain
    }
}) // End C400



// +------------------------------------+
// |               UDMPU_25             |
// +------------------------------------+
Name(E011, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x21},
        Package (2) {"mipi-sdca-entity-label", "UDMPU 25"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", 0x60000 }, // 0x11, 0x12
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C053"},
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C054"},
        Package (2) {"mipi-sdca-input-pin-1", "E00B"}, // Input Pin 1 connected to FU_23
    }
}) // End E011


Name(C053, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Acoustic_Energy_Level_Monitor: Platform, DC = 0
        Package (2) {"mipi-sdca-control-access-layer", 8},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) // End C053


Name(C054, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Ultrasound_Level_Report: Platform, DC = 0
        Package (2) {"mipi-sdca-control-access-layer", 8},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) // End C054




// +------------------------------------+
// |                OT_23               |
// +------------------------------------+
Name(E00C, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x03},
        Package(2) { "mipi-sdca-entity-label", "OT 23"},
        // ATR: Ideally, this should be 0x382 (Primary Tweeter Speaker Transducer),
        // but specifying this value instead of 0x380 causes the audio functions
        // to fail to initialize.
        Package(2) { "mipi-sdca-terminal-type", 0x0380},
        Package(2) { "mipi-sdca-input-pin-list", 0x2},
        Package(2) { "mipi-sdca-control-list", 0x100 }, // Usage, Latency
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x8-subproperties", "CB08"}, // Latency
        Package(2) { "mipi-sdca-input-pin-1", "E00B"}, // Input Pin 1 connected to FU_23
    }
}) // End E00C


Name(CB08, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Latency: Class, DC = 0xA (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0xA},
    }
}) // End CB08





// +------------------------------------+
// |                PDE_23              |
// +------------------------------------+
Name(E002, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x11 },
        Package(2) { "mipi-sdca-entity-label", "PDE 23"},
        Package(2)
        {
            "mipi-sdca-powerdomain-managed-list",
            Package() {0xC}, // Entities OT_23 managed by this PDE.
        },
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
        Package(2) { "mipi-sdca-control-list", 0x10002 }, // Requested_PS, Actual_PS
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "RPS0" }, // Reuse. Requested PS
        Package(2) { "mipi-sdca-control-0x10-subproperties", "APS0"}, // Reuse. Actual PS
    }
}) // End E002


Name(RPS0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Requested_PS, Class, R/W
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 0},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "PDER"},
    },
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
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 3},
    }
}) // End APS0




// +------------------------------------+
// |                OT_25               |
// +------------------------------------+
Name(E012, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) { "mipi-sdca-entity-type", 0x3},
        Package (2) { "mipi-sdca-entity-label", "OT 25"},
        Package (2) { "mipi-sdca-input-pin-list", 0x2},
        Package (2) { "mipi-sdca-terminal-type", 0x188},
        Package (2) { "mipi-sdca-control-list", 0x60000 }, // DataPort Selector, Matching_GUID
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) { "mipi-sdca-control-0x11-subproperties", "C056"},
        Package (2) { "mipi-sdca-control-0x12-subproperties", "C057"},
        Package (2) { "mipi-sdca-terminal-clock-connection", "E008"}, // CS_21
        Package (2) { "mipi-sdca-input-pin-1", "E011"}, // Input Pin 1 connected to UDMPU_25
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) { "mipi-sdca-terminal-dp-numbers", "DPM2"},
    }
}) // End E012


Name(C056, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // DataPort Selector: Class, DC = 4
        Package (2) { "mipi-sdca-control-access-layer", 0x4},
        Package (2) { "mipi-sdca-control-access-mode", 5},
        Package (2) { "mipi-sdca-control-dc-value", 0x4},
    }
}) // End C056


Name(C057, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Matching_GUID: Class, DC = 0
        Package (2) { "mipi-sdca-control-access-layer", 0x4},
        Package (2) { "mipi-sdca-control-access-mode", 5},
        Package (2) { "mipi-sdca-control-dc-value", 0},
    }
}) // End C057




// +------------------------------------+
// |                IT_29               |
// +------------------------------------+
Name(E007, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x2},
        Package (2) {"mipi-sdca-entity-label", "IT 29"},
        Package (2) {"mipi-sdca-terminal-type", 0x280},
        Package (2) {"mipi-sdca-control-list", 0x00100 }, // Latency
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x8-subproperties", "C059"},
    },
}) // End E007


Name(C059, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Latency: Class, DC = 7
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x7}, // Dummy Value
    }
}) // End C059




// +------------------------------------+
// |                TG_23               |
// +------------------------------------+
Name(E003, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x30},
        Package (2) {"mipi-sdca-entity-label", "TG 23"},
        Package (2) {"mipi-sdca-control-list", 0x10000 }, // Class_Divider
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C300"},
    },
}) // End E003


Name(C300, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Class_Divider: Platform, RW
        Package (2) {"mipi-sdca-control-access-layer", 8},
        Package (2) {"mipi-sdca-control-access-mode", 0},
    }
}) // End C300
