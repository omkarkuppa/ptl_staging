/** @file
  Integrated Error Handler policy.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#ifndef _IEH_CONFIG_H_
#define _IEH_CONFIG_H_

#include <ConfigBlock.h>

#define IEH_MODE_BYPASS 0
#define IEH_MODE_ENABLE 1

#define IEH_CONFIG_REVISION 1
extern EFI_GUID gIehConfigGuid;

#pragma pack (push,1)

/**
  The IEH_CONFIG block describes the expected configuration of the PCH
  Integrated Error Handler.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           ///< Config Block Header
  /**
    IEH mode <b>0: Bypass Mode</b>; 1: Enable
  **/
  UINT32    Mode            :  1;
  UINT32    RsvdBits0       : 31;       ///< Reserved bits
} IEH_CONFIG;

#pragma pack (pop)

#endif // _IEH_CONFIG_H_
