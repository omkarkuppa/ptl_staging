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

// Check by Allen, Date: 2023/02/15
Name(_DSD, Package()
{
  // Function Descriptor
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package(2) {"mipi-sdw-sw-interface-revision", 0x00020001}, // v2.1
    Package(2) {"mipi-sdw-sdca-interface-revision", 0x0801}, // v0.8r01
    //Package(2) {"mipi-sdca-function-topology-features", 0x0080000000000000}, // Security/Privacy(bit-55)
    Package(2) {"mipi-sdca-function-topology-features", 0x0000000000000000}, // No option
    //Package(2) {"mipi-sdca-control-list", Package() {0x4, 0x5, 0x6, 0x7, 0x8} }, // Function level controls
    Package(2) {"mipi-sdca-control-list",  0xF000000301F2 }, // Bitmap: 0x1, 0x4, 0x5, 0x6, 0x7, 0x8, 0x10, 0x11, 0x2C, 0x2D, 0x2E, 0x2F - Function level controls
                //Package(2) {"mipi-sdca-control-list", 0x01F0}, // Function level controls
    //Package(2) {"mipi-sdca-entity-id-list",Package() {0x13, 0x14, 0x12, 0x18, 0x11, 0x26, 0x15, 0x1C, 0x16, 0x1D, 0x1E, 0x20, 0x1F, 0x17, 0x6a, 0x6B, 0x1a, 0x19, 0x1b} }, // List of entities in this function (No Security/Privacy)
    Package(2) {"mipi-sdca-entity-id-list",Package() {0x13, 0x14, 0x12, 0x18, 0x11, 0x26, 0x15, 0x1C, 0x16, 0x1D, 0x1E, 0x20, 0x1F, 0x6a, 0x6B, 0x1a, 0x19, 0x1b} }, // List of entities in this function (No Security/Privacy)
    // Cluster Library
    Package(2) {"mipi-sdca-cluster-id-list", Package() {0x11, 0x12, 0x21, 0x22, 0x31, 0x32, 0x41, 0x42} },
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package(2) {"mipi-sdca-control-0x4-subproperties", "C004"},
    Package(2) {"mipi-sdca-control-0x5-subproperties", "C005"},
    Package(2) {"mipi-sdca-control-0x6-subproperties", "C006"},
    Package(2) {"mipi-sdca-control-0x7-subproperties", "C007"},
    Package(2) {"mipi-sdca-control-0x8-subproperties", "C008"},
          Package(2) {"mipi-sdca-control-0x1-subproperties", "C042"},
    Package(2) {"mipi-sdca-control-0x10-subproperties", "C043"},//Function_Status, Class, RW1C
    Package(2) {"mipi-sdca-control-0x11-subproperties", "C044"},//Function_Action, Class, RW1S
    Package(2) {"mipi-sdca-control-0x2C-subproperties", "C02C"},//Device_Manufacturer_ID
    Package(2) {"mipi-sdca-control-0x2D-subproperties", "C02D"},//Device_Part_ID
    Package(2) {"mipi-sdca-control-0x2E-subproperties", "C02E"},//Device_Version
                Package(2) {"mipi-sdca-control-0x2F-subproperties", "C02F"},//Device_SDCA_Version
                Package(2) {"mipi-sdca-entity-id-0x13-subproperties", "E013"},
                Package(2) {"mipi-sdca-entity-id-0x14-subproperties", "E014"},
                Package(2) {"mipi-sdca-entity-id-0x12-subproperties", "E012"},
                Package(2) {"mipi-sdca-entity-id-0x18-subproperties", "E018"},
                Package(2) {"mipi-sdca-entity-id-0x11-subproperties", "E011"},
                Package(2) {"mipi-sdca-entity-id-0x26-subproperties", "E026"},
                Package(2) {"mipi-sdca-entity-id-0x15-subproperties", "E015"},
                Package(2) {"mipi-sdca-entity-id-0x1C-subproperties", "E01C"},
                //Package(2) {"mipi-sdca-entity-id-0x53-subproperties", "E053"},
                Package(2) {"mipi-sdca-entity-id-0x16-subproperties", "E016"},
                Package(2) {"mipi-sdca-entity-id-0x1D-subproperties", "E01D"},
                Package(2) {"mipi-sdca-entity-id-0x1E-subproperties", "E01E"},
                Package(2) {"mipi-sdca-entity-id-0x20-subproperties", "E020"},
                Package(2) {"mipi-sdca-entity-id-0x1F-subproperties", "E01F"},
                //Package(2) {"mipi-sdca-entity-id-0x17-subproperties", "E017"},
                Package(2) {"mipi-sdca-entity-id-0x6a-subproperties", "E06a"},
    Package(2) {"mipi-sdca-entity-id-0x6B-subproperties", "E06B"},
                Package(2) {"mipi-sdca-entity-id-0x1a-subproperties", "E01a"},
                Package(2) {"mipi-sdca-entity-id-0x19-subproperties", "E019"},
                Package(2) {"mipi-sdca-entity-id-0x1b-subproperties", "E01b"},
    Package(2) {"mipi-sdca-function-expansion-subproperties", "EXT0"},
    // Cluster ID subproperties
    Package(2) {"mipi-sdca-cluster-id-0x11-subproperties", "CL11"},
    Package(2) {"mipi-sdca-cluster-id-0x12-subproperties", "CL12"},
    Package(2) {"mipi-sdca-cluster-id-0x21-subproperties", "CL21"},
    Package(2) {"mipi-sdca-cluster-id-0x22-subproperties", "CL22"},
    Package(2) {"mipi-sdca-cluster-id-0x31-subproperties", "CL31"},
    Package(2) {"mipi-sdca-cluster-id-0x32-subproperties", "CL32"},
    Package(2) {"mipi-sdca-cluster-id-0x41-subproperties", "CL41"},
    Package(2) {"mipi-sdca-cluster-id-0x42-subproperties", "CL42"},
  },

  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package (2) {"mipi-sdca-function-initialization-table", "BUF0"},
  }
}) //End _DSD

Name(BUF0, Buffer()
{
  0x06, 0x20, 0x10, 0x06, 0x80, // 0x06102006, 0x80
  0x06, 0x00, 0x10, 0x06, 0x10, // 0x06100006, 0x10

  //0x00, 0x18, 0x98, 0x40, 0x01, // 0x40981800, 0x01

  0x1E, 0x20, 0x90, 0x05, 0x32, // 0x0590201E, 0x32
  0x1E, 0x00, 0x90, 0x05, 0x20, // 0x0590001E, 0x20

  0x01, 0x32, 0x00, 0x00, 0x23, // 0x00003201, 0x23

  0x58, 0x2F, 0x00, 0x00, 0x05, // 0x00002F58, 0x05

  0x59, 0x2F, 0x00, 0x00, 0x05, // 0x00002F59, 0x05

  0xA6, 0x20, 0x10, 0x06, 0x15, // 0x061020A6, 0x15
  0xA6, 0x00, 0x10, 0x06, 0x26, // 0x061000A6, 0x26

  0xB1, 0x20, 0x10, 0x06, 0x03, // 0x061020B1, 0x03
  0xB1, 0x00, 0x10, 0x06, 0x04, // 0x061000B1, 0x04

  0xA2, 0x20, 0x10, 0x06, 0x1F, // 0x061020A2, 0x1F
  0xA2, 0x00, 0x10, 0x06, 0x1E, // 0x061000A2, 0x1E

  0xB0, 0x20, 0x10, 0x06, 0x03, // 0x061020B0, 0x03
  0xB0, 0x00, 0x10, 0x06, 0x04, // 0x061000B0, 0x04

  0x00, 0x13, 0x98, 0x40, 0x01, // 0x40981300, 0x01

}) //End AF02.BUF0

        Name(C042, Package()
  {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
      //Commit_Group_Mask, Class, RW
      Package(2) {"mipi-sdca-control-access-layer", 0x4},
      Package(2) {"mipi-sdca-control-access-mode", 0},
    }
  }) //End AF04.C042

Name(C043, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {
    //Function_Status, Class, RW1C
    Package(2) {"mipi-sdca-control-access-layer", 0x4},
    Package(2) {"mipi-sdca-control-access-mode", 2},
    Package(2) {"mipi-sdca-control-interrupt-position", 19}, //IntStat_SDCA_19
  }
}) //End AF04.C043

Name(C044, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {
    //Function_Action, Class, RW1S
    Package(2) {"mipi-sdca-control-access-layer", 0x4},
    Package(2) {"mipi-sdca-control-access-mode", 4},
  }
}) //End AF04.C044

Name(C02C, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {
    //Device_Manufacturer_ID = 0x025d, Class, DC
    Package(2) {"mipi-sdca-control-access-layer", 0x4},
    Package(2) {"mipi-sdca-control-access-mode", 5},
    Package(2) {"mipi-sdca-control-dc-value", 0x025D},
  }
}) //End AF04.C02C

Name(C02D, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {
    //Device_Part_ID = 0x712, Class, DC
    Package(2) {"mipi-sdca-control-access-layer", 0x4},
    Package(2) {"mipi-sdca-control-access-mode", 5},
    Package(2) {"mipi-sdca-control-dc-value", 0x0712},
  }
}) //End AF04.C02D

Name(C02E, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {
    //Device_Version, Class, DC
    Package(2) {"mipi-sdca-control-access-layer", 0x4},
    Package(2) {"mipi-sdca-control-access-mode", 5},
    Package(2) {"mipi-sdca-control-dc-value", 0x1},
  }
}) //End AF04.C02E

Name(C02F, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {
    //Device_SDCA_Version, Class, DC
    Package(2) {"mipi-sdca-control-access-layer", 0x4},
    Package(2) {"mipi-sdca-control-access-mode", 5},
    Package(2) {"mipi-sdca-control-dc-value", 0x08},
  }
}) //End AF04.C02F

Name(C004, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Function_SDCA_Version = 0.8, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0x8},
  }
}) //End C004

Name(C005, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Function_Topology = 0x3 Smart-Mic, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0x3},
  }
}) //End C005

Name(C006, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Function_ManufacturerId = 0x025d, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0x025d},
  }
}) //End C006

Name(C007, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Function_ID = 0x0722 (Device specific), Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0x0712},
  }
}) //End C007

Name(C008, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Function_Version = 0x1, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0x1},
  }
}) //End C008


Name(E013, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x0b},
    Package (2) {"mipi-sdca-entity-label", "CS 11"},
    Package (2) {"mipi-sdca-cs-type", 0},
    //Package (2) {"mipi-sdca-control-list", Package() {0x2} }, // clock_valid
    Package (2) {"mipi-sdca-control-list", 0x04}, // Bitmap: 0x2 - clock_valid
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {

    Package (2) {"mipi-sdca-control-0x2-subproperties", "C132"},
  }
}) //End E013

Name(C132, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Clock_Valid, Class, RO
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 3},
    Package (2) {"mipi-sdca-control-interrupt-position", 18},
  }
}) //End C132

Name(E012, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x0b},
    Package (2) {"mipi-sdca-entity-label", "CS 18"},
    Package (2) {"mipi-sdca-cs-type", 0},
    //Package (2) {"mipi-sdca-control-list", Package() {0x2} }, // clock_valid
    Package (2) {"mipi-sdca-control-list", 0x04}, // Bitmap: 0x2 - clock_valid
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {

    Package (2) {"mipi-sdca-control-0x2-subproperties", "C132"},
  }
}) //End E013

Name(C122, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Clock_Valid, Class, RO
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 3},
    Package (2) {"mipi-sdca-control-interrupt-position", 19},
  }
}) //End C122



Name(E014, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x0c},
    Package (2) {"mipi-sdca-entity-label", "CX 11"},
    Package (2) {"mipi-sdca-cs-type", 0},
    Package (2) {"mipi-sdca-control-list", 0x02}, // Bitmap: 0x1 - Clock_Select
    Package (2) {"mipi-sdca-input-pin-list", 0x6}, // Input Pin 1&2 connected
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E013"}, // Input Pin 1 connected to CS11
    Package (2) {"mipi-sdca-input-pin-2", "E012"}, // Input Pin 2 connected to CS18
    Package (2) {"mipi-sdca-control-0x1-subproperties", "C141"},

  }
}) //End E013

Name(C141, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Clock_Select, Application, RW
    Package (2) {"mipi-sdca-control-access-layer", 0x2}, // layer is application
    Package (2) {"mipi-sdca-control-access-mode", 0},
    Package (2) {"mipi-sdca-control-cn-list", 0x1E },
  }
}) //End C141


Name(E011, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x11},
    Package (2) {"mipi-sdca-entity-label", "PDE 11"},
    Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x13,0x26} },
    Package (2) {"mipi-sdca-powerdomain-transition-typical-delay",
      Package()
      {
        0x3, 0x0, 192000,
        0x0, 0x3, 192000,
        0x2, 0x0, 0,
        0x0, 0x2, 0,
        0x1, 0x0, 0,
        0x0, 0x1, 0,
        0x0, 0x0, 0,
      }},
    Package (2) {"mipi-sdca-powerdomain-transition-max-delay",
      Package()
      {
        0x3, 0x0, 300000,
        0x0, 0x3, 300000,
        0x2, 0x0, 0,
        0x0, 0x2, 0,
        0x1, 0x0, 0,
        0x0, 0x1, 0,
        0x0, 0x0, 0,
      }},
    //Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x10} }, // Requested_PS, Actual_PS
    Package (2) {"mipi-sdca-control-list", 0x00010002}, // Bitmap: 0x1 - Requested_PS, 0x10 - Actual_PS
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-control-0x1-subproperties", "RP11"},
    Package (2) {"mipi-sdca-control-0x10-subproperties", "AP11"},
  }
}) //End E011

Name(RP11, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Requested_PS, Class, R/W
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 0},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package ()
  {
    Package (2) {"mipi-sdca-control-range", "PDER"},
  }
}) //End RP11

Name(PDER, Buffer()
{
  0x01, 0x00, // Range type 0x0001
  0x02, 0x00, // Count of ranges = 0x2
  0x00, 0x00, 0x00, 0x00, // PS0
  0x03, 0x00, 0x00, 0x00, // PS3
}) //End PDER

Name(AP11, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Actual_PS, Class, RO
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 3},
  }
}) //End AP11


Name(E018, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x11},
    Package (2) {"mipi-sdca-entity-label", "PDE 18"},
    Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x12} },
    Package (2) {"mipi-sdca-powerdomain-transition-typical-delay",
      Package()
      {
        0x3, 0x0, 192000,
        0x0, 0x3, 192000,
        0x2, 0x0, 0,
        0x0, 0x2, 0,
        0x1, 0x0, 0,
        0x0, 0x1, 0,
        0x0, 0x0, 0,
      }},
    Package (2) {"mipi-sdca-powerdomain-transition-max-delay",
      Package()
      {
        0x3, 0x0, 300000,
        0x0, 0x3, 300000,
        0x2, 0x0, 0,
        0x0, 0x2, 0,
        0x1, 0x0, 0,
        0x0, 0x1, 0,
        0x0, 0x0, 0,
      }},
    //Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x10} }, // Requested_PS, Actual_PS
    Package (2) {"mipi-sdca-control-list", 0x00010002}, // Bitmap: 0x1 - Requested_PS, 0x10 - Actual_PS
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-control-0x1-subproperties", "RP18"},
    Package (2) {"mipi-sdca-control-0x10-subproperties", "AP18"},
  }
}) //End E018

Name(RP18, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Requested_PS, Class, R/W
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 0},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package ()
  {
    Package (2) {"mipi-sdca-control-range", "PDER"},
  }
}) //End RP18


Name(AP18, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Actual_PS, Class, RO
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 3},
  }
}) //End AP18


Name(E026, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x02},
    Package (2) {"mipi-sdca-entity-label", "IT 11"},
    Package (2) {"mipi-sdca-terminal-type", 0x0205}, // Mic array transducer input
    //Package (2) {"mipi-sdca-control-list", Package() {0x4, 0x10} }, // Usage, ClusterIndex
    Package (2) {"mipi-sdca-control-list", 0x00010010}, // Bitmap: 0x4 - Usage, 0x10 - ClusterIndex
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-terminal-clock-connection", "E014"},
    Package (2) {"mipi-sdca-control-0x4-subproperties", "C264"},
    Package (2) {"mipi-sdca-control-0x10-subproperties", "C265"},
  },

    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
  {
        Package (2) {"mipi-sdca-microphone-array-geometry", "MGEO"},
        Package (2) {"mipi-sdca-microphone-array-snr", "MSNR"},
        Package (2) {"mipi-sdca-microphone-array-sensitivity", "MSNS"},
  }
}) //End E026

Name(MGEO, Buffer()
{
  0x00, 0x01,       // Version
  0x00, 0x00,       // Mic Array Type, KSMICARRAY_MICARRAYTYPE_LINEAR=0
  0xEA, 0xDD,       // VerticalAngleBegin, 0xDDEA = -8726*180/31416 = -50 degree
  0x16, 0x22,       // VerticalAngleEnd, 0x2216 = 8726*180/31416 = 50 degree
  0x19, 0xD7,       // HorizontalAngleBegin, 0xD719 = -10471*180/31416 = -60 degree
  0xE7, 0x28,       // HorizontalAngleEnd, 0x28E7 = 10471*180/31416 = 60 degree
  0x64, 0x00,       // FrequencyBandLo, 100Hz
  0x4C, 0x1D,       // FrequencyBandHi, 7500Hz
  0x02, 0x00,       // NumberOfMicrophones, 2 mic elements

    0x00, 0x00,   // Type, KSMICARRAY_MICTYPE_OMNIDIRECTIONAL=0
    0x00, 0x00,   // XCoord
    0xE7, 0xFF,   // YCoord, -25mm, nagtive to user's left hand side
    0x00, 0x00,   // ZCoord
    0x00, 0x00,   // VerticalAngle
    0x00, 0x00,   // HorizontalAngle

    0x00, 0x00,   // Type, KSMICARRAY_MICTYPE_OMNIDIRECTIONAL=0
    0x00, 0x00,   // XCoord
    0x19, 0x00,   // YCoord, 25mm, positive to user's right hand side
    0x00, 0x00,   // ZCoord
    0x00, 0x00,   // VerticalAngle
    0x00, 0x00,   // HorizontalAngle
}) //End MGEO

Name(MSNR, Buffer()
{
  0x00, 0x01,           // Version
  0x02, 0x00,           // NumberOfMicrophones, 2 mic elements
  0x00, 0x00, 0x41, 0x00,  // SNR of 1st Microphone, 65dB in 16.16 format
  0x00, 0x00, 0x41, 0x00,  // SNR of 2nd Microphone, 65dB in 16.16 format
}) //End MSNR

Name(MSNS, Buffer()
{
  0x00, 0x01,           // Version
  0x02, 0x00,           // NumberOfMicrophones, 2 mic elements
  0x00, 0x00, 0xE6, 0xFF,  // Sensitivity of 1st Microphone, -26dB in 16.16 format
  0x00, 0x00, 0xE6, 0xFF,  // Sensitivity of 2nd Microphone, -26dB in 16.16 format
}) //End MSNS

Name(C264, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Usage, class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 1},
    },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
  {
    Package(2) {"mipi-sdca-control-range", "USG1"},
  }
}) //End C264

Name(USG1, Buffer()
{
  0x07, 0x00,  // Range type 0x0007 (Septuples)
  0x01, 0x00,  // Count of ranges = 0x1
  0x01, 0x00, 0x00, 0x00, // Usage Number
  0xae, 0x01, 0x00, 0x00, // Usage CBN, 430 (0x1ae) Full band & Near-ultrasound
  0x00, 0x00, 0x00, 0x00, // Sample Rate, 0 not report
  0x00, 0x00, 0x00, 0x00, // Sample Width, 0 not report
  0x00, 0x80, 0x00, 0x00, // Full Scale, -128.0dB (0x8000) not report actual value
  0x00, 0x80, 0x00, 0x00, // Noise Floor, -128.0dB (0x8000) not report actual value
  0x00, 0x00, 0x00, 0x00, // Usage Tag
}) //End USG1

Name(C265, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // ClusterIndex, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 1},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package(2) {"mipi-sdca-control-range", "CLM1"},
  }
}) //End C265

Name(CLM1, Buffer()
{
  0x02, 0x00,  // Range type 2 parameters
  0x01, 0x00,  // Count of ranges = 0x1
  0x01, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, // Cluster Index = 0x01, Cluster ID = 0x11
}) //End CLM1




// FU 0x11 should be platform access only
Name(E015, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x07},
    Package (2) {"mipi-sdca-entity-label", "FU 11"},
    //Package (2) {"mipi-sdca-control-list", Package() {0xb} }, // gain
    Package (2) {"mipi-sdca-control-list", 0x0800}, // Bitmap: 0xB - gain
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E026"}, // Input Pin 1 connected to IT11
    Package (2) {"mipi-sdca-control-0xb-subproperties", "C15B"},
  }
}) //End E015

Name(C15B, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // gain, platform, RW
    Package (2) {"mipi-sdca-control-access-layer", 0x8},
    Package (2) {"mipi-sdca-control-access-mode", 0},
    //Package (2) {"mipi-sdca-control-default-value", 0}, // 0dB
    //Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2, 0x3, 0x4} },
    Package (2) {"mipi-sdca-control-cn-list", 0x1E },
  }
}) //End C15B




Name(E01C, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x25},
    Package (2) {"mipi-sdca-entity-label", "PPU 11"},
    //Package (2) {"mipi-sdca-control-list", Package() {0x10} }, // PostureNumber
    Package (2) {"mipi-sdca-control-list", 0x10000}, // Bitmap: 0x10 - PostureNumber
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E015"},  // Input Pin 1 connected to FU11
    Package (2) {"mipi-sdca-control-0x10-subproperties", "C1C1"},
  }
}) //End E01C

Name(C1C1, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 1},
    },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
  {
    Package(2) {"mipi-sdca-control-range", "PSM1"},
  }
})  // End C1C1

Name(PSM1, Buffer()
{
  0x0B, 0x00,  // Range type 0x000B (Septuples)
  0x01, 0x00,  // Count of ranges = 0x1
  0x00, 0x00, 0x00, 0x00, 0x67, 0x01, 0x00, 0x00, // Pitch Min (0), Pitch Max (359)
  0x00, 0x00, 0x00, 0x00, 0x67, 0x01, 0x00, 0x00, // Roll Min (0), Roll Max (359)
  0x00, 0x00, 0x00, 0x00, 0x67, 0x01, 0x00, 0x00, // Yaw Min (0), Yaw Max (359)
  0x00, 0x00, 0x00, 0x00, 0x67, 0x01, 0x00, 0x00, // HingeAngle Min (0), HingeAngle Max (359)
  0x00, 0x00, 0x00, 0x00, // System Extension Value
  0x01, 0x00, 0x00, 0x00, // Posture Number 0x01
  0x21, 0x00, 0x00, 0x00, // Cluster ID 0x21 (Use 0x22 for 4 channel mic array)
}) //End PSM1

/*
Name(E053, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x11},
    Package (2) {"mipi-sdca-entity-label", "PDE 12"},
    Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x16} },
    Package (2) {"mipi-sdca-powerdomain-transition-typical-delay",
      Package()
      {
        0x3, 0x0, 0,
        0x0, 0x3, 0,
        0x2, 0x0, 0,
        0x0, 0x2, 0,
        0x1, 0x0, 0,
        0x0, 0x1, 0,
        0x0, 0x0, 0,
      }},
    Package (2) {"mipi-sdca-powerdomain-transition-max-delay",
      Package()
      {
        0x3, 0x0, 0,
        0x0, 0x3, 0,
        0x2, 0x0, 0,
        0x0, 0x2, 0,
        0x1, 0x0, 0,
        0x0, 0x1, 0,
        0x0, 0x0, 0,
      }},
    //Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x10} }, // Requested_PS, Actual_PS
    Package (2) {"mipi-sdca-control-list", 0x00010002}, // Bitmap: 0x1 - Requested_PS, 0x10 - Actual_PS
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-control-0x1-subproperties", "RP53"},
    Package (2) {"mipi-sdca-control-0x10-subproperties", "AP53"},
  }
}) //End E053


Name(RP53, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Requested_PS, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0x0},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package ()
  {
    Package (2) {"mipi-sdca-control-range", "PDEC"},
  }
})

Name(PDEC, Buffer()
{
  0x01, 0x00, // Range type 0x0001
  0x01, 0x00, // Count of ranges = 0x1
  0x00, 0x00, 0x00, 0x00, // PS0
  //0x03, 0x00, 0x00, 0x00, // PS3
}) //End PDEC

Name(AP53, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Actual_PS, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0x0},
  }
})

*/

Name(E016, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x0A},
    Package (2) {"mipi-sdca-entity-label", "XU 12"},
    //Package (2) {"mipi-sdca-control-list", Package() {0x1} }, // Bypass, ClusterIndex
    Package (2) {"mipi-sdca-control-list", 0x02}, // Bitmap: 0x1 - Bypass
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E01C"}, // Input Pin 1 connected to PPU 11
    Package (2) {"mipi-sdca-control-0x1-subproperties", "C161"},
  }
}) //End E016

Name(C161, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Bypass, Class, R/W
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 0},
  }
}) //End C161


Name(E01D, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x22},
    Package (2) {"mipi-sdca-entity-label", "MFPU 113"},
    //Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x10} }, // Bypass, ClusterIndex
    Package (2) {"mipi-sdca-control-list", 0x000010002}, // Bitmap: 0x1 - Bypass, 0x10 - ClusterIndex
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E016"}, // Input Pin 1 connected to XU12
    Package (2) {"mipi-sdca-control-0x1-subproperties", "C1D1"},
    Package (2) {"mipi-sdca-control-0x10-subproperties", "C1D2"},
  }
}) //End E01D


Name(C1D1, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Bypass, Class, DC:1
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 1},  // DC:1 permanently bypassed for NULL MFPU
  }
}) //End C1D1

Name(C1D2, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // ClusterIndex, Class, DC:1
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 1},  // DC:1
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package(2) {"mipi-sdca-control-range", "CLM2"},
  }
}) //End C1D2

Name(CLM2, Buffer()
{
  0x02, 0x00,  // Range type 2 parameters
  0x01, 0x00,  // Count of ranges = 0x1
  0x01, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, // Cluster Index = 0x01, Cluster ID = 0x31 (Use 0x32 for 4ch MicArray)
}) //End CLM2



Name(E01E, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x07},
    Package (2) {"mipi-sdca-entity-label", "FU 113"},
    //Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x2, 0x10} }, // Mute, Volume, Latency
    Package (2) {"mipi-sdca-control-list", 0x00010006}, // Bitmap: 0x1 - Mute, 0x2 - Volume, 0x10 - Latency
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E01D"}, // Input Pin 1 connected MFPU113
    Package (2) {"mipi-sdca-control-0x1-subproperties", "C1E1"},
    Package (2) {"mipi-sdca-control-0x2-subproperties", "C1E2"},
    Package (2) {"mipi-sdca-control-0x10-subproperties", "LC00"},
  }
}) //End E01E

Name(C1E1, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // mute, application, RW
    Package (2) {"mipi-sdca-control-access-layer", 0x2}, // layer is application
    Package (2) {"mipi-sdca-control-access-mode", 0},
    //Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2, 0x3, 0x4} },
    Package (2) {"mipi-sdca-control-cn-list", 0x1E },
  }
}) //End C1E1

Name(C1E2, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Volume, application, RW
    Package (2) {"mipi-sdca-control-access-layer", 0x2}, // layer is application
    Package (2) {"mipi-sdca-control-access-mode", 0},
    Package (2) {"mipi-sdca-control-default-value", 0x1500},  // 21dB
    //Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2, 0x3, 0x4} },
    Package (2) {"mipi-sdca-control-cn-list", 0x1E },
  },

  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package(2) {"mipi-sdca-control-range", "BUF1"},
  }
}) //End C1E2

Name(BUF1, Buffer()
{
  0x03, 0x00, // Range type 0x0003
  0x01, 0x00, // Count of ranges = 0x1
  0xC0, 0xEE, 0x00, 0x00, // Min = 0x0000EEC0
  0x00, 0x1E, 0x00, 0x00, // Max = 0x00001E00
  0xC0, 0x00, 0x00, 0x00, // Step = 0x000000C0
}) //End BUF1


Name(LC00, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Latency, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0},
  }
}) //End AF01.LC00

Name(E01F, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x0b},
    Package (2) {"mipi-sdca-entity-label", "CS 113"},
    Package (2) {"mipi-sdca-cs-type", 0},
    //Package (2) {"mipi-sdca-control-list", Package() {0x10} }, // Sample_Freq_Index
    Package (2) {"mipi-sdca-control-list", 0x00010000}, // Bitmap: 0x10 - Sample_Freq_Index
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-control-0x10-subproperties", "C1F1"},
  }
}) //End E01F

Name(C1F1, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Sample_Freq_Index, Class, RW
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 0},
  },

  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package(2) {"mipi-sdca-control-range", "BUF2"},
  }
}) //End C1F1


Name(BUF2, Buffer()
{
  0x02, 0x00, // Range type 0x0002
  0x02, 0x00, // Count of ranges = 0x2
  // 0x04, 0x00, 0x00, 0x00, 0x80, 0x3E, 0x00, 0x00, // 0x00000004, 0x00003E80(16000)
  // 0x07, 0x00, 0x00, 0x00, 0x00, 0x7D, 0x00, 0x00, // 0x00000007, 0x00007D00(32000)
  // 0x08, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // 0x00000008, 0x0000AC44(44100)
  0x09, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // 0x00000009, 0x0000BB80(48000)
  0x0B, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // 0x0000000B, 0x00017700(96000)
  // 0x0D, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x02, 0x00, // 0x0000000D, 0x0002EE00(192000)
}) //End BUF2


Name(E020, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x03},
    Package (2) {"mipi-sdca-entity-label", "OT 113"},
    Package (2) {"mipi-sdca-terminal-type", 0x0181}, // Raw PCM Capture
    Package (2) {"mipi-sdca-input-pin-list", 0x2},  // Input pin 1 connected
    //Package (2) {"mipi-sdca-control-list", Package() {0x4, 0x8, 0x11} }, // Usage, Latency, DataPortIndex
    Package (2) {"mipi-sdca-control-list", 0x00020110}, // Bitmap: 0x4 - Usage, 0x8 - Latency, 0x11 - DataPortIndex
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E01E"},  // Input pin 1 connected FU113
    Package (2) {"mipi-sdca-terminal-clock-connection", "E01F"},
    Package (2) {"mipi-sdca-control-0x4-subproperties", "C200"},
    Package (2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
    Package (2) {"mipi-sdca-control-0x11-subproperties", "C201"},
  },

  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package(2) {"mipi-sdca-terminal-dp-numbers", "BUF3"},
  }
}) //End E020

Name(BUF3, Buffer()
{
  0x0, 0x8, // DP_Index, DP_Num = 0x8
}) //End BUF3

Name(C200, Package() {
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Usage, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 0x5},
    Package (2) {"mipi-sdca-control-dc-value", 1},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package(2) {"mipi-sdca-control-range", "USG2"},
  }
}) //End C200

Name(USG2, Buffer()
{
  0x07, 0x00,  // Range type 0x0007 (Septuples)
  0x04, 0x00,  // Count of ranges = 0x4
  0x01, 0x00, 0x00, 0x00, // Usage Number
  0xae, 0x01, 0x00, 0x00, // Usage CBN, 430 (0x1ae) Full band & Near Ultrasound
  0x80, 0xbb, 0x00, 0x00, // Sample Rate, 48000Hz (0xBB80)
  0x18, 0x00, 0x00, 0x00, // Sample Width, 24-bits (0x18)
  0x00, 0x00, 0x00, 0x00, // Full Scale, 0 not report
  0x00, 0x00, 0x00, 0x00, // Noise Floor, 0 not report
  0x00, 0x00, 0x00, 0x00, // Usage Tag
  0x01, 0x00, 0x00, 0x00, // Usage Number
  0xae, 0x01, 0x00, 0x00, // Usage CBN, 430 (0x1ae) Full band & Near Ultrasound
  0x80, 0xbb, 0x00, 0x00, // Sample Rate, 48000Hz (0xBB80)
  0x10, 0x00, 0x00, 0x00, // Sample Width, 16-bits (0x10)
  0x00, 0x00, 0x00, 0x00, // Full Scale, 0 not report
  0x00, 0x00, 0x00, 0x00, // Noise Floor, 0 not report
  0x00, 0x00, 0x00, 0x00, // Usage Tag
  0x01, 0x00, 0x00, 0x00, // Usage Number
  0xe0, 0x01, 0x00, 0x00, // Usage CBN, 480 (0x1e0) HiFi Full band & Ultrasound40
  0x00, 0x77, 0x01, 0x00, // Sample Rate, 96000Hz (0x17700)
  0x18, 0x00, 0x00, 0x00, // Sample Width, 24-bits (0x18)
  0x00, 0x00, 0x00, 0x00, // Full Scale, 0 not report
  0x00, 0x00, 0x00, 0x00, // Noise Floor, 0 not report
  0x00, 0x00, 0x00, 0x00, // Usage Tag
  0x01, 0x00, 0x00, 0x00, // Usage Number
  0xe0, 0x01, 0x00, 0x00, // Usage CBN, 480 (0x1e0) HiFi Full band & Ultrasound40
  0x00, 0x77, 0x01, 0x00, // Sample Rate, 96000Hz (0x17700)
  0x10, 0x00, 0x00, 0x00, // Sample Width, 16-bits (0x10)
  0x00, 0x00, 0x00, 0x00, // Full Scale, 0 not report
  0x00, 0x00, 0x00, 0x00, // Noise Floor, 0 not report
  0x00, 0x00, 0x00, 0x00, // Usage Tag
}) //End USG2

Name(C201, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // DataPortIndex, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0},
  }
}) //End C201

// Name(E017, Package()
// {
//   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
//   Package ()
//   {
//     Package (2) {"mipi-sdca-entity-type", 0x20},
//     Package (2) {"mipi-sdca-entity-label", "CRU 14"},
//     //Package (2) {"mipi-sdca-control-list", Package() {0x10} }, // clusterindex
//     Package (2) {"mipi-sdca-control-list", 0x00010000}, // Bitmap:  0x10 - clusterindex
//     Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
//   },

//   ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
//   Package ()
//   {
//     Package (2) {"mipi-sdca-input-pin-1", "E016"}, // Input Pin 1 connected XU 12
//     Package (2) {"mipi-sdca-control-0x10-subproperties", "C171"},
//   }
// }) //End E017


// Name(C171, Package()
// {
//   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
//   Package ()
//   {
//     // ClusterIndex, Class, DC
//     Package (2) {"mipi-sdca-control-access-layer", 0x4},
//     Package (2) {"mipi-sdca-control-access-mode", 5},
//     Package (2) {"mipi-sdca-control-dc-value", 1},
//   },
//   ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
//   Package()
//   {
//     Package(2) {"mipi-sdca-control-range", "CLM3"},
//   }
// }) //End C171

// Name(CLM3, Buffer()
// {
//   0x02, 0x00,  // Range type 2 parameters
//   0x01, 0x00,  // Count of ranges = 0x1
//   0x01, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, // for CRU 14, Cluster Index = 0x01, Cluster ID = 0x41 (Use 0x42 for 4ch MicArray)
// }) //End CLM3

Name(E06A, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x07},
    //Package (2) {"mipi-sdca-entity-label", "FU 14"},
    Package (2) {"mipi-sdca-entity-label", "FU 13"},
    //Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x2, 0x10} }, // Mute, Volume, Latency
    Package (2) {"mipi-sdca-control-list", 0x00010006}, // Bitmap: 0x1 - Mute, 0x2 - Volume, 0x10 - Latency
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    //Package (2) {"mipi-sdca-input-pin-1", "E017"}, // Input Pin 1 connected CRU 14
    Package (2) {"mipi-sdca-input-pin-1", "E019"}, // Input Pin 1 connected MFPU 13
    Package (2) {"mipi-sdca-control-0x1-subproperties", "C6A1"},
    Package (2) {"mipi-sdca-control-0x2-subproperties", "C6A2"},
    Package (2) {"mipi-sdca-control-0x10-subproperties", "LC00"},
  }
}) //End E06A

Name(C6A1, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // mute, application, RW
    Package (2) {"mipi-sdca-control-access-layer", 0x2}, // layer is application
    Package (2) {"mipi-sdca-control-access-mode", 0},
    Package (2) {"mipi-sdca-control-fixed-value", 0x0000},  // unmute
    //Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} },
    Package (2) {"mipi-sdca-control-cn-list", 0x06 },
  }
}) //End C6A1

Name(C6A2, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Volume, application, RW
    Package (2) {"mipi-sdca-control-access-layer", 0x2}, // layer is application
    Package (2) {"mipi-sdca-control-access-mode", 0},
    Package (2) {"mipi-sdca-control-fixed-value", 0x0000},  // 0dB
    //Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} },
    Package (2) {"mipi-sdca-control-cn-list", 0x06 },
  },

  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package(2) {"mipi-sdca-control-range", "BUF4"},
  }
}) //End C6A2

Name(BUF4, Buffer()
{
  0x03, 0x00, // Range type 0x0003
  0x01, 0x00, // Count of ranges = 0x1
  0xC0, 0xEE, 0x00, 0x00, // Min = 0x0000EEC0
  0x00, 0x1E, 0x00, 0x00, // Max = 0x00001E00
  0xC0, 0x00, 0x00, 0x00, // Step = 0x000000C0
}) //End BUF4


Name(E01A, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x23},
    //Package (2) {"mipi-sdca-entity-label", "SMPU 14"},
    Package (2) {"mipi-sdca-entity-label", "SMPU 13"},
           Package (2) {"mipi-sdca-input-pin-list", 0x2},  // Input Pin 1 connected
    Package (2) {"mipi-sdca-control-list", 0x00070000}, // Bitmap: 0x10 - Trigger_Enable, 0x11 - Trigger_Status, 0x12 - Hist_Buffer_Mode
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
    //Package (2) {"mipi-sdca-input-pin-1", "E06A"}, // Input Pin 1 connected to FU 14
    Package (2) {"mipi-sdca-input-pin-1", "E06A"}, // Input Pin 1 connected to FU 13
    Package (2) {"mipi-sdca-control-0x10-subproperties", "C1A0"},  // Trigger_Enable
    Package (2) {"mipi-sdca-control-0x11-subproperties", "C1A1"},  // Trigger_Status
    Package (2) {"mipi-sdca-control-0x12-subproperties", "C1A2"},  // Hist_Buffer_Mode
        }
}) //End E01A

Name(C1A0, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // trigger_enable, Class, RW
            Package (2) {"mipi-sdca-control-access-layer", 4},
            Package (2) {"mipi-sdca-control-access-mode", 0},
        }
}) //End C1A0

Name(C1A1, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { // trigger_status, Class, RW1C
            Package (2) {"mipi-sdca-control-access-layer", 4},
            Package (2) {"mipi-sdca-control-interrupt-position", 17},
            Package (2) {"mipi-sdca-control-access-mode", 2},  // RW1C
        }
}) //End C1A1

Name(C1A2, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {
    // Hist_Buffer_Mode, Class, DC:3 (No buffer)
    Package(2) {"mipi-sdca-control-access-layer", 0x4},
    Package(2) {"mipi-sdca-control-access-mode", 5},
    Package(2) {"mipi-sdca-control-dc-value", 3},
  }
}) //End C1A2

Name(E019, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x22},
    //Package (2) {"mipi-sdca-entity-label", "MFPU 14"},
    Package (2) {"mipi-sdca-entity-label", "MFPU 13"},
    //Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x10} }, // Bypass, ClusterIndex
    Package (2) {"mipi-sdca-control-list", 0x000010002}, // Bitmap: 0x1 - Bypass, 0x10 - ClusterIndex
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    //Package (2) {"mipi-sdca-input-pin-1", "E01A"}, // Input Pin 1 connected to SMPU 14
    Package (2) {"mipi-sdca-input-pin-1", "E016"}, // Input Pin 1 connected XU 12
    Package (2) {"mipi-sdca-control-0x1-subproperties", "C191"},
    Package (2) {"mipi-sdca-control-0x10-subproperties", "C192"},
  }
}) //End E019


Name(C191, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Bypass, Class, DC:1
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 1},  // DC:1 permanently bypassed for NULL MFPU
  }
}) //End C191

Name(C192, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // ClusterIndex, Class, DC:1
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 1},  // DC:1
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package(2) {"mipi-sdca-control-range", "CLM4"},
  }
}) //End C192

Name(CLM4, Buffer()
{
  0x02, 0x00,  // Range type 2 parameters
  0x01, 0x00,  // Count of ranges = 0x1
  //0x01, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, // for MFPU 14, Cluster Index = 0x01, Cluster ID = 0x41 (Use 0x42 for 4ch MicArray)
  0x01, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, // for MFPU 13, Cluster Index = 0x01, Cluster ID = 0x31 (Use 0x32 for 4ch MicArray)
}) //End CLM4


Name(E06B, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x0b},
    //Package (2) {"mipi-sdca-entity-label", "CS 14"},
    Package (2) {"mipi-sdca-entity-label", "CS 13"},
    Package (2) {"mipi-sdca-cs-type", 0},
    //Package (2) {"mipi-sdca-control-list", Package() {0x10} }, // Sample_Freq_Index
    Package (2) {"mipi-sdca-control-list", 0x00010000}, // Bitmap: 0x10 - Sample_Freq_Index
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-control-0x10-subproperties", "C6B1"},
  }
}) //End E01F

Name(C6B1, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Sample_Freq_Index, Class, RW
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 0},
  },

  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package(2) {"mipi-sdca-control-range", "BUF5"},
  }
}) //End C1F1


Name(BUF5, Buffer()
{
  0x02, 0x00, // Range type 0x0002
  0x01, 0x00, // Count of ranges = 0x1
  0x09, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // 0x00000009, 0x0000BB80(48000)
  //0x0B, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // 0x0000000B, 0x00017700(96000)

}) //End BUF5



Name(E01B, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x03},
    //Package (2) {"mipi-sdca-entity-label", "OT 14"},
    //Package (2) {"mipi-sdca-terminal-type", 0x0180}, // Ultrasound Source Data Port
    Package (2) {"mipi-sdca-entity-label", "OT 13"},
    Package (2) {"mipi-sdca-terminal-type", 0x0185}, // Secondary PCM Mic Source Data Port
    Package (2) {"mipi-sdca-input-pin-list", 0x2},  // Input pin 1 connected
    //Package (2) {"mipi-sdca-control-list", Package() {0x4, 0x8, 0x11} }, // Usage, Latency, DataPortIndex
    Package (2) {"mipi-sdca-control-list", 0x00020110}, // Bitmap: 0x4 - Usage, 0x8 - Latency, 0x11 - DataPortIndex
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    //Package (2) {"mipi-sdca-input-pin-1", "E019"},  // Input pin 1 connected MFPU 14
    Package (2) {"mipi-sdca-input-pin-1", "E01A"},  // Input pin 1 connected SMPU 13
    Package (2) {"mipi-sdca-terminal-clock-connection", "E06B"},
    Package (2) {"mipi-sdca-control-0x4-subproperties", "C1B0"},
    Package (2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
    Package (2) {"mipi-sdca-control-0x11-subproperties", "C1B1"},
  },

  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package(2) {"mipi-sdca-terminal-dp-numbers", "BUF6"},
  }
}) //End E01B

Name(BUF6, Buffer()
{
  0x0, 0xA, // DP_Index, DP_Num = 0xA
}) //End BUF6

Name(C1B0, Package() {
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Usage, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 0x5},
    Package (2) {"mipi-sdca-control-dc-value", 1},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package(2) {"mipi-sdca-control-range", "USG3"},
  }
}) //End C1B0

Name(USG3, Buffer()
{
  0x07, 0x00,  // Range type 0x0007 (Septuples)
  0x02, 0x00,  // Count of ranges = 0x2
  0x01, 0x00, 0x00, 0x00, // Usage Number
  0xae, 0x01, 0x00, 0x00, // Usage CBN, 430 (0x1ae) Full band & Near-ultrasound
  0x80, 0xbb, 0x00, 0x00, // Sample Rate, 48000Hz (0xBB80)
  0x18, 0x00, 0x00, 0x00, // Sample Width, 24-bits (0x18)
  0x00, 0x00, 0x00, 0x00, // Full Scale, 0 not report
  0x00, 0x00, 0x00, 0x00, // Noise Floor, 0 not report
  0x00, 0x00, 0x00, 0x00, // Usage Tag
        0x01, 0x00, 0x00, 0x00, // Usage Number
  0xae, 0x01, 0x00, 0x00, // Usage CBN, 430 (0x1ae) Full band & Near-ultrasound
  0x80, 0xbb, 0x00, 0x00, // Sample Rate, 48000Hz (0xBB80)
  0x10, 0x00, 0x00, 0x00, // Sample Width, 16-bits (0x10)
  0x00, 0x00, 0x00, 0x00, // Full Scale, 0 not report
  0x00, 0x00, 0x00, 0x00, // Noise Floor, 0 not report
  0x00, 0x00, 0x00, 0x00, // Usage Tag
  // 0x01, 0x00, 0x00, 0x00, // Usage Number
  // 0xe0, 0x01, 0x00, 0x00, // Usage CBN, 480 (0x1e0) HiFi Full band & Ultrasound40
  // 0x00, 0x77, 0x01, 0x00, // Sample Rate, 96000Hz (0x17700)
  // 0x18, 0x00, 0x00, 0x00, // Sample Width, 24-bits (0x18)
  // 0x00, 0x00, 0x00, 0x00, // Full Scale, 0 not report
  // 0x00, 0x00, 0x00, 0x00, // Noise Floor, 0 not report
  // 0x00, 0x00, 0x00, 0x00, // Usage Tag
}) //End USG3

Name(C1B1, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // DataPortIndex, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0},
  }
}) //End C1B1

// =================================== Below part is SPE function.
/*
Name(E023, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x2},
    Package (2) {"mipi-sdca-entity-label", "IT 199"},
    Package (2) {"mipi-sdca-terminal-type", 0x0741},
    //Package (2) {"mipi-sdca-control-list", Package() {0x30} }, // imp-def control 0x30
    Package (2) {"mipi-sdca-control-list", 0x0001000000000000}, // Bitmap: 0x30 - imp-def control 0x30
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    // imp-def control 0x30
    Package (2) {"mipi-sdca-control-0x30-subproperties", "C230"},
  },

}) //End AF01.E002

Name(C230, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // imp-def control 0x30, Extension, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x20},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0},
  }
})

    //=============================================================================

Name(E024, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x13},
    Package (2) {"mipi-sdca-entity-label", "SPE 199"},
    Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
    //Package (2) {"mipi-sdca-control-list",  Package() {0x10} },  // Sample_Freq_Index
    Package (2) {"mipi-sdca-control-list",  0x001E}, // Bitmap: 0x01 - Private, 0x02 - Privacy_policy, 0x03 - Privacy_lockstate, 0x04 - Privacy_owner
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E023"}, // Input Pin 1 connected to IT199
    Package (2) {"mipi-sdca-control-0x01-subproperties", "C241"},
    Package (2) {"mipi-sdca-control-0x02-subproperties", "C242"},
    Package (2) {"mipi-sdca-control-0x03-subproperties", "C243"},
    Package (2) {"mipi-sdca-control-0x04-subproperties", "C244"},
  }
}) //End AF01.E001

Name(C241, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Private, Class, R/W
    Package (2) {"mipi-sdca-control-access-layer", 2},
    Package (2) {"mipi-sdca-control-interrupt-position", 21}, // SDCA Interrupt Bit 5
    Package (2) {"mipi-sdca-control-access-mode", 0},
  }
})

Name(C242, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Privacy_Policy, Class, R/W
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 0},
  }
})

Name(C243, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Privacy_Lockstate, Class, R/W
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 0},
  }
})

Name(C244, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Privacy_owner, Class, R/W
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 0},

  }
})

  //=============================================================================

Name(E025, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x11},
    Package (2) {"mipi-sdca-entity-label", "PDE 199"},
    Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x27}},
    Package (2) {"mipi-sdca-powerdomain-transition-typical-delay",
      Package()
      {
        0x3, 0x0, 0x0,
        0x0, 0x3, 0x0,
        0x2, 0x0, 0x0,
        0x0, 0x2, 0x0,
        0x1, 0x0, 0x0,
        0x0, 0x1, 0x0,
        0x0, 0x0, 0x0,
      }
    },
      Package (2) {"mipi-sdca-powerdomain-transition-max-delay",
      Package()
      {
        0x3, 0x0, 0x0,
        0x0, 0x3, 0x0,
        0x2, 0x0, 0x0,
        0x0, 0x2, 0x0,
        0x1, 0x0, 0x0,
        0x0, 0x1, 0x0,
        0x0, 0x0, 0x0,
      }
    },
    //Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x10}}, // Requested_PS, Actual_PS
    Package (2) {"mipi-sdca-control-list", 0x00010002}, // 0x1 - Requested_PS, 0x10 - Actual_PS
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-control-0x1-subproperties", "RP25"},
    Package (2) {"mipi-sdca-control-0x10-subproperties", "AP25"},
  }
})

Name(RP25, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Requested_PS, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package ()
  {
    Package (2) {"mipi-sdca-control-range", "PDEC"},
  }
})

Name(AP25, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // Actual_PS, Class, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x4},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0},
  }
})

  //=============================================================================================================

Name(E027, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x03},
    Package (2) {"mipi-sdca-entity-label", "OT 199"},
    Package (2) {"mipi-sdca-terminal-type", 0x0747},
    Package (2) {"mipi-sdca-input-pin-list", 0x2},  // Input Pin 1 connected
    //Package (2) {"mipi-sdca-control-list", Package() {0x30} }, // imp-def control 0x30
    Package (2) {"mipi-sdca-control-list", 0x0001000000000000}, // Bitmap: 0x30 - imp-def control 0x30
  },

  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-input-pin-1", "E024"},  // Input Pin 1 connected to SPE199
    Package (2) {"mipi-sdca-control-0x30-subproperties", "C270"},
  }
})

Name(C270, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    // imp-def control 0x30, Extension, DC
    Package (2) {"mipi-sdca-control-access-layer", 0x20},
    Package (2) {"mipi-sdca-control-access-mode", 5},
    Package (2) {"mipi-sdca-control-dc-value", 0},
  }
})

*/

Name(EXT0, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-function-expansion-subsystem-id", 0x1234},
    Package (2) {"mipi-sdca-function-expansion-subsystem-revision-id", 0x1234},
  }
}) //End EXT0

//===================================================
// Cluster Library

Name(CL11, Package()  // default-mic-set, 2 channel
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-count", 2},          // 2 channel
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-1-subproperties", "CH11"},  // channel 1, Generic Audio, AR
    Package (2) {"mipi-sdca-channel-2-subproperties", "CH12"},  // channel 2, Generic Audio, AR
  }
}) //End CL11

Name(CL12, Package()  // default-mic-set, 4 channel
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-count", 4},          // 4 channel
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-1-subproperties", "CH11"},  // channel 1, Generic Audio, AR
    Package (2) {"mipi-sdca-channel-2-subproperties", "CH12"},  // channel 2, Generic Audio, AR
    Package (2) {"mipi-sdca-channel-3-subproperties", "CH13"},  // channel 3, Generic Audio, AR
    Package (2) {"mipi-sdca-channel-4-subproperties", "CH14"},  // channel 4, Generic Audio, AR
  }
}) //End CL11

Name(CH11, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 1},      // channel 1
    Package (2) {"mipi-sdca-cluster-channel-purpose", 1},    // 0x01: Generic Audio
    Package (2) {"mipi-sdca-cluster-channel-relationship", 4},  // 0x04: AR
  }
}) //End CH11

Name(CH12, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 2},      // channel 2
    Package (2) {"mipi-sdca-cluster-channel-purpose", 1},    // 0x01: Generic Audio
    Package (2) {"mipi-sdca-cluster-channel-relationship", 4},  // 0x04: AR
  }
}) //End CH12

Name(CH13, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 3},      // channel 3
    Package (2) {"mipi-sdca-cluster-channel-purpose", 1},    // 0x01: Generic Audio
    Package (2) {"mipi-sdca-cluster-channel-relationship", 4},  // 0x04: AR
  }
}) //End CH13

Name(CH14, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 4},      // channel 4
    Package (2) {"mipi-sdca-cluster-channel-purpose", 1},    // 0x01: Generic Audio
    Package (2) {"mipi-sdca-cluster-channel-relationship", 4},  // 0x04: AR
  }
}) //End CH14

Name(CL21, Package()  // 2 channel, Pass through
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-count", 2},          // 2 channel
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-1-subproperties", "CH21"},  // channel 1, Generic Audio, MIC
    Package (2) {"mipi-sdca-channel-2-subproperties", "CH22"},  // channel 2, Generic Audio, MIC
  }
}) //End CL21

Name(CL22, Package()  // 4 channel, Pass through
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-count", 4},          // 4 channel
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-1-subproperties", "CH21"},  // channel 1, Generic Audio, MIC
    Package (2) {"mipi-sdca-channel-2-subproperties", "CH22"},  // channel 2, Generic Audio, MIC
    Package (2) {"mipi-sdca-channel-3-subproperties", "CH23"},  // channel 3, Generic Audio, MIC
    Package (2) {"mipi-sdca-channel-4-subproperties", "CH24"},  // channel 4, Generic Audio, MIC
  }
}) //End CL21

Name(CH21, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 1},        // channel 1
    Package (2) {"mipi-sdca-cluster-channel-purpose", 1},      // 0x01: Generic Audio
    Package (2) {"mipi-sdca-cluster-channel-relationship", 0x53},  // 0x53: MIC
  }
}) //End CH21

Name(CH22, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 2},        // channel 2
    Package (2) {"mipi-sdca-cluster-channel-purpose", 1},      // 0x01: Generic Audio
    Package (2) {"mipi-sdca-cluster-channel-relationship", 0x53},  // 0x53: MIC
  }
}) //End CH22

Name(CH23, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 3},        // channel 3
    Package (2) {"mipi-sdca-cluster-channel-purpose", 1},      // 0x01: Generic Audio
    Package (2) {"mipi-sdca-cluster-channel-relationship", 0x53},  // 0x53: MIC
  }
}) //End CH23

Name(CH24, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 4},        // channel 4
    Package (2) {"mipi-sdca-cluster-channel-purpose", 1},      // 0x01: Generic Audio
    Package (2) {"mipi-sdca-cluster-channel-relationship", 0x53},  // 0x53: MIC
  }
}) //End CH24

Name(CL31, Package()  // 2 channel, Pass through
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-count", 2},          // 2 channel
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-1-subproperties", "CH31"},  // channel 1, Generic Audio, MIC
    Package (2) {"mipi-sdca-channel-2-subproperties", "CH32"},  // channel 2, Generic Audio, MIC
  }
}) //End CL31

Name(CL32, Package()  // 4 channel, Pass through
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-count", 4},          // 4 channel
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-1-subproperties", "CH31"},  // channel 1, Generic Audio, MIC
    Package (2) {"mipi-sdca-channel-2-subproperties", "CH32"},  // channel 2, Generic Audio, MIC
    Package (2) {"mipi-sdca-channel-3-subproperties", "CH33"},  // channel 3, Generic Audio, MIC
    Package (2) {"mipi-sdca-channel-4-subproperties", "CH34"},  // channel 4, Generic Audio, MIC
  }
}) //End CL32

Name(CH31, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 1},        // channel 1
    Package (2) {"mipi-sdca-cluster-channel-purpose", 1},      // 0x01: Generic Audio
    Package (2) {"mipi-sdca-cluster-channel-relationship", 0x51},  // 0x51: MIC
  }
}) //End CH31

Name(CH32, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 2},        // channel 2
    Package (2) {"mipi-sdca-cluster-channel-purpose", 1},      // 0x01: Generic Audio
    Package (2) {"mipi-sdca-cluster-channel-relationship", 0x51},  // 0x51: MIC
  }
}) //End CH32

Name(CH33, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 3},        // channel 3
    Package (2) {"mipi-sdca-cluster-channel-purpose", 1},      // 0x01: Generic Audio
    Package (2) {"mipi-sdca-cluster-channel-relationship", 0x51},  // 0x51: MIC
  }
}) //End CH33

Name(CH34, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 4},        // channel 4
    Package (2) {"mipi-sdca-cluster-channel-purpose", 1},      // 0x01: Generic Audio
    Package (2) {"mipi-sdca-cluster-channel-relationship", 0x51},  // 0x51: MIC
  }
}) //End CH34

Name(CL41, Package()  // 2 channel, ultrasound
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-count", 2},          // 2 channel
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-1-subproperties", "CH41"},  // channel 1, ultrasound, MIC
    Package (2) {"mipi-sdca-channel-2-subproperties", "CH42"},  // channel 2, ultrasound, MIC
  }
}) //End CL41

Name(CL42, Package()  // 4 channel, ultrasound
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-count", 4},          // 4 channel
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-channel-1-subproperties", "CH41"},  // channel 1, ultrasound, MIC
    Package (2) {"mipi-sdca-channel-2-subproperties", "CH42"},  // channel 2, ultrasound, MIC
    Package (2) {"mipi-sdca-channel-3-subproperties", "CH43"},  // channel 3, ultrasound, MIC
    Package (2) {"mipi-sdca-channel-4-subproperties", "CH44"},  // channel 4, ultrasound, MIC
  }
}) //End CL42

Name(CH41, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 1},        // channel 1
    Package (2) {"mipi-sdca-cluster-channel-purpose", 6},      // 0x06: ultrasound
    Package (2) {"mipi-sdca-cluster-channel-relationship", 0x51},  // 0x51: MIC
  }
}) //End CH41

Name(CH42, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 2},        // channel 2
    Package (2) {"mipi-sdca-cluster-channel-purpose", 6},      // 0x06: ultrasound
    Package (2) {"mipi-sdca-cluster-channel-relationship", 0x51},  // 0x51: MIC
  }
}) //End CH42

Name(CH43, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 3},        // channel 3
    Package (2) {"mipi-sdca-cluster-channel-purpose", 6},      // 0x06: ultrasound
    Package (2) {"mipi-sdca-cluster-channel-relationship", 0x51},  // 0x51: MIC
  }
}) //End CH43

Name(CH44, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-cluster-channel-id", 4},        // channel 4
    Package (2) {"mipi-sdca-cluster-channel-purpose", 6},      // 0x06: ultrasound
    Package (2) {"mipi-sdca-cluster-channel-relationship", 0x51},  // 0x51: MIC
  }
}) //End CH44

// End Of Cluster Library
