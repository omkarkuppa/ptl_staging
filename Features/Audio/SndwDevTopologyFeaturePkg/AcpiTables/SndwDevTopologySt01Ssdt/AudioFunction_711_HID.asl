/** @file
  SoundWire topology definitions

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

Name (_DSD, Package ()  // _DSD: Device-Specific Data
{
  ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),  //Device Properties for _DSD
  Package ()
  {
    Package (2) {"mipi-sdw-sw-interface-revision", 0x00010001},
    Package (2) {"msft-sdca-spec-revision", 0x4},
    Package (2) {"mipi-sdca-control-selector-list",
      Package() {0x4, 0x5, 0x6, 0x7, 0x8} },
    Package (2) {"mipi-sdca-entity-id-list",
      Package() {0x1} },
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package(2) {"mipi-sdca-controlselector-0x4-subproperties", "C004"},
    Package(2) {"mipi-sdca-controlselector-0x5-subproperties", "C005"},
    Package(2) {"mipi-sdca-controlselector-0x6-subproperties", "C006"},
    Package(2) {"mipi-sdca-controlselector-0x7-subproperties", "C007"},
    Package(2) {"mipi-sdca-controlselector-0x8-subproperties", "C008"},
    Package(2) {"mipi-sdca-entity-id-0x1-subproperties", "E001"},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package()
  {
    Package (2) {"mipi-sdca-function-initialization-table", "BUF0"},
    Package (2) {"mipi-sdca-hid-descriptor", "BUF1"},
    Package (2) {"mipi-sdca-report-descriptor", "BUF2"},
  }
}) //End _DSD

Name(BUF0, Buffer()
{
  0x11, 0x20, 0x00, 0x02, 0x04,  // 0x02002011, 0x04
  0x11, 0x00, 0x00, 0x02, 0x7a,  // 0x02000011, 0x7a

  0x08, 0x20, 0x00, 0x02, 0xa8,  // 0x02002008, 0xa8
  0x08, 0x00, 0x00, 0x02, 0x07,  // 0x02000008, 0x07

  0x09, 0x20, 0x00, 0x02, 0x30,  // 0x02002009, 0x30
  0x09, 0x00, 0x00, 0x02, 0x2b,  // 0x02000009, 0x2b

  0x4a, 0x20, 0x00, 0x02, 0x20,  // 0x0200204a, 0x20
  0x4a, 0x00, 0x00, 0x02, 0x1b,  // 0x0200004a, 0x1b

  0x35, 0x20, 0x10, 0x06, 0x8c,  // 0x06102035, 0x8c
  0x35, 0x00, 0x10, 0x06, 0x60,  // 0x06100035, 0x60

  0x3c, 0x20, 0x10, 0x06, 0x8f,  // 0x0610203c, 0x8f
  0x3c, 0x00, 0x10, 0x06, 0x00,  // 0x0610003c, 0x00

  0x3d, 0x20, 0x10, 0x06, 0x00,  // 0x0610203d, 0x00
  0x3d, 0x00, 0x10, 0x06, 0x0f,  // 0x0610003d, 0x0f

  0x38, 0x20, 0x10, 0x06, 0x11,  // 0x06102038, 0x11
  0x38, 0x00, 0x10, 0x06, 0x00,  // 0x06100038, 0x00

  0x3f, 0x20, 0x10, 0x06, 0x0c,  // 0x0610203f, 0x0c
  0x3f, 0x00, 0x10, 0x06, 0x12,  // 0x0610003f, 0x12

}) //End BUF0

Name(BUF1, Buffer()
{
  0x09,   // size of decriptor
  0x21,   // HID descriptor type
  0x11,   // HID class spec release number BCD LO
  0x01,   // HID class spec release number BCD HI
  0x00,   // country code
  0x01,   // number of HID class descripotr to follow
  0x22,   // Report Type
  0x33,   // CLASS DESC[0] LENGTH Lo
  0x00,   // CLASS DESC[0] LENGTH Hi
}) //End BUF1

Name(BUF2, Buffer()
{
  0x05, 0x0C,       // USAGE_PAGE (Consumer devices)
  0x09, 0x01,       // USAGE (Consumer Control)
  0xa1, 0x01,       // COLLECTION (Application)
      0x85, 0x11,   // REPORT_ID

    0x09, 0xe9,   // USAGE (Volume up) 08
    0x15, 0x00,   // LOGICAL_MINIMUM (0)
    0x25, 0x01,   // LOGICAL_MAXIMUM (1)
    0x75, 0x01,   // REPORT_SIZE (1)
    0x95, 0x01,   // REPORT_COUNT (1)
    0x81, 0x02,   // INPUT (Data, Var, Abs)

    0x09, 0xea,   // USAGE (Volume down) 09
    0x15, 0x00,   // LOGICAL_MINIMUM (0)
    0x25, 0x01,   // LOGICAL_MAXIMUM (1)
    0x75, 0x01,   // REPORT_SIZE (1)
    0x95, 0x01,   // REPORT_COUNT (1)
    0x81, 0x02,   // INPUT (Data, Var, Abs)

    0x09, 0xcd,   // USAGE (play/pause) 10
    0x15, 0x00,   // LOGICAL_MINIMUM (0)
    0x25, 0x01,   // LOGICAL_MAXIMUM (1)
    0x75, 0x01,   // REPORT_SIZE (1)
    0x95, 0x01,   // REPORT_COUNT (1)
    0x81, 0x06,   // INPUT (Data, Var, Relative)

    0x75, 0x0D,   // REPORT_SIZE (1) 11~23
    0x95, 0x01,   // REPORT_COUNT (1)
    0x81, 0x03,   // INPUT (Cnst,Ary,Abs)
  0xC0,             // MAIN::END_COLLECTION

}) //End BUF2

Name(C004, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {
    // Function_SDCA_Version = 0.6, Class, DC
    Package(2) {"mipi-sdca-control-selector-access-layer", 4},
    Package(2) {"mipi-sdca-control-selector-access-mode", 5},
    Package(2) {"mipi-sdca-control-number-dc-value", 0x6},
   }
}) // End C004

Name(C005, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {    // Function_Topology = 0x6 HID, Class, DC
    Package(2) {"mipi-sdca-control-selector-access-layer", 4},
    Package(2) {"mipi-sdca-control-selector-access-mode", 5},
    Package(2) {"mipi-sdca-control-number-dc-value", 0x6},
  }
}) // End C005

Name(C006, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {
    Package(2) {"mipi-sdca-control-selector-access-layer", 4},
    Package(2) {"mipi-sdca-control-selector-access-mode", 5},
    Package(2) {"mipi-sdca-control-number-dc-value", 0x025d},
  }
}) // End C006

Name(C007, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {
    Package(2) {"mipi-sdca-control-selector-access-layer", 4},
    Package(2) {"mipi-sdca-control-selector-access-mode", 5},
    Package(2) {"mipi-sdca-control-number-dc-value", 0x0711},
  }
}) // End C007

Name(C008, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package()
  {
    // Function_Version = 0x1, Class, DC
    Package(2) {"mipi-sdca-control-selector-access-layer", 4},
    Package(2) {"mipi-sdca-control-selector-access-mode", 5},
    Package(2) {"mipi-sdca-control-number-dc-value", 0x1},
  }
}) // End C008

Name(E001, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {
    Package (2) {"mipi-sdca-entity-type", 0x31},
    Package (2) {"mipi-sdca-control-selector-list", Package() {0x10, 0x11, 0x12, 0x13} },
    Package (2) {"mipi-sdca-HIDTx-supported-report-ids", Package() {0x11} },   // HIDE 1 TX Routing Table
  },
  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
  Package ()
  {
    Package (2) {"mipi-sdca-controlselector-0x10-subproperties", "CS10"},
    Package (2) {"mipi-sdca-controlselector-0x11-subproperties", "CS11"},
    Package (2) {"mipi-sdca-controlselector-0x12-subproperties", "CS12"},
    Package (2) {"mipi-sdca-controlselector-0x13-subproperties", "CS13"},
  }
}) //End E001

Name(CS10, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // HIDTx_CurrentOwner, Class, RO
    Package (2) {"mipi-sdca-control-selector-interrupt-position", 8},// bit 8 in SDCA interrupt
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 3},
  }
}) //End CS10

Name(CS11, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // HIDTx_SetOwnerToDevice, Class, W1S
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 4},
  }
}) //End CS11

Name(CS12, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // HIDTx_MessageOffset, Class, RO
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 3},
  },
  ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
  Package() {
    Package (2) {"mipi-sdca-control-number-range", "BUF3"},
  }
}) //End CS12

Name(BUF3, Buffer() {
    0x03, 0x00,  // Range type 0x0003
    0x01, 0x00,  // Count of ranges = 0x1
    0x00, 0x00, 0x03, 0x44,  // Buffer Start = 0x44030000
    0x18, 0x00, 0x00, 0x00,  // Buffer Length = 0x18
    0x00, 0x00, 0x00, 0x00,  // UMP Type = 0 (Direct)
}) //End BUF3

Name(CS13, Package()
{
  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
  Package ()
  {  // HIDTx_MessageLength, Class, RO
    Package (2) {"mipi-sdca-control-selector-access-layer", 4},
    Package (2) {"mipi-sdca-control-selector-access-mode", 3},
  }
}) //End CS13

