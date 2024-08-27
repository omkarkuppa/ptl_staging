/** @file
  This file contains contains structures for FSP Boot Manifest.

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

#ifndef __FBM_DEF_H__
#define __FBM_DEF_H__

#include <BpmDef.h>

//
// Data structure definitions
//
#pragma pack (1)

#define FSP_REGION_TYPE_FSPOT       0
#define FSP_REGION_TYPE_FSPM        1
#define FSP_REGION_TYPE_FSPS        2
#define FSP_REGION_TYPE_FSPVERSION        15

#define VERSION_DIGEST_PTR(Fbm)  \
  (FSP_REGION_DIGEST *)((UINT8 *)(Fbm) + sizeof (FSP_BOOT_MANIFEST_STRUCTURE) - \
  4 * sizeof (FSP_REGION_DIGEST) - 7 * sizeof (UINT8))

#define COMPONENT_DIGEST0_PTR(Fbm)  \
  (FSP_REGION_DIGEST *)((UINT8 *)(Fbm) + sizeof (FSP_BOOT_MANIFEST_STRUCTURE) - \
  3 * sizeof (FSP_REGION_DIGEST))

#define FSP_REGION0_PTR(Fbm)  \
  (FSP_REGION_STRUCTURE *)((UINT8 *)(Fbm) + sizeof (FSP_BOOT_MANIFEST_STRUCTURE))

#define IBB_SEGMENTS_PTR(FspRegion)  \
  (REGION_SEGMENT *) ((UINT8 *) FspRegion + sizeof (FSP_REGION_STRUCTURE))

//
// In FBM, all TPM required type digest should be there.
// Only SHA384 digest will be verified, others are kept for measurement use.
//
typedef struct {
  UINT8             ComponentID;  //0: FSP-O/T  1: FSP-M  2: FSP-S
  MAX_HASH_LIST     ComponentDigests;
} FSP_REGION_DIGEST;

typedef struct {
  UINT8             ComponentID;  //0: FSP-O/T  1: FSP-M  2: FSP-S
  UINT8             SegmentCnt;
  //REGION_SEGMENT     SegmentArray[];
} FSP_REGION_STRUCTURE;

#define FSP_BOOT_MANIFEST_STRUCTURE_ID  (*(UINT64 *)"__FBMS__")
#define FSP_BOOT_MANIFEST_STRUCTURE_VERSION_0_1          0x01
typedef struct {
  UINT8                         StructureId[8];
  UINT8                         StructVersion;         // 0x01
  UINT8                         Reserved1[3];          // UINT32 Alignment

  UINT16                        KeySignatureOffset;
  UINT8                         FspVersion[6];
  FSP_REGION_DIGEST             FspVersionDigests;    // digest for FSP version
  UINT8                         FspSvn;
  UINT8                         Reserved2;
  UINT32                        Flags;                 // UINT32 Alignment

  UINT8                         CompCnt;
  FSP_REGION_DIGEST             ComponentDigests_0;    // digest for FSP/T
  FSP_REGION_DIGEST             ComponentDigests[2];   // digest for FSP-M, FSP-S

  //FSP_REGION_STRUCTURE        FspRegions_0;          // FSP-O/T
  //FSP_REGION_STRUCTURE        FspRegions[2];         // FSP-M, FSP-S

  //KEY_AND_SIGNATURE_STRUCT    Signature;

} FSP_BOOT_MANIFEST_STRUCTURE;

#pragma pack ()

#endif
