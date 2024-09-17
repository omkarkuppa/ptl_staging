/** @file
  Source file for installing selected ACX Codec Topology.

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

#include <Base.h>
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <PiDxe.h>
#include <Pi/PiFirmwareFile.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/AslUpdateLib.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/PciIo.h>
#include <SndwDevTopologyConfigurationVariable.h>

#define MAX_TMP_HDAS_DEVICE_PATH       100
#define MAX_ACPI_NAME_LENGTH           4
#define V_ACE_BCC_NON_UAA_DSP_EN       0x4
#define V_ACE_SCC_NON_UAA_DSP_EN       0x1
#define V_ACE_PI_NON_UAA_DSP_EN        0x0
#define V_ACE_BCC_UAA_DSP_EN           0x4
#define V_ACE_SCC_UAA_DSP_EN           0x3
#define V_ACE_PI_UAA_DSP_EN            0x80

GLOBAL_REMOVE_IF_UNREFERENCED UINT64 SupportedHdaDidValue[] = {
  0x67A8,
  0x67A9,
  0x67AA,
  0x67AB,
  0x67AC,
  0x67AD,
  0x67AE,
  0x67AF,
  0x7E28,
  0x7E29,
  0x7E2A,
  0x7E2B,
  0x7E2C,
  0x7E2D,
  0x7E2E,
  0x7E2F,
  0x7F50,
  0x7F51,
  0x7F52,
  0x7F53,
  0x7F54,
  0x7F55,
  0x7F56,
  0x7F57,
  0xA828,
  0xA829,
  0xA82A,
  0xA82B,
  0xA82C,
  0xA82D,
  0xA82E,
  0xA82F,
  0x8E28,
  0x7728,
  0x7729,
  0x772A,
  0x772B,
  0x772C,
  0x772D,
  0x772E,
  0x772F,
  0xE428,
  0xE429,
  0xE42A,
  0xE42B,
  0xE42C,
  0xE42D,
  0xE42E,
  0xE42F,
  0xE328,
  0xE329,
  0xE32A,
  0xE32B,
  0xE32C,
  0xE32D,
  0xE32E,
  0xE32F,
  0x4D28,
  0x4D29,
  0x4D2A,
  0x4D2B,
  0x4D2C,
  0x4D2D,
  0x4D2E,
  0x4D2F,
  0x6E50,
  0x6E51,
  0x6E52,
  0x6E53,
  0x6E54,
  0x6E55,
  0x6E56,
  0x6E57,
  0xD328,
  0xD329,
  0xD32A,
  0xD32B,
  0xD32C,
  0xD32D,
  0xD32E,
  0xD32F
};

#define DISABLE_LOADING_SSDT                                0xFF
#define ACX_CONFIG_NUMBER_TO_SIGNATURE(AcxConfNr)           \
          SIGNATURE_64 ('S','t', (CHAR8)((AcxConfNr / 10) + (UINT8)'0'), (CHAR8)((AcxConfNr % 10) + (UINT8)'0'),'S','s','d','t')

GLOBAL_REMOVE_IF_UNREFERENCED CHAR8 DefaultHdasDevicePath[] = "\\_SB.PC00.HDAS";
GLOBAL_REMOVE_IF_UNREFERENCED CHAR8 HdasDevicePathRp01[]    = "\\_SB.PC02.HDAS";


/**
  Is Hda device supported.

  @param[in] DeviceId     Device Id
  @retval TRUE            Device Id supported
  @retval FALSE           Device Id not suppoorted
**/
STATIC
BOOLEAN
IsHdaDeviceIdSupported (
  UINT16  DeviceId
  )
{
  UINT8 Index = 0;

  for (Index = 0; Index < ARRAY_SIZE (SupportedHdaDidValue); Index++) {
    if (SupportedHdaDidValue[Index] == DeviceId) {
        return TRUE;
    }
  }
  return FALSE;
}

/**
  Replace path string in SSDT ACPI table. User can modify current device path to new one.

  @param[in] Table             SSDT ACPI table.
  @param[in] OldPathString     Pointer to old ACPI device path string with null termination.
  @param[in] NewPathString     Pointer to new ACPI device path string with null termination.

  @retval EFI_SUCCESS          The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES Memory allocation failed.
**/
EFI_STATUS
ReplacePathStringInSsdtTable (
    IN OUT EFI_ACPI_DESCRIPTION_HEADER  **Table,
    IN CHAR8                            *OldPathString,
    IN CHAR8                            *NewPathString
  )
{
  UINTN                         TableSize;
  UINT8                         *TablePtr;
  UINT8                         *StartCopyTablePtr;
  UINTN                         Index;
  UINTN                         AdditionalSize;
  UINTN                         ResultTableSize;
  UINT8                         *ResultTablePtr;
  UINT8                         *StartCopyResultTablePtr;

  if ((*Table == NULL) || (OldPathString == NULL) || (NewPathString == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "Current device path: %a.\n", OldPathString));
  DEBUG ((DEBUG_INFO, "New device path %a.\n", NewPathString));

  TablePtr = (UINT8 *) *Table;
  TableSize = (*Table)->Length;
  StartCopyTablePtr = TablePtr;
  ResultTableSize = 0;
  ResultTablePtr = NULL;
  StartCopyResultTablePtr = ResultTablePtr;
  AdditionalSize = 0;

  for (Index = sizeof (EFI_ACPI_DESCRIPTION_HEADER); Index < TableSize; Index++) {
    if (CompareMem (&TablePtr[Index], OldPathString, AsciiStrLen (OldPathString)) == 0) {

      AdditionalSize = (&TablePtr[Index] - StartCopyTablePtr) + AsciiStrLen (NewPathString);

      ResultTablePtr = ReallocatePool (ResultTableSize, ResultTableSize + AdditionalSize, ResultTablePtr);
      if (ResultTablePtr == NULL) {
        DEBUG ((DEBUG_WARN, "Re-allocate failed! - %a () End.\n", __FUNCTION__));
        return EFI_OUT_OF_RESOURCES;
      }

      StartCopyResultTablePtr = (UINT8 *) (ResultTablePtr + ResultTableSize);

      CopyMem (StartCopyResultTablePtr, StartCopyTablePtr,  &TablePtr[Index] - StartCopyTablePtr);

      StartCopyResultTablePtr += &TablePtr[Index] - StartCopyTablePtr;

      CopyMem (StartCopyResultTablePtr, NewPathString, AsciiStrLen (NewPathString));

      StartCopyResultTablePtr += AsciiStrLen (NewPathString);

      ResultTableSize = (UINTN) (AdditionalSize + ResultTableSize);

      StartCopyTablePtr = &TablePtr[Index] + (UINTN) AsciiStrLen(OldPathString);
    }
  }

  AdditionalSize = TableSize - (StartCopyTablePtr - TablePtr);
  ResultTablePtr = ReallocatePool (ResultTableSize, ResultTableSize + AdditionalSize, ResultTablePtr);
  if (ResultTablePtr == NULL) {
    DEBUG ((DEBUG_WARN, "Re-allocate failed!\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  StartCopyResultTablePtr = (UINT8 *) (ResultTablePtr + ResultTableSize);
  ResultTableSize = (UINTN) (AdditionalSize + ResultTableSize);

  CopyMem (StartCopyResultTablePtr, StartCopyTablePtr, TableSize - (StartCopyTablePtr - TablePtr));

  FreePool (*Table);
  *Table = AllocateZeroPool (ResultTableSize);
  if (*Table == NULL) {
    DEBUG ((DEBUG_WARN, "Allocate memory for new table failed!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (*Table, ResultTablePtr, ResultTableSize);
  (*Table)->Length = (UINT32) ResultTableSize;
  FreePool (ResultTablePtr);
  DEBUG ((DEBUG_INFO, "Result Table Size: %d.\n", ResultTableSize));

  return EFI_SUCCESS;
}

/**
  Create AML scope path string based on ASL scope path string. E.g. convert \\_SB.PC00.XYZ to _SB_PC00_XYZ_

  @param[in] DevicePath         ASL scpe path string.
  @param[in] ReturnPath         AML scpe path string.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Memory allocation failed.
  @retval EFI_INVALID_PARAMETER Parameters are invalid.
**/
EFI_STATUS
CreateScopePath (
  IN  CHAR8     *DevicePath,
  OUT CHAR8     **ReturnPath
  )
{
  UINTN  Index;
  CHAR8  TmpScopeName[] = "____";
  UINTN  NameSpaceIndex;
  CHAR8  TmpDevicePath[MAX_TMP_HDAS_DEVICE_PATH];

  ZeroMem (TmpDevicePath, sizeof(TmpDevicePath));
  NameSpaceIndex = 0;

  if ((DevicePath == NULL) || (DevicePath[0] != '\\')) {
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 1; Index < AsciiStrSize (DevicePath); Index++) {
    if ((DevicePath[Index] == '.') || (DevicePath[Index] == '\0') || (NameSpaceIndex == MAX_ACPI_NAME_LENGTH)) {
      AsciiStrCatS (TmpDevicePath, MAX_TMP_HDAS_DEVICE_PATH, TmpScopeName);
      NameSpaceIndex = 0;
      AsciiStrCpyS (TmpScopeName, sizeof(TmpScopeName), "____");
    } else {
      TmpScopeName[NameSpaceIndex] = DevicePath[Index];
      NameSpaceIndex++;
    }
  }

  *ReturnPath = AllocateZeroPool (AsciiStrSize (TmpDevicePath));
  if (*ReturnPath == NULL) {
    DEBUG ((DEBUG_WARN, "Allocation failed!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  AsciiStrCpyS (*ReturnPath, AsciiStrSize (TmpDevicePath), TmpDevicePath);

  return EFI_SUCCESS;
}

/**
  Override ACPI device path in ACPI SSDT table.

  @param[in] Table              SSDT ACPI table.
  @param[in] OldPathString      Pointer to old ACPI device path string with null termination.
  @param[in] NewPathString      Pointer to new ACPI device path string with null termination.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Memory allocation failed.
  @retval EFI_INVALID_PARAMETER Parameters are invalid.
**/
EFI_STATUS
OverrideDevicePath (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  **Table,
  IN CHAR8                            *OldDevicePath,
  IN CHAR8                            *NewDevicePath
  )
{
  EFI_STATUS   Status;
  CHAR8        *OldScopePath;
  CHAR8        *NewScopePath;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  Status = ReplacePathStringInSsdtTable (Table, OldDevicePath, NewDevicePath);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Replace HDAS path string in SSDT table failed. %a () End.\n", __FUNCTION__));
    return Status;
  }

  Status = CreateScopePath (OldDevicePath, &OldScopePath);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Failed to create ACPI old scope path for HDAS. %a () End.\n", __FUNCTION__));
    return Status;
  }

  Status = CreateScopePath (NewDevicePath, &NewScopePath);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Failed to create ACPI new scope path for HDAS. %a () End.\n", __FUNCTION__));
    FreePool (OldScopePath);
    return Status;
  }

  Status = ReplacePathStringInSsdtTable (Table, OldScopePath, NewScopePath);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Replace HDAS scope path string in SSDT table failed. %a () End.\n", __FUNCTION__));
  }

  FreePool (OldScopePath);
  FreePool (NewScopePath);

  DEBUG ((DEBUG_INFO, "%a () End.\n", __FUNCTION__));
  return Status;
}

/**
  Get Sndw Dev Topology Configuration Variable.

  @param[out] Table  SndwDevTopologyConfigurationVariable    Sndw Dev Topology Configuration Variable

  @retval EFI_SUCCESS                      The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES             Memory allocation failed.
  @retval EFI_INVALID_PARAMETER            Parameters are invalid.
**/
EFI_STATUS
SndwGetConfigurationVariable (
  OUT SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE  *SndwDevTopologyConfiguration
  )
{
  UINTN                                        VariableSize;
  EFI_STATUS                                   Status;
  SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE     SndwDevTopologyConfigurationVariable;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  VariableSize = sizeof (SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE);
  Status = gRT->GetVariable (
                  SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE_NAME,
                  &gSndwDevTopologyConfigurationVariableGuid,
                  NULL,
                  &VariableSize,
                  &SndwDevTopologyConfigurationVariable
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Failed to get Sndw Dev topology configuration variable. %a () End.\n", __FUNCTION__));
    return Status;
  }

  CopyMem (SndwDevTopologyConfiguration,
    &SndwDevTopologyConfigurationVariable,
    sizeof (SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE)
    );

  DEBUG ((DEBUG_INFO, "%a () End.\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Locate and get SSDT table based on selected configuration.

  @param[in]  NumberOfSsidTable    Number of configuration.
  @param[out] SsidTable            Pointer to SsidTable.

  @retval EFI_SUCCESS           The function completed successfully
  @retval EFI_OUT_OF_RESOURCES  Memory allocation failed.
  @retval EFI_INVALID_PARAMETER Parameters are invalid.
**/
EFI_STATUS
SndwGetSsdtSndwDevTopologyFileFromFv (
  IN  EFI_GUID                          *SsdtTableGuid,
  IN  UINT64                            TableId,
  OUT EFI_ACPI_DESCRIPTION_HEADER       **SsidTable
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  UINTN                         Index;
  INTN                          Instance;
  UINTN                         Size;
  UINT32                        FvStatus;
  EFI_FV_FILETYPE               FileType;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  EFI_ACPI_COMMON_HEADER        *Table;

  FwVol         = NULL;
  Table         = NULL;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  //
  // Locate FV protocol.
  //
  Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiFirmwareVolume2ProtocolGuid, NULL, &NumberOfHandles, &HandleBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Look for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiFirmwareVolume2ProtocolGuid, (VOID **) &FwVol);
    ASSERT_EFI_ERROR (Status);
    if (FwVol == NULL) {
      DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
      FreePool (HandleBuffer);
      return Status;
    }
    //
    // See if it has the ACPI storage file
    //
    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (FwVol, SsdtTableGuid, NULL, &Size, &FileType, &Attributes, &FvStatus);
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  //
  // Free any allocated buffers
  //
  FreePool (HandleBuffer);

  //
  // Read tables from the storage file.
  //
  if (FwVol == NULL) {
    DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  Instance = 0;

  while (Status == EFI_SUCCESS) {
    //
    // Read the ACPI tables
    //
    Status = FwVol->ReadSection (FwVol, SsdtTableGuid, EFI_SECTION_RAW, Instance, (VOID **) &Table, &Size, &FvStatus);
    if (!EFI_ERROR (Status)) {
      TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;
      DEBUG ((DEBUG_INFO, "Selected TableId: 0x%lX, OemTableId: 0x%lX \n", TableId, ((EFI_ACPI_DESCRIPTION_HEADER *) TableHeader)->OemTableId));

      if (CompareMem (&TableHeader->OemTableId, &TableId, 8) == 0) {
        //
        // This is the SSDT table that match the Signature
        //
        DEBUG ((DEBUG_INFO, "Correct SSDT table signature.\n"));

        *SsidTable = AllocateZeroPool (TableHeader->Length);
        if (*SsidTable == NULL) {
          Status = EFI_OUT_OF_RESOURCES;
          DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
          FreePool (Table);
          return Status;
        }
        CopyMem (*SsidTable, TableHeader, TableHeader->Length);
        DEBUG ((DEBUG_INFO, "TableSize: %d\n", TableHeader->Length));

        DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
        FreePool (Table);
        return Status;
      }
      //
      // Increment the instance
      //
      Instance++;
      FreePool (Table);
      Table = NULL;
    }
  }

  if (*SsidTable == NULL) {
    Status = EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Install SSDT ACPI table for Sndw devices topology.

  @param[in] NumberOfSsidTable              Number of configuration.
  @param[in] OverrideDevicePathEnabled      Enable/Disable device path override.
  @param[in] HdasDevicePath                 New Hdas device path string with NULL termination.

  @retval EFI_SUCCESS           The function completed successfully
  @retval EFI_OUT_OF_RESOURCES  Memory allocation failed.
  @retval EFI_INVALID_PARAMETER Parameters are invalid.
**/
EFI_STATUS
SndwDevTopologyInstallAcpiTable (
  IN EFI_GUID                      *SsdtTableGuid,
  IN UINT64                        TableId,
  IN BOOLEAN                       OverrideDevicePathEnabled,
  IN CHAR8                         *HdasDevicePath
  )
{
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader = NULL;
  EFI_STATUS                    Status;
  UINTN                         TableHandle;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  if ((OverrideDevicePathEnabled == TRUE) && (HdasDevicePath == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = SndwGetSsdtSndwDevTopologyFileFromFv (SsdtTableGuid, TableId, &TableHeader);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Cannot find SSDT table. %a () End.\n", __FUNCTION__));
    return Status;
  }

  if (OverrideDevicePathEnabled == TRUE) {
    Status = OverrideDevicePath (&TableHeader, DefaultHdasDevicePath, HdasDevicePath);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "SSDT table override failed. %a () End.\n", __FUNCTION__));
      FreePool (TableHeader);
      return Status;
    }
  }

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Cannot locate AcpiTable Protocol. %a () End.\n", __FUNCTION__));
    FreePool (TableHeader);
    return Status;
  }

  TableHandle = 0;
  Status = AcpiTable->InstallAcpiTable (AcpiTable, TableHeader, TableHeader->Length, &TableHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Install SSDT Acpi Table failed. %a () End.\n", __FUNCTION__));
    FreePool (TableHeader);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Ssdt table installed correctly.\n"));
  FreePool (TableHeader);

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return EFI_SUCCESS;
}

/**
  Function returns Hda Rp Index.

  @param[out] PciRootBridgeIndexPtr    Pointer to Pci Root Bridge Index.

  @retval EFI_SUCCESS           The function completed successfully
  @retval EFI_OUT_OF_RESOURCES  Memory allocation failed.
  @retval EFI_INVALID_PARAMETER Parameters are invalid.
**/
EFI_STATUS
SndwDevTopologyGetHdaBusNumber (
  OUT UINTN                     *BusNumber
  )
{
  UINTN                         PciIoHandleCount;
  EFI_HANDLE                    *PciIoHandleBuffer;
  UINTN                         Index;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINTN                         PciSegment;
  UINTN                         PciBusNumber;
  UINTN                         PciDeviceNumber;
  UINTN                         PciFunctionNumber;
  EFI_STATUS                    Status;
  PCI_TYPE00                    PciConfig;

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciIoHandleCount,
                  &PciIoHandleBuffer
                  );
  if (EFI_ERROR (Status) || (PciIoHandleBuffer == NULL) || (PciIoHandleCount == 0)) {
    return Status;
  }

  for (Index = 0; Index < PciIoHandleCount; Index++) {
    Status = gBS->HandleProtocol (PciIoHandleBuffer[Index], &gEfiPciIoProtocolGuid, (VOID **) &PciIo);
    ASSERT_EFI_ERROR (Status);
    Status = PciIo->GetLocation (PciIo, &PciSegment, &PciBusNumber, &PciDeviceNumber, &PciFunctionNumber);
    if (!EFI_ERROR (Status)) {
      Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, 0, sizeof (PciConfig), &PciConfig);
      if (EFI_ERROR (Status)) {
        continue;
      }

      if ((((PciConfig.Hdr.ClassCode[2] == V_ACE_BCC_NON_UAA_DSP_EN) &&
        (PciConfig.Hdr.ClassCode[1] == V_ACE_SCC_NON_UAA_DSP_EN)) ||
        ((PciConfig.Hdr.ClassCode[2] == V_ACE_BCC_UAA_DSP_EN) &&
        (PciConfig.Hdr.ClassCode[1] == V_ACE_SCC_UAA_DSP_EN))) &&
        (PciConfig.Hdr.VendorId == 0x8086) &&
        (IsHdaDeviceIdSupported (PciConfig.Hdr.DeviceId) == TRUE)) {

        DEBUG ((DEBUG_INFO,
          "HDA controller found on [S:B:D:F][VID:DID] = [%x:%x:%x:%x][%x:%x]\n",
          PciSegment,
          PciBusNumber,
          PciDeviceNumber,
          PciFunctionNumber,
          PciConfig.Hdr.VendorId,
          PciConfig.Hdr.DeviceId));
        DEBUG ((DEBUG_INFO, "HDA Class Code: [BCC:SCC:PI] = [%x:%x:%x]\n",
          PciConfig.Hdr.ClassCode[2],
          PciConfig.Hdr.ClassCode[1],
          PciConfig.Hdr.ClassCode[0]));
        *BusNumber = PciBusNumber;
        gBS->FreePool (PciIoHandleBuffer);
        return EFI_SUCCESS;
      }
    }
  }
  gBS->FreePool (PciIoHandleBuffer);
  *BusNumber = 0;
  DEBUG ((DEBUG_INFO, "%a () End.\n", __FUNCTION__));
  return EFI_DEVICE_ERROR;
}

/**
  Install SSDT ACPI table for Sndw devices topology.

  @param[in] NumberOfSsidTable              Number of configuration.
  @param[in] OverrideDevicePathEnabled      Enable/Disable device path override.
  @param[in] HdasDevicePath                 New Hdas device path string with NULL termination.

  @retval EFI_SUCCESS           The function completed successfully
  @retval EFI_OUT_OF_RESOURCES  Memory allocation failed.
  @retval EFI_INVALID_PARAMETER Parameters are invalid.
**/
EFI_STATUS
SndwDevTopologyInstallAcpiTablesBasedOnPolicy (
  IN BOOLEAN                       OverrideDevicePathEnabled,
  IN CHAR8                         *HdasDevicePath
  )
{
  SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE     SndwDevTopologyConfigurationVariable;
  EFI_STATUS                                   Status;

  Status = SndwGetConfigurationVariable (&SndwDevTopologyConfigurationVariable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Cannot get SoundWire Device Topology Confguration Variable. %a () End.\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  if (FeaturePcdGet (PcdSndwDevTopologyBtSndwSupported) && SndwDevTopologyConfigurationVariable.AcxSndwDevTopologyBtNdaiEnabled) {
    DEBUG ((DEBUG_INFO, "Looking for ACPI SSDT table with description of Bluetooth NDAI device in the FV and trying to install it.\n"));
    Status = SndwDevTopologyInstallAcpiTable (
               &gSndwBtNdaiAcpiTableStorageGuid,
               SIGNATURE_64 ('B','t','N','d','S','s','d','t'),
               OverrideDevicePathEnabled,
               HdasDevicePath
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Failed to install ACPI SSDT table. %a () End.\n", __FUNCTION__));
      return EFI_NOT_FOUND;
    }
  }

  if (FeaturePcdGet (PcdSndwDevTopologyIDispSndwSupported)) {
    DEBUG ((DEBUG_INFO, "Looking for ACPI SSDT table with description of iDisplay NDAI device in the FV and trying to install it.\n"));
    Status = SndwDevTopologyInstallAcpiTable (
               &gSndwIDispNdaiAcpiTableStorageGuid,
               SIGNATURE_64 ('i','D','N','d','S','s','d','t'),
               OverrideDevicePathEnabled,
               HdasDevicePath
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Failed to install ACPI SSDT table. %a () End.\n", __FUNCTION__));
      return EFI_NOT_FOUND;
    }
  }

  if (SndwDevTopologyConfigurationVariable.SndwDevTopologyConfigurationNumber == DISABLE_LOADING_SSDT) {
    DEBUG ((DEBUG_INFO, "Selected SoundWire codec topology installation is disabled.\n%a () End.\n", __FUNCTION__));
    return EFI_SUCCESS;
  } else {
    DEBUG ((DEBUG_INFO, "Looking for ACPI SSDT table with selected SndwDevTopology in the FV and trying to install it.\n"));
    DEBUG ((
      DEBUG_INFO,
      "Number of selected configuration: 0x%x translates to TableId equal to: 0x%lx\n",
      SndwDevTopologyConfigurationVariable.SndwDevTopologyConfigurationNumber,
      ACX_CONFIG_NUMBER_TO_SIGNATURE (SndwDevTopologyConfigurationVariable.SndwDevTopologyConfigurationNumber)
      ));
    Status = SndwDevTopologyInstallAcpiTable (
                &gSndwDevTopologyAcpiTableStorageGuid,
                ACX_CONFIG_NUMBER_TO_SIGNATURE (SndwDevTopologyConfigurationVariable.SndwDevTopologyConfigurationNumber),
                OverrideDevicePathEnabled,
                HdasDevicePath
                );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Failed to install ACPI SSDT table for the selected SoundWire topology. %a () End.\n", __FUNCTION__));
      return EFI_NOT_FOUND;
    }
  }

  return EFI_SUCCESS;
}

/**
  Handler to gather and publish SSDT SoundWire Devices Topology ACPI table.

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context
**/
VOID
EFIAPI
SndwDevTopologyInstallEventOnAcpiProtocol (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                                   Status;
  UINTN                                        HdaBusNumber = 0;

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  Status = SndwDevTopologyGetHdaBusNumber (&HdaBusNumber);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Cannot find HDA device. %a () End - %r.\n", __FUNCTION__, Status));
    return ;
  }

  if (HdaBusNumber == 0) {
    Status = SndwDevTopologyInstallAcpiTablesBasedOnPolicy (FALSE, NULL);
  } else {
    Status = SndwDevTopologyInstallAcpiTablesBasedOnPolicy (TRUE, HdasDevicePathRp01);
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Failed to install selected ACPI SSDT tables. %a () End.\n", __FUNCTION__));
    return ;
  }

  DEBUG ((DEBUG_INFO, "%a () End.\n", __FUNCTION__));
  return ;
}

/**
  Entry point for SndwInstallDevTopology driver for Intel platforms

  @param[in] ImageHandle  This image handle
  @param[in] SystemTable  System table pointer

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_ABORTED             Event not installed
**/
EFI_STATUS
EFIAPI
SndwInstallDevTopologyDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  VOID       *Registration;
  EFI_EVENT  Event;

  Event = NULL;

  Event = EfiCreateProtocolNotifyEvent (
    &gEfiAcpiTableProtocolGuid,
    TPL_CALLBACK,
    SndwDevTopologyInstallEventOnAcpiProtocol,
    NULL,
    &Registration
    );
  if (Event == NULL) {
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}
