/** @file
  This is file contain Opregion definition for DG.
  Software SCI interface between system BIOS, ASL code, and Graphics drivers.

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
  - OpRegion / Software SCI SPEC
**/
#ifndef _DG_OPREGION_H_
#define _DG_OPREGION_H_

#define DG_HEADER_SIGNATURE        "IntelGraphicsMem"
#define HEADER_OPREGION_VER         0x0202
#define HEADER_SIZE                 0x2000
#define HEADER_OPREGION_REV         0x00
#define HD_MBOX1                    BIT0
#define HD_MBOX2                    BIT1
#define HD_MBOX3                    BIT2
#define HD_MBOX4                    BIT3
#define HD_MBOX5                    BIT4
#define HEADER_MBOX_SUPPORT         (HD_MBOX5 + HD_MBOX4 + HD_MBOX3 + HD_MBOX1)

#define BACKLIGHT_BRIGHTNESS  0xFF
#define INIT_BRIGHT_LEVEL     0x64
#define FIELD_VALID_BIT       BIT31
#define WORD_FIELD_VALID_BIT  BIT15

/**
  OpRegion structures:
  Sub-structures define the different parts of the OpRegion followed by the
  main structure representing the entire OpRegion.

  @note These structures are packed to 1 byte offsets because the exact
  data location is required by the supporting design specification due to
  the fact that the data is used by ASL and Graphics driver code compiled
  separately.
**/
#pragma pack(1)
///
/// OpRegion Mailbox 0 Header structure. The OpRegion Header is used to
/// identify a block of memory as the graphics driver OpRegion.
/// Offset 0x0, Size 0x100
///
typedef struct {
  CHAR8  SIGN[0x10];    ///< Offset 0x00 OpRegion Signature
  UINT32 SIZE;          ///< Offset 0x10 OpRegion Size
  UINT32 OVER;          ///< Offset 0x14 OpRegion Structure Version
  UINT8  SVER[0x20];    ///< Offset 0x18 System BIOS Build Version
  UINT8  VVER[0x10];    ///< Offset 0x38 Video BIOS Build Version
  UINT8  GVER[0x10];    ///< Offset 0x48 Graphic Driver Build Version
  UINT32 MBOX;          ///< Offset 0x58 Supported Mailboxes
  UINT32 DMOD;          ///< Offset 0x5C Driver Model
  UINT32 PCON;          ///< Offset 0x60 Platform Configuration
  CHAR16 DVER[0x10];    ///< Offset 0x64 GOP Version
  UINT8  RM01[0x7C];    ///< Offset 0x84 Reserved Must be zero
} DG_OPREGION_HEADER;

///
/// OpRegion Mailbox 1 - Public ACPI Methods
/// Offset 0x100, Size 0x100
///
typedef struct {
  UINT8  RSVD[0x100];    ///< Offset 0x100 - 0x1FF  BIOS does not uses this section of OpRegion
} DG_OPREGION_MBOX1;

///
/// OpRegion Mailbox 2 - Software SCI Interface
/// Offset 0x200, Size 0x100
///
typedef struct {
  UINT8  UNUSED[0x100];   ///< Offset 0x200 - 0x2FF  Obselete in DG
} DG_OPREGION_MBOX2;

///
/// OpRegion Mailbox 3 - BIOS/Driver Notification - ASLE Support
/// Offset 0x300, Size 0x100
///
typedef struct {
  UINT32 ARDY;          ///< Offset 0x300 Driver Readiness
  UINT32 ASLC;          ///< Offset 0x304 ASLE Interrupt Command / Status
  UINT32 TCHE;          ///< Offset 0x308 Technology Enabled Indicator
  UINT32 ALSI;          ///< Offset 0x30C Current ALS Luminance Reading
  UINT32 BCLP;          ///< Offset 0x310 Requested Backlight Brightness
  UINT32 PFIT;          ///< Offset 0x314 Panel Fitting State or Request
  UINT32 CBLV;          ///< Offset 0x318 Current Brightness Level
  UINT16 BCLM[0x14];    ///< Offset 0x31C Backlight Brightness Levels Duty Cycle Mapping Table
  UINT8  RSVD[0x82];    ///< Offset 0x344 - 0x3C5  BIOS does not uses this section of OpRegion
  UINT8  VRSR;          ///< Offset 0x3C6 VRAM SR supported by driver.
  UINT8  RM32[0x39];    ///< Offset 0x3C7 - 0x3FF  Reserved Must be zero.
} DG_OPREGION_MBOX3;

///
/// OpRegion Mailbox 4 - VBT Video BIOS Table
/// Offset 0x400, Size 0x1800
///
typedef struct {
  UINT8  RVBT[0x1800];  ///< Offset 0x400 - 0x1BFF Raw VBT Data
} DG_OPREGION_MBOX4;

///
/// OpRegion Mailbox 5 - BIOS/Driver Notification - Data storage BIOS to Driver data sync
/// Offset 0x1C00, Size 0x400
///
typedef struct {
  UINT32 PHED;          ///< Offset 0x1C00 Panel Header
  UINT8  BDDC[0x100];   ///< Offset 0x1C04 Panel EDID (DDC data)
  UINT8  RM51[0x2FC];   ///< Offset 0x1D04 - 0x1FFF Reserved Must be zero
} DG_OPREGION_MBOX5;

///
/// DG OpRegion Structure
///
typedef struct {
  DG_OPREGION_HEADER Header; ///< OpRegion header (Offset 0x0, Size 0x100)
  DG_OPREGION_MBOX1  MBox1;  ///< Mailbox 1: Public ACPI Methods (Offset 0x100, Size 0x100)
  DG_OPREGION_MBOX2  Mbox2;  ///< Mailbox 2: Obselete in DG
  DG_OPREGION_MBOX3  MBox3;  ///< Mailbox 3: BIOS to Driver Notification (Offset 0x300, Size 0x100)
  DG_OPREGION_MBOX4  MBox4;  ///< Mailbox 4: Video BIOS Table (VBT) (Offset 0x400, Size 0x1800)
  DG_OPREGION_MBOX5  MBox5;  ///< Mailbox 5: BIOS to Driver Notification Extension (Offset 0x1C00, Size 0x400)
} DG_OPREGION_STRUCTURE;
#pragma pack()

///
/// DG OpRegion Protocol
///
typedef struct {
  DG_OPREGION_STRUCTURE  *OpRegion; ///< DG Operation Region Structure
} DG_OPREGION_PROTOCOL;

#define R_SA_DG_ASLS_OFFSET       0x00FC  ///< ASL Storage
///
/// Native Device Id for DG card in motherboard down configuration
///
#define DG2_DEVICE_ID_1           0x5690
#define DG2_DEVICE_ID_2           0x5691
#define DG2_DEVICE_ID_3           0x5692
#define DG2_DEVICE_ID_4           0x5693
#define DG2_DEVICE_ID_5           0x5694
#define DG2_DEVICE_ID_6           0x5695
#define DG2_DEVICE_ID_7           0x5696
#define DG2_DEVICE_ID_8           0x5697
#define DG2_DEVICE_ID_9           0x5698
#define DG2_DEVICE_ID_19          0x56B0
#define DG2_DEVICE_ID_20          0x56B2
#define DG2_DEVICE_ID_21          0x4F89

#endif
