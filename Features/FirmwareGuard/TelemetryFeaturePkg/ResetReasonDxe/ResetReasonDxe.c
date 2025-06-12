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
#include <Guid/ResetCauseHobGuid.h>

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
  DEBUG ((DEBUG_INFO, "[%a] InstallPhatTable for reset reason: %r\n", __FUNCTION__, Status));

  FreePool (HealthDataRecord);

  return Status;
}

/**
  Get reset cause relative PM registers.

  Considering the registers might be cleared during POST, expect a GUID HOB reported
  by platform which contains a snapshot of the registers in early boot phase.

  If the GUID HOB does not exist, get current registers then.

  @param[out] ResetCauseRegisters  Pointer to RESET_CAUSE_PM_REGISTERS structure which
                                   contains PM registers related to reset cause.

  @param[out] PmcSleepState        (Optional) Pointer to PMC_SLEEP_STATE

  @retval None.

**/
VOID
GetResetCauseRegisters (
  OUT RESET_CAUSE_PM_REGISTERS  *ResetCauseRegisters,
  OUT PMC_SLEEP_STATE           *PmcSleepState OPTIONAL
  )
{
  EFI_HOB_GUID_TYPE     *GuidHob;
  RESET_CAUSE_HOB_DATA  *ResetCauseHobData;

  if (ResetCauseRegisters == NULL) {
    ASSERT (FALSE);
    return;
  }

  GuidHob = GetFirstGuidHob (&gIntelResetCauseHobGuid);
  if (GuidHob != NULL) {
    ResetCauseHobData = (RESET_CAUSE_HOB_DATA *)GET_GUID_HOB_DATA (GuidHob);
    CopyMem (
      (VOID *)ResetCauseRegisters,
      (VOID *)&(ResetCauseHobData->ResetCauseRegs),
      sizeof (RESET_CAUSE_PM_REGISTERS));

    if (PmcSleepState != NULL) {
      *PmcSleepState = ResetCauseHobData->PmcSleepState;
    }

    return;
  }

  DEBUG ((DEBUG_WARN, "!!Reset cause HOB does not exist, the data might be inaccurate!!\n"));

  GetPmcResetRegisters (
    &(ResetCauseRegisters->GenPmconA),
    &(ResetCauseRegisters->HbrCause0),
    &(ResetCauseRegisters->GblrstCause0),
    &(ResetCauseRegisters->GblrstCause1)
    );

  if (PmcSleepState != NULL) {
    *PmcSleepState = PmcGetSleepTypeAfterWake ();
  }

  return;
}

/**
  Construct a buffer contains all vendor data to be attached to reset reason record.
  Each vendor data should start with the header
  EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_VENDOR_DATA_ENTRY

  @param[out] VendorDataBufferPtr   Pointer to reset reason vendor data.
                                    It's caller's responsibility to free the pool after using.

  @param[out] VendorDataTotalSize   Pointer to the total size of VendorDataBufferPtr buffer.

  @param[out] VendorDataEntryCount  Pointer to how many vendor entries in VendorDataBufferPtr buffer

  @retval EFI_SUCCESS               Successfully contructed the vendor data buffer
  @retval Other                     Something occurred causing a failure

**/
EFI_STATUS
ConstructResetReasonVendorData (
  OUT EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_VENDOR_DATA_ENTRY  **VendorDataBufferPtr,
  OUT UINT16                                                          *VendorDataTotalSize,
  OUT UINT16                                                          *VendorDataEntryCount
  )
{
  INTEL_PHAT_RESET_REASON_VENDOR_DATA IntelVendorDataEntry;
  UINT16                              IntelVendorDataEntrySize;

  *VendorDataBufferPtr = NULL;

  //
  // Prepare Intel vendor data entry
  //
  IntelVendorDataEntrySize = sizeof (INTEL_PHAT_RESET_REASON_VENDOR_DATA);
  ZeroMem (&IntelVendorDataEntry, IntelVendorDataEntrySize);


  IntelVendorDataEntry.Header.VendorDataID = (GUID) INTEL_PHAT_RESET_REASON_VENDOR_DATA_GUID;
  IntelVendorDataEntry.Header.Revision     = INTEL_PHAT_RESET_REASON_HEALTH_RECORD_VENDOR_DATA_REVISION_0_1;
  IntelVendorDataEntry.Header.Length       = IntelVendorDataEntrySize;

  //
  // Update Intel vendor data payload field
  //
  GetResetCauseRegisters (&(IntelVendorDataEntry.Payload), NULL);

  //
  // Prepare the whole vendor data output buffer
  //
  // Currently we only have one vendor data entry defined and reported.
  //
  *VendorDataTotalSize  = IntelVendorDataEntrySize;
  *VendorDataEntryCount = 1;
  *VendorDataBufferPtr  = AllocateCopyPool (IntelVendorDataEntrySize, (VOID *)&IntelVendorDataEntry);

  if (*VendorDataBufferPtr == NULL) {
    *VendorDataTotalSize  = 0;
    *VendorDataEntryCount = 0;
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  Checks if reset was from cold boot.
  If yes, update reset source.

  @param[in]  ResetCauseReg     Pointer to RESET_CAUSE_PM_REGISTERS structure which
                                contains PM registers related to reset cause

  @param[in]  PmcSleepState     Previous sleep state

  @param[out] Source            Pointer to reset source to be updated in this function.

  @retval TRUE    Cold boot is detected
  @retval FALSE   Cold boot is not detected

**/
BOOLEAN
IsColdBoot (
  IN     CONST RESET_CAUSE_PM_REGISTERS  *ResetCauseReg,
  IN           PMC_SLEEP_STATE           PmcSleepState,
     OUT       UINT8                     *Source
  )
{
  if ((ResetCauseReg == NULL) || (Source == NULL)) {
    return FALSE;
  }

  if (PmcSleepState == PmcS5SleepState) {
    *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
    return TRUE;
  }

  return FALSE;
}

/**
  Checks if reset was from cold reset.
  If yes, update reset source.

  @param[in]  ResetCauseReg     Pointer to RESET_CAUSE_PM_REGISTERS structure which
                                contains PM registers related to reset cause
  @param[out] Source            Pointer to reset source to be updated in this function.

  @retval TRUE    Cold reset is detected
  @retval FALSE   Cold reset is not detected

**/
BOOLEAN
IsColdReset (
  IN     CONST RESET_CAUSE_PM_REGISTERS  *ResetCauseReg,
     OUT       UINT8                     *Source
  )
{
  if ((ResetCauseReg == NULL) || (Source == NULL)) {
    return FALSE;
  }

  if ((ResetCauseReg->HbrCause0 & B_PMC_PWRM_HPR_CAUSE0_COLD_RESET) != 0) {
    *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
    return TRUE;
  }

  if ((ResetCauseReg->HbrCause0 & B_PMC_PWRM_HPR_CAUSE0_CF9_ES) != 0) {
    //
    // Host software writes a value of 06h or 0Eh to the CF9 register.
    // Further leverage MEM_SR to check if it's cold or warm reset.
    //
    if ((ResetCauseReg->GenPmconA & B_PMC_PWRM_GEN_PMCON_A_MEM_SR) == 0) {
      *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Checks if reset was from warm reset.
  If yes, update reset source.

  @param[in]  ResetCauseReg     Pointer to RESET_CAUSE_PM_REGISTERS structure which
                                contains PM registers related to reset cause
  @param[out] Source            Pointer to reset source to be updated in this function.

  @retval TRUE    Warm reset is detected
  @retval FALSE   Warn reset is not detected

**/
BOOLEAN
IsWarmReset (
  IN     CONST RESET_CAUSE_PM_REGISTERS  *ResetCauseReg,
     OUT       UINT8                     *Source
  )
{
  if ((ResetCauseReg == NULL) || (Source == NULL)) {
    return FALSE;
  }

  if ((ResetCauseReg->HbrCause0 & B_PMC_PWRM_HPR_CAUSE0_WARM_RESET) != 0) {
    *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
    return TRUE;
  }

  if ((ResetCauseReg->HbrCause0 & B_PMC_PWRM_HPR_CAUSE0_CF9_ES) != 0) {
    //
    // Host software writes a value of 06h or 0Eh to the CF9 register.
    // Further leverage MEM_SR to check if it's cold or warm reset.
    //
    if ((ResetCauseReg->GenPmconA & B_PMC_PWRM_GEN_PMCON_A_MEM_SR) != 0) {
      *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Checks if reset was caused by a fault occured.
  If yes, update reset source.

  @param[in]  ResetCauseReg     Pointer to RESET_CAUSE_PM_REGISTERS structure which
                                contains PM registers related to reset cause
  @param[out] Source            Pointer to reset source to be updated in this function.

  @retval TRUE    Fault reset is detected
  @retval FALSE   Fault reset is not detected

**/
BOOLEAN
IsFaultReset (
  IN     CONST RESET_CAUSE_PM_REGISTERS  *ResetCauseReg,
     OUT       UINT8                     *Source
  )
{
  if ((ResetCauseReg == NULL) || (Source == NULL)) {
    return FALSE;
  }

  if ((ResetCauseReg->GblrstCause0 & B_PMC_PWRM_GBLRST_CAUSE0_FAULT_RESET) != 0) {

    *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;

    if ((ResetCauseReg->GblrstCause0 & B_PMC_PWRM_GBLRST_CAUSE0_FAULT_RESET_HW) != 0) {
      *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_HARDWARE;
    } else if ((ResetCauseReg->GblrstCause0 & B_PMC_PWRM_GBLRST_CAUSE0_ME_PBO) != 0) {
      *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_FIRMWARE;
    }
    return TRUE;
  }

  if ((ResetCauseReg->GblrstCause1 & B_PMC_PWRM_GBLRST_CAUSE1_FAULT_RESET) != 0) {

    *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_HARDWARE;
    return TRUE;
  }

  return FALSE;
}

/**
  Checks if reset was unexpected.
  If yes, update reset source.

  @param[in]  ResetCauseReg     Pointer to RESET_CAUSE_PM_REGISTERS structure which
                                contains PM registers related to reset cause
  @param[out] Source            Pointer to reset source to be updated in this function.

  @retval TRUE    Unexpected reset is detected
  @retval FALSE   Unexpected reset is not detected

**/
BOOLEAN
IsUnexpectedReset (
  IN     CONST RESET_CAUSE_PM_REGISTERS  *ResetCauseReg,
     OUT       UINT8                     *Source
  )
{
  if ((ResetCauseReg == NULL) || (Source == NULL)) {
    return FALSE;
  }

  if ((ResetCauseReg->GblrstCause0 & B_PMC_PWRM_GBLRST_CAUSE0_ESE_GBLRST_REQ) != 0) {
    *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
    return TRUE;
  }

  return FALSE;
}

/**
  Checks if reset was caused by a timeout occured.
  If yes, update reset source.

  @param[in]  ResetCauseReg     Pointer to RESET_CAUSE_PM_REGISTERS structure which
                                contains PM registers related to reset cause
  @param[out] Source            Pointer to reset source to be updated in this function.

  @retval TRUE    Timeout reset is detected
  @retval FALSE   Timeout reset is not detected

**/
BOOLEAN
IsTimeoutReset (
  IN     CONST RESET_CAUSE_PM_REGISTERS  *ResetCauseReg,
     OUT       UINT8                     *Source
  )
{
  if ((ResetCauseReg == NULL) || (Source == NULL)) {
    return FALSE;
  }

  if (((ResetCauseReg->GblrstCause0 & B_PMC_PWRM_GBLRST_CAUSE0_TIMEOUT_RESET) != 0) || \
      ((ResetCauseReg->HbrCause0    & B_PMC_PWRM_HPR_CAUSE0_TCO_WDT) != 0)) {
    *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
    return TRUE;
  }

  return FALSE;
}

/**
  Checks if reset was caused by a thermal limit exceeded.
  If yes, update reset source.

  @param[in]  ResetCauseReg     Pointer to RESET_CAUSE_PM_REGISTERS structure which
                                contains PM registers related to reset cause
  @param[out] Source            Pointer to reset source to be updated in this function.

  @retval TRUE    Thermal reset is detected
  @retval FALSE   Thermal reset is not detected

**/
BOOLEAN
IsThermalReset (
  IN     CONST RESET_CAUSE_PM_REGISTERS  *ResetCauseReg,
     OUT       UINT8                     *Source
  )
{
  if ((ResetCauseReg == NULL) || (Source == NULL)) {
    return FALSE;
  }

  if (((ResetCauseReg->GblrstCause0 & B_PMC_PWRM_GBLRST_CAUSE0_THERMAL_RESET) != 0) ||
      ((ResetCauseReg->GblrstCause1 & B_PMC_PWRM_GBLRST_CAUSE1_THERMAL_RESET) != 0)) {
    *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_HARDWARE;
    return TRUE;
  }

  return FALSE;
}

/**
  Checks if reset was from Power Loss occurrence
  If yes, update reset source.

  @param[in]  ResetCauseReg     Pointer to RESET_CAUSE_PM_REGISTERS structure which
                                contains PM registers related to reset cause
  @param[out] Source            Pointer to reset source to be updated in this function.

  @retval TRUE    Power loss is detected
  @retval FALSE   Power loss is not detected

**/
BOOLEAN
IsPowerLossReset (
  IN     CONST RESET_CAUSE_PM_REGISTERS  *ResetCauseReg,
     OUT       UINT8                     *Source
  )
{
  if ((ResetCauseReg == NULL) || (Source == NULL)) {
    return FALSE;
  }

  if ((ResetCauseReg->GenPmconA & B_PMC_PWRM_GEN_PMCON_A_PF) != 0) {
    *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_HARDWARE;
    return TRUE;
  }

  return FALSE;
}

/**
  Checks if reset was from power button press
  If yes, update reset source.

  @param[in]  ResetCauseReg     Pointer to RESET_CAUSE_PM_REGISTERS structure which
                                contains PM registers related to reset cause
  @param[out] Source            Pointer to reset source to be updated in this function.

  @retval TRUE    Power button press is detected
  @retval FALSE   Power button press is not detected

**/
BOOLEAN
IsPowerButtonReset (
  IN     CONST RESET_CAUSE_PM_REGISTERS  *ResetCauseReg,
     OUT       UINT8                     *Source
  )
{
  if ((ResetCauseReg == NULL) || (Source == NULL)) {
    return FALSE;
  }

  if ((ResetCauseReg->GblrstCause0 & B_PMC_PWRM_GBLRST_CAUSE0_PBO) != 0) {
    *Source = EFI_ACPI_6_5_PHAT_RESET_REASON_SOURCES_UNKNOWN;
    return TRUE;
  }

  return FALSE;
}

/**
  Checks the different possible reasons for the platform reset and update the reset reason
  and source for the table.

  @param[in,out] ResetReasonRecord  Pointer to the reset reason health record structure

  @retval None.

**/
VOID
UpdateSourceAndReasonInRecord (
  IN OUT EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_STRUCTURE  *ResetReasonRecord
  )
{
  RESET_CAUSE_PM_REGISTERS  ResetCauseRegs;
  PMC_SLEEP_STATE           PmcSleepState;

  if (ResetReasonRecord == NULL) {
    ASSERT (FALSE);
    return;
  }

  //
  // Get the PMC reset registers
  //
  PmcSleepState = PmcNotASleepState;
  ZeroMem (&ResetCauseRegs, sizeof (RESET_CAUSE_PM_REGISTERS));
  GetResetCauseRegisters (&ResetCauseRegs, &PmcSleepState);

  DEBUG ((DEBUG_INFO, "GEN_PMCON_A   = 0x%x\n", ResetCauseRegs.GenPmconA));
  DEBUG ((DEBUG_INFO, "HPR_CAUSE0    = 0x%x\n", ResetCauseRegs.HbrCause0));
  DEBUG ((DEBUG_INFO, "GBLRST_CAUSE0 = 0x%x\n", ResetCauseRegs.GblrstCause0));
  DEBUG ((DEBUG_INFO, "GBLRST_CAUSE1 = 0x%x\n", ResetCauseRegs.GblrstCause1));
  DEBUG ((DEBUG_INFO, "PmcSleepState = 0x%x\n", PmcSleepState));

  //
  // Update supported sources from current platform.
  //
  ResetReasonRecord->SupportedSources = \
    EFI_ACPI_6_5_PHAT_RESET_REASON_SUPPORTED_SOURCES_UNKNOWN  | \
    EFI_ACPI_6_5_PHAT_RESET_REASON_SUPPORTED_SOURCES_HARDWARE | \
    EFI_ACPI_6_5_PHAT_RESET_REASON_SUPPORTED_SOURCES_FIRMWARE;

  //
  // Initialize reset reason and source.
  //
  ResetReasonRecord->Reason = EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_UNKNOWN;
  ResetReasonRecord->Source = 0;

  if (IsColdBoot (&ResetCauseRegs, PmcSleepState, &(ResetReasonRecord->Source))) {
    ResetReasonRecord->Reason = EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_COLD_BOOT;
    DEBUG ((DEBUG_INFO, "Cold boot is detected.\n"));
    return;
  }

  if (IsUnexpectedReset (&ResetCauseRegs, &(ResetReasonRecord->Source))) {
    ResetReasonRecord->Reason = EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_UNEXPECTED_RESET;
    DEBUG ((DEBUG_INFO, "Unexpected reset is detected.\n"));
    return;
  }

  if (IsFaultReset (&ResetCauseRegs, &(ResetReasonRecord->Source))) {
    ResetReasonRecord->Reason = EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_FAULT;
    DEBUG ((DEBUG_INFO, "Fault reset is detected.\n"));
    return;
  }

  if (IsTimeoutReset (&ResetCauseRegs, &(ResetReasonRecord->Source))) {
    ResetReasonRecord->Reason = EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_TIMEOUT;
    DEBUG ((DEBUG_INFO, "Timeout reset is detected.\n"));
    return;
  }

  if (IsThermalReset (&ResetCauseRegs, &(ResetReasonRecord->Source))) {
    ResetReasonRecord->Reason = EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_THERMAL;
    DEBUG ((DEBUG_INFO, "Thermal reset is detected.\n"));
    return;
  }

  if (IsPowerLossReset (&ResetCauseRegs, &(ResetReasonRecord->Source))) {
    ResetReasonRecord->Reason = EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_POWER_LOSS;
    DEBUG ((DEBUG_INFO, "Power loss is detected.\n"));
    return;
  }

  if (IsPowerButtonReset (&ResetCauseRegs, &(ResetReasonRecord->Source))) {
    ResetReasonRecord->Reason = EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_POWER_BUTTON;
    DEBUG ((DEBUG_INFO, "Power button override is detected.\n"));
    return;
  }

  //
  // Check Cold/Warm reset last because CF9_ES is set most of the time.
  //
  if (IsColdReset (&ResetCauseRegs, &(ResetReasonRecord->Source))) {
    ResetReasonRecord->Reason = EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_COLD_RESET;
    DEBUG ((DEBUG_INFO, "Cold reset is detected.\n"));
    return;
  }

  if (IsWarmReset (&ResetCauseRegs, &(ResetReasonRecord->Source))) {
    ResetReasonRecord->Reason = EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_WARM_RESET;
    DEBUG ((DEBUG_INFO, "Warm reset is detected.\n"));
    return;
  }

  return;
}

/**
  Map reset reason to AmHealthy according to ACPI spec.

  @param[in] ResetReason  Reason in Reset Reason Health Record Structure

  @retval AmHealthy  Indicate the healthy state according to reset reason.
                     The value should be one of the following:
                       EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_ERRORS_FOUND
                       EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_NO_ERRORS_FOUND
                       EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_UNKNOWN
                       EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_ADVISORY

**/
UINT8
MapResetReasonToAmHealthy (
  IN UINT8  ResetReason
  )
{
  UINT8  AmHealthy;

  switch (ResetReason) {
    case EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_UNKNOWN:
      AmHealthy = EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_UNKNOWN;
      break;

    case EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_COLD_BOOT:
    case EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_COLD_RESET:
    case EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_WARM_RESET:
    case EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_UPDATE:
      AmHealthy = EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_NO_ERRORS_FOUND;
      break;

    case EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_UNEXPECTED_RESET:
    case EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_FAULT:
    case EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_TIMEOUT:
    case EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_THERMAL:
    case EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_POWER_LOSS:
    case EFI_ACPI_6_5_PHAT_RESET_REASON_REASON_POWER_BUTTON:
      AmHealthy = EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_ERRORS_FOUND;
      break;

    default:
      AmHealthy = EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_UNKNOWN;
      break;
  }

  return AmHealthy;
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
  EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_STRUCTURE          *ResetReasonHealthRecord;
  UINT16                                                          ResetReasonHealthRecordSize;
  EFI_STATUS                                                      Status;
  EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_VENDOR_DATA_ENTRY  *VendorDataBuffer;
  UINT16                                                          VendorDataTotalSize;
  UINT16                                                          VendorDataEntryCount;
  UINT8                                                           AmHealthy;

  VendorDataBuffer     = NULL;
  VendorDataTotalSize  = 0;
  VendorDataEntryCount = 0;
  AmHealthy            = EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_UNKNOWN;

  //
  // Retrieve vendor data first to calcuate the totol reset reason record size
  //
  Status = ConstructResetReasonVendorData (
             &VendorDataBuffer,
             &VendorDataTotalSize,
             &VendorDataEntryCount
             );

  if (EFI_ERROR (Status)) {
    VendorDataBuffer     = NULL;
    VendorDataTotalSize  = 0;
    VendorDataEntryCount = 0;
  }

  ResetReasonHealthRecordSize = \
      sizeof (EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_STRUCTURE) \
    + VendorDataTotalSize;

  ResetReasonHealthRecord = AllocateZeroPool (ResetReasonHealthRecordSize);
  if (ResetReasonHealthRecord == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  //
  // Attach vendor data and update VendorCount accordingly.
  //
  CopyMem ((UINT8 *)ResetReasonHealthRecord + sizeof (EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_STRUCTURE), \
           VendorDataBuffer, \
           VendorDataTotalSize
           );

  ResetReasonHealthRecord->VendorCount = VendorDataEntryCount;

  //
  // Update reset reason and source
  //
  UpdateSourceAndReasonInRecord (ResetReasonHealthRecord);

  //
  // Update AmHealthy according to reset reason.
  //
  AmHealthy = MapResetReasonToAmHealthy (ResetReasonHealthRecord->Reason);

  DEBUG ((DEBUG_INFO, "Reset Reason : 0x%X\n", ResetReasonHealthRecord->Reason));
  DEBUG ((DEBUG_INFO, "Reset Source : 0x%X\n", ResetReasonHealthRecord->Source));
  DEBUG ((DEBUG_INFO, "AmHealthy    : 0x%X\n", AmHealthy));

  //
  // Publish table to PHAT
  //
  Status = PublishResetReasonPhat (
             ResetReasonHealthRecord,
             ResetReasonHealthRecordSize,
             AmHealthy
             );

Exit:

  if (ResetReasonHealthRecord != NULL) {
    FreePool (ResetReasonHealthRecord);
  }

  if (VendorDataBuffer != NULL) {
    FreePool (VendorDataBuffer);
  }
  
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
