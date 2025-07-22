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

#include "version.h"

// Function initialization-table
Name(BUF0, Buffer()
{
    // enable UAJ input, by default enabled
    //0x0c, 0xfe, 0x11, 0x00, 0x00,   // 0x11FE0C = 0x0
    //0x0d, 0xfe, 0x11, 0x00, 0x00,
    //0x0e, 0xfe, 0x11, 0x00, 0x00,
    //0x0f, 0xfe, 0x11, 0x00, 0x00,

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
        Package(2) { "01fa-release-version", RELEASE_VERSION },
        Package(2) { "01fa-ssid-ex", 0x7 },
        Package(2) { "01fa-supported-jack-types-mask",
            COHEN_PHIFE_UAJ_UNKNOWN_EN | COHEN_PHIFE_UAJ_HEADPHONE_EN | COHEN_PHIFE_UAJ_HEADSET_EN |
            COHEN_PHIFE_UAJ_LINE_OUT_EN | COHEN_PHIFE_UAJ_LINE_IN_EN | COHEN_PHIFE_UAJ_MIC_EN },    // msft-ge-mode-terminaltype-list bitmap
        //
        // This property is used to select HWKWS FW.
        // Since the audio function performing the FDL is selected at random,
        // all audio functions involved with FDL must have it defined.
        Package(2) {"01fa-xu-features", (FEATURE_ENABLE_HWKWS | FEATURE_ENABLE_WT | FEATURE_ENABLE_KNCK | FEATURE_NO_FUN_STS |
                                         FEATURE_CS42L43_UAJ_NO_VOL_MUTE_C_COND | FEATURE_CS42L43_UAJ_NO_VOL_MUTE_R_COND)},
#ifdef GLOBAL_MUTE_LED_MIC_GPIO_NUM
        package(2) {"01fa-global-mute-led-mic-mute-gpio", GLOBAL_MUTE_LED_MIC_GPIO_NUM },
#endif
#ifdef GLOBAL_MUTE_LED_SPK_GPIO_NUM
        package(2) {"01fa-global-mute-led-spk-mute-gpio", GLOBAL_MUTE_LED_SPK_GPIO_NUM },
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
        Package(2) { "mipi-sdca-ge-managed-terminal-reference-number", 0x1},
#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
        Package(2) { "msft-ge-mode-terminaltype-list",
            Package()
            {
                // HP, HS, LineOut, LineIn, StereoMic
                0x3, 0x06C0,   // Mode 3 maps to Headphone
                0x4, 0x06D0,   // Mode 4 maps to Headset
                0x5, 0x0690,   // Mode 5 maps to LineOut stereo
                0x6, 0x0680,   // Mode 6 maps to LineIn stereo
                0x7, 0x06A0,   // Mode 7 maps to StereoMic
            }},
        Package (2) { "msft-ge-managed-list",
            Package()
            {
                0x13,   // Entity Id of Headphone terminal
                0x17,   // Entity Id of Headset Speaker terminal
                0x15,   // Entity Id of Lineout stereo terminal
                0x9,    // Entity Id of LineIn stereo terminal
                0x7,    // Entity Id of to Mic terminal
                0xB,    // Entity Id of Headset Mic terminal
            }},
#endif
        Package(2) { "mipi-sdca-control-list", CTL_GE_DETECTED_MODE | CTL_GE_SELECTED_MODE |
                                               CS42L43_GE35_CTL_LOAD_DET | CS42L43_GE35_CTL_ASP_OUTPUT},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "CE01"}, // Selected_Mode
        Package(2) { "mipi-sdca-control-0x2-subproperties", "CE02"}, // Detected_Mode
        Package(2) { "mipi-sdca-control-0x31-subproperties", "CE31"}, // Load Detection
        Package(2) { "mipi-sdca-control-0x32-subproperties", "CE32"}, // ASP Output
    },

    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-ge-selectedmode-controls-affected", "MD01"},
    }
}) // End E011

// Selected_Mode
Name(BMT2, Buffer() {
    0x02, 0x00,             // Range type 0x0002
    0x05, 0x00,             // NumRows = 5
    0x03, 0x00, 0x00, 0x00, // Selected Mode = 3
    0xC0, 0x06, 0x00, 0x00, // Terminal Type = 0x06C0, Headphone
    0x04, 0x00, 0x00, 0x00, // Selected Mode = 4
    0xD0, 0x06, 0x00, 0x00, // Terminal Type = 0x06D0, Headset
    0x05, 0x00, 0x00, 0x00, // Selected Mode = 5
    0x90, 0x06, 0x00, 0x00, // Terminal Type = 0x0690, LineOut stereo
    0x06, 0x00, 0x00, 0x00, // Selected Mode = 6
    0x80, 0x06, 0x00, 0x00, // Terminal Type = 0x0680, LineIn stereo
    0x07, 0x00, 0x00, 0x00, // Selected Mode = 7
    0xA0, 0x06, 0x00, 0x00, // Terminal Type = 0x06A0, Stereo Mic-on-Jack
}) // End BMT2

Name(MD01, Buffer()
{
    0x06, // Mode count
    0x00, // Mode, No Jack
        0x04, //Control count
            0x0D, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 35, Selector, cn, value (4-bytes)
            0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 43, Selector, cn, value (4-bytes)
            0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 44, Selector, cn, value (4-bytes)
            0x16, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 45, Selector, cn, value (4-bytes)
    0x03, // Mode, Headphone
        0x04, //Control count
            0x0D, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 35, Selector, cn, value (4-bytes)
            0x12, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, // SU 43, Selector, cn, value (4-bytes)
            0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 44, Selector, cn, value (4-bytes)
            0x16, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 45, Selector, cn, value (4-bytes)
    0x04, // Mode, Headset
        0x04, //Control count
            0x0D, 0x01, 0x00, 0x03, 0x00, 0x00, 0x00, // SU 35, Selector, cn, value (4-bytes)
            0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 43, Selector, cn, value (4-bytes)
            0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 44, Selector, cn, value (4-bytes)
            0x16, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, // SU 45, Selector, cn, value (4-bytes)
    0x05, // Mode, LineOut Stereo
        0x04, //Control count
            0x0D, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 35, Selector, cn, value (4-bytes)
            0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 43, Selector, cn, value (4-bytes)
            0x14, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, // SU 44, Selector, cn, value (4-bytes)
            0x16, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 45, Selector, cn, value (4-bytes)
    0x06, // Mode, LineIn Stereo
        0x04, //Control count
            0x0D, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, // SU 35, Selector, cn, value (4-bytes)
            0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 43, Selector, cn, value (4-bytes)
            0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 44, Selector, cn, value (4-bytes)
            0x16, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 45, Selector, cn, value (4-bytes)
    0x07, // Mode, Microphone
        0x04, //Control count
            0x0D, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, // SU 35, Selector, cn, value (4-bytes)
            0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 43, Selector, cn, value (4-bytes)
            0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 44, Selector, cn, value (4-bytes)
            0x16, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 45, Selector, cn, value (4-bytes)
}) // End MD01
