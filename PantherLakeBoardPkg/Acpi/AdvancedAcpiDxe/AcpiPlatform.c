/** @file
  Advanced ACPI Driver

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
#include <Base.h>
#include <Uefi.h>
#include <Library/HobLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Guid/HobList.h>
#include <Protocol/FirmwareVolume2.h>
#include <PlatformBoardId.h>
#include <IndustryStandard/AcpiAml.h>
#include <IndustryStandard/Pci30.h>
#include "AcpiPlatform.h"
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/GbeLib.h>
#include <Library/PchPciBdfLib.h>
#include <MeBiosPayloadHob.h>
#include <PlatformBoardConfig.h>
#include <Library/CpuPlatformLib.h>
#include <PlatformBoardType.h>
#include <PlatformInfo.h>
#include <Guid/S3MemoryVariable.h>
#include <Library/VariablePolicyHelperLib.h>
#if FixedPcdGetBool(PcdTcssSupport) == 1
#include <TcssDataHob.h>
#endif
#include <Library/Usb4PlatformHob.h>
#include <Library/IpuLib.h>
#include "GenSsdtLib.h"
#include <Library/NpuInfoLib.h>
#include <Library/CnviLib.h>
#include <CnvVfrSetupMenuHii.h>
#include <CnviConfig.h>
#include <Guid/FileInfo.h>
#include <Protocol/SimpleFileSystem.h>
#define SSDT_FILE_NAME L"\\ssdt.aml"
#include <VtioSetup.h>

#include <Library/VmdInfoLib.h>
#include <Protocol/PlatformNvsArea.h>
#include <UcsiNvs.h>
#include <UsbPortMapNvs.h>
#include <Library/UefiLib.h>
#include <Library/PcieHelperLib.h>
#include <Library/PciLib.h>
#include <MemInfoHob.h>
//
// Global variables
//
GLOBAL_REMOVE_IF_UNREFERENCED VOID                                      *mGfxNvsAddress;
GLOBAL_REMOVE_IF_UNREFERENCED ME_SETUP                                  mMeSetup;
GLOBAL_REMOVE_IF_UNREFERENCED PCH_SETUP                                 mPchSetup;
GLOBAL_REMOVE_IF_UNREFERENCED SETUP_DATA                                mSystemConfiguration;
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_NVS_AREA_PROTOCOL                mPlatformNvsAreaProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_GUID                                  mVtioFormSetGuid = VTIO_FORMSET_GUID;
GLOBAL_REMOVE_IF_UNREFERENCED UCSI_NVS_AREA_PROTOCOL                    *mUcsiNvsAreaProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED USB_PORT_MAP_NVS_AREA_PROTOCOL            *mUsbPortMapNvsAreaProtocol;

#if FixedPcdGetBool(PcdTcssSupport) == 1
GLOBAL_REMOVE_IF_UNREFERENCED TCSS_DATA_HOB                             *mTcssHob;
#endif
//
// Function implementations
//

/**
  Update Group Position for PCH USB2 Port, PCH USB3 Port, TCSS USB3 Port, DTBT USB3 Port
  @param[in]  UsbConnectorBoardConfig           The USB Connector Board Config
  @param[in]  Index                             The Index of USB Connector Board Config
  @param[in]  NumberOfUsbCConnectors            The Number of USBC Connectors
**/
VOID
UpdateGroupPosition (
  IN USB_CONNECTOR_BOARD_CONFIG   *UsbConnectorBoardConfig,
  IN UINT8                        Index,
  IN UINT8                        NumberOfUsbCConnectors
  )
{
  if (UsbConnectorBoardConfig != NULL) {
    mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortGroupPosition[UsbConnectorBoardConfig->Usb2PortNum] = (UINT8)(UsbConnectorBoardConfig->Usb2PortNum + (UsbConnectorBoardConfig->Usb2Controller << 5));
    if (UsbConnectorBoardConfig->Usb3Controller == PCH_USB3) {
      mUsbPortMapNvsAreaProtocol->Area->PchUsb3PortGroupPosition[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortGroupPosition[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == TCSS_USB3) {
      mPlatformNvsAreaProtocol.Area->TcssUsb3PortGroupPosition[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortGroupPosition[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == DTBT1_USB3) {
      mPlatformNvsAreaProtocol.Area->Dtbt1Usb3PortGroupPosition[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortGroupPosition[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == DTBT2_USB3) {
      mPlatformNvsAreaProtocol.Area->Dtbt2Usb3PortGroupPosition[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortGroupPosition[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (Index < NumberOfUsbCConnectors) {
      mUcsiNvsAreaProtocol->Area->ConnectorGroupPosition[Index] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortGroupPosition[UsbConnectorBoardConfig->Usb2PortNum];
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorBoardConfig is NULL!!\n", __FUNCTION__));
  }
}
/**
  Update Visibility for PCH USB2 Port, PCH USB3 Port, TCSS USB3 Port, DTBT USB3 Port
  @param[in]  UsbConnectorBoardConfig        The USB Connector Board Config
  @param[in]  Index                          The Index of USB Connector Board Config
  @param[in]  NumberOfUsbCConnectors         The Number of USBC Connectors
**/
VOID
UpdateVisibility (
  IN USB_CONNECTOR_BOARD_CONFIG   *UsbConnectorBoardConfig,
  IN UINT8                        Index,
  IN UINT8                        NumberOfUsbCConnectors
  )
{
  if (UsbConnectorBoardConfig != NULL) {
    mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortVisibility[UsbConnectorBoardConfig->Usb2PortNum] = (UINT8)(UsbConnectorBoardConfig->ConnectorVisibility);
    if (UsbConnectorBoardConfig->Usb3Controller == PCH_USB3) {
      mUsbPortMapNvsAreaProtocol->Area->PchUsb3PortVisibility[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortVisibility[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == TCSS_USB3) {
      mPlatformNvsAreaProtocol.Area->TcssUsb3PortVisibility[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortVisibility[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == DTBT1_USB3) {
      mPlatformNvsAreaProtocol.Area->Dtbt1Usb3PortVisibility[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortVisibility[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == DTBT2_USB3) {
      mPlatformNvsAreaProtocol.Area->Dtbt2Usb3PortVisibility[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortVisibility[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (Index < NumberOfUsbCConnectors) {
      mUcsiNvsAreaProtocol->Area->ConnectorVisibility[Index] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortVisibility[UsbConnectorBoardConfig->Usb2PortNum];
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorBoardConfig is NULL!!\n", __FUNCTION__));
  }
}
/**
  Update Connectable for PCH USB2 Port, PCH USB3 Port, TCSS USB3 Port, DTBT USB3 Port
  @param[in]  UsbConnectorBoardConfig         The USB Connector Board Config
  @param[in]  Index                           The Index of USB Connector Board Config
  @param[in]  NumberOfUsbCConnectors          The Number of USBC Connectors
**/
VOID
UpdateConnectable (
  IN USB_CONNECTOR_BOARD_CONFIG   *UsbConnectorBoardConfig,
  IN UINT8                        Index,
  IN UINT8                        NumberOfUsbCConnectors
  )
{
  if (UsbConnectorBoardConfig != NULL) {
    mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortConnectable[UsbConnectorBoardConfig->Usb2PortNum] = (UINT8)(UsbConnectorBoardConfig->ConnectorConnectable);
    if (UsbConnectorBoardConfig->Usb3Controller == PCH_USB3) {
      mUsbPortMapNvsAreaProtocol->Area->PchUsb3PortConnectable[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortConnectable[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == TCSS_USB3) {
      mPlatformNvsAreaProtocol.Area->TcssUsb3PortConnectable[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortConnectable[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == DTBT1_USB3) {
      mPlatformNvsAreaProtocol.Area->Dtbt1Usb3PortConnectable[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortConnectable[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == DTBT2_USB3) {
      mPlatformNvsAreaProtocol.Area->Dtbt2Usb3PortConnectable[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortConnectable[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (Index < NumberOfUsbCConnectors) {
      mUcsiNvsAreaProtocol->Area->ConnectorConnectable[Index] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortConnectable[UsbConnectorBoardConfig->Usb2PortNum];
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorBoardConfig is NULL!!\n", __FUNCTION__));
  }
}
/**
  Update Type for PCH USB2 Port, PCH USB3 Port, TCSS USB3 Port, DTBT USB3 Port
  @param[in]  UsbConnectorBoardConfig    The USB Connector Board Config
  @param[in]  Index                      The Index of USB Connector Board Config
  @param[in]  NumberOfUsbCConnectors     The Number of USBC Connectors
**/
VOID
UpdateType (
  IN USB_CONNECTOR_BOARD_CONFIG   *UsbConnectorBoardConfig,
  IN UINT8                        Index,
  IN UINT8                        NumberOfUsbCConnectors
  )
{
  if (UsbConnectorBoardConfig != NULL) {
    mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortType[UsbConnectorBoardConfig->Usb2PortNum] = (UINT8)(UsbConnectorBoardConfig->ConnectorType);
    if (UsbConnectorBoardConfig->Usb3Controller == PCH_USB3) {
      mUsbPortMapNvsAreaProtocol->Area->PchUsb3PortType[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortType[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == TCSS_USB3) {
      mPlatformNvsAreaProtocol.Area->TcssUsb3PortType[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortType[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == DTBT1_USB3) {
      mPlatformNvsAreaProtocol.Area->Dtbt1Usb3PortType[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortType[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == DTBT2_USB3) {
      mPlatformNvsAreaProtocol.Area->Dtbt2Usb3PortType[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortType[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (Index < NumberOfUsbCConnectors) {
      mUcsiNvsAreaProtocol->Area->ConnectorType[Index] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortType[UsbConnectorBoardConfig->Usb2PortNum];
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorBoardConfig is NULL!!\n", __FUNCTION__));
  }
}
/**
  Update Capability for PCH USB2 Port, PCH USB3 Port, TCSS USB3 Port, DTBT USB3 Port
  @param[in]  UsbConnectorBoardConfig        The USB Connector Board Config
  @param[in]  UsbCConnectorBoardConfig       The USBC Connector Board Config
  @param[in]  Index                          The Index of USB Connector Board Config
**/
VOID
UpdateCapability (
  IN USB_CONNECTOR_BOARD_CONFIG    *UsbConnectorBoardConfig,
  IN USBC_CONNECTOR_BOARD_CONFIG   *UsbCConnectorBoardConfig,
  IN UINT8                         Index
  )
{
  if (UsbConnectorBoardConfig != NULL && UsbCConnectorBoardConfig != NULL) {
    mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortCapability[UsbConnectorBoardConfig->Usb2PortNum] = (UINT8)(UsbCConnectorBoardConfig->RetimerCount
                                                                                                  + (UsbCConnectorBoardConfig->PcieSupported << 2)
                                                                                                  + (UsbCConnectorBoardConfig->DpAltModeSupported << 3)
                                                                                                  + (UsbCConnectorBoardConfig->Usb4Supported << 4)
                                                                                                  + (UsbCConnectorBoardConfig->Tbt3Supported << 5));

    if (UsbConnectorBoardConfig->Usb3Controller == PCH_USB3) {
      mUsbPortMapNvsAreaProtocol->Area->PchUsb3PortCapability[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortCapability[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == TCSS_USB3) {
      mPlatformNvsAreaProtocol.Area->TcssUsb3PortCapability[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortCapability[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == DTBT1_USB3) {
      mPlatformNvsAreaProtocol.Area->Dtbt1Usb3PortCapability[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortCapability[UsbConnectorBoardConfig->Usb2PortNum];
    }
    if (UsbConnectorBoardConfig->Usb3Controller == DTBT2_USB3) {
      mPlatformNvsAreaProtocol.Area->Dtbt2Usb3PortCapability[UsbConnectorBoardConfig->Usb3PortNum] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortCapability[UsbConnectorBoardConfig->Usb2PortNum];
    }
    mUcsiNvsAreaProtocol->Area->ConnectorCapability[Index] = mUsbPortMapNvsAreaProtocol->Area->PchUsb2PortCapability[UsbConnectorBoardConfig->Usb2PortNum];
  } else {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorBoardConfig or UsbCConnectorBoardConfig is NULL!!\n", __FUNCTION__));
  }
}

/**
  Update USB related ACPI NVS
**/
EFI_STATUS
UpdateUsbAcpiNvs (
  VOID
  )
{
  UINT8                         EntryNum;
  USB_CONNECTOR_BOARD_CONFIG    *UsbConnectorBoardConfig;
  USBC_CONNECTOR_BOARD_CONFIG   *UsbCConnectorBoardConfig;
  USB_CONNECTOR_HOB_DATA        *UsbConnectorHobDataPtr;
  USBC_CONNECTOR_HOB_DATA       *UsbCConnectorHobDataPtr;
  EFI_HOB_GUID_TYPE             *GuidHob;
  UINT8                         NumberOfUsbConnectors;
  UINT8                         NumberOfUsbCConnectors;

  //
  // Get USB and USBC Connector Tables
  //
  GuidHob = GetFirstGuidHob (&gUsbConnectorHobGuid);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorHob Not Found!!\n", __FUNCTION__));
    return EFI_NOT_FOUND ;
  }
  UsbConnectorHobDataPtr = GET_GUID_HOB_DATA (GuidHob);
  GuidHob = GetFirstGuidHob (&gUsbCConnectorHobGuid);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: UsbCConnectorHob Not Found!!\n", __FUNCTION__));
    return EFI_NOT_FOUND ;
  }

  UsbCConnectorHobDataPtr = GET_GUID_HOB_DATA (GuidHob);
  UsbConnectorBoardConfig = UsbConnectorHobDataPtr->UsbConnectorBoardConfig;
  UsbCConnectorBoardConfig = UsbCConnectorHobDataPtr->UsbCConnectorBoardConfig;

  //
  // Update Connectable Type C ports map
  //
  mUcsiNvsAreaProtocol->Area->TypeCConnectorsMap = UsbCConnectorHobDataPtr->ConnectableUsbCConnectorsMap;
  DEBUG ((DEBUG_INFO, "TypeCConnectorsMap = 0x%04x\n", mUcsiNvsAreaProtocol->Area->TypeCConnectorsMap));

  //
  // Get Board Config Table Entry Count
  //
  NumberOfUsbConnectors = UsbConnectorHobDataPtr->NumberOfUsbConnectors;
  NumberOfUsbCConnectors = UsbCConnectorHobDataPtr->NumberOfUsbCConnectors;
  //
  // Update GroupPosition, Visibility, Connectable, Type and Capability
  //
  for(EntryNum = 0; EntryNum < NumberOfUsbConnectors; EntryNum ++){
    if (UsbConnectorBoardConfig[EntryNum].ConnectorConnectable == CONNECTABLE) {
      UpdateGroupPosition (&UsbConnectorBoardConfig[EntryNum], EntryNum, NumberOfUsbCConnectors);
      UpdateVisibility (&UsbConnectorBoardConfig[EntryNum], EntryNum, NumberOfUsbCConnectors);
      UpdateConnectable (&UsbConnectorBoardConfig[EntryNum], EntryNum, NumberOfUsbCConnectors);
      UpdateType (&UsbConnectorBoardConfig[EntryNum], EntryNum, NumberOfUsbCConnectors);
      if(EntryNum < NumberOfUsbCConnectors){
        UpdateCapability (&UsbConnectorBoardConfig[EntryNum], &UsbCConnectorBoardConfig[EntryNum], EntryNum);
      }
    }
  }
  return EFI_SUCCESS;
}

/**
  Get UCSI NVS Protocol and Update ACPI NVS

  @param[in] Event    The Event this notify function registered to.
  @param[in] Context  Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
UcsiEventCallBack (
  EFI_EVENT Event,
  VOID      *Context
  )
{
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (
                  &gUcsiNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &mUcsiNvsAreaProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  if (mUcsiNvsAreaProtocol != NULL && mUsbPortMapNvsAreaProtocol != NULL) {
    UpdateUsbAcpiNvs ();
  }
}

/**
  Get USB Port Map NVS Protocol and Update ACPI NVS

  @param[in] Event    The Event this notify function registered to.
  @param[in] Context  Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
UsbPortMapEventCallBack (
  EFI_EVENT Event,
  VOID      *Context
  )
{
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (
                  &gUsbPortMapNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &mUsbPortMapNvsAreaProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  if (mUcsiNvsAreaProtocol != NULL && mUsbPortMapNvsAreaProtocol != NULL) {
    UpdateUsbAcpiNvs ();
  }
}

/**
  Install Txt ACPI Table
**/
VOID
InstallTxtAcpiTable (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  BOOLEAN                       LoadTable;
  UINTN                         NumberOfHandles;
  UINTN                         Index;
  INTN                          Instance;
  UINTN                         Size;
  UINT32                        FvStatus;
  UINTN                         TableHandle;
  EFI_FV_FILETYPE               FileType;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  EFI_ACPI_COMMON_HEADER        *Table;
  UINT64                        Signature;

  FwVol         = NULL;
  Table         = NULL;
  Signature     = SIGNATURE_64 ('T', 'x', 't', 'S', 's', 'd', 't', 0);

  //
  // Locate FV protocol.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Look for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &FwVol
                    );
    ASSERT_EFI_ERROR (Status);
    if (FwVol == NULL) {
      FreePool (HandleBuffer);
      return;
    }
    //
    // See if it has the ACPI storage file
    //
    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &gTxtAcpiTableStorageGuid,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  //
  // Free any allocated buffers
  //
  FreePool (HandleBuffer);

  //
  // Sanity check that we found our data file
  //
  ASSERT (FwVol);

  //
  // Locate ACPI tables
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);

  //
  // Read tables from the storage file.
  //
  if (FwVol == NULL) {
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    return;
  }
  Instance = 0;

  while (Status == EFI_SUCCESS) {
    //
    // Read the ACPI tables
    //
    Status = FwVol->ReadSection (
                      FwVol,
                      &gTxtAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &Table,
                      &Size,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      LoadTable   = FALSE;
      TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;
      if (TableHeader == NULL)
        continue;

      if (((EFI_ACPI_DESCRIPTION_HEADER *) TableHeader)->OemTableId == Signature) {
        //
        // This is the SSDT table that match the Signature
        //
        LoadTable = TRUE;
      }

      //
      // Add the table
      //
      if (LoadTable) {
        TableHandle = 0;
        Status = AcpiTable->InstallAcpiTable (
                              AcpiTable,
                              TableHeader,
                              TableHeader->Length,
                              &TableHandle
                              );
      }
      //
      // Increment the instance
      //
      Instance++;
      if (TableHeader != NULL) {
        gBS->FreePool (TableHeader);
        Table = NULL;
      }
    }
  }
}


/**
  Install Soc ACPI Table

  @param[in] *SsdtTableGuid      Pointer to GUID that identifies new SSDT table
  @param[in] Signature           Signature of the table

**/
VOID
STATIC
InstallSocAcpiTable (
  IN EFI_GUID  *SsdtTableGuid,
  IN UINT64    Signature
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  UINTN                         Index;
  INTN                          Instance;
  UINTN                         Size;
  UINT32                        FvStatus;
  UINTN                         TableHandle;
  EFI_FV_FILETYPE               FileType;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  EFI_ACPI_COMMON_HEADER        *Table;

  FwVol         = NULL;
  Table         = NULL;

  //
  // Locate FV protocol.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Look for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &FwVol
                    );
    ASSERT_EFI_ERROR (Status);
    if (FwVol == NULL) {
      FreePool (HandleBuffer);
      return;
    }
    //
    // See if it has the ACPI storage file
    //
    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      SsdtTableGuid,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  //
  // Free any allocated buffers
  //
  FreePool (HandleBuffer);

  //
  // Sanity check that we found our data file
  //
  ASSERT (FwVol);

  //
  // Locate ACPI tables
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);

  //
  // Read tables from the storage file.
  //
  if (FwVol == NULL) {
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    return;
  }
  Instance = 0;

  while (Status == EFI_SUCCESS) {
    //
    // Read the ACPI tables
    //
    Status = FwVol->ReadSection (
                      FwVol,
                      SsdtTableGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &Table,
                      &Size,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

      if (((EFI_ACPI_DESCRIPTION_HEADER *) TableHeader)->OemTableId == Signature) {
        TableHandle = 0;
        //
        // Add the table
        //
        Status = AcpiTable->InstallAcpiTable (
                              AcpiTable,
                              TableHeader,
                              TableHeader->Length,
                              &TableHandle
                              );
      }

      //
      // Increment the instance
      //
      Instance++;
      gBS->FreePool (TableHeader);
      Table = NULL;
    }
  }
}

VOID
PublishOverClockingAcpiTable (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  UINTN                         Index;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_ACPI_COMMON_HEADER        *Table;
  UINTN                         Size;
  UINT32                        FvStatus;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  UINTN                         TableHandle;

  FwVol = NULL;
  Table = NULL;

  DEBUG ((DEBUG_INFO, "PublishOverClockingAcpiTable\n"));

  //
  // Locate FV protocol.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Look for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {

    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                  HandleBuffer[Index],
                  &gEfiFirmwareVolume2ProtocolGuid,
                  (VOID **) &FwVol
                  );
    ASSERT_EFI_ERROR (Status);
    if (FwVol == NULL) {
      FreePool (HandleBuffer);
      return;
    }

    //
    // See if it has the OverClocking Acpi Table file
    //
    Status = FwVol->ReadSection (
                      FwVol,
                      &gOcAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      0,
                      (VOID **) &Table,
                      &Size,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  //
  // Free any allocated buffers
  //
  FreePool (HandleBuffer);

  //
  // Not find the table, it's fine the over clocking support is not enabled
  //
  if (Table == NULL) {
    return;
  }

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);

  TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;
  if (CompareMem (&TableHeader->OemTableId, "PerfTune", 8) == 0) {
    TableHandle = 0;
    Status = AcpiTable->InstallAcpiTable (
                          AcpiTable,
                          Table,
                          Table->Length,
                          &TableHandle
                          );
  }

  if (Table != NULL) {
    gBS->FreePool (Table);
    Table = NULL;
  }

}

/**
  Locate the first instance of a protocol.  If the protocol requested is an
  FV protocol, then it will return the first FV that contains the ACPI table
  storage file.

  @param[in] Protocol           The protocol to find.
  @param[in] Instance           Return pointer to the first instance of the protocol.
  @param[in] Type               TRUE if the desired protocol is a FV protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_NOT_FOUND         The protocol could not be located.
  @retval EFI_OUT_OF_RESOURCES  There are not enough resources to find the protocol.
**/
EFI_STATUS
LocateSupportProtocol (
  IN     EFI_GUID                      *Protocol,
  IN     EFI_GUID                      gEfiAcpiMultiTableStorageGuid,
     OUT VOID                          **Instance,
  IN     BOOLEAN                       Type
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              *HandleBuffer;
  UINTN                   NumberOfHandles;
  EFI_FV_FILETYPE         FileType;
  UINT32                  FvStatus;
  EFI_FV_FILE_ATTRIBUTES  Attributes;
  UINTN                   Size;
  UINTN                   Index;

  //
  // Locate protocol.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  Protocol,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    //
    // Defined errors at this time are not found and out of resources.
    //
    return Status;
  }

  //
  // Looking for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {

    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    Protocol,
                    Instance
                    );
    ASSERT_EFI_ERROR (Status);

    if (!Type) {

      //
      // Not looking for the FV protocol, so find the first instance of the
      // protocol.  There should not be any errors because our handle buffer
      // should always contain at least one or LocateHandleBuffer would have
      // returned not found.
      //
      break;
    }

    //
    // See if it has the ACPI storage file
    //
    Size      = 0;
    FvStatus  = 0;
    Status = ((EFI_FIRMWARE_VOLUME2_PROTOCOL *) (*Instance))->ReadFile (
                                                              *Instance,
                                                              &gEfiAcpiMultiTableStorageGuid,
                                                              NULL,
                                                              &Size,
                                                              &FileType,
                                                              &Attributes,
                                                              &FvStatus
                                                              );

    //
    // If we found it, then we are done
    //
    if (Status == EFI_SUCCESS) {
      break;
    }
  }

  //
  // Our exit status is determined by the success of the previous operations
  // If the protocol was found, Instance already points to it.
  //
  //
  // Free any allocated buffers
  //
  FreePool (HandleBuffer);

  return Status;
}

/*
  This function will determine whether Native ASPM is supported on the platform.

  @param[in] SystemConfiguration     System configuration data

  @return TRUE if Native ASPM is supported, FALSE otherwise
*/
BOOLEAN
IsNativeAspmSupported (
  CONST SETUP_DATA* SystemConfiguration
  )
{
  if (SystemConfiguration->PciExpNative == 0) {
    return FALSE;
  }
  switch (SystemConfiguration->NativeAspmEnable) {
  case 0:
    return FALSE;
  case 1:
  case 2: /* AUTO */
    return TRUE;
  default:
    ASSERT (0);
  }
  return TRUE;
}

/**
  This function fixes up all FADT following table values if configuration requires it.

  @retval EFI_SUCCESS             The function returned successfully.
  @retval Other                   Some error occurs when executing this function.

**/
EFI_STATUS
UpdateFadtSetting (
  VOID
  )
{
  EFI_STATUS             Status;
  UINT32                 FadtFlags;
  UINT64                 SleepControlRegAddress;

  FadtFlags              = EFI_ACPI_FIXED_FEATURE_FLAGS;
  SleepControlRegAddress = 0;

  DEBUG((DEBUG_INFO, "%a() - Start\n", __FUNCTION__));
  //
  // Modify MSI capability based on setup
  //
  if (mSystemConfiguration.EnableMsiInFadt == 0) {
    Status = PcdSet16S (PcdFadtIaPcBootArch, EFI_ACPI_IAPC_BOOT_ARCH | (1 << 3));
    ASSERT_EFI_ERROR (Status);
  }

  //
  // if Native ASPM is disabled, set FACP table to skip Native ASPM
  //
  if (!IsNativeAspmSupported (&mSystemConfiguration)) {
    Status = PcdSet16S (PcdFadtIaPcBootArch, EFI_ACPI_IAPC_BOOT_ARCH | 0x10);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // PME WAKE supported, set PCI_EXP_WAK, BIT14 of Fixed feature flags.
  //
  FadtFlags |= (EFI_ACPI_6_5_PCI_EXP_WAK);

  if (mSystemConfiguration.LowPowerS0Idle) {
    //
    // The Flags field within the FADT (offset 112)
    //   1) will have a new Low Power S0 Idle Capable ACPI flag (bit offset 21).
    //
    FadtFlags = (BIT21 | FadtFlags);
    //
    //Only passive docking available in Conected Standby mode. Clear Docking capability Bit
    //
    FadtFlags &= ~BIT9;
    //
    // set indicates the power button is handled as a control method device.
    //
    FadtFlags |= EFI_ACPI_6_5_PWR_BUTTON;
  } else {
    FadtFlags &= ~(EFI_ACPI_6_5_PWR_BUTTON); // clear indicates the power button is handled as a fixed feature programming model
  }

  Status = PcdSet32S (PcdFadtFlags, FadtFlags);
  ASSERT_EFI_ERROR (Status);

  // SLEEP_CONTROL_REG and SLEEP_STATUS_REG are optional
  // Only implement in HW-Reduced ACPI system
  // In non-HW-Reduced system will remain all 0 as default
  if ((FadtFlags & EFI_ACPI_6_5_HW_REDUCED_ACPI) != 0) {
    //
    // If the HW_REDUCED_ACPI flag in the table is set, OSPM will ignore fields related to the ACPI HW
    //
    Status = PcdSet8S (PcdAcpiSleepControlRegisterAddressSpaceId, 0x1);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet8S (PcdAcpiSleepControlRegisterBitWidth, 0x8);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet8S (PcdAcpiSleepControlRegisterBitOffset, 0);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet8S (PcdAcpiSleepControlRegisterAccessSize, EFI_ACPI_6_5_DWORD);
    ASSERT_EFI_ERROR (Status);
    SleepControlRegAddress = EFI_ACPI_PM1A_EVT_BLK_ADDRESS + 4;
    Status = PcdSet64S (PcdAcpiSleepControlRegisterAddress,  SleepControlRegAddress);
    ASSERT_EFI_ERROR (Status);

    Status = PcdSet8S (PcdAcpiSleepStatusRegisterAddressSpaceId, 0x1);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet8S (PcdAcpiSleepStatusRegisterBitWidth, 0x8);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet8S (PcdAcpiSleepStatusRegisterBitOffset, 0);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet8S (PcdAcpiSleepStatusRegisterAccessSize, EFI_ACPI_6_5_DWORD);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet64S (PcdAcpiSleepStatusRegisterAddress, EFI_ACPI_PM1A_EVT_BLK_ADDRESS);
    ASSERT_EFI_ERROR (Status);
  }

  /**
  @todo RtcS4WakeEnable and PmTimerEnable default value is 1. So, the following logic is not executed.
  //
  // Check if RTC S4 is enabled
  //
  if (!mSystemConfiguration.RtcS4WakeEnable) {

   //
   // Clear the S4 RTC flag
   //
   FadtFlags &= ~EFI_ACPI_6_5_RTC_S4;
  }

  //
  // Check if PM timer is enabled
  //
  if (!mSystemConfiguration.PmTimerEnable) {

   //
   // Clear the PM timer flag
   //
   FadtFlags &= ~EFI_ACPI_6_5_USE_PLATFORM_CLOCK;
  }
  **/

  DEBUG((DEBUG_INFO, "%a() - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  This function will update any runtime platform specific information.
  This currently includes:
    Setting OEM table values, ID, table ID, creator ID and creator revision.
    Enabling the proper processor entries in the APIC tables
  It also indicates with which ACPI table version the table belongs.

  @param[in] Table        The table to update
  @param[in] Version      Where to install this table

  @retval EFI_SUCCESS     Updated tables commplete.
**/
EFI_STATUS
PlatformUpdateTables (
  IN OUT EFI_ACPI_COMMON_HEADER       *Table,
  IN OUT EFI_ACPI_TABLE_VERSION       *Version
  )
{
  EFI_ACPI_DESCRIPTION_HEADER                 *TableHeader;
  UINT8                                       *CurrPtr;
  UINT8                                       *TmpDsdtPointer;
  EFI_STATUS                                  Status;

  UINT8                                       MaximumDsdtPointLength;

  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL             *PciRootBridgeIo;

  CurrPtr           = NULL;

  MaximumDsdtPointLength = 20;

  //
  // This will be accurate except for FACS structure
  //
  TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

  //
  // Update the OEM and creator information for every table except FACS.
  //
  if (Table->Signature != EFI_ACPI_6_5_FIRMWARE_ACPI_CONTROL_STRUCTURE_SIGNATURE) {
    if ((Table->Signature == EFI_ACPI_6_5_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) &&
        (CompareMem (&TableHeader->OemId, "Rtd3", 4) == 0)) {
      // For SsdtRtd3 table, "OemId" update is done later
    } else {
      CopyMem (TableHeader->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (TableHeader->OemId));
    }

    //
    // Skip OEM table ID and creator information for SSDT tables, since these are
    // created by an ASL compiler and the creator information is useful.
    //
    if (Table->Signature != EFI_ACPI_6_5_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
      //
      // Set through patchable PCD value
      //
      TableHeader->OemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);
      TableHeader->OemRevision = PcdGet32(PcdAcpiDefaultOemRevision);

      //
      // Update creator information
      //
       TableHeader->CreatorId        = PcdGet32 (PcdAcpiDefaultCreatorId);
       TableHeader->CreatorRevision  = PcdGet32 (PcdAcpiDefaultCreatorRevision) ;
    }
  }

  //
  // Locate the PCI IO protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiPciRootBridgeIoProtocolGuid,
                  NULL,
                  (VOID **) &PciRootBridgeIo
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // By default, a table belongs in all ACPI table versions published.
  // Some tables will override this because they have different versions of the table.
  //
  *Version = EFI_ACPI_TABLE_VERSION_1_0B | EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0;

  //
  // Update the various table types with the necessary updates
  //
  switch (Table->Signature) {

  case EFI_ACPI_6_5_PERSISTENT_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
    //
    // These items only belong in the 1.0 tables.
    //
    *Version = EFI_ACPI_TABLE_VERSION_1_0B;
    break;

  case EFI_ACPI_6_5_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
    //
    // Fix up the AML code in the DSDT affected by end user options.
    // Fix up the following ASL Code:
    // (1)  ACPI Global NVS Memory Base and Size.
    // (2)  ACPI Graphics NVS Memory Base and Size.
    // (3)  SMBus I/O Base.
    // (4)  Thermal Management Methods.
    //
    {
      UINT8   *DsdtPointer;
      UINT32  *Signature;
      UINT8   *Operation;
      UINT32  *Address;
      UINT8   *Value;
      UINT16  *Size;
      BOOLEAN EnterDock = FALSE;
      //
      // Loop through the ASL looking for values that we must fix up.
      //
      CurrPtr = (UINT8 *) TableHeader + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
      for (DsdtPointer = CurrPtr;
           DsdtPointer < ((UINT8 *) TableHeader + TableHeader->Length - sizeof (UINT32));
           DsdtPointer++
          ) {
        Signature = (UINT32 *) DsdtPointer;
        switch (*Signature) {
        //
        // GNVS operation region
        //
        case (SIGNATURE_32 ('G', 'N', 'V', 'S')):
          //
          // Conditional match.  For Region Objects, the Operator will always be the
          // byte immediately before the specific name.  Therefore, subtract 1 to check
          // the Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_EXT_REGION_OP) {
            Address   = (UINT32 *) (DsdtPointer + 6);
            *Address  = (UINT32) (UINTN) mPlatformNvsAreaProtocol.Area;
            Size      = (UINT16 *) (DsdtPointer + 11);
            *Size     = sizeof (PLATFORM_NVS_AREA);
          }
          break;

        //
        // _AC0 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '0')):
          //
          // Conditional match.  _AC0 is >63 and <4095 bytes, so the package length is 2 bytes.
          // Therefore, subtract 3 to check the Operator.
          //
          Operation = DsdtPointer - 3;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _AC0 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '0');
            }
          }
          break;

        //
        // _AL0 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '0')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL0 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '0');
            }
          }
          break;

        //
        // _AC1 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '1')):
          //
          // Conditional match.  _AC1 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC1 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '1');
            }
          }
          break;

        //
        // _AL1 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '1')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL1 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '1');
            }
          }
          break;

        //
        // _AC2 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '2')):
          //
          // Conditional match.  _AC2 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC2 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '2');
            }
          }
          break;

        //
        // _AL2 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '2')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL2 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '2');
            }
          }
          break;

        //
        // _AC3 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '3')):
          //
          // Conditional match.  _AC3 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC3 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '3');
            }
          }
          break;

        //
        // _AL3 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '3')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL3 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '3');
            }
          }
          break;

        //
        // _AC4 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '4')):
          //
          // Conditional match.  _AC4 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC4 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '4');
            }
          }
          break;

        //
        // _AL4 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '4')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL4 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '4');
            }
          }
          break;

        //
        // _AC5 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '5')):
          //
          // Conditional match.  _AC5 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC5 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '5');
            }
          }
          break;

        //
        // _AL5 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '5')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL5 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '5');
            }
          }
          break;

        //
        // _AC6 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '6')):
          //
          // Conditional match.  _AC6 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC6 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '6');
            }
          }
          break;

        //
        // _AL6 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '6')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL6 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '6');
            }
          }
          break;

        //
        // _AC7 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '7')):
          //
          // Conditional match.  _AC7 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC7 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '7');
            }
          }
          break;

        //
        // _AL7 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '7')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL7 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '7');
            }
          }
          break;

        //
        // _AC8 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '8')):
          //
          // Conditional match.  _AC8 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC8 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '8');
            }
          }
          break;

        //
        // _AL8 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '8')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL8 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '8');
            }
          }
          break;

        //
        // _AC9 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '9')):
          //
          // Conditional match.  _AC9 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC9 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '9');
            }
          }
          break;

        //
        // _AL9 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '9')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL9 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '9');
            }
          }
          break;

        //
        // _PSL method
        //
        case (SIGNATURE_32 ('_', 'P', 'S', 'L')):
          //
          // Conditional match.  _PSL is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 3;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _PSL enabled
            //
            if (mSystemConfiguration.DisablePassiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'P', 'S', 'L');
            }
          }
          break;

        //
        // _PSV method
        //
        case (SIGNATURE_32 ('_', 'P', 'S', 'V')):
          //
          // Conditional match.  _PSV is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 3;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _PSV enabled
            //
            if (mSystemConfiguration.DisablePassiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'P', 'S', 'V');
            }
          }
          break;

        //
        // _CRT method
        //
        case (SIGNATURE_32 ('_', 'C', 'R', 'T')):
          //
          // Conditional match.  _CRT is < 256 bytes, so the package length is 1 byte.
          // Subtract 3 to check the Operator for CRB, subract 2 for Harris Beach.
          //
          Operation = DsdtPointer - 3;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _CRT enabled
            //
            if (mSystemConfiguration.DisableCriticalTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'C', 'R', 'T');
            }
          }
          break;

        //
        // _TC1 method
        //
        case (SIGNATURE_32 ('_', 'T', 'C', '1')):
          //
          // Conditional match.  _TC1 is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _TC1 enabled
            //
            if (mSystemConfiguration.DisablePassiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'T', 'C', '1');
            }
          }
          break;

        //
        // _TC2 method
        //
        case (SIGNATURE_32 ('_', 'T', 'C', '2')):
          //
          // Conditional match.  _TC2 is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _TC2 enabled
            //
            if (mSystemConfiguration.DisablePassiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'T', 'C', '2');
            }
          }
          break;

        //
        // _TSP method
        //
        case (SIGNATURE_32 ('_', 'T', 'S', 'P')):
          //
          // Conditional match.  _TSP is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _TSP enabled
            //
            if (mSystemConfiguration.DisablePassiveTripPoints == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'T', 'S', 'P');
            }
          }
          break;

        //
        // Update SS3 Name with Setup value
        //
        case (SIGNATURE_32 ('S', 'S', '3', '_')):
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {
            if (mSystemConfiguration.LowPowerS0Idle == 0) {
              Value   = (UINT8 *) DsdtPointer + 4;
              *Value  = (UINT8) mSystemConfiguration.AcpiSleepState;
            } else {
              Value   = (UINT8 *) DsdtPointer + 4;
              *Value  = 0;
            }
          }
          break;
        //
        // Update SS4 Name with Setup value
        //
        case (SIGNATURE_32 ('S', 'S', '4', '_')):
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {
            Value   = (UINT8 *) DsdtPointer + 4;
            *Value  = mSystemConfiguration.AcpiHibernate;
          }
          break;
        //
        // _EJ0 method
        //
        case (SIGNATURE_32 ('_', 'E', 'J', '0')):
          if (mSystemConfiguration.LowPowerS0Idle) {
            //
            // Remove _EJ0 for SOC
            //
            if (*(DsdtPointer-3) == AML_METHOD_OP) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'E', 'J', '0');
              EnterDock = TRUE;
            }
          }
          break;
        //
        // _STA method for Device (\_SB.PC00.DOCK)
        //
        case (SIGNATURE_32 ('_', 'S', 'T', 'A')):
          if (mSystemConfiguration.LowPowerS0Idle) {
            //
            // Remove _STA in (\_SB.PC00.DOCK) for SOC
            //
            if ((*(DsdtPointer-3) == AML_METHOD_OP) && (EnterDock)) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'S', 'T', 'A');
              EnterDock = FALSE;
            }
          }
          break;

        //
        // _DCK method
        //
        case (SIGNATURE_32 ('_', 'D', 'C', 'K')):
          if (mSystemConfiguration.LowPowerS0Idle) {
            //
            // Remove _DCK for SOC
            //
            if (*(DsdtPointer-3) == AML_METHOD_OP) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'D', 'C', 'K');
            }
          }
          break;

        //
        // mask _DEP from CPU's scope if CS disabled.
        //
        case (SIGNATURE_32 ('P', 'R', '0', '0')):
        case (SIGNATURE_32 ('P', 'R', '0', '1')):
        case (SIGNATURE_32 ('P', 'R', '0', '2')):
        case (SIGNATURE_32 ('P', 'R', '0', '3')):
        case (SIGNATURE_32 ('P', 'R', '0', '4')):
        case (SIGNATURE_32 ('P', 'R', '0', '5')):
        case (SIGNATURE_32 ('P', 'R', '0', '6')):
        case (SIGNATURE_32 ('P', 'R', '0', '7')):
        case (SIGNATURE_32 ('P', 'R', '0', '8')):
        case (SIGNATURE_32 ('P', 'R', '0', '9')):
        case (SIGNATURE_32 ('P', 'R', '1', '0')):
        case (SIGNATURE_32 ('P', 'R', '1', '1')):
        case (SIGNATURE_32 ('P', 'R', '1', '2')):
        case (SIGNATURE_32 ('P', 'R', '1', '3')):
        case (SIGNATURE_32 ('P', 'R', '1', '4')):
        case (SIGNATURE_32 ('P', 'R', '1', '5')):
          if (mSystemConfiguration.LowPowerS0Idle == 0) {
            for (TmpDsdtPointer = DsdtPointer; TmpDsdtPointer <= DsdtPointer + MaximumDsdtPointLength; TmpDsdtPointer++){
              Signature = (UINT32 *) TmpDsdtPointer;
              switch (*Signature) {
                case(SIGNATURE_32('_', 'D', 'E', 'P')):
                  *(UINT8 *) TmpDsdtPointer = 'X';
                  break;
                default:
                  //
                  // Do nothing.
                  //
                  break;
              }
            }
          }
          break;

        //
        // _EDL name
        //
        case (SIGNATURE_32 ('_', 'E', 'D', 'L')):
          if (mSystemConfiguration.LowPowerS0Idle) {
            //
            // Remove _EDL for SOC
            //
            if (*(DsdtPointer-1) == AML_NAME_OP) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'E', 'D', 'L');
            }
          }
          break;

        default:
          //
          // Do nothing.
          //
          break;
        }
      }
    }
    break;

/* @todo Need to uncomment once MSFT fully implements uPEP
  case SIGNATURE_32 ('L', 'P', 'I', 'T'):
    //
    // If L0s is disabled in setup, don't publish the table.
    //
    if (mSystemConfiguration.LowPowerS0Idle == 0) {
      *Version = EFI_ACPI_TABLE_VERSION_NONE;
    }
    break;
*/
  case EFI_ACPI_6_5_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
  {
    UINT8   *TablePointer;
    UINT32  *Signature;
    UINT8   *Operation;

    //
    // Do not load the xHCI table. It is handled by separate function.
    //
    if (CompareMem (&TableHeader->OemTableId, "xh_", 3) == 0) {
      DEBUG((DEBUG_INFO,"TableHeader->OemTableId = %x\n ", TableHeader->OemTableId));
      *Version = EFI_ACPI_TABLE_VERSION_NONE;
    }

    //
    // Load SSDT tables for the platforms based on boardID; default to RVP tables.
    // Load Ther_Rvp for all others.
    //
    if (CompareMem (&TableHeader->OemTableId, "Ther_Rvp", 8) == 0) {
      CurrPtr = (UINT8 *) Table + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
      for (TablePointer = CurrPtr;TablePointer < ((UINT8 *) Table + Table->Length - sizeof (UINT32));TablePointer++)
      {
        Signature = (UINT32 *) TablePointer;
        switch (*Signature) {
        //
        // _AC0 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '0')):
          //
          // Conditional match.  _AC0 is >63 and <4095 bytes, so the package length is 2 bytes.
          // Therefore, subtract 3 to check the Operator.
          //
          Operation = TablePointer - 3;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _AC0 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '0');
            }
          }
          break;
        //
        // _AL0 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '0')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = TablePointer - 1;
          if (*Operation == AML_NAME_OP) {
            //
            // Check if we want _AL0 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '0');
            }
          }
          break;
        //
        // _AC1 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '1')):
          //
          // Conditional match.  _AC1 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = TablePointer - 2;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _AC1 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '1');
            }
          }
          break;
        //
        // _AL1 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '1')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = TablePointer - 1;
          if (*Operation == AML_NAME_OP) {
            //
            // Check if we want _AL1 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '1');
            }
          }
          break;
        //
        // _AC2 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '2')):
          //
          // Conditional match.  _AC2 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = TablePointer - 2;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _AC2 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '2');
            }
          }
          break;
        //
        // _AL2 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '2')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = TablePointer - 1;
          if (*Operation == AML_NAME_OP) {
            //
            // Check if we want _AL2 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '2');
            }
          }
          break;
        //
        // _AC3 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '3')):
          //
          // Conditional match.  _AC3 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = TablePointer - 2;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _AC3 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '3');
            }
          }
          break;
        //
        // _AL3 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '3')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = TablePointer - 1;
          if (*Operation == AML_NAME_OP) {
            //
            // Check if we want _AL3 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '3');
            }
          }
          break;
        //
        // _AC4 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '4')):
          //
          // Conditional match.  _AC4 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = TablePointer - 2;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _AC4 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '4');
            }
          }
          break;
        //
        // _AL4 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '4')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = TablePointer - 1;
          if (*Operation == AML_NAME_OP) {
            //
            // Check if we want _AL4 enabled
            //
            if (mSystemConfiguration.DisableActiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '4');
            }
          }
          break;
        // _PSL method
        //
        case (SIGNATURE_32 ('_', 'P', 'S', 'L')):
          //
          // Conditional match.  _PSL is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = TablePointer - 3;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _PSL enabled
            //
            if (mSystemConfiguration.DisablePassiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'P', 'S', 'L');
            }
          }
          break;
        //
        // _PSV method
        //
        case (SIGNATURE_32 ('_', 'P', 'S', 'V')):
          //
          // Conditional match.  _PSV is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = TablePointer - 3;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _PSV enabled
            //
            if (mSystemConfiguration.DisablePassiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'P', 'S', 'V');
            }
          }
          break;
        //
        // _CRT method
        //
        case (SIGNATURE_32 ('_', 'C', 'R', 'T')):
          //
          // Conditional match.  _CRT is < 256 bytes, so the package length is 1 byte.
          // Subtract 3 to check the Operator for CRB, subract 2 for Harris Beach.
          //
          Operation = TablePointer - 3;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _CRT enabled
            //
            if (mSystemConfiguration.DisableCriticalTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'C', 'R', 'T');
            }
          }
          break;
        //
        // _TC1 method
        //
        case (SIGNATURE_32 ('_', 'T', 'C', '1')):
          //
          // Conditional match.  _TC1 is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = TablePointer - 2;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _TC1 enabled
            //
            if (mSystemConfiguration.DisablePassiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'T', 'C', '1');
            }
          }
          break;
        //
        // _TC2 method
        //
        case (SIGNATURE_32 ('_', 'T', 'C', '2')):
          //
          // Conditional match.  _TC2 is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = TablePointer - 2;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _TC2 enabled
            //
            if (mSystemConfiguration.DisablePassiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'T', 'C', '2');
            }
          }
          break;
        //
        // _TSP method
        //
        case (SIGNATURE_32 ('_', 'T', 'S', 'P')):
          //
          // Conditional match.  _TSP is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = TablePointer - 2;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _TSP enabled
            //
            if (mSystemConfiguration.DisablePassiveTripPoints == 0) {
              Signature   = (UINT32 *) TablePointer;
              *Signature  = SIGNATURE_32 ('X', 'T', 'S', 'P');
            }
          }
          break;
        }
      }
    }

    //
    // Load RTD3 SSDT table for PTL RVP SKUs
    // Note: "OemId" field is used to indentify whether SSDT table is for RTD3 usage
    //
    if ((CompareMem (&TableHeader->OemId,  "Rtd3", 4) == 0)) {
      //
      // OemId should be replaced with PcdAcpiDefaultOemId
      //
      CopyMem (TableHeader->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (TableHeader->OemId));
      //
      // set default = none
      //
      *Version = EFI_ACPI_TABLE_VERSION_NONE;

      //
      // check RTD3 enabled in setup
      //
      if (mSystemConfiguration.Rtd3Support == 1) {
        //
        // match table ID and BoardId
        //

        if (TableHeader->OemTableId == PcdGet64 (PcdBoardRtd3TableSignature)) {
          DEBUG ((DEBUG_INFO, "Board SsdtRtd3 Table: %lx\n", TableHeader->OemTableId));
          *Version = EFI_ACPI_TABLE_VERSION_1_0B | EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0;
        }

      } // Rtd3Support
    } // Load RTD3 SSDT table
  }
    break;

  default:
    break;
  }
  return EFI_SUCCESS;
}

/**
  Update available PCI BUS resource boundary to ACPI MCFG table

  @param[in]      SubBusBoundary     available PCI bus resource boundary
**/
VOID
UpdateMcfgAcpiTable (
  IN UINT8 SubBusBoundary
  )
{
  EFI_STATUS               Status;
  UINTN                    Handle;
  EFI_ACPI_TABLE_PROTOCOL  *AcpiTable;
  EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER *McfgTable;
  EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE *Segment;

  Handle = 0;

  //
  // Find the Acpi Table protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  ASSERT_EFI_ERROR (Status);

  Status = LocateAcpiTableBySignature (
              EFI_ACPI_6_5_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE,
              (EFI_ACPI_DESCRIPTION_HEADER **) &McfgTable,
              &Handle
              );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Found existing MCFG table\n"));
    Segment = (VOID *)(McfgTable + 1);
    //
    // Update SubBusBoundary to ACPI MCFG table
    //
    Segment[0].EndBusNumber = SubBusBoundary;

    if (Handle != 0) {
      Status = AcpiTable->UninstallAcpiTable (AcpiTable, Handle);
      if (EFI_ERROR (Status)) {
        FreePool (McfgTable);
        return;
      }
    }
  } else {
    DEBUG ((DEBUG_ERROR, "No existing MCFG table and failed to update available PCI BUS resource boundary.\n"));
    return;
  }

  ///
  /// Update ACPI MCFG table
  ///
  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        McfgTable,
                        McfgTable->Header.Length,
                        &Handle
                        );
  FreePool (McfgTable);
}

/**
  This function will report available PCI BUS resource boundary to ACPI MCFG table.
**/
VOID
UpdateMcfgPciBusBoundary (
  VOID
  )
{
  EFI_STATUS                Status;
  UINTN                     HandleCount;
  EFI_HANDLE                *HandleBuffer;
  UINTN                     Index;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  UINT8                     PciBaseClass;
  UINT8                     PciSubClass;
  PCI_REG_PCIE_CAPABILITY   PcieCap;
  UINT8                     SubBusBoundary;
  UINT8                     SubBus;
  UINTN                     Seg;
  UINTN                     Bus;
  UINTN                     Dev;
  UINTN                     Fun;
  UINT64                    PciBaseAdr;
  UINT8                     CapHeaderOffset;

  HandleCount  = 0;
  HandleBuffer = NULL;
  SubBusBoundary = 0;
  Seg = 0;

  DEBUG ((DEBUG_INFO, "UpdateMcfgPciBusBoundary Start\n"));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "No available PciIo devices\n"));
    return;
  }

  //
  // To determine maximum valid subordinate bus number among P2P bridges (RP)
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo
                    );
    if (EFI_ERROR (Status)) {
      //
      // Skip this PciIo handle
      //
      continue;
    }

    Status = PciIo->GetLocation (PciIo, (UINTN *) &Seg, (UINTN *) &Bus, (UINTN *) &Dev, (UINTN *) &Fun);
    if (EFI_ERROR (Status)) {
      continue;
    }
    if (Bus != 0) {
      //
      // P2P bridge is located at Bus 0 only
      //
      continue;
    }

    PciBaseAdr = PCI_SEGMENT_LIB_ADDRESS (Seg, Bus, Dev, Fun, 0);

    //
    // Get PCI device's class code (0xB)
    //
    PciBaseClass = PciSegmentRead8 (PciBaseAdr + PCI_CLASSCODE_OFFSET + 2);
    if (PciBaseClass == PCI_CLASS_BRIDGE) {
      //
      // Get PCI device's sub-class code (0xA)
      //
      PciSubClass = PciSegmentRead8 (PciBaseAdr + PCI_CLASSCODE_OFFSET + 1);
      if (PciSubClass == PCI_CLASS_BRIDGE_P2P) {
        CapHeaderOffset = PcieFindCapId ((UINT8) Seg, (UINT8) Bus, (UINT8) Dev, (UINT8) Fun, EFI_PCI_CAPABILITY_ID_PCIEXP);
        PciSegmentReadBuffer (PciBaseAdr + CapHeaderOffset + OFFSET_OF (PCI_CAPABILITY_PCIEXP, Capability), sizeof (UINT16), &PcieCap);
        if (PcieCap.Bits.DevicePortType != PCIE_DEVICE_PORT_TYPE_ROOT_PORT) {
          //
          // Skip non RootPort type
          //
          continue;
        }
        DEBUG ((DEBUG_INFO, "Bus: 0x%x, Dev: 0x%x, Fun:0x%x is P2P bridge (type: RootPort)\n", Bus, Dev, Fun));
        //
        // Get subordinate bus number (0x1A)
        //
        SubBus = PciSegmentRead8 (PciBaseAdr + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);
        DEBUG ((DEBUG_INFO, "Subordinate bus number: 0x%x\n", SubBus));
        //
        // Initial SubBusBoundary to a meaningful value only one time
        //
        if (SubBus > SubBusBoundary) {
          SubBusBoundary = SubBus;
        }
      }
    }
  }

  //
  // Check if max sub bus exceeds 224 when VMD enabled
  //
  if (IsVmdEnabled ()) {
    if (SubBusBoundary > MAX_OS_VISIBLE_BUSES_WITH_VMD) {
      DEBUG ((DEBUG_WARN, "Subordiante bus number is not allowed to exceed 224 when VMD enabled !\n"));
      SubBusBoundary = MAX_OS_VISIBLE_BUSES_WITH_VMD;
    }
  }

  mPlatformNvsAreaProtocol.Area->SubBusBoundary = SubBusBoundary;
  DEBUG ((DEBUG_INFO, "Valid Subordinate bus: 0x%x\n",mPlatformNvsAreaProtocol.Area->SubBusBoundary));

  //
  // Update MCFG table
  //
  UpdateMcfgAcpiTable (SubBusBoundary);

  if (HandleBuffer != NULL) {
    FreePool ((VOID*)HandleBuffer);
  }

  DEBUG ((DEBUG_INFO, "UpdateMcfgPciBusBoundary End\n"));
}

EFI_STATUS
PublishAcpiTablesFromFv (
  IN EFI_GUID gEfiAcpiMultiTableStorageGuid
  )
{
  EFI_STATUS                    Status;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  UINT32                        FvStatus;
  UINTN                         Size;
  EFI_ACPI_TABLE_VERSION        Version;
  UINTN                         TableHandle;
  INTN                          Instance;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;

  Instance      = 0;
  TableHandle   = 0;
  CurrentTable  = NULL;
  FwVol         = NULL;

  //
  // Find the AcpiSupport protocol
  //
  Status = LocateSupportProtocol (
            &gEfiAcpiTableProtocolGuid,
            gEfiAcpiMultiTableStorageGuid,
            (VOID **) &AcpiTable,
            FALSE
            );
  ASSERT_EFI_ERROR (Status);

  //
  // Locate the firmware volume protocol
  //
  Status = LocateSupportProtocol (
            &gEfiFirmwareVolume2ProtocolGuid,
            gEfiAcpiMultiTableStorageGuid,
            (VOID **) &FwVol,
            TRUE
            );

  /* TODO: Hang at native mode. Need to find the cause
   * when the related drivers are finished for native support. */
  //ASSERT_EFI_ERROR (Status);

  //
  // Read tables from the storage file.
  //
  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiAcpiMultiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &CurrentTable,
                      &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {

      //
      // Perform any table specific updates.
      //
      PlatformUpdateTables (CurrentTable, &Version);

      //
      // Add the table
      //
      TableHandle = 0;

      if (Version != EFI_ACPI_TABLE_VERSION_NONE) {
        DEBUG((DEBUG_INFO, "[Advanced ACPI] Installing table: %x : %x\n", CurrentTable->Signature, CurrentTable));
        Status = AcpiTable->InstallAcpiTable (
                              AcpiTable,
                              CurrentTable,
                              CurrentTable->Length,
                              &TableHandle
                              );
      }

      ASSERT_EFI_ERROR (Status);

      //
      // Increment the instance
      //
      Instance++;
      gBS->FreePool (CurrentTable);
      CurrentTable = NULL;
    }

    if (CurrentTable != NULL) {
      gBS->FreePool (CurrentTable);
      CurrentTable = NULL;
    }
  }

  //
  // Finished
  //
  return EFI_SUCCESS;
}

UINTN
GetFileLength (
  IN EFI_FILE_HANDLE              FHand
  )
{
  EFI_STATUS                      Status;
  EFI_FILE_INFO                   *Buffer;
  UINTN                           BufferSize;

  ///
  /// Initialize for GrowBuffer loop
  ///
  BufferSize  = SIZE_OF_EFI_FILE_INFO + 200;

  ///
  /// Call the real function
  ///
  do {
    Buffer = AllocateZeroPool (BufferSize);
    Status = FHand->GetInfo (
                      FHand,
                      &gEfiFileInfoGuid,
                      &BufferSize,
                      Buffer
                      );
    DEBUG ((DEBUG_INFO, "GetFileLenght: GetInfo: %r", Status));
    DEBUG ((DEBUG_INFO, "GetFileLength: FileSize: %lX (%ld), Buffer: %lX\n", BufferSize, BufferSize, Buffer));
    if (!EFI_ERROR (Status)) {
      break;
    }
    if (Status != EFI_BUFFER_TOO_SMALL) {
      FreePool (Buffer);
      break;
    }
    FreePool (Buffer);
  } while (TRUE);

  if (Buffer) {
    BufferSize = Buffer->FileSize;
    FreePool (Buffer);
  }  else {
    BufferSize = 0;
  }
  return BufferSize;
}

EFI_STATUS
LoadSSDTableFromFile (
  VOID
  )
{
  EFI_STATUS                          Status = EFI_SUCCESS;
  EFI_HANDLE                          *SimpleFileSystemHandles;
  UINTN                               NumberSimpleFileSystemHandles;
  UINTN                               Index;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL     *Fs;
  EFI_FILE_HANDLE                     Root;
  EFI_FILE_HANDLE                     FileHandle;
  UINTN                               BufferSize;
  VOID                                *Buffer;
  UINTN                               TableHandle = 0;
  EFI_ACPI_DESCRIPTION_HEADER         *Table;
  EFI_ACPI_TABLE_PROTOCOL             *AcpiTable;

  DEBUG ((DEBUG_INFO, "LoadSSDTableFromFile -> Start\n"));

  //
  // Find all the instances of the File System Protocol
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &NumberSimpleFileSystemHandles,
                  &SimpleFileSystemHandles
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "LoadSSDTableFromFile failed at LocateHandleBuffer(). Status = %r\n", Status));
    return Status;
  } else {
    DEBUG((DEBUG_INFO, "LoadSSDTableFromFile: Number of Simple FS handles: %x\n", NumberSimpleFileSystemHandles));
  }

  for (Index = 0; Index < NumberSimpleFileSystemHandles; Index++)
  {
    DEBUG((DEBUG_INFO, "LoadSSDTableFromFile: FS handle Index = %x\n", Index));
    Status = gBS->HandleProtocol (SimpleFileSystemHandles[Index], &gEfiSimpleFileSystemProtocolGuid, (VOID **) &Fs);
    if (EFI_ERROR (Status)) {
      continue;
    }
    Status = Fs->OpenVolume (Fs, &Root);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "LoadSSDTableFromFile: OpenVolume failed = %r\n", Status));
      continue;
    }
    FileHandle = NULL;
    Status = Root->Open (Root, &FileHandle, SSDT_FILE_NAME, EFI_FILE_MODE_READ, 0);
    DEBUG ((DEBUG_INFO, "LoadSSDTableFromFile: Find %S %r\n", SSDT_FILE_NAME, Status));
    if (EFI_ERROR (Status)) {
      Root->Close (Root);
      continue;
    }
    BufferSize = GetFileLength (FileHandle);
    if (BufferSize == 0) {
      FileHandle->Close (FileHandle);
      Root->Close (Root);
      DEBUG ((DEBUG_INFO, "LoadSSDTableFromFile: FileSize is 0\n"));
      continue;
    }
    Buffer = AllocateZeroPool (BufferSize);
    DEBUG ((DEBUG_INFO, "LoadSSDTableFromFile: Allocate BufferSize:%lX Buffer:%lX\n", BufferSize, Buffer));
    if (Buffer == NULL) {
      FileHandle->Close (FileHandle);
      Root->Close (Root);
      DEBUG ((DEBUG_ERROR, "LoadSSDTableFromFile: AllocateZeroPool: Failed\n"));
      continue;
    }
    Status = FileHandle->Read (FileHandle, &BufferSize, Buffer);
    DEBUG ((DEBUG_INFO, "LoadSSDTableFromFile: Read status %r\n", Status));
    DEBUG ((DEBUG_INFO, "LoadSSDTableFromFile: BufferSize:%lX (%ld) Buffer:%lX\n", BufferSize, BufferSize, Buffer));
    FileHandle->Close (FileHandle);
    Root->Close (Root);
    if (EFI_ERROR (Status)) {
      FreePool (Buffer);
      DEBUG ((DEBUG_ERROR, "LoadSSDTableFromFile: Reading from file failed %r\n", Status));
      continue;
    }

    Table = (EFI_ACPI_DESCRIPTION_HEADER *)Buffer;
    if (Table->OemTableId == SIGNATURE_64 ('s', 's', 'd', 't', 'f', 'i', 'l', 'e'))
    {
      Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
      if (EFI_ERROR (Status)) {
        FreePool (Buffer);
        continue;
      }
      DEBUG ((DEBUG_INFO, "LoadSSDTableFromFile: Table length %x\n", Table->Length));
      Status = AcpiTable->InstallAcpiTable (
                            AcpiTable,
                            Table,
                            Table->Length,
                            &TableHandle
                            );
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "LoadSSDTableFromFile: ACPI table installed SUCCESSFULLY\n"));
        break;
      }
    }

    FreePool (Buffer);
  }

  FreePool (SimpleFileSystemHandles);
  DEBUG ((DEBUG_INFO, "LoadSSDTableFromFile -> End\n"));
  return Status;
}

/**
  Allocate MemoryType below 4G memory address.

  @param[in] Size     - Size of memory to allocate.
  @param[in] Buffer   - Allocated address for output.

  @retval EFI_SUCCESS - Memory successfully allocated.
  @retval Other       - Other errors occur.

**/
EFI_STATUS
AllocateMemoryBelow4G (
  IN   EFI_MEMORY_TYPE MemoryType,
  IN   UINTN           Size,
  OUT  VOID           **Buffer
  )
{
  UINTN                 Pages;
  EFI_PHYSICAL_ADDRESS  Address;
  EFI_STATUS            Status;

  Pages = EFI_SIZE_TO_PAGES (Size);
  Address = 0xffffffff;

  Status  = (gBS->AllocatePages) (
                   AllocateMaxAddress,
                   MemoryType,
                   Pages,
                   &Address
                   );

  *Buffer = (VOID *) (UINTN) Address;

  return Status;
}

EFI_STATUS
ReserveS3Memory (
  VOID
  )
{
  VOID                                          *AcpiMemoryBase;
  UINT32                                        AcpiMemorySize;
  EFI_PEI_HOB_POINTERS                          Hob;
  UINT64                                        MemoryLength;
  S3_MEMORY_VARIABLE                            S3MemVariable;
  VOID                                          *VariableData;
  UINTN                                         VarSize;
  EFI_STATUS                                    Status;
  EDKII_VARIABLE_POLICY_PROTOCOL                *VariablePolicy;

  DEBUG ((DEBUG_INFO, "ReserveS3Memory\n"));

  //
  // Allocate ACPI reserved memory under 4G
  //
  Status = AllocateMemoryBelow4G (EfiReservedMemoryType, PcdGet32 (PcdS3AcpiReservedMemorySize), &AcpiMemoryBase);
  ASSERT (AcpiMemoryBase != 0);
  AcpiMemorySize = PcdGet32 (PcdS3AcpiReservedMemorySize);

  //
  // Calculate the system memory length by memory hobs
  //
  MemoryLength  = 0x100000;
  Hob.Raw = GetFirstHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR);
  ASSERT (Hob.Raw != NULL);
  while ((Hob.Raw != NULL) && (!END_OF_HOB_LIST (Hob))) {
    if (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) {
      //
      // Skip the memory region below 1MB
      //
      if (Hob.ResourceDescriptor->PhysicalStart >= 0x100000) {
        MemoryLength += Hob.ResourceDescriptor->ResourceLength;
      }
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
    Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw);
  }

  S3MemVariable.AcpiReservedMemoryBase = (EFI_PHYSICAL_ADDRESS)(UINTN) AcpiMemoryBase;
  S3MemVariable.AcpiReservedMemorySize = AcpiMemorySize;
  S3MemVariable.SystemMemoryLength = MemoryLength;

  DEBUG((DEBUG_INFO, "ReserveS3Memory: AcpiMemoryBase is 0x%8x\n", S3MemVariable.AcpiReservedMemoryBase));
  DEBUG((DEBUG_INFO, "ReserveS3Memory: AcpiMemorySize is 0x%8x\n", S3MemVariable.AcpiReservedMemorySize));
  DEBUG((DEBUG_INFO, "ReserveS3Memory: SystemMemoryLength is 0x%8x\n", S3MemVariable.SystemMemoryLength));

  VarSize = sizeof(S3_MEMORY_VARIABLE);
  Status = gBS->AllocatePool(
                  EfiBootServicesData,
                  VarSize,
                  (VOID **) &VariableData
                  );
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)){
    return Status;
  }
  Status = gRT->GetVariable (
                    S3_MEMORY_VARIABLE_NAME,
                    &gS3MemoryVariableGuid,
                    NULL,
                    &VarSize,
                    VariableData
                    );
  if (EFI_ERROR (Status) || (0 != CompareMem (&S3MemVariable, VariableData, VarSize))) {
    DEBUG((DEBUG_INFO, "Update S3MemVariable.\n"));
    gRT->SetVariable (
                    S3_MEMORY_VARIABLE_NAME,
                    &gS3MemoryVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    VarSize,
                    &S3MemVariable
                    );
  }
  gBS->FreePool(VariableData);

  Status = gBS->LocateProtocol (&gEdkiiVariablePolicyProtocolGuid, NULL, (VOID**)&VariablePolicy);
  DEBUG ((DEBUG_INFO, "Locate Variable Policy protocol - %r\n", Status));
  if (!EFI_ERROR (Status)) {
      Status = RegisterBasicVariablePolicy (VariablePolicy,
                 &gS3MemoryVariableGuid,
                 S3_MEMORY_VARIABLE_NAME,
                 VARIABLE_POLICY_NO_MIN_SIZE,
                 VARIABLE_POLICY_NO_MAX_SIZE,
                 VARIABLE_POLICY_NO_MUST_ATTR,
                 VARIABLE_POLICY_NO_CANT_ATTR,
                 VARIABLE_POLICY_TYPE_LOCK_NOW
                 );
      ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

VOID
EFIAPI
AcpiEndOfDxeEvent (
  EFI_EVENT           Event,
  VOID                *ParentImageHandle
  )
{
  CPU_SETUP     CpuSetup;
  UINTN         VarDataSize;
  EFI_STATUS    Status;

  if (Event != NULL) {
    gBS->CloseEvent(Event);
  }


  if (mSystemConfiguration.LoadSSDTFromFile == 1) {
    Status = LoadSSDTableFromFile();
    if (Status == EFI_SUCCESS) {
      DEBUG((DEBUG_INFO, "LoadSSDTableFromFile executed successfully\n"));
    }
  }

  //
  // Install Txt SSDT ACPI Table
  //
  VarDataSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  NULL,
                  &VarDataSize,
                  &CpuSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  if (CpuSetup.Txt) {
    InstallTxtAcpiTable ();
  }

  //
  // Install Soc Gpe SSDT ACPI Table
  //
  InstallSocAcpiTable (&gSocGpeSsdtAcpiTableStorageGuid, SIGNATURE_64 ('S', 'o', 'c', 'G', 'p', 'e', ' ', 0));

  //
  // Install Soc Common SSDT ACPI Table
  //
  InstallSocAcpiTable (&gSocCmnSsdtAcpiTableStorageGuid, SIGNATURE_64 ('S', 'o', 'c', 'C', 'm', 'n', ' ', 0));

  //
  // This function updates PCI BUS resource boundary to ACPI MCFG table
  //
  UpdateMcfgPciBusBoundary ();

  //
  // Reserve memory for S3 resume
  //
  ReserveS3Memory ();
}

/**
  Convert string containing GUID in the canonical form:
  "aabbccdd-eeff-gghh-iijj-kkllmmnnoopp"
  where aa - pp are unicode hexadecimal digits
  to the buffer format to be used in ACPI, byte ordering:
  [Byte 0] gg, hh, ee, ff, aa, bb, cc, dd [Byte 7]
  [Byte 8] pp, oo, nn, mm, ll, kk, jj, ii [Byte 16]

  @param[in]  GuidString    - GUID String null terminated (aligned on a 16-bit boundary)
  @param[out] AcpiGuidPart1 - First half of buffer  (bytes 0 - 7)
  @param[out] AcpiGuidPart2 - Second half of buffer (bytes 8 - 16)

  @retval EFI_SUCCESS     - String converted successfully.
  @retval EFI_UNSUPPORTED - Wrong input string format.

**/
EFI_STATUS
GuidStringToAcpiBuffer (
  IN  CHAR16 *GuidString,
  OUT UINT64 *AcpiGuidPart1,
  OUT UINT64 *AcpiGuidPart2
  )
{
  UINT32 GuidTempPart32 = 0;
  UINT16 GuidTempPart16 = 0;
  UINT8  GuidPartIndex;

  DEBUG((DEBUG_INFO,"GuidStringToAcpiBuffer() - GUID = %s\n", GuidString));

  for (GuidPartIndex = 0; GuidPartIndex < 4; GuidPartIndex++) {
    switch (GuidPartIndex){
      case 0:
        GuidTempPart32 = SwapBytes32((UINT32)StrHexToUint64(GuidString));
        *AcpiGuidPart1 = ((UINT64)GuidTempPart32 << 0x20);
        break;
      case 1:
        GuidTempPart16 = SwapBytes16((UINT16)StrHexToUint64(GuidString));
        *AcpiGuidPart1 += ((UINT64)GuidTempPart16 << 0x10);
        break;
      case 2:
        GuidTempPart16 = SwapBytes16((UINT16)StrHexToUint64(GuidString));
        *AcpiGuidPart1 += ((UINT64)GuidTempPart16);
        break;
      case 3:
        GuidTempPart16 = (UINT16)StrHexToUint64(GuidString);
        break;
    }

    while ((*GuidString != L'-') && (*GuidString != L'\0')) {
      GuidString++;
    }

    if (*GuidString == L'-') {
      GuidString++;
    } else {
      return EFI_UNSUPPORTED;
    }
  }

  *AcpiGuidPart2 = ((UINT64)GuidTempPart16 << 0x30) + StrHexToUint64(GuidString);

  // Switch endianess because it will be swapped again in ACPI Buffer object
  *AcpiGuidPart1 = SwapBytes64(*AcpiGuidPart1);
  *AcpiGuidPart2 = SwapBytes64(*AcpiGuidPart2);

  return EFI_SUCCESS;
}

/**
  Update USB connector properties for USB Camera

  @param[in] Usb2PortNum     USB2 port number with USB Camera attached.
  @retval EFI_SUCCESS        The function is called without error.
**/
EFI_STATUS
UpdateUsbCameraConnector (
  IN UINT8  Usb2PortNum
  )
{
  UINT8                         EntryNum;
  VOID                          *Hob;
  USB_CONNECTOR_HOB_DATA        *UsbConnectorHobDataPtr;
  USB_CONNECTOR_BOARD_CONFIG    *UsbConnectorBoardConfig;

  //
  // Get USB Connector Table
  //
  Hob = GetFirstGuidHob (&gUsbConnectorHobGuid);
  if (Hob == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorHob Not Found!!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  UsbConnectorHobDataPtr = GET_GUID_HOB_DATA (Hob);
  UsbConnectorBoardConfig = UsbConnectorHobDataPtr->UsbConnectorBoardConfig;

  //
  // Look for the USB connector used by USB Camera and update connector properties
  //
  for (EntryNum = 0; EntryNum < UsbConnectorHobDataPtr->NumberOfUsbConnectors; EntryNum++) {
    if (UsbConnectorBoardConfig[EntryNum].Usb2Controller == PCH_USB2 && UsbConnectorBoardConfig[EntryNum].Usb2PortNum == Usb2PortNum) {
      UsbConnectorBoardConfig[EntryNum].ConnectorVisibility = 0;
      UsbConnectorBoardConfig[EntryNum].Usb3Controller = 0;
      UsbConnectorBoardConfig[EntryNum].Usb3PortNum = 0;
    }
  }
  return EFI_SUCCESS;
}

/**
  ACPI Platform driver installation function.

  @param[in] ImageHandle     Handle for this drivers loaded image protocol.
  @param[in] SystemTable     EFI system table.

  @retval EFI_SUCCESS        The driver installed without error.
  @retval EFI_ABORTED        The driver encountered an error and could not complete installation of
                             the ACPI tables.

**/
EFI_STATUS
EFIAPI
InstallAcpiPlatform (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                    Status;
  UINTN                         VarDataSize;
  EFI_HANDLE                    Handle;
#if FixedPcdGetBool (PcdEcEnable) == 1
  UINT8                         PortData;
#endif
  CPUID_VERSION_INFO_EAX        CpuidVersionInfoEax;
  UINTN                         Index;
  EFI_EVENT                     EndOfDxeEvent;
  CPU_SETUP                     CpuSetup;
  CNV_VFR_CONFIG_SETUP          CnvSetup;
  VTIO_SETUP_DATA_STRUCT        VtioSetupData;
  UINT8                         PreBootCmMode;
  UINT8                         CmTbtMask;
  UINT8                         Usb4CmSwitchEnable;
  VOID                          *HobPtr;
  USB4_PLATFORM_HOB             *Usb4PlatformHob;
  USB4_PLATFORM_INFO            *Usb4PlatformInfo;
  UINT32                        *PcdRootPortPewakeConfig;

  UINT32                        TableLoadBufferSize;
  VOID                          *TableLoadBuffer;

  UINT64                        AcpiGuidPart1;
  UINT64                        AcpiGuidPart2;
  CHAR16                        LocalGuidString[GUID_CHARS_NUMBER];
  SETUP_DATA                    SetupData;
  EFI_STATUS                    SetupStatus;
  UINTN                         VariableSize;
  UINT32                        SetupAttr;
  UINT16                        BoardId;
  RMT_VAR                       mRmtdata;

  Handle                     = NULL;
  Usb4PlatformInfo           = NULL;
  mUcsiNvsAreaProtocol       = NULL;
  mUsbPortMapNvsAreaProtocol = NULL;

  BoardId = PcdGet16 (PcdBoardId);

  EfiNamedEventListen (
    &gUcsiNvsAreaProtocolGuid,
    TPL_CALLBACK,
    UcsiEventCallBack,
    NULL,
    NULL
    );

  EfiNamedEventListen (
    &gUsbPortMapNvsAreaProtocolGuid,
    TPL_CALLBACK,
    UsbPortMapEventCallBack,
    NULL,
    NULL
    );

  ZeroMem (&VtioSetupData, sizeof (VTIO_SETUP_DATA_STRUCT));

  AsmCpuid (
    CPUID_VERSION_INFO,
    &CpuidVersionInfoEax.Uint32,
    NULL,
    NULL,
    NULL
    );

  PublishOverClockingAcpiTable ();

  //
  // Locate the setup configuration data
  // We assert if it is not found because we have a dependency on PlatformCpu,
  // published by the platform setup driver.
  // This should ensure that we only get called after the setup has updated the
  // variable.
  //
  VarDataSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VarDataSize,
                  &mSystemConfiguration
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  SetupAttr = 0;
  VariableSize = sizeof (SETUP_DATA);
  SetupStatus = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  &SetupAttr,
                  &VariableSize,
                  &SetupData
                  );
  if (EFI_ERROR (SetupStatus)) {
    ASSERT_EFI_ERROR (SetupStatus);
    return SetupStatus;
  }

  VarDataSize = sizeof (ME_SETUP);
  Status = gRT->GetVariable (
                  L"MeSetup",
                  &gMeSetupVariableGuid,
                  NULL,
                  &VarDataSize,
                  &mMeSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  VarDataSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  NULL,
                  &VarDataSize,
                  &CpuSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  VarDataSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  NULL,
                  &VarDataSize,
                  &mPchSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  VarDataSize = sizeof (CNV_VFR_CONFIG_SETUP);
  ZeroMem (&CnvSetup, VarDataSize);
#if FixedPcdGetBool (PcdCnvFeatureEnable) == 1
  Status = gRT->GetVariable (
                  L"CnvSetup",
                  &gCnvFeatureSetupGuid,
                  NULL,
                  &VarDataSize,
                  &CnvSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
#endif

  VarDataSize = sizeof (VTIO_SETUP_DATA_STRUCT);
  Status = gRT->GetVariable (
                VTIO_CFG_VARIABLE_NAME,
                &mVtioFormSetGuid,
                NULL,
                &VarDataSize,
                &VtioSetupData
                );

  VarDataSize = sizeof (RMT_VAR);
  mRmtdata.EnDsRmt = 0;
  Status = gRT->GetVariable (
                  L"Rmt",
                  &gRmtVariableGuid,
                  NULL,
                  &VarDataSize,
                  &mRmtdata
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rmt data not found - Status = %r\n",Status));
  } else {
    DEBUG ((DEBUG_INFO, "Update RMT NVS Area \n"));
  }

  //
  // Allocate and initialize the NVS area for SMM and ASL communication.
  //
  AcpiGnvsInit((VOID **) &mPlatformNvsAreaProtocol.Area);

  //
  // Update global NVS area for ASL and SMM init code to use
  //
  //mPlatformNvsAreaProtocol.Area->EnableThermalOffset        = mSystemConfiguration.EnableThermalOffset;
  mPlatformNvsAreaProtocol.Area->Ac1TripPoint                 = mSystemConfiguration.Ac1TripPoint;
  mPlatformNvsAreaProtocol.Area->Ac0TripPoint                 = mSystemConfiguration.Ac0TripPoint;
  mPlatformNvsAreaProtocol.Area->Ac1FanSpeed                  = mSystemConfiguration.Ac1FanSpeed;
  mPlatformNvsAreaProtocol.Area->Ac0FanSpeed                  = mSystemConfiguration.Ac0FanSpeed;
  mPlatformNvsAreaProtocol.Area->PassiveThermalTripPoint      = mSystemConfiguration.PassiveThermalTripPoint;
  mPlatformNvsAreaProtocol.Area->PassiveTc1Value              = mSystemConfiguration.PassiveTc1Value;
  mPlatformNvsAreaProtocol.Area->PassiveTc2Value              = mSystemConfiguration.PassiveTc2Value;
  mPlatformNvsAreaProtocol.Area->PassiveTspValue              = mSystemConfiguration.PassiveTspValue;
  mPlatformNvsAreaProtocol.Area->CriticalThermalTripPoint     = mSystemConfiguration.CriticalThermalTripPoint;
  mPlatformNvsAreaProtocol.Area->EmaEnable                    = 0; //mSystemConfiguration.EmaEnable; //todo

  mPlatformNvsAreaProtocol.Area->HebcValue               = mSystemConfiguration.HebcValue;

  //
  // BIOS only version of CTDP. (CTDP without using Intel(R) Dynamic Tuning Technology)
  //
  if (CpuSetup.ConfigTdpLock == 1 || PcdGetBool (VpdPcdIpfEnable)) {
    mPlatformNvsAreaProtocol.Area->ConfigTdpBios      = 0;
  } else {
    mPlatformNvsAreaProtocol.Area->ConfigTdpBios      = CpuSetup.ConfigTdpBios;
  }

  //
  // PECI Access Method
  //
  mPlatformNvsAreaProtocol.Area->PeciAccessMethod     = mSystemConfiguration.PeciAccessMethod;

  mPlatformNvsAreaProtocol.Area->SystemTimeAndAlarmSource = mSystemConfiguration.SystemTimeAndAlarmSource;
  mPlatformNvsAreaProtocol.Area->XdciFnEnable       = mPchSetup.PchXdciSupport;
  //
  // Initilize platform ID in Global NVS Area
  //
  mPlatformNvsAreaProtocol.Area->GenerationId   = PcdGet8 (PcdPlatformGeneration);
  mPlatformNvsAreaProtocol.Area->PlatformFlavor = PcdGet8 (PcdPlatformFlavor);
  mPlatformNvsAreaProtocol.Area->BoardRev       = PcdGet16 (PcdBoardRev);
  mPlatformNvsAreaProtocol.Area->BoardType      = PcdGet8 (PcdBoardType);
  mPlatformNvsAreaProtocol.Area->PlatformBoardId     = (UINT8) PcdGet16 (PcdBoardId);

  mPlatformNvsAreaProtocol.Area->SerialPortAcpiDebug        = mSystemConfiguration.AcpiDebug && mSystemConfiguration.SerialPortAcpiDebug;
  mPlatformNvsAreaProtocol.Area->EnableAcpiDebug            = mSystemConfiguration.AcpiDebug;

  mPlatformNvsAreaProtocol.Area->Rtd3Support                = mSystemConfiguration.Rtd3Support;
  mPlatformNvsAreaProtocol.Area->Rtd3P0dl                   = mSystemConfiguration.Rtd3P0dl;
  mPlatformNvsAreaProtocol.Area->Rtd3AudioDelay             = mSystemConfiguration.Rtd3AudioDelay;
  mPlatformNvsAreaProtocol.Area->Rtd3SensorHub              = mSystemConfiguration.Rtd3SensorHub;
  mPlatformNvsAreaProtocol.Area->Rtd3TouchPanelDelay        = mSystemConfiguration.Rtd3TouchPanelDelay;
  mPlatformNvsAreaProtocol.Area->Rtd3TouchPadDelay          = mSystemConfiguration.Rtd3TouchPadDelay;
  mPlatformNvsAreaProtocol.Area->TouchpadIrqGpioPolarity    = PcdGetBool(PcdTouchpadIrqGpioPolarity);
  mPlatformNvsAreaProtocol.Area->VRRampUpDelay              = mSystemConfiguration.VRRampUpDelay;
  mPlatformNvsAreaProtocol.Area->PstateCapping              = mSystemConfiguration.PstateCapping;
  mPlatformNvsAreaProtocol.Area->SensorStandby              = mSystemConfiguration.SensorStandby;

  mPlatformNvsAreaProtocol.Area->Rtd3Config0                = (mSystemConfiguration.Rtd3I2CTouchPanel << 4);

  mPlatformNvsAreaProtocol.Area->StorageRtd3Support           = mSystemConfiguration.StorageRtd3Support;
  mPlatformNvsAreaProtocol.Area->StorageDynamicLinkManagement = mSystemConfiguration.StorageDynamicLinkManagement;

  switch (BoardId) {
    case BoardIdPtlUHLp5Rvp1:
    case BoardIdPtlUHLp5Rvp3:
    case BoardIdPtlUHDdr5Rvp4:
      mPlatformNvsAreaProtocol.Area->M2Ssd1Gen4_5Dlrm              = mSystemConfiguration.StorageDynamicLinkManagement;
      mPlatformNvsAreaProtocol.Area->M2Ssd2Gen4_5Dlrm              = mSystemConfiguration.StorageDynamicLinkManagement;
      break;
    case BoardIdPtlUHCammDTbTRvp2:
    case BoardIdPtlUHLp5Aep:
    case BoardIdPtlHLp5Gcs1:
    case BoardIdPtlHLp5Gcs2:
    case BoardIdPtlUHLp5Gcs9600:
      mPlatformNvsAreaProtocol.Area->M2Ssd1Gen4_5Dlrm              = 0;
      mPlatformNvsAreaProtocol.Area->M2Ssd2Gen4_5Dlrm              = mSystemConfiguration.StorageDynamicLinkManagement;
      break;
    default:
      mPlatformNvsAreaProtocol.Area->M2Ssd1Gen4_5Dlrm              = 0;
      mPlatformNvsAreaProtocol.Area->M2Ssd2Gen4_5Dlrm              = 0;
      break;
  }
  DEBUG ((DEBUG_INFO, "M2Ssd1Gen4_5Dlrm %x\n", mPlatformNvsAreaProtocol.Area->M2Ssd1Gen4_5Dlrm));
  DEBUG ((DEBUG_INFO, "M2Ssd2Gen4_5Dlrm %x\n", mPlatformNvsAreaProtocol.Area->M2Ssd1Gen4_5Dlrm));
  //
  // Enable PowerState
  //
  mPlatformNvsAreaProtocol.Area->PowerState = 1; // AC =1; for mobile platform, will update this value in SmmPlatform.c

  //
  // These don't have setup controls yet
  //
  mPlatformNvsAreaProtocol.Area->IdeMode = 0;

#if FixedPcdGetBool (PcdEcEnable) == 1
  //
  // Call EC lib to get Virtual Dock status
  //
  PortData = 0;
  Status = GetVirtualDockStatus (&PortData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Status = %r\n", Status));
  }

  //
  // the bit0 is Virtual Dock Status, 1 = attached
  //
  mPlatformNvsAreaProtocol.Area->VirtualDockStatus = (PortData & 1);
#endif

  mPlatformNvsAreaProtocol.Area->NativePCIESupport        = mSystemConfiguration.PciExpNative;
  mPlatformNvsAreaProtocol.Area->PlatformCpuId            = (CpuidVersionInfoEax.Uint32 & 0x0FFFFF);

  //
  // Create an End of DXE event.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AcpiEndOfDxeEvent,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Enable APIC
  //
  mPlatformNvsAreaProtocol.Area->ApicEnable = PLATFORM_NVS_DEVICE_ENABLE;

  mPlatformNvsAreaProtocol.Area->EnableVoltageMargining = PcdGetBool (PcdEnableVoltageMargining);

  //
  // Enable 10sec Power Button Override only if CS is enabled.
  //
  if (mSystemConfiguration.LowPowerS0Idle == 1) {
    mPlatformNvsAreaProtocol.Area->TenSecondPowerButtonEnable |= BIT0; // Enable 10sec Power Button OVR.
    if (mSystemConfiguration.HidEventFilterDriverEnable) {
      mPlatformNvsAreaProtocol.Area->HidEventFilterEnable = 0x01;        // Enable Hid Event Filter Driver
    } else {
      mPlatformNvsAreaProtocol.Area->HidEventFilterEnable = 0x00;        // Disable Hid Event Filter Driver
    }
  } else {
    mPlatformNvsAreaProtocol.Area->TenSecondPowerButtonEnable &= ~(BIT0); // Disable 10sec Power Button OVR.
    //
    // HID Event Filter Enable/Disable
    //
    mPlatformNvsAreaProtocol.Area->HidEventFilterEnable = mSystemConfiguration.HidEventFilterDriverEnable;
  }
  if (SetupStatus != EFI_SUCCESS) {
    mPlatformNvsAreaProtocol.Area->TenSecondPowerButtonEnable |= (SetupData.SlateIndicatorRT << 3);
    mPlatformNvsAreaProtocol.Area->TenSecondPowerButtonEnable |= (SetupData.DockIndicatorRT << 4);

    mPlatformNvsAreaProtocol.Area->VirtualGpioButtonSxBitmask  = 0x00;
    mPlatformNvsAreaProtocol.Area->VirtualGpioButtonSxBitmask |=  SetupData.SlateIndicatorSx << 6;
    mPlatformNvsAreaProtocol.Area->VirtualGpioButtonSxBitmask |=  SetupData.DockIndicatorSx << 7;
  }

  //
  // Low Power S0 Idle - Enabled/Disabled
  //
  mPlatformNvsAreaProtocol.Area->LowPowerS0Idle = mSystemConfiguration.LowPowerS0Idle;
  mPlatformNvsAreaProtocol.Area->PseudoG3State = mSystemConfiguration.PseudoG3State;
  mPlatformNvsAreaProtocol.Area->PuisEnable = mSystemConfiguration.PuisEnable;


  mPlatformNvsAreaProtocol.Area->Rtd3Pcie = mSystemConfiguration.Rtd3Pcie;

  // SD Card Enable/Disable
  mPlatformNvsAreaProtocol.Area->SdCardEnable         = PcdGet8 (PcdSdCardEnable);

  //
  // PCIe Slot 1 GPIO pin configuration
  //
  mPlatformNvsAreaProtocol.Area->PcieSlot1PowerEnableGpio = PcdGet32(PcdPcieSlot1PwrEnableGpioNo);
  mPlatformNvsAreaProtocol.Area->PcieSlot1PowerEnableGpioPolarity = PcdGetBool(PcdPcieSlot1PwrEnableGpioPolarity);
  mPlatformNvsAreaProtocol.Area->PcieSlot1RstGpio = PcdGet32(PcdPcieSlot1HoldRstGpioNo);
  mPlatformNvsAreaProtocol.Area->PcieSlot1RstGpioPolarity = PcdGetBool(PcdPcieSlot1HoldRstGpioPolarity);
  mPlatformNvsAreaProtocol.Area->PcieSlot1RpNumber = PcdGet8 (PcdPcieSlot1RootPort);
  mPlatformNvsAreaProtocol.Area->Dg1VramSRGpio = PcdGet32 (PcdDg1VramSRGpio);

  //
  // PCIe Slot 2 GPIO pin configuration
  //
  mPlatformNvsAreaProtocol.Area->PcieSlot2PowerEnableGpio = PcdGet32 (PcdPcieSlot2PwrEnableGpioNo);
  mPlatformNvsAreaProtocol.Area->PcieSlot2PowerEnableGpioPolarity = PcdGetBool (PcdPcieSlot2PwrEnableGpioPolarity);
  mPlatformNvsAreaProtocol.Area->PcieSlot2RstGpio = PcdGet32 (PcdPcieSlot2HoldRstGpioNo);
  mPlatformNvsAreaProtocol.Area->PcieSlot2RstGpioPolarity = PcdGetBool (PcdPcieSlot2HoldRstGpioPolarity);
  mPlatformNvsAreaProtocol.Area->PcieSlot2RpNumber = PcdGet8 (PcdPcieSlot2RootPort);

  //
  // PCIe Slot 3 GPIO pin configuration
  //
  mPlatformNvsAreaProtocol.Area->PcieSlot3PowerEnableGpio = PcdGet32 (PcdPcieSlot3PwrEnableGpioNo);
  mPlatformNvsAreaProtocol.Area->PcieSlot3PowerEnableGpioPolarity = PcdGetBool (PcdPcieSlot3PwrEnableGpioPolarity);
  mPlatformNvsAreaProtocol.Area->PcieSlot3RstGpio = PcdGet32 (PcdPcieSlot3HoldRstGpioNo);
  mPlatformNvsAreaProtocol.Area->PcieSlot3RstGpioPolarity = PcdGetBool (PcdPcieSlot3HoldRstGpioPolarity);
  mPlatformNvsAreaProtocol.Area->PcieSlot3RpNumber = PcdGet8 (PcdPcieSlot3RootPort);
  //
  // PCH M.2 SSD GPIO pin configuration
  //
  mPlatformNvsAreaProtocol.Area->PchM2SsdPowerEnableGpio = PcdGet32(PcdPchSsd1PwrEnableGpioNo);
  mPlatformNvsAreaProtocol.Area->PchM2SsdPowerEnableGpioPolarity = PcdGetBool(PcdPchSsd1PwrEnableGpioPolarity);
  mPlatformNvsAreaProtocol.Area->PchM2SsdRstGpio = PcdGet32(PcdPchSsd1RstGpioNo);
  mPlatformNvsAreaProtocol.Area->PchM2SsdRstGpioPolarity = PcdGetBool(PcdPchSsd1RstGpioPolarity);

  //
  // PCH GEN5 M.2 SSD2 GPIO pin configuration
  //
  mPlatformNvsAreaProtocol.Area->M2Ssd2PowerEnableGpio = PcdGet32 (PcdM2Ssd2PowerEnableGpio);
  mPlatformNvsAreaProtocol.Area->M2Ssd2PowerEnableGpioPolarity = PcdGetBool (PcdM2Ssd2PowerEnableGpioPolarity);
  mPlatformNvsAreaProtocol.Area->M2Ssd2RstGpio = PcdGet32 (PcdM2Ssd2RstGpio);
  mPlatformNvsAreaProtocol.Area->M2Ssd2RstGpioPolarity = PcdGetBool (PcdM2Ssd2RstGpioPolarity);

  //
  // PEWAKE GPIO pin configuration
  //
  PcdRootPortPewakeConfig = (UINT32 *) PcdGetPtr (VpdPcdRootPortPewakeConfig);
  for (Index = 0; (PcdRootPortPewakeConfig[Index] != 0xFFFFFFFF) && Index < 12; Index++) {
    mPlatformNvsAreaProtocol.Area->PewakePin[Index] = PcdRootPortPewakeConfig[Index];
  }

  //
  // Platform AUX power limit configuration
  //
  mPlatformNvsAreaProtocol.Area->AuxPowerLimit = 10000; //Maximum power supported by the aux rail. In this case hardcoding to 10W

  //
  //Disable PEP constraint if VMD B0:D14:F0 device is not present
  //
  if (!IsVmdEnabled ()) {
    mSystemConfiguration.PepVmd = 0;
  }

  mPlatformNvsAreaProtocol.Area->PL1LimitCS      = mSystemConfiguration.PL1LimitCS;
  mPlatformNvsAreaProtocol.Area->PL1LimitCSValue = mSystemConfiguration.PL1LimitCSValue;
  //
  // I2C Touch Pad Data Update to I2cTouchPad.asl
  //
  mPlatformNvsAreaProtocol.Area->TPDT           = mSystemConfiguration.PchI2cTouchPadType;
  mPlatformNvsAreaProtocol.Area->TPDM           = mSystemConfiguration.PchI2cTouchPadIrqMode;
  mPlatformNvsAreaProtocol.Area->TPDB           = mSystemConfiguration.PchI2cTouchPadBusAddress;
  mPlatformNvsAreaProtocol.Area->TPDH           = mSystemConfiguration.PchI2cTouchPadHidAddress;
  mPlatformNvsAreaProtocol.Area->TPDS           = mSystemConfiguration.PchI2cTouchPadSpeed;

  //
  // I2C Touch Panel Data Update to I2cTouchPanel.asl
  //
  mPlatformNvsAreaProtocol.Area->TPLT           = mSystemConfiguration.PchI2cTouchPanelType;
  mPlatformNvsAreaProtocol.Area->TPLM           = mSystemConfiguration.PchI2cTouchPanelIrqMode;
  mPlatformNvsAreaProtocol.Area->TPLB           = mSystemConfiguration.PchI2cTouchPanelBusAddress;
  mPlatformNvsAreaProtocol.Area->TPLH           = mSystemConfiguration.PchI2cTouchPanelHidAddress;
  mPlatformNvsAreaProtocol.Area->TPLS           = mSystemConfiguration.PchI2cTouchPanelSpeed;

  mPlatformNvsAreaProtocol.Area->SDS0           = mSystemConfiguration.PchI2cSensorDevicePort[SERIAL_IO_I2C0];
  mPlatformNvsAreaProtocol.Area->SDS1           = mSystemConfiguration.PchI2cSensorDevicePort[SERIAL_IO_I2C1];
  mPlatformNvsAreaProtocol.Area->SDS2           = mSystemConfiguration.PchI2cSensorDevicePort[SERIAL_IO_I2C2];
  mPlatformNvsAreaProtocol.Area->SDS3           = mSystemConfiguration.PchI2cSensorDevicePort[SERIAL_IO_I2C3];
  mPlatformNvsAreaProtocol.Area->SDS4           = mSystemConfiguration.PchI2cSensorDevicePort[SERIAL_IO_I2C4];
  mPlatformNvsAreaProtocol.Area->SDS5           = mSystemConfiguration.PchI2cSensorDevicePort[SERIAL_IO_I2C5];
  //mPlatformNvsAreaProtocol.Area->SDS6           = mSystemConfiguration.PchSpi0SensorDevice;
  //mPlatformNvsAreaProtocol.Area->SDM6           = mSystemConfiguration.PchI2c0SensorIrqMode;
  mPlatformNvsAreaProtocol.Area->SDS7           = mSystemConfiguration.PchSpiSensorDevicePort[SERIAL_IO_SPI1];
  mPlatformNvsAreaProtocol.Area->SDS8           = mSystemConfiguration.PchSpiSensorDevicePort[SERIAL_IO_SPI2];

  mPlatformNvsAreaProtocol.Area->SDS9           = (!IsCnviWifiEnabled () || (CnvSetup.CnviMode == CnviModeDisabled)) ? CnvSetup.DiscreteBtModule : 0;

  mPlatformNvsAreaProtocol.Area->WTVX           = mSystemConfiguration.PchI2cWittVersion;
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  mPlatformNvsAreaProtocol.Area->SIOI           = mSystemConfiguration.PchIoI2cWittDeviceEnable;
  mPlatformNvsAreaProtocol.Area->SII0           = mSystemConfiguration.PchIoI2cWittDevice[0];
  mPlatformNvsAreaProtocol.Area->SII1           = mSystemConfiguration.PchIoI2cWittDevice[1];
  mPlatformNvsAreaProtocol.Area->SII2           = mSystemConfiguration.PchIoI2cWittDevice[2];
  mPlatformNvsAreaProtocol.Area->SII3           = mSystemConfiguration.PchIoI2cWittDevice[3];
  mPlatformNvsAreaProtocol.Area->SII4           = mSystemConfiguration.PchIoI2cWittDevice[4];
  mPlatformNvsAreaProtocol.Area->SII5           = mSystemConfiguration.PchIoI2cWittDevice[5];

  mPlatformNvsAreaProtocol.Area->SIOC           = mSystemConfiguration.PchIoI3cWittDeviceEnable;
  mPlatformNvsAreaProtocol.Area->SIC0           = mSystemConfiguration.PchIoI3cWittDevice[0];
  mPlatformNvsAreaProtocol.Area->SIC1           = mSystemConfiguration.PchIoI3cWittDevice[1];

  mPlatformNvsAreaProtocol.Area->SIOS           = mSystemConfiguration.PchIoSpiWittDeviceEnable;
  mPlatformNvsAreaProtocol.Area->SIS0           = mSystemConfiguration.PchIoSpiWittDevice[0];
  mPlatformNvsAreaProtocol.Area->SIS1           = mSystemConfiguration.PchIoSpiWittDevice[1];
  mPlatformNvsAreaProtocol.Area->SIS2           = mSystemConfiguration.PchIoSpiWittDevice[2];
#else
  mPlatformNvsAreaProtocol.Area->WITX           = mSystemConfiguration.PchI2cWittDevice;
#endif
  mPlatformNvsAreaProtocol.Area->UTKX           = mSystemConfiguration.PchUartUtkDevice;
  mPlatformNvsAreaProtocol.Area->GDBT           = mPchSetup.PchGpioDebounce;
  mPlatformNvsAreaProtocol.Area->GPTD           = mPchSetup.PchGpioTestDevices;
  mPlatformNvsAreaProtocol.Area->SPTD           = mPchSetup.PchAdditionalSerialIoDevices;

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  mPlatformNvsAreaProtocol.Area->TsnPcsEnabled  = mPchSetup.TsnPcsEnabled;
#else
  mPlatformNvsAreaProtocol.Area->TsnPcsEnabled  = 0;
#endif

  mPlatformNvsAreaProtocol.Area->UCMS           = PcdGet8 (PcdUcmSelection);

  //
  // Serial IO timing parameters
  //
  mPlatformNvsAreaProtocol.Area->USTP = mSystemConfiguration.PchSerialIoUseTimingParameters;

  mPlatformNvsAreaProtocol.Area->SSH0 = mSystemConfiguration.PchSerialIoTimingSSH0;  // [SSH0] SSCN-HIGH for I2C0
  mPlatformNvsAreaProtocol.Area->SSL0 = mSystemConfiguration.PchSerialIoTimingSSL0;  // [SSL0] SSCN-LOW  for I2C0
  mPlatformNvsAreaProtocol.Area->SSD0 = mSystemConfiguration.PchSerialIoTimingSSD0;  // [SSD0] SSCN-HOLD for I2C0
  mPlatformNvsAreaProtocol.Area->FMH0 = mSystemConfiguration.PchSerialIoTimingFMH0;  // [FMH0] FMCN-HIGH for I2C0
  mPlatformNvsAreaProtocol.Area->FML0 = mSystemConfiguration.PchSerialIoTimingFML0;  // [FML0] FMCN-LOW  for I2C0
  mPlatformNvsAreaProtocol.Area->FMD0 = mSystemConfiguration.PchSerialIoTimingFMD0;  // [FMD0] FMCN-HOLD for I2C0
  mPlatformNvsAreaProtocol.Area->FPH0 = mSystemConfiguration.PchSerialIoTimingFPH0;  // [FPH0] FPCN-HIGH for I2C0
  mPlatformNvsAreaProtocol.Area->FPL0 = mSystemConfiguration.PchSerialIoTimingFPL0;  // [FPL0] FPCN-LOW  for I2C0
  mPlatformNvsAreaProtocol.Area->FPD0 = mSystemConfiguration.PchSerialIoTimingFPD0;  // [FPD0] FPCN-HOLD for I2C0
  mPlatformNvsAreaProtocol.Area->HSH0 = mSystemConfiguration.PchSerialIoTimingHSH0;  // [HSH0] HSCN-HIGH for I2C0
  mPlatformNvsAreaProtocol.Area->HSL0 = mSystemConfiguration.PchSerialIoTimingHSL0;  // [HSL0] HSCN-LOW  for I2C0
  mPlatformNvsAreaProtocol.Area->HSD0 = mSystemConfiguration.PchSerialIoTimingHSD0;  // [HSD0] HSCN-HOLD for I2C0

  mPlatformNvsAreaProtocol.Area->SSH1 = mSystemConfiguration.PchSerialIoTimingSSH1;  // [SSH1] SSCN-HIGH for I2C1
  mPlatformNvsAreaProtocol.Area->SSL1 = mSystemConfiguration.PchSerialIoTimingSSL1;  // [SSL1] SSCN-LOW  for I2C1
  mPlatformNvsAreaProtocol.Area->SSD1 = mSystemConfiguration.PchSerialIoTimingSSD1;  // [SSD1] SSCN-HOLD for I2C1
  mPlatformNvsAreaProtocol.Area->FMH1 = mSystemConfiguration.PchSerialIoTimingFMH1;  // [FMH1] FMCN-HIGH for I2C1
  mPlatformNvsAreaProtocol.Area->FML1 = mSystemConfiguration.PchSerialIoTimingFML1;  // [FML1] FMCN-LOW  for I2C1
  mPlatformNvsAreaProtocol.Area->FMD1 = mSystemConfiguration.PchSerialIoTimingFMD1;  // [FMD1] FMCN-HOLD for I2C1
  mPlatformNvsAreaProtocol.Area->FPH1 = mSystemConfiguration.PchSerialIoTimingFPH1;  // [FPH1] FPCN-HIGH for I2C1
  mPlatformNvsAreaProtocol.Area->FPL1 = mSystemConfiguration.PchSerialIoTimingFPL1;  // [FPL1] FPCN-LOW  for I2C1
  mPlatformNvsAreaProtocol.Area->FPD1 = mSystemConfiguration.PchSerialIoTimingFPD1;  // [FPD1] FPCN-HOLD for I2C1
  mPlatformNvsAreaProtocol.Area->HSH1 = mSystemConfiguration.PchSerialIoTimingHSH1;  // [HSH1] HSCN-HIGH for I2C1
  mPlatformNvsAreaProtocol.Area->HSL1 = mSystemConfiguration.PchSerialIoTimingHSL1;  // [HSL1] HSCN-LOW  for I2C1
  mPlatformNvsAreaProtocol.Area->HSD1 = mSystemConfiguration.PchSerialIoTimingHSD1;  // [HSD1] HSCN-HOLD for I2C1

  mPlatformNvsAreaProtocol.Area->SSH2 = mSystemConfiguration.PchSerialIoTimingSSH2;  // [SSH2] SSCN-HIGH for I2C2
  mPlatformNvsAreaProtocol.Area->SSL2 = mSystemConfiguration.PchSerialIoTimingSSL2;  // [SSL2] SSCN-LOW  for I2C2
  mPlatformNvsAreaProtocol.Area->SSD2 = mSystemConfiguration.PchSerialIoTimingSSD2;  // [SSD2] SSCN-HOLD for I2C2
  mPlatformNvsAreaProtocol.Area->FMH2 = mSystemConfiguration.PchSerialIoTimingFMH2;  // [FMH2] FMCN-HIGH for I2C2
  mPlatformNvsAreaProtocol.Area->FML2 = mSystemConfiguration.PchSerialIoTimingFML2;  // [FML2] FMCN-LOW  for I2C2
  mPlatformNvsAreaProtocol.Area->FMD2 = mSystemConfiguration.PchSerialIoTimingFMD2;  // [FMD2] FMCN-HOLD for I2C2
  mPlatformNvsAreaProtocol.Area->FPH2 = mSystemConfiguration.PchSerialIoTimingFPH2;  // [FPH2] FPCN-HIGH for I2C2
  mPlatformNvsAreaProtocol.Area->FPL2 = mSystemConfiguration.PchSerialIoTimingFPL2;  // [FPL2] FPCN-LOW  for I2C2
  mPlatformNvsAreaProtocol.Area->FPD2 = mSystemConfiguration.PchSerialIoTimingFPD2;  // [FPD2] FPCN-HOLD for I2C2
  mPlatformNvsAreaProtocol.Area->HSH2 = mSystemConfiguration.PchSerialIoTimingHSH2;  // [HSH2] HSCN-HIGH for I2C2
  mPlatformNvsAreaProtocol.Area->HSL2 = mSystemConfiguration.PchSerialIoTimingHSL2;  // [HSL2] HSCN-LOW  for I2C2
  mPlatformNvsAreaProtocol.Area->HSD2 = mSystemConfiguration.PchSerialIoTimingHSD2;  // [HSD2] HSCN-HOLD for I2C2

  mPlatformNvsAreaProtocol.Area->SSH3 = mSystemConfiguration.PchSerialIoTimingSSH3;  // [SSH3] SSCN-HIGH for I2C3
  mPlatformNvsAreaProtocol.Area->SSL3 = mSystemConfiguration.PchSerialIoTimingSSL3;  // [SSL3] SSCN-LOW  for I2C3
  mPlatformNvsAreaProtocol.Area->SSD3 = mSystemConfiguration.PchSerialIoTimingSSD3;  // [SSD3] SSCN-HOLD for I2C3
  mPlatformNvsAreaProtocol.Area->FMH3 = mSystemConfiguration.PchSerialIoTimingFMH3;  // [FMH3] FMCN-HIGH for I2C3
  mPlatformNvsAreaProtocol.Area->FML3 = mSystemConfiguration.PchSerialIoTimingFML3;  // [FML3] FMCN-LOW  for I2C3
  mPlatformNvsAreaProtocol.Area->FMD3 = mSystemConfiguration.PchSerialIoTimingFMD3;  // [FMD3] FMCN-HOLD for I2C3
  mPlatformNvsAreaProtocol.Area->FPH3 = mSystemConfiguration.PchSerialIoTimingFPH3;  // [FPH3] FPCN-HIGH for I2C3
  mPlatformNvsAreaProtocol.Area->FPL3 = mSystemConfiguration.PchSerialIoTimingFPL3;  // [FPL3] FPCN-LOW  for I2C3
  mPlatformNvsAreaProtocol.Area->FPD3 = mSystemConfiguration.PchSerialIoTimingFPD3;  // [FPD3] FPCN-HOLD for I2C3
  mPlatformNvsAreaProtocol.Area->HSH3 = mSystemConfiguration.PchSerialIoTimingHSH3;  // [HSH3] HSCN-HIGH for I2C3
  mPlatformNvsAreaProtocol.Area->HSL3 = mSystemConfiguration.PchSerialIoTimingHSL3;  // [HSL3] HSCN-LOW  for I2C3
  mPlatformNvsAreaProtocol.Area->HSD3 = mSystemConfiguration.PchSerialIoTimingHSD3;  // [HSD3] HSCN-HOLD for I2C3

  mPlatformNvsAreaProtocol.Area->SSH4 = mSystemConfiguration.PchSerialIoTimingSSH4;  // [SSH4] SSCN-HIGH for I2C4
  mPlatformNvsAreaProtocol.Area->SSL4 = mSystemConfiguration.PchSerialIoTimingSSL4;  // [SSL4] SSCN-LOW  for I2C4
  mPlatformNvsAreaProtocol.Area->SSD4 = mSystemConfiguration.PchSerialIoTimingSSD4;  // [SSD4] SSCN-HOLD for I2C4
  mPlatformNvsAreaProtocol.Area->FMH4 = mSystemConfiguration.PchSerialIoTimingFMH4;  // [FMH4] FMCN-HIGH for I2C4
  mPlatformNvsAreaProtocol.Area->FML4 = mSystemConfiguration.PchSerialIoTimingFML4;  // [FML4] FMCN-LOW  for I2C4
  mPlatformNvsAreaProtocol.Area->FMD4 = mSystemConfiguration.PchSerialIoTimingFMD4;  // [FMD4] FMCN-HOLD for I2C4
  mPlatformNvsAreaProtocol.Area->FPH4 = mSystemConfiguration.PchSerialIoTimingFPH4;  // [FPH4] FPCN-HIGH for I2C4
  mPlatformNvsAreaProtocol.Area->FPL4 = mSystemConfiguration.PchSerialIoTimingFPL4;  // [FPL4] FPCN-LOW  for I2C4
  mPlatformNvsAreaProtocol.Area->FPD4 = mSystemConfiguration.PchSerialIoTimingFPD4;  // [FPD4] FPCN-HOLD for I2C4
  mPlatformNvsAreaProtocol.Area->HSH4 = mSystemConfiguration.PchSerialIoTimingHSH4;  // [HSH4] HSCN-HIGH for I2C4
  mPlatformNvsAreaProtocol.Area->HSL4 = mSystemConfiguration.PchSerialIoTimingHSL4;  // [HSL4] HSCN-LOW  for I2C4
  mPlatformNvsAreaProtocol.Area->HSD4 = mSystemConfiguration.PchSerialIoTimingHSD4;  // [HSD4] HSCN-HOLD for I2C4

  mPlatformNvsAreaProtocol.Area->SSH5 = mSystemConfiguration.PchSerialIoTimingSSH5;  // [SSH5] SSCN-HIGH for I2C5
  mPlatformNvsAreaProtocol.Area->SSL5 = mSystemConfiguration.PchSerialIoTimingSSL5;  // [SSL5] SSCN-LOW  for I2C5
  mPlatformNvsAreaProtocol.Area->SSD5 = mSystemConfiguration.PchSerialIoTimingSSD5;  // [SSD5] SSCN-HOLD for I2C5
  mPlatformNvsAreaProtocol.Area->FMH5 = mSystemConfiguration.PchSerialIoTimingFMH5;  // [FMH5] FMCN-HIGH for I2C5
  mPlatformNvsAreaProtocol.Area->FML5 = mSystemConfiguration.PchSerialIoTimingFML5;  // [FML5] FMCN-LOW  for I2C5
  mPlatformNvsAreaProtocol.Area->FMD5 = mSystemConfiguration.PchSerialIoTimingFMD5;  // [FMD5] FMCN-HOLD for I2C5
  mPlatformNvsAreaProtocol.Area->FPH5 = mSystemConfiguration.PchSerialIoTimingFPH5;  // [FPH5] FPCN-HIGH for I2C5
  mPlatformNvsAreaProtocol.Area->FPL5 = mSystemConfiguration.PchSerialIoTimingFPL5;  // [FPL5] FPCN-LOW  for I2C5
  mPlatformNvsAreaProtocol.Area->FPD5 = mSystemConfiguration.PchSerialIoTimingFPD5;  // [FPD5] FPCN-HOLD for I2C5
  mPlatformNvsAreaProtocol.Area->HSH5 = mSystemConfiguration.PchSerialIoTimingHSH5;  // [HSH5] HSCN-HIGH for I2C5
  mPlatformNvsAreaProtocol.Area->HSL5 = mSystemConfiguration.PchSerialIoTimingHSL5;  // [HSL5] HSCN-LOW  for I2C5
  mPlatformNvsAreaProtocol.Area->HSD5 = mSystemConfiguration.PchSerialIoTimingHSD5;  // [HSD5] HSCN-HOLD for I2C5

  mPlatformNvsAreaProtocol.Area->M0C0 = mSystemConfiguration.PchSerialIoTimingM0C0;  // [M0C0] M0D3 for I2C0
  mPlatformNvsAreaProtocol.Area->M1C0 = mSystemConfiguration.PchSerialIoTimingM1C0;  // [M1C0] M1D3 for I2C0
  mPlatformNvsAreaProtocol.Area->M0C1 = mSystemConfiguration.PchSerialIoTimingM0C1;  // [M0C1] M0D3 for I2C1
  mPlatformNvsAreaProtocol.Area->M1C1 = mSystemConfiguration.PchSerialIoTimingM1C1;  // [M1C1] M1D3 for I2C1
  mPlatformNvsAreaProtocol.Area->M0C2 = mSystemConfiguration.PchSerialIoTimingM0C2;  // [M0C2] M0D3 for I2C2
  mPlatformNvsAreaProtocol.Area->M1C2 = mSystemConfiguration.PchSerialIoTimingM1C2;  // [M1C2] M1D3 for I2C2
  mPlatformNvsAreaProtocol.Area->M0C3 = mSystemConfiguration.PchSerialIoTimingM0C3;  // [M0C3] M0D3 for I2C3
  mPlatformNvsAreaProtocol.Area->M1C3 = mSystemConfiguration.PchSerialIoTimingM1C3;  // [M1C3] M1D3 for I2C3
  mPlatformNvsAreaProtocol.Area->M0C4 = mSystemConfiguration.PchSerialIoTimingM0C4;  // [M0C4] M0D3 for I2C4
  mPlatformNvsAreaProtocol.Area->M1C4 = mSystemConfiguration.PchSerialIoTimingM1C4;  // [M1C4] M1D3 for I2C4
  mPlatformNvsAreaProtocol.Area->M0C5 = mSystemConfiguration.PchSerialIoTimingM0C5;  // [M0C5] M0D3 for I2C5
  mPlatformNvsAreaProtocol.Area->M1C5 = mSystemConfiguration.PchSerialIoTimingM1C5;  // [M1C5] M1D3 for I2C5
  mPlatformNvsAreaProtocol.Area->M0C6 = mSystemConfiguration.PchSerialIoTimingM0C6;  // [M0C6] M0D3 for SPI0
  mPlatformNvsAreaProtocol.Area->M1C6 = mSystemConfiguration.PchSerialIoTimingM1C6;  // [M1C6] M1D3 for SPI0
  mPlatformNvsAreaProtocol.Area->M0C9 = mSystemConfiguration.PchSerialIoTimingM0C9;  // [M0C9] M0D3 for UART0
  mPlatformNvsAreaProtocol.Area->M1C9 = mSystemConfiguration.PchSerialIoTimingM1C9;  // [M1C9] M1D3 for UART0
  mPlatformNvsAreaProtocol.Area->M0CA = mSystemConfiguration.PchSerialIoTimingM0CA;  // [M0CA] M0D3 for UART1
  mPlatformNvsAreaProtocol.Area->M1CA = mSystemConfiguration.PchSerialIoTimingM1CA;  // [M1CA] M1D3 for UART1
  mPlatformNvsAreaProtocol.Area->M0CB = mSystemConfiguration.PchSerialIoTimingM0CB;  // [M0CB] M0D3 for UART2
  mPlatformNvsAreaProtocol.Area->M1CB = mSystemConfiguration.PchSerialIoTimingM1CB;  // [M1CB] M1D3 for UART2

  //
  // I3C
  //
  mPlatformNvsAreaProtocol.Area->I3c0QueueThld            = mSystemConfiguration.I3c0QueueThld;            // I3C0 Queue Threshold Control
  mPlatformNvsAreaProtocol.Area->I3c0DataBufferThld       = mSystemConfiguration.I3c0DataBufferThld;       // I3C0 Data Buffer Threshold Control
  mPlatformNvsAreaProtocol.Area->I3c0OdTiming             = mSystemConfiguration.I3c0OdTiming;             // I3C0 SCL I3C Open Drain Timing
  mPlatformNvsAreaProtocol.Area->I3c0PpTiming             = mSystemConfiguration.I3c0PpTiming;             // I3C0 SCL I3C Push Pull Timing
  mPlatformNvsAreaProtocol.Area->I3c0SclI2cFmTiming       = mSystemConfiguration.I3c0SclI2cFmTiming;       // I3C0 SCL I2C Fast Mode Timing
  mPlatformNvsAreaProtocol.Area->I3c0SclI2cFmpTiming      = mSystemConfiguration.I3c0SclI2cFmpTiming;      // I3C0 SCL I2C Fast Mode Plus Timing
  mPlatformNvsAreaProtocol.Area->I3c0SclI2cSsTiming       = mSystemConfiguration.I3c0SclI2cSsTiming;       // I3C0 SCL I2C Standard Speed Timing
  mPlatformNvsAreaProtocol.Area->I3c0ExtLcntTiming        = mSystemConfiguration.I3c0ExtLcntTiming;        // I3C0 SCL Extended Low Count Timing
  mPlatformNvsAreaProtocol.Area->I3c0ExtTermLcntTiming    = mSystemConfiguration.I3c0ExtTermLcntTiming;    // I3C0 SCL Termination Bit Low Count
  mPlatformNvsAreaProtocol.Area->I3c0SdaSwitchDelayTiming = mSystemConfiguration.I3c0SdaSwitchDelayTiming; // I3C0 SDA Hold and Mode Switch Delay Timing
  mPlatformNvsAreaProtocol.Area->I3c0BusDelayTiming       = mSystemConfiguration.I3c0BusDelayTiming;       // I3C0 Speed Mode Selection

  mPlatformNvsAreaProtocol.Area->I3c2QueueThld            = mSystemConfiguration.I3c2QueueThld;            // I3C2 Queue Threshold Control
  mPlatformNvsAreaProtocol.Area->I3c2DataBufferThld       = mSystemConfiguration.I3c2DataBufferThld;       // I3C2 Data Buffer Threshold Control
  mPlatformNvsAreaProtocol.Area->I3c2OdTiming             = mSystemConfiguration.I3c2OdTiming;             // I3C2 SCL I3C Open Drain Timing
  mPlatformNvsAreaProtocol.Area->I3c2PpTiming             = mSystemConfiguration.I3c2PpTiming;             // I3C2 SCL I3C Push Pull Timing
  mPlatformNvsAreaProtocol.Area->I3c2SclI2cFmTiming       = mSystemConfiguration.I3c2SclI2cFmTiming;       // I3C2 SCL I2C Fast Mode Timing
  mPlatformNvsAreaProtocol.Area->I3c2SclI2cFmpTiming      = mSystemConfiguration.I3c2SclI2cFmpTiming;      // I3C2 SCL I2C Fast Mode Plus Timing
  mPlatformNvsAreaProtocol.Area->I3c2SclI2cSsTiming       = mSystemConfiguration.I3c2SclI2cSsTiming;       // I3C2 SCL I2C Standard Speed Timing
  mPlatformNvsAreaProtocol.Area->I3c2ExtLcntTiming        = mSystemConfiguration.I3c2ExtLcntTiming;        // I3C2 SCL Extended Low Count Timing
  mPlatformNvsAreaProtocol.Area->I3c2ExtTermLcntTiming    = mSystemConfiguration.I3c2ExtTermLcntTiming;    // I3C2 SCL Termination Bit Low Count
  mPlatformNvsAreaProtocol.Area->I3c2SdaSwitchDelayTiming = mSystemConfiguration.I3c2SdaSwitchDelayTiming; // I3C2 SDA Hold and Mode Switch Delay Timing
  mPlatformNvsAreaProtocol.Area->I3c2BusDelayTiming       = mSystemConfiguration.I3c2BusDelayTiming;       // I3C2 Speed Mode Selection

  mPlatformNvsAreaProtocol.Area->Revision       = PLATFORM_NVS_AREA_REVISION_1; // for TPM related fields
  if (mSystemConfiguration.FastBoot == 0) {
    mPlatformNvsAreaProtocol.Area->Ps2MouseEnable = IsPs2MouseConnected ();
  } else {
    mPlatformNvsAreaProtocol.Area->Ps2MouseEnable = 0;
  }
  mPlatformNvsAreaProtocol.Area->ScanMatrixEnable  = mSystemConfiguration.ScanMatrixEnable;
  mPlatformNvsAreaProtocol.Area->IpfEnable         = PcdGetBool (VpdPcdIpfEnable);

  //
  // Feature Specific RVP Details
  //
  mPlatformNvsAreaProtocol.Area->UsbTypeCSupport                      = PcdGet8 (VpdPcdUsbCUcmMode);
  mPlatformNvsAreaProtocol.Area->PcdTsOnDimmTemperature               = PcdGetBool (PcdTsOnDimmTemperature);
  mPlatformNvsAreaProtocol.Area->PcdMipiCamSensor                     = PcdGetBool (PcdMipiCamSensor);
  mPlatformNvsAreaProtocol.Area->PcdEcUart                            = PcdGetBool (PcdEcUart);
  mPlatformNvsAreaProtocol.Area->PcdConvertableDockSupport            = PcdGetBool (PcdConvertableDockSupport);
  mPlatformNvsAreaProtocol.Area->PcdAcpiEnableAllButtonSupport        = PcdGetBool (PcdAcpiEnableAllButtonSupport);
  mPlatformNvsAreaProtocol.Area->PcdAcpiHidDriverButtonSupport        = PcdGetBool (PcdAcpiHidDriverButtonSupport);

  //
  //Hebc Value Support
  //
  SetupData.HebcValueSupport = PcdGetBool (PcdAcpiHidDriverButtonSupport);
  Status = gRT->SetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  SetupAttr,
                  VariableSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  DEBUG ((DEBUG_INFO, "HebcValueSupport = %d\n", SetupData.HebcValueSupport));

  // USBC DR_SWAP
  mPlatformNvsAreaProtocol.Area->UsbcDataRoleSwap = (mSystemConfiguration.UsbcDataRoleSwapPlatformDisable) ? 1 : 0;

  //
  // Always report PS2MouseEnable for non-EC board
  //
#if FixedPcdGetBool (PcdEcEnable) == 0
  mPlatformNvsAreaProtocol.Area->Ps2MouseEnable = 1;
#endif

  mPlatformNvsAreaProtocol.Area->UsbSensorHub    = mSystemConfiguration.UsbSensorHub;

  //
  // Lch configuration
  //
  mPlatformNvsAreaProtocol.Area->LchSupport = mSystemConfiguration.LchSupport;
  mPlatformNvsAreaProtocol.Area->LchTestDevice = mSystemConfiguration.LchTestDevice;
  mPlatformNvsAreaProtocol.Area->LchIrqGpioPin = PcdGet32(PcdLchIrqGpioPin);
  mPlatformNvsAreaProtocol.Area->LchResetGpioPin  = PcdGet32(PcdLchResetGpioPin);

  // HDAudio Configuration
  //
#if (FixedPcdGet8(PcdEmbeddedEnable) == 0x1)
  mPlatformNvsAreaProtocol.Area->EVSA = 0x18;     //  Everest8326 I2c slave address
#endif
  mPlatformNvsAreaProtocol.Area->I2SC = mPchSetup.PchHdAudioI2sCodecSelect;
  mPlatformNvsAreaProtocol.Area->I2SI = PcdGet32 (PcdHdaI2sCodecIrqGpio);
  mPlatformNvsAreaProtocol.Area->I2SB = PcdGet8 (PcdHdaI2sCodecI2cBusNumber);

  DEBUG ((DEBUG_INFO, "HDA: I2S Codec Select [I2SC] = 0x%02x\n", mPlatformNvsAreaProtocol.Area->I2SC));
  DEBUG ((DEBUG_INFO, "HDA: I2S Codec JD Int pin [I2SI] = 0x%08x\n", mPlatformNvsAreaProtocol.Area->I2SI));
  DEBUG ((DEBUG_INFO, "HDA: I2S Codec I2C Bus Number [I2SB] = 0x%02x\n", mPlatformNvsAreaProtocol.Area->I2SB));

  for (Index = 0; Index < HDAUDIO_PP_MODULES; Index++) {
    mPlatformNvsAreaProtocol.Area->HdaDspPpModuleMask |= (UINT32)(mPchSetup.PchHdAudioPostProcessingMod[Index] ? (1 << Index) : 0);
  }
  DEBUG((DEBUG_INFO,"HDA: HdaDspPpModuleMask [ADPM] = 0x%08x\n", mPlatformNvsAreaProtocol.Area->HdaDspPpModuleMask));

  if (mPchSetup.PchHdAudioPostProcessingMod[29]){
    DEBUG ((DEBUG_INFO, "HDA: AudioDSP Pre/Post-Processing custom module 'Alpha' enabled (BIT29)\n"));

    CopyMem (LocalGuidString, mPchSetup.PchHdAudioPostProcessingModCustomGuid1, GUID_CHARS_NUMBER * sizeof(CHAR16));
    GuidStringToAcpiBuffer (LocalGuidString, &AcpiGuidPart1, &AcpiGuidPart2);
    mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid1Low  = AcpiGuidPart1;
    mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid1High = AcpiGuidPart2;
    DEBUG ((DEBUG_INFO, "HdaDspPpModCustomGuid1Low  = 0x%016Lx\nHdaDspPpModCustomGuid2High = 0x%016Lx\n",
            mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid1Low, mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid1High));
  }

  if (mPchSetup.PchHdAudioPostProcessingMod[30]){
    DEBUG ((DEBUG_INFO, "HDA: AudioDSP Pre/Post-Processing custom module 'Beta' enabled (BIT30)\n"));

    CopyMem (LocalGuidString, mPchSetup.PchHdAudioPostProcessingModCustomGuid2, GUID_CHARS_NUMBER * sizeof(CHAR16));
    GuidStringToAcpiBuffer (LocalGuidString, &AcpiGuidPart1, &AcpiGuidPart2);
    mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid2Low  = AcpiGuidPart1;
    mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid2High = AcpiGuidPart2;
    DEBUG ((DEBUG_INFO, "HdaDspPpModCustomGuid2Low  = 0x%016Lx\nHdaDspPpModCustomGuid2High = 0x%016Lx\n",
            mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid2Low, mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid2High));
  }

  if (mPchSetup.PchHdAudioPostProcessingMod[31]){
    DEBUG ((DEBUG_INFO, "HDA: AudioDSP Pre/Post-Processing custom module 'Gamma' enabled (BIT31)\n"));

    CopyMem (LocalGuidString, mPchSetup.PchHdAudioPostProcessingModCustomGuid3, GUID_CHARS_NUMBER * sizeof(CHAR16));
    GuidStringToAcpiBuffer (LocalGuidString, &AcpiGuidPart1, &AcpiGuidPart2);
    mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid3Low  = AcpiGuidPart1;
    mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid3High = AcpiGuidPart2;
    DEBUG ((DEBUG_INFO, "HdaDspPpModCustomGuid3Low  = 0x%016Lx\nHdaDspPpModCustomGuid3High = 0x%016Lx\n",
            mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid3Low, mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid3High));
  }

  if (mPchSetup.PchHdAudioPostProcessingMod[25]){
    DEBUG((DEBUG_INFO, "HDA: AudioDSP Pre/Post-Processing custom module 'Delta' enabled (BIT25)\n"));

    CopyMem (LocalGuidString, mPchSetup.PchHdAudioPostProcessingModCustomGuid4, GUID_CHARS_NUMBER * sizeof(CHAR16));
    GuidStringToAcpiBuffer (LocalGuidString, &AcpiGuidPart1, &AcpiGuidPart2);
    mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid4Low  = AcpiGuidPart1;
    mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid4High = AcpiGuidPart2;
    DEBUG ((DEBUG_INFO, "HdaDspPpModCustomGuid4Low  = 0x%016Lx\nHdaDspPpModCustomGuid4High = 0x%016Lx\n",
            mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid4Low, mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid4High));
  }

  if (mPchSetup.PchHdAudioPostProcessingMod[26]){
    DEBUG ((DEBUG_INFO, "HDA: AudioDSP Pre/Post-Processing custom module 'Epsilon' enabled (BIT26)\n"));

    CopyMem (LocalGuidString, mPchSetup.PchHdAudioPostProcessingModCustomGuid5, GUID_CHARS_NUMBER * sizeof(CHAR16));
    GuidStringToAcpiBuffer (LocalGuidString, &AcpiGuidPart1, &AcpiGuidPart2);
    mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid5Low  = AcpiGuidPart1;
    mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid5High = AcpiGuidPart2;
    DEBUG ((DEBUG_INFO, "HdaDspPpModCustomGuid5Low  = 0x%016Lx\nHdaDspPpModCustomGuid5High = 0x%016Lx\n",
            mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid5Low, mPlatformNvsAreaProtocol.Area->HdaDspPpModCustomGuid5High));
  }

  ///
  /// PMAX Configuration
  ///
  mPlatformNvsAreaProtocol.Area->PmaxDevice = mSystemConfiguration.PmaxDevice;
  mPlatformNvsAreaProtocol.Area->PmaxAudioCodec = mSystemConfiguration.PmaxAudioCodec;
  mPlatformNvsAreaProtocol.Area->PmaxWfCamera = mSystemConfiguration.PmaxWfCamera;
  mPlatformNvsAreaProtocol.Area->PmaxUfCamera = mSystemConfiguration.PmaxUfCamera;
  mPlatformNvsAreaProtocol.Area->PmaxFlashDevice = mSystemConfiguration.PmaxFlashDevice;

  //
  // Initialize PCIE device ON/OFF delay
  //
  mPlatformNvsAreaProtocol.Area->PcieDevOnOffDelay = mSystemConfiguration.PcieDevOnOffDelay;

  ///
  /// Get TcssHob HOB
  ///
#if FixedPcdGetBool(PcdTcssSupport) == 1
  mTcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
#endif
  mPlatformNvsAreaProtocol.Area->PciDelayOptimizationEcr = mSystemConfiguration.PciDelayOptimizationEcr;

  if ((VtioSetupData.SdevXhciEntry == TRUE) && (VtioSetupData.SdevXhciNumberOfDevices > 0)) {
    mPlatformNvsAreaProtocol.Area->SdevXhciInterfaceNumber1 = VtioSetupData.SdevXhciInterfaceNumber[0];
    mPlatformNvsAreaProtocol.Area->SdevXhciRootPortNumber1  = VtioSetupData.SdevXhciRootPortNumber[0];
    if (VtioSetupData.SdevXhciNumberOfDevices > 1) {
      mPlatformNvsAreaProtocol.Area->SdevXhciInterfaceNumber2 = VtioSetupData.SdevXhciInterfaceNumber[1];
      mPlatformNvsAreaProtocol.Area->SdevXhciRootPortNumber2  = VtioSetupData.SdevXhciRootPortNumber[1];
    }
    for (Index = 0; Index < 2; Index++) {
      if (VtioSetupData.SdevXhciRootPortNumber[Index] > 0) {
        UpdateUsbCameraConnector (VtioSetupData.SdevXhciRootPortNumber[Index]);
      }
    }
  }

  mPlatformNvsAreaProtocol.Area->SdevXhciEntry = VtioSetupData.SdevXhciEntry;

  mPlatformNvsAreaProtocol.Area->DeepestUSBSleepWakeCapability = mSystemConfiguration.DeepestUSBSleepWakeCapability;

  mPlatformNvsAreaProtocol.Area->Gpe1BlockEnable = mSystemConfiguration.Gpe1BlockEnable;

  PreBootCmMode      = 0;
  CmTbtMask          = 0;
  Usb4CmSwitchEnable = 0;

  //
  // Find USB4 Hob to get platform USB4 host router information
  //
  HobPtr = NULL;
  HobPtr = GetFirstGuidHob (&gUsb4PlatformHobGuid);
  if (HobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Unable to find USB4 platform hob!\n"));
  } else {
    Usb4PlatformHob = GET_GUID_HOB_DATA (HobPtr);
    Usb4PlatformInfo = &(Usb4PlatformHob->Usb4PlatformInfo);
    //
    // Bit 7   - Valid bit
    // Bit 6:4 - Setup bit
    // Bit 3   - Reserved
    // Bit 2:0 - Pre-boot CM mode
    //
    PreBootCmMode = (Usb4PlatformInfo->CmModeOption & 0x07) << 4;
    PreBootCmMode |= (Usb4PlatformInfo->CmMode & 0x07);
    PreBootCmMode |= 0x80;

    CmTbtMask          = Usb4PlatformInfo->Usb4HrMask;
    Usb4CmSwitchEnable = Usb4PlatformInfo->CmSwitchSupport;
  }

  mPlatformNvsAreaProtocol.Area->PreBootCmMode               = PreBootCmMode;
  mPlatformNvsAreaProtocol.Area->CmTbtMask                   = CmTbtMask;
  mPlatformNvsAreaProtocol.Area->Usb4CmSwitchEnable          = Usb4CmSwitchEnable;
  if (Usb4PlatformInfo != NULL) {
    mPlatformNvsAreaProtocol.Area->EnablePcieTunnelingOverUsb4 = Usb4PlatformInfo->PcieOverUsb4En;
  }
#if FixedPcdGetBool (PcdDTbtEnable) == 1
  mPlatformNvsAreaProtocol.Area->TbtRTD3En                   = mSystemConfiguration.DTbtRtd3;
#endif
  DEBUG ((DEBUG_INFO, "PreBootCmMode = 0x%x, CmTbtMask = 0x%x, Usb4CmSwitchEnable = 0x%x\n",
          mPlatformNvsAreaProtocol.Area->PreBootCmMode,
          mPlatformNvsAreaProtocol.Area->CmTbtMask,
          mPlatformNvsAreaProtocol.Area->Usb4CmSwitchEnable
          ));

  //Update RMTE NVS Area
  mPlatformNvsAreaProtocol.Area->RMTE = mRmtdata.EnDsRmt;

  if (mSystemConfiguration.AcpiDebug) {
    TableLoadBufferSize = 16 * 1024;
    Status = AllocateMemoryBelow4G (
               EfiACPIMemoryNVS,
               TableLoadBufferSize,
               &TableLoadBuffer
               );
    ASSERT_EFI_ERROR(Status);
    mPlatformNvsAreaProtocol.Area->TableLoadBuffer = (UINT32) (UINTN) TableLoadBuffer;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gPlatformNvsAreaProtocolGuid,
                  &mPlatformNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Allocate NVS area for VBIOS. This is not currently used.
  //
  Status = AllocateMemoryBelow4G (
             EfiACPIMemoryNVS,
             0x10000,
             &mGfxNvsAddress
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Generate ACPI tables from configuration data
  //
  Status = GenerateSsdtFromConfigFile();
  ASSERT_EFI_ERROR (Status);

  //
  // Update FADT Setting
  //
  Status = UpdateFadtSetting();
  ASSERT_EFI_ERROR (Status);

  //
  // Platform ACPI Tables
  //
  PublishAcpiTablesFromFv(gAcpiTableStorageGuid);

  //
  // Reference Code ACPI Tables
  //
  PublishAcpiTablesFromFv(gRcAcpiTableStorageGuid);

#if FixedPcdGetBool (PcdEcEnable) == 1
  Status = EcInitialize (&CpuSetup);
#endif

  //
  // Finished
  //
  return EFI_SUCCESS;
}

/**
  I/O work flow to wait input buffer empty in given time.

  @param[in] Timeout       Wating time.

  @retval EFI_TIMEOUT      if input is still not empty in given time.
  @retval EFI_SUCCESS      input is empty.
**/
EFI_STATUS
WaitInputEmpty (
  IN UINTN Timeout
  )
{
  UINTN Delay;
  UINT8 Data;

  Delay = Timeout / 50;

  do {
    Data = IoRead8 (KBC_CMD_STS_PORT);

    //
    // Check keyboard controller status bit 0 and 1 (Output Buffer and Input Buffer status)
    //
    if ((Data & KBC_OUTB) != 0) {
      IoRead8 (KBC_DATA_PORT);
    } else if ((Data & KBC_INPB) == 0) {
      break;
    }

    gBS->Stall (50);
    Delay--;
  } while (Delay != 0);

  if (Delay == 0) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/**
  I/O work flow to wait output buffer full in given time.

  @param[in] Timeout       given time

  @retval EFI_TIMEOUT      output is not full in given time
  @retval EFI_SUCCESS      output is full in given time.
**/
EFI_STATUS
WaitOutputFull (
  IN UINTN Timeout
  )
{
  UINTN Delay;
  UINT8 Data;

  Delay = Timeout / 50;

  do {
    Data = IoRead8 (KBC_CMD_STS_PORT);

    //
    // Check keyboard controller status bit 0(output buffer status)
    //  & bit5(output buffer for auxiliary device)
    //
    if ((Data & (KBC_OUTB | KBC_AUXB)) == (KBC_OUTB | KBC_AUXB)) {
      break;
    }

    gBS->Stall (50);
    Delay--;
  } while (Delay != 0);

  if (Delay == 0) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/**
  I/O work flow of in 8042 Aux data.

  @param Data    Buffer holding return value.

  @retval EFI_SUCCESS Success to excute I/O work flow
  @retval EFI_TIMEOUT Keyboard controller time out.
**/
EFI_STATUS
In8042AuxData (
  IN OUT UINT8 *Data
  )
{
  EFI_STATUS Status;

  //
  // wait for output data
  //
  Status = WaitOutputFull (BAT_TIMEOUT);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *Data = IoRead8 (KBC_DATA_PORT);

  return EFI_SUCCESS;
}

/**
  I/O work flow of outing 8042 Aux command.

  @param Command Aux I/O command

  @retval EFI_SUCCESS Success to excute I/O work flow
  @retval EFI_TIMEOUT Keyboard controller time out.
**/
EFI_STATUS
Out8042AuxCommand (
  IN UINT8 Command
  )
{
  EFI_STATUS  Status;
  UINT8       Data;

  //
  // Wait keyboard controller input buffer empty
  //
  Status = WaitInputEmpty (TIMEOUT);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Send write to auxiliary device command
  //
  Data = WRITE_AUX_DEV;
  IoWrite8 (KBC_CMD_STS_PORT, Data);

  Status = WaitInputEmpty (TIMEOUT);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Send auxiliary device command
  //
  IoWrite8 (KBC_DATA_PORT, Command);

  //
  // Read return code
  //
  Status = In8042AuxData (&Data);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Data == PS2_ACK) {
    //
    // Receive mouse acknowledge, command send success
    //
    return EFI_SUCCESS;

  } else if (Data == PS2_RESEND) {
    //
    // Resend command
    //
    Status = Out8042AuxCommand (Command);
    if (EFI_ERROR (Status)) {
      return Status;
    }

  } else {
    //
    // Invalid return code
    //
    return EFI_DEVICE_ERROR;

  }

  return EFI_SUCCESS;
}

/**
  Issue command to enable Ps2 mouse.

  @return Status of command issuing.
**/
EFI_STATUS
PS2MouseEnable (
  VOID
  )
{
  //
  // Send auxiliary command to enable mouse
  //
  return Out8042AuxCommand (ENABLE_CMD);
}

/**
  Check whether there is Ps/2 mouse device in system

  @retval TRUE      - Keyboard in System.
  @retval FALSE     - Keyboard not in System.

**/
BOOLEAN
IsPs2MouseConnected (
  VOID
  )
{
  EFI_STATUS Status;

  Status = PS2MouseEnable ();

  if (!EFI_ERROR (Status)) {
    return TRUE;
  }

  return FALSE;
}
