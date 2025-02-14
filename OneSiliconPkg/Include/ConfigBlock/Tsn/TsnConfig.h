/** @file
  TSN Config policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _TSN_CONFIG_H_
#define _TSN_CONFIG_H_

#include <ConfigBlock.h>
#include <Library/PchLimits.h>

/**
  The TSN_CONFIG block describes policies related to Time Sensitive Networking(TSN)
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added MultiVcEnable
  <b>Revision 3</b>:
  - Added Config
  <b>Revision 4</b>:
  - Created MAC Addr Array for each port
  - Created Enable Array for each port

**/

#define TSN_CONFIG_REVISION 4
extern EFI_GUID gTsnConfigGuid;

#pragma pack (push,1)

//
// This struct will be consumes by TSN
//
typedef struct {
  UINT32                MacAddr[2];             // MAC address associated with this port
} TSN_MAC_ADDR;

/**
  TSN Config settings.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Config Block Header
  /**
    Determines if enable PCH internal TSN, 0: Disable; <b>1: Enable</b>.
  **/
  UINT8                 Enable[PCH_MAX_TSN_PORT];
  TSN_MAC_ADDR          Port[PCH_MAX_TSN_PORT];     ///< MAC Address Data Structure
} TSN_CONFIG;

#pragma pack (pop)

#endif
