/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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

#include "PeiPchPolicyUpdate.h"
#include <Guid/FmpCapsule.h>
#include <Guid/GlobalVariable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CnviLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PcdPolicyLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/SerialPortParameterLib.h>
#include <HdAudioConfig.h>
#include <Library/PeiHdaVerbTables.h>
#include <Library/SiPolicyLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/PmcLib.h>
#include <Library/ResiliencySupportLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPciBdfLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/PcdLib.h>
#include <Library/PlatformUsbConfigLib.h>
#include <Ppi/Spi.h>

#include <Library/GbeLib.h>

#include <PlatformBoardConfig.h>
#include <PolicyUpdateMacro.h>
#include <SerialIoDevices.h>
#include <Setup.h>
#include <Register/GpioV2PcdPins.h>
#include <SerialIoConfig.h>
#include <LpssI2cConfig.h>
#include <LpssI3cConfig.h>
#include <LpssSpiConfig.h>
#include <LpssUartConfig.h>
#include <ScsConfig.h>
#include <PmConfig.h>
#include <ThcConfig.h>
#include <EspiConfig.h>
#include <IehConfig.h>
#include <CnviConfig.h>
#include <ThermalConfig.h>
#include <IshConfig.h>
#include <GbeConfig.h>
#include <LockDownConfig.h>
#include <InterruptConfig.h>
#include <FlashProtectionConfig.h>
#include <IoApicConfig.h>
#include <PchPcieRpConfig.h>
#include <PchDmiConfig.h>
#include <TcssPeiPreMemConfig.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <HsioConfig.h>
#include <RtcConfig.h>

#if FixedPcdGetBool (PcdDTbtEnable) == 1
#include <Ppi/PeiDTbtPolicy.h>
#endif

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
#include <Library/PeiTsnFvLib.h>
#endif
#include <CnvVfrSetupMenuHii.h>
#if FixedPcdGetBool(PcdCapsuleEnable) == 1
#include <Guid/SysFwUpdateProgress.h>
#include <Library/PayloadResiliencySupportLib.h>

/**
  Return if input ImageGuid belongs to a FMP device which would perform BIOS update

  @param[in] ImageGuid A pointer to GUID

  @retval TRUE  ImageGuid belongs to a FMP which would perform BIOS update
  @retval FALSE ImageGuid does not belong to a FMP which would perform BIOS update

**/
BOOLEAN
IsFmpGuidForBiosUpdate (
  IN  EFI_GUID                  *ImageGuid,
  OUT SYSTEM_FIRMWARE_COMPONENT *UpdatingComponent
  )
{
  if (CompareGuid (ImageGuid, &gFmpDeviceMonolithicDefaultGuid)) {
    *UpdatingComponent = UpdatingTypeMax;
    return TRUE;
  } else if (CompareGuid (ImageGuid, &gFmpDeviceBiosDefaultGuid)) {
    *UpdatingComponent = UpdatingBios;
    return TRUE;
#if FixedPcdGetBool(PcdExtendedBiosRegionSupport) == 1
  } else if (CompareGuid (ImageGuid, &gFmpDeviceExtendedBiosDefaultGuid)) {
    *UpdatingComponent = UpdatingBios;
    return TRUE;
#endif
#if FixedPcdGetBool (PcdIfwiCapsuleUpdateEnable) == 1
  } else if (CompareGuid (ImageGuid, &gFmpDeviceIfwiDefaultGuid)) {
    return TRUE;
#endif
  } else if (CompareGuid (ImageGuid, &gFmpDeviceBtGAcmDefaultGuid)) {
    *UpdatingComponent = UpdatingBtGAcm;
    return TRUE;
  } else if (CompareGuid (ImageGuid, &gFmpDeviceMicrocodeDefaultGuid)) {
    *UpdatingComponent = UpdatinguCode;
    return TRUE;
  } else if (CompareGuid (ImageGuid, &gFmpDeviceFspDefaultGuid)) {
    *UpdatingComponent = UpdatingFsp;
    return TRUE;
  }

  return FALSE;
}

/**
  Detect if there is a FMP capsule which would perform BIOS update

  @retval TRUE        Such capsule is detected.
  @retval FALSE       No such capsules there.

**/
BOOLEAN
IsFmpCapsuleForBiosUpdateDetected (
  OUT SYSTEM_FIRMWARE_COMPONENT                 *UpdatingComponent
  )
{
  EFI_PEI_HOB_POINTERS                          HobPointer;
  EFI_CAPSULE_HEADER                            *CapsuleHeader;
  EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER        *FmpCapsuleHeader;
  EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER  *ImageHeader;
  UINTN                                         Index;
  UINT64                                        *ItemOffsetList;

  //
  // Search all capsule images from hob
  //
  HobPointer.Raw = GetHobList ();
  while ((HobPointer.Raw = GetNextHob (EFI_HOB_TYPE_UEFI_CAPSULE, HobPointer.Raw)) != NULL) {
    CapsuleHeader = (EFI_CAPSULE_HEADER *) (UINTN) HobPointer.Capsule->BaseAddress;

    //
    // Must be a nested FMP capsule or FMP capsule with pre-defined GUIDs
    //
    if (IsFmpGuidForBiosUpdate (&CapsuleHeader->CapsuleGuid, UpdatingComponent)) {
      CapsuleHeader = (EFI_CAPSULE_HEADER *)((UINT8 *)CapsuleHeader + CapsuleHeader->HeaderSize);
    }

    if (!CompareGuid (&gEfiFmpCapsuleGuid, &CapsuleHeader->CapsuleGuid)) {
      HobPointer.Raw = GET_NEXT_HOB (HobPointer);
      continue;
    }

    FmpCapsuleHeader = (EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER *)((UINT8 *)CapsuleHeader + CapsuleHeader->HeaderSize);
    ItemOffsetList = (UINT64 *)(FmpCapsuleHeader + 1);
    for (Index = FmpCapsuleHeader->EmbeddedDriverCount; Index < (UINT32)FmpCapsuleHeader->EmbeddedDriverCount + FmpCapsuleHeader->PayloadItemCount; Index++) {
      ImageHeader = (EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER *)((UINT8 *)FmpCapsuleHeader + ItemOffsetList[Index]);
      if (IsFmpGuidForBiosUpdate (&ImageHeader->UpdateImageTypeId, UpdatingComponent)) {
        DEBUG ((DEBUG_INFO, "A FMP capsule for BIOS update is detected.\n"));
        return TRUE;
      }
    }

    HobPointer.Raw = GET_NEXT_HOB (HobPointer);
  }

  return FALSE;
}

/**
  Check if there is bios region update in progress.
  @retval TRUE        Bios region update is in progress.
  @retval FALSE       Not in bios region update progress.
**/
BOOLEAN
IsUpdatingBiosRegion (
  VOID
  )
{
  EFI_HOB_GUID_TYPE          *GuidHob;
  SYSTEM_FIRMWARE_COMPONENT  UpdatingComponent;
  UINT16                     UpdatingFlag;
  BOOLEAN                    IsUpdating;

  GuidHob           = GetFirstGuidHob (&gSysFwUpdateProgressGuid);
  UpdatingComponent = 0xFF;
  IsUpdating        = FALSE;

  if (GuidHob == NULL) {
    goto Exit;
  }

  UpdatingComponent = ((SYSTEM_FIRMWARE_UPDATE_PROGRESS *) GET_GUID_HOB_DATA (GuidHob))->Component;
  UpdatingFlag      = ((SYSTEM_FIRMWARE_UPDATE_PROGRESS *) GET_GUID_HOB_DATA (GuidHob))->Flag;

  switch (UpdatingComponent) {
    case UpdatingBios:
    case UpdatingBtGAcm:
    case UpdatinguCode:
    case UpdatingFsp:
#if (FixedPcdGetBool (PcdNvmeRecoveryEnable) == 0)
    case UpdatingResiliency:
#endif
      IsUpdating = TRUE;
      break;

    default:
      IsUpdating = FALSE;
  }
  if ((UpdatingComponent != NotStarted) && (UpdatingFlag & SYSFW_UNDER_MONOLITHIC_UPDATE) == 1) {
    IsUpdating = TRUE;
  }

Exit:
  return IsUpdating;
}

/**
  Check if there is FitPayload region update in progress.
  @retval TRUE        FitPayload region update is in progress.
  @retval FALSE       Not in FitPayload region update progress.
**/
BOOLEAN
IsUpdatingFitPayloadRegion (
  VOID
  )
{
  BOOLEAN                         IsUpdating;
  EFI_STATUS                      Status;
  UINTN                           VarSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;

  IsUpdating = FALSE;
  VarSize    = 0;

  if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY && IsPayloadBackupEnabled ()) {
    //
    // Retrieve FitPayloadStatus variable
    //
    Status = PeiServicesLocatePpi (
              &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
              0,                                // INSTANCE
              NULL,                             // EFI_PEI_PPI_DESCRIPTOR
              (VOID **) &VariableServices       // PPI
              );
    ASSERT_EFI_ERROR (Status);

    Status = VariableServices->GetVariable (
                                VariableServices,
                                L"InitSetupVariable",
                                &gSetupVariableGuid,
                                NULL,
                                &VarSize,
                                NULL
                                );
    DEBUG((DEBUG_INFO, "Get InitSetupVariable -- (%r)\n", Status));
    if (Status == EFI_NOT_FOUND) {
      DEBUG((DEBUG_INFO, "InitSetupVariable not found! -- (%r)\n", Status));
      IsUpdating = TRUE;
    }
  }

  return IsUpdating;
}

/**
  Check if BIOS Update is going to happen during this boot.

  This function checks:

    1. If BootMode is BOOT_ON_FLASH_UPDATE and there is a Capsule to update BIOS region detected.
    2. If TopSwap is get set, it is necessary for doing the BIOS resiliency restore.
    3. If BIOS update is in progress in the last boot.
  If one of above condition stands, set a GUID HOB as an indication to be consumed in DXE/BDS

  and return TRUE. Otherwise, return FALSE.

  @retval TRUE       BIOS Update is going to happen during this boot.
  @retval FALSE      No request of BIOS update is detected.

**/
BOOLEAN
IsBiosUpdateRequired (
  VOID
  )
{
  EFI_HOB_GUID_TYPE               *GuidHob;
  SYSTEM_FIRMWARE_COMPONENT       CapsuleComponent;

  CapsuleComponent = 0xFF;

  if (((GetBootModeHob () == BOOT_ON_FLASH_UPDATE) && IsFmpCapsuleForBiosUpdateDetected (&CapsuleComponent)) || \
      (SpiIsTopSwapEnabled ()) || \
      (IsUpdatingBiosRegion ()) || \
      (IsUpdatingFitPayloadRegion ())) {
    //
    // Build gSkipBiosLockForSysFwUpdateGuid HOB if it does not exist.
    //
    GuidHob = GetFirstGuidHob (&gSkipBiosLockForSysFwUpdateGuid);
    if (GuidHob == NULL) {
      DEBUG ((DEBUG_INFO, "Build gSkipBiosLockForSysFwUpdateGuid HOB to reset system before EndOfDxe\n"));
      BuildGuidHob (&gSkipBiosLockForSysFwUpdateGuid, 0);
    }

    return TRUE;
  }

  return FALSE;
}

#else // FixedPcdGetBool(PcdCapsuleEnable) != 1
/**
  Check if BIOS Update is going to happen during this boot.

  @retval TRUE       BIOS Update is going to happen during this boot.
  @retval FALSE      No request of BIOS update is detected.

**/
STATIC
BOOLEAN
IsBiosUpdateRequired (
  VOID
  )
{
  return FALSE;
}
#endif

/**
  This is helper function for getting I2C Pads Internal Termination settings from Pcd

  @param[in]  Index            I2C Controller Index
**/
UINT8
STATIC
GetSerialIoI2cPadsTerminationFromPcd (
  IN UINT8 Index
  )
{
  switch (Index) {
    case 0:
      return PcdGet8 (PcdPchSerialIoI2c0PadInternalTerm);
    case 1:
      return PcdGet8 (PcdPchSerialIoI2c1PadInternalTerm);
    case 2:
      return PcdGet8 (PcdPchSerialIoI2c2PadInternalTerm);
    case 3:
      return PcdGet8 (PcdPchSerialIoI2c3PadInternalTerm);
    case 4:
      return PcdGet8 (PcdPchSerialIoI2c4PadInternalTerm);
    case 5:
      return PcdGet8 (PcdPchSerialIoI2c5PadInternalTerm);
    case 6:
      return PcdGet8 (PcdPchSerialIoI2c6PadInternalTerm);
    case 7:
      return PcdGet8 (PcdPchSerialIoI2c7PadInternalTerm);
    default:
      ASSERT (FALSE); // Invalid I2C Controller Index
  }
  return 0;
}

/**
  This function performs PCH Serial IO Platform Policy initialization

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
  @param[in] DebugConfigData      Pointer to DEBUG_CONFIG_DATA buffer
  @param[in] SetupVariables       Pointer to SETUP_DATA buffer
**/
VOID
UpdateSerialIoConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup,
  IN DEBUG_CONFIG_DATA         *DebugConfigData,
  IN SETUP_DATA                *SetupVariables
  )
{
  UINT8              Index;
  SERIAL_IO_CONFIG   *SerialIoConfig;
  EFI_STATUS         Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // I2C
  //
  // Serial IO I2C Pin Mux tbd
  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (SerialIoConfig->I2cDeviceConfig[Index].Mode, (UINT8)(SERIAL_IO_I2C_MODE)PchSetup->PchSerialIoI2c[Index], Index);
    UPDATE_POLICY (SerialIoConfig->I2cDeviceConfig[Index].PadTermination, GetSerialIoI2cPadsTerminationFromPcd (Index));
  }
  UPDATE_POLICY (SerialIoConfig->I2cDeviceConfig[2].PinMux.Sda, 0x0);
  UPDATE_POLICY (SerialIoConfig->I2cDeviceConfig[2].PinMux.Scl, 0x0);
  UPDATE_POLICY (SerialIoConfig->I2cDeviceConfig[3].PinMux.Sda, 0x0);
  UPDATE_POLICY (SerialIoConfig->I2cDeviceConfig[3].PinMux.Scl, 0x0);
  UPDATE_POLICY (SerialIoConfig->I2cDeviceConfig[4].PinMux.Sda, 0x0);
  UPDATE_POLICY (SerialIoConfig->I2cDeviceConfig[4].PinMux.Scl, 0x0);
  UPDATE_POLICY (SerialIoConfig->I2cDeviceConfig[5].PinMux.Sda, 0x0);
  UPDATE_POLICY (SerialIoConfig->I2cDeviceConfig[5].PinMux.Scl, 0x0);
}

/**
  This function performs PCH I3c Platform Policy initialization

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
VOID
UpdateLpssI3cConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  UINT8              Index;
  LPSS_I3C_CONFIG    *I3cConfig;
  EFI_STATUS         Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssI3cConfigGuid, (VOID *) &I3cConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // I3C
  //
  // Serial IO I3C Pin Mux tbd
  for (Index = 0; Index < GetMaxI3cInterfacesNum (); Index++) {
    UPDATE_POLICY (I3cConfig->I3cDeviceConfig[Index].Mode, (UINT8)(SERIAL_IO_I3C_MODE)PchSetup->PchSerialIoI3c[Index]);
    UPDATE_POLICY (I3cConfig->I3cDeviceConfig[Index].Sda.PinMux, 0x0);
    UPDATE_POLICY (I3cConfig->I3cDeviceConfig[Index].Scl.PinMux, 0x0);
    UPDATE_POLICY (I3cConfig->I3cDeviceConfig[Index].SclFb.PinMux, 0x0);
  }
}

/**
  This function performs PCH Lpss Spi Platform Policy initialization

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
VOID
UpdateLpssSpiConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  UINT8              Index;
  UINT8              CsIndex;
  LPSS_SPI_CONFIG    *LpssSpiConfig;
  EFI_STATUS         Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssSpiConfigGuid, (VOID *) &LpssSpiConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // SPI
  //
  for (Index = 0; Index < GetMaxSpiInterfacesNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (LpssSpiConfig->SpiDeviceConfig[Index].Mode, (UINT8)(SERIAL_IO_SPI_MODE) PchSetup->PchSerialIoSpi[Index], Index);
  }

  // Lpss SPI Pin Mux tbd
  UPDATE_POLICY (LpssSpiConfig->SpiDeviceConfig[0].PinMux.Cs[0], 0x0);
  UPDATE_POLICY (LpssSpiConfig->SpiDeviceConfig[0].PinMux.Clk, 0x0);
  UPDATE_POLICY (LpssSpiConfig->SpiDeviceConfig[0].PinMux.Miso, 0x0);
  UPDATE_POLICY (LpssSpiConfig->SpiDeviceConfig[0].PinMux.Mosi, 0x0);

  for (Index = 0; Index < GetMaxSpiInterfacesNum (); Index++) {
    for (CsIndex = 0; CsIndex < PCH_MAX_SERIALIO_SPI_CHIP_SELECTS; CsIndex++) {
#if FixedPcdGet8(PcdFspModeSelection) == 0
      // Two Array Index Special Case, Check Policy Default Manually
      if ((LpssSpiConfig->SpiDeviceConfig[Index].CsPolarity[CsIndex] != PchSetup->PchSpiCsPolarity[PCH_MAX_SERIALIO_SPI_CHIP_SELECTS * Index + CsIndex]) && (IsPolicyDefaultCheckRequired())) {
        DEBUG ((DEBUG_INFO, "Policy Default Mismatch With Setup Default\n"));
        DEBUG ((DEBUG_INFO, "ConfigBlock : LpssSpiConfig->SpiDeviceConfig[0x%x].CsPolarity[0x%x] = 0x%x\n", Index, CsIndex, LpssSpiConfig->SpiDeviceConfig[Index].CsPolarity[CsIndex]));
        DEBUG ((DEBUG_INFO, "Setup : PchSetup->PchSpiCsPolarity[0x%x] = 0x%x\n", (PCH_MAX_SERIALIO_SPI_CHIP_SELECTS * Index + CsIndex), PchSetup->PchSpiCsPolarity[PCH_MAX_SERIALIO_SPI_CHIP_SELECTS * Index + CsIndex]));
      }
#endif
      UPDATE_POLICY (LpssSpiConfig->SpiDeviceConfig[Index].CsPolarity[CsIndex], PchSetup->PchSpiCsPolarity[PCH_MAX_SERIALIO_SPI_CHIP_SELECTS * Index + CsIndex]);
    }
  }
}

/**
  This function performs PCH I2c Platform Policy initialization

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
VOID
UpdateLpssI2cConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  UINT8              Index;
  LPSS_I2C_CONFIG    *I2cConfig;
  EFI_STATUS         Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssI2cConfigGuid, (VOID *) &I2cConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  // I2C
  //
  // Serial IO I2C Pin Mux tbd
  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    UPDATE_POLICY (I2cConfig->I2cDeviceConfig[Index].Mode, (UINT8)(SERIAL_IO_I2C_MODE)PchSetup->PchSerialIoI2c[Index]);
    UPDATE_POLICY (I2cConfig->I2cDeviceConfig[Index].PinMux.Sda, 0x0);
    UPDATE_POLICY (I2cConfig->I2cDeviceConfig[Index].PinMux.Scl, 0x0);
    UPDATE_POLICY (I2cConfig->I2cDeviceConfig[Index].PadTermination, GetSerialIoI2cPadsTerminationFromPcd (Index));
  }
}

/**
  This function performs PCH Lpss Uart Platform Policy initialization

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
  @param[in] DebugConfigData      Pointer to DEBUG_CONFIG_DATA buffer
  @param[in] SetupVariables       Pointer to SETUP_DATA buffer
**/
VOID
UpdateLpssUartConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup,
  IN DEBUG_CONFIG_DATA         *DebugConfigData,
  IN SETUP_DATA                *SetupVariables
  )
{
  UINT8              Index;
  LPSS_UART_CONFIG    *LpssUartConfig;
  EFI_STATUS         Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssUartConfigGuid, (VOID *) &LpssUartConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // UART
  //
  // Serial IO UART Pin Mux tbd
  for (Index = 0; Index < GetMaxUartInterfacesNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (LpssUartConfig->UartDeviceConfig[Index].Mode,                (UINT8)(LPSS_UART_MODE) PchSetup->PchSerialIoUart[Index], Index);
    COMPARE_UPDATE_POLICY_ARRAY (LpssUartConfig->UartDeviceConfig[Index].Attributes.AutoFlow, PchSetup->PchUartHwFlowCtrl[Index],                           Index);
    COMPARE_UPDATE_POLICY_ARRAY (LpssUartConfig->UartDeviceConfig[Index].PowerGating,         PchSetup->PchUartPowerGating[Index],                          Index);
    COMPARE_UPDATE_POLICY_ARRAY (LpssUartConfig->UartDeviceConfig[Index].DmaEnable,           PchSetup->PchUartDmaEnable[Index],                            Index);
  }

  UPDATE_POLICY (LpssUartConfig->UartDeviceConfig[1].PinMux.Rx, 0x0);
  UPDATE_POLICY (LpssUartConfig->UartDeviceConfig[1].PinMux.Tx, 0x0);

  //
  // UART1 has no RTS and CTS pins available.
  //
//#if FixedPcdGet8(PcdFspModeSelection) == 0  //Dispatch mode
  if (LpssUartConfig->UartDeviceConfig[1].Attributes.AutoFlow != 0) {
    DEBUG ((DEBUG_ERROR, "UART1 HW Flow Control enable failed, it has no RTS/CTS pins available\n"));
    ASSERT (FALSE);
  }
// #else
//   if (((FSPS_UPD *)FspsUpd)->FspsConfig.SerialIoUartAutoFlow[1] != 0) {
//     DEBUG((DEBUG_ERROR, "UART1 HW Flow Control enable failed, it has no RTS/CTS pins available\n"));
//     ASSERT(FALSE);
//   }
//#endif
  //
  // UART2 has to be disabled when CNVi is present - {F0-F3} pins are occupied by CNVi RF signals.
  //
  if (CnviCrfModuleIsPresent ()) {
    UPDATE_POLICY (LpssUartConfig->UartDeviceConfig[2].Mode, LpssUartDisabled);
  }

  if ((SetupVariables->OsDebugPort != 0) && (SetupVariables->OsDebugPort <= GetMaxUartInterfacesNum ())) {
    //
    // This UART controller will be used as OS debug port
    //
    UPDATE_POLICY (LpssUartConfig->UartDeviceConfig[SetupVariables->OsDebugPort - 1].Mode, LpssUartHidden);
    UPDATE_POLICY (LpssUartConfig->UartDeviceConfig[SetupVariables->OsDebugPort - 1].DBG2,               TRUE);
  }

DEBUG_CODE_BEGIN();
  if (GetDebugInterface () & STATUS_CODE_USE_SERIALIO) {
    UPDATE_POLICY (
      LpssUartConfig->UartDeviceConfig[DebugConfigData->SerialIoUartDebugControllerNumber].Attributes.BaudRate,
      DebugConfigData->SerialIoUartDebugBaudRate
    );
    UPDATE_POLICY (
      LpssUartConfig->UartDeviceConfig[DebugConfigData->SerialIoUartDebugControllerNumber].Attributes.DataBits,
      DebugConfigData->SerialIoUartDebugDataBits
    );
    UPDATE_POLICY (
      LpssUartConfig->UartDeviceConfig[DebugConfigData->SerialIoUartDebugControllerNumber].Attributes.StopBits,
      DebugConfigData->SerialIoUartDebugStopBits
    );
    UPDATE_POLICY (
      LpssUartConfig->UartDeviceConfig[DebugConfigData->SerialIoUartDebugControllerNumber].Attributes.Parity,
      DebugConfigData->SerialIoUartDebugParity
    );
    UPDATE_POLICY (
      LpssUartConfig->UartDeviceConfig[DebugConfigData->SerialIoUartDebugControllerNumber].Attributes.AutoFlow,
      DebugConfigData->SerialIoUartDebugFlowControl
    );
  }
DEBUG_CODE_END();

}

/**
  This function updates USB2 Enable/Disable policy based on Type-C connector option.

  @param[in]  SiPreMemPolicyPpi Pointer to SI_PREMEM_POLICY_PPI
  @param[in]  UsbConfig         Pointer to USB_CONFIG data buffer
**/
VOID
UpdateUsb2PortEnablePolicy (
  IN SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi,
  IN USB_CONFIG                *UsbConfig
  )
{
  UINT8                      ConnectorIndex;
  TCSS_PEI_PREMEM_CONFIG     *TcssPeiPreMemConfig;
  USB_CONNECTOR_HOB_DATA     *UsbConnectorHobDataPtr;
  USBC_CONNECTOR_HOB_DATA    *UsbCConnectorHobDataPtr;
  USB_CONNECTOR_BOARD_CONFIG *UsbConnectorBoardConfig;

  TcssPeiPreMemConfig     = NULL;
  UsbConnectorHobDataPtr  = NULL;
  UsbCConnectorHobDataPtr = NULL;
  UsbConnectorBoardConfig = NULL;

  GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
  if (TcssPeiPreMemConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: TcssPeiPreMemConfig is not available!!\n", __FUNCTION__));
    return;
  }

  UsbConnectorHobDataPtr  = GetUsbConnectorHobData ();
  UsbCConnectorHobDataPtr = GetUsbCConnectorHobData ();
  if (UsbConnectorHobDataPtr == NULL || UsbCConnectorHobDataPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorHobDataPtr or UsbCConnectorHobDataPtr is not available!!\n", __FUNCTION__));
    return;
  }

  UsbConnectorBoardConfig = UsbConnectorHobDataPtr->UsbConnectorBoardConfig;
  if (UsbConnectorBoardConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorBoardConfig is not available!!\n", __FUNCTION__));
    return;
  }

  if (PcdGet8 (VpdPcdUsbCUcmMode)) {
    for (ConnectorIndex = 0; ConnectorIndex < UsbCConnectorHobDataPtr->NumberOfUsbCConnectors; ConnectorIndex++) {
      //
      // Check TCSS USBC port map to USB2
      //
      if (UsbConnectorBoardConfig[ConnectorIndex].Usb3Controller == TCSS_USB3 &&
          UsbConnectorBoardConfig[ConnectorIndex].Usb2Controller == PCH_USB2) {
        //
        // Disable USB2 port if the corresponding Type-C connector option is UsbCDisable or DpOnly
        // Enable USB2 port if the corresponding Type-C connector option is NoThunderbolt, NoPcie or FullFunction
        //
        UPDATE_POLICY (
          UsbConfig->PortUsb20[UsbConnectorBoardConfig[ConnectorIndex].Usb2PortNum].Enable,
          IS_TC_PORT_USB_SUPPORTED (TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[ConnectorIndex])
          );
        DEBUG ((
          DEBUG_INFO,
          "USB2 port for Type-C connector %d is %s\n",
          ConnectorIndex,
          (UsbConfig->PortUsb20[UsbConnectorBoardConfig[ConnectorIndex].Usb2PortNum].Enable == 1) ? "Enable" : "Disable"
          ));
      }
    }
  }
}

/**
  This function updates USB OverCurrent policy mapping based on board design

  @param[in]  UsbConfig        Pointer to USB_CONFIG data buffer
**/
VOID
UpdateUsbOverCurrentPolicy (
  IN USB_CONFIG                 *UsbConfig
  )
{
  USB_CONNECTOR_BOARD_CONFIG *UsbConnectorBoardConfig;
  UINT8                      PortIndex;
  UINT8                      Usb2PortCount;
  UINT8                      Usb3PortCount;
  UINT8                      ConnectorIndex;
  USB_CONNECTOR_HOB_DATA     *UsbConnectorHobDataPtr;

  UsbConnectorBoardConfig = NULL;

  Usb2PortCount = GetPchXhciMaxUsb2PortNum ();
  Usb3PortCount = GetPchXhciMaxUsb3PortNum ();

  UsbConnectorHobDataPtr = GetUsbConnectorHobData ();
  if (UsbConnectorHobDataPtr != NULL) {
    UsbConnectorBoardConfig = UsbConnectorHobDataPtr->UsbConnectorBoardConfig;
  }

  if (UsbConnectorBoardConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "UpdateUsbOverCurrentPolicy: UsbConnectorBoardConfig is not available!!\n"));
    return;
  }

  // Initialize USB2 and USB3 USB OC Config
  for (PortIndex = 0; PortIndex < MAX_USB2_PORTS; PortIndex++) {
    UPDATE_POLICY (
      UsbConfig->PortUsb20[PortIndex].OverCurrentPin,
      USB_OC_SKIP
      );
  }
  for (PortIndex = 0; PortIndex < MAX_USB3_PORTS; PortIndex++) {
    UPDATE_POLICY (
      UsbConfig->PortUsb30[PortIndex].OverCurrentPin,
      USB_OC_SKIP
      );
  }

  // Update USB2 and USB3 USB OC Config
  DEBUG ((DEBUG_INFO, "UpdateUsbOverCurrentPolicy: Updating USB OC mapping for %d USB2 ports and %d USB3 ports.\n", Usb2PortCount, Usb3PortCount));
  for (ConnectorIndex = 0; ConnectorIndex < UsbConnectorHobDataPtr->NumberOfUsbConnectors; ConnectorIndex++, UsbConnectorBoardConfig++) {
    if (UsbConnectorBoardConfig->ConnectorConnectable == CONNECTABLE) {
      if (UsbConnectorBoardConfig->Usb2Controller == PCH_USB2) {
        if (UsbConnectorBoardConfig->Usb2PortNum < Usb2PortCount) {
          if (UsbConnectorBoardConfig->UsbOcPinType != 0) {
            if (UsbConnectorBoardConfig->UsbOcPinType <= USB_OC_MAX_TYPE &&
                UsbConnectorBoardConfig->UsbOcPin < USB_OC_MAX_PINS) {
              UPDATE_POLICY (
                UsbConfig->PortUsb20[UsbConnectorBoardConfig->Usb2PortNum].OverCurrentPin,
                (UINT8) GET_USB2_OCM_REG (UsbConnectorBoardConfig->UsbOcPinType, UsbConnectorBoardConfig->UsbOcPin)
                );
            } else {
              DEBUG ((DEBUG_ERROR, "UpdateUsbOverCurrentPolicy: Invalid OverCurrent pin specified USB2 port %d.\n", UsbConnectorBoardConfig->Usb2PortNum));
            }
          }
        } else {
          DEBUG ((
            DEBUG_ERROR,
            "UpdateUsbOverCurrentPolicy: USB2 port#%d is over the max port number supported by platform.\n",
            UsbConnectorBoardConfig->Usb2PortNum
            ));
        }
      }
      if (UsbConnectorBoardConfig->Usb3Controller == PCH_USB3) {
        if (UsbConnectorBoardConfig->Usb3PortNum < Usb3PortCount) {
          if (UsbConnectorBoardConfig->UsbOcPinType != 0) {
            if (UsbConnectorBoardConfig->UsbOcPinType <= USB_OC_MAX_TYPE &&
                UsbConnectorBoardConfig->UsbOcPin < USB_OC_MAX_PINS) {
              UPDATE_POLICY (
                UsbConfig->PortUsb30[UsbConnectorBoardConfig->Usb3PortNum].OverCurrentPin,
                (UINT8) GET_USB3_OCM_REG (UsbConnectorBoardConfig->UsbOcPinType, UsbConnectorBoardConfig->UsbOcPin)
                );
            } else {
              DEBUG ((DEBUG_ERROR, "UpdateUsbOverCurrentPolicy: Invalid OverCurrent pin specified USB3 port %d.\n", UsbConnectorBoardConfig->Usb3PortNum));
            }
          }
        } else {
          DEBUG ((
            DEBUG_ERROR,
            "UpdateUsbOverCurrentPolicy: USB3 port#%d is over the max port number supported by platform.\n",
            UsbConnectorBoardConfig->Usb3PortNum
            ));
        }
      }
    }
  }
}

/**
  This function updates USB2 PortResetMessageEnable policy for PCH USB2 Ports
  those are paired with TCSS XHCI Ports based on board design.

  @param[in]  UsbConfig        Pointer to USB_CONFIG data buffer
**/
VOID
UpdateUsb2PortResetMessageEnablePolicy (
  IN USB_CONFIG                 *UsbConfig
  )
{
  UINT8                      ConnectorIndex;
  USB_CONNECTOR_HOB_DATA     *UsbConnectorHobDataPtr;
  USBC_CONNECTOR_HOB_DATA    *UsbCConnectorHobDataPtr;
  USB_CONNECTOR_BOARD_CONFIG *UsbConnectorBoardConfig;

  UsbConnectorHobDataPtr  = GetUsbConnectorHobData ();
  UsbCConnectorHobDataPtr = GetUsbCConnectorHobData ();
  if (UsbConnectorHobDataPtr == NULL || UsbCConnectorHobDataPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorHobDataPtr or UsbCConnectorHobDataPtr is not available!!\n", __FUNCTION__));
    return;
  }

  UsbConnectorBoardConfig = UsbConnectorHobDataPtr->UsbConnectorBoardConfig;
  if (UsbConnectorBoardConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: UsbConnectorBoardConfig is not available!!\n", __FUNCTION__));
    return;
  }

  if (PcdGet8 (VpdPcdUsbCUcmMode)) {
    for (ConnectorIndex = 0; ConnectorIndex < UsbCConnectorHobDataPtr->NumberOfUsbCConnectors; ConnectorIndex++) {
      //
      // Check TCSS USBC port map to USB 2.0
      //
      if (UsbConnectorBoardConfig[ConnectorIndex].ConnectorConnectable == CONNECTABLE &&
          UsbConnectorBoardConfig[ConnectorIndex].Usb3Controller == TCSS_USB3 &&
          UsbConnectorBoardConfig[ConnectorIndex].Usb2Controller == PCH_USB2) {
        UPDATE_POLICY (
          UsbConfig->PortUsb20[UsbConnectorBoardConfig[ConnectorIndex].Usb2PortNum].PortResetMessageEnable,
          TRUE
          );
      }
    }
  }
}

/**
  Update Usb config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] SiPreMemPolicyPpi    Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateUsbConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi,
  IN PCH_SETUP                 *PchSetup
  )
{
  UINTN           PortIndex;
  USB_CONFIG      *UsbConfig;
  UINT8           Usb2PortCount;
  UINT8           Usb3PortCount;
  EFI_STATUS      Status;

  UsbConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  Usb2PortCount = GetPchUsb2MaxPhysicalPortNum ();
  Usb3PortCount = GetPchXhciMaxUsb3PortNum ();

  UPDATE_POLICY (UsbConfig->PdoProgramming, PchSetup->PchUsbPdoProgramming);
  UPDATE_POLICY (UsbConfig->OverCurrentEnable, PchSetup->PchUsbOverCurrentEnable);
  UPDATE_POLICY (UsbConfig->UaolEnable, !!PchSetup->PchXhciUaol);

  for (PortIndex = 0; PortIndex < Usb2PortCount; PortIndex++) {
    UPDATE_POLICY (UsbConfig->PortUsb20[PortIndex].Enable, !!PchSetup->PchUsbHsPort[PortIndex]);

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
    UPDATE_POLICY (
      UsbConfig->PortUsb20[PortIndex].SwDeviceModeEnable,
      !!PchSetup->PchUsbSwDevModeEnable[PortIndex]
      );
#endif
  }
  for (PortIndex = 0; PortIndex < Usb3PortCount; PortIndex++) {
    UPDATE_POLICY (
      UsbConfig->PortUsb30[PortIndex].Enable,
      !!PchSetup->PchUsbSsPort[PortIndex]
      );
  }

  //
  // xDCI (USB device) related settings from setup variable
  //
  UPDATE_POLICY (UsbConfig->XdciConfig.Enable, !!PchSetup->PchXdciSupport);
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  UPDATE_POLICY (UsbConfig->Usb31Speed, !!PchSetup->PchUsb31Speed);
#endif

  //
  // Update both USB2 and USB3 OC pin mapping based on platform design
  //
  UpdateUsbOverCurrentPolicy (UsbConfig);

  //
  // Update USB2 Port Reset Message Enable Policy based on platform design
  //
  UpdateUsb2PortResetMessageEnablePolicy (UsbConfig);

  //
  // Update USB2 Port Enable Policy based on USBC port capability option
  //
  UpdateUsb2PortEnablePolicy (SiPreMemPolicyPpi, UsbConfig);
}

/**
  Update USB2 PHY config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdatePchUsb2PhyConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  USB2_PHY_TABLE    *Usb2PhyTuningTable;
  UINT8             PortIndex;
  UINT8             Usb2PortCount;
  USB2_PHY_CONFIG   *Usb2PhyConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsb2PhyConfigGuid, (VOID *) &Usb2PhyConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  Usb2PortCount = GetPchUsb2MaxPhysicalPortNum ();

  //
  // Get USB2 PHY tuning table address and verify it's available
  //
  Usb2PhyTuningTable = (USB2_PHY_TABLE *)(UINTN) PcdGet64 (PcdUsb2PhyTuningTable);
  if (Usb2PhyTuningTable == NULL) {
    DEBUG ((DEBUG_INFO, "UpdateUsb2PhyPolicy: No data for USB2 PHY tuning provided.\n"));
    return;
  }

  //
  // Display warning for size difference
  //
  if (Usb2PhyTuningTable->Size != Usb2PortCount) {
    DEBUG ((DEBUG_WARN, "UpdateUsb2PhyPolicy: Invalid USB2 PHY tuning table size. Got: %d\n", Usb2PhyTuningTable->Size));
  }

  //
  // To avoid overflow get minimum from either table size or max USB2 port count
  //
  Usb2PortCount = MIN (Usb2PhyTuningTable->Size, Usb2PortCount);
  DEBUG ((DEBUG_INFO, "UpdateUsb2PhyPolicy: USB2 PHY parameters will be updated for %d USB2 ports.\n", Usb2PortCount));

  for (PortIndex = 0; PortIndex < Usb2PortCount; PortIndex++) {
    UPDATE_POLICY (Usb2PhyConfig->Port[PortIndex].Petxiset , Usb2PhyTuningTable->Data[PortIndex].Petxiset);
    UPDATE_POLICY (Usb2PhyConfig->Port[PortIndex].Txiset   , Usb2PhyTuningTable->Data[PortIndex].Txiset);
    UPDATE_POLICY (Usb2PhyConfig->Port[PortIndex].Predeemp , Usb2PhyTuningTable->Data[PortIndex].Predeemp);
    UPDATE_POLICY (Usb2PhyConfig->Port[PortIndex].Pehalfbit, Usb2PhyTuningTable->Data[PortIndex].Pehalfbit);
  }
}

/**
  Update UFS configuration.

  @param[in] SiPolicyPpi  Pointer to SI_POLICY_PPI
  @param[in] PchSetup  Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateUfsConfig (
  IN SI_POLICY_PPI  *SiPolicyPpi,
  IN PCH_SETUP      *PchSetup
  )
{
  UINT8           UfsIndex;
  SCS_UFS_CONFIG  *UfsConfig;
  EFI_STATUS      Status;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gUfsConfigGuid, (VOID *) &UfsConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

#if FixedPcdGet8(PcdFspModeSelection) == 1
  //
  // FSP has disjoint configuration space allocation so we check for both UFS number supported in code
  // and UFS supported by FSPS_UPD
  //
  for (UfsIndex = 0; (UfsIndex < PchGetMaxUfsNum ()) && UfsIndex < 2; UfsIndex++) {
#else
  for (UfsIndex = 0; UfsIndex < PchGetMaxUfsNum (); UfsIndex++) {
#endif
    COMPARE_UPDATE_POLICY_ARRAY(UfsConfig->UfsControllerConfig[UfsIndex].Enable, PchSetup->PchScsUfsEnable[UfsIndex], UfsIndex);
    UPDATE_POLICY (UfsConfig->UfsControllerConfig[UfsIndex].InlineEncryption, PchSetup->PchScsUfsInlineEncryption[UfsIndex]);
  }
}

/**
  Update HD Audio configuration.

  @param[in] SiPolicyPpi          Pointer to SI_POLICY_PPI
  @param[in] SiPreMemPolicyPpi Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup          Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateHdAudioConfig (
  IN SI_POLICY_PPI         *SiPolicyPpi,
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi,
  IN PCH_SETUP             *PchSetup
  )
{
  HDA_VERB_TABLE_DATABASE  *HdaVerbTableDatabase;
  HDAUDIO_CONFIG           *HdAudioConfig;
  HDAUDIO_PREMEM_CONFIG    *HdAudioPreMemConfig;
  EFI_STATUS               Status;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gHdAudioConfigGuid, (VOID *) &HdAudioConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHdAudioPreMemConfigGuid, (VOID *) &HdAudioPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  COMPARE_AND_UPDATE_POLICY (HdAudioConfig->Pme,                   PchSetup->PchHdAudioPme                  );
  COMPARE_AND_UPDATE_POLICY (HdAudioConfig->HdAudioLinkFrequency,  PchSetup->PchHdaHdAudioLinkFreq          );
  COMPARE_AND_UPDATE_POLICY (HdAudioConfig->CodecSxWakeCapability, PchSetup->PchHdAudioCodecSxWakeCapability);
  //
  // Install HDA Link/iDisplay Codec Verb Table
  //
  if (PchSetup->PchHdAudio) {
    HdaVerbTableDatabase = (HDA_VERB_TABLE_DATABASE *) (UINTN) PcdGet64 (PcdHdaVerbTableDatabase);

    if (HdaVerbTableDatabase == NULL) {
      DEBUG ((DEBUG_ERROR, "HdaVerbTableDatabase is NULL!\n"));
      ASSERT (FALSE);
      return;
    }

    UPDATE_POLICY (HdAudioConfig->VerbTableEntryNum, HdaVerbTableDatabase->Size);
    UPDATE_POLICY (HdAudioConfig->VerbTablePtr, (UINTN) HdaVerbTableDatabase->HdaVerbTable);
  }

  COMPARE_AND_UPDATE_POLICY (HdAudioConfig->MicPrivacy.Mode,                            PchSetup->PchHdaMicPrivacyMode                           );
  COMPARE_AND_UPDATE_POLICY (HdAudioConfig->MicPrivacy.Deglitch,                        PchSetup->PchHdaMicPrivacyDeglitch                       );
  COMPARE_AND_UPDATE_POLICY (HdAudioConfig->MicPrivacy.HwModeSoundWire0,                PchSetup->PchHdaMicPrivacyHwModeSoundWire0               );
  COMPARE_AND_UPDATE_POLICY (HdAudioConfig->MicPrivacy.HwModeSoundWire1,                PchSetup->PchHdaMicPrivacyHwModeSoundWire1               );
  COMPARE_AND_UPDATE_POLICY (HdAudioConfig->MicPrivacy.HwModeSoundWire2,                PchSetup->PchHdaMicPrivacyHwModeSoundWire2               );
  COMPARE_AND_UPDATE_POLICY (HdAudioConfig->MicPrivacy.HwModeSoundWire3,                PchSetup->PchHdaMicPrivacyHwModeSoundWire3               );
  COMPARE_AND_UPDATE_POLICY (HdAudioConfig->MicPrivacy.HwModeSoundWire4,                PchSetup->PchHdaMicPrivacyHwModeSoundWire4               );
  COMPARE_AND_UPDATE_POLICY (HdAudioConfig->MicPrivacy.HwModeDmic,                      PchSetup->PchHdaMicPrivacyHwModeDmic                     );
  COMPARE_AND_UPDATE_POLICY (HdAudioConfig->MicPrivacy.Timeout,                         PchSetup->PchHdaMicPrivacyTimeout                        );
}

/**
  Update IO APIC Configuration

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
  @param[in] SetupVariables       Pointer to SETUP_DATA buffer
**/
VOID
UpdateIoApicConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup,
  IN SETUP_DATA                *SetupVariables
  )
{
  PCH_IOAPIC_CONFIG   *IoApicConfig;
  EFI_STATUS          Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  COMPARE_AND_UPDATE_POLICY (IoApicConfig->IoApicEntry24_119, PchSetup->PchIoApic24119Entries);
  //
  // To support SLP_S0, it's required to disable 8254 timer.
  // Note that CSM may require this option to be disabled for correct operation.
  // Once 8254 timer disabled, some legacy OPROM and legacy OS will fail while using 8254 timer.
  // For some OS environment that it needs to set 8254CGE in late state it should
  // set this policy to FALSE and use PmcSet8254ClockGateState (TRUE) in SMM later.
  // This is also required during S3 resume.
  //
  // The Enable8254ClockGatingOnS3 is only applicable when Enable8254ClockGating is disabled.
  // If Enable8254ClockGating is enabled, RC will do 8254 CGE programming on S3 as well.
  // else, RC will do the programming on S3 when Enable8254ClockGatingOnS3 is enabled.
  // This avoids the SMI requirement for the programming.
  //
  if (PchSetup->Enable8254ClockGating == 1) {
#if ((!defined (MDEPKG_NDEBUG)) && (FixedPcdGet8 (PcdFspModeSelection) == 0) && (FixedPcdGetBool (PcdPolicyDefaultChkEnable) == 1))
    if ((IoApicConfig->Enable8254ClockGating != TRUE) && (IsPolicyDefaultCheckRequired())) {
      POLICY_DEBUG_WARNING (IoApicConfig->Enable8254ClockGating, PchSetup->Enable8254ClockGating);
   }
#endif
    UPDATE_POLICY (IoApicConfig->Enable8254ClockGating, TRUE);
  } else if (PchSetup->Enable8254ClockGating == 2) {
#if ((!defined (MDEPKG_NDEBUG)) && (FixedPcdGet8 (PcdFspModeSelection) == 0) && (FixedPcdGetBool (PcdPolicyDefaultChkEnable) == 1))
    if ((IoApicConfig->Enable8254ClockGating != FALSE) && (IsPolicyDefaultCheckRequired())) {
      POLICY_DEBUG_WARNING (IoApicConfig->Enable8254ClockGating, PchSetup->Enable8254ClockGating);
    }
    if ((IoApicConfig->Enable8254ClockGatingOnS3 != TRUE) && (IsPolicyDefaultCheckRequired())) {
      POLICY_DEBUG_WARNING (IoApicConfig->Enable8254ClockGatingOnS3, PchSetup->Enable8254ClockGating);
    }
#endif
    UPDATE_POLICY (IoApicConfig->Enable8254ClockGating, FALSE);
    UPDATE_POLICY (IoApicConfig->Enable8254ClockGatingOnS3, TRUE);
  } else {
#if ((!defined (MDEPKG_NDEBUG)) && (FixedPcdGet8 (PcdFspModeSelection) == 0) && (FixedPcdGetBool (PcdPolicyDefaultChkEnable) == 1))
    if ((IoApicConfig->Enable8254ClockGating != FALSE) && (IsPolicyDefaultCheckRequired())) {
      POLICY_DEBUG_WARNING (IoApicConfig->Enable8254ClockGating, PchSetup->Enable8254ClockGating);
    }
    if ((IoApicConfig->Enable8254ClockGatingOnS3 != FALSE) && (IsPolicyDefaultCheckRequired())) {
      POLICY_DEBUG_WARNING (IoApicConfig->Enable8254ClockGatingOnS3, PchSetup->Enable8254ClockGating);
    }
#endif
    UPDATE_POLICY (IoApicConfig->Enable8254ClockGating, FALSE);
    UPDATE_POLICY (IoApicConfig->Enable8254ClockGatingOnS3, FALSE);
  }

}

/**
  Update PCIe Root Port Configuration

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
  @param[in] SetupVariables       Pointer to SETUP_DATA buffer
**/
VOID
UpdatePcieRpConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup,
  IN SETUP_DATA                *SetupVariables
  )
{
  UINT8                           Index;
  UINTN                           MaxPciePorts;
  UINT8                           RpIndex;
  EFI_STATUS                      Status;

  FIA_CONFIG                      *FiaConfig;
  PCH_PCIE_CONFIG                 *PchPcieConfig;

#if FixedPcdGetBool (PcdDTbtEnable) == 1
  PEI_DTBT_POLICY                 *PeiDTbtConfig;
  UINT8                           TbtSelector;
#endif

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFiaConfigGuid, (VOID *) &FiaConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchPcieConfigGuid, (VOID *) &PchPcieConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  MaxPciePorts = GetPchMaxPciePortNum ();
#if FixedPcdGetBool (PcdDTbtEnable) == 1
  PeiDTbtConfig = NULL;

  if (SetupVariables->DiscreteTbtSupport == 1) {
    //
    // Obtain TBT Info from PEI TBT Policy.
    //
    Status = PeiServicesLocatePpi (
               &gPeiDTbtPolicyPpiGuid,
               0,
               NULL,
               (VOID **) &PeiDTbtConfig
               );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return;
    }
    if (PeiDTbtConfig != NULL) {
      for (Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
        if (PeiDTbtConfig->DTbtControllerConfig[Index].DTbtControllerEn == 0x1) {
          //
          // Get DTBT PCIe root port number
          //
          TbtSelector = PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpNumber;

          //
          // Set DTBT PCIe root port maximum payload size (MPS)
          // PcieMps = 0 : PchPcieMaxPayload128
          //           1 : PchPcieMaxPayload256
          //
          PchSetup->PcieRootPortMaxPayload[TbtSelector] = (PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpMps == 1) ? PchPcieMaxPayload256 : PchPcieMaxPayload128;
        }
      }
    }
  }
#endif

  //
  // PCI express config
  //
  COMPARE_AND_UPDATE_POLICY (PchPcieConfig->PcieCommonConfig.ComplianceTestMode, PchSetup->PcieComplianceTestMode);
  UPDATE_POLICY (PchPcieConfig->PcieCommonConfig.EnablePort8xhDecode, PchSetup->PcieRootPort8xhDecode);
  UPDATE_POLICY (PchPcieConfig->PchPciePort8xhDecodePortIndex, PchSetup->Pcie8xhDecodePortIndex);
  for (Index = 0; Index < MaxPciePorts; Index++) {
    UPDATE_POLICY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PhysicalSlotNumber, (UINT16) Index);
    UPDATE_POLICY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.ClkReqDetect,       TRUE);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.Aspm,                          PchSetup->PcieRootPortAspm[Index],        Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.L1Substates,                   PchSetup->PcieRootPortL1SubStates[Index], Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieSpeed,                     PchSetup->PcieRootPortSpeed[Index],       Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AcsEnabled,                    PchSetup->PcieRootPortACS[Index],         Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PtmEnabled,                    PchSetup->PcieRootPortPTM[Index],         Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SlotImplemented,               PchSetup->PcieRootPortSI[Index],          Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PmSci,                         PchSetup->PcieRootPortPMCE[Index],        Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.HotPlug,                       PchSetup->PcieRootPortHPE[Index],         Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.UnsupportedRequestReport,      PchSetup->PcieRootPortURE[Index],         Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.FatalErrorReport,              PchSetup->PcieRootPortFEE[Index],         Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.NoFatalErrorReport,            PchSetup->PcieRootPortNFE[Index],         Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.CorrectableErrorReport,        PchSetup->PcieRootPortCEE[Index],         Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnFatalError,       PchSetup->PcieRootPortSFE[Index],         Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnNonFatalError,    PchSetup->PcieRootPortSNE[Index],         Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnCorrectableError, PchSetup->PcieRootPortSCE[Index],         Index);
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.DetectTimeoutMs,               PchSetup->PcieDetectTimeoutMs[Index],     Index);
    UPDATE_POLICY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EnablePeerMemoryWrite,         PchSetup->PcieRootPortPeerMemoryWriteEnable[Index]);
    UPDATE_POLICY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.LinkDownGpios,                 PchSetup->PcieRootPortLinkDownGpios[Index]);
    UPDATE_POLICY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieFomsCp, PchSetup->PcieFomsCp[Index]);
    UPDATE_POLICY (PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EqPhBypass, PchSetup->PcieRootPortEqPhBypass[Index]);
  }


  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum(); RpIndex++) {
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.LtrEnable,                     PchSetup->PchPcieLtrEnable[RpIndex],     RpIndex);
    if (PchSetup->PchPcieLtrEnable[RpIndex]) {
      UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency, 0x100F);
      UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency,0x100F);
    }
    UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMode,          PchSetup->PchPcieSnoopLatencyOverrideMode[RpIndex]);
    UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMultiplier,    PchSetup->PchPcieSnoopLatencyOverrideMultiplier[RpIndex]);
    UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMode,       PchSetup->PchPcieNonSnoopLatencyOverrideMode[RpIndex]);
    UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMultiplier, PchSetup->PchPcieNonSnoopLatencyOverrideMultiplier[RpIndex]);
    UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideValue,         PchSetup->PchPcieSnoopLatencyOverrideValue[RpIndex]);
    UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideValue,      PchSetup->PchPcieNonSnoopLatencyOverrideValue[RpIndex]);
    UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.LtrOverrideSpecCompliant,          PchSetup->PchPcieLtrOverrideSpecCompliant[RpIndex]);
  //
  // Completion Timeout
  //
    COMPARE_UPDATE_POLICY_ARRAY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.CompletionTimeout,              PchSetup->CompletionTimeout[RpIndex], RpIndex);

  //
  // Update Gen3 EQ settings
  //
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PcieLinkEqMethod,                   PchSetup->PcieGen3EqMethod[RpIndex]              );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PcieLinkEqMode,                     PchSetup->PcieGen3EqMode[RpIndex]                );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.LocalTxOverrideEn,                  PchSetup->PcieGen3EqPh2LocalTxOverrideEn[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph1DpTxPreset,                      PchSetup->PcieGen3EqPh1DpTxPreset[RpIndex]       );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph1UpTxPreset,                      PchSetup->PcieGen3EqPh1UpTxPreset[RpIndex]       );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff,               PchSetup->PcieGen3EqPh3NoOfPresetOrCoeff[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[0].PreCursor,   PchSetup->PcieGen3EqPh3PreCursor0List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[0].PostCursor,  PchSetup->PcieGen3EqPh3PostCursor0List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[1].PreCursor,   PchSetup->PcieGen3EqPh3PreCursor1List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[1].PostCursor,  PchSetup->PcieGen3EqPh3PostCursor1List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[2].PreCursor,   PchSetup->PcieGen3EqPh3PreCursor2List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[2].PostCursor,  PchSetup->PcieGen3EqPh3PostCursor2List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[3].PreCursor,   PchSetup->PcieGen3EqPh3PreCursor3List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[3].PostCursor,  PchSetup->PcieGen3EqPh3PostCursor3List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[4].PreCursor,   PchSetup->PcieGen3EqPh3PreCursor4List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[4].PostCursor,  PchSetup->PcieGen3EqPh3PostCursor4List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[5].PreCursor,   PchSetup->PcieGen3EqPh3PreCursor5List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[5].PostCursor,  PchSetup->PcieGen3EqPh3PostCursor5List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[6].PreCursor,   PchSetup->PcieGen3EqPh3PreCursor6List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[6].PostCursor,  PchSetup->PcieGen3EqPh3PostCursor6List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[7].PreCursor,   PchSetup->PcieGen3EqPh3PreCursor7List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[7].PostCursor,  PchSetup->PcieGen3EqPh3PostCursor7List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[8].PreCursor,   PchSetup->PcieGen3EqPh3PreCursor8List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[8].PostCursor,  PchSetup->PcieGen3EqPh3PostCursor8List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[9].PreCursor,   PchSetup->PcieGen3EqPh3PreCursor9List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[9].PostCursor,  PchSetup->PcieGen3EqPh3PostCursor9List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[0],                   PchSetup->PcieGen3EqPh3Preset0List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[1],                   PchSetup->PcieGen3EqPh3Preset1List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[2],                   PchSetup->PcieGen3EqPh3Preset2List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[3],                   PchSetup->PcieGen3EqPh3Preset3List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[4],                   PchSetup->PcieGen3EqPh3Preset4List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[5],                   PchSetup->PcieGen3EqPh3Preset5List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[6],                   PchSetup->PcieGen3EqPh3Preset6List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[7],                   PchSetup->PcieGen3EqPh3Preset7List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[8],                   PchSetup->PcieGen3EqPh3Preset8List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[9],                   PchSetup->PcieGen3EqPh3Preset9List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[10],                  PchSetup->PcieGen3EqPh3Preset10List[RpIndex]     );
    if (PchSetup->PcieGen3EqPh2LocalTxOverrideEn[RpIndex]) {
      COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph2LocalTxOverridePreset,           PchSetup->PcieGen3EqPh2LocalTxOverridePreset[RpIndex]);
    }
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.EqPh3Bypass,                        PchSetup->PcieRootPortGen3EqPh3Bypass[RpIndex] );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.EqPh23Bypass,                       PchSetup->PcieRootPortGen3EqPh23Bypass[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PCETTimer,                          PchSetup->PcieGen3PcetTimer[RpIndex]           );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.TsLockTimer,                        PchSetup->PcieGen3TsLockTimer[RpIndex]         );
  //
  // Update Gen4 EQ settings
  //
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PcieLinkEqMethod,                   PchSetup->PcieGen4EqMethod[RpIndex]              );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PcieLinkEqMode,                     PchSetup->PcieGen4EqMode[RpIndex]                );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.LocalTxOverrideEn,                  PchSetup->PcieGen4EqPh2LocalTxOverrideEn[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph1DpTxPreset,                      PchSetup->PcieGen4EqPh1DpTxPreset[RpIndex]       );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph1UpTxPreset,                      PchSetup->PcieGen4EqPh1UpTxPreset[RpIndex]       );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff,               PchSetup->PcieGen4EqPh3NoOfPresetOrCoeff[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[0].PreCursor,   PchSetup->PcieGen4EqPh3PreCursor0List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[0].PostCursor,  PchSetup->PcieGen4EqPh3PostCursor0List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[1].PreCursor,   PchSetup->PcieGen4EqPh3PreCursor1List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[1].PostCursor,  PchSetup->PcieGen4EqPh3PostCursor1List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[2].PreCursor,   PchSetup->PcieGen4EqPh3PreCursor2List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[2].PostCursor,  PchSetup->PcieGen4EqPh3PostCursor2List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[3].PreCursor,   PchSetup->PcieGen4EqPh3PreCursor3List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[3].PostCursor,  PchSetup->PcieGen4EqPh3PostCursor3List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[4].PreCursor,   PchSetup->PcieGen4EqPh3PreCursor4List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[4].PostCursor,  PchSetup->PcieGen4EqPh3PostCursor4List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[5].PreCursor,   PchSetup->PcieGen4EqPh3PreCursor5List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[5].PostCursor,  PchSetup->PcieGen4EqPh3PostCursor5List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[6].PreCursor,   PchSetup->PcieGen4EqPh3PreCursor6List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[6].PostCursor,  PchSetup->PcieGen4EqPh3PostCursor6List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[7].PreCursor,   PchSetup->PcieGen4EqPh3PreCursor7List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[7].PostCursor,  PchSetup->PcieGen4EqPh3PostCursor7List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[8].PreCursor,   PchSetup->PcieGen4EqPh3PreCursor8List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[8].PostCursor,  PchSetup->PcieGen4EqPh3PostCursor8List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[9].PreCursor,   PchSetup->PcieGen4EqPh3PreCursor9List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[9].PostCursor,  PchSetup->PcieGen4EqPh3PostCursor9List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[0],                   PchSetup->PcieGen4EqPh3Preset0List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[1],                   PchSetup->PcieGen4EqPh3Preset1List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[2],                   PchSetup->PcieGen4EqPh3Preset2List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[3],                   PchSetup->PcieGen4EqPh3Preset3List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[4],                   PchSetup->PcieGen4EqPh3Preset4List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[5],                   PchSetup->PcieGen4EqPh3Preset5List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[6],                   PchSetup->PcieGen4EqPh3Preset6List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[7],                   PchSetup->PcieGen4EqPh3Preset7List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[8],                   PchSetup->PcieGen4EqPh3Preset8List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[9],                   PchSetup->PcieGen4EqPh3Preset9List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[10],                  PchSetup->PcieGen4EqPh3Preset10List[RpIndex]     );
    if (PchSetup->PcieGen4EqPh2LocalTxOverrideEn[RpIndex]) {
      COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph2LocalTxOverridePreset,           PchSetup->PcieGen4EqPh2LocalTxOverridePreset[RpIndex]);
    }
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.EqPh3Bypass,                        PchSetup->PcieRootPortGen4EqPh3Bypass[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.EqPh23Bypass,                       PchSetup->PcieRootPortGen4EqPh23Bypass[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PCETTimer,                          PchSetup->PcieGen4PcetTimer[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.TsLockTimer,                        PchSetup->PcieGen4TsLockTimer[RpIndex]);
  //
  // Update Gen5 EQ settings
  //
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PcieLinkEqMethod,                   PchSetup->PcieGen5EqMethod[RpIndex]              );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PcieLinkEqMode,                     PchSetup->PcieGen5EqMode[RpIndex]                );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.LocalTxOverrideEn,                  PchSetup->PcieGen5EqPh2LocalTxOverrideEn[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph1DpTxPreset,                      PchSetup->PcieGen5EqPh1DpTxPreset[RpIndex]       );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph1UpTxPreset,                      PchSetup->PcieGen5EqPh1UpTxPreset[RpIndex]       );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff,               PchSetup->PcieGen5EqPh3NoOfPresetOrCoeff[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[0].PreCursor,   PchSetup->PcieGen5EqPh3PreCursor0List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[0].PostCursor,  PchSetup->PcieGen5EqPh3PostCursor0List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[1].PreCursor,   PchSetup->PcieGen5EqPh3PreCursor1List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[1].PostCursor,  PchSetup->PcieGen5EqPh3PostCursor1List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[2].PreCursor,   PchSetup->PcieGen5EqPh3PreCursor2List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[2].PostCursor,  PchSetup->PcieGen5EqPh3PostCursor2List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[3].PreCursor,   PchSetup->PcieGen5EqPh3PreCursor3List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[3].PostCursor,  PchSetup->PcieGen5EqPh3PostCursor3List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[4].PreCursor,   PchSetup->PcieGen5EqPh3PreCursor4List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[4].PostCursor,  PchSetup->PcieGen5EqPh3PostCursor4List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[5].PreCursor,   PchSetup->PcieGen5EqPh3PreCursor5List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[5].PostCursor,  PchSetup->PcieGen5EqPh3PostCursor5List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[6].PreCursor,   PchSetup->PcieGen5EqPh3PreCursor6List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[6].PostCursor,  PchSetup->PcieGen5EqPh3PostCursor6List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[7].PreCursor,   PchSetup->PcieGen5EqPh3PreCursor7List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[7].PostCursor,  PchSetup->PcieGen5EqPh3PostCursor7List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[8].PreCursor,   PchSetup->PcieGen5EqPh3PreCursor8List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[8].PostCursor,  PchSetup->PcieGen5EqPh3PostCursor8List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[9].PreCursor,   PchSetup->PcieGen5EqPh3PreCursor9List[RpIndex]   );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[9].PostCursor,  PchSetup->PcieGen5EqPh3PostCursor9List[RpIndex]  );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[0],                   PchSetup->PcieGen5EqPh3Preset0List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[1],                   PchSetup->PcieGen5EqPh3Preset1List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[2],                   PchSetup->PcieGen5EqPh3Preset2List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[3],                   PchSetup->PcieGen5EqPh3Preset3List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[4],                   PchSetup->PcieGen5EqPh3Preset4List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[5],                   PchSetup->PcieGen5EqPh3Preset5List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[6],                   PchSetup->PcieGen5EqPh3Preset6List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[7],                   PchSetup->PcieGen5EqPh3Preset7List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[8],                   PchSetup->PcieGen5EqPh3Preset8List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[9],                   PchSetup->PcieGen5EqPh3Preset9List[RpIndex]      );
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[10],                  PchSetup->PcieGen5EqPh3Preset10List[RpIndex]     );
    if (PchSetup->PcieGen5EqPh2LocalTxOverrideEn[RpIndex]) {
      COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph2LocalTxOverridePreset,           PchSetup->PcieGen5EqPh2LocalTxOverridePreset[RpIndex]);
    }
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.EqPh3Bypass,                        PchSetup->PcieRootPortGen5EqPh3Bypass[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.EqPh23Bypass,                       PchSetup->PcieRootPortGen5EqPh23Bypass[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PCETTimer,                          PchSetup->PcieGen5PcetTimer[RpIndex]);
    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.TsLockTimer,                        PchSetup->PcieGen5TsLockTimer[RpIndex]);

    COMPARE_AND_UPDATE_POLICY (PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.OverrideEqualizationDefaults,                                      PchSetup->PcieEqOverrideDefault[RpIndex]);
  }
}

/**
  Update ISH config

  @param[in] SiPolicyPpi             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateIshConfig (
  IN SI_POLICY_PPI             *SiPolicyPpi,
  IN PCH_SETUP                 *PchSetup
  )
{
  UINT8                           Index;
  ISH_CONFIG                      *IshConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gIshConfigGuid, (VOID *) &IshConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < GetPchMaxIshSpiControllersNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (IshConfig->Spi[Index].Enable,      PchSetup->PchIshSpiEnable[Index],    Index);
    COMPARE_UPDATE_POLICY_ARRAY (IshConfig->Spi[Index].CsEnable[0], PchSetup->PchIshSpiCs0Enable[Index], Index);
  }
  for (Index = 0; Index < GetPchMaxIshUartControllersNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (IshConfig->Uart[Index].Enable,     PchSetup->PchIshUartEnable[Index],   Index);
  }
  for (Index = 0; Index < GetPchMaxIshI2cControllersNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (IshConfig->I2c[Index].Enable,      PchSetup->PchIshI2cEnable[Index],    Index);
  }
  for (Index = 0; Index < GetPchMaxIshI3cControllersNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (IshConfig->I3c[Index].Enable,      PchSetup->PchIshI3cEnable[Index],    Index);
  }
  for (Index = 0; Index < GetPchMaxIshGpNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (IshConfig->Gp[Index].Enable,       PchSetup->PchIshGpEnable[Index],     Index);
  }
  COMPARE_AND_UPDATE_POLICY (IshConfig->PdtUnlock,              PchSetup->PchIshPdtUnlock   );
  COMPARE_AND_UPDATE_POLICY (IshConfig->MsiInterrupt,           PchSetup->PchIshMsiInterrupt);

  // ISH Pin MUX - TBD
  UPDATE_POLICY (IshConfig->Spi[0].PinConfig.Cs[0].PinMux, 0x0);
  UPDATE_POLICY (IshConfig->Spi[0].PinConfig.Clk.PinMux, 0x0);
  UPDATE_POLICY (IshConfig->Spi[0].PinConfig.Miso.PinMux, 0x0);
  UPDATE_POLICY (IshConfig->Spi[0].PinConfig.Mosi.PinMux, 0x0);
  UPDATE_POLICY (IshConfig->Uart[1].PinConfig.Rx.PinMux, 0x0);
  UPDATE_POLICY (IshConfig->Uart[1].PinConfig.Tx.PinMux, 0x0);

  UPDATE_POLICY (IshConfig->I2c[2].PinConfig.Sda.PinMux, 0x0);
  UPDATE_POLICY (IshConfig->I2c[2].PinConfig.Scl.PinMux, 0x0);

  UPDATE_POLICY (IshConfig->Gp[5].PinConfig.PinMux, 0x0);
  UPDATE_POLICY (IshConfig->Gp[6].PinConfig.PinMux, 0x0);
  UPDATE_POLICY (IshConfig->Gp[8].PinConfig.PinMux, 0x0);
  UPDATE_POLICY (IshConfig->Gp[9].PinConfig.PinMux, 0x0);
  UPDATE_POLICY (IshConfig->Gp[10].PinConfig.PinMux, 0x0);
  UPDATE_POLICY (IshConfig->Gp[11].PinConfig.PinMux, 0x0);

}

/**
  Update THC config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateThcConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  UINT8       ThcIndex;
  THC_CONFIG  *ThcConfig;
  EFI_STATUS  Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gThcConfigGuid, (VOID *) &ThcConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (ThcIndex = 0; ThcIndex < PCH_MAX_THC_CONTROLLERS; ThcIndex++) {
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].Assignment,                           PchSetup->ThcAssignment[ThcIndex],             ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].WakeOnTouch,                          PchSetup->ThcWakeOnTouch[ThcIndex],            ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].WotPinConfig.EdgeLevel,               PchSetup->ThcWotEdgeLevel[ThcIndex],           ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].WotPinConfig.ActiveLevel,             PchSetup->ThcWotActiveLevel[ThcIndex],         ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].WotPinConfig.PinConfig,               PchSetup->ThcWotPinConfig[ThcIndex],           ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].Mode,                                 PchSetup->ThcMode[ThcIndex],                   ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].ThcSubSystemId.CustomizedSsid,        PchSetup->ThcCustomizedSsdid[ThcIndex],         ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].ThcSubSystemId.CustomizedSvid,        PchSetup->ThcCustomizedSsvid[ThcIndex],         ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].ActiveLtr,                            PchSetup->ThcActiveLtr[ThcIndex],              ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].IdleLtr,                              PchSetup->ThcIdleLtr[ThcIndex],                ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].TimestampTimerMode,                   PchSetup->ThcTimestampTimerMode[ThcIndex],     ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].DisplayFrameSyncPeriod,               PchSetup->ThcDisplayFrameSyncPeriod[ThcIndex], ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].PerformanceLimitation,                PchSetup->ThcPerformanceLimitation[ThcIndex],  ThcIndex);
    //
    // Reset Pad configuration is always 0x0, which indicates HW THC default.
    // Platform BIOS code can override it if Board design is not aligned with THC pads.
    //
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].Reset.ResetPad,                  0x0,                                                 ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].Reset.ResetPadTrigger,           PchSetup->ThcResetPadTrigger[ThcIndex],              ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].Reset.ResetSequencingDelay,      300,                                                 ThcIndex);

    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].ThcDsyncPadEnable,                    PchSetup->ThcDsyncPad[ThcIndex],                        ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverSpi.Frequency,                 PchSetup->ThcHidSpiConnectionSpeed[ThcIndex],           ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverSpi.InputReportHeaderAddress,  PchSetup->ThcHidSpiInputReportHeaderAddress[ThcIndex],  ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverSpi.InputReportBodyAddress,    PchSetup->ThcHidSpiInputReportBodyAddress[ThcIndex],    ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverSpi.OutputReportAddress,       PchSetup->ThcHidSpiOutputReportAddress[ThcIndex],       ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverSpi.ReadOpcode,                PchSetup->ThcHidSpiReadOpcode[ThcIndex],                ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverSpi.WriteOpcode,               PchSetup->ThcHidSpiWriteOpcode[ThcIndex],               ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverSpi.Flags,                     PchSetup->ThcHidSpiFlags[ThcIndex],                     ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverSpi.LimitPacketSize ,          PchSetup->ThcHidSpiLimitPacketSize[ThcIndex],           ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].Clock,                                PchSetup->ThcClock[ThcIndex],                           ThcIndex);

    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.DeviceAddress,                                     PchSetup->ThcHidI2cDeviceAddress[ThcIndex],                                 ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.ConnectionSpeed,                                   PchSetup->ThcHidI2cConnectionSpeed[ThcIndex],                               ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.AddressingMode,                                    PchSetup->ThcHidI2cAddressingMode[ThcIndex],                                ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.DeviceDescriptorAddress,                           PchSetup->ThcHidI2cDeviceDescriptorAddress[ThcIndex],                       ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.StandardModeSerialClockLineHighPeriod,             PchSetup->ThcHidI2cStandardModeSerialClockLineHighPeriod[ThcIndex],         ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.StandardModeSerialClockLineLowPeriod,              PchSetup->ThcHidI2cStandardModeSerialClockLineLowPeriod[ThcIndex],          ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.StandardModeSerialDataLineTransmitHoldPeriod,      PchSetup->ThcHidI2cStandardModeSerialDataLineTransmitHoldPeriod[ThcIndex],  ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.StandardModeSerialDataLineReceiveHoldPeriod,       PchSetup->ThcHidI2cStandardModeSerialDataLineReceiveHoldPeriod[ThcIndex],   ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModeSerialClockLineHighPeriod,                 PchSetup->ThcHidI2cFastModeSerialClockLineHighPeriod[ThcIndex],             ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModeSerialClockLineLowPeriod,                  PchSetup->ThcHidI2cFastModeSerialClockLineLowPeriod[ThcIndex],              ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModeSerialDataLineTransmitHoldPeriod,          PchSetup->ThcHidI2cFastModeSerialDataLineTransmitHoldPeriod[ThcIndex],      ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModeSerialDataLineReceiveHoldPeriod,           PchSetup->ThcHidI2cFastModeSerialDataLineReceiveHoldPeriod[ThcIndex],       ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.MaxSuppressedSpikesSMFMFMP,                        PchSetup->ThcHidI2cMaxSuppressedSpikesSMFMFMP[ThcIndex],                    ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModePlusSerialClockLineHighPeriod,             PchSetup->ThcHidI2cFastModePlusSerialClockLineHighPeriod[ThcIndex],         ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModePlusSerialClockLineLowPeriod,              PchSetup->ThcHidI2cFastModePlusSerialClockLineLowPeriod[ThcIndex],          ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModePlusSerialDataLineTransmitHoldPeriod,      PchSetup->ThcHidI2cFastModePlusSerialDataLineTransmitHoldPeriod[ThcIndex],  ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModePlusSerialDataLineReceiveHoldPeriod,       PchSetup->ThcHidI2cFastModePlusSerialDataLineReceiveHoldPeriod[ThcIndex],   ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.HighSpeedModePlusSerialClockLineHighPeriod,        PchSetup->ThcHidI2cHighSpeedModePlusSerialClockLineHighPeriod[ThcIndex],    ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.HighSpeedModePlusSerialClockLineLowPeriod,         PchSetup->ThcHidI2cHighSpeedModePlusSerialClockLineLowPeriod[ThcIndex],     ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.HighSpeedModePlusSerialDataLineTransmitHoldPeriod, PchSetup->ThcHidI2cHighSpeedModePlusSerialDataLineTransmitHoldPeriod[ThcIndex], ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.HighSpeedModePlusSerialDataLineReceiveHoldPeriod,  PchSetup->ThcHidI2cHighSpeedModePlusSerialDataLineReceiveHoldPeriod[ThcIndex],  ThcIndex);
    COMPARE_UPDATE_POLICY_ARRAY (ThcConfig->ThcPort[ThcIndex].HidOverI2c.MaximumLengthOfSuppressedSpikesInHighSpeedMode,    PchSetup->ThcHidI2cMaximumLengthOfSuppressedSpikesInHighSpeedMode[ThcIndex], ThcIndex);
  }
}

#define MAX_CNVI_WIFI_SUBSYSTEM_ID_LIST    32

/**
  Check if CRF support USB only.
  @retval  TRUE   CRF support USB interface only
  @retval  FALSE  CRF support both USB and PCI interface
**/
STATIC
BOOLEAN
CrfSupportUsbInterfaceOnly (
  VOID
  )
{
  UINT16  SubSystemId;
  UINTN   Index;
  UINT16  UsbOnlySubSysIdList[MAX_CNVI_WIFI_SUBSYSTEM_ID_LIST];
  UINT8   UsbOnlySubSysIdListSize;

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  //
  // Check if CRF does support BT USB interface only and then apply policy accordingly.
  // Set BtInterface to only USB (1) if CRF does support USB only
  //
  UsbOnlySubSysIdListSize = PcdGet8 (PcdCnviWiFiSubSystemIdBtUsbOnlyListSize);
  if (UsbOnlySubSysIdListSize > MAX_CNVI_WIFI_SUBSYSTEM_ID_LIST) {
    //
    // Truncating entries beyond Max Table size
    //
    UsbOnlySubSysIdListSize = MAX_CNVI_WIFI_SUBSYSTEM_ID_LIST;
  }
  CopyMem (&UsbOnlySubSysIdList, PcdGetPtr (PcdCnviWiFiSubSystemIdBtUsbOnlyList), (UsbOnlySubSysIdListSize * sizeof (UINT16)));

  SubSystemId = PciSegmentRead16 (CnviWifiPciCfgBase () + PCI_SUBSYSTEM_ID_OFFSET);
  DEBUG ((DEBUG_INFO, "CNVi - CRF Wifi SubSystemId = %x\n", SubSystemId));
  for (Index = 0; Index < UsbOnlySubSysIdListSize; Index++) {
    if (SubSystemId == UsbOnlySubSysIdList [Index]) {
      PcdSetBoolS (PcdCnviCrfBtUsbOnly, TRUE);
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Update CNVi config

  @param[in] SiPolicyPpi          Pointer to SI_POLICY_PPI
  @param[in] CnvSetup             Pointer to CNV_VFR_CONFIG_SETUP buffer
**/
STATIC
VOID
UpdateCnviConfig (
  IN SI_POLICY_PPI             *SiPolicyPpi,
  IN CNV_VFR_CONFIG_SETUP      *CnvSetup
  )
{
  BOOLEAN                         IsUsbOnly;
  BOOLEAN                         NeedUpdate;
  CNVI_CONFIG                     *CnviConfig;
  EFI_STATUS                      Status;

  CnviConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCnviConfigGuid, (VOID *) &CnviConfig);

  if (EFI_ERROR (Status) || CnviConfig == NULL) {
    return;
  }

  COMPARE_AND_UPDATE_POLICY (CnviConfig->Mode,           CnvSetup->CnviMode          );
  COMPARE_AND_UPDATE_POLICY (CnviConfig->WifiCore,       CnvSetup->CnviWifiCore      );
  COMPARE_AND_UPDATE_POLICY (CnviConfig->BtCore,         CnvSetup->CnviBtCore        );

  IsUsbOnly = CrfSupportUsbInterfaceOnly ();
  NeedUpdate = FALSE;

  if (GetBootModeHob() == BOOT_WITH_DEFAULT_SETTINGS || GetBootModeHob() == BOOT_IN_RECOVERY_MODE) {
    //
    // Boot with default, Update.
    //
    DEBUG ((DEBUG_INFO, "Boot with default, update.o\n"));
    NeedUpdate = TRUE;
  } else if ((IsUsbOnly == 1 && CnvSetup->CnviBtInterfaceUsbOnly == 0) || (IsUsbOnly == 0 && CnvSetup->CnviBtInterfaceUsbOnly == 1)) {
    //
    // If detected CNV card not matched, Update.
    //
    DEBUG ((DEBUG_INFO, "Detected CNV card not matched, update.\n"));
    NeedUpdate = TRUE;
  }

  if (NeedUpdate) {
    if (IsUsbOnly) {
        UPDATE_POLICY (CnviConfig->BtInterface, 1);
        PcdSet8S (PcdCnviBtInterfaceUpdate, 1);
        DEBUG ((DEBUG_INFO, "Set PcdCnviBtInterfaceUpdate = 1 \n"));
    } else {
        UPDATE_POLICY (CnviConfig->BtInterface, 2);
        PcdSet8S (PcdCnviBtInterfaceUpdate, 2);
        DEBUG ((DEBUG_INFO, "Set PcdCnviBtInterfaceUpdate = 2 \n"));
    }
  } else {
    COMPARE_AND_UPDATE_POLICY (CnviConfig->BtInterface, CnvSetup->CnviBtInterface);
    PcdSet8S (PcdCnviBtInterfaceUpdate, 0);
    DEBUG ((DEBUG_INFO, "Set PcdCnviBtInterfaceUpdate = 0 \n"));
  }
  COMPARE_AND_UPDATE_POLICY (CnviConfig->BtAudioOffload,          CnvSetup->CnviBtAudioOffload);
  COMPARE_AND_UPDATE_POLICY (CnviConfig->BtAudioOffloadInterface, CnvSetup->CnviBtAudioOffloadInterface);
  UPDATE_POLICY (CnviConfig->PinMux.RfReset, 0);
  UPDATE_POLICY (CnviConfig->PinMux.Clkreq,  0);
}

/**
  Update Thermal config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
  @param[in] SaSetup              Pointer to SA_SETUP
**/
STATIC
VOID
UpdateThermalConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup,
  IN SA_SETUP                  *SaSetup
  )
{
  THERMAL_CONFIG                  *ThermalConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gThermalConfigGuid, (VOID *) &ThermalConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  UPDATE_POLICY (ThermalConfig->PchHotEnable, PcdGetBool (PcdPchThermalHotEnable));

  //
  // Program Thermal Throttling Level
  //
  COMPARE_AND_UPDATE_POLICY (ThermalConfig->TTLevels.SuggestedSetting,   PchSetup->PchTtLevelSuggestSet  );
  COMPARE_AND_UPDATE_POLICY (ThermalConfig->TTLevels.TTLock,             PchSetup->PchThrmTtLock         );
  COMPARE_AND_UPDATE_POLICY (ThermalConfig->TTLevels.TTState13Enable,    PchSetup->PchThrmTtState13Enable);
  COMPARE_AND_UPDATE_POLICY (ThermalConfig->TTLevels.TTEnable,           PchSetup->PchThrmTtEnable       );
  COMPARE_AND_UPDATE_POLICY (ThermalConfig->TTLevels.T2Level,            PchSetup->PchThrmT2Level        );
  COMPARE_AND_UPDATE_POLICY (ThermalConfig->TTLevels.T1Level,            PchSetup->PchThrmT1Level        );
  COMPARE_AND_UPDATE_POLICY (ThermalConfig->TTLevels.T0Level,            PchSetup->PchThrmT0Level        );
}

/**
  Update GBE config

  @param[in] SiPolicyPpi          The RC POSTMEM Policy PPI instance
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateGbeConfig (
  IN SI_POLICY_PPI             *SiPolicyPpi,
  IN PCH_SETUP                 *PchSetup
  )
{
  GBE_CONFIG                   *GbeConfig;
  EFI_STATUS                   Status;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGbeConfigGuid, (VOID *) &GbeConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  //
  // LAN Config
  // Check Gbe Region to determine if hide LAN controller setup option
  //
  if (IsGbePresent (PmcGetPwrmBase ())) {
    COMPARE_AND_UPDATE_POLICY (GbeConfig->Enable, PchSetup->PchLan);

    //
    // WOL Support when Fast Startup is on in OS
    //
    UPDATE_POLICY (GbeConfig->PchWOLFastSupport, PchSetup->PchWOLFastSupport);
  } else {
    UPDATE_POLICY (GbeConfig->Enable, FALSE);
  }
}

#if FixedPcdGetBool (PcdEmbeddedEnable)
/**
  Update PCH TSN config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateTsnConfig (
  IN SI_POLICY_PPI             *SiPolicyPpi,
  IN PCH_SETUP                 *PchSetup
  )
{
  TSN_SUB_REGION           *TsnSubRegion;
  TSN_CONFIG               *TsnConfig;
  EFI_STATUS               Status;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gTsnConfigGuid, (VOID *) &TsnConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Acquire MAC Addresses from TSN Sub Region
  InstallTsnFv ();
  GetTsnSubRegion (&TsnSubRegion);

  UPDATE_POLICY (TsnConfig->TsnMacAddressHigh, TsnSubRegion->Port.MacAddressHigh);
  UPDATE_POLICY (TsnConfig->TsnMacAddressLow,  TsnSubRegion->Port.MacAddressLow);
  COMPARE_AND_UPDATE_POLICY (TsnConfig->Enable,        PchSetup->PchTsnEnable    );
  COMPARE_AND_UPDATE_POLICY (TsnConfig->TsnLinkSpeed,  PchSetup->PchTsnLinkSpeed );
  COMPARE_AND_UPDATE_POLICY (TsnConfig->MultiVcEnable, PchSetup->TsnMultiVcEnable);
}
#endif

/**
  Get Bios Guard variable

  @param[in] CpuSecurityPreMemConfig   Pointer to CPU Security PreMem Config
**/
STATIC
UINT32
GetBiosGuardEnable (
  IN CPU_SECURITY_PREMEM_CONFIG  *CpuSecurityPreMemConfig
  )
{
  UINT32 BiosGuard;
  GET_POLICY (CpuSecurityPreMemConfig->BiosGuard, BiosGuard);

  return BiosGuard;
}

/**
  Update LockDown config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
  @param[in] BiosGuardEnable      BIOS Guard enable
**/
STATIC
VOID
UpdateLockDownConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup,
  IN UINT32                    BiosGuardEnable
  )
{
#if FixedPcdGetBool(PcdCapsuleEnable) == 1
  EFI_HOB_GUID_TYPE               *GuidHob;

  GuidHob                 = NULL;
#endif

  PCH_LOCK_DOWN_CONFIG            *LockDownConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (BiosGuardEnable) {
    DEBUG ((DEBUG_INFO, "Enable InSMM.STS (EISS) and BIOS Lock Enable (BLE) since BIOS Guard is enabled.\n"));
    UPDATE_POLICY (LockDownConfig->BiosLock, TRUE);
  } else {
    COMPARE_AND_UPDATE_POLICY (LockDownConfig->BiosLock, PchSetup->PchBiosLock);
  }

#if FixedPcdGetBool(PcdCapsuleEnable) == 1
  GuidHob = GetFirstGuidHob (&gSysFwUpdateProgressGuid);

  if (IsBiosUpdateRequired () || ((GetBootModeHob () == BOOT_IN_RECOVERY_MODE) &&
     ((((SYSTEM_FIRMWARE_UPDATE_PROGRESS *) GET_GUID_HOB_DATA (GuidHob))->Component == UpdatinguCode)||
     (((SYSTEM_FIRMWARE_UPDATE_PROGRESS *) GET_GUID_HOB_DATA (GuidHob))->Component == UpdatingBtGAcm)))) {
    DEBUG ((DEBUG_INFO, "Disabling BIOS lock for BIOS Update Process\n"));
    PchSetup->PchBiosLock = 0;
    COMPARE_AND_UPDATE_POLICY (LockDownConfig->BiosLock, PchSetup->PchBiosLock);
    DEBUG ((DEBUG_INFO, "Disabling BIOS interface lockdown for supporting Fault tolerant BIOS Update\n"));
    UPDATE_POLICY (LockDownConfig->BiosInterface, FALSE);
  }
#endif

  COMPARE_AND_UPDATE_POLICY (LockDownConfig->UnlockGpioPads, PchSetup->UnlockGpioPads);
}

/**
  Update Pm config
  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdatePmWakeConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_PM_CONFIG                   *PmConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

}


/**
  Update Pm config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
  @param[in] SetupVariables       Pointer to SETUP_DATA buffer
**/
STATIC
VOID
UpdatePmConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup,
  IN SETUP_DATA                *SetupVariables
  )
{
  PCH_PM_CONFIG                   *PmConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  COMPARE_AND_UPDATE_POLICY (PmConfig->WakeConfig.WolEnableOverride,  PchSetup->PchWakeOnLan);
  //
  // M.2 WiFi/BT slot power needs to be kept during Sx for CNVi. Hence overriding PchPmWoWlanEnable to 1 when CNVi is present.
  //
  if (CnviCrfModuleIsPresent ()) {
    UPDATE_POLICY (PmConfig->WakeConfig.WoWlanEnable, 1);
  } else {
    COMPARE_AND_UPDATE_POLICY (PmConfig->WakeConfig.WoWlanEnable, PchSetup->PchWakeOnWlan);
  }
  COMPARE_AND_UPDATE_POLICY (PmConfig->SlpLanLowDc,                   PchSetup->SlpLanLowDc);
  COMPARE_AND_UPDATE_POLICY (PmConfig->PsOnEnable,                    PchSetup->PsOnEnable );

  // Power Button Override Period (PBOP): refer to EDS for detail
  // Encoding:
  // 000b - 4 seconds
  // 011b - 10 seconds
  // power button behavior will depend on this setting and other features
  // such as power button SMI which will shutdown system immediately when it is enabled in pre-OS.
  UPDATE_POLICY (PmConfig->PwrBtnOverridePeriod, !!SetupVariables->LowPowerS0Idle? 0x3 : 0);

  UPDATE_POLICY (PmConfig->MeWakeSts,             TRUE);

  UPDATE_POLICY (PmConfig->PmcWdtTimerEn, PchSetup->PmcWdtTimerEn);

  COMPARE_AND_UPDATE_POLICY (PmConfig->EnableTcoTimer,              PchSetup->EnableTcoTimer  );
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  COMPARE_AND_UPDATE_POLICY (PmConfig->EnableTimedGpio0,            PchSetup->EnableTimedGpio0);
  COMPARE_AND_UPDATE_POLICY (PmConfig->EnableTimedGpio1,            PchSetup->EnableTimedGpio1);
#endif
  UPDATE_POLICY (PmConfig->VrAlert,               PcdGetBool (PcdVrAlertEnable));
  UPDATE_POLICY (PmConfig->CpuC10GatePinEnable,   1);
  UPDATE_POLICY (PmConfig->ModPhySusPgEnable,     IsPchLp ());
  if (IsPchLp ()) {
    COMPARE_AND_UPDATE_POLICY (PmConfig->Usb2PhySusPgEnable, PchSetup->PchUsb2SusWellPgEnable);
  } else {
    UPDATE_POLICY (PmConfig->Usb2PhySusPgEnable, 0);
  }
  COMPARE_AND_UPDATE_POLICY (PmConfig->OsIdleEnable,                  PchSetup->PchPmcOsIdleModeEnable          );
  COMPARE_AND_UPDATE_POLICY (PmConfig->S0ixAutoDemotion,              PchSetup->PchS0ixAutoDemotion             );
  COMPARE_AND_UPDATE_POLICY (PmConfig->LatchEventsC10Exit,            PchSetup->PchLatchEventsC10Exit           );
  COMPARE_AND_UPDATE_POLICY (PmConfig->C10DynamicThresholdAdjustment, PchSetup->PmcC10DynamicThresholdAdjustment);

  UPDATE_POLICY (PmConfig->V1p05PhyExtFetControlEn, TRUE);
  UPDATE_POLICY (PmConfig->V1p05IsExtFetControlEn,  FALSE);
}

/**
  Update Espi config

  @param[in] SiPolicyPpi          Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateEspiConfig (
  IN SI_POLICY_PPI             *SiPolicyPpi,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_ESPI_CONFIG                 *EspiConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gEspiConfigGuid, (VOID *) &EspiConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  COMPARE_AND_UPDATE_POLICY (EspiConfig->LgmrEnable,          PchSetup->PchEspiLgmrEnable         );
  COMPARE_AND_UPDATE_POLICY (EspiConfig->HostC10ReportEnable, PchSetup->PchEspiHostC10ReportEnable);
}

/**
  Update the Hsio config.

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
**/
VOID
UpdateHsioConfig (
  IN SI_POLICY_PPI             *SiPolicy
  )
{
  EFI_STATUS                    Status;
  HSIO_CONFIG                   *HsioConfig;
  EFI_FIRMWARE_VOLUME_HEADER    *FvHeader;
  EFI_FFS_FILE_HEADER           *FfsHeader;
  EFI_FV_FILE_INFO              OemCsiFileInfo;
  EFI_FV_FILE_INFO              OemSphyFileInfo;
  EFI_FV_FILE_INFO              OemNphyFileInfo;

  FvHeader = NULL;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gHsioConfigGuid, (VOID *) &HsioConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Find Binaries FV base
  ///
  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)(UINTN) FixedPcdGet32 (PcdFlashFvFirmwareBinariesBase);

  ///
  /// ChipsetInit
  ///
  Status = PeiServicesFfsFindFileByName (&gOemCsiBinGuid, FvHeader, (VOID **) &FfsHeader);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM ChipsetInit binary not found!\n"));
  } else {
    Status = PeiServicesFfsGetFileInfo (FfsHeader, &OemCsiFileInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Get ChipsetInit file info failed\n"));
    } else {
      UPDATE_POLICY (HsioConfig->ChipsetInitBinPtr, OemCsiFileInfo.Buffer);
      UPDATE_POLICY (HsioConfig->ChipsetInitBinLen, OemCsiFileInfo.BufferSize);

      DEBUG ((DEBUG_INFO, "OEM ChipsetInit binary location: %x\n", OemCsiFileInfo.Buffer));
      DEBUG ((DEBUG_INFO, "OEM ChipsetInit binary size: %x\n", OemCsiFileInfo.BufferSize));
    }
  }

  ///
  /// Sphy
  ///
  Status = PeiServicesFfsFindFileByName (&gOemSnpsPhyTableGuid, FvHeader, (VOID **) &FfsHeader);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM Sphy binary not found!\n"));
  } else {
    Status = PeiServicesFfsGetFileInfo (FfsHeader, &OemSphyFileInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Get Sphy file info failed\n"));
    } else {
      UPDATE_POLICY (HsioConfig->SynpsPhyBinPtr, OemSphyFileInfo.Buffer);
      UPDATE_POLICY (HsioConfig->SynpsPhyBinLen, OemSphyFileInfo.BufferSize);

      DEBUG ((DEBUG_INFO, "OEM Sphy binary location: %x\n", OemSphyFileInfo.Buffer));
      DEBUG ((DEBUG_INFO, "OEM Sphy binary size: %x\n", OemSphyFileInfo.BufferSize));
    }
  }

  ///
  /// Nphy
  ///
  Status = PeiServicesFfsFindFileByName (&gOemNphyBinGuid, FvHeader, (VOID **) &FfsHeader);

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "OEM Nphy binary not found"));
  } else {
    Status = PeiServicesFfsGetFileInfo (FfsHeader, &OemNphyFileInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Get Nphy file info failed\n"));
    } else {
      UPDATE_POLICY (HsioConfig->NphyBinPtr, OemNphyFileInfo.Buffer);
      UPDATE_POLICY (HsioConfig->NphyBinLen, OemNphyFileInfo.BufferSize);

      DEBUG ((DEBUG_INFO, "OEM Nphy binary location: %x\n", OemNphyFileInfo.Buffer));
      DEBUG ((DEBUG_INFO, "OEM Nphy binary size: %x\n", OemNphyFileInfo.BufferSize));
    }
  }
}

/**
  Update FlashProtection config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
  @param[in] BiosGuardEnable      BIOS Guard enable
**/
STATIC
VOID
UpdateFlashProtectionConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup,
  IN UINT32                    BiosGuardEnable
  )
{
  UINT32                          BaseAddr;
  UINT32                          RegionSize;
  BOOLEAN                         FlashProtectionEnabled;
  EFI_STATUS                      Status;
  PCH_SPI_PPI                     *SpiPpi;
  UINT16                          ProtectedRangeBase;
  UINT16                          ProtectedRangeLimit;
  PCH_FLASH_PROTECTION_CONFIG     *FlashProtectionConfig;
#if FixedPcdGetBool(PcdCapsuleEnable) == 1
  SYSTEM_FIRMWARE_COMPONENT       UpdatingComponent;
#endif

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  FlashProtectionEnabled = PchSetup->FprrEnable;
#if FixedPcdGetBool(PcdCapsuleEnable) == 1
  UpdatingComponent      = 0xFF;
#endif

  if (IsBiosUpdateRequired () && ((GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) || (!BiosGuardEnable))) {
    DEBUG ((DEBUG_INFO, "Disable Flash Protection Range Registers for BIOS Update Process\n"));
    FlashProtectionEnabled = FALSE;
  }

#if FixedPcdGetBool(PcdCapsuleEnable) == 1
  IsFmpCapsuleForBiosUpdateDetected(&UpdatingComponent);
  if (IsBiosUpdateRequired () &&
     ((UpdatingComponent == (SYSTEM_FIRMWARE_COMPONENT) UpdatingBtGAcm) || \
     (UpdatingComponent == (SYSTEM_FIRMWARE_COMPONENT) UpdatinguCode))) {
    DEBUG ((DEBUG_INFO, "Disable Flash Protection Range Registers for uCode or BtG ACM Update Process\n"));
    FlashProtectionEnabled = FALSE;
  }
#endif

  //
  // Flash Protection Range Register initialization
  //
  if (FlashProtectionEnabled) {
    Status = PeiServicesLocatePpi (
               &gPchSpiPpiGuid,
               0,
               NULL,
               (VOID **)&SpiPpi
               );

    DEBUG ((DEBUG_INFO, "Enabling Flash Protection Range: LocatePpi gPchSpiPpiGuid = %r\n", Status));
    if (EFI_ERROR (Status)) {
      return;
    }
    //
    // Find the base address for BIOS region
    //
    Status = SpiPpi->GetRegionAddress (SpiPpi, FlashRegionBios, &BaseAddr, &RegionSize);
    DEBUG ((DEBUG_INFO, "BIOS Region: %r - Start Address: 0x%x - Size: 0x%x\n", Status, BaseAddr, RegionSize));
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return;
    }
    //
    // Assign FPRR ranges
    //
    // |--------------------------------------|
    // |ProtectRange[0]: OBB + IBB            |
    // |--------------------------------------|
    // |NV Variable (Skip protection)         |
    // |--------------------------------------|
    // |ProtectRange[1]: Fit/Non Fit Payload  |
    // |--------------------------------------|
    // |ProtectRange[2]: Extended BIOS region |
    // |    (If Extended BIOS is enabled)     |
    // |--------------------------------------|
#if FixedPcdGetBool (PcdExtendedBiosRegionSupport) == 0
    //
    // Initialize flash protection
    //
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[0].WriteProtectionEnable, TRUE);
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[0].ReadProtectionEnable, FALSE);
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[0].ProtectedRangeBase,  (UINT16)((BaseAddr + (FixedPcdGet32 (PcdFlashFvMicrocodeSize) + FixedPcdGet32 (PcdFlashNonFitPayloadSize)) + FixedPcdGet32 (PcdFlashNvStorageSize)) >> 12));
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[0].ProtectedRangeLimit, (UINT16)(((BaseAddr + RegionSize) - 1) >> 12));
    GET_POLICY (FlashProtectionConfig->ProtectRange[0].ProtectedRangeBase, ProtectedRangeBase);
    GET_POLICY (FlashProtectionConfig->ProtectRange[0].ProtectedRangeLimit, ProtectedRangeLimit);
    DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[0].ProtectedRangeBase  %x\n", ProtectedRangeBase));
    DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[0].ProtectedRangeLimit %x\n", ProtectedRangeLimit));

    // Initialize flash protection for Payload Region
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[1].WriteProtectionEnable, TRUE);
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[1].ReadProtectionEnable, FALSE);
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[1].ProtectedRangeBase,  (UINT16)(BaseAddr >> 12));
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[1].ProtectedRangeLimit, (UINT16)((BaseAddr + FixedPcdGet32 (PcdFlashFvMicrocodeSize) + FixedPcdGet32 (PcdFlashNonFitPayloadSize) - 1) >> 12));
    GET_POLICY (FlashProtectionConfig->ProtectRange[1].ProtectedRangeBase, ProtectedRangeBase);
    GET_POLICY (FlashProtectionConfig->ProtectRange[1].ProtectedRangeLimit, ProtectedRangeLimit);
    DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[1].ProtectedRangeBase  %x\n", ProtectedRangeBase));
    DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[1].ProtectedRangeLimit %x\n", ProtectedRangeLimit));
#else
    //
    // Initialize flash protection
    //
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[0].WriteProtectionEnable, TRUE);
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[0].ReadProtectionEnable, FALSE);
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[0].ProtectedRangeBase,  (UINT16)((BaseAddr + FixedPcdGet32 (PcdFlashExtendRegionSizeInUse) + (FixedPcdGet32 (PcdFlashFvMicrocodeSize) + FixedPcdGet32 (PcdFlashNonFitPayloadSize)) + FixedPcdGet32 (PcdFlashNvStorageSize)) >> 12));
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[0].ProtectedRangeLimit, (UINT16)(((BaseAddr + RegionSize) - 1) >> 12));
    GET_POLICY (FlashProtectionConfig->ProtectRange[0].ProtectedRangeBase, ProtectedRangeBase);
    GET_POLICY (FlashProtectionConfig->ProtectRange[0].ProtectedRangeLimit, ProtectedRangeLimit);
    DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[0].ProtectedRangeBase  %x\n", ProtectedRangeBase));
    DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[0].ProtectedRangeLimit %x\n", ProtectedRangeLimit));

    // Initialize flash protection for Payload Region
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[1].WriteProtectionEnable, TRUE);
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[1].ReadProtectionEnable, FALSE);
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[1].ProtectedRangeBase,  (UINT16)((BaseAddr + FixedPcdGet32 (PcdFlashExtendRegionSizeInUse)) >> 12));
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[1].ProtectedRangeLimit, (UINT16)((BaseAddr + FixedPcdGet32 (PcdFlashExtendRegionSizeInUse) + FixedPcdGet32 (PcdFlashFvMicrocodeSize) + FixedPcdGet32 (PcdFlashNonFitPayloadSize) - 1) >> 12));
    GET_POLICY (FlashProtectionConfig->ProtectRange[1].ProtectedRangeBase, ProtectedRangeBase);
    GET_POLICY (FlashProtectionConfig->ProtectRange[1].ProtectedRangeLimit, ProtectedRangeLimit);
    DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[1].ProtectedRangeBase  %x\n", ProtectedRangeBase));
    DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[1].ProtectedRangeLimit %x\n", ProtectedRangeLimit));

    // Initialize flash protection for Extended BIOS Region
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[2].WriteProtectionEnable, TRUE);
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[2].ReadProtectionEnable, FALSE);
    // Assign FPRR ranges for Extended BIOS Region
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[2].ProtectedRangeBase,  (UINT16)(BaseAddr >> 12));
    UPDATE_POLICY (FlashProtectionConfig->ProtectRange[2].ProtectedRangeLimit, (UINT16)(((BaseAddr + FixedPcdGet32 (PcdFlashExtendRegionSizeInUse)) - 1) >> 12));
    GET_POLICY (FlashProtectionConfig->ProtectRange[2].ProtectedRangeBase, ProtectedRangeBase);
    GET_POLICY (FlashProtectionConfig->ProtectRange[2].ProtectedRangeLimit, ProtectedRangeLimit);
    DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[2].ProtectedRangeBase  %x\n", ProtectedRangeBase));
    DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[2].ProtectedRangeLimit %x\n", ProtectedRangeLimit));
#endif
  }
}

/**
  Update Rtc config

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] PchSetup  Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateRtcConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  BOOLEAN                      RtcStatus = TRUE;
  RTC_CONFIG                   *RtcConfig;
  EFI_STATUS                   Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gRtcConfigGuid, (VOID *) &RtcConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (IsBiosUpdateRequired () || GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
    RtcStatus = FALSE;
  }

  DEBUG ((DEBUG_INFO, "set Rtc Bios Interface Lock based on the condition: %d\n", RtcStatus));
  UPDATE_POLICY (RtcConfig->BiosInterfaceLock, RtcStatus);

  COMPARE_AND_UPDATE_POLICY (RtcConfig->MemoryLock, PchSetup->PchRtcMemoryLock);
}

/**
  Update Ieh config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateIehConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  IEH_CONFIG                  *IehConfig;
  EFI_STATUS                  Status;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gIehConfigGuid, (VOID *) &IehConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  COMPARE_AND_UPDATE_POLICY (IehConfig->Mode, PchSetup->IehMode);
}

/**
  Update Interrupt config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateInterruptConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_INTERRUPT_CONFIG         *InterruptConfig;
  EFI_STATUS                   Status;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  COMPARE_AND_UPDATE_POLICY (InterruptConfig->GpioIrqRoute, PchSetup->PchGpioIrqRoute);
}

/**
  Update PCH General config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdatePchGeneralConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_GENERAL_CONFIG           *PchGeneralConfig;
  EFI_STATUS                   Status;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  COMPARE_AND_UPDATE_POLICY (PchGeneralConfig->Crid,               PchSetup->PchCrid              );
  COMPARE_AND_UPDATE_POLICY (PchGeneralConfig->LegacyIoLowLatency, PchSetup->PchLegacyIoLowLatency);
}

/**
  Correct mismatch configuration of ISH and Serial IO

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
CorrectMismatchConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  UINT8                           I2c2Mode;
  UINT8                           I2c3Mode;
  UINT8                           I2c5Mode;
  UINT8                           Uart1Mode;
  ISH_CONFIG                      *IshConfig;
  SERIAL_IO_CONFIG                *SerialIoConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gIshConfigGuid, (VOID *) &IshConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  GET_POLICY (SerialIoConfig->I2cDeviceConfig[2].Mode, I2c2Mode);
  if (IsPchH () && I2c2Mode != SerialIoI2cDisabled) {
    //
    // ISH UART0 use the same GPIO pins as SerialIo I2C2 on PCH-H
    //
    UPDATE_POLICY (IshConfig->Uart[0].Enable, FALSE);
  }

  GET_POLICY (SerialIoConfig->UartDeviceConfig[1].Mode, Uart1Mode);
  if (Uart1Mode != SerialIoUartDisabled) {
    //
    // ISH UART1 use the same GPIO pins as SerialIo UART1
    //
    UPDATE_POLICY (IshConfig->Uart[1].Enable, FALSE);
  }

  GET_POLICY (SerialIoConfig->I2cDeviceConfig[3].Mode, I2c3Mode);
  GET_POLICY (SerialIoConfig->I2cDeviceConfig[5].Mode, I2c5Mode);
  if ((IsPchLp () && I2c5Mode != SerialIoI2cDisabled) ||
      (IsPchH ()  && I2c3Mode != SerialIoI2cDisabled)) {
    //
    // ISH I2C2 use the same GPIO pins as SerialIo I2C5 on PCH-LP / I2C3 on PCH-H
    //
    UPDATE_POLICY (IshConfig->I2c[2].Enable, FALSE);
  }

}

#if FixedPcdGetBool (PcdEmbeddedEnable)
/**
  Update Fusa config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
  @param[in] SaSetup              Pointer to SA_SETUP buffer
**/
STATIC
VOID
UpdateFusaConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup,
  IN SA_SETUP                  *SaSetup
  )
{
  FUSA_CONFIG                *FusaConfig;
  EFI_STATUS                 Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFusaConfigGuid, (VOID *) &FusaConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  COMPARE_AND_UPDATE_POLICY (FusaConfig->DisplayFusaConfigEnable, SaSetup->DisplayFusaConfigEnable);
  COMPARE_AND_UPDATE_POLICY (FusaConfig->GraphicFusaConfigEnable, SaSetup->GraphicFusaConfigEnable);
  COMPARE_AND_UPDATE_POLICY (FusaConfig->OpioFusaConfigEnable,    SaSetup->OpioFusaConfigEnable   );
  COMPARE_AND_UPDATE_POLICY (FusaConfig->PsfFusaConfigEnable,     PchSetup->PsfFusaConfigEnable   );
}
#endif

/**
  This function performs PCH PEI Policy initialization.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicy (
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi,
  SI_POLICY_PPI                   *SiPolicy
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  SA_SETUP                        SaSetup;
  CPU_SETUP                       CpuSetup;
  PCH_SETUP                       PchSetup;
  SETUP_DATA                      SetupVariables;
  CNV_VFR_CONFIG_SETUP            CnvSetup;
  DEBUG_CONFIG_DATA               DebugConfigData;
  UINT32                          BiosGuardEnable;
  CPU_SECURITY_PREMEM_CONFIG      *CpuSecurityPreMemConfig;

  DEBUG ((DEBUG_INFO, "Update PeiPchPolicyUpdate Pos-Mem Start\n"));

  CpuSecurityPreMemConfig = NULL;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  BiosGuardEnable = GetBiosGuardEnable (CpuSecurityPreMemConfig);

  //
  // Retrieve Setup variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices       // PPI
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SetupVariables
                               );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SA_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"SaSetup",
                               &gSaSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SaSetup
                               );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (CPU_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"CpuSetup",
                               &gCpuSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &CpuSetup
                               );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (PCH_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &PchSetup
                               );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (CNV_VFR_CONFIG_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"CnvSetup",
                               &gCnvFeatureSetupGuid,
                               NULL,
                               &VariableSize,
                               &CnvSetup
                               );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (DEBUG_CONFIG_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"DebugConfigData",
                               &gDebugConfigVariableGuid,
                               NULL,
                               &VariableSize,
                               &DebugConfigData
                               );
DEBUG_CODE_BEGIN();
  ASSERT_EFI_ERROR (Status);
DEBUG_CODE_END();

  UpdatePchGeneralConfig (SiPolicy, &PchSetup);
  UpdateGbeConfig (SiPolicy, &PchSetup);
#if FixedPcdGetBool (PcdEmbeddedEnable)
  UpdateTsnConfig (SiPolicy, &PchSetup);
#endif
  UpdateThcConfig (SiPolicy, &PchSetup);
  UpdatePcieRpConfig (SiPolicy, &PchSetup, &SetupVariables);
  UpdateHdAudioConfig (SiPolicy, SiPreMemPolicyPpi, &PchSetup);
  UpdateLockDownConfig (SiPolicy, &PchSetup, BiosGuardEnable);
  UpdateFlashProtectionConfig (SiPolicy, &PchSetup, BiosGuardEnable);
  UpdateIehConfig (SiPolicy, &PchSetup);
  UpdateIoApicConfig (SiPolicy, &PchSetup, &SetupVariables);
  UpdateInterruptConfig (SiPolicy, &PchSetup);
  UpdateSerialIoConfig (SiPolicy, &PchSetup, &DebugConfigData, &SetupVariables);
  UpdatePmWakeConfig (SiPolicy, &PchSetup);
  UpdateLpssI3cConfig (SiPolicy, &PchSetup);
  UpdateLpssSpiConfig (SiPolicy, &PchSetup);
  UpdateLpssUartConfig (SiPolicy, &PchSetup, &DebugConfigData, &SetupVariables);
  UpdateLpssI2cConfig (SiPolicy, &PchSetup);
  UpdatePmConfig (SiPolicy, &PchSetup, &SetupVariables);
  UpdateThermalConfig (SiPolicy, &PchSetup, &SaSetup);
  UpdateUsbConfig (SiPolicy, SiPreMemPolicyPpi, &PchSetup);
  UpdatePchUsb2PhyConfig (SiPolicy, &PchSetup);
  UpdateUfsConfig (SiPolicy, &PchSetup);
  UpdateIshConfig (SiPolicy, &PchSetup);
  CorrectMismatchConfig (SiPolicy, &PchSetup);
  UpdateCnviConfig (SiPolicy, &CnvSetup);
  UpdateEspiConfig (SiPolicy, &PchSetup);
  UpdateHsioConfig (SiPolicy);
  UpdateRtcConfig (SiPolicy, &PchSetup);
#if FixedPcdGetBool (PcdEmbeddedEnable)
  UpdateFusaConfig (SiPolicy, &PchSetup, &SaSetup);
#endif

  return EFI_SUCCESS;
}
