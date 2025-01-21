/** @file
  P2sb policy

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
#ifndef _P2SB_CONFIG_H_
#define _P2SB_CONFIG_H_

#define PCH_P2SB_CONFIG_REVISION 1
extern EFI_GUID gP2sbConfigGuid;

#pragma pack (push,1)

/**
  This structure contains the policies which are related to P2SB device.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    <b>(Test)</b>
    The sideband MMIO register access to specific ports will be locked
    before 3rd party code execution. Currently it disables PSFx access.
    This policy unlocks the sideband MMIO space for those IPs.
    <b>0: Lock sideband access </b>; 1: Unlock sideband access.
    NOTE: Do not set this policy "SbAccessUnlock" unless its necessary.
  **/
  UINT32    SbAccessUnlock    :  1;
  UINT32    Rsvdbits          : 31;    ///< Reserved bits
} PCH_P2SB_CONFIG;

#pragma pack (pop)

#endif // _P2SB_CONFIG_H_
