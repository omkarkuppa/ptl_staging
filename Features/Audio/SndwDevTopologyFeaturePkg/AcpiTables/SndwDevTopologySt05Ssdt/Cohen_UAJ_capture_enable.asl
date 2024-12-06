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
    // enable UAJ input, by default enabled
    //0x0c, 0xfe, 0x11, 0x00, 0x00,   // 0x11FE0C = 0x0
    //0x0d, 0xfe, 0x11, 0x00, 0x00,
    //0x0e, 0xfe, 0x11, 0x00, 0x00,
    //0x0f, 0xfe, 0x11, 0x00, 0x00,

#ifndef EXCLUDE_FUN_STS
    //
    // FDL config
    //
    0x00, 0xff, 0x11, 0x00, 0x01,   // 0x0011FF00 = 0x1 (FILE_SET_5)
    0x01, 0xff, 0x11, 0x00, 0x00,
    0x02, 0xff, 0x11, 0x00, 0x00,
    0x03, 0xff, 0x11, 0x00, 0x00,

    0x04, 0xff, 0x11, 0x00, 0x01,   // 0x0011FF04 = 0x1 (FILE_SET_6)
    0x05, 0xff, 0x11, 0x00, 0x00,
    0x06, 0xff, 0x11, 0x00, 0x00,
    0x07, 0xff, 0x11, 0x00, 0x00,

    0x0C, 0x40, 0x11, 0x00, COHEN_NEED_CONFIGS_VAL_0C,  // 0x0011400C = 800000xx (NEED_CONFIGS)
    0x0D, 0x40, 0x11, 0x00, 0x00,
    0x0E, 0x40, 0x11, 0x00, 0x00,
    0x0F, 0x40, 0x11, 0x00, 0x80,

    0x04, 0x40, 0x11, 0x00, 0x00,   // 0x00114004 = 00014800 (PATCH_START)
    0x05, 0x40, 0x11, 0x00, 0x48,
    0x06, 0x40, 0x11, 0x00, 0x01,
    0x07, 0x40, 0x11, 0x00, 0x00,

    0xF8, 0xFB, 0x11, 0x00, 0x00,   // 0x0011FBF8 = 00014800(FW_PATCH)
    0xF9, 0xFB, 0x11, 0x00, 0x48,
    0xFA, 0xFB, 0x11, 0x00, 0x01,
    0xFB, 0xFB, 0x11, 0x00, 0x00,
    //
    // FDL config ends here
    //
#endif
}) // End BUF0


Name(EXT0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
#ifdef COHEN_BRIDGE
# ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
        // NOTE: 01fa-spk-id-val must ALWAYS be the first package inside the
        // inner package of EXT0. It will be updated at ACPI initialization time
        // by the _INI method in the SDCA function ACPI driver node for this
        // device.
        Package(2) {"01fa-spk-id-val", 0}, // value to be set by _INI function
# endif
        #include <Sidecar_Cohen_Tweeter_Jamerson_Woofer.asl>
#endif
        Package(2) { "mipi-sdca-function-expansion-subsystem-id", 0 },  // MIPI required, but not used by MSFT
        Package(2) { "01fa-chip-id", 0x4243 },
        Package(2) { "01fa-ssid-ex", 0x7 },
        Package(2) { "01fa-supported-jack-types-mask",
            COHEN_UAJ_UNKNOWN_EN | COHEN_UAJ_HEADPHONE_EN | COHEN_UAJ_HEADSET_EN |
            COHEN_UAJ_LINE_OUT_EN | COHEN_UAJ_LINE_IN_EN | COHEN_UAJ_MIC_EN },    // msft-ge-mode-terminaltype-list bitmap
        //
        // This property is used to select HWKWS FW.
        // Since the audio function performing the FDL is selected at random,
        // all audio functions involved with FDL must have it defined.
        Package(2) {"01fa-xu-features", (FEATURE_ENABLE_HWKWS | FEATURE_ENABLE_WT | FEATURE_ENABLE_KNCK | FEATURE_NO_FUN_STS |
                                         FEATURE_CS42L43_UAJ_NO_VOL_MUTE_C_COND | FEATURE_CS42L43_UAJ_NO_VOL_MUTE_R_COND)},
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
                0x4, 0x06D0,   // Mode 4 maps to Headset
                0x5, 0x0690,   // Mode 5 maps to LineOut stereo
                0x6, 0x0680,   // Mode 6 maps to LineIn stereo
                0x7, 0x06A0,   // Mode 7 maps to StereoMic
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
        Package(2) { "mipi-sdca-control-list", CTL_GE_DETECTED_MODE | CTL_GE_SELECTED_MODE |
                                               CS42L43_GE35_CTL_LOAD_DET | CS42L43_GE35_CTL_ASP_OUTPUT},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "CE01"}, // Selected_Mode
        Package(2) { "mipi-sdca-control-0x2-subproperties", "CE02"}, // Detected_Mode
        Package(2) { "mipi-sdca-control-0x2-subproperties", "CE31"}, // Load Detection
        Package(2) { "mipi-sdca-control-0x2-subproperties", "CE32"}, // ASP Output
    },
}) // End E011
