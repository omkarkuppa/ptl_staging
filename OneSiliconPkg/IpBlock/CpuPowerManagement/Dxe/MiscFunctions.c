/** @file
  This file contains Processor Power Management ACPI related functions for
  processors.

  <b>Acronyms:</b>
     - PPM:  Processor Power Management
     - TM:   Thermal Monitor
     - IST:  Intel(R) Speedstep technology
     - HT:   Hyper-Threading Technology
     - ITBM: Intel(R) Turbo Boost Max Technology

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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

#include "PowerMgmtInit.h"
#if (FixedPcdGetBool(PcdBiosGuardEnable) == 1) && (FixedPcdGetBool(PcdBiosGuardLegacyAcpi) == 1)
#include <Protocol/BiosGuardNvsArea.h>
#endif

/**
  Update the SSDT table pointers and config DWORD CFGD with the PpmFlags current configuration value
**/
VOID
PatchCpuSsdtTable (
  VOID
  )
{
  UINT8                         *CurrPtr;
  UINT32                        *Signature;

#if (FixedPcdGetBool(PcdBiosGuardEnable) == 1) && (FixedPcdGetBool(PcdBiosGuardLegacyAcpi) == 1)
  BIOSGUARD_NVS_AREA_PROTOCOL   *BiosGuardNvsAreaProtocol = NULL;
  EFI_STATUS                    Status;
  BOOLEAN                       BiosGuardEnable;
  BOOLEAN                       BiosGuardReady;

  BiosGuardEnable           = FALSE;
  BiosGuardReady            = FALSE;
  BiosGuardEnable           = IsBiosGuardEnabled ();
  ///
  /// Locate BIOS Guard NVS if BIOS Guard is Enabled
  ///
  if (BiosGuardEnable == TRUE) {
    DEBUG ((DEBUG_INFO, "\n BIOS Guard is Enabled\n"));
    Status = gBS->LocateProtocol (&gBiosGuardNvsAreaProtocolGuid, NULL, (VOID **) &BiosGuardNvsAreaProtocol);
    if ((!EFI_ERROR (Status)) && BiosGuardNvsAreaProtocol != NULL ) {
      BiosGuardReady = TRUE;
    } else {
      DEBUG ((DEBUG_INFO, "\n Unable to Locate BIOS Guard Protocol\n"));
    }
  } else {
    DEBUG ((DEBUG_INFO, "\n BIOS Guard is Disabled\n"));
  }
#endif

  if (mCpuSsdtTable == NULL) {
      DEBUG((DEBUG_INFO, "mCpuSsdtTable is NULL \n"));
      ASSERT(FALSE);
      return;
  }
  //
  // Update OperationRegion (PNVS & BGNV)'s offset and length
  // Reserve OperationRegion space to avoid pointer out-of-bounds access when traversing the CPU SSDT table.
  // OperationRegion space (14 bytes) = 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1 + sizeof (UINT16)
  //
  for (CurrPtr = (UINT8 *) mCpuSsdtTable; CurrPtr <= ((UINT8 *) mCpuSsdtTable + mCpuSsdtTable->Length - 14); CurrPtr++) {
    Signature = (UINT32 *) (CurrPtr + 1);
    ///
    /// Update the CPU GlobalNvs area
    ///
    if ((*CurrPtr == AML_EXT_REGION_OP) && *Signature == SIGNATURE_32 ('P', 'N', 'V', 'S')) {
      ASSERT (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) == 0xFFFF0000);
      ASSERT (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) == 0xAA55);
      ///
      /// Cpu Nvs Area address
      ///
      *(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) = (UINT32) (UINTN) gCpuNvsAreaProtocol->Area;
      ///
      /// Cpu Nvs Area size
      ///
      *(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) = sizeof (CPU_NVS_AREA);
#if (FixedPcdGetBool(PcdBiosGuardEnable) == 0) || (FixedPcdGetBool(PcdBiosGuardLegacyAcpi) == 0)
      break;
#endif
    }

#if (FixedPcdGetBool(PcdBiosGuardEnable) == 1) && (FixedPcdGetBool(PcdBiosGuardLegacyAcpi) == 1)
    ///
    /// Update the BIOS Guard NVS area if BIOS Guard is Enabled and Ready
    ///
    if ((BiosGuardEnable == TRUE) &&
        (BiosGuardReady == TRUE) &&
        (*CurrPtr == AML_EXT_REGION_OP) &&
        (*Signature == SIGNATURE_32 ('B', 'G', 'N', 'V'))) {
      ASSERT (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) == 0xFFFF0000);
      ASSERT (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) == 0xAA55);
      ///
      /// BIOS Guard Nvs Area address
      ///
      *(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) = (UINT32) (UINTN) gBiosGuardNvsAreaProtocol->Area;
      ///
      /// BIOS Guard Nvs Area size
      ///
      *(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) = sizeof (BIOSGUARD_NVS_AREA);
      break;
    }
#endif
  }
  return;
}

/**
  Install BSP and AP tables based on PpmFlagsS check

  @param [in]       IstTable
  @param [in]       HwpTable
  @param [in]       PsdTable
  @param [in]       CstTable
  @param [in]       TstTable
  @param [in]       Ppmflags

  @retval           EFI_SUCCESS  If the Tables are installed correctly
  @retval           EFI_ERROR    If any one of the tables is not installed correctly.
**/
STATIC
EFI_STATUS
InstallCpuSsdtTables (
  IN EFI_ACPI_DESCRIPTION_HEADER    *IstTable,
  IN EFI_ACPI_DESCRIPTION_HEADER    *HwpTable,
  IN EFI_ACPI_DESCRIPTION_HEADER    *PsdTable,
  IN EFI_ACPI_DESCRIPTION_HEADER    *CstTable,
  IN EFI_ACPI_DESCRIPTION_HEADER    *TstTable,
  IN UINT32                         PpmFlags
  )
{
  UINTN                             AcpiTableHandle;
  EFI_STATUS                        Status;

  Status = EFI_SUCCESS;
  ///
  /// Install CpuIst SSDT if EIST is enabled
  ///
  if (PpmFlags & PPM_EIST) {
    Status = mAcpiTable->InstallAcpiTable (
                           mAcpiTable,
                           IstTable,
                           IstTable->Length,
                           &AcpiTableHandle
                           );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  ///
  /// Install CpuHwp & HwpLvt SSDT to reserved location and checksum them
  ///
  if (PpmFlags & PPM_HWP) {
    Status = mAcpiTable->InstallAcpiTable (
                           mAcpiTable,
                           HwpTable,
                           HwpTable->Length,
                           &AcpiTableHandle
                           );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  ///
  /// Install CpuPsd SSDT if EIST or HWP is enabled
  ///
  if ((PpmFlags & PPM_EIST) || (PpmFlags & PPM_HWP)) {
    Status = mAcpiTable->InstallAcpiTable (
                           mAcpiTable,
                           PsdTable,
                           PsdTable ->Length,
                           &AcpiTableHandle
                           );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  ///
  /// Install CST SSDT if C states are enabled
  ///
  if (PpmFlags & PPM_C_STATES) {
    Status = mAcpiTable->InstallAcpiTable (
                           mAcpiTable,
                           CstTable,
                           CstTable->Length,
                           &AcpiTableHandle
                           );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  ///
  ///  Install  CpuTst SSDT table in the ACPI tables
  ///
  if (PpmFlags & PPM_TSTATES) {
    Status = mAcpiTable->InstallAcpiTable (
                           mAcpiTable,
                           TstTable,
                           TstTable->Length,
                           &AcpiTableHandle
                           );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  return Status;
}
/**
  Locate the PPM ACPI tables data file and read ACPI SSDT tables.
  Publish the appropriate SSDT based on current configuration and capabilities.

  @retval EFI_SUCCESS - On success
  @retval EFI_NOT_FOUND - Required firmware volume not found
  @retval EFI_OUT_OF_RESOURCES - No enough resoruces (such as out of memory).
  @retval - Appropiate failure code on error
**/
EFI_STATUS
InitCpuAcpiTable (
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
  UINTN                          Index;
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol;
  UINTN                          Instance;
  EFI_ACPI_COMMON_HEADER         *CurrentTable;
  UINTN                          AcpiTableHandle;

  FwVol = NULL;

  ///
  /// Locate Firmware volume protocol.
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
  /// Look for FV with ACPI storage file
  ///
  FwVol = NULL;
  for (Index = 0; Index < NumberOfHandles; Index++) {
    ///
    /// Get the protocol on this handle
    /// This should not fail because of LocateHandleBuffer
    ///
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
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
                      &gCpuAcpiTableStorageGuid,
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
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);
  ///
  /// Sanity check that we found our data file
  ///
  ASSERT (FwVol != NULL);
  if (FwVol == NULL) {
    return EFI_NOT_FOUND;
  }
  ///
  /// Read tables from the storage file.
  ///
  Instance      = 0;
  CurrentTable  = NULL;
  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gCpuAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &CurrentTable,
                      &Size,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      ///
      /// Check the table ID to modify the table
      ///
      switch (((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->OemTableId) {
        case (SIGNATURE_64 ('C', 'p', 'u', '0', 'I', 's', 't', 0)) :
          mCpu0IstTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
          if (gCpuNvsAreaProtocol->Area->PpmFlags & PPM_EIST) {
            ///
            /// Patch the native _PSS package with the EIST values
            ///
            Status = AcpiPatchPss ();
            if (EFI_ERROR (Status)) {
              FreePool (CurrentTable);
              CurrentTable = NULL;
              return Status;
            }
          }
          break;
        case (SIGNATURE_64 ('C', 'p', 'u', '0', 'C', 's', 't', 0)) :
          mCpu0CstTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
          break;
        case (SIGNATURE_64 ('C', 'p', 'u', '0', 'T', 's', 't', 0)) :
          mCpu0TstTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
          break;
        case (SIGNATURE_64 ('A', 'p', 'I', 's', 't', 0, 0, 0)) :
          mApIstTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
          break;
        case (SIGNATURE_64 ('A', 'p', 'C', 's', 't', 0, 0, 0)) :
          mApCstTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
          break;
        case (SIGNATURE_64 ('A', 'p', 'T', 's', 't', 0, 0, 0)) :
          mApTstTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
          break;
        case (SIGNATURE_64 ('C', 'p', 'u', 'S', 's', 'd', 't', 0)) :
          mCpuSsdtTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
          break;
        case (SIGNATURE_64 ('C', 'p', 'u', '0', 'H', 'w', 'p', 0)) :
          mCpu0HwpTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
          break;
        case (SIGNATURE_64 ('A', 'p', 'H', 'w', 'p', 0, 0, 0)) :
          mApHwpTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
          break;
        case (SIGNATURE_64 ('C', 'p', 'u', '0', 'P', 's', 'd', 0)) :
          mCpu0PsdTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
          break;
        case (SIGNATURE_64 ('A', 'p', 'P', 's', 'd', 0, 0, 0)) :
          mApPsdTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
          break;
        default:
          break;
      }
      Instance++; // Increment the instance
      CurrentTable = NULL;
    }
  }
  ///
  /// Install BSP tables
  ///
  Status = InstallCpuSsdtTables (mCpu0IstTable, mCpu0HwpTable, mCpu0PsdTable, mCpu0CstTable, mCpu0TstTable, gCpuNvsAreaProtocol->Area->PpmFlags);

  FreePool (mCpu0IstTable);
  FreePool (mCpu0HwpTable);
  FreePool (mCpu0PsdTable);
  FreePool (mCpu0CstTable);
  FreePool (mCpu0TstTable);
  ///
  /// Assigning the pointer to NULL.To avoid any potentional risk.
  ///
  mCpu0IstTable = NULL;
  mCpu0HwpTable = NULL;
  mCpu0PsdTable = NULL;
  mCpu0CstTable = NULL;
  mCpu0TstTable = NULL;

  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Install Ap Tables
  ///
  if (gCpuNvsAreaProtocol->Area->PpmFlags & PPM_CMP) {
    Status = InstallCpuSsdtTables (mApIstTable, mApHwpTable, mApPsdTable, mApCstTable, mApTstTable, gCpuNvsAreaProtocol->Area->PpmFlags);
    if (EFI_ERROR (Status)) {
      goto Exit;
    }
  }

  ///
  /// Update the CpuSsdt SSDT table in the ACPI tables.
  ///
  PatchCpuSsdtTable ();
  AcpiTableHandle = 0;
  Status = mAcpiTable->InstallAcpiTable (
                         mAcpiTable,
                         mCpuSsdtTable,
                         mCpuSsdtTable->Length,
                         &AcpiTableHandle
                         );
  FreePool (mCpuSsdtTable);
  mCpuSsdtTable = NULL;

Exit:
  FreePool (mApIstTable);
  FreePool (mApHwpTable);
  FreePool (mApPsdTable);
  FreePool (mApCstTable);
  FreePool (mApTstTable);
  ///
  /// Assigning the pointer to NULL.To avoid any potentional risk.
  ///
  mApIstTable = NULL;
  mApHwpTable = NULL;
  mApPsdTable = NULL;
  mApCstTable = NULL;
  mApTstTable = NULL;
  return Status;
}

