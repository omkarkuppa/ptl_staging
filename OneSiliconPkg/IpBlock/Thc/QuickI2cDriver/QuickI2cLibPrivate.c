/** @file
  Touch Host Controller QuickI2c Private Lib for Driver usage

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

#include "QuickI2cPrivate.h"
#include <Protocol/ThcResetProtocol.h>
#include "QuickI2cSubIP.h"
#include "QuickI2cLibPrivate.h"
#include <Library/DxeThcLib.h>
#include <QuickI2cDriver.h>
#include <ThcConfig.h>

/**
  Sets TSSGO bit to start a cycle

  @param[in]  MmioBase         QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibGo (
  IN UINT64  MmioBase
  )
{
  MmioOr32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL), B_THC_MEM_PRT_SW_SEQ_CNTRL_TSSGO);
}

/**
  Sets Edge Interrupt mode

  @param[in]  MmioBase         QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibSetEdgeInterrupts (
  IN UINT64  MmioBase
  )
{
  MmioOr32 (MmioBase + R_THC_MEM_PRT_TSEQ_CNTRL_1, BIT31);
}

/**
  Returns last received interrupt of THC_INTERRUPT_TYPE

  @param[in]  MmioBase     QuickI2c MMIO BAR0

  @retval                  One of THC_INTERRUPT_TYPE
**/
UINT8
QuickI2cLibInterruptStatus (
  IN UINT64  MmioBase
  )
{
  return (UINT8)(MmioRead32 ((UINTN) (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_1)) & (BIT0|BIT1|BIT2|BIT3));
}

/**
  Start Quiesce to de-initialize THC

  @param[in]  MmioBase      QuickI2c MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for HW Sts bit to be 1

  @retval EFI_SUCCESS       Quiesce Enabled
  @retval EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
QuickI2cLibStartQuiesce (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  //
  // Set QUICK_I2C_DEVINT_QUIESCE_EN to 1
  //
  MmioOr32 (MmioBase + R_THC_MEM_PRT_CONTROL, B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_EN);

  //
  // Poll on Quiesce HW Sts bit to be 1
  //
  while (Timeout > 0) {
    if (MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL) & B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_HW_STS) {
      return EFI_SUCCESS;
    }
    gBS->Stall (THC_DELAY_PERIOD_1MS);
    Timeout--;
    if (Timeout % 100 == 0) {
      THC_LOCAL_DEBUG(L"QuickI2cLibStartQuiesce HW_STS: 0x%X | Timeout: %d\n", MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL), Timeout)
      DEBUG ((DEBUG_WARN, "QuickI2cLibStartQuiesce HW_STS: 0x%X | Timeout: %d\n", MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL), Timeout));
      // continue
    }
  }
  return EFI_DEVICE_ERROR;
}

/**
  Ends Quiesce

  @param[in] MmioBase       QuickI2c MMIO BAR0
  @param[in] Timeout        [MILLISECONDS] Indicates how long driver waits for HW STS response

  @retval EFI_SUCCESS       Quiesce Disabled
  @retval EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
QuickI2cLibEndQuiesce (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  MmioAnd32 (MmioBase + R_THC_MEM_PRT_CONTROL, (UINT32) ~(B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_EN));
  //
  // Poll on Quiesce HW Sts bit to be 0
  //
  while (Timeout > 0) {
    if (MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL) & B_THC_MEM_PRT_CONTROL_THC_DEVINT_QUIESCE_HW_STS) {
      gBS->Stall (THC_DELAY_PERIOD_1MS);
      Timeout--;
      if (Timeout % 100 == 0) {
        THC_LOCAL_DEBUG(L"QuickI2cLibEndQuiesce HW_STS: 0x%X | Timeout: %d\n", MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL), Timeout)
        DEBUG ((DEBUG_WARN, "QuickI2cLibEndQuiesce HW_STS: 0x%X | Timeout: %d\n", MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL), Timeout));
      }
    } else {
     return EFI_SUCCESS;
    }
  }
  return EFI_DEVICE_ERROR;
}

/**
  Check if Quiesce is disabled

  @param[in]  MmioBase  QuickI2c MMIO BAR0

  @retval  TRUE         Quiesce is Disabled
  @retval  FALSE        Quiesce is Enabled
**/
BOOLEAN
QuickI2cLibIsQuiesceDisabled (
  IN UINT64   MmioBase
  )
{
  THC_M_PRT_CONTROL PortControl;

  PortControl.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL);

  //
  // Disable interrupts - If already disabled (Quisced) no need to disable again
  //
  if ((PortControl.Fields.ThcDevIntQuiesceEn == 1) || (PortControl.Fields.ThcDevIntQuiesceHwSts == 1)) {
    THC_LOCAL_DEBUG (L"QuickI2cLibIsQuiesceDisabled HW Quiesce status is already 1, skip disabling interrupt \n")
    return FALSE;
  }
  return TRUE;
}

/**
  Check if Quiesce is enabled

  @param[in]  MmioBase  QuickI2c MMIO BAR0

  @retval  TRUE         Quiesce is Disabled
  @retval  FALSE        Quiesce is Enabled
**/
BOOLEAN
QuickI2cLibIsQuiesceEnabled (
  IN UINT64   MmioBase
  )
{
  THC_M_PRT_CONTROL PortControl;

  PortControl.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL);

  //
  // Enable interrupts - If already enabled (UnQuisced) no need to enable again
  //
  if ((PortControl.Fields.ThcDevIntQuiesceEn == 0) || (PortControl.Fields.ThcDevIntQuiesceHwSts == 0)) {
    THC_LOCAL_DEBUG (L"QuickI2cLibIsQuiesceEnabled HW Quiesce status is already 0, skip enabling interrupt \n")
    return FALSE;
  }
  return TRUE;
}

/**
  Waits until cycle is completed

  @param[in]  MmioBase         QuickI2c MMIO BAR0
  @param[in]  Timeout          [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS          Cycle completed
  @retval EFI_DEVICE_ERROR     ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickI2cLibWaitCycleCompleted (
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
        THC_LOCAL_DEBUG (L"QuickI2cLibWaitCycleCompleted Cycle is still in progress THC_M_PRT_SW_SEQ_STS- <0x%x>\n", SwSeqSts.Data32)
        DEBUG ((DEBUG_WARN, "QuickI2cLibWaitCycleCompleted Cycle is still in progress THC_M_PRT_SW_SEQ_STS- <0x%x>\n", SwSeqSts.Data32));
      }
    } else {
      break;
    }
  }
  if (Timeout == 0) {
    return EFI_TIMEOUT;
  }

  if (SwSeqSts.Fields.ThcSsErr) {
    DEBUG ((DEBUG_ERROR, "QuickI2cLibWaitCycleCompleted THC_M_PRT_SW_SEQ_STS.ThcSsErr is set <0x%x>\n", SwSeqSts.Data32));
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}

/**
  Checks if no cycles are pending

  @param[in]  MmioBase         QuickI2c MMIO BAR0
  @param[in]  Timeout          [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS          No Cycles running
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickI2cLibNoCyclesPending (
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
        THC_LOCAL_DEBUG (L"QuickI2cLibNoCyclesPending, Cycle is still in progress \n")
        DEBUG ((DEBUG_INFO, "QuickI2cLibNoCyclesPending, Cycle is still in progress\n"));
      }
    } else {
      return EFI_SUCCESS;
    }
  }
  return EFI_TIMEOUT;
}

/**
  Prepares THC to return its configuration

  @param[in]  Operation              Operation Command
  @param[in]  MmioBase               QuickI2c MMIO BAR0
  @param[in]  ByteCount              Number of bytes
  @param[in]  I2CAddressRegister     I2C Address Register
**/
VOID
QuickI2cLibPreparePioOperation (
  IN THC_I2C_PIO_OP Operation,
  IN UINT64   MmioBase,
  IN UINT16   ByteCount,
  IN UINT32   I2CAddressRegister
  )
{
  THC_M_PRT_SW_SEQ_CNTRL  SwSeqCntrl;

  MmioOr32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));

  SwSeqCntrl.Data = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));
  SwSeqCntrl.Fields.QuickI2cSsBc   = ByteCount;
  SwSeqCntrl.Fields.QuickI2cSsCmd  = Operation;
  SwSeqCntrl.Fields.QuickI2cSsCdIe = 0x1; // Enable Interrupt
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL), SwSeqCntrl.Data);

  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA0_ADDR), I2CAddressRegister);
}

/**
  Prepares THC to return its configuration specifically used to read device descriptor

  @param[in]  Operation              Operation Command
  @param[in]  MmioBase               QuickI2c MMIO BAR0
  @param[in]  ByteCount              Number of bytes
  @param[in]  I2CAddressRegister     I2C Address Register
**/
VOID
QuickI2cLibPrepareRwPioOperation (
  IN THC_I2C_PIO_OP Operation,
  IN UINT64   MmioBase,
  IN UINT16   ByteCount,
  IN UINT16   WriteCount,
  IN UINT32   I2CAddressRegister
  )
{
  THC_M_PRT_SW_SEQ_CNTRL        SwSeqCntrl;
  THC_M_PRT_SW_SEQ_I2C_WR_CNTRL I2cWrCntrl;

  MmioOr32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));

  I2cWrCntrl.Data32 = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_I2C_WR_CNTRL));
  I2cWrCntrl.Fields.ThcI2cRwPioEn = 1;
  I2cWrCntrl.Fields.ThcPioI2cWbc = WriteCount;
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_I2C_WR_CNTRL), I2cWrCntrl.Data32);

  SwSeqCntrl.Data = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));
  SwSeqCntrl.Fields.QuickI2cSsBc   = ByteCount;
  SwSeqCntrl.Fields.QuickI2cSsCmd  = Operation;
  SwSeqCntrl.Fields.QuickI2cSsCdIe = 0x1; // Enable Interrupt
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL), SwSeqCntrl.Data);

  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA0_ADDR), I2CAddressRegister);

}

/**
  Reads THC device registers

  @param[in]      MmioBase      QuickI2c MMIO BAR0
  @param[in/out]  ReturnData    Return Data Pointer

  @retval EFI_SUCCESS              Cycle completed
  @retval EFI_DEVICE_ERROR         Returned Byte Count is too big or empty
**/
EFI_STATUS
QuickI2cLibReadComplete (
  IN     UINT64  MmioBase,
  IN OUT UINT32  *ReturnData
  )
{
  THC_M_PRT_SW_SEQ_CNTRL  SwSeqCntrl;
  UINT32                  Index;
  UINT32                  DWordCount;

  SwSeqCntrl.Data = MmioRead32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));
  if ((SwSeqCntrl.Fields.QuickI2cSsBc > sizeof (QUICK_I2C_DEVICE_DESCRIPTOR)) || (SwSeqCntrl.Fields.QuickI2cSsBc == 0)) {
    return EFI_DEVICE_ERROR;
  }

  DWordCount = SwSeqCntrl.Fields.QuickI2cSsBc / sizeof (UINT32);

  // Go through every DATAx register and read its content
  for (Index = 0; Index < DWordCount; Index++) {
    THC_LOCAL_DEBUG (L"QuickI2cLibReadComplete DATAx register Offset 0x%X Data: 0x%X\n", (R_THC_MEM_PRT_SW_SEQ_DATA1 + (Index * sizeof (UINT32))), MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA1 + (Index * sizeof (UINT32))))
    ReturnData[Index] = MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA1 + (Index * sizeof (UINT32)));
  }

  MmioOr32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));

  return EFI_SUCCESS;
}

/**
  Power up device through RST GPIO

  @param[in] Timeout            [MILLISECONDS] Indicates how long driver waits for device to power up
  @param[in] InstanceId         SPI instance id
  @param[in] ResetPadTrigger    Reset Pad Trigger

**/
EFI_STATUS
QuickI2cLibGpioGetOutOfReset (
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
    return Status;
  }

  //
  // Place TIC in Reset state Reset Assert
  //
  ThcResetProtocol->SetOutputValue (ThcResetProtocol, InstanceId, !(ResetPadTrigger & BIT0));
  gBS->Stall (EFI_TIMER_PERIOD_MILLISECONDS (Timeout));
  return EFI_SUCCESS;
}

/**
  Waits for TOUCH_INT_CAUSE to return ResetOccurred

  @param[in]  MmioBase      QuickI2c MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS       Reset was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
QuickI2cLibWaitForResetOccured (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  )
{
  THC_M_PRT_READ_INT_STS  ReadIntSts;

  while (Timeout > 0) {
    ReadIntSts.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_1);
    if (ReadIntSts.Fields.ThcRdDmaNonDmaIntStatus == 1) {
      MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_1, ReadIntSts.Data32);
      return EFI_SUCCESS;
    }
    gBS->Stall (THC_DELAY_PERIOD_1MS);
    Timeout--;
    if (Timeout % 100 == 0) {
    }
  }
  THC_LOCAL_DEBUG (L"QuickI2cLibWaitForResetOccured R_THC_MEM_PRT_READ_DMA_INT_STS_1: 0x%X \n", MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_1))
  return EFI_TIMEOUT;
}

/**
  Write to sequencing Data 1

  @param[in]  MmioBase         QuickI2c MMIO BAR0
  @param[in]  Data             Data

**/
VOID
QuickI2cLibWriteSeqData1 (
  IN UINT64  MmioBase,
  IN UINT32  Data
  )
{
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA1), Data);
}

/**
  Sets Prd Tables Adddress in THC MMIO

  @param[in]  MmioBase            QuickI2c MMIO BAR0
  @param[in]  WriteAddr           Write Prd Address
  @param[in]  Read2Addr           Read2 Prd Address
  @param[in]  SwDmaReadAddr       SwDmaReadAddr Prd Address

  @retval EFI_SUCCESS             Cycle completed
  @retval EFI_INVALID_PARAMETER   Returned Byte Count is too big or empty
**/
EFI_STATUS
QuickI2cLibSetPrdTablesAddress (
  IN UINT64  MmioBase,
  IN UINT64  WriteAddr,
  IN UINT64  Read2Addr,
  IN UINT64  SwDmaReadAddr
  )
{
  THC_LOCAL_DEBUG (L"QuickI2cLibSetPrdTablesAddress Read2Addr: 0x%X Write: 0x%X SwDmaReadAddr: 0x%X\n", Read2Addr, WriteAddr, SwDmaReadAddr)
  if ((Read2Addr & EFI_PAGE_MASK) || (WriteAddr & EFI_PAGE_MASK) || (SwDmaReadAddr & EFI_PAGE_MASK)) {
    THC_LOCAL_DEBUG (L"QuickI2cLibSetPrdTablesAddress WriteRead Address not 4K alligned first 12 bits should be 0. Read2Addr: 0x%X Write: 0x%X \n", Read2Addr, WriteAddr)
    DEBUG ((DEBUG_WARN, "QuickI2cLibSetPrdTablesAddress WriteRead Address not 4K alligned first 12 bits should be 0. Read2Addr: 0x%X Write: 0x%X \n", Read2Addr, WriteAddr));
    return EFI_INVALID_PARAMETER;
  }

  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WPRD_BA_LOW, WriteAddr & 0xFFFFF000);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WPRD_BA_HI, (UINT32) RShiftU64 ((UINTN) WriteAddr, 32));

  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_BA_LOW_2, Read2Addr & 0xFFFFF000);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_BA_HI_2, (UINT32) RShiftU64 ((UINTN) Read2Addr, 32));

  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_BA_LOW_SW, SwDmaReadAddr & 0xFFFFF000);
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_BA_HI_SW, (UINT32) RShiftU64 ((UINTN) SwDmaReadAddr, 32));

  return EFI_SUCCESS;
}

/**
  Sets PrdTable/CircularBuffer length

  @param[in]  MmioBase              QuickI2c MMIO BAR0
  @param[in]  ReadEntryCount        Number Of Read Entries
  @param[in]  ReadCBDepth           Read Circullar Buffer Depth
  @param[in]  WriteEntryCount       Number of Write Entries
  @param[in]  SwDmaReadEntryCount   Number Of Read Entries
  @param[in]  SwDmaReadCBDepth      Read Circullar Buffer Depth

**/
VOID
QuickI2cLibSetLengthInPrd (
  IN UINT64  MmioBase,
  IN UINT8   ReadEntryCount,
  IN UINT8   ReadCBDepth,
  IN UINT8   WriteEntryCount,
  IN UINT8   SwDmaReadEntryCount,
  IN UINT8   SwDmaReadCBDepth
  )
{
  THC_M_PRT_RPRD_CNTRL_2       ReadRprdCntrl2;
  THC_M_PRT_WRITE_DMA_CNTRL    WriteDmaCntrl;
  THC_M_PRT_RPRD_CNTRL_SW      RprdCntrlSw;

  THC_LOCAL_DEBUG (L"QuickI2cLibSetLengthInPrd ReadEntryCount: %d WriteEntryCount: %d ReadCBDepth %d\n", ReadEntryCount, WriteEntryCount, ReadCBDepth)

  ReadRprdCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_2);
  ReadRprdCntrl2.Fields.Ptec = ReadEntryCount - 1; // 0 MmioBased
  ReadRprdCntrl2.Fields.Pcd  = ReadCBDepth - 1; // 0 MmioBased
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_2, ReadRprdCntrl2.Data);

  WriteDmaCntrl.Data  = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
  WriteDmaCntrl.Fields.QuickI2cWrDmaPtec = WriteEntryCount - 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL, WriteDmaCntrl.Data);

  RprdCntrlSw.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_SW);
  RprdCntrlSw.Fields.Ptec = SwDmaReadEntryCount - 1; // 0 MmioBased
  RprdCntrlSw.Fields.Pcd  = SwDmaReadCBDepth - 1; // 0 MmioBased
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_SW, RprdCntrlSw.Data32);

}

/**
  Check Write DMA status

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibCheckWriteDmaStatus (
  IN UINT64  MmioBase
  )
{
  THC_LOCAL_DEBUG (L"QuickI2cLibCheckWriteDmaStatus ()\n")

  THC_M_PRT_DMA_INT_STS      WriteDmaIntSts;
  UINT32                     Timeout;

  Timeout = WritePollingTimeout;

  while (Timeout > 0) {
    WriteDmaIntSts.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_INT_STS);
    if (WriteDmaIntSts.Fields.ThcWrDmaActive == 0) {
      return;
    }
    MicroSecondDelay (1);
    Timeout--;
  }

  THC_LOCAL_DEBUG (L"QuickI2cLibCheckWriteDmaStatus WriteDmaIntSts.ThcWrDmaActive active\n")
  return;
}

/**
  Sets Write DMA start bit

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibSetWriteStartBit (
  IN UINT64  MmioBase
  )
{
  THC_M_PRT_WRITE_DMA_CNTRL  WriteDmaCntrl;
  THC_M_PRT_DMA_INT_STS      WriteDmaIntSts;

  THC_LOCAL_DEBUG (L"QuickI2cLibSetWriteStartBit ()\n")
  // clear
  WriteDmaIntSts.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_INT_STS);
  WriteDmaIntSts.Fields.ThcWrDmaCmplStatus = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_INT_STS, WriteDmaIntSts.Data);

  //
  WriteDmaCntrl.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
  WriteDmaCntrl.Fields.QuickI2cWrDmaIeDmaCpl = 1;
  WriteDmaCntrl.Fields.QuickI2cWrDmaStart    = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL, WriteDmaCntrl.Data);
}

/**
  Sets Read SwDMA start bit

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibSwDmaSetReadStartBit (
  IN UINT64  MmioBase
  )
{
  THC_M_PRT_READ_DMA_CNTRL_SW ReadDmaCntrlSw;

  THC_LOCAL_DEBUG (L"QuickI2cLibSwDmaSetReadStartBit ()\n")

  ReadDmaCntrlSw.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_SW);
  // Configure read interrupts
  ReadDmaCntrlSw.Fields.IeDmaCpl = 1;
  ReadDmaCntrlSw.Fields.IeIoc = 1;
  ReadDmaCntrlSw.Fields.Soo = 1;
  ReadDmaCntrlSw.Fields.Tpcwp = POINTER_WRAPAROUND;
  ReadDmaCntrlSw.Fields.Start = 1;

  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_SW, ReadDmaCntrlSw.Data32);
}

/**
  Polls until DMA Complete bit is set

  @param[in]  MmioBase       QuickI2c MMIO BAR0
  @param[in]  Timeout        [MILLISECONDS] Indicates how long polling lasts

  @retval  EFI_SUCCESS       DMA transfer is complete
  @retval  EFI_TIMEOUT       Timeout reached
**/
EFI_STATUS
QuickI2cLibPollDmaComplete (
  IN UINT64  MmioBase,
  IN UINT32  Timeout
  )
{
  THC_M_PRT_WRITE_DMA_CNTRL  WriteDmaCntrl;
  THC_M_PRT_DMA_INT_STS      WriteDmaIntSts;

  THC_LOCAL_DEBUG (L"QuickI2cLibPollDmaComplete ()\n")

  while (Timeout > 0) {
    WriteDmaCntrl.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
    if (WriteDmaCntrl.Fields.QuickI2cWrDmaStart == 0) {
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

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibSetReadRx2StartBit (
  IN UINT64  MmioBase
  )
{
  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;

  THC_LOCAL_DEBUG (L"QuickI2cLibSetReadRx2StartBit ()\n")

  ReadDmaCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
  ReadDmaCntrl2.Fields.IeEof = 1;
  ReadDmaCntrl2.Fields.QuickI2cPrdCbWritePointer = POINTER_WRAPAROUND;
  ReadDmaCntrl2.Fields.Start = 1;
  ReadDmaCntrl2.Fields.IntSwDmaEn = 0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);
}

/**
  Start SwDma Operation

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cStartWriteSwdma (
  IN UINT64   MmioBase,
  IN UINT32   Address,
  IN UINT32   Data0
  )
{
  EFI_STATUS  Status;

  THC_LOCAL_DEBUG (L"QuickI2cStartWriteSwdma Entry ()\n")

  Status = QuickI2cLibNoCyclesPending (MmioBase, CycleTimeout);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cLibNoCyclesPending error status - %r\n", Status)
  }

  MmioOr32 ((UINTN) (MmioBase + R_THC_MEM_PRT_SW_SEQ_STS), (UINT32) (B_THC_MEM_PRT_SW_SEQ_STS_TSSDONE | B_THC_MEM_PRT_SW_SEQ_STS_THC_SS_ERR));
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_DATA0_ADDR), Address);
  QuickI2cLibWriteSeqData1 (MmioBase, Data0);
}

/**
  Complete SwDma Operation

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cCompleteSwdma (
  IN QUICK_I2C_DEV        *QuickI2cDev
  )
{
  EFI_STATUS Status;

  THC_LOCAL_DEBUG (L"QuickI2cCompleteSwdma()\n")

  //
  // To complete SwDMA, need to reset DMA/SwDMA settings
  // Restart ReadRx2 process and enable interrupts
  //

  QuickI2cWaitForSwRxDMAPause (QuickI2cDev->PciBar0);
  QuickI2cResetDmaSettings (QuickI2cDev->PciBar0);
  QuickI2cResetSwdmaSettings (QuickI2cDev->PciBar0);
  QuickI2cLibSetReadRx2StartBit (QuickI2cDev->PciBar0);
  if (QuickI2cDev->InitProcessDoneEnableInterrupt) {
    if (QuickI2cLibIsQuiesceEnabled (QuickI2cDev->PciBar0)) {
      THC_LOCAL_DEBUG (L"QuickI2cCompleteSwdma QuiesceEnabled \n")
      Status = QuickI2cLibEndQuiesce (QuickI2cDev->PciBar0, EndQuiesceTimeout);
      THC_LOCAL_DEBUG (L"QuickI2cCompleteSwdma QuickI2cLibEndQuiesce Status: %r\n", Status)
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_WARN, "QuickI2cCompleteSwdma QuickI2cLibEndQuiesce error, Status %r\n", Status));
        return;
      }
    }
  }
}

/**
  Wait For Sw RxDma Pause

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
EFI_STATUS
QuickI2cWaitForSwRxDMAPause (
 IN UINT64 MmioBase
  )
{
  UINT64                        Timeout;
  THC_M_PRT_READ_DMA_INT_STS_SW ReadDmaIntStsSw;
  THC_M_PRT_READ_DMA_CNTRL_SW   ReadDmaCntrlSw;

  Timeout = QUICK_I2C_DEFAULT_POLLING_ITERATIONS;

  ReadDmaCntrlSw.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_SW);
  ReadDmaCntrlSw.Fields.Start = 0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_SW, ReadDmaCntrlSw.Data32);

  while (Timeout > 0) {
    ReadDmaIntStsSw.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_SW);
    if (!ReadDmaIntStsSw.Fields.Active) {
      THC_LOCAL_DEBUG (L"QuickI2cWaitForSwRxDMAPause ReadDmaIntStsSw.Data32- 0x%x\n", ReadDmaIntStsSw.Data32)
      return EFI_SUCCESS;
    }
    gBS->Stall (THC_DELAY_PERIOD_1MS);
    Timeout--;
  }

  return EFI_TIMEOUT;
}

/**
  Program SwDma read length

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cSwDmaProgramReadLen (
  IN UINT64 MmioBase,
  IN UINT32 RxDlenEn,
  IN UINT32 Wbc,
  IN UINT32 wReportDescLength
  )
{
  THC_M_PRT_RPRD_CNTRL_SW         RprdCntrlSw;
  THC_M_PRT_SW_DMA_PRD_TABLE_LEN  DmaPrdTableLen;

  //
  // Set appropriate values in R_THC_MEM_PRT_RPRD_CNTRL_SW for report descriptor retrieval via SWDMA
  //
  RprdCntrlSw.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_SW);
  RprdCntrlSw.Fields.SwDmaI2cWbc = Wbc;
  RprdCntrlSw.Fields.SwDmaI2cRxDlenEn = RxDlenEn;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_SW, RprdCntrlSw.Data32);

  if (RxDlenEn == 0) {
    DmaPrdTableLen.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_DMA_PRD_TABLE_LEN);
    DmaPrdTableLen.Fields.SwDmaPrdTableLen = wReportDescLength;
    MmioWrite32 (MmioBase + R_THC_MEM_PRT_SW_DMA_PRD_TABLE_LEN, DmaPrdTableLen.Data32);
  }

  return;
}

/**
  Sets Read Rx2 DMA write pointer

  @param[in]  MmioBase       QuickI2c MMIO BAR0
  @param[in]  WritePointer   PRD CB Write Pointer

**/
VOID
QuickI2cLibSetReadRx2WritePointer (
  IN UINT64  MmioBase,
  IN UINT8   WritePointer
  )
{
  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;

  ReadDmaCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
  ReadDmaCntrl2.Fields.QuickI2cPrdCbWritePointer = WritePointer;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);
}

/**
  Sets Read Rx2 DMA write pointer

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
UINT32
QuickI2cLibGetReadRx2Data (
  IN UINT64  MmioBase
  )
{
  return MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
}

/**
  Wait for RxDma2 status for Pause

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
EFI_STATUS
QuickI2cWaitForRxDMA2Pause (
  IN UINT64  MmioBase
)
{
  UINT64                       Timeout;
  THC_M_PRT_READ_DMA_CNTRL_2   ReadDmaCntrl2;
  THC_M_PRT_READ_DMA_INT_STS_2 ReadDmaIntSts2;

  Timeout                     = QUICK_I2C_DEFAULT_POLLING_ITERATIONS;
  ReadDmaCntrl2.Data          = QuickI2cLibGetReadRx2Data (MmioBase);
  ReadDmaIntSts2.Data32       = QuickI2cLibGetReadRx2IntSts (MmioBase);
  ReadDmaCntrl2.Fields.Start  = 0;

  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);

  while (Timeout > 0) {
    ReadDmaIntSts2.Data32 = QuickI2cLibGetReadRx2IntSts (MmioBase);
    if (!ReadDmaIntSts2.Fields.Active) {
      THC_LOCAL_DEBUG (L"QuickI2cWaitForRxDMA2Pause ReadDmaIntSts2.Data32- 0x%x\n", ReadDmaIntSts2.Data32)
      return EFI_SUCCESS;
    }
    gBS->Stall (THC_DELAY_PERIOD_1MS);
    Timeout--;
  }

  return EFI_TIMEOUT;
}

/**
  Reset DMA settings

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cResetDmaSettings (
  IN UINT64  MmioBase
  )
{
  THC_M_PRT_WRITE_DMA_CNTRL   WriteDmaCntrl;

  QuickI2cLibStopDma (MmioBase);

  WriteDmaCntrl.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
  WriteDmaCntrl.Fields.QuickI2cWrDmaStart = 0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL, WriteDmaCntrl.Data);
}

/**
  Reset SwDMA settings

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
void
QuickI2cResetSwdmaSettings (
  IN UINT64  MmioBase
  )
{
  THC_M_PRT_READ_DMA_CNTRL_SW ReadDmaCntrlSw;

  ReadDmaCntrlSw.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_SW);
  ReadDmaCntrlSw.Fields.Start = 0;
  ReadDmaCntrlSw.Fields.Tpcpr = 1; //reset SWDMA read pointers
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_SW, ReadDmaCntrlSw.Data32);
}

/**
  Unconfigure SwDMA

  @param[in]  MmioBase           QuickI2c MMIO BAR0

  @retval
**/
VOID
SwdmaUnconfigure (
  IN QUICK_I2C_DEV  *QuickI2cDev
  )
{
  THC_M_PRT_READ_DMA_CNTRL_SW ReadDmaCntrlSw;

  ReadDmaCntrlSw.Data32 = MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_READ_DMA_CNTRL_SW);
  ReadDmaCntrlSw.Fields.Start = 0;
  MmioWrite32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_READ_DMA_CNTRL_SW, ReadDmaCntrlSw.Data32);

  //
  // Reset PRD table settings
  //
  QuickI2cDmaFillSinglePrdTable (&(QuickI2cDev->DmaWrite), 0);
}

/**
  Confure SwDMA and program PRD table

  @param[in]    *QuickI2cDev    QuickI2cDev struct pointer

  @retval  Status
**/
EFI_STATUS
QuickI2cSwDmaConfigure (
  IN QUICK_I2C_DEV  *QuickI2cDev
  )
{
  //
  // Reset SwDma settings
  //
  QuickI2cResetSwdmaSettings (QuickI2cDev->PciBar0);

  return EFI_SUCCESS;
}

/**
  Configure/Clear internal IP state
  @param[in]    *QuickI2cDev    QuickI2cDev struct pointer
  @retval  Status
**/
EFI_STATUS
QuickI2cInternalStateClear (
  IN UINT64 MmioBase
  )
{
  THC_M_PRT_INT_STATUS            ThcIntStatus;
  THC_M_PRT_SW_SEQ_STS            SwSeqSts;
  THC_M_PRT_READ_DMA_CNTRL_2      ReadDmaCntrl2;
  THC_M_PRT_READ_DMA_INT_STS_2    ReadDmaIntSts2;
  THC_M_PRT_WRITE_DMA_CNTRL       WriteDmaCntrl;
  THC_M_PRT_ERR_CAUSE             ThcMPrtErrCause;
  THC_M_PRT_READ_DMA_INT_STS_1    ThcMPrtReadDmaIntSts1;
  THC_M_PRT_READ_DMA_CNTRL_1      ThcMPrtReadDmaCntrl1;
  THC_M_PRT_WRITE_INT_STS         ThcMPrtWriteIntSts;
  THC_M_PRT_DEVINT_CNT            ThcMPrtDevintCnt;
  THC_M_PRT_DB_CNT_1              ThcMPrtDbCnt1;
  THC_M_PRT_FRAME_DROP_CNT_1      ThcMPrtFrameDropCnt1;
  THC_M_PRT_FRAME_DROP_CNT_2      ThcMPrtFrameDropCnt2;
  THC_M_PRT_FRM_CNT_1             ThcMPrtFrmCnt1;
  THC_M_PRT_FRM_CNT_2             ThcMPrtFrmCnt2;
  THC_M_PRT_RXDMA_PKT_CNT_1       ThcMPrtRxdmaPktCnt1;
  THC_M_PRT_RXDMA_PKT_CNT_2       ThcMPrtRxdmaPktCnt2;
  THC_M_PRT_SWINT_CNT_1           ThcMPrtSwintCnt1;
  THC_M_PRT_SWINT_CNT_2           ThcMPrtSwintCnt2;
  THC_M_PRT_TX_FRM_CNT            ThcMPrtTxFrmCnt;
  THC_M_PRT_TXDMA_PKT_CNT         ThcMPrtTxdmaPktCnt;
  THC_M_PRT_UFRM_CNT_1            ThcMPrtUfrmCnt1;
  THC_M_PRT_UFRM_CNT_2            ThcMPrtUfrmCnt2;
  THC_M_PRT_PRD_EMPTY_CNT_1       ThcMPrtPrdEmptyCnt1;
  THC_M_PRT_PRD_EMPTY_CNT_2       ThcMPrtPrdEmptyCnt2;

  ThcMPrtErrCause.Fields.BufOvrrunErr = 1; // 0x1028
  ThcMPrtErrCause.Fields.InvldDevEntry = 1;
  ThcMPrtErrCause.Fields.FrameBabbleErr = 1;
  ThcMPrtErrCause.Fields.PrdEntryErr = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_ERR_CAUSE, ThcMPrtErrCause.Data32);

  ReadDmaIntSts2.Fields.StallSts = 1; //0x1210
  ReadDmaIntSts2.Fields.EofIntSts = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_2, ReadDmaIntSts2.Data32);

  ThcMPrtReadDmaIntSts1.Fields.StallSts = 1; // 0x1110h
  ThcMPrtReadDmaIntSts1.Fields.EofIntSts = 1;
  ThcMPrtReadDmaIntSts1.Fields.NondmaIntSts = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_1, ThcMPrtReadDmaIntSts1.Data32);

  ThcIntStatus.Fields.TxnErrIntSts = 1; // 1024h
  ThcIntStatus.Fields.FatalErrIntSts = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_INT_STATUS, ThcIntStatus.Data32);

  SwSeqSts.Fields.ThcSsErr = 1; //0x1044
  SwSeqSts.Fields.TssDone = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_STS, SwSeqSts.Data32);

  ThcMPrtReadDmaCntrl1.Fields.IeEof = 0; // 0x110C
  ThcMPrtReadDmaCntrl1.Fields.Tpcpr = 1; 
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_1, ThcMPrtReadDmaCntrl1.Data32);
  ReadDmaCntrl2.Fields.IeEof = 0; // 0x120C
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);

  WriteDmaCntrl.Fields.QuickI2cWrDmaIeDmaCpl = 0; //0x1098
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL, WriteDmaCntrl.Data);

  ThcMPrtWriteIntSts.Fields.ThcWrdmaErrorSts = 1; // 0x109C
  ThcMPrtWriteIntSts.Fields.ThcWrdmaIocSts = 1;
  ThcMPrtWriteIntSts.Fields.ThcWrdmaCmplStatus = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_WRITE_INT_STS, ThcMPrtWriteIntSts.Data32);

  ThcMPrtDbCnt1.Fields.ThcMPrtDbCntRst = 1; //0x11A0
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_DB_CNT_1, ThcMPrtDbCnt1.Data32);

  ThcMPrtDevintCnt.Fields.ThcMPrtDevintCntRst = 1; // 0x10E8
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_DEVINT_CNT, ThcMPrtDevintCnt.Data32);

  ThcMPrtFrameDropCnt1.Fields.Rfdc = 1; // 0x11B4
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_FRAME_DROP_CNT_1, ThcMPrtFrameDropCnt1.Data32);

  ThcMPrtFrameDropCnt2.Fields.Rfdc = 1; // 0x12B4
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_FRAME_DROP_CNT_2, ThcMPrtFrameDropCnt2.Data32);

  ThcMPrtFrmCnt1.Fields.ThcMPrtFrmCntRst = 1; // 0x11A4
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_FRM_CNT_1, ThcMPrtFrmCnt1.Data32);

  ThcMPrtFrmCnt2.Fields.ThcMPrtFrmCntRst = 1; // 0x12A4
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_FRM_CNT_2, ThcMPrtFrmCnt2.Data32);

  ThcMPrtRxdmaPktCnt1.Fields.ThcMPrtRxdmaPktCntRst = 1; // 0x11AC
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RXDMA_PKT_CNT_1, ThcMPrtRxdmaPktCnt1.Data32);

  ThcMPrtRxdmaPktCnt2.Fields.ThcMPrtRxdmaPktCntRst = 1; // 0x12AC
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_RXDMA_PKT_CNT_2, ThcMPrtRxdmaPktCnt2.Data32);

  ThcMPrtSwintCnt1.Fields.ThcMPrtSwintCntRst = 1; // 0x11B0
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SWINT_CNT_1, ThcMPrtSwintCnt1.Data32);

  ThcMPrtSwintCnt2.Fields.ThcMPrtSwintCntRst = 1; // 0x12B0
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SWINT_CNT_2, ThcMPrtSwintCnt2.Data32);

  ThcMPrtTxFrmCnt.Fields.ThcMPrtTxFrmCntRst = 1; // 0x10E0
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_TX_FRM_CNT, ThcMPrtTxFrmCnt.Data32);

  ThcMPrtTxdmaPktCnt.Fields.ThcMPrtTxdmaPktCntRst = 1; // 0x10E4
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_TXDMA_PKT_CNT, ThcMPrtTxdmaPktCnt.Data32);

  ThcMPrtUfrmCnt1.Fields.ThcMPrtUfrmCntRst = 1; // 0x11A8
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_UFRM_CNT_1, ThcMPrtUfrmCnt1.Data32);

  ThcMPrtUfrmCnt2.Fields.ThcMPrtUfrmCntRst = 1; // 0x12A8
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_UFRM_CNT_2, ThcMPrtUfrmCnt2.Data32);

  ThcMPrtPrdEmptyCnt1.Fields.Rptec = 1; // 0x12F0
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_PRD_EMPTY_CNT_1, ThcMPrtPrdEmptyCnt1.Data32);

  ThcMPrtPrdEmptyCnt2.Fields.Rptec = 1; // 0x12F4
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_PRD_EMPTY_CNT_2, ThcMPrtPrdEmptyCnt2.Data32);
  return EFI_SUCCESS;
}

/**
  Program Sub IP register to default settings

  @param[in]  MmioBase              QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibProgramSubIpRegisterToDefault (
  IN UINT64  MmioBase
  )
{
  THC_M_PRT_CONTROL               PortControl;
  THC_M_PRT_INT_EN                ThcIntEn;
  THC_M_PRT_INT_STATUS            ThcIntStatus;
  THC_M_PRT_SW_SEQ_STS            SwSeqSts;
  THC_M_PRT_SW_SEQ_CNTRL          SwSeqCntrl;
  THC_M_PRT_READ_DMA_CNTRL_2      ReadDmaCntrl2;
  THC_M_PRT_READ_DMA_INT_STS_2    ReadDmaIntSts2;
  THC_M_PRT_RPRD_CNTRL_SW         RprdCntrlSw;
  THC_M_PRT_SW_DMA_PRD_TABLE_LEN  DmaPrdTableLen;
  THC_M_PRT_READ_DMA_CNTRL_SW     ReadDmaCntrlSw;
  THC_M_PRT_SW_SEQ_I2C_WR_CNTRL   I2cWrCntrl;
  THC_M_PRT_WRITE_DMA_CNTRL       WriteDmaCntrl;

  THC_LOCAL_DEBUG (L"%a: Entry \n", __FUNCTION__)
  // Set Port type as 00 = SPI default value
  PortControl.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL);
  PortControl.Fields.PortType = 0x0;
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_CONTROL), PortControl.Data32);
  THC_LOCAL_DEBUG (L"%a:  R_THC_MEM_PRT_CONTROL- 0x%x \n", __FUNCTION__, MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL))

  // Set Int Enable to default value
  ThcIntEn.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_INT_ENABLE);
  ThcIntEn.Data32 = THC_M_PRT_INT_EN_DEFAULT;
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_INT_ENABLE), ThcIntEn.Data32);
  THC_LOCAL_DEBUG (L"%a: R_THC_MEM_PRT_INT_ENABLE- 0x%x \n", __FUNCTION__, MmioRead32 (MmioBase + R_THC_MEM_PRT_INT_ENABLE))

  // Set other registers to default value i.e. 0
  ThcIntStatus.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_INT_STATUS);
  ThcIntStatus.Data32 = 0;
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_INT_STATUS), ThcIntStatus.Data32);
  THC_LOCAL_DEBUG (L"%a: R_THC_MEM_PRT_INT_STATUS- 0x%x \n", __FUNCTION__, MmioRead32 (MmioBase + R_THC_MEM_PRT_INT_STATUS))

  SwSeqSts.Data32 = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_STS));
  SwSeqSts.Data32 = 0;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_STS, SwSeqSts.Data32);

  SwSeqCntrl.Data = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL));
  SwSeqCntrl.Data = 0x0;
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL), SwSeqCntrl.Data);
  THC_LOCAL_DEBUG (L"%a: R_THC_MEM_PRT_SW_SEQ_CNTRL- 0x%x \n", __FUNCTION__, MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_CNTRL))

  ReadDmaIntSts2.Data32 = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_2));
  ReadDmaIntSts2.Data32 = 0;
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_2), ReadDmaIntSts2.Data32);
  THC_LOCAL_DEBUG (L"%a: R_THC_MEM_PRT_READ_DMA_INT_STS_2- 0x%x \n", __FUNCTION__, MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_2))


  RprdCntrlSw.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_SW);
  RprdCntrlSw.Fields.Pcd = 0;
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_SW), RprdCntrlSw.Data32);
  THC_LOCAL_DEBUG (L"%a: R_THC_MEM_PRT_RPRD_CNTRL_SW- 0x%x \n", __FUNCTION__, MmioRead32 (MmioBase + R_THC_MEM_PRT_RPRD_CNTRL_SW))


  DmaPrdTableLen.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_DMA_PRD_TABLE_LEN);
  DmaPrdTableLen.Data32 = 0;
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_DMA_PRD_TABLE_LEN), DmaPrdTableLen.Data32);
  THC_LOCAL_DEBUG (L"%a: R_THC_MEM_PRT_SW_DMA_PRD_TABLE_LEN- 0x%x \n", __FUNCTION__, MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_DMA_PRD_TABLE_LEN))

  I2cWrCntrl.Data32 = MmioRead32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_I2C_WR_CNTRL));
  I2cWrCntrl.Data32 = 0;
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_SW_SEQ_I2C_WR_CNTRL), I2cWrCntrl.Data32);
   THC_LOCAL_DEBUG (L"%a: R_THC_MEM_PRT_SW_SEQ_I2C_WR_CNTRL- 0x%x \n", __FUNCTION__, MmioRead32 (MmioBase + R_THC_MEM_PRT_SW_SEQ_I2C_WR_CNTRL))

  // Dma Ctr2 Bit6 IeEOF = 0
  ReadDmaCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
  ReadDmaCntrl2.Fields.IeEof = 0;
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2), ReadDmaCntrl2.Data);
  THC_LOCAL_DEBUG (L"%a: R_THC_MEM_PRT_READ_DMA_CNTRL_2- 0x%x \n", __FUNCTION__, MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2))

  // 0x40000000 : BIT30 : Update HW status for SW rxdma
  ReadDmaCntrlSw.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_SW);
  ReadDmaCntrlSw.Fields.IeDmaCpl = 0;
  ReadDmaCntrlSw.Fields.Soo = 0;
  ReadDmaCntrlSw.Fields.IeIoc = 0;
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_SW), ReadDmaCntrlSw.Data32);
  THC_LOCAL_DEBUG (L"%a: R_THC_MEM_PRT_READ_DMA_CNTRL_SW- 0x%x \n", __FUNCTION__, MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_SW))


  //0x00800000 -- > BIT 23 : Update HW Status for WRDMA (THC_WRDMA)
  WriteDmaCntrl.Data  = MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL);
  WriteDmaCntrl.Fields.QuickI2cWrDmaIeDmaCpl = 0;
  MmioWrite32 ((UINTN)(MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL), WriteDmaCntrl.Data);
  THC_LOCAL_DEBUG (L"%a: R_THC_MEM_PRT_WRITE_DMA_CNTRL- 0x%x \n", __FUNCTION__, MmioRead32 (MmioBase + R_THC_MEM_PRT_WRITE_DMA_CNTRL))
}

/**
  Reset all programmed SubIP registers

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cSubIpResetConfiguredSettings (
  IN UINT64  MmioBase
  )
{
  EFI_STATUS              Status;
  THC_I2C_IC_CON          IcCon;
  THC_I2C_IC_ENABLE       IcEnable;
  THC_I2C_IC_TAR          IcTar;
  THC_I2C_IC_SDA_HOLD     IcSdaHold;
  THC_I2C_IC_SS_SCL_HCNT  IcSsSclHcnt;
  THC_I2C_IC_SS_SCL_LCNT  IcSsSclLcnt;
  THC_I2C_IC_FS_SCL_HCNT  IcFsSclHcnt;
  THC_I2C_IC_FS_SCL_LCNT  IcFsSclLcnt;
  THC_I2C_IC_HS_SCL_HCNT  IcHsSclHcnt;
  THC_I2C_IC_HS_SCL_LCNT  IcHsSclLcnt;
  THC_I2C_IC_INTR_MASK    IcIntrMask;
  THC_I2C_IC_RX_TL        IcRxTl;
  THC_I2C_IC_TX_TL        IcTxTl;
  THC_I2C_IC_DMA_CR       IcDmaCr;
  THC_I2C_IC_DMA_TDLR     IcDmaTdlr;
  THC_I2C_IC_DMA_RDLR     IcDmaRdlr;

  THC_LOCAL_DEBUG (L"QuickI2cSubIpResetConfiguredSettings: Start ()\n")

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_ENABLE, &IcEnable.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_ENABLE error, Status %r\n", Status));
    return;
  }
  IcEnable.Fields.Enable = 0;
  IcEnable.Fields.SdaStuckRecoveryEnable = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_ENABLE, IcEnable.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_ENABLE error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_CON, &IcCon.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_CON error, Status %r\n", Status));
    return;
  }
  IcCon.Data32 = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_CON, IcCon.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_CON error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_TAR, &IcTar.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_TAR error, Status %r\n", Status));
    return;
  }
  IcTar.Fields.IcTar = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_TAR, IcTar.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_TAR error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_SS_SCL_HCNT, &IcSsSclHcnt.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_SS_SCL_HCNT error, Status %r\n", Status));
    return;
  }
  IcSsSclHcnt.Fields.IcSsSclHcnt = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_SS_SCL_HCNT, IcSsSclHcnt.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_SS_SCL_HCNT error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_SS_SCL_LCNT, &IcSsSclLcnt.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_SS_SCL_LCNT error, Status %r\n", Status));
    return;
  }
  IcSsSclLcnt.Fields.IcSsSclLcnt = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_SS_SCL_LCNT, IcSsSclLcnt.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_SS_SCL_LCNT error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_SDA_HOLD, &IcSdaHold.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
    return;
  }
  IcSdaHold.Fields.IcSdaTxHold = 0;
  IcSdaHold.Fields.IcSdaRxHold = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_SDA_HOLD, IcSdaHold.Data32);
    if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_FS_SCL_HCNT, &IcFsSclHcnt.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_FS_SCL_HCNT error, Status %r\n", Status));
    return;
  }
  IcFsSclHcnt.Fields.IcFsSclHcnt = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_FS_SCL_HCNT, IcFsSclHcnt.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_FS_SCL_HCNT error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_FS_SCL_LCNT, &IcFsSclLcnt.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_FS_SCL_LCNT error, Status %r\n", Status));
    return;
  }
  IcFsSclLcnt.Fields.IcFsSclLcnt = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_FS_SCL_LCNT, IcFsSclLcnt.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_FS_SCL_LCNT error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_SDA_HOLD, &IcSdaHold.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
    return;
  }
  IcSdaHold.Fields.IcSdaTxHold = 0;
  IcSdaHold.Fields.IcSdaRxHold = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_SDA_HOLD, IcSdaHold.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_HS_SCL_HCNT, &IcHsSclHcnt.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_HS_SCL_HCNT error, Status %r\n", Status));
    return;
  }
  IcHsSclHcnt.Fields.IcHsSclHcnt = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_HS_SCL_HCNT, IcHsSclHcnt.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_HS_SCL_HCNT error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_HS_SCL_LCNT, &IcHsSclLcnt.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_HS_SCL_LCNT error, Status %r\n", Status));
    return;
  }
  IcHsSclLcnt.Fields.IcHsSclLcnt  = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_HS_SCL_LCNT, IcHsSclLcnt.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_HS_SCL_LCNT error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_SDA_HOLD, &IcSdaHold.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
    return;
  }
  IcSdaHold.Fields.IcSdaTxHold = 0;
  IcSdaHold.Fields.IcSdaRxHold = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_SDA_HOLD, IcSdaHold.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_INTR_MASK, &IcIntrMask.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_INTR_MASK error, Status %r\n", Status));
    return;
  }
  IcIntrMask.Data32 = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_INTR_MASK, IcIntrMask.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_INTR_MASK error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_RX_TL, &IcRxTl.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_RX_TL error, Status %r\n", Status));
    return;
  }
  IcRxTl.Fields.RxTl = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_RX_TL, IcRxTl.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_RX_TL error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_TX_TL, &IcTxTl.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_TX_TL error, Status %r\n", Status));
    return;
  }
  IcTxTl.Fields.TxTl = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_TX_TL, IcTxTl.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_TX_TL error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_DMA_CR, &IcDmaCr.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_DMA_CR error, Status %r\n", Status));
    return;
  }
  IcDmaCr.Data32 = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_DMA_CR, IcDmaCr.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_DMA_CR error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_DMA_TDLR, &IcDmaTdlr.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_DMA_TDLR error, Status %r\n", Status));
    return;
  }
  IcDmaTdlr.Fields.Dmatdl = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_DMA_TDLR, IcDmaTdlr.Data32);
      if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_DMA_TDLR error, Status %r\n", Status));
    return;
  }

  Status = QuickI2cLibReadSubIpRegister (MmioBase, THC_I2C_REG_IC_DMA_RDLR, &IcDmaRdlr.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_DMA_RDLR error, Status %r\n", Status));
    return;
  }
  IcDmaRdlr.Fields.Dmadrl = 0;
  Status = QuickI2cLibWriteSubIpRegister (MmioBase, THC_I2C_REG_IC_DMA_RDLR, IcDmaRdlr.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cSubIpResetConfiguredSettings THC_I2C_REG_IC_DMA_RDLR error, Status %r\n", Status));
    return;
  }

  THC_LOCAL_DEBUG (L"QuickI2cSubIpResetConfiguredSettings: End()\n")
}

/**
  Prepares Touch Panel and THC for OS hand off
  a.	Wait till all PIOs are finished
  b.	Quiesce THC interrupt and wait THC_DEVINT_QUIESCE_HW_STS bit to be 1
  c.	Do THC internal state clear up
  d.	Disable THC DMAs

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibCleanUp (
  IN UINT64       MmioBase
  )
{
  THC_LOCAL_DEBUG (L"QuickI2cLibCleanUp ()\n")
  
  QuickI2cDisIntAndInternalStateClr (MmioBase);
  QuickI2cResetDmaSettings (MmioBase);
  QuickI2cResetSwdmaSettings (MmioBase);
  QuickI2cSubIpResetConfiguredSettings (MmioBase);
  QuickI2cLibSetReadRx2WritePointer (MmioBase, 0);
  QuickI2cLibSetPrdTablesAddress (MmioBase, 0, 0, 0);
  QuickI2cLibSetLengthInPrd (MmioBase, 1, 1, 1, 1, 1);
  QuickI2cLibProgramSubIpRegisterToDefault (MmioBase);
}

/**
  Configures THC registers with default value for QuickI2c support

  @param[in]  HidOverI2c         QuickI2c HidOverI2c struct pointer
**/
VOID
QuickI2cLibConfigureDefaultPolicy (
  IN THC_HID_OVER_I2C  *HidOverI2c
  )
{
  THC_LOCAL_DEBUG (L"QuickI2cLibConfigureDefaultPolicy Start ()\n")
  //
  // Program Serial Clock Line High Period, Standard Mode Serial Clock Line Low Period
  //
  HidOverI2c->ConnectionSpeed                                   = QUICK_I2C_FAST_MODE_MAX_SPEED;
  HidOverI2c->DeviceDescriptorAddress                           = QUICK_I2C_DEFAULT_DEVICE_DESCRIPTOR_ADDRESS;
  HidOverI2c->StandardModeSerialClockLineHighPeriod             = QUICK_I2C_DEFAULT_FS_HCNT;
  HidOverI2c->StandardModeSerialClockLineLowPeriod              = QUICK_I2C_DEFAULT_FS_LCNT;
  HidOverI2c->StandardModeSerialDataLineReceiveHoldPeriod       = QUICK_I2C_MEM_SDA_RX_100_1000_KHZ;
  HidOverI2c->StandardModeSerialDataLineTransmitHoldPeriod      = QUICK_I2C_MEM_SDA_TX_100_1000_KHZ;
}

/**
  Sets Read Rx2 DMA Interrupt Status

  @param[in]  MmioBase           Thc MMIO BAR0

  @retval     Contents of Dma Interrupt Status 2 register
**/
UINT32
QuickI2cLibGetReadRx2IntSts (
  IN UINT64  MmioBase
  )
{
  return MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_2);
}

/**
  Clear SwDma interrupt status once read is complete

  @param[in]  MmioBase           Thc MMIO BAR0

  @retval     Contents of Dma Interrupt Status 2 register
**/
VOID
QuickI2cLibClearSwDmaReadIntSts (
  IN UINT64  MmioBase
  )
{
  THC_M_PRT_READ_DMA_INT_STS_SW ReadDmaIntStsSw;

  THC_LOCAL_DEBUG (L"QuickI2cLibClearSwDmaReadIntSts Start ()\n");
  ReadDmaIntStsSw.Data32 = QuickI2cLibGetReadSwDmaRx2IntSts (MmioBase);
  // Clear the STS bit
  ReadDmaIntStsSw.Fields.DmacplSts = 1;
  ReadDmaIntStsSw.Fields.IocSts = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_SW, ReadDmaIntStsSw.Data32);
}

/**
  Clear RxDma2 interrupt status once read is complete

  @param[in]  MmioBase           Thc MMIO BAR0

  @retval     Contents of Dma Interrupt Status 2 register
**/
VOID
QuickI2cLibClearReadRx2IntSts (
  IN UINT64  MmioBase
  )
{
  EFI_STATUS                    Status;
  THC_M_PRT_READ_DMA_INT_STS_2  ReadDmaIntSts2;
  THC_M_PRT_INT_STATUS          ThcIntStatus;

  THC_LOCAL_DEBUG (L"QuickI2cLibClearReadRx2IntSts Start ()\n");

  ReadDmaIntSts2.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_2);
  ReadDmaIntSts2.Fields.EofIntSts = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_2, ReadDmaIntSts2.Data32);

  ThcIntStatus.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_INT_STATUS);
  THC_LOCAL_DEBUG (L"QuickI2cLibClearReadRx2IntSts before ThcIntStatus - 0x%X\n", MmioRead32 (MmioBase + R_THC_MEM_PRT_INT_STATUS));

  // Checking for I2C SubIP errors
  if (ThcIntStatus.Fields.ThcI2cIcRxUnderIntSts) {
    ThcIntStatus.Fields.ThcI2cIcRxUnderIntSts = 1;
  }
  if (ThcIntStatus.Fields.ThcI2cIcRxOverIntSts) {
    ThcIntStatus.Fields.ThcI2cIcRxOverIntSts = 1;
  }
  if (ThcIntStatus.Fields.ThcI2cIcRxFullIntSts) {
    ThcIntStatus.Fields.ThcI2cIcRxFullIntSts = 1;
  }
  if (ThcIntStatus.Fields.ThcI2cIcTxOverIntSts) {
    ThcIntStatus.Fields.ThcI2cIcTxOverIntSts = 1;
  }
  if (ThcIntStatus.Fields.ThcI2cIcTxAbrtTIntSts) {
    ThcIntStatus.Fields.ThcI2cIcTxAbrtTIntSts = 1;
  }
  if (ThcIntStatus.Fields.ThcI2cIcSclStuckAtLowIntSts) {
    ThcIntStatus.Fields.ThcI2cIcSclStuckAtLowIntSts = 1;
  }
  if (ThcIntStatus.Fields.ThcI2cIcMstOnHoldIntSts) {
    ThcIntStatus.Fields.ThcI2cIcMstOnHoldIntSts = 1;
  }
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_INT_STATUS, ThcIntStatus.Data32);
  THC_LOCAL_DEBUG (L"QuickI2cLibClearReadRx2IntSts ThcIntStatus - 0x%X\n", MmioRead32 (MmioBase + R_THC_MEM_PRT_INT_STATUS));

  if (QuickI2cLibIsQuiesceEnabled (MmioBase)) {
    Status = QuickI2cLibEndQuiesce (MmioBase, EndQuiesceTimeout);
    if (EFI_ERROR (Status)) {
      THC_LOCAL_DEBUG (L"QuickI2cLibClearReadRx2IntSts QuickI2cLibEndQuiesce error Status: %r\n", Status)
      DEBUG ((DEBUG_WARN, "QuickI2cLibClearReadRx2IntSts QuickI2cLibEndQuiesce error, Status %r\n", Status));
      return;
    }
  }
  THC_LOCAL_DEBUG (L"QuickI2cLibClearReadRx2IntSts End\n");
}

/**
  Sets Read SwDMA Rx2 DMA Interrupt Status

  @param[in]  MmioBase           Thc MMIO BAR0

  @retval     Contents of Dma Interrupt Status 2 register
**/
UINT32
QuickI2cLibGetReadSwDmaRx2IntSts (
  IN UINT64  MmioBase
  )
{
  return MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_INT_STS_SW);
}

/**
  Stops DMA traffic by clearing Start BIT and CB Pointer Reset for
  the 2nd RXDMA Read Rx2 DMA Interrupt Status

  @param[in]  MmioBase           Thc MMIO BAR0

**/
VOID
QuickI2cLibStopDma (
  IN UINT64  MmioBase
  )
{
  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;

  ReadDmaCntrl2.Data = MmioRead32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2);
  ReadDmaCntrl2.Fields.Start                     = 0;
  ReadDmaCntrl2.Fields.QuickI2cPrdCbPointerReset = 1;
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);
}

/**
  Sets THC to work in I2C mode

  @param[in]  MmioBase  Thc MMIO BAR0

**/
VOID
QuickI2cLibSetI2cPort (
  IN UINT64  MmioBase
  )
{
  THC_M_PRT_CONTROL PortControl;

  PortControl.Data32 = MmioRead32 (MmioBase + R_THC_MEM_PRT_CONTROL);
  PortControl.Fields.PortType = 0x1; // 01 = I2C
  MmioWrite32 (MmioBase + R_THC_MEM_PRT_CONTROL, PortControl.Data32);
}

/**
  Perfomrs PIO read operation
  @param[in]  MmioBase          QuickI2c MMIO BAR0
  @param[in]  RegisterAddress   I2C register address
  @param[in]  DataSize          Data Size
  @param[out] Data              I2C register output data
  @retval EFI_SUCCESS           Read was successful
  @retval EFI_TIMEOUT           Timeout reached - might be expected
**/
EFI_STATUS
QuickI2cLibPerformPioRead (
  IN UINT64       MmioBase,
  IN UINT32       RegisterAddress,
  IN UINT16       DataSize,
  IN OUT UINT32   *Data
  )
{
  EFI_STATUS      Status;

  THC_LOCAL_DEBUG (L"QuickI2cLibPerformPioRead: RegisterAddress- <0x%x>: \n", RegisterAddress)
  Status = QuickI2cLibNoCyclesPending (MmioBase, CycleTimeout);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cLibPerformPioRead error, Status %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cLibPerformPioRead error, Status %r\n", Status));
    return Status;
  }

  QuickI2cLibPreparePioOperation (
    QuickI2cReadDeviceCommand,
    MmioBase,
    DataSize,
    RegisterAddress
    );
  QuickI2cLibGo (MmioBase);

  Status = QuickI2cLibWaitCycleCompleted (MmioBase, CycleTimeout);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cLibPerformPioRead error, Status %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cLibPerformPioRead error, Status %r\n", Status));
    return Status;
  }

  QuickI2cLibReadComplete (MmioBase, Data);

  return EFI_SUCCESS;
}

/**
  Reads I2C Ip register

  @param[in]  MmioBase          QuickI2c MMIO BAR0
  @param[in]  RegisterAddress   I2C register address
  @param[in/out] Data           I2C register output data

  @retval EFI_SUCCESS       Read was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
QuickI2cLibReadSubIpRegister (
  IN UINT64       MmioBase,
  IN UINT32       RegisterAddress,
  IN OUT UINT32   *Data
  )
{
  EFI_STATUS      Status;

  THC_LOCAL_DEBUG (L"QuickI2cLibReadSubIpRegister: SubIpRegisterAddress- <0x%x>: \n", RegisterAddress)
  Status = QuickI2cLibNoCyclesPending (MmioBase, CycleTimeout);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cLibReadSubIpRegister error, Status %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cLibReadSubIpRegister error, Status %r\n", Status));
    return Status;
  }

  QuickI2cLibPreparePioOperation (
    QuickI2cReadSubIpCommand,
    MmioBase,
    sizeof (UINT32),
    RegisterAddress
    );
  QuickI2cLibGo (MmioBase);

  Status = QuickI2cLibWaitCycleCompleted (MmioBase, CycleTimeout);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cLibReadSubIpRegister error, Status %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cLibReadSubIpRegister error, Status %r\n", Status));
    return Status;
  }

  QuickI2cLibReadComplete (MmioBase, Data);

  return EFI_SUCCESS;
}

/**
  Writes to I2C Ip register

  @param[in]  MmioBase          QuickI2c MMIO BAR0
  @param[in]  RegisterAddress   I2C register address
  @param[in]  Data              I2C register input data

  @retval EFI_SUCCESS       Read was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
QuickI2cLibWriteSubIpRegister (
  IN UINT64   MmioBase,
  IN UINT32   RegisterAddress,
  IN UINT32   Data
  )
{
  EFI_STATUS  Status;

  THC_LOCAL_DEBUG (L"QuickI2cLibWriteSubIpRegister: RegisterAddress- <0x%x>, WriteData- <0x%x>\n", RegisterAddress, Data)
  Status = QuickI2cLibNoCyclesPending (MmioBase, CycleTimeout);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cLibWriteSubIpRegister error, Status %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cLibWriteSubIpRegister error, Status %r\n", Status));
    return Status;
  }

  QuickI2cLibPreparePioOperation (
    QuickI2cWriteSubIpCommand,
    MmioBase,
    sizeof (UINT32),
    RegisterAddress
    );
  QuickI2cLibWriteSeqData1 (MmioBase, Data);
  QuickI2cLibGo (MmioBase);

  Status = QuickI2cLibWaitCycleCompleted (MmioBase, CycleTimeout);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickI2cLibWriteSubIpRegister error, Status %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}
