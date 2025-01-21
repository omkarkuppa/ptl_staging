/** @file
  The PEI Library Implements OcWdt Support.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/WdtCommonLib.h>
#include <Library/PmcLib.h>
#include <Library/IoLib.h>
#include <Ppi/Wdt.h>
#include <Register/PmcRegs.h>

static WDT_PPI mWdtPpi = {
  WdtReloadAndStart,
  WdtCheckStatus,
  WdtDisable,
  WdtAllowKnownReset,
  IsWdtRequired,
  IsWdtEnabled
};

static EFI_PEI_PPI_DESCRIPTOR  mInstallWdtPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gWdtPpiGuid,
  &mWdtPpi
};

/**
  Reads PCH registers to check if platform wakes from S3/S4

  @retval TRUE                    if platfrom wakes from S3/S4
  @retval FALSE                   otherwise
**/
BOOLEAN
IsWakeFromS3S4 (
  VOID
  )
{
  PMC_SLEEP_STATE  SleepType;

  SleepType = PmcGetSleepTypeAfterWake ();
  if ((SleepType == PmcS3SleepState) || (SleepType == PmcS4SleepState)) {
    return TRUE;
  }

  return FALSE;

}

/**
  Check for unexpected reset.
  If there was an unexpected reset, enforces WDT expiration.
**/
VOID
OcWdtResetCheck (
  VOID
  )
{
  UINT32      Readback;

  Readback = IoRead32 (WdtGetAddress ());
  DEBUG ((DEBUG_INFO, "(WDT) OcWdtResetCheck()\n"));

  ///
  /// If there was a WDT expiration, set Failure Status and clear timeout status bits
  /// Timeout status bits are cleared by writing '1'
  ///
  if (Readback & (B_ACPI_IO_OC_WDT_CTL_OC_WDT_ICCSURV_STS | B_ACPI_IO_OC_WDT_CTL_OC_WDT_NO_ICCSURV_STS)) {
    DEBUG ((DEBUG_ERROR, "(WDT) Expiration detected. Readback %d\n", Readback));
    Readback |= B_ACPI_IO_OC_WDT_CTL_FAILURE_STS;
    Readback |= (B_ACPI_IO_OC_WDT_CTL_OC_WDT_ICCSURV_STS | B_ACPI_IO_OC_WDT_CTL_OC_WDT_NO_ICCSURV_STS);
    Readback &= ~(B_ACPI_IO_OC_WDT_CTL_UNXP_RESET_STS);
  } else {
    ///
    /// If there was unexpected reset but no WDT expiration and no resume from S3/S4,
    /// clear unexpected reset status and enforce expiration. This is to inform Firmware
    /// which has no access to unexpected reset status bit, that something went wrong.
    ///
    if ((Readback & B_ACPI_IO_OC_WDT_CTL_UNXP_RESET_STS) && !IsWakeFromS3S4()) {
#ifndef MDEPKG_NDEBUG
      DEBUG ((DEBUG_WARN, "(WDT) Unexpected reset detected and ignored.\n"));
      Readback &= ~(B_ACPI_IO_OC_WDT_CTL_FAILURE_STS | B_ACPI_IO_OC_WDT_CTL_UNXP_RESET_STS);
      Readback |= (B_ACPI_IO_OC_WDT_CTL_OC_WDT_ICCSURV_STS | B_ACPI_IO_OC_WDT_CTL_OC_WDT_NO_ICCSURV_STS);
#else
      DEBUG ((DEBUG_ERROR, "(WDT) Unexpected reset detected. Enforcing Wdt expiration.\n"));
      WdtReloadAndStart (1);
      ///
      /// wait for reboot caused by WDT expiration
      ///
      CpuDeadLoop ();
#endif
    } else {
      ///
      /// No WDT expiration and no unexpected reset - clear Failure status
      ///
      DEBUG ((DEBUG_INFO, "(WDT) Status OK.\n"));
      Readback &= ~(B_ACPI_IO_OC_WDT_CTL_FAILURE_STS);
      Readback |= (B_ACPI_IO_OC_WDT_CTL_OC_WDT_ICCSURV_STS | B_ACPI_IO_OC_WDT_CTL_OC_WDT_NO_ICCSURV_STS);
    }
  }

  IoWrite32 (WdtGetAddress (), Readback);

  return;
}

/**
  This function install WDT PPI

  @retval EFI_STATUS  Results of the installation of the WDT PPI
**/
EFI_STATUS
EFIAPI
OcWdtInit (
  VOID
  )
{
  EFI_STATUS  Status;
  WDT_PPI     *WdtPei;

  Status = PeiServicesLocatePpi (
             &gWdtPpiGuid,
             0,
             NULL,
             (VOID **) &WdtPei
             );
  if (EFI_ERROR (Status)) {
    Status = PeiServicesInstallPpi (&mInstallWdtPpi);
    ASSERT_EFI_ERROR (Status);
  } else {
    DEBUG ((DEBUG_INFO, "OCWDT PPI is already installed\n"));
  }

  return Status;
}

