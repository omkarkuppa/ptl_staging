/** @file
  Source code file for Board Init PEI module

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

@par Specification
**/

#include <CpuRegs.h>
#include <SetupVariable.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Register/GpioV2PcdPins.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <PchPolicyCommon.h>
#include <Library/SiPolicyLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/GpioV2AccessLib.h>

#include <Guid/FirmwareFileSystem2.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Guid/SystemNvDataGuid.h>

#include <Library/CnviLib.h>
#include <Library/EcLib.h>
#include <Library/EcMiscLib.h>
#include <IntelRcStatusCode.h>
#include <EcCommon.h>
#include <Ppi/Spi.h>
#include <Library/PciSegmentLib.h>
#include <Register/PchRegs.h>
#include <PlatformBoardId.h>
#include <Core/Pei/PeiMain.h>
#include <Library/PchPciBdfLib.h>
#include <Ppi/IGpuPlatformPolicyPpi.h>
#include <Library/PeiGetFvInfoLib.h>
#include <Library/BiosIdLib.h>

#if FixedPcdGetBool (PcdDTbtEnable) == 1
#include <Ppi/PeiDTbtPolicy.h>
#include <Library/PeiTbtTaskDispatchLib.h>
#include <Library/PeiDTbtInitLib.h>
#endif

EFI_STATUS
EFIAPI
GpioV2LockCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

#if FixedPcdGetBool(PcdFspModeSelection) == 1
static EFI_PEI_NOTIFY_DESCRIPTOR  mGpioV2LockCallbackNotifyList  = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gFspSiliconInitDonePpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) GpioV2LockCallback
};
#else
static EFI_PEI_NOTIFY_DESCRIPTOR  mGpioV2LockCallbackNotifyList  = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) GpioV2LockCallback
};
#endif

EFI_STATUS
EFIAPI
PlatformInitAdvancedEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR  mEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) PlatformInitAdvancedEndOfPei
};

#if FixedPcdGetBool (PcdDTbtEnable) == 1
/**
  This function handles TbtInit task at the end of PEI

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS        The function completes successfully
  @retval     RETURN_UNSUPPORTED DTBT is not supported
**/
EFI_STATUS
EFIAPI
DTbtInitEndOfPei (
  VOID
  )
{
  EFI_STATUS      Status;
  UINT8           Index;
  BOOLEAN         DTbtExisted;
  PEI_DTBT_POLICY *PeiDTbtConfig;

  DEBUG ((DEBUG_INFO, "DTbtInitEndOfPei Entry\n"));

  Status       = EFI_SUCCESS;
  PeiDTbtConfig = NULL;
  Index        = 0;
  DTbtExisted  = FALSE;

  Status = PeiServicesLocatePpi (
             &gPeiDTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiDTbtConfig
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, " gPeiDTbtPolicyPpiGuid Not installed!!!\n"));
    return RETURN_UNSUPPORTED;
  }

  if (PeiDTbtConfig != NULL) {
    for (Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
      if (PeiDTbtConfig->DTbtControllerConfig[Index].DTbtControllerEn == 1) {
        DTbtExisted = TRUE;
        break;
      }
    }

    if (DTbtExisted == TRUE) {
      //
      // Dispatch DTBT task table
      //
      TbtTaskDispatch (DTbtCallTable, (VOID *) PeiDTbtConfig);
    }
  }

  return EFI_SUCCESS;
}
#endif

#if FixedPcdGetBool (PcdTelemetryFeatureEnable) == 1
#include <Library/PmcLib.h>
#include <Register/PmcRegs.h>

/**
  Clear PMC GBLRST_CAUSE register to avoid confusion when determining reset reason
  in the following boots.

  @retval None.

**/
VOID
ClearPmcGblrstCause (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "Clear GBLRST_CAUSE\n"));
  MmioWrite32 ((PmcGetPwrmBase () + R_PMC_PWRM_GBLRST_CAUSE0), 0xFFFFFFFF);
  MmioWrite32 ((PmcGetPwrmBase () + R_PMC_PWRM_GBLRST_CAUSE1), 0xFFFFFFFF);
}
#endif

/**
  This function handles PlatformInit task at the end of PEI

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
PlatformInitAdvancedEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
#if FixedPcdGetBool (PcdDTbtEnable) == 1
  EFI_STATUS     Status;
  //
  // Perform DTBT init
  //
  Status = DTbtInitEndOfPei ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: DTBT Initial fail, (%r)\n", __FUNCTION__, Status));
  }
#endif

#if FixedPcdGetBool (PcdTelemetryFeatureEnable) == 1
  ClearPmcGblrstCause ();
#endif

  return EFI_SUCCESS;
}

/**
  This callback function invokes gpio lock procedure.

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
GpioV2LockCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                          Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI     *VariableServices;
  UINTN                               VariableSize;
  PCH_SETUP                           PchSetup;
  GPIOV2_SERVICES                     *GpioServices;

  DEBUG ((DEBUG_INFO, "[GPIOV2][GpioV2LockCallback]: START\n"));

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices       // PPI
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

  Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);

    GpioServices->UnlockAllPads = PchSetup.UnlockGpioPads;
  GpioV2LockAll ();

  DEBUG ((DEBUG_INFO, "[GPIOV2][GpioV2LockCallback]: END\n"));

  return EFI_SUCCESS;
}

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

  DEBUG ((DEBUG_INFO, "ConfigureGpio() Start\n"));

  Status = GpioV2ConfigurePads (GpioDefinition, GpioTableCount);
  ASSERT_EFI_ERROR (Status);

  Status = GpioV2SetConflictCheckData (GpioDefinition, GpioTableCount);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "ConfigureGpio() End\n"));
}

/**
  Configure advanced GPIO

**/
VOID
GpioInitAdvanced (
  VOID
  )
{
  return;
}

/**
  Platform Init PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
BoardInitAdvancedPostMemEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                        Status;
  PEI_CORE_INSTANCE                 *PrivateData;
  UINTN                             CurrentFv;
  PEI_CORE_FV_HANDLE                *CoreFvHandle;
  VOID                              *HobData;
  SETUP_DATA                        SystemConfiguration;
  UINTN                             VariableSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI*  VariableServices;
  BIOS_ID_IMAGE                     BiosIdImage;

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_BOARD_INIT_POSTMEM_ENTRY); // PostCode (BOARD_INIT_POSTMEM_ENTRY);


  Status = GetBiosId (&BiosIdImage);

  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "+==================================================+\n"));
    DEBUG ((DEBUG_INFO, "| BIOS version [%s]  |\n", &BiosIdImage.BiosIdString));
    DEBUG ((DEBUG_INFO, "+==================================================+\n"));
  }

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,  // GUID
             0,                                 // INSTANCE
             NULL,                              // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices        // PPI
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
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to get System Configuration and set the configuration to production mode!\n"));
  }

  GpioInitAdvanced ();
  //
  // Build a HOB to show current FV location for SA policy update code to consume.
  //
  PrivateData = PEI_CORE_INSTANCE_FROM_PS_THIS (PeiServices);
  CurrentFv = PrivateData->CurrentPeimFvCount;
  CoreFvHandle = &(PrivateData->Fv[CurrentFv]);

  HobData = BuildGuidHob (
             &gPlatformInitFvLocationGuid,
             sizeof (VOID *)
             );
  ASSERT (HobData != NULL);
  CopyMem (HobData, (VOID *) &CoreFvHandle, sizeof (VOID *));


  Status = PeiServicesNotifyPpi (&mGpioV2LockCallbackNotifyList);

  ///
  ///
  //
  // Performing PlatformInitEndOfPei after EndOfPei PPI produced
  //
  Status = PeiServicesNotifyPpi (&mEndOfPeiNotifyList);
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_BOARD_INIT_POSTMEM_EXIT); // PostCode (BOARD_INIT_POSTMEM_EXIT);

  return Status;
}
