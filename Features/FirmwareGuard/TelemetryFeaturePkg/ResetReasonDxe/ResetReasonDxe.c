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
#include <Register/PmcRegs.h>
#include <Register/EspiRegs.h>
#include <Library/IoLib.h>
#include <Library/PmcLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PhatAcpiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  Construct and attach Reset reason health data record to PHAT.

  @param[in]  ResetReasonRecord      A pointer to Reset Reason health record.
  @param[in]  ResetReasonRecordSize  Size of ResetReasonRecord buffer
  @param[in]  AmHealthy              AmHeathy field of reset reason healthy data

  @retval EFI_SUCCESS       Successfully posted data to existing or new PHAT ACPI table
  @retval other             Failed to publish data

**/
EFI_STATUS
PublishResetReasonPhat (
  IN EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_STRUCTURE  *ResetReasonRecord,
  IN UINT16                                                  ResetReasonRecordSize,
  IN UINT8                                                   AmHealthy
  )
{
  EFI_STATUS                                               Status;
  EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE  *HealthDataRecord;
  UINT16                                                   HealthDataRecordSize;
  CHAR16                                                   DevicePathText[] = RESET_REASON_DEVICE_PATH_STR;
  UINT16                                                   DevicePathTextSize;
  UINT16                                                   DeviceSpecificDataOffset;

  if ((ResetReasonRecord == NULL) || \
      (ResetReasonRecordSize < sizeof (EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_STRUCTURE))) {
    DEBUG ((DEBUG_ERROR, "Reset Reason Record is expected\n"));
    return EFI_INVALID_PARAMETER;
  }

  HealthDataRecord         = NULL;
  DevicePathTextSize       = RESET_REASON_DEVICE_PATH_STR_SIZE;

  DeviceSpecificDataOffset = \
      sizeof (EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE) \
    + DevicePathTextSize;

  HealthDataRecordSize     = \
      sizeof (EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE) \
    + DevicePathTextSize \
    + ResetReasonRecordSize;

  HealthDataRecord = AllocateZeroPool (HealthDataRecordSize);
  if (HealthDataRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Configure the Reset Reason Health Data header
  //
  HealthDataRecord->PlatformRecordType       = EFI_ACPI_6_5_PHAT_RECORD_TYPE_FIRMWARE_HEALTH_DATA_RECORD;
  HealthDataRecord->RecordLength             = HealthDataRecordSize;
  HealthDataRecord->Revision                 = EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_REVISION;
  HealthDataRecord->AmHealthy                = AmHealthy;
  HealthDataRecord->DeviceSignature          = (GUID)EFI_ACPI_6_5_PHAT_RESET_REASON_HEADER_GUID;
  HealthDataRecord->DeviceSpecificDataOffset = DeviceSpecificDataOffset;

  //
  // Update DevicePath field
  //
  CopyMem ((UINT8 *)HealthDataRecord + sizeof (EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE), \
           DevicePathText, \
           DevicePathTextSize
           );
  //
  // Attach Device-specificData (reset reason health record)
  //
  CopyMem ((UINT8 *)HealthDataRecord + DeviceSpecificDataOffset, \
           ResetReasonRecord, \
           ResetReasonRecordSize
           );

  //
  // Attach reset reason health data record to PHAT
  //
  Status = InstallPhatTable (HealthDataRecord, HealthDataRecordSize);
  DEBUG ((DEBUG_INFO, "InstallPhatTable for reset reason: %r\n", Status));

  FreePool (HealthDataRecord);

  return Status;
}

/**
  Checks if reset was from cold boot

  @param[in]      GenPmConA   PMC CON A register
  @param[in,out]  Reason      Reset reason structure
  @param[in,out]  Source      Reset source structure

  @retval TRUE            Cold boot bit set
  @retval FALSE           Cold boot bit not set
**/
BOOLEAN
IsColdBoot (
  IN      UINT32    GenPmConA,
  IN OUT  UINT8     Reason,
  IN OUT  UINT8     Source
  )
{
  if ((GenPmConA & B_ACPI_IO_PM1_STS_WAK) == B_ACPI_IO_PM1_STS_WAK) {
    Reason |= EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_COLD_BOOT;
    Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from cold reset

  @param[in]      HprCause0   PMC HPR Cause 0 Register
  @param[in,out]  Reason      Reset reason structure
  @param[in,out]  Source      Reset source structure

  @retval TRUE            Cold reset bit set
  @retval FALSE           Cold reset bit not set
**/
BOOLEAN
IsColdReset (
  IN      UINT32    HprCause0,
  IN OUT  UINT8     Reason,
  IN OUT  UINT8     Source
)
{
  UINT32    ResetReg;
  BOOLEAN   TypeFound;

  TypeFound = FALSE;

  if (HprCause0 & B_PMC_PWRM_HPR_CAUSE0_COLD_RESET) {
    Reason |= EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_COLD_RESET;
    Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
    TypeFound = TRUE;
  }

  if (HprCause0 & B_PMC_PWRM_HPR_CAUSE0_SYSRST_ES) {
    ResetReg = IoRead8 ((UINTN) R_PCH_IO_RST_CNT);
    if (ResetReg == V_PMC_PWRM_HPR_CAUSE0_CF9CR) {
      Reason |= EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_COLD_RESET;
      Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
      TypeFound = TRUE;
    }
  }
  return TypeFound;
}

/**
  Checks if reset was from warm reset

  @param[in]      HprCause0   PMC HPR Cause 0 Register
  @param[in,out]  Reason      Reset reason structure
  @param[in,out]  Source      Reset source structure

  @retval TRUE            Warm reset bit set
  @retval FALSE           Warm reset bit not set
**/
BOOLEAN
IsWarmReset (
  IN      UINT32    HprCause0,
  IN OUT  UINT8     Reason,
  IN OUT  UINT8     Source
  )
{
  UINT32    ResetReg;
  BOOLEAN   TypeFound;

  TypeFound = FALSE;

  if (HprCause0 & B_PMC_PWRM_HPR_CAUSE0_SYSRST_ES) {
    ResetReg = IoRead8 ((UINTN) R_PCH_IO_RST_CNT);
    if (ResetReg == V_PMC_PWRM_HPR_CAUSE0_CF9WR) {
      Reason |= EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_WARM_RESET;
      Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
      TypeFound = TRUE;
    }
  }

  if (HprCause0 & B_PMC_PWRM_HPR_CAUSE0_WARM_RESET) {
    Reason |= EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_WARM_RESET;
    Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
    TypeFound = TRUE;
  }
  return TypeFound;
}

/**
  Checks if reset was from system update

  @param[in,out] Reason   Reset reason structure
  @param[in,out] Source   Reset source structure

  @retval TRUE            Boot flash update found
  @retval FALSE           Not a boot flash update
**/
BOOLEAN
IsUpdate (
  IN OUT  UINT8   Reason,
  IN OUT  UINT8   Source
  )
{
  if (GetBootModeHob () == BOOT_ON_FLASH_UPDATE) {
    Reason |= EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_UPDATE;
    Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_SOFTWARE;
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from Fault

  @param[in]      GblCause0   PMC GBL reset cause 0 Register
  @param[in]      GblCause1   PMC GBL reset cause 1 Register
  @param[in,out]  Reason      Reset reason structure
  @param[in,out]  Source      Reset source structure

  @retval TRUE      Fault reset bit set
  @retval FALSE     Fault reset bit not set
**/
BOOLEAN
IsFaultReset (
  IN      UINT32    GblCause0,
  IN      UINT32    GblCause1,
  IN OUT  UINT8     Reason,
  IN OUT  UINT8     Source
  )
{
  if ((GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_FAULT_RESET) ||
      (GblCause1 & B_PMC_PWRM_GBLRST_CAUSE1_FAULT_RESET)) {
    Reason |= EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_FAULT;
    if (GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_HW_SOURCE) {
      Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_HARDWARE;
    } else if (GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_FW_SOURCE) {
      Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_FIRMWARE;
    } else {
      Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
    }
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was unexpected

  @param[in]      GblCause0   PMC GBL reset cause 0 Register
  @param[in,out]  Reason      Reset reason structure
  @param[in,out]  Source      Reset source structure

  @retval TRUE      unexpected reset bit set
  @retval FALSE     unexpected reset bit not set
**/
BOOLEAN
IsUnexpectedReset (
  IN      UINT32      GblCause0,
  IN OUT  UINT8       Reason,
  IN OUT  UINT8       Source
  )
{
  if (GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_UNEXPECTED_RESET) {
    Reason |= EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_UNEXPECTED_RESET;
    Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from timeout

  @param[in]      GblCause0   PMC GBL reset cause 0 Register
  @param[in,out]  Reason      Reset reason structure
  @param[in,out]  Source      Reset source structure

  @retval TRUE      Timeout reset bit set
  @retval FALSE     Timeout reset bit not set
**/
BOOLEAN
IsTimeoutReset (
  IN      UINT32    GblCause0,
  IN OUT  UINT8     Reason,
  IN OUT  UINT8     Source
  )
{
  if (GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_TIMEOUT_RESET) {
    Reason |= EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_TIMEOUT;
    Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from thermal reason

  @param[in]      GblCause0   PMC GBL reset cause 0 Register
  @param[in]      GblCause1   PMC GBL reset cause 0 Register
  @param[in,out]  Reason      Reset reason structure
  @param[in,out]  Source      Reset source structure

  @retval TRUE      Thermal reset bit set
  @retval FALSE     Thermal reset bit not set
**/
BOOLEAN
IsThermalReset (
  IN      UINT32      GblCause0,
  IN      UINT32      GblCause1,
  IN OUT  UINT8       Reason,
  IN OUT  UINT8       Source
  )
{
  if ((GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_THERMAL_RESET) ||
      (GblCause1 & B_PMC_PWRM_GBLRST_CAUSE1_THERMAL_RESET)) {
    Reason |= EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_THERMAL;
    Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_HARDWARE;
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from Power Loss occurrence

  @param[in]      GenPmConA   PMC CON A register
  @param[in,out]  Reason      Reset reason structure
  @param[in,out]  Source      Reset source structure

  @retval TRUE      Power loss reset bit set
  @retval FALSE     Power loss reset bit not set
**/
BOOLEAN
IsPowerLossReset (
  IN      UINT32      GenPmConA,
  IN OUT  UINT8       Reason,
  IN OUT  UINT8       Source
  )
{
  if (GenPmConA & B_PMC_PWRM_GEN_PMCON_A_PF) {
    Reason |= EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_POWER_LOSS;
    Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_HARDWARE;
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if reset was from power button press

  @param[in]      GblCause0   PMC GBL reset cause 0 Register
  @param[in,out]  Reason      Reset reason structure
  @param[in,out]  Source      Reset source structure

  @retval TRUE      Power button reset bit set
  @retval FALSE     Power button reset bit not set
**/
BOOLEAN
IsPowerButtonReset (
  IN      UINT32      GblCause0,
  IN OUT  UINT8       Reason,
  IN OUT  UINT8       Source
  )
{
  if (GblCause0 & B_PMC_PWRM_GBLRST_CAUSE0_PBO) {
    Reason |= EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_POWER_BUTTON;
    Source |= EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
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
  IN OUT EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_STRUCTURE  *ResetHealthTable
  )
{
  EFI_STATUS      Status;
  UINT32          GenPmConA;
  UINT32          HprCause0;
  UINT32          GblCause0;
  UINT32          GblCause1;

  if (ResetHealthTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_NOT_FOUND;

  ///
  /// Get the PMC reset registers
  ///
  GenPmConA = 0;
  HprCause0 = 0;
  GblCause0 = 0;
  GblCause1 = 0;
  GetPmcResetRegisters (GenPmConA, HprCause0, GblCause0, GblCause1);

  ///
  /// Check for Cold Boot
  ///
  if (IsColdBoot (GenPmConA, ResetHealthTable->Reason, ResetHealthTable->Source)) {
    return EFI_SUCCESS;
  }

  ///
  /// Cold Reset Check
  ///
  if (IsColdReset (HprCause0, ResetHealthTable->Reason, ResetHealthTable->Source)) {
    return EFI_SUCCESS;
  }

  ///
  /// Check for Warm Reset
  ///
  if (IsWarmReset (HprCause0, ResetHealthTable->Reason, ResetHealthTable->Source)) {
    return EFI_SUCCESS;
  }

  ///
  /// Check if Update was the reason
  ///
  if (IsUpdate(ResetHealthTable->Reason, ResetHealthTable->Source)) {
    return EFI_SUCCESS;
  }

  ///
  /// Check for Unexpected Reset Reason
  ///
  if (IsUnexpectedReset (GblCause0, ResetHealthTable->Reason, ResetHealthTable->Source)) {
    return EFI_SUCCESS;
  }

  ///
  /// Check for Fault Reset Reason
  ///
  if (IsFaultReset (GblCause0, GblCause1, ResetHealthTable->Reason, ResetHealthTable->Source)) {
    return EFI_SUCCESS;
  }

  ///
  /// Check for Timeout Reset Reason
  ///
  if (IsTimeoutReset (GblCause0, ResetHealthTable->Reason, ResetHealthTable->Source)) {
    return EFI_SUCCESS;
  }

  ///
  /// Check for Thermal Reset Reason
  ///
  if (IsThermalReset (GblCause0, GblCause1, ResetHealthTable->Reason, ResetHealthTable->Source)) {
    return EFI_SUCCESS;
  }

  ///
  /// Power Loss Reset Reason
  ///
  if (IsPowerLossReset (GenPmConA, ResetHealthTable->Reason, ResetHealthTable->Source)) {
    return EFI_SUCCESS;
  }

  ///
  /// Power Button Pressed Reset Reason
  ///
  if (IsPowerButtonReset (GblCause0, ResetHealthTable->Reason, ResetHealthTable->Source)) {
    return EFI_SUCCESS;
  }

  ///
  /// If no reset reason found, marking as unknown
  ///
  ResetHealthTable->Reason = EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_UNKNOWN;
  ResetHealthTable->Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
  return Status;
}

/**
  Construct reset reason record and health data table to PHAT

  @retval EFI_SUCCESS   Successfully attach reset reason health data record to PHAT
  @retval Other         Something occurred causing a failure

**/
EFI_STATUS
EFIAPI
ResetReasonMain (
  VOID
  )
{
  EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_STRUCTURE  *ResetReasonHealthRecord;
  UINT16                                                  ResetReasonHealthRecordSize;
  EFI_STATUS                                              Status;

  //
  // Initiate reset reason table
  // Todo: Record needs to consider Vendor specific reset reason which might be needed in the future.
  //
  ResetReasonHealthRecordSize = sizeof (EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_STRUCTURE);
  ResetReasonHealthRecord     = AllocateZeroPool (ResetReasonHealthRecordSize);
  if (ResetReasonHealthRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Get and set reset reason and source
  // Todo: This function needs revisiting
  //
  // Status = GetSourceAndReason (ResetReasonHealthRecord);

  //
  // Publish table to PHAT
  // Todo: input correct AmHealthy once GetSourceAndReason works.
  //
  Status = PublishResetReasonPhat (
             ResetReasonHealthRecord,
             ResetReasonHealthRecordSize,
             EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_UNKNOWN
             );

  FreePool (ResetReasonHealthRecord);
  return Status;
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

  DEBUG ((DEBUG_INFO, "[%a] Entry\n", __FUNCTION__));

  Status = ResetReasonMain ();
  DEBUG ((DEBUG_INFO, "[%a] Exit with status %r\n", __FUNCTION__, Status));
  return Status;
}
