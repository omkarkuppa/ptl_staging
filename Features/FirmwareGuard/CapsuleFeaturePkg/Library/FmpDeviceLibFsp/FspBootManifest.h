/** @file
  Definition of FSP Boot Manifest.

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

#ifndef __FSP_BOOT_MANIFEST__
#define __FSP_BOOT_MANIFEST__

#define FBM_STRUCTURE_ID  (*(UINT64 *)"__FBMS__")

typedef struct {
  UINT8     StructureId[8];
  UINT8     StructVersion;
  UINT8     Reserved1[3];
  UINT16    KeySignatureOffset;
  UINT8     FspVersion[6];
} FBM_HEADER;

#endif
