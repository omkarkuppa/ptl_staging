/** @file
  TBT DXE Common Definition

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _DXE_TBT_COMMON_DEFINITION_H_
#define _DXE_TBT_COMMON_DEFINITION_H_

//
// TBT Resource Data Structure
//
typedef struct _TBT_RESOURCE_CONFIG{
  UINT8  TbtPcieExtraBusRsvd;           ///< Preserve Bus resource for PCIe RP
  UINT8  TbtPcieMemAddrRngMax;          ///< Alignment of Preserve MEM resource for PCIe RP
  UINT16 TbtPcieMemRsvd;                ///< Preserve MEM resource for PCIe RP
  UINT32 TbtPciePMemRsvd;               ///< Preserve PMEM resource for PCIe RP
  UINT8  TbtPciePMemAddrRngMax;         ///< Alignment of Preserve PMEM resource for PCIe RP
  UINT8  Reserved[7];                   ///< Reserved for DWORD alignment
} TBT_RESOURCE_CONFIG;

#endif
