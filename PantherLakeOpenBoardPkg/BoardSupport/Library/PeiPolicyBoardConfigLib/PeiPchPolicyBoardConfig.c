/** @file
 Intel PEI PCH Policy update by board configuration

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

@par Specification Reference:
**/

#include "PeiPolicyBoardConfig.h"
#include <Library/ConfigBlockLib.h>
#include <Pins/GpioPinsVer2Lp.h>
#include <SerialIoConfig.h>
#include <UsbConfig.h>
#include <PchPcieRpConfig.h>
#include <Library/PchInfoLib.h>
#include <PolicyUpdateMacro.h>

/**
  This function updates USB Policy per port OC Pin number

  @param[in]  PchUsbConfig     Pointer to USB_CONFIG data buffer
  @param[in]  PortIndex        USB Port index
  @param[in]  Pin              OverCurrent pin number
**/
VOID
UpdateUsb2OverCurrentPolicy (
  IN USB_CONFIG                 *UsbConfig,
  IN UINT8                      PortIndex,
  IN UINT8                      Pin
)
{
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspsUpd;
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#endif
  if (PortIndex < MAX_USB2_PORTS && ((Pin < USB_OC_MAX_PINS) || (Pin == USB_OC_SKIP))) {
    UPDATE_POLICY_V2 (
      ((FSPS_UPD *)FspsUpd)->FspsConfig.Usb2OverCurrentPin[PortIndex],
      UsbConfig->PortUsb20[PortIndex].OverCurrentPin,
      Pin
      );
  } else {
    if (PortIndex >= MAX_USB2_PORTS) {
      DEBUG ((DEBUG_ERROR, "UpdateUsb2OverCurrentPolicy: USB2 port number %d is not a valid USB2 port number\n", PortIndex));
    } else {
      DEBUG ((DEBUG_ERROR, "UpdateUsb2OverCurrentPolicy: Invalid OverCurrent pin specified USB2 port %d\n", PortIndex));
    }
  }
}

/**
  This function updates USB Policy per port OC Pin number

  @param[in]  PchUsbConfig     Pointer to USB_CONFIG data buffer
  @param[in]  PortIndex        USB Port index
  @param[in]  Pin              OverCurrent pin number
**/
VOID
UpdateUsb3OverCurrentPolicy (
  IN USB_CONFIG                 *UsbConfig,
  IN UINT8                      PortIndex,
  IN UINT8                      Pin
)
{
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspsUpd;
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#endif
  if (PortIndex < MAX_USB3_PORTS && ((Pin < USB_OC_MAX_PINS) || (Pin == USB_OC_SKIP))) {
    UPDATE_POLICY_V2 (
      ((FSPS_UPD *)FspsUpd)->FspsConfig.Usb3OverCurrentPin[PortIndex],
      UsbConfig->PortUsb30[PortIndex].OverCurrentPin,
      Pin
      );
  } else {
    if (PortIndex >= MAX_USB2_PORTS) {
      DEBUG ((DEBUG_ERROR, "UpdateUsb3OverCurrentPolicy: USB3 port number %d is not a valid USB3 port number\n", PortIndex));
    } else {
      DEBUG ((DEBUG_ERROR, "UpdateUsb3OverCurrentPolicy: Invalid OverCurrent pin specified USB3 port %d\n", PortIndex));
    }
  }
}

/**
  This function performs PEI PCH Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi         The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi               The RC POSTMEM Policy PPI instance

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyBoardConfig (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
  UINTN                        Index;
  SERIAL_IO_CONFIG             *SerialIoConfig;
  EFI_STATUS                   Status;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiPchPolicy\n"));
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                         *FspsUpd;
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#else
  SerialIoConfig = NULL;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
#endif

  //
  // I2C
  //
  for (Index = 0; Index < 8; Index++) {
    UPDATE_POLICY_V2 (((FSPS_UPD *) FspsUpd)->FspsConfig.SerialIoI2cMode[Index], SerialIoConfig->I2cDeviceConfig[Index].Mode, 0);
    UPDATE_POLICY_V2 (((FSPS_UPD *) FspsUpd)->FspsConfig.PchSerialIoI2cPadsTermination[Index], SerialIoConfig->I2cDeviceConfig[Index].PadTermination, 0);
  }

  UPDATE_POLICY_V2 (((FSPS_UPD *)FspsUpd)->FspsConfig.PchSerialIoI2cSdaPinMux[4], SerialIoConfig->I2cDeviceConfig[4].PinMux.Sda, GPIO_VER2_LP_MUXING_SERIALIO_I2C4_SDA_GPP_H8);
  UPDATE_POLICY_V2 (((FSPS_UPD *)FspsUpd)->FspsConfig.PchSerialIoI2cSclPinMux[4], SerialIoConfig->I2cDeviceConfig[4].PinMux.Scl, GPIO_VER2_LP_MUXING_SERIALIO_I2C4_SCL_GPP_H9);

  return EFI_SUCCESS;
}
