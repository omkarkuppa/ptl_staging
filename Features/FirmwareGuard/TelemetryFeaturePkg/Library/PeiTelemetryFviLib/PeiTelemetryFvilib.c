/** @file
  PEI Telemetry Lib implementation.

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <TelemetryHob.h>
#include <TelemetryCommon.h>

/**
  Append an entry after Fvi AIP table.

  @param[in] ComponentId      ComponentId for the Fvi entry.
  @param[in] Version          Version for Fvi entry.
  @param[in] ProducerId       ProducerId for Fvi entry, e.g. 'INTC'.

  @retval EFI_SUCCESS           - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES  - if not able to get resources.
  @retval EFI_INVALID_PARAMETER - if Data == NULL.
**/
EFI_STATUS
EFIAPI
AppendTelemetryFviBlock (
  IN  EFI_GUID  ComponentId,
  IN  UINT64    Version,
  IN  UINT32    ProducerId
  )
{
  TELEMETRY_FIRMWARE_VERSION_INFO2_HOB *FviHob;
  EFI_ACPI_6_5_PHAT_VERSION_ELEMENT    *TelemetryVersionRecord;
  UINT8                                FviHobSize;
  UINT8                                FviHobHeaderSize;
  VOID                                 *FviHobPtr;

  FviHobPtr        = NULL;
  FviHobHeaderSize = OFFSET_OF (TELEMETRY_FIRMWARE_VERSION_INFO2_HOB, Records);
  FviHobSize        = FviHobHeaderSize + sizeof (EFI_ACPI_6_5_PHAT_VERSION_ELEMENT);

  //
  // Initialize Fvi Telemetry data.
  //
  FviHob = (TELEMETRY_FIRMWARE_VERSION_INFO2_HOB *) AllocateZeroPool (FviHobSize);
  if (FviHob == NULL) {
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  }

  FviHob->Header.HobType        = EFI_HOB_TYPE_GUID_EXTENSION;
  FviHob->Header.HobLength      = FviHobSize;
  FviHob->Header.Reserved       = 0;
  FviHob->Revision              = TELEMETRY_HOB_STRUCTURE_REVISION;
  CopyGuid (&FviHob->Signature, &gTelemetryVersionHobGuid);
  FviHob->RecordCount  = 1;

  TelemetryVersionRecord = (EFI_ACPI_6_5_PHAT_VERSION_ELEMENT *) ((UINTN)FviHob + FviHobHeaderSize);

  TelemetryVersionRecord->VersionValue = Version;
  TelemetryVersionRecord->ProducerId   = ProducerId;
  CopyGuid (&TelemetryVersionRecord->ComponentId, &ComponentId);

  FviHobPtr = BuildGuidDataHob (
                &gTelemetryVersionHobGuid,
                (VOID *) FviHob,
                FviHobSize
                );
  ASSERT (FviHobPtr != NULL);

  FreePool (FviHob);
  return EFI_SUCCESS;
}