/** @file
  BIOS wrapper data structures.

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

**/
#ifndef CPLATFORM_DATA_H_
#define CPLATFORM_DATA_H_

#include "CMrcRmtData.h"  // for BDAT definitions

//
// BIOS wrapper implementation of PlatformData abstraction.
// Contains Pointers to memory allocated from the platform.
//
typedef struct PlatformData_ {
  BDAT_SCHEMAS_HOB_STRUCT BdatSchemasHob;                        ///< Pointer to the BDAT schema list.
  BDAT_MEMORY_HOB_STRUCT  BdatMemoryHob[MAX_SCHEMA_LIST_LENGTH]; ///< Pointer to the BDAT memory data HOB.
} PLATFORM_DATA;
#endif //CPLATFORM_DATA_H_
