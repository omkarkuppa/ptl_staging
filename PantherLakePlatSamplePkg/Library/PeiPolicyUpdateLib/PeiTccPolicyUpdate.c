/** @file
  This file is SampleCode of the library for Intel TCC PEI
  Platform Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include "PeiTccPolicyUpdate.h"
#include "PeiPchPolicyUpdate.h"
#include <Setup.h>
#include <SetupVariable.h>
#include <Library/PeiServicesLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchInfoLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/TccPolicyLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/MeRegs.h>
#include <TccConfig.h>
#include <Register/CommonMsr.h>
#include <CpuRegs.h>
#if FixedPcdGetBool(PcdFspModeSelection) == 1
#include <FspmUpd.h>
#include <FspsUpd.h>
#endif
#include <PolicyUpdateMacro.h>
#include <Library/IoLib.h>
#include <Register/PmcRegs.h>
#include <Ppi/Wdt.h>
#include <Library/PmcLib.h>
#include <PeiSaPolicyUpdate.h>

#if FixedPcdGet8(PcdTccSupport) == 0x1
/**
  This function performs TCC PEI Policy initialization.

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI_NOT_FOUND           The FspmUpd is not found.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiTccPolicyPreMem (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  UINTN                             VariableSize;
  SA_SETUP                          SaSetup;
  CPU_SETUP                         CpuSetup;
  SETUP_DATA                        SetupData;
  UINT32                            Data32;
#if FixedPcdGetBool(PcdFspModeSelection) == 1
  VOID                              *FspmUpd;
#else
  SI_PREMEM_POLICY_PPI              *SiPreMemPolicyPpi;
  CPU_CONFIG_LIB_PREMEM_CONFIG      *CpuConfigLibPreMemConfig;
  MEMORY_CONFIGURATION              *MemConfig;
#endif

#if FixedPcdGetBool(PcdFspModeSelection) == 1
  FspmUpd = NULL;
#else
  SiPreMemPolicyPpi = NULL;
  CpuConfigLibPreMemConfig = NULL;
  MemConfig = NULL;
  TccPreMemConfig = NULL;
#endif


#if FixedPcdGetBool(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *) PcdGet32 (PcdFspmUpdDataAddress);
  ASSERT (FspmUpd != NULL);
#else
  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuConfigLibPreMemConfigGuid, (VOID *) &CpuConfigLibPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
  ASSERT_EFI_ERROR (Status);
#if FixedPcdGetBool(PcdTccSupport) == 0x1
  Status = GetConfigBlock((VOID *)SiPreMemPolicyPpi, &gTccPreMemConfigGuid, (VOID *)&TccPreMemConfig);
  ASSERT_EFI_ERROR(Status);
#endif
#endif

  DEBUG ((DEBUG_INFO, "Update PeiTccPolicyUpdate Pre-Mem Start\n"));

  //
  // Retrieve Setup variable
  //
  Status = PeiServicesLocatePpi (
            &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
            0,                                // INSTANCE
            NULL,                             // EFI_PEI_PPI_DESCRIPTOR
            (VOID **) &VariableServices       // PPI
            );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (CPU_SETUP);
  Status = VariableServices->GetVariable (
                                VariableServices,
                                L"CpuSetup",
                                &gCpuSetupVariableGuid,
                                NULL,
                                &VariableSize,
                                &CpuSetup
                                );
  ASSERT_EFI_ERROR(Status);

  VariableSize = sizeof (SA_SETUP);
  Status = VariableServices->GetVariable (
                                VariableServices,
                                L"SaSetup",
                                &gSaSetupVariableGuid,
                                NULL,
                                &VariableSize,
                                &SaSetup
                                );
  ASSERT_EFI_ERROR(Status);

  VariableSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SetupData
                               );
  ASSERT_EFI_ERROR (Status);

  if (IsTccModeEnabled (&SetupData)) {
    GetTccModeConfigPolicy (&CpuSetup, NULL, &SaSetup, &SetupData);
  }

  if (IsTccModeEnabled (&SetupData)) {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SaGv,                    MemConfig->SaGv,                  SaSetup.SaGv);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisPgCloseIdleTimeout,   MemConfig->DisPgCloseIdleTimeout, SaSetup.DisPgCloseIdleTimeout);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableStarv2medPrioOnNewReq,   MemConfig->DisableStarv2medPrioOnNewReq, SetupData.DisableStarv2medPrioOnNewReq);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerDownMode,           MemConfig->PowerDownMode,         SaSetup.PowerDownMode);
  }

  DEBUG ((DEBUG_INFO, "Update PeiTccPolicyUpdate Pre-Mem End\n"));

  return EFI_SUCCESS;
}

/**
  This function performs TCC PEI Policy initialization.

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI_NOT_FOUND           The FspsUpd is not found.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiTccPolicy (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  UINTN                             VariableSize;
  CPU_SETUP                         CpuSetup;
  PCH_SETUP                         PchSetup;
  SA_SETUP                          SaSetup;
  SETUP_DATA                        SetupData;
  UINT8                             Index;
  BOOLEAN                           IsBootGuardDeveloperOrLegacy;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                              *FspsUpd;
#else
  SI_POLICY_PPI                     *SiPolicyPpi;
  CPU_POWER_MGMT_TEST_CONFIG        *CpuPowerMgmtTestConfig;
  CPU_POWER_MGMT_BASIC_CONFIG       *CpuPowerMgmtBasicConfig;
  CPU_INIT_CONFIG                   *CpuInitConfig;
  PCH_PCIE_CONFIG                   *PcieRpConfig;
  PCH_DMI_CONFIG                    *DmiConfig;
  GRAPHICS_PEI_CONFIG               *GtConfig;
  PCH_GENERAL_CONFIG                *PchGeneralConfig;
  PCH_PM_CONFIG                     *PmConfig;
  TCC_CONFIG                        *TccConfig;
#endif


#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspsUpd = NULL;
#else
  SiPolicyPpi = NULL;
  CpuPowerMgmtTestConfig = NULL;
  CpuPowerMgmtBasicConfig = NULL;
  CpuInitConfig = NULL;
  PcieRpConfig = NULL;
  DmiConfig = NULL;
  GtConfig = NULL;
  PchGeneralConfig = NULL;
  PmConfig = NULL;
  TccConfig = NULL;
#endif

#if FixedPcdGetBool(PcdFspModeSelection) == 1
  FspsUpd = (FSPS_UPD *) PcdGet32 (PcdFspsUpdDataAddress);
  ASSERT (FspsUpd != NULL);
#else
  Status = PeiServicesLocatePpi (&gSiPolicyPpiGuid, 0, NULL, (VOID **) &SiPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtTestConfigGuid, (VOID *) &CpuPowerMgmtTestConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtBasicConfigGuid, (VOID *) &CpuPowerMgmtBasicConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchPcieConfigGuid, (VOID *) &PcieRpConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchDmiConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuConfigGuid, (VOID *) &CpuInitConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGraphicsPeiConfigGuid, (VOID *) &GtConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);


  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gTccConfigGuid, (VOID *) &TccConfig);
  ASSERT_EFI_ERROR (Status);
#endif

  DEBUG ((DEBUG_INFO, "Update PeiTccPolicyUpdate Post-Mem Start\n"));


  //
  // Retrieve Setup variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices       // PPI
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SetupData
                               );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (PCH_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &PchSetup
                               );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (CPU_SETUP);
  Status = VariableServices->GetVariable (
                                VariableServices,
                                L"CpuSetup",
                                &gCpuSetupVariableGuid,
                                NULL,
                                &VariableSize,
                                &CpuSetup
                                );
  ASSERT_EFI_ERROR(Status);

  VariableSize = sizeof (SA_SETUP);
  Status = VariableServices->GetVariable (
                                VariableServices,
                                L"SaSetup",
                                &gSaSetupVariableGuid,
                                NULL,
                                &VariableSize,
                                &SaSetup
                                );
  ASSERT_EFI_ERROR(Status);

  if (IsTccModeEnabled (&SetupData)) {
    GetTccModeConfigPolicy (&CpuSetup, NULL, &SaSetup, &SetupData);
  }


  if (IsTccModeEnabled (&SetupData)) {

    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.Eist,          CpuPowerMgmtTestConfig->Eist,     CpuSetup.EnableGv);
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.Cx,            CpuPowerMgmtTestConfig->Cx,       CpuSetup.EnableCx);
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.Hwp,           CpuPowerMgmtBasicConfig->Hwp,     CpuSetup.EnableHwp);
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.AcSplitLock,   CpuInitConfig->AcSplitLock,           CpuSetup.AcSplitLock);

    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PchDmiAspmCtrl,            DmiConfig->DmiAspmCtrl,                 PchSetup.PchDmiAspm);
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PchLegacyIoLowLatency,     PchGeneralConfig->LegacyIoLowLatency,   PchSetup.PchLegacyIoLowLatency);
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PchPmPwrBtnOverridePeriod, PmConfig->PwrBtnOverridePeriod,         !!SetupData.LowPowerS0Idle? 0x3 : 0);
  }
  DEBUG ((DEBUG_INFO, "Update PeiTccPolicyUpdate Post-mem End\n"));

  return EFI_SUCCESS;
}
#endif
