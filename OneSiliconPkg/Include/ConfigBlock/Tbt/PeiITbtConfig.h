/** @file
  Header file for TBT PEI Policy

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

#ifndef _PEI_ITBT_CONFIG_H_
#define _PEI_ITBT_CONFIG_H_

#include <ConfigBlock.h>
#include <TcssInfo.h>
#include <PeiITbtGenericStructure.h>

//
// CM mode in Pre-boot - bit 2:0
//
#define USB4_CM_MODE_FW_CM                  0x00
#define USB4_CM_MODE_SW_CM                  0x01

#pragma pack(push, 1)
#define PEI_ITBT_CONFIG_REVISION 2

/**
 ITBT PEI configuration\n
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add ITbtPcieTunnelingForUsb4, deprecated ITbtSecurityLevel.
**/
typedef struct _PEI_ITBT_CONFIG {
  CONFIG_BLOCK_HEADER           Header;                                            ///< Offset  0-27 Config Block Header
  ITBT_GENERIC_CONFIG           ITbtGenericConfig;                                 ///< ITbt Common Configuration
  UINT16                        ITbtDmaLtr [MAX_HOST_ITBT_DMA_NUMBER];             ///< iTbt Host controller DMA LTR value
  UINT8                         RsvdBytes[4];   ///< Reserved for future use
} PEI_ITBT_CONFIG;
#pragma pack(pop)

#endif
