/**@file
    POWER_LOSS_NOTIFY_SETTING_PROTOCOL Mock for the Power Loss Notify Dxe driver.

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
#ifndef _PLN_SETTING_PROTOCOL_MOCK_H_
#define _PLN_SETTING_PROTOCOL_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Library/UnitTestLib.h>

#include <Protocol/PowerLossNotifySetting.h>

extern UINTN gPlnSetting;

/**
  Mock for get the PLN Setting.

  @param[in]  This                 A pointer to the POWER_LOSS_NOTIFY_SETTING_PROTOCOL instance.
  @param[out] Setting              The PLN Setting.

  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    The parameters are invalid.

**/
EFI_STATUS
EFIAPI
MockPlnSettingProtocol_GetPlnSetting (
  IN  POWER_LOSS_NOTIFY_SETTING_PROTOCOL    *This,
  OUT UINTN                                 *Setting
  );

/**
  Mock for set the EC data for PLN and set the GPIO for PLN.

  @param[in]  This                 A pointer to the POWER_LOSS_NOTIFY_SETTING_PROTOCOL instance.
  @param[in]  Setting              Enable/Disable of PLN Setting.

  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    The parameters are invalid.

**/
EFI_STATUS
EFIAPI
MockPlnSettingProtocol_SetPlnController (
  IN  POWER_LOSS_NOTIFY_SETTING_PROTOCOL    *This,
  IN  UINTN                                 Setting
  );

#endif /*_PLN_SETTING_PROTOCOL_MOCK_H_*/
