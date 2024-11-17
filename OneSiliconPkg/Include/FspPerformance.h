/** @file
  Library for Fsp Performance Lib Config file.

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

#ifndef _FSP_PERFORMANCE_H_
#define _FSP_PERFORMANCE_H_

#define FSP_PERFORMANCE_HOB_GUID  \
  { 0xaa4d127a, 0x6c1f, 0x4833, {0xa4, 0x6c, 0x07, 0xf2, 0x27, 0x14, 0x71, 0x69} }

//
// Performance Entries currently present
//
// 1. uGOP -> Identifier = FspuGopPerf
//
// FspMaxPerf -> Holds the total count of entries
//
typedef enum {
  FspuGopPerf,
  FspMaxPerf
} FSP_PERFORMANCE;

#endif // __FSP_PERFORMANCE_H_
