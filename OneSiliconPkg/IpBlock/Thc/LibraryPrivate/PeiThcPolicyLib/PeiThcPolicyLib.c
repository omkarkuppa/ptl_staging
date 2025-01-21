/** @file
  This file provides services for Thc policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/ConfigBlockLib.h>
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Ppi/SiPolicy.h>
#include <ThcConfig.h>
#include <AcpiGpio.h>
#include <ConfigBlock/SiConfig.h>

/**
  Print THC_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
ThcPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS    Status;
  THC_CONFIG    *ThcConfig;
  UINT8         Index;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gThcConfigGuid, (VOID *) &ThcConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ PCH Thc Config ------------------------\n"));
  DEBUG ((DEBUG_INFO, " THC ports: \n"));
  for (Index = 0; Index < GetPchMaxThcCount (); Index ++) {
    DEBUG ((DEBUG_INFO, " PORT%d Assignment: 0x%X\n", Index, ThcConfig->ThcPort[Index].Assignment));
    DEBUG ((DEBUG_INFO, " PORT%d Mode <0:Thc 1:HidOverSpi 2: HidOverI2C> Current: %d\n", Index, ThcConfig->ThcPort[Index].Mode));
    DEBUG ((DEBUG_INFO, " PORT%d Customized SubSystem ID: 0x%X\n", Index, ThcConfig->ThcPort[Index].ThcSubSystemId.CustomizedSsid));
    DEBUG ((DEBUG_INFO, " PORT%d Customized SubSystem Vendor ID: 0x%X\n", Index, ThcConfig->ThcPort[Index].ThcSubSystemId.CustomizedSvid));

    DEBUG ((DEBUG_INFO, " PORT%d WakeOnTouch: 0x%X\n", Index, ThcConfig->ThcPort[Index].WakeOnTouch));
    DEBUG ((DEBUG_INFO, " PORT%d EdgeLevel 0x%X\n", Index, ThcConfig->ThcPort[Index].WotPinConfig.EdgeLevel));
    DEBUG ((DEBUG_INFO, " PORT%d ActiveLevel 0x%X\n", Index, ThcConfig->ThcPort[Index].WotPinConfig.ActiveLevel));
    DEBUG ((DEBUG_INFO, " PORT%d PinConfig 0x%X\n", Index, ThcConfig->ThcPort[Index].WotPinConfig.PinConfig));

    DEBUG ((DEBUG_INFO, " PORT%d Interrupt Pin Muxing: %d\n", Index, ThcConfig->ThcPort[Index].InterruptPinMuxing));
    DEBUG ((DEBUG_INFO, " PORT%d Active Ltr: 0x%X\n", Index, ThcConfig->ThcPort[Index].ActiveLtr));
    DEBUG ((DEBUG_INFO, " PORT%d Idle Ltr: 0x%X\n", Index, ThcConfig->ThcPort[Index].IdleLtr));
    DEBUG ((DEBUG_INFO, " PORT%d PerformanceLimitation: 0x%X\n", Index, ThcConfig->ThcPort[Index].PerformanceLimitation));
    DEBUG ((DEBUG_INFO, " PORT%d TimestampTimerMode 0x%X\n", Index, ThcConfig->ThcPort[Index].TimestampTimerMode));
    DEBUG ((DEBUG_INFO, " PORT%d DisplayFrameSyncPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].DisplayFrameSyncPeriod));

    DEBUG ((DEBUG_INFO, " PORT%d ResetPad: 0x%X\n", Index, ThcConfig->ThcPort[Index].Reset.ResetPad));
    DEBUG ((DEBUG_INFO, " PORT%d ResetPadTrigger: 0x%X\n", Index, ThcConfig->ThcPort[Index].Reset.ResetPadTrigger));
    DEBUG ((DEBUG_INFO, " PORT%d ResetSequencingDelay= 0x%X\n", Index, ThcConfig->ThcPort[Index].Reset.ResetSequencingDelay));

    DEBUG ((DEBUG_INFO, " PORT%d ThcDsyncPadEnable: 0x%X\n", Index, ThcConfig->ThcPort[Index].ThcDsyncPadEnable));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverSpi ConnectionSpeed (Frequency): 0x%d\n", Index, ThcConfig->ThcPort[Index].HidOverSpi.Frequency));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverSpi InputReportHeaderAddress: 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverSpi.InputReportHeaderAddress));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverSpi InputReportBodyAddress: 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverSpi.InputReportBodyAddress));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverSpi OutputReportAddress: 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverSpi.OutputReportAddress));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverSpi ReadOpcode: 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverSpi.ReadOpcode));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverSpi WriteOpcode: 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverSpi.WriteOpcode));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverSpi Flags: 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverSpi.Flags));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverSpi LimitPacketSize: 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverSpi.LimitPacketSize));

    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c DeviceAddress 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.DeviceAddress));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c ConnectionSpeed 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.ConnectionSpeed));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c AddressingMode 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.AddressingMode));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c DeviceDescriptorAddress 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.DeviceDescriptorAddress));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c StandardModeSerialClockLineHighPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.StandardModeSerialClockLineHighPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c StandardModeSerialClockLineLowPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.StandardModeSerialClockLineLowPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c StandardModeSerialDataLineTransmitHoldPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.StandardModeSerialDataLineTransmitHoldPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c StandardModeSerialDataLineReceiveHoldPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.StandardModeSerialDataLineReceiveHoldPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c FastModeSerialClockLineHighPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.FastModeSerialClockLineHighPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c FastModeSerialClockLineLowPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.FastModeSerialClockLineLowPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c FastModeSerialDataLineTransmitHoldPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.FastModeSerialDataLineTransmitHoldPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c FastModeSerialDataLineReceiveHoldPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.FastModeSerialDataLineReceiveHoldPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c MaxSuppressedSpikesSMFMFMP 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.MaxSuppressedSpikesSMFMFMP));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c FastModePlusSerialClockLineHighPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.FastModePlusSerialClockLineHighPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c FastModePlusSerialClockLineLowPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.FastModePlusSerialClockLineLowPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c FastModePlusSerialDataLineTransmitHoldPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.FastModePlusSerialDataLineTransmitHoldPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c FastModePlusSerialDataLineReceiveHoldPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.FastModePlusSerialDataLineReceiveHoldPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c HighSpeedModePlusSerialClockLineHighPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.HighSpeedModePlusSerialClockLineHighPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c HighSpeedModePlusSerialClockLineLowPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.HighSpeedModePlusSerialClockLineLowPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c HighSpeedModePlusSerialDataLineTransmitHoldPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.HighSpeedModePlusSerialDataLineTransmitHoldPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c HighSpeedModePlusSerialDataLineReceiveHoldPeriod 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.HighSpeedModePlusSerialDataLineReceiveHoldPeriod));
    DEBUG ((DEBUG_INFO, " PORT%d HidOverI2c MaximumLengthOfSuppressedSpikesInHighSpeedMode 0x%X\n", Index, ThcConfig->ThcPort[Index].HidOverI2c.MaximumLengthOfSuppressedSpikesInHighSpeedMode));
    DEBUG ((DEBUG_INFO, " PORT%d Clock: 0x%X\n", Index, ThcConfig->ThcPort[Index].Clock));
  }
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
ThcLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  THC_CONFIG  *ThcConfig;
  UINT8       Index;

  ThcConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "ThcConfig->Header.GuidHob.Name = %g\n", &ThcConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "ThcConfig->Header.GuidHob.Header.HobLength = 0x%x\n", ThcConfig->Header.GuidHob.Header.HobLength));
  for (Index = 0; Index < GetPchMaxThcCount (); Index ++) {
    ThcConfig->ThcPort[Index].Assignment                    = ThcAssignmentNone;
    ThcConfig->ThcPort[Index].InterruptPinMuxing            = 0;
    ThcConfig->ThcPort[Index].Mode                          = 0;
    ThcConfig->ThcPort[Index].ActiveLtr                     = 0xFFFFFFFF;
    ThcConfig->ThcPort[Index].IdleLtr                       = 0xFFFFFFFF;
    ThcConfig->ThcPort[Index].WotPinConfig.EdgeLevel        = GPIO_INT_MOD_LEVEL;
    ThcConfig->ThcPort[Index].WotPinConfig.ActiveLevel      = GPIO_INT_POL_ACTIVE_LOW;
    ThcConfig->ThcPort[Index].WotPinConfig.PinConfig        = GPIO_PPI_PULL_DEFAULT;
    ThcConfig->ThcPort[Index].ThcSubSystemId.CustomizedSsid = DEFAULT_SSDID;
    ThcConfig->ThcPort[Index].ThcSubSystemId.CustomizedSvid = DEFAULT_SSVID;
  }
}

STATIC COMPONENT_BLOCK_ENTRY  mThcBlocks = {
  &gThcConfigGuid,
  sizeof (THC_CONFIG),
  THC_CONFIG_REVISION,
  ThcLoadConfigDefault
};

/**
  Get Thc config block table size.

  @retval      Size of config block
**/
UINT16
ThcGetConfigBlockTotalSize (
  VOID
  )
{
  return mThcBlocks.Size;
}

/**
  Add Thc ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
ThcAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mThcBlocks, 1);
}
