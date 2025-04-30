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

#ifndef ENABLE_USAGE
# define USAGE_IT_11
# define USAGE_OT_13
# define USAGE_OT_113
# define USAGE_OT_14

#else

# define USAGE_IT_11  Package(2) {"mipi-sdca-control-0x4-subproperties", "UN11"}
# define USAGE_OT_13  Package(2) {"mipi-sdca-control-0x4-subproperties", "UN13"}
# define USAGE_OT_113 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN23"}
# define USAGE_OT_14  Package(2) {"mipi-sdca-control-0x4-subproperties", "UN14"}

// +------------------------------------+
// |                IT 11               |
// |     Mic Array Transducer Input     |
// +------------------------------------+
Name(UN11, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Class, DC, Usage
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x11},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB11"},
    }
})

// Usage buffer for transducer terminals
Name(UB11, Buffer()
{
    0x07, 0x00,             // Range type 0x0007 (Septuples)
    0x01, 0x00,             // NumRows = 0x1
    0x11, 0x00, 0x00, 0x00, // UsageNumber 11
    0x9A, 0x01, 0x00, 0x00, // CBN 410
    0x00, 0x00, 0x00, 0x00, // Wildcard (0x0) - any sample rate should use these samples rates (opt. - see Table 143 CBN Fidelity Levels used in DisCo Usage Map)
    0x00, 0x00, 0x00, 0x00, // Transducer terminals are not allowed to have a sample rate or size in the Microsoft driver.
    0x46, 0x00, 0x00, 0x00, // Full Scale = 70 dBSPL
    0x14, 0x00, 0x00, 0x00, // Noise Floor = 20 dBSPL
    0x00, 0x00, 0x00, 0x00, // Usage Tag
})

// +------------------------------------+
// |                OT 113              |
// |     Capture Direct PCM Stream      |
// +------------------------------------+
Name(UN23, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Class, DC, Usage
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x23},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB23"},
    }
})

// OT 13 usage buffer
Name(UB23, Buffer()
{
    0x07, 0x00,             // Range type 0x0007 (Septuples)
    0x01, 0x00,             // NumRows = 0x1
    0x23, 0x00, 0x00, 0x00, // UsageNumber 0x23
    0x9A, 0x01, 0x00, 0x00, // CBN 410 (0x19A): Full band
    0x80, 0xBB, 0x00, 0x00, // Sample Rate = 48kHz (0xBB80)
#ifdef DMIC_16BIT
    0x10, 0x00, 0x00, 0x00, // Sample Width = 16 or 24 bit
#else
    0x18, 0x00, 0x00, 0x00,
#endif
    0x00, 0x00, 0x00, 0x00, // Full Scale = 0 dBSPL
    0x00, 0x00, 0x00, 0x00, // Noise Floor = 0 dBSPL
    0x00, 0x00, 0x00, 0x00  // Usage Tag
})

// +------------------------------------+
// |                OT 13               |
// |      Secondary PCM Mic Stream      |
// +------------------------------------+
Name(UN13, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Class, DC, Usage
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x13},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB13"},
    }
}) // End UN13

// OT 13 usage buffer
Name(UB13, Buffer()
{
    0x07, 0x00,             // Range type 0x0007 (Septuples)
    0x01, 0x00,             // NumRows = 1
    0x13, 0x00, 0x00, 0x00, // UsageNumber  0x13
    0xD2, 0x00, 0x00, 0x00, // CBN = 210 (0xd2): Voice
    0x80, 0x3e, 0x00, 0x00, // Sample Rate = 16kHz (0x3e80)
#ifdef DMIC_16BIT
    0x10, 0x00, 0x00, 0x00, // Sample Width = 16 or 24 bit
#else
    0x18, 0x00, 0x00, 0x00,
#endif
    0x00, 0x00, 0x00, 0x00, // Full Scale = 0 dBSPL
    0x00, 0x00, 0x00, 0x00, // Noise Floor = 0 dBSPL
    0x00, 0x00, 0x00, 0x00  // Usage Tag
})


// +------------------------------------+
// |                OT 14               |
// |          UltraSound Capture        |
// +------------------------------------+
// See Documentation AN0580R1
Name(UN14, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Class, DC, Usage
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x14},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB14"},
    }
}) // End UN14

// OT 14 usage buffer
Name(UB14, Buffer()
{
    0x07, 0x00,             // Range type 0x0007 (Septuples)
    0x01, 0x00,             // NumRows = 1
    0x14, 0x00, 0x00, 0x00, // UsageNumber = 0x14
    0x9A, 0x01, 0x00, 0x00, // CBN = 410 (0x19A): Full band
    0x80, 0xBB, 0x00, 0x00, // Sample Rate = 48kHz (0xBB80)
#ifdef DMIC_16BIT
    0x10, 0x00, 0x00, 0x00, // Sample Width = 16 bit (0x10)
#else
    0x18, 0x00, 0x00, 0x00, // Sample Width = 24 bit (0x18)
#endif
    0x00, 0x00, 0x00, 0x00, // Full Scale (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Noise Floor (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00  // Usage Tag
}) // End UB14

#endif // defined ENABLE_USAGE
