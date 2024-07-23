/** @file
  Thermal Access Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/PmcLib.h>
#include <Register/ThermalRegs.h>

/**
  Check catastrophic trip point is locked

  @retval TRUE  catastrophic trip point is locked
          FALSE catastrophic trip point is not locked
**/
BOOLEAN
EFIAPI
ThermalIsCatastrophicTripPointLock (
  VOID
  )
{
  return !!(MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_THERMAL_CTEN) & B_PMC_PWRM_THERMAL_CTEN_CTENLOCK);
}

/**
  Check thermal throttle level is locked

  @retval TRUE  Thermal throttle level is locked
          FALSE Thermal throttle level is not locked
**/
BOOLEAN
ThermalIsThrottleLock (
  VOID
  )
{
  return !!(MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_THERMAL_TL) & B_PMC_PWRM_THERMAL_TL_TLLOCK);
}

/**
  Check thermal EC thermal report is locked

  @retval TRUE  EC thermal report is locked
          FALSE EC thermal report is not locked
**/
BOOLEAN
ThermalIsEcThermalReportLock (
  VOID
  )
{
  return !!(MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_THERMAL_ECRPTEN) & B_PMC_PWRM_THERMAL_ECRPTEN_ECRPTENLOCK);
}

/**
  Check thermal PCH HOT is locked

  @retval TRUE  PCH HOT is locked
          FALSE PCH HOT is not locked
**/
BOOLEAN
ThermalIsPchHotLock (
  VOID
  )
{
  return !!(MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_THERMAL_PHLC) & B_PMC_PWRM_THERMAL_PHLC_PHLCLOCK);
}

/**
  Check thermal PMC throttle is locked

  @retval TRUE  PMC throttle is locked
          FALSE PMC throttle is not locked
**/
BOOLEAN
ThermalIsPmcThrottleLock  (
  VOID
  )
{
  return !!(MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_THERMAL_TLEN) & B_PMC_PWRM_THERMAL_TLEN_TLENLOCK);
}

/**
  Check Thermal Internal Faric Thermal Throttling Configuration is locked

  @retval TRUE  SoC Internal Fabric Thermal Throttling Configuration is locked
          FALSE  SoC Internal Fabric Thermal Throttling Configuration is not locked
**/
BOOLEAN
SocInternalFabricThermalThrottleLock (
  VOID
  )
{
  return !!(MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_SOCIFTTC) & B_PMC_PWRM_SOCIFTTC_SOCIFTTML);
}
