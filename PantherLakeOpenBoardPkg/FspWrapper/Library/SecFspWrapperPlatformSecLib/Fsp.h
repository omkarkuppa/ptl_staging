/** @file
  Fsp related definitions

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

@par Specification
**/

#ifndef __FSP_H__
#define __FSP_H__

//
// Fv Header
//
#define FVH_SIGINATURE_OFFSET         0x28
#define FVH_SIGINATURE_VALID_VALUE    0x4856465F  // valid signature:_FVH
#define FVH_HEADER_LENGTH_OFFSET      0x30
#define FVH_EXTHEADER_OFFSET_OFFSET   0x34
#define FVH_EXTHEADER_SIZE_OFFSET     0x10

//
// Ffs Header
//
#define FSP_HEADER_GUID_DWORD1        0x912740BE
#define FSP_HEADER_GUID_DWORD2        0x47342284
#define FSP_HEADER_GUID_DWORD3        0xB08471B9
#define FSP_HEADER_GUID_DWORD4        0x0C3F3527
#define FFS_HEADER_SIZE_VALUE         0x18

//
// Section Header
//
#define SECTION_HEADER_TYPE_OFFSET    0x03
#define RAW_SECTION_HEADER_SIZE_VALUE 0x04

//
// Fsp Header
//
#define FSP_HEADER_IMAGEBASE_OFFSET     0x1C
#define FSP_HEADER_TEMPRAMINIT_OFFSET   0x30

#endif
