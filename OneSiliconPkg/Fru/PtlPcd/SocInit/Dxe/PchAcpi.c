/** @file
  This is the driver that initializes the Intel PCH.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "PchInit.h"
#include <Protocol/PchPolicy.h>
#include <GpioDevConfig.h>
#include <ScsConfig.h>
#include <ScsInfoHob.h>
#include <Library/AslUpdateLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <GpioV2Functionalities.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PmcLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/SerialIoPrivateLib.h>
#include <Library/GbeLib.h>
#include <PchRstHob.h>
#include <Library/PchPcieRpLib.h>
#include <Library/CpuPlatformLib.h>
#include <TraceHubConfig.h>
#include <Library/PciSegmentLib.h>
#include <Register/PchRegs.h>
#include <Library/BaseLib.h>
#include <Register/UsbDeviceRegs.h>
#include <Library/PcdLib.h>
#include <IndustryStandard/Pci22.h>
#include <PcieRegs.h>
#include <Library/PchPciBdfLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Library/SataLib.h>
#include <Library/SataSocLib.h>
#include <TraceHubDataHob.h>
#include <Library/PcdInfoLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/GpioV2AccessLib.h>
#include <Register/GpioAcpiDefines.h>
#include <GpioV2Services.h>
#include <PcdSbPortIds.h>
#include <ThcConfigHob.h>

//
// Module variables
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_NVS_AREA_PROTOCOL    mPchNvsAreaProtocol;

/**
  Retrieve interrupt information about a PCH device from policy

  @param[in] UartNumber                 Uart number

  @retval PCH_DEVICE_INTERRUPT_CONFIG   structure with device's interrupt information
**/
PCH_DEVICE_INTERRUPT_CONFIG
STATIC
GetUartInterrupt (
  IN UINT8  UartNumber
  )
{
  PCH_DEVICE_INTERRUPT_CONFIG EmptyRecord;
  UINT8                       DevNum;
  UINT8                       FuncNum;
  UINT8                       Index;

  ZeroMem (&EmptyRecord, sizeof (PCH_DEVICE_INTERRUPT_CONFIG));
  DevNum  = LpssUartDevNumber (UartNumber);
  FuncNum = LpssUartFuncNumber (UartNumber);

  for (Index = 0; Index < mPchItssConfigHob->NumOfDevIntConfig; Index++) {
    if ((mPchItssConfigHob->DevIntConfig[Index].Device == DevNum) &&
        (mPchItssConfigHob->DevIntConfig[Index].Function == FuncNum)) {
      return mPchItssConfigHob->DevIntConfig[Index];
    }
  }
  return EmptyRecord;
}

/**
  Update ASL definitions for SerialIo devices.

**/
VOID
STATIC
UpdateLpssAcpiData (
  VOID
  )
{
  UINT8                 Index;
  VOID                  *HobPtr;
  LPSS_I2C_CONFIG_HOB   *LpssI2cConfigHob;

  HobPtr = GetFirstGuidHob (&gPchLpssI2cConfigHobGuid);
  if (HobPtr == NULL) {
    ASSERT (FALSE);
    return;
  }
  LpssI2cConfigHob = (LPSS_I2C_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr);

  for (Index = 0; Index < GetMaxSpiInterfacesNum (); Index++) {
    mPchNvsAreaProtocol.Area->SM0[Index] = mPchLpssSpiConfigHob->SpiDeviceConfig[Index].Mode;
    mPchNvsAreaProtocol.Area->SC0[Index] = LpssSpiPciCfgBase (Index);
  }
  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    mPchNvsAreaProtocol.Area->IM0[Index] = LpssI2cConfigHob->I2cDeviceConfig[Index].Mode;
    mPchNvsAreaProtocol.Area->IC0[Index] = LpssI2cPciCfgBase (Index);
  }
  for (Index = 0; Index < GetMaxUartInterfacesNum (); Index++) {
    mPchNvsAreaProtocol.Area->UM0[Index] = mPchLpssUartConfigHob->UartDeviceConfig[Index].Mode;
    mPchNvsAreaProtocol.Area->UC0[Index] = GetSerialIoUartPciCfg (Index);
    mPchNvsAreaProtocol.Area->UD0[Index] = mPchLpssUartConfigHob->UartDeviceConfig[Index].DmaEnable;
    mPchNvsAreaProtocol.Area->UP0[Index] = mPchLpssUartConfigHob->UartDeviceConfig[Index].PowerGating;
    mPchNvsAreaProtocol.Area->UI0[Index] = (GetUartInterrupt (Index)).Irq;
  }
}

/**
  Update ASL definitions for Touch Host Controller devices.

**/
VOID
STATIC
UpdateThcAcpiData (
  VOID
  )
{
  GPIOV2_SERVICES           *GpioServices;
  UINT8                     Index;
  GPIOV2_PAD                GpioPad;
  EFI_STATUS                Status;
  VOID                      *HobPtr;
  THC_CONFIG_HOB            *ThcConfigHob;
  UINT8                     MultiSpiMode;


  HobPtr = GetFirstGuidHob (&gPchThcConfigHobGuid);
  ASSERT (HobPtr != NULL);
  ThcConfigHob = (THC_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr);

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "THC: Failed to get access to the Gpio Protocol - UpdateThcAcpiData()\n"));
    return;
  }

  for (Index = 0; Index < GetPchMaxThcCount (); Index++) {
    mPchNvsAreaProtocol.Area->ThcWakOnTouchInterrupt[Index]    = 0;
    mPchNvsAreaProtocol.Area->ThcMode[Index]                   = ThcConfigHob->ThcPort[Index].Mode;
    mPchNvsAreaProtocol.Area->ThcActiveLtr[Index]              = ThcConfigHob->ThcPort[Index].ActiveLtr;
    mPchNvsAreaProtocol.Area->ThcIdleLtr[Index]                = ThcConfigHob->ThcPort[Index].IdleLtr;
    mPchNvsAreaProtocol.Area->ThcPerformanceLimitation[Index]  = ThcConfigHob->ThcPort[Index].PerformanceLimitation;
    mPchNvsAreaProtocol.Area->ThcDisplayFrameSyncPeriod[Index] = ThcConfigHob->ThcPort[Index].DisplayFrameSyncPeriod;

    mPchNvsAreaProtocol.Area->ThcResetPad[Index]              = ThcConfigHob->ThcPort[Index].Reset.ResetPad;
    mPchNvsAreaProtocol.Area->ThcResetPadTrigger[Index]       = (UINT8) ThcConfigHob->ThcPort[Index].Reset.ResetPadTrigger;
    mPchNvsAreaProtocol.Area->ThcResetSequencingDelay[Index]  = ThcConfigHob->ThcPort[Index].Reset.ResetSequencingDelay;
    if (ThcConfigHob->ThcPort[Index].Reset.ResetPad == 0x0) {
      // Use HW default
      Status = PtlPcdGpioGetThcSpiResetPin (GpioServices, Index, &GpioPad);
      if (!EFI_ERROR (Status)) {
        mPchNvsAreaProtocol.Area->ThcResetPad[Index] = GpioPad;
      }
    } else {
      // Use Pad provided by the policy
      mPchNvsAreaProtocol.Area->ThcResetPad[Index] = ThcConfigHob->ThcPort[Index].Reset.ResetPad;
    }
    DEBUG ((DEBUG_INFO, "UpdateThcAcpiData() THC %d HidOverSpi.ResetPad: %X \n", Index, mPchNvsAreaProtocol.Area->ThcResetPad[Index]));

    mPchNvsAreaProtocol.Area->ThcHidSpiInputReportBodyAddress[Index]   = ThcConfigHob->ThcPort[Index].HidOverSpi.InputReportBodyAddress;
    mPchNvsAreaProtocol.Area->ThcHidSpiInputReportHeaderAddress[Index] = ThcConfigHob->ThcPort[Index].HidOverSpi.InputReportHeaderAddress;
    mPchNvsAreaProtocol.Area->ThcHidSpiOutputReportAddress[Index]      = ThcConfigHob->ThcPort[Index].HidOverSpi.OutputReportAddress;
    mPchNvsAreaProtocol.Area->ThcHidSpiReadOpcode[Index]               = ThcConfigHob->ThcPort[Index].HidOverSpi.ReadOpcode;
    mPchNvsAreaProtocol.Area->ThcHidSpiWriteOpcode[Index]              = ThcConfigHob->ThcPort[Index].HidOverSpi.WriteOpcode;
    mPchNvsAreaProtocol.Area->ThcHidSpiConnectionSpeed[Index]          = ThcConfigHob->ThcPort[Index].HidOverSpi.Frequency;
    // Extract MultiSpiMode by masking and shifting the Flags field
    MultiSpiMode                                                       = (ThcConfigHob->ThcPort[Index].HidOverSpi.Flags & (BIT14|BIT15)) >> 14;
    // Check if MultiSpiMode is 0b01 - Dual SPI Mode or 0b10 - Quad SPI Mode
    if(MultiSpiMode == 1 || MultiSpiMode == 2) {
      // Set the MULTI_SPI_MODE_ENABLE bit in the Flags field
      ThcConfigHob->ThcPort[Index].HidOverSpi.Flags |= BIT13;
    }
    mPchNvsAreaProtocol.Area->ThcHidSpiFlags[Index]                    = ThcConfigHob->ThcPort[Index].HidOverSpi.Flags;
    mPchNvsAreaProtocol.Area->ThcHidSpiLimitPacketSize[Index]          = ThcConfigHob->ThcPort[Index].HidOverSpi.LimitPacketSize;

    mPchNvsAreaProtocol.Area->ThcHidI2cDeviceAddress[Index]                                      = (UINT16) ThcConfigHob->ThcPort[Index].HidOverI2c.DeviceAddress;
    mPchNvsAreaProtocol.Area->ThcHidI2cConnectionSpeed[Index]                                    = ThcConfigHob->ThcPort[Index].HidOverI2c.ConnectionSpeed;
    mPchNvsAreaProtocol.Area->ThcHidI2cAddressingMode[Index]                                     = (UINT8) ThcConfigHob->ThcPort[Index].HidOverI2c.AddressingMode;
    mPchNvsAreaProtocol.Area->ThcHidI2cDeviceDescriptorAddress[Index]                            = (UINT16) ThcConfigHob->ThcPort[Index].HidOverI2c.DeviceDescriptorAddress;
    mPchNvsAreaProtocol.Area->ThcHidI2cStandardModeSerialClockLineHighPeriod[Index]              = ThcConfigHob->ThcPort[Index].HidOverI2c.StandardModeSerialClockLineHighPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cStandardModeSerialClockLineLowPeriod[Index]               = ThcConfigHob->ThcPort[Index].HidOverI2c.StandardModeSerialClockLineLowPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cStandardModeSerialDataLineTransmitHoldPeriod[Index]       = ThcConfigHob->ThcPort[Index].HidOverI2c.StandardModeSerialDataLineTransmitHoldPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cStandardModeSerialDataLineReceiveHoldPeriod[Index]        = ThcConfigHob->ThcPort[Index].HidOverI2c.StandardModeSerialDataLineReceiveHoldPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cFastModeSerialClockLineHighPeriod[Index]                  = ThcConfigHob->ThcPort[Index].HidOverI2c.FastModeSerialClockLineHighPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cFastModeSerialClockLineLowPeriod[Index]                   = ThcConfigHob->ThcPort[Index].HidOverI2c.FastModeSerialClockLineLowPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cFastModeSerialDataLineTransmitHoldPeriod[Index]           = ThcConfigHob->ThcPort[Index].HidOverI2c.FastModeSerialDataLineTransmitHoldPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cFastModeSerialDataLineReceiveHoldPeriod[Index]            = ThcConfigHob->ThcPort[Index].HidOverI2c.FastModeSerialDataLineReceiveHoldPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cMaxSuppressedSpikesSMFMFMP[Index]                         = ThcConfigHob->ThcPort[Index].HidOverI2c.MaxSuppressedSpikesSMFMFMP;
    mPchNvsAreaProtocol.Area->ThcHidI2cFastModePlusSerialClockLineHighPeriod[Index]              = ThcConfigHob->ThcPort[Index].HidOverI2c.FastModePlusSerialClockLineHighPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cFastModePlusSerialClockLineLowPeriod[Index]               = ThcConfigHob->ThcPort[Index].HidOverI2c.FastModePlusSerialClockLineLowPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cFastModePlusSerialDataLineTransmitHoldPeriod[Index]       = ThcConfigHob->ThcPort[Index].HidOverI2c.FastModePlusSerialDataLineTransmitHoldPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cFastModePlusSerialDataLineReceiveHoldPeriod[Index]        = ThcConfigHob->ThcPort[Index].HidOverI2c.FastModePlusSerialDataLineReceiveHoldPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cHighSpeedModePlusSerialClockLineHighPeriod[Index]         = ThcConfigHob->ThcPort[Index].HidOverI2c.HighSpeedModePlusSerialClockLineHighPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cHighSpeedModePlusSerialClockLineLowPeriod[Index]          = ThcConfigHob->ThcPort[Index].HidOverI2c.HighSpeedModePlusSerialClockLineLowPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cHighSpeedModePlusSerialDataLineTransmitHoldPeriod[Index]  = ThcConfigHob->ThcPort[Index].HidOverI2c.HighSpeedModePlusSerialDataLineTransmitHoldPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cHighSpeedModePlusSerialDataLineReceiveHoldPeriod[Index]   = ThcConfigHob->ThcPort[Index].HidOverI2c.HighSpeedModePlusSerialDataLineReceiveHoldPeriod;
    mPchNvsAreaProtocol.Area->ThcHidI2cMaximumLengthOfSuppressedSpikesInHighSpeedMode[Index]     = ThcConfigHob->ThcPort[Index].HidOverI2c.MaximumLengthOfSuppressedSpikesInHighSpeedMode;

    if (ThcConfigHob->ThcPort[Index].WakeOnTouch == TRUE) {
      DEBUG ((DEBUG_INFO, "UpdateThcAcpiData() THC %d WakeOnTouch Pad: %X \n", Index, PtlPcdGpioGetThcWotGpioPad(Index)));
      mPchNvsAreaProtocol.Area->ThcWakOnTouchInterrupt[Index] = PtlPcdGpioGetThcWotGpioPad(Index);
      mPchNvsAreaProtocol.Area->ThcWakeIntEdgeLevel[Index]    = ThcConfigHob->ThcPort[Index].WotPinConfig.EdgeLevel;
      mPchNvsAreaProtocol.Area->ThcWakeIntActiveLevel[Index]  = ThcConfigHob->ThcPort[Index].WotPinConfig.ActiveLevel;
      mPchNvsAreaProtocol.Area->ThcWakeIntPinConfig[Index]    = ThcConfigHob->ThcPort[Index].WotPinConfig.PinConfig;
    }
  }
}

/**
  Update ASL definitions for SCS.
  @retval EFI_SUCCESS         The function completed successfully
**/
VOID
UpdateScsAcpiData (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS HobPtr;
  SCS_INFO_HOB         *ScsInfoHob;

  // Get SCS Info HOB.
  HobPtr.Guid = GetFirstGuidHob (&gScsInfoHobGuid);
  if (HobPtr.Guid != NULL) {
    ScsInfoHob = (SCS_INFO_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);
    mPchNvsAreaProtocol.Area->Ufs0Enabled        = (UINT8)ScsInfoHob->UfsInfo[0].Supported;
    mPchNvsAreaProtocol.Area->Ufs1Enabled = (UINT8)ScsInfoHob->UfsInfo[1].Supported;
    DEBUG((DEBUG_INFO, "UpdateScsAcpiData() Ufs0 %d Ufs1 %d \n", mPchNvsAreaProtocol.Area->Ufs0Enabled, mPchNvsAreaProtocol.Area->Ufs1Enabled));
  }
}
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
/**
  Update NVS Area for Timed GPIO devices.
**/
VOID
UpdateTimedGpioSetup (
  VOID
  )
{
  mPchNvsAreaProtocol.Area->EnableTimedGpio0 = (UINT8)mPchPmcConfigHob->EnableTimedGpio0;
  mPchNvsAreaProtocol.Area->EnableTimedGpio1 = (UINT8)mPchPmcConfigHob->EnableTimedGpio1;
}
#endif

/**
  Update NVS Area after RST PCIe Storage Remapping and before Boot

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND           Couldn't fetch RstHob
**/
EFI_STATUS
PchUpdateNvsAreaAfterRemapping (
  VOID
  )
{
  UINTN                 Index;
  VOID                  *Hob;
  PCH_RST_HOB           *RstHob;

  Hob = GetFirstGuidHob (&gPchRstHobGuid);
  if (Hob == NULL) {
    return EFI_NOT_FOUND;
  }

  RstHob = (PCH_RST_HOB *) GET_GUID_HOB_DATA (Hob);

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    mPchNvsAreaProtocol.Area->RstPcieStorageInterfaceType[Index]        = RstHob->RstCrConfiguration[Index].DeviceInterface;
    mPchNvsAreaProtocol.Area->RstPcieStoragePmCapPtr[Index]             = RstHob->SavedRemapedDeviceConfigSpace[Index].PmCapPtr;
    mPchNvsAreaProtocol.Area->RstPcieStoragePcieCapPtr[Index]           = RstHob->SavedRemapedDeviceConfigSpace[Index].PcieCapPtr;
    mPchNvsAreaProtocol.Area->RstPcieStorageL1ssCapPtr[Index]           = RstHob->SavedRemapedDeviceConfigSpace[Index].L1ssCapPtr;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpL1ssControl2[Index]       = RstHob->SavedRemapedDeviceConfigSpace[Index].EndpointL1ssControl2;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpL1ssControl1[Index]       = RstHob->SavedRemapedDeviceConfigSpace[Index].EndpointL1ssControl1;
    mPchNvsAreaProtocol.Area->RstPcieStorageLtrCapPtr[Index]            = RstHob->SavedRemapedDeviceConfigSpace[Index].LtrCapPtr;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpLtrData[Index]            = RstHob->SavedRemapedDeviceConfigSpace[Index].EndpointLtrData;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpLctlData16[Index]         = RstHob->SavedRemapedDeviceConfigSpace[Index].EndpointLctlData16;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpDctlData16[Index]         = RstHob->SavedRemapedDeviceConfigSpace[Index].EndpointDctlData16;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpDctl2Data16[Index]        = RstHob->SavedRemapedDeviceConfigSpace[Index].EndpointDctl2Data16;
    mPchNvsAreaProtocol.Area->RstPcieStorageRpDctl2Data16[Index]        = RstHob->SavedRemapedDeviceConfigSpace[Index].RootPortDctl2Data16;
    mPchNvsAreaProtocol.Area->RstPcieStorageUniqueTableBar[Index]       = RstHob->RstCrConfiguration[Index].EndPointUniqueMsixTableBar;
    mPchNvsAreaProtocol.Area->RstPcieStorageUniqueTableBarValue[Index]  = RstHob->RstCrConfiguration[Index].EndPointUniqueMsixTableBarValue;
    mPchNvsAreaProtocol.Area->RstPcieStorageUniquePbaBar[Index]         = RstHob->RstCrConfiguration[Index].EndPointUniqueMsixPbaBar;
    mPchNvsAreaProtocol.Area->RstPcieStorageUniquePbaBarValue[Index]    = RstHob->RstCrConfiguration[Index].EndPointUniqueMsixPbaBarValue;
    mPchNvsAreaProtocol.Area->RstPcieStorageRootPortNum[Index]          = RstHob->RstCrConfiguration[Index].RootPortNum;
  }
  return EFI_SUCCESS;
}

/**
  PCH ACPI initialization before Boot Sript Table is closed
  It update ACPI table and ACPI NVS area.

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
PchAcpiOnEndOfDxe (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  DEBUG ((DEBUG_INFO, "PchAcpiOnEndOfDxe() Start\n"));

  ///
  /// Closed the event to avoid call twice when launch shell
  ///
  gBS->CloseEvent (Event);

  //
  // Init HDA Audio ACPI tables
  //
  PchHdAudioAcpiInit ();
  //
  // Update ASL definitions for SerialIo devices.
  //
  UpdateLpssAcpiData ();
  UpdateCnviAcpiData ();
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  UpdateTimedGpioSetup();
#endif

  UpdateThcAcpiData ();
  UpdateScsAcpiData ();
  //
  // Update Pch Nvs Area
  //
  PchUpdateNvsArea ();

  //
  // Patch PchNvsArea Address
  //
  PatchPchNvsAreaAddress ();

  DEBUG ((DEBUG_INFO, "PchAcpiOnEndOfDxe() End\n"));

  return;
}

/**
  Initialize Pch acpi
  @param[in] ImageHandle          Handle for the image of this driver

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchAcpiInit (
  IN EFI_HANDLE         ImageHandle
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   EndOfDxeEvent;

  DEBUG ((DEBUG_INFO, "Install PCH NVS protocol\n"));

  Status = (gBS->AllocatePool) (EfiACPIMemoryNVS, sizeof (PCH_NVS_AREA), (VOID **) &mPchNvsAreaProtocol.Area);
  ASSERT_EFI_ERROR (Status);

  ZeroMem ((VOID *) mPchNvsAreaProtocol.Area, sizeof (PCH_NVS_AREA));
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gPchNvsAreaProtocolGuid,
                  &mPchNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Update the NVS Area after RST PCIe Storage Remapping
  ///
  PchUpdateNvsAreaAfterRemapping ();

  //
  // Register an end of DXE event for PCH ACPI to do tasks before invoking any UEFI drivers,
  // applications, or connecting consoles,...
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  PchAcpiOnEndOfDxe,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Updates eMMC related fields in NVS area.

  @param[in] PchPolicy  Pointer to the PCH_POLICY_PROTOCOL
**/
STATIC
VOID
EmmcUpdateNvsArea (
  IN PCH_POLICY_PROTOCOL  *PchPolicy
  )
{
  SCS_EMMC_DXE_CONFIG  *EmmcDxeConfig;
  EFI_STATUS           Status;

  Status = GetConfigBlock ((VOID*)PchPolicy, &gEmmcDxeConfigGuid, (VOID*)&EmmcDxeConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Update eMMC HS400 mode enablement
  //
  mPchNvsAreaProtocol.Area->EMH4 = 0;
  mPchNvsAreaProtocol.Area->EmmcEnabled = 0;

  //
  // Update eMMC Driver Strength
  // Per eMMC 5.01 JEDEC Specification (JESD84-B50.1, Table 186)
  // Nominal Impedance - Driver Type Values:
  // 50 Ohm              0x0
  // 33 Ohm              0x1
  // 40 Ohm              0x4
  //
  switch (EmmcDxeConfig->DriverStrength) {
    case DriverStrength33Ohm:
      mPchNvsAreaProtocol.Area->EMDS = 0x1;
      break;
    case DriverStrength40Ohm:
      mPchNvsAreaProtocol.Area->EMDS = 0x4;
      break;
    case DriverStrength50Ohm:
    default:
      mPchNvsAreaProtocol.Area->EMDS = 0x0;
  }
}

/**
  Updates SD card related fields in NVS area.

  @param[in] PchPolicy  Pointer to the PCH_POLICY_PROTOCOL
**/
STATIC
VOID
SdCardUpdateNvsArea (
  VOID
  )
{
  mPchNvsAreaProtocol.Area->SdCardEnabled = 0;
}

/**
  Update NVS area for PCIe root ports.
**/
STATIC
VOID
PcieRpUpdateNvsArea (
  VOID
  )
{
  UINT32          Index;

  for (Index = 0; Index < PCH_MAX_PCIE_CLOCKS; Index++) {
    mPchNvsAreaProtocol.Area->ClockToRootPortMap[Index] = mPchPcieRpConfigHob->PcieClock[Index].Usage;
  }
  mPchNvsAreaProtocol.Area->ClkreqIpcCmdSupported = (UINT8)mPchPcieRpConfigHob->ClkreqIpcCmdSupport;
}

/**
  Updates USB related fields in NVS area.
**/
STATIC
VOID
UsbUpdateNvsArea (
  VOID
  )
{
  UINT64                  XdciPciBase;
  UINT64                  XdciBar;
  UINT8                   ClearXdciBar = FALSE;
  UINT16                  PciMemConfig;
  EFI_PEI_HOB_POINTERS    HobPtr;
  PCH_GENERAL_CONFIG_HOB  *GeneralConfigHob;

  //
  // Saving GCTL value into PCH NVS area
  //
  XdciPciBase = PchXdciPciCfgBase ();

  //
  // Determine Base address for Base address register (Offset 0x10)
  //
  if (PciSegmentRead32 (XdciPciBase) != 0xFFFFFFFF) {
    XdciBar = PciSegmentRead32 (XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;

    if ((PciSegmentRead32 (XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET) & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
      XdciBar += (UINT64) PciSegmentRead32 (XdciPciBase + (PCI_BASE_ADDRESSREG_OFFSET + 4)) << 32;
    }

    if (XdciBar == 0x0) {
      ClearXdciBar = TRUE;
      PciSegmentWrite32 ((XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET), PcdGet32 (PcdSiliconInitTempMemBaseAddr));
      XdciBar = PciSegmentRead32 (XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;

      if ((PciSegmentRead32 (XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET) & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
        XdciBar += (UINT64) PciSegmentRead32 (XdciPciBase + (PCI_BASE_ADDRESSREG_OFFSET + 4)) << 32;
      }
    }

    //
    // Enable Pci Memconfig to read the correct value for GCTL register
    //
    PciMemConfig = PciSegmentRead16 (XdciPciBase + PCI_COMMAND_OFFSET);
    PciSegmentWrite16 (XdciPciBase + PCI_COMMAND_OFFSET, PciMemConfig | (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));

    mPchNvsAreaProtocol.Area->PchxDCIPwrDnScale = MmioRead32(XdciBar + R_XDCI_MEM_GCTL);
    DEBUG ((DEBUG_INFO, "PchxDCIPwrDnScale  0x%x\n", (UINT64)mPchNvsAreaProtocol.Area->PchxDCIPwrDnScale));
    //
    // Disable Pci Memconfig & clear Base address
    //
    PciSegmentWrite16(XdciPciBase + PCI_COMMAND_OFFSET, PciMemConfig);

    if (ClearXdciBar == TRUE) {
      PciSegmentWrite32 ((XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET), 0x0);
      if ((PciSegmentRead32 (XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET) & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
        PciSegmentWrite32 (XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET + 4, 0x0);
      }
    }
  }

  //
  // Update PCH USB ports count
  //
  mPchNvsAreaProtocol.Area->PchUsb2PortCount = (UINT8) PtlPcdGetUsb2MaxPhysicalPortNum ();
  mPchNvsAreaProtocol.Area->PchUsb3PortCount = (UINT8) PtlPcdGetXhciMaxUsb3PortNum ();

  //
  // Update USB Audio Offload enable status
  //
  HobPtr.Guid = GetFirstGuidHob (&gPchGeneralConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  GeneralConfigHob = (PCH_GENERAL_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);
  mPchNvsAreaProtocol.Area->UaolEnabled = (UINT8) GeneralConfigHob->UaolEnable;
}

/**
  Update ASL definitions for Gpio Group to GPE mapping devices.

  @retval EFI_SUCCESS         The function completed successfully
**/
EFI_STATUS
PtlPcdGpioAcpiInit (
  VOID
  )
{
  EFI_STATUS            Status;
  GPIOV2_SERVICES       *GpioServices;
  GPIOV2_PAD_GROUP      GroupToGpeMapped[3];
  UINT32                GlobalGroupIndex[3];
  UINT32                Index;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2][%a] ERROR getting GpioServices %a\n", __FUNCTION__, GPIO_HID_PTL_PCD_P));
    return Status;
  }

  GpioServices->GetGroupToGpeMapping (
    GpioServices,
    &GroupToGpeMapped[0],
    &GroupToGpeMapped[1],
    &GroupToGpeMapped[2]
  );

  for (Index = 0; Index < ARRAY_SIZE (GroupToGpeMapped); Index++) {
    GpioServices->GetGlobalGroupIndex (
      GpioServices,
      GroupToGpeMapped[Index],
      &GlobalGroupIndex[Index]
    );
  }

  DEBUG ((DEBUG_INFO, "[%a] GlobalGroupIndex[0]: 0x%X, GlobalGroupIndex[1]: 0x%X, GlobalGroupIndex[2]: 0x%X\n", __FUNCTION__, GlobalGroupIndex[0], GlobalGroupIndex[1], GlobalGroupIndex[2]));


  //
  // GEI0/1/2 and GED0/1/2 are objects for informing how GPIO groups are mapped to GPE0.
  // If Group is mapped to 1-Tier GPE information is also stored on what Group DW
  // is mapped to GPE_DWx. Because GPE_DWx register is 32 bits large if groups have more than
  // 32 pads only part of it can be mapped.
  //
  //  GEIx - GroupIndex mapped to GPE0_DWx
  //  GEDx - DoubleWorld part of Group: 0 - pins 31-0, 1 - pins 63-32, ...
  //
  mPchNvsAreaProtocol.Area->GEI0 = (UINT8) GlobalGroupIndex[0];
  mPchNvsAreaProtocol.Area->GEI1 = (UINT8) GlobalGroupIndex[1];
  mPchNvsAreaProtocol.Area->GEI2 = (UINT8) GlobalGroupIndex[2];
  mPchNvsAreaProtocol.Area->GED0 = (UINT8) 0;
  mPchNvsAreaProtocol.Area->GED1 = (UINT8) 0;
  mPchNvsAreaProtocol.Area->GED2 = (UINT8) 0;

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Update ASL object before Boot

  @retval EFI_STATUS
  @retval EFI_NOT_READY         The Acpi protocols are not ready.
**/
EFI_STATUS
PchUpdateNvsArea (
  VOID
  )
{
  EFI_STATUS                 Status;
  UINTN                      Index;
  UINTN                      HpetBaseAdress;
  UINTN                      RpDev;
  UINTN                      RpFun;
  UINT32                     Data32;
  PCH_POLICY_PROTOCOL        *PchPolicy;
  GPIO_DXE_CONFIG            *GpioDxeConfig;
  UINT16                     TcoBase;
  TRACEHUB_DATA_HOB          *TraceHubDataHob;
  P2SB_PORT_16_ID            P2SBPid;
  PCH_PCIE_ROOT_PORT_CONFIG  *RootPort;

  ///
  /// Get PCH Policy Protocol
  ///
  Status = gBS->LocateProtocol (&gPchPolicyProtocolGuid, NULL, (VOID **)&PchPolicy);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Get GPIO DXE Config Block
  ///
  Status = GetConfigBlock ((VOID *)PchPolicy, &gGpioDxeConfigGuid, (VOID *)&GpioDxeConfig);
  ASSERT_EFI_ERROR (Status);

  TraceHubDataHob = (TRACEHUB_DATA_HOB *) GetFirstGuidHob (&gTraceHubDataHobGuid);

  //
  // Update ASL PCIE port address according to root port device and function
  //
  RootPort = mPchPcieRpConfigHob->RootPort;
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    RpDev = PchPcieRpDevNumber (Index);
    RpFun = PchPcieRpFuncNumber (Index);
    Data32 = ((UINT8) RpDev << 16) | (UINT8) RpFun;
    mPchNvsAreaProtocol.Area->RpAddress[Index] = Data32;

    //
    // Update Maximum Snoop Latency and Maximum No-Snoop Latency values for PCIE
    //
    mPchNvsAreaProtocol.Area->PcieLtrMaxSnoopLatency[Index]   = RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency;
    mPchNvsAreaProtocol.Area->PcieLtrMaxNoSnoopLatency[Index] = RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency;
  }

  //
  // Update PCHS.
  //
  mPchNvsAreaProtocol.Area->PchSeries     = PchSeries ();
  //
  // Update PCHG.
  //
  mPchNvsAreaProtocol.Area->PchGeneration = (UINT16) PchGeneration ();
  //
  // Update PSTP.
  //
  mPchNvsAreaProtocol.Area->PchStepping = (UINT16) PchStepping ();
  //
  // Update HPET base address.
  //
  PchHpetBaseGet (&HpetBaseAdress);
  mPchNvsAreaProtocol.Area->HPTE          = TRUE;  // @todo remove the NVS, since it's always enabled.
  mPchNvsAreaProtocol.Area->HPTB          = (UINT32) HpetBaseAdress;
  //
  // Update SBREG_BAR.
  //
  mPchNvsAreaProtocol.Area->SBRG          = PCH_PCR_BASE_ADDRESS;

  //
  // Update base address
  //
  mPchNvsAreaProtocol.Area->PMBS = PmcGetAcpiBase ();
  mPchNvsAreaProtocol.Area->PWRM = PmcGetPwrmBase ();
  PchTcoBaseGet (&TcoBase);
  mPchNvsAreaProtocol.Area->TcoBase = TcoBase;

  //
  // Update PCH PID info
  //
  P2SBPid.Pid16bit = PTL_SID_F2_PID_ICLK;
  mPchNvsAreaProtocol.Area->IclkPid = P2SBPid.PortId.LocalPid;

  //
  // Update GPIO device ACPI variables
  //
  mPchNvsAreaProtocol.Area->SGIR = mPchItssConfigHob->GpioIrqRoute;
  mPchNvsAreaProtocol.Area->GPHD = (UINT8)GpioDxeConfig->HideGpioAcpiDevice;

  //
  // Update GPP_X to GPE_DWX mapping.
  //
  PtlPcdGpioAcpiInit ();

  //
  // SCS Configuration
  //
  EmmcUpdateNvsArea (PchPolicy);
  SdCardUpdateNvsArea ();

  PcieRpUpdateNvsArea ();

  //
  // SATA configuration.
  //
  mPchNvsAreaProtocol.Area->SataPortPresence = 0;
  DEBUG ((DEBUG_INFO, "SataPortPresence: 0x%x\n", mPchNvsAreaProtocol.Area->SataPortPresence));

  //
  // CPU SKU
  //
  mPchNvsAreaProtocol.Area->CpuSku = GetCpuSku ();
  mPchNvsAreaProtocol.Area->PsOnEnable            = (UINT8)mPchPmcConfigHob->PsOnEnable;
  mPchNvsAreaProtocol.Area->DtrSciEnable             = 0;

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    mPchNvsAreaProtocol.Area->LtrEnable[Index]  = (UINT8)RootPort[Index].PcieRpCommonConfig.LtrEnable;
  }

  mPchNvsAreaProtocol.Area->GBES = IsGbePresent (PmcGetPwrmBase ());

  //
  // Update TraceHub TraceHubDataHob
  //
  if (TraceHubDataHob != NULL) {
    mPchNvsAreaProtocol.Area->TraceVerbosity = TraceHubDataHob->TraceVerbosity;
    mPchNvsAreaProtocol.Area->TraceHubFwBase = TraceHubDataHob->TraceHubFwBase;
    DEBUG ((DEBUG_INFO, "TraceVerbosity: 0x%x\n", mPchNvsAreaProtocol.Area->TraceVerbosity));
    DEBUG ((DEBUG_INFO, "TraceHubFwBase: 0x%x\n", mPchNvsAreaProtocol.Area->TraceHubFwBase));
  } else {
    DEBUG ((DEBUG_ERROR, "TraceHubDataHob fails to pass to Pch Gnvs\n"));
  }

  UsbUpdateNvsArea ();

  return Status;
}

/**
  Initialize PCH Nvs Area opeartion region.

  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
PatchPchNvsAreaAddress (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINT32                                Address;
  UINT16                                Length;

  Address = (UINT32) (UINTN) mPchNvsAreaProtocol.Area;
  Length  = (UINT16) sizeof (PCH_NVS_AREA);
  DEBUG ((DEBUG_INFO, "PatchPchNvsAreaAddress: PCH NVS Address %x Length %x\n", Address, Length));
  Status  = UpdateNameAslCode (SIGNATURE_32 ('P','N','V','B'), &Address, sizeof (Address));
  ASSERT_EFI_ERROR (Status);
  Status  = UpdateNameAslCode (SIGNATURE_32 ('P','N','V','L'), &Length, sizeof (Length));
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
