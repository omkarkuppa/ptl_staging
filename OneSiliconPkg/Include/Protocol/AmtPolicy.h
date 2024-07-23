/** @file
  Interface definition details between AMT and platform drivers during DXE phase.

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
#ifndef _AMT_POLICY_H_
#define _AMT_POLICY_H_

#include <ConfigBlock.h>

///
/// AMT policy provided by platform for DXE phase
/// This protocol provides the information about Intel AMT platform configuration settings.
///

///
/// Protocol revision number
/// Any backwards compatible changes to this protocol will result in an update in the revision number
/// Major changes will require publication of a new protocol
///
#define AMT_POLICY_PROTOCOL_REVISION 1   ///< Initial Revision

extern EFI_GUID gDxeAmtPolicyGuid;

#pragma pack (push,1)

/**
  AMT DXE Policy
  This protocol is initialized by Policy driver. Other modules can locate this protocol
  to retrieve Intel AMT related setup options setting

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct _AMT_POLICY_PROTOCOL {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
} AMT_POLICY_PROTOCOL;

#pragma pack (pop)

#endif  // _AMT_POLICY_H_
