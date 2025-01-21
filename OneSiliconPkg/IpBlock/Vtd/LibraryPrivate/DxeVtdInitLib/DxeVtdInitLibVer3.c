/** @file
  DXE Library for VTD ACPI table initialization.

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
#include <Library/DxeVtdInitLib.h>
#include <Library/VtdInfoLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/TmeInfoLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Register/VtdRegs.h>
#include <Library/IGpuInfoLib.h>
#include <Library/LocalApicLib.h>
#include <Defines/HostBridgeDefines.h>

#define R_IO_APIC_MEM_INDEX_OFFSET            0x00000000U
#define R_IO_APIC_MEM_DATA_OFFSET             0x00000010U

///
/// Global Variables
///
GLOBAL_REMOVE_IF_UNREFERENCED VTD_DATA_HOB     *mVtdDataHob;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN          mInterruptRemappingSupport;

#define MAX_PHYSICAL_ADDRESS_SIZE_WITH_MKTME   42

/**
  Update the DRHD structure for GFX VT-d engine

  @param[in, out]  DrhdEngine   A pointer to DRHD structure for GFX VT-d engine
**/
VOID
UpdateDrhd1 (
  IN OUT EFI_ACPI_DRHD_ENGINE1_STRUCT *DrhdEngine
  )
{
  UINT16  Length;
  UINT16  VidDid;

  if (DrhdEngine == NULL) {
    return;
  }

  ///
  /// If a base address for given DRHD is 0, then this DRHD must be removed from DMAR ACPI table
  ///
  if (DrhdEngine->DrhdHeader.RegisterBaseAddress == 0) {
    DrhdEngine->DrhdHeader.Header.Length = 0;
    return;
  }

  Length = DrhdEngine->DrhdHeader.Header.Length;

  VidDid = PciSegmentRead16 (
             PCI_SEGMENT_LIB_ADDRESS (
               SA_SEG_NUM,
               DrhdEngine->DeviceScope[0].DeviceScopeStructureHeader.StartBusNumber,
               DrhdEngine->DeviceScope[0].PciPath.Device,
               DrhdEngine->DeviceScope[0].PciPath.Function,
               PCI_VENDOR_ID_OFFSET
               )
             );

  if (VidDid == 0xffff) {
    Length -= sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
  }
  ///
  /// If no devicescope is left, we set the structure length as 0x00
  ///
  if ((Length > EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH) || (DrhdEngine->DrhdHeader.Flags == 0x01)) {
    DrhdEngine->DrhdHeader.Header.Length = Length;
  } else {
    DrhdEngine->DrhdHeader.Header.Length = 0;
  }
}

/**
  Update the DRHD structure for non-GFX VT-d engine

  @param[in, out] DrhdEngine  A pointer to DRHD structure for non-GFX VT-d engine
**/
VOID
UpdateDrhd2 (
  IN OUT EFI_ACPI_DRHD_ENGINE2_STRUCT *DrhdEngine
  )
{
  UINT16  Length;
  UINT16  VidDid;
  UINT32  DeviceScopeNum;
  UINT32  ValidDeviceScopeNum;
  UINT32  Index;

  if (DrhdEngine == NULL) {
    return;
  }

  ///
  /// If a base address for given DRHD is 0, then this DRHD must be removed from DMAR ACPI table
  ///
  if (DrhdEngine->DrhdHeader.RegisterBaseAddress == 0) {
    DrhdEngine->DrhdHeader.Header.Length = 0;
    return;
  }

  Length              = DrhdEngine->DrhdHeader.Header.Length;
  DeviceScopeNum      = (DrhdEngine->DrhdHeader.Header.Length - sizeof (EFI_ACPI_DMAR_DRHD_HEADER)) / sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
  ValidDeviceScopeNum = 0;

  for (Index = 0; Index < DeviceScopeNum; Index++) {
    VidDid = PciSegmentRead16 (
              PCI_SEGMENT_LIB_ADDRESS (
                SA_SEG_NUM,
                DrhdEngine->DeviceScope[Index].DeviceScopeStructureHeader.StartBusNumber,
                DrhdEngine->DeviceScope[Index].PciPath.Device,
                DrhdEngine->DeviceScope[Index].PciPath.Function,
                PCI_VENDOR_ID_OFFSET
                )
              );

    ///
    /// If a device isn't present, then remove it from device scope
    ///
    if (VidDid != 0xFFFF) {
      CopyMem (
        &DrhdEngine->DeviceScope[ValidDeviceScopeNum],
        &DrhdEngine->DeviceScope[Index],
        sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE)
        );
      ValidDeviceScopeNum++;
    } else {
      Length -= sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
    }
  }

  ///
  /// If no devicescope is left, we set the structure length as 0x00
  ///
  if ((Length > EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH) || (DrhdEngine->DrhdHeader.Flags == 0x01)) {
    DrhdEngine->DrhdHeader.Header.Length = Length;
  } else {
    DrhdEngine->DrhdHeader.Header.Length = 0;
  }
}


/**
  Get IOAPIC ID from LPC

  @retval APIC ID
**/
UINT8
GetIoApicId (
  VOID
  )
{
  UINT32                IoApicAddress;
  UINT32                IoApicId;

  IoApicAddress = PcdGet32 (PcdSiIoApicBaseAddress);
  ///
  /// Get current IO APIC ID
  ///
  MmioWrite8 ((UINTN)IoApicAddress + R_IO_APIC_MEM_INDEX_OFFSET, 0);
  IoApicId = MmioRead32 ((UINTN)IoApicAddress + R_IO_APIC_MEM_DATA_OFFSET) >> 24;

  return (UINT8) IoApicId;
}

/**
  Update the DRHD structure for global VT-d engine

  @param[in, out] DrhdEngine  A pointer to DRHD structure for global VT-d engine
**/
VOID
UpdateDrhd3 (
  IN OUT EFI_ACPI_DRHD_ENGINE3_STRUCT *DrhdEngine
  )
{
  UINT16                        Length;
  UINTN                         DeviceScopeNum;
  UINTN                         ValidDeviceScopeNum;
  UINTN                         Index;
  UINT8                         Bus;
  UINT8                         Path[2];
  BOOLEAN                       NeedRemove;
  VOID                          *HobPtr;
  PCH_INFO_HOB                  *PchInfoHob;

  Length          = DrhdEngine->DrhdHeader.Header.Length;
  DeviceScopeNum  = (DrhdEngine->DrhdHeader.Header.Length - EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH) / sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
  Bus             = 0;
  ValidDeviceScopeNum = 0;
  Path[0]         = 0;
  Path[1]         = 0;

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  ASSERT (HobPtr != NULL);
  if (HobPtr == NULL) {
    return;
  }
  PchInfoHob = (PCH_INFO_HOB *) GET_GUID_HOB_DATA (HobPtr);

  for (Index = 0; Index < DeviceScopeNum; Index++) {
    NeedRemove = FALSE;
    /**
      For HPET and APIC, update device scope if Interrupt remapping is supported. remove device scope
      if interrupt remapping is not supported.
      - Index = 0 - IOAPIC
      - Index = 1 - HPET
    **/
    if (mInterruptRemappingSupport) {
      if (Index == 0) {
        ///
        /// Update source id for IoApic's device scope entry
        ///
        Bus = (UINT8) PchInfoHob->IoApicBusNum;
        Path[0] = (UINT8) PchInfoHob->IoApicDevNum;
        Path[1] = (UINT8) PchInfoHob->IoApicFuncNum;
        DrhdEngine->DeviceScope[Index].DeviceScopeStructureHeader.StartBusNumber = Bus;
        DrhdEngine->DeviceScope[Index].PciPath.Device = Path[0];
        DrhdEngine->DeviceScope[Index].PciPath.Function = Path[1];
        //
        // Update APIC ID
        //
        DrhdEngine->DeviceScope[Index].DeviceScopeStructureHeader.EnumerationId = GetIoApicId ();
      }
      if (Index == 1) {
        ///
        /// Update source id for HPET's device scope entry
        ///
        Bus     = (UINT8) PchInfoHob->HpetBusNum;
        Path[0] = (UINT8) PchInfoHob->HpetDevNum;
        Path[1] = (UINT8) PchInfoHob->HpetFuncNum;
        DrhdEngine->DeviceScope[Index].DeviceScopeStructureHeader.StartBusNumber = Bus;
        DrhdEngine->DeviceScope[Index].PciPath.Device = Path[0];
        DrhdEngine->DeviceScope[Index].PciPath.Function = Path[1];
      }
    } else {
      if ((Index == 0) || (Index == 1)) {
        NeedRemove = TRUE;
      }
    }

    CopyMem (
      &DrhdEngine->DeviceScope[ValidDeviceScopeNum],
      &DrhdEngine->DeviceScope[Index],
      sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE)
      );
    if (NeedRemove) {
      Length -= sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
    } else {
      ValidDeviceScopeNum++;
    }
  }
  ///
  /// If no devicescope is left, we set the structure length as 0x00
  ///
  if ((Length > EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH) || (DrhdEngine->DrhdHeader.Flags == 0x01)) {
    DrhdEngine->DrhdHeader.Header.Length = Length;
  } else {
    DrhdEngine->DrhdHeader.Header.Length = 0;
  }
}

/**
  Update the SATC structure

  @param[in, out] SatcPtr             - A pointer to SATC structure
**/
VOID
UpdateSatc (
  IN OUT EFI_ACPI_SATC_STRUCT *Satc
  )
{
  UINT16                  Length;
  UINTN                   DeviceScopeNum;
  UINTN                   ValidDeviceScopeNum;
  UINTN                   Index;
  BOOLEAN                 NeedRemove;
  VTD_ENGINE              VtdEngine;
  UINT32                  VtdBase;
  ECAP_VTD_IOMMU_STRUCT   Ecap;

  if (Satc == NULL) {
    return;
  }

  Length              = Satc->SatcHeader.Header.Length;
  DeviceScopeNum      = (Satc->SatcHeader.Header.Length - sizeof (EFI_ACPI_DMAR_SATC_HEADER)) / sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
  ValidDeviceScopeNum = 0;

  for (Index = 0; Index < DeviceScopeNum; Index++) {
    NeedRemove = FALSE;
    if ((PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, 0, Satc->DeviceScope[Index].PciPath.Device, Satc->DeviceScope[Index].PciPath.Function, 0x00)) == 0xFFFFFFFF)) {
      NeedRemove = TRUE;
    }

    //
    // For each PCIe device from SATC device scope, there is a need to check whether its IOMMU is enabled.
    // If not, then this device must be removed from SATC device scope.
    //
    VtdEngine = GetVtdEngineForPciDevice (0, Satc->DeviceScope[Index].PciPath.Device, Satc->DeviceScope[Index].PciPath.Function);
    if (!IsVtdEngineEnabled (VtdEngine)) {
      NeedRemove = TRUE;
    }

    VtdBase   = GetVtdBaseAddress (VtdEngine);
    Ecap.Data = MmioRead64 (VtdBase + ECAP_VTD_IOMMU_REG);

    //
    // If IOMMU doesn't support Device-TLBs, then respective devices mustn't be published in SATC table
    //
    if (Ecap.Bits.dt == 0) {
      NeedRemove = TRUE;
    }

    CopyMem (
      &Satc->DeviceScope[ValidDeviceScopeNum],
      &Satc->DeviceScope[Index],
      sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE)
      );

    if (NeedRemove) {
      Length -= sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
    } else {
      ValidDeviceScopeNum++;
    }
  }

  ///
  /// If no deviceScope is left, set length as 0x00
  ///
  if (Length > sizeof (EFI_ACPI_DMAR_SATC_HEADER)) {
    Satc->SatcHeader.Header.Length = Length;
  } else {
    Satc->SatcHeader.Header.Length = 0;
  }

}

/**
  Update the SIDP structure

  @param[in, out] Sidp       A pointer to SIDP structure
**/
VOID
UpdateSidp (
  IN OUT EFI_ACPI_SIDP_STRUCT *Sidp
  )
{
  UINT16                  Length;
  UINTN                   DeviceScopeNum;
  UINTN                   ValidDeviceScopeNum;
  UINTN                   Index;
  BOOLEAN                 NeedRemove;
  VTD_ENGINE              VtdEngine;
  UINT32                  VtdBase;
  ECAP_VTD_IOMMU_STRUCT   Ecap;

  if (Sidp == NULL) {
    return;
  }

  Length              = Sidp->SidpHeader.Header.Length;
  DeviceScopeNum      = (Sidp->SidpHeader.Header.Length - sizeof (EFI_ACPI_DMAR_SIDP_HEADER)) / sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
  ValidDeviceScopeNum = 0;

  for (Index = 0; Index < DeviceScopeNum; Index++) {
    NeedRemove = FALSE;
    if ((PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, 0, Sidp->DeviceScope[Index].PciPath.Device, Sidp->DeviceScope[Index].PciPath.Function, 0x00)) == 0xFFFFFFFF)) {
      NeedRemove = TRUE;
    }

    Sidp->DeviceScope[Index].DeviceScopeStructureHeader.Flags = GetPropertiesForPciDevice (
                                                                  0,
                                                                  Sidp->DeviceScope[Index].PciPath.Device,
                                                                  Sidp->DeviceScope[Index].PciPath.Function
                                                                  );
    //
    // For each PCIe device from SIDP device scope, there is a need to check whether its IOMMU is enabled.
    // If not, then this device must be removed from SIDP device scope.
    //
    VtdEngine = GetVtdEngineForPciDevice (0, Sidp->DeviceScope[Index].PciPath.Device, Sidp->DeviceScope[Index].PciPath.Function);
    if (!IsVtdEngineEnabled (VtdEngine)) {
      NeedRemove = TRUE;
    }

    VtdBase   = GetVtdBaseAddress (VtdEngine);
    Ecap.Data = MmioRead64 (VtdBase + ECAP_VTD_IOMMU_REG);

    //
    // If IOMMU doesn't support Device-TLBs, then respective devices mustn't be published in SIDP table
    //
    if (Ecap.Bits.dt == 0) {
      NeedRemove = TRUE;
    }

    CopyMem (
      &Sidp->DeviceScope[ValidDeviceScopeNum],
      &Sidp->DeviceScope[Index],
      sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE)
      );

    if (NeedRemove) {
      Length -= sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
    } else {
      ValidDeviceScopeNum++;
    }
  }

  ///
  /// If no deviceScope is left, set length as 0x00
  ///
  if (Length > sizeof (EFI_ACPI_DMAR_SIDP_HEADER)) {
    Sidp->SidpHeader.Header.Length = Length;
  } else {
    Sidp->SidpHeader.Header.Length = 0;
  }
}

/**
  Update the DMAR table

  @param[in, out] TableHeader         - The table to be set
  @param[in, out] Version             - Version to publish
**/
VOID
DmarTableUpdate (
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER       *TableHeader,
  IN OUT   EFI_ACPI_TABLE_VERSION            *Version
  )
{
  EFI_ACPI_DMAR_TABLE       *DmarTable;
  EFI_ACPI_DMAR_TABLE       TempDmarTable;
  UINTN                     Offset;
  UINTN                     StructureLen;

  DmarTable   = (EFI_ACPI_DMAR_TABLE *) TableHeader;

  ///
  /// Set INTR_REMAP bit (BIT 0) if interrupt remapping is supported
  ///
  if (mInterruptRemappingSupport) {
    DmarTable->DmarHeader.Flags |= BIT0;
  }

  ///
  /// X2APIC_OPT_OUT flag automatically becomes the inverse of X2APIC configuration.
  /// The flag is "don't care" if VT-d is disabled.
  ///
  if (GetApicMode () == LOCAL_APIC_MODE_XAPIC) {
    DmarTable->DmarHeader.Flags |= BIT1;
  } else {
    DmarTable->DmarHeader.Flags &= 0xFD;
  }

  ///
  /// Set DMA_CONTROL_GUARANTEE bit (BIT 2) if Dma Control Guarantee is supported
  ///
  if (mVtdDataHob->DmaControlGuarantee == 1) {
    DmarTable->DmarHeader.Flags |= BIT2;
  }
  ///
  /// Get OemId
  ///
  CopyMem (DmarTable->DmarHeader.Header.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (DmarTable->DmarHeader.Header.OemId));
  DmarTable->DmarHeader.Header.OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  DmarTable->DmarHeader.Header.OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  DmarTable->DmarHeader.Header.CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  DmarTable->DmarHeader.Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  ///
  /// Update Host Adress Width
  ///
  if (IsTmeActivated ()) {
    DmarTable->DmarHeader.HostAddressWidth = MAX_PHYSICAL_ADDRESS_SIZE_WITH_MKTME - 1;
  } else {
    DmarTable->DmarHeader.HostAddressWidth = GetMaxPhysicalAddressSize () - 1;
  }

  ///
  /// Update DRHD structures of DmarTable
  ///
  if (IsVtdEngineEnabled (GFX_VTD) && IGpuGetVendorId () != 0xFFFF) {
    DmarTable->DrhdEngine1.DrhdHeader.RegisterBaseAddress = GetVtdBaseAddress (GFX_VTD);
  }

  if (IsVtdEngineEnabled (NON_GFX_VTD)) {
    DmarTable->DrhdEngine2.DrhdHeader.RegisterBaseAddress = GetVtdBaseAddress (NON_GFX_VTD);
  }

  if (IsVtdEngineEnabled (GLOBAL_VTD)) {
    DmarTable->DrhdEngine3.DrhdHeader.RegisterBaseAddress = GetVtdBaseAddress (GLOBAL_VTD);
  }

  DEBUG ((DEBUG_INFO, "VTD base address 1 = %x\n", DmarTable->DrhdEngine1.DrhdHeader.RegisterBaseAddress));
  DEBUG ((DEBUG_INFO, "VTD base address 2 = %x\n", DmarTable->DrhdEngine2.DrhdHeader.RegisterBaseAddress));
  DEBUG ((DEBUG_INFO, "VTD base address 3 = %x\n", DmarTable->DrhdEngine3.DrhdHeader.RegisterBaseAddress));
  ///
  /// copy DmarTable to TempDmarTable to be processed
  ///
  CopyMem (&TempDmarTable, DmarTable, sizeof (EFI_ACPI_DMAR_TABLE));

  ///
  /// Update DRHD structures of temp DMAR table
  ///
  UpdateDrhd1 (&TempDmarTable.DrhdEngine1);
  UpdateDrhd2 (&TempDmarTable.DrhdEngine2);
  UpdateDrhd3 (&TempDmarTable.DrhdEngine3);

  ///
  /// Update SATC structure of temp DMAR table
  ///
  UpdateSatc (&TempDmarTable.Satc);

  ///
  /// Update SIDP structure of temp DMAR table
  ///
  UpdateSidp (&TempDmarTable.Sidp);

  ///
  /// Remove unused device scope or entire DRHD structures
  ///
  Offset = (UINTN) (&TempDmarTable.DrhdEngine1);
  if (TempDmarTable.DrhdEngine1.DrhdHeader.Header.Length != 0) {
    Offset += TempDmarTable.DrhdEngine1.DrhdHeader.Header.Length;
  }
  if (TempDmarTable.DrhdEngine2.DrhdHeader.Header.Length != 0) {
    StructureLen = TempDmarTable.DrhdEngine2.DrhdHeader.Header.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.DrhdEngine2, TempDmarTable.DrhdEngine2.DrhdHeader.Header.Length);
    Offset += StructureLen;
  }
  if (TempDmarTable.DrhdEngine3.DrhdHeader.Header.Length != 0) {
    StructureLen = TempDmarTable.DrhdEngine3.DrhdHeader.Header.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.DrhdEngine3, TempDmarTable.DrhdEngine3.DrhdHeader.Header.Length);
    Offset += StructureLen;
  }
  ///
  /// Remove unused device scope or entire SATC structure
  ///
  if (TempDmarTable.Satc.SatcHeader.Header.Length != 0) {
    StructureLen = TempDmarTable.Satc.SatcHeader.Header.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.Satc, TempDmarTable.Satc.SatcHeader.Header.Length);
    Offset += StructureLen;
  }

  ///
  /// Remove unused device scope or entire SIDP structure
  ///
  if (TempDmarTable.Sidp.SidpHeader.Header.Length != 0) {
    StructureLen = TempDmarTable.Sidp.SidpHeader.Header.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.Sidp, TempDmarTable.Sidp.SidpHeader.Header.Length);
    Offset += StructureLen;
  }

  Offset = Offset - (UINTN) &TempDmarTable;
  ///
  /// Re-calculate DMAR table check sum
  ///
  TempDmarTable.DmarHeader.Header.Checksum = (UINT8) (TempDmarTable.DmarHeader.Header.Checksum + TempDmarTable.DmarHeader.Header.Length - Offset);
  ///
  /// Set DMAR table length
  ///
  TempDmarTable.DmarHeader.Header.Length = (UINT32) Offset;
  ///
  /// Replace DMAR table with rebuilt table TempDmarTable
  ///
  CopyMem ((VOID *) DmarTable, (VOID *) &TempDmarTable, TempDmarTable.DmarHeader.Header.Length);
}

/**
  EndOfPcieEnumration routine for update DMAR
**/
VOID
UpdateDmarEndOfPcieEnum (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_HANDLE                      *HandleBuffer;
  UINTN                           NumberOfHandles;
  EFI_FV_FILETYPE                 FileType;
  UINT32                          FvStatus;
  EFI_FV_FILE_ATTRIBUTES          Attributes;
  UINTN                           Size;
  UINTN                           i;
  INTN                            Instance;
  EFI_ACPI_TABLE_VERSION          Version;
  EFI_ACPI_COMMON_HEADER          *CurrentTable;
  UINTN                           AcpiTableHandle;
  EFI_FIRMWARE_VOLUME2_PROTOCOL   *FwVol;
  EFI_ACPI_TABLE_PROTOCOL         *AcpiTable;
  EFI_ACPI_DESCRIPTION_HEADER     *VtdAcpiTable;
  STATIC BOOLEAN                  Triggered = FALSE;

  if (Triggered) {
    return;
  }

  Triggered     = TRUE;
  FwVol         = NULL;
  AcpiTable     = NULL;
  VtdAcpiTable  = NULL;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  ///
  /// Fix DMAR Table always created, skip install when disabled
  ///
  if ((mVtdDataHob->VtdDisable == TRUE) || !IsVtdSupported ()) {
    DEBUG ((DEBUG_INFO, "Vtd Disabled, skip DMAR Table install\n"));
    return;
  }

  ///
  /// Locate ACPI support protocol
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);

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
                    (VOID **) &FwVol
                    );
    ASSERT_EFI_ERROR (Status);

    ///
    /// See if it has the ACPI storage file
    ///
    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &gDmarAcpiTableStorageGuid,
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
  /// Our exit status is determined by the success of the previous operations
  /// If the protocol was found, Instance already points to it.
  ///
  ///
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);

  ///
  /// Sanity check that we found our data file
  ///
  ASSERT (FwVol);
  if (FwVol == NULL) {
    return;
  }
  ///
  /// By default, a table belongs in all ACPI table versions published.
  ///
  Version = EFI_ACPI_TABLE_VERSION_1_0B | EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0;

  ///
  /// Read tables from the storage file.
  ///
  Instance      = 0;
  CurrentTable  = NULL;

  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gDmarAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &CurrentTable,
                      &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
      ///
      /// Check the Signature ID to modify the table
      ///
      if ((CurrentTable != NULL) && ((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->Signature == EFI_ACPI_VTD_DMAR_TABLE_SIGNATURE) {
        VtdAcpiTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
        DmarTableUpdate (VtdAcpiTable, &Version);
        break;
      }
      ///
      /// Increment the instance
      ///
      Instance++;
      if (CurrentTable != NULL) {
        gBS->FreePool (CurrentTable);
        CurrentTable = NULL;
      }
    }
  }

  ///
  /// Update the VTD table in the ACPI tables.
  ///
  AcpiTableHandle = 0;
  if (VtdAcpiTable != NULL) {
    Status = AcpiTable->InstallAcpiTable (
                          AcpiTable,
                          VtdAcpiTable,
                          VtdAcpiTable->Length,
                          &AcpiTableHandle
                          );
    FreePool (VtdAcpiTable);
  }
}

/**
  Locate the VT-d ACPI tables data file and read ACPI SSDT tables.
  Publish the appropriate SSDT based on current configuration and capabilities.

  @retval EFI_SUCCESS     - Vtd initialization complete
  @exception EFI_UNSUPPORTED - Vtd is not enabled by policy
**/
EFI_STATUS
VtdInit (
  )
{
  mInterruptRemappingSupport  = FALSE;
  mVtdDataHob = NULL;
  mVtdDataHob = GetFirstGuidHob(&gVtdDataHobGuid);
  if (mVtdDataHob != NULL) {
    mInterruptRemappingSupport = mVtdDataHob->InterruptRemappingSupport;
  }

  if (mVtdDataHob != NULL) {
    if ((mVtdDataHob->VtdDisable) || !IsVtdSupported ()) {
      DEBUG ((DEBUG_WARN, "VTd disabled or no capability!\n"));
      return EFI_UNSUPPORTED;
    }
  }
  ///
  /// Check SA supports VTD and VTD is enabled in setup menu
  ///
  DEBUG ((DEBUG_INFO, "VTd enabled\n"));

  InstallVtdTopologyProtocol ();

  return EFI_SUCCESS;
}
