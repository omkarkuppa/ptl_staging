/** @file
  Touch Host Controller QuickSpi Private Lib for Driver usage

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include "QuickSpiPrivate.h"
#include <Protocol/ThcResetProtocol.h>

/**
  Sets TSSGO bit to start a cycle

  @param[in]  MmioBase         QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibGo (
  IN UINT64  MmioBase
  )
{
  THC_LOCAL_DEBUG(L"QuickSpiLibGo\n")
  MmioOr32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL), B_THC_MEM_PRT_SW_SEQ_CNTRL_TSSGO);
}

/**
  Sets Edge Interrupt mode

  @param[in]  MmioBase         QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibSetEdgeInterrupts (
  IN UINT64  MmioBase
  )
{
  MmioOr32 (MmioBase + R_THC_MEM_PRT_TSEQ_CNTRL_1, BIT31);
}

/**
  Returns last received interrupt of THC_INTERRUPT_TYPE

  @param[in]  MmioBase     QuickSpi MMIO BAR0

  @retval                  One of THC_INTERRUPT_TYPE
**/
UINT8
QuickSpiLibInterruptStatus (
  IN UINT64  MmioBase
  )
{
  return (UINT8)(MmioRead32 ((UINTN) (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_1)) & (BIT0|BIT1|BIT2|BIT3));
}

/**
  Waits for SPI_IO_RDY BIT after it was moved to D0

  @param[in]  MmioBase       QuickSpi MMIO BAR0
  @param[in]  Timeout        [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval  EFI_SUCCESS       Device is ready
  @retval  EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
QuickSpiLibIoReady (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  while (Timeout > 0) {
    if (MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL) & B_THC_MEM_PRT_CONTROL_SPI_IO_RDY) {
      return EFI_SUCCESS;
    }
    gBS->Stall (THC_DELAY_PERIOD_1MS);
    Timeout--;
  }
  return EFI_DEVICE_ERROR;
}

/**
  Start Quiesce to de-initialize THC

  @param[in]  MmioBase      QuickSpi MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for HW Sts bit to be 1

  @retval EFI_SUCCESS       Quiesce Enabled
  @retval EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
QuickSpiLibStartQuiesce (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  //
  // Set QUICK_SPI_DEVINT_QUIESCE_EN to 1
  //
  MmioOr32 (MmioBase + R_THC_MEM_PRT_CONTROL, B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_EN);

  //
  // Poll on Quiesce HW Sts bit to be 1
  //
  while (Timeout > 0) {
    THC_LOCAL_DEBUG(L"QuickSpiLibStartQuiesce HW_STS: 0x%X | Timeout: %d\n", MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL), Timeout)
    if (MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL) & B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_HW_STS) {
      return EFI_SUCCESS;
    }
    gBS->Stall (THC_DELAY_PERIOD_1MS);
    Timeout--;
  }
  return EFI_DEVICE_ERROR;
}

/**
  Ends Quiesce

  @param[in] MmioBase       QuickSpi MMIO BAR0
  @param[in] Timeout        [MILLISECONDS] Indicates how long driver waits for HW STS response

  @retval EFI_SUCCESS       Quiesce Disabled
  @retval EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
QuickSpiLibEndQuiesce (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  THC_LOCAL_DEBUG(L"QuickSpiLibEndQuiesce R_THC_MEM_PRT_SPI_CFG 0x%X, VAL: 0x%X\n", R_THC_MEM_PRT_CONTROL, MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL))
  MmioAnd32 (MmioBase + R_THC_MEM_PRT_CONTROL, (UINT32) ~(B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_EN));
  THC_LOCAL_DEBUG(L"QuickSpiLibEndQuiesce R_THC_MEM_PRT_SPI_CFG 0x%X, VAL: 0x%X\n", R_THC_MEM_PRT_CONTROL, MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL))

  //
  // Poll on Quiesce HW Sts bit to be 0
  //
  while (Timeout > 0) {
    if ( MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL) & B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_HW_STS) {
      gBS->Stall (THC_DELAY_PERIOD_1MS);
      Timeout--;
    } else {
     return EFI_SUCCESS;
    }
  }
  return EFI_DEVICE_ERROR;
}

/**
  Check if Quiesce is disabled

  @param[in]  MmioBase  QuickSpi MMIO BAR0

  @retval  TRUE         Quiesce is Disabled
  @retval  FALSE        Quiesce is Enabled
**/
BOOLEAN
QuickSpiLibIsQuiesceDisabled (
  IN UINT64   MmioBase
  )
{
  if (MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL) & (B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_EN | B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_HW_STS)) {
    return FALSE;
  }
  return TRUE;
}

/**
  Sets Port Frequency

  @param[in] MmioBase          QuickSpi MMIO BAR0
  @param[in] Frequency         Read & Write Frequency
**/
VOID
QuickSpiLibSetFrequency (
  IN UINT64               MmioBase,
  IN UINT32               Frequency
  )
{
  UINT32              NewFrequency;
  THC_M_PRT_SPI_CFG   SpiCfg;

  SpiCfg.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_SPI_CFG);
  SpiCfg.Fields.SpiLowFreqEn = 0x0;

  if (Frequency < 17000000) {
    SpiCfg.Fields.SpiLowFreqEn = 0x1;
  }

  //
  // Map Frequency to match THC HW Compliant setting
  //
  if (Frequency < 2100000) {
    NewFrequency = V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_2P1MHZ;
  } else  if (Frequency < 2500000) {
    NewFrequency = V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_2P5MHZ;
  } else  if (Frequency < 3000000) {
    NewFrequency = V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_3MHZ;
  } else  if (Frequency < 3750000) {
    NewFrequency = V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_3P75MHZ;
  } else  if (Frequency < 5000000) {
    NewFrequency = V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_5MHZ;
  } else  if (Frequency < 8000000) {
    NewFrequency = V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_7P5MHZ;
  } else if (Frequency < 17000000) {
    NewFrequency = V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_15MHZ;
  } else if (Frequency < 20000000) {
    NewFrequency = V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_17MHZ;
  } else if (Frequency < 25000000) {
    NewFrequency = V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_20MHZ;
  } else if (Frequency < 30000000) {
    NewFrequency = V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_24MHZ;
  } else if (Frequency < 40000000) {
    NewFrequency = V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_30MHZ;
  } else { // >= 40MHZ
    NewFrequency = V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_40MHZ;
  }

  SpiCfg.Fields.SpiTCRF = NewFrequency;
  SpiCfg.Fields.SpiTCWF = NewFrequency;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SPI_CFG, SpiCfg.Data32);
}


/**
  Sets Single IO mode for both read and write

  @param[in] MmioBase          QuickSpi MMIO BAR0
  @param[in] IoMode            Single/Dual/Quad

**/
VOID
QuickSpiLibSetIoMode (
  IN UINT64   MmioBase,
  IN UINT8    IoMode
  )
{
  THC_M_PRT_SPI_CFG   SpiCfg;

  SpiCfg.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_SPI_CFG);
  SpiCfg.Fields.SpiTrMode = IoMode & (BIT0 | BIT1);
  SpiCfg.Fields.SpiTwMode = IoMode & (BIT0 | BIT1);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SPI_CFG, SpiCfg.Data32);
}

/**
  Waits until cycle is completed

  @param[in]  MmioBase         QuickSpi MMIO BAR0
  @param[in]  Timeout          [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS          Cycle completed
  @retval EFI_DEVICE_ERROR     ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickSpiLibWaitCycleCompleted (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  THC_M_PRT_SW_SEQ_STS  SwSeqSts;

  while (Timeout > 0) {
    SwSeqSts.Data32 = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_STS));
    if (SwSeqSts.Fields.ThcSsCip || !SwSeqSts.Fields.TssDone) {
      gBS->Stall (THC_DELAY_PERIOD_1MS);
      Timeout--;
      if (Timeout % 100 == 0) {
        THC_LOCAL_DEBUG(L"QuickSpiLibWaitCycleCompleted Cycle is still in progress\n")
        DEBUG ((DEBUG_INFO, "QuickSpiLibWaitCycleCompleted Cycle is still in progress\n"));
      }
    } else {
      break;
    }
  }
  if (Timeout == 0) {
    return EFI_TIMEOUT;
  }

  if (SwSeqSts.Fields.ThcSsErr) {
    DEBUG ((DEBUG_ERROR, "QuickSpiLibWaitCycleCompleted EFI_DEVICE_ERROR ThcSsErr is set\n"));
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}

/**
  Checks if no cycles are pending

  @param[in]  MmioBase         QuickSpi MMIO BAR0
  @param[in]  Timeout          [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS          No Cycles running
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickSpiLibNoCyclesPending (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  THC_M_PRT_SW_SEQ_STS  SwSeqSts;

  while (Timeout > 0) {
    SwSeqSts.Data32 = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_STS));
    if (SwSeqSts.Fields.ThcSsCip == 1) {
      gBS->Stall (THC_DELAY_PERIOD_1MS);
      Timeout--;
      if (Timeout % 100 == 0) {
        THC_LOCAL_DEBUG(L"QuickSpiLibNoCyclesPending, Cycle is still in progress\n")
        DEBUG ((DEBUG_INFO, "QuickSpiLibNoCyclesPending, Cycle is still in progress\n"));
      }
    } else {
      return EFI_SUCCESS;
    }
  }
  return EFI_TIMEOUT;
}

/**
  Power up device through RST GPIO

  @param[in] MmioBase           QuickSpi MMIO BAR0
  @param[in] Timeout            [MILLISECONDS] Indicates how long driver waits for device to power up
  @param[in] InstanceId         SPI instance id
  @param[in] ResetPadTrigger    Reset Pad Trigger

**/
VOID
QuickSpiLibGetOutOfReset (
  IN UINT64                    MmioBase,
  IN UINT32                    Timeout,
  IN UINT8                     InstanceId,
  IN UINT32                    ResetPadTrigger
  )
{
  EFI_STATUS         Status;
  THC_RESET_PROTOCOL *ThcResetProtocol;

  //
  // Locate THC Reset Protocol
  //
  Status = gBS->LocateProtocol (&gThcResetProtocolGuid, NULL, (VOID **) &ThcResetProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  //
  // Place TIC in Reset state
  // Reset Assert
  //
  ThcResetProtocol->SetOutputValue (ThcResetProtocol, InstanceId, (ResetPadTrigger & BIT0));

  gBS->Stall (EFI_TIMER_PERIOD_MILLISECONDS (Timeout));
  //
  // Reset DeAssert
  //
  ThcResetProtocol->SetOutputValue (ThcResetProtocol, InstanceId, !(ResetPadTrigger & BIT0));
}

/**
  Power up device through RST GPIO

  @param[in] MmioBase           QuickSpi MMIO BAR0
  @param[in] InstanceId         SPI instance id
  @param[in] ResetPadTrigger    Reset Pad Trigger

**/
VOID
QuickSpiLibSetReset (
  IN UINT64                    MmioBase,
  IN UINT8                     InstanceId,
  IN UINT32                    ResetPadTrigger
  )
{
  EFI_STATUS         Status;
  THC_RESET_PROTOCOL *ThcResetProtocol;

  //
  // Locate THC Reset Protocol
  //
  Status = gBS->LocateProtocol (&gThcResetProtocolGuid, NULL, (VOID **) &ThcResetProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  //
  // Place TIC in Reset state
  // Reset Assert
  //
  ThcResetProtocol->SetOutputValue (ThcResetProtocol, InstanceId, (ResetPadTrigger & BIT0));
}

/**
  Prepares THC to return its configuration

  @param[in]  Operation              Operation Command
  @param[in]  MmioBase               QuickSpi MMIO BAR0
  @param[in]  ByteCount              Number of bytes
  @param[in]  SpiAddressRegister     SPI Address Register
**/
VOID
QuickSpiLibPreparePioOperation (
  IN PIO_OP   Operation,
  IN UINT64   MmioBase,
  IN UINT16   ByteCount,
  IN UINT32   SpiAddressRegister
  )
{
  THC_M_PRT_SW_SEQ_CNTRL  SwSeqCntrl;

  THC_LOCAL_DEBUG(L"QuickSpiLibPreparePioOperation\n")

  MmioOr32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));

  SwSeqCntrl.Data = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));
  SwSeqCntrl.Fields.QuickSpiSsBc   = ByteCount;
  SwSeqCntrl.Fields.QuickSpiSsCmd  = Operation;
  SwSeqCntrl.Fields.QuickSpiSsCdIe = 0x1; // Enable Interrupt
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL), SwSeqCntrl.Data);

  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA0_ADDR), SpiAddressRegister);
}

/**
  Reads THC device registers

  @param[in]      MmioBase      QuickSpi MMIO BAR0
  @param[in/out]  ReturnData    Return Data Pointer

  @retval EFI_SUCCESS              Cycle completed
  @retval EFI_DEVICE_ERROR         Returned Byte Count is too big or empty
**/
EFI_STATUS
QuickSpiLibReadComplete (
  IN     UINT64  MmioBase,
  IN OUT UINT32  *ReturnData
  )
{
  THC_M_PRT_SW_SEQ_CNTRL  SwSeqCntrl;
  UINT32                  Index;
  UINT32                  DWordCount;

  THC_LOCAL_DEBUG(L"QuickSpiLibReadComplete Start ()\n")

  SwSeqCntrl.Data = MmioRead32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));
  if ((SwSeqCntrl.Fields.QuickSpiSsBc > 64) || (SwSeqCntrl.Fields.QuickSpiSsBc == 0)) {
    return EFI_DEVICE_ERROR;
  }

  DWordCount = SwSeqCntrl.Fields.QuickSpiSsBc / sizeof (UINT32);
  // Go through every DATAx register and read its content
  for (Index = 0; Index < DWordCount; Index++) {
    THC_LOCAL_DEBUG(L"QuickSpiLibReadComplete Offset 0x%X Data: 0x%X\n", (R_THC_MEM_PRT_SW_SEQ_DATA1 + (Index * sizeof (UINT32))), MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA1 + (Index * sizeof (UINT32))))
    ReturnData[Index] = MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA1 + (Index * sizeof (UINT32)));
  }

  MmioOr32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));

  return EFI_SUCCESS;
}

/**
  Waits for TOUCH_INT_CAUSE to return ResetOccurred

  @param[in]  MmioBase      QuickSpi MMIO BAR0

  @retval EFI_SUCCESS       Reset was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
STATIC
QuickSpiLibConfirmResetResponse (
  IN UINT64   MmioBase
  )
{
  EFI_STATUS                          Status;
  QUICK_SPI_INPUT_REPORT_BODY_HEADER  InputReportBody;
  THC_M_PRT_INPUT_REPORT_HEADER_VAL   InputReportHeader;

  Status = QuickSpiLibNoCyclesPending (MmioBase, CycleTimeout);
  THC_LOCAL_DEBUG(L"QuickSpiLibConfirmResetResponse  QuickSpiLibNoCyclesPending Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiLibConfirmResetResponse error, Status %r\n", Status));
    return Status;
  }

  InputReportHeader.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_DEV_INT_CAUSE_REG_VAL);

  QuickSpiLibPreparePioOperation (
    QuickSpiPioRead,
    MmioBase,
    (UINT16) InputReportHeader.Fields.InputReportLength * sizeof (UINT32),
    MmioRead32 (MmioBase + R_THC_MEM_PRT_RD_BULK_ADDR_1) // QUICK_SPI_DEFAULT_INPUT_REPORT_BODY_ADDRESS
    );
  QuickSpiLibGo (MmioBase);

  Status = QuickSpiLibWaitCycleCompleted (MmioBase, CycleTimeout);
  THC_LOCAL_DEBUG (L"QuickSpiLibConfirmResetResponse QuickSpiLibWaitCycleCompleted Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiLibConfirmResetResponse error, Status %r\n", Status));
    return Status;
  }

  QuickSpiLibReadComplete (MmioBase, &InputReportBody.Data32);

  if (InputReportBody.Fields.ReportType == QuickSpiInputReportResetResponse) {
    return EFI_SUCCESS;
  }

  return EFI_DEVICE_ERROR;
}

/**
  Waits for TOUCH_INT_CAUSE to return ResetOccurred

  @param[in]  MmioBase      QuickSpi MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS       Reset was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
QuickSpiLibWaitForResetOccured (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  THC_M_PRT_READ_INT_STS  ReadIntSts;
  while (Timeout > 0) {
    ReadIntSts.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_1);
    if (ReadIntSts.Fields.ThcRdDmaNonDmaIntStatus == 1) {
      MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_1, ReadIntSts.Data32);
      return QuickSpiLibConfirmResetResponse (MmioBase);
    }
    gBS->Stall (THC_DELAY_PERIOD_1MS);
    Timeout--;
    if (Timeout % 100 == 0) {
      THC_LOCAL_DEBUG(L"QuickSpiLibWaitForResetOccured R_THC_MEM_PRT_READ_DMA_INT_STS_1: 0x%X\n", MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_1))
      DEBUG ((DEBUG_INFO, "QuickSpiLibWaitForResetOccured Timeout %d\n", Timeout));
    }
  }

  return EFI_TIMEOUT;
}

/**
  Waits for TOUCH_INT_CAUSE to return ResetOccurred

  @param[in]  MmioBase              QuickSpi MMIO BAR0
  @param[in]  DeviceDescriptor      Pointer to Device Descriptor

  @retval EFI_SUCCESS       Reset was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
STATIC
QuickSpiLibConfirmDescResponse (
  IN UINT64       MmioBase,
  IN OUT UINT32   *DeviceDescriptor
  )
{
  EFI_STATUS Status;
  QUICK_SPI_INPUT_REPORT_BODY_HEADER  InputReportBody;
  THC_M_PRT_INPUT_REPORT_HEADER_VAL   InputReportHeader;
  UINT8                               *Data;

  Data = AllocateZeroPool (sizeof (QUICK_SPI_INPUT_REPORT_BODY_HEADER) + sizeof (QUICK_SPI_DEVICE_DESCRIPTOR));

  Status = QuickSpiLibNoCyclesPending (MmioBase, CycleTimeout);
  THC_LOCAL_DEBUG(L"QuickSpiLibConfirmDescResponse  QuickSpiLibNoCyclesPending Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiLibConfirmDescResponse error, Status %r\n", Status));
    return Status;
  }

  InputReportHeader.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_DEV_INT_CAUSE_REG_VAL);

  QuickSpiLibPreparePioOperation (
    QuickSpiPioRead,
    MmioBase,
    (UINT16) InputReportHeader.Fields.InputReportLength * sizeof (UINT32),
    MmioRead32 (MmioBase + R_THC_MEM_PRT_RD_BULK_ADDR_1) //QUICK_SPI_DEFAULT_INPUT_REPORT_BODY_ADDRESS
    );
  QuickSpiLibGo (MmioBase);

  Status = QuickSpiLibWaitCycleCompleted (MmioBase, CycleTimeout);
  THC_LOCAL_DEBUG(L"QuickSpiLibConfirmDescResponse  QuickSpiLibWaitCycleCompleted Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiLibConfirmDescResponse error, Status %r\n", Status));
    return Status;
  }

  QuickSpiLibReadComplete (MmioBase, (UINT32*) Data);

  CopyMem (&InputReportBody, Data, sizeof (QUICK_SPI_INPUT_REPORT_BODY_HEADER)); // Ignore Report body

  DEBUG ((DEBUG_WARN, "QuickSpiLibConfirmDescResponse InputReportBody.Fields.ReportType = %d\n", InputReportBody.Fields.ReportType));
  DEBUG ((DEBUG_WARN, "QuickSpiLibConfirmDescResponse InputReportBody.Fields.ContentLength = %d\n", InputReportBody.Fields.ContentLength));

  if (InputReportBody.Fields.ReportType == QuickSpiInputReportDeviceDescriptor) {
    CopyMem (DeviceDescriptor, Data + sizeof (QUICK_SPI_INPUT_REPORT_BODY_HEADER), sizeof (QUICK_SPI_DEVICE_DESCRIPTOR)); // Ignore Report body
    return EFI_SUCCESS;
  }

  return EFI_DEVICE_ERROR;
}

/**
  Waits for TOUCH_INT_CAUSE to return ResetOccurred

  @param[in]  MmioBase      QuickSpi MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS       Reset was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
QuickSpiLibWaitForDeviceDescriptor (
  IN UINT64       MmioBase,
  IN UINT32       Timeout,
  IN OUT UINT32   *DeviceDescriptor
  )
{
  THC_M_PRT_READ_INT_STS  ReadIntSts;
  while (Timeout > 0) {
    ReadIntSts.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_1);
    if (ReadIntSts.Fields.ThcRdDmaNonDmaIntStatus == 1) {
      MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_1, ReadIntSts.Data32);
      return QuickSpiLibConfirmDescResponse (MmioBase, DeviceDescriptor);
    }
    gBS->Stall (THC_DELAY_PERIOD_1MS);
    Timeout--;
    if (Timeout % 100 == 0) {
      THC_LOCAL_DEBUG(L"QuickSpiLibWaitForDeviceDescriptor R_THC_MEM_PRT_READ_DMA_INT_STS_1: 0x%X\n", MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_1))
      DEBUG ((DEBUG_INFO, "QuickSpiLibWaitForDeviceDescriptor Timeout %d\n", Timeout));
    }
  }

  return EFI_TIMEOUT;
}

/**
  Write to sequencing Data 1

  @param[in]  MmioBase         QuickSpi MMIO BAR0
  @param[in]  Data             Data

**/
VOID
QuickSpiLibWriteSeqData1 (
  IN UINT64  MmioBase,
  IN UINT32  Data
  )
{
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA1), Data);
}

/**
  Sets Prd Tables Adddress in THC MMIO

  @param[in]  MmioBase            QuickSpi MMIO BAR0
  @param[in]  WriteAddr           Write Prd Address
  @param[in]  Read2Addr           Read2 Prd Address

  @retval EFI_SUCCESS             Cycle completed
  @retval EFI_INVALID_PARAMETER   Returned Byte Count is too big or empty
**/
EFI_STATUS
QuickSpiLibSetPrdTablesAddress (
  IN UINT64  MmioBase,
  IN UINT64  WriteAddr,
  IN UINT64  Read2Addr
  )
{
  THC_LOCAL_DEBUG(L"QuickSpiLibSetPrdTablesAddress Read2Addr: 0x%X Write: 0x%X \n", Read2Addr, WriteAddr)
  if ((Read2Addr & EFI_PAGE_MASK) || (WriteAddr & EFI_PAGE_MASK)) {
    THC_LOCAL_DEBUG(L"QuickSpiLibSetPrdTablesAddress WriteRead Address not 4K alligned first 12 bits should be 0. Read2Addr: 0x%X Write: 0x%X \n", Read2Addr, WriteAddr)
    DEBUG ((DEBUG_WARN, "QuickSpiLibSetPrdTablesAddress WriteRead Address not 4K alligned first 12 bits should be 0. Read2Addr: 0x%X Write: 0x%X \n", Read2Addr, WriteAddr));
    return EFI_INVALID_PARAMETER;
  }

  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WPRD_BA_LOW, WriteAddr & 0xFFFFF000);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WPRD_BA_HI, (UINT32) RShiftU64 ((UINTN) WriteAddr, 32));

  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_BA_LOW_2, Read2Addr & 0xFFFFF000);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_BA_HI_2, (UINT32) RShiftU64 ((UINTN) Read2Addr, 32));

  return EFI_SUCCESS;
}

/**
  Sets PrdTable/CircularBuffer length

  @param[in]  MmioBase        QuickSpi MMIO BAR0
  @param[in]  ReadEntryCount  Number Of Read Entries
  @param[in]  ReadCBDepth     Read Circullar Buffer Depth
  @param[in]  WriteEntryCount Number of Write Entries

**/
VOID
QuickSpiLibSetLengthInPrd (
  IN UINT64  MmioBase,
  IN UINT8   ReadEntryCount,
  IN UINT8   ReadCBDepth,
  IN UINT8   WriteEntryCount
  )
{
  THC_M_PRT_RPRD_CNTRL_2       ReadRprdCntrl2;
  THC_M_PRT_WRITE_DMA_CNTRL    WriteDmaCntrl;

  THC_LOCAL_DEBUG(L"QuickSpiLibSetLengthInPrd ReadEntryCount: %d WriteEntryCount: %d ReadCBDepth %d\n", ReadEntryCount, WriteEntryCount, ReadCBDepth)

  ReadRprdCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_2);
  ReadRprdCntrl2.Fields.Ptec = ReadEntryCount - 1; // 0 MmioBased
  ReadRprdCntrl2.Fields.Pcd  = ReadCBDepth - 1; // 0 MmioBased
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_2, ReadRprdCntrl2.Data);

  WriteDmaCntrl.Data  = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
  WriteDmaCntrl.Fields.QuickSpiWrDmaPtec = WriteEntryCount - 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL, WriteDmaCntrl.Data);
}

/**
  Sets Write DMA start bit

  @param[in]  MmioBase           QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibSetWriteStartBit (
  IN UINT64  MmioBase
  )
{
  THC_LOCAL_DEBUG(L"QuickSpiLibSetWriteStartBit ()\n")

  THC_M_PRT_WRITE_DMA_CNTRL  WriteDmaCntrl;
  WriteDmaCntrl.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
  WriteDmaCntrl.Fields.QuickSpiWrDmaIeDmaCpl = 1;
  WriteDmaCntrl.Fields.QuickSpiWrDmaStart    = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL, WriteDmaCntrl.Data);
}

/**
  Polls until DMA Complete bit is set

  @param[in]  MmioBase       QuickSpi MMIO BAR0
  @param[in]  Timeout        [MILLISECONDS] Indicates how long polling lasts

  @retval  EFI_SUCCESS       DMA transfer is complete
  @retval  EFI_TIMEOUT       Timeout reached
**/
EFI_STATUS
QuickSpiLibPollDmaComplete (
  IN UINT64  MmioBase,
  IN UINT32  Timeout
  )
{
  THC_M_PRT_WRITE_DMA_CNTRL  WriteDmaCntrl;
  THC_M_PRT_DMA_INT_STS      WriteDmaIntSts;

  THC_LOCAL_DEBUG(L"QuickSpiLibPollDmaComplete ()\n")

  while (Timeout > 0) {
    WriteDmaCntrl.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
    if (WriteDmaCntrl.Fields.QuickSpiWrDmaStart == 0) {
      WriteDmaIntSts.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_INT_STS);
      WriteDmaIntSts.Fields.ThcWrDmaCmplStatus = 1;
      MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_INT_STS, WriteDmaIntSts.Data);
      return EFI_SUCCESS;
    }
    MicroSecondDelay (1);
    Timeout--;
  }
  return EFI_TIMEOUT;
}

/**
  Sets Read Rx2 DMA start bit

  @param[in]  MmioBase           QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibSetReadRx2StartBit (
  IN UINT64  MmioBase
  )
{
  THC_LOCAL_DEBUG(L"QuickSpiLibSetReadRx2StartBit ()\n")

  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;
  ReadDmaCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
  ReadDmaCntrl2.Fields.IeEof = 1;
  ReadDmaCntrl2.Fields.QuickSpiPrdCbWritePointer = POINTER_WRAPAROUND;
  ReadDmaCntrl2.Fields.Start = 1;
  ReadDmaCntrl2.Fields.IntSwDmaEn = 0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);
}

/**
  Sets Read Rx2 DMA write pointer

  @param[in]  MmioBase       QuickSpi MMIO BAR0
  @param[in]  WritePointer   PRD CB Write Pointer

**/
VOID
QuickSpiLibSetReadRx2WritePointer (
  IN UINT64  MmioBase,
  IN UINT8   WritePointer
  )
{
  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;
  ReadDmaCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
  ReadDmaCntrl2.Fields.QuickSpiPrdCbWritePointer = WritePointer;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);
}

/**
  Sets Read Rx2 DMA write pointer

  @param[in]  MmioBase           QuickSpi MMIO BAR0

  @retval     Contents of Dma Control 2 register
**/
UINT32
QuickSpiLibGetReadRx2Data (
  IN UINT64  MmioBase
  )
{
  return MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
}

/**
  Prepares Touch Panel and THC for OS hand off

  @param[in]  MmioBase           QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibCleanUp (
  IN UINT64  MmioBase
  )
{
  THC_LOCAL_DEBUG(L"QuickSpiLibCleanUp ()\n")
  THC_M_PRT_READ_DMA_CNTRL_2  ReadDmaCntrl2;
  THC_M_PRT_WRITE_DMA_CNTRL   WriteDmaCntrl;
  THC_M_PRT_DMA_INT_STS       WriteDmaIntSts;
  THC_M_PRT_SPI_ICRRD_OPCODE  IcrrdOpCode;
  THC_M_PRT_SPI_WR_OPCODE     WrOpCode;
  THC_M_PRT_DEVINT_CFG_1      DevIntCfg1;
  THC_M_PRT_DEVINT_CFG_2      DevIntCfg2;
  THC_M_PRT_SPI_CFG           SpiCfg;
  THC_M_PRT_SPI_DMARD_OPCODE  DmaRdOpCode;

  //
  // Clear OpCodes
  //
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_DEV_INT_CAUSE_ADDR, 0x0);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RD_BULK_ADDR_1, 0x0);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RD_BULK_ADDR_2, 0x0);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WR_BULK_ADDR, 0x0);

  IcrrdOpCode.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_SPI_ICRRD_OPCODE);
  IcrrdOpCode.Fields.SpiQioIcrrdOpCode = 0x0;
  IcrrdOpCode.Fields.SpiDioIcrrdOpCode = 0x0;
  IcrrdOpCode.Fields.SpiSioIcrrdOpCode = 0x0;
  IcrrdOpCode.Fields.SpiQpiIcrrdOpCode = 0x0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SPI_ICRRD_OPCODE, IcrrdOpCode.Data32);

  DmaRdOpCode.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_SPI_DMARD_OPCODE);
  DmaRdOpCode.Fields.SpiQpiDmaRdOpCode = 0x0;
  DmaRdOpCode.Fields.SpiQioDmaRdOpCode = 0x0;
  DmaRdOpCode.Fields.SpiDioDmaRdOpCode = 0x0;
  DmaRdOpCode.Fields.SpiSioDmaRdOpCode = 0x0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SPI_DMARD_OPCODE, DmaRdOpCode.Data32);

  WrOpCode.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_SPI_WR_OPCODE);
  WrOpCode.Fields.SpiQpiWrOpCode = 0x0;
  WrOpCode.Fields.SpiQioWrOpCode = 0x0;
  WrOpCode.Fields.SpiDioWrOpCode = 0x0;
  WrOpCode.Fields.SpiSioWrOpCode = 0x0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SPI_WR_OPCODE, WrOpCode.Data32);

  DevIntCfg1.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_DEVINT_CFG_1);;
  DevIntCfg1.Fields.IntTypOffset  = 0x0;
  DevIntCfg1.Fields.IntTypLen     = 0x0;
  DevIntCfg1.Fields.EofOffset     = 0x0;
  DevIntCfg1.Fields.IntTypDataVal = 0x0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_DEVINT_CFG_1, DevIntCfg1.Data32);

  DevIntCfg2.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_DEVINT_CFG_2);
  DevIntCfg2.Fields.TxDmaPktStrmEn  = 0x0;
  DevIntCfg2.Fields.RxDmaAddrIncDis = 0x0;
  DevIntCfg2.Fields.TxDmaAddrIncDis = 0x0;
  DevIntCfg2.Fields.FTypeIgnore     = 0x0;
  DevIntCfg2.Fields.FTypeVal        = 0x0;
  DevIntCfg2.Fields.UfSizeOffset    = 0x0;
  DevIntCfg2.Fields.UfSizeLen       = 0x0;
  DevIntCfg2.Fields.UfSizeUnit      = 0x0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_DEVINT_CFG_2, DevIntCfg2.Data32);

  SpiCfg.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_SPI_CFG);
  SpiCfg.Fields.SpiTrMode = 0x0;
  SpiCfg.Fields.SpiTCRF   = 0x0;
  SpiCfg.Fields.SpiRdMps  = 0x0;
  SpiCfg.Fields.SpiTwMode = 0x0;
  SpiCfg.Fields.SpiTCWF   = 0x0;
  SpiCfg.Fields.SpiWrMps  = 0x0;
  SpiCfg.Fields.SpiLowFreqEn = 0x0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SPI_CFG, SpiCfg.Data32);

  ReadDmaCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
  ReadDmaCntrl2.Fields.IeEof = 0;
  ReadDmaCntrl2.Fields.QuickSpiPrdCbWritePointer = 0;
  ReadDmaCntrl2.Fields.Start = 0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);

  WriteDmaCntrl.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
  WriteDmaCntrl.Fields.QuickSpiWrDmaIeDmaCpl = 0;
  WriteDmaCntrl.Fields.QuickSpiWrDmaStart    = 0;
  WriteDmaCntrl.Fields.QuickSpiWrDmaPtec     = 0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL, WriteDmaCntrl.Data);

  WriteDmaIntSts.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_INT_STS);
  WriteDmaIntSts.Fields.ThcWrDmaCmplStatus = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_INT_STS, WriteDmaIntSts.Data);

  QuickSpiLibSetReadRx2WritePointer (MmioBase, 0);
  QuickSpiLibSetPrdTablesAddress (MmioBase, 0, 0);
  QuickSpiLibSetLengthInPrd (MmioBase, 1, 1, 1);
  QuickSpiLibStartQuiesce (MmioBase, StartQuiesceTimeout);
}

/**
  Read Int Cause Register in PIO mode.

  @param[in]  MmioBase          QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibPioReadIntCause (
  IN UINT64   MmioBase
  )
{
  THC_M_PRT_SW_SEQ_CNTRL  SwSeqCntrl;

  THC_LOCAL_DEBUG(L"QuickSpiLibPioReadIntCause\n")

  MmioOr32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));
  SwSeqCntrl.Data = MmioRead32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));

  SwSeqCntrl.Fields.QuickSpiSsBc  = 4; // 4 bytes
  SwSeqCntrl.Fields.QuickSpiSsCmd = QuickSpiPioRead;

  MmioWrite32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL), SwSeqCntrl.Data);

  QuickSpiLibGo (MmioBase);

  SwSeqCntrl.Data = MmioRead32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));
  if ((SwSeqCntrl.Fields.QuickSpiSsBc > 64) || (SwSeqCntrl.Fields.QuickSpiSsBc == 0)) {
    THC_LOCAL_DEBUG(L"Error. Wrong data size QuickSpiSsBc = 0x%X.\n", SwSeqCntrl.Fields.QuickSpiSsBc)
    return;
  }

  THC_LOCAL_DEBUG(L"QuickSpiLibPioReadIntCause Register Offset 0x%X Data: 0x%X\n", R_THC_MEM_PRT_SW_SEQ_DATA1, MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA1))

  MmioOr32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));
}

/**
  Configures THC registers for QuickSpi support

  @param[in]  MmioBase         QuickSpi MMIO BAR0
**/
VOID
QuickSpiLibConfigure (
  IN UINT64  MmioBase,
  IN UINT32  InputReportHeaderAddress,
  IN UINT32  InputReportBodyAddress,
  IN UINT32  OutputReportAddres,
  IN UINT8   SioReadOpCode,
  IN UINT8   DioReadOpCode,
  IN UINT8   QioReadOpCode,
  IN UINT8   QpioReadOpCode,
  IN UINT8   SioWriteOpCode,
  IN UINT8   DioWriteOpCode,
  IN UINT8   QioWriteOpCode,
  IN UINT8   QpioWriteOpCode
  )
{
  THC_M_PRT_SPI_ICRRD_OPCODE  IcrrdOpCode;
  THC_M_PRT_SPI_DMARD_OPCODE  DmaRdOpCode;
  THC_M_PRT_SPI_WR_OPCODE     WrOpCode;
  THC_M_PRT_DEVINT_CFG_1      DevIntCfg1;
  THC_M_PRT_DEVINT_CFG_2      DevIntCfg2;
  THC_M_PRT_SPI_CFG           SpiCfg;

  MmioWrite32 (MmioBase + R_THC_MEM_PRT_DEV_INT_CAUSE_ADDR, InputReportHeaderAddress);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RD_BULK_ADDR_1, InputReportBodyAddress);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RD_BULK_ADDR_2, InputReportBodyAddress);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WR_BULK_ADDR, OutputReportAddres);

  IcrrdOpCode.Fields.SpiQpiIcrrdOpCode = QpioReadOpCode;
  IcrrdOpCode.Fields.SpiQioIcrrdOpCode = QioReadOpCode;
  IcrrdOpCode.Fields.SpiDioIcrrdOpCode = DioReadOpCode;
  IcrrdOpCode.Fields.SpiSioIcrrdOpCode = SioReadOpCode;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SPI_ICRRD_OPCODE, IcrrdOpCode.Data32);

  DmaRdOpCode.Fields.SpiQpiDmaRdOpCode = QpioReadOpCode;
  DmaRdOpCode.Fields.SpiQioDmaRdOpCode = QioReadOpCode;
  DmaRdOpCode.Fields.SpiDioDmaRdOpCode = DioReadOpCode;
  DmaRdOpCode.Fields.SpiSioDmaRdOpCode = SioReadOpCode;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SPI_DMARD_OPCODE, DmaRdOpCode.Data32);

  WrOpCode.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_SPI_WR_OPCODE);
  WrOpCode.Fields.SpiQpiWrOpCode = QpioWriteOpCode;
  WrOpCode.Fields.SpiQioWrOpCode = QioWriteOpCode;
  WrOpCode.Fields.SpiDioWrOpCode = DioWriteOpCode;
  WrOpCode.Fields.SpiSioWrOpCode = SioWriteOpCode;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SPI_WR_OPCODE, WrOpCode.Data32);

  DevIntCfg1.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_DEVINT_CFG_1);
  DevIntCfg1.Fields.IntTypOffset  = QUICK_SPI_BIT_OFFSET_INTERRUPT_TYPE;
  DevIntCfg1.Fields.IntTypLen     = QUICK_SPI_BIT_LENGTH_INTERRUPT_TYPE;
  DevIntCfg1.Fields.EofOffset     = QUICK_SPI_BIT_OFFSET_LAST_FRAGMENT_FLAG;
  DevIntCfg1.Fields.IntTypDataVal = QUICK_SPI_BITMASK_INVALID_TYPE_DATA << 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_DEVINT_CFG_1, DevIntCfg1.Data32);

  DevIntCfg2.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_DEVINT_CFG_2);
  DevIntCfg2.Fields.TxDmaPktStrmEn  = 0x0;
  DevIntCfg2.Fields.RxDmaAddrIncDis = 0x0;
  DevIntCfg2.Fields.TxDmaAddrIncDis = 0x0;
  DevIntCfg2.Fields.FTypeIgnore     = 0x1;
  DevIntCfg2.Fields.FTypeVal        = 0x1;
  DevIntCfg2.Fields.UfSizeOffset    = QUICK_SPI_BIT_OFFSET_MICROFRAME_SIZE;
  DevIntCfg2.Fields.UfSizeLen       = QUICK_SPI_BIT_LENGTH_MICROFRAME_SIZE;
  DevIntCfg2.Fields.UfSizeUnit      = QUICK_SPI_UNIT_MICROFRAME_SIZE;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_DEVINT_CFG_2, DevIntCfg2.Data32);

  SpiCfg.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_SPI_CFG);
  SpiCfg.Fields.SpiRdMps = QUICK_SPI_DEFAULT_MAX_PACKET_SIZE;
  SpiCfg.Fields.SpiWrMps = QUICK_SPI_DEFAULT_MAX_PACKET_SIZE_MIN;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SPI_CFG, SpiCfg.Data32);
}

/**
  Sets Read Rx2 DMA Interrupt Status

  @param[in]  MmioBase           Thc MMIO BAR0

  @retval     Contents of Dma Interrupt Status 2 register
**/
UINT32
QuickSpiLibGetReadRx2IntSts (
  IN UINT64  MmioBase
  )
{
  return MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_2);
}

/**
  Stops DMA traffic by clearing Start BIT and CB Pointer Reset for the 2nd RXDMA Read Rx2 DMA Interrupt Status

  @param[in]  MmioBase           Thc MMIO BAR0

**/
VOID
QuickSpiLibStopDma (
  IN UINT64  MmioBase
  )
{
  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;
  ReadDmaCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
  ReadDmaCntrl2.Fields.Start                     = 0;
  ReadDmaCntrl2.Fields.QuickSpiPrdCbPointerReset = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);
}