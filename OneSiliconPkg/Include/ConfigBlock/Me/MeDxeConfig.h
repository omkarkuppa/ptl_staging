/** @file
  ME config block for DXE phase

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _ME_DXE_CONFIG_H_
#define _ME_DXE_CONFIG_H_

#define ME_DXE_CONFIG_REVISION 1
extern EFI_GUID gMeDxeConfigGuid;

#pragma pack (push,1)

/**
  ME Dxe Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER Header;                   ///< Config Block Header
  UINT32              MeFwDowngrade       : 1;  ///< <b>0: Disabled</b>; 1: Enabled - Enable/Disable ME FW downgrade
  UINT32              CoreBiosDoneEnabled : 1;  ///< 0: Disabled; <b>1: Enabled</b> - Enable/Disable Sending notification that BIOS is starting to run 3rd party code
  UINT32              AutoArbSvnCommit    : 1;  ///< 0: Disabled; <b>1: Enabled</b> - Enable/Disable attempt to perform Anti-Rollback SVN on each boot
  UINT32              RsvdBits            : 29; ///< Reserved for future use & Config block alignment
} ME_DXE_CONFIG;
#pragma pack (pop)

#endif // _ME_DXE_CONFIG_H_
