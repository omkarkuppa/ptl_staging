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

#ifndef _PLATFORM_VOLTAGE_CONTROL_LIB_H_
#define _PLATFORM_VOLTAGE_CONTROL_LIB_H_

#include <SetupVariable.h>

/**
  Programs the overrides for Vcc1p8 voltage which are controlled by UP1816
  chip using SVID/PMBUS interface.

  @param[in] Voltage      Voltage target in mV

  @retval EFI_SUCCESS     Successful to override voltage.
**/
EFI_STATUS
ProgramVcc1p8Override (
  IN  UINT16                Voltage
  );

/**
  Programs the overrides for Vcc1p05 voltage which are controlled by UP1816
  chip using SVID/PMBUS interface.

  @param[in] Voltage      Voltage target in mV

  @retval EFI_SUCCESS     Successful to override voltage.
**/
EFI_STATUS
ProgramVcc1p05Override (
  IN  UINT16                Voltage
  );

/**
  Programs the overrides for VccDd2 voltage which are controlled by UP1816
  chip using SVID/PMBUS interface.

  @param[in] Voltage      Voltage target in mV

  @retval EFI_SUCCESS     Successful to override voltage.
**/
EFI_STATUS
ProgramVccDd2Override (
  IN  UINT16                Voltage
  );

/**
  Programs the overrides for VCCIN_AUX_CPU voltage which are controlled by PCA9555 (IO Port expander).

  @param[in] Voltage        Voltage target in mV

  @retval    EFI_SUCCESS   The function completes successfully
  @retval    EFI_STATUS    Fail status return from SmBusWriteDataWord ()
**/
EFI_STATUS
ProgramVccInAuxOverride (
  IN  UINT16                Voltage
  );

/**
  Programs the overrides for Richtek VccIa voltage which are controlled by Richtek VR (External VR).

  @param[in] VidModeEn      Enabled/ Disable VCC VID Mode
  @param[in] Voltage        Voltage target in mV

  @retval    EFI_SUCCESS   The function completes successfully
  @retval    EFI_STATUS    Fail status return from SmBusWriteDataWord ()
**/
EFI_STATUS
ProgramRichtekVccIaOverride (
  IN  UINT8                 VidModeEn,
  IN  UINT16                Voltage
  );

/**
  Programs the overrides for Richtek VccGt voltage which are controlled by Richtek VR (External VR).

  @param[in] VidModeEn      Enabled/ Disable VCC VID Mode
  @param[in] Voltage        Voltage target in mV

  @retval    EFI_SUCCESS   The function completes successfully
  @retval    EFI_STATUS    Fail status return from SmBusWriteDataWord ()
**/
EFI_STATUS
ProgramRichtekVccGtOverride (
  IN  UINT8                 VidModeEn,
  IN  UINT16                Voltage
  );

/**
  Check and apply the override knobs for external voltage rails based on board design

  @param[in] OcSetup       Setup variables under OC Setup
**/
VOID
ProgramVoltageOverrides (
  IN  OC_SETUP             *OcSetup
  );

/**
  Platform Voltage Init - Programs platform voltages over SMBUS.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
PlatformVoltageInit (
  VOID
  );
#endif
