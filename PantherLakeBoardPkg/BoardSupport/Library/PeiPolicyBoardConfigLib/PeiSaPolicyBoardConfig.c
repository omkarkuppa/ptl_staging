/** @file
 Intel PEI SA Policy update by board configuration

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
#if FixedPcdGetBool(PcdTcssSupport) == 1
#include <TcssPeiPreMemConfig.h>
#include <TcssPeiConfig.h>
#include <PlatformBoardConfig.h>
#include <Library/PlatformUsbConfigLib.h>
#include <TcssInfo.h>
#endif
#include <PlatformBoardId.h>
#include <PolicyUpdateMacro.h>


/**
  This function performs PEI SA Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyBoardConfig (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
  EFI_STATUS                         Status;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspsUpd;
  VOID                            *FspmUpd;
#endif
  TCSS_PEI_CONFIG     *TcssConfig;
  TcssConfig          = NULL;

#if FixedPcdGetBool(PcdTcssSupport) == 1
    TCSS_PEI_PREMEM_CONFIG             *TcssPeiPreMemConfig;
    TcssPeiPreMemConfig = NULL;
#endif

#if FixedPcdGetBool(PcdTcssSupport) == 1
  UINT8                              PortIndex;
  USB_CONNECTOR_BOARD_CONFIG         *UsbConnectorBoardConfig;
  UINT8                              ConnectorIndex;
  USB_CONNECTOR_HOB_DATA             *UsbConnectorHobDataPtr;
#endif

  EFI_PEI_READ_ONLY_VARIABLE2_PPI    *VariableServices;
  UINTN                              VariableSize;
  SETUP_DATA                         SetupData;

  DEBUG ((DEBUG_INFO, "Updating SA Policy by board config in Post-Mem\n"));
  Status = EFI_SUCCESS;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
  if (FspsUpd == NULL){
    return EFI_NOT_FOUND;
  }
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
  if (FspmUpd == NULL){
    return EFI_NOT_FOUND;
  }
#else
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gTcssPeiConfigGuid, (VOID *) &TcssConfig);
  ASSERT_EFI_ERROR (Status);
#endif // FspMode Check

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

#if FixedPcdGetBool(PcdTcssSupport) == 1
  if (TcssConfig != NULL) {
    //
    // Ascertain BoardId
    //
    DEBUG((DEBUG_INFO, "BoardId is 0x%04X\n", PcdGet16(PcdBoardId)));

    UsbConnectorHobDataPtr = GetUsbConnectorHobData ();
    if (UsbConnectorHobDataPtr != NULL) {
      UsbConnectorBoardConfig = UsbConnectorHobDataPtr->UsbConnectorBoardConfig;

      if (UsbConnectorBoardConfig != NULL) {
        // Initialize TCSS USB3 OC Config
        for (PortIndex = 0; PortIndex < MAX_USB3_PORTS; PortIndex++) {
          UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.CpuUsb3OverCurrentPin[PortIndex], TcssConfig->UsbConfig.PortUsb30[PortIndex].OverCurrentPin, USB_OC_SKIP);
        }

        // Update TCSS USB3 USB OC Config
        DEBUG ((DEBUG_INFO, "Updating USB OC mapping for %d TCSS USB3 ports\n", MAX_TCSS_USB3_PORTS));
        for (ConnectorIndex = 0; ConnectorIndex < UsbConnectorHobDataPtr->NumberOfUsbConnectors; ConnectorIndex++, UsbConnectorBoardConfig++) {
          if (UsbConnectorBoardConfig->ConnectorConnectable == CONNECTABLE && UsbConnectorBoardConfig->Usb3Controller == TCSS_USB3) {
            if (UsbConnectorBoardConfig->Usb3PortNum < MAX_TCSS_USB3_PORTS) {
              if (UsbConnectorBoardConfig->UsbOcPinType != 0) {
                if (UsbConnectorBoardConfig->UsbOcPinType <= USB_OC_MAX_TYPE &&
                    UsbConnectorBoardConfig->UsbOcPin < USB_OC_MAX_PINS) {
                  UPDATE_POLICY (
                    ((FSPS_UPD *) FspsUpd)->FspsConfig.CpuUsb3OverCurrentPin[UsbConnectorBoardConfig->Usb3PortNum],
                    TcssConfig->UsbConfig.PortUsb30[UsbConnectorBoardConfig->Usb3PortNum].OverCurrentPin,
                    (UINT8) GET_USB3_OCM_REG (UsbConnectorBoardConfig->UsbOcPinType, UsbConnectorBoardConfig->UsbOcPin)
                    );
                } else {
                  DEBUG ((DEBUG_ERROR, "Invalid OverCurrent pin specified TCSS USB3 port %d\n", UsbConnectorBoardConfig->Usb3PortNum));
                }
              }
            } else {
              DEBUG ((
                DEBUG_ERROR,
                "TCSS USB3 port#%d is over the max port number supported by platform\n",
                UsbConnectorBoardConfig->Usb3PortNum
                ));
            }
          }
        }
      } else {
        DEBUG ((DEBUG_ERROR, "UsbConnectorBoardConfig is not available\n"));
      }
    }

    for (PortIndex = 0; PortIndex < MAX_TCSS_USB3_PORTS; PortIndex++) {
      UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PortUsb30Enable[PortIndex], TcssConfig->UsbConfig.PortUsb30[PortIndex].Enable, IS_TC_PORT_USB_SUPPORTED (TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[PortIndex]));
    }
  }
#endif  // PcdTcssSupport
  return Status;
}
