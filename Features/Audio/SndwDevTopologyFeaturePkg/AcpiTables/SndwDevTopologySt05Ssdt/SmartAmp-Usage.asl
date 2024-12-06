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

# define USAGE_IT_21
# define USAGE_IT_26
# define USAGE_OT_23
# define USAGE_OT_24
# define USAGE_OT_25
# define USAGE_IT_29

#else

# define USAGE_IT_21 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN21"}
# define USAGE_IT_26 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN26"}
# define USAGE_IT_29 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN29"}
# define USAGE_OT_23 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN23"}
# define USAGE_OT_24 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN24"}
# define USAGE_OT_25 Package(2) {"mipi-sdca-control-0x4-subproperties", "UN25"}

//  +--------------------------+
//  |     Usage for IT 26      |
//  | Ultrasound Render Stream |
//  +--------------------------+
Name(UN26, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x26},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB26"},
    }
})

// Used for IT26, OT24 and OT25 since no sample rate is currently specified.
// If this changes please check US25 and US24 .
Name(UB26, Buffer()
{
    0x07, 0x00,  // Range type 0x0007 (Septuples)
    0x01, 0x00,  // NumRows = 1
    // Usage Index 0 - Wildcard Sample Rate - Streaming terminals
    0x26, 0x00, 0x00, 0x00, // UsageNumber 0x26
    0x9A, 0x01, 0x00, 0x00, // CBN 410 Full band
    0x00, 0x00, 0x00, 0x00, // Wildcard (0x0) - any sample rate should use these samples rates (opt. - see Table 143 CBN Fidelity Levels used in DisCo Usage Map)
    0x18, 0x00, 0x00, 0x00, // 24-bit (opt. - see Table 143 CBN Fidelity Levels used in DisCo Usage Map)
    0x00, 0x00, 0x00, 0x00, // Full Scale (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Noise Floor (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Usage Tag
})

//  +-----------------------+
//  |   Usage for IT 21     |
//  | Speaker Render Stream |
//  +-----------------------+
Name(UN21, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, R/W
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package (2) {"mipi-sdca-control-default-value", 0x21},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "UB21"},
    }
})

Name(UB21, Buffer()
{
    0x07, 0x00,  // Range type 0x0007 (Septuples)
#ifdef JAMERSON_96K
    0x02, 0x00,  // NumRows = 2
#else
    0x01, 0x00,  // NumRows = 1
#endif
    // Usage Index 0 - 48000Hz Sample Rate - Streaming terminals
    0x21, 0x00, 0x00, 0x00, // UsageNumber 0x21
    0x9A, 0x01, 0x00, 0x00, // CBN 410 Full band
    0x80, 0xBB, 0x00, 0x00, // 48000
    0x18, 0x00, 0x00, 0x00, // 24-bit
    0x00, 0x00, 0x00, 0x00, // Full Scale (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Noise Floor (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Usage Tag
#ifdef JAMERSON_96K
    // Usage Index 1 - 96000Hz Sample Rate - Streaming terminals
    0x22, 0x00, 0x00, 0x00, // UsageNumber 0x22
    0xCC, 0x01, 0x00, 0x00, // CBN 460 HiFI Full band
    0x00, 0x77, 0x01, 0x00, // 96000
    0x18, 0x00, 0x00, 0x00, // 24-bit
    0x00, 0x00, 0x00, 0x00, // Full Scale (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Noise Floor (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Usage Tag
#endif
})

//  +----------------------+
//  |    Usage for OT 24   |
//  | Speaker Sense Stream |
//  +----------------------+
Name(UN24, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x26},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        // Reuse UB26 since it does not currently specify a sample rate.
        Package(2) {"mipi-sdca-control-range", "UB26"},
    }
})

//  +-------------------------+
//  |     Usage for OT 25     |
//  | Render Reference Stream |
//  +-------------------------+
Name(UN25, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x26},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        // Reuse UB26 since it does not currently specify a sample rate.
        Package(2) {"mipi-sdca-control-range", "UB26"},
    }
})

//  +---------------------------+
//  |      Usage for OT 23      |
//  | Primary Transducer Output |
//  +---------------------------+
Name(UN23, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
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

// Used for both US23 and US29 since the sample rate is not specified.
// If it was to be specified this may need to change.
Name(UB23, Buffer()
{
    0x07, 0x00,  // Range type 0x0007 (Septuples)
    0x01, 0x00,  // NumRows = 1
    0x23, 0x00, 0x00, 0x00, // UsageNumber 0x23
    0x9A, 0x01, 0x00, 0x00, // CBN 410 Full band
    0x00, 0x00, 0x00, 0x00, // None
    0x00, 0x00, 0x00, 0x00, // None
    0x00, 0x00, 0x00, 0x00, // Full Scale (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Noise Floor (dBSPL) (opt)
    0x00, 0x00, 0x00, 0x00, // Usage Tag
})

//  +-----------------------------+
//  |      Usage for IT 29        |
//  | Ultrasound Reference Stream |
//  +-----------------------------+
Name(UN29, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Usage: Class, DC
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x23},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        // Reuse UB23 since it does not currently specify a sample rate.
        Package(2) {"mipi-sdca-control-range", "UB23"},
    }
})

#endif //defined ENABLE_USAGE
