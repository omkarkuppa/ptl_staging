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

Name(_DSD, Package()
{
    // Device properties UUID
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package() {
        Package(2) { "mipi-sdw-sw-interface-revision", 0x00020001 },  // Revision 2.1
        Package(2) { "mipi-sdca-control-list", (CTL_E0_FUNCTION_VERSION | CTL_E0_FUNCTION_ID | CTL_E0_FUNCTION_MANUFACTURER_ID |
                                                CTL_E0_FUNCTION_TYPE | CTL_E0_FUNCTION_SDCA_VERSION | CTL_E0_FUNCTION_STATUS |
                                                CTL_E0_DEVICE_MANUFACTURER_ID | CTL_E0_DEVICE_PART_ID | CTL_E0_DEVICE_VERSION | CTL_E0_DEVICE_SDCA_VERSION
                                                )}, // Function-Level Controls
        Package(2) { "mipi-sdca-entity-id-list", Package() { 0x1 } }, // HIDE1
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // The hierarchical Data Extension UUID
    Package()
    {
        // Function-level
        Package(2) { "mipi-sdca-control-0x4-subproperties", "C004"},
        Package(2) { "mipi-sdca-control-0x5-subproperties", "C005"},
        Package(2) { "mipi-sdca-control-0x6-subproperties", "C006"},
        Package(2) { "mipi-sdca-control-0x7-subproperties", "C007"},
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C008"},
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C010"},
        Package(2) { "mipi-sdca-control-0x2C-subproperties", "C02C"},
        Package(2) { "mipi-sdca-control-0x2D-subproperties", "C02D"},
        Package(2) { "mipi-sdca-control-0x2E-subproperties", "C02E"},
        Package(2) { "mipi-sdca-control-0x2F-subproperties", "C02F"},

        // Entity-level
        Package(2) { "mipi-sdca-entity-id-0x1-subproperties", "E001"},  // HIDE_101
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"), // The Buffer Data Extension UUID
    Package()
    {
        Package(2) { "mipi-sdca-hid-descriptor", "BUF0"},
        Package(2) { "mipi-sdca-report-descriptor", "BUF1"},
        Package(2) { "mipi-sdca-function-initialization-table", "IBUF"},
    }
}) // End _DSD

// +------------------------------------+
// |    Function level controls         |
// +------------------------------------+

// Function-level control definitions
Name(C004, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_SDCA_Version = 1.0
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", CTL_E0_FUNCTION_SDCA_VERSION_VAL },
    }
}) // End C004

Name(C005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_Type = 0xA (HID)
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0xA},
    }
}) // End C005

Name(C006, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_Manufacturer_Id = 0x01FA
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x01FA},
    }
}) // End C006

Name(C007, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_Id = 0x4245
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x4245},
    }
}) // End C007

Name(C008, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Shows up as VER in HWID
        // Class, DC, Function_Version = 0x1
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x1},
    }
}) // End C008

Name(C010, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Class, RW1C, Function_Status
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_RW1C},
        Package(2) { "mipi-sdca-control-interrupt-position", PHIFE_SDCA_HID_FUNC_STATUS_INT},
    }
}) // End C010


Name(C02C, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Device_Manufacturer_ID: Class, DC = 0x01FA
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x01FA},
    }
}) // End C02C


Name(C02D, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Device_Part_ID: Class, DC = 0x4245
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x4245},
    }
}) // End C02D


Name(C02E, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Device_Version: Class, DC = 0xA1
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0xA1},
    }
}) // End C02E


Name(C02F, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Device_SDCA_Version: Class, DC = 0x10
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", CTL_E0_DEVICE_SDCA_VERSION_VAL},
    }
}) // End C02F


// +------------------------------------+
// |                HIDE1               |
// +------------------------------------+
Name(E001, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x31 },
        Package(2) { "mipi-sdca-entity-label", "HIDE1"},
        Package(2) { "mipi-sdca-hide-related-audio-function", Package() {0x3}}, // AF03 - UAJ
        //Package(2) { "mipi-sdca-hide-tx-report-type", Package() {0x22}}, // HID Report Descriptor Type, follo with MSFT
        Package(2) { "mipi-sdca-HIDTx-supported-report-ids", Package() {0x1, 0x2}}, // HID report ID = 1, 2
        Package(2) { "mipi-sdca-control-list", 0xD0000}, // HIDTx_???
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x10-subproperties", "CS10"}, // HIDTx_CurrentOwner
        Package(2) { "mipi-sdca-control-0x12-subproperties", "CS12"}, // HIDTx_MessageOffset
        Package(2) { "mipi-sdca-control-0x13-subproperties", "CS13"}, // HIDTx_MessageLength
    }
}) // End E001

Name(CS10, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // HIDTx_CurrentOwner, Class, RO
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_RO},
        Package(2) { "mipi-sdca-control-interrupt-position", PHIFE_SDCA_HID_FDL_BUFFER_OWNER_CHNG_INT},   // HID interrupt bit position   // ATR Phife: Make a define
    }
}) // End CS10

Name(CS12, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // HIDTx_MessageOffset, Class, DC
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x0},   // Begining of the UMP buffer
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package (2) {"mipi-sdca-control-range", "BUF2"},     // UMP buffer
    }
}) // End CS12

Name(BUF2, Buffer() {
    0x03, 0x00,  // Range type 0x0003
    0x01, 0x00,  // Count of ranges = 0x1
    0xe8, 0xfd, 0x11, 0x00,  // Buffer Start = 0x0011fde8
    0x04, 0x00, 0x00, 0x00,  // Buffer Length = 0x4
    0x00, 0x00, 0x00, 0x00,  // UMP Type = 0 (Direct)
}) //End BUF2

Name(CS13, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // HIDTx_MessageLength, Class, DC
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x4},   // 4B
    }
}) // End CS13

//
// HID descriptor
//
Name(BUF0, Buffer()
{
    0x09,           // HID_DESCRIPTOR::bLength = 0x9
    0x21,           // HID_DESCRIPTOR::bDescriptorType = 0x21 (HID)
    0x00, 0x01,     // HID_DESCRIPTOR::bcdHID = 0x0100 (1.0)
    0x00,           // HID_DESCRIPTOR::bCountry = 0x00 (N/A)
    0x01,           // HID_DESCRIPTOR::bNumDescriptors = 0x01
        0x22,       // HID_DESCRIPTOR::DescriptorList[0].bReportType = 0x22 (HID Report Descriptor)
        0x6a, 0x00, // HID_DESCRIPTOR::DescriptorList[0].wReportLength = 0x6a (106 bytes)
})  // End BUF0

//
// Raport1 format
//      7      6      5       4      3      2      1     0
//   +------+------+------+------+------+------+------+------+
// 0 |                     Raport Id = 1                     |
//   +------+------+------+------+------+------+------+------+
// 1 | N/A  | Mute | Prev | Vol- | Vol+ | VCMD | Next | P/P  |
//   +------+------+------+------+------+------+------+------+
// 2 |                          N/A                          |
//   +------+------+------+------+------+------+------+------+
// 3 |                          N/A                          |
//   +------+------+------+------+------+------+------+------+

//
// Raport2 format
//       7      6      5       4      3      2      1     0
//   +------+------+------+------+------+------+------+------+
// 0 |                     Raport Id = 2                     |
//   +------+------+------+------+------+------+------+------+
// 1 |                          N/A                   | Mute |
//   +------+------+------+------+------+------+------+------+
// 2 |                          N/A                          |
//   +------+------+------+------+------+------+------+------+
// 3 |                          N/A                          |
//   +------+------+------+------+------+------+------+------+
//
Name(BUF1, Buffer()
{
    0x05, 0x0c,     // Usage Page (Consumer)
    0x09, 0x01,     // Usage (Consumer Control)
    0xa1, 0x01,     // Collection (Application)
        0x85, 0x01, // Report Id (1)
        0x15, 0x00, // Logical minimum (0)
        0x25, 0x01, // Logical maximum (1)
        0x09, 0xcd, // Usage (Play/Pause)
        0x75, 0x01, // Report Size (1)
        0x95, 0x01, // Report Count (1)
        0x81, 0x06, // Input (Data,Value,Relative,Preferred,Bit Field)
        0x09, 0xb5, // Usage (Scan Next Track)
        0x75, 0x01, // Report Size (1)
        0x95, 0x01, // Report Count (1)
        0x81, 0x06, // Input (Data,Value,Relative,Preferred,Bit Field)
        0x09, 0xcf, // Usage (Voice Command)
        0x75, 0x01, // Report Size (1)
        0x95, 0x01, // Report Count (1)
        0x81, 0x06, // Input (Data,Value,Relative,Preferred,Bit Field)
        0x09, 0xe9, // Usage (Volume Up)
        0x75, 0x01, // Report Size (1)
        0x95, 0x01, // Report Count (1)
        0x81, 0x02, // Input (Data,Value,Absolute,Preferred,Bit Field)
        0x09, 0xea, // Usage (Volume Down)
        0x75, 0x01, // Report Size (1)
        0x95, 0x01, // Report Count (1)
        0x81, 0x02, // Input (Data,Value,Absolute,Preferred,Bit Field)
        0x09, 0xb6, // Usage (Scan Previous Track)
        0x75, 0x01, // Report Size (1)
        0x95, 0x01, // Report Count (1)
        0x81, 0x06, // Input (Data,Value,Relative,Preferred,Bit Field)
        0x09, 0xe2, // Usage (Mute)
        0x75, 0x01, // Report Size (1)
        0x95, 0x01, // Report Count (1)
        0x81, 0x06, // Input (Data,Value,Relative,Preferred,Bit Field)
        0x09, 0x00, // Usage (none)
        0x75, 0x11, // Report Size (17)
        0x95, 0x01, // Report Count (1)
        0x81, 0x03, // Input (Constant,Variable), padding to the byte
    0xc0,           // End Collection

    0x05, 0x0b,     // Usage Page (Telephony)
    0x09, 0x05,     // Usage (Headset)
    0xa1, 0x01,     // Collection (Application)
        0x85, 0x02, // Report Id (2)
        0x15, 0x00, // Logical minimum (0)
        0x25, 0x01, // Logical maximum (1)
        0x09, 0x2f, // Usage (Phone Mute)
        0x75, 0x01, // Report Size (1)
        0x95, 0x01, // Report Count (1)
        0x81, 0x06, // Input (Data,Value,Relative,Preferred,Bit Field)
        0x09, 0x00, // Usage (none)
        0x75, 0x17, // Report Size (23)
        0x95, 0x01, // Report Count (1)
        0x81, 0x03, // Input (Constant,Variable), padding to the byte
    0xc0            // End Collection
})  // End BUF1


// Function initialization-table
Name(IBUF, Buffer()
{
    //
    // FDL config
    //
    0xB8, 0xFD, 0x11, 0x00, 0x01,  // 0x0011FDB8 = 0x1 (FILE_SET_5)
    0xB9, 0xFD, 0x11, 0x00, 0x00,
    0xBA, 0xFD, 0x11, 0x00, 0x00,
    0xBC, 0xFD, 0x11, 0x00, 0x00,

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
}) // End IBUF
