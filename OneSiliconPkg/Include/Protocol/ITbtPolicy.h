/** @file
  Interface definition details between TBT and platform drivers during DXE phase.

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
#ifndef _ITBT_POLICY_H_
#define _ITBT_POLICY_H_

#include <ConfigBlock.h>
#include <Library/ConfigBlockLib.h>
#include <DxeITbtConfig.h>
///
/// Extern the GUID for protocol users.
///
extern EFI_GUID gITbtPolicyProtocolGuid;
extern EFI_GUID gDxeITbtConfigGuid;

/**
  Don't change the original TBT_POLICY_PROTOCOL_REVISION macro, external
  modules maybe have consumed this macro in their source code.  Directly
  update the TBT_POLICY_PROTOCOL_REVISION version number may cause those
  external modules to auto mark themselves wrong version info.
  Always create new version macro for new Policy protocol interface.
**/
#define ITBT_POLICY_PROTOCOL_REVISION  1

/**
  ITBT DXE Policy

 The ITBT_POLICY_PROTOCOL producer drvier is recommended to
 set all the ITBT_POLICY_PROTOCOL size buffer zero before init any member parameter,
 this clear step can make sure no random value for those unknow new version parameters.

 Make sure to update the Revision if any change to the protocol, including the existing
 internal structure definations.\n
  Note: Here revision will be bumped up when adding/removing any config block under this structure.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;    ///< Offset 0-31
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
} ITBT_POLICY_PROTOCOL;

#endif
