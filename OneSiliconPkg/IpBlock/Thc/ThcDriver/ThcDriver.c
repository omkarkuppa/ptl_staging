/** @file
  Touch Host Controller Driver API

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

#include "ThcLibPrivate.h"
#include "ThcAbsPtr.h"
#include "ThcHid.h"


/**
  Sets Frequency based on Touch Capabilities MAX_SPI_FREQUENCY_SUPPORTED accordingly to EDS

  @param[in]  ThcDev          Context of Thc device

  @retval EFI_SUCCESS         Thc frequency set successfuly
  @retval EFI_DEVICE_ERROR    Error during frequency mapping with EDS
**/
EFI_STATUS
ThcSetFrequency (
  IN THC_DEV                *ThcDev
  )
{
  UINT32 MaxFrequency;
  UINT32 NewFrequency;

  NewFrequency = ThcFreqInvalid;
  MaxFrequency = ThcDev->DeviceRegisters.TouchCapabilities.Fields.MaxSpiFrequencySupported;

  //
  // Map Frequency to match THC and EDS
  //
  if (MaxFrequency == ThcEdsFreq7p812MHz) {
    NewFrequency = ThcFreqLow7p5MHz;
  } else if (MaxFrequency == ThcEdsFreq15p62MHz) {
    NewFrequency = ThcFreqLow15MHz;
  } else if (MaxFrequency == ThcEdsFreq17p86MHz) {
    NewFrequency = ThcFreqHigh17MHz;
  } else if (MaxFrequency == ThcEdsFreq20p83MHz) {
    NewFrequency = ThcFreqHigh20MHz;
  } else if (MaxFrequency == ThcEdsFreq25MHz) {
    NewFrequency = ThcFreqHigh24MHz;
  } else if (MaxFrequency == ThcEdsFreq31p25MHz) {
    NewFrequency = ThcFreqHigh30MHz;
  } else if (MaxFrequency == ThcEdsFreq41p67MHz) {
    NewFrequency = ThcFreqHigh40MHz;
  }

  if (NewFrequency == ThcFreqInvalid) {
    DEBUG ((DEBUG_WARN, "ThcSetFrequency error Max Device Frequency does not meet EDS specification:%d\n", MaxFrequency));
    return EFI_DEVICE_ERROR;
  }

  ThcLibSetPortFrequency (ThcDev->PciBar0, N_THC_MEM_PRT_SPI_CFG_SPI_TCRF, NewFrequency);
  ThcLibSetPortFrequency (ThcDev->PciBar0, N_THC_MEM_PRT_SPI_CFG_SPI_TCWF, NewFrequency);

  return EFI_SUCCESS;
}

/**
  Initialize Thc according to procedure described in EDS

  @param[in]  ThcDev      Context of Thc device

  @retval EFI_SUCCESS     Thc initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
ThcInitialize (
  IN THC_DEV                *ThcDev
  )
{
  EFI_STATUS Status;
  UINT8      ResetRetryCount;

  ResetRetryCount = THC_RESET_RETRY_COUNT;

  THC_LOCAL_DEBUG(L"ThcInitialize Start ()\n")
  Status = ThcLibIoReady (ThcDev->PciBar0, IoReadyTimeout);
  THC_LOCAL_DEBUG(L"ThcInitialize  ThcLibIoReady Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcLibIoReady error, Status %r\n", Status));
    return Status;
  }

  if (ThcLibIsQuiesceDisabled (ThcDev->PciBar0)) {
    THC_LOCAL_DEBUG(L"ThcInitialize  QuiesceDisabled \n")
    Status = ThcLibStartQuiesce (ThcDev->PciBar0, StartQuiesceTimeout);
    THC_LOCAL_DEBUG(L"ThcInitialize  ThcLibStartQuiesce Status: %r\n", Status)
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "ThcLibStartQuiesce error, Status %r\n", Status));
      return Status;
    }
  }

  //
  // Configure THC Controller to operate at 7MHz, Single I/O
  //
  THC_LOCAL_DEBUG(L"ThcInitialize  Configure THC Controller to operate at 7MHz, Single I/O\n")
  ThcLibSetPortFrequency (ThcDev->PciBar0, N_THC_MEM_PRT_SPI_CFG_SPI_TCRF, ThcFreqLow7p5MHz);
  ThcLibSetPortFrequency (ThcDev->PciBar0, N_THC_MEM_PRT_SPI_CFG_SPI_TCWF, ThcFreqLow7p5MHz);
  ThcLibSetSingleIoMode (ThcDev->PciBar0);

  ThcLibSetEdgeInterrupts (ThcDev->PciBar0);

  //
  // Reset THC and clear THC_DEVINT_QUIESCE_EN
  //
  ThcLibGetOutOfReset (ThcDev->PciBar0, OutOfResetTimeout);

  Status = ThcLibEndQuiesce (ThcDev->PciBar0, EndQuiesceTimeout);
  THC_LOCAL_DEBUG(L"ThcInitialize  ThcLibEndQuiesce Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcLibEndQuiesce error, Status %r\n", Status));
    return Status;
  }

  do {
    Status = ThcLibWaitForResetOccured (ThcDev->PciBar0, WaitForResetTimeout);
    THC_LOCAL_DEBUG(L"ThcInitialize  ThcLibWaitForResetOccured Status: %r\n", Status)
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "ThcLibWaitForResetOccured error, Status %r Reset Count %d\n", Status, ResetRetryCount));
    } else {
      return Status;
    }
    ThcLibGetOutOfReset (ThcDev->PciBar0, OutOfResetTimeout);
    ResetRetryCount--;
  } while (EFI_ERROR (Status) && (ResetRetryCount > 0));

  return Status;
}

/**
  Reads Touch Panels Device registers (TouchCapabilities, TouchId etc.)

  @param[in]  ThcDev      Context of Thc device

  @retval EFI_SUCCESS     Thc initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
ThcReadConfig (
  IN THC_DEV                *ThcDev
  )
{
  EFI_STATUS Status;

  Status = ThcLibNoCyclesPending (ThcDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG(L"ThcReadConfig  ThcLibNoCyclesPending Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcReadConfig error, Status %r\n", Status));
    return Status;
  }

  ThcLibPrepareReadDevRegCmd (ThcDev->PciBar0);
  ThcLibGo (ThcDev->PciBar0);

  Status = ThcLibWaitCycleCompleted (ThcDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG(L"ThcReadConfig  ThcLibWaitCycleCompleted Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcReadConfig error, Status %r\n", Status));
    return Status;
  }

  Status = ThcLibReadDeviceRegisters (ThcDev->PciBar0, (UINT32*) &ThcDev->DeviceRegisters);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcLibReadDeviceRegisters error, Status %r\n", Status));
  }

  return Status;
}

/**
  Prints end Device Configuration

  @param[in]  ThcDev      Context of Thc device

  @retval EFI_SUCCESS     Thc initialized successfully
  @retval other           Error during initialization
**/
VOID
ThcPrintConfig (
  IN  THC_DEV                *ThcDev
  )
{
  THC_LOCAL_DEBUG(L"ThcPrintConfig () Start\n")
  THC_LOCAL_DEBUG(L"| 0x00 | TouchIntCause        | 0x%X\n", ThcDev->DeviceRegisters.TouchIntCause)
  THC_LOCAL_DEBUG(L"| 0x04 | TouchErr             | 0x%X\n", ThcDev->DeviceRegisters.TouchErr)
  THC_LOCAL_DEBUG(L"| 0x08 | TouchDataSize        | 0x%X\n", ThcDev->DeviceRegisters.TouchDataSize.Data)
  THC_LOCAL_DEBUG(L"| 0x08 | - MAXIMUM_READ_SIZE  | %d\n",   ThcDev->DeviceRegisters.TouchDataSize.Fields.MaximumReadSize)
  THC_LOCAL_DEBUG(L"| 0x08 | - MAXIMUM_WRITE_SIZE | %d\n",   ThcDev->DeviceRegisters.TouchDataSize.Fields.MaximumWriteSize)
  THC_LOCAL_DEBUG(L"| 0x0C | TouchCfg             | 0x%X\n", ThcDev->DeviceRegisters.TouchCfg.Data)
  THC_LOCAL_DEBUG(L"| 0x0C | - TOUCH_ENABLE       | %d\n",   ThcDev->DeviceRegisters.TouchCfg.Fields.TouchEnable)
  THC_LOCAL_DEBUG(L"| 0x0C | - PROP_DATA_ENABLED  | %d\n",   ThcDev->DeviceRegisters.TouchCfg.Fields.ProprietaryDataModeEnabled)
  THC_LOCAL_DEBUG(L"| 0x0C | - HID_REPORT_ENABLED | %d\n",   ThcDev->DeviceRegisters.TouchCfg.Fields.HidReportModeEnabled)
  THC_LOCAL_DEBUG(L"| 0x0C | - POWER_STATE        | %d\n",   ThcDev->DeviceRegisters.TouchCfg.Fields.PowerState)
  THC_LOCAL_DEBUG(L"| 0x10 | TouchState           | 0x%X\n", ThcDev->DeviceRegisters.TouchState.Data)
  THC_LOCAL_DEBUG(L"| 0x10 | - TOUCH_ENABLE       | %d\n",   ThcDev->DeviceRegisters.TouchState.Fields.TouchEnable)
  THC_LOCAL_DEBUG(L"| 0x10 | - PROP_DATA_ENABLED  | %d\n",   ThcDev->DeviceRegisters.TouchState.Fields.ProprietaryDataModeEnabled)
  THC_LOCAL_DEBUG(L"| 0x10 | - HID_REPORT_ENABLED | %d\n",   ThcDev->DeviceRegisters.TouchState.Fields.HidReportModeEnabled)
  THC_LOCAL_DEBUG(L"| 0x10 | - POWER_STATE        | %d\n",   ThcDev->DeviceRegisters.TouchState.Fields.PowerState)
  THC_LOCAL_DEBUG(L"| 0x14 | TouchId              | 0x%X\n", ThcDev->DeviceRegisters.TouchId)
  THC_LOCAL_DEBUG(L"| 0x18 | TouchCapabilities    | 0x%X\n", ThcDev->DeviceRegisters.TouchCapabilities.Data)
  THC_LOCAL_DEBUG(L"| 0x18 | - PROPR_DATA_SUPPORT   | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.ProprietaryDataModeSupported)
  THC_LOCAL_DEBUG(L"| 0x18 | - MAX_SPI_FREQ_SUPPORT | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.MaxSpiFrequencySupported)
  THC_LOCAL_DEBUG(L"| 0x18 | - SINGLE_IO_SUPPORTED  | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.SingleIoSupported)
  THC_LOCAL_DEBUG(L"| 0x18 | - DUAL_IO_SUPPORTED    | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.DualIoSupported)
  THC_LOCAL_DEBUG(L"| 0x18 | - QUAD_IO_SUPPORTED    | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.QuadIoSupported)
  THC_LOCAL_DEBUG(L"| 0x18 | - MAXIMUM_TOUCH_POINTS | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.MaximumTouchPoints)
  THC_LOCAL_DEBUG(L"| 0x18 | - MINIMUM_RESET_TIME   | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.MaximumResetTime)
  THC_LOCAL_DEBUG(L"| 0x18 | - HEARTBEAT_REPORT_SUP | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.HeartBeatReportSupported)
  THC_LOCAL_DEBUG(L"| 0x18 | - PERFORMANCE_LIMIT    | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.PerformanceLimitation)
  THC_LOCAL_DEBUG(L"| 0x18 | - PREP_FOR_REGSTR_READ | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.PrepForRegisterRead)
  THC_LOCAL_DEBUG(L"| 0x1C | TouchIcHwId          | 0x%X\n", ThcDev->DeviceRegisters.TouchIcHwId)
  THC_LOCAL_DEBUG(L"| 0x20 | TouchIcHwRev         | 0x%X\n", ThcDev->DeviceRegisters.TouchIcHwRev)
  THC_LOCAL_DEBUG(L"| 0x24 | TouchIcFwRev         | 0x%X\n", ThcDev->DeviceRegisters.TouchIcFwRev)
  THC_LOCAL_DEBUG(L"| 0x28 | TouchCommand         | 0x%X\n", ThcDev->DeviceRegisters.TouchCommand)
  THC_LOCAL_DEBUG(L"| 0x30 | TouchInit            | 0x%X\n", ThcDev->DeviceRegisters.TouchInit)
  THC_LOCAL_DEBUG(L"| 0x34 | TouchCompatibilityId | 0x%X\n", ThcDev->DeviceRegisters.TouchCompatibilityId.Data)
  THC_LOCAL_DEBUG(L"| 0x34 | - EDS_MINOR_VERSION  | 0x%X\n", ThcDev->DeviceRegisters.TouchCompatibilityId.Fields.EdsMinorVersion)
  THC_LOCAL_DEBUG(L"| 0x34 | - EDS_MAJOR_VERSION  | 0x%X\n", ThcDev->DeviceRegisters.TouchCompatibilityId.Fields.EdsMajorVersion)
  THC_LOCAL_DEBUG(L"| 0x34 | - INTERFACE_REV_NUM  | 0x%X\n", ThcDev->DeviceRegisters.TouchCompatibilityId.Fields.InterfaceRevisionNumber)
  THC_LOCAL_DEBUG(L"| 0x34 | - EU_KERNEL_VERSION  | 0x%X\n", ThcDev->DeviceRegisters.TouchCompatibilityId.Fields.EuKernelCompatibilityVersion)
  THC_LOCAL_DEBUG(L"ThcPrintConfig () End\n")
  DEBUG ((DEBUG_INFO, "ThcPrintConfig () Start\n"));
  DEBUG ((DEBUG_INFO, "| 0x00 | TouchIntCause        | 0x%X\n", ThcDev->DeviceRegisters.TouchIntCause));
  DEBUG ((DEBUG_INFO, "| 0x04 | TouchErr             | 0x%X\n", ThcDev->DeviceRegisters.TouchErr));
  DEBUG ((DEBUG_INFO, "| 0x08 | TouchDataSize        | 0x%X\n", ThcDev->DeviceRegisters.TouchDataSize.Data));
  DEBUG ((DEBUG_INFO, "| 0x08 | - MAXIMUM_READ_SIZE  | %d\n",   ThcDev->DeviceRegisters.TouchDataSize.Fields.MaximumReadSize));
  DEBUG ((DEBUG_INFO, "| 0x08 | - MAXIMUM_WRITE_SIZE | %d\n",   ThcDev->DeviceRegisters.TouchDataSize.Fields.MaximumWriteSize));
  DEBUG ((DEBUG_INFO, "| 0x0C | TouchCfg             | 0x%X\n", ThcDev->DeviceRegisters.TouchCfg.Data));
  DEBUG ((DEBUG_INFO, "| 0x0C | - TOUCH_ENABLE       | %d\n",   ThcDev->DeviceRegisters.TouchCfg.Fields.TouchEnable));
  DEBUG ((DEBUG_INFO, "| 0x0C | - PROP_DATA_ENABLED  | %d\n",   ThcDev->DeviceRegisters.TouchCfg.Fields.ProprietaryDataModeEnabled));
  DEBUG ((DEBUG_INFO, "| 0x0C | - HID_REPORT_ENABLED | %d\n",   ThcDev->DeviceRegisters.TouchCfg.Fields.HidReportModeEnabled));
  DEBUG ((DEBUG_INFO, "| 0x0C | - POWER_STATE        | %d\n",   ThcDev->DeviceRegisters.TouchCfg.Fields.PowerState));
  DEBUG ((DEBUG_INFO, "| 0x10 | TouchState           | 0x%X\n", ThcDev->DeviceRegisters.TouchState.Data));
  DEBUG ((DEBUG_INFO, "| 0x10 | - TOUCH_ENABLE       | %d\n",   ThcDev->DeviceRegisters.TouchState.Fields.TouchEnable));
  DEBUG ((DEBUG_INFO, "| 0x10 | - PROP_DATA_ENABLED  | %d\n",   ThcDev->DeviceRegisters.TouchState.Fields.ProprietaryDataModeEnabled));
  DEBUG ((DEBUG_INFO, "| 0x10 | - HID_REPORT_ENABLED | %d\n",   ThcDev->DeviceRegisters.TouchState.Fields.HidReportModeEnabled));
  DEBUG ((DEBUG_INFO, "| 0x10 | - POWER_STATE        | %d\n",   ThcDev->DeviceRegisters.TouchState.Fields.PowerState));
  DEBUG ((DEBUG_INFO, "| 0x14 | TouchId              | 0x%X\n", ThcDev->DeviceRegisters.TouchId));
  DEBUG ((DEBUG_INFO, "| 0x18 | TouchCapabilities    | 0x%X\n", ThcDev->DeviceRegisters.TouchCapabilities.Data));
  DEBUG ((DEBUG_INFO, "| 0x18 | - PROPR_DATA_SUPPORT   | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.ProprietaryDataModeSupported));
  DEBUG ((DEBUG_INFO, "| 0x18 | - MAX_SPI_FREQ_SUPPORT | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.MaxSpiFrequencySupported));
  DEBUG ((DEBUG_INFO, "| 0x18 | - SINGLE_IO_SUPPORTED  | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.SingleIoSupported));
  DEBUG ((DEBUG_INFO, "| 0x18 | - DUAL_IO_SUPPORTED    | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.DualIoSupported));
  DEBUG ((DEBUG_INFO, "| 0x18 | - QUAD_IO_SUPPORTED    | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.QuadIoSupported));
  DEBUG ((DEBUG_INFO, "| 0x18 | - MAXIMUM_TOUCH_POINTS | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.MaximumTouchPoints));
  DEBUG ((DEBUG_INFO, "| 0x18 | - MINIMUM_RESET_TIME   | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.MaximumResetTime));
  DEBUG ((DEBUG_INFO, "| 0x18 | - HEARTBEAT_REPORT_SUP | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.HeartBeatReportSupported));
  DEBUG ((DEBUG_INFO, "| 0x18 | - PERFORMANCE_LIMIT    | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.PerformanceLimitation));
  DEBUG ((DEBUG_INFO, "| 0x18 | - PREP_FOR_REGSTR_READ | %d\n", ThcDev->DeviceRegisters.TouchCapabilities.Fields.PrepForRegisterRead));
  DEBUG ((DEBUG_INFO, "| 0x1C | TouchIcHwId          | 0x%X\n", ThcDev->DeviceRegisters.TouchIcHwId));
  DEBUG ((DEBUG_INFO, "| 0x20 | TouchIcHwRev         | 0x%X\n", ThcDev->DeviceRegisters.TouchIcHwRev));
  DEBUG ((DEBUG_INFO, "| 0x24 | TouchIcFwRev         | 0x%X\n", ThcDev->DeviceRegisters.TouchIcFwRev));
  DEBUG ((DEBUG_INFO, "| 0x28 | TouchCommand         | 0x%X\n", ThcDev->DeviceRegisters.TouchCommand));
  DEBUG ((DEBUG_INFO, "| 0x30 | TouchInit            | 0x%X\n", ThcDev->DeviceRegisters.TouchInit));
  DEBUG ((DEBUG_INFO, "| 0x34 | TouchCompatibilityId | 0x%X\n", ThcDev->DeviceRegisters.TouchCompatibilityId.Data));
  DEBUG ((DEBUG_INFO, "| 0x34 | - EDS_MINOR_VERSION  | 0x%X\n", ThcDev->DeviceRegisters.TouchCompatibilityId.Fields.EdsMinorVersion));
  DEBUG ((DEBUG_INFO, "| 0x34 | - EDS_MAJOR_VERSION  | 0x%X\n", ThcDev->DeviceRegisters.TouchCompatibilityId.Fields.EdsMajorVersion));
  DEBUG ((DEBUG_INFO, "| 0x34 | - INTERFACE_REV_NUM  | 0x%X\n", ThcDev->DeviceRegisters.TouchCompatibilityId.Fields.InterfaceRevisionNumber));
  DEBUG ((DEBUG_INFO, "| 0x34 | - EU_KERNEL_VERSION  | 0x%X\n", ThcDev->DeviceRegisters.TouchCompatibilityId.Fields.EuKernelCompatibilityVersion));
  DEBUG ((DEBUG_INFO, "ThcPrintConfig () End\n"));
}

/**
  Enables Touch Panel connected to the Touch Host Controller

  @param[in]  ThcDev             Context of Thc device

  @retval EFI_SUCCESS            Touch Panel Enabled
  @retval other                  Error during device enabling process
**/
EFI_STATUS
ThcEnableEndDevice (
  IN  THC_DEV              *ThcDev
  )
{
  EFI_STATUS Status;

  THC_LOCAL_DEBUG(L"ThcEnableEndDevice Start ()\n");
  ThcSetFrequency (ThcDev);

  Status = ThcLibNoCyclesPending (ThcDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG(L"ThcEnableEndDevice  ThcLibNoCyclesPending Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcEnableEndDevice error, Status %r\n", Status));
    return Status;
  }

  ThcLibPrepareWriteConfigCmd (ThcDev->PciBar0);
  ThcDev->DeviceRegisters.TouchCfg.Fields.PowerState = 0x3;
  ThcDev->DeviceRegisters.TouchCfg.Fields.ProprietaryDataModeEnabled = 0x0;
  ThcDev->DeviceRegisters.TouchCfg.Fields.HidReportModeEnabled = 0x1;
  ThcLibWriteSeqData1 (ThcDev->PciBar0, ThcDev->DeviceRegisters.TouchCfg.Data);
  ThcLibGo (ThcDev->PciBar0);

  Status = ThcLibWaitCycleCompleted (ThcDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG(L"ThcEnableEndDevice  ThcLibWaitCycleCompleted Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcEnableEndDevice error, Status %r\n", Status));
    return Status;
  }

  ThcLibPrepareWriteConfigCmd (ThcDev->PciBar0);
  ThcDev->DeviceRegisters.TouchCfg.Fields.TouchEnable = 0x1;
  ThcLibWriteSeqData1 (ThcDev->PciBar0, ThcDev->DeviceRegisters.TouchCfg.Data);
  ThcLibGo (ThcDev->PciBar0);

  Status = ThcLibWaitCycleCompleted (ThcDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG(L"ThcEnableEndDevice  ThcLibWaitCycleCompleted Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcEnableEndDevice error, Status %r\n", Status));
    return Status;
  }

  return Status;
}

/**
  Disables Touch Panel connected to the Touch Host Controller

  @param[in]  ThcDev             Context of Thc device

  @retval EFI_SUCCESS            Touch Panel Disabled
  @retval other                  Error during device enabling process
**/
EFI_STATUS
ThcDisableEndDevice (
  IN THC_DEV                *ThcDev
  )
{
  EFI_STATUS Status;

  THC_LOCAL_DEBUG(L"ThcDisableEndDevice Start ()\n")
  ThcSetFrequency (ThcDev);

  Status = ThcLibNoCyclesPending (ThcDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG(L"ThcDisableEndDevice  ThcLibNoCyclesPending Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ThcDisableEndDevice error, Status %r\n", Status));
    return Status;
  }

  ThcLibPrepareWriteConfigCmd (ThcDev->PciBar0);
  ThcDev->DeviceRegisters.TouchCfg.Fields.PowerState = 0x0;
  ThcDev->DeviceRegisters.TouchCfg.Fields.ProprietaryDataModeEnabled = 0x0;
  ThcDev->DeviceRegisters.TouchCfg.Fields.HidReportModeEnabled = 0x1;
  ThcLibWriteSeqData1 (ThcDev->PciBar0, ThcDev->DeviceRegisters.TouchCfg.Data);
  ThcLibGo (ThcDev->PciBar0);

  Status = ThcLibWaitCycleCompleted (ThcDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG(L"ThcDisableEndDevice  ThcLibWaitCycleCompleted Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ThcDisableEndDevice error, Status %r\n", Status));
    return Status;
  }

  ThcLibPrepareWriteConfigCmd (ThcDev->PciBar0);
  ThcDev->DeviceRegisters.TouchCfg.Fields.TouchEnable = 0x0;
  ThcLibWriteSeqData1 (ThcDev->PciBar0, ThcDev->DeviceRegisters.TouchCfg.Data);
  ThcLibGo (ThcDev->PciBar0);

  Status = ThcLibWaitCycleCompleted (ThcDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG(L"ThcDisableEndDevice  ThcLibWaitCycleCompleted Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ThcDisableEndDevice error, Status %r\n", Status));
    return Status;
  }

  return Status;
}

/**
  Initialize Memory for Prd used by the HW
  Only ReadRx2 and Write are supported by this driver

  @param[in]  ThcDev             Context of Thc device

  @retval EFI_SUCCESS            Thc Dma Initialized successfuly
  @retval EFI_OUT_OF_RESOURCES   Not enough memory available
**/
EFI_STATUS
ThcDmaCreatePrdCommonBuffers (
  IN  THC_DEV                *ThcDev
  )
{
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINT64                        PageCount;
  UINT64                        ByteCount;
  EFI_STATUS                    Status;

  PciIo = ThcDev->PciIo;

  PageCount = EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_READ_TABLES_SUPPORTED);
  Status = PciIo->AllocateBuffer (
                    PciIo,
                    AllocateAnyPages,
                    EfiBootServicesData,
                    PageCount,
                    (VOID*) &ThcDev->DmaRead2.CommonPrdBuffer,
                    0
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcDmaCreateCommonBuffers Read2 buffer allocateBuffer failed Status %r\n", Status));
    return EFI_OUT_OF_RESOURCES;
  }

  ByteCount = EFI_PAGES_TO_SIZE (PageCount);
  Status    = PciIo->Map (
                       PciIo,
                       EfiPciIoOperationBusMasterCommonBuffer,
                       (VOID*) ThcDev->DmaRead2.CommonPrdBuffer,
                       &ByteCount,
                       &ThcDev->DmaRead2.CommonPrdBufferDevAddr,
                       &ThcDev->DmaRead2.CommonPrdBufferMapping
                       );
  if (EFI_ERROR (Status) || (ByteCount != EFI_PAGES_TO_SIZE (PageCount))) {
    DEBUG ((DEBUG_WARN, "ThcDmaCreateCommonBuffers Read2 buffer map failed Status %r\n", Status));
    DEBUG ((DEBUG_WARN, "ThcDmaCreateCommonBuffers Requsted ByteCount: %d Mapped Count: %d\n", EFI_PAGES_TO_SIZE (PageCount), ByteCount));
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem ((UINT8*) ThcDev->DmaRead2.CommonPrdBuffer, ByteCount);

  PageCount = EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_WRITE_TABLES_SUPPORTED);
  Status = PciIo->AllocateBuffer (
                    PciIo,
                    AllocateAnyPages,
                    EfiBootServicesData,
                    PageCount,
                    (VOID*) &ThcDev->DmaWrite.CommonPrdBuffer,
                    0
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcDmaCreateCommonBuffers Read2 buffer allocateBuffer failed Status %r\n", Status));
    return EFI_OUT_OF_RESOURCES;
  }

  ByteCount = EFI_PAGES_TO_SIZE (PageCount);
  Status    = PciIo->Map (
                       PciIo,
                       EfiPciIoOperationBusMasterCommonBuffer,
                       (VOID*) ThcDev->DmaWrite.CommonPrdBuffer,
                       &ByteCount,
                       &ThcDev->DmaWrite.CommonPrdBufferDevAddr,
                       &ThcDev->DmaWrite.CommonPrdBufferMapping
                       );
  if (EFI_ERROR (Status) || (ByteCount != EFI_PAGES_TO_SIZE (PageCount))) {
    DEBUG ((DEBUG_WARN, "ThcDmaCreateCommonBuffers Write buffer map failed Status %r\n", Status));
    DEBUG ((DEBUG_WARN, "ThcDmaCreateCommonBuffers Requsted ByteCount: %d Mapped Count: %d\n", EFI_PAGES_TO_SIZE (PageCount), ByteCount));
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem ((UINT8*) ThcDev->DmaWrite.CommonPrdBuffer, ByteCount);

  return Status;
}

/**
  Fills single Prd Table with the defualt values

  @param[in]  ThcDma           Dma that stores Driver PRD table
  @param[in]  PrdTableIndex    Index of current Prd table

**/
VOID
ThcDmaFillSinglePrdTable (
  IN THC_DMA                    *ThcDma,
  IN UINT32                     PrdTableIndex
  )
{
  PRD_TABLE                     *PrdTable;
  UINT32                        CurrentEntry = 0;
  UINT32                        EntryOffset  = ThcDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries * PrdTableIndex;

  PrdTable =  (PRD_TABLE*) ThcDma->CommonPrdBuffer;

  for (CurrentEntry = 0; CurrentEntry < ThcDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries; CurrentEntry++) {
    CopyMem (
      &PrdTable->Entries[CurrentEntry + EntryOffset],
      &ThcDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry],
      sizeof (PRD_ENTRY)
      );
  }
}

/**
  Fills every Prd Table with the defualt values

  @param[in]  ThcDma           Dma that stores Driver PRD Table

**/
VOID
ThcDmaFillPrdTables (
  THC_DMA                        *ThcDma
  )
{
  UINT32                        PrdTableIndex;

  THC_LOCAL_DEBUG(L"ThcDmaFillPrdTables Start ()\n")

  //
  // Loop through total number of Entries in Common Prd
  // Fill every entry with the corresponding data for the HW to use
  //
  for (PrdTableIndex = 0; PrdTableIndex < ThcDma->NumOfPrdTables; PrdTableIndex++) {
    ThcDmaFillSinglePrdTable (ThcDma, PrdTableIndex);
  }
}

/**
  Initialize Driver Prd Tables
  Allocates and maps every single page for each used entry
  besed on the supported BufferSize for each MAXIMUM_READ_SIZE/MAXIMUM_WRITE_SIZE

  @param[in]  ThcDev           Context of Thc device
  @param[in]  ThcDma           Dma that stores Driver PRD Table
  @param[in]  PciIoOperation   BusMasterRead or BusMasterWrite

  @retval EFI_SUCCESS            Thc Dma Initialized successfuly
  @retval EFI_OUT_OF_RESOURCES   Not enough memory available
**/
EFI_STATUS
ThcDmaCreatePrds (
  IN THC_DEV                        *ThcDev,
  IN THC_DMA                        *ThcDma,
  IN EFI_PCI_IO_PROTOCOL_OPERATION  PciIoOperation
  )
{
  EFI_STATUS                    Status;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINTN                         PageCount;
  UINTN                         ByteCount;
  UINT16                        CurrentEntry;
  UINT64                        CurrentAddress;
  UINT32                        PrdTableIndex;
  UINTN                         BytesRemaining;

  THC_LOCAL_DEBUG(L"ThcDmaCreatePrds Start ()\n")

  PciIo = ThcDev->PciIo;

  for (PrdTableIndex = 0; PrdTableIndex < ThcDma->NumOfPrdTables; PrdTableIndex++) {

    PageCount = EFI_SIZE_TO_PAGES (ThcDma->BufferSize);
    Status    = PciIo->AllocateBuffer (
                         PciIo,
                         AllocateAnyPages,
                         EfiBootServicesData,
                         PageCount,
                         (VOID*) &ThcDma->DriverPrdTable[PrdTableIndex].Buffer,
                         0
                         );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "ThcDmaCreateReadPrds AllocateBuffer for PrdTableIndex: %d failed with Status %r\n", PrdTableIndex, Status));
      return EFI_OUT_OF_RESOURCES;
    }

    ByteCount = EFI_PAGES_TO_SIZE (PageCount);
    Status    = PciIo->Map (
                         PciIo,
                         PciIoOperation,
                         (VOID*) ThcDma->DriverPrdTable[PrdTableIndex].Buffer,
                         &ByteCount,
                         &ThcDma->DriverPrdTable[PrdTableIndex].DevAddr,
                         &ThcDma->DriverPrdTable[PrdTableIndex].Mapping
                         );

    if (EFI_ERROR (Status) || (ByteCount != EFI_PAGES_TO_SIZE (PageCount))) {
      DEBUG ((DEBUG_WARN, "ThcDmaCreateReadPrds Read buffer map failed PrdTableIndex: %d Status %r\n", PrdTableIndex, Status));
      DEBUG ((DEBUG_WARN, "ThcDmaCreateReadPrds Requsted ByteCount: %d Mapped Count: %d\n", EFI_PAGES_TO_SIZE (PageCount), ByteCount));
      return EFI_OUT_OF_RESOURCES;
    }

    ZeroMem ((UINT8*) ThcDma->DriverPrdTable[PrdTableIndex].Buffer, ByteCount);

    BytesRemaining   = ByteCount;
    CurrentEntry     = 0;
    CurrentAddress   = ThcDma->DriverPrdTable[PrdTableIndex].Buffer;
    //
    // Loop through total number of Entries
    // Each entry Max length is 4K (size of the single page), max entries is 256.
    // Max allocated total for single PRD table is 1MB
    //
    while (BytesRemaining > 0 && CurrentEntry < 256) {
      ThcDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].DestinationAddress = RShiftU64 (CurrentAddress, ADDRESS_SHIFT);
      ThcDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].EndOfPrd           = 0x0;
      ThcDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].Length             = EFI_PAGE_SIZE;
      if (BytesRemaining <= EFI_PAGE_SIZE) {
        ThcDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].Length           = BytesRemaining;
        ThcDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].EndOfPrd         = 0x1;
        break;
      }
      CurrentEntry++;
      CurrentAddress += EFI_PAGE_SIZE;
      BytesRemaining -= EFI_PAGE_SIZE;
    }
    ThcDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries = CurrentEntry + 1;
  }

  return EFI_SUCCESS;
}

/**
  Initialize Thc DMA

  @param[in]  ThcDev      Context of Thc device

  @retval EFI_SUCCESS     Thc Dma Initialized successfuly
  @retval other           error during initialization
**/
EFI_STATUS
ThcDmaInitialize (
  IN THC_DEV                *ThcDev
  )
{
  EFI_STATUS Status;

  THC_LOCAL_DEBUG(L"ThcDmaInitialize Start ()\n")

  Status = ThcDmaCreatePrdCommonBuffers (ThcDev);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcDmaInitialize error, failed to CreatePrdCommonBuffers Status %r\n", Status));
    return Status;
  }

  Status = ThcLibSetPrdTablesAddress (ThcDev->PciBar0, ThcDev->DmaWrite.CommonPrdBuffer, ThcDev->DmaRead2.CommonPrdBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcDmaInitialize error, failed to SetPrdTablesAddress Status %r\n", Status));
    return Status;
  }

  ThcDev->DmaRead2.BufferSize     = (ThcDev->DeviceRegisters.TouchDataSize.Fields.MaximumReadSize + 1) * SIZE_FIELD_RESOLUTION;
  ThcDev->DmaRead2.NumOfPrdTables = PRD_READ_TABLES_SUPPORTED;
  ThcDev->DmaWrite.BufferSize     = (ThcDev->DeviceRegisters.TouchDataSize.Fields.MaximumWriteSize + 1) * SIZE_FIELD_RESOLUTION;
  ThcDev->DmaWrite.NumOfPrdTables = PRD_WRITE_TABLES_SUPPORTED;

  Status = ThcDmaCreatePrds (ThcDev, &ThcDev->DmaRead2, EfiPciIoOperationBusMasterCommonBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcDmaInitialize error, failed to Create Read Prds Status %r\n", Status));
    return Status;
  }

  ThcDmaFillPrdTables (&ThcDev->DmaRead2);

  Status = ThcDmaCreatePrds (ThcDev, &ThcDev->DmaWrite, EfiPciIoOperationBusMasterCommonBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcDmaInitialize error, failed to Create Write Prds Status %r\n", Status));
    return Status;
  }

  ThcDmaFillPrdTables (&ThcDev->DmaWrite);

  ThcLibSetLengthInPrd  (ThcDev->PciBar0,
                         (UINT8) ThcDev->DmaRead2.DriverPrdTable[0].NumOfConfiguredEntries,
                         PRD_READ_TABLES_SUPPORTED,
                         (UINT8) ThcDev->DmaWrite.DriverPrdTable[0].NumOfConfiguredEntries
                        );

  ThcLibSetReadRx2StartBit (ThcDev->PciBar0);

  return EFI_SUCCESS;
}

/**
  Calculates the length of the current message

  @param[in]  ThcDma             Dma that stores Driver PRD Table
  @param[in]  PrdTable           Prd Table updated by the THC HW
  @param[in]  PrdTableIndex      Index of the Prd Table
  @param[in]  EntryOffset        Entry offset

  @retval MessageLength          Size of the message
**/
UINT64
ThcDmaGetMessageLength (
  IN THC_DMA                        *ThcDma,
  IN PRD_TABLE                      *PrdTable,
  IN UINT16                         PrdTableIndex,
  IN UINT32                         EntryOffset
  )
{
  UINT32  Index;
  UINT64  MessageLength = 0;

  for (Index = 0; Index < ThcDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries; Index++) {
    MessageLength += PrdTable->Entries[EntryOffset + Index].Length;
    if (PrdTable->Entries[EntryOffset + Index].EndOfPrd == 1) {
      return MessageLength;
    }
  }
  return MessageLength;
}

/**
  Reads first PRD Table Entry and checks if header data type
  equals to TouchReadDataTypeHidReport
  and Report Id is equal to SINGLE_FINGER_REPORT_ID.
  If passed report data is copied to the Thc Context.

  @param[in]  ThcDev             Context of Thc device
  @param[in]  ThcDma             Dma that stores Driver PRD Table
  @param[in]  CurrentPrdTable    Index of current Prd table

  @retval EFI_SUCCESS            Data read done
  @retval EFI_BAD_BUFFER_SIZE    MessageLength is smaller then its header
  @retval EFI_BUFFER_TOO_SMALL   Allocated entry length is smaller then one present in Prd
**/
EFI_STATUS
ThcDmaReadSingleReport (
  IN THC_DEV                *ThcDev,
  IN THC_DMA                *ThcDma,
  IN UINT16                 CurrentPrdTable
  )
{
  READ_DATA_BUFF           *ReadDataBuff;
  PRD_TABLE                *PrdTable;
  UINT64                   MessageLength = 0;
  UINT64                   DataAddress   = 0;
  UINT32                   EntryOffset   = 0;
  HID_TOUCH_OUTPUT         TouchOutput;
  HID_XY_BOUNDARY          XyBoundary;
  EFI_STATUS               Status;

  EntryOffset = (UINT32)(ThcDma->DriverPrdTable[CurrentPrdTable].NumOfConfiguredEntries) * CurrentPrdTable;
  PrdTable    = (PRD_TABLE *) ThcDma->CommonPrdBuffer;

  THC_LOCAL_DEBUG(L"ThcDmaReadSingleReport Start ()\n")
  ReadDataBuff = AllocateZeroPool (sizeof (READ_DATA_BUFF));
  if (ReadDataBuff == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  MessageLength = ThcDmaGetMessageLength (ThcDma, PrdTable, CurrentPrdTable, EntryOffset);
  THC_LOCAL_DEBUG(L"MessageLength %d \n", MessageLength)
  if (MessageLength < sizeof (RAW_DATA_HEADER)) {
    FreePool (ReadDataBuff);
    return EFI_BAD_BUFFER_SIZE;
  }
  if (ThcDma->DriverPrdTable[CurrentPrdTable].Entries[0].Length < PrdTable->Entries[EntryOffset].Length) {
    FreePool (ReadDataBuff);
    return EFI_BUFFER_TOO_SMALL;
  }

  DataAddress = LShiftU64 (PrdTable->Entries[EntryOffset].DestinationAddress, ADDRESS_SHIFT);
  CopyMem ((UINT8*) ReadDataBuff, (UINT8*) DataAddress, sizeof (READ_DATA_BUFF));

  THC_LOCAL_DEBUG(L"ReadDataBuff->Header.DataType %d\n", ReadDataBuff->Header.DataType)

  //ShowBuffer ((UINT8*)(DataAddress), (UINT32) MessageLength); // Uncomment when THC_LOCAL_DEBUG is enabled

  if ((ReadDataBuff->Header.DataType == TouchReadDataTypeHidReport) && (ReadDataBuff->ReadData[0] == SINGLE_FINGER_REPORT_ID)) {
    CopyMem ((UINT8*) &ThcDev->Report, (UINT8*) DataAddress + sizeof (RAW_DATA_HEADER), sizeof (SINGLE_FINGER_REPORT));
    THC_LOCAL_DEBUG(L"X:%d Y:%d Btn: %d \n",
                    (ThcDev->Report.HigherXByte << 8) + ThcDev->Report.LowerXByte,
                    (ThcDev->Report.HigherYByte << 8) + ThcDev->Report.LowerYByte,
                    ThcDev->Report.TouchSts);

    ThcDev->AbsPtrDataAvailable = TRUE;
    gBS->SignalEvent (ThcDev->AbsPtrProtocol.WaitForInput);
  } else if ((ReadDataBuff->Header.DataType == TouchReadDataTypeGetFeatures) &&
             (ThcDev->HidActive == TRUE) &&
             (ThcDev->HidBuffer != NULL)) {
    THC_LOCAL_DEBUG(L"ThcDmaReadSingleReport TouchReadDataTypeGetFeatures\n")
    DEBUG ((DEBUG_INFO, "TouchReadDataTypeGetFeatures\n"));
    CopyMem ((UINT8*) ThcDev->HidBuffer, (UINT8*) DataAddress + sizeof (RAW_DATA_HEADER), MessageLength);
    ThcDev->HidBuffer += MessageLength;
    ThcDev->HidDataAvailable = TRUE;
  } else if ((ReadDataBuff->Header.DataType == TouchReadDataTypeTctlRsp) &&
             (ThcDev->HidActive == TRUE) &&
             (ThcDev->HidBuffer != NULL)) {
    THC_LOCAL_DEBUG(L"ThcDmaReadSingleReport TouchReadDataTypeTctlRsp\n")
    CopyMem ((UINT8*) ThcDev->HidBuffer, (UINT8*) DataAddress, MessageLength);
    ThcDev->HidBuffer += MessageLength;
    ThcDev->HidDataAvailable = TRUE;
  } else if ((ReadDataBuff->Header.DataType == TouchReadDataTypeHidReport || ReadDataBuff->Header.DataType == TouchReadDataTypeResetOccurred) &&
             (ThcDev->HidActive == TRUE) &&
             (ThcDev->HidBuffer != NULL)) {
    THC_LOCAL_DEBUG(L"ThcDmaReadSingleReport Warning! Consuming HidReport as Get/Set Feature response\n")
    DEBUG ((DEBUG_WARN, "ThcDmaReadSingleReport Warning! Consuming HidReport as Get/Set Feature response\n"));
    CopyMem ((UINT8*) ThcDev->HidBuffer, (UINT8*) DataAddress + sizeof (RAW_DATA_HEADER), MessageLength);
    ThcDev->HidBuffer += MessageLength;
    ThcDev->HidDataAvailable = TRUE;
  } else if (ReadDataBuff->Header.DataType == TouchReadDataTypeHidReport) {
    // Initialize the TouchOutput X/Y values with previous pointer location.
    TouchOutput.B = 0;
    TouchOutput.X = (ThcDev->Report.HigherXByte << 8) + (ThcDev->Report.LowerXByte);
    TouchOutput.Y = (ThcDev->Report.HigherYByte << 8) + (ThcDev->Report.LowerYByte);
    Status = HidParseInput (ThcDev->InputReportTable, (UINT8*) DataAddress + (sizeof (RAW_DATA_HEADER)), &TouchOutput, &XyBoundary);
    if (Status == EFI_SUCCESS) {
      ThcDev->Report.HigherXByte = (UINT8) (((TouchOutput.X) & 0xFF00) >> 8);
      ThcDev->Report.LowerXByte  = (UINT8) (TouchOutput.X) & 0xFF;
      ThcDev->Report.HigherYByte = (UINT8) (((TouchOutput.Y) & 0xFF00) >> 8);
      ThcDev->Report.LowerYByte  = (UINT8) (TouchOutput.Y) & 0xFF;
      ThcDev->Report.TouchSts = (UINT8) TouchOutput.B;
      ThcDev->AbsPtrProtocol.Mode->AbsoluteMaxX = XyBoundary.MaxX;
      ThcDev->AbsPtrProtocol.Mode->AbsoluteMinX = XyBoundary.MinX;
      ThcDev->AbsPtrProtocol.Mode->AbsoluteMaxY = XyBoundary.MaxY;
      ThcDev->AbsPtrProtocol.Mode->AbsoluteMinY = XyBoundary.MinY;
      ThcDev->AbsPtrDataAvailable = TRUE;
      gBS->SignalEvent (ThcDev->AbsPtrProtocol.WaitForInput);
    }
  }

  FreePool (ReadDataBuff);

  return EFI_SUCCESS;
}

/**
  Reads data from each Read Rx2 DMA PRD Table Entry until write pointer is equal to read pointer
  Write pointer is updated after each PRD Table read is finished

  @param[in]  ThcDev                    Context of Thc device
  @param[in]  CurrentCBReadPointer      Circullar Buffer Read Pointer
  @param[in]  CurrentCBWritePointer     Circullar Buffer Write Pointer

**/
VOID
ThcDmaReadRx2 (
  IN THC_DEV                *ThcDev,
  IN UINT8                  CurrentCBWritePointer,
  IN UINT8                  CurrentCBReadPointer
  )
{
  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;
  EFI_STATUS                 Status;
  UINT8                      CurrentPrdTable;

  while ((CurrentCBWritePointer & POINTER_MASK) != (CurrentCBReadPointer & POINTER_MASK)) {
    CurrentPrdTable = (UINT8) (CurrentCBWritePointer & POINTER_MASK);
    THC_LOCAL_DEBUG(L"PrdTable %d CBWritePointer %d CBReadPointer %d CB 0x%X \n",
                     CurrentPrdTable,
                     CurrentCBWritePointer & POINTER_MASK,
                     CurrentCBReadPointer & POINTER_MASK,
                     CurrentCBWritePointer
                    )
    Status = ThcDmaReadSingleReport (ThcDev, &ThcDev->DmaRead2, CurrentPrdTable);
    if (EFI_ERROR (Status)) {
      THC_LOCAL_DEBUG(L"ThcDmaReadRx2 ThcDmaReadbuffer error, failed to Read Data Buffer Status %r\n", Status)
      DEBUG ((DEBUG_ERROR, "ThcDmaReadRx2 Error Read Data Buffer Status %r\n", Status));
      //no return to avoid dead loop, always change Write Pointer, even on bad data
    }

    //Return PRD table to its initial state
    ThcDmaFillSinglePrdTable (&ThcDev->DmaRead2, CurrentPrdTable);

    if ((CurrentCBWritePointer + 1) == WRAPAROUND_VALUE_16) {
      THC_LOCAL_DEBUG(L"WRAPAROUND_VALUE_16 \n")
      ThcLibSetReadRx2WritePointer (ThcDev->PciBar0, POINTER_WRAPAROUND);
    } else if ((CurrentCBWritePointer + 1) == WRAPAROUND_VALUE_0X90) {
      THC_LOCAL_DEBUG(L"WRAPAROUND_VALUE_0X90 \n")
      ThcLibSetReadRx2WritePointer (ThcDev->PciBar0, 0x0);
    } else {
      THC_LOCAL_DEBUG(L"WP +1\n")
      ThcLibSetReadRx2WritePointer (ThcDev->PciBar0, CurrentCBWritePointer + 1);
    }

    ReadDmaCntrl2.Data    = ThcLibGetReadRx2Data (ThcDev->PciBar0);
    CurrentCBWritePointer = (UINT8) ReadDmaCntrl2.Fields.ThcPrdCbWritePointer;
  }
}

/**
  Writes data to a single Tx DMA PRD Table

  @param[in]  ThcDev             Context of Thc device
  @param[in]  Buffer             Data Container that will be sent to external device
  @param[in]  DataHeader         Write Data Header

  @retval EFI_BUFFER_TOO_SMALL   THC DMA buffer is unable to fit that much data
  @retval EFI_TIMEOUT            DMA transaction did not finish in time
  @retval EFI_SUCCESS            Data sent
**/
EFI_STATUS
ThcDmaWriteTx (
  IN THC_DEV                *ThcDev,
  IN UINT8                  *Buffer,
  IN THC_WRITE_DATA_HDR     *DataHeader
  )
{
  PRD_TABLE                *PrdTable;
  UINT64                   DataAddress;
  UINT32                   PrdEntryLength;
  UINT32                   WriteLength;
  UINT32                   EntryIndex;
  UINT8                    ByteAlignmentCount;

  DataAddress         = 0;
  PrdEntryLength      = 0;
  EntryIndex          = 0;
  ByteAlignmentCount  = 0;
  PrdTable            = (PRD_TABLE *) ThcDev->DmaWrite.CommonPrdBuffer;

  if (DataHeader->WriteDataLength % 4 > 0) {
    ByteAlignmentCount = 4 - DataHeader->WriteDataLength % 4;
  }

  if ((PrdTable->Entries[0].Length * PRD_MAX_ENTRIES) < (DataHeader->WriteDataLength + ByteAlignmentCount + sizeof (THC_WRITE_DATA_HDR))) {
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // Copy Header information, increase address and set remaining length of the first entry
  //
  DataAddress = LShiftU64 (PrdTable->Entries[EntryIndex].DestinationAddress, ADDRESS_SHIFT);
  //
  // To make sure DW alignment requirement is met driver will increase MSG size
  //
  if (ByteAlignmentCount > 0) {
    DataHeader->WriteDataLength = DataHeader->WriteDataLength + ByteAlignmentCount;
  }
  CopyMem ((UINT8*) DataAddress, (UINT8*) DataHeader, sizeof (THC_WRITE_DATA_HDR));
  DataHeader->WriteDataLength = DataHeader->WriteDataLength - ByteAlignmentCount;
  DataAddress += sizeof (THC_WRITE_DATA_HDR);
  PrdEntryLength = (UINT32) PrdTable->Entries[EntryIndex].Length - sizeof (THC_WRITE_DATA_HDR);

  do {
    PrdTable->Entries[EntryIndex].EndOfPrd = 0;
    //
    // Set write length for given Prd Entry
    //
    if (DataHeader->WriteDataLength >= PrdEntryLength) {
      WriteLength = PrdEntryLength;
    } else {
      WriteLength = DataHeader->WriteDataLength;
    }
    //
    // Put the data in the destination address
    //
    THC_LOCAL_DEBUG(L"ThcDmaWriteTx WriteLength %d\n", WriteLength)
    CopyMem ((UINT8*) DataAddress, Buffer, WriteLength);
    //
    // Substract copied data from the WriteDataLength
    //
    DataHeader->WriteDataLength = DataHeader->WriteDataLength - WriteLength;

    //
    // Store new write length along with the header in Prd entry.
    //
    if (EntryIndex == 0) {
      WriteLength += sizeof (THC_WRITE_DATA_HDR);
    }
    PrdTable->Entries[EntryIndex].Length   = WriteLength;

    if (DataHeader->WriteDataLength > 0) {
      Buffer += WriteLength;
      EntryIndex++;
      DataAddress = LShiftU64 (PrdTable->Entries[EntryIndex].DestinationAddress, ADDRESS_SHIFT);
      PrdEntryLength = (UINT32) PrdTable->Entries[EntryIndex].Length;
    } else {
      break;
    }
  } while (EntryIndex < ThcDev->DmaWrite.DriverPrdTable[0].NumOfConfiguredEntries);

  //
  // Fill Prd to meet DW alignment
  //
  if (ByteAlignmentCount > 0) {
    PrdTable->Entries[EntryIndex].Length += ByteAlignmentCount;
    DataAddress += WriteLength;
    ZeroMem ((UINT8*) DataAddress, ByteAlignmentCount);
  }

  //
  // Set End on the last PRD entry used
  //
  PrdTable->Entries[EntryIndex].EndOfPrd = 1;

  //
  // Start transfer
  //
  ThcLibSetWriteStartBit (ThcDev->PciBar0);

  //
  // Poll for Dma Complete
  //
  if (EFI_ERROR (ThcLibPollDmaComplete (ThcDev->PciBar0, WritePollingTimeout))) {
    return EFI_TIMEOUT;
  }

  //
  // Reset PRD table settings
  //
  ThcDmaFillSinglePrdTable (&ThcDev->DmaWrite, 0);

  return EFI_SUCCESS;
}

/**
  Unmaps and frees Driver Prd Tables

  @param[in]  ThcDev             Context of Thc device
  @param[in]  ThcDma             Dma that stores Driver PRD Table

**/
VOID
ThcDmaFreePrds (
  IN THC_DEV                        *ThcDev,
  IN THC_DMA                        *ThcDma
  )
{
  UINT32         PrdTableIndex;

  for (PrdTableIndex = 0; PrdTableIndex < ThcDma->NumOfPrdTables; PrdTableIndex++) {
    ThcDev->PciIo->Unmap (
                        ThcDev->PciIo,
                        ThcDma->DriverPrdTable[PrdTableIndex].Mapping
                        );
    ThcDev->PciIo->FreeBuffer (
                          ThcDev->PciIo,
                          EFI_SIZE_TO_PAGES (ThcDma->BufferSize),
                          (UINT8 *) ThcDma->DriverPrdTable[PrdTableIndex].Buffer
                          );
  }
}

/**
  Stop Touch Panel, brings it back to reset state
  Freees all allocated memory and Thc device context

  @param[in]  ThcDev         Context of Thc device

**/
VOID
EFIAPI
ThcStop (
  IN THC_DEV                *ThcDev
  )
{
  if (ThcDev == NULL) {
    DEBUG ((DEBUG_WARN, "ThcStop ThcDev is already Stopped\n"));
    return;
  }

  THC_LOCAL_DEBUG(L"ThcStop\n")

  ThcLibStopDma (ThcDev->PciBar0);
  ThcStopAbsPtr (ThcDev);
  ThcDisableEndDevice (ThcDev);
  ThcPrintConfig (ThcDev);
  ThcLibCleanUp (ThcDev->PciBar0);

  //
  // Free/Unmap Common Buffers and Prd Tables
  //
  if (ThcDev->DmaRead2.CommonPrdBuffer != 0) {
    THC_LOCAL_DEBUG(L"ThcStop Free DmaRead2.CommonPrdBuffer\n")
    ThcDev->PciIo->Unmap (
                        ThcDev->PciIo,
                        ThcDev->DmaRead2.CommonPrdBufferMapping
                        );
    ThcDev->PciIo->FreeBuffer (
                        ThcDev->PciIo,
                        EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_READ_TABLES_SUPPORTED),
                        (UINT8 *) ThcDev->DmaRead2.CommonPrdBuffer
                        );
    ThcDmaFreePrds (ThcDev, &ThcDev->DmaRead2);
  }
  if (ThcDev->DmaWrite.CommonPrdBuffer != 0) {
    THC_LOCAL_DEBUG(L"ThcStop Free DmaWrite.CommonPrdBuffer\n")
    ThcDev->PciIo->Unmap (
                        ThcDev->PciIo,
                        ThcDev->DmaWrite.CommonPrdBufferMapping
                        );
    ThcDev->PciIo->FreeBuffer (
                        ThcDev->PciIo,
                        EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_WRITE_TABLES_SUPPORTED),
                        (UINT8 *) ThcDev->DmaWrite.CommonPrdBuffer
                        );
    ThcDmaFreePrds (ThcDev, &ThcDev->DmaWrite);
  }

  //
  // Restore original PCI attributes
  //
  ThcDev->PciIo->Attributes (
                   ThcDev->PciIo,
                   EfiPciIoAttributeOperationSet,
                   ThcDev->PciAttributes,
                   NULL
                   );

  //
  // Close and Uninstall PCI I/O Protocol that was opened with PROTOCOL_BY_DRIVER
  //
  gBS->CloseProtocol (
         ThcDev->ControllerHandle,
         &gEfiPciIoProtocolGuid,
         &ThcDev->PciIo,
         ThcDev->ControllerHandle
         );
  gBS->UninstallMultipleProtocolInterfaces (
         ThcDev->ControllerHandle,
         &gEfiPciIoProtocolGuid,
         &ThcDev->PciIo,
         NULL
         );

  //
  // Done with the controller name table
  //
  FreeUnicodeStringTable (ThcDev->ControllerNameTable);

  //
  //  Done with the driver context
  //
  ZeroMem (ThcDev, sizeof (THC_DEV));
  FreePool (ThcDev);
}

/**
  Polls on Thc Rx2 Circullar Buffers state
  If CB Write and Read Pointer are not equal Read Rx2 Dma is trigerred

  @param[in]  Event       Polling event
  @param[in]  Context     Context of Thc device

**/
VOID
EFIAPI
ThcPolling (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  THC_DEV                    *ThcDev;
  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;
  UINT32                     Timeout;

  ThcDev = (THC_DEV *) Context;

  if (ThcDev == NULL) {
    DEBUG ((DEBUG_ERROR, "ThcPolling ThcDev is Null\n"));
    return;
  }
  if (ThcDev->ReadDone) {
    Timeout = PollingLoopTimeout;
    ReadDmaCntrl2.Data    = ThcLibGetReadRx2Data (ThcDev->PciBar0);
    while (((UINT8)(ReadDmaCntrl2.Fields.ThcPrdCbWritePointer  & POINTER_MASK)) != ((UINT8)(ReadDmaCntrl2.Fields.ThcPrdCbReadPointer & POINTER_MASK))) {
      ThcDev->ReadDone = FALSE;
      ThcDmaReadRx2 (ThcDev, (UINT8) ReadDmaCntrl2.Fields.ThcPrdCbWritePointer, (UINT8) ReadDmaCntrl2.Fields.ThcPrdCbReadPointer);
      ReadDmaCntrl2.Data    = ThcLibGetReadRx2Data (ThcDev->PciBar0);
      if (((UINT8)(ReadDmaCntrl2.Fields.ThcPrdCbWritePointer  & POINTER_MASK)) == ((UINT8)(ReadDmaCntrl2.Fields.ThcPrdCbReadPointer & POINTER_MASK))) {
        ThcDev->ReadDone = TRUE;
      }
      if (Timeout == 0) {
        DEBUG ((DEBUG_ERROR, "ThcPolling Timeout while reading DMA, possible interrupt storm\n"));
        ASSERT (FALSE);
        ThcDev->ReadDone = TRUE;
        return;
      }
      MicroSecondDelay (10);
      Timeout -= 10;
    }
  }
}

/**
  Enables end point device.
  - Reads all Touch Panels devices registers
  - Sets TouchEnable

  @param[in]  ThcDev           Context of Thc device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_DEVICE_ERROR     TSSDONE not set or ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
ThcEnableAfterReset (
  IN  THC_DEV              *ThcDev
  )
{
  EFI_STATUS Status;

  ThcReadConfig (ThcDev);
  ThcPrintConfig (ThcDev);

  ThcLibPrepareWriteConfigCmd (ThcDev->PciBar0);
  ThcDev->DeviceRegisters.TouchCfg.Fields.TouchEnable = 0x1;
  ThcLibWriteSeqData1 (ThcDev->PciBar0, ThcDev->DeviceRegisters.TouchCfg.Data);
  ThcLibGo (ThcDev->PciBar0);

  Status = ThcLibWaitCycleCompleted (ThcDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG(L"ThcEnableEndDevice  ThcLibWaitCycleCompleted Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ThcEnableEndDevice error, Status %r\n", Status));
    return Status;
  }

  return Status;
}

/**
  Reads HID Descriptor for non EDS complaint devices that do not support Single Finger Report.
  Flow:
  - send TCTL_REQUEST with Get Hid desc command
  - waits for response
  - Run Descriptor parser and populate all known Report Ids in Thc HID_INPUT_REPORT_TABLE

  @param[in]  ThcDev           Context of Thc device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device Did not respond to TCTLCMD
  @retval EFI_TIMEOUT          Timeout while waiting for response from the device
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for Thc HID support
**/
EFI_STATUS
ThcReadHidDescriptor (
  IN  THC_DEV              *ThcDev
  )
{
  EFI_STATUS          Status;
  THC_WRITE_DATA_HDR  WriteData;
  UINT32              Timeout;
  THC_CTL             ThcCtl;
  UINT32              HidMessageLength;
  UINT8               *Buffer;

  THC_LOCAL_DEBUG(L"ThcReadHidDescriptor\n")

  ThcCtl.TctlCmdCode  = TCTL_GET_HID_REPORT_DESCRIPTOR;
  ThcCtl.TctlStatus   = TCTL_SUCCESS;

  WriteData.WriteDataType   = TCTL_REQUEST;
  WriteData.WriteDataLength = sizeof (ThcCtl);

  ThcDev->HidDataAvailable = FALSE;
  ThcDev->HidActive        = TRUE;
  ThcDev->HidBuffer        = AllocateAlignedPages (EFI_SIZE_TO_PAGES (THC_MAX_HID_BUFFER_SIZE), sizeof (UINT8));
  if (ThcDev->HidBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Buffer = ThcDev->HidBuffer;

  ZeroMem (ThcDev->HidBuffer, THC_MAX_HID_BUFFER_SIZE);

  Status = ThcDmaWriteTx (ThcDev, (UINT8*) &ThcCtl, &WriteData);
  THC_LOCAL_DEBUG(L"ThcReadHidDescriptor ThcDmaWriteTx: %r\n", Status)
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Timeout = HidResponseTimeout;

  //
  // Wait for Response
  //
  if (Timeout > 0) {
    THC_LOCAL_DEBUG(L"ThcReadHidDescriptor Waiting for Response \n")
    DEBUG ((DEBUG_INFO, "ThcReadHidDescriptor Waiting for Response \n"));
    do {
      if (ThcDev->HidDataAvailable == TRUE && ThcDev->ReadDone == TRUE) {
        Status = EFI_SUCCESS;
        break;
      }
      MicroSecondDelay (1000);
      Timeout--;
    } while (Timeout > 0);

    if (Timeout == 0) {
      THC_LOCAL_DEBUG(L"ThcReadHidDescriptor Response Timeout\n")
      DEBUG ((DEBUG_ERROR, "ThcReadHidDescriptor Response Timeout\n"));
      return EFI_TIMEOUT;
    }
  }

  ThcDev->HidDataAvailable = FALSE;
  ThcDev->HidActive        = FALSE;

  //
  // Restore initial pointer
  //
  ThcDev->HidBuffer = Buffer;
  //
  // Calculate Recieved data length
  //
  HidMessageLength = ((RAW_DATA_HEADER*) (ThcDev->HidBuffer))->ReadDataSize;
  //
  // Shift the pointer to data location
  //
  ThcDev->HidBuffer += sizeof (RAW_DATA_HEADER);
  //
  // Capture TCTL result
  //
  CopyMem (&ThcCtl, ThcDev->HidBuffer, sizeof (THC_CTL));
  DEBUG ((DEBUG_INFO, "ThcReadHidDescriptor\n  ThcCtl.TctlCmdCode = %d\n  ThcCtl.TctlStatus = %d\n", ThcCtl.TctlCmdCode, ThcCtl.TctlStatus));
  THC_LOCAL_DEBUG(L"ThcReadHidDescriptor\n  ThcCtl.TctlCmdCode = %d\n  ThcCtl.TctlStatus = %d\n", ThcCtl.TctlCmdCode, ThcCtl.TctlStatus)
  if (ThcCtl.TctlStatus != TCTL_SUCCESS) {
    return EFI_NO_RESPONSE;
  }

  //
  // Shift the pointer to the actual HID Descriptor
  //
  ThcDev->HidBuffer += sizeof (THC_CTL) -1; // Ignore TctlData
  THC_LOCAL_DEBUG(L"ThcReadHidDescriptor HidMessageLength = %d\n", HidMessageLength)
  //
  // Parse HID data
  //
  HidParseDescriptor (ThcDev, ThcDev->HidBuffer, HidMessageLength - sizeof (THC_CTL));

  THC_LOCAL_DEBUG(L"ThcReadHidDescriptor End\n")
  return Status;
}
