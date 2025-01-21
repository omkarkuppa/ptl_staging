/** @file
  Interface definition details between Pch and platform drivers during DXE phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#ifndef _PCH_POLICY_H_
#define _PCH_POLICY_H_

#include <ConfigBlock.h>
#include <Library/ConfigBlockLib.h>
#include <PcieConfig.h>
#include <Library/HobLib.h>

extern EFI_GUID gPchPolicyProtocolGuid;

#define PCH_POLICY_PROTOCOL_REVISION  1

#define FORCE_ENABLE  1
#define FORCE_DISABLE 2
#define PLATFORM_POR  0


/**
  PCH DXE Policy

  The PCH_POLICY_PROTOCOL producer drvier is recommended to
  set all the PCH_POLICY_PROTOCOL size buffer zero before init any member parameter,
  this clear step can make sure no random value for those unknown new version parameters.

  Make sure to update the Revision if any change to the protocol, including the existing
  internal structure definations.\n
  Note: Here revision will be bumped up when adding/removing any config block under this structure.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
} PCH_POLICY_PROTOCOL;

#endif
