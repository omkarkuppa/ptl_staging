/** @file
  Implementation of PlatformInitRecovery in PEI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <Uefi.h>
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Guid/SysFwUpdateProgress.h>
#include <Ppi/RecoveryModule.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/HobLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/TopSwapSupportLib.h>
#include <Library/ResiliencySupportLib.h>
#include <Library/PlatformWdtLib.h>
#include <Library/BootGuardLib.h>
#include <Register/GenerationMsr.h>
#include <Txt.h>

#define FIT_SUCCESSFUL                            0x0
#define FIT_STARTUP_ACM_ENTRY                     0x2
#define FIT_STARTUP_ACM_NOT_SUPPORTED             0x1

UINT8                                             mWdtTimeOut = 3;

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR mPpiListRecoveryBootMode = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiBootInRecoveryModePpiGuid,
  NULL
};

/**
  Provide the functionality of the recovery module.

  @retval     EFI_SUCCESS  The function completed successfully.
**/
EFI_STATUS
EFIAPI
InitializeRecovery (
  VOID
  );

/**
  Callback on BootMode availbale
**/
EFI_STATUS
EFIAPI
PlatformInitRecoveryCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Notify on gEfiBeforePostMemFvReportPpiGuid
**/
STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mBeforePostMemFvReportNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiBeforePostMemFvReportPpiGuid,
  PlatformInitRecoveryCallback
};

/**
  Notify on gEfiPeiMasterBootModePpiGuid
**/
STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mPlatformInitRecoveryNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMasterBootModePpiGuid,
  PlatformInitRecoveryCallback
};

/**
  Callback on memory availbale
**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Notify on gEfiPeiMemoryDiscoveredPpiGuid
**/
STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mMemDiscoveredNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) MemoryDiscoveredPpiNotifyCallback
};

/**
  Load recovery module from external storage (such as SATA or NVMe).

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  The typedef structure of the notification
                                descriptor. Not used in this function.
  @param[in]  Ppi               The typedef structure of the PPI descriptor.
                                Not used in this function.
  @retval EFI_SUCCESS           The function completed successfully
**/
EFI_STATUS
EFIAPI
LoadRecoveryModule (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Notify list on VirtualBlockIoPpi
**/
STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mBlockIoNotifyList[] = {
  //
  // Use EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH flag rather than EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK
  // to ensure all VirtualBlockIoPpi notify callbacks (especially the one in FatLite) are executed
  // before invoking LoadRecoveryModule().
  //
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH,
    &gEfiPeiVirtualBlockIoPpiGuid,  //gEfiPeiDeviceRecoveryModulePpiGuid
    LoadRecoveryModule
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPeiVirtualBlockIo2PpiGuid,  //gEfiPeiDeviceRecoveryModulePpiGuid
    LoadRecoveryModule
  }
};

/**
  Load recovery module from external storage (such as SATA or NVMe).

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  The typedef structure of the notification
                                descriptor. Not used in this function.
  @param[in]  Ppi               The typedef structure of the PPI descriptor.
                                Not used in this function.

  @retval EFI_SUCCESS           The function completed successfully
**/
EFI_STATUS
EFIAPI
LoadRecoveryModule (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                                  Status;
  EFI_PEI_RECOVERY_MODULE_PPI                *PeiRecovery;

  DEBUG ((DEBUG_INFO, "LoadRecoveryModule Entry\n"));

  //
  // load recovery file and install FV (original done in DXEIPL, but we copy it here)
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiRecoveryModulePpiGuid,
             0,
             NULL,
             (VOID **) &PeiRecovery
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate Pei Recovery Module Ppi Failed.(Status = %r)\n", Status));
    return Status;
  }

  Status = PeiRecovery->LoadRecoveryCapsule ((EFI_PEI_SERVICES **)PeiServices, PeiRecovery);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Load Recovery Capsule Failed.(Status = %r)\n", Status));
  }

  return Status;
}

/**
  Detect if there was an incompleted system firmware update.

  @retval  TRUE   Incompleted system firmware is detected.
  @retavl  FALSE  Not detected.

**/
BOOLEAN
IsSystemFirmwareUpdateInProgress (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *PeiReadOnlyVarPpi;
  UINTN                             VarSize;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS   UpdateProgress;
  VOID                              *HobPtr;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &PeiReadOnlyVarPpi
             );

  ASSERT_EFI_ERROR (Status);

  if (Status == EFI_SUCCESS) {
    VarSize = sizeof (SYSTEM_FIRMWARE_UPDATE_PROGRESS);
    Status = PeiReadOnlyVarPpi->GetVariable (
                                  PeiReadOnlyVarPpi,
                                  SYSFW_UPDATE_PROGRESS_VARIABLE_NAME,
                                  &gSysFwUpdateProgressGuid,
                                  NULL,
                                  &VarSize,
                                  &UpdateProgress
                                  );
    if (Status == EFI_SUCCESS) {
      DEBUG ((
        DEBUG_INFO,
        "Detected system firmware update is in progress: Component 0x%x, Progress 0x%x\n",
        UpdateProgress.Component,
        UpdateProgress.Progress
        ));

      if (GetBiosResiliencyType () == NOT_SUPPORT_RECOVERY) {
        //
        // Add check for handle res bios update to bkc bios case.
        //
        if (UpdateProgress.Component == UpdatingResiliency) {
          return FALSE;
        }
      }

      //
      // Add check for NOT_STARTED case
      //
      if (UpdateProgress.Component == NotStarted) {
        return FALSE;
      }
      HobPtr = BuildGuidDataHob (&gSysFwUpdateProgressGuid, &UpdateProgress, sizeof (UpdateProgress));
      ASSERT (HobPtr != 0);
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Callback once there is main memory

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                        Status;
  EFI_BOOT_MODE                     BootMode;
  EFI_HOB_GUID_TYPE                 *GuidHob;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS   *UpdateProgress;

  GuidHob = NULL;
  UpdateProgress = NULL;

  GuidHob = GetFirstGuidHob (&gSysFwUpdateProgressGuid);
  if (GuidHob != NULL) {
    UpdateProgress = (SYSTEM_FIRMWARE_UPDATE_PROGRESS *) GET_GUID_HOB_DATA (GuidHob);
  }

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  ///
  /// If S3 resume or legacy recovery, then we are done
  ///
  if ((BootMode == BOOT_ON_S3_RESUME) || (BootMode == BOOT_IN_RECOVERY_MODE)) {
    return EFI_SUCCESS;
  }

  //
  // Check if flash update mode and recovery servie are needed.
  //
  if ((UpdateProgress != NULL) || IsTopSwapEnabled ()) {
    //
    // Indicate an interrupted update or post-bios update (UpdatingResiliency) or
    // somehow BIOS ROM is corrupted. Therefore flash update would be required.
    // Set flash update mode again in case MRC modified the boot mode when MrcBootMode is bmCold or bmCold.
    //
    PeiServicesSetBootMode (BOOT_ON_FLASH_UPDATE);

    if (!IsTopSwapEnabled () && (UpdateProgress != NULL) && (UpdateProgress->Component == UpdatingResiliency)) {
      //
      // This is a post-bios update phase. System is supposed to boot from ROM.
      // No recovery serivice is required.
      //
      DEBUG ((DEBUG_INFO, "UpdatingResiliency phase, recovery service is not needed\n"));
    } else {
      //
      // Initialize recovery servie to load recovery image or capsules on disk.
      //
      DEBUG ((DEBUG_INFO, "Initialize recovery servie\n"));

      Status = InitializeRecovery ();
      ASSERT_EFI_ERROR (Status);
      Status = PeiServicesNotifyPpi (mBlockIoNotifyList);
      ASSERT_EFI_ERROR (Status);
    }
  }

  return Status;
}


/**
  Callback on gEfiPeiMasterBootModePpiGuidiGuid so install gEfiPeiBootInRecoveryModePpiGuid.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The MasterBootMode PPI.  Not used.

  @retval EFI_SUCCESS          - The function completed successfully.
**/
EFI_STATUS
EFIAPI
PlatformInitRecoveryCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_BOOT_MODE                     BootMode;
  EFI_HOB_GUID_TYPE                 *GuidHob;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS   *UpdateProgress;

  GuidHob                           = NULL;
  UpdateProgress                    = NULL;

  PeiServicesGetBootMode (&BootMode);

  if ((BootMode != BOOT_IN_RECOVERY_MODE) && (BootMode != BOOT_ON_S3_RESUME)) {
    //
    // Check if flash update might be needed even if there is no Capsule detected.
    //
    if (IsSystemFirmwareUpdateInProgress () || IsTopSwapEnabled ()) {
      if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
        PeiServicesSetBootMode (BOOT_ON_FLASH_UPDATE);
      } else {
        GuidHob = GetFirstGuidHob (&gSysFwUpdateProgressGuid);
        if (GuidHob != NULL) {
          UpdateProgress = (SYSTEM_FIRMWARE_UPDATE_PROGRESS *) GET_GUID_HOB_DATA (GuidHob);
        }
        //
        // No need to change the boot mode when it is BKC BIOS and the update progress is UpdatingResiliency
        //
        if (UpdateProgress != NULL) {
          if (UpdateProgress->Component != UpdatingResiliency) {
            PeiServicesSetBootMode (BOOT_ON_FLASH_UPDATE);
          }
        }
      }

      //
      //  Install recovery ppi to make RstVmdPeim module install blockio
      //
      PeiServicesInstallPpi (&mPpiListRecoveryBootMode);
    }
  }

  return EFI_SUCCESS;
}

/**
  Check if ACM policy (e.g., BPM, KM) is corrupted

  @retval TRUE  - ACM policy is corrupted
  @retval FALSE - ACM policy is valid

**/
BOOLEAN
IsAcmPolicyCorrupted (
  VOID
  )
{
  UINT64                      BootGuardBootStatus;

  BootGuardBootStatus  = 0;

  BootGuardBootStatus = *(UINT64 *) (UINTN) (TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_BOOTSTATUS);
  DEBUG ((DEBUG_INFO, "R_CPU_BOOT_GUARD_BOOTSTATUS = 0x%016lx\n", BootGuardBootStatus));

  //
  // Check the BIT54 on ACM policy valid bit.
  // If ACM policy is valid then it would be set to 1, otherwise 0.
  //
  if ((BootGuardBootStatus & BIT54) == 0) {
    DEBUG ((DEBUG_ERROR, "ACM policy is corrupted\n"));
    return TRUE;
  }

  return FALSE;
}

/**
  This function checks whether there is a FIT BIOS error or ACM policy error.

  @retval TRUE  - FIT or policy error is detected
  @retval FALSE - FIT or policy error is not detected
**/
BOOLEAN
IsFitOrPolicyError (
  VOID
  )
{
  UINT8                       FitEntryType;
  UINT8                       FitErrorCode;
  MSR_FIT_BIOS_ERROR_REGISTER FitData;
  UINT64                      AcmPolicyStatus;

  DEBUG ((DEBUG_INFO, "Check MSR FIT or ACM policy error\n"));

  AcmPolicyStatus = 0;

  //
  // Check FIT BIOS Error in MSR first
  //
  FitData.Uint64 = AsmReadMsr64 (MSR_FIT_BIOS_ERROR);
  FitEntryType = (UINT8) FitData.Bits.EntryType;
  FitErrorCode = (UINT8) FitData.Bits.ErrorCode;
  DEBUG ((DEBUG_INFO, "FitEntryType:0x%x\n", FitEntryType));
  DEBUG ((DEBUG_INFO, "FitErrorCode:0x%x\n", FitErrorCode));

  if ((FitEntryType == FIT_STARTUP_ACM_ENTRY) && \
      (FitErrorCode == FIT_STARTUP_ACM_NOT_SUPPORTED)) {
    //
    // ACM is not supported (Profile 0). Skip the following check.
    //
    DEBUG ((DEBUG_INFO, "Skip FIT ACM Entry error when BtG is unsupported.\n"));
    return FALSE;
  }

  if (FitErrorCode != FIT_SUCCESSFUL) {
    //
    // FIT error is detected. (e.g., Microcode or ACM loading failure)
    //
    DEBUG ((DEBUG_ERROR, "Fit Error in MSR is detected.\n"));
    return TRUE;
  }

  //
  // FIT error in MSR is not detected.
  // Further check FIT table and ACM policy valid bit.
  // Below check is targeted on 0T profile specifically.
  // In 4/4T/5/5T profile, ACM policy corruption is supposed to be handled by ACM/CSME.
  //
  AcmPolicyStatus = *(UINT64 *) (UINTN) (TXT_PUBLIC_BASE + R_CPU_ACM_POLICY_STATUS);
  DEBUG ((DEBUG_INFO, "R_CPU_ACM_POLICY_STATUS = 0x%016lx\n", AcmPolicyStatus));

  //
  // Check ACM policy
  //
  if (IsAcmPolicyCorrupted ()) {
    DEBUG ((DEBUG_ERROR, "Invalid ACM policy detected.\n"));
    return TRUE;
  }

  DEBUG ((DEBUG_INFO, "Fit or policy error is not detected.\n"));
  return FALSE;
}

/**
  Function to arm the watchdog timer to monitor the boot healthy.

  @param[in] TimeOut  Value to second to arm the watchdog timer.
                      1 stands for 1 second, max is 255 seconds with input 0xFF.

  @retval EFI_SUCCESS       The operation succeed.
  @retval EFI_DEVICE_ERROR  The operation failed.

**/
EFI_STATUS
ArmMonitorBootHealthWdt (
  IN UINT8  TimeOut
  )
{
  EFI_STATUS     Status;
  EFI_BOOT_MODE  BootMode;

  Status = EFI_SUCCESS;

  PeiServicesGetBootMode (&BootMode);

  if (BootMode != BOOT_ON_S3_RESUME) {
    Status = ArmPlatformWdt (TimeOut);
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "Arm the EC WDT timer [0x%x] to monitor boot healthy\n", TimeOut));

  return Status;
}

/**
  Entry point of this module.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  Describes the list of possible PEI Services.

  @retval EFI_SUCCESS     The function completed successfully.

**/
EFI_STATUS
EFIAPI
PlatformInitRecoveryEntryPoint (
  IN       EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES         **PeiServices
  )
{
  //
  // Disarm the EC WDT first since it might be armed from capsule update
  // or had been expired.
  //
  DisarmPlatformWdt ();
  DEBUG ((DEBUG_INFO, "Disarm EC WDT timeout.\n"));

  if (IsBiosBackupValid () == TRUE) {
    //
    // Skip BIOS Fit or ACM Policy check, since Simics environment don't support to read HardWare register
    // and that'll lead to ACM policy check failure
    //
    if (!IsSimicsEnvironment () && !IsHfpgaEnvironment () && IsFitOrPolicyError ()) {
      DEBUG ((DEBUG_INFO, "Trigger EC WDT timeout for BIOS recovery.\n"));
      ArmPlatformWdt (mWdtTimeOut);
      CpuDeadLoop ();
    }

#ifdef MDEPKG_NDEBUG
    //
    // Arm EC WDT with a max timeout value (255s).
    // It is used to monitor system boot in release build.
    //
    ArmMonitorBootHealthWdt (MAX_EC_WDT_TIMEOUT);
#endif
  }

  PeiServicesNotifyPpi (&mPlatformInitRecoveryNotifyList);
  PeiServicesNotifyPpi (&mMemDiscoveredNotifyList);
  PeiServicesNotifyPpi (&mBeforePostMemFvReportNotifyList);

  return EFI_SUCCESS;
}
