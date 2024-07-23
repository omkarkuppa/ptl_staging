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

         Name(_DSD, Package()
			{	// Function Descriptor
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdw-sw-interface-revision", 0x00020001}, // v2.1
					Package (2) {"mipi-sdw-sdca-interface-revision", 0x0801}, // v0.8r01
					//Package (2) {"mipi-sdca-control-list",	Package() {0x4, 0x5, 0x6, 0x7, 0x8} }, // Function level controls
					Package (2) {"mipi-sdca-control-list",	0x000001F0 }, // Bitmap: 0x4, 0x5, 0x6, 0x7, 0x8 - Function level controls
					Package (2) {"mipi-sdca-entity-id-list",
						Package() {0x4B, 0x66, 0x67, 0x68, 0x69, 0x6C, 0x59, 0x6D, 0x58, 0x6A, 0x5C, 0x6B, 0x61,
						0x62, 0x63, 0x57, 0x49, 0x4F, 0x4A, 0x64, 0x6E, 0x75} }, // List of entities in this function
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-control-0x4-subproperties", "C004"},
					Package (2) {"mipi-sdca-control-0x5-subproperties", "C005"},
					Package (2) {"mipi-sdca-control-0x6-subproperties", "C006"},
					Package (2) {"mipi-sdca-control-0x7-subproperties", "C007"},
					Package (2) {"mipi-sdca-control-0x8-subproperties", "C008"},
					Package (2) {"mipi-sdca-entity-id-0x4B-subproperties", "E04B"},
					Package (2) {"mipi-sdca-entity-id-0x66-subproperties", "E066"},
					Package (2) {"mipi-sdca-entity-id-0x67-subproperties", "E067"},
					Package (2) {"mipi-sdca-entity-id-0x68-subproperties", "E068"},
					Package (2) {"mipi-sdca-entity-id-0x69-subproperties", "E069"},
					Package (2) {"mipi-sdca-entity-id-0x6C-subproperties", "E06C"},
					Package (2) {"mipi-sdca-entity-id-0x59-subproperties", "E059"},
					Package (2) {"mipi-sdca-entity-id-0x6D-subproperties", "E06D"},
					Package (2) {"mipi-sdca-entity-id-0x58-subproperties", "E058"},
					Package (2) {"mipi-sdca-entity-id-0x6A-subproperties", "E06A"},
					Package (2) {"mipi-sdca-entity-id-0x5C-subproperties", "E05C"},
					Package (2) {"mipi-sdca-entity-id-0x6B-subproperties", "E06B"},
					Package (2) {"mipi-sdca-entity-id-0x61-subproperties", "E061"},
					Package (2) {"mipi-sdca-entity-id-0x62-subproperties", "E062"},
					Package (2) {"mipi-sdca-entity-id-0x63-subproperties", "E063"},
					Package (2) {"mipi-sdca-entity-id-0x57-subproperties", "E057"},
					Package (2) {"mipi-sdca-entity-id-0x49-subproperties", "E049"},
					Package (2) {"mipi-sdca-entity-id-0x4F-subproperties", "E04F"},
					Package (2) {"mipi-sdca-entity-id-0x4A-subproperties", "E04A"},
					Package (2) {"mipi-sdca-entity-id-0x64-subproperties", "E064"},
					Package (2) {"mipi-sdca-entity-id-0x6E-subproperties", "E06E"},
					Package (2) {"mipi-sdca-entity-id-0x75-subproperties", "E075"},
					//Package (2) {"mipi-sdca-entity-id-0x24-subproperties", "E024"},
					//Package (2) {"mipi-sdca-entity-id-0x25-subproperties", "E025"},
					//Package (2) {"mipi-sdca-entity-id-0x26-subproperties", "E026"},
					//Package (2) {"mipi-sdca-entity-id-0x27-subproperties", "E027"},
		 			Package (2) {"mipi-sdca-function-expansion-subproperties", "EXT0"},
				},

				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package()
				{
					Package (2) {"mipi-sdca-function-initialization-table", "BUF0"},
				}
			}) //End AF01.DSD

			// By Programming guide, Application 3. Version: 0.08 (2023/07/13)
			Name(BUF0, Buffer()
			{
				0x04, 0x20, 0x00, 0x02, 0xA0, // 0x02002004, 0xA0
				0x04, 0x00, 0x00, 0x02, 0x81, // 0x02000004, 0x81

				0x38, 0x20, 0x10, 0x06, 0x55, // 0x06102038, 0x55
				0x38, 0x00, 0x10, 0x06, 0x05, // 0x06100038, 0x05

				0x39, 0x20, 0x10, 0x06, 0x50, // 0x06102039, 0x50
				0x39, 0x00, 0x10, 0x06, 0x55, // 0x06100039, 0x55

				0x3A, 0x20, 0x10, 0x06, 0x55, // 0x0610203A, 0x55
				0x3A, 0x00, 0x10, 0x06, 0x55, // 0x0610003A, 0x55

				0x3B, 0x20, 0x10, 0x06, 0x55, // 0x0610203B, 0x55
				0x3B, 0x00, 0x10, 0x06, 0x50, // 0x0610003B, 0x50

				0x3C, 0x20, 0x10, 0x06, 0x00, // 0x0610203C, 0x00
				0x3C, 0x00, 0x10, 0x06, 0x55, // 0x0610003C, 0x55

				0x3D, 0x20, 0x10, 0x06, 0x50, // 0x0610203D, 0x50
				0x3D, 0x00, 0x10, 0x06, 0x55, // 0x0610003D, 0x55

				0x3E, 0x20, 0x10, 0x06, 0x55, // 0x0610203E, 0x55
				0x3E, 0x00, 0x10, 0x06, 0x54, // 0x0610003E, 0x54

				0x47, 0x20, 0x10, 0x06, 0xA0, // 0x06102047, 0xA0
				0x47, 0x00, 0x10, 0x06, 0x09, // 0x06100047, 0x09

				0x26, 0x20, 0x10, 0x06, 0x11, // 0x06102023, 0x34
				0x26, 0x00, 0x10, 0x06, 0x12, // 0x06100023, 0x29

				0x58, 0x2F, 0x00, 0x00, 0x07, // 0x00002F58, 0x07

				0x03, 0x2F, 0x00, 0x00, 0x06, // 0x00002F03, 0x06 (PHYtiming TDZ/TZD control)
			}) //End AF01.BUF0

	//==============================================================================================

			Name(C004, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Function_SDCA_Version = 0.8, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0x8},
				}
			}) //End AF01.C004

			Name(C005, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Function_Topology = 0x7 RJ, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0x7},
				}
			}) //End AF01.C005

			Name(C006, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Function_ManufacturerId = 0x025d, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0x025d},
				}
			}) //End AF01.C006

			Name(C007, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Function_ID = 0x0722 (Device specific), Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0x0722},
				}
			}) //End AF01.C007

			Name(C008, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Function_Version = 0x1, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0x1},
				}
			}) //End AF01.C008


	//==============================================================================================

			Name(E04B, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x11},
					Package (2) {"mipi-sdca-entity-label", "PDE 134"}, // = PDE 4B in ALC722 internal spec
					//Package (2) {"mipi-sdca-powerdomain-state-list", 0xF},
					Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x66, 0x67}},
					Package (2) {"mipi-sdca-powerdomain-transition-typical-delay",
						Package()
						{
							0x3, 0x0, 384000,
							0x0, 0x3, 384000,
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
							0x3, 0x0, 500000,
							0x0, 0x3, 500000,
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
					Package (2) {"mipi-sdca-control-0x1-subproperties", "RP4B"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "AP4B"},
				}
			})

			Name(RP4B, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Requested_PS, Class, R/W
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 0},
				},
				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package ()
				{
					Package (2) {"mipi-sdca-control-range", "PDER"},
				}
			})

			Name(PDER, Buffer()
			{
				0x01, 0x00, // Range type 0x0001
				0x02, 0x00, // Count of ranges = 0x2
				0x00, 0x00, 0x00, 0x00, // PS0
				0x03, 0x00, 0x00, 0x00, // PS3
			}) //End PDER

			Name(AP4B, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Actual_PS, Class, RO
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 3},
				}
			})


	//==============================================================================================

			Name(E066, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x02},
					Package (2) {"mipi-sdca-entity-label", "IT 131"}, // = IT 66 in ALC722 internal spec
					Package (2) {"mipi-sdca-terminal-type", 0x06A0},
					Package (2) {"mipi-sdca-terminal-reference-number", 0x1},
					//Package (2) {"mipi-sdca-control-list",	Package() {0x8, 0x10}},
					Package (2) {"mipi-sdca-control-list",	0x00010100},	// Bitmap: 0x8, 0x10
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-powerdomain-entity", "E04B"},
					Package (2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "C661"},
				},

				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package ()
				{
					Package (2) {"mipi-sdca-terminal-reference-description", "TRD1"},
				}
			}) //End AF01.E066

			Name(TRD1, Buffer()
			{
				0x00, 0x01, // Version
				0x00, 0x00, 0x00, 0x00, // Color: Black
				0x02, 0x00, // Gen Location: NA(0), Rear(1), Front(2), Left(3), Right(4), Top(5), Bottom(6)
				0x00, 0x00, // Geo Location: On Primary Chassis(0), Inside Primary Chasis(1), On Seperate Chasis(2), Other(3)
			}) //End TRD1

			Name(C661, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					//cluster, class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 1},
				}
			}) //End AF01.E066.C661

	//==============================================================================================

			Name(E067, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x02},
					Package (2) {"mipi-sdca-entity-label", "IT 132"}, // = IT 67 in ALC722 internal spec
					Package (2) {"mipi-sdca-terminal-type", 0x0680},
					Package (2) {"mipi-sdca-terminal-reference-number", 0x2},
					//Package (2) {"mipi-sdca-control-list", Package() {0x8, 0x10}},
					Package (2) {"mipi-sdca-control-list", 0x00010100},	// Bitmap: 0x8, 0x10
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-powerdomain-entity", "E04B"},
					Package (2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "C671"},
				},

				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package ()
				{
					Package (2) {"mipi-sdca-terminal-reference-description", "TRD1"},
				}
			})

			Name(C671, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					//cluster, class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 1},
				}
			})

	//==============================================================================================

			Name(E068, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x07},
					Package (2) {"mipi-sdca-entity-label", "FU 51"}, // = FU 68 in ALC722 internal spec
					//Package (2) {"mipi-sdca-control-list", Package() {0xB, 0x10} }, // boost gain, Latency
					Package (2) {"mipi-sdca-control-list", 0x00010800}, // 0xB - boost gain, 0x10 - Latency
					Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-input-pin-1", "E066"},
					Package (2) {"mipi-sdca-control-0xB-subproperties", "C68B"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "LC00"}, // Latency
				}
			}) //End AF01.E00A

			Name(C68B, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// boost gain, platform, RW
					Package (2) {"mipi-sdca-control-access-layer", 8},
					Package (2) {"mipi-sdca-control-access-mode", 0},
					//Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} },
					Package (2) {"mipi-sdca-control-cn-list", 0x6 },
				}
			}) //End AF01.CA0B

	//==============================================================================================

			Name(E069, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x07},
					Package (2) {"mipi-sdca-entity-label", "FU 52"}, // = FU 69 in ALC722 internal spec
					//Package (2) {"mipi-sdca-control-list", Package() { 0xB, 0x10} }, // boost gain, Latency
					Package (2) {"mipi-sdca-control-list", 0x00010800}, // Bitmap: 0xB - boost gain, 0x10 - Latency
					Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-input-pin-1", "E067"}, //
					Package (2) {"mipi-sdca-control-0xB-subproperties", "C69B"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "LC00"}, // Latency
				}
			}) //End AF01.E00B

			Name(C69B, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// boost gain, platform, RW
					Package (2) {"mipi-sdca-control-access-layer", 8},
					Package (2) {"mipi-sdca-control-access-mode", 0},
					//Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} },
					Package (2) {"mipi-sdca-control-cn-list", 0x6 },
				}
			}) //End AF01.CB0B

	//==============================================================================================


			Name(E06C, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x06},
					Package (2) {"mipi-sdca-entity-label", "SU 53"}, // = SU 6C in ALC722 internal spec
					//Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x2} }, // selector, latency
					Package (2) {"mipi-sdca-control-list", 0x06}, // Bitmap: 0x1 - selector, 0x2 - latency
					Package (2) {"mipi-sdca-group-entity", "E059"},
					Package (2) {"mipi-sdca-input-pin-list", 0x6}, // Input Pin 1/2 connected
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-input-pin-1", "E068"}, // Input Pin 1 connected to FU51
					Package (2) {"mipi-sdca-input-pin-2", "E069"}, // Input Pin 1 connected to FU52
					Package (2) {"mipi-sdca-control-0x1-subproperties", "C6C1"},
					Package (2) {"mipi-sdca-control-0x2-subproperties", "LC00"},
				}
			})

			Name(C6C1, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// selector, Device, RO (SU controlled by GE)
					Package (2) {"mipi-sdca-control-access-layer", 0x10},
					Package (2) {"mipi-sdca-control-access-mode", 3},
					//Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} }, // channel 1 and 2
					Package (2) {"mipi-sdca-control-cn-list", 0x6 }, // channel 1 and 2
				}
			})




	//==============================================================================================

			Name(E059, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x12},
					Package (2) {"mipi-sdca-entity-label", "GE 135"},
					Package (2) {"mipi-sdca-ge-default-selectedmode", 4}, // This is Line in
					Package (2) {"msft-ge-mode-terminaltype-list",
						Package()
						{
							0x3, 0x06A0, // Mode 3 maps to Mic
							0x4, 0x0680, // Mode 4 maps to Line in
							0x5, 0x0690, // Mode 5 maps to Line out
						}
					},
					Package (2) {"msft-ge-managed-list",
						Package()
						{
							0x75, // Entity Id of Lineout stereo terminal
							0x66, // Entity Id of to Mic terminal
							0x67, // Entity Id of LineIn stereo terminal
						}
					},
					//Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x2}},  // seleted_mode, detected mode
					Package (2) {"mipi-sdca-control-list", 0x06},  // 0x1 - seleted_mode, 0x2 - detected mode
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-control-0x1-subproperties", "GE01"},
					Package (2) {"mipi-sdca-control-0x2-subproperties", "GE02"}
				},

				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package()
				{
					Package (2) {"mipi-sdca-ge-selectedmode-controls-affected", "BUF4"},
				}
			}) //End AF01.E049

			Name(BUF4, Buffer()
			{
				0x08, // Mode count

				0x00, // Mode 0, Unplugged
					0x02,  // Control count
						0x6C, 0x1, 0x0, 0x0, // entity id, control selector, control number, value
						0x64, 0x1, 0x0, 0x0,

				0x01, // Mode 1, Unknown
					0x02,  // Control count
						0x6C, 0x1, 0x0, 0x0,
						0x64, 0x1, 0x0, 0x0,

				0x02, // Mode 2, Undefined
					0x02,  // Control count
						0x6C, 0x1, 0x0, 0x0,
						0x64, 0x1, 0x0, 0x0,

				0x03, // Mode 3, Mic in
					0x02,  // Control count
						0x6C, 0x1, 0x0, 0x1,
						0x64, 0x1, 0x0, 0x0,


				0x04, // Mode 4, Line in
					0x02,  // Control count
						0x6C, 0x1, 0x0, 0x2,
						0x64, 0x1, 0x0, 0x0,

				0x05, // Mode 5, Line out
					0x02,  // Control count
						0x6C, 0x1, 0x0, 0x0,
						0x64, 0x1, 0x0, 0x1,

				0x06, // Mode 6, reserved
					0x02,  // Control count
						0x6C, 0x1, 0x0, 0x0,
						0x64, 0x1, 0x0, 0x0,

				0x07, // Mode 7, reserved
					0x02,  // Control count
						0x6C, 0x1, 0x0, 0x0,
						0x64, 0x1, 0x0, 0x0,
			})

			Name(GE01, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Class, RW
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 0}
				}
			})

			Name(GE02, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Class, RO
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-interrupt-position", 1}, // bit 1 in SDCA interrupt
					Package (2) {"mipi-sdca-control-access-mode", 3}
				}
			})

	//==============================================================================================

			Name(E06D, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x0A},
					Package (2) {"mipi-sdca-entity-label", "XU 136"}, // = XU 6D in ALC722 internal spec
					//Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x2, 0x06} }, // Bypass, ClusterIndex
					Package (2) {"mipi-sdca-control-list", 0x46}, // 0x1 - Bypass, 0x2 - ClusterIndex, 0x6 - Latency
					Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-powerdomain-entity", "E058"},
					Package (2) {"mipi-sdca-input-pin-1", "E06C"}, // Input Pin 1 connected to SU 6C
					Package (2) {"mipi-sdca-control-0x1-subproperties", "C6D1"},
					Package (2) {"mipi-sdca-control-0x2-subproperties", "C6D2"},
					Package (2) {"mipi-sdca-control-0x6-subproperties", "LC00"},
				}
			}) //End AF01.E00D

			Name(C6D1, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Bypass, Class, R/W
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 0},
				}
			}) //End AF01.CD01

			Name(C6D2, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// ClusterIndex, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 1},
				}
			}) //End AF01.CD02

	//==============================================================================================

			Name(E058, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x11},
					Package (2) {"mipi-sdca-entity-label", "PDE 136"}, // = PDE 58 in ALC722 internal spec
					//Package (2) {"mipi-sdca-powerdomain-state-list", 0xF},
					Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x6d}},
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
					Package (2) {"mipi-sdca-control-list", 0x00010002}, // Bitmap: 0x1 - Requested_PS, 0x10 - Actual_PS
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-control-0x1-subproperties", "RP58"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "AP58"},
				}
			})

			Name(RP58, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Requested_PS, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0x0},
				},
				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package ()
				{
					Package (2) {"mipi-sdca-control-range", "PDEC"},
				}
			}) //End AF05.E058.RP58


			Name(AP58, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Actual_PS, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0x0},
			}
			}) //End AF05.E058.AP58

	//==============================================================================================

			Name(E06A, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x07},
					Package (2) {"mipi-sdca-entity-label", "FU 136"}, // = FU 6A in ALC722 internal spec
					//Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x2, 0x10} }, // Mute, Volume, Latency
					Package (2) {"mipi-sdca-control-list", 0x00010006}, // Bitmap: 0x1 - Mute, 0x2 - Volume, 0x10 - Latency
					Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-input-pin-1", "E06D"}, // Input Pin 1 connected to XU 6D
					Package (2) {"mipi-sdca-control-0x1-subproperties", "C6A1"},
					Package (2) {"mipi-sdca-control-0x2-subproperties", "C6A2"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "LC00"}, // Latency
				}
			})

			Name(C6A1, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Mute, User, RW
					Package (2) {"mipi-sdca-control-access-layer", 1},
					Package (2) {"mipi-sdca-control-access-mode", 0},
					//Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} }, // channel 1 and 2
					Package (2) {"mipi-sdca-control-cn-list", 0x6 }, // channel 1 and 2
				}
			}) //End AF01.CF01

			Name(C6A2, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Volume, User, RW
					Package (2) {"mipi-sdca-control-access-layer", 1},
					Package (2) {"mipi-sdca-control-access-mode", 0},
					//Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} },
					Package (2) {"mipi-sdca-control-cn-list", 0x6 },
				},

				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package()
				{
					Package (2) {"mipi-sdca-control-range", "BUF5"},
				}
			}) //End AF01.CF02

			Name(BUF5, Buffer()
			{
				0x03, 0x00, // Range type 0x0003
				0x01, 0x00, // Count of ranges = 0x1
				0xC0, 0xEE, 0x00, 0x00, // Min = 0x0000EEC0
				0x00, 0x1E, 0x00, 0x00, // Max = 0x00001E00
				0xC0, 0x00, 0x00, 0x00, // Step = 0x000000C0
			}) //End AF02.BUF5

	//==============================================================================================

			Name(E05C, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x03},
					Package (2) {"mipi-sdca-entity-label", "OT 136"}, // = OT 5C in ALC722 internal spec
					Package (2) {"mipi-sdca-terminal-type", 0x0101},
					Package (2) {"mipi-sdca-input-pin-list", 0x2},
					//Package (2) {"mipi-sdca-control-list", Package() {0x8, 0x11}}, // Latency, DataPortIndex
					Package (2) {"mipi-sdca-control-list", 0x00020100}, // Bitmap: 0x8 - Latency, 0x11 - DataPortIndex
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-input-pin-1", "E06A"},
					Package (2) {"mipi-sdca-terminal-clock-connection", "E06B"},
					Package (2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
					Package (2) {"mipi-sdca-control-0x11-subproperties", "C5C1"},
				},

				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package()
				{
					Package(2) {"mipi-sdca-terminal-dp-numbers", "BUF6"},
				}
			})

			Name(BUF6, Buffer()
			{
				0x0, 0x4, // DP_Index, DP_Num
 			})

			Name(C5C1, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// DataPortIndex, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0},
				}
			})

	//==============================================================================================

			Name(E06B, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x0B},
					Package (2) {"mipi-sdca-entity-label", "CS 136"}, // = CS 6B in ALC722 internal spec
					Package (2) {"mipi-sdca-cs-type", 2},
					//Package (2) {"mipi-sdca-control-list", Package() {0x10} }, // Sample_Freq_Index
					Package (2) {"mipi-sdca-control-list", 0x00010000}, // Bitmap: 0x10 - Sample_Freq_Index
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-control-0x10-subproperties", "C6B1"},
				}
			}) //End AF01.E011

			Name(C6B1, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Sample_Freq_Index, Class, RW
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 0},
				},

				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package()
				{
					Package(2) {"mipi-sdca-control-range", "BUF7"},
				}
			}) //End AF01.E011.C111

			Name(BUF7, Buffer()
			{
				0x02, 0x00, // Range type 0x0002
				0x04, 0x00, // Count of ranges = 0x4
				0x08, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // 0x00000008, 0x0000AC44(44100)
				0x09, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // 0x00000009, 0x0000BB80(48000)
				0x0B, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // 0x0000000B, 0x00017700(96000)
				0x0D, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x02, 0x00, // 0x0000000D, 0x0002EE00(192000)
			}) //End AF01.E011.C111.BUF7

	//==============================================================================================
			Name(E061, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x0b},
					Package (2) {"mipi-sdca-entity-label", "CS 41"}, // = CS 61 in ALC722 internal spec
					Package (2) {"mipi-sdca-cs-type", 2},
					//Package (2) {"mipi-sdca-control-list",	Package() {0x10} },  // Sample_Freq_Index
					Package (2) {"mipi-sdca-control-list",	0x00010000}, // Bitmap: 0x10 - Sample_Freq_Index
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-control-0x10-subproperties", "C610"},
				}
			}) //End AF01.E001

			Name(C610, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Sample_Freq_Index, Class, RW
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 0},
				},

				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package()
				{
					Package(2) {"mipi-sdca-control-range", "BUF1"},
				}
			}) //End AF01.E001.C110

			Name(BUF1, Buffer()
			{
				0x02, 0x00,  // Range type 0x0002
				0x04, 0x00,  // Count of ranges = 0x4
				0x08, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00,0x00, // 0x00000008, 0x0000AC44(44100)
				0x09, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00,0x00, // 0x00000009, 0x0000BB80(48000)
				0x0b, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01,0x00, // 0x0000000b, 0x00017700(96000)
				0x0d, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x02,0x00, // 0x0000000d, 0x0002EE00(192000)
			}) //End AF01.E061.C610.BUF1

	//==============================================================================================

			Name(E062, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x2},
					Package (2) {"mipi-sdca-entity-label", "IT 41"}, // = IT 62 in ALC722 internal spec
					Package (2) {"mipi-sdca-terminal-type", 0x0101},
					//Package (2) {"mipi-sdca-control-list", Package() {0x8, 0x10, 0x11} }, // Latency, ClusterIndex, DataPortIndex
					Package (2) {"mipi-sdca-control-list", 0x00030100}, // Bitmap: 0x8 - Latency, 0x10 - ClusterIndex, 0x11 - DataPortIndex
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					// Latency, ClusterIndex, DataPort Selector
					Package (2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "C620"},
					Package (2) {"mipi-sdca-control-0x11-subproperties", "C621"},
					Package (2) {"mipi-sdca-terminal-clock-connection", "E061"},
				},

				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package()
				{
					Package(2) {"mipi-sdca-terminal-dp-numbers", "BUF2"},
				}
			}) //End AF01.E062

			Name(BUF2, Buffer()
			{
				0x0, 0x5, // DP_Index, DP_Num
			}) //End AF01.E062.BUF2

			Name(LC00, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Latency, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0},
				}
			}) //End AF01.LC00

			Name(C620, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// ClusterIndex, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0},
				}
			}) //End AF01.E062.C620

			Name(C621, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// DataPort Selector, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0},
				}
			}) //End AF01.E062.C621

	//==============================================================================================

			Name(E063, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x0A},
					Package (2) {"mipi-sdca-entity-label", "XU 42"}, // = XU 63 in ALC722 internal spec
					Package (2) {"mipi-sdca-control-interrupt-position", 6}, // bit 6 in SDCA interrupt
					//Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x2, 0x6} }, // Bypass, ClusterIndex, latency
					//Package (2) {"mipi-sdca-control-list", 0x0046}, // Bitmap: 0x01 - Bypass, 0x02 - ClusterIndex, 0x06
					//Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x6} }, // Bypass, latency
					Package (2) {"mipi-sdca-control-list", 0x0042}, // Bitmap: 0x01 - Bypass, 0x06 - latency
					Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-powerdomain-entity", "E057"},
					Package (2) {"mipi-sdca-input-pin-1", "E062"}, // Input Pin 1 connected to IT62
					Package (2) {"mipi-sdca-control-0x1-subproperties", "C631"},
					//Package (2) {"mipi-sdca-control-0x2-subproperties", "C302"},
					Package (2) {"mipi-sdca-control-0x6-subproperties", "LC00"},
				}
			}) //End AF01.E063

			Name(C631, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Bypass, Class, R/W
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 0},
				}
			}) //End AF01.E003.C631

			//Name(C302, Package()
			//{
			//	ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
			//	Package ()
			//	{
			//		// ClusterIndex, Class, DC
			//		Package (2) {"mipi-sdca-control-access-layer", 4},
			//		Package (2) {"mipi-sdca-control-access-mode", 5},
			//		Package (2) {"mipi-sdca-control-dc-value", 1},
			//	}
			//}) //End AF01.E003.C302

	//==============================================================================================

			Name(E057, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x11},
					Package (2) {"mipi-sdca-entity-label", "PDE 142"}, // = PDE 57 in ALC722 internal spec
					//Package (2) {"mipi-sdca-powerdomain-state-list", 0xF},
					Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x63} },
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
					//Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x10} },  // Requested_PS, Actual_PS
					Package (2) {"mipi-sdca-control-list", 0x00010002},  // Bitmap: 0x1 - Requested_PS, 0x10 - Actual_PS
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-control-0x1-subproperties", "RP57"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "AP57"},
				}
			}) //End AF01.E050

			Name(RP57, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Requested_PS, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0x0},
				},
				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package ()
				{
					Package (2) {"mipi-sdca-control-range", "PDEC"},
				}
			}) //End AF01.E050.RP50

			Name(PDEC, Buffer()
            {
	             0x01, 0x00, // Range type 0x0001
	             0x01, 0x00, // Count of ranges = 0x1
	             0x00, 0x00, 0x00, 0x00, // PS0
	             //0x03, 0x00, 0x00, 0x00, // PS3
            }) //End PDEC

			Name(AP57, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Actual_PS, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0x0},
			}
			}) //End AF01.E050.AP50



	//===================================================================================

			Name(E049, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package()
				{
					Package(2) {"mipi-sdca-entity-type", 0x21},
					Package(2) {"mipi-sdca-entity-label", "UDMPU 42"}, // = UDMPU 49 in ALC722 internal spec
					//Package(2) {"mipi-sdca-control-list", Package() {0x1, 0x6, 0x10} }, // Bypass, Latency, ClusterIndex
					Package(2) {"mipi-sdca-control-list", 0x00010042}, // Bitmap: 0x1 - Bypass, 0x6 - Latency, 0x10 - ClusterIndex
					Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package()
				{
					Package(2) {"mipi-sdca-input-pin-1", "E063"}, // Input Pin 1 connected to XU63
					Package(2) {"mipi-sdca-control-0x1-subproperties", "C491"},
					Package(2) {"mipi-sdca-control-0x6-subproperties", "LC00"},
					Package(2) {"mipi-sdca-control-0x10-subproperties", "C490"},
				}
			}) //End E049

			Name(C491, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package()
				{
					// Bypass, Class, RW
					Package(2) {"mipi-sdca-control-access-layer", 4},
					Package(2) {"mipi-sdca-control-access-mode", 0},
				}
			}) //End C491


			Name(C490, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package()
				{
					// ClusterIndex, Class, DC
					Package(2) {"mipi-sdca-control-access-layer", 4},
					Package(2) {"mipi-sdca-control-access-mode", 5},
					Package(2) {"mipi-sdca-control-dc-value", 0x1},
				}
			}) //End C490



			Name(E04F, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x07},
					Package (2) {"mipi-sdca-entity-label", "FU 42"}, // = FU 4F in ALC722 internal spec
					//Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x2, 0x10} }, // Mute, Volume, Latency
					Package (2) {"mipi-sdca-control-list", 0x00010006}, // Bitmap: 0x1 - Mute, 0x2 - Volume, 0x10 - Latency
					Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-input-pin-1", "E049"}, // Input Pin 1 connected to UDMPU42
					Package (2) {"mipi-sdca-control-0x1-subproperties", "C4F1"},
					Package (2) {"mipi-sdca-control-0x2-subproperties", "C4F2"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "LC00"},//Latency
				}
			}) //End AF01.E04F

			Name(C4F1, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Mute, User, RW
					Package (2) {"mipi-sdca-control-access-layer", 1},
					Package (2) {"mipi-sdca-control-access-mode", 0},
					//Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} }, // channel 1 and 2
					Package (2) {"mipi-sdca-control-cn-list", 0x6 }, // channel 1 and 2
				}
			}) //End AF01.E04F.C4F1

			Name(C4F2, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Volume, User, RW
					Package (2) {"mipi-sdca-control-access-layer", 1},
					Package (2) {"mipi-sdca-control-access-mode", 0},
					//Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} },
					Package (2) {"mipi-sdca-control-cn-list", 0x6 },
				},

				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package()
				{
					Package(2) {"mipi-sdca-control-range", "BUF3"},
				}
			}) //End AF01.E04F.C4F2

			Name(BUF3, Buffer()
			{
				0x03, 0x00, // Range type 0x0003
				0x01, 0x00, // Count of ranges = 0x1
				0xC0, 0xBE, 0x00, 0x00, // Min = 0x0000BEC0
				0x00, 0x00, 0x00, 0x00, // Max = 0x00000000
				0xC0, 0x00, 0x00, 0x00, // Step = 0x000000C0
			}) //End AF01.E005.BUF3

	//==============================================================================================

			Name(E04A, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package()
				{
					Package(2) {"mipi-sdca-entity-type", 0x21},
					Package(2) {"mipi-sdca-entity-label", "UDMPU 47"}, // = UDMPU 4A in ALC722 internal spec
					//Package(2) {"mipi-sdca-control-list", Package() {0x1, 0x6, 0x10} }, // Bypass, Latency, ClusterIndex
					Package(2) {"mipi-sdca-control-list", 0x00010042}, // Bitmap: 0x1 - Bypass, 0x6 - Latency, 0x10 - ClusterIndex
					Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package()
				{
					Package(2) {"mipi-sdca-input-pin-1", "E04F"}, // Input Pin 1 connected to FU42
					Package(2) {"mipi-sdca-control-0x1-subproperties", "C4A1"},
					Package(2) {"mipi-sdca-control-0x6-subproperties", "LC00"},
					Package(2) {"mipi-sdca-control-0x10-subproperties", "C4A0"},
				}
			}) //End E048

			Name(C4A1, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package()
				{
					// Bypass, Class, RW
					Package(2) {"mipi-sdca-control-access-layer", 4},
					Package(2) {"mipi-sdca-control-access-mode", 0},
				}
			}) //End C481


			Name(C4A0, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package()
				{
					// ClusterIndex, Class, DC
					Package(2) {"mipi-sdca-control-access-layer", 4},
					Package(2) {"mipi-sdca-control-access-mode", 5},
					Package(2) {"mipi-sdca-control-dc-value", 0x1},
				}
			}) //End C480



			Name(E064, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x06},
					Package (2) {"mipi-sdca-entity-label", "SU 44"}, // = SU 64 in ALC722 internal spec
					//Package (2) {"mipi-sdca-control-list",	Package() {0x1, 0x2}}, // selector, latency
					Package (2) {"mipi-sdca-control-list",	0x06}, // Bitmap: 0x1 - selector, 0x2 - latency
					Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-input-pin-1", "E04A"}, // Input Pin 1 connected to UDMPU47
					Package (2) {"mipi-sdca-control-0x1-subproperties", "C641"},
					Package (2) {"mipi-sdca-control-0x2-subproperties", "LC00"},
				}
			}) //End AF01.E064

			Name(C641, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// selector, Device, RO (SU controlled by GE)
					Package (2) {"mipi-sdca-control-access-layer", 0x10},
					Package (2) {"mipi-sdca-control-access-mode", 3},
					//Package (2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} }, // channel 1 and 2
					Package (2) {"mipi-sdca-control-cn-list", 0x6 }, // channel 1 and 2
				}
			}) //End AF01.E064.C641

	//==============================================================================================



			Name(E06E, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x11},
					Package (2) {"mipi-sdca-entity-label", "PDE 47"}, // = PDE 6E in ALC722 internal spec
					//Package (2) {"mipi-sdca-powerdomain-state-list", 0xF},
					Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x75} },
					Package (2) {"mipi-sdca-powerdomain-transition-typical-delay",
						Package()
						{
							0x3, 0x0, 384000,
							0x0, 0x3, 384000,
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
							0x3, 0x0, 500000,
							0x0, 0x3, 500000,
							0x2, 0x0, 0x0,
							0x0, 0x2, 0x0,
							0x1, 0x0, 0x0,
							0x0, 0x1, 0x0,
							0x0, 0x0, 0x0,
						}
					},
					//Package (2) {"mipi-sdca-control-list", Package() {0x1, 0x10} }, // Requested_PS, Actual_PS
					Package (2) {"mipi-sdca-control-list", 0x00010002}, // 0x1 - Requested_PS, 0x10 - Actual_PS
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-control-0x1-subproperties", "RP57"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "AP57"},
				}
			}) //End AF01.E040


	//==============================================================================================

			Name(E075, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x03},
					Package (2) {"mipi-sdca-entity-label", "OT 44"}, // = OT 75 in ALC722 internal spec
					Package (2) {"mipi-sdca-terminal-type", 0x0690},
					Package (2) {"mipi-sdca-terminal-reference-number", 0x2},
					Package (2) {"mipi-sdca-input-pin-list", 0x2},
					//Package (2) {"mipi-sdca-control-list", Package() {0x4, 0x8} },  // Usage, Latency
					Package (2) {"mipi-sdca-control-list", 0x0110},  // 0x4 - Usage, 0x8 - Latency
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-input-pin-1", "E064"},
					Package (2) {"mipi-sdca-powerdomain-entity", "E06E"},
					Package (2) {"mipi-sdca-control-0x4-subproperties", "C754"},
					Package (2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
				},

				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package ()
				{
					Package (2) {"mipi-sdca-terminal-reference-description", "TRD1"},
				}
			}) //End AF01.E006

			Name(C754, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Usage, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0x60}, // Full band
				}
			}) //End AF01.E006.C604


	//==============================================================================================

	// =================================== Below part is SPE function.
	/*
			Name(E024, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x2},
					Package (2) {"mipi-sdca-entity-label", "IT 199"}, // = IT 24 in ALC722 internal spec
					Package (2) {"mipi-sdca-terminal-type", 0x0741},
					//Package (2) {"mipi-sdca-control-list", Package() {0x8, 0x10, 0x11} }, // Latency, ClusterIndex, DataPortIndex
					Package (2) {"mipi-sdca-control-list", 0x00030100}, // Bitmap: 0x8 - Latency, 0x10 - ClusterIndex, 0x11 - DataPortIndex
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					// Latency, ClusterIndex, DataPort Selector
					Package (2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "C240"},
					Package (2) {"mipi-sdca-control-0x11-subproperties", "C241"},
				},

			})

			Name(C240, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// ClusterIndex, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0},
				}
			})

			Name(C241, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// DataPort Selector, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0},
				}
			})

    //=============================================================================

			Name(E025, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x13},
					Package (2) {"mipi-sdca-entity-label", "PE 199"}, // = PE 25 in ALC722 internal spec
					Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1
					//Package (2) {"mipi-sdca-control-list",	Package() {0x10} },  // Sample_Freq_Index
					Package (2) {"mipi-sdca-control-list",	0x001E}, // Bitmap: 0x01 - Private, 0x02 - Privacy_policy, 0x03 - Privacy_lockstate, 0x04 - Privacy_owner
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-input-pin-1", "E024"}, // Input Pin 1 connected to IT 20
					Package (2) {"mipi-sdca-control-0x01-subproperties", "C251"},
					Package (2) {"mipi-sdca-control-0x02-subproperties", "C252"},
					Package (2) {"mipi-sdca-control-0x03-subproperties", "C253"},
					Package (2) {"mipi-sdca-control-0x04-subproperties", "C254"},
				}
			}) //End AF01.E001

			Name(C251, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Private,
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-interrupt-position", 13}, // SDCA Interrupt Bit 5
					Package (2) {"mipi-sdca-control-access-mode", 0},
				}
			})

			Name(C252, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Privacy_Policy,
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 0},
				}
			})

			Name(C253, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Privacy_Lockstate,
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 0},
				}
			})

			Name(C254, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Privacy_owner,
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 0},

				}
			})

	//=============================================================================

			Name(E026, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x11},
					Package (2) {"mipi-sdca-entity-label", "PDE 199"}, // = PDE 26
					//Package (2) {"mipi-sdca-powerdomain-state-list", 0xF},
					//Package (2) {"mipi-sdca-powerdomain-managed-list", Package() {0x8, 0x9, 0x43}},
					Package (2) {"mipi-sdca-powerdomain-transition-typical-delay",
						Package()
						{
							0x3, 0x0, 384000,
							0x0, 0x3, 384000,
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
							0x3, 0x0, 500000,
							0x0, 0x3, 500000,
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
					Package (2) {"mipi-sdca-control-0x1-subproperties", "RP26"},
					Package (2) {"mipi-sdca-control-0x10-subproperties", "AP26"},
				}
			})

			Name(RP26, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Requested_PS, Class, R/W
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 0},
				},
				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package ()
				{
					Package (2) {"mipi-sdca-control-range", "PDER"},
				}
			})

			Name(AP26, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Actual_PS, Class, RO
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 3},
				}
			})

	//=============================================================================================================

			Name(E027, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					Package (2) {"mipi-sdca-entity-type", 0x03},
					Package (2) {"mipi-sdca-entity-label", "OT 199"}, // = OT 27 in ALC722 internal spec
					Package (2) {"mipi-sdca-terminal-type", 0x0747},
					Package (2) {"mipi-sdca-terminal-reference-number", 0x1},
					Package (2) {"mipi-sdca-input-pin-list", 0x2},
					//Package (2) {"mipi-sdca-control-list", Package() {0x4, 0x8} }, // Usage, Latency
					Package (2) {"mipi-sdca-control-list", 0x0110}, // 0x4 - Usage, 0x8 - Latency
				},

				ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
				Package ()
				{
					Package (2) {"mipi-sdca-input-pin-1", "E025"},
					Package (2) {"mipi-sdca-powerdomain-entity", "E026"},
					Package (2) {"mipi-sdca-control-0x4-subproperties", "C274"},
					Package (2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
				},

				ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
				Package ()
				{
					Package (2) {"mipi-sdca-terminal-reference-description", "TRD1"},
				}
			})

			Name(C274, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package ()
				{
					// Usage, Class, DC
					Package (2) {"mipi-sdca-control-access-layer", 4},
					Package (2) {"mipi-sdca-control-access-mode", 5},
					Package (2) {"mipi-sdca-control-dc-value", 0x60}, // Full band
				}
			})

  //=============================================================================================================
	*/

			Name(EXT0, Package()
			{
				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package()
				{
					Package(2) {"mipi-sdca-function-expansion-subsystem-id", 0x1234 },
					Package(2) {"mipi-sdca-function-expansion-subsystem-revision-id", 0x1234},
					// both render and capture are supported
					Package(2) {"realtek-ge-supported-terminals", Package() {0x680, 0x6A0, 0x6D0, 0x690, 0x6C0} }, // LineIn Stereo, Mic on Jack, Headset, LineOut Stereo, Headphone on Jack
					// capture is disabled
					//Package(2) {"realtek-ge-supported-terminals", Package() {0x690, 0x6C0} }, // LineOut Stereo, Headphone on Jack
					// render is disabled
					//Package(2) {"realtek-ge-supported-terminals", Package() {0x680, 0x6A0} }, // LineIn Stereo, Mic on Jack
				}
			}) //End EXT0

