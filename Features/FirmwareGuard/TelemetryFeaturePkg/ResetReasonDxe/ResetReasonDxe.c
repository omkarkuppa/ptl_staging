/** @file
  Dxe Telemetry Publish Reset Reason to PHAT ACPI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include "ResetReasonDxe.h"
#include <TelemetryCommon.h>
#include <Register/PmcRegs.h>
#include <Register/EspiRegs.h>
#include <Library/IoLib.h>
#include <Library/PmcLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  Checks if reset was from cold boot

  @param[in]      GenPmConA   PMC CON A register
  @param[in,out]  Reason      Reset reason structure pointer
  @param[in,out]  Source      Reset source structure pointer

  @retval TRUE            Cold boot bit set
  @retval FALSE           Cold boot bit not set
**/
BOOLEAN
IsColdBoot (
  IN      UINT32        GenPmConA,
  IN OUT  RESET_REASON  *Reason,
  IN OUT  RESET_SOURCE  *Source
  )
{
  if (Reason == NULL || Source == NULL) {
    return FALSE;
  }
  if ((GenPmConA & B_ACPI_IO_PM1_STS_WAK) == B_ACPI_IO_PM1_STS_WAK) {
    Reason->Bits.ColdBoot = 0x1;
    Source->Bits.Unknown  = 0x1;
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from cold reset

  @param[in]      HprCause0   PMC HPR Cause 0 Register
  @param[in,out]  Reason      Reset reason structure pointer
  @param[in,out]  Source      Reset source structure pointer

  @retval TRUE            Cold reset bit set
  @retval FALSE           Cold reset bit not set
**/
BOOLEAN
IsColdReset (
  IN      UINT32        HprCause0,
  IN OUT  RESET_REASON  *Reason,
  IN OUT  RESET_SOURCE  *Source
)
{
  UINT32    ResetReg;

  if (Reason == NULL || Source == NULL) {
    return FALSE;
  }

  if (HprCause0 & B_PMC_PWRM_HPR_CAUSE0_COLD_RESET) {
    return TRUE;
  }

  if (HprCause0 & B_PMC_PWRM_HPR_CAUSE0_SYSRST_ES) {
    ResetReg = IoRead8 ((UINTN) R_PCH_IO_RST_CNT);
    if (ResetReg == B_PMC_PWRM_HPR_CAUSE0_CF9CR) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Checks if reset was from warm reset

  @param[in]      HprCause0   PMC HPR Cause 0 Register
  @param[in,out]  Reason      Reset reason structure pointer
  @param[in,out]  Source      Reset source structure pointer

  @retval TRUE            Warm reset bit set
  @retval FALSE           Warm reset bit not set
**/
BOOLEAN
IsWarmReset (
  IN      UINT32        HprCause0,
  IN OUT  RESET_REASON  *Reason,
  IN OUT  RESET_SOURCE  *Source
  )
{
  UINT32  ResetReg;

  if (Reason == NULL || Source == NULL) {
    return FALSE;
  }

  if (HprCause0 & B_PMC_PWRM_HPR_CAUSE0_SYSRST_ES) {
    ResetReg = IoRead8 ((UINTN) R_PCH_IO_RST_CNT);
    if (ResetReg == B_PMC_PWRM_HPR_CAUSE0_CF9WR) {
      return TRUE;
    }
  }

  if (HprCause0 & B_PMC_PWRM_HPR_CAUSE0_WARM_RESET) {
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from system update

  @param[in,out] Reason   Reset reason structure pointer
  @param[in,out] Source   Reset source structure pointer

  @retval TRUE            Boot flash update found
  @retval FALSE           Not a boot flash update
**/
BOOLEAN
IsUpdate (
  IN OUT  RESET_REASON  *Reason,
  IN OUT  RESET_SOURCE  *Source
  )
{
  if (Reason == NULL || Source == NULL) {
    return FALSE;
  }

  if (GetBootModeHob () == BOOT_ON_FLASH_UPDATE) {
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from Fault

  @param[in]      GblCause0   PMC GBL reset cause 0 Register
  @param[in]      GblCause1   PMC GBL reset cause 1 Register
  @param[in,out]  Reason      Reset reason structure pointer
  @param[in,out]  Source      Reset source structure pointer

  @retval TRUE      Fault reset bit set
  @retval FALSE     Fault reset bit not set
**/
BOOLEAN
IsFaultReset (
  IN      UINT32        GblCause0,
  IN      UINT32        GblCause1,
  IN OUT  RESET_REASON  *Reason,
  IN OUT  RESET_SOURCE  *Source
  )
{
  if (Reason == NULL || Source == NULL) {
    return FALSE;
  }

  if ((GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_FAULT_RESET) ||
      (GblCause1 & R_PMC_PWRM_GBLRST_CAUSE1_FAULT_RESET)) {
    Reason->Bits.Fault      = 0x1;
    if (GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_HW_SOURCE) {
      Source->Bits.Hardware = 0x1;
    } else if (GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_FW_SOURCE) {
      Source->Bits.Firmware = 0x1;
    } else {
      Source->Bits.Unknown  = 0x1;
    }
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was unexpected

  @param[in]      GblCause0   PMC GBL reset cause 0 Register
  @param[in,out]  Reason      Reset reason structure pointer
  @param[in,out]  Source      Reset source structure pointer

  @retval TRUE      unexpected reset bit set
  @retval FALSE     unexpected reset bit not set
**/
BOOLEAN
IsUnexpectedReset (
  IN      UINT32        GblCause0,
  IN OUT  RESET_REASON  *Reason,
  IN OUT  RESET_SOURCE  *Source
  )
{
  if (Reason == NULL || Source == NULL) {
    return FALSE;
  }

  if (GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_UNEXPECTED_RESET) {
    Reason->Bits.UnexpectedReset = 0x1;
    Source->Bits.Unknown         = 0x1;
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from timeout

  @param[in]      GblCause0   PMC GBL reset cause 0 Register
  @param[in,out]  Reason      Reset reason structure pointer
  @param[in,out]  Source      Reset source structure pointer

  @retval TRUE      Timeout reset bit set
  @retval FALSE     Timeout reset bit not set
**/
BOOLEAN
IsTimeoutReset (
  IN UINT32             GblCause0,
  IN OUT  RESET_REASON  *Reason,
  IN OUT  RESET_SOURCE  *Source
  )
{
  if (Reason == NULL || Source == NULL) {
    return FALSE;
  }

  if (GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_TIMEOUT_RESET) {
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from thermal reason

  @param[in]      GblCause0   PMC GBL reset cause 0 Register
  @param[in]      GblCause1   PMC GBL reset cause 0 Register
  @param[in,out]  Reason      Reset reason structure pointer
  @param[in,out]  Source      Reset source structure pointer

  @retval TRUE      Thermal reset bit set
  @retval FALSE     Thermal reset bit not set
**/
BOOLEAN
IsThermalReset (
  IN      UINT32        GblCause0,
  IN      UINT32        GblCause1,
  IN OUT  RESET_REASON  *Reason,
  IN OUT  RESET_SOURCE  *Source
  )
{
  if (Reason == NULL || Source == NULL) {
    return FALSE;
  }

  if ((GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_THERMAL_RESET) ||
      (GblCause1 & B_PMC_PWRM_GBLRST_CAUSE1_THERMAL_RESET)) {
    Reason->Bits.Thermal  = 0x1;
    Source->Bits.Hardware = 0x1;
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from Power Loss occurrence

  @param[in]      GenPmConA   PMC CON A register
  @param[in,out]  Reason      Reset reason structure pointer
  @param[in,out]  Source      Reset source structure pointer

  @retval TRUE      Power loss reset bit set
  @retval FALSE     Power loss reset bit not set
**/
BOOLEAN
IsPowerLossReset (
  IN      UINT32        GenPmConA,
  IN OUT  RESET_REASON  *Reason,
  IN OUT  RESET_SOURCE  *Source
  )
{
  if (Reason == NULL || Source == NULL) {
    return FALSE;
  }

  if (GenPmConA & B_PMC_PWRM_GEN_PMCON_A_PF) {
    Reason->Bits.PowerLoss = 0x1;
    Source->Bits.Hardware  = 0x1;
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from power button press

  @param[in]      GblCause0   PMC GBL reset cause 0 Register
  @param[in,out]  Reason      Reset reason structure pointer
  @param[in,out]  Source      Reset source structure pointer

  @retval TRUE      Power button reset bit set
  @retval FALSE     Power button reset bit not set
**/
BOOLEAN
IsPowerButtonReset (
  IN      UINT32        GblCause0,
  IN OUT  RESET_REASON  *Reason,
  IN OUT  RESET_SOURCE  *Source
  )
{
  if (Reason == NULL || Source == NULL) {
    return FALSE;
  }

  if (GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_PWR_BTN_RESET) {
    Reason->Bits.PowerButton = 0x1;
    Source->Bits.Unknown     = 0x1;
    return EFI_SUCCESS;
  }
  return FALSE;
}

/**
  Checks the different possible reasons for the platform reset and
  sets the reset reason and source for the table.

  @param[in,out] ResetHealthTable   Pointer to the reset structure table

  @retval EFI_SUCCESS               Able to get and parse the reset reasons
  @retval Other                     Something occurred causing a failure
**/
EFI_STATUS
GetSourceAndReason (
  IN OUT RESET_REASON_HEALTH_STRUCTURE  *ResetHealthTable
  )
{
  UINT32          GenPmConA;
  UINT32          HprCause0;
  UINT32          GblCause0;
  UINT32          GblCause1;

  if (ResetHealthTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get the PMC reset registers
  GenPmConA = 0;
  HprCause0 = 0;
  GblCause0 = 0;
  GblCause1 = 0;
  GetPmcResetRegisters (GenPmConA, HprCause0, GblCause0, GblCause1);

  //
  // Check for Cold Boot
  //
  if (IsColdBoot (GenPmConA, &ResetHealthTable->ResetReason, &ResetHealthTable->ResetSource)) {
    return EFI_SUCCESS;
  }

  //
  // Cold Reset Check
  //
  if (IsColdReset (HprCause0, &ResetHealthTable->ResetReason, &ResetHealthTable->ResetSource)) {
    return EFI_SUCCESS;
  }

  //
  // Check for Warm Reset
  //
  if (IsWarmReset (HprCause0, &ResetHealthTable->ResetReason, &ResetHealthTable->ResetSource)) {
    return EFI_SUCCESS;
  }

  //
  // Check if Update was the reason
  //
  if (IsUpdate(&ResetHealthTable->ResetReason, &ResetHealthTable->ResetSource)) {
    return EFI_SUCCESS;
  }

  //
  // Check for Unexpected Reset Reason
  //
  if (IsUnexpectedReset (GblCause0, &ResetHealthTable->ResetReason, &ResetHealthTable->ResetSource)) {
    return EFI_SUCCESS;
  }

  //
  // Check for Fault Reset Reason
  //
  if (IsFaultReset (GblCause0, GblCause1, &ResetHealthTable->ResetReason, &ResetHealthTable->ResetSource)) {
    return EFI_SUCCESS;
  }

  //
  // Check for Timeout Reset Reason
  //
  if (IsTimeoutReset (GblCause0, &ResetHealthTable->ResetReason, &ResetHealthTable->ResetSource)) {
    return EFI_SUCCESS;
  }

  //
  // Check for Thermal Reset Reason
  //
  if (IsThermalReset (GblCause0, GblCause1, &ResetHealthTable->ResetReason, &ResetHealthTable->ResetSource)) {
    return EFI_SUCCESS;
  }

  //
  // Power Loss Reset Reason
  //
  if (IsPowerLossReset (GenPmConA, &ResetHealthTable->ResetReason, &ResetHealthTable->ResetSource)) {
    return EFI_SUCCESS;
  }

  //
  // Power Button Pressed Reset Reason
  //
  if (IsPowerButtonReset (GblCause0, &ResetHealthTable->ResetReason, &ResetHealthTable->ResetSource)) {
    return EFI_SUCCESS;
  }

  //
  // If no reset reason found, marking as unknown
  //
  ResetHealthTable->ResetReason.Bits.Unknown = 0x1;
  ResetHealthTable->ResetSource.Bits.Unknown = 0x1;
  return EFI_NOT_FOUND;
}

/**
  Callback to get and install telemetry platform reset reason PHAT.

  @retval EFI_SUCCESS   Able to get and set reset reason to PHAT
  @retval Other         Something occurred causing a failure
**/
EFI_STATUS
EFIAPI
ResetReasonCallback (
  VOID
  )
{
  EFI_STATUS                      Status;
  RESET_REASON_HEALTH_STRUCTURE   *ResetReasonHealthTable;

  // Initiate reset reason table
  ResetReasonHealthTable = (RESET_REASON_HEALTH_STRUCTURE *) AllocateZeroPool (sizeof (RESET_REASON_HEALTH_STRUCTURE));
  if (ResetReasonHealthTable == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Get and set reset reason and source
  Status = GetSourceAndReason (ResetReasonHealthTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "[%a] Get reset reason and source exit with status: %r", __FUNCTION__, Status));
    if (Status != EFI_NOT_FOUND) {
      // Reason may not be found but will still publish the data as unknown
      FreePool (ResetReasonHealthTable);
    }
  }

  return Status;
}

/**
  Callback handler for Telemetry Reset Reason

  @param[in] Event                  A pointer to the Event that triggered the callback
  @param[in] Context                A pointer to private data registered with the callback function
**/
VOID
EFIAPI
ResetReasonCallbackHandler(
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS  Status;

  Status = ResetReasonCallback ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "[%a] Failed to get reset reason or source with status: %r", __FUNCTION__, Status));
  }
  gBS->CloseEvent (Event);
}

/**
  Entry point to telemetry platform reset reason PHAT.

  @param[in] ImageHandle    The firmware allocated handle for the UEFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS   Able to get and set reset reason to PHAT
  @retval Other         Something occurred causing a failure
**/
EFI_STATUS
EFIAPI
ResetReasonEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS      Status;
  EFI_EVENT       ReadyToBootEvent;

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  ResetReasonCallbackHandler,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "[%a] Failed to load vendor data with status %r", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}