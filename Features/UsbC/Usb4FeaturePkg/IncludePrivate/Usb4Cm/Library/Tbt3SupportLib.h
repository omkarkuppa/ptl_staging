/** @file
  Thunderbolt 3 support library header.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _TBT3_SUPPORT_LIB_H_
#define _TBT3_SUPPORT_LIB_H_

#include <Library/Usb4RouterDefs.h>

#define TR_LTTPR_SUPPORT_REV_MIN    62

/**
  Set DP TMU stable bit of TitanRidge DP OUT adapter.
  @param[in] Router - Pointer to router instance.
  @param[in] AdpNum - Adapter number.

  @retval EFI_SUCCESS           - Set DP TMU stable bit successfully.
  @retval EFI_UNSUPPORTED       - Fail to set DP TMU stable bit.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3TrSetTmuStable (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  );

/**
  Query TBT3 router VSEC6 capability

  @param[in] Router - Pointer to router instance.

  @retval EFI_SUCCESS           - Query TBT3 router VSEC6 capability successfully.
  @retval EFI_UNSUPPORTED       - Fail to query TBT3 router VSEC6 capability.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3QueryRtVsec6 (
  IN PUSB4_ROUTER    Router
  );

/**
  Set link configured bit of TBT3 VSEC6 LC_SX_CTRL for TBT3 adapter.
  (Refer to TBT3 CM guide 3.2.2)

  @param[in] Router - Pointer to router instance.

  @retval EFI_SUCCESS           - Set link configured successfully.
  @retval EFI_UNSUPPORTED       - Fail to set link configured.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3RtLinkConfigured (
  IN PUSB4_ROUTER    Router
  );

/**
  Set TBT3 VSEC6 CM bit and SLI bit for TBT3 router.
  (Refer to TBT3 CM guide 3.2.2)

  @param[in] Router - Pointer to router instance.

  @retval EFI_SUCCESS           - Set TBT3 CM bit and SLI bit successfully.
  @retval EFI_UNSUPPORTED       - Fail to set TBT3 CM bit and SLI bit.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3SetCmSli (
  IN PUSB4_ROUTER    Router
  );

/**
  Enable AlpineRidge USB hot plug events
  (Refer to CM TBT3 Device Support 4.1.2 xHC enablement)

  @param[in] Router - Pointer to router instance.

  @retval EFI_SUCCESS           - Enable AlpineRidge USB hot plug events successfully.
  @retval EFI_UNSUPPORTED       - Fail to enable AlpineRidge USB hot plug events.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
*/
EFI_STATUS
Tbt3ArEnableUsbPlugEvents (
  IN PUSB4_ROUTER    Router
  );

/**
  Set xHC connect bit of all router VSEC6 port region for TitanRidge router.
  (Refer to CM TBT3 Device Support 4.1.2 xHC enablement)

  @param[in] Router  - Pointer to router instance.
  @param[in] Connect - 1: Connect xHC, 0: Disconnect xHC.

  @retval EFI_SUCCESS           - Set xHC connect bit successfully.
  @retval EFI_UNSUPPORTED       - Fail to set xHC connect bit.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
*/
EFI_STATUS
Tbt3TrConnectXhc (
  IN PUSB4_ROUTER    Router,
  IN BOOLEAN         Connect
  );

/**
  Set xHC connect bit based on VSEC6 USB plug bit fo all router VSEC6 port region of AlpineRidge router.
  (Refer to CM TBT3 Device Support 4.1.2 xHC enablement)

  @param[in] Router  - Pointer to router instance.
  @param[in] Connect - 1: Connect xHC, 0: Disconnect xHC.

  @retval EFI_SUCCESS           - Set xHC connect bit successfully.
  @retval EFI_UNSUPPORTED       - Fail to set xHC connect bit.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
*/
EFI_STATUS
Tbt3ArConnectXhc (
  IN PUSB4_ROUTER    Router,
  IN BOOLEAN         Connect
  );

/**
  Check if lane bonding can be supported in TBT3 router.
  (Refer to CM TBT3 Device Support 6.1 Lane Bonding).

  @param[in] Router - Pointer to router instance.
  @param[in] AdpNum - Adapter number.

  @retval TRUE  - TBT3 lane bonding is supported.
  @retval FALSE - TBT3 lane bonding is not supported.
**/
BOOLEAN
IsTbt3LaneBondingSupported (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  );

/**
  Initialize DP IP register data befor CMHS
  (Refer to CM TBT3 Device Support 5.2.1.2)

  @param[in] Router - Pointer to router instance.

  @retval EFI_SUCCESS           - Initialize DP IP register successfully.
  @retval EFI_UNSUPPORTED       - Fail to initialize DP IP register.
  @retval EFI_TIMEOUT           - Fail to initialize DP IP register within timeout period.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3TrDpCmhsInit (
  IN PUSB4_ROUTER    Router
  );

/**
  Read NVM version for TitanRidge router.

  @param[in] Router      - Pointer to router instance.
  @param[out] NvmVersion - Pointer to the returned NVM version.

  @retval EFI_SUCCESS           - Read NVM version successfully.
  @retval EFI_UNSUPPORTED       - Fail to read NVM version.
  @retval EFI_TIMEOUT           - Fail to get response for NVM command within timeout period.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3TrReadNvmVersion (
  IN PUSB4_ROUTER    Router,
  OUT UINT8          *NvmVersion
  );

#endif
