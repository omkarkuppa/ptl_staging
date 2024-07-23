/** @file
  USBC Retimer Compliance Mode Configure Library.

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

#include <Library/PeiUsbcRetimerConfigLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/EcTcssLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <SetupVariable.h>
#include <GpioV2Config.h>
#include <Register/GpioV2PcdPins.h>
#include <PlatformBoardConfig.h>

/**
  Configure TypeC Retimer FP pin for USB compliance mode function.
**/
VOID
UsbcRetimerComplianceModeConfig (
  VOID
  )
{
  GPIOV2_CONFIG                   GpioConfig;
  GPIOV2_PAD                      GpioPad;
  EFI_STATUS                      Status;
  SETUP_DATA                      SetupData;
  UINTN                           VarSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINT8                           EcDataBuffer;
  VPD_GPIO_PAD                    *VpdGpio;

  DEBUG ((DEBUG_INFO, "USBCRetimerComplianceModeConfig: Start\n"));
  EcDataBuffer = 0;

  //
  // Locate Setup variables
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "PeiServicesLocatePpi failed\n"));
    return;
  }

  VarSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &SetupData
                               );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "GetVariable (SetupData) failed\n"));
    return;
  }

  VpdGpio = PcdGetPtr (VpdPcdBoardRetimerForcePwrGpio);
  GpioPad = VpdGpio->GpioPad;
  DEBUG ((DEBUG_INFO, "Retimer Force Power Gpio is : 0x%08x\n", GpioPad));

  if (GpioPad != 0) {
    ZeroMem (&GpioConfig, sizeof (GPIOV2_CONFIG));
    GpioConfig.PadMode = GpioV2PadModeGpio;
    GpioConfig.HostOwn = GpioV2HostOwnAcpi;
    GpioConfig.Direction = GpioV2DirOut;
    GpioConfig.InterruptConfig = GpioV2IntDis;
    GpioConfig.ResetConfig = GpioV2ResetHost;
    GpioConfig.TerminationConfig = GpioV2TermNone;
    GpioConfig.LockTx = GpioV2Unlock;

    if (SetupData.TypecRetimerTxComplianceModeEn != 0) {
      if (SetupData.TypecRetimerPD0 == 1) { //PD 0 Retimer enable
        EcDataBuffer |= BIT0;
      } else {
        EcDataBuffer &= ~BIT0;
      }

      if (SetupData.TypecRetimerPD1 == 1) { //PD 1 Retimer enable
        EcDataBuffer |= BIT1;
      } else {
        EcDataBuffer &= ~BIT1;
      }

      if (SetupData.TypecRetimerPD2 == 1) { //PD 2 Retimer enable
        EcDataBuffer |= BIT2;
      } else {
        EcDataBuffer &= ~BIT2;
      }

      if (SetupData.TypecRetimerPD3 == 1) { //PD 3 Retimer enable
        EcDataBuffer |= BIT3;
      } else {
        EcDataBuffer &= ~BIT3;
      }

      GpioConfig.OutputState = GpioV2StateHigh;
      GpioV2ConfigurePad (GpioPad, &GpioConfig);
      UsbcRetimerCompliancePDMessage (&EcDataBuffer);
      DEBUG ((DEBUG_INFO, "Compliance Mode: Enable\nAssert Retimer FP Pin\n"));
    } else {
      EcDataBuffer = 0;
      DEBUG ((DEBUG_INFO, "TCompliance Mode: Disable\nNo Action\n"));
    }
  }

  DEBUG ((DEBUG_INFO, "USBCRetimerComplianceModeConfig: End\n"));
}
