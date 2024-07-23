/** @file
  Protocol used to report CPU information

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _CPU_INFO_H_
#define _CPU_INFO_H_

typedef struct _CPU_INFO_PROTOCOL CPU_INFO_PROTOCOL;


extern EFI_GUID gCpuInfoProtocolGuid;

//
// DXE_CPU_INFO_PROTOCOL revisions
//
#define CPU_INFO_PROTOCOL_REVISION 1

#define CORE_TYPE_NON_HYBRID 0

#define SMALL_CORE      0
#define BIG_CORE        1
#define CORE_TYPE_NUM   2

#define MAX_BRANDSTRING_SIZE   49

#define MAX_CACHE_LEVEL_COUNT  8

#pragma pack(push, 1)
///
/// Cache descriptor information
///
typedef struct {
  UINT8   Desc;                                    ///< Cache Descriptor
  UINT8   Level;                                   ///< Cache Level
  UINT8   Type;                                    ///< Cache Type. 0: Data, 1: Instruction, 3: Unified
  UINT32  Size;                                    ///< Cache Size.
  UINT16  Associativity;                           ///< Cache Ways of Associativity.
} CACHE_DESCRIPTOR_INFO;

///
/// Processor information
///
typedef struct {
  UINT32                CpuSignature;                               ///< Processor signature and version information.
  CHAR8                 BrandString[MAX_BRANDSTRING_SIZE];          ///< Processor Brand String.
  UINT8                 NumCores;                                   ///< Number of Enabled or Active Cores.
  UINT8                 NumHts;                                     ///< Max Number of Enabled Threads per Core. This will be 1 or 2.
  UINT32                IntendedFreq;                               ///< Maximum non turbo ratio in MHz
  CACHE_DESCRIPTOR_INFO CacheInfo[MAX_CACHE_LEVEL_COUNT];           ///< Cache descriptor information.
  UINT8                 MaxCacheSupported;                          ///< Maximum cache supported.
} CPU_INFO;

#pragma pack(pop)

///
/// This protocol provides information about the common features available in this CPU.
///
struct _CPU_INFO_PROTOCOL {
  /**
  Revision for the protocol structure.
  Any backwards compatible changes to this protocol will result in an update in the revision number.
  Major changes will require publication of a new protocol

  <b>Revision 1</b>:
   -  Initial version
  **/

  UINT8             Revision;
  CPU_INFO          CpuInfo[CORE_TYPE_NUM];         ///< Processor Basic Information Contains Big Core and Small Core
};

#endif
