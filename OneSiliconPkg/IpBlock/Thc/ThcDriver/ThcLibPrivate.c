/** @file
  Touch Host Controller Private Lib for Driver usage

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

#include "ThcPrivate.h"

/**
  Sets TSSGO bit to start a cycle

  @param[in]  MmioBase         Thc MMIO BAR0

**/
VOID
ThcLibGo (
  IN UINT64  MmioBase
  )
{
  THC_LOCAL_DEBUG(L"ThcLibGo\n")
  MmioOr32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL), B_THC_MEM_PRT_SW_SEQ_CNTRL_TSSGO);
}

/**
  Sets Edge Interrupt mode

  @param[in]  MmioBase         Thc MMIO BAR0

**/
VOID
ThcLibSetEdgeInterrupts (
  IN UINT64  MmioBase
  )
{
  MmioOr32 (MmioBase + R_THC_MEM_PRT_TSEQ_CNTRL_1, BIT31);
}

/**
  Returns last received interrupt of THC_INTERRUPT_TYPE

  @param[in]  MmioBase     Thc MMIO BAR0

  @retval                  One of THC_INTERRUPT_TYPE
**/
UINT8
ThcLibInterruptStatus (
  IN UINT64  MmioBase
  )
{
  return (UINT8)(MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_DEV_INT_CAUSE_REG_VAL)) & (BIT0|BIT1|BIT2|BIT3));
}

/**
  Waits for SPI_IO_RDY BIT after it was moved to D0

  @param[in]  MmioBase       Thc MMIO BAR0
  @param[in]  Timeout        [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval  EFI_SUCCESS       Device is ready
  @retval  EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
ThcLibIoReady (
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

  @param[in]  MmioBase      Thc MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for HW Sts bit to be 1

  @retval EFI_SUCCESS       Quiesce Enabled
  @retval EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
ThcLibStartQuiesce (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  //
  // Set THC_DEVINT_QUIESCE_EN to 1
  //
  MmioOr32 (MmioBase + R_THC_MEM_PRT_CONTROL, B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_EN);

  //
  // Poll on Quiesce HW Sts bit to be 1
  //
  while (Timeout > 0) {
    THC_LOCAL_DEBUG(L"ThcLibStartQuiesce HW_STS: 0x%X | Timeout: %d\n", MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL), Timeout)
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

  @param[in] MmioBase       Thc MMIO BAR0
  @param[in] Timeout        [MILLISECONDS] Indicates how long driver waits for HW STS response

  @retval EFI_SUCCESS       Quiesce Disabled
  @retval EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
ThcLibEndQuiesce (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  THC_LOCAL_DEBUG(L"ThcLibEndQuiesce R_THC_MEM_PRT_SPI_CFG 0x%X, VAL: 0x%X\n", R_THC_MEM_PRT_CONTROL, MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL))
  MmioAnd32 (MmioBase + R_THC_MEM_PRT_CONTROL, (UINT32) ~(B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_EN));
  THC_LOCAL_DEBUG(L"ThcLibEndQuiesce R_THC_MEM_PRT_SPI_CFG 0x%X, VAL: 0x%X\n", R_THC_MEM_PRT_CONTROL, MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL))

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

  @param[in]  MmioBase  Thc MMIO BAR0

  @retval  TRUE         Quiesce is Disabled
  @retval  FALSE        Quiesce is Enabled
**/
BOOLEAN
ThcLibIsQuiesceDisabled (
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

  @param[in] MmioBase    Thc MMIO BAR0
  @param[in] TypeShift   Frequency shift value for read or write
  @param[in] Frequency   Frequency value

**/
VOID
ThcLibSetPortFrequency (
  IN UINT64               MmioBase,
  IN UINT32               TypeShift,
  IN THC_PORT_FREQUENCY   Frequency
  )
{
  UINT32  Data32;

  Data32 = 0x0;

  if (Frequency <= ThcFreqLow15MHz) {
    Data32 |= B_THC_MEM_PRT_SPI_CFG_SPI_LOW_FREQ_EN;
  }

  switch (Frequency) {
    case ThcFreqLow2p1MHz:
      Data32 |= (V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_2P1MHZ << TypeShift);
      break;
    case ThcFreqLow2p5MHz:
      Data32 |= (V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_2P5MHZ << TypeShift);
      break;
    case ThcFreqLow3MHz:
      Data32 |= (V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_3MHZ << TypeShift);
      break;
    case ThcFreqLow3p75MHz:
      Data32 |= (V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_3P75MHZ << TypeShift);
      break;
    case ThcFreqLow5MHz:
      Data32 |= (V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_5MHZ << TypeShift);
      break;
    case ThcFreqLow7p5MHz:
      Data32 |= (V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_7P5MHZ << TypeShift);
      break;
    case ThcFreqLow15MHz:
      Data32 |= (V_THC_MEM_PRT_SPI_CFG_LOW_FREQ_15MHZ << TypeShift);
      break;
    case ThcFreqHigh17MHz:
      Data32 |= (V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_17MHZ << TypeShift);
      break;
    case ThcFreqHigh20MHz:
      Data32 |= (V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_20MHZ << TypeShift);
      break;
    case ThcFreqHigh24MHz:
      Data32 |= (V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_24MHZ << TypeShift);
      break;
    case ThcFreqHigh30MHz:
      Data32 |= (V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_30MHZ << TypeShift);
      break;
    case ThcFreqHigh40MHz:
      Data32 |= (V_THC_MEM_PRT_SPI_CFG_HIGH_FREQ_40MHZ << TypeShift);
      break;
    default:
    case ThcFreqInvalid:
      ASSERT (FALSE);
      return;
  }

  MmioAndThenOr32 (MmioBase + R_THC_MEM_PRT_SPI_CFG,
                   (UINT32) ~(B_THC_MEM_PRT_SPI_CFG_SPI_LOW_FREQ_EN | (B_THC_MEM_PRT_SPI_CFG_FREQ << TypeShift)),
                    Data32);
}

/**
  Sets Single IO mode for both read and write

  @param[in] MmioBase          Thc MMIO BAR0

**/
VOID
ThcLibSetSingleIoMode (
  IN UINT64   MmioBase
  )
{
  UINT32 Data32;

  Data32 = V_THC_MEM_PRT_SPI_CFG_SINLGE_IO;

  MmioAndThenOr32 (MmioBase + R_THC_MEM_PRT_SPI_CFG,
                   (UINT32) ~((B_THC_MEM_PRT_SPI_CFG_SPI_TRDC << N_THC_MEM_PRT_SPI_CFG_SPI_TRMODE)),
                    Data32);
  MmioAndThenOr32 (MmioBase + R_THC_MEM_PRT_SPI_CFG,
                   (UINT32) ~((B_THC_MEM_PRT_SPI_CFG_SPI_TRDC << N_THC_MEM_PRT_SPI_CFG_SPI_TWMODE)),
                    Data32);
}

/**
  Power down device through GPIO

  @param[in] MmioBase          Thc MMIO BAR0

**/
VOID
ThcLibReset (
  IN UINT64   MmioBase
  )
{
  //
  // Reset Assert
  //
  MmioAndThenOr32 (MmioBase + R_THC_MEM_PRT_CONTROL, (UINT32) ~ (B_THC_MEM_PRT_CONTROL_DEVRST), 0);
  THC_LOCAL_DEBUG(L"ThcLibReset R_THC_MEM_PRT_CONTROL 0x%X, VAL: 0x%X\n", R_THC_MEM_PRT_CONTROL, MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL))
}

/**
  Power up device through RST GPIO

  @param[in] MmioBase       Thc MMIO BAR0
  @param[in] Timeout       [MILLISECONDS] Indicates how long driver waits for device to power up

**/
VOID
ThcLibGetOutOfReset (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  //
  // Check If THC is already In Reset state
  //
  THC_LOCAL_DEBUG(L"ThcLibGetOutOfReset R_THC_MEM_PRT_CONTROL 0x%X, VAL: 0x%X\n", R_THC_MEM_PRT_CONTROL, MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL))
  if ((MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL) & B_THC_MEM_PRT_CONTROL_DEVRST)) {
    ThcLibReset (MmioBase);
    gBS->Stall (EFI_TIMER_PERIOD_MILLISECONDS(Timeout));
  }
  //
  // Reset DeAssert
  //
  MmioOr32 (MmioBase + R_THC_MEM_PRT_CONTROL, B_THC_MEM_PRT_CONTROL_DEVRST);
  THC_LOCAL_DEBUG(L"ThcLibGetOutOfReset R_THC_MEM_PRT_CONTROL 0x%X, VAL: 0x%X\n", R_THC_MEM_PRT_CONTROL, MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL))
}

/**
  Waits for TOUCH_INT_CAUSE to return ResetOccurred

  @param[in]  MmioBase      Thc MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS       Reset was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
ThcLibWaitForResetOccured (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  while (Timeout > 0) {
    if (ThcLibInterruptStatus (MmioBase) == ResetOccurred) {
      return EFI_SUCCESS;
    }
    gBS->Stall (THC_DELAY_PERIOD_1MS);
    Timeout--;
    if (Timeout % 100 == 0) {
      THC_LOCAL_DEBUG(L"ThcLibWaitForResetOccured Timeout %d InterruptStatus %d \n", Timeout, ThcLibInterruptStatus (MmioBase))
      DEBUG ((DEBUG_INFO, "ThcLibWaitForResetOccured Timeout %d\n", Timeout));
    }
  }
  return EFI_TIMEOUT;
}

/**
  Waits for TOUCH_INT_CAUSE to return ConfigChange

  @param[in]  MmioBase      Thc MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS       Reset was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
ThcLibWaitForConfigChange (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  while (Timeout > 0) {
    if (ThcLibInterruptStatus (MmioBase) == ConfigChange) {
      return EFI_SUCCESS;
    }
    gBS->Stall (THC_DELAY_PERIOD_1MS);
    Timeout--;
    if (Timeout % 100 == 0) {
      THC_LOCAL_DEBUG(L"ThcLibWaitForConfigChange Timeout %d InterruptStatus %d \n", Timeout, ThcLibInterruptStatus (MmioBase))
      DEBUG ((DEBUG_INFO, "ThcLibWaitForConfigChange Timeout %d\n", Timeout));
    }
  }
  return EFI_TIMEOUT;
}

/**
  Checks if any cycle is currently being served by THC

  @param[in]  MmioBase Thc MMIO BAR0

  @retval TRUE         Cycle is in progress
  @retval FALSE        No cycles running
**/
BOOLEAN
ThcLibCycleInProgress (
  IN UINT64   MmioBase
  )
{
  if (MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_STS) & B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_CIP) {
    return TRUE;
  }
  return FALSE;
}

/**
  Waits until cycle is completed

  @param[in]  MmioBase         Thc MMIO BAR0
  @param[in]  Timeout          [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS          Cycle completed
  @retval EFI_DEVICE_ERROR     TSSDONE not set or ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
ThcLibWaitCycleCompleted (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  while (Timeout > 0) {
    if (ThcLibCycleInProgress (MmioBase)) {
      gBS->Stall (THC_DELAY_PERIOD_1MS);
      Timeout--;
      if (Timeout % 100 == 0) {
        THC_LOCAL_DEBUG(L"ThcLibWaitCycleCompleted Cycle is still in progress\n")
        DEBUG ((DEBUG_INFO, "ThcLibWaitCycleCompleted Cycle is still in progress\n"));
      }
    } else {
      break;
    }
  }
  if (Timeout == 0) {
    return EFI_TIMEOUT;
  }

  if ((MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_STS)) & (UINT32)(B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR))) { // ERR set
    return EFI_DEVICE_ERROR;
  }
  if (!(MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_STS)) & (UINT32)(B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE))) { // DONE not set
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}

/**
  Checks if no cycles are pending

  @param[in]  MmioBase          Thc MMIO BAR0
  @param[in]  Timeout          [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS          No Cycles running
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
ThcLibNoCyclesPending (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  while (Timeout > 0) {
    if (ThcLibCycleInProgress (MmioBase)) {
      gBS->Stall (THC_DELAY_PERIOD_1MS);
      Timeout--;
      if (Timeout % 100 == 0) {
        THC_LOCAL_DEBUG(L"ThcLibNoCyclesPending, Cycle is still in progress\n")
        DEBUG ((DEBUG_INFO, "ThcLibNoCyclesPending, Cycle is still in progress\n"));
      }
    } else {
      return EFI_SUCCESS;
    }
  }
  return EFI_TIMEOUT;
}

/**
  Prepares THC to return its configuration

  @param[in]  MmioBase          Thc MMIO BAR0

**/
VOID
ThcLibPrepareReadDevRegCmd (
  IN UINT64   MmioBase
  )
{
  THC_M_PRT_SW_SEQ_CNTRL  SwSeqCntrl;

  THC_LOCAL_DEBUG(L"ThcLibPrepareReadDevRegCmd\n")

  MmioOr32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));
  SwSeqCntrl.Data = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));

  SwSeqCntrl.Fields.ThcSsBc  = sizeof (THC_DEVICE_REGISTERS); // 64 bytes
  SwSeqCntrl.Fields.ThcSsCmd = ThcPioRead;

  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL), SwSeqCntrl.Data);

  // Read all defined Touch IC registers. Start from 0 till 64 bytes
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA0_ADDR), 0x00000000);

}

/**
  Prepares THC to change Touch Panels TOUCH_CONFIG fields

  @param[in]  MmioBase          Thc MMIO BAR0

**/
VOID
ThcLibPrepareWriteConfigCmd (
  IN UINT64   MmioBase
  )
{
  THC_M_PRT_SW_SEQ_CNTRL  SwSeqCntrl;

  THC_LOCAL_DEBUG(L"ThcLibPrepareWriteConfigCmd\n")

  MmioOr32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));
  SwSeqCntrl.Data = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));

  SwSeqCntrl.Fields.ThcSsBc = sizeof (TOUCH_CONFIG); // 4 bytes
  SwSeqCntrl.Fields.ThcSsCmd = ThcPioWrite;

  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL), SwSeqCntrl.Data);
  THC_LOCAL_DEBUG(L"ThcLibPrepareWriteConfigCmd R_THC_MEM_PRT_SW_SEQ_CNTRL 0x%X Data: 0x%X \n", R_THC_MEM_PRT_SW_SEQ_CNTRL, MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL)))

  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA0_ADDR), 0x0000000C);
  THC_LOCAL_DEBUG(L"ThcLibPrepareWriteConfigCmd R_THC_MEM_PRT_SW_SEQ_DATA0_ADDR 0x%X Data: 0x%X \n", R_THC_MEM_PRT_SW_SEQ_DATA0_ADDR, MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA0_ADDR)))
}

/**
  Reads THC device registers

  @param[in]      MmioBase         Thc MMIO BAR0
  @param[in/out]  DeviceRegisters  Pointer to the device registers

  @retval EFI_SUCCESS              Cycle completed
  @retval EFI_DEVICE_ERROR         Returned Byte Count is too big or empty
**/
EFI_STATUS
ThcLibReadDeviceRegisters (
  IN     UINT64  MmioBase,
  IN OUT UINT32  *DeviceRegisters
  )
{
  THC_M_PRT_SW_SEQ_CNTRL  SwSeqCntrl;
  UINT32                  Index;
  UINT32                  DWordCount;

  THC_LOCAL_DEBUG(L"ThcLibReadDeviceRegisters Start ()\n")

  SwSeqCntrl.Data = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));
  if ((SwSeqCntrl.Fields.ThcSsBc > 64) || (SwSeqCntrl.Fields.ThcSsBc == 0)) {
    return EFI_DEVICE_ERROR;
  }

  DWordCount = SwSeqCntrl.Fields.ThcSsBc / sizeof (UINT32);
  // Go through every DATAx register and read it's content
  for (Index = 0; Index < DWordCount; Index++) {
    THC_LOCAL_DEBUG(L"ThcLibReadDeviceRegisters Offset 0x%X Data: 0x%X\n", (R_THC_MEM_PRT_SW_SEQ_DATA1 + (Index * sizeof (UINT32))), MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA1 + (Index * sizeof (UINT32))))
    DeviceRegisters[Index] = MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA1 + (Index * sizeof (UINT32)));
  }

  MmioOr32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));

  return EFI_SUCCESS;
}

/**
  Write to sequencing Data 1

  @param[in]  MmioBase         Thc MMIO BAR0
  @param[in]  Data             Data

**/
VOID
ThcLibWriteSeqData1 (
  IN UINT64  MmioBase,
  IN UINT32  Data
  )
{
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA1), Data);
}

/**
  Sets Prd Tables Adddress in THC MMIO

  @param[in]  MmioBase            Thc MMIO BAR0
  @param[in]  WriteAddr           Write Prd Address
  @param[in]  Read2Addr           Read2 Prd Address

  @retval EFI_SUCCESS             Cycle completed
  @retval EFI_INVALID_PARAMETER   Returned Byte Count is too big or empty
**/
EFI_STATUS
ThcLibSetPrdTablesAddress (
  IN UINT64  MmioBase,
  IN UINT64  WriteAddr,
  IN UINT64  Read2Addr
  )
{
  THC_LOCAL_DEBUG(L"ThcLibSetPrdTablesAddress Read2Addr: 0x%X Write: 0x%X \n", Read2Addr, WriteAddr)
  if ((Read2Addr & EFI_PAGE_MASK) || (WriteAddr & EFI_PAGE_MASK)) {
    THC_LOCAL_DEBUG(L"ThcLibSetPrdTablesAddress WriteRead Address not 4K alligned first 12 bits should be 0. Read2Addr: 0x%X Write: 0x%X \n", Read2Addr, WriteAddr)
    DEBUG ((DEBUG_WARN, "ThcLibSetPrdTablesAddress WriteRead Address not 4K alligned first 12 bits should be 0. Read2Addr: 0x%X Write: 0x%X \n", Read2Addr, WriteAddr));
    return EFI_INVALID_PARAMETER;
  }

  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WPRD_BA_LOW, WriteAddr & 0xFFFFF000);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WPRD_BA_HI, (UINT32) RShiftU64 ((UINTN) WriteAddr, 32));

  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_BA_LOW_2, Read2Addr & 0xFFFFF000);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_BA_HI_2, (UINT32) RShiftU64 ((UINTN) Read2Addr, 32));

  return EFI_SUCCESS;
}

/**
  Sets PrdTable/CircullarBuffer length

  @param[in]  MmioBase        Thc MMIO BAR0
  @param[in]  ReadEntryCount  Number Of Read Entries
  @param[in]  ReadCBDepth     Read Circullar Buffer Depth
  @param[in]  WriteEntryCount Number of Write Entries

**/
VOID
ThcLibSetLengthInPrd (
  IN UINT64  MmioBase,
  IN UINT8   ReadEntryCount,
  IN UINT8   ReadCBDepth,
  IN UINT8   WriteEntryCount
  )
{
  THC_M_PRT_RPRD_CNTRL_2       ReadRprdCntrl2;
  THC_M_PRT_WRITE_DMA_CNTRL    WriteDmaCntrl;

  THC_LOCAL_DEBUG(L"ThcLibSetLengthInPrd ReadEntryCount: %d WriteEntryCount: %d ReadCBDepth %d\n", ReadEntryCount, WriteEntryCount, ReadCBDepth)

  ReadRprdCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_2);
  ReadRprdCntrl2.Fields.Ptec = ReadEntryCount - 1; // 0 MmioBased
  ReadRprdCntrl2.Fields.Pcd  = ReadCBDepth - 1; // 0 MmioBased
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_2, ReadRprdCntrl2.Data);

  WriteDmaCntrl.Data  = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
  WriteDmaCntrl.Fields.ThcWrDmaPtec = WriteEntryCount - 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL, WriteDmaCntrl.Data);
}

/**
  Sets Write DMA start bit

  @param[in]  MmioBase           Thc MMIO BAR0

**/
VOID
ThcLibSetWriteStartBit (
  IN UINT64  MmioBase
  )
{
  THC_LOCAL_DEBUG(L"ThcLibSetWriteStartBit ()\n")

  THC_M_PRT_WRITE_DMA_CNTRL  WriteDmaCntrl;
  WriteDmaCntrl.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
  WriteDmaCntrl.Fields.ThcWrDmaIeDmaCpl = 1;
  WriteDmaCntrl.Fields.ThcWrDmaStart    = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL, WriteDmaCntrl.Data);
}

/**
  Polls until DMA Complete bit is set

  @param[in]  MmioBase       Thc MMIO BAR0
  @param[in]  Timeout        [MILLISECONDS] Indicates how long polling lasts

  @retval  EFI_SUCCESS       DMA transfer is complete
  @retval  EFI_TIMEOUT       Timeout reached
**/
EFI_STATUS
ThcLibPollDmaComplete (
  IN UINT64  MmioBase,
  IN UINT32  Timeout
  )
{
  THC_M_PRT_WRITE_DMA_CNTRL  WriteDmaCntrl;
  THC_M_PRT_DMA_INT_STS      WriteDmaIntSts;

  THC_LOCAL_DEBUG(L"ThcLibPollDmaComplete ()\n")

  while (Timeout > 0) {
    WriteDmaCntrl.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
    if (WriteDmaCntrl.Fields.ThcWrDmaStart == 0) {
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

  @param[in]  MmioBase           Thc MMIO BAR0

**/
VOID
ThcLibSetReadRx2StartBit (
  IN UINT64  MmioBase
  )
{
  THC_LOCAL_DEBUG(L"ThcLibSetReadRx2StartBit ()\n")

  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;
  ReadDmaCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
  ReadDmaCntrl2.Fields.IeEof = 1;
  ReadDmaCntrl2.Fields.ThcPrdCbWritePointer = POINTER_WRAPAROUND;
  ReadDmaCntrl2.Fields.Start = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);
}

/**
  Sets Read Rx2 DMA write pointer

  @param[in]  MmioBase       Thc MMIO BAR0
  @param[in]  WritePointer   PRD CB Write Pointer

**/
VOID
ThcLibSetReadRx2WritePointer (
  IN UINT64  MmioBase,
  IN UINT8   WritePointer
  )
{
  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;
  ReadDmaCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
  ReadDmaCntrl2.Fields.ThcPrdCbWritePointer = WritePointer;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);
}

/**
  Sets Read Rx2 DMA write pointer

  @param[in]  MmioBase           Thc MMIO BAR0

  @retval     Contents of Dma Control 2 register
**/
UINT32
ThcLibGetReadRx2Data (
  IN UINT64  MmioBase
  )
{
  return MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
}

/**
  Prepares Touch Panel and THC for OS hand off

  @param[in]  MmioBase           Thc MMIO BAR0

**/
VOID
ThcLibCleanUp (
  IN UINT64  MmioBase
  )
{
  THC_LOCAL_DEBUG(L"ThcLibCleanUp ()\n")
  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;
  THC_M_PRT_WRITE_DMA_CNTRL  WriteDmaCntrl;
  THC_M_PRT_DMA_INT_STS      WriteDmaIntSts;

  ReadDmaCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
  ReadDmaCntrl2.Fields.IeEof = 0;
  ReadDmaCntrl2.Fields.ThcPrdCbWritePointer = 0;
  ReadDmaCntrl2.Fields.Start = 0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);

  WriteDmaCntrl.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
  WriteDmaCntrl.Fields.ThcWrDmaIeDmaCpl = 0;
  WriteDmaCntrl.Fields.ThcWrDmaStart    = 0;
  WriteDmaCntrl.Fields.ThcWrDmaPtec     = 0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL, WriteDmaCntrl.Data);

  WriteDmaIntSts.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_INT_STS);
  WriteDmaIntSts.Fields.ThcWrDmaCmplStatus = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_INT_STS, WriteDmaIntSts.Data);

  ThcLibSetReadRx2WritePointer (MmioBase, 0);

  ThcLibSetPrdTablesAddress (MmioBase, 0, 0);
  ThcLibSetLengthInPrd (MmioBase, 1, 1, 1);

  ThcLibStartQuiesce (MmioBase, StartQuiesceTimeout);

  ThcLibReset (MmioBase);
}

/**
  Read Int Cause Register in PIO mode.

  @param[in]  MmioBase          Thc MMIO BAR0

**/
VOID
ThcLibPioReadIntCause (
  IN UINT64   MmioBase
  )
{
  THC_M_PRT_SW_SEQ_CNTRL  SwSeqCntrl;

  THC_LOCAL_DEBUG(L"ThcLibPioReadIntCause\n")

  MmioOr32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));
  SwSeqCntrl.Data = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));

  SwSeqCntrl.Fields.ThcSsBc  = 4; // 4 bytes
  SwSeqCntrl.Fields.ThcSsCmd = ThcPioRead;

  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL), SwSeqCntrl.Data);

  ThcLibGo (MmioBase);

  SwSeqCntrl.Data = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));
  if ((SwSeqCntrl.Fields.ThcSsBc > 64) || (SwSeqCntrl.Fields.ThcSsBc == 0)) {
    THC_LOCAL_DEBUG(L"Error. Wrong data size ThcSsBc = 0x%X.\n", SwSeqCntrl.Fields.ThcSsBc)
    return;
  }

  THC_LOCAL_DEBUG(L"ThcLibPioReadIntCause Register Offset 0x%X Data: 0x%X\n", R_THC_MEM_PRT_SW_SEQ_DATA1, MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA1))

  MmioOr32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));
}

/**
  Stops DMA traffic by clearing Start BIT and CB Pointer Reset for the 2nd RXDMA Read Rx2 DMA Interrupt Status

  @param[in]  MmioBase           Thc MMIO BAR0

**/
VOID
ThcLibStopDma (
  IN UINT64  MmioBase
  )
{
  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;
  ReadDmaCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
  ReadDmaCntrl2.Fields.Start                = 0;
  ReadDmaCntrl2.Fields.ThcPrdCbPointerReset = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);
}