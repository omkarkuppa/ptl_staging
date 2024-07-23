/** @file
  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _PSF_CONFIG_H_
#define _PSF_CONFIG_H_

#include <ConfigBlock.h>
#define PSF_CONFIG_REVISION 1

extern EFI_GUID gPsfConfigGuid;

#pragma pack (push,1)

/**
  PSF Configuration
  <b>Revision 1</b>:  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header

  /**
    Flush Threshold
    -  0x000: Reserved
    -  0x001: 1/8
    -  0x010: 2/8
    -  0x011: 3/8
    -  0x100: 4/8
    -  0x101: 5/8
    -  0x110: 6/8
    -- <b>0x111: 7/8</b>;
  **/
  UINT32    DwbConfigDwbFlushThreshold   :  3;
  /**
    Non-xHCI Enable
    <b>0: Disable</b>; 1: Enable</b>
  **/
  UINT32    DwbConfigNonxHCIEn           :  1;
  /**
    OBFF Enable
    <b>0: Disable</b>; 1: Enable</b>
  **/
  UINT32    DwbConfigOBFFEn              :  1;
  /**
    Deferred Write Buffer Enable
    <b>0: Disable</b>; 1: Enable</b>
  **/
  UINT32    DwbConfigDWBEn               :  1;

  UINT32    Rsvdbits                     : 26;
} PSF_CONFIG;

#pragma pack (pop)
#endif
