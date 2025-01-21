/** @file
  IAX policy definitions

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

#ifndef _IAX_CONFIG_H_
#define _IAX_CONFIG_H_

#include <ConfigBlock.h>
#pragma pack(push, 1)

#define IAX_PEI_CONFIG_REVISION 1

/**
  IAX PEI configuration
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           /// Offset 0-27 : Config Block Header
  /**
    Offset 28
    It controls enabling/disabling IAX device if supported and not fused off.
    If FALSE, all other polices can be ignored.
    <b>1=TRUE</b>;
    0=FALSE.
  **/
  UINT8              IaxEnable;
  UINT8              Reserved[3];        ///< Offset 29 : Reserved for 4 bytes of alignment
} IAX_PEI_CONFIG;
#pragma pack(pop)

#endif // _IAX_CONFIG_H_
