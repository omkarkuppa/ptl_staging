/** @file
  Definitions of Startup ACM image.

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

#ifndef _ACM_IMAGE_PARSER_H_
#define _ACM_IMAGE_PARSER_H_

#include <Uefi.h>

#pragma pack (1)

#define ACM_INFO_TABLE_VERSION_9  0x9
#define ACM_PROCESSOR_ID_LIST_ID  SIGNATURE_32 ('L', 'U', 'P', 'C')

#define ACM_HEADER_LEN_MULTIPLES_VALUE    4
#define ACM_SCRATCH_SIZE_MULTIPLES_VALUE  4

typedef struct _ACM_HEADER {
  UINT32    ModuleType;       ///< Module type
  UINT32    HeaderLen;        ///< 4 4 Header length (in multiples of four bytes) (161 for version 0.0)
  UINT32    HeaderVersion;    ///< 8 4 Module format version
  UINT32    ModuleId;         ///< 12 4 Module release identifier
  UINT32    ModuleVendor;     ///< 16 4 Module vendor identifier
  UINT32    Date;             ///< 20 4 Creation date (BCD format: year.month.day)
  UINT32    Size;             ///< 24 4 Module size (in multiples of four bytes)
  UINT16    AcmSvn;           ///< 28 2 ACM-SVN Number
  UINT16    SeSvn;            ///< 30 2 SE-SVN number
  UINT32    CodeControl;      ///< 32 4 Authenticated code control flags
  UINT32    ErrorEntryPoint;  ///< 36 4 Error response entry point offset (bytes)
  UINT32    GdtLimit;         ///< 40 4 GDT limit (defines last byte of GDT)
  UINT32    GdtBasePtr;       ///< 44 4 GDT base pointer offset (bytes)
  UINT32    SegSel;           ///< 48 4 Segment selector initializer
  UINT32    EntryPoint;       ///< 52 4 Authenticated code entry point offset (bytes)
  UINT32    Reserved2[16];    ///< 56 64 Reserved for future extensions
  UINT32    KeySize;          ///< 120 4 Module public key size less the exponent  (in multiples of four bytes - 64 for version 0.0)
  UINT32    ScratchSize;      ///< 124 4 Scratch field size (in multiples of four bytes - 2 * KeySize + 15 for version 0.0)
} ACM_HEADER;

typedef struct {
  EFI_GUID    Guid;
  UINT8       ChipsetAcmType;
  UINT8       AitVersion;
  UINT16      AitLength;
  UINT32      ChipsetIdList;
  UINT32      OsSinitTblVer;
  UINT32      MinMleHeaderVer;
  UINT32      SinitCaps;
  UINT8       SinitVersion;
  UINT8       AitRevision[3];
  UINT32      ProcessorIdList;
  UINT32      TpmInfoList;
} ACM_INFO_TABLE;

typedef struct {
  UINT32    Fms;
  UINT32    FmsMask;
  UINT64    PlatformID;
  UINT64    PlatformMask;
} ACM_PROCESSOR_ID;

//
// Offset get record located in the ProcessorIDList is point to the
// Count field inside the PROCESSOR_ID_LIST. (For backward comptability.)
// Need to minus back this bytes size to point to correct structure.
//
#define PROCESSOR_ID_LIST_HEADER_PREPRENDED_SIZE  12

//
// Acm Information Table Version (AitVersion) 9:
// offset from the beginning of the AC Module to the "count" field
// of the Chipset ID list for backward compatibility.
// Prepend the "header" part (TableId, Length, and Revision fields) in ver.9.
//
typedef struct {
  UINT32              TableId;
  UINT32              Length;
  UINT32              Revision;
  UINT32              Count;
  ACM_PROCESSOR_ID    ProcessorID[1];
} PROCESSOR_ID_LIST;

#pragma pack ()

/**
  Check if the input ACM buffer is supported on current CPU.

  @param[in]  Image      Pointer that point to ACM image buffer.
  @param[in]  ImageSize  Size of ACM image buffer.

  @retval  TRUE   Input ACM buffer is supported on current CPU.
  @retval  FALSE  Input ACM buffer is not supported on current CPU.

**/
BOOLEAN
EFIAPI
IsSAcmImageForCurrentCpu (
  IN CONST VOID   *Image,
  IN       UINTN  ImageSize
  );

#endif
