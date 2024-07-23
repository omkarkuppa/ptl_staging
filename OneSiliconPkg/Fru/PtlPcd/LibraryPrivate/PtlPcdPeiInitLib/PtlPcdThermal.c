/** @file
  Initializes Thermal

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
#include "PtlPcdInitPei.h"
#include <Library/BaseLib.h>
#include <Library/PostCodeLib.h>
#include <Library/CpuPowerManagementLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Register/GpioAcpiDefines.h>

/**
  Enable Thermal PCH HOT
**/
VOID
EnablePchHot (
  VOID
  )
{
  EFI_STATUS      Status;
  GPIOV2_SERVICES *GpioServices;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    return;
  }

  PtlPcdGpioEnablePchHot (GpioServices);
}

/**
  Initialize Thermal handle structure

  @param[in] PwrmBase                Address to Pwrm Base
  @param[in] SiPolicy                The SI Policy PPI instance
  @param[in] ThermalHandle           Pointer to THERMAL_HANDLE structure
  @param[in] PrivateConfig           Pointer to THERMAL_PRIVATE_CONFIG structure
  @param[in] Callbacks               THERMAL_CALLBACKS structure
**/
STATIC
VOID
InitThermalHandle (
  IN  UINTN                          PwrmBase,
  IN  SI_POLICY_PPI                  *SiPolicy,
  IN THERMAL_HANDLE                  *ThermalHandle,
  IN THERMAL_PRIVATE_CONFIG          *PrivateConfig,
  IN THERMAL_CALLBACKS               *Callbacks
  )
{
  EFI_STATUS                     Status;
  THERMAL_CONFIG                 *ThermalConfig;

  ZeroMem (ThermalHandle, sizeof (THERMAL_HANDLE));
  ZeroMem (Callbacks, sizeof (THERMAL_CALLBACKS));

  Status = GetConfigBlock ((VOID *) SiPolicy, &gThermalConfigGuid, (VOID *) &ThermalConfig);
  ASSERT_EFI_ERROR (Status);

  ThermalHandle->Config = ThermalConfig;
  ThermalHandle->PwrmBase = PwrmBase;

  PrivateConfig->PchHotSupported            = TRUE;
  PrivateConfig->EcThermalReportSupported   = TRUE;
  PrivateConfig->SocIntFabThermTtSupported  = TRUE;

  Callbacks->EnablePchHot                = EnablePchHot;

  ThermalHandle->PrivateConfig = PrivateConfig;
  ThermalHandle->Callbacks = Callbacks;
}

/**
  Perform Thermal Management Support initialization

  @param[in] SiPolicy                The SI Policy PPI instance
  @param[in] PwrmBase                Address to Pwrm Base
**/
VOID
ThermalInit (
  IN  SI_POLICY_PPI                     *SiPolicy,
  IN  UINTN                             PwrmBase
  )
{
  THERMAL_HANDLE                     ThermalHandle;
  THERMAL_PRIVATE_CONFIG             PrivateConfig;
  THERMAL_CALLBACKS                  Callbacks;

  DEBUG ((DEBUG_INFO, "%a (): Start\n", __FUNCTION__));
  //
  // Initialize Thermal handle.
  //
  InitThermalHandle (
    PwrmBase,
    SiPolicy,
    &ThermalHandle,
    &PrivateConfig,
    &Callbacks
    );

  ThermalConfiguration (&ThermalHandle);

  DEBUG ((DEBUG_INFO, "%a (): End\n", __FUNCTION__));
}

