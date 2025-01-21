/**@file
  Create ASF ACPI table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#include "AsfTable.h"

EFI_ACPI_DESCRIPTION_HEADER AsfTableHeader =
{
  EFI_ACPI_ASF_DESCRIPTION_TABLE_SIGNATURE,
  0,                          // overwritten after table building
  EFI_ACPI_2_0_ASF_DESCRIPTION_TABLE_REVISION
};

//
// Remote Control Capabilities supported Bit Masks
//
ASF_RMCP_REMOTE_CTRL_CAPS mRemoteControlCapabilities =
{
  {                      // Boot Options/System Firmware Capabilities Bit Mask
    LOCK_KEYBOARD,
    FORCE_PROGRESS_EVENTS | USER_PASSWORD_BYPASS,
    0x00,                // Reserved
    0x00                 // Reserved
  },
  {                      // Special Commands Bit Mask
    0x00,                // Reserved
    SUPPORT_FORCE_CDDVD | SUPPORT_FORCE_HARDDRIVE | SUPPORT_FORCE_PXE
  },
  {                      // System Capabilities Bit Mask
    SUPPORT_RESET_COMBO | SUPPORT_POWER_UP_COMBO | SUPPORT_POWER_DOWN_COMBO | SUPPORT_POWER_CYCLE_RESET_COMBO
  }
};

//
// Target SMBUS Read devices
//
EFI_ACPI_ASF_ALERTDATA mAsfAlertDevices[] =
{
  { 0x89, 0x04, 0x01, 0x01, 0x05, 0x6F, 0x00, 0x68, 0x08, 0x88, 0x17, 0x00 },  // Device 0
  { 0x89, 0x04, 0x04, 0x04, 0x07, 0x6F, 0x00, 0x68, 0x20, 0x88, 0x03, 0x00 },  // Device 1
  { 0x89, 0x05, 0x01, 0x01, 0x19, 0x6F, 0x00, 0x68, 0x20, 0x88, 0x22, 0x00 }   // Device 2
};

//
// Target SMBUS Write cmds
//
EFI_ACPI_ASF_CONTROLDATA mAsfAlertControls[] =
{
  { 0x00, 0x88, 0x00, 0x03 },           // Control 0 --> Reset system
  { 0x01, 0x88, 0x00, 0x02 },           // Control 1 --> Power Off system
  { 0x02, 0x88, 0x00, 0x01 },           // Control 2 --> Power On system
  { 0x03, 0x88, 0x00, 0x04 }            // Control 3 --> Power Cycle Reset (off then on)
};

//
// Fixed SMBUS Addresses
//
UINT8 mAsfAlertAddresses[] =
{
  0x5C, 0x68, 0x88, 0xC2, 0xD2, // Fixed SMBus Address (Heceta 6, 82573E device - static def (also an ARP addr),
  0xDC, 0xA0, 0xA2, 0xA4, 0xA6, // Target Device, SMB ARP, CK410, DB400/800, SDRAM SPD devices
  0xC8                          // TEKOA Target Device
};

/**
  Entry point for the ACPI ASF Table Driver.
  Building Asf Table when Ready To Boot event is signaled.

  @param[in] ImageHandle   - Pointer to the loaded image protocol for this driver
  @param[in] SystemTable   - Pointer to the EFI System Table

  @retval EFI_SUCCESS      - Building ASF Table successfully.
**/
EFI_STATUS
EFIAPI
AsfTableEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_EVENT   AsfEvent;
  EFI_STATUS  Status;

  Status = EfiCreateEventReadyToBootEx (
             TPL_NOTIFY,
             BuildAsfTable,
             (VOID *)&ImageHandle,
             &AsfEvent
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Calculate ASF Table Info part.

  @param[in]        AsfTable     - Pointer to ASF Table.
  @param[in,out]    TableOffset  - On input - offset to the beginning of ASF_INFO Table
                                   On output - current size of the ASF Table
**/
VOID
CalculateAsfTableInfo (
  IN      ASF_TABLE        *AsfTable,
  IN OUT  UINT32           *TableOffset
  )
{
  EFI_ACPI_ASF_INFO        *AsfInfo;

  AsfInfo = (EFI_ACPI_ASF_INFO *) &AsfTable->TableData[*TableOffset];
  ZeroMem (AsfInfo, sizeof (EFI_ACPI_ASF_INFO));

  AsfInfo->RecordHeader.Type         = ASF_INFO_TYPE;
  AsfInfo->RecordHeader.RecordLength = sizeof (EFI_ACPI_ASF_INFO);
  AsfInfo->MinWatchDogResetValue     = ASF_INFO_MIN_WATCH_DOG_RESET_VALUE;
  AsfInfo->MinPollingInterval        = ASF_INFO_MIN_POLLING_INTERVAL;
  AsfInfo->SystemID                  = ASF_INFO_SYSTEM_ID;
  AsfInfo->IANAManufactureID         = ASF_INTEL_CONVERTED_IANA;

  *TableOffset += sizeof (EFI_ACPI_ASF_INFO);
}

/**
  Calculate ASF Table Alert part.

  @param[in]        AsfTable     - Pointer to ASF Table.
  @param[in,out]    TableOffset  - On input - offset to the beginning of ASF_ALRT Table
                                   On output - current size of the ASF Table
**/
VOID
CalculateAsfTableAlert (
  IN      ASF_TABLE        *AsfTable,
  IN OUT  UINT32           *TableOffset
  )
{
  EFI_ACPI_ASF_ALRT            *AsfAlert;

  AsfAlert = (EFI_ACPI_ASF_ALRT *) &AsfTable->TableData[*TableOffset];
  ZeroMem (AsfAlert, sizeof (EFI_ACPI_ASF_ALRT) + sizeof (mAsfAlertDevices));

  AsfAlert->RecordHeader.Type         = ASF_ALRT_TYPE;
  AsfAlert->RecordHeader.RecordLength = sizeof (EFI_ACPI_ASF_ALRT) + sizeof (mAsfAlertDevices);
  AsfAlert->NumberOfAlerts            = sizeof (mAsfAlertDevices) / sizeof (EFI_ACPI_ASF_ALERTDATA);
  AsfAlert->ArrayElementLength        = sizeof (EFI_ACPI_ASF_ALERTDATA);

  CopyMem ((UINT8 *)AsfAlert + sizeof (EFI_ACPI_ASF_ALRT), &mAsfAlertDevices, sizeof (mAsfAlertDevices));
  *TableOffset += AsfAlert->RecordHeader.RecordLength;
}

/**
  Calculate ASF Table Rctl part.

  @param[in]        AsfTable     - Pointer to ASF Table.
  @param[in,out]    TableOffset  - On input - offset to the beginning of ASF_RCTL Table
                                   On output - current size of the ASF Table
**/
VOID
CalculateAsfTableRctl (
  IN      ASF_TABLE        *AsfTable,
  IN OUT  UINT32           *TableOffset
  )
{
  EFI_ACPI_ASF_RCTL            *AsfRctl;

  AsfRctl = (EFI_ACPI_ASF_RCTL *) &AsfTable->TableData[*TableOffset];
  ZeroMem (AsfRctl, sizeof (EFI_ACPI_ASF_RCTL) + sizeof (mAsfAlertControls));

  AsfRctl->RecordHeader.Type         = ASF_RCTL_TYPE;
  AsfRctl->RecordHeader.RecordLength = sizeof (EFI_ACPI_ASF_RCTL) + sizeof (mAsfAlertControls);
  AsfRctl->NumberOfControls          = sizeof (mAsfAlertControls) / sizeof (EFI_ACPI_ASF_CONTROLDATA);
  AsfRctl->ArrayElementLength        = sizeof (EFI_ACPI_ASF_CONTROLDATA);

  CopyMem ((UINT8 *)AsfRctl + sizeof (EFI_ACPI_ASF_RCTL), &mAsfAlertControls, sizeof (mAsfAlertControls));
  *TableOffset += AsfRctl->RecordHeader.RecordLength;
}

/**
  Calculate ASF Table Rmpc part.

  @param[in]        AsfTable     - Pointer to ASF Table.
  @param[in,out]    TableOffset  - On input - offset to the beginning of ASF_RCMP Table
                                   On output - current size of the ASF Table
**/
VOID
CalculateAsfTableRmcp (
  IN      ASF_TABLE        *AsfTable,
  IN OUT  UINT32           *TableOffset
  )
{
  EFI_ACPI_ASF_RMCP                   *AsfRmcp;

  AsfRmcp = (EFI_ACPI_ASF_RMCP *) &AsfTable->TableData[*TableOffset];
  ZeroMem (AsfRmcp, sizeof (EFI_ACPI_ASF_RMCP));

  AsfRmcp->RecordHeader.Type         = ASF_RMCP_TYPE;
  AsfRmcp->RecordHeader.RecordLength = sizeof (EFI_ACPI_ASF_RMCP);

  CopyMem (&AsfRmcp->RemoteControlCapabilities, &mRemoteControlCapabilities, sizeof (ASF_RMCP_REMOTE_CTRL_CAPS));

  #if FixedPcdGetBool (PcdPlatformSanitizeRpeSupport) == 1
    AsfRmcp->RemoteControlCapabilities[1] |= CONFIG_DATA_RESET;
  #endif

  AsfRmcp->RMCPCompletionCode = ASF_RMCP_NO_BOOT_OPTION;

  *TableOffset += sizeof (EFI_ACPI_ASF_RMCP);
}

/**
  Calculate ASF Table Address part.

  @param[in]        AsfTable     - Pointer to ASF Table.
  @param[in,out]    TableOffset  - On input - offset to the beginning of ASF_ADDR Table
                                   On output - current size of the ASF Table
**/
VOID
CalculateAsfTableAddr (
  IN      ASF_TABLE        *AsfTable,
  IN OUT  UINT32           *TableOffset
  )
{
  EFI_ACPI_ASF_ADDR          *AsfAddress;

  AsfAddress = (EFI_ACPI_ASF_ADDR *) &AsfTable->TableData[*TableOffset];
  ZeroMem (AsfAddress, sizeof (EFI_ACPI_ASF_ADDR) + sizeof (mAsfAlertAddresses));

  AsfAddress->RecordHeader.Type         = ASF_LAST_RECORD_TYPE | ASF_ADDR_TYPE;
  AsfAddress->RecordHeader.RecordLength = sizeof (EFI_ACPI_ASF_ADDR) + sizeof (mAsfAlertAddresses);
  AsfAddress->NumberOfDevices           = sizeof (mAsfAlertAddresses) / sizeof (UINT8);

  CopyMem ((UINT8 *)AsfAddress + sizeof (EFI_ACPI_ASF_ADDR), &mAsfAlertAddresses, sizeof (mAsfAlertAddresses));
  *TableOffset += AsfAddress->RecordHeader.RecordLength;
}

/**
  Get ASF Sensors table presence info from ME Setup

  @retval TRUE        MeSetup.AsfSensorsTable is present.
  @retval FALSE       MeSetup.AsfSensorsTable is NOT present.

**/
BOOLEAN
IsAsfSensorsTablePresent (
  VOID
  )
{
  EFI_STATUS                  Status;
  ME_SETUP                    MeSetup;
  UINTN                       SetupSize;

  SetupSize = sizeof (ME_SETUP);

  Status = gRT->GetVariable (L"MeSetup", &gMeSetupVariableGuid, NULL, &SetupSize, &MeSetup);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Cannot get \"MeSetup\" variable\n"));
    return FALSE;
  }
  return (BOOLEAN)(MeSetup.AsfSensorsTable == 1);
}

/**
  Install Alert Standard Format (ASF) Table.

  @param[in] Event         - A pointer to the Event that triggered the callback.
  @param[in] Context       - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
BuildAsfTable (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                Status;
  UINTN                     AsfTableHandle;
  EFI_ACPI_TABLE_PROTOCOL   *AcpiTable;
  ASF_TABLE                 *AsfTable;
  UINT32                    TableOffset;
  UINT32                    TableSize;
  BOOLEAN                   AsfSensorsTablePresent;

  AsfTableHandle = 0;

  gBS->CloseEvent (Event);

  AsfSensorsTablePresent = IsAsfSensorsTablePresent ();
  TableSize = sizeof (EFI_ACPI_DESCRIPTION_HEADER) +
              sizeof (EFI_ACPI_ASF_INFO) +
              sizeof (EFI_ACPI_ASF_RCTL) +
              sizeof (mAsfAlertControls) +
              sizeof (EFI_ACPI_ASF_RMCP) +
              sizeof (EFI_ACPI_ASF_ADDR) +
              sizeof (mAsfAlertAddresses);
  if (AsfSensorsTablePresent) {
    TableSize += (sizeof (EFI_ACPI_ASF_ALRT) + sizeof (mAsfAlertDevices));
  }

  AsfTable = AllocateZeroPool (TableSize);
  if (AsfTable == NULL) {
    DEBUG ((DEBUG_WARN, "BuildAsfTable Error: Could not allocate Memory\n"));
    return;
  }

  CopyMem (&AsfTable->TableHeader, &AsfTableHeader, sizeof (AsfTableHeader));
  CopyMem (AsfTable->TableHeader.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (AsfTable->TableHeader.OemId));
  AsfTable->TableHeader.Length          = TableSize;
  AsfTable->TableHeader.OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  AsfTable->TableHeader.OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  AsfTable->TableHeader.CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  AsfTable->TableHeader.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  TableOffset = 0;
  CalculateAsfTableInfo (AsfTable, &TableOffset);

  if (AsfSensorsTablePresent) {
    CalculateAsfTableAlert (AsfTable, &TableOffset);
  }

  CalculateAsfTableRctl (AsfTable, &TableOffset);
  CalculateAsfTableRmcp (AsfTable, &TableOffset);
  CalculateAsfTableAddr (AsfTable, &TableOffset);

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  if (!EFI_ERROR (Status)) {
    Status = AcpiTable->InstallAcpiTable (
                          AcpiTable,
                          &AsfTable->TableHeader,
                          AsfTable->TableHeader.Length,
                          &AsfTableHandle
                          );
  }
  FreePool (AsfTable);
}
