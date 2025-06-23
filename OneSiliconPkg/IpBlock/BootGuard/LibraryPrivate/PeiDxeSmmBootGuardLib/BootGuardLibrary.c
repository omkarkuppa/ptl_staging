/** @file
  BootGuardLibrary implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/SiScheduleResetLib.h>
#include <Library/PmcLib.h>
#include <Library/BootGuardLib.h>
#include <Txt.h>
#include <Library/BaseMemoryLib.h>

///
/// Below MSR definition is copied from <Register/MsrRegs.h>
///   Because Server side <Register/MsrRegs.h> has not been created and this
///   Library is required for both client and server.
///   Then definition will be deleted once server side <Register/MsrRegs.h> is create
///

#define MSR_ANC_INITIAL_BOOT_BLOCK_COMPLETE (0x00000139)

typedef union {
///
/// Individual bit fields
///
  struct {
    UINT64 Done : 1;

                            /* Bits[0:0], Access Type=NA, default=0x00000000*/

                            /*
                               Bit indicates that ACM has successfully launched.
                               Stop deadman timer counter
                            */

    UINT64 Rsvd1 : 63; /**< Undefined - auto filled rsvd_[63:01] */

                            /* Bits[63:1], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Uint32;
  UINT64 Uint64;
} MSR_ANC_INITIAL_BOOT_BLOCK_COMPLETE_REGISTER;  /// MSR definition

/**
  Determine if Boot Guard is supported

  @retval TRUE  - Processor is Boot Guard capable.
  @retval FALSE - Processor is not Boot Guard capable.

**/
BOOLEAN
EFIAPI
IsBootGuardSupported (
  VOID
  )
{
  UINT64          BootGuardCapability;

  BootGuardCapability = AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & B_BOOT_GUARD_SACM_INFO_CAPABILITY;

  if (BootGuardCapability != 0) {
    DEBUG ((DEBUG_INFO, "Processor supports Boot Guard.\n"));
    return TRUE;
  } else {
    DEBUG ((DEBUG_INFO, "Processor does not support Boot Guard.\n"));
    return FALSE;
  }
}

/**
   Handle Tpm2Startup (State) failures reported by ACM.

   The function checks for the TpmStartupFailureOnS3 flag if TRUE it will
   request the system to perform a cold reset.

   @param[in] *BootGuardInfo - Pointer to BootGuardInfo.
**/
VOID
EFIAPI
HandleTpmStartupFailureOnS3 (
  IN BOOT_GUARD_INFO *BootGuardInfo
  )
{
  if (BootGuardInfo->TpmStartupFailureOnS3 == TRUE) {
    DEBUG ((DEBUG_ERROR, "ERROR: Tpm2Startup (State) failure during S3, requesting cold reset... \n"));
    SiScheduleResetSetType (EfiResetCold, NULL);
  }
}

/**
  Report platform specific Boot Guard information.

  @param[out] *BootGuardInfo - Pointer to BootGuardInfo.
**/
VOID
EFIAPI
GetBootGuardInfo (
  OUT BOOT_GUARD_INFO *BootGuardInfo
  )
{
  UINT32             MsrValue;
  UINT32             BootGuardAcmStatus;
  UINT64             BootGuardBootStatus;
  UINT64             BootGuardAcmPolicyStatus;
  PMC_SLEEP_STATE    SleepType;

  ZeroMem (BootGuardInfo, sizeof (BOOT_GUARD_INFO));

  BootGuardInfo->BootGuardCapability = FALSE;

  ///
  /// Check if System Supports Boot Guard
  ///
  if (IsBootGuardSupported ()) {
    BootGuardInfo->BootGuardCapability = TRUE;

    ///
    /// Initialize default values
    ///
    BootGuardInfo->BypassTpmInit = FALSE;
    BootGuardInfo->MeasuredBoot = FALSE;
    // This register is updated by S-ACM
    BootGuardAcmStatus  = *(UINT32 *) (UINTN) (TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_ACM_STATUS);
    DEBUG ((DEBUG_INFO, "Boot Guard ACM Status = 0x%x\n", BootGuardAcmStatus));

    BootGuardBootStatus  = *(UINT64 *) (UINTN) (TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_BOOTSTATUS);
    DEBUG ((DEBUG_INFO, "Boot Guard Boot Status = 0x%llx\n", BootGuardBootStatus));

    BootGuardAcmPolicyStatus = *(UINT64 *) (UINTN) (TXT_PUBLIC_BASE + R_CPU_ACM_POLICY_STATUS);
    DEBUG ((DEBUG_INFO, "Boot Guard ACM Policy Status = 0x%llx\n", BootGuardAcmPolicyStatus));
    ///
    /// Read Boot Guard S-ACM Info MSR
    ///
    MsrValue = (UINT32) AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO);
    DEBUG ((DEBUG_INFO, "MSR_BOOT_GUARD_SACM_INFO MSR = %x\n", MsrValue));

    ///
    /// Check Bit 46 in BootGuardBootStatus to handle Tpm2Startup(Restore) errors.
    ///
    if ((BootGuardBootStatus & B_CPU_BOOT_GUARD_BOOTSTATUS_S3_TPM_STARTUP_FAILED) != 0) {
      BootGuardInfo->TpmStartupFailureOnS3 = TRUE;
      DEBUG ((DEBUG_INFO, "ACM Tpm2Startup (State) failure detected during S3 flow.\n"));
    }

    SleepType = PmcGetSleepTypeAfterWake ();

    ///
    /// Check for Sx Resume path to bypass Tpm event log
    ///
    if (SleepType == PmcS3SleepState) {
      DEBUG ((DEBUG_INFO, "Sx Resume Type Identified - TPM Event Log not required for ACM Measurements\n"));
      BootGuardInfo->ByPassTpmEventLog = TRUE;
    }

    ///
    /// Check Bit 0 of BOOT_GUARD_SACM_INFO MSR if system is in Boot Guard boot mode
    ///

    if ((MsrValue & B_BOOT_GUARD_SACM_INFO_NEM_ENABLED) == 0) {
      DEBUG ((DEBUG_INFO, "NEM is not initiated by Boot Guard ACM\n"));
    }

    if ((MsrValue & B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT) != 0) {
      BootGuardInfo->MeasuredBoot = TRUE;
      ///
      /// If measured bit is set, BIOS needs to bypass startup command
      ///
      if ((MsrValue & B_BOOT_GUARD_SACM_INFO_TPM_SUCCESS) != 0) {
        BootGuardInfo->BypassTpmInit = TRUE;
      }
      ///
      /// Read present TPM type
      ///
      BootGuardInfo->TpmType = (TPM_TYPE) ((MsrValue & V_TPM_PRESENT_MASK) >> 1 );
      DEBUG ((DEBUG_INFO, "TPM Type is %x\n", BootGuardInfo->TpmType));
    }

    ///
    /// Disconnect all TPMs when:
    ///   BtG ACM status reports error, Measured boot is enabled and TPM was not successfully initialized by BtG.
    ///
    if (((BootGuardAcmStatus & B_BOOT_GUARD_ACM_ERRORCODE_MASK) != 0) &&
         ((MsrValue & B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT) == B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT) &&
         ((MsrValue & B_BOOT_GUARD_SACM_INFO_TPM_SUCCESS) == 0)) {
      DEBUG ((DEBUG_INFO, "All TPM's on Platform are Disconnected\n"));
      BootGuardInfo->DisconnectAllTpms = TRUE;
    }
  }

  DEBUG ((DEBUG_INFO, "Boot Guard Support status: %x\n", BootGuardInfo->BootGuardCapability));
  return;
}

/**
  Stop PBE timer if system is in Boot Guard boot

  @retval EFI_SUCCESS        - Stop PBE timer
  @retval EFI_UNSUPPORTED    - Not in Boot Guard boot mode.
**/
EFI_STATUS
EFIAPI
StopPbeTimer (
  VOID
  )
{
  UINT64                                        BootGuardBootStatus;
  UINT64                                        BootGuardOperationMode;
  MSR_ANC_INITIAL_BOOT_BLOCK_COMPLETE_REGISTER  MsrAncInitialBootBlockComplete;

  if (IsBootGuardSupported ()) {
    BootGuardBootStatus = (*(UINT64 *) (UINTN) (TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_BOOTSTATUS) & (BIT63|BIT62));
    BootGuardOperationMode = AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & (B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT | B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT);

    //
    // Stop PBET if Verified/Measured/NEM bit is set in MSR 0x13A or
    // Boot Guard fails to launch or fails to execute successfully for avoiding brick platform
    //

    if (BootGuardBootStatus == V_CPU_BOOT_GUARD_LOAD_ACM_SUCCESS) {
      if (BootGuardOperationMode == 0) {
        DEBUG ((DEBUG_INFO, "Platform is in Legacy boot mode.\n"));
        return EFI_UNSUPPORTED;
      } else {
        DEBUG ((DEBUG_INFO, "Platform in Boot Guard Boot mode.\n"));
      }
    } else {
      DEBUG ((DEBUG_ERROR, "Boot Guard ACM launch failed or ACM execution failed.\n"));
    }

    DEBUG ((DEBUG_INFO, "Disable PBET\n"));
    MsrAncInitialBootBlockComplete.Uint64    = 0;
    MsrAncInitialBootBlockComplete.Bits.Done = 1;
    AsmWriteMsr64 (MSR_ANC_INITIAL_BOOT_BLOCK_COMPLETE, MsrAncInitialBootBlockComplete.Uint64);
  } else {
    DEBUG ((DEBUG_WARN, "Boot Guard is not supported.\n"));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}
