/**@file
  Power Loss Notification Setting

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
#ifndef _POWER_LOST_NOTIFY_SETTING_H_
#define _POWER_LOST_NOTIFY_SETTING_H_

//
// Power loss Notify Setting Value
//
enum PLN_SETTING {
  PLN_DISABLE = 0,
  PLN_ENABLE,
  PLN_DEFAULT
};

//
// Forward declaration for the POWER_LOSS_NOTIFY_SETTING_PROTOCOL.
//
typedef struct _POWER_LOSS_NOTIFY_SETTING_PROTOCOL  POWER_LOSS_NOTIFY_SETTING_PROTOCOL;

/**
  Get the PLN Setting.

  @param[in]  This                 A pointer to the POWER_LOSS_NOTIFY_SETTING_PROTOCOL instance.
  @param[out] Setting              The PLN Setting.

  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    The parameters are invalid.

**/
typedef
EFI_STATUS
(EFIAPI *POWER_LOSS_NOTIFY_GET_SETTING) (
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
typedef
EFI_STATUS
(EFIAPI *POWER_LOSS_NOTIFY_SET_CONTROLLER) (
  IN  POWER_LOSS_NOTIFY_SETTING_PROTOCOL    *This,
  IN  UINTN                                 Setting
  );

//
// This Protocol contains a set of functionality to interact with the PLN Settings
//
struct _POWER_LOSS_NOTIFY_SETTING_PROTOCOL {
  POWER_LOSS_NOTIFY_GET_SETTING       GetPlnSetting;
  POWER_LOSS_NOTIFY_SET_CONTROLLER    SetPlnController;
};

extern EFI_GUID gPowerLossNotifySettingProtocolGuid;

#endif
