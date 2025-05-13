/** @file
  This file contains structures for BSSS FSP Element.

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

#ifndef __BSSS_FSPE_DEF_H__
#define __BSSS_FSPE_DEF_H__

#include <BpmDef.h>
#include <FsptUpd.h>

#define BSP_ENTRY_POINT_PTR(FspElement)  \
  (UINT32 *)((UINT8 *)(FspElement) + 8 * sizeof (UINT8) + sizeof (UINT8) + sizeof (UINT16) + sizeof (UINT8))

#define SEGMENT_ARRAY_PTR(FspElement)  \
  (REGION_SEGMENT *)((UINT8 *)(FspElement) + sizeof (BSPM_ELEMENT))

#define FSPM_UPD_DIGEST_PTR(FspElement, BspSegCount, FspUpdSegCount)  \
  (UINT8 *)(FspElement) + sizeof (BSPM_ELEMENT) + BspSegCount * sizeof (REGION_SEGMENT) + BspSegCount * BSPM_SUPPORTED_SHAX_HASHSTRUCTS_SIZE + FspUpdSegCount * sizeof (REGION_SEGMENT)
   
#define FSPS_UPD_DIGEST_PTR(FspElement, BspSegCount, FspUpdSegCount)  \
  FSPM_UPD_DIGEST_PTR(FspElement, BspSegCount, FspUpdSegCount) + BSPM_SUPPORTED_SHAX_HASHSTRUCTS_SIZE

#define BSPM_SUPPORTED_SHAX_HASHSTRUCTS_SIZE  \
  sizeof (SHA384_HASH_STRUCTURE) + sizeof (SHA1_HASH_STRUCTURE) + sizeof (SHA256_HASH_STRUCTURE) + sizeof (SHA256_HASH_STRUCTURE)

//
// Data structure definitions
//
#pragma pack (1)

//
// FspmLoadingPolicy Bits definition
// BIT0 indicates FSP-M should be loaded by FSP or not:
// 0: FSP skips loading FSP-M
// 1: FSP loads FSP-M
//
// BIT1 indicates FSP-M is compressed or not:
// 0: FSP-M is not compressed
// 1: FSP-M is compressed and need be decompressed before use
//
#define    SKIP_LOADING_FSPM    0
#define    LOADING_FSPM         1

#define    FSPM_UNCOMPRESSED    0
#define    FSPM_COMPRESSED      2

typedef struct {
  UINT8               StructureId[8];
  UINT8               StructVersion;
  UINT8               Reserved0;
  UINT16              ElementSize;
} BPM_ELEMENT_HEADER;

typedef struct {
  UINT8               StructureId[8];
  UINT8               StructVersion;
  UINT8               Reserved0;
  UINT16              SizeOfData;
} PCDE_ELEMENT_HEADER;

#define BSPM_ELEMENT_STRUCTURE_ID (*(UINT64 *)"__BSPM__")
#define BSPM_ELEMENT_VERSION      0x10
#define BSSS_SEARCH_START_ADDRESS 0xFFFFF000
#define BSSS_SEARCH_END_ADDRESS   0xFFFFFF80
typedef struct {

  UINT8                       StructureID[8];
  UINT8                       StructVersion;
  UINT16                      SizeOfElement;
  UINT8                       Reserved0;

  //
  // BSP Entrypoint for FSP to jump into
  //
  UINT32                      BspEntryPoint;

  //
  // FSP-T UPD
  //
  FSPT_UPD                    FsptUpd;

  //
  // 0: FSP loads FSP-M
  // 1: FSP skips loading FSP-M
  //
  UINT8                       FspmLoadingPolicy;
  UINT32                      FspmBaseAddress;
  UINT64                      TpmBaseAddress;

  //
  // FSP Signing Enabled:  BspSegmentCount should describe BspPreMem segment count
  // FSP Signing Disabled: BspSegmentCount should be 0
  //
  UINT32                      BspSegmentCount;
  UINT32                      FspConfigRegSegmentCount;
} BSPM_ELEMENT;

#pragma pack ()

#endif
