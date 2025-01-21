/** @file
  PCH Policy PPI is used for specifying platform
  related Intel silicon information and policy setting.
  This PPI is consumed by the silicon PEI modules and carried
  over to silicon DXE modules.

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

#ifndef PCH_POLICY_PPI_H
#define PCH_POLICY_PPI_H

extern EFI_GUID gPchPolicyPpiGuid;
extern EFI_GUID gPchPreMemPolicyPpiGuid;

#include <ConfigBlock.h>

/**
  PCH pre-memory Policy revision number
  Any change to this structure will result in an update in the revision number

  <b>Revision 1</b>:
   - Initial version.
**/
#define PCH_PREMEM_POLICY_REVISION  1

/**
  PCH Policy revision number
  Any change to this structure will result in an update in the revision number

  This member specifies the revision of the PCH Policy. This field is used to indicate change
  to the policy structure.

  <b>Revision 1</b>:
   - Initial version.
**/
#define PCH_POLICY_REVISION  1

/**
  PCH Policy PPI in Pre-Mem\n
  All PCH config block change history will be listed here\n\n

  - <b>Revision 1</b>:
    - Initial version.\n
**/
typedef struct _PCH_PREMEM_POLICY_STRUCT {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;   ///< Config Block Table Header
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
} PCH_PREMEM_POLICY_PPI;

/**
  PCH Policy PPI\n
  All PCH config block change history will be listed here\n\n

  - <b>Revision 1</b>:
    - Initial version.\n
**/
typedef struct _PCH_POLICY_STRUCT {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;   ///< Config Block Table Header
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
} PCH_POLICY_PPI;

#endif // PCH_POLICY_PPI_H
