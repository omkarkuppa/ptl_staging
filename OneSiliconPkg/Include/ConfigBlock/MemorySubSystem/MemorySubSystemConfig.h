/** @file
  Policy definition for Memory SubSystem Config Block.

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
#ifndef _MEMORY_SUB_SYSTEM_CONFIG_H
#define _MEMORY_SUB_SYSTEM_CONFIG_H

#define MEMORY_SUB_SYSTEM_CONFIG_REVISION 1

#pragma pack(push, 1)

/**
  This Configuration block configures Memory SubSystem
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER Header;                ///< Offset 0-27 Config Block Header
  UINT32  AcpiReservedMemorySize;            ///< Offset 28 The Size of a Reserved memory buffer allocated in previous boot for S3 resume used. Originally it is retrieved from AcpiVariableCompatibility variable.
  UINT64  AcpiReservedMemoryBase;            ///< Offset 32 The Base address of a Reserved memory buffer allocated in previous boot for S3 resume used. Originally it is retrieved from AcpiVariableCompatibility variable.
  UINT8   MemMapOverlapCheckSupport;         ///< Offset 40 MemoryOverlap check Enable/Disable
  UINT8   Rsvd[3];                           ///< Reserved for future use
} MEMORY_SUB_SYSTEM_CONFIG;
#pragma pack(pop)
#endif // _MEMORY_SUB_SYSTEM_CONFIG_H
