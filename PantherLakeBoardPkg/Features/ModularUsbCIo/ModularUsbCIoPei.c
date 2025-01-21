/** @file
  Modular TypeC IO PEI driver

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

**/

#include "ModularUsbCIo.h"

EFI_STATUS
EFIAPI
ModularUsbCIoBoardDetectedCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

EFI_STATUS
EFIAPI
ModularUsbCIoPostMemCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mModularUsbCIoBoardDetectedNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gBoardDetectedPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) ModularUsbCIoBoardDetectedCallback
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mModularUsbCIoPostMemNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) ModularUsbCIoPostMemCallback
};

// Map DP connector to IOM DP type
STATIC UINT8 mDpConnector2IomDpMap[] = {
  IOM_HDMI,
  IOM_DP,
  IOM_EDP
};

STATIC DDI_CONFIG mDdiConfig[] = {
  {DdiHpdEnable,  DdiDdcEnable}, // HDMI
  {DdiHpdEnable,  DdiDisable},   // DP
  {DdiHpdEnable,  DdiDisable}    // eDP
};


/**
  Get USB Physical OC pin for TCSS convert USBA port

  @param[in] PortNum   TCSS port#

  @retval OcPin        USB Physical OC pin#
**/
UINT8
GetTcssUsbAOcPin (
  IN  UINT8       PortNum
  )
{
  UINT8                       OcPin;
  TCSS_COV_USBA_BOARD_CONFIG  *TcssCovUsbA;

  TcssCovUsbA = PcdGetPtr (VpdPcdTcssCovUsbA);

  switch (PortNum) {
    case 0:
      OcPin = (UINT8) TcssCovUsbA->Port1PhysicalOcPin;
      break;
    case 1:
      OcPin = (UINT8) TcssCovUsbA->Port2PhysicalOcPin;
      break;
    case 2:
      OcPin = (UINT8) TcssCovUsbA->Port3PhysicalOcPin;
      break;
    case 3:
      OcPin = (UINT8) TcssCovUsbA->Port4PhysicalOcPin;
      break;
    default:
      OcPin = 0xFF;
      break;
  }
  return OcPin;
}

/**
  Update DDI Config

  @param[in] TcssPort            TCSS Port
  @param[in] DdiPortConfig       DDI Port Config
  @param[in] HpdEnable           DDI HPD Enable/Disable
  @param[in] DdcEnable           DDI DDC Enable/Disable

  @retval EFI_SUCCESS            DDI Config is updated.
  @retval EFI_INVALID_PARAMETER  Port# is invalid.
**/
EFI_STATUS
UpdateDdiConfig (
  IN  UINT8       TcssPort,
  IN  UINT8       DdiPortConfig,
  IN  UINT8       HpdEnable,
  IN  UINT8       DdcEnable
  )
{
  VOID                    *Hob;
  UINT16                  DataSize;
  UINT8                   *DisplayDdiConfigTable;
  UINT8                   DisplayDdiConfigTableSize;
  VPD_DISPLAY_DDI_CONFIG  DisplayDdiConfigHob;
  VPD_DISPLAY_DDI_CONFIG  *DisplayDdiConfigPtr;

  if (TcssPort >= MAX_TCSS_USB3_PORTS) {
    return EFI_INVALID_PARAMETER;
  }

  Hob = GetFirstGuidHob (&gDisplayDdiConfigHobGuid);
  if (Hob == NULL) {
    ZeroMem (&DisplayDdiConfigHob, sizeof (VPD_DISPLAY_DDI_CONFIG));
    DisplayDdiConfigPtr = PcdGetPtr(VpdPcdDisplayDdiConfigTable);
    DataSize = (DisplayDdiConfigPtr->Size * sizeof (UINT8)) + sizeof (DisplayDdiConfigPtr->Size);
    CopyMem (&DisplayDdiConfigHob, DisplayDdiConfigPtr, DataSize);
    DisplayDdiConfigTable     = DisplayDdiConfigHob.DdiConfigTable;
    DisplayDdiConfigTableSize = DisplayDdiConfigPtr->Size;
  } else {
    DisplayDdiConfigTable     = (UINT8 *) ((VPD_DISPLAY_DDI_CONFIG *) GET_GUID_HOB_DATA (Hob))->DdiConfigTable;
    DisplayDdiConfigTableSize = ((VPD_DISPLAY_DDI_CONFIG *) GET_GUID_HOB_DATA (Hob))->Size;
  }

  if (TcssPort == 0 && DdiPortConfig == DdiPortEdp) {
    DisplayDdiConfigTable[DDI_PORT_B_CONFIG_OFFSET] = DdiPortConfig;
  }
  if ((TcssPort + TCP0_DDI_HPD_CONFIG_OFFSET + 7) < DisplayDdiConfigTableSize) {
    DisplayDdiConfigTable[TcssPort + TCP0_DDI_HPD_CONFIG_OFFSET]     = HpdEnable;
    DisplayDdiConfigTable[TcssPort + TCP0_DDI_HPD_CONFIG_OFFSET + 7] = DdcEnable;
  } else {
    DEBUG ((DEBUG_WARN, "%a: Over the size of DDI Config Table!!\n", __FUNCTION__));
  }

  if (Hob == NULL) {
    //
    // Create Hob for new DDI config
    //
    Hob = BuildGuidDataHob (
            &gDisplayDdiConfigHobGuid,
            &DisplayDdiConfigHob,
            DataSize
            );
    ASSERT (Hob != NULL);
  }

  return EFI_SUCCESS;
}

/**
  Convert EC modular USBC IO config to CSE modular USBC IO config

  @param[in] EcModularIoConfig   EC Modular IO Config Data Pointer

  @retval CseModularIoConfig     CSE Modular IO Config Data
**/
UINT32
ConvertModularUsbIoConfig (
  IN  EC_MODULAR_IO_CONFIG       *EcModularIoConfig
  )
{
  UINT8                          PortIndex;
  UINT32                         CseModularIoConfig;
  MODULAR_IO_TYPE_C_CONFIG_DATA  *CseModularIoConfigPtr;
  USB_CONNECTOR_HOB_DATA         *UsbConnectorHobPtr;
  USBC_CONNECTOR_HOB_DATA        *UsbCConnectorHobPtr;
  UINT8                          ConnectorIndex;

  CseModularIoConfig    = 0;
  CseModularIoConfigPtr = (MODULAR_IO_TYPE_C_CONFIG_DATA *) &CseModularIoConfig;
  UsbConnectorHobPtr  = GetUsbConnectorHobData ();
  UsbCConnectorHobPtr = GetUsbCConnectorHobData ();

  if (UsbConnectorHobPtr != NULL && UsbCConnectorHobPtr != NULL) {
    // Convert EC modular IO config to CSE modular IO config
    for (ConnectorIndex = 0; ConnectorIndex < UsbCConnectorHobPtr->NumberOfUsbCConnectors; ConnectorIndex++) {
      if ((UsbConnectorHobPtr->UsbConnectorBoardConfig[ConnectorIndex].Usb3Controller == TCSS_USB3) &&
          (UsbCConnectorHobPtr->UsbCConnectorBoardConfig[ConnectorIndex].ModularIoSupported == 1)) {
        PortIndex = (UINT8) UsbConnectorHobPtr->UsbConnectorBoardConfig[ConnectorIndex].Usb3PortNum;

        switch (PortIndex) {
          case MODULAR_IO_PORT1:
            CONVERT_EC_CONFIG_TO_CSE_CONFIG (1);
            break;
          case MODULAR_IO_PORT2:
            CONVERT_EC_CONFIG_TO_CSE_CONFIG (2);
            break;
          case MODULAR_IO_PORT3:
            CONVERT_EC_CONFIG_TO_CSE_CONFIG (3);
            break;
          case MODULAR_IO_PORT4:
            CONVERT_EC_CONFIG_TO_CSE_CONFIG (4);
            break;
          default:
            DEBUG ((DEBUG_WARN, "%a: Unsupported modular IO port index!!\n", __FUNCTION__));
            break;
        } // switch (PortIndex)
      }
    }
  }

  return CseModularIoConfig;
}

/**
  Update USBC properties according to EC modular IO config

  @param[in] EcModularIoConfig   EC Modular IO Config Data Pointer

  @retval    EFI_SUCCESS         Modular IO is supported.
  @retval    EFI_UNSUPPORTED     Modular IO is unsupported.
**/
EFI_STATUS
UpdateUsbCProperties (
  IN  EC_MODULAR_IO_CONFIG       *EcModularIoConfig
  )
{
  UINT8                          ConnectorIndex;
  USB_CONNECTOR_HOB_DATA         *UsbConnectorHobPtr;
  USBC_CONNECTOR_HOB_DATA        *UsbCConnectorHobPtr;
  UINT8                          PortIndex;
  UINT8                          TbtSupported;
  UINT8                          Usb4Supported;
  UINT8                          NumOfRetimer;
  UINT8                          OcPin;
  BOOLEAN                        ModularIoSupported;
  UINT8                          ConnectorType;
  GOP_CONFIG_DRIVER_HOB          *GopConfigDriverHob;
  EFI_HOB_GUID_TYPE              *GuidHob;

  ModularIoSupported  = FALSE;
  UsbConnectorHobPtr  = GetUsbConnectorHobData ();
  UsbCConnectorHobPtr = GetUsbCConnectorHobData ();
  GopConfigDriverHob  = NULL;

  GuidHob = GetFirstGuidHob (&gGopConfigDriverHobGuid);
  if (GuidHob != NULL) {
    GopConfigDriverHob = (GOP_CONFIG_DRIVER_HOB *) GET_GUID_HOB_DATA (GuidHob);
  } else {
    GopConfigDriverHob = BuildGuidHob (&gGopConfigDriverHobGuid, sizeof (GOP_CONFIG_DRIVER_HOB));
    ASSERT (GopConfigDriverHob != NULL);
    ZeroMem (GopConfigDriverHob, sizeof (GOP_CONFIG_DRIVER_HOB));
  }
  DEBUG ((DEBUG_INFO, "GopConfigDriverHob = 0x%X\n", GopConfigDriverHob));

  if (UsbConnectorHobPtr != NULL && UsbCConnectorHobPtr != NULL) {
    for (ConnectorIndex = 0; ConnectorIndex < UsbCConnectorHobPtr->NumberOfUsbCConnectors; ConnectorIndex++) {
      if (UsbConnectorHobPtr->UsbConnectorBoardConfig[ConnectorIndex].Usb3Controller == TCSS_USB3) {
        if (UsbCConnectorHobPtr->UsbCConnectorBoardConfig[ConnectorIndex].ModularIoSupported == 1) {
          ModularIoSupported = TRUE;
          PortIndex = (UINT8) UsbConnectorHobPtr->UsbConnectorBoardConfig[ConnectorIndex].Usb3PortNum;
          if (EcModularIoConfig->PortConfig[PortIndex].Fields.PortPresent == 1) {
            ConnectorType = (UINT8) EcModularIoConfig->PortConfig[PortIndex].Fields.ConnectorType;
            switch (ConnectorType) {
              case MODULAR_IO_CONNECTOR_TYPE_USBA:
                OcPin = GetTcssUsbAOcPin ((UINT8) UsbConnectorHobPtr->UsbConnectorBoardConfig[ConnectorIndex].Usb3PortNum);
                if (OcPin != 0xFF) {
                  UPDATE_USB_OC_PIN (PHYSICAL_OC, OcPin);
                } else {
                  UPDATE_USB_OC_PIN (0, 0);
                }
                UPDATE_USBC_DP_ATTRIBUTE (0);
                UPDATE_USB_CONNECTOR_HOB (CONNECTABLE, USB_TYPE_A);
                UPDATE_USBC_CONNECTOR_HOB (RETIMERLESS,
                                           PCIE_TUNNELING_UNSUPPORTED,
                                           DP_ALT_MODE_UNSUPPORTED,
                                           USB4_UNSUPPORTED,
                                           TBT3_UNSUPPORTED);
                UpdateDdiConfig (PortIndex, DdiPortDisabled, DdiHpdDisable, DdiDisable);
                break;
              case MODULAR_IO_CONNECTOR_TYPE_USBC:
                TbtSupported = (UINT8) EcModularIoConfig->PortConfig[PortIndex].Fields.TbtStatus;
                Usb4Supported = TbtSupported;
                NumOfRetimer = (UINT8) EcModularIoConfig->PortConfig[PortIndex].Fields.RetimerStatus;
                UPDATE_USBC_DP_ATTRIBUTE (0);
                UPDATE_USB_CONNECTOR_HOB (CONNECTABLE, USB_TYPE_C);
                UPDATE_USBC_CONNECTOR_HOB (NumOfRetimer,
                                           TbtSupported,
                                           DP_ALT_MODE_SUPPORTED,
                                           Usb4Supported,
                                           TbtSupported);
                UpdateDdiConfig (PortIndex, DdiPortDisabled, DdiHpdDisable, DdiDisable);
                break;
              case MODULAR_IO_CONNECTOR_TYPE_HDMI:
              case MODULAR_IO_CONNECTOR_TYPE_DP:
              case MODULAR_IO_CONNECTOR_TYPE_EDP:
                UPDATE_USBC_DP_ATTRIBUTE (
                  mDpConnector2IomDpMap[ConnectorType - MODULAR_IO_CONNECTOR_TYPE_HDMI]);
                UPDATE_USB_CONNECTOR_HOB (UNCONNECTABLE, 0xFF);
                UPDATE_USBC_CONNECTOR_HOB (RETIMERLESS,
                                           PCIE_TUNNELING_UNSUPPORTED,
                                           DP_ALT_MODE_UNSUPPORTED,
                                           USB4_UNSUPPORTED,
                                           TBT3_UNSUPPORTED);
                if (ConnectorType != MODULAR_IO_CONNECTOR_TYPE_EDP) {
                  if (GopConfigDriverHob != NULL) {
                    if (ConnectorType == MODULAR_IO_CONNECTOR_TYPE_HDMI) {
                      GopConfigDriverHob->ModuleTypeCId |= (MODULE_IO_VBT_HDMI << (PortIndex * MAX_MODULAR_IO_PORT_INDEX));
                    } else {
                      GopConfigDriverHob->ModuleTypeCId |= (MODULE_IO_VBT_DP << (PortIndex * MAX_MODULAR_IO_PORT_INDEX));
                    }
                  }
                  UpdateDdiConfig (PortIndex,
                                   DdiPortDisabled,
                                   mDdiConfig[ConnectorType - 3].DdiHpd,
                                   mDdiConfig[ConnectorType - 3].DdiDdc);
                } else {
                  if (PortIndex == 0) {
                    if (GopConfigDriverHob != NULL) {
                      GopConfigDriverHob->ModuleTypeCId |= (MODULE_IO_VBT_EDP << (PortIndex * MAX_MODULAR_IO_PORT_INDEX));
                    }
                    UpdateDdiConfig (PortIndex,
                                     DdiPortEdp,
                                     mDdiConfig[ConnectorType - 3].DdiHpd,
                                     mDdiConfig[ConnectorType - 3].DdiDdc);
                  } else {
                    DEBUG ((DEBUG_ERROR, "[TCSS] eDP is only supported on TCP0!!\n"));
                  }
                }
                break;
              default:
                DEBUG ((DEBUG_WARN, "%a: Unsupported modular IO connector type (0x%02x) at connector#%d!!\n",
                        __FUNCTION__, ConnectorType, ConnectorIndex));
                break;
            }
          } else {
            UPDATE_USB_CONNECTOR_HOB (UNCONNECTABLE, USB_TYPE_C);
          }
        }
      }
    }
  }

  if (ModularIoSupported == FALSE) {
    return EFI_UNSUPPORTED;
  } else {
    return EFI_SUCCESS;
  }
}

/**
  Check if system is booting from G3

  @retval TRUE     System is booting from G3.
  @retval FALSE    System is not ooting from G3.
**/
BOOLEAN
IsBootingFromG3 (
  VOID
  )
{
  SYS_G3_STATE_HOB    *SysG3StateHob;

  SysG3StateHob = (SYS_G3_STATE_HOB *) GetFirstGuidHob (&gG3StateHobGuid);

  if (SysG3StateHob == NULL) {
    DEBUG ((DEBUG_ERROR, "[TCSS] Failed to get System G3 State!!\n"));
    if (PmcIsPowerFailureDetected () == TRUE) {
      return TRUE;
    }
  } else {
    if (SysG3StateHob->SysG3Status == 1) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Get Current Modular USBC IO Config from CSE

  @param[in]  CseModularUsbCIoConfig  Modular USBC IO Config Pointer.

  @retval     EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
GetCurrentTcssStrapConfig (
  IN UINT32  *CseModularUsbCIoConfig
  )
{
  EFI_STATUS           Status;
  UINT32               StrapGroupsNumber;
  STRAP_OVERRIDE_DATA  StrapData;

  StrapGroupsNumber = 1;
  Status = PeiHeciGetStrapOverrideConfig (StrapGroupModularIoTypeCConfigStraps, &StrapGroupsNumber, &StrapData);
  if (!EFI_ERROR (Status)) {
    *CseModularUsbCIoConfig = StrapData.OverrideData.ConfigData;
  } else {
    DEBUG ((DEBUG_ERROR, "[TCSS] Failed to get modular USBC IO Config from CSE - %r\n", Status));
  }

  return Status;
}

/**
  Modular USBC IO Board Detected NotifyCallback.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
ModularUsbCIoBoardDetectedCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                       Status;
  UINT64                           PlatformModularUsbCIoConfig;
  EFI_BOOT_MODE                    BootMode;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  if (GetUsbConnectorHobData () == NULL) {
    BuildUsbConnectorHob ();
  }

  if (GetUsbCConnectorHobData () == NULL) {
    BuildUsbCConnectorHob ();
  }

  //
  // Get Platform Modular USBC IO Config from EC
  //
  Status = GetModularUsbCIoConfig (&PlatformModularUsbCIoConfig, sizeof (UINT64));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TCSS] Failed to get modular USBC IO config from EC - %r\n", Status));
    return Status;
  } else {
    DEBUG ((DEBUG_INFO, "[TCSS] Modular IO config read from EC: 0x%016lx\n", PlatformModularUsbCIoConfig));
    PcdSet64S (PcdPlatformModularUsbCIoConfig, PlatformModularUsbCIoConfig);
  }

  // Update USB and USBC connector HOBs accordingly
  Status = UpdateUsbCProperties ((EC_MODULAR_IO_CONFIG *) &PlatformModularUsbCIoConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Modular IO is unsupported!!\n", __FUNCTION__));
    return Status;
  }

  // Update the number of connectable USBC connectors and map
  Status = UpdateNumOfConnectableUsbCConnectorsAndMap ();
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  if (PlatformModularUsbCIoConfig != 0 && BootMode != BOOT_ON_S3_RESUME) {
    //
    // Install ME reset call back function.
    //
    RegisterMeReset ();

    //
    // Install Notify Callback for Strap Override
    //
    PeiServicesNotifyPpi (&mModularUsbCIoPostMemNotifyList);
  }

  return Status;
}

/**
  Modular USBC IO Post Mem NotifyCallback.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
ModularUsbCIoPostMemCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                         Status;
  UINT64                             PlatformModularUsbCIoConfig;
  UINT32                             CseModularUsbCIoConfig;
  UINT32                             Flags;
  STRAP_OVERRIDE_DATA                StrapData;
  PCH_RESET_DATA                     ResetData;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI    *VariableServices;
  UINT32                             *CurrentTcssStrapConfigPtr;
  UINT32                             CurrentTcssStrapConfig;
  UINTN                              VarSize;
  BOOLEAN                            IsFirstBoot;
  BOOLEAN                            VariableUpdateIsNeeded;
  VOID                               *Hob;
  UINT16                             DataSize;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  Hob                       = NULL;
  Flags                     = 0;
  IsFirstBoot               = FALSE;
  CurrentTcssStrapConfigPtr = NULL;
  VariableUpdateIsNeeded    = FALSE;
  CurrentTcssStrapConfig    = 0xFFFFFFFF;
  PlatformModularUsbCIoConfig = PcdGet64 (PcdPlatformModularUsbCIoConfig);
  CseModularUsbCIoConfig = ConvertModularUsbIoConfig ((EC_MODULAR_IO_CONFIG *) &PlatformModularUsbCIoConfig);
  DEBUG ((DEBUG_INFO, "[TCSS] Expected ModularIoConfig = 0x%08x\n", CseModularUsbCIoConfig));

  //
  // Check PWR FLR and clear it if it's set.
  //
  if (PmcIsPowerFailureDetected ()) {
    PmcClearPowerFailureStatus ();
  }

  Status = PeiServicesLocatePpi (
           &gEfiPeiReadOnlyVariable2PpiGuid,
           0,
           NULL,
           (VOID **) &VariableServices
           );
  if (!EFI_ERROR (Status)) {
    VarSize = sizeof (UINT32);
    CurrentTcssStrapConfigPtr = &CurrentTcssStrapConfig;
    Status = VariableServices->GetVariable (
                                 VariableServices,
                                 L"TcssStrapData",
                                 &gTcssStrapDataVariableGuid,
                                 NULL,
                                 &VarSize,
                                 CurrentTcssStrapConfigPtr
                                 );
    if (Status == EFI_NOT_FOUND) {
      IsFirstBoot = TRUE;
      CurrentTcssStrapConfigPtr = NULL;
    } else {
      ASSERT_EFI_ERROR (Status);
    }
  }

  //
  // If booting from G3, send HECI CMD 0x26 to override CSE modular USBC IO config.
  //
  if (IsBootingFromG3 () == TRUE) {
    if (IsFirstBoot == TRUE) {
      VariableUpdateIsNeeded = TRUE;
    } else {
      if (CurrentTcssStrapConfigPtr != NULL) {
        if (*CurrentTcssStrapConfigPtr != CseModularUsbCIoConfig) {
          VariableUpdateIsNeeded = TRUE;
        }
      } else {
        VariableUpdateIsNeeded = TRUE;
      }
    }

    StrapData.StrapGroupId = StrapGroupModularIoTypeCConfigStraps;
    StrapData.OverrideData.ConfigData = CseModularUsbCIoConfig;
    Status = PeiHeciSetStrapOverrideConfig (1, &StrapData, &Flags);
    DEBUG ((DEBUG_INFO, "[TCSS] Modular USBC IO Strap Overrride Status: %r\n", Status));

    if (!EFI_ERROR (Status)) {
      if (Flags & MODULAR_IO_GLOBAL_RESET_MASK) {
        DEBUG ((DEBUG_INFO, "Global reset is requested by CSE for Modular USBC IO. Reseting system ...\n"));
        CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
        StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
        (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
      }
      if (VariableUpdateIsNeeded) {
        CurrentTcssStrapConfig = CseModularUsbCIoConfig;
      }
    } else {
      DEBUG ((DEBUG_ERROR, "[TCSS] Failed to set modular USBC IO Config in CSE - %r\n", Status));
      Status = GetCurrentTcssStrapConfig (&CurrentTcssStrapConfig);
      if (!EFI_ERROR (Status)) {
        VariableUpdateIsNeeded = TRUE;
      }
    }
  } else {
    CurrentTcssStrapConfig = CseModularUsbCIoConfig;
    if (CurrentTcssStrapConfigPtr != NULL) {
      if (*CurrentTcssStrapConfigPtr != CseModularUsbCIoConfig) {
        Status = GetCurrentTcssStrapConfig (&CurrentTcssStrapConfig);
        if (!EFI_ERROR (Status)) {
          VariableUpdateIsNeeded = TRUE;
        }
      }
    } else {
      Status = GetCurrentTcssStrapConfig (&CurrentTcssStrapConfig);
      if (!EFI_ERROR (Status)) {
        VariableUpdateIsNeeded = TRUE;
      }
    }
  }

  DEBUG ((DEBUG_INFO, "[TCSS] Actual ModularIoConfig = 0x%08x\n", CurrentTcssStrapConfig));

  if (VariableUpdateIsNeeded) {
    DataSize = sizeof (MODULAR_IO_TYPE_C_CONFIG_DATA);
    //
    // Create Hob for TCSS strap config variable update
    //
    Hob = BuildGuidDataHob (
            &gTcssStrapDataHobGuid,
            &CurrentTcssStrapConfig,
            DataSize
            );
    ASSERT (Hob != NULL);
  }

  return Status;
}

/**
  Modular USBC IO PEI Driver Entry

  @param[in] FileHandle  Handle of the file being invoked.
  @param[in] PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCCESS    mModularUsbCIoBoardDetectedNotifyList is installed.
  @retval Others         Other errors as indicated.
**/
EFI_STATUS
EFIAPI
ModularUsbCIoEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  //
  // Install Board Detected Notify to update Platform settings for Modular IO
  //
  return PeiServicesNotifyPpi (&mModularUsbCIoBoardDetectedNotifyList);
}
