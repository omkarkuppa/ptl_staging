/** @file
  Headr file for iTBT DXE Policy

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

#ifndef _DXE_ITBT_CONFIG_H_
#define _DXE_ITBT_CONFIG_H_

#include <ConfigBlock.h>
#include <TcssInfo.h>
#include <DxeTbtCommonDefinition.h>

#pragma pack(push, 1)

#define DXE_ITBT_CONFIG_REVISION 2

///
/// ITBT Generic Data Structure to be used cross to RP and controller in DXE phase
///
typedef struct _DXE_ITBT_GENERIC_CONFIG{
  UINT8    ITbtRtd3;                    ///< ITBT RTD3. - 0: Disable; <b>1: Enable</b>.
  UINT8    ITbtVproDockSupport;         ///< ITBT vPro Dock support. - <b>0: not support</b>; 1: supported.
  UINT16   ITbtRtd3ExitDelay;           ///< ITBT RTD3 delay before exit, unit is ms <b>0</b>.
} DXE_ITBT_GENERIC_CONFIG;

/**
 ITBT DXE configuration\n
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add ITbtVproDockSupport in DXE_ITBT_GENERIC_CONFIG structure.
**/
typedef struct _DXE_ITBT_CONFIG {
  CONFIG_BLOCK_HEADER     Header;                                                       ///< Offset 0-27 Config Block Header
  DXE_ITBT_GENERIC_CONFIG ITbtGenericConfig;                                            ///< ITbt Generic Information
  TBT_RESOURCE_CONFIG     ITbtResourceConfig[MAX_ITBT_PCIE_PORT];                       ///< iTbt Resource Configuration
  UINT8                   OsNativeResourceBalance;
  UINT16                  PcieRtd3LinkActiveTimeout;                                    ///< PCIE RTD3 exit polling Link Active timeout and delay in PS0 before return to OS, unit is ms <b>100</b>
  UINT8                   Reserved[1];
} DXE_ITBT_CONFIG;

#pragma pack(pop)

#endif
