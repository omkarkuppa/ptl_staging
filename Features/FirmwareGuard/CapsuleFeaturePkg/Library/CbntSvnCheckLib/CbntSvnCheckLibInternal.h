/**@file
  Required definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef __CBNT_SVN_CHECK_LIB_INTERNAL__
#define __CBNT_SVN_CHECK_LIB_INTERNAL__

//
// @todo: Backport to SiliconPkg
//
#define MFT_KEY_USAGE_INDEX_OEM_ACM_MANIFEST      56
#define MFT_KEY_USAGE_INDEX_BOOT_POLICY_MANIFEST  32
#define MFT_KEY_USAGE_INDEX_OEM_BTG_KEY_MANIFEST  54

#pragma pack (1)

typedef struct _ACM_HEADER {
  UINT32 ModuleType;      ///< Module type
  UINT32 HeaderLen;       ///< 4 4 Header length (in multiples of four bytes) (161 for version 0.0)
  UINT32 HeaderVersion;   ///< 8 4 Module format version
  UINT32 ModuleId;        ///< 12 4 Module release identifier
  UINT32 ModuleVendor;    ///< 16 4 Module vendor identifier
  UINT32 Date;            ///< 20 4 Creation date (BCD format: year.month.day)
  UINT32 Size;            ///< 24 4 Module size (in multiples of four bytes)
  UINT16 AcmSvn;          ///< 28 2 ACM-SVN Number
  UINT16 SeSvn;           ///< 30 2 SE-SVN number
  UINT32 CodeControl;     ///< 32 4 Authenticated code control flags
  UINT32 ErrorEntryPoint; ///< 36 4 Error response entry point offset (bytes)
  UINT32 GdtLimit;        ///< 40 4 GDT limit (defines last byte of GDT)
  UINT32 GdtBasePtr;      ///< 44 4 GDT base pointer offset (bytes)
  UINT32 SegSel;          ///< 48 4 Segment selector initializer
  UINT32 EntryPoint;      ///< 52 4 Authenticated code entry point offset (bytes)
  UINT32 Reserved2[16];   ///< 56 64 Reserved for future extensions
  UINT32 KeySize;         ///< 120 4 Module public key size less the exponent  (in multiples of four bytes - 64 for version 0.0)
  UINT32 ScratchSize;     ///< 124 4 Scratch field size (in multiples of four bytes - 2 * KeySize + 15 for version 0.0)
} ACM_HEADER;

#define BOOT_POLICY_MANIFEST_HEADER_STRUCTURE_ID  (*(UINT64 *)"__ACBP__")
#define BOOT_POLICY_MANIFEST_HEADER_VERSION_2_1          0x21
typedef struct {
  UINT8              StructureId[8];
  UINT8              StructVersion;        // 0x21
  UINT8              HdrStructVersion;     // 0x20
  UINT16             HdrSize;              // total number of bytes in Header (i.e., offset to first element)
  UINT16             KeySignatureOffset;   // Offset from start of Bpm to KeySignature field of Signature Element
  UINT8              BpmRevision;
  UINT8              BpmRevocation;
  UINT8              AcmRevocation;
  UINT8              Reserved;
  UINT16             NemPages;
} BOOT_POLICY_MANIFEST_HEADER;

#define KEY_MANIFEST_STRUCTURE_ID  (*(UINT64 *)"__KEYM__")
#define KEY_MANIFEST_STRUCTURE_VERSION_2_1     0x21
typedef struct {
  UINT8               StructureId[8];
  UINT8               StructVersion;       // 0x21
  UINT8               reserved[3];         // 3 bytes to make KeySignatureOffset same offset as for BPM
  UINT16              KeySignatureOffset;  // Offset from start of KM to KeyManifestSignature
  UINT8               Reserved2[3];        // Alignment
  UINT8               KeyManifestRevision;
  UINT8               KmSvn;
  UINT8               KeyManifestId;
  UINT16              KmPubKeyHashAlg;
  UINT16              KeyCount;
} KEY_MANIFEST_STRUCTURE;

#pragma pack ()

#endif
