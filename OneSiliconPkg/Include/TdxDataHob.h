/** @file
  This file contains definitions required for creation of TDX Data HOB.

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

#ifndef _TDX_DATA_HOB_H_
#define _TDX_DATA_HOB_H_

#include <Base.h>
#include <CMrcInterface.h>

extern EFI_GUID gTdxDataHobGuid;

#define TDX_DATA_HOB_REVISION  1

//
// Definition for CMR table
//
#define MCHECK_MAX_CMR_ENTRIES               32
#define UUID_BUFFER_SIZE                     16
#define RESERVED_BUFFER_LENGTH               12
#define MAXIMUM_NUMBER_OF_RANKS              ( MAX_CONTROLLER * MAX_DDR5_CHANNEL * MAX_DIMMS_IN_CHANNEL * MAX_RANK_IN_DIMM )
#define SIZE_OF_MANIFEST_MAC                 ( 32 ) // 32 bytes
#define MAXIMUM_SIZE_OF_MANIFEST             ( sizeof(DIMM_DESCRIPTION) * MAXIMUM_NUMBER_OF_RANKS + SIZE_OF_MANIFEST_MAC )

typedef struct {
  UINT64 CmrBase; // Base address of CMR, aligned to 4KB, bits 11:0 are always 0
  UINT64 CmrSize; // Size of CMR in bytes, aligned to 4KB, bits 11:0 are always 0
} MCHECK_CMR; // Total: 16 bytes

typedef struct {
  // TDX 1V0
  MCHECK_CMR             Cmr[MCHECK_MAX_CMR_ENTRIES];
  UINT32                 NumCmrArrayEntries; // Actual number of populated CMRs
} MCHECK_PARAM_INFO_SEAM_DATA;

#pragma pack (push, 1)

///
/// Header for DIMM manifest
///
typedef struct {
  UINT8   Uuid[UUID_BUFFER_SIZE];
  UINT16  Size;
  UINT16  Version;
  UINT8   Reserved[RESERVED_BUFFER_LENGTH];
} ACTM_HEADER_DESCRIPTION;

///
/// Data for DIMM manifest
///
typedef struct {
  UINT8 MemoryController;
  UINT8 Channel;
  UINT8 Dimm;
  UINT8 Rank;
  UINT8 SubRankBits;
  UINT8 BankGroupBits;
  UINT8 BankAddressBits;
  UINT8 ColumnBits;
  UINT8 RowBits;
  UINT8 Asymmetric;
  UINT8 ModeRegister58;
  UINT8 ModeRegister59;
} DIMM_DESCRIPTION;

typedef struct {
  ACTM_HEADER_DESCRIPTION  Header;
  UINT8                    ManifestBlob[MAXIMUM_SIZE_OF_MANIFEST];
} ACTM_DIMM_MANIFEST;
#pragma pack (pop)

#pragma pack(push, 1)
///
/// TDX Hob data.
///
typedef struct _TDX_DATA {
  EFI_HOB_GUID_TYPE            EfiHobGuidType;    /// EFI Hob Guid Type.
  /**
     This member specifies the revision of the TDX Data HOB. This field is used
     to indicate back wards compatible changes to the protocol. Platform code that
     consumes this protocol must read the correct revision value to correctly interpret
     the content of the protocol fields.
  **/
  UINT8                       Revision;
  UINT32                      TdxEnabled;        /// 0: Not Enabled, 1: Enabled
  ACTM_DIMM_MANIFEST          ActmDimmManifest;
  MCHECK_PARAM_INFO_SEAM_DATA CmrList;
  UINT32                      SeamrrSize;
  UINT64                      SeamRrBaseAddress;
} TDX_DATA_HOB;

#pragma pack(pop)

#endif // _TDX_DATA_HOB_H_
