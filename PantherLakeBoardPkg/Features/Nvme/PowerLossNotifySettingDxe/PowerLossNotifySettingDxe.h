/**@file
  Power Loss Notification Setting get the PLN setting and set the EC and GPIO for PLN

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _POWER_LOSS_NOTIFY_SETTING_H_
#define _POWER_LOSS_NOTIFY_SETTING_H_

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/EcMiscLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Protocol/PowerLossNotifySetting.h>
#include <SetupVariable.h>

#define GPIO_PLN_ENABLED                          GpioV2StateLow
#define GPIO_PLN_DISABLED                         GpioV2StateHigh
#define GPIO_PLN_PB_OVR_NO_CS                     GpioV2StateLow
#define GPIO_PLN_PB_OVR_CS                        GpioV2StateHigh

#define EC_PLN_DISABLED                           0
#define EC_PLN_PB_OVR_NO_CS                       4  // 4 sec
#define EC_PLN_PB_OVR_CS                          10 // 10 sec

/**
  Get the PLN Setting.

  @param[in]  This                 A pointer to the POWER_LOSS_NOTIFY_SETTING_PROTOCOL instance.
  @param[out] Setting              The PLN Setting.

  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    The parameters are invalid.

**/
EFI_STATUS
EFIAPI
GetPlnSetting (
  IN  POWER_LOSS_NOTIFY_SETTING_PROTOCOL    *This,
  OUT UINTN                                 *Setting
  );

/**
  Set the EC data for PLN and set the GPIO for PLN.

  @param[in]  This                 A pointer to the POWER_LOSS_NOTIFY_SETTING_PROTOCOL instance.
  @param[in]  Setting              Enable/Disable of PLN Setting.

  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    The parameters are invalid.

**/
EFI_STATUS
EFIAPI
SetPlnController (
  IN  POWER_LOSS_NOTIFY_SETTING_PROTOCOL    *This,
  IN  UINTN                                 Setting
  );

#endif
