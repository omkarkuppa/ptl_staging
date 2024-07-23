/** @file
  PCH configuration based on PCH policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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
#ifndef _PCH_PREMEM_POLICY_COMMON_H_
#define _PCH_PREMEM_POLICY_COMMON_H_

#include <ConfigBlock.h>

#pragma pack (push,1)

#ifndef FORCE_ENABLE
#define FORCE_ENABLE  1
#endif
#ifndef FORCE_DISABLE
#define FORCE_DISABLE 2
#endif
#ifndef PLATFORM_POR
#define PLATFORM_POR  0
#endif

/**
  PCH Policy revision number
  Any backwards compatible changes to this structure will result in an update in the revision number
**/
#define PCH_PREMEM_POLICY_REVISION  1

/**
  PCH Policy PPI\n
  All PCH config block change history will be listed here\n\n

  - <b>Revision 1</b>:
    - Initial version.\n
**/
typedef struct _PCH_PREMEM_POLICY {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
} PCH_PREMEM_POLICY;

#pragma pack (pop)

#endif // _PCH_PREMEM_POLICY_COMMON_H_
