/** @file
  VMD PEI policy
  VMD Config Block

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
#ifndef _VMD_PEI_CONFIG_H_
#define _VMD_PEI_CONFIG_H_

#include <ConfigBlock.h>
#include <Library/VmdInfoLib.h>

#define VMD_PEI_CONFIG_REVISION 1

#pragma pack (push,1)


typedef struct {
    UINT8  RpBus;
    UINT8  RpDevice;
    UINT8  RpFunction;
    UINT8  RpEnable;
} RP_BDF_DATA;

/**
  This configuration block is to configure VMD related variables used in PostMem PEI.
  If VMD Device is not supported, all policies can be ignored.
  <b>Revision 1</b>:
  - Initial version.
**/

typedef struct {
  CONFIG_BLOCK_HEADER  Header;                          ///< Offset 0-27 Config Block Header
  UINT8                VmdEnable;                       ///< Offset 28 This field used to enable VMD controller <b>1=Enable(default)</b> 0=Disable
  UINT8                VmdGlobalMapping;                ///< Offset 29 This field used to enable Global Mapping 1=Enable <b>0=Disable(default)</b>
  UINT8                Rsvd[2];                        ///< Offset 30-39 Reserved
  RP_BDF_DATA          VmdPortEnable[VMD_MAX_DEVICES];  ///< Offset 40 to 163 This field used to to store b/d/f for each root port along with enable Support  1=Enable <b>0=Disable (default)</b>
  VOID                 *VmdVariablePtr;                 ///< This config block will be updated as per the EFI variable.
  UINT32               VmdCfgBarBase;                   ///< Temp Address VMD CFG BAR Default is <b>0xA0000000</b>
  UINT32               VmdMemBar1Base;                  ///< Temp Address VMD CFG BAR Default is <b>0xA2000000</b>
  UINT32               VmdMemBar2Base;                  ///< Temp Address VMD CFG BAR Default is <b>0xA4000000</b>
} VMD_PEI_CONFIG;

#pragma pack (pop)

#endif /* _VMD_PEI_PREMEM_CONFIG_H_ */
