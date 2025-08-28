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

#include "version.h"

// Function initialization-table
Name(BUF0, Buffer()
{
    // disable UAJ input, by default enabled
    0x0c, 0xfc, 0x11, 0x00, 0x04,   // 0x11FCC0 = 0x4 (FW_MM_CONTROL_SELECTION::MM_CTRL_JCK_INP_DIS == 4)
    0x0d, 0xfc, 0x11, 0x00, 0x00,
    0x0e, 0xfc, 0x11, 0x00, 0x00,
    0x0f, 0xfc, 0x11, 0x00, 0x00,

    //
    // FDL config
    //
    0xB8, 0xFD, 0x11, 0x00, 0x01,  // 0x0011FDB8 = 0x1 (FILE_SET_5)
    0xB9, 0xFD, 0x11, 0x00, 0x00,
    0xBA, 0xFD, 0x11, 0x00, 0x00,
    0xBB, 0xFD, 0x11, 0x00, 0x00,

    0x82, 0xFA, 0x11, 0x00, 0x01,  // 0x0011FA82 = 00000001 (NEED_CONFIGS)
    0x83, 0xFA, 0x11, 0x00, 0x00,
    0x84, 0xFA, 0x11, 0x00, 0x00,
    0x85, 0xFA, 0x11, 0x00, 0x00,

    0x34, 0xFA, 0x11, 0x00, 0x00,  // 0x0011FA34 = 00014800 (FW_IMAGE_ADDRESS)
    0x35, 0xFA, 0x11, 0x00, 0x48,
    0x36, 0xFA, 0x11, 0x00, 0x01,
    0x37, 0xFA, 0x11, 0x00, 0x00,

    //
    // FDL config ends here
    //
}) // End BUF0


Name(EXT0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-function-expansion-subsystem-id", 0 },  // MIPI required, but not used by MSFT
        Package(2) { "01fa-chip-id", 0x4245 },
        Package(2) { "01fa-release-version", RELEASE_VERSION },
        Package(2) { "01fa-ssid-ex", 0x7 },
        Package(2) { "01fa-supported-jack-types-mask",
            COHEN_PHIFE_UAJ_UNKNOWN_EN | COHEN_PHIFE_UAJ_HEADPHONE_EN | COHEN_PHIFE_UAJ_LINE_OUT_EN },    // msft-ge-mode-terminaltype-list bitmap
        Package(2) {"01fa-xu-features", (FEATURE_ENABLE_WT | FEATURE_ENABLE_KNCK | FEATURE_NO_FUN_STS |
                                         FEATURE_CS42L45_UAJ_NO_VOL_MUTE_C_COND | FEATURE_CS42L45_UAJ_NO_VOL_MUTE_R_COND)},
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
        Package(2) { "mipi-sdca-ge-managed-terminal-reference-number", 0x01},
#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
        Package (2) {"msft-ge-mode-terminaltype-list",
            Package()
            {
                // HP, HS, LineOut, LineIn, StereoMic
                0x3, 0x06C0,   // Mode 3 maps to Headphone
                //0x4, 0x06D0,   // Mode 4 maps to Headset
                0x5, 0x0690,   // Mode 5 maps to LineOut stereo
                //0x6, 0x0680,   // Mode 6 maps to LineIn stereo
                //0x7, 0x06A0,   // Mode 7 maps to StereoMic
            }
        },
        Package (2) {"msft-ge-managed-list",
            Package()
            {
                0x13,   // Entity Id of Headphone terminal
                0x17,   // Entity Id of Headset Speaker terminal
                0x15,   // Entity Id of Lineout stereo terminal
                0x9,    // Entity Id of LineIn stereo terminal
                0x7,    // Entity Id of to Mic terminal
                0xB,    // Entity Id of Headset Mic terminal
            }
        },
#endif
        Package(2) { "mipi-sdca-control-list", CTL_GE_DETECTED_MODE | CTL_GE_SELECTED_MODE |
                                               CS42L45_GE35_CTL_LOAD_DET | CS42L45_GE35_CTL_ASP_OUTPUT},
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
    0x02, 0x00,             // NumRows = 2
    0x03, 0x00, 0x00, 0x00, // Selected Mode = 3
    0xC0, 0x06, 0x00, 0x00, // Terminal Type = 0x06C0, Headphone
    //0x04, 0x00, 0x00, 0x00, // Selected Mode = 4
    //0xD0, 0x06, 0x00, 0x00, // Terminal Type = 0x06D0, Headset
    0x05, 0x00, 0x00, 0x00, // Selected Mode = 5
    0x90, 0x06, 0x00, 0x00, // Terminal Type = 0x0690, LineOut stereo
    //0x06, 0x00, 0x00, 0x00, // Selected Mode = 6
    //0x80, 0x06, 0x00, 0x00, // Terminal Type = 0x0680, LineIn stereo
    //0x07, 0x00, 0x00, 0x00, // Selected Mode = 7
    //0xA0, 0x06, 0x00, 0x00, // Terminal Type = 0x06A0, Stereo Mic-on-Jack
}) // End BMT2

Name(MD01, Buffer()
{
    0x03, // Mode count
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
    0x05, // Mode, LineOut Stereo
        0x04, //Control count
            0x0D, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 35, Selector, cn, value (4-bytes)
            0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 43, Selector, cn, value (4-bytes)
            0x14, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, // SU 44, Selector, cn, value (4-bytes)
            0x16, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // SU 45, Selector, cn, value (4-bytes)
}) // End MD01
