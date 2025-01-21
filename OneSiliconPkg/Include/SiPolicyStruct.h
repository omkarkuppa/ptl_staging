/** @file
  Intel reference code configuration policies.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _SI_POLICY_STRUCT_H_
#define _SI_POLICY_STRUCT_H_

#include <ConfigBlock.h>
#include <ConfigBlock/SiPreMemConfig.h>
#include <ConfigBlock/SiConfig.h>

/**
  Silicon Policy revision number
  Any change to this structure will result in an update in the revision number

  This member specifies the revision of the Silicon Policy. This field is used to indicate change
  to the policy structure.

  <b>Revision 1</b>:
   - Initial version.
**/
#define SI_POLICY_REVISION  1

/**
  Silicon pre-memory Policy revision number
  Any change to this structure will result in an update in the revision number

  <b>Revision 1</b>:
   - Initial version.
**/
#define SI_PREMEM_POLICY_REVISION  1


/**
  SI Policy PPI in Pre-Mem\n
  All SI config block change history will be listed here\n\n

  - <b>Revision 1</b>:
    - Initial version.\n
**/
struct _SI_PREMEM_POLICY_STRUCT {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;   ///< Config Block Table Header
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
};

/**
  SI Policy PPI\n
  All SI config block change history will be listed here\n\n

  - <b>Revision 1</b>:
    - Initial version.\n
**/
struct _SI_POLICY_STRUCT {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;   ///< Config Block Table Header
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
};

#endif
