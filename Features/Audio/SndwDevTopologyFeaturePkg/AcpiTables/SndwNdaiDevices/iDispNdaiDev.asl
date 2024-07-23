/**@file
  DisCo Tool Version: 2022.12.23

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
Device(IDP0) {
Name (_ADR, 0x000000d010010500)
Name (_DSD, Package() { ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), Package () {
// Topology: SampleTopo
    // Adheres to DisCo for SoundWire 2.0 r7 (2022-11-03)
    // DisCo rev 2.0
    Package (2) {"mipi-sdw-sw-interface-revision", 0x20000},
    // SDCA 0.6 r54
    Package (2) {"mipi-sdw-sdca-interface-revision", 0x636},
    Package (2) {"mipi-sdw-paging-supported", 1},
    // 0x3 = 2 IntStat DWORDs
    Package (2) {"mipi-sdw-sdca-interrupt-register-list", 0x3},
    // Two additional Data Lanes used
    Package (2) {"mipi-sdw-lane-1-mapping", "mipi-sdw-manager-lane-1"},
    Package (2) {"mipi-sdw-lane-2-mapping", "mipi-sdw-manager-lane-2"},
    Package (2) {"mipi-sdw-lane-3-mapping", "mipi-sdw-manager-lane-3"},
    Package(2) {"dummy", 0}
}, // params
ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
Package () {
    Package(2) {"dummy", 0}
    } // refs
}) // Name
// Audio Function: AF_AudioFunctionType.NDAI_6 (instance <__main__.AudioFunction object at 0x032C0CE8>)
Device(IDPN) {

Name (_ADR, 0x000000d011010500)
    Name (_DSD, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // A mipi-sdca-function-topology-features field, which is a 64-bit bitmask identifying which of various optional features are implemented within that Function.
        //
        // # (SDCA 0.6 r54) Table 7 Topology Options in Function Type: NDIS
        // BT NDAI bits: 0 -> 0x0080'0000'0000'0000
        Package (2) {"mipi-sdca-function-topology-features", 0x80000000000000},
        // Blind write table. Set of {addr, val} applied by SW to device
        Package (2) {"mipi-sdca-function-initialization-table", Buffer() {0, 0, 0, 0}},
        Package (2) {"mipi-sdca-system-posture-config", 1},
        Package (2) {"mipi-sdca-function-busy-max-delay", 1000},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0x10, 0x11, 0x12, 0x2c, 0x2d, 0x2e, 0x2f, 0x3f}},
        Package (2) {"mipi-sdca-entity-id-list", Package() {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x72, 0x73, 0x71}},
        // Clusters: 0 = Disabled, 1 = 2ch, 2 = 3ch, 3 = 4ch, 4 = 5ch, 5 = 6ch, 6 = 7ch, 7 = 8ch
        Package (2) {"mipi-sdca-cluster-id-list", Package() {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7}},
        // Posture: select 2ch out of 4 on input terminal
        // 2ch:
        // DMIC0, direct
        // DMIC0, inverted
        // DMIC1,direct
        // DMIC1 inverted
        // 4ch
        // direct (1,2,3,4)
        // inverted (4,3,2,1)
        Package (2) {"mipi-sdca-system-posture-list", Package() {0x7, 0x9}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-function-expansion-subproperties", "FE00"},
        // AF.Commit_Group_Mask
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO00"},
        // AF.Function_SDCA_Version
        Package (2) {"mipi-sdca-control-selector-0x4-subproperties", "CO01"},
        // AF.Function_Type
        Package (2) {"mipi-sdca-control-selector-0x5-subproperties", "CO02"},
        // AF.Function_Manufacturer_ID
        Package (2) {"mipi-sdca-control-selector-0x6-subproperties", "CO03"},
        // AF.Function_ID
        Package (2) {"mipi-sdca-control-selector-0x7-subproperties", "CO04"},
        // AF.Function_Version
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "CO05"},
        // AF.Extension_ID
        Package (2) {"mipi-sdca-control-selector-0x9-subproperties", "CO06"},
        // AF.Extension_Version
        Package (2) {"mipi-sdca-control-selector-0xa-subproperties", "CO07"},
        // AF.Function_Status
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO08"},
        // AF.Function_Action
        Package (2) {"mipi-sdca-control-selector-0x11-subproperties", "CO09"},
        // AF.Matching_GUID
        Package (2) {"mipi-sdca-control-selector-0x12-subproperties", "CO0A"},
        // AF.Device_Manufacturer_ID
        Package (2) {"mipi-sdca-control-selector-0x2c-subproperties", "CO0B"},
        // AF.Device_Part_ID
        Package (2) {"mipi-sdca-control-selector-0x2d-subproperties", "CO0C"},
        // AF.Device_Version
        Package (2) {"mipi-sdca-control-selector-0x2e-subproperties", "CO0D"},
        // AF.Device_SDCA_Version
        Package (2) {"mipi-sdca-control-selector-0x2f-subproperties", "CO0E"},
        // AF.Chickenbits
        Package (2) {"mipi-sdca-control-selector-0x3f-subproperties", "CO0F"},
        Package (2) {"mipi-sdca-entity-id-0x1-subproperties", "EN01"},
        Package (2) {"mipi-sdca-entity-id-0x2-subproperties", "EN02"},
        Package (2) {"mipi-sdca-entity-id-0x3-subproperties", "EN03"},
        Package (2) {"mipi-sdca-entity-id-0x4-subproperties", "EN04"},
        Package (2) {"mipi-sdca-entity-id-0x5-subproperties", "EN05"},
        Package (2) {"mipi-sdca-entity-id-0x6-subproperties", "EN06"},
        Package (2) {"mipi-sdca-entity-id-0x7-subproperties", "EN07"},
        Package (2) {"mipi-sdca-entity-id-0x11-subproperties", "EN11"},
        Package (2) {"mipi-sdca-entity-id-0x12-subproperties", "EN12"},
        Package (2) {"mipi-sdca-entity-id-0x13-subproperties", "EN13"},
        Package (2) {"mipi-sdca-entity-id-0x14-subproperties", "EN14"},
        Package (2) {"mipi-sdca-entity-id-0x15-subproperties", "EN15"},
        Package (2) {"mipi-sdca-entity-id-0x16-subproperties", "EN16"},
        Package (2) {"mipi-sdca-entity-id-0x17-subproperties", "EN17"},
        Package (2) {"mipi-sdca-entity-id-0x21-subproperties", "EN21"},
        Package (2) {"mipi-sdca-entity-id-0x22-subproperties", "EN22"},
        Package (2) {"mipi-sdca-entity-id-0x23-subproperties", "EN23"},
        Package (2) {"mipi-sdca-entity-id-0x24-subproperties", "EN24"},
        Package (2) {"mipi-sdca-entity-id-0x25-subproperties", "EN25"},
        Package (2) {"mipi-sdca-entity-id-0x26-subproperties", "EN26"},
        Package (2) {"mipi-sdca-entity-id-0x27-subproperties", "EN27"},
        Package (2) {"mipi-sdca-entity-id-0x31-subproperties", "EN31"},
        Package (2) {"mipi-sdca-entity-id-0x32-subproperties", "EN32"},
        Package (2) {"mipi-sdca-entity-id-0x33-subproperties", "EN33"},
        Package (2) {"mipi-sdca-entity-id-0x34-subproperties", "EN34"},
        Package (2) {"mipi-sdca-entity-id-0x35-subproperties", "EN35"},
        Package (2) {"mipi-sdca-entity-id-0x36-subproperties", "EN36"},
        Package (2) {"mipi-sdca-entity-id-0x37-subproperties", "EN37"},
        Package (2) {"mipi-sdca-entity-id-0x72-subproperties", "EN72"},
        Package (2) {"mipi-sdca-entity-id-0x73-subproperties", "EN73"},
        Package (2) {"mipi-sdca-entity-id-0x71-subproperties", "EN71"},
        Package (2) {"mipi-sdca-cluster-id-0x0-subproperties", "CL00"},
        Package (2) {"mipi-sdca-cluster-id-0x1-subproperties", "CL01"},
        Package (2) {"mipi-sdca-cluster-id-0x2-subproperties", "CL02"},
        Package (2) {"mipi-sdca-cluster-id-0x3-subproperties", "CL03"},
        Package (2) {"mipi-sdca-cluster-id-0x4-subproperties", "CL04"},
        Package (2) {"mipi-sdca-cluster-id-0x5-subproperties", "CL05"},
        Package (2) {"mipi-sdca-cluster-id-0x6-subproperties", "CL06"},
        Package (2) {"mipi-sdca-cluster-id-0x7-subproperties", "CL07"},
        Package (2) {"mipi-sdca-system-posture-0x7-subproperties", "PO07"},
        Package (2) {"mipi-sdca-system-posture-0x9-subproperties", "PO09"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    // Cluster definition for CL00
    Name (CL00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-channel-count", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    // Cluster definition for CL01
    Name (CL01, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-channel-count", 2},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-channel-1-subproperties", "CC01"},
        Package (2) {"mipi-sdca-channel-2-subproperties", "CC02"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC01, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC02, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    // Cluster definition for CL02
    Name (CL02, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-channel-count", 3},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-channel-1-subproperties", "CC03"},
        Package (2) {"mipi-sdca-channel-2-subproperties", "CC04"},
        Package (2) {"mipi-sdca-channel-3-subproperties", "CC05"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC03, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC04, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC05, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    // Cluster definition for CL03
    Name (CL03, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-channel-count", 4},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-channel-1-subproperties", "CC06"},
        Package (2) {"mipi-sdca-channel-2-subproperties", "CC07"},
        Package (2) {"mipi-sdca-channel-3-subproperties", "CC08"},
        Package (2) {"mipi-sdca-channel-4-subproperties", "CC09"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC06, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC07, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC08, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC09, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    // Cluster definition for CL04
    Name (CL04, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-channel-count", 5},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-channel-1-subproperties", "CC0A"},
        Package (2) {"mipi-sdca-channel-2-subproperties", "CC0B"},
        Package (2) {"mipi-sdca-channel-3-subproperties", "CC0C"},
        Package (2) {"mipi-sdca-channel-4-subproperties", "CC0D"},
        Package (2) {"mipi-sdca-channel-5-subproperties", "CC0E"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC0A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC0B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC0C, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC0D, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC0E, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    // Cluster definition for CL05
    Name (CL05, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-channel-count", 6},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-channel-1-subproperties", "CC0F"},
        Package (2) {"mipi-sdca-channel-2-subproperties", "CC10"},
        Package (2) {"mipi-sdca-channel-3-subproperties", "CC11"},
        Package (2) {"mipi-sdca-channel-4-subproperties", "CC12"},
        Package (2) {"mipi-sdca-channel-5-subproperties", "CC13"},
        Package (2) {"mipi-sdca-channel-6-subproperties", "CC14"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC0F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC10, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC11, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC12, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC13, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC14, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    // Cluster definition for CL06
    Name (CL06, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-channel-count", 7},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-channel-1-subproperties", "CC15"},
        Package (2) {"mipi-sdca-channel-2-subproperties", "CC16"},
        Package (2) {"mipi-sdca-channel-3-subproperties", "CC17"},
        Package (2) {"mipi-sdca-channel-4-subproperties", "CC18"},
        Package (2) {"mipi-sdca-channel-5-subproperties", "CC19"},
        Package (2) {"mipi-sdca-channel-6-subproperties", "CC1A"},
        Package (2) {"mipi-sdca-channel-7-subproperties", "CC1B"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC15, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC16, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC17, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC18, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC19, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC1A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC1B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    // Cluster definition for CL07
    Name (CL07, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-channel-count", 8},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-channel-1-subproperties", "CC1C"},
        Package (2) {"mipi-sdca-channel-2-subproperties", "CC1D"},
        Package (2) {"mipi-sdca-channel-3-subproperties", "CC1E"},
        Package (2) {"mipi-sdca-channel-4-subproperties", "CC1F"},
        Package (2) {"mipi-sdca-channel-5-subproperties", "CC20"},
        Package (2) {"mipi-sdca-channel-6-subproperties", "CC21"},
        Package (2) {"mipi-sdca-channel-7-subproperties", "CC22"},
        Package (2) {"mipi-sdca-channel-8-subproperties", "CC23"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC1C, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC1D, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC1E, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC1F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC20, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC21, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC22, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    Name (CC23, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-cluster-channel-id", 0},
        Package (2) {"mipi-sdca-cluster-channel-purpose", 0},
        Package (2) {"mipi-sdca-cluster-channel-relationship", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    // Posture definition for PO07
    Name (PO07, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // static (will not change cluster due to posture changes)
        Package (2) {"mipi-sdca-posture-cluster-index-control", 2},
        Package (2) {"mipi-sdca-posture-volume-control", "(1,logical left), (2,logical right), (3,logical surr left), (4,logical surr right)"},
        Package (2) {"mipi-sdca-posture-mute-control", "(3,mute all), (4,attenuate)"},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    // Posture definition for PO09
    Name (PO09, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // static (will not change cluster due to posture changes)
        Package (2) {"mipi-sdca-posture-cluster-index-control", 3},
        Package (2) {"mipi-sdca-posture-volume-control", "(1,logical left), (2,logical right)"},
        Package (2) {"mipi-sdca-posture-mute-control", "(8,mute all), (7,attenuate)"},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN01 Entity: Entity_CS_1
    Name (EN01, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 11},
        Package (2) {"mipi-sdca-entity-label", "1"},
        // Default Controls for CS
        // CS Type: 2 - internal synchronizable
        Package (2) {"mipi-sdca-cs-type", "2"},
        // CS Clock Valid Max Delay: 0 - always valid
        Package (2) {"mipi-sdca-cs-clock-valid-max-delay", "0"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x2, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // CS.Clock_Valid
        Package (2) {"mipi-sdca-control-selector-0x2-subproperties", "CO10"},
        // CS.Sample_Rate_Index
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO11"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN02 Entity: Entity_IT_1
    Name (EN02, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 2},
        Package (2) {"mipi-sdca-entity-label", "1"},
        // Default Controls for IT
        Package (2) {"mipi-sdca-terminal-type", 0x101},
        Package (2) {"mipi-sdca-terminal-dp-numbers", Buffer() {0, 1}},
        Package (2) {"mipi-sdca-clock-origin", 1},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x8, 0x10, 0x11, 0x12, 0x3f}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // IT.Usage
        Package (2) {"mipi-sdca-control-selector-0x4-subproperties", "CO12"},
        // IT.Latency
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "CO13"},
        // IT.ClusterIndex
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO14"},
        // IT.DataPortIndex
        Package (2) {"mipi-sdca-control-selector-0x11-subproperties", "CO15"},
        // IT.Matching_GUID
        Package (2) {"mipi-sdca-control-selector-0x12-subproperties", "CO16"},
        // IT.Chickenbits
        Package (2) {"mipi-sdca-control-selector-0x3f-subproperties", "CO17"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN03 Entity: Entity_XU_1
    Name (EN03, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 10},
        Package (2) {"mipi-sdca-entity-label", "1"},
        // Default Controls for XU
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.IT1 id=2 EN02
        Package (2) {"mipi-sdca-input-pin-0", "EN02"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x6, 0x7, 0x8, 0x30, 0x31, 0x32}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // XU.Bypass
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO18"},
        // XU.Latency
        Package (2) {"mipi-sdca-control-selector-0x6-subproperties", "CO19"},
        // XU.XU_ID
        Package (2) {"mipi-sdca-control-selector-0x7-subproperties", "CO1A"},
        // XU.XU_Version
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "CO1B"},
        // XU.GfxMailbox
        Package (2) {"mipi-sdca-control-selector-0x30-subproperties", "CO1C"},
        // XU.AudMailbox
        Package (2) {"mipi-sdca-control-selector-0x31-subproperties", "CO1D"},
        // XU.AudMailboxStatus
        Package (2) {"mipi-sdca-control-selector-0x32-subproperties", "CO1E"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN04 Entity: Entity_FU_1
    Name (EN04, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 7},
        Package (2) {"mipi-sdca-entity-label", "1"},
        // Default Controls for FU
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.XU1 id=3 EN03
        Package (2) {"mipi-sdca-input-pin-0", "EN03"},
        // Channel Volume is MBQ: 2B
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // FU.Mute
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO1F"},
        // FU.Latency
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO20"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN05 Entity: Entity_OT_1
    Name (EN05, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 3},
        Package (2) {"mipi-sdca-entity-label", "1"},
        Package (2) {"mipi-sdca-terminal-type", 0x0},
        Package (2) {"mipi-sdca-terminal-dp-numbers", Buffer() {0, 0}},
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.FU1 id=4 EN04
        Package (2) {"mipi-sdca-input-pin-0", "EN04"},
        Package (2) {"mipi-sdca-clock-origin", 0},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x8, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3f}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // OT.Usage
        Package (2) {"mipi-sdca-control-selector-0x4-subproperties", "CO21"},
        // OT.Latency
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "CO22"},
        // OT.Matching_GUID
        Package (2) {"mipi-sdca-control-selector-0x12-subproperties", "CO23"},
        // OT.KeepAlive
        Package (2) {"mipi-sdca-control-selector-0x13-subproperties", "CO24"},
        // OT.NDAI_Stream
        Package (2) {"mipi-sdca-control-selector-0x14-subproperties", "CO25"},
        // OT.NDAI_Category
        Package (2) {"mipi-sdca-control-selector-0x15-subproperties", "CO26"},
        // OT.NDAI_CodingType
        Package (2) {"mipi-sdca-control-selector-0x16-subproperties", "CO27"},
        // OT.NDAI_PacketType
        Package (2) {"mipi-sdca-control-selector-0x17-subproperties", "CO28"},
        // OT.ChannelMap0
        Package (2) {"mipi-sdca-control-selector-0x30-subproperties", "CO29"},
        // OT.ChannelMap1
        Package (2) {"mipi-sdca-control-selector-0x31-subproperties", "CO2A"},
        // OT.ChannelMap2
        Package (2) {"mipi-sdca-control-selector-0x32-subproperties", "CO2B"},
        // OT.ChannelMap3
        Package (2) {"mipi-sdca-control-selector-0x33-subproperties", "CO2C"},
        // OT.ChannelMap4
        Package (2) {"mipi-sdca-control-selector-0x34-subproperties", "CO2D"},
        // OT.ChannelMap5
        Package (2) {"mipi-sdca-control-selector-0x35-subproperties", "CO2E"},
        // OT.ChannelMap6
        Package (2) {"mipi-sdca-control-selector-0x36-subproperties", "CO2F"},
        // OT.ChannelMap7
        Package (2) {"mipi-sdca-control-selector-0x37-subproperties", "CO30"},
        // OT.ChannelLowest
        Package (2) {"mipi-sdca-control-selector-0x38-subproperties", "CO31"},
        // OT.JackConnectorId
        Package (2) {"mipi-sdca-control-selector-0x39-subproperties", "CO32"},
        // OT.ToneControlCoeff
        Package (2) {"mipi-sdca-control-selector-0x3a-subproperties", "CO33"},
        // OT.ToneControlInitVal
        Package (2) {"mipi-sdca-control-selector-0x3b-subproperties", "CO34"},
        // OT.ToneControlVolShift
        Package (2) {"mipi-sdca-control-selector-0x3c-subproperties", "CO35"},
        // OT.ToneControlMisc
        Package (2) {"mipi-sdca-control-selector-0x3d-subproperties", "CO36"},
        // OT.Chickenbits
        Package (2) {"mipi-sdca-control-selector-0x3f-subproperties", "CO37"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN06 Entity: Entity_PDE_1
    Name (EN06, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 17},
        Package (2) {"mipi-sdca-entity-label", "1"},
        // Default Controls for PDE
        // 0x11 - should at least support PS3 and PS0
        Package (2) {"mipi-sdca-powerdomain-state-list", 0x19},
        Package (2) {"mipi-sdca-powerdomain-transition-typical-delay", Package() { 3, 0, 450, 3, 1, 250}},
        Package (2) {"mipi-sdca-powerdomain-transition-max-delay", Package() { 3, 0, 500, 3, 1, 300}},
        // Managed list: EN05 links to {EntityType.OT1}}
        Package (2) {"mipi-sdca-powerdomain-managed-list", "EN05"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // PDE.Requested_PS
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO38"},
        // PDE.Actual_PS
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO39"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN07 Entity: Entity_GE_1
    Name (EN07, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 18},
        Package (2) {"mipi-sdca-entity-label", "1"},
        // Default Controls for GE
        // Controls affected: EN05 links to {EntityType.OT1}}. Need to convert Entity to Control Selector
        Package (2) {"mipi-sdca-geselectedmode-controls-affected", "EN05"},
        Package (2) {"mipi-sdca-ge-default-selectedmode", 0},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // GE.Selected_Mode
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO3A"},
        // GE.Detected_Mode
        Package (2) {"mipi-sdca-control-selector-0x2-subproperties", "CO3B"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN11 Entity: Entity_CS_2
    Name (EN11, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 11},
        Package (2) {"mipi-sdca-entity-label", "2"},
        // Default Controls for CS
        // CS Type: 2 - internal synchronizable
        Package (2) {"mipi-sdca-cs-type", "2"},
        // CS Clock Valid Max Delay: 0 - always valid
        Package (2) {"mipi-sdca-cs-clock-valid-max-delay", "0"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x2, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // CS.Clock_Valid
        Package (2) {"mipi-sdca-control-selector-0x2-subproperties", "CO3C"},
        // CS.Sample_Rate_Index
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO3D"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN12 Entity: Entity_IT_2
    Name (EN12, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 2},
        Package (2) {"mipi-sdca-entity-label", "2"},
        // Default Controls for IT
        Package (2) {"mipi-sdca-terminal-type", 0x101},
        Package (2) {"mipi-sdca-terminal-dp-numbers", Buffer() {0, 3}},
        Package (2) {"mipi-sdca-clock-origin", 2},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x8, 0x10, 0x11, 0x12, 0x3f}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // IT.Usage
        Package (2) {"mipi-sdca-control-selector-0x4-subproperties", "CO3E"},
        // IT.Latency
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "CO3F"},
        // IT.ClusterIndex
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO40"},
        // IT.DataPortIndex
        Package (2) {"mipi-sdca-control-selector-0x11-subproperties", "CO41"},
        // IT.Matching_GUID
        Package (2) {"mipi-sdca-control-selector-0x12-subproperties", "CO42"},
        // IT.Chickenbits
        Package (2) {"mipi-sdca-control-selector-0x3f-subproperties", "CO43"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN13 Entity: Entity_XU_2
    Name (EN13, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 10},
        Package (2) {"mipi-sdca-entity-label", "2"},
        // Default Controls for XU
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.IT2 id=18 EN12
        Package (2) {"mipi-sdca-input-pin-0", "EN12"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x6, 0x7, 0x8, 0x30, 0x31, 0x32}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // XU.Bypass
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO44"},
        // XU.Latency
        Package (2) {"mipi-sdca-control-selector-0x6-subproperties", "CO45"},
        // XU.XU_ID
        Package (2) {"mipi-sdca-control-selector-0x7-subproperties", "CO46"},
        // XU.XU_Version
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "CO47"},
        // XU.GfxMailbox
        Package (2) {"mipi-sdca-control-selector-0x30-subproperties", "CO48"},
        // XU.AudMailbox
        Package (2) {"mipi-sdca-control-selector-0x31-subproperties", "CO49"},
        // XU.AudMailboxStatus
        Package (2) {"mipi-sdca-control-selector-0x32-subproperties", "CO4A"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN14 Entity: Entity_FU_2
    Name (EN14, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 7},
        Package (2) {"mipi-sdca-entity-label", "2"},
        // Default Controls for FU
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.XU2 id=19 EN13
        Package (2) {"mipi-sdca-input-pin-0", "EN13"},
        // Channel Volume is MBQ: 2B
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // FU.Mute
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO4B"},
        // FU.Latency
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO4C"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN15 Entity: Entity_OT_2
    Name (EN15, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 3},
        Package (2) {"mipi-sdca-entity-label", "2"},
        Package (2) {"mipi-sdca-terminal-type", 0x0},
        Package (2) {"mipi-sdca-terminal-dp-numbers", Buffer() {0, 0}},
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.FU2 id=20 EN14
        Package (2) {"mipi-sdca-input-pin-0", "EN14"},
        Package (2) {"mipi-sdca-clock-origin", 0},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x8, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3f}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // OT.Usage
        Package (2) {"mipi-sdca-control-selector-0x4-subproperties", "CO4D"},
        // OT.Latency
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "CO4E"},
        // OT.Matching_GUID
        Package (2) {"mipi-sdca-control-selector-0x12-subproperties", "CO4F"},
        // OT.KeepAlive
        Package (2) {"mipi-sdca-control-selector-0x13-subproperties", "CO50"},
        // OT.NDAI_Stream
        Package (2) {"mipi-sdca-control-selector-0x14-subproperties", "CO51"},
        // OT.NDAI_Category
        Package (2) {"mipi-sdca-control-selector-0x15-subproperties", "CO52"},
        // OT.NDAI_CodingType
        Package (2) {"mipi-sdca-control-selector-0x16-subproperties", "CO53"},
        // OT.NDAI_PacketType
        Package (2) {"mipi-sdca-control-selector-0x17-subproperties", "CO54"},
        // OT.ChannelMap0
        Package (2) {"mipi-sdca-control-selector-0x30-subproperties", "CO55"},
        // OT.ChannelMap1
        Package (2) {"mipi-sdca-control-selector-0x31-subproperties", "CO56"},
        // OT.ChannelMap2
        Package (2) {"mipi-sdca-control-selector-0x32-subproperties", "CO57"},
        // OT.ChannelMap3
        Package (2) {"mipi-sdca-control-selector-0x33-subproperties", "CO58"},
        // OT.ChannelMap4
        Package (2) {"mipi-sdca-control-selector-0x34-subproperties", "CO59"},
        // OT.ChannelMap5
        Package (2) {"mipi-sdca-control-selector-0x35-subproperties", "CO5A"},
        // OT.ChannelMap6
        Package (2) {"mipi-sdca-control-selector-0x36-subproperties", "CO5B"},
        // OT.ChannelMap7
        Package (2) {"mipi-sdca-control-selector-0x37-subproperties", "CO5C"},
        // OT.ChannelLowest
        Package (2) {"mipi-sdca-control-selector-0x38-subproperties", "CO5D"},
        // OT.JackConnectorId
        Package (2) {"mipi-sdca-control-selector-0x39-subproperties", "CO5E"},
        // OT.ToneControlCoeff
        Package (2) {"mipi-sdca-control-selector-0x3a-subproperties", "CO5F"},
        // OT.ToneControlInitVal
        Package (2) {"mipi-sdca-control-selector-0x3b-subproperties", "CO60"},
        // OT.ToneControlVolShift
        Package (2) {"mipi-sdca-control-selector-0x3c-subproperties", "CO61"},
        // OT.ToneControlMisc
        Package (2) {"mipi-sdca-control-selector-0x3d-subproperties", "CO62"},
        // OT.Chickenbits
        Package (2) {"mipi-sdca-control-selector-0x3f-subproperties", "CO63"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN16 Entity: Entity_PDE_2
    Name (EN16, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 17},
        Package (2) {"mipi-sdca-entity-label", "2"},
        // Default Controls for PDE
        // 0x11 - should at least support PS3 and PS0
        Package (2) {"mipi-sdca-powerdomain-state-list", 0x19},
        Package (2) {"mipi-sdca-powerdomain-transition-typical-delay", Package() { 3, 0, 450, 3, 1, 250}},
        Package (2) {"mipi-sdca-powerdomain-transition-max-delay", Package() { 3, 0, 500, 3, 1, 300}},
        // Managed list: EN15 links to {EntityType.OT2}}
        Package (2) {"mipi-sdca-powerdomain-managed-list", "EN15"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // PDE.Requested_PS
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO64"},
        // PDE.Actual_PS
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO65"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN17 Entity: Entity_GE_2
    Name (EN17, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 18},
        Package (2) {"mipi-sdca-entity-label", "2"},
        // Default Controls for GE
        // Controls affected: EN15 links to {EntityType.OT2}}. Need to convert Entity to Control Selector
        Package (2) {"mipi-sdca-geselectedmode-controls-affected", "EN15"},
        Package (2) {"mipi-sdca-ge-default-selectedmode", 0},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // GE.Selected_Mode
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO66"},
        // GE.Detected_Mode
        Package (2) {"mipi-sdca-control-selector-0x2-subproperties", "CO67"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN21 Entity: Entity_CS_3
    Name (EN21, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 11},
        Package (2) {"mipi-sdca-entity-label", "3"},
        // Default Controls for CS
        // CS Type: 2 - internal synchronizable
        Package (2) {"mipi-sdca-cs-type", "2"},
        // CS Clock Valid Max Delay: 0 - always valid
        Package (2) {"mipi-sdca-cs-clock-valid-max-delay", "0"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x2, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // CS.Clock_Valid
        Package (2) {"mipi-sdca-control-selector-0x2-subproperties", "CO68"},
        // CS.Sample_Rate_Index
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO69"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN22 Entity: Entity_IT_3
    Name (EN22, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 2},
        Package (2) {"mipi-sdca-entity-label", "3"},
        // Default Controls for IT
        Package (2) {"mipi-sdca-terminal-type", 0x101},
        Package (2) {"mipi-sdca-terminal-dp-numbers", Buffer() {0, 5}},
        Package (2) {"mipi-sdca-clock-origin", 3},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x8, 0x10, 0x11, 0x12, 0x3f}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // IT.Usage
        Package (2) {"mipi-sdca-control-selector-0x4-subproperties", "CO6A"},
        // IT.Latency
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "CO6B"},
        // IT.ClusterIndex
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO6C"},
        // IT.DataPortIndex
        Package (2) {"mipi-sdca-control-selector-0x11-subproperties", "CO6D"},
        // IT.Matching_GUID
        Package (2) {"mipi-sdca-control-selector-0x12-subproperties", "CO6E"},
        // IT.Chickenbits
        Package (2) {"mipi-sdca-control-selector-0x3f-subproperties", "CO6F"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN23 Entity: Entity_XU_3
    Name (EN23, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 10},
        Package (2) {"mipi-sdca-entity-label", "3"},
        // Default Controls for XU
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.IT3 id=34 EN22
        Package (2) {"mipi-sdca-input-pin-0", "EN22"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x6, 0x7, 0x8, 0x30, 0x31, 0x32}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // XU.Bypass
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO70"},
        // XU.Latency
        Package (2) {"mipi-sdca-control-selector-0x6-subproperties", "CO71"},
        // XU.XU_ID
        Package (2) {"mipi-sdca-control-selector-0x7-subproperties", "CO72"},
        // XU.XU_Version
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "CO73"},
        // XU.GfxMailbox
        Package (2) {"mipi-sdca-control-selector-0x30-subproperties", "CO74"},
        // XU.AudMailbox
        Package (2) {"mipi-sdca-control-selector-0x31-subproperties", "CO75"},
        // XU.AudMailboxStatus
        Package (2) {"mipi-sdca-control-selector-0x32-subproperties", "CO76"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN24 Entity: Entity_FU_3
    Name (EN24, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 7},
        Package (2) {"mipi-sdca-entity-label", "3"},
        // Default Controls for FU
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.XU3 id=35 EN23
        Package (2) {"mipi-sdca-input-pin-0", "EN23"},
        // Channel Volume is MBQ: 2B
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // FU.Mute
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO77"},
        // FU.Latency
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO78"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN25 Entity: Entity_OT_3
    Name (EN25, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 3},
        Package (2) {"mipi-sdca-entity-label", "3"},
        Package (2) {"mipi-sdca-terminal-type", 0x0},
        Package (2) {"mipi-sdca-terminal-dp-numbers", Buffer() {0, 0}},
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.FU3 id=36 EN24
        Package (2) {"mipi-sdca-input-pin-0", "EN24"},
        Package (2) {"mipi-sdca-clock-origin", 0},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x8, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3f}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // OT.Usage
        Package (2) {"mipi-sdca-control-selector-0x4-subproperties", "CO79"},
        // OT.Latency
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "CO7A"},
        // OT.Matching_GUID
        Package (2) {"mipi-sdca-control-selector-0x12-subproperties", "CO7B"},
        // OT.KeepAlive
        Package (2) {"mipi-sdca-control-selector-0x13-subproperties", "CO7C"},
        // OT.NDAI_Stream
        Package (2) {"mipi-sdca-control-selector-0x14-subproperties", "CO7D"},
        // OT.NDAI_Category
        Package (2) {"mipi-sdca-control-selector-0x15-subproperties", "CO7E"},
        // OT.NDAI_CodingType
        Package (2) {"mipi-sdca-control-selector-0x16-subproperties", "CO7F"},
        // OT.NDAI_PacketType
        Package (2) {"mipi-sdca-control-selector-0x17-subproperties", "CO80"},
        // OT.ChannelMap0
        Package (2) {"mipi-sdca-control-selector-0x30-subproperties", "CO81"},
        // OT.ChannelMap1
        Package (2) {"mipi-sdca-control-selector-0x31-subproperties", "CO82"},
        // OT.ChannelMap2
        Package (2) {"mipi-sdca-control-selector-0x32-subproperties", "CO83"},
        // OT.ChannelMap3
        Package (2) {"mipi-sdca-control-selector-0x33-subproperties", "CO84"},
        // OT.ChannelMap4
        Package (2) {"mipi-sdca-control-selector-0x34-subproperties", "CO85"},
        // OT.ChannelMap5
        Package (2) {"mipi-sdca-control-selector-0x35-subproperties", "CO86"},
        // OT.ChannelMap6
        Package (2) {"mipi-sdca-control-selector-0x36-subproperties", "CO87"},
        // OT.ChannelMap7
        Package (2) {"mipi-sdca-control-selector-0x37-subproperties", "CO88"},
        // OT.ChannelLowest
        Package (2) {"mipi-sdca-control-selector-0x38-subproperties", "CO89"},
        // OT.JackConnectorId
        Package (2) {"mipi-sdca-control-selector-0x39-subproperties", "CO8A"},
        // OT.ToneControlCoeff
        Package (2) {"mipi-sdca-control-selector-0x3a-subproperties", "CO8B"},
        // OT.ToneControlInitVal
        Package (2) {"mipi-sdca-control-selector-0x3b-subproperties", "CO8C"},
        // OT.ToneControlVolShift
        Package (2) {"mipi-sdca-control-selector-0x3c-subproperties", "CO8D"},
        // OT.ToneControlMisc
        Package (2) {"mipi-sdca-control-selector-0x3d-subproperties", "CO8E"},
        // OT.Chickenbits
        Package (2) {"mipi-sdca-control-selector-0x3f-subproperties", "CO8F"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN26 Entity: Entity_PDE_3
    Name (EN26, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 17},
        Package (2) {"mipi-sdca-entity-label", "3"},
        // Default Controls for PDE
        // 0x11 - should at least support PS3 and PS0
        Package (2) {"mipi-sdca-powerdomain-state-list", 0x19},
        Package (2) {"mipi-sdca-powerdomain-transition-typical-delay", Package() { 3, 0, 450, 3, 1, 250}},
        Package (2) {"mipi-sdca-powerdomain-transition-max-delay", Package() { 3, 0, 500, 3, 1, 300}},
        // Managed list: EN25 links to {EntityType.OT3}}
        Package (2) {"mipi-sdca-powerdomain-managed-list", "EN25"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // PDE.Requested_PS
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO90"},
        // PDE.Actual_PS
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO91"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN27 Entity: Entity_GE_3
    Name (EN27, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 18},
        Package (2) {"mipi-sdca-entity-label", "3"},
        // Default Controls for GE
        // Controls affected: EN25 links to {EntityType.OT3}}. Need to convert Entity to Control Selector
        Package (2) {"mipi-sdca-geselectedmode-controls-affected", "EN25"},
        Package (2) {"mipi-sdca-ge-default-selectedmode", 0},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // GE.Selected_Mode
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO92"},
        // GE.Detected_Mode
        Package (2) {"mipi-sdca-control-selector-0x2-subproperties", "CO93"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN31 Entity: Entity_CS_4
    Name (EN31, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 11},
        Package (2) {"mipi-sdca-entity-label", "4"},
        // Default Controls for CS
        // CS Type: 2 - internal synchronizable
        Package (2) {"mipi-sdca-cs-type", "2"},
        // CS Clock Valid Max Delay: 0 - always valid
        Package (2) {"mipi-sdca-cs-clock-valid-max-delay", "0"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x2, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // CS.Clock_Valid
        Package (2) {"mipi-sdca-control-selector-0x2-subproperties", "CO94"},
        // CS.Sample_Rate_Index
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO95"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN32 Entity: Entity_IT_4
    Name (EN32, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 2},
        Package (2) {"mipi-sdca-entity-label", "4"},
        // Default Controls for IT
        Package (2) {"mipi-sdca-terminal-type", 0x101},
        Package (2) {"mipi-sdca-terminal-dp-numbers", Buffer() {0, 7}},
        Package (2) {"mipi-sdca-clock-origin", 4},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x8, 0x10, 0x11, 0x12, 0x3f}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // IT.Usage
        Package (2) {"mipi-sdca-control-selector-0x4-subproperties", "CO96"},
        // IT.Latency
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "CO97"},
        // IT.ClusterIndex
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "CO98"},
        // IT.DataPortIndex
        Package (2) {"mipi-sdca-control-selector-0x11-subproperties", "CO99"},
        // IT.Matching_GUID
        Package (2) {"mipi-sdca-control-selector-0x12-subproperties", "CO9A"},
        // IT.Chickenbits
        Package (2) {"mipi-sdca-control-selector-0x3f-subproperties", "CO9B"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN33 Entity: Entity_XU_4
    Name (EN33, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 10},
        Package (2) {"mipi-sdca-entity-label", "4"},
        // Default Controls for XU
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.IT4 id=50 EN32
        Package (2) {"mipi-sdca-input-pin-0", "EN32"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x6, 0x7, 0x8, 0x30, 0x31, 0x32}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // XU.Bypass
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "CO9C"},
        // XU.Latency
        Package (2) {"mipi-sdca-control-selector-0x6-subproperties", "CO9D"},
        // XU.XU_ID
        Package (2) {"mipi-sdca-control-selector-0x7-subproperties", "CO9E"},
        // XU.XU_Version
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "CO9F"},
        // XU.GfxMailbox
        Package (2) {"mipi-sdca-control-selector-0x30-subproperties", "COA0"},
        // XU.AudMailbox
        Package (2) {"mipi-sdca-control-selector-0x31-subproperties", "COA1"},
        // XU.AudMailboxStatus
        Package (2) {"mipi-sdca-control-selector-0x32-subproperties", "COA2"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN34 Entity: Entity_FU_4
    Name (EN34, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 7},
        Package (2) {"mipi-sdca-entity-label", "4"},
        // Default Controls for FU
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.XU4 id=51 EN33
        Package (2) {"mipi-sdca-input-pin-0", "EN33"},
        // Channel Volume is MBQ: 2B
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // FU.Mute
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "COA3"},
        // FU.Latency
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "COA4"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN35 Entity: Entity_OT_4
    Name (EN35, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 3},
        Package (2) {"mipi-sdca-entity-label", "4"},
        Package (2) {"mipi-sdca-terminal-type", 0x0},
        Package (2) {"mipi-sdca-terminal-dp-numbers", Buffer() {0, 0}},
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.FU4 id=52 EN34
        Package (2) {"mipi-sdca-input-pin-0", "EN34"},
        Package (2) {"mipi-sdca-clock-origin", 0},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x8, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3f}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // OT.Usage
        Package (2) {"mipi-sdca-control-selector-0x4-subproperties", "COA5"},
        // OT.Latency
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "COA6"},
        // OT.Matching_GUID
        Package (2) {"mipi-sdca-control-selector-0x12-subproperties", "COA7"},
        // OT.KeepAlive
        Package (2) {"mipi-sdca-control-selector-0x13-subproperties", "COA8"},
        // OT.NDAI_Stream
        Package (2) {"mipi-sdca-control-selector-0x14-subproperties", "COA9"},
        // OT.NDAI_Category
        Package (2) {"mipi-sdca-control-selector-0x15-subproperties", "COAA"},
        // OT.NDAI_CodingType
        Package (2) {"mipi-sdca-control-selector-0x16-subproperties", "COAB"},
        // OT.NDAI_PacketType
        Package (2) {"mipi-sdca-control-selector-0x17-subproperties", "COAC"},
        // OT.ChannelMap0
        Package (2) {"mipi-sdca-control-selector-0x30-subproperties", "COAD"},
        // OT.ChannelMap1
        Package (2) {"mipi-sdca-control-selector-0x31-subproperties", "COAE"},
        // OT.ChannelMap2
        Package (2) {"mipi-sdca-control-selector-0x32-subproperties", "COAF"},
        // OT.ChannelMap3
        Package (2) {"mipi-sdca-control-selector-0x33-subproperties", "COB0"},
        // OT.ChannelMap4
        Package (2) {"mipi-sdca-control-selector-0x34-subproperties", "COB1"},
        // OT.ChannelMap5
        Package (2) {"mipi-sdca-control-selector-0x35-subproperties", "COB2"},
        // OT.ChannelMap6
        Package (2) {"mipi-sdca-control-selector-0x36-subproperties", "COB3"},
        // OT.ChannelMap7
        Package (2) {"mipi-sdca-control-selector-0x37-subproperties", "COB4"},
        // OT.ChannelLowest
        Package (2) {"mipi-sdca-control-selector-0x38-subproperties", "COB5"},
        // OT.JackConnectorId
        Package (2) {"mipi-sdca-control-selector-0x39-subproperties", "COB6"},
        // OT.ToneControlCoeff
        Package (2) {"mipi-sdca-control-selector-0x3a-subproperties", "COB7"},
        // OT.ToneControlInitVal
        Package (2) {"mipi-sdca-control-selector-0x3b-subproperties", "COB8"},
        // OT.ToneControlVolShift
        Package (2) {"mipi-sdca-control-selector-0x3c-subproperties", "COB9"},
        // OT.ToneControlMisc
        Package (2) {"mipi-sdca-control-selector-0x3d-subproperties", "COBA"},
        // OT.Chickenbits
        Package (2) {"mipi-sdca-control-selector-0x3f-subproperties", "COBB"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN36 Entity: Entity_PDE_4
    Name (EN36, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 17},
        Package (2) {"mipi-sdca-entity-label", "4"},
        // Default Controls for PDE
        // 0x11 - should at least support PS3 and PS0
        Package (2) {"mipi-sdca-powerdomain-state-list", 0x19},
        Package (2) {"mipi-sdca-powerdomain-transition-typical-delay", Package() { 3, 0, 450, 3, 1, 250}},
        Package (2) {"mipi-sdca-powerdomain-transition-max-delay", Package() { 3, 0, 500, 3, 1, 300}},
        // Managed list: EN35 links to {EntityType.OT4}}
        Package (2) {"mipi-sdca-powerdomain-managed-list", "EN35"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // PDE.Requested_PS
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "COBC"},
        // PDE.Actual_PS
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "COBD"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN37 Entity: Entity_GE_4
    Name (EN37, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 18},
        Package (2) {"mipi-sdca-entity-label", "4"},
        // Default Controls for GE
        // Controls affected: EN35 links to {EntityType.OT4}}. Need to convert Entity to Control Selector
        Package (2) {"mipi-sdca-geselectedmode-controls-affected", "EN35"},
        Package (2) {"mipi-sdca-ge-default-selectedmode", 0},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x2}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // GE.Selected_Mode
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "COBE"},
        // GE.Detected_Mode
        Package (2) {"mipi-sdca-control-selector-0x2-subproperties", "COBF"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN72 Entity: Entity_UDMPU_100
    Name (EN72, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 33},
        Package (2) {"mipi-sdca-entity-label", "100"},
        // Default Controls for UDMPU
        Package (2) {"mipi-sdca-input-pin-list", 0xf},
        // EntityType.FU1 id=4 EN04
        Package (2) {"mipi-sdca-input-pin-0", "EN04"},
        // EntityType.FU2 id=20 EN14
        Package (2) {"mipi-sdca-input-pin-1", "EN14"},
        // EntityType.FU3 id=36 EN24
        Package (2) {"mipi-sdca-input-pin-2", "EN24"},
        // EntityType.FU4 id=52 EN34
        Package (2) {"mipi-sdca-input-pin-3", "EN34"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x1, 0x8, 0x10, 0x30}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // UDMPU.Bypass
        Package (2) {"mipi-sdca-control-selector-0x1-subproperties", "COC0"},
        // UDMPU.Latency
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "COC1"},
        // UDMPU.ClusterIndex
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "COC2"},
        // UDMPU.LoopbackControl
        Package (2) {"mipi-sdca-control-selector-0x30-subproperties", "COC3"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN73 Entity: Entity_OT_100
    Name (EN73, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 3},
        Package (2) {"mipi-sdca-entity-label", "100"},
        Package (2) {"mipi-sdca-terminal-type", 0x188},
        Package (2) {"mipi-sdca-terminal-dp-numbers", Buffer() {0, 13}},
        Package (2) {"mipi-sdca-input-pin-list", 0x1},
        // EntityType.UDMPU100 id=114 EN72
        Package (2) {"mipi-sdca-input-pin-0", "EN72"},
        Package (2) {"mipi-sdca-clock-origin", 100},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x4, 0x8, 0x11, 0x12, 0x3f}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // OT.Usage
        Package (2) {"mipi-sdca-control-selector-0x4-subproperties", "COC4"},
        // OT.Latency
        Package (2) {"mipi-sdca-control-selector-0x8-subproperties", "COC5"},
        // OT.DataPortIndex
        Package (2) {"mipi-sdca-control-selector-0x11-subproperties", "COC6"},
        // OT.Matching_GUID
        Package (2) {"mipi-sdca-control-selector-0x12-subproperties", "COC7"},
        // OT.Chickenbits
        Package (2) {"mipi-sdca-control-selector-0x3f-subproperties", "COC8"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// EN71 Entity: Entity_CS_100
    Name (EN71, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 11},
        Package (2) {"mipi-sdca-entity-label", "100"},
        // Default Controls for CS
        // CS Type: 2 - internal synchronizable
        Package (2) {"mipi-sdca-cs-type", "2"},
        // CS Clock Valid Max Delay: 0 - always valid
        Package (2) {"mipi-sdca-cs-clock-valid-max-delay", "0"},
        Package (2) {"mipi-sdca-control-selector-list", Package() {0x2, 0x10}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        // CS.Clock_Valid
        Package (2) {"mipi-sdca-control-selector-0x2-subproperties", "COC9"},
        // CS.Sample_Rate_Index
        Package (2) {"mipi-sdca-control-selector-0x10-subproperties", "COCA"},
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// Placeholder for Control Selectors
    //

    Name (CO00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Commit_Group_Mask)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO01, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Function_SDCA_Version)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 7},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO02, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Function_Type)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 9},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO03, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Function_Manufacturer_ID)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 261},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO04, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Function_ID)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO05, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Function_Version)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO06, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Extension_ID)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO07, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Extension_Version)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO08, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Function_Status)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-selector-interruptposition", 13},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 2},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO09, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Function_Action)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO0A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Matching_GUID)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO0B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Device_Manufacturer_ID)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 261},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO0C, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Device_Part_ID)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 53264},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO0D, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Device_Version)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 1},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO0E, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Device_SDCA_Version)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 7},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO0F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (AF.Chickenbits)(Entity Label: AF0)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO10, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (CS.Clock_Valid)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 1},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO11, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (CS.Sample_Rate_Index)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO12, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Usage)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 6},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO13, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Latency)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO14, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.ClusterIndex)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 1},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO15, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.DataPortIndex)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 6},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO16, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Matching_GUID)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO17, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Chickenbits)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO18, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.Bypass)(Entity Label: 1)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO19, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.Latency)(Entity Label: 1)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO1A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.XU_ID)(Entity Label: 1)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO1B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.XU_Version)(Entity Label: 1)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO1C, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.GfxMailbox)(Entity Label: 1)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO1D, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.AudMailbox)(Entity Label: 1)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO1E, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.AudMailboxStatus)(Entity Label: 1)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO1F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (FU.Mute)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-list", Package() {1,2,3,4}},
        Package (2) {"mipi-sdca-control-number-1-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-2-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-3-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-4-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-1-dc-value", 0},
        Package (2) {"mipi-sdca-control-number-2-dc-value", 0},
        Package (2) {"mipi-sdca-control-number-3-dc-value", 0},
        Package (2) {"mipi-sdca-control-number-4-dc-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO20, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (FU.Latency)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO21, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Usage)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO22, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Latency)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO23, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Matching_GUID)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO24, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.KeepAlive)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO25, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_Stream)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO26, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_Category)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO27, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_CodingType)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO28, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_PacketType)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO29, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap0)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO2A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap1)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO2B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap2)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO2C, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap3)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO2D, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap4)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO2E, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap5)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO2F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap6)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO30, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap7)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO31, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelLowest)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO32, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.JackConnectorId)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO33, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlCoeff)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO34, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlInitVal)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO35, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlVolShift)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO36, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlMisc)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO37, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Chickenbits)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO38, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (PDE.Requested_PS)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO39, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (PDE.Actual_PS)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO3A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (GE.Selected_Mode)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-selector-interruptposition", 16},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO3B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (GE.Detected_Mode)(Entity Label: 1)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO3C, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (CS.Clock_Valid)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 1},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO3D, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (CS.Sample_Rate_Index)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO3E, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Usage)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 6},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO3F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Latency)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO40, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.ClusterIndex)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 1},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO41, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.DataPortIndex)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 24},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO42, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Matching_GUID)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO43, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Chickenbits)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO44, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.Bypass)(Entity Label: 2)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO45, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.Latency)(Entity Label: 2)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO46, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.XU_ID)(Entity Label: 2)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO47, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.XU_Version)(Entity Label: 2)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO48, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.GfxMailbox)(Entity Label: 2)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO49, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.AudMailbox)(Entity Label: 2)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO4A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.AudMailboxStatus)(Entity Label: 2)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO4B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (FU.Mute)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-list", Package() {1,2,3,4}},
        Package (2) {"mipi-sdca-control-number-1-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-2-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-3-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-4-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-1-dc-value", 0},
        Package (2) {"mipi-sdca-control-number-2-dc-value", 0},
        Package (2) {"mipi-sdca-control-number-3-dc-value", 0},
        Package (2) {"mipi-sdca-control-number-4-dc-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO4C, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (FU.Latency)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO4D, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Usage)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO4E, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Latency)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO4F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Matching_GUID)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO50, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.KeepAlive)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO51, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_Stream)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO52, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_Category)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO53, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_CodingType)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO54, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_PacketType)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO55, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap0)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO56, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap1)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO57, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap2)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO58, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap3)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO59, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap4)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO5A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap5)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO5B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap6)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO5C, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap7)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO5D, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelLowest)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO5E, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.JackConnectorId)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO5F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlCoeff)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO60, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlInitVal)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO61, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlVolShift)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO62, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlMisc)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO63, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Chickenbits)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO64, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (PDE.Requested_PS)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO65, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (PDE.Actual_PS)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO66, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (GE.Selected_Mode)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-selector-interruptposition", 17},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO67, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (GE.Detected_Mode)(Entity Label: 2)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO68, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (CS.Clock_Valid)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 1},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO69, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (CS.Sample_Rate_Index)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO6A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Usage)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 6},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO6B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Latency)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO6C, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.ClusterIndex)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 1},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO6D, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.DataPortIndex)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 96},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO6E, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Matching_GUID)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO6F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Chickenbits)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO70, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.Bypass)(Entity Label: 3)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO71, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.Latency)(Entity Label: 3)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO72, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.XU_ID)(Entity Label: 3)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO73, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.XU_Version)(Entity Label: 3)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO74, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.GfxMailbox)(Entity Label: 3)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO75, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.AudMailbox)(Entity Label: 3)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO76, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.AudMailboxStatus)(Entity Label: 3)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO77, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (FU.Mute)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-list", Package() {1,2,3,4}},
        Package (2) {"mipi-sdca-control-number-1-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-2-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-3-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-4-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-1-dc-value", 0},
        Package (2) {"mipi-sdca-control-number-2-dc-value", 0},
        Package (2) {"mipi-sdca-control-number-3-dc-value", 0},
        Package (2) {"mipi-sdca-control-number-4-dc-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO78, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (FU.Latency)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO79, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Usage)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO7A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Latency)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO7B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Matching_GUID)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO7C, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.KeepAlive)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO7D, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_Stream)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO7E, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_Category)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO7F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_CodingType)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO80, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_PacketType)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO81, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap0)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO82, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap1)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO83, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap2)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO84, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap3)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO85, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap4)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO86, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap5)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO87, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap6)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO88, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap7)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO89, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelLowest)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO8A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.JackConnectorId)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO8B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlCoeff)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO8C, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlInitVal)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO8D, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlVolShift)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO8E, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlMisc)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO8F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Chickenbits)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO90, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (PDE.Requested_PS)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO91, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (PDE.Actual_PS)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO92, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (GE.Selected_Mode)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-selector-interruptposition", 18},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO93, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (GE.Detected_Mode)(Entity Label: 3)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO94, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (CS.Clock_Valid)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 1},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO95, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (CS.Sample_Rate_Index)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO96, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Usage)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 6},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO97, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Latency)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO98, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.ClusterIndex)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 1},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO99, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.DataPortIndex)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 384},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO9A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Matching_GUID)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO9B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (IT.Chickenbits)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO9C, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.Bypass)(Entity Label: 4)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO9D, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.Latency)(Entity Label: 4)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO9E, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.XU_ID)(Entity Label: 4)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (CO9F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.XU_Version)(Entity Label: 4)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COA0, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.GfxMailbox)(Entity Label: 4)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COA1, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.AudMailbox)(Entity Label: 4)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COA2, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (XU.AudMailboxStatus)(Entity Label: 4)
        // mipi-sdca-control-selector-access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        // mipi-sdca-control-selector-interruptposition
        // mipi-sdca-control-selector-access-mode
        // mipi-sdca-control-number-list - A Package containing one integer entry for each control number for this control selector. e.g. {0, 1, 2, 5}
        // mipi-sdca-control-number-range
        // Range formats
        // mipi-sdca-control-number--range
        // mipi-sdca-control-number-dc-value
        // mipi-sdca-control-number--dc-value
        // mipi-sdca-control-number-default-value
        // mipi-sdca-control-number--default-value
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COA3, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (FU.Mute)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-list", Package() {1,2,3,4}},
        Package (2) {"mipi-sdca-control-number-1-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-2-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-3-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-4-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-1-dc-value", 0},
        Package (2) {"mipi-sdca-control-number-2-dc-value", 0},
        Package (2) {"mipi-sdca-control-number-3-dc-value", 0},
        Package (2) {"mipi-sdca-control-number-4-dc-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COA4, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (FU.Latency)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COA5, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Usage)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COA6, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Latency)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COA7, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Matching_GUID)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COA8, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.KeepAlive)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COA9, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_Stream)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COAA, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_Category)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COAB, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_CodingType)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COAC, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.NDAI_PacketType)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COAD, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap0)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COAE, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap1)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COAF, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap2)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COB0, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap3)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COB1, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap4)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COB2, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap5)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COB3, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap6)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COB4, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelMap7)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COB5, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ChannelLowest)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COB6, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.JackConnectorId)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COB7, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlCoeff)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COB8, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlInitVal)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COB9, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlVolShift)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COBA, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.ToneControlMisc)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COBB, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Chickenbits)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COBC, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (PDE.Requested_PS)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COBD, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (PDE.Actual_PS)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COBE, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (GE.Selected_Mode)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-selector-interruptposition", 19},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COBF, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (GE.Detected_Mode)(Entity Label: 4)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COC0, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (UDMPU.Bypass)(Entity Label: 100)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COC1, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (UDMPU.Latency)(Entity Label: 100)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COC2, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (UDMPU.ClusterIndex)(Entity Label: 100)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 1},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COC3, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (UDMPU.LoopbackControl)(Entity Label: 100)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 1},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COC4, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Usage)(Entity Label: 100)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COC5, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Latency)(Entity Label: 100)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 100},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COC6, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.DataPortIndex)(Entity Label: 100)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 222},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COC7, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Matching_GUID)(Entity Label: 100)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 3},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COC8, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (OT.Chickenbits)(Entity Label: 100)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COC9, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (CS.Clock_Valid)(Entity Label: 100)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 5},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-dc-value", 1},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
    //

    Name (COCA, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // (CS.Sample_Rate_Index)(Entity Label: 100)
        // access-layer - Bit 0: User; Bit 1: Application; Bit 2: Class; Bit 3: Platform; Bit 4: Device; Bit 5: Extension
        Package (2) {"mipi-sdca-control-selector-access-layer", 0x4},
        // access-mode - 0: RW; 1: Dual (RW); 2: RW1C; 3: RO; 4: W1S; 5: DC (DisCo Constant)
        Package (2) {"mipi-sdca-control-selector-access-mode", 0},
        Package (2) {"mipi-sdca-control-number-range", Buffer() {0}},
        Package (2) {"mipi-sdca-control-number-default-value", 0},
        Package(2) {"dummy", 0}
    }, // params
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package(2) {"dummy", 0}
        } // refs
    }) // Name
// Audio Function: AF_AudioFunctionType.NDAI_6 OUT
} // AF
} // device
