/** @file
 Intel PEI SA Pre-Memory Policy update by board configuration

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#include "PeiPolicyBoardConfig.h"
#include <TcssPeiPreMemConfig.h>
#include <PlatformBoardId.h>
#include <Library/PlatformUsbConfigLib.h>

#if (FixedPcdGetBool(PcdTcssSupport) == 1)
IOM_AUX_ORI_GPIO_PAD_CONFIG mIomAuxNullTable[MAX_IOM_AUX_BIAS_COUNT] = {
  // RECEPTACLE_SBU1_BIAS_CTL, RECEPTACLE_SBU2_BIAS_CTL, AUX_ISO_CTRL
  {0,                          0,                        0}, // Port 0
  {0,                          0,                        0}, // Port 1
  {0,                          0,                        0}, // Port 2
  {0,                          0,                        0}, // Port 3
};
#endif

/**
  This function performs PEI SA Pre-Memory Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyBoardConfigPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                         Status;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                               *FspmUpd;
#else
#if (FixedPcdGetBool(PcdTcssSupport) == 1)
  TCSS_PEI_PREMEM_CONFIG             *TcssPeiPreMemConfig;
  TcssPeiPreMemConfig = NULL;
#endif
#endif

#if (FixedPcdGetBool(PcdTcssSupport) == 1)
  UINT16                             BoardId;
  UINT16                             TcssIomAuxOriEnableBitmap;
  UINT32                             PortIndex;
  IOM_AUX_ORI_GPIO_PAD_CONFIG        *IomAuxOriOverrideTablePtr;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI    *VariableServices;
  UINTN                              VariableSize;
  SETUP_DATA                         SetupData;

  USBC_CONNECTOR_HOB_DATA            *UsbCConnectorHobDataPtr;
  USB_CONNECTOR_HOB_DATA             *UsbConnectorHobDataPtr;
  USB_CONNECTOR_BOARD_CONFIG         *UsbConnectorBoardConfig;
  UINT8                              AuxDpMode;
  UINT8                              ConnectorIndex;
#endif

  DEBUG ((DEBUG_INFO, "Updating SA Policy by board config in Pre-Mem\n"));

  Status = EFI_SUCCESS;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
#else
#if (FixedPcdGetBool(PcdTcssSupport) == 1)
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);
#endif
#endif

#if (FixedPcdGetBool(PcdTcssSupport) == 1)

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                  VariableServices,
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SetupData
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Ascertain BoardId
  //
  BoardId = PcdGet16(PcdBoardId);
  DEBUG((DEBUG_INFO, "BoardId is 0x%04X\n", BoardId));

  //
  // Initialize TCSS IOM_AUX_ORI_ENA BIT MAP to 0
  //
  TcssIomAuxOriEnableBitmap = 0;

  //
  // Initialize Aux and HSL orientation Table to mIomAuxNullTable.
  //
  IomAuxOriOverrideTablePtr = mIomAuxNullTable;

  //
  // Initialize IomUsbCDpConfig to 0 which indicates disabled.
  //
  for (ConnectorIndex = 0; ConnectorIndex < MAX_TCSS_USB3_PORTS; ConnectorIndex++) {
#if FixedPcdGet8(PcdFspModeSelection) == 1
    ((FSPM_UPD *) FspmUpd)->FspmConfig.IomUsbCDpConfig[ConnectorIndex] = 0;
#else
    TcssPeiPreMemConfig->IomConfig.IomUsbCDpConfig[ConnectorIndex] = 0;
#endif
  }
  //
  // Get UsbC Connector and Usb Connector hob data.
  //
  UsbCConnectorHobDataPtr = GetUsbCConnectorHobData ();
  UsbConnectorHobDataPtr = GetUsbConnectorHobData ();

  if (UsbConnectorHobDataPtr != NULL && UsbCConnectorHobDataPtr != NULL) {
    UsbConnectorBoardConfig = UsbConnectorHobDataPtr->UsbConnectorBoardConfig;
    for (ConnectorIndex = 0; ConnectorIndex < UsbCConnectorHobDataPtr->NumberOfUsbCConnectors; ConnectorIndex++, UsbConnectorBoardConfig++) {
      AuxDpMode = (UINT8) UsbCConnectorHobDataPtr->UsbCConnectorBoardConfig[ConnectorIndex].AuxDpMode;
      if (UsbConnectorBoardConfig->ConnectorConnectable == UNCONNECTABLE &&
          UsbConnectorBoardConfig->Usb3Controller == TCSS_USB3 &&
          UsbConnectorBoardConfig->ConnectorType == 0xFF &&
          AuxDpMode != 0) {
        if (UsbConnectorBoardConfig->Usb3PortNum < MAX_TCSS_USB3_PORTS) {
          UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IomUsbCDpConfig[UsbConnectorBoardConfig->Usb3PortNum], TcssPeiPreMemConfig->IomConfig.IomUsbCDpConfig[UsbConnectorBoardConfig->Usb3PortNum], AuxDpMode);
        }
      }
    }
  }
  //
  // Set Aux and HSL orientation Table based on BoardId value if AuxOriOverride is enable.
  //
  // TODO: As platforms are defined and added, new cases and values will need to be
  //       added to the below switch to set Aux and HSL orientations as appropriate.
  //
  if (SetupData.AuxOriOverride == 1) {
  switch (BoardId) {
    case BoardIdUnknown1:
    default:
      IomAuxOriOverrideTablePtr = mIomAuxNullTable;
      DEBUG ((DEBUG_INFO, "Unhandled BoardId 0x%04X\n", BoardId));
      break;
    }
  }

  for (PortIndex = 0; PortIndex < MAX_IOM_AUX_BIAS_COUNT; PortIndex++) {
    //
    // The TypeC Port GPIO config setting reference on board design
    //
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IomTypeCPortPadCfg[(PortIndex * 3)], TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[PortIndex].ReceptacleSbu1BiasCtrl, IomAuxOriOverrideTablePtr[PortIndex].ReceptacleSbu1BiasCtrl);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IomTypeCPortPadCfg[(PortIndex * 3) + 1], TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[PortIndex].ReceptacleSbu2BiasCtrl, IomAuxOriOverrideTablePtr[PortIndex].ReceptacleSbu2BiasCtrl);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IomTypeCPortPadCfg[(PortIndex * 3) + 2], TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[PortIndex].AuxIsoCtrl, IomAuxOriOverrideTablePtr[PortIndex].AuxIsoCtrl);
    if ((IomAuxOriOverrideTablePtr[PortIndex].ReceptacleSbu1BiasCtrl != 0) && (IomAuxOriOverrideTablePtr[PortIndex].ReceptacleSbu2BiasCtrl != 0)) {
      TcssIomAuxOriEnableBitmap |= (BIT0 << (PortIndex * 2));
    }
  }

  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TcssAuxOri, TcssPeiPreMemConfig->IomConfig.IomOverrides.AuxOri, TcssIomAuxOriEnableBitmap);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TcssHslOri, TcssPeiPreMemConfig->IomConfig.IomOverrides.HslOri, 0);
#if FixedPcdGet8(PcdFspModeSelection) == 1
  DEBUG ((DEBUG_INFO, "AuxOri is 0x%04X\n", ((FSPM_UPD *) FspmUpd)->FspmConfig.TcssAuxOri));
#else
  DEBUG ((DEBUG_INFO, "AuxOri is 0x%04X\n", TcssPeiPreMemConfig->IomConfig.IomOverrides.AuxOri));
#endif

#endif // #if (FixedPcdGetBool(PcdTcssSupport) == 1)

  return Status;
}
