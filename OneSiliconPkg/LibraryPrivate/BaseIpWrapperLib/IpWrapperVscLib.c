/** @file
  This file contains standard version specific configuration library for IP wrapper

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

@par Specification
**/

//EDK and SoC includes
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>

#include <IpCsi.h>
#include <IpWrapperCntxtInfoClient.h>
#include <IpWrapper.h>

/**
  IP Wrapper (IPWR) Version Specific Configuration (VSC) Enabled

  @param[in]      pCntxt  IP Wrapper(IPWR) Version Specific Configuration(VSC) context. Opaque to IP FW and defined by IP Wrapper
  @param[in]      VscId   IP's Version Specific Configuration(VSC).

  @retval         1=Version Specific Configuration(VSC) should be applied by caller. Else, 0.

**/
BOOLEAN
IpWrVersionSpecificConfigurationEnabled (
  IP_WR_VSC_CNTXT pCntxt,
  UINT32          VscId
  )
{
  IP_WR_VSC_INFO  *IpWrVscInfo;

  if (pCntxt == (IP_WR_VSC_CNTXT)0) return FALSE;
  IpWrVscInfo = (IP_WR_VSC_INFO *)(UINTN)pCntxt;

  DEBUG ((DEBUG_INFO, "VSC %x\n", VscId));
  if (IpWrVscInfo->IpType == IpWrVscIpNone) {
    switch (VscId) {
      default:
        break;
    }
  }

  return FALSE;
}
