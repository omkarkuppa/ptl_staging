/** @file
  Init for MipiCam.

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

#include <Uefi.h>
#include <PiPei.h>
#include <SetupVariable.h>
#include <MipiCamConfig.h>
#include <Register/GpioV2PcdPins.h>
#include <GpioV2Config.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Library/HobLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Guid/MipiCamConfigHob.h>
#if FixedPcdGet8 (PcdEmbeddedEnable) == 0x1
  #include <Pins/GpioPinsVer2Lp.h>
  #define HDAC_I2S_EVEREST8316  3
  #define HDAC_I2S_EVEREST8326  4
  #define HDAC_I2S_EVEREST8336  5
  #define HDAC_I2S_ALC5682I_VD  6
  #define HDAC_I2S_ALC5682I_VS  7
#endif

VOID
MipiCamConfigureGpio (
  IN GPIOV2_CONFIG  *GpioConfig,
  IN UINT8          GpioFunctions,
  IN UINT8          GpioOutputState
  )
{
  ZeroMem (GpioConfig, sizeof (GPIOV2_CONFIG));

#if FixedPcdGetBool (PcdMipiLontiumEnable) == 0x0
  GpioConfig->PadMode           = GpioV2PadModeGpio;
  GpioConfig->HostOwn           = GpioV2HostOwnGpio;
  GpioConfig->Direction         = GpioV2DirOut;
  GpioConfig->OutputState       = GpioV2StateDefault;
  GpioConfig->InterruptConfig   = GpioV2IntDis;
  GpioConfig->ResetConfig       = GpioV2ResetHost;
  GpioConfig->TerminationConfig = GpioV2TermDefault;
#else
  /* Ready Status 0x13 */
  if (GpioFunctions == 0x13) {
    DEBUG ((DEBUG_INFO, "MIPI Ready/HDMI Input Pin\n"));
    GpioConfig->PadMode           = GpioV2PadModeGpio;
    GpioConfig->HostOwn           = GpioV2HostOwnGpio;
    GpioConfig->Direction         = GpioV2DirIn;
    GpioConfig->OutputState       = GpioV2StateDefault;
    GpioConfig->InterruptConfig   = GpioV2IntEdge | GpioV2IntApic;
    GpioConfig->ResetConfig       = GpioV2ResetHost;
    GpioConfig->TerminationConfig = GpioV2TermDefault;
    GpioConfig->LockConfig        = GpioV2Unlock;
    GpioConfig->LockTx            = GpioV2Unlock;
  } else if (GpioFunctions == 0x14) { /* HDMI detect 0x14 */
    DEBUG ((DEBUG_INFO, "MIPI AOB HDMI Detect Input Pin\n"));
    GpioConfig->PadMode           = GpioV2PadModeGpio;
    GpioConfig->HostOwn           = GpioV2HostOwnGpio;
    GpioConfig->Direction         = GpioV2DirIn;
    GpioConfig->OutputState       = GpioV2StateDefault;
    GpioConfig->InterruptConfig   = GpioV2IntBothEdge | GpioV2IntApic;
    GpioConfig->ResetConfig       = GpioV2ResetHost;
    GpioConfig->TerminationConfig = GpioV2TermDefault;
    GpioConfig->LockConfig        = GpioV2Unlock;
    GpioConfig->LockTx            = GpioV2Unlock;
  } else {
    DEBUG ((DEBUG_INFO, "%a - Some Other pins\n", __FUNCTION__));

    GpioConfig->PadMode   = GpioV2PadModeGpio;
    GpioConfig->HostOwn   = GpioV2HostOwnGpio;
    GpioConfig->Direction = GpioV2DirOut;

    if (GpioOutputState == 0) {
      GpioConfig->OutputState = GpioV2StateLow;
    } else if (GpioOutputState == 1) {
      GpioConfig->OutputState = GpioV2StateHigh;
    } else {
      GpioConfig->OutputState = GpioV2StateDefault;
    }

    GpioConfig->InterruptConfig   = GpioV2IntDis;
    GpioConfig->ResetConfig       = GpioV2ResetHost;
    GpioConfig->TerminationConfig = GpioV2TermDefault;
    GpioConfig->LockConfig        = GpioV2Unlock;
    GpioConfig->LockTx            = GpioV2Unlock;
  }
#endif
}

VOID
DumpGpioConfig (
  IN GPIOV2_PAD  GpioPad
  )
{
#if FixedPcdGet8 (PcdEmbeddedEnable) == 1
  EFI_STATUS        Status;
  GPIOV2_CONFIG     GpioConfig;

  Status = GpioV2GetPadConfiguration (GpioPad, &GpioConfig);
  DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioGetPadConfig GpioPad %r\n", Status));
  if (!EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioConfig.PadMode           = %d\n", GpioConfig.PadMode));
    DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioConfig.HostOwn           = %d\n", GpioConfig.HostOwn));
    DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioConfig.Direction         = %d\n", GpioConfig.Direction)); /* Input Pin */
    DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioConfig.OutputState       = %d\n", GpioConfig.OutputState)) ;
    DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioConfig.InterruptConfig   = %d\n", GpioConfig.InterruptConfig));
    DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioConfig.ResetConfig       = %d\n", GpioConfig.ResetConfig));
    DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioConfig.TerminationConfig = %d\n", GpioConfig.TerminationConfig));
    DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioConfig.LockConfig        = %d\n", GpioConfig.LockConfig));
    DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioConfig.LockTx            = %d\n", GpioConfig.LockTx));
  }
#endif

  return;
}

#if FixedPcdGetBool (PcdMipiLontiumEnable) == 0x1
VOID
MipiI2SGpioInit (
  VOID
  )
{
  GPIOV2_CONFIG                   GpioConfig;
  EFI_STATUS                      Status;
  UINTN                           VarSize;
  PCH_SETUP                       PchSetup;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;

  DEBUG ((DEBUG_INFO, "%a starts.\n", __FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "EverestGpioInit: PeiServicesLocatePpi failed\n"));
    return;
  }
  VarSize = sizeof (PCH_SETUP);
  Status  = VariableServices->GetVariable (
                                VariableServices,
                                L"PchSetup",
                                &gPchSetupVariableGuid,
                                NULL,
                                &VarSize,
                                &PchSetup
                                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "EverestGpioInit: GetVariable (PchSetup) failed\n"));
    return;
  }

  /// <summary>
  ///  Program, GPIOV2_PTL_PCD_XXGPP_S_7 pin input mode and disable interrupt

  GpioConfig.PadMode           = GpioV2PadModeNative6;
  GpioConfig.HostOwn           = GpioV2HostOwnGpio;
  GpioConfig.Direction         = GpioV2DirDefault;
  GpioConfig.OutputState       = GpioV2StateDefault;
  GpioConfig.InterruptConfig   = GpioV2IntDis;
  GpioConfig.ResetConfig       = GpioV2ResetHost;
  GpioConfig.TerminationConfig = GpioV2TermDefault;
  GpioConfig.LockConfig        = GpioV2Unlock;
  GpioConfig.LockTx            = GpioV2Unlock;
  Status = GpioV2SetLock (GPIOV2_PTL_PCD_XXGPP_S_7, GpioV2Unlock);
  DEBUG ((DEBUG_INFO, "Lontium I2S GpioInit SetLock GpioPad %r\n", Status));
  Status = GpioV2ConfigurePad (GPIOV2_PTL_PCD_XXGPP_S_7, &GpioConfig);
  DEBUG ((DEBUG_INFO, "Lontium I2S ConfigurePad GpioPad %r\n", Status));
  DumpGpioConfig (GPIOV2_PTL_PCD_XXGPP_S_7);

  DEBUG ((DEBUG_INFO, "%a ends.\n", __FUNCTION__));
}
#endif

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
VOID
EverestGpioInit (
  VOID
  )
{
  GPIOV2_CONFIG                   GpioConfig;
  EFI_STATUS                      Status;
  UINTN                           VarSize;
  PCH_SETUP                       PchSetup;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;

  DEBUG ((DEBUG_INFO, "%a starts.\n", __FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "EverestGpioInit: PeiServicesLocatePpi failed\n"));
    return;
  }
  VarSize = sizeof (PCH_SETUP);
  Status  = VariableServices->GetVariable (
                                VariableServices,
                                L"PchSetup",
                                &gPchSetupVariableGuid,
                                NULL,
                                &VarSize,
                                &PchSetup
                                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "EverestGpioInit: GetVariable (PchSetup) failed\n"));
    return;
  }

  if ((HDAC_I2S_EVEREST8316 != PchSetup.PchHdAudioI2sCodecSelect) &&
      (HDAC_I2S_EVEREST8326 != PchSetup.PchHdAudioI2sCodecSelect) &&
      (HDAC_I2S_EVEREST8336 != PchSetup.PchHdAudioI2sCodecSelect)) {
    DEBUG ((DEBUG_ERROR, "EverestGpioInit: Everest codec not selected\n"));
    return;
  }

  /// <summary>
  ///  Program, GPIOV2_PTL_PCD_XXGPP_F_17 pin input mode and disable interrupt

  GpioConfig.PadMode           = GpioV2PadModeGpio;
  GpioConfig.HostOwn           = GpioV2HostOwnGpio;
  GpioConfig.Direction         = GpioV2DirIn; /* Input Pin */
  GpioConfig.OutputState       = GpioV2StateDefault;
  GpioConfig.InterruptConfig   = GpioV2IntDefault;
  GpioConfig.ResetConfig       = GpioV2ResetHost;
  GpioConfig.TerminationConfig = GpioV2TermDefault;
  GpioConfig.LockConfig        = GpioV2Unlock;
  GpioConfig.LockTx            = GpioV2Unlock;
  Status = GpioV2SetLock (GPIOV2_PTL_PCD_XXGPP_F_17, GpioV2Unlock);
  DEBUG ((DEBUG_INFO, "EverestGpioInit SetLock GpioPad %r\n", Status));
  Status = GpioV2ConfigurePad (GPIOV2_PTL_PCD_XXGPP_F_17, &GpioConfig);
  DEBUG ((DEBUG_INFO, "EverestGpioInit ConfigurePad GpioPad %r\n", Status));
  DumpGpioConfig (GPIOV2_PTL_PCD_XXGPP_F_17);

  GpioConfig.PadMode           = GpioV2PadModeGpio;
  GpioConfig.HostOwn           = GpioV2HostOwnGpio;
  GpioConfig.Direction         = GpioV2DirOut;
  GpioConfig.OutputState       = GpioV2StateDefault;
  GpioConfig.InterruptConfig   = GpioV2IntDis;
  GpioConfig.ResetConfig       = GpioV2ResetHost;
  GpioConfig.TerminationConfig = GpioV2TermDefault;
  GpioConfig.LockConfig        = GpioV2Unlock;
  GpioConfig.LockTx            = GpioV2Unlock;
  Status = GpioV2SetLock (GPIOV2_PTL_PCD_XXGPP_D_17, GpioV2Unlock);
  DEBUG ((DEBUG_INFO, "EverestGpioInit SetLock GpioPad %r\n", Status));
  Status = GpioV2ConfigurePad (GPIOV2_PTL_PCD_XXGPP_D_17, &GpioConfig);
  DEBUG ((DEBUG_INFO, "EverestGpioInit ConfigurePad GpioPad %r\n", Status));
  DumpGpioConfig (GPIOV2_PTL_PCD_XXGPP_D_17);

  DEBUG ((DEBUG_INFO, "%a ends.\n", __FUNCTION__));
}
VOID
RealtekGpioInit (
  VOID
  )
{
  GPIOV2_CONFIG                   GpioConfig;
  EFI_STATUS                      Status;
  UINTN                           VarSize;
  PCH_SETUP                       PchSetup;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;

  DEBUG ((DEBUG_INFO, "%a starts.\n", __FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RealtekGpioInit: PeiServicesLocatePpi failed\n"));
    return;
  }
  VarSize = sizeof (PCH_SETUP);
  Status  = VariableServices->GetVariable (
                                VariableServices,
                                L"PchSetup",
                                &gPchSetupVariableGuid,
                                NULL,
                                &VarSize,
                                &PchSetup
                                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RealtekGpioInit: GetVariable (PchSetup) failed\n"));
    return;
  }

  if ((HDAC_I2S_ALC5682I_VD != PchSetup.PchHdAudioI2sCodecSelect) &&
      (HDAC_I2S_ALC5682I_VS != PchSetup.PchHdAudioI2sCodecSelect)) {
    DEBUG ((DEBUG_ERROR, "RealtekGpioInit: Realtek codec not selected\n"));
    return;
  }

  /// <summary>
  ///  Program, GPIOV2_PTL_PCD_XXGPP_F_17 pin input mode and disable interrupt

  GpioConfig.PadMode           = GpioV2PadModeGpio;
  GpioConfig.HostOwn           = GpioV2HostOwnGpio;
  GpioConfig.Direction         = GpioV2DirIn; /* Input Pin */
  GpioConfig.OutputState       = GpioV2StateDefault;
  GpioConfig.InterruptConfig   = GpioV2IntDefault;
  GpioConfig.ResetConfig       = GpioV2ResetHost;
  GpioConfig.TerminationConfig = GpioV2TermDefault;
  GpioConfig.LockConfig        = GpioV2Unlock;
  GpioConfig.LockTx            = GpioV2Unlock;
  Status = GpioV2SetLock (GPIOV2_PTL_PCD_XXGPP_F_17, GpioV2Unlock);
  DEBUG ((DEBUG_INFO, "RealtekGpioInit SetLock GpioPad %r\n", Status));
  Status = GpioV2ConfigurePad (GPIOV2_PTL_PCD_XXGPP_F_17, &GpioConfig);
  DEBUG ((DEBUG_INFO, "RealtekGpioInit ConfigurePad GpioPad %r\n", Status));
  DumpGpioConfig (GPIOV2_PTL_PCD_XXGPP_F_17);

  GpioConfig.PadMode           = GpioV2PadModeGpio;
  GpioConfig.HostOwn           = GpioV2HostOwnGpio;
  GpioConfig.Direction         = GpioV2DirOut;
  GpioConfig.OutputState       = GpioV2StateDefault;
  GpioConfig.InterruptConfig   = GpioV2IntDis;
  GpioConfig.ResetConfig       = GpioV2ResetHost;
  GpioConfig.TerminationConfig = GpioV2TermDefault;
  GpioConfig.LockConfig        = GpioV2Unlock;
  GpioConfig.LockTx            = GpioV2Unlock;
  Status = GpioV2SetLock (GPIOV2_PTL_PCD_XXGPP_D_17, GpioV2Unlock);
  DEBUG ((DEBUG_INFO, "RealtekGpioInit SetLock GpioPad %r\n", Status));
  Status = GpioV2ConfigurePad (GPIOV2_PTL_PCD_XXGPP_D_17, &GpioConfig);
  DEBUG ((DEBUG_INFO, "PTL ConfigurePad GpioPad %r\n", Status));
  DumpGpioConfig (GPIOV2_PTL_PCD_XXGPP_D_17);

  DEBUG ((DEBUG_INFO, "%a ends.\n", __FUNCTION__));
}
#endif  // #if FixedPcdGet8(PcdEmbeddedEnable) == 0x1

VOID
MipiCamGpioInit (
  VOID
  )
{
  SA_SETUP                        SaSetup;
  PCH_SETUP                       PchSetup;
  MIPICAM_CONFIG                  MipiCamConfig;
  UINTN                           VarSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  EFI_STATUS                      Status;
  GPIOV2_CONFIG                   GpioConfig;
  GPIOV2_PAD                      GpioPad;
  UINT8                           GpioPin;

  //
  // Locate Setup variables
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MipiCamGpioInit: PeiServicesLocatePpi failed\n"));
    return;
  }

  VarSize = sizeof (SA_SETUP);
  Status  = VariableServices->GetVariable (
                                VariableServices,
                                L"SaSetup",
                                &gSaSetupVariableGuid,
                                NULL,
                                &VarSize,
                                &SaSetup
                                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MipiCamGpioInit: GetVariable (SaSetup) failed\n"));
    return;
  }

  if (SaSetup.SaIpuEnable == 0) {
    DEBUG ((DEBUG_INFO, "Avoid the MipiCamGpioInit when IPU device is disabled\n"));
    return;
  }

  VarSize = sizeof (PCH_SETUP);
  Status  = VariableServices->GetVariable (
                                VariableServices,
                                L"PchSetup",
                                &gPchSetupVariableGuid,
                                NULL,
                                &VarSize,
                                &PchSetup
                                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MipiCamGpioInit: GetVariable (PchSetup) failed\n"));
    return;
  }

  VarSize = sizeof (MIPICAM_CONFIG);
  Status  = VariableServices->GetVariable (
                                VariableServices,
                                MIPICAM_CONFIG_VARIABLE_NAME,
                                &gMipiCamConfigVariableGuid,
                                NULL,
                                &VarSize,
                                &MipiCamConfig
                                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MipiCamGpioInit: GetVariable (MipiCamConfig) failed\n"));
    return;
  }

  if ((MipiCamConfig.MipiCam_ControlLogic0) && (MipiCamConfig.MipiCam_ControlLogic0_GpioPinsEnabled)) {
    DEBUG ((DEBUG_INFO, "MipiCamGpioInit ControlLogic0 = %X\n", MipiCamConfig.MipiCam_ControlLogic0_Type));
    for (GpioPin = 0; GpioPin < MipiCamConfig.MipiCam_ControlLogic0_GpioPinsEnabled; GpioPin++) {
      GpioPad = GPIOV2_PAD_ID (
                  0,
                  GPIOV2_PTL_PCD_CHIPSET_ID, // todo: deal with the case GPIO came from other SOC
                  0,
                  MipiCamConfig.MipiCam_ControlLogic0_GpioComNumber[GpioPin],
                  MipiCamConfig.MipiCam_ControlLogic0_GpioGroupNumber[GpioPin],
                  MipiCamConfig.MipiCam_ControlLogic0_GpioGroupPadNumber[GpioPin]
                  );
      DEBUG ((DEBUG_INFO, "MipiCamGpioInit GpioPad 0x%x\n", GpioPad));

      MipiCamConfigureGpio (
        &GpioConfig,
        ((UINT32) MipiCamConfig.MipiCam_ControlLogic0_GpioFunction[GpioPin]),
        ((UINT32) MipiCamConfig.MipiCam_ControlLogic0_GpioInitialValue[GpioPin])
        );
      //
      // If PMIC based control logic, configure gpio with TX state high to avoid PMIC goes into reset.
      // Keep the default OutputState to GpioV2StateHigh for MipiCam_ControlLogic0_Type 2 and 3
      //
      if ((MipiCamConfig.MipiCam_ControlLogic0_Type == 2)) {
        GpioConfig.OutputState = GpioV2StateHigh;
      }
#if FixedPcdGetBool (PcdMipiLontiumEnable) == 0x1
      Status = GpioV2SetLock (GpioPad, GpioV2Unlock);
      DEBUG ((DEBUG_INFO, "MipiCamGpioInit SetLock GpioPad %r\n", Status));
#endif
      Status = GpioV2ConfigurePad (GpioPad, &GpioConfig);
      DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioSetPadConfig GpioPad %r\n", Status));
      DumpGpioConfig (GpioPad);
    }
  }
  if ((MipiCamConfig.MipiCam_ControlLogic1) && (MipiCamConfig.MipiCam_ControlLogic1_GpioPinsEnabled)) {
    DEBUG ((DEBUG_INFO, "MipiCamGpioInit ControlLogic1 = %X\n", MipiCamConfig.MipiCam_ControlLogic1_Type));

    for (GpioPin = 0; GpioPin < MipiCamConfig.MipiCam_ControlLogic1_GpioPinsEnabled; GpioPin++) {
      GpioPad = GPIOV2_PAD_ID (
                  0,
                  GPIOV2_PTL_PCD_CHIPSET_ID,
                  0,
                  MipiCamConfig.MipiCam_ControlLogic1_GpioComNumber[GpioPin],
                  MipiCamConfig.MipiCam_ControlLogic1_GpioGroupNumber[GpioPin],
                  MipiCamConfig.MipiCam_ControlLogic1_GpioGroupPadNumber[GpioPin]
                  );
      DEBUG ((DEBUG_INFO, "MipiCamGpioInit GpioPad 0x%x\n", GpioPad));

      MipiCamConfigureGpio (
        &GpioConfig,
        ((UINT32) MipiCamConfig.MipiCam_ControlLogic1_GpioFunction[GpioPin]),
        ((UINT32) MipiCamConfig.MipiCam_ControlLogic1_GpioInitialValue[GpioPin])
        );
      //
      // If PMIC based control logic, configure gpio with TX state high to avoid PMIC goes into reset.
      // Keep the default OutputState to GpioV2StateHigh for MipiCam_ControlLogic1_Type 2 and 3
      //
      if ((MipiCamConfig.MipiCam_ControlLogic1_Type == 2)) {
        GpioConfig.OutputState = GpioV2StateHigh;
      }
#if FixedPcdGetBool (PcdMipiLontiumEnable) == 0x1
      Status = GpioV2SetLock (GpioPad, GpioV2Unlock);
      DEBUG ((DEBUG_INFO, "MipiCamGpioInit SetLock GpioPad %r\n", Status));
#endif
      Status = GpioV2ConfigurePad (GpioPad, &GpioConfig);
      DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioSetPadConfig GpioPad %r\n", Status));
      DumpGpioConfig (GpioPad);
    }
  }
  if ((MipiCamConfig.MipiCam_ControlLogic2) && (MipiCamConfig.MipiCam_ControlLogic2_GpioPinsEnabled)) {
    DEBUG ((DEBUG_INFO, "MipiCamGpioInit ControlLogic2 Type = %X\n", MipiCamConfig.MipiCam_ControlLogic2_Type));

    for (GpioPin = 0; GpioPin < MipiCamConfig.MipiCam_ControlLogic2_GpioPinsEnabled; GpioPin++) {
      GpioPad = GPIOV2_PAD_ID (
                  0,
                  GPIOV2_PTL_PCD_CHIPSET_ID, // todo: deal with the case GPIO came from MTPS
                  0,
                  MipiCamConfig.MipiCam_ControlLogic2_GpioComNumber[GpioPin],
                  MipiCamConfig.MipiCam_ControlLogic2_GpioGroupNumber[GpioPin],
                  MipiCamConfig.MipiCam_ControlLogic2_GpioGroupPadNumber[GpioPin]
                  );
      DEBUG ((DEBUG_INFO, "MipiCamGpioInit GpioPad 0x%x\n", GpioPad));

      MipiCamConfigureGpio (
        &GpioConfig,
        ((UINT32) MipiCamConfig.MipiCam_ControlLogic2_GpioFunction[GpioPin]),
        ((UINT32) MipiCamConfig.MipiCam_ControlLogic2_GpioInitialValue[GpioPin])
        );
      //
      // If PMIC based control logic, configure gpio with TX state high to avoid PMIC goes into reset.
      // Keep the default OutputState to GpioV2StateHigh for MipiCam_ControlLogic2_Type 2 and 3
      //
      if ((MipiCamConfig.MipiCam_ControlLogic2_Type == 2)) {
        GpioConfig.OutputState = GpioV2StateHigh;
      }
#if FixedPcdGetBool (PcdMipiLontiumEnable) == 0x1
      Status = GpioV2SetLock (GpioPad, GpioV2Unlock);
      DEBUG ((DEBUG_INFO, "MipiCamGpioInit SetLock GpioPad %r\n", Status));
#endif
      Status = GpioV2ConfigurePad (GpioPad, &GpioConfig);
      DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioSetPadConfig GpioPad %r\n", Status));
      DumpGpioConfig (GpioPad);
    }
  }
  if ((MipiCamConfig.MipiCam_ControlLogic3) && (MipiCamConfig.MipiCam_ControlLogic3_GpioPinsEnabled)) {
    DEBUG ((DEBUG_INFO, "MipiCamGpioInit ControlLogic3 Type = %X\n", MipiCamConfig.MipiCam_ControlLogic3_Type));

    for (GpioPin = 0; GpioPin < MipiCamConfig.MipiCam_ControlLogic3_GpioPinsEnabled; GpioPin++) {
      GpioPad = GPIOV2_PAD_ID (
                  0,
                  GPIOV2_PTL_PCD_CHIPSET_ID, // todo: deal with the case GPIO came from MTPS
                  0,
                  MipiCamConfig.MipiCam_ControlLogic3_GpioComNumber[GpioPin],
                  MipiCamConfig.MipiCam_ControlLogic3_GpioGroupNumber[GpioPin],
                  MipiCamConfig.MipiCam_ControlLogic3_GpioGroupPadNumber[GpioPin]
                  );
      DEBUG ((DEBUG_INFO, "MipiCamGpioInit GpioPad 0x%x\n", GpioPad));

      MipiCamConfigureGpio (
        &GpioConfig,
        ((UINT32) MipiCamConfig.MipiCam_ControlLogic3_GpioFunction[GpioPin]),
        ((UINT32) MipiCamConfig.MipiCam_ControlLogic3_GpioInitialValue[GpioPin])
        );
      //
      // If PMIC based control logic, configure gpio with TX state high to avoid PMIC goes into reset.
      // Keep the default OutputState to GpioV2StateHigh for MipiCam_ControlLogic3_Type 2 and 3
      //
      if((MipiCamConfig.MipiCam_ControlLogic3_Type == 2)) {
        GpioConfig.OutputState = GpioV2StateHigh;
      }
#if FixedPcdGetBool (PcdMipiLontiumEnable) == 0x1
      Status = GpioV2SetLock (GpioPad, GpioV2Unlock);
      DEBUG ((DEBUG_INFO, "MipiCamGpioInit SetLock GpioPad %r\n", Status));
#endif
      Status = GpioV2ConfigurePad (GpioPad, &GpioConfig);
      DEBUG ((DEBUG_INFO, "MIPI_GPIO GpioSetPadConfig GpioPad %r\n", Status));
      DumpGpioConfig (GpioPad);
    }
  }

  return;
}

/**
  Create a Hob data for Mipi Camera configure setting

  @retval     EFI_OUT_OF_RESOURCES  If Hob create failed, it will show error.
  @retval     EFI_SUCCESS  Cretae Hob success.

**/
EFI_STATUS
InstallMipiConfigHob (
  VOID
  )
{
  MIPICAM_CONFIG_HOB  *MipiCamConfigHob;

  MipiCamConfigHob = BuildGuidHob (
                       &gMipiCamConfigHobGuid,
                       sizeof (MIPICAM_CONFIG_HOB)
                       );
  if (MipiCamConfigHob == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  DEBUG ((DEBUG_INFO, "<MipiCamConfigHob Create> \n" ));
  MipiCamConfigHob->PlatformId        = FixedPcdGet8 (PcdIpuDriverPlatformId); // PlatformGenerationID
  MipiCamConfigHob->EmulationConfig   = IsSimicsEnvironment();                 // Check Emulation Status
  MipiCamConfigHob->PlatformChipId    = GPIOV2_PTL_PCD_CHIPSET_ID;             // GPIOV2 Chipset ID

  return EFI_SUCCESS;
}

/**
  MipiCam Init EntryPoint

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS  This constructor always return EFI_SUCCESS.
              It will ASSERT on errors.
**/
EFI_STATUS
EFIAPI
MipiCamInitEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  DEBUG ((DEBUG_INFO, "MipiCamInitEntryPoint() Start\n"));
  InstallMipiConfigHob ();
#if FixedPcdGetBool (PcdMipiLontiumEnable) == 0x1
  MipiCamGpioInit ();
  MipiI2SGpioInit();
#endif
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  EverestGpioInit ();
  RealtekGpioInit ();
#endif

  DEBUG ((DEBUG_INFO, "MipiCamInitEntryPoint() End\n"));
  return EFI_SUCCESS;
}
