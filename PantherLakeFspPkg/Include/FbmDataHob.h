/** @file
  FBM Data HOB GUID and structure definitions.
  
  This HOB contains FSP-S verification data extracted from FBM early in boot
  to prevent TOCTOU vulnerability.

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

**/

#ifndef _FBM_DATA_HOB_H_
#define _FBM_DATA_HOB_H_

#include <FbmDef.h>

extern EFI_GUID gFbmDataHobGuid;

///
/// FSP-S Verification Data HOB
/// Contains SHA384 digest and region segments for FSP-S verification
///
typedef struct {
  SHA384_HASH_STRUCTURE    FspDigestSha384;   ///< SHA384 digest
  UINT8                    SegmentCount;      ///< Number of region segments
  UINT8                    CmosOffset;        ///< CMOS offset for TXT disable
  REGION_SEGMENT           Segments[1];       ///< Variable-length array of segments
} FBM_DATA_HOB;

#endif // _FBM_DATA_HOB_H_
