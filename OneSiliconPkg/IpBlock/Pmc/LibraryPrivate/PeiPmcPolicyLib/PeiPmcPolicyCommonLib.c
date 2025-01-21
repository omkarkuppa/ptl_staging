/** @file
  This file provides services for Pmc policy function

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
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <PmConfig.h>

/**
  Check if PCH PM Timer enabled based on platform policy

  @retval TRUE       PCH PM Timer is enabled.
  @retval FALSE      PCH PM Timer is disabled.
**/
BOOLEAN
PmcIsPchPmTimerEnabled (
  VOID
  )
{
  BOOLEAN           PchPmTimerEnabled;
  EFI_STATUS        Status;
  SI_POLICY_PPI     *SiPolicy;
  PCH_PM_CONFIG     *PmConfig;

  Status = PeiServicesLocatePpi (
             &gPchPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  if (EFI_ERROR (Status)) {
    Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  }
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);

  PchPmTimerEnabled = TRUE;
  if (!PmConfig->EnableTcoTimer) {
    PchPmTimerEnabled = FALSE;
  }

  DEBUG ((DEBUG_INFO, "PmcIsPchPmTimerEnabled () = %x\n", PchPmTimerEnabled));

  return PchPmTimerEnabled;
}

/**
  Print PCH_PM_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
PmcPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS    Status;
  PCH_PM_CONFIG *PmConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ PCH PM Config ------------------\n"));

  DEBUG ((DEBUG_INFO, " WakeConfig PmeB0S5Dis               = %x\n", PmConfig->WakeConfig.PmeB0S5Dis));
  DEBUG ((DEBUG_INFO, " WakeConfig WolEnableOverride        = %x\n", PmConfig->WakeConfig.WolEnableOverride));
  DEBUG ((DEBUG_INFO, " WakeConfig LanWakeFromDeepSx        = %x\n", PmConfig->WakeConfig.LanWakeFromDeepSx));
  DEBUG ((DEBUG_INFO, " WakeConfig PcieWakeFromDeepSx       = %x\n", PmConfig->WakeConfig.PcieWakeFromDeepSx));
  DEBUG ((DEBUG_INFO, " WakeConfig WoWlanEnable             = %x\n", PmConfig->WakeConfig.WoWlanEnable));
  DEBUG ((DEBUG_INFO, " WakeConfig WoWlanDeepSxEnable       = %x\n", PmConfig->WakeConfig.WoWlanDeepSxEnable));

  DEBUG ((DEBUG_INFO, " PchDeepSxPol                        = %x\n", PmConfig->PchDeepSxPol));
  DEBUG ((DEBUG_INFO, " PchSlpS3MinAssert                   = %x\n", PmConfig->PchSlpS3MinAssert));
  DEBUG ((DEBUG_INFO, " PchSlpS4MinAssert                   = %x\n", PmConfig->PchSlpS4MinAssert));
  DEBUG ((DEBUG_INFO, " PchSlpSusMinAssert                  = %x\n", PmConfig->PchSlpSusMinAssert));
  DEBUG ((DEBUG_INFO, " PchSlpAMinAssert                    = %x\n", PmConfig->PchSlpAMinAssert));
  DEBUG ((DEBUG_INFO, " SlpStrchSusUp                       = %x\n", PmConfig->SlpStrchSusUp));
  DEBUG ((DEBUG_INFO, " SlpLanLowDc                         = %x\n", PmConfig->SlpLanLowDc));
  DEBUG ((DEBUG_INFO, " PwrBtnOverridePeriod                = %x\n", PmConfig->PwrBtnOverridePeriod));
  DEBUG ((DEBUG_INFO, " DisableEnergyReport                 = %x\n", PmConfig->DisableEnergyReport));
  DEBUG ((DEBUG_INFO, " DisableDsxAcPresentPulldown         = %x\n", PmConfig->DisableDsxAcPresentPulldown));
  DEBUG ((DEBUG_INFO, " PchPwrCycDur                        = %x\n", PmConfig->PchPwrCycDur));
  DEBUG ((DEBUG_INFO, " PciePllSsc                          = %x\n", PmConfig->PciePllSsc));
  DEBUG ((DEBUG_INFO, " DisableNativePowerButton            = %x\n", PmConfig->DisableNativePowerButton));
  DEBUG ((DEBUG_INFO, " MeWakeSts                           = %x\n", PmConfig->MeWakeSts));
  DEBUG ((DEBUG_INFO, " WolOvrWkSts                         = %x\n", PmConfig->WolOvrWkSts));
  DEBUG ((DEBUG_INFO, " EnableTcoTimer                      = %x\n", PmConfig->EnableTcoTimer));
  DEBUG ((DEBUG_INFO, " VrAlert                             = %x\n", PmConfig->VrAlert));
  DEBUG ((DEBUG_INFO, " PowerButtonDebounce                 = %x\n", PmConfig->PowerButtonDebounce));
  DEBUG ((DEBUG_INFO, " PsOnEnable                          = %x\n", PmConfig->PsOnEnable));
  DEBUG ((DEBUG_INFO, " CpuC10GatePinEnable                 = %x\n", PmConfig->CpuC10GatePinEnable));
  DEBUG ((DEBUG_INFO, " ModPhySusPgEnable                   = %x\n", PmConfig->ModPhySusPgEnable));
  DEBUG ((DEBUG_INFO, " Usb2PhySusPgEnable                  = %x\n", PmConfig->Usb2PhySusPgEnable));
  DEBUG ((DEBUG_INFO, " OsIdleEnable                        = %x\n", PmConfig->OsIdleEnable));
  DEBUG ((DEBUG_INFO, " V1p05PhyExtFetControlEn             = %x\n", PmConfig->V1p05PhyExtFetControlEn));
  DEBUG ((DEBUG_INFO, " V1p05IsExtFetControlEn              = %x\n", PmConfig->V1p05IsExtFetControlEn));
  DEBUG ((DEBUG_INFO, " LpmS0ixSubStateEnable               = %x\n", PmConfig->LpmS0ixSubStateEnable.Val));
  DEBUG ((DEBUG_INFO, " S0ixAutoDemotion                    = %x\n", PmConfig->S0ixAutoDemotion));
  DEBUG ((DEBUG_INFO, " LatchEventsC10Exit                  = %x\n", PmConfig->LatchEventsC10Exit));
  DEBUG ((DEBUG_INFO, " EnableTimedGpio0                    = %x\n", PmConfig->EnableTimedGpio0));
  DEBUG ((DEBUG_INFO, " EnableTimedGpio1                    = %x\n", PmConfig->EnableTimedGpio1));
  DEBUG ((DEBUG_INFO, " DirtyWarmResetEnable                = %x\n", PmConfig->DirtyWarmResetEnable));
  DEBUG ((DEBUG_INFO, " GlobalResetMasksOverride            = %x\n", PmConfig->GlobalResetMasksOverride));
  DEBUG ((DEBUG_INFO, " GlobalResetTriggerMask              = %x\n", PmConfig->GlobalResetTriggerMask));
  DEBUG ((DEBUG_INFO, " GlobalResetEventMask                = %x\n", PmConfig->GlobalResetEventMask));
  DEBUG ((DEBUG_INFO, " ThermTimerDelay                     = %x\n", PmConfig->ThermTimerDelay));
  DEBUG ((DEBUG_INFO, " ER Debug Mode                       = %x\n", PmConfig->PmErDebugMode));
  DEBUG ((DEBUG_INFO, " CF9LockdownEnable                   = %x\n", PmConfig->Cf9LockdownEnable));
  DEBUG ((DEBUG_INFO, " PmcWdtTimerEn                       = %x\n", PmConfig->PmcWdtTimerEn));
}