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

Device(AF04) {
	Name(_ADR, 04)

	Name(_DSD, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdw-sw-interface-revision", 0x00020000}, // v2.0
		   Package(2) {"mipi-sdw-sdca-interface-revision", 0x1000}, // v1.0
		   Package(2) {"mipi-sdw-wake-up-unavailable", 0}, // support wakeup
		   Package(2) {"mipi-sdw-test-mode-supported", 1}, // Integer/Boolean
		   Package (2) {"mipi-sdca-function-topology-features", 0x8000000000042D},
		   // Secondary Transducer,Ultrasound Render,Sense Capture, Reference Stream, Companion Data Output, Security / Privacy
           //Package (2) {"mipi-sdca-function-topology-features", 0xB00},
           //Package (2) {"mipi-sdca-control-list", 0x01F0}, // Bitmap: 0x4, 0x5, 0x6, 0x7, 0x8 - Function level controls
		   Package (2) {"mipi-sdca-control-list", 0x301F2}, // Bitmap: 0x0, 0x4, 0x5, 0x6, 0x7, 0x8, 0x10, 0x11 - Function level controls
           Package (2) {"mipi-sdca-entity-id-list",
           Package() {0x0, 0x33, 0x27, 0x3, 0x26, 0x31, 0x29, 0x40, 0x21, 0x28, 0x22, 0x24, 0x50, 0x4, 0x6, 0x51, 0x20, 0x36, 0x13, 0x14, 0x15, 0x17, 0x38, 0x52} },
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-control-0x8-subproperties", "C008"},
           Package (2) {"mipi-sdca-control-0x7-subproperties", "C007"},
           Package (2) {"mipi-sdca-control-0x6-subproperties", "C006"},
           Package (2) {"mipi-sdca-control-0x5-subproperties", "C005"},
           Package (2) {"mipi-sdca-control-0x4-subproperties", "C004"},
		   Package (2) {"mipi-sdca-control-0x1-subproperties", "C042"},
           Package (2) {"mipi-sdca-control-0x10-subproperties", "C043"},
           Package (2) {"mipi-sdca-control-0x11-subproperties", "C044"},
           Package (2) {"mipi-sdca-entity-id-0x0-subproperties", "E000"},
           Package (2) {"mipi-sdca-entity-id-0x33-subproperties", "E051"},
           Package (2) {"mipi-sdca-entity-id-0x27-subproperties", "E039"},
           Package (2) {"mipi-sdca-entity-id-0x3-subproperties", "E003"},
           Package (2) {"mipi-sdca-entity-id-0x26-subproperties", "E038"},
           Package (2) {"mipi-sdca-entity-id-0x31-subproperties", "E049"},
           Package (2) {"mipi-sdca-entity-id-0x29-subproperties", "E041"},
           Package (2) {"mipi-sdca-entity-id-0x40-subproperties", "E064"},
           Package (2) {"mipi-sdca-entity-id-0x21-subproperties", "E033"},
           Package (2) {"mipi-sdca-entity-id-0x28-subproperties", "E040"},
           Package (2) {"mipi-sdca-entity-id-0x22-subproperties", "E034"},
           Package (2) {"mipi-sdca-entity-id-0x24-subproperties", "E036"},
           Package (2) {"mipi-sdca-entity-id-0x50-subproperties", "E080"},
           Package (2) {"mipi-sdca-entity-id-0x4-subproperties", "E004"},
           Package (2) {"mipi-sdca-entity-id-0x6-subproperties", "E006"},
           Package (2) {"mipi-sdca-entity-id-0x51-subproperties", "E081"},
           Package (2) {"mipi-sdca-entity-id-0x20-subproperties", "E032"},
           Package (2) {"mipi-sdca-entity-id-0x36-subproperties", "E054"},
           Package (2) {"mipi-sdca-entity-id-0x13-subproperties", "E019"},
           Package (2) {"mipi-sdca-entity-id-0x14-subproperties", "E020"},
           Package (2) {"mipi-sdca-entity-id-0x15-subproperties", "E021"},
           Package (2) {"mipi-sdca-entity-id-0x17-subproperties", "E023"},
           Package (2) {"mipi-sdca-entity-id-0x38-subproperties", "E056"},
		   Package (2) {"mipi-sdca-entity-id-0x52-subproperties", "E082"},
		},
		   ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
					Package()
					{
						Package(2) {"mipi-sdca-function-initialization-table","BUF0"},
						Package(2) {"mipi-sdca-function-supported-postures", "BUF1"},
					}

	}) //End AF04._DSD

	Name(BUF0, Buffer()
				{
				//ALC1320 Blind Write Commands
				}) //End AF04.BUF0
	Name(C008, Package() {
	   				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{
						// Function_Version = 0x1, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0x1},
					}
	}) //End AF04.C008

	Name(C007, Package() {
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{
						// Function_ID = 0x1320 (Device specific), Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0x1320},
					}
	}) //End AF04.C007

	Name(C006, Package() {
	   				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0x025d},
					}
	}) //End AF04.C006

	Name(C005, Package() {
	   				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{
						// Function_Topology = 0x1 Smart-Amp, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0x1},
					}
	}) //End AF04.C005

	Name(C004, Package() {
	   				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{
						// Function_SDCA_Version = 1.0, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0x10},
					}
	}) //End AF04.C004

	Name(C042, Package() {
	   	   			ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{
						//Commit_Group_Mask
						Package(2) {"mipi-sdca-control-access-layer", 4 }, //Access Layer = Class
						Package(2) {"mipi-sdca-control-access-mode", 0x0}, //Read-Write
					}

	}) //End AF04.C042

	Name(C043, Package() {
	   	   	   		ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{
						//Function_Action
						Package(2) {"mipi-sdca-control-access-layer", 4 }, //Access Layer = Class
						Package(2) {"mipi-sdca-control-access-mode", 0x10}, //Read-and-Write-1-to-Set
					}
	}) //End AF04.C043

	Name(C044, Package() {
	   	   	   		ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{
						//Function_Status
						Package (2) {"mipi-sdca-control-interrupt-position", 1}, //IntStat_SDCA_1
					}
	}) //End AF04.C044


	Name(E000, Package() {
	}) //End AF04.E000

	Name(E051, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x11},
           Package (2) {"mipi-sdca-entity-label", "PDE 23"},
           Package (2) {"mipi-sdca-control-list", 0x10002},
           Package (2) {
							"mipi-sdca-powerdomain-managed-list",
							Package() {0x13}, //OT23
						},
		   Package(2)
						{
							"mipi-sdca-powerdomain-transition-typical-delay",
							Package()
							{
								3, 0, 500000,
								0, 3, 500000,
							},
						},
		   Package(2)
						{
							"mipi-sdca-powerdomain-transition-max-delay",
							Package()
							{
								3, 0, 800000,
								0, 3, 800000,
							},
						},
					},
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-control-0x1-subproperties", "C001"}, //PDE23 Request PS0
		   Package (2) {"mipi-sdca-control-0x10-subproperties", "C016"}, //PDE23 Actual PS0

        }
	}) //End AF04.E051

	Name(C001, Package() {
		ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package()
					{  // Requested_PS, Class, R/W
						Package(2) {"mipi-sdca-control-access-layer", 4},
						Package(2) {"mipi-sdca-control-access-mode", 0},
					},
					ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
					Package ()
					{
						Package (2) {"mipi-sdca-control-range", "PDER"},
					}
	}) //End AF04.C001
	Name(PDER, Buffer()
			{
				0x01, 0x00, // Range type 0x0001
				0x02, 0x00, // Count of ranges = 0x2
				0x00, 0x00, 0x00, 0x00, // PS0
				0x03, 0x00, 0x00, 0x00, // PS3
			}) //End PDER
	Name(C016, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-control-access-layer", 0x4},
           Package (2) {"mipi-sdca-control-access-mode", 0x3},
        }
	}) //End AF04.C016

	Name(E039, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
			Package (2) {"mipi-sdca-entity-type", 0x11},
			Package (2) {"mipi-sdca-entity-label", "PDE 27"},
			Package (2) {"mipi-sdca-control-list", 0x10002},
			Package (2) {
							"mipi-sdca-powerdomain-managed-list",
							Package() {0x17}, //OT27
						},
						Package(2)
						{
							"mipi-sdca-powerdomain-transition-typical-delay",
							Package()
							{
								3, 0, 500000,
								0, 3, 500000,
							},
						},
						Package(2)
						{
							"mipi-sdca-powerdomain-transition-max-delay",
							Package()
							{
								3, 0, 800000,
								0, 3, 800000,
							},
						},
					},
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-control-0x1-subproperties", "C002"}, //PDE27 Request PS0
		   Package (2) {"mipi-sdca-control-0x10-subproperties", "C017"}, //PDE27 Actual PS0

        }
	}) //End AF04.E039

	Name(C002, Package() {
	   	ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
				Package()
					{  // Requested_PS, Class, R/W
						Package(2) {"mipi-sdca-control-access-layer", 4},
						Package(2) {"mipi-sdca-control-access-mode", 0},
					},
					ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
					Package ()
					{
						Package (2) {"mipi-sdca-control-range", "PDER"},
					}
	}) //End AF04.C002

	Name(C017, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-control-access-layer", 0x4},
           Package (2) {"mipi-sdca-control-access-mode", 0x3},
        }
	}) //End AF04.C017

	Name(E003, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x07},
           Package (2) {"mipi-sdca-entity-label", "FU 21"},
           Package (2) {"mipi-sdca-control-list", 0x406},
           Package (2) {"mipi-sdca-input-pin-list", 0x02},
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-control-0x2-subproperties", "C003"}, //User FU Volume
           Package (2) {"mipi-sdca-control-0x1-subproperties", "C009"}, //User FU Mute
           Package (2) {"mipi-sdca-control-0xA-subproperties", "C010"}, //User FU Loudness
           Package (2) {"mipi-sdca-input-pin-1", "E004"},
        }
	}) //End AF04.E003

	Name(C003, Package() {
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{  // Volume, User, RW
						Package(2) {"mipi-sdca-control-access-layer", 1 },
						Package(2) {"mipi-sdca-control-access-mode", 0},
						//Package(2) {"mipi-sdca-control-default-value", 0x0000},
						Package(2) {"mipi-sdca-control-fixed-value", 0x0000},
						//Package(2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} },
						Package(2) {"mipi-sdca-control-cn-list", 0x6},
					},
					ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
					Package()
					{
						Package(2) {"mipi-sdca-control-range","BUF3"},
					}
	}) //End AF04.C003
	Name(BUF3, Buffer()
				{
					0x03, 0x00,  // Range type 0x0003 (Triples)
					0x01, 0x00,  // Count of ranges = 0x1
					0xC0, 0xA0, 0x00, 0x00,  // Min =0x0000A0C0
					0x00, 0x00, 0x00, 0x00,  // Max =0x00000000
					0xC0, 0x00, 0x00, 0x00,  // Step =0x000000C0, -0.75dB/step
				}) //End AF04.BUF3

	Name(C009, Package() {
	   				ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{  // Mute, User, RW
						Package(2) {"mipi-sdca-control-access-layer", 1 },
						Package(2) {"mipi-sdca-control-access-mode", 0},
						//Package(2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} },
						Package(2) {"mipi-sdca-control-cn-list", 0x6},
					}
	}) //End AF04.C009

	Name(C010, Package() {
	}) //End AF04.C010

	Name(E038, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x07},
           Package (2) {"mipi-sdca-entity-label", "FU 26"},
           Package (2) {"mipi-sdca-control-list", 0x802},
           Package (2) {"mipi-sdca-input-pin-list", 0x02},
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-control-0x1-subproperties", "C011"}, //Platform FU Mute
           Package (2) {"mipi-sdca-control-0xB-subproperties", "C012"}, //Platform FU Volume
           Package (2) {"mipi-sdca-input-pin-1", "E006"},
        }
	}) //End AF04.E038

	Name(C011, Package() {
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{  // Mute, Platform, RW
						Package(2) {"mipi-sdca-control-access-layer", 1 },
						Package(2) {"mipi-sdca-control-access-mode", 0},
						//Package(2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} },
						Package(2) {"mipi-sdca-control-cn-list", 0x6},
					}
	}) //End AF04.C011

	Name(C012, Package() {
	   					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{  // Volume, User, RW
						Package(2) {"mipi-sdca-control-access-layer", 1 },
						Package(2) {"mipi-sdca-control-access-mode", 0},
						//Package(2) {"mipi-sdca-control-default-value", 0x0000},
						Package(2) {"mipi-sdca-control-fixed-value", 0x0000},
						//Package(2) {"mipi-sdca-control-cn-list", Package() {0x1, 0x2} },
						Package(2) {"mipi-sdca-control-cn-list", 0x6},
					},
					ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
					Package()
					{
						Package(2) {"mipi-sdca-control-range","BUF7"},
					}
	}) //End AF04.C012
	Name(BUF7, Buffer()
				{
					0x03, 0x00,  // Range type 0x0003 (Triples)
					0x01, 0x00,  // Count of ranges = 0x1
					0x7F, 0xBF, 0x00, 0x00,  // Min =0x0000BF7F
					0x00, 0x00, 0x00, 0x00,  // Max =0x00000000
					0xC0, 0x00, 0x00, 0x00,  // Step =0x000000C0, -0.75dB/step
				}) //End AF04.BUF7


	Name(E049, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
			Package (2) {"mipi-sdca-entity-type", 0x22},
			Package (2) {"mipi-sdca-entity-label", "MFPU 21"},
			Package (2) {"mipi-sdca-control-list", 0x1000000000230},
			Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
			Package (2) {"mipi-sdca-input-pin-1", "E003"},// Input Pin 1 connected to FU 21
			Package (2) {"mipi-sdca-control-0x30-subproperties", "C048"}, //Bypass
			Package (2) {"mipi-sdca-control-0x5-subproperties", "C013"}, //Algorithm_Enable
			Package (2) {"mipi-sdca-control-0x4-subproperties", "C014"}, //Algorithm_Ready
			Package (2) {"mipi-sdca-control-0x9-subproperties", "C015"}, //Algorithm_Prepare

        }
	}) //End AF04.E049

	Name(C048, Package() {
		ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
			Package ()
			{
			// Bypass, Class, DC:1 (Null MFPU)
			Package (2) {"mipi-sdca-control-access-layer", 0x4},
			Package (2) {"mipi-sdca-control-access-mode", 5},
			Package (2) {"mipi-sdca-control-dc-value", 1},
			}
	}) //End AF04.C048

	Name(C013, Package() {
	}) //End AF04.C013

	Name(C014, Package() {
	}) //End AF04.C014

	Name(C015, Package() {
	}) //End AF04.C015

	Name(E041, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x24},
           Package (2) {"mipi-sdca-entity-label", "SAPU 29"},
           Package (2) {"mipi-sdca-control-list", 0x30000},
           Package (2) {"mipi-sdca-input-pin-list", 0x6},
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-control-0x11-subproperties", "C018"}, //Protection_Status
           Package (2) {"mipi-sdca-control-0x10-subproperties", "C019"}, //Protection_Mode
		   Package (2) {"mipi-sdca-input-pin-1", "E034"}, // Input Pin 1 connected to XU22
           Package (2) {"mipi-sdca-input-pin-2", "E082"}, // Input Pin 2 connected to IT29

        }
	}) //End AF04.E041

	Name(C018, Package() {
		ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{   // Protection_Status, Class, RO
						Package(2) {"mipi-sdca-control-access-layer", 4},
						//Package (2) {"mipi-sdca-control-access-mode", 3},
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0},
					}
	}) //End AF04.C018

	Name(C019, Package() {
	ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{   // Protection_Mode, Class, RO
						Package(2) {"mipi-sdca-control-access-layer", 4},
						Package(2) {"mipi-sdca-control-interrupt-position", 0},  // SDCA Interrupt Bit 0
						Package(2) {"mipi-sdca-control-access-mode", 3},
					}
	}) //End AF04.C019

	Name(E064, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x30},
           Package (2) {"mipi-sdca-entity-label", "TG 23"},
           Package (2) {"mipi-sdca-control-list", 0x10000}, // Class Divider
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-control-0x10-subproperties", "C020"},
		   Package (2) {"mipi-sdca-terminal-clock-connection", "E033"},
        }
	}) //End AF04.E064

	Name(C020, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-control-access-layer", 0x4},
           Package (2) {"mipi-sdca-control-access-mode", 0x0},
        }
	}) //End AF04.C020

	Name(E033, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x0b},
           Package (2) {"mipi-sdca-entity-label", "CS 21"},
           Package (2) {"mipi-sdca-control-list", 0x10000},
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-control-0x10-subproperties", "C021"},
        }
	}) //End AF04.E033

	Name(C021, Package() {
	   			ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // Sample_Freq_Index, Class, RW
						Package(2) {"mipi-sdca-control-access-layer", 4},
						Package(2) {"mipi-sdca-control-access-mode", 0},
					},
					ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
					Package()
					{
						Package(2) {"mipi-sdca-control-range","BUF5"},
					}
	}) //End AF04.C021

				Name(BUF5, Buffer()
				{
					0x02, 0x00,  // Range type 0x0002 (Pairs)
					0x02, 0x00,  // Count of ranges = 0x2
					0x04, 0x00, 0x00, 0x00, 0x80, 0x3E, 0x00, 0x00, // 0x00000004, 0x00003E80(16000)
					0x07, 0x00, 0x00, 0x00, 0x00, 0x7D, 0x00, 0x00, // 0x00000007, 0x00007D00(32000)
					0x08, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // 0x00000008, 0x0000AC44(44100)
					0x09, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // 0x00000009, 0x0000BB80(48000)
					0x0B, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // 0x0000000B, 0x00017700(96000)
					0x0d, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x02, 0x00, // 0x0000000d, 0x0002EE00(192000)
				}) //End AF04.BUF5

	Name(E040, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x0b},
           Package (2) {"mipi-sdca-entity-label", "CS 26"},
           Package (2) {"mipi-sdca-control-list", 0x10000},
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-control-0x10-subproperties", "C022"},
        }
	}) //End AF04.E040

	Name(C022, Package() {
	   	   			ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // Sample_Freq_Index, Class, RW
						Package(2) {"mipi-sdca-control-access-layer", 4},
						Package(2) {"mipi-sdca-control-access-mode", 0},
					},
					ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
					Package()
					{
						Package(2) {"mipi-sdca-control-range","BUF9"},
					}
	}) //End AF04.C022
   				Name(BUF9, Buffer()
				{
					0x02, 0x00,  // Range type 0x0002 (Pairs)
					0x02, 0x00,  // Count of ranges = 0x2
					0x04, 0x00, 0x00, 0x00, 0x80, 0x3E, 0x00, 0x00, // 0x00000004, 0x00003E80(16000)
					0x07, 0x00, 0x00, 0x00, 0x00, 0x7D, 0x00, 0x00, // 0x00000007, 0x00007D00(32000)
					0x08, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // 0x00000008, 0x0000AC44(44100)
					0x09, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // 0x00000009, 0x0000BB80(48000)
					0x0B, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // 0x0000000B, 0x00017700(96000)
					0x0d, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x02, 0x00, // 0x0000000d, 0x0002EE00(192000)
				}) //End AF04.BUF9

	Name(E034, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) {"mipi-sdca-entity-type", 0x0a},
            Package (2) {"mipi-sdca-entity-label", "XU 22"},
            Package (2) {"mipi-sdca-control-list", 0x7D0002},
		    Package (2) {"mipi-sdca-input-pin-list", 0x6},
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
			Package (2) { "mipi-sdca-input-pin-1", "E049"},   // Input Pin 1 connected to MFPU 21
			Package (2) {"mipi-sdca-input-pin-2", "E082"},   // Input Pin 2 connected to IT 29
            Package (2) {"mipi-sdca-control-0x16-subproperties", "C023"}, //FDL_Host_Request
            Package (2) {"mipi-sdca-control-0x15-subproperties", "C024"}, //FDL_Set_Index
            Package (2) {"mipi-sdca-control-0x14-subproperties", "C025"}, //FDL_Status
            Package (2) {"mipi-sdca-control-0x13-subproperties", "C026"}, //FDL_MessageLength
            Package (2) {"mipi-sdca-control-0x12-subproperties", "C027"}, //FDL_MessageOffset
            Package (2) {"mipi-sdca-control-0x10-subproperties", "C028"}, //FDL_CurrentOwner
            Package (2) {"mipi-sdca-control-0x1-subproperties", "C029"}, //Bypass
        }
	}) //End AF04.E034

	Name(C023, Package() {
	}) //End AF04.C023

	Name(C024, Package() {
	}) //End AF04.C024

	Name(C025, Package() {
	}) //End AF04.C025

	Name(C026, Package() {
	}) //End AF04.C026

	Name(C027, Package() {
	}) //End AF04.C027

	Name(C028, Package() {
        Package () { // FDL_CurrentOwner, Class, RW1C
            Package (2) {"mipi-sdca-control-access-layer", 4},
            Package (2) {"mipi-sdca-control-interrupt-position", 2}, //IntStat_SDCA_2
            Package (2) {"mipi-sdca-control-access-mode", 2},  // RW1C
		}
	}) //End AF04.C028

	Name(C029, Package() {
	}) //End AF04.C029

	Name(E036, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x0a},
           Package (2) {"mipi-sdca-entity-label", "XU 24"},
           Package (2) {"mipi-sdca-control-list", 0x2},
		   Package (2) {"mipi-sdca-input-pin-list", 0x2},
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
			Package(2) { "mipi-sdca-input-pin-1", "E082"},   // Input Pin 1 connected to IT 29
            Package (2) {"mipi-sdca-control-0x1-subproperties", "C030"},
        }
	}) //End AF04.E036

	Name(C030, Package() {
	}) //End AF04.C030

	Name(E080, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x05},
           Package (2) {"mipi-sdca-entity-label", "MU 26"},
           Package (2) {"mipi-sdca-control-list", 0x2},
		   Package (2) {"mipi-sdca-input-pin-list", 0x6},
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
			Package(2) { "mipi-sdca-input-pin-1", "E041"},   // Input Pin 1 connected to SAPU 29
			Package(2) {"mipi-sdca-input-pin-2", "E038"},   // Input Pin 2 connected to FU 26
            Package (2) {"mipi-sdca-control-0x1-subproperties", "C031"},
        }
	}) //End AF04.E080

	Name(C031, Package() {
	}) //End AF04.C031

	Name(E004, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
			Package (2) {"mipi-sdca-entity-type", 0x25},
			Package (2) {"mipi-sdca-entity-label", "PPU 21"},
			Package (2) {"mipi-sdca-control-list", 0xD0000},
			Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
			Package	(2) {"mipi-sdca-input-pin-1", "E032" },   // Input Pin 1 connected to IT21
			Package (2) {"mipi-sdca-control-0x12-subproperties", "C032"},
			Package (2) {"mipi-sdca-control-0x10-subproperties", "C034"},
			Package (2) {"mipi-sdca-control-0x13-subproperties", "C033"},
        }
	}) //End AF04.E004

	Name(C032, Package() {
	}) //End AF04.C032

	Name(C034, Package() {
	}) //End AF04.C034

	Name(C033, Package() {
	}) //End AF04.C033

	Name(E006, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
			Package (2) {"mipi-sdca-entity-type", 0x25},
			Package (2) {"mipi-sdca-entity-label", "PPU 26"},
			Package (2) {"mipi-sdca-control-list", 0xD0000},
			Package (2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
			Package	(2) {"mipi-sdca-input-pin-1", "E054" },   // Input Pin 1 connected to IT26
			Package (2) {"mipi-sdca-control-0x13-subproperties", "C035"},
			Package (2) {"mipi-sdca-control-0x12-subproperties", "C036"},
			Package (2) {"mipi-sdca-control-0x10-subproperties", "C037"},
        }
	}) //End AF04.E006

	Name(C035, Package() {
	}) //End AF04.C035

	Name(C036, Package() {
	}) //End AF04.C036

	Name(C037, Package() {
	}) //End AF04.C037

	Name(E081, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x13},
           Package (2) {"mipi-sdca-entity-label", "SPE 199"},
           Package (2) {"mipi-sdca-control-list", 0x1E},
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-control-0x4-subproperties", "C038"}, //Privacy_Owner
           Package (2) {"mipi-sdca-control-0x3-subproperties", "C039"}, //Privacy_LockState
           Package (2) {"mipi-sdca-control-0x2-subproperties", "C040"}, //Privacy_Policy
           Package (2) {"mipi-sdca-control-0x1-subproperties", "C041"}, //Private
        }
   }) //End AF04.E081

   Name(C038, Package() {
   }) //End AF04.C038

   Name(C039, Package() {
   }) //End AF04.C039

   Name(C040, Package() {
   }) //End AF04.C040

   Name(C041, Package() {
   }) //End AF04.C041

   Name(E032, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x02},
           Package (2) {"mipi-sdca-entity-label", "IT 21"}, //Speaker render stream
           Package (2) {"mipi-sdca-terminal-type", 0x0101},
		   Package (2) {"mipi-sdca-control-list", 0x00030110},  // Bitmap: 0x4 - Usage, 0x8 - Latency, 0x10 - ClusterIndex, 0x11 - DataPortIndex
		ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
					Package()
					{
						Package(2) {"mipi-sdca-control-0x4-subproperties", "C104" },
						Package(2) {"mipi-sdca-control-0x8-subproperties", "LC00" },
						Package(2) {"mipi-sdca-control-0x10-subproperties", "C110"},
						Package(2) {"mipi-sdca-control-0x11-subproperties", "C111"},
						Package(2) {"mipi-sdca-terminal-clock-connection", "E033"},
					},
		ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
					Package()
					{
						Package(2) {"mipi-sdca-terminal-dp-numbers","BUF2"},
					}
        }
   }) //End AF04.E032
	Name(BUF2, Buffer()
				{
					0x0, 0x1, // DP_Index: 0x0 DP_Num: 0x1
				}) //End AF04.BUF2

	Name(C104, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // Usage, Class, DC
						Package(2) { "mipi-sdca-control-access-layer", 0x4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0x1AE}, // Usage CBN, 430 (0x1ae) Full band and Near-ultrasound
					}
				}) //End AF04.C104

	Name(C110, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // ClusterIndex, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 1},
					}
				}) //End AF04.C110

	Name(C111, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // DataPortIndex, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 1},
					}
				}) //End AF04.C111

   Name(E054, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
			Package (2) {"mipi-sdca-entity-type", 0x02},
			Package (2) {"mipi-sdca-entity-label", "IT 26"}, //Ultrasound render stream
			Package (2) {"mipi-sdca-terminal-type", 0x180},
			Package (2) {"mipi-sdca-control-list", 0x00030110},  // Bitmap: 0x4 - Usage, 0x8 - Latency, 0x10 - ClusterIndex, 0x11 - DataPortIndex
			ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
					Package()
					{
						Package(2) {"mipi-sdca-control-0x4-subproperties", "C204" },
						Package(2) {"mipi-sdca-control-0x8-subproperties", "LC00" },
						Package(2) {"mipi-sdca-control-0x10-subproperties", "C210"},
						Package(2) {"mipi-sdca-control-0x11-subproperties", "C211"},
						Package(2) {"mipi-sdca-terminal-clock-connection", "E040"},
					},
			ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
					Package()
					{
						Package(2) {"mipi-sdca-terminal-dp-numbers","BUF4"},
					}
        }
   }) //End AF04.E054
	Name(BUF4, Buffer()
				{
					0x0, 0x5, // DP_Index: 0x0 DP_Num: 0x5
				}) //End AF04.BUF4

	Name(C204, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // Usage, Class, DC
						Package(2) { "mipi-sdca-control-access-layer", 0x4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0x1B8}, // Usage CBN, 440 (0x1b8) Full Band & Ultrasound40
					}
				}) //End AF04.C204

	Name(C210, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // ClusterIndex, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 1},
					}
				}) //End AF04.C210

	Name(C211, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // DataPortIndex, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 1},
					}
				}) //End AF04.C211


	Name(E082, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x02},
           Package (2) {"mipi-sdca-entity-label", "IT 29"},
           Package (2) {"mipi-sdca-control-list", 0x10100}, // Bitmap: 0x8 - Latency, 0x10 - ClusterIndex
           Package (2) {"mipi-sdca-terminal-type", 0x0280},
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
           Package (2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
		   Package (2) {"mipi-sdca-control-0x10-subproperties", "C045"},

        }
	}) //End AF04.E082

	Name(C045, Package() {
			ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
		Package()
		{
			// ClusterIndex, Class, DC:1
			Package(2) {"mipi-sdca-control-access-layer", 4},
			Package(2) {"mipi-sdca-control-access-mode", 5},
			Package(2) {"mipi-sdca-control-dc-value", 1},
		}
	}) //End AF04.C045


	Name(E019, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x03},
           Package (2) {"mipi-sdca-entity-label", "OT 23"}, //Primary transducer
           Package (2) {"mipi-sdca-terminal-type", 0x380},
		   Package (2) {"mipi-sdca-input-pin-list", 0x2},
		   Package (2) {"mipi-sdca-control-list", 0x110 },  // Bitmap: 0x4-Usage, 0x8-Latency
		   ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
					Package()
					{
						Package(2) {"mipi-sdca-input-pin-1", "E080"}, //MU 26
						Package(2) {"mipi-sdca-powerdomain-entity", "E051"}, //PDE 23
						Package(2) {"mipi-sdca-control-0x4-subproperties", "C054"},
						Package(2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
					}
        }
	}) //End AF04.E019

	Name(C054, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // Usage, Class, DC:60h
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0x60},
					}
				}) //End AF04.C054

	Name(E023, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x03},
           Package (2) {"mipi-sdca-entity-label", "OT 27"}, //Secondery transducer
           Package (2) {"mipi-sdca-terminal-type", 0x189},
		   Package (2) {"mipi-sdca-input-pin-list", 0x2},
		   Package (2) {"mipi-sdca-control-list", 0x110 },  // Bitmap: 0x4-Usage, 0x8-Latency
		ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
					Package()
					{
						Package(2) {"mipi-sdca-input-pin-1", "E080"}, //MU 26
						Package(2) {"mipi-sdca-powerdomain-entity", "E039"}, //PDE 27
						Package(2) {"mipi-sdca-control-0x4-subproperties", "C154"},
						Package(2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
					},
        }
	}) //End AF04.E023

	Name(C154, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // Usage, Class, DC:60h
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0x60},
					}
				}) //End AF04.C154


	Name(E020, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x03},
           Package (2) {"mipi-sdca-entity-label", "OT 24"}, //Speaker Sense
           Package (2) {"mipi-sdca-terminal-type", 0x280},
		   Package (2) {"mipi-sdca-input-pin-list", 0x2},
		   Package (2) {"mipi-sdca-control-list", 0x00020110}, // Bitmap: 0x4 - Usage, 0x8 - Latency, 0x11 - DataPortIndex
		   ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
					Package()
					{
						Package(2) {"mipi-sdca-input-pin-1", "E082"}, //IT 29
						Package(2) {"mipi-sdca-control-0x4-subproperties", "C340"},
						Package(2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
						Package(2) {"mipi-sdca-control-0x11-subproperties", "C341"},
					},
			ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
					Package()
					{
						Package(2) {"mipi-sdca-terminal-dp-numbers","BUFA"},
					}
        }
	}) //End AF04.E020

	Name(BUFA, Buffer()
				{
					0x0, 0x2, // DP_Index: 0x0 DP_Num: 0x2
				}) //End AF04.BUFA

	Name(C340, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // Usage, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0x264}, // Usage CBN, 612 (0x264) IV-Sense (2 Transducer)
					}
				}) //End AF04.C340

	Name(C341, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // DataPortIndex, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 1},
					}
				}) //End AF04.C341

	Name(E021, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x03},
           Package (2) {"mipi-sdca-entity-label", "OT 25"}, //Reference stream
           Package (2) {"mipi-sdca-terminal-type", 0x0188},
		   Package (2) {"mipi-sdca-input-pin-list", 0x2},
		   Package (2) {"mipi-sdca-control-list", 0x00020110}, // Bitmap: 0x4 - Usage, 0x8 - Latency, 0x11 - DataPortIndex
		ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
					Package()
					{
						Package(2) {"mipi-sdca-input-pin-1", "E080"}, //MU 26
						Package(2) {"mipi-sdca-terminal-clock-connection", "E033"},
						Package(2) {"mipi-sdca-control-0x4-subproperties", "C440"},
						Package(2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
						Package(2) {"mipi-sdca-control-0x11-subproperties", "C441"},
					},
		ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
					Package()
					{
						Package(2) {"mipi-sdca-terminal-dp-numbers","BUF8"},
					}
        }
	}) //End AF04.E021

	Name(BUF8, Buffer()
				{
					0x0, 0x4, // DP_Index: 0x0 DP_Num: 0x4
				}) //End AF04.BUF8

	Name(C440, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // Usage, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0x2BC}, // Usage CBN, 700 (0x2BC) Reference Stream
					}
				}) //End AF04.C440

	Name(C441, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // DataPortIndex, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 1},
					}
				}) //End AF04.C441

	Name(E056, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
           Package (2) {"mipi-sdca-entity-type", 0x03},
           Package (2) {"mipi-sdca-entity-label", "OT 127"}, //Companion Data Output
           Package (2) {"mipi-sdca-terminal-type", 0x192},
		   Package (2) {"mipi-sdca-input-pin-list", 0x2},
		   Package (2) {"mipi-sdca-control-list", 0x00020110}, // Bitmap: 0x4 - Usage, 0x8 - Latency, 0x11 - DataPortIndex
		ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
					Package()
					{
						Package(2) {"mipi-sdca-input-pin-1", "E080"}, //MU 26
						Package(2) {"mipi-sdca-control-0x4-subproperties", "C540"},
						Package(2) {"mipi-sdca-control-0x8-subproperties", "LC00"},
						Package(2) {"mipi-sdca-control-0x11-subproperties", "C541"},
					},
		ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
					Package()
					{
						Package(2) {"mipi-sdca-terminal-dp-numbers","BUF6"},
					}
        }
	}) //End AF04.E056
	Name(BUF6, Buffer()
				{
					0x0, 0x6, // DP_Index: 0x0 DP_Num: 0x6
				}) //End AF04.BUF6

	Name(C540, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // Usage, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0x32A}, // Usage CBN, 810 (0x32A) Companion Amp Data Out
					}
				}) //End AF04.C540

	Name(C541, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // DataPortIndex, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 1},
					}
				}) //End AF04.C541

	Name(LC00, Package()
				{
					ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
					Package()
					{ // Latency, Class, DC
						Package(2) {"mipi-sdca-control-access-layer", 4 },
						Package(2) {"mipi-sdca-control-access-mode", 5},
						Package(2) {"mipi-sdca-control-dc-value", 0},
					}
	}) //End AF04.LC00
}