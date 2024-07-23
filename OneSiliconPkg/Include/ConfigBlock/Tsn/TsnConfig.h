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

#define TSN_CONFIG_REVISION 2
extern EFI_GUID gTsnConfigGuid;

#pragma pack (push,1)

typedef struct {
  UINT32  MacAddr[2]; ///< MAC Address
} TSN_MAC_ADDR_PORT;

/**
  The TSN_CONFIG block describes policies related to Time Sensitive Networking(TSN)

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added MultiVcEnable
  <b>Revision 3</b>:
  - Added MAC Address Port values for both TSN devices
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Config Block Header
  /**
    Determines if enable PCH internal TSN, 0: Disable; <b>1: Enable</b>.
  **/
  UINT8             Enable;
  UINT8             TsnLinkSpeed;           ///< Refer to "TSN_LINK_SPEED_OPTIONS". Default is <b> TSN_LINK_SPEED_2_5G_34MHZ</b>
  UINT8             MultiVcEnable;          ///< Enable Multi-Vc via Tc to DMA mapping. Deafult is </b> 0:disable</b>
  UINT8             Rsvd0;                  ///< Reserved bytes, align to multiple 4.
  TSN_MAC_ADDR_PORT Port[2];                ///< MAC Address
} TSN_CONFIG;

#pragma pack (pop)

typedef enum {
  TSN_LINK_SPEED_2_5G_24MHZ = 0,
  TSN_LINK_SPEED_1G_24MHZ,
  TSN_LINK_SPEED_2_5G_34MHZ,
  TSN_LINK_SPEED_1G_34MHZ
} TSN_LINK_SPEED_OPTIONS;

#endif
