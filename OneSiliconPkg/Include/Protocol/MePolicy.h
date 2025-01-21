/** @file
  Interface definition details between ME and platform drivers during DXE phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2006 Intel Corporation.

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
#ifndef _ME_POLICY_H_
#define _ME_POLICY_H_

#include <ConfigBlock.h>
#include <MeDxeConfig.h>

/**
  ME Policy Protocol.
  All ME Policy Protocol change history listed here.

**/
#define ME_POLICY_PROTOCOL_REVISION  1

extern EFI_GUID gDxeMePolicyGuid;

#pragma pack (push,1)

/**
  ME policy provided by platform for DXE phase
  This protocol provides an interface to get Intel ME Configuration information

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct _ME_POLICY_PROTOCOL {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
} ME_POLICY_PROTOCOL;

#pragma pack (pop)

#endif // _ME_POLICY_H_
