/** @file
  DTBT init Dxe driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include <DTbtInfoHob.h>
#include <Protocol/TbtDisBmeProtocol.h>
#include <Protocol/DxeDTbtPolicy.h>
#include <DTbtNvsArea.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/TbtCommonLib.h>
#include <Library/PcdLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeTbtDisBmeLib.h>
#include <Library/DTbtCommonLib.h>
#include <Library/DevicePathLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED DTBT_NVS_AREA_PROTOCOL                    mDTbtNvsAreaProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED DTBT_INFO_HOB                             *gDTbtInfoHob = NULL;

GLOBAL_REMOVE_IF_UNREFERENCED UINT8      mUsb4ClassCodeArgv = 0;

/**
  Initialize System Agent SSDT ACPI tables.

  @retval EFI_SUCCESS    ACPI tables are initialized successfully
  @retval EFI_NOT_FOUND  ACPI tables not found
**/
EFI_STATUS
InitializeDTbtSsdtAcpiTables (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  EFI_FV_FILETYPE               FileType;
  UINT32                        FvStatus;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  UINTN                         Size;
  UINTN                         i;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  INTN                          Instance;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  UINTN                         AcpiTableKey;
  UINT8                         *CurrPtr;
  UINT8                         *EndPtr;
  UINT32                        *Signature;
  EFI_ACPI_DESCRIPTION_HEADER   *TbtAcpiTable;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;

  FwVol       = NULL;
  TbtAcpiTable = NULL;
  ///
  /// Locate ACPI Table protocol
  ///
  DEBUG ((DEBUG_INFO, "Init DTBT SSDT table\n"));
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_WARN, "Fail to locate EfiAcpiTableProtocol.\n"));
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
  ASSERT (FwVol != NULL);
  if (FwVol == NULL) {
    DEBUG ((DEBUG_INFO, "TBT Global NVS table not found\n"));
    return EFI_NOT_FOUND;
  }
  ///
  /// Our exit status is determined by the success of the previous operations
  /// If the protocol was found, Instance already points to it.
  /// Read tables from the storage file.
  ///
  Instance      = 0;
  CurrentTable  = NULL;
  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiCallerIdGuid,
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
      if (((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->OemTableId == SIGNATURE_64 ('D','T', 'b', 't', 'S', 's', 'd', 't')) {
        TbtAcpiTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
        ///
        /// Locate the SSDT package
        ///
        CurrPtr = (UINT8 *) TbtAcpiTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
        /// OperationRegion space (14 bytes) = 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1 + sizeof(UINT16)
        EndPtr  = (UINT8 *) TbtAcpiTable + TbtAcpiTable->Length - 14;

        for (; CurrPtr <= EndPtr; CurrPtr++) {
          Signature = (UINT32 *) (CurrPtr + 1);
          if (*Signature == SIGNATURE_32 ('D', 'T', 'N', 'V')) {
            ASSERT (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) == 0xFFFF0000);
            if (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) != 0xFFFF0000) {
              return EFI_NOT_FOUND;
            }
            ASSERT (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) == 0xAA55);
            if (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) != 0xAA55) {
              return EFI_NOT_FOUND;
            }
            ///
            /// TBT Global NVS Area address
            ///
            *(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) = (UINT32) (UINTN) mDTbtNvsAreaProtocol.Area;
            ///
            /// TBT Global NVS Area size
            ///
            *(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) = sizeof (DTBT_NVS_AREA);

            AcpiTableKey = 0;
            Status = AcpiTable->InstallAcpiTable (
                                  AcpiTable,
                                  TbtAcpiTable,
                                  TbtAcpiTable->Length,
                                  &AcpiTableKey
                                  );
            ASSERT_EFI_ERROR (Status);
            gBS->FreePool (TbtAcpiTable);
            return EFI_SUCCESS;
          }
        }
      }
      ///
      /// Increment the instance
      ///
      Instance++;
      CurrentTable = NULL;
    }
  }
  return Status;
}
/**
  DisableDTbtBmeCallBackFunction

  Disable BME on DTbt tree at ExitBootServices to hand off security of TBT hierarchies to the OS.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
DisableDTbtBmeCallBackFunction (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  UINT8                     Index;
  SBDF                      Sbdf = {0,0,0,0,0};
  DTBT_INFO_HOB             *DTbtInfoHob = NULL;

  DEBUG((DEBUG_INFO, "[TBT] DisableDTbtBmeCallBackFunction START\n"));

  Index       = 0;

  //
  // Get DTbt INFO HOB
  //
  DTbtInfoHob = (DTBT_INFO_HOB *) GetFirstGuidHob (&gDTbtInfoHobGuid);
  if (DTbtInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to get gDTbtInfoHob\n"));
    goto Exit;
  }

  for (Index = 0; Index < PcdGet8 (PcdBoardDTbtControllerNumber); Index++) {
    DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtControllerConfig[%d].PcieRpNumber: %d\n", Index, DTbtInfoHob->DTbtControllerConfig[Index].PcieRpNumber));

    if (DTbtInfoHob->DTbtControllerConfig[Index].DTbtControllerEn == 1) {
      if (IS_DTBT_RP_NUM_VALID (DTbtInfoHob->DTbtControllerConfig, Index)) {
        Sbdf.Bus  = (UINT32) DTbtInfoHob->DTbtControllerConfig[Index].PcieRpBus;
        Sbdf.Dev  = (UINT32) DTbtInfoHob->DTbtControllerConfig[Index].PcieRpDev;
        Sbdf.Func = (UINT32) DTbtInfoHob->DTbtControllerConfig[Index].PcieRpFunc;
      }
      else {
        continue;
      }
      RecursiveTbtHierarchyConfiguration (Sbdf);
    } else {
      DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtControllerConfig[%d].DTbtControllerEn: %d\n", Index, DTbtInfoHob->DTbtControllerConfig[Index].DTbtControllerEn));
    }
  }

Exit:
  gBS->CloseEvent (Event);
  DEBUG((DEBUG_INFO, "[TBT] DisableDTbtBmeCallBackFunction END\n"));
}

EFI_DISABLE_TBT_BME_PROTOCOL mDisableDTbtBmeProtocol = {
    DisableDTbtBmeCallBackFunction,
};

/**
  Install DisableBme Protocol for DTBT UEFI-Shell Validation
**/
VOID
InstallDTbtDisableBmeProtocol (
  VOID
  )
{
  EFI_STATUS        Status;
  EFI_HANDLE        Handle;

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gDxeDisableDTbtBmeProtocolGuid,
                  &mDisableDTbtBmeProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "DTbtDisableBmeProtocol Failed. Status: %d\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "DTbtDisableBmeProtocol Installed\n"));
  }
}

/**
  DTBT NVS Area Initialize
**/
VOID
DTbtNvsAreaInit (
  IN  VOID              **mDTbtNvsAreaPtr
  )
{
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "DTbtNvsAreaInit Start\n"));

  Pages = EFI_SIZE_TO_PAGES (sizeof (DTBT_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);

  *mDTbtNvsAreaPtr = (VOID *) (UINTN) Address;
  SetMem (*mDTbtNvsAreaPtr, sizeof (DTBT_NVS_AREA), 0);

  DEBUG ((DEBUG_INFO, "DTbtNvsAreaInit End\n"));
}

/**
  DTBT NVS Area Update
**/
VOID
DTbtNvsAreaUpdate (
)
{
  DTBT_NVS_AREA_PROTOCOL        *DTbtNvsAreaProtocol;
  DXE_DTBT_POLICY_PROTOCOL      *DxeDTbtConfig;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "DTbtNvsAreaUpdate Start\n"));

  Status = gBS->LocateProtocol(
                  &gDxeDTbtPolicyProtocolGuid,
                  NULL,
                  (VOID **) &DxeDTbtConfig
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // DTBTNvsAreaProtocol default value init here
  //
  DTbtNvsAreaProtocol = (DTBT_NVS_AREA_PROTOCOL *) &mDTbtNvsAreaProtocol;

  //
  // Initialize default values for DTBT NVS
  //
  DTbtNvsAreaProtocol->Area->DiscreteTbtSupport      = ((gDTbtInfoHob->DTbtControllerConfig[0].DTbtControllerEn == 1 || gDTbtInfoHob->DTbtControllerConfig[1].DTbtControllerEn == 1) ? TRUE : FALSE);

  //
  // the dTBT RTD3 NVS setting is required to aligned between platform NVS and dTBT NVS
  //
  DTbtNvsAreaProtocol->Area->DTbtRtd3                = (UINT8) DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3;             // DTBT RTD3 Enable.
  DTbtNvsAreaProtocol->Area->DTbtRtd3OffDelay        = (UINT16) DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3OffDelay;    // DTBT RTD3 Off delay in ms.
  DTbtNvsAreaProtocol->Area->DTbtRtd3ClkReq          = (UINT8) DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3ClkReq;       // DTBT RTD3 ClkReq Mask Enable.
  DTbtNvsAreaProtocol->Area->DTbtRtd3ClkReqDelay     = (UINT16) DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3ClkReqDelay; // DTBT RTD3 ClkReq mask delay in ms.

  //
  // USB4 Class Code option
  //
  DTbtNvsAreaProtocol->Area->Usb4ClassOption         = gDTbtInfoHob->DTbtGenericConfig.Usb4ClassOption;

  //
  // DTBT Controller 0
  //
  DTbtNvsAreaProtocol->Area->RootportSelected0       = gDTbtInfoHob->DTbtControllerConfig[0].PcieRpNumber;
  DTbtNvsAreaProtocol->Area->RootportAcpiNumber0     = gDTbtInfoHob->DTbtControllerConfig[0].RpAcpiNumber;
  DTbtNvsAreaProtocol->Area->RootportSelected0Type   = gDTbtInfoHob->DTbtControllerConfig[0].RpType;
  DTbtNvsAreaProtocol->Area->DTbtWakeGpioPin0        = gDTbtInfoHob->DTbtControllerConfig[0].WakeGpioPin;
  DTbtNvsAreaProtocol->Area->RootportEnabled0        = gDTbtInfoHob->DTbtControllerConfig[0].DTbtControllerEn;

  //
  // DTBT Controller 1
  //
  DTbtNvsAreaProtocol->Area->RootportSelected1       = gDTbtInfoHob->DTbtControllerConfig[1].PcieRpNumber;
  DTbtNvsAreaProtocol->Area->RootportAcpiNumber1     = gDTbtInfoHob->DTbtControllerConfig[1].RpAcpiNumber;
  DTbtNvsAreaProtocol->Area->RootportSelected1Type   = gDTbtInfoHob->DTbtControllerConfig[1].RpType;
  DTbtNvsAreaProtocol->Area->DTbtWakeGpioPin1        = gDTbtInfoHob->DTbtControllerConfig[1].WakeGpioPin;
  DTbtNvsAreaProtocol->Area->RootportEnabled1        = gDTbtInfoHob->DTbtControllerConfig[1].DTbtControllerEn;

  DEBUG ((DEBUG_INFO, "DTbtNvsAreaUpdate End\n"));
}

VOID
DtbtUpdateUsb4Ver2ClassCode (VOID)
{
  UINT64        Tbt2Pcie;
  UINT64        Pcie2Tbt;
  UINT32        RegVal;
  UINT8         Index;
  UINT8         RpBus;
  UINT8         RpDev;
  UINT8         RpFunc;
  UINT32        OrgBusInfo;
  UINT32        DevId;
  UINT8         TbtBus;
  BOOLEAN       BusRestore;

  DEBUG ((DEBUG_INFO, "DtbtUpdateUsb4Ver2ClassCode entry\n"));

  if (gDTbtInfoHob != NULL) {
    for (Index = 0; Index < PcdGet8 (PcdBoardDTbtControllerNumber); Index++) {
      if (gDTbtInfoHob->DTbtControllerConfig[Index].DTbtControllerEn == 1) {
        if (IS_DTBT_RP_NUM_VALID (gDTbtInfoHob->DTbtControllerConfig, Index)) {
            RpBus  = gDTbtInfoHob->DTbtControllerConfig[Index].PcieRpBus;
            RpDev  = gDTbtInfoHob->DTbtControllerConfig[Index].PcieRpDev;
            RpFunc = gDTbtInfoHob->DTbtControllerConfig[Index].PcieRpFunc;
        }
        else {
          continue;
        }
        OrgBusInfo = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET));
        DEBUG ((DEBUG_INFO, "OrgBusInfo = 0x%08x\n", OrgBusInfo));
        TbtBus = (OrgBusInfo & 0x0000FF00) >> 8;
        if ((TbtBus == 0) || (TbtBus == 0xFF)) {
          DEBUG ((DEBUG_INFO, "Assign a temporary bus number 0x%x for sending TBT mailbox\n", (UINT8) RpBus + TbtBus));
          TbtBus = (UINT8) RpBus + FixedPcdGet8 (PcdDTbtTempBusNumber);
          //
          // Set Sec/Sub buses to a temporary value
          //
          PciSegmentWrite8 (PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET), TbtBus);
          PciSegmentWrite8 (PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET), TbtBus);
          BusRestore = TRUE;
        } else {
          BusRestore = FALSE;
        }
        //
        // Send Set Class code command if the host router supports USB4 Ver2
        //
        DevId = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (0, TbtBus, 0, 0, 0));
        if (DtbtQueryUsb4Ver (DevId & 0x0000FFFF, DevId >> 16) == 2) {
          Tbt2Pcie = PCI_SEGMENT_LIB_ADDRESS (0, TbtBus, 0, 0, FixedPcdGet16 (PcdDTbtToPcieRegister));
          Pcie2Tbt = PCI_SEGMENT_LIB_ADDRESS (0, TbtBus, 0, 0, FixedPcdGet16 (PcdPcieToDTbtRegister));
          DEBUG ((DEBUG_INFO, "Send Set Class Code command with data 0x%x to TBT %d\n", mUsb4ClassCodeArgv, Index));
          PciSegmentWrite32 (Pcie2Tbt, (mUsb4ClassCodeArgv << 8) | PCIE2TBT_SET_CLASS_CODE | PCIE2TBT_VLD_B);
          if (InternalWaitforCommandCompletion (Tbt2Pcie, 500, TRUE, &RegVal)) {
            DEBUG ((DEBUG_INFO, "Set Class Code with ARGV=%d to TBT %d successfully\n", mUsb4ClassCodeArgv, Index));
          } else {
            DEBUG ((DEBUG_ERROR, "Fail to set USB4 Class Code for TBT %d - RegVal = %x \n", Index, RegVal));
          }
          PciSegmentWrite32 (Pcie2Tbt, 0);
          InternalWaitforCommandCompletion (Tbt2Pcie, 500, FALSE, &RegVal);
        }
        if (BusRestore) {
          //
          // Reset Sec/Sub buses to original value
          //
          PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), OrgBusInfo);
        }
      }  // if (gDTbtInfoHob->DTbtControllerConfig[Index].DTbtControllerEn == 1)
    }  // for (Index = 0; Index < PcdGet8 (PcdBoardDTbtControllerNumber); Index++)
  }

  DEBUG ((DEBUG_INFO, "DtbtUpdateUsb4Ver2ClassCode exit\n"));
}

/**
  This function gets registered as a callback to patch TBT ASL code

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
DTbtAcpiEndOfDxeCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  UINT8                                 RpBus;
  UINT8                                 RpDev;
  UINT8                                 RpFunc;
  UINT16                                HrDeviceId;
  BOOLEAN                               EnableResetBridge;
  UINT8                                 HostRouterBus;
  UINT8                                 DTbtControllerBus;
  UINT8                                 RootPortSubordinateBus;
  UINT8                                 Index;

  EnableResetBridge = FALSE;

  if (gDTbtInfoHob != NULL) {

    // Check and Update the Validity and Availability of TBT Host Router
    if (gDTbtInfoHob->DTbtGenericConfig.TbtRtd3PepEnable) {
      for (Index = 0; Index < PcdGet8 (PcdBoardDTbtControllerNumber); Index++) {
        if (gDTbtInfoHob->DTbtControllerConfig[Index].DTbtControllerEn == 1) {
          if (IS_DTBT_RP_NUM_VALID (gDTbtInfoHob->DTbtControllerConfig, Index)) {
            RpBus  = gDTbtInfoHob->DTbtControllerConfig[Index].PcieRpBus;
            RpDev  = gDTbtInfoHob->DTbtControllerConfig[Index].PcieRpDev;
            RpFunc = gDTbtInfoHob->DTbtControllerConfig[Index].PcieRpFunc;
          }
          else {
            continue;
          }

          HostRouterBus = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (0, RpBus, (UINT32) RpDev, (UINT32) RpFunc, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
          RootPortSubordinateBus = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (0, RpBus, (UINT32) RpDev, (UINT32) RpFunc, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));
          DEBUG ((DEBUG_INFO, "Validating dTBT %d Controller HR for TBT PEP Constraint(), TBT_HR_BUS = 0x%x \n", Index, HostRouterBus));
          DTbtControllerBus = HostRouterBus;

          if (HostRouterBus == 0 || HostRouterBus == 0xFF) {
            //
            // Set Sec/Sub buses to a temporary value
            //
            PciSegmentWrite8 (PCI_SEGMENT_LIB_ADDRESS (0, RpBus, RpDev, RpFunc, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET), (UINT8) RpBus + FixedPcdGet8 (PcdDTbtTempBusNumber));
            PciSegmentWrite8 (PCI_SEGMENT_LIB_ADDRESS (0, RpBus, RpDev, RpFunc, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET), (UINT8) RpBus + FixedPcdGet8 (PcdDTbtTempBusNumber));
            DTbtControllerBus = (UINT8) RpBus + FixedPcdGet8 (PcdDTbtTempBusNumber);
            DEBUG ((DEBUG_INFO, "HostRouterBus is set to a temporary bus number: 0x%x\n", HostRouterBus));
            EnableResetBridge = TRUE;
          }

          //
          // AS TBT PEP should be enable only if TBT is in RTD3 mode. So Tbt
          // Host Router Device ID should be visible now. If Device Id = 0xFFFF or
          // Device ID is not a Tbt Host Router Device ID then dTBT card is not
          // connected. So disable Corresponding TBT RP Enable NVS Variable and exit.
          //
          HrDeviceId = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (0, DTbtControllerBus, 0, 0, PCI_DEVICE_ID_OFFSET));

          switch (Index) {
          case 0:
            mDTbtNvsAreaProtocol.Area->RootportEnabled0 = IsTbtHostRouter (HrDeviceId);
            break;
          case 1:
            mDTbtNvsAreaProtocol.Area->RootportEnabled1 = IsTbtHostRouter (HrDeviceId);
            break;
          default:
            DEBUG ((DEBUG_INFO, "TBT RTD3 PEP Invalid Controller Number.\n"));
            break;
          }
          if (EnableResetBridge == TRUE) {
            //
            // Reset Sec/Sub buses to original value
            //
            PciSegmentWrite8 (PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET), HostRouterBus);
            PciSegmentWrite8 (PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET), RootPortSubordinateBus);
            EnableResetBridge = FALSE;
          }
        }
      }
    }
  }

  //
  // Update USB4 Class Code of host router if USB4 Ver2 support is enabled
  //
  if (mUsb4ClassCodeArgv != 0) {
    DtbtUpdateUsb4Ver2ClassCode ();
  }

  return;
}

/**
  Initialize Thunderbolt(TM) SSDT ACPI tables

  @retval EFI_SUCCESS    ACPI tables are initialized successfully
  @retval EFI_NOT_FOUND  ACPI tables not found
**/

EFI_STATUS
EFIAPI
DTbtDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_HANDLE                Handle;
  EFI_EVENT                 EndOfDxeEvent;
  EFI_EVENT                 ExitBootServiceEvent;
  DXE_DTBT_POLICY_PROTOCOL  *DxeTbtConfig;

  Status                    = EFI_SUCCESS;
  Handle                    = NULL;
  EndOfDxeEvent             = NULL;
  ExitBootServiceEvent      = NULL;
  DxeTbtConfig              = NULL;

  DEBUG ((DEBUG_INFO, "[TBT] DTbtDxeEntryPoint START\n"));

  //
  // Get DTBT INFO HOB
  //
  gDTbtInfoHob = (DTBT_INFO_HOB *) GetFirstGuidHob (&gDTbtInfoHobGuid);
  if (gDTbtInfoHob == NULL) {
    DEBUG ((DEBUG_INFO, "gDTbtInfoHobGuid not found\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Initialize DTbt Nvs Area
  //
  DTbtNvsAreaInit ((VOID **) &mDTbtNvsAreaProtocol.Area);

  //
  // Install DTbt Global NVS ACPI table
  //
  Status = InitializeDTbtSsdtAcpiTables ();

  //
  // Update DTbt Nvs Area
  //
  DTbtNvsAreaUpdate ();

  //
  // [ACPI] DTBT ACPI table
  //

  Handle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gDTbtNvsAreaProtocolGuid,
                  &mDTbtNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register an end of DXE event for DTBT ACPI to do some patch
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  DTbtAcpiEndOfDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // USB4 Set Class Code ARGV = 2 if USB4 Class Code option is 'USB4 Standard' for loading OS inbox driver.
  // USB4 Set Class Code ARGV = 1 if USB4 Class Code option is 'Auto' or 'Intel USB4 Ver2' for loading Intel driver.
  // _OSC will set class code again if USB4 Class Code option is 'Auto' and OS inbox driver should be loaded.
  //
  mUsb4ClassCodeArgv = (gDTbtInfoHob->DTbtGenericConfig.Usb4ClassOption == 2) ? 2 : 1;

  Status = gBS->LocateProtocol (
              &gDxeDTbtPolicyProtocolGuid,
              NULL,
              (VOID **) &DxeTbtConfig
              );
  if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to DxeTbtConfig, Status: %d\n", Status));
      goto Exit;
  }

  if (DxeTbtConfig->TbtCommonConfig.TbtVtdBaseSecurity == TRUE) {

    //
    // Register a Exit Boot Service for disable DTbt BME
    //
    DEBUG ((DEBUG_INFO, "Register a Exit Boot Service for disable DTbt BME\n"));
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    DisableDTbtBmeCallBackFunction,
                    NULL,
                    &gEfiEventExitBootServicesGuid,
                    &ExitBootServiceEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to Register a Exit Boot Service for disable DTbt BME, Status: %d\n", Status));
      gBS->CloseEvent (ExitBootServiceEvent);
      goto Exit;
    }

    //
    // Install DTbt DisableBme for UEFI-Shell testing
    //
    InstallDTbtDisableBmeProtocol ();
  }

Exit:
  DEBUG ((DEBUG_INFO, "[TBT] DTbtDxeEntryPoint END\n"));
  return Status;
}
