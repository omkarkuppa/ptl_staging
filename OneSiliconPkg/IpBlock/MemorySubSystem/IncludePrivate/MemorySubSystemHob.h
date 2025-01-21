/** @file
  The GUID definition for Memory Sub System Hob

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _MEMORY_SUB_SYSTEM_HOB_h
#define _MEMORY_SUB_SYSTEM_HOB_h

#include <Base.h>
#include <Pi/PiHob.h>

extern EFI_GUID gMemorySubSystemHobGuid;
#define MEMORY_SUB_SYSTEM_DATA_HOB_REVISION         1

#pragma pack (push,1)

///
/// PSMI Ver1 Data Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;        ///< GUID Hob type structure for gPsmiDataHobGuid
  UINT8                    Revision;              ///< Revision of this config block
  UINT32                   MeStolenBase;          ///< Me Stolen Base
  UINT32                   MeStolenSize;          ///< Me Slolen Size
  UINT8                    Reserved[3];           ///< Reserved for future use
} MEMORY_SUB_SYSTEM_DATA_HOB;

#pragma pack (pop)
#endif
