/** @file
  Source code file for CNV Pre-Memory PEI module

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

#include <PiPei.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiServicesLib.h>
#include <SetupVariable.h>
#include <Library/GpioV2WrapperLib.h>
#include <Register/GpioV2PcdPins.h>
#include <CnvVfrSetupMenuHii.h>
#include <Library/BaseMemoryLib.h>

static EFI_PEI_PPI_DESCRIPTOR  mFvCnvDispatchFlagPpi[] = {
  {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiFvCnvDispatchFlagPpiGuid,
  NULL
  }
};

/**
  Configures GPIO

  @param[in]  GpioDefinition  Point to Platform Gpio table
  @param[in]  GpioTableCount  Number of Gpio table entries

**/
VOID
ConfigureGpio (
  IN GPIOV2_INIT_CONFIG               *GpioDefinition,
  IN UINT16                           GpioTableCount
  )
{
  EFI_STATUS          Status;

  Status = GpioV2ConfigurePads (GpioDefinition, GpioTableCount);
  ASSERT_EFI_ERROR (Status);

  Status = GpioV2SetConflictCheckData (GpioDefinition, GpioTableCount);
  ASSERT_EFI_ERROR (Status);
}

/**
  This function handles CNV Init task after PeiReadOnlyVariable2 PPI produced

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
CnvInitPreMem (
  VOID
  )
{

  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  UINTN                             VariableSize;
  SETUP_DATA                        SystemConfiguration;
  CNV_VFR_CONFIG_SETUP              CnvSetup;

  //
  // Locate Setup variables
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SystemConfiguration
                               );


  VariableSize = sizeof (CNV_VFR_CONFIG_SETUP);
  ZeroMem (&CnvSetup, VariableSize);
#if FixedPcdGetBool (PcdCnvFeatureEnable) == 1
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"CnvSetup",
                               &gCnvFeatureSetupGuid,
                               NULL,
                               &VariableSize,
                               &CnvSetup
                               );
#endif
  //  install PPI descriptor for CNV, based on policy settings.
  if ((SystemConfiguration.EfiNetworkSupport != 0) ||
      (CnvSetup.PrebootBleEnable == 1)) {
    Status = PeiServicesInstallPpi (mFvCnvDispatchFlagPpi);
    ASSERT_EFI_ERROR (Status);
    #if FixedPcdGetBool (PcdCnvBinLoadFromESP) == 1
    if(CnvSetup.CnvCompatibilityCheck == 1) {
      PcdSetBoolS (PcdCnvDispatch, TRUE);
      PcdSetBoolS (PcdCnvUnloadRequirement, TRUE);
    }
    #endif
  }

  return Status;
}


/**
  CNV Init before memory PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
ConnectivityPreMemEntryPoint(
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS     Status;

  Status = CnvInitPreMem ();

  return Status;
}
