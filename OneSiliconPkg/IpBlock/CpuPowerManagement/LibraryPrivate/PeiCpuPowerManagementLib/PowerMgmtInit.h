/** @file
  This header file contains processors power management definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _POWER_MANAGEMENT_H_
#define _POWER_MANAGEMENT_H_

#include "PowerMgmtCommon.h"

/**
  Initializes the CPU power management global variable.
  This must be called prior to any of the functions being used.

  @param[in] CpuPowerDeliveryConfig  Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
InitPowerManagementGlobalVariables (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Sets up the PPM flags based upon capabilities

  @retval EFI_STATUS
**/
VOID
SetPpmFlags (
  VOID
  );

/**
  Set the PPM flags based on current user configuration obtained from PPM Policy protocol

  @param[in] CpuPowerDeliveryConfig  Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
SetUserConfigurationPpmFlags (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Initialize Ctdp related NVS Variables in Hob

  @param[in] ApplyConfigTdp             If apply ConfigTdp
  @param[in] ConfigTdpPowerLimit1       ConfigTdpPowerLimit1
  @param[in] ConfigTdpPowerLimit2       ConfigTdpPowerLimit2
  @param[in] ConfigTdpPowerLimitWindow  ConfigTdpPowerLimitWindow
  @param[in] ConfigTdpTar               ConfigTdpTar
  @param[in] ConfigTdpBootModeIndex     ConfigTdpBootModeIndex
  @param[in] CpuPowerDeliveryConfig     CPU_POWER_DELIVERY_CONFIG
**/
VOID
InitCpuCtdpNvsDataHob (
  IN BOOLEAN                   ApplyConfigTdp,
  IN UINT16                    ConfigTdpPowerLimit1,
  IN UINT16                    ConfigTdpPowerLimit2,
  IN UINT8                     ConfigTdpPowerLimitWindow,
  IN UINT8                     ConfigTdpTar,
  IN UINT8                     ConfigTdpBootModeIndex,
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Update CPU NVS Variables in Hob.
**/
VOID
UpdateCpuNvsDataHob (
  VOID
  );

/**
  Initialize the platform power management based on hardware capabilities
  and user configuration settings.

  This includes creating FVID table, updating ACPI tables,
  and updating processor and chipset hardware configuration.

  This should be called prior to any Px, Cx, Tx activity.

  @retval EFI_SUCCESS - on success
  @retval Appropiate failure code on error
**/
EFI_STATUS
InitPpm (
  VOID
  );
#endif
