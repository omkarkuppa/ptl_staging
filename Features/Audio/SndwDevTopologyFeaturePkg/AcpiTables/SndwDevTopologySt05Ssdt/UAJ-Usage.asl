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

#ifndef ENABLE_USAGE

# define USAGE_IT_31
# define USAGE_IT_32
# define USAGE_IT_33
# define USAGE_IT_41
# define USAGE_OT_36
# define USAGE_OT_43
# define USAGE_OT_44
# define USAGE_OT_45

#else

# define USAGE_IT_31 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN31"}
# define USAGE_IT_32 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN32"}
# define USAGE_IT_33 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN32"}
# define USAGE_IT_41 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN41"}
# define USAGE_OT_36 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN36"}
# define USAGE_OT_43 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN43"}
# define USAGE_OT_44 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN44"}
# define USAGE_OT_45 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN45"}

// +------------------------------------+
// |                IT_41               |
// |       Generic Sink Data Port       |
// +------------------------------------+
Name(UN41, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {    // Class, DC, Usage
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x41},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB41"},
    }
})

// The usage numbers in the BUSx blocks below are all unique, but they do not need to be.
// IT_41 Usage buffer
Name(UB41, Buffer()
{
    0x07, 0x00,  // Range type 0x0007
    0x01, 0x00,  // NumRows = 0x1
    0x41, 0x00, 0x00, 0x00, // UsageNumber 0x41
    0x9A, 0x01, 0x00, 0x00, // CBN 410
    0x00, 0x00, 0x00, 0x00, // Wildcard (0x0) - any sample rate should use these samples rates (opt. - see Table 143 CBN Fidelity Levels used in DisCo Usage Map)
    0x18, 0x00, 0x00, 0x00, // 24-bit (opt. - see Table 143 CBN Fidelity Levels used in DisCo Usage Map)
    0x00, 0x00, 0x00, 0x00, // Full Scale (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Noise Floor (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Usage Tag
})

Name(CUS1, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUS1"},
    }
})

// Ultrasound usage
Name(BUS1, Buffer()
{
    0x07, 0x00,  // Range type 0x0007 (Septuples)
    0x01, 0x00,  // NumRows = 1
    // Usage Index 1 - Reference stream with sample rate of 48K
    0x01, 0x00, 0x00, 0x00, // UsageNumber 1
    0xBC, 0x02, 0x00, 0x00, // CBN 700 Full band
    0x80, 0xBB, 0x00, 0x00, // 48K rates (opt. - see Table 143 CBN Fidelity Levels used in DisCo Usage Map)
    0x18, 0x00, 0x00, 0x00, // 24-bit (opt. - see Table 143 CBN Fidelity Levels used in DisCo Usage Map)
    0x00, 0x00, 0x00, 0x00, // Full Scale (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Noise Floor (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Usage Tag
}) // End BUS1

// +------------------------------------+
// |                OT_43               |
// |       Headphone Output Jack        |
// +------------------------------------+
Name(UN43, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x43},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB43"},
    }
})

// +----------------+
// |     IT_32      |
// | Line In Stereo |
// +----------------+
Name(UN32, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x43},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB43"},
    }
})

// +-----------------------------+
// |          IT_33              |
// | Headeset (Mono) Mic On Jack |
// +-----------------------------+
Name(UN33, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x43},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB43"},
    }
})


// Transducer terminals which cannot have a sample rate.
Name(UB43, Buffer()
{
    0x07, 0x00,  // Range type 0x0007 (Septuples)
    0x01, 0x00,  // NumRows = 1
    // Usage Number 0x43 - Transducer Terminals (not allowed to have sample rate and size by Microsoft)
    0x43, 0x00, 0x00, 0x00, // UsageNumber 0x43
    0x9A, 0x01, 0x00, 0x00, // CBN 410 Full band
    0x00, 0x00, 0x00, 0x00, // None
    0x00, 0x00, 0x00, 0x00, // None
    0x00, 0x00, 0x00, 0x00, // Full Scale (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Noise Floor (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Usage Tag
})

// +-----------------+
// |      OT_44      |
// | Line Out Stereo |
// +-----------------+
Name(UN44, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        // Reuse OT_43
        Package (2) {"mipi-sdca-control-dc-value", 0x43},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB43"},
    }
})

// +------------------------+
// |        OT_45           |
// | Headset Output On Jack |
// +------------------------+
Name(UN45, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        // Reuse OT_43
        Package (2) {"mipi-sdca-control-dc-value", 0x43},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB43"},
    }
})

// +--------------------+
// |        IT_31       |
// | Stereo Mic Om Jack |
// +--------------------+
Name(UN31, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        // Reuse OT_43
        Package (2) {"mipi-sdca-control-dc-value", 0x43},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB43"},
    }
})

// +-----------------------+
// |        OT_36          |
// | Generic Sink Dataport |
// +-----------------------+
Name(UN36, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x36},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB36"},
    }
})


// Outputs limited to 48K 24 bits
Name(UB36, Buffer()
{
    0x07, 0x00,  // Range type 0x0007 (Septuples)
    0x01, 0x00,  // NumRows = 1
    0x36, 0x00, 0x00, 0x00, // UsageNumber 0x36
    0x9A, 0x01, 0x00, 0x00, // CBN 410 Full band
    0x00, 0x00, 0x00, 0x00, // Wildcard - support all rates in clock source entity
//!!! ATR
#ifdef DMIC_16BIT
    0x10, 0x00, 0x00, 0x00, // Sample Width = 16 or 24 bit
#else
    0x18, 0x00, 0x00, 0x00,
#endif
    0x00, 0x00, 0x00, 0x00, // Full Scale (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Noise Floor (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Usage Tag
})

#endif // defined ENABLE_USAGE
