/** @file
  Initializes PCH thermal controller for ICL.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiThermalLib.h>
#include <Register/ThermalRegs.h>
#include <Register/PmcRegs.h>

/**
  Lock Thermal configuration

  @param[in]   *ThermalHandle
**/
STATIC
VOID
ThermalLockConfiguration (
  IN    THERMAL_HANDLE       *ThermalHandle
  )
{
  // PHLC [31] locks the PHLC register for PCH_Hot#
  MmioOr32 (ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_PHLC, B_PMC_PWRM_THERMAL_PHLC_PHLCLOCK);

  // Set TLEN lock.
  MmioOr32 (ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_TLEN, B_PMC_PWRM_THERMAL_TLEN_TLENLOCK);

  // Lock SOC Internal Fabric Thermal Throttling Configuration
  if (ThermalHandle->PrivateConfig->SocIntFabThermTtSupported) {
    MmioOr32 (ThermalHandle->PwrmBase + R_PMC_PWRM_SOCIFTTC, B_PMC_PWRM_SOCIFTTC_SOCIFTTML);
  }

  // Set PM_DOWN_PPB_CFG lock.
  MmioOr32 (ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_PM_DOWN_PPB_CFG, B_PMC_PWRM_THERMAL_PM_DOWN_PPB_CFG_PPB_CFG_LOCK);
}

/**
  Perform Thermal Management Support initialization

  @param[in]   *ThermalHandle      Pointer to Thermal Handle structure
**/
VOID
ThermalConfiguration (
  IN    THERMAL_HANDLE        *ThermalHandle
  )
{
  UINT32              PchTTLevels;

  DEBUG ((DEBUG_INFO, "%a (): Start\n", __FUNCTION__));

  if (ThermalHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return;
  }

  ///
  /// Ensure CTEN.CPDEN is set high for enabling Catastrophic Trip point assertion into S5
  /// and lockdown CTEN.CTENLOCK to prevent overriding of this feature
  ///
  MmioWrite32 (ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_CTEN, B_PMC_PWRM_THERMAL_CTEN_CPDEN);
  MmioOr32 (ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_CTEN, B_PMC_PWRM_THERMAL_CTEN_CTENLOCK);

  ///
  /// Enable thermal reporting to an EC over SMBus or eSPI and PMC.
  /// Set bit[0] of ECRPTEN register to 1.
  ///
  if (ThermalHandle->PrivateConfig->EcThermalReportSupported) {
    MmioWrite32 (ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_ECRPTEN, B_PMC_PWRM_THERMAL_ECRPTEN_EN_PMC_TO_EC_TEMP_RPT);
    MmioOr32 (ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_ECRPTEN, B_PMC_PWRM_THERMAL_ECRPTEN_ECRPTENLOCK);
  }

  ///
  /// If the PCH_Hot pin reporting is supported, then write the temperature value in PHLC.PHLL[8:0] and set the enablebit PHLC.PHLE to 1
  /// Note: For PCHHOT# support, we need to make sure if the GPIO pin is set to PCHHOTB mode (pad mode == 2)
  /// and flash descriptor PCH Strap 9 Bit 22is set to 1.
  ///
  if (ThermalHandle->PrivateConfig->PchHotSupported) {
    if ((ThermalHandle->Config->PchHotEnable) && (ThermalHandle->Config->PchHotLevel < 0x0200)) {
      if (ThermalHandle->Callbacks->EnablePchHot != NULL) {
        ThermalHandle->Callbacks->EnablePchHot ();
      }
      MmioWrite32 (ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_PHLC, ThermalHandle->Config->PchHotLevel | B_PMC_PWRM_THERMAL_PHLC_PHLE);
    }
  }

  ///
  /// If thermal throttling is supported, then set the desired throttle values in TL.T2L[28:20], TL.T1L[18:10] and TL.T0l[8:0].
  /// Once the throttle levels are programmed then write 1 to the TL.TTEN bit.
  /// TL [31] locks the thermal throttling register.
  ///
  if (ThermalHandle->Config->TTLevels.SuggestedSetting == FALSE) {
    ///
    /// Trip Point Temperature = Resolution of 1'C in S9.8.0 2s complement format of Trip Point Register [8:0]
    /// If Trip Point Temperature <= T0Level, the system will be in T0 state.
    /// If T1Level >= Trip Point Temperature > T0Level, the system will be in T1 state.
    /// If T2Level >= Trip Point Temperature > T1Level, the system will be in T2 state.
    /// If Trip Point Temperature > T2Level, the system will be in T3 state.
    ///
    PchTTLevels =
      (UINT32) (ThermalHandle->Config->TTLevels.T2Level << 20) |
      (UINT32) (ThermalHandle->Config->TTLevels.T1Level << 10) |
      (UINT32) (ThermalHandle->Config->TTLevels.T0Level);
    MmioWrite32 (ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_TL, PchTTLevels);

    MmioOr32 (
      ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_TL,
      (UINT32) (ThermalHandle->Config->TTLevels.TTLock << 31) |
      (UINT32) (ThermalHandle->Config->TTLevels.TTState13Enable << 30) |
      (UINT32) (ThermalHandle->Config->TTLevels.TTEnable << 29)
      );
  } else {
    ///
    /// Set TL + 1520h[28:20] = 100
    /// Set TL + 1520h[18:10] = 97
    /// Set TL + 1520h[8:0] = 94
    /// Set TL + 1520h[31:29] = 101b in separate write
    ///
    PchTTLevels = ((100 << 20) | (97 << 10) | (94));
    MmioWrite32 (ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_TL, PchTTLevels);
    MmioOr32 (ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_TL, B_PMC_PWRM_THERMAL_TL_TLLOCK | B_PMC_PWRM_THERMAL_TL_TTEN);
  }

  // Enable PMC Throttling.
  MmioOr32 (ThermalHandle->PwrmBase + R_PMC_PWRM_THERMAL_TLEN, B_PMC_PWRM_THERMAL_TLEN_PMCTEN);

  ///
  /// Lock down Thermal Reporting to prevent outside agents from changing the values.
  ///
  ThermalLockConfiguration (ThermalHandle);

  ///
  /// S0ix Configuration
  ///
  MmioAndThenOr32 (
    ThermalHandle->PwrmBase + R_PMC_PWRM_DTS_S0IX_CONFIG,
    (UINT32) ~(B_PMC_PWRM_DTS_S0IX_CONFIG_ENABLE_DTS_THRESHOLD_XING_CHECK),
    B_PMC_PWRM_DTS_S0IX_CONFIG_DISABLE_DTS_IN_S0IX
    );
  MmioOr32 (ThermalHandle->PwrmBase + R_PMC_PWRM_DTS_S0IX_CONFIG, B_PMC_PWRM_DTS_S0IX_CONFIG_S0IXLOCK);

  DEBUG ((DEBUG_INFO, "%a ():  End\n", __FUNCTION__));
}
