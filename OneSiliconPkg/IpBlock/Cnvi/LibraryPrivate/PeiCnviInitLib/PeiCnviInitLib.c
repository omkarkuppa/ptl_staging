/** @file
  This files contains CNVi devices configuration

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include <IndustryStandard/Pci30.h>
#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CnviLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/HobLib.h>
#include <Library/SiScheduleResetLib.h>
#include <Library/ConfigBlockLib.h>
#include <CnviHandle.h>
#include <CnviConfigHob.h>
#include <CnviConfig.h>
#include <Register/CnviRegs.h>

/**
  Schedules Global Reset required to change CNVi static power gating state.
**/
VOID
CnviScheduleGlobalReset (
  VOID
  )
{
  PCH_RESET_DATA             ResetData;

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
  StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
  SiScheduleResetSetType (EfiResetPlatformSpecific, &ResetData);
}

/**
  Enable CNVi

  @param[in] CnviHandle          Pointer to CNVi Handle Structure

  @retval EFI_SUCCESS    CNVi is enabled
  @retval EFI_NOT_FOUND  CNVi is not present
**/
EFI_STATUS
CnviEnable (
  IN CNVI_HANDLE             *CnviHandle
  )
{
  CNVI_PRIVATE_CONFIG        *CnviPrivateConfig;
  CNVI_CALLBACKS             *CnviCallbacks;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  CnviPrivateConfig = CnviHandle->PrivateConfig;
  CnviCallbacks     = CnviHandle->Callbacks;

  //
  // If CNVi is static power gated but has not been fused out, clear the static power gate bit and do a reset.
  //
  if (CnviPrivateConfig->IsFunctionDisabled) {
    CnviCallbacks->SetFunction (FunctionEnable);
    //
    // Schedule reset for enabling to take effect
    //
    CnviScheduleGlobalReset ();
  }

  //
  // Check if CNVi CRF module is present
  //
  if (!CnviPrivateConfig->IsCrfModulePresent) {
    DEBUG ((DEBUG_INFO, "CNVi CRF module is not present\n"));
    //
    // Clear BT_EN and BT_IF_SELECT to disable UTIM2UTIM bridge if CNVi is not present.
    //
    CnviCallbacks->BtSetCore (CnviHandle, FunctionDisable);
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}

/**
  Disable CNVi

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
**/
VOID
CnviDisable (
  IN CNVI_HANDLE             *CnviHandle
  )
{
  CNVI_CONFIG                *CnviConfig;
  CNVI_PRIVATE_CONFIG        *CnviPrivateConfig;
  CNVI_CONTROLLER            *CnviController;
  CNVI_CALLBACKS             *CnviCallbacks;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  CnviConfig        = CnviHandle->Config;
  CnviPrivateConfig = CnviHandle->PrivateConfig;
  CnviController    = CnviHandle->Controller;
  CnviCallbacks     = CnviHandle->Callbacks;

  //
  // Clear BT_EN and BT_IF_SELECT to disable UTIM2UTIM bridge if CNVi is disabled.
  //
  CnviCallbacks->BtSetCore (CnviHandle, FunctionDisable);

  //
  // Check if CNVi is static power gated
  //
  if (CnviPrivateConfig->IsFunctionDisabled) {
    DEBUG ((DEBUG_INFO, "CNVi found already static power gated\n"));
    return;
  }

  //
  // Put CNVi in D3 power state
  //
  PciSegmentOr32 (CnviController->WifiPciCfgBaseAddr + R_CNVI_CFG_PMCSR, (UINT32) B_CNVI_CFG_PMCSR_PWR_STATE);
  if (CnviConfig->BtInterface == CnviBtIfPci) {
    PciSegmentOr32 (CnviController->BtPciCfgBaseAddr + R_CNVI_CFG_PMCSR, (UINT32) B_CNVI_CFG_PMCSR_PWR_STATE);
  }

  CnviCallbacks->SetFunction (FunctionDisable);

  //
  // Schedule reset for disabling to take effect
  //
  CnviScheduleGlobalReset ();
}

/**
  Configures CNVi Wi-Fi.

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
**/
VOID
CnviWifiConfig (
  IN CNVI_HANDLE             *CnviHandle
  )
{
  UINT16                     Data16;
  CNVI_CONFIG                *CnviConfig;
  CNVI_CONTROLLER            *CnviController;
  CNVI_PRIVATE_CONFIG        *PrivateConfig;
  REGISTER_ACCESS            *CnviPcrAccess;
  CNVI_CALLBACKS             *CnviCallbacks;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  CnviConfig     = CnviHandle->Config;
  PrivateConfig  = CnviHandle->PrivateConfig;
  CnviController = CnviHandle->Controller;
  CnviPcrAccess  = CnviHandle->SbAccess;
  CnviCallbacks  = CnviHandle->Callbacks;

  if (CnviConfig->WifiCore == TRUE) {
    //
    // Check LTR capability and enable it
    //
    Data16 = PciSegmentRead16 (CnviController->WifiPciCfgBaseAddr + R_CNVI_CFG_GIO_DEV_CAP_2);
    if (Data16 & B_CNVI_CFG_GIO_DEV_CAP_2_LTR_MEC_SUP) {
      PciSegmentOr16 (CnviController->WifiPciCfgBaseAddr + R_CNVI_CFG_GIO_DEV_2, B_CNVI_CFG_GIO_DEV_2_LTR_MEC_EN);
    }

    //
    // Set Interrupt Pin
    //
    PciSegmentWrite8 (CnviController->WifiPciCfgBaseAddr + PCI_INT_PIN_OFFSET, PrivateConfig->WifiInterruptPin);

    //
    // Assign interrupt number
    //
    CnviPcrAccess->AndThenOr32 (
                     CnviPcrAccess,
                     R_CNVI_PCR_WIDGET_CONTROL,
                     (UINT32)~B_CNVI_PCR_WIDGET_CONTROL_MEW_INT,
                     PrivateConfig->WifiIrq
                     );
  } else {
    //
    // Disable Wi-Fi core
    //
    PciSegmentOr16 (CnviController->WifiPciCfgBaseAddr + R_CNVI_CFG_GIO_DEV, B_CNVI_CFG_GIO_DEV_INIT_FNC_LV_RS);
    CnviCallbacks->WifiSetCore (CnviHandle, FunctionDisable);
  }
}

/**
  The function update CNVi config HOB

  @param[in] CnviConfig             Pointer to Cnvi Post-Mem Configuration
**/
VOID
BuildCnviConfigHob (
  IN CNVI_CONFIG          *CnviConfig
  )
{
  CNVI_CONFIG_HOB      *CnviConfigHob;
  EFI_STATUS           Status;
  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi;
  CNVI_PREMEM_CONFIG   *CnviPreMemConfig;

  CnviConfigHob = BuildGuidHob (&gCnviConfigHobGuid, sizeof (CNVI_CONFIG_HOB));
  ASSERT (CnviConfigHob != 0);
  if (CnviConfigHob == NULL) {
    return;
  }
  CnviConfigHob->WifiCore                = CnviConfig->WifiCore;
  CnviConfigHob->BtCore                  = CnviConfig->BtCore;
  CnviConfigHob->BtAudioOffload          = CnviConfig->BtAudioOffload;
  CnviConfigHob->BtAudioOffloadInterface = CnviConfig->BtAudioOffloadInterface;
  CnviConfigHob->BtInterface             = CnviConfig->BtInterface;
  CnviConfigHob->DdrRfim                 = 0;

  Status = PeiServicesLocatePpi (
             &gPchPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  if (EFI_ERROR (Status)) {
    Status = PeiServicesLocatePpi (
               &gSiPreMemPolicyPpiGuid,
               0,
               NULL,
               (VOID **) &SiPreMemPolicyPpi
               );
  }
  if (!EFI_ERROR (Status)) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCnviPreMemConfigGuid, (VOID *) &CnviPreMemConfig);
    if (!EFI_ERROR (Status)) {
      //
      // The register for DDR RFI Mitigation is programmed when DdrRfim is set and CNVi CRF module is present.
      //
      CnviConfigHob->DdrRfim = ((CnviPreMemConfig->DdrRfim == 1) && IsCnviWifiEnabled ()) ? 1 : 0;
    }
  }
}

/**
  Initialize CNVi devices

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
**/
VOID
CnviInit (
  IN CNVI_HANDLE             *CnviHandle
  )
{
  EFI_STATUS                 Status;
  CNVI_CONFIG                *CnviConfig;
  CNVI_CALLBACKS             *CnviCallbacks;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  if (CnviHandle == NULL) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
    return;
  }

  Status            = EFI_SUCCESS;
  CnviConfig        = CnviHandle->Config;
  CnviCallbacks     = CnviHandle->Callbacks;

  if (CnviConfig->Mode == CnviModeAuto) {
    //
    // BIOS menu option configured to [Auto] - Check if CNVi is enabled
    //
    if (CnviEnable (CnviHandle) == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "CNVi discovered\n"));

      CnviCallbacks->ConfigPinFunction (CnviHandle, ConfigCrfPin);
      //
      // Configure CNVi Wi-Fi
      //
      CnviWifiConfig (CnviHandle);
    }
  } else {
    //
    // Disable CNVi
    //
    CnviDisable (CnviHandle);
  }

  CnviCallbacks->BtPreInit (CnviHandle);

  BuildCnviConfigHob (CnviConfig);

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}

/**
  Configures CNVi Bluetooth PCI interface

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
**/
VOID
CnviBtConfig (
  IN CNVI_HANDLE             *CnviHandle
  )
{
  UINT16                     Data16;
  CNVI_CONFIG                *CnviConfig;
  CNVI_CONTROLLER            *CnviController;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  CnviConfig     = CnviHandle->Config;
  CnviController = CnviHandle->Controller;

  if (CnviConfig->BtInterface != CnviBtIfPci) {
    return;
  }

  //
  // Check LTR capability and enable it
  //
  Data16 = PciSegmentRead16 (CnviController->BtPciCfgBaseAddr + R_CNVI_CFG_GIO_DEV_CAP_2);
  if (Data16 & B_CNVI_CFG_GIO_DEV_CAP_2_LTR_MEC_SUP) {
    PciSegmentOr16 (CnviController->BtPciCfgBaseAddr + R_CNVI_CFG_GIO_DEV_2, B_CNVI_CFG_GIO_DEV_2_LTR_MEC_EN);
  }
}

/**
  Configure CNVi Bluetooth

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
**/
VOID
CnviConfigureBt (
  IN CNVI_HANDLE             *CnviHandle
  )
{
  EFI_STATUS                 Status;
  CNVI_CONFIG                *CnviConfig;
  CNVI_PRIVATE_CONFIG        *CnviPrivateConfig;
  CNVI_CALLBACKS             *CnviCallbacks;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  if (CnviHandle == NULL) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
    return;
  }

  Status            = EFI_SUCCESS;
  CnviConfig        = CnviHandle->Config;
  CnviPrivateConfig = CnviHandle->PrivateConfig;
  CnviCallbacks     = CnviHandle->Callbacks;

  if (!CnviPrivateConfig->IsCrfModulePresent) {
    DEBUG ((DEBUG_INFO, "CNVi CRF module is not present\n"));
    return;
  }

  if (CnviConfig->BtCore) {
    CnviCallbacks->BtSetInterface (CnviHandle);
    CnviBtConfig (CnviHandle);
    CnviCallbacks->ConfigPinFunction (CnviHandle, ConfigBtI2sPin);
    CnviCallbacks->BtSetCore (CnviHandle, FunctionEnable);
  } else {
    CnviCallbacks->ConfigPinFunction (CnviHandle, ConfigBtDisPin);
    CnviCallbacks->BtSetCore (CnviHandle, FunctionDisable);
  }

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}
