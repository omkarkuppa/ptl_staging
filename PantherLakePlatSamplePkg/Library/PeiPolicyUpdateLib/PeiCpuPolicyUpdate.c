/** @file
  CPU PEI Policy Update & initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include "PeiCpuPolicyUpdate.h"
#include <Library/ConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/HobLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/PeiVrDomainLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/Tpm12CommandLib.h>
#include <Library/Tpm2CommandLib.h>
#include <Ppi/SecPlatformInformation2.h>

#include <PolicyUpdateMacro.h>
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
#include <DptfConfig.h>
#endif
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspmUpd.h>
#include <FspsUpd.h>
#endif
#include <Library/FspCommonLib.h>

/**
  Update Cpu Power Management Policy settings according to the related BIOS Setup options

  @param[in] SetupData             The Setup variables instance
  @param[in] CpuSetup              The Setup variables instance
  @param[in] DptfConfig            Dptf Config Variable
  @param[in] SiPolicyPpi           The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
InitCpuPmConfigBySetupValues (
  IN SETUP_DATA                    *SetupData,
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
  IN DPTF_CONFIG                   *DptfConfig,
#endif
  IN CPU_SETUP                     *CpuSetup,
  IN SI_POLICY_PPI                 *SiPolicyPpi
  )
{
  UINT8                            Index;
  UINT8                            MaxBusRatio;
  UINT8                            MinBusRatio;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                             *FspsUpd;
#else
  CPU_POWER_MGMT_BASIC_CONFIG      *CpuPowerMgmtBasicConfig;
  CPU_POWER_MGMT_CUSTOM_CONFIG     *CpuPowerMgmtCustomConfig;
  EFI_STATUS                       Status;
#endif

  MaxBusRatio = 0;
  MinBusRatio = 0;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#else
  CpuPowerMgmtBasicConfig = NULL;
  CpuPowerMgmtCustomConfig = NULL;
  Status = EFI_SUCCESS;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtBasicConfigGuid, (VOID *) &CpuPowerMgmtBasicConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtCustomConfigGuid, (VOID *) &CpuPowerMgmtCustomConfig);
  ASSERT_EFI_ERROR (Status);
#endif
  ///
  /// Get Maximum Non-Turbo bus ratio (HFM) from Platform Info MSR Bits[15:8]
  /// Get Maximum Efficiency bus ratio (LFM) from Platform Info MSR Bits[47:40]
  ///
  GetBusRatio (&MaxBusRatio, &MinBusRatio);

  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.TurboMode, CpuPowerMgmtBasicConfig->TurboMode, CpuSetup->TurboMode);

  if (CpuSetup->BootMaxFrequency == 1) {
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.BootFrequency, CpuPowerMgmtBasicConfig->BootFrequency, CpuSetup->BootFrequency);
  } else {
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.BootFrequency, CpuPowerMgmtBasicConfig->BootFrequency, 0);
  }

  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.TurboPowerLimitLock, CpuPowerMgmtBasicConfig->TurboPowerLimitLock, CpuSetup->TurboPowerLimitLock);

  //
  // Intel Speed Shift Technology
  //
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.Hwp, CpuPowerMgmtBasicConfig->Hwp, CpuSetup->EnableHwp);
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.HwpInterruptControl, CpuPowerMgmtBasicConfig->HwpInterruptControl, CpuSetup->HwpInterruptControl);
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.HwpLock, CpuPowerMgmtBasicConfig->HwpLock, CpuSetup->HwpLock);

  //
  // Resource Priority Feature
  //
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.EnableRp, CpuPowerMgmtBasicConfig->EnableRp, CpuSetup->EnableRp);

  //
  // HwP Misc Functions
  //
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.EnableHwpAutoPerCorePstate, CpuPowerMgmtBasicConfig->EnableHwpAutoPerCorePstate, CpuSetup->EnableHwpAutoPerCorePstate);
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.EnableHwpAutoEppGrouping, CpuPowerMgmtBasicConfig->EnableHwpAutoEppGrouping, CpuSetup->EnableHwpAutoEppGrouping);

  //
  // Power Floor Survivability controls
  //
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PowerFloorManagement,        CpuPowerMgmtBasicConfig->PowerFloorManagement,        CpuSetup->PowerFloorManagement);
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PowerFloorDisplayDisconnect, CpuPowerMgmtBasicConfig->PowerFloorDisplayDisconnect, CpuSetup->PowerFloorDisplayDisconnect);
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PowerFloorPcieGenDowngrade,  CpuPowerMgmtBasicConfig->PowerFloorPcieGenDowngrade,  CpuSetup->PowerFloorPcieGenDowngrade);

  //
  // Custom VID table
  //
  if (CpuSetup->StateRatio[0] < MinBusRatio) {
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.MaxRatio, CpuPowerMgmtCustomConfig->CustomRatioTable.MaxRatio, MinBusRatio);
  } else {
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.MaxRatio, CpuPowerMgmtCustomConfig->CustomRatioTable.MaxRatio, CpuSetup->StateRatio[0]);
  }

  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.NumberOfEntries, CpuPowerMgmtCustomConfig->CustomRatioTable.NumberOfEntries, CpuSetup->NumOfCustomPStates);

  for (Index = 0; Index < CpuSetup->NumOfCustomPStates; Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *) FspsUpd)->FspsConfig.StateRatio[Index], CpuPowerMgmtCustomConfig->CustomRatioTable.StateRatio[Index], CpuSetup->StateRatio[Index], Index);
  }

  if (CpuSetup->NumOfCustomPStates > MAX_16_CUSTOM_RATIO_TABLE_ENTRIES) {
    for (Index = 0; Index < MAX_16_CUSTOM_RATIO_TABLE_ENTRIES; Index++) {
      COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *) FspsUpd)->FspsConfig.StateRatioMax16[Index], CpuPowerMgmtCustomConfig->CustomRatioTable.StateRatioMax16[Index], CpuSetup->StateRatioMax16[Index], Index);
    }
  }

  //
  // Update Turbo Ratio limit override table
  //
  if (CpuSetup->IsTurboRatioDefaultsInitialized != 0) {
    for (Index = 0; Index < TURBO_RATIO_LIMIT_ARRAY_SIZE; Index++) {
      COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *) FspsUpd)->FspsConfig.TurboRatioLimitNumCore[Index],     CpuPowerMgmtBasicConfig->TurboRatioLimitNumCore[Index],     CpuSetup->RatioLimitNumCore[Index],     Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *) FspsUpd)->FspsConfig.TurboRatioLimitRatio[Index],       CpuPowerMgmtBasicConfig->TurboRatioLimitRatio[Index],       CpuSetup->RatioLimitRatio[Index],       Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *) FspsUpd)->FspsConfig.AtomTurboRatioLimitNumCore[Index], CpuPowerMgmtBasicConfig->AtomTurboRatioLimitNumCore[Index], CpuSetup->AtomRatioLimitNumCore[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *) FspsUpd)->FspsConfig.AtomTurboRatioLimitRatio[Index],   CpuPowerMgmtBasicConfig->AtomTurboRatioLimitRatio[Index],   CpuSetup->AtomRatioLimitRatio[Index],   Index);
    }
  }

  //
  // Update Ring Ratio limits
  //
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.MinRingRatioLimit, CpuPowerMgmtBasicConfig->MinRingRatioLimit, CpuSetup->MinRingRatioLimit);
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.MaxRingRatioLimit, CpuPowerMgmtBasicConfig->MaxRingRatioLimit, CpuSetup->MaxRingRatioLimit);

  return EFI_SUCCESS;
}

#if FixedPcdGet8(PcdFspModeSelection) == 1
/**
  This routine is used to get Sec Platform Information Record2 Pointer.

  @param[in] PeiServices    Pointer to the PEI services table

  @retval GetSecPlatformInformation2 - The pointer of Sec Platform Information Record2 Pointer.
**/
EFI_SEC_PLATFORM_INFORMATION_RECORD2 *
GetSecPlatformInformation2 (
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_SEC_PLATFORM_INFORMATION2_PPI    *SecPlatformInformation2Ppi;
  EFI_SEC_PLATFORM_INFORMATION_RECORD2 *SecPlatformInformation2 = NULL;
  UINT64                               InformationSize;
  EFI_STATUS Status;

  //
  // Get BIST information from Sec Platform Information2 Ppi firstly
  //
  Status = PeiServicesLocatePpi (
             &gEfiSecPlatformInformation2PpiGuid,   // GUID
             0,                                     // Instance
             NULL,                                  // EFI_PEI_PPI_DESCRIPTOR
             (VOID ** ) &SecPlatformInformation2Ppi // PPI
             );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "LocatePpi SecPlatformInformationPpi2 Status - %r\n", Status));
    return NULL;
  }

  InformationSize = 0;

  Status = SecPlatformInformation2Ppi->PlatformInformation2 (
                                         (CONST EFI_PEI_SERVICES **) PeiServices,
                                         &InformationSize,
                                         SecPlatformInformation2
                                         );

  ASSERT (Status == EFI_BUFFER_TOO_SMALL);
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return NULL;
  }

  SecPlatformInformation2 = AllocatePool((UINTN)InformationSize);
  ASSERT (SecPlatformInformation2 != NULL);
  if (SecPlatformInformation2 == NULL) {
    return NULL;
  }

  //
  // Retrieve BIST data from SecPlatform2
  //
  Status = SecPlatformInformation2Ppi->PlatformInformation2 (
                                         (CONST EFI_PEI_SERVICES  **) PeiServices,
                                         &InformationSize,
                                         SecPlatformInformation2
                                         );
  DEBUG((DEBUG_INFO, "SecPlatformInformation2Ppi->PlatformInformation2 Status - %r\n", Status));
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  return SecPlatformInformation2;
}


/**
  This routine is used to get Sec Platform Information Record Pointer.

  @param[in] PeiServices    Pointer to the PEI services table

  @retval GetSecPlatformInformation2 - The pointer of Sec Platform Information Record Pointer.
**/
EFI_SEC_PLATFORM_INFORMATION_RECORD2 *
GetSecPlatformInformationInfoInFormat2 (
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_SEC_PLATFORM_INFORMATION_PPI     *SecPlatformInformationPpi;
  EFI_SEC_PLATFORM_INFORMATION_RECORD  *SecPlatformInformation = NULL;
  EFI_SEC_PLATFORM_INFORMATION_RECORD2 *SecPlatformInformation2;
  UINT64                               InformationSize;
  EFI_STATUS                           Status;

  //
  // Get BIST information from Sec Platform Information
  //
  Status = PeiServicesLocatePpi (
             &gEfiSecPlatformInformationPpiGuid,    // GUID
             0,                                     // Instance
             NULL,                                  // EFI_PEI_PPI_DESCRIPTOR
             (VOID ** ) &SecPlatformInformationPpi  // PPI
             );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "LocatePpi SecPlatformInformationPpi Status - %r\n", Status));
    return NULL;
  }

  InformationSize = 0;
  Status = SecPlatformInformationPpi->PlatformInformation (
                                        (CONST EFI_PEI_SERVICES  **) PeiServices,
                                        &InformationSize,
                                        SecPlatformInformation
                                        );

  ASSERT (Status == EFI_BUFFER_TOO_SMALL);
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return NULL;
  }

  SecPlatformInformation = AllocatePool((UINTN)InformationSize);
  ASSERT (SecPlatformInformation != NULL);
  if (SecPlatformInformation == NULL) {
    return NULL;
  }

  //
  // Retrieve BIST data from SecPlatform
  //
  Status = SecPlatformInformationPpi->PlatformInformation (
                                        (CONST EFI_PEI_SERVICES  **) PeiServices,
                                        &InformationSize,
                                        SecPlatformInformation
                                        );
  DEBUG((DEBUG_INFO, "FSP  SecPlatformInformation2Ppi->PlatformInformation Status - %r\n", Status));
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    FreePool(SecPlatformInformation);
    return NULL;
  }

  SecPlatformInformation2 = AllocatePool(sizeof (EFI_SEC_PLATFORM_INFORMATION_RECORD2));
  ASSERT (SecPlatformInformation2 != NULL);
  if (SecPlatformInformation2 == NULL) {
    FreePool(SecPlatformInformation);
    return NULL;
  }

  SecPlatformInformation2->NumberOfCpus = 1;
  SecPlatformInformation2->CpuInstance[0].CpuLocation = 0;
  SecPlatformInformation2->CpuInstance[0].InfoRecord.x64HealthFlags.Uint32 = SecPlatformInformation->x64HealthFlags.Uint32;

  FreePool(SecPlatformInformation);

  return SecPlatformInformation2;
}
#endif

/**
 Read Seamldr svn from TpmNv Index and update the policy

 @param[out] SeamldrSvn - UINT8 Seamldr Svn value

 @retval None
**/
VOID
UpdateTdxSeamldrSeSvn (
  UINT8 *SeamldrSvn
  )
{
  EFI_STATUS       Status = EFI_SUCCESS;
  TPM2B_MAX_BUFFER TpmNvSvnData;
  TPMI_RH_NV_AUTH  AuthHandle = OS_BIOS_ACM_HANDOFF;

  DEBUG ((DEBUG_INFO, "%a Begin\n", __func__));

  ZeroMem (&TpmNvSvnData, sizeof (TpmNvSvnData));

  // 1. Set buffer size to 1 byte
  TpmNvSvnData.size = 1;

    //2. read the index
  Status = Tpm2NvRead (
    AuthHandle,
    (TPMI_RH_NV_INDEX) OS_BIOS_ACM_HANDOFF,
    NULL,
    SEAMLDR_TPM_NV_INDEX_SIZE,
    0,
    &TpmNvSvnData
  );
  if (Status == EFI_SUCCESS) {
    *SeamldrSvn = TpmNvSvnData.buffer[SEAMLDR_SE_SVN_NV_BYTE_OFFSET];
    DEBUG ((DEBUG_INFO, "SeamldrSeSvn found in NV area = %x\n", *SeamldrSvn));
  } else {
    *SeamldrSvn = SEAMLDR_SE_SVN_DEFAULT_VALUE; // Default value
    DEBUG ((DEBUG_INFO, "SeamldrSeSvn is not found in NV area, setting to default value = %x\n", *SeamldrSvn));
  }

  DEBUG ((DEBUG_INFO, "%a End (%r)\n", __func__, Status));
}

/**
  This function performs CPU PEI Policy initialization.

  @param[in] SiPreMemPolicyPpi     The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi           The RC POSTMEM Policy PPI instance
  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicy (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
  EFI_STATUS                       Status;
  UINTN                            VariableSize;
  SETUP_DATA                       SetupData;
  CPU_SETUP                        CpuSetup;
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
  DPTF_CONFIG                      DptfConfig;
#endif
#if FixedPcdGet8(PcdFspModeSelection) == 1
  EFI_SEC_PLATFORM_INFORMATION_RECORD2 *SecPlatformInformation2;
#endif
#if FixedPcdGetBool (PcdTdxEnable) == 1
  UINT8 SeamldrSvn = 0;
#endif
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                             *FspsUpd;
#if FixedPcdGetBool (PcdTdxEnable) == 1
  VOID                             *FspmUpd;
#endif // PcdTdxEnable
#else
  CPU_INIT_CONFIG                  *CpuInitConfig;
  CPU_POWER_MGMT_BASIC_CONFIG      *CpuPowerMgmtBasicConfig;
  CPU_SECURITY_PREMEM_CONFIG       *CpuSecurityPreMemConfig;
#endif

  DEBUG ((DEBUG_INFO, "Update PeiCpuPolicyUpdate Pos-Mem Start\n"));

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#if FixedPcdGetBool (PcdTdxEnable) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#endif // PcdTdxEnable
#else
  CpuInitConfig           = NULL;
  CpuPowerMgmtBasicConfig = NULL;
  CpuSecurityPreMemConfig = NULL;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuInitConfigGuid, (VOID *) &CpuInitConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtBasicConfigGuid, (VOID *) &CpuPowerMgmtBasicConfig);
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

#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
  VariableSize = sizeof (DPTF_CONFIG);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"DptfConfig",
                               &gDptfConfigVariableGuid,
                               NULL,
                               &VariableSize,
                               &DptfConfig
                               );
  ASSERT_EFI_ERROR (Status);
#endif

  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.AesEnable, CpuInitConfig->AesEnable, CpuSetup.AES);
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.TxtEnable, CpuInitConfig->TxtEnable, CpuSetup.Txt);

  if (CpuSetup.PpinSupport == 0) {
    // Update value is related with Setup value, Need to check Policy Default
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PpinSupport, CpuInitConfig->PpinSupport, 0); ///< reference code policy is disabled
  } else if ((CpuSetup.PpinSupport == 1) && (CpuSetup.PpinEnableMode == 0)) {
    // Update value is related with Setup value, Need to check Policy Default
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PpinSupport, CpuInitConfig->PpinSupport, 2); ///< reference code policy is set to Auto. The feature is disabled if End of Manufacturing flag is set.
  } else if ((CpuSetup.PpinSupport == 1) && (CpuSetup.PpinEnableMode == 1)) {
    //Update value is related with Setup value, Need to check Policy Default
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PpinSupport, CpuInitConfig->PpinSupport, 1); ///< reference code policy is enabled
  }

  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *)FspsUpd)->FspsConfig.AvxDisable, CpuInitConfig->AvxDisable,  CpuSetup.AvxDisable);

  //
  // Directly assign the microcode flash location to FSPS UPD (API mode) or CpuInitConfig (Dispatch mode)
  // The microcode copy from flash to memory will be done just before microcode reload.
  //
  UPDATE_POLICY (((FSPS_UPD *)FspsUpd)->FspsConfig.MicrocodeRegionBase, CpuInitConfig->MicrocodePatchAddress,
    FixedPcdGet32 (PcdFlashFvMicrocodeBase) + FixedPcdGet32 (PcdMicrocodeOffsetInFv)
    );
  UPDATE_POLICY (((FSPS_UPD *)FspsUpd)->FspsConfig.MicrocodeRegionSize, CpuInitConfig->MicrocodePatchRegionSize,
    FixedPcdGet32 (PcdFlashFvMicrocodeSize) - FixedPcdGet32 (PcdMicrocodeOffsetInFv)
    );

  //
  // Init Power Management Policy Variables based on setup values
  //
  InitCpuPmConfigBySetupValues (
    &SetupData,
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
    &DptfConfig,
#endif
    &CpuSetup,
    SiPolicyPpi
    );

#if FixedPcdGet8(PcdFspModeSelection) == 1
  //
  // Get BIST information from Sec Platform Information
  //
  SecPlatformInformation2 = GetSecPlatformInformation2 ((EFI_PEI_SERVICES **) GetPeiServicesTablePointer ());
  if (SecPlatformInformation2 == NULL) {
    SecPlatformInformation2 = GetSecPlatformInformationInfoInFormat2 ((EFI_PEI_SERVICES **) GetPeiServicesTablePointer ());
  }

  ASSERT (SecPlatformInformation2 != NULL);

  if (SecPlatformInformation2 != NULL) {
    DEBUG((DEBUG_INFO, "SecPlatformInformation NumberOfCpus - %x\n", SecPlatformInformation2->NumberOfCpus));
    DEBUG((DEBUG_INFO, "SecPlatformInformation BIST - %x\n", SecPlatformInformation2->CpuInstance[0].InfoRecord.x64HealthFlags.Uint32));
  }
#endif

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
    //
    // AC Split Lock
    //
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.AcSplitLock, CpuInitConfig->AcSplitLock, CpuSetup.AcSplitLock);
#endif

  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.X2ApicEnable, CpuInitConfig->X2ApicEnable, CpuSetup.X2ApicEnable);

#if FixedPcdGetBool (PcdTdxEnable) == 1
    //
    // Update TdxSeamldrSvn Policy
    //
    UpdateTdxSeamldrSeSvn (&SeamldrSvn);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TdxSeamldrSvn, CpuSecurityPreMemConfig->TdxSeamldrSvn, SeamldrSvn);
#endif

  return EFI_SUCCESS;
}
