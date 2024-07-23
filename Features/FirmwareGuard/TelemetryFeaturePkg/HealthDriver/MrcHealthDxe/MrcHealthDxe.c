/** @file
  This code install AIP protocol to report MRC health data to expose through ACPI PHAT for Telemetry.

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
#include <Bdat4.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AcpiTable.h>
#include <Library/AslUpdateLib.h>
#include <Library/DevicePathLib.h>
#include <TelemetryCommon.h>
#include <MemInfoHob.h>
#include <Library/HobLib.h>

///
/// Ensure proper structure formats
///
typedef struct {
  EFI_HANDLE                                  Handle;
  EFI_ADAPTER_INFORMATION_PROTOCOL            AdapterInfo;
  EFI_DEVICE_PATH_PROTOCOL                    *MrcDevicePath;
} MRC_HEALTH_DEV;

typedef struct {
  EFI_AIP_HEALTH_HEADER  Header;
  UINT8                  Revision;
  BDAT_STRUCTURE         BDAT;
} EFI_ADAPTER_INFO_MRC_HEALTH_STATE;

///
/// structure Revision (as defined in Telemetry Gen 2.0 spec.)
///
#define MRC_AIP_HEADER_REVISION  0x01
#define MRC_AIP_DATA_REVISION    0x01

/**
  This function gets the MRC health data.

  @param[in] This                      A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[in] InformationType           A pointer to gAdapterInfoMrcGuid that defines the contents of InformationBlock.
                                        The caller must use the InformationType to specify the infromation it needs to retrieve
                                        from this service and to determine how to parse the InformationBlock.
                                        The driver should not attempt to free InformationType.
  @param[out] InformationBlock         A pointer to the number of GUIDs present in InfoTypesBuffer.
  @param[out] InformationBlockSize     The driver return the size of the InformationBlock.

  @retval EFI_INVALID_PARAMETER
  @retval EFI_OUT_OF_RESOURCES
  @retval EFI_UNSUPPORTED
  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
MrcHealthDataAipGetInfo  (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  OUT VOID                              **InformationBlock,
  OUT UINTN                             *InformationBlockSize
)
{
  EFI_STATUS                          Status;
  EFI_ACPI_TABLE_PROTOCOL             *AcpiTable;
  UINTN                               Handle;
  UINTN                               BlockSize;
  EFI_ADAPTER_INFO_MRC_HEALTH_STATE   *HealthDataBlockPtr;
  EFI_ACPI_DESCRIPTION_HEADER         *Table;
  VOID                                *BdatBuffer;
  EFI_BDAT_ACPI_DESCRIPTION_TABLE     *BdatAcpiTable;
  UINTN                               Index;
  UINT8                               IsHealthy;
  MEMORY_PLATFORM_DATA_HOB            *MemPlatformDataHob;
  BOOLEAN                             IsBdatFound;

  if ((This == NULL) || (InformationBlock == NULL) || (InformationBlockSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (!CompareGuid (InformationType, &gAdapterInfoMrcGuid)) {
    return EFI_UNSUPPORTED;
  }

  *InformationBlockSize = 0;
  *InformationBlock     = NULL;
  IsHealthy             = TelemetryUnknown;
  IsBdatFound           = FALSE;

  BlockSize = sizeof (EFI_ADAPTER_INFO_MRC_HEALTH_STATE);
  HealthDataBlockPtr = (EFI_ADAPTER_INFO_MRC_HEALTH_STATE *) AllocateZeroPool (BlockSize);
  if (HealthDataBlockPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "MrcHealthDataAipGetInfo : Out of resource to allocate for EFI_ADAPTER_INFO_MRC_HEALTH_STATE.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Getting health Information ...
  //

  //
  // #1. Try to locate ACPI BDAT which is supposed to be installed after MRC.
  //     If BDAT is found, report it.
  AcpiTable = NULL;
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  if (EFI_ERROR (Status) || (AcpiTable == NULL)) {
    DEBUG ((DEBUG_ERROR, "MrcHealthDataAipGetInfo : locate gEfiAcpiTableProtocolGuid failure \n"));
  }
  Table = NULL;
  Handle = 0;
  Status = LocateAcpiTableBySignature (
             EFI_BDAT_TABLE_SIGNATURE,
             &Table,
             &Handle
             );
  if (EFI_ERROR (Status) || (Table == NULL)) {
    DEBUG ((DEBUG_ERROR, "MrcHealthDataAipGetInfo : BDAT is not found \n"));
  } else {
    IsBdatFound = TRUE;
    BdatAcpiTable = (EFI_BDAT_ACPI_DESCRIPTION_TABLE *) Table;
    BdatBuffer = (VOID *) BdatAcpiTable->BdatGas.Address;
    DEBUG ((DEBUG_INFO, "MrcHealthDataAipGetInfo : BDAT found @ Address %x \n", BdatBuffer));
    CopyMem (&(HealthDataBlockPtr->BDAT), BdatBuffer, sizeof (BDAT_STRUCTURE));
  }

  //
  // Storing MRC Health Data ...
  //
  HealthDataBlockPtr->Revision = MRC_AIP_DATA_REVISION;
  MemPlatformDataHob = GetFirstGuidHob (&gSiMemoryPlatformDataGuid);
  ASSERT (MemPlatformDataHob != NULL);
  if (MemPlatformDataHob != NULL) {
    if (MemPlatformDataHob->Data.MrcBasicMemoryTestPass == FALSE) {
      IsHealthy = TelemetryAdvisory;
    } else {
      IsHealthy = TelemetryHealthy;
    }
  }

  //
  //  Create the data header
  //
  HealthDataBlockPtr->Header.Length     = sizeof (EFI_ADAPTER_INFO_MRC_HEALTH_STATE);
  HealthDataBlockPtr->Header.Revision   = MRC_AIP_HEADER_REVISION;
  HealthDataBlockPtr->Header.Healthy    = IsHealthy;
  CopyGuid (&HealthDataBlockPtr->Header.Signature, &gAdapterInfoMrcGuid);

  //
  // Print MRC health data block
  //
  DEBUG ((DEBUG_INFO, "=== EFI_ADAPTER_INFO_MRC_HEALTH_STATE ==\n"));
  DEBUG ((DEBUG_INFO, " EFI_AIP_HEALTH_HEADER \n"));
  DEBUG ((DEBUG_INFO, "     Length    = 0x%04x \n",      HealthDataBlockPtr->Header.Length));
  DEBUG ((DEBUG_INFO, "     Revision  = 0x%02x \n",      HealthDataBlockPtr->Header.Revision));
  DEBUG ((DEBUG_INFO, "     Healthy   = 0x%02x \n",      HealthDataBlockPtr->Header.Healthy));
  DEBUG ((DEBUG_INFO, "     Signature = %g     \n",      HealthDataBlockPtr->Header.Signature));
  DEBUG ((DEBUG_INFO, " Revision          = 0x%02x \n",  HealthDataBlockPtr->Revision));
  if (IsBdatFound) {
    DEBUG ((DEBUG_INFO, " BDAT Table:\n"));
    DEBUG ((DEBUG_INFO, "==================================\n"));
    DEBUG ((DEBUG_INFO, " BDAT_STRUCTURE data\n"));
    DEBUG ((DEBUG_INFO, " BDAT_HEADER_STRUCTURE \n"));
    DEBUG ((DEBUG_INFO, "        BiosDataSignature  =  "));
    for (Index = 0; Index < sizeof (HealthDataBlockPtr->BDAT.Header.BiosDataSignature); Index++) {
      DEBUG ((DEBUG_INFO, " 0x%02x", HealthDataBlockPtr->BDAT.Header.BiosDataSignature[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
    DEBUG ((DEBUG_INFO, "        BiosDataStructureSize  =  0x%08x\n", HealthDataBlockPtr->BDAT.Header.BiosDataStructSize));
    DEBUG ((DEBUG_INFO, "        Crc16                  =  0x%04x\n", HealthDataBlockPtr->BDAT.Header.Crc16));
    DEBUG ((DEBUG_INFO, "        Reserved               =  0x%04x\n", HealthDataBlockPtr->BDAT.Header.Reserved));
    DEBUG ((DEBUG_INFO, "        Version.Rev.Primary    =  0x%04x\n", HealthDataBlockPtr->BDAT.Header.Version.Rev.Primary));
    DEBUG ((DEBUG_INFO, "        Version.Rev.Secondary  =  0x%04x\n", HealthDataBlockPtr->BDAT.Header.Version.Rev.Secondary));
    DEBUG ((DEBUG_INFO, "        Version.Data32         =  0x%08x\n", HealthDataBlockPtr->BDAT.Header.Version.Data32));
    DEBUG ((DEBUG_INFO, "        OemOffset              =  0x%08x\n", HealthDataBlockPtr->BDAT.Header.OemOffset));
    DEBUG ((DEBUG_INFO, "        Reserved1              =  0x%08x\n", HealthDataBlockPtr->BDAT.Header.Reserved1));
    DEBUG ((DEBUG_INFO, "        Reserved2              =  0x%08x\n", HealthDataBlockPtr->BDAT.Header.Reserved2));
    DEBUG ((DEBUG_INFO, "  BDAT_SCHEMA_LIST_STRUCTURE \n"));
    DEBUG ((DEBUG_INFO, "        SchemaListLength       =  0x%04x\n", HealthDataBlockPtr->BDAT.Schemas.SchemaListLength));
    DEBUG ((DEBUG_INFO, "        Reserved1              =  0x%04x\n", HealthDataBlockPtr->BDAT.Schemas.Reserved1));
    DEBUG ((DEBUG_INFO, "        Time.Year              =  0x%04x\n", HealthDataBlockPtr->BDAT.Schemas.Time.Year));
    DEBUG ((DEBUG_INFO, "        Time.Month             =  0x%02x\n", HealthDataBlockPtr->BDAT.Schemas.Time.Month));
    DEBUG ((DEBUG_INFO, "        Time.Day               =  0x%02x\n", HealthDataBlockPtr->BDAT.Schemas.Time.Day));
    DEBUG ((DEBUG_INFO, "        Time.Hour              =  0x%02x\n", HealthDataBlockPtr->BDAT.Schemas.Time.Hour));
    DEBUG ((DEBUG_INFO, "        Time.Minute            =  0x%02x\n", HealthDataBlockPtr->BDAT.Schemas.Time.Minute));
    DEBUG ((DEBUG_INFO, "        Time.Second            =  0x%02x\n", HealthDataBlockPtr->BDAT.Schemas.Time.Second));
    DEBUG ((DEBUG_INFO, "        Time.Reserved2         =  0x%02x\n", HealthDataBlockPtr->BDAT.Schemas.Time.Reserved2));
  }
  *InformationBlock  = HealthDataBlockPtr;
  *InformationBlockSize = sizeof (EFI_ADAPTER_INFO_MRC_HEALTH_STATE);

  return EFI_SUCCESS;
}

/**
  This function gets a list of Information types for this instance of the protocol.

  @param[in] *This                   A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[out] **InfoTypesBuffer      A pointer to the array of InformationType GUID is supported by This.
                                     This buffer is allocated by this service, and it is the responsibility
                                     of the caller to free it after using it.
  @param[out] *InfoTypesBufferCount  A pointer to the number of GUIDs present in InfoTypesBuffer.
  @retval EFI_INVALID_PARAMETER
  @retval EFI_OUT_OF_RESOURCES
  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
MrcHealthDataGetSupportedTypes (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  OUT EFI_GUID                          **InfoTypesBuffer,
  OUT UINTN                             *InfoTypesBufferCount
  )
{
  if ((This == NULL) || (InfoTypesBuffer == NULL) || (InfoTypesBufferCount == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *InfoTypesBuffer = AllocateCopyPool (sizeof (gAdapterInfoMrcGuid), &gAdapterInfoMrcGuid);

  if (*InfoTypesBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *InfoTypesBufferCount = 1;

  return EFI_SUCCESS;
}

/**
  This function sets the MRC health data.

  @param[in] *This                      A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[in] EFI_GUID *InformationType  A pointer to gAdapterInfoMrcGuid that defines the contents of InformationBlock.
                                        The caller must use the InformationType to specify the infromation it needs to retrieve
                                        from this service and to determine how to parse the InformationBlock.
                                        The driver should not attempt to free InformationType.
  @param[in] VOID **InformationBlock    A poointer to the number of GUIDs present in InfoTypesBuffer.
  @retval EFI_WRITE_PROTECTED
  @retval EFI_INVALID_PARAMETER         This is NULL.
  @retval EFI_INVALID_PARAMETER         InformationBlock is NULL.
  @retval EFI_WRITE_PROTECTED           The InformationType cannot be modified using EFI_ADAPTER_INFO_SET_INFO().
**/
EFI_STATUS
EFIAPI
MrcHealthDataAipSetInfo (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  IN  VOID                              *InformationBlock,
  IN  UINTN                             InformationBlockSize
  )
{
  if ((This == NULL) || (InformationBlock == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (!CompareGuid (InformationType, &gAdapterInfoMrcGuid)) {
    return EFI_UNSUPPORTED;
  }
  return EFI_WRITE_PROTECTED;;
}

/**
  This function installs EFI_ADAPTER_INFORMATION_PROTOCOL for MRC Health Data.
  The data is consumed by Telemetry data collector.

  @retval EFI_SUCCESS           The MRC Health data is successfully published in AIP protocol.
  @retval EFI_OUT_OF_RESOURCES  Out of resources to allocate EFI_ADAPTER_INFORMATION_PROTOCOL.
**/
EFI_STATUS
EFIAPI
MrcHealthDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                       Status;
  MRC_HEALTH_DEV                   *MrcHealthPrivate;
  VENDOR_DEVICE_PATH               VendorDeviceNode;
  EFI_DEVICE_PATH_PROTOCOL         *DevicePath;


  Status                      = EFI_SUCCESS;
  DevicePath                  = NULL;

  DEBUG ((DEBUG_INFO, "InitializeMrcHealthData Begins\n"));
  MrcHealthPrivate = (MRC_HEALTH_DEV *) AllocateZeroPool (sizeof (MRC_HEALTH_DEV));
  if (MrcHealthPrivate == NULL) {
    DEBUG ((EFI_D_ERROR, "EFI_OUT_OF_RESOURCES to Allocate MRC_HEALTH_DEV.\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  MrcHealthPrivate->Handle    = NULL;

  MrcHealthPrivate->AdapterInfo.GetInformation    = MrcHealthDataAipGetInfo;
  MrcHealthPrivate->AdapterInfo.SetInformation    = MrcHealthDataAipSetInfo;
  MrcHealthPrivate->AdapterInfo.GetSupportedTypes = MrcHealthDataGetSupportedTypes;

  ZeroMem (&VendorDeviceNode, sizeof (VENDOR_DEVICE_PATH));
  VendorDeviceNode.Header.Type    = HARDWARE_DEVICE_PATH;
  VendorDeviceNode.Header.SubType = HW_VENDOR_DP;
  CopyGuid (&VendorDeviceNode.Guid, &gAdapterInfoMrcGuid);
  SetDevicePathNodeLength (&VendorDeviceNode.Header, sizeof (VENDOR_DEVICE_PATH));
  DevicePath = AppendDevicePathNode (NULL, (EFI_DEVICE_PATH_PROTOCOL*)&VendorDeviceNode);
  if (DevicePath == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ERROR;
  }

  MrcHealthPrivate->MrcDevicePath = DevicePath;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &MrcHealthPrivate->Handle,
                  &gEfiAdapterInformationProtocolGuid,
                  &MrcHealthPrivate->AdapterInfo,
                  &gEfiDevicePathProtocolGuid,
                  MrcHealthPrivate->MrcDevicePath,
                  NULL
                  );


  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "InitializeMrcHealthData: faild to install AIP protocol, %r.\n", Status));
    goto ERROR;
  }

  DEBUG ((DEBUG_ERROR, "InitializeMrcHealthData Ends\n"));

  return Status;

ERROR:
  //
  // On error, clean the Aip service context data, and free the memory allocated.
  //
  FreePool (MrcHealthPrivate);
  if (DevicePath != NULL) {
    FreePool (DevicePath);
  }

  return Status;
}