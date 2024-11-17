/** @file
  This file is SampleCode of the library for Intel CPU PEI Debug Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include "PeiPolicyDebug.h"
#include <Library/CpuPlatformLib.h>
#include <Library/PcdLib.h>
#include <PolicyUpdateMacro.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspsUpd.h>
#endif

/**
  This function performs CPU PEI Debug Policy initialization.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance
  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyDebug (
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi,
  SI_POLICY_PPI                   *SiPolicyPpi
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  BOOLEAN                         C1UnDemotion;
  BOOLEAN                         C1AutoDemotion;
  CPU_SETUP                       CpuSetup;
#if FixedPcdGet8(PcdFspModeSelection) == 0
  CPU_TEST_CONFIG                 *CpuTestConfig;
  CPU_POWER_MGMT_TEST_CONFIG      *CpuPowerMgmtTestConfig;
  CPU_INIT_PREMEM_CONFIG          *CpuInitPreMemConfig;
#endif

  DEBUG ((DEBUG_INFO, "Update PeiCpuPolicyDebug Pos-Mem Start\n"));

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspsUpd;
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#else
  CpuTestConfig            = NULL;
  CpuPowerMgmtTestConfig   = NULL;
  CpuInitPreMemConfig      = NULL;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &CpuInitPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuTestConfigGuid, (VOID *) &CpuTestConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtTestConfigGuid, (VOID *) &CpuPowerMgmtTestConfig);
  ASSERT_EFI_ERROR (Status);
#endif

  //
  // Make sure ReadOnlyVariablePpi is available
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Get Setup Variable
  //
  VariableSize = sizeof (CPU_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"CpuSetup",
                               &gCpuSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &CpuSetup
                               );
  ASSERT_EFI_ERROR (Status);

  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.ProcessorTraceOutputScheme, CpuTestConfig->ProcessorTraceOutputScheme,           CpuSetup.ProcessorTraceOutputScheme);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.ProcessorTraceEnable,       CpuTestConfig->ProcessorTraceEnable,                 CpuSetup.ProcessorTraceEnable);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.ProcessorTraceMemSize,      CpuTestConfig->ProcessorTraceMemSize,                CpuSetup.ProcessorTraceMemSize);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.ProcessorTraceBspOnly,      CpuTestConfig->ProcessorTraceBspOnly,                CpuSetup.ProcessorTraceBspOnly);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.ProcessorTraceTimingPacket, CpuTestConfig->ProcessorTraceTimingPacket,           CpuSetup.ProcessorTraceTimingPacket);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.MachineCheckEnable,         CpuTestConfig->MachineCheckEnable,                   CpuSetup.MachineCheck);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.MonitorMwaitEnable,         CpuTestConfig->MonitorMwaitEnable,                   CpuSetup.MonitorMwait);

  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.ThreeStrikeCounter,          CpuTestConfig->ThreeStrikeCounter,                   CpuSetup.ThreeStrikeCounter);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PpmIrmSetting,               CpuPowerMgmtTestConfig->PpmIrmSetting,               CpuSetup.InterruptRedirectMode);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.Cx,                          CpuPowerMgmtTestConfig->Cx,                          CpuSetup.EnableCx);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.Eist,                        CpuPowerMgmtTestConfig->Eist,                        CpuSetup.EnableGv);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.EnergyEfficientPState,       CpuPowerMgmtTestConfig->EnergyEfficientPState,       CpuSetup.EnergyEfficientPState);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.CStatePreWake,               CpuPowerMgmtTestConfig->CStatePreWake,               CpuSetup.CStatePreWake);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.TimedMwait,                  CpuPowerMgmtTestConfig->TimedMwait,                  CpuSetup.TimedMwait);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PmgCstCfgCtrlLock,           CpuPowerMgmtTestConfig->PmgCstCfgCtrlLock,           CpuSetup.PmgCstCfgCtrlLock);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.ForcePrDemotion,             CpuPowerMgmtTestConfig->ForcePrDemotion,             CpuSetup.ForcePrDemotion);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VrAlertDemotion,             CpuPowerMgmtTestConfig->VrAlertDemotion,             CpuSetup.VrAlertDemotion);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.EnableAllThermalFunctions,   CpuPowerMgmtTestConfig->EnableAllThermalFunctions,   CpuSetup.EnableAllThermalFunctions);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.TStates,                     CpuPowerMgmtTestConfig->TStates,                     CpuSetup.TStatesEnable);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.EnergyEfficientTurbo,        CpuPowerMgmtTestConfig->EnergyEfficientTurbo,        CpuSetup.EnergyEfficientTurbo);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.RaceToHalt,                  CpuPowerMgmtTestConfig->RaceToHalt,                  CpuSetup.RaceToHalt);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PkgCStateLimit,              CpuPowerMgmtTestConfig->PkgCStateLimit,              CpuSetup.PkgCStateLimit);

  //
  // Core C-State AutoDemotion/UnDemotion
  //
  C1AutoDemotion = CpuSetup.CStateAutoDemotion;
  C1UnDemotion   = CpuSetup.CStateUnDemotion;

  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.C1StateUnDemotion, CpuPowerMgmtTestConfig->C1UnDemotion, C1UnDemotion);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.C1StateAutoDemotion, CpuPowerMgmtTestConfig->C1AutoDemotion, C1AutoDemotion);
  //
  // Pkg C-state Demotion/Un Demotion
  //
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PkgCStateDemotion, CpuPowerMgmtTestConfig->PkgCStateDemotion, CpuSetup.PkgCStateDemotion);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PkgCStateUnDemotion, CpuPowerMgmtTestConfig->PkgCStateUnDemotion, CpuSetup.PkgCStateUnDemotion);

  return EFI_SUCCESS;
}
