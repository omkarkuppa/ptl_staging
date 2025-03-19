/** @file
  Definitions of types used in cMRC code that are not used in exernal APIs.

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
#ifndef CMRC_INTERNAL_TYPES_H_
#define CMRC_INTERNAL_TYPES_H_

#include "CMrcTypes.h"  // for UINT32 (indirectly)

  #ifndef UINT64_STRUCT_T
    #define UINT64_STRUCT_T
    typedef union {
      struct {
        UINT32  Low;
        UINT32  High;
      } Data32;
      UINT64 Data;
      UINT16 Data16[4];
      UINT8  Data8[8];
    } UINT64_STRUCT;
  #endif // UINT64_STRUCT_T
#endif // CMRC_INTERNAL_TYPES_H_
