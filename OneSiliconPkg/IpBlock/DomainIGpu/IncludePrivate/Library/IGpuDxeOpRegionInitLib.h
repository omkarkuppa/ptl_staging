/** @file
  This is part of the implementation of an Intel IGpu drivers OpRegion /
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
**/
#ifndef _IGPU_DXE_OPREGION_INIT_LIB_H_
#define _IGPU_DXE_OPREGION_INIT_LIB_H_

///
/// Statements that include other header files.
///
#include <Uefi.h>
#include <IndustryStandard/Pci.h>
#include <Defines/IgdDefines.h>
#include <SiConfigHob.h>
#include <Defines/HostBridgeDefines.h>
///
/// Driver Consumed Protocol Prototypes
///
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
///
/// Driver Produced Protocol Prototypes
///
#include <Protocol/IgdOpRegion.h>

#pragma pack(push, 1)
///
///
/// OpRegion (Miscellaneous) defines.
///
/// OpRegion Header defines.
///
typedef UINT16  STRING_REF;
#define HEADER_SIGNATURE            "IntelGraphicsMem"
#define HEADER_SIZE                 0x2000
#define HEADER_OPREGION_REV         0x00
#define HEADER_MBOX_SUPPORT         (HD_MBOX5 + HD_MBOX4 + HD_MBOX3 + HD_MBOX2 + HD_MBOX1)
#define HD_MBOX1                    IGD_OPREGION_HEADER_MBOX1
#define HD_MBOX2                    IGD_OPREGION_HEADER_MBOX2_VER_3_0
#define HD_MBOX3                    IGD_OPREGION_HEADER_MBOX3
#define HD_MBOX4                    IGD_OPREGION_HEADER_MBOX4
#define HD_MBOX5                    IGD_OPREGION_HEADER_MBOX5
#define SVER_SIZE                   32

///
/// OpRegion Mailbox 1 EQUates.
///
/// OpRegion Mailbox 3 EQUates.
///
#define ALS_ENABLE            BIT0
#define BACKLIGHT_BRIGHTNESS  0xFF
#define FIELD_VALID_BIT       BIT31
#define INIT_BRIGHT_LEVEL     0x64

#define VBT_SIGNATURE           SIGNATURE_32 ('$', 'V', 'B', 'T')
///
/// Typedef stuctures
///
typedef struct {
  UINT16  Signature;  /// 0xAA55
  UINT8   Size512;
  UINT8   Reserved[21];
  UINT16  PcirOffset;
  UINT16  VbtOffset;
} INTEL_VBIOS_OPTION_ROM_HEADER;

typedef struct {
  UINT32  Signature;  /// "PCIR"
  UINT16  VendorId;   /// 0x8086
  UINT16  DeviceId;
  UINT16  Reserved0;
  UINT16  Length;
  UINT8   Revision;
  UINT8   ClassCode[3];
  UINT16  ImageLength;
  UINT16  CodeRevision;
  UINT8   CodeType;
  UINT8   Indicator;
  UINT16  Reserved1;
} INTEL_VBIOS_PCIR_STRUCTURE;

typedef struct {
  UINT8   HeaderSignature[20];
  UINT16  HeaderVersion;
  UINT16  HeaderSize;
  UINT16  HeaderVbtSize;
  UINT8   HeaderVbtCheckSum;
  UINT8   HeaderReserved;
  UINT32  HeaderOffsetVbtDataBlock;
  UINT32  HeaderOffsetAim1;
  UINT32  HeaderOffsetAim2;
  UINT32  HeaderOffsetAim3;
  UINT32  HeaderOffsetAim4;
  UINT8   DataHeaderSignature[16];
  UINT16  DataHeaderVersion;
  UINT16  DataHeaderSize;
  UINT16  DataHeaderDataBlockSize;
  UINT8   CoreBlockId;
  UINT16  CoreBlockSize;
  UINT16  CoreBlockBiosSize;
  UINT8   CoreBlockBiosType;
  UINT8   CoreBlockReleaseStatus;
  UINT8   CoreBlockHWSupported;
  UINT8   CoreBlockIntegratedHW;
  UINT8   CoreBlockBiosBuild[4];
  UINT8   CoreBlockBiosSignOn[155];
} VBIOS_VBT_STRUCTURE;
#pragma pack(pop)
///
/// Driver Private Function definitions
///

/**
  IGPU OpRegion / Software SCI driver installation function.

  @retval EFI_SUCCESS     - The driver installed without error.
  @retval EFI_ABORTED     - The driver encountered an error and could not complete
                            installation of the ACPI tables.
**/
EFI_STATUS
IGpuOpRegionInit (
  VOID
  );

/**
  Update IGPU OpRegion after PCI enumeration.

  @retval EFI_SUCCESS     - The function completed successfully.
**/
EFI_STATUS
IGpuUpdateOpRegion (
  VOID
  );
#endif
