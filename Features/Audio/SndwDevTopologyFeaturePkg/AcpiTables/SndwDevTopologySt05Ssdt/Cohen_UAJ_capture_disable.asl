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

// Function initialization-table
Name(BUF0, Buffer()
{
    // disable UAJ input, by default enabled
    0x0c, 0xfe, 0x11, 0x00, 0x04,   // 0x11FE0C = 0x4
    0x0d, 0xfe, 0x11, 0x00, 0x00,
    0x0e, 0xfe, 0x11, 0x00, 0x00,
    0x0f, 0xfe, 0x11, 0x00, 0x00,
}) // End BUF0


Name(EXT0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-function-expansion-subsystem-id", 0 },  // MIPI required, but not used by MSFT
        Package(2) { "01fa-chip-id", 0x4243 },
        Package(2) { "01fa-ssid-ex", 0x7 },
        Package(2) { "01fa-supported-jack-types-mask", 0x2F },    // msft-ge-mode-terminaltype-list bitmap
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


// +------------------------------------+
// |                GE_35               |
// +------------------------------------+
Name(E011, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x12},
        Package(2) { "mipi-sdca-entity-label", "GE 35"},
        Package(2) { "mipi-sdca-ge-default-selectedmode", 0x0},
        Package(2) { "mipi-sdca-ge-managed-reference-number", 0x01},
        Package (2) {"msft-ge-mode-terminaltype-list",
            Package()
            {
                // HP, HS, LineOut, LineIn, StereoMic
                0x3, 0x06C0,   // Mode 3 maps to Headphone
                //0x4, 0x06D0,   // Mode 4 maps to Headset
                0x5, 0x0690,   // Mode 5 maps to LineOut stereo
                //0x6, 0x0680,   // Mode 6 maps to LineIn stereo
                //0x7, 0x06A0,   // Mode 7 maps to StereoMic
            }},
            Package (2) {"msft-ge-managed-list",
            Package()
            {
                0x13,   // Entity Id of Headphone terminal
                0x17,   // Entity Id of Headset Speaker terminal
                0x15,   // Entity Id of Lineout stereo terminal
                0x9,    // Entity Id of LineIn stereo terminal
                0x7,    // Entity Id of to Mic terminal
                0xB,    // Entity Id of Headset Mic terminal
            }},
        Package(2) { "mipi-sdca-control-list", 0x6},  // Selected_Mode, Detected_Mode
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "CE01"}, // Selected_Mode
        Package(2) { "mipi-sdca-control-0x2-subproperties", "CE02"}, // Detected_Mode
    },
}) // End E011
