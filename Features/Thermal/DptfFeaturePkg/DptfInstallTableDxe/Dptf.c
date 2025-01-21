/** @file
  This DXE driver configures and supports Intel(R) Dynamic Tuning Technology.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#include <Library/BaseMemoryLib.h>
#include "Dptf.h"
#include <DptfConfig.h>
#include <Protocol/DptfNvsArea.h>

GLOBAL_REMOVE_IF_UNREFERENCED  DPTF_CONFIG             mDptfSetupData;
GLOBAL_REMOVE_IF_UNREFERENCED  DPTF_NVS_AREA_PROTOCOL  mDptfNvsAreaProtocol;

EFI_STATUS
EFIAPI
InitializeDptf (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
/**
@brief
  This procedure does all the Intel(R) Dynamic Tuning Technology initialization and loads the ACPI tables.

  @param[in] ImageHandle   - The firmware allocated handle to the Driver Image
  @param[in] SystemTable   - Pointer to the EFI System Table

  @retval EFI_SUCCESS - The driver installed/initialized correctly.
**/
{
  EFI_STATUS            Status;
  UINTN                 VariableSize;
  UINT32                gDptfVariableAttributes;
  EFI_PHYSICAL_ADDRESS  Area;
  UINTN                 Pages;

  DEBUG ((DEBUG_INFO, "Intel(R) Dynamic Tuning Technology: EntryPoint\n"));
  VariableSize = sizeof (DPTF_CONFIG);
  Status = gRT->GetVariable (
                  DPTF_CONFIG_VARIABLE_NAME,
                  &gDptfConfigVariableGuid,
                  &gDptfVariableAttributes,
                  &VariableSize,
                  &mDptfSetupData
                  );

  if (Status == EFI_NOT_FOUND) {

    gDptfVariableAttributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
    mDptfSetupData.EnableDptf = 1;
    VariableSize = sizeof (DPTF_CONFIG);
    Status = gRT->SetVariable (
                    DPTF_CONFIG_VARIABLE_NAME,
                    &gDptfConfigVariableGuid,
                    gDptfVariableAttributes,
                    VariableSize,
                    &mDptfSetupData
                    );
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "DPTF: SetVariable Failed. Skipping loading dptf ACPI tables\n"));
      ASSERT_EFI_ERROR(Status);
      return Status;
    }
    DEBUG ((DEBUG_INFO, "Intel(R) Dynamic Tuning Technology: Creating Dptf Config Variable\n"));
  }

  mDptfSetupData.Revision = DPTF_CONFIG_REVISION;

  ///
  /// Check if Intel(R) Dynamic Tuning Technology is enabled and load the ACPI SSDT.
  ///
  if (mDptfSetupData.EnableDptf == 1) {
    ///
    /// Allocated ACPI NVS type memory for Cpu Nvs Configuration.
    ///
    Pages = EFI_SIZE_TO_PAGES (sizeof (DPTF_NVS_AREA));
    Area = MAX_UINT32; // allocate address below 4G.
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiACPIMemoryNVS,
                    Pages,
                    &Area
                    );
    ASSERT_EFI_ERROR (Status);
    mDptfNvsAreaProtocol.Area = (DPTF_NVS_AREA *)(UINTN) Area;
    ZeroMem ((VOID *)mDptfNvsAreaProtocol.Area, sizeof (DPTF_NVS_AREA));
    //
    // Pseudo code for Intel(R) Dynamic Tuning Technology Devices and trip points without BIOS setup.
    //
    mDptfNvsAreaProtocol.Area->EnableDptf                   = 1;
    mDptfNvsAreaProtocol.Area->PpccStepSize                 = 500;
    mDptfNvsAreaProtocol.Area->EnableChargerParticipant     = 1;
    mDptfNvsAreaProtocol.Area->EnableBatteryParticipant     = 1;
    mDptfNvsAreaProtocol.Area->EnableSen1Participant        = 1;
    mDptfNvsAreaProtocol.Area->EnableSen2Participant        = 1;
    mDptfNvsAreaProtocol.Area->EnableSen3Participant        = 1;
    mDptfNvsAreaProtocol.Area->EnableSen4Participant        = 1;
    mDptfNvsAreaProtocol.Area->EnableSen5Participant        = 1;
    mDptfNvsAreaProtocol.Area->EnablePowerParticipant       = 1;
    mDptfNvsAreaProtocol.Area->EnablePchFivrParticipant     = 1;
    mDptfNvsAreaProtocol.Area->EnableFanAutoMode            = 1;
    mDptfNvsAreaProtocol.Area->OemDesignVariable0           = 0;
    mDptfNvsAreaProtocol.Area->OemDesignVariable1           = 0;
    mDptfNvsAreaProtocol.Area->OemDesignVariable2           = 0;
    mDptfNvsAreaProtocol.Area->OemDesignVariable3           = 0;
    mDptfNvsAreaProtocol.Area->OemDesignVariable4           = 0;
    mDptfNvsAreaProtocol.Area->OemDesignVariable5           = 0;
    DEBUG ((DEBUG_INFO, "Intel(R) Dynamic Tuning Technology: Acpi Defaults initialize\n"));
    //
    // Configure Intel(R) Dynamic Tuning Technology Variables using BIOS Setup menu
    //
    mDptfNvsAreaProtocol.Area->EnableDptf                     = mDptfSetupData.EnableDptf;
    mDptfNvsAreaProtocol.Area->EnableDCFG                     = mDptfSetupData.EnableDCFG;
    mDptfNvsAreaProtocol.Area->PpccStepSize                   = mDptfSetupData.PpccStepSize;
    //
    // Configure Intel(R) Dynamic Tuning Technology Devices using BIOS Setup menu
    //
    mDptfNvsAreaProtocol.Area->EnableFan1Device               = mDptfSetupData.EnableFan1Device;
    mDptfNvsAreaProtocol.Area->EnableFan2Device               = mDptfSetupData.EnableFan2Device;
    mDptfNvsAreaProtocol.Area->EnableFan3Device               = mDptfSetupData.EnableFan3Device;
    mDptfNvsAreaProtocol.Area->EnableChargerParticipant       = mDptfSetupData.EnableChargerParticipant;
    mDptfNvsAreaProtocol.Area->EnablePowerParticipant         = mDptfSetupData.EnablePowerParticipant;
    mDptfNvsAreaProtocol.Area->PowerParticipantPollingRate    = mDptfSetupData.PowerParticipantPollingRate;
    mDptfNvsAreaProtocol.Area->EnableBatteryParticipant       = mDptfSetupData.EnableBatteryParticipant;
    mDptfNvsAreaProtocol.Area->EnablePchFivrParticipant       = mDptfSetupData.EnablePchFivrParticipant;
    mDptfNvsAreaProtocol.Area->EnableSen1Participant          = mDptfSetupData.EnableSen1Participant;
    mDptfNvsAreaProtocol.Area->EnableSen2Participant          = mDptfSetupData.EnableSen2Participant;
    mDptfNvsAreaProtocol.Area->EnableSen3Participant          = mDptfSetupData.EnableSen3Participant;
    mDptfNvsAreaProtocol.Area->EnableSen4Participant          = mDptfSetupData.EnableSen4Participant;
    mDptfNvsAreaProtocol.Area->EnableSen5Participant          = mDptfSetupData.EnableSen5Participant;
    mDptfNvsAreaProtocol.Area->EnableFanAutoMode              = mDptfSetupData.EnableFanAutoMode;
    //
    // Configure Intel(R) Dynamic Tuning Technology Oem Design Variable using BIOS Setup menu
    //
    mDptfNvsAreaProtocol.Area->OemDesignVariable0             = mDptfSetupData.OemDesignVariable0;
    mDptfNvsAreaProtocol.Area->OemDesignVariable1             = mDptfSetupData.OemDesignVariable1;
    mDptfNvsAreaProtocol.Area->OemDesignVariable2             = mDptfSetupData.OemDesignVariable2;
    mDptfNvsAreaProtocol.Area->OemDesignVariable3             = mDptfSetupData.OemDesignVariable3;
    mDptfNvsAreaProtocol.Area->OemDesignVariable4             = mDptfSetupData.OemDesignVariable4;
    mDptfNvsAreaProtocol.Area->OemDesignVariable5             = mDptfSetupData.OemDesignVariable5;

    DEBUG ((DEBUG_INFO, "EnableDptf =0x%0x\n",mDptfSetupData.EnableDptf));
    DEBUG ((DEBUG_INFO, "EnableDCFG =0x%0x\n",mDptfSetupData.EnableDCFG));
    DEBUG ((DEBUG_INFO, "PPCC Step Size =0x%0x\n",mDptfSetupData.PpccStepSize));
    DEBUG ((DEBUG_INFO, "EnableFan1Device =0x%0x\n",mDptfSetupData.EnableFan1Device));
    DEBUG ((DEBUG_INFO, "EnableFan2Device =0x%0x\n",mDptfSetupData.EnableFan2Device));
    DEBUG ((DEBUG_INFO, "EnableFan3Device =0x%0x\n",mDptfSetupData.EnableFan3Device));
    DEBUG ((DEBUG_INFO, "EnableChargerParticipant =0x%0x\n",mDptfSetupData.EnableChargerParticipant));
    DEBUG ((DEBUG_INFO, "EnablePowerParticipant =0x%0x\n",mDptfSetupData.EnablePowerParticipant));
    DEBUG ((DEBUG_INFO, "PowerParticipantPollingRate =0x%0x\n",mDptfSetupData.PowerParticipantPollingRate));
    DEBUG ((DEBUG_INFO, "EnableBatteryParticipant =0x%0x\n",mDptfSetupData.EnableBatteryParticipant));
    DEBUG ((DEBUG_INFO, "EnablePchFivrParticipant =0x%0x\n",mDptfSetupData.EnablePchFivrParticipant));
    DEBUG ((DEBUG_INFO, "EnableSen1Participant =0x%0x\n",mDptfSetupData.EnableSen1Participant));
    DEBUG ((DEBUG_INFO, "EnableSen2Participant =0x%0x\n",mDptfSetupData.EnableSen2Participant));
    DEBUG ((DEBUG_INFO, "EnableSen3Participant =0x%0x\n",mDptfSetupData.EnableSen3Participant));
    DEBUG ((DEBUG_INFO, "EnableSen4Participant  =0x%0x\n",mDptfSetupData.EnableSen4Participant));
    DEBUG ((DEBUG_INFO, "EnableSen5Participant =0x%0x\n",mDptfSetupData.EnableSen5Participant));
    DEBUG ((DEBUG_INFO, "EnableFanAutoMode =0x%0x\n",mDptfSetupData.EnableFanAutoMode));
    DEBUG ((DEBUG_INFO, "OemDesignVariable0 =0x%0x\n",mDptfSetupData.OemDesignVariable0));
    DEBUG ((DEBUG_INFO, "OemDesignVariable1 =0x%0x\n",mDptfSetupData.OemDesignVariable1));
    DEBUG ((DEBUG_INFO, "OemDesignVariable2 =0x%0x\n",mDptfSetupData.OemDesignVariable2));
    DEBUG ((DEBUG_INFO, "OemDesignVariable3 =0x%0x\n",mDptfSetupData.OemDesignVariable3));
    DEBUG ((DEBUG_INFO, "OemDesignVariable4 =0x%0x\n",mDptfSetupData.OemDesignVariable4));
    DEBUG ((DEBUG_INFO, "OemDesignVariable5 =0x%0x\n",mDptfSetupData.OemDesignVariable5));

    DEBUG ((DEBUG_INFO, "Intel(R) Dynamic Tuning Technology: Initializing acpi space\n"));
    ///
    /// Install DPTF GlobalNVS Area protocol
    ///
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &ImageHandle,
                    &gDptfNvsAreaProtocolGuid,
                    &mDptfNvsAreaProtocol,
                    NULL
                    );
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "DPTF: InstallProtocol for NVS area Failed. Skipping loading dptf ACPI tables\n"));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
    ///
    /// Load the SSDT ACPI Tables.
    ///
    DEBUG ((DEBUG_INFO, "Intel(R) Dynamic Tuning Technology: LoadAcpiTables\n"));
    LoadAcpiTables ();
  }

  VariableSize = sizeof (DPTF_CONFIG);
  Status = gRT->SetVariable (
                  DPTF_CONFIG_VARIABLE_NAME,
                  &gDptfConfigVariableGuid,
                  gDptfVariableAttributes,
                  VariableSize,
                  &mDptfSetupData
                  );
  if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "DPTF: SetVariable Failed. Skipping loading dptf ACPI tables\n"));
      ASSERT_EFI_ERROR(Status);
      return Status;
    }

  DEBUG ((DEBUG_INFO, "Intel(R) Dynamic Tuning Technology: Exit\n"));
  return Status;
}

EFI_STATUS
EFIAPI
LoadAcpiTables(
  VOID
  )
/**
@brief
  This procedure loads the ACPI SSDT tables.

**/
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  UINTN                         Index;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  INTN                          Instance;
  EFI_ACPI_COMMON_HEADER        *Table;
  UINTN                         Size;
  EFI_FV_FILETYPE               FileType;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  UINT32                        FvStatus;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  UINTN                         TableHandle;
  BOOLEAN                       LoadTable;
  UINT8                         *CurrPtr;
  UINT32                        *Signature;
  UINT8                         UpdateCounter;

  FwVol = NULL;
  Table = NULL;
  UpdateCounter = 0;
  ///
  /// Locate FV protocol.
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "DPTF: No Efi Firmware Volume Protocol available.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "DPTF: Efi Firmware Volume Protocol is loaded.\n"));
  }
  ASSERT_EFI_ERROR (Status);

  ///
  /// Look for FV with ACPI storage file
  ///
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

    if ((Status == EFI_SUCCESS) && (FwVol != NULL)) {
      ///
      /// See if it has the ACPI storage file
      ///
      Size      = 0;
      FvStatus  = 0;
      Status = FwVol->ReadFile (
                      FwVol,
                      &gDptfAcpiTableStorageGuid,
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
        DEBUG ((DEBUG_INFO, "DPTF: Intel(R) Dynamic Tuning Technology Acpi Table Storage for RVP is found.\n"));
        break;
      }
    }
  }

  ///
  /// Our exit status is determined by the success of the previous operations
  /// If the protocol was found, Instance already points to it.
  ///
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);

  ///
  /// Sanity check that we found our data file
  ///
  ASSERT (FwVol);
  if (FwVol == NULL) {
    return EFI_NOT_FOUND;
  }

  ///
  /// Find the Table protocol
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);

  ///
  /// Read tables from the storage file.
  ///
  Instance = 0;

  while (Status == EFI_SUCCESS) {
    ///
    /// Read the ACPI tables
    ///
    Status = FwVol->ReadSection (
                          FwVol,
                          &gDptfAcpiTableStorageGuid,
                          EFI_SECTION_RAW,
                          Instance,
                          (VOID **) &Table,
                          &Size,
                          &FvStatus
                          );

    if (!EFI_ERROR (Status)) {
      ///
      /// Check the table size is at least as large as an EFI_ACPI_COMMON_HEADER
      ///
      if (Size < sizeof (EFI_ACPI_COMMON_HEADER)) {
         if(Table != NULL) {
          gBS->FreePool (Table);
          Table = NULL;
         }
        return EFI_BUFFER_TOO_SMALL;
      }

      LoadTable = FALSE;
      TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

      switch (((EFI_ACPI_DESCRIPTION_HEADER*) TableHeader)->OemTableId) {

      case SIGNATURE_64 ('D', 'p', 't', 'f', 'T', 'a', 'b', 'l'):
        ///
        /// This is Intel(R) Dynamic Tuning Technology SSDT. Intel(R) Dynamic Tuning Technology should be enabled if we reach here so load the table.
        ///
        LoadTable = TRUE;
        DEBUG ((DEBUG_INFO, "Intel(R) Dynamic Tuning Technology: Found Intel(R) Dynamic Tuning Technology SSDT signature.\n"));

          if (mDptfSetupData.PpccObject==ACPI_OBJECT_DISABLE) {
            ///
            /// Invalidate objects before loading the SSDT by changing their names.
            /// For some Intel(R) Dynamic Tuning Technology validation, it is needed a capability to enable or disable the PPCC ACPI object.
            /// We do this twice to remove PPCC from Processor and Memory participants.
            ///
            UpdateCounter = 2; // Number of Objects to invalidate.
            for (CurrPtr = (UINT8 *) TableHeader + sizeof (EFI_ACPI_DESCRIPTION_HEADER); (CurrPtr <= ((UINT8 *) TableHeader + TableHeader->Length - sizeof(*Signature))) && (UpdateCounter !=0); CurrPtr++) {
              Signature = (UINT32 *) (CurrPtr + 1);
              if (*Signature == SIGNATURE_32 ('P', 'P', 'C', 'C')) {
                *Signature = SIGNATURE_32 ('P', 'P', 'C', 'X');
                UpdateCounter--;
              }
            }
          }

          if (mDptfSetupData.ArtgObject==ACPI_OBJECT_DISABLE) {
            ///
            /// Invalidate objects before loading the SSDT by changing their names.
            /// Intel(R) Dynamic Tuning Technology validation needs a capability to enable or disable ACPI objects.
            ///
            UpdateCounter = 1; // Number of Objects to invalidate.
            for (CurrPtr = (UINT8 *) TableHeader + sizeof (EFI_ACPI_DESCRIPTION_HEADER); (CurrPtr <= ((UINT8 *) TableHeader + TableHeader->Length- sizeof(*Signature))) && (UpdateCounter !=0); CurrPtr++) {
              Signature = (UINT32 *) (CurrPtr + 1);
              if (*Signature == SIGNATURE_32 ('A', 'R', 'T', 'G')) {
                *Signature = SIGNATURE_32 ('A', 'R', 'T', 'X');
                UpdateCounter--;
              }
            }
          }

          if (mDptfSetupData.PmaxObject==ACPI_OBJECT_DISABLE) {
            ///
            /// Invalidate objects before loading the SSDT by changing their names.
            /// Intel(R) Dynamic Tuning Technology validation needs a capability to enable or disable ACPI objects.
            ///
            UpdateCounter = 1; // Number of Objects to invalidate.
            for (CurrPtr = (UINT8 *) TableHeader + sizeof (EFI_ACPI_DESCRIPTION_HEADER); (CurrPtr <= ((UINT8 *) TableHeader + TableHeader->Length - sizeof(*Signature))) && (UpdateCounter !=0); CurrPtr++) {
              Signature = (UINT32 *) (CurrPtr + 1);
              if (*Signature == SIGNATURE_32 ('P', 'M', 'A', 'X')) {
                *Signature = SIGNATURE_32 ('P', 'M', 'X', 'X');
                UpdateCounter--;
              }
            }
          }

          // Update the DptfNvs.asl Operation Region to the allocated address
          // OperationRegion space (14 bytes) = 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1 + size of (UINT16)
          for (CurrPtr = (UINT8 *) TableHeader + sizeof (EFI_ACPI_DESCRIPTION_HEADER); CurrPtr <= ((UINT8 *) TableHeader + TableHeader->Length - 14); CurrPtr++) {
            Signature = (UINT32 *) (CurrPtr + 1);
            if ((*CurrPtr == AML_EXT_REGION_OP) && (*Signature == SIGNATURE_32 ('D', 'N', 'V', 'S'))) {
              DEBUG ((DEBUG_INFO, "Intel(R) Dynamic Tuning Technology: Update NVS Area %X\n",*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) ));
              DEBUG ((DEBUG_INFO, "Intel(R) Dynamic Tuning Technology: Update NVS Area %X\n",*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) ));
              ASSERT (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) == 0xFFFF0000);
              if (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) != 0xFFFF0000) {
                return EFI_NOT_FOUND;
              }
              ASSERT (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) == 0xAA55);
              if (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) != 0xAA55) {
                return EFI_NOT_FOUND;
              }
              ///
              /// DPTF NVS Area address
              ///
              *(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) = (UINT32) (UINTN) mDptfNvsAreaProtocol.Area;
              ///
              /// DPTF NVS Area size
              ///
              *(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) = sizeof (DPTF_NVS_AREA);
              DEBUG ((DEBUG_INFO, "Intel(R) Dynamic Tuning Technology: Update NVS Area\n"));
            }
          }
        break;

      default:
        break;
      }

      ///
      /// Add the table
      ///
      if (LoadTable) {
        TableHandle = 0;
        ///
        /// Check the length field isn't larger than the size read in section
        ///
        if (Table->Length > Size) {
          gBS->FreePool (Table);
          Table = NULL;
          return EFI_BAD_BUFFER_SIZE;
        }
        Status = AcpiTable->InstallAcpiTable (
                                  AcpiTable,
                                  Table,
                                  Table->Length,
                                  &TableHandle
                                  );
      }

      ///
      /// Increment the instance
      ///
      Instance++;
      gBS->FreePool (Table);
      Table = NULL;
    }

  }
  return EFI_SUCCESS;
}
