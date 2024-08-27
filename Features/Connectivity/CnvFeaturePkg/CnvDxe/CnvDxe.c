/** @file
  CNV Init DXE Driver

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

@par Specification
**/

#include <Uefi.h>
#include <Uefi/UefiSpec.h>
#include <Protocol/CnvNvsArea.h>
#include <PiDxe.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TpmMeasurementLib.h>
#include <CnvVfrSetupMenuHii.h>
#include <CnvBoardConfigPcd.h>
#include <CnvUefiConfigVariables.h>
#include <Protocol/CnvUefiProtocol.h>
#include <IndustryStandard/Pci.h>
#include <IndustryStandard/Pci22.h>
#include <Library/PciSegmentLib.h>
#include <Protocol/PciIo.h>
#include <Include/CnvRegisters.h>

#define CNV_R_PCIE_DCAP_OFFSET  0x04                // Device Capabilities Register (Offset 04h)

#define VENDOR_SPECIFIC_EXTENDED_CAP_ID  0x0B
#define PCI_VSEC_CAPABILITY_ID_OFFSET    4
#define V_VSEC_CAPABILITY_ID             0x0023
#define PCI_VSEC_NEXT_CAPABILITY_OFFSET  2
#define CNV_R_PCIE_CFG_EXCAP_OFFSET      0x100

/**
Macro that checks whether device is a WLAN module
  @param  _p      Specified device.
  @retval TRUE    Device is a WLAN module.
  @retval FALSE   Device is not a WLAN module.
**/
#define IS_PCI_WLAN(_p)  IS_CLASS2 (_p, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER)

GLOBAL_REMOVE_IF_UNREFERENCED CNV_NVS_AREA_PROTOCOL              mCnvNvsAreaProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED CNV_BOARD_CONFIG                   *mCnvDxeBoardConfigPtr;
GLOBAL_REMOVE_IF_UNREFERENCED CNV_VFR_CONFIG_SETUP               mCnvSetup;
GLOBAL_REMOVE_IF_UNREFERENCED CNV_UEFI_VARIABLE_CONFIG_PROTOCOL  mCnvUefiVariableConfigProtocol;

/**
  Initialize Connectivity SSDT ACPI tables.

  @retval EFI_SUCCESS    ACPI tables are initialized successfully
  @retval EFI_NOT_FOUND  ACPI tables not found
**/
EFI_STATUS
InitializeCnvSsdtAcpiTables (
  VOID
  )
{
  EFI_STATUS                     Status;
  EFI_HANDLE                     *HandleBuffer;
  UINTN                          NumberOfHandles;
  EFI_FV_FILETYPE                FileType;
  UINT32                         FvStatus;
  EFI_FV_FILE_ATTRIBUTES         Attributes;
  UINTN                          Size;
  UINTN                          i;
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol;
  INTN                           Instance;
  EFI_ACPI_COMMON_HEADER         *CurrentTable;
  UINTN                          AcpiTableKey;
  UINT8                          *CurrPtr;
  UINT8                          *EndPtr;
  UINT32                         *Signature;
  EFI_ACPI_DESCRIPTION_HEADER    *CnvAcpiTable;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTable;

  FwVol        = NULL;
  CnvAcpiTable = NULL;
  ///
  /// Locate ACPI Table protocol
  ///
  DEBUG ((DEBUG_INFO, "Init CNV SSDT table\n"));
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_WARN, "[CNV] Fail to locate EfiAcpiTableProtocol.\n"));
    return EFI_NOT_FOUND;
  }

  ///
  /// Locate protocol.
  /// There is little chance we can't find an FV protocol
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Looking for FV with ACPI storage file
  ///
  for (i = 0; i < NumberOfHandles; i++) {
    ///
    /// Get the protocol on this handle
    /// This should not fail because of LocateHandleBuffer
    ///
    Status = gBS->HandleProtocol (
                    HandleBuffer[i],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **)&FwVol
                    );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    ///
    /// See if it has the ACPI storage file
    ///
    Size     = 0;
    FvStatus = 0;
    Status   = FwVol->ReadFile (
                      FwVol,
                      &gEfiCallerIdGuid,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );
    ///
    /// If we found it, then we are done
    ///
    if (Status == EFI_SUCCESS) {
      break;
    }
  }

  ///
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);

  ///
  /// Sanity check that we found our data file
  ///
  if (FwVol == NULL) {
    DEBUG ((DEBUG_INFO, "CNV Global NVS table not found\n"));
    ASSERT (FwVol != NULL);
    return EFI_NOT_FOUND;
  }

  ///
  /// Our exit status is determined by the success of the previous operations
  /// If the protocol was found, Instance already points to it.
  /// Read tables from the storage file.
  ///
  Instance     = 0;
  CurrentTable = NULL;
  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiCallerIdGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **)&CurrentTable,
                      &Size,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      ///
      /// Check the table ID to modify the table
      ///
      if (((EFI_ACPI_DESCRIPTION_HEADER *)CurrentTable)->OemTableId == SIGNATURE_64 ('C', 'n', 'v', '_', 'S', 's', 'd', 't')) {
        CnvAcpiTable = (EFI_ACPI_DESCRIPTION_HEADER *)CurrentTable;
        ///
        /// Locate the SSDT package
        ///
        CurrPtr = (UINT8 *) CnvAcpiTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
        EndPtr  = (UINT8 *) CnvAcpiTable + CnvAcpiTable->Length - sizeof (*Signature);

        for ( ; CurrPtr <= EndPtr; CurrPtr++) {
          Signature = (UINT32 *)(CurrPtr + 3);
          if (*Signature == SIGNATURE_32 ('C', 'N', 'N', 'V')) {
            ASSERT (*(UINT32 *)(CurrPtr + 3 + sizeof (*Signature) + 2) == 0xFFFF0000);
            if (*(UINT32 *)(CurrPtr + 3 + sizeof (*Signature) + 2) != 0xFFFF0000) {
              return EFI_NOT_FOUND;
            }

            ASSERT (*(UINT16 *)(CurrPtr + 3 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) == 0xAA55);
            if (*(UINT16 *)(CurrPtr + 3 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) != 0xAA55) {
              return EFI_NOT_FOUND;
            }

            ///
            /// CNV Global NVS Area address
            ///
            *(UINT32 *)(CurrPtr + 3 + sizeof (*Signature) + 2) = (UINT32)(UINTN)mCnvNvsAreaProtocol.Area;
            ///
            /// CNV Global NVS Area size
            ///
            *(UINT16 *)(CurrPtr + 3 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) = sizeof (CNV_PKG_NVS_AREA);

            AcpiTableKey = 0;
            Status       = AcpiTable->InstallAcpiTable (
                                  AcpiTable,
                                  CnvAcpiTable,
                                  CnvAcpiTable->Length,
                                  &AcpiTableKey
                                  );
            ASSERT_EFI_ERROR (Status);
            gBS->FreePool (CnvAcpiTable);
            return EFI_SUCCESS;
          }
        }
      }

      ///
      /// Increment the instance
      ///
      Instance++;
      gBS->FreePool (CurrentTable);
      CurrentTable = NULL;
    }
  }

  return Status;
}

/**
  CNV NVS Area Initialize

  @param[in] mCnvNvsAreaPtr      Pointer to CNV NVS Area.
**/
VOID
CnvNvsAreaInit (
  IN  VOID  **mCnvNvsAreaPtr
  )
{
  UINTN                 Pages;
  EFI_PHYSICAL_ADDRESS  Address;
  EFI_STATUS            Status;

  DEBUG ((DEBUG_INFO, "CnvNvsAreaInit Start\n"));

  Pages   = EFI_SIZE_TO_PAGES (sizeof (CNV_PKG_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.

  Status = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  *mCnvNvsAreaPtr = (VOID *)(UINTN)Address;
  SetMem (*mCnvNvsAreaPtr, sizeof (CNV_PKG_NVS_AREA), 0);

  DEBUG ((DEBUG_INFO, "CnvNvsAreaInit End\n"));
}

/**
  CNV NVS Area Update
**/
VOID
CnvNvsAreaUpdate (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "CnvNvsAreaUpdate Start\n"));

  //
  // CnvNvsAreaProtocol default value init here
  //
  mCnvNvsAreaProtocol.Area->SkipVidDidCheck        = mCnvSetup.SkipVidDidCheck;
  mCnvNvsAreaProtocol.Area->DiscreteWlanRfKillGpio = mCnvDxeBoardConfigPtr->WlanRfKillGpio;
  mCnvNvsAreaProtocol.Area->WlanWakeGpio           = mCnvDxeBoardConfigPtr->WlanWakeGpio;
  mCnvNvsAreaProtocol.Area->WlanRstGpio            = mCnvDxeBoardConfigPtr->WlanRstGpio;
  mCnvNvsAreaProtocol.Area->WlanRootPortNumber     = mCnvDxeBoardConfigPtr->WlanRootPortNumber;

  mCnvNvsAreaProtocol.Area->BtModuleRfKillGpio  = mCnvDxeBoardConfigPtr->BtRfKillGpio;
  mCnvNvsAreaProtocol.Area->BtModuleIrqGpio     = mCnvDxeBoardConfigPtr->BtIrqGpio;
  mCnvNvsAreaProtocol.Area->DiscreteUsbBtPort   = mCnvDxeBoardConfigPtr->DiscreteUsbBtPortNumber;
  mCnvNvsAreaProtocol.Area->IntegratedUsbBtPort = mCnvDxeBoardConfigPtr->IntegratedUsbBtPortNumber;
  mCnvNvsAreaProtocol.Area->CnvDlvrRfim         = PcdGet8 (PcdGlobalCnvDlvrRfim);


  mCnvNvsAreaProtocol.Area->CnvGuidLockStatus =  PcdGet8 (PcdCnvGuidLockStatus);


  DEBUG ((DEBUG_INFO, "CnvNvsAreaUpdate End\n"));
}

/**
  This function gets registered as a callback to patch CNV ASL code

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
CnvAcpiEndOfDxeCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
}

/**
  This function updates variable.

**/
VOID
EFIAPI
CnvUpdateVariable (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       DataSize;
  UINT32      VariableAttr;

  //
  // 1. Assign defaults to mCnvSetup by FixedAtBuild PCD
  //
  mCnvSetup.SkipVidDidCheck = FixedPcdGetBool (PcdSkipVidDidCheck);

  //
  // 2. GetVariable - gCnvFeatureSetupGuid
  //
  // EFI_SUCCESS    - mCnvSetup will be overridden by setup variable in flash or Hii structure PCD default
  // Others         - mCnvSetup will be untouched and hold the values assigned by step 1
  //
  DataSize = sizeof (CNV_VFR_CONFIG_SETUP);
  Status   = gRT->GetVariable (
                    CNV_SETUP_VARIABLE_NAME,
                    &gCnvFeatureSetupGuid,
                    &VariableAttr,
                    &DataSize,
                    &mCnvSetup
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get gCnvFeatureSetupGuid failed\n"));
    return;
  }
}

/**
 Return whether the Handle is a WLAN module handle.

  @param Handle  The handle to be tested.
  @retval TRUE   The handle is a WLAN handle.
  @retval FALSE  The handle is not a WLAN handle..
**/
BOOLEAN
IsIntelWlanHandle (
  IN EFI_HANDLE  Handle,
  IN OUT UINTN   *Segment,
  IN OUT UINTN   *Bus,
  IN OUT UINTN   *Device,
  IN OUT UINTN   *Function
  )
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  PCI_TYPE00           Pci;
  EFI_STATUS           Status;

  PciIo = NULL;

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **)&PciIo
                  );
  if ((!EFI_ERROR (Status)) && (PciIo != NULL)) {
    Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint32,
                          0,
                          sizeof (Pci) / sizeof (UINT32),
                          &Pci
                          );
    if ((!EFI_ERROR (Status)) && (Pci.Hdr.VendorId == 0x8086)) {
      if (IS_PCI_WLAN (&Pci)) {
        PciIo->GetLocation (PciIo, Segment, Bus, Device, Function);
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
  Finds the Offset to a given Capabilities ID for CnvFeaturePkg
  Each capability has an ID and a pointer to next Capability, so they form a linked list.
  This function walks the list of Capabilities present in device's pci cfg. If requested capability
  can be found, its offset is returned.
  If the capability can't be found or if device doesn't exist, function returns 0
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] DeviceBase           Device's base address
  @param[in] CapId                CAPID to search for

  @retval 0                       CAPID not found (this includes situation where device doesn't exit)
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT8
CnvPcieBaseFindCapId (
  IN UINT64  DeviceBase,
  IN UINT8   CapId
  )
{
  UINT8   CapHeaderOffset;
  UINT8   CapHeaderId;
  UINT16  Data16;

  //
  // We do not explicitly check if device exists to save time and avoid unnecessary PCI access
  // If the device doesn't exist, check for CapHeaderId != 0xFF will fail and function will return offset 0
  //
  if ((PciSegmentRead8 (DeviceBase + PCI_PRIMARY_STATUS_OFFSET) & EFI_PCI_STATUS_CAPABILITY) != 0x00) {
    //
    // Check the header layout to determine the Offset of Capabilities Pointer Register
    //
    if ((PciSegmentRead8 (DeviceBase + PCI_HEADER_TYPE_OFFSET) & HEADER_LAYOUT_CODE) == (HEADER_TYPE_DEVICE)) {
      //
      // Check PCI Type as device.
      //
      CapHeaderOffset = PCI_CAPBILITY_POINTER_OFFSET;
    } else {
      ASSERT ((FALSE));
      return 0;
    }

    //
    // Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
    //
    CapHeaderId     = 0;
    CapHeaderOffset = PciSegmentRead8 (DeviceBase + CapHeaderOffset) & ((UINT8) ~(BIT0 | BIT1));
    while (CapHeaderOffset != 0 && CapHeaderId != 0xFF) {
      Data16      = PciSegmentRead16 (DeviceBase + CapHeaderOffset);
      CapHeaderId = (UINT8)(Data16 & 0xFF);
      if (CapHeaderId == CapId) {
        if (CapHeaderOffset > PCI_MAXLAT_OFFSET) {
          //
          // Return valid capability offset
          //
          return CapHeaderOffset;
        } else {
          ASSERT ((FALSE));
          return 0;
        }
      }

      //
      // Each capability must be DWORD aligned.
      // The bottom two bits of all pointers (including the initial pointer at 34h) are reserved
      // and must be implemented as 00b although software must mask them to allow for future uses of these bits.
      //
      CapHeaderOffset = (UINT8)(Data16 >> 8);
    }

    return 0;
  } else {
    //
    // Function has no capability pointer
    //
    return 0;
  }
}

/**
  Search and return the offset of desired Pci Express Capability ID for CnvFeaturePkg
  CAPID list:
    0x0001 = Advanced Error Reporting Capability
    0x0002 = Virtual Channel Capability
    0x0003 = Device Serial Number Capability
    0x0004 = Power Budgeting Capability

  @param[in] DeviceBase           Device base address
  @param[in] CapId                Extended CAPID to search for

  @retval 0                       CAPID not found, this includes situation where device doesn't exist
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT16
CnvPcieBaseFindExtendedCapId (
  IN UINT64  DeviceBase,
  IN UINT16  CapId
  )
{
  UINT16  CapHeaderOffset;
  UINT16  CapHeaderId;

  ///
  /// Start to search at Offset 0x100
  /// Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
  ///
  CapHeaderId     = 0;
  CapHeaderOffset = CNV_R_PCIE_CFG_EXCAP_OFFSET;

  while (CapHeaderOffset != 0 && CapHeaderId != MAX_UINT16) {
    CapHeaderId = PciSegmentRead16 (DeviceBase + CapHeaderOffset);
    if (CapHeaderId == CapId) {
      return CapHeaderOffset;
    }

    ///
    /// Each capability must be DWORD aligned.
    /// The bottom two bits of all pointers are reserved and must be implemented as 00b,
    /// although software must mask them to allow for future uses of these bits.
    ///
    CapHeaderOffset = (PciSegmentRead16 (DeviceBase + CapHeaderOffset + 2) >> 4) & ((UINT16) ~(BIT0 | BIT1));
  }

  return 0;
}

/**
 Find WNIC CSR Capability Header and Device Capability Register offset
**/
VOID
FindWlanVsecHeaderAndDevCapOffset (
  VOID
)
{
  UINT64      EpBase;
  UINT32      DeviceCapRegOffset;
  UINT32      VsecCapHeaderOffset;
  BOOLEAN     CapFound;
  UINTN       Segment;
  UINTN       Bus;
  UINTN       Device;
  UINTN       Function;
  UINTN       Index;
  EFI_HANDLE  *PciHandles;
  UINTN       PciHandlesSize;
  EFI_STATUS  Status;
  BOOLEAN     CnviPresence;

  EpBase              = 0;
  Segment             = 0;
  Bus                 = 0;
  Device              = 0;
  Function            = 0;
  DeviceCapRegOffset  = 0;
  VsecCapHeaderOffset = 0;
  CapFound            = FALSE;
  PciHandles          = NULL;
  PciHandlesSize      = 0;
  CnviPresence        = FALSE;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciHandlesSize,
                  &PciHandles
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to locate protocol handle\n"));
    return;
  }

  CnviPresence = PcdGetBool (PcdCnviPresence);
  DEBUG ((DEBUG_ERROR, "CnviPresence = %d\n", CnviPresence));

  for (Index = 0; Index < PciHandlesSize; Index++) {
    if (IsIntelWlanHandle (PciHandles[Index], &Segment, &Bus, &Device, &Function) && !(CnviPresence)) {
      EpBase = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, 0);

      //
      // Find offset of Device Capability Reg
      //
      DeviceCapRegOffset = (CnvPcieBaseFindCapId (EpBase, EFI_PCI_CAPABILITY_ID_PCIEXP) + CNV_R_PCIE_DCAP_OFFSET);

      //
      // Find offset of VSEC Capability Header
      //
      VsecCapHeaderOffset = CnvPcieBaseFindExtendedCapId (EpBase, VENDOR_SPECIFIC_EXTENDED_CAP_ID);
      do {
        //
        // Verify PCI Standard Capability ID is for Vendor Specific Extended Capability
        //
        if (PciSegmentRead16 (EpBase + VsecCapHeaderOffset + PCI_VSEC_CAPABILITY_ID_OFFSET) != V_VSEC_CAPABILITY_ID) {
          VsecCapHeaderOffset = PciSegmentRead16 (EpBase + VsecCapHeaderOffset + PCI_VSEC_NEXT_CAPABILITY_OFFSET) >> 4;
        } else {
          CapFound = TRUE;
        }
      } while ((VsecCapHeaderOffset != 0) && (!CapFound));

      if (CapFound) {
        break;
      }
    } else {
      continue;
    }
  }

  mCnvNvsAreaProtocol.Area->WlanDeviceCapRegOffset = DeviceCapRegOffset;
  mCnvNvsAreaProtocol.Area->WlanVsecHeaderOffset   = VsecCapHeaderOffset;
  DEBUG ((DEBUG_INFO, "VSEC CAP Header Offset = %x\n", VsecCapHeaderOffset));
  DEBUG ((DEBUG_INFO, "Device Capabilities Reg Offset = %x\n", DeviceCapRegOffset));
  //
  // Free any allocated buffers
  //
  FreePool (PciHandles);
}

/**
  This function gets registered as a callback to patch CNV ASL code

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
CnvAcpiExitBootServicesCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  UINT8                RegIndex;
  UINT64               EpBase;
  UINTN                Segment;
  UINTN                Bus;
  UINTN                Device;
  UINTN                Function;
  UINTN                Index;
  EFI_HANDLE           *PciHandles;
  UINTN                PciHandlesSize;
  EFI_STATUS           Status;

  EpBase   = 0;
  Segment  = 0;
  Bus      = 0;
  Device   = 0;
  Function = 0;
  RegIndex = 0;

  PciHandles = NULL;
  PciHandlesSize = 0;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciHandlesSize,
                  &PciHandles
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to locate protocol handle\n"));
    return;
  }
  if (ARRAY_SIZE (mCnvNvsAreaProtocol.Area->CNVdRegister) != ARRAY_SIZE (mCnvdRegOffestTable)) {
    DEBUG ((DEBUG_ERROR, "Size of CNVd register backup array not match\n"));
    return;
  } else {
    for (Index = 0; Index < PciHandlesSize; Index++) {
      if (IsIntelWlanHandle (PciHandles[Index], &Segment, &Bus, &Device, &Function) && !(PcdGetBool (PcdCnviPresence))) {
        EpBase = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, 0);
        //
        // Find CNVd Device
        //
        for (RegIndex = 0; RegIndex < ARRAY_SIZE (mCnvdRegOffestTable); RegIndex++) {
          mCnvNvsAreaProtocol.Area->CNVdRegister[RegIndex] = PciSegmentRead32 (EpBase + mCnvdRegOffestTable[RegIndex]);
        }
      } else {
        continue;
      }
    }
  }
}

/**
  Initialize Connectivity SSDT ACPI tables

  @param[in] ImageHandle  Handle to this EFI Image which will be used to
                          install Base, Clock and Performance protocols.
  @param[in] SystemTable  A pointer to boot time system table.

  @retval EFI_SUCCESS     ACPI tables are initialized successfully
  @retval EFI_NOT_FOUND   ACPI tables not found
**/
EFI_STATUS
EFIAPI
CnvDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;
  EFI_EVENT   EndOfDxeEvent;
  EFI_EVENT   EventExitBootServices;

  Status                = EFI_SUCCESS;
  Handle                = NULL;
  EndOfDxeEvent         = NULL;
  EventExitBootServices = NULL;

  DEBUG ((DEBUG_INFO, "CnvDxeEntryPoint START\n"));

  //
  // Get CNV Pcd Data Structure
  //
  mCnvDxeBoardConfigPtr = PcdGetPtr (PcdCnvBoardConfig);

  if (mCnvDxeBoardConfigPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "CnvDxeEntryPoint Invalid PCD Pointer.\n"));
    return EFI_NOT_FOUND;
  }

  //
  // This function updates variable.
  //
  CnvUpdateVariable ();

  //
  // Initialize CNV NVS Area
  //
  CnvNvsAreaInit ((VOID **) &mCnvNvsAreaProtocol.Area);

  //
  // Install CNV Global NVS ACPI table
  //
  Status = InitializeCnvSsdtAcpiTables ();
  ASSERT_EFI_ERROR (Status);

  FindWlanVsecHeaderAndDevCapOffset ();

  //
  // Update CNV NVS Area
  //
  CnvNvsAreaUpdate ();

  //
  // [ACPI] CNV Install NvsAreaProtocol
  //
  Handle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gCnvNvsAreaProtocolGuid,
                  &mCnvNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Register an end of DXE event for CNV ACPI to do some patch
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  CnvAcpiEndOfDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register an end of ExitBootServices for CNV ACPI to do some patch
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  CnvAcpiExitBootServicesCallback,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &EventExitBootServices
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Create ExitBootServices event for CNVd pcie cfg backup fail\n"));
  }

  DEBUG ((DEBUG_INFO, "CnvDxeEntryPoint END\n"));
  return Status;
}
