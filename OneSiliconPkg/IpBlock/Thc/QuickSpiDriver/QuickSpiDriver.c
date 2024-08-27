/** @file
  Touch Host Controller QuickSpi Driver API

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

#include "QuickSpiLibPrivate.h"
#include "QuickSpiAbsPtr.h"
#include "QuickSpiHid.h"
#include <ThcConfig.h>


/**
  Initialize QuickSpi according to procedure described in EDS

  @param[in]  QuickSpiDev      Context of QuickSpi device
  @param[in]  HidOverSpi       Hid Over Spi policy data
  @param[in]  Reset            Reset data

  @retval EFI_SUCCESS     QuickSpi initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickSpiInitialize (
  IN QUICK_SPI_DEV                *QuickSpiDev,
  IN THC_HID_OVER_SPI             *HidOverSpi,
  IN THC_RESET                    *Reset
  )
{
  EFI_STATUS       Status;
  UINT8            ResetRetryCount;
  QUICK_SPI_FLAGS  Flags;
  UINT32           Frequency;

  ResetRetryCount = THC_RESET_RETRY_COUNT;

  Flags.Data32 = HidOverSpi->Flags;

  //
  // Use defaults when policy is not set
  //
  if (HidOverSpi->ReadOpcode == 0) {
    DEBUG ((DEBUG_WARN, "QuickSpiInitialize HidOverSpi->ReadOpcode = 0, using default policy settings\n"));
    QuickSpiLibConfigure (
      QuickSpiDev->PciBar0,
      QUICK_SPI_DEFAULT_INPUT_REPORT_HEADER_ADDRESS,
      QUICK_SPI_DEFAULT_INPUT_REPORT_BODY_ADDRESS,
      QUICK_SPI_DEFAULT_OUTPUT_REPORT_ADDRESS,
      QUICK_SPI_DEFAULT_READ_OPCODE,
      QUICK_SPI_DEFAULT_DUAL_READ_OPCODE,
      QUICK_SPI_DEFAULT_QUAD_READ_OPCODE,
      QUICK_SPI_DEFAULT_READ_OPCODE,
      QUICK_SPI_DEFAULT_WRITE_OPCODE,
      QUICK_SPI_DEFAULT_DUAL_WRITE_OPCODE,
      QUICK_SPI_DEFAULT_QUAD_WRITE_OPCODE,
      QUICK_SPI_DEFAULT_QPI_WRITE_OPCODE,
      QUICK_SPI_DEFAULT_MAX_PACKET_SIZE
     );
    Frequency = QUICK_SPI_DEFAULT_FREQUENCY;
  } else { // consume policy
    QuickSpiLibConfigure (
      QuickSpiDev->PciBar0,
      HidOverSpi->InputReportHeaderAddress,
      HidOverSpi->InputReportBodyAddress,
      HidOverSpi->OutputReportAddress,
      (UINT8) HidOverSpi->ReadOpcode,
      (UINT8) HidOverSpi->ReadOpcode,
      (UINT8) HidOverSpi->ReadOpcode,
      (UINT8) HidOverSpi->ReadOpcode,
      (UINT8) HidOverSpi->WriteOpcode,
      (UINT8) HidOverSpi->WriteOpcode,
      (UINT8) HidOverSpi->WriteOpcode,
      (UINT8) HidOverSpi->WriteOpcode,
      (UINT8) HidOverSpi->LimitPacketSize
      );
    Frequency = HidOverSpi->Frequency;
  }


  THC_LOCAL_DEBUG(L"QuickSpiInitialize Start ()\n")
  Status = QuickSpiLibIoReady (QuickSpiDev->PciBar0, IoReadyTimeout);
  THC_LOCAL_DEBUG(L"QuickSpiInitialize QuickSpiLibIoReady Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiLibIoReady error, Status %r\n", Status));
    return Status;
  }

  if (QuickSpiLibIsQuiesceDisabled (QuickSpiDev->PciBar0)) {
    THC_LOCAL_DEBUG(L"QuickSpiInitialize QuiesceDisabled \n")
    Status = QuickSpiLibStartQuiesce (QuickSpiDev->PciBar0, StartQuiesceTimeout);
    THC_LOCAL_DEBUG(L"QuickSpiInitialize QuickSpiLibStartQuiesce Status: %r\n", Status)
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "QuickSpiLibStartQuiesce error, Status %r\n", Status));
      return Status;
    }
  }

  //
  // Configure THC Controller Frequency (round down) and Io Mode
  //
  THC_LOCAL_DEBUG(L"QuickSpiInitialize  Configure THC Controller to operate at %d, I/O mode: %d\n", Frequency, (UINT8) Flags.Fields.MultiSpiMode)
  QuickSpiLibSetFrequency (QuickSpiDev->PciBar0, Frequency);
  QuickSpiLibSetIoMode (QuickSpiDev->PciBar0, (UINT8) Flags.Fields.MultiSpiMode);

  QuickSpiLibSetEdgeInterrupts (QuickSpiDev->PciBar0);

  //
  // Reset THC
  //
  QuickSpiLibGetOutOfReset (QuickSpiDev->PciBar0, Reset->ResetSequencingDelay, QuickSpiDev->InstanceId, Reset->ResetPadTrigger);

  Status = QuickSpiLibEndQuiesce (QuickSpiDev->PciBar0, EndQuiesceTimeout);
  THC_LOCAL_DEBUG(L"QuickSpiInitialize QuickSpiLibEndQuiesce Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiLibEndQuiesce error, Status %r\n", Status));
    return Status;
  }

  do {
    Status = QuickSpiLibWaitForResetOccured (QuickSpiDev->PciBar0, WaitForResetTimeout);
    THC_LOCAL_DEBUG(L"QuickSpiInitialize QuickSpiLibWaitForResetOccured Status: %r\n", Status)
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "QuickSpiLibWaitForResetOccured error, Status %r Reset Count %d\n", Status, ResetRetryCount));
    } else {
      return Status;
    }
    QuickSpiLibGetOutOfReset (QuickSpiDev->PciBar0, Reset->ResetSequencingDelay, QuickSpiDev->InstanceId, Reset->ResetPadTrigger);
    ResetRetryCount--;
  } while (EFI_ERROR (Status) && (ResetRetryCount > 0));

  return Status;
}

/**
  Reads Touch Panels Device registers (TouchCapabilities, TouchId etc.)

  @param[in]  QuickSpiDev      Context of QuickSpi device

  @retval EFI_SUCCESS     QuickSpi initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickSpiReadDeviceDescriptor (
  IN QUICK_SPI_DEV                *QuickSpiDev
  )
{
  EFI_STATUS                      Status;
  QUICK_SPI_OUTPUT_REPORT_HEADER  OutputReportHeader;

  Status = QuickSpiLibNoCyclesPending (QuickSpiDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG(L"QuickSpiReadDeviceDescriptor  QuickSpiLibNoCyclesPending Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiReadDeviceDescriptor error, Status %r\n", Status));
    return Status;
  }

  THC_LOCAL_DEBUG(L"QuickSpiReadDeviceDescriptor\n")

  QuickSpiLibPreparePioOperation (
    QuickSpiPioWrite,
    QuickSpiDev->PciBar0,
    sizeof (QUICK_SPI_OUTPUT_REPORT_HEADER),
    MmioRead32 (QuickSpiDev->PciBar0 + R_THC_MEM_PRT_WR_BULK_ADDR) //QUICK_SPI_DEFAULT_OUTPUT_REPORT_ADDRESS
    );
  OutputReportHeader.Fields.ReportType    = QuickSpiOutputReportDeviceDescriptor;
  OutputReportHeader.Fields.ContentLength = 0x0;
  OutputReportHeader.Fields.ContentId     = 0x0;

  QuickSpiLibWriteSeqData1 (QuickSpiDev->PciBar0, OutputReportHeader.Data32);
  QuickSpiLibGo (QuickSpiDev->PciBar0);

  Status = QuickSpiLibWaitCycleCompleted (QuickSpiDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG(L"QuickSpiReadDeviceDescriptor  QuickSpiLibWaitCycleCompleted Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiReadDeviceDescriptor error, Status %r\n", Status));
    return Status;
  }

  Status =  QuickSpiLibWaitForDeviceDescriptor (QuickSpiDev->PciBar0, GetDeviceDescriptorTimeout, (UINT32*) &QuickSpiDev->DeviceDescriptor);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiReadDeviceDescriptor error, Status %r\n", Status));
  }

  QuickSpiDev->HidSolutionFlag = (QuickSpiDev->DeviceDescriptor.wVendorID == THC_HID_ELAN_VID)
                                 ? ElanHidProtocol : WacomHidProtocol;

  return Status;
}

/**
  Prints end Device Configuration

  @param[in]  QuickSpiDev      Context of QuickSpi device
**/
VOID
QuickSpiPrintDeviceDescriptor (
  IN  QUICK_SPI_DEV    *QuickSpiDev
  )
{
  THC_LOCAL_DEBUG(L"QuickSpiPrintDeviceDescriptor () Start\n")
  THC_LOCAL_DEBUG(L"| 0x00 | wDeviceDescLength        | 0x%X\n", QuickSpiDev->DeviceDescriptor.wDeviceDescLength)
  THC_LOCAL_DEBUG(L"| 0x02 | bcdVersion               | 0x%X\n", QuickSpiDev->DeviceDescriptor.bcdVersion)
  THC_LOCAL_DEBUG(L"| 0x04 | wReportDescLength        | 0x%X\n", QuickSpiDev->DeviceDescriptor.wReportDescLength)
  THC_LOCAL_DEBUG(L"| 0x06 | wMaxInputLength          | 0x%X\n", QuickSpiDev->DeviceDescriptor.wMaxInputLength)
  THC_LOCAL_DEBUG(L"| 0x08 | wMaxFragmentLength       | 0x%X\n", QuickSpiDev->DeviceDescriptor.wMaxFragmentLength)
  THC_LOCAL_DEBUG(L"| 0x0A | wMaxOutputLength         | 0x%X\n", QuickSpiDev->DeviceDescriptor.wMaxOutputLength)
  THC_LOCAL_DEBUG(L"| 0x0C | wVendorID                | 0x%X\n", QuickSpiDev->DeviceDescriptor.wVendorID)
  THC_LOCAL_DEBUG(L"| 0x0E | wProductID               | 0x%X\n", QuickSpiDev->DeviceDescriptor.wProductID)
  THC_LOCAL_DEBUG(L"| 0x10 | wVersionID               | 0x%X\n", QuickSpiDev->DeviceDescriptor.wVersionID)
  THC_LOCAL_DEBUG(L"| 0x12 | wFlags.NoOutputReportAck | 0x%X\n", QuickSpiDev->DeviceDescriptor.wFlags.Fields.NoOutputReportAck)
  THC_LOCAL_DEBUG(L"| 0x14 | Reserved                 | 0x%X\n", QuickSpiDev->DeviceDescriptor.Reserved)
  THC_LOCAL_DEBUG(L"THC HID Protocol                  | %a  \n", (QuickSpiDev->HidSolutionFlag == ElanHidProtocol) ? "ELAN " : "WACOM ")
  THC_LOCAL_DEBUG(L"QuickSpiPrintDeviceDescriptor () End\n")
  DEBUG ((DEBUG_INFO, "QuickSpiPrintDeviceDescriptor () Start\n"));
  DEBUG ((DEBUG_INFO, "| 0x00 | wDeviceDescLength        | 0x%X\n", QuickSpiDev->DeviceDescriptor.wDeviceDescLength));
  DEBUG ((DEBUG_INFO, "| 0x02 | bcdVersion               | 0x%X\n", QuickSpiDev->DeviceDescriptor.bcdVersion));
  DEBUG ((DEBUG_INFO, "| 0x04 | wReportDescLength        | 0x%X\n", QuickSpiDev->DeviceDescriptor.wReportDescLength));
  DEBUG ((DEBUG_INFO, "| 0x06 | wMaxInputLength          | 0x%X\n", QuickSpiDev->DeviceDescriptor.wMaxInputLength));
  DEBUG ((DEBUG_INFO, "| 0x08 | wMaxFragmentLength       | 0x%X\n", QuickSpiDev->DeviceDescriptor.wMaxFragmentLength));
  DEBUG ((DEBUG_INFO, "| 0x0A | wMaxOutputLength         | 0x%X\n", QuickSpiDev->DeviceDescriptor.wMaxOutputLength));
  DEBUG ((DEBUG_INFO, "| 0x0C | wVendorID                | 0x%X\n", QuickSpiDev->DeviceDescriptor.wVendorID));
  DEBUG ((DEBUG_INFO, "| 0x0E | wProductID               | 0x%X\n", QuickSpiDev->DeviceDescriptor.wProductID));
  DEBUG ((DEBUG_INFO, "| 0x10 | wVersionID               | 0x%X\n", QuickSpiDev->DeviceDescriptor.wVersionID));
  DEBUG ((DEBUG_INFO, "| 0x12 | wFlags.NoOutputReportAck | 0x%X\n", QuickSpiDev->DeviceDescriptor.wFlags.Fields.NoOutputReportAck));
  DEBUG ((DEBUG_INFO, "| 0x14 | Reserved                 | 0x%X\n", QuickSpiDev->DeviceDescriptor.Reserved));
  DEBUG ((DEBUG_INFO, "THC HID Protocol                  | %a  \n", (QuickSpiDev->HidSolutionFlag == ElanHidProtocol) ? "ELAN " : "WACOM "));
  DEBUG ((DEBUG_INFO, "QuickSpiPrintDeviceDescriptor () End\n"));
}

/**
  Initialize Memory for Prd used by the HW
  Only ReadRx2 and Write are supported by this driver

  @param[in]  QuickSpiDev        Context of QuickSpi device

  @retval EFI_SUCCESS            QuickSpi Dma Initialized successfuly
  @retval EFI_OUT_OF_RESOURCES   Not enough memory available
**/
EFI_STATUS
QuickSpiDmaCreatePrdCommonBuffers (
  IN  QUICK_SPI_DEV                *QuickSpiDev
  )
{
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINT64                        PageCount;
  UINT64                        ByteCount;
  EFI_STATUS                    Status;

  PciIo = QuickSpiDev->PciIo;

  PageCount = EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_READ_TABLES_SUPPORTED);
  Status = PciIo->AllocateBuffer (
                    PciIo,
                    AllocateAnyPages,
                    EfiBootServicesData,
                    PageCount,
                    (VOID*) &QuickSpiDev->DmaRead2.CommonPrdBuffer,
                    0
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiDmaCreateCommonBuffers Read2 buffer allocateBuffer failed Status %r\n", Status));
    return EFI_OUT_OF_RESOURCES;
  }

  ByteCount = EFI_PAGES_TO_SIZE (PageCount);
  Status    = PciIo->Map (
                       PciIo,
                       EfiPciIoOperationBusMasterCommonBuffer,
                       (VOID*) QuickSpiDev->DmaRead2.CommonPrdBuffer,
                       &ByteCount,
                       &QuickSpiDev->DmaRead2.CommonPrdBufferDevAddr,
                       &QuickSpiDev->DmaRead2.CommonPrdBufferMapping
                       );
  if (EFI_ERROR (Status) || (ByteCount != EFI_PAGES_TO_SIZE (PageCount))) {
    DEBUG ((DEBUG_WARN, "QuickSpiDmaCreateCommonBuffers Read2 buffer map failed Status %r\n", Status));
    DEBUG ((DEBUG_WARN, "QuickSpiDmaCreateCommonBuffers Requsted ByteCount: %d Mapped Count: %d\n", EFI_PAGES_TO_SIZE (PageCount), ByteCount));
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem ((UINT8*) QuickSpiDev->DmaRead2.CommonPrdBuffer, ByteCount);

  PageCount = EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_WRITE_TABLES_SUPPORTED);
  Status = PciIo->AllocateBuffer (
                    PciIo,
                    AllocateAnyPages,
                    EfiBootServicesData,
                    PageCount,
                    (VOID*) &QuickSpiDev->DmaWrite.CommonPrdBuffer,
                    0
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiDmaCreateCommonBuffers Read2 buffer allocateBuffer failed Status %r\n", Status));
    return EFI_OUT_OF_RESOURCES;
  }

  ByteCount = EFI_PAGES_TO_SIZE (PageCount);
  Status    = PciIo->Map (
                       PciIo,
                       EfiPciIoOperationBusMasterCommonBuffer,
                       (VOID*) QuickSpiDev->DmaWrite.CommonPrdBuffer,
                       &ByteCount,
                       &QuickSpiDev->DmaWrite.CommonPrdBufferDevAddr,
                       &QuickSpiDev->DmaWrite.CommonPrdBufferMapping
                       );
  if (EFI_ERROR (Status) || (ByteCount != EFI_PAGES_TO_SIZE (PageCount))) {
    DEBUG ((DEBUG_WARN, "QuickSpiDmaCreateCommonBuffers Write buffer map failed Status %r\n", Status));
    DEBUG ((DEBUG_WARN, "QuickSpiDmaCreateCommonBuffers Requsted ByteCount: %d Mapped Count: %d\n", EFI_PAGES_TO_SIZE (PageCount), ByteCount));
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem ((UINT8*) QuickSpiDev->DmaWrite.CommonPrdBuffer, ByteCount);

  return Status;
}

/**
  Fills single Prd Table with the defualt values

  @param[in]  QuickSpiDma      Dma that stores Driver PRD table
  @param[in]  PrdTableIndex    Index of current Prd table
**/
VOID
QuickSpiDmaFillSinglePrdTable (
  IN THC_DMA    *QuickSpiDma,
  IN UINT32     PrdTableIndex
  )
{
  PRD_TABLE     *PrdTable;
  UINT32        CurrentEntry;
  UINT32        EntryOffset;

  CurrentEntry = 0;
  EntryOffset  = QuickSpiDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries * PrdTableIndex;
  PrdTable =  (PRD_TABLE*) QuickSpiDma->CommonPrdBuffer;

  for (CurrentEntry = 0; CurrentEntry < QuickSpiDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries; CurrentEntry++) {
    CopyMem (
      &PrdTable->Entries[CurrentEntry + EntryOffset],
      &QuickSpiDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry],
      sizeof (PRD_ENTRY)
      );
  }
}

/**
  Fills every Prd Table with the defualt values

  @param[in]  QuickSpiDma           Dma that stores Driver PRD Table
**/
VOID
QuickSpiDmaFillPrdTables (
  THC_DMA                        *QuickSpiDma
  )
{
  UINT32                        PrdTableIndex;

  THC_LOCAL_DEBUG(L"QuickSpiDmaFillPrdTables Start ()\n")

  //
  // Loop through total number of Entries in Common Prd
  // Fill every entry with the corresponding data for the HW to use
  //
  for (PrdTableIndex = 0; PrdTableIndex < QuickSpiDma->NumOfPrdTables; PrdTableIndex++) {
    QuickSpiDmaFillSinglePrdTable (QuickSpiDma, PrdTableIndex);
  }
}

/**
  Initialize Driver Prd Tables
  Allocates and maps every single page for each used entry
  besed on the supported BufferSize for each MAXIMUM_READ_SIZE/MAXIMUM_WRITE_SIZE

  @param[in]  QuickSpiDev      Context of QuickSpi device
  @param[in]  QuickSpiDma      Dma that stores Driver PRD Table
  @param[in]  PciIoOperation   BusMasterRead or BusMasterWrite

  @retval EFI_SUCCESS            QuickSpi Dma Initialized successfully
  @retval EFI_OUT_OF_RESOURCES   Not enough memory available
**/
EFI_STATUS
QuickSpiDmaCreatePrds (
  IN QUICK_SPI_DEV                  *QuickSpiDev,
  IN THC_DMA                        *QuickSpiDma,
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

  THC_LOCAL_DEBUG(L"QuickSpiDmaCreatePrds Start ()\n")

  PciIo = QuickSpiDev->PciIo;

  for (PrdTableIndex = 0; PrdTableIndex < QuickSpiDma->NumOfPrdTables; PrdTableIndex++) {

    PageCount = EFI_SIZE_TO_PAGES (QuickSpiDma->BufferSize);
    Status    = PciIo->AllocateBuffer (
                         PciIo,
                         AllocateAnyPages,
                         EfiBootServicesData,
                         PageCount,
                         (VOID*) &QuickSpiDma->DriverPrdTable[PrdTableIndex].Buffer,
                         0
                         );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "QuickSpiDmaCreateReadPrds AllocateBuffer for PrdTableIndex: %d failed with Status %r\n", PrdTableIndex, Status));
      return EFI_OUT_OF_RESOURCES;
    }

    ByteCount = EFI_PAGES_TO_SIZE (PageCount);
    Status    = PciIo->Map (
                         PciIo,
                         PciIoOperation,
                         (VOID*) QuickSpiDma->DriverPrdTable[PrdTableIndex].Buffer,
                         &ByteCount,
                         &QuickSpiDma->DriverPrdTable[PrdTableIndex].DevAddr,
                         &QuickSpiDma->DriverPrdTable[PrdTableIndex].Mapping
                         );

    if (EFI_ERROR (Status) || (ByteCount != EFI_PAGES_TO_SIZE (PageCount))) {
      DEBUG ((DEBUG_WARN, "QuickSpiDmaCreateReadPrds Read buffer map failed PrdTableIndex: %d Status %r\n", PrdTableIndex, Status));
      DEBUG ((DEBUG_WARN, "QuickSpiDmaCreateReadPrds Requsted ByteCount: %d Mapped Count: %d\n", EFI_PAGES_TO_SIZE (PageCount), ByteCount));
      return EFI_OUT_OF_RESOURCES;
    }

    ZeroMem ((UINT8*) QuickSpiDma->DriverPrdTable[PrdTableIndex].Buffer, ByteCount);

    BytesRemaining   = ByteCount;
    CurrentEntry     = 0;
    CurrentAddress   = QuickSpiDma->DriverPrdTable[PrdTableIndex].Buffer;
    //
    // Loop through total number of Entries
    // Each entry Max length is 4K (size of the single page), max entries is 256.
    // Max allocated total for single PRD table is 1MB
    //
    while (BytesRemaining > 0 && CurrentEntry < 256) {
      QuickSpiDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].DestinationAddress = RShiftU64 (CurrentAddress, ADDRESS_SHIFT);
      QuickSpiDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].EndOfPrd           = 0x0;
      QuickSpiDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].Length             = EFI_PAGE_SIZE;
      if (BytesRemaining <= EFI_PAGE_SIZE) {
        QuickSpiDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].Length           = BytesRemaining;
        QuickSpiDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].EndOfPrd         = 0x1;
        break;
      }
      CurrentEntry++;
      CurrentAddress += EFI_PAGE_SIZE;
      BytesRemaining -= EFI_PAGE_SIZE;
    }
    if (CurrentEntry == 256) {
      QuickSpiDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry-1].EndOfPrd = 0x1;
      QuickSpiDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries = CurrentEntry;
      DEBUG ((DEBUG_WARN, "QuickSpiDmaCreatePrds QUICKSPI ERROR: 256 Entries are not sufficient for the TBL[%d]:NumOfConfiguredEntries=0x%x BytesRemaining=0x%x\n", PrdTableIndex, QuickSpiDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries, BytesRemaining));
    } else {
      QuickSpiDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries = CurrentEntry + 1;
    }
  }

  return EFI_SUCCESS;
}

/**
  Initialize QuickSpi DMA

  @param[in]  QuickSpiDev      Context of QuickSpi device

  @retval EFI_SUCCESS     QuickSpi Dma Initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickSpiDmaInitialize (
  IN QUICK_SPI_DEV                *QuickSpiDev
  )
{
  EFI_STATUS Status;
  THC_M_PRT_DEVINT_CFG_1      DevIntCfg1;
  UINT32                      MaxSize;

  THC_LOCAL_DEBUG(L"QuickSpiDmaInitialize Start ()\n")

  DevIntCfg1.Data32 = MmioRead32 (QuickSpiDev->PciBar0 + R_THC_MEM_PRT_DEVINT_CFG_1);
  DevIntCfg1.Fields.IntTypDataVal = 0xFFFF; // Enabling all interrupts
  MmioWrite32 (QuickSpiDev->PciBar0 + R_THC_MEM_PRT_DEVINT_CFG_1, DevIntCfg1.Data32);

  Status = QuickSpiDmaCreatePrdCommonBuffers (QuickSpiDev);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiDmaInitialize error, failed to CreatePrdCommonBuffers Status %r\n", Status));
    return Status;
  }

  Status = QuickSpiLibSetPrdTablesAddress (QuickSpiDev->PciBar0, QuickSpiDev->DmaWrite.CommonPrdBuffer, QuickSpiDev->DmaRead2.CommonPrdBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiDmaInitialize error, failed to SetPrdTablesAddress Status %r\n", Status));
    return Status;
  }

  MaxSize = MAX (QuickSpiDev->DeviceDescriptor.wMaxInputLength, QuickSpiDev->DeviceDescriptor.wReportDescLength);
  QuickSpiDev->DmaRead2.BufferSize     = (((MaxSize) + (THC_MAX_HID_BUFFER_SIZE) - 1) & ~((THC_MAX_HID_BUFFER_SIZE) - 1));
  QuickSpiDev->DmaRead2.NumOfPrdTables = PRD_READ_TABLES_SUPPORTED;
  QuickSpiDev->DmaWrite.BufferSize     = (((QuickSpiDev->DeviceDescriptor.wMaxOutputLength) + (THC_MAX_HID_BUFFER_SIZE) - 1) & ~((THC_MAX_HID_BUFFER_SIZE) - 1));
  QuickSpiDev->DmaWrite.NumOfPrdTables = PRD_WRITE_TABLES_SUPPORTED;

  Status = QuickSpiDmaCreatePrds (QuickSpiDev, &QuickSpiDev->DmaRead2, EfiPciIoOperationBusMasterCommonBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiDmaInitialize error, failed to Create Read Prds Status %r\n", Status));
    return Status;
  }

  QuickSpiDmaFillPrdTables (&QuickSpiDev->DmaRead2);

  Status = QuickSpiDmaCreatePrds (QuickSpiDev, &QuickSpiDev->DmaWrite, EfiPciIoOperationBusMasterCommonBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiDmaInitialize error, failed to Create Write Prds Status %r\n", Status));
    return Status;
  }

  QuickSpiDmaFillPrdTables (&QuickSpiDev->DmaWrite);

  QuickSpiLibSetLengthInPrd  (QuickSpiDev->PciBar0,
                         (UINT8) QuickSpiDev->DmaRead2.DriverPrdTable[0].NumOfConfiguredEntries,
                         PRD_READ_TABLES_SUPPORTED,
                         (UINT8) QuickSpiDev->DmaWrite.DriverPrdTable[0].NumOfConfiguredEntries
                        );

  QuickSpiLibSetReadRx2StartBit (QuickSpiDev->PciBar0);

  return EFI_SUCCESS;
}

/**
  Calculates the length of the current message

  @param[in]  QuickSpiDma        Dma that stores Driver PRD Table
  @param[in]  PrdTable           Prd Table updated by the THC HW
  @param[in]  PrdTableIndex      Index of the Prd Table
  @param[in]  EntryOffset        Entry offset

  @retval MessageLength          Size of the message
**/
UINT64
QuickSpiDmaGetMessageLength (
  IN THC_DMA                        *QuickSpiDma,
  IN PRD_TABLE                      *PrdTable,
  IN UINT16                         PrdTableIndex,
  IN UINT32                         EntryOffset
  )
{
  UINT32  Index;
  UINT64  MessageLength = 0;

  for (Index = 0; Index < QuickSpiDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries; Index++) {
    MessageLength += PrdTable->Entries[EntryOffset + Index].Length;
    if (PrdTable->Entries[EntryOffset + Index].EndOfPrd == 1) {
      return MessageLength;
    }
  }
  return MessageLength;
}

/**
  Reads first PRD Table Entry and checks if header data type
  equals TouchReadDataTypeHidReport
  and Report Id is equal to SINGLE_FINGER_REPORT_ID.
  If passed report data is copied to the QuickSpi Context.

  @param[in]  QuickSpiDev        Context of QuickSpi device
  @param[in]  QuickSpiDma        Dma that stores Driver PRD Table
  @param[in]  CurrentPrdTable    Index of current Prd table

  @retval EFI_SUCCESS            Data read done
  @retval EFI_BAD_BUFFER_SIZE    MessageLength is smaller than its header
  @retval EFI_BUFFER_TOO_SMALL   Allocated entry length is smaller the one present in Prd
**/
EFI_STATUS
QuickSpiDmaReadSingleReport (
  IN QUICK_SPI_DEV          *QuickSpiDev,
  IN THC_DMA                *QuickSpiDma,
  IN UINT16                 CurrentPrdTable
  )
{
  QUICK_SPI_READ_DATA_BUFF  *ReadDataBuff;
  PRD_TABLE                 *PrdTable;
  UINT64                    MessageLength = 0;
  UINT64                    DataAddress   = 0;
  UINT32                    EntryOffset   = 0;
  HID_TOUCH_OUTPUT          TouchOutput;
  HID_XY_BOUNDARY           XyBoundary;
  EFI_STATUS                Status;

  EntryOffset = (UINT32) (QuickSpiDma->DriverPrdTable[CurrentPrdTable].NumOfConfiguredEntries) * CurrentPrdTable;
  PrdTable    = (PRD_TABLE *) QuickSpiDma->CommonPrdBuffer;

  THC_LOCAL_DEBUG(L"QuickSpiDmaReadSingleReport Start ()\n")
  ReadDataBuff = AllocateZeroPool (sizeof (QUICK_SPI_READ_DATA_BUFF));
  if (ReadDataBuff == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  MessageLength = QuickSpiDmaGetMessageLength (QuickSpiDma, PrdTable, CurrentPrdTable, EntryOffset);
  THC_LOCAL_DEBUG(L"MessageLength %d \n", MessageLength)
  if (MessageLength < sizeof (QUICK_SPI_INPUT_REPORT_BODY_HEADER)) {
    FreePool (ReadDataBuff);
    return EFI_BAD_BUFFER_SIZE;
  }
  if (QuickSpiDma->DriverPrdTable[CurrentPrdTable].Entries[0].Length < PrdTable->Entries[EntryOffset].Length) {
    FreePool (ReadDataBuff);
    return EFI_BUFFER_TOO_SMALL;
  }

  DataAddress = LShiftU64 (PrdTable->Entries[EntryOffset].DestinationAddress, ADDRESS_SHIFT);
  CopyMem ((UINT8*) ReadDataBuff, (UINT8*) DataAddress, sizeof (QUICK_SPI_INPUT_REPORT_BODY_HEADER));

  THC_LOCAL_DEBUG(L"ReadDataBuff->Header.Fields.ReportType %d\n", ReadDataBuff->Header.Fields.ReportType)

  //ShowBuffer ((UINT8*)(DataAddress), (UINT32) MessageLength); // Uncomment when THC_LOCAL_DEBUG is enabled

  if ((ReadDataBuff->Header.Fields.ReportType  == QuickSpiInputReportGetFeatureResponse) &&
             (QuickSpiDev->HidActive == TRUE) &&
             (QuickSpiDev->HidBuffer != NULL)) {
    THC_LOCAL_DEBUG(L"QuickSpiDmaReadSingleReport TouchReadDataTypeGetFeatures\n")
    DEBUG ((DEBUG_INFO, "TouchReadDataTypeGetFeatures\n"));
    CopyMem ((UINT8*) QuickSpiDev->HidBuffer, (UINT8*) DataAddress + sizeof (QUICK_SPI_INPUT_REPORT_BODY_HEADER), MessageLength);
    QuickSpiDev->HidBuffer += MessageLength;
    QuickSpiDev->HidDataAvailable = TRUE;
  } else if ((ReadDataBuff->Header.Fields.ReportType  == QuickSpiInputReportSetFeatureResponse) &&
               (QuickSpiDev->HidActive == TRUE) &&
               (QuickSpiDev->HidBuffer != NULL)) {
    THC_LOCAL_DEBUG(L"QuickSpiDmaReadSingleReport QuickSpiInputReportSetFeatureResponse\n")
    CopyMem ((UINT8*) QuickSpiDev->HidBuffer, (UINT8*) DataAddress, MessageLength);
    QuickSpiDev->HidBuffer += MessageLength;
    QuickSpiDev->HidDataAvailable = TRUE;
  } else if ((ReadDataBuff->Header.Fields.ReportType  == QuickSpiInputReportReportDescriptor) &&
             (QuickSpiDev->HidActive == TRUE) &&
             (QuickSpiDev->HidBuffer != NULL)) {
    THC_LOCAL_DEBUG(L"QuickSpiDmaReadSingleReport QuickSpiInputReportReportDescriptor\n")
    CopyMem ((UINT8*) QuickSpiDev->HidBuffer, (UINT8*) DataAddress, MessageLength);
    QuickSpiDev->HidBuffer += MessageLength;
    QuickSpiDev->HidDataAvailable = TRUE;
  } else if (ReadDataBuff->Header.Fields.ReportType  == QuickSpiInputReportData) {
    // Initialize the TouchOutput X/Y values with previous pointer location.
    TouchOutput.B = 0;
    TouchOutput.X = (QuickSpiDev->Report.HigherXByte << 8) + (QuickSpiDev->Report.LowerXByte);
    TouchOutput.Y = (QuickSpiDev->Report.HigherYByte << 8) + (QuickSpiDev->Report.LowerYByte);
    Status = HidParseInput (QuickSpiDev->InputReportTable, (UINT8*) DataAddress, &TouchOutput, &XyBoundary, QuickSpiDev->HidSolutionFlag);
    if (Status == EFI_SUCCESS) {
      QuickSpiDev->Report.HigherXByte = (UINT8) (((TouchOutput.X) & 0xFF00) >> 8);
      QuickSpiDev->Report.LowerXByte  = (UINT8) (TouchOutput.X) & 0xFF;
      QuickSpiDev->Report.HigherYByte = (UINT8) (((TouchOutput.Y) & 0xFF00) >> 8);
      QuickSpiDev->Report.LowerYByte  = (UINT8) (TouchOutput.Y) & 0xFF;
      QuickSpiDev->Report.TouchSts = (UINT8) TouchOutput.B;
      QuickSpiDev->AbsPtrProtocol.Mode->AbsoluteMaxX = XyBoundary.MaxX;
      QuickSpiDev->AbsPtrProtocol.Mode->AbsoluteMinX = XyBoundary.MinX;
      QuickSpiDev->AbsPtrProtocol.Mode->AbsoluteMaxY = XyBoundary.MaxY;
      QuickSpiDev->AbsPtrProtocol.Mode->AbsoluteMinY = XyBoundary.MinY;
      QuickSpiDev->AbsPtrDataAvailable = TRUE;
      THC_LOCAL_DEBUG(L"Touch data: State | X:%d | Y:%d | ActiveButton: %d \n",
                      (QuickSpiDev->Report.HigherXByte << 8) + QuickSpiDev->Report.LowerXByte,
                      (QuickSpiDev->Report.HigherYByte << 8) + QuickSpiDev->Report.LowerYByte,
                      QuickSpiDev->Report.TouchSts)
      gBS->SignalEvent (QuickSpiDev->AbsPtrProtocol.WaitForInput);
    }
  }

  FreePool (ReadDataBuff);

  return EFI_SUCCESS;
}

/**
  Reads data from each Read Rx2 DMA PRD Table Entry until write pointer is equal to read pointer
  Write pointer is updated after each PRD Table read is finished

  @param[in]  QuickSpiDev               Context of QuickSpi device
  @param[in]  CurrentCBReadPointer      Circular Buffer Read Pointer
  @param[in]  CurrentCBWritePointer     Circular Buffer Write Pointer

**/
VOID
QuickSpiDmaReadRx2 (
  IN QUICK_SPI_DEV          *QuickSpiDev,
  IN UINT8                  CurrentCBWritePointer,
  IN UINT8                  CurrentCBReadPointer
  )
{
  THC_M_PRT_READ_DMA_CNTRL_2 ReadDmaCntrl2;
  EFI_STATUS                 Status;
  UINT8                      CurrentPrdTable;

  while (((CurrentCBWritePointer & POINTER_MASK) != (CurrentCBReadPointer & POINTER_MASK)) ||  // CB Not Empty
    (CurrentCBWritePointer == CurrentCBReadPointer)) {
    CurrentPrdTable = (UINT8) (CurrentCBWritePointer & POINTER_MASK);                                         // CB Full  CurrentPrdTable = (UINT8) (CurrentCBWritePointer & POINTER_MASK);
    THC_LOCAL_DEBUG(L"PrdTable %d CBWritePointer %d CBReadPointer %d CB 0x%X \n",
                     CurrentPrdTable,
                     CurrentCBWritePointer & POINTER_MASK,
                     CurrentCBReadPointer & POINTER_MASK,
                     CurrentCBWritePointer
                    )
    Status = QuickSpiDmaReadSingleReport (QuickSpiDev, &QuickSpiDev->DmaRead2, CurrentPrdTable);
    if (EFI_ERROR (Status)) {
      THC_LOCAL_DEBUG(L"QuickSpiDmaReadRx2 QuickSpiDmaReadbuffer error, failed to Read Data Buffer Status %r\n", Status)
      DEBUG ((DEBUG_ERROR, "QuickSpiDmaReadRx2 Error Read Data Buffer Status %r\n", Status));
      // No return to avoid dead loop, always change Write Pointer, even on bad data
    }

    // Return PRD table to its initial state
    QuickSpiDmaFillSinglePrdTable (&QuickSpiDev->DmaRead2, CurrentPrdTable);

    if ((CurrentCBWritePointer + 1) == WRAPAROUND_VALUE_16) {
      THC_LOCAL_DEBUG(L"WRAPAROUND_VALUE_16 \n")
      QuickSpiLibSetReadRx2WritePointer (QuickSpiDev->PciBar0, POINTER_WRAPAROUND);
    } else if ((CurrentCBWritePointer + 1) == WRAPAROUND_VALUE_0X90) {
      THC_LOCAL_DEBUG(L"WRAPAROUND_VALUE_0X90 \n")
      QuickSpiLibSetReadRx2WritePointer (QuickSpiDev->PciBar0, 0x0);
    } else {
      THC_LOCAL_DEBUG(L"WP +1\n")
      QuickSpiLibSetReadRx2WritePointer (QuickSpiDev->PciBar0, CurrentCBWritePointer + 1);
    }

    ReadDmaCntrl2.Data    = QuickSpiLibGetReadRx2Data (QuickSpiDev->PciBar0);
    CurrentCBWritePointer = (UINT8) ReadDmaCntrl2.Fields.QuickSpiPrdCbWritePointer;
    //
    // Cashing last WP
    //
    QuickSpiDev->LastWritePointer = CurrentCBWritePointer;
  }
}

/**
  Writes data to a single Tx DMA PRD Table

  @param[in]  QuickSpiDev        Context of QuickSpi device
  @param[in]  Buffer             Data Container that will be sent to external device
  @param[in]  DataHeader         Write Data Header

  @retval EFI_BUFFER_TOO_SMALL   THC DMA buffer is unable to fit that much data
  @retval EFI_TIMEOUT            DMA transaction did not finish in time
  @retval EFI_SUCCESS            Data sent
**/
EFI_STATUS
QuickSpiDmaWriteTx (
  IN QUICK_SPI_DEV                      *QuickSpiDev,
  IN QUICK_SPI_OUTPUT_REPORT_HEADER     OutputHeader,
  IN UINT8                              *DataBuffer
  )
{
  PRD_TABLE   *PrdTable;
  UINT64      DataAddress;
  UINT32      PrdEntryLength;
  UINT32      WriteLength;
  UINT32      EntryIndex;
  UINT8       ByteAlignmentCount;

  DataAddress         = 0;
  PrdEntryLength      = 0;
  EntryIndex          = 0;
  ByteAlignmentCount  = 0;
  PrdTable            = (PRD_TABLE *) QuickSpiDev->DmaWrite.CommonPrdBuffer;

  if (OutputHeader.Fields.ContentLength % 4 > 0) {
    ByteAlignmentCount = 4 - OutputHeader.Fields.ContentLength % 4;
  }

  if ((PrdTable->Entries[0].Length * PRD_MAX_ENTRIES) < (OutputHeader.Fields.ContentLength + ByteAlignmentCount + sizeof (QUICK_SPI_OUTPUT_REPORT_HEADER))) {
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // Copy Header information, increase address and set remaining length of the first entry
  //
  DataAddress = LShiftU64 (PrdTable->Entries[EntryIndex].DestinationAddress, ADDRESS_SHIFT);
  //
  // DMA QUICK_SPI_OUTPUT_REPORT_HEADER
  //
  CopyMem ((UINT8*) DataAddress, &OutputHeader, sizeof (QUICK_SPI_OUTPUT_REPORT_HEADER));

  DataAddress += sizeof (QUICK_SPI_OUTPUT_REPORT_HEADER);
  PrdEntryLength = (UINT32) PrdTable->Entries[EntryIndex].Length - sizeof (QUICK_SPI_OUTPUT_REPORT_HEADER);

  do {
    PrdTable->Entries[EntryIndex].EndOfPrd = 0;
    //
    // Set write length for given Prd Entry
    //
    if (OutputHeader.Fields.ContentLength >= PrdEntryLength) {
      WriteLength = PrdEntryLength;
    } else {
      WriteLength = OutputHeader.Fields.ContentLength;
    }
    //
    // Put the data in the destination address
    //
    if (WriteLength > 0) {
      CopyMem ((UINT8*) DataAddress, DataBuffer, WriteLength);
    }
    //
    // Substract copied data from the ContentLength
    //
    OutputHeader.Fields.ContentLength = OutputHeader.Fields.ContentLength - WriteLength;

    //
    // Store new write length along with the header in Prd entry.
    //
    if (EntryIndex == 0) {
      WriteLength += sizeof (QUICK_SPI_OUTPUT_REPORT_HEADER);
    }
    PrdTable->Entries[EntryIndex].Length = WriteLength;
    THC_LOCAL_DEBUG(L"QuickSpiDmaWriteTx WriteLength %d\n", WriteLength)
    THC_LOCAL_DEBUG(L"QuickSpiDmaWriteTx OutputHeader.Fields.ContentLength %d\n", OutputHeader.Fields.ContentLength)
    if (OutputHeader.Fields.ContentLength > 0) {
      DataBuffer += WriteLength;
      EntryIndex++;
      DataAddress = LShiftU64 (PrdTable->Entries[EntryIndex].DestinationAddress, ADDRESS_SHIFT);
      PrdEntryLength = (UINT32) PrdTable->Entries[EntryIndex].Length;
    } else {
      break;
    }
  } while (EntryIndex < QuickSpiDev->DmaWrite.DriverPrdTable[0].NumOfConfiguredEntries);

  //
  // Fill Prd to meet DW alignment
  //
  if (ByteAlignmentCount > 0) {
    PrdTable->Entries[EntryIndex].Length += ByteAlignmentCount;
    THC_LOCAL_DEBUG(L"QuickSpiDmaWriteTx PrdTable->Entries[EntryIndex].Length %d\n", PrdTable->Entries[EntryIndex].Length)
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
  QuickSpiLibSetWriteStartBit (QuickSpiDev->PciBar0);

  //
  // Poll for Dma Complete
  //
  if (EFI_ERROR (QuickSpiLibPollDmaComplete (QuickSpiDev->PciBar0, WritePollingTimeout))) {
    return EFI_TIMEOUT;
  }

  //
  // Reset PRD table settings
  //
  QuickSpiDmaFillSinglePrdTable (&QuickSpiDev->DmaWrite, 0);

  return EFI_SUCCESS;
}

/**
  Unmaps and frees Driver Prd Tables

  @param[in]  QuickSpiDev             Context of QuickSpi device
  @param[in]  QuickSpiDma             Dma that stores Driver PRD Table

**/
VOID
QuickSpiDmaFreePrds (
  IN QUICK_SPI_DEV                  *QuickSpiDev,
  IN THC_DMA                        *QuickSpiDma
  )
{
  UINT32         PrdTableIndex;

  for (PrdTableIndex = 0; PrdTableIndex < QuickSpiDma->NumOfPrdTables; PrdTableIndex++) {
    QuickSpiDev->PciIo->Unmap (
                        QuickSpiDev->PciIo,
                        QuickSpiDma->DriverPrdTable[PrdTableIndex].Mapping
                        );
    QuickSpiDev->PciIo->FreeBuffer (
                          QuickSpiDev->PciIo,
                          EFI_SIZE_TO_PAGES (QuickSpiDma->BufferSize),
                          (UINT8 *) QuickSpiDma->DriverPrdTable[PrdTableIndex].Buffer
                          );
  }
}

/**
  Stops Touch Panel and brings it back to reset state
  Frees all allocated memory and QuickSpi device context

  @param[in]  QuickSpiDev         Context of QuickSpi device
  @param[in]  HidOverSpi          Hid Over Spi policy data
  @param[in]  Reset               Reset data

**/
VOID
EFIAPI
QuickSpiStop (
  IN QUICK_SPI_DEV                *QuickSpiDev,
  IN THC_HID_OVER_SPI             *HidOverSpi,
  IN THC_RESET                    *Reset
  )
{
  if (QuickSpiDev == NULL) {
    DEBUG ((DEBUG_WARN, "QuickSpiStop QuickSpiDev is already Stopped\n"));
    return;
  }

  THC_LOCAL_DEBUG(L"QuickSpiStop\n")

  QuickSpiLibStopDma (QuickSpiDev->PciBar0);
  QuickSpiStopAbsPtr (QuickSpiDev);
  QuickSpiLibSetReset (QuickSpiDev->PciBar0, QuickSpiDev->InstanceId, Reset->ResetPadTrigger);
  QuickSpiLibCleanUp (QuickSpiDev->PciBar0);

  //
  // Free/Unmap Common Buffers and Prd Tables
  //
  if (QuickSpiDev->DmaRead2.CommonPrdBuffer != 0) {
    THC_LOCAL_DEBUG(L"QuickSpiStop Free DmaRead2.CommonPrdBuffer\n")
    QuickSpiDev->PciIo->Unmap (
                        QuickSpiDev->PciIo,
                        QuickSpiDev->DmaRead2.CommonPrdBufferMapping
                        );
    QuickSpiDev->PciIo->FreeBuffer (
                        QuickSpiDev->PciIo,
                        EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_READ_TABLES_SUPPORTED),
                        (UINT8 *) QuickSpiDev->DmaRead2.CommonPrdBuffer
                        );
    QuickSpiDmaFreePrds (QuickSpiDev, &QuickSpiDev->DmaRead2);
  }
  if (QuickSpiDev->DmaWrite.CommonPrdBuffer != 0) {
    THC_LOCAL_DEBUG(L"QuickSpiStop Free DmaWrite.CommonPrdBuffer\n")
    QuickSpiDev->PciIo->Unmap (
                        QuickSpiDev->PciIo,
                        QuickSpiDev->DmaWrite.CommonPrdBufferMapping
                        );
    QuickSpiDev->PciIo->FreeBuffer (
                        QuickSpiDev->PciIo,
                        EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_WRITE_TABLES_SUPPORTED),
                        (UINT8 *) QuickSpiDev->DmaWrite.CommonPrdBuffer
                        );
    QuickSpiDmaFreePrds (QuickSpiDev, &QuickSpiDev->DmaWrite);
  }

  //
  // Restore original PCI attributes
  //
  QuickSpiDev->PciIo->Attributes (
                   QuickSpiDev->PciIo,
                   EfiPciIoAttributeOperationSet,
                   QuickSpiDev->PciAttributes,
                   NULL
                   );

  //
  // Close and Uninstall PCI I/O Protocol that was opened with PROTOCOL_BY_DRIVER
  //
  gBS->CloseProtocol (
         QuickSpiDev->ControllerHandle,
         &gEfiPciIoProtocolGuid,
         &QuickSpiDev->PciIo,
         QuickSpiDev->ControllerHandle
         );
  gBS->UninstallMultipleProtocolInterfaces (
         QuickSpiDev->ControllerHandle,
         &gEfiPciIoProtocolGuid,
         &QuickSpiDev->PciIo,
         NULL
         );

  //
  // Done with the controller name table
  //
  FreeUnicodeStringTable (QuickSpiDev->ControllerNameTable);

  //
  //  Done with the driver context
  //
  ZeroMem (QuickSpiDev, sizeof (QUICK_SPI_DEV));
  FreePool (QuickSpiDev);
}

/**
  Polls on QuickSpi Rx2 Circullar Buffers state
  If CB Write and Read Pointer are not equal Read Rx2 Dma is trigerred

  @param[in]  Event       Polling event
  @param[in]  Context     Context of QuickSpi device

**/
VOID
EFIAPI
QuickSpiPolling (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  QUICK_SPI_DEV                *QuickSpiDev;
  THC_M_PRT_READ_DMA_CNTRL_2   ReadDmaCntrl2;
  THC_M_PRT_READ_DMA_INT_STS_2 ReadDmaIntSts2;
  UINT32                       Timeout;
  UINT8                        CurrentPrdTable;
  UINT8                        PrdLoopCounter;

  QuickSpiDev = (QUICK_SPI_DEV *) Context;

  if (QuickSpiDev == NULL) {
    DEBUG ((DEBUG_ERROR, "QuickSpiPolling QuickSpiDev is Null\n"));
    return;
  }
  if (QuickSpiDev->ReadDone) {
    Timeout = PollingLoopTimeout;
    ReadDmaCntrl2.Data    = QuickSpiLibGetReadRx2Data (QuickSpiDev->PciBar0);
    ReadDmaIntSts2.Data32 = QuickSpiLibGetReadRx2IntSts (QuickSpiDev->PciBar0);
    if (ReadDmaIntSts2.Fields.StallSts == 1) {
      DEBUG ((DEBUG_WARN, "QuickSpi Stall detected.\n"));
      //
      // 1. Read all PRD tables
      //
      CurrentPrdTable = (UINT8) (QuickSpiDev->LastWritePointer & POINTER_MASK);
      for (PrdLoopCounter = PRD_READ_TABLES_SUPPORTED; PrdLoopCounter > 0; PrdLoopCounter--) {
        if (CurrentPrdTable + 1 >= 16) {
          CurrentPrdTable = 0;
        } else {
          CurrentPrdTable++;
        }
        QuickSpiDmaReadSingleReport (QuickSpiDev, &QuickSpiDev->DmaRead2, CurrentPrdTable);
        //Return PRD table to its initial state
        QuickSpiDmaFillSinglePrdTable (&QuickSpiDev->DmaRead2, CurrentPrdTable);
      }
      //
      // 2. Reset CB pointer and STOP DMA
      //
      ReadDmaCntrl2.Fields.Start                     = 0;
      ReadDmaCntrl2.Fields.QuickSpiPrdCbPointerReset = 1;
      MmioWrite32 (QuickSpiDev->PciBar0 + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);
      //
      // 3. Clear Global error and stall
      //
      ReadDmaIntSts2.Fields.StallSts = 1;
      ReadDmaIntSts2.Fields.ErrorSts = 1;
      MmioWrite32 (QuickSpiDev->PciBar0 + R_THC_MEM_PRT_READ_DMA_INT_STS_2, ReadDmaIntSts2.Data32);
      ReadDmaCntrl2.Data    = QuickSpiLibGetReadRx2Data (QuickSpiDev->PciBar0);
      //
      // 4. Start DMA
      //
      QuickSpiLibSetReadRx2StartBit (QuickSpiDev->PciBar0);
      ReadDmaCntrl2.Data = QuickSpiLibGetReadRx2Data (QuickSpiDev->PciBar0);
      //
      // Reset cashed WP
      //
      QuickSpiDev->LastWritePointer = (UINT8) ReadDmaCntrl2.Fields.QuickSpiPrdCbWritePointer;
      return;
    }
    while ((((UINT8) (ReadDmaCntrl2.Fields.QuickSpiPrdCbWritePointer  & POINTER_MASK)) != ((UINT8) (ReadDmaCntrl2.Fields.QuickSpiPrdCbReadPointer & POINTER_MASK))) ||
      ((UINT8) ReadDmaCntrl2.Fields.QuickSpiPrdCbWritePointer == (UINT8) ReadDmaCntrl2.Fields.QuickSpiPrdCbReadPointer)) {                                             // CB Full  
      QuickSpiDev->ReadDone = FALSE;
      QuickSpiDmaReadRx2 (QuickSpiDev, (UINT8) ReadDmaCntrl2.Fields.QuickSpiPrdCbWritePointer, (UINT8) ReadDmaCntrl2.Fields.QuickSpiPrdCbReadPointer);
      ReadDmaCntrl2.Data = QuickSpiLibGetReadRx2Data (QuickSpiDev->PciBar0);
      ReadDmaIntSts2.Data32 = QuickSpiLibGetReadRx2IntSts (QuickSpiDev->PciBar0);
      if (ReadDmaIntSts2.Fields.StallSts == 1) {
        DEBUG ((DEBUG_ERROR, "QuickSpiPolling Stall while detected ! ReadDmaIntSts2: 0x%X\n", ReadDmaIntSts2.Data32));
        THC_LOCAL_DEBUG(L"QuickSpiPollingStall while detected ! ReadDmaIntSts2: 0x%X\n", ReadDmaIntSts2.Data32);
        QuickSpiDev->ReadDone = TRUE;
        return;
      }
      if (((UINT8) (ReadDmaCntrl2.Fields.QuickSpiPrdCbWritePointer  & POINTER_MASK)) == ((UINT8) (ReadDmaCntrl2.Fields.QuickSpiPrdCbReadPointer & POINTER_MASK))) {
        QuickSpiDev->ReadDone = TRUE;
      }
      if (Timeout == 0) {
        DEBUG ((DEBUG_ERROR, "QuickSpiPolling Timeout while reading DMA, possible interrupt storm\n"));
        ASSERT (FALSE);
        QuickSpiDev->ReadDone = TRUE;
        return;
      }
      MicroSecondDelay (10);
      Timeout -= 10;
    }
  }
}

/**
  Enables end point device.
 - TBD

  @param[in]  QuickSpiDev           Context of QuickSpi device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_DEVICE_ERROR     TSSDONE not set or ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickSpiEnableAfterReset (
  IN  QUICK_SPI_DEV              *QuickSpiDev
  )
{
/**
  EFI_STATUS Status;

  QuickSpiReadDeviceDescriptor (QuickSpiDev);
  QuickSpiPrintDeviceDescriptor (QuickSpiDev);

  QuickSpiLibPrepareWriteConfigCmd (QuickSpiDev->PciBar0);
//  QuickSpiDev->DeviceRegisters.TouchCfg.Fields.TouchEnable = 0x1;
//  QuickSpiLibWriteSeqData1 (QuickSpiDev->PciBar0, QuickSpiDev->DeviceRegisters.TouchCfg.Data);
  QuickSpiLibGo (QuickSpiDev->PciBar0);

  Status = QuickSpiLibWaitCycleCompleted (QuickSpiDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG(L"QuickSpiEnableEndDevice  QuickSpiLibWaitCycleCompleted Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickSpiEnableEndDevice error, Status %r\n", Status));
    return Status;
  }
**/
  return EFI_NOT_AVAILABLE_YET;
}

/**
  Reads HID Descriptor

  @param[in]  QuickSpiDev           Context of QuickSpi device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device did not respond to request for report descriptro
  @retval EFI_TIMEOUT          Timeout while waiting for response from the device
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for QuickSpi HID support
**/
EFI_STATUS
QuickSpiReadHidDescriptor (
  IN  QUICK_SPI_DEV              *QuickSpiDev
  )
{
  EFI_STATUS          Status;
  UINT32              Timeout;
  UINT32              HidMessageLength;
  UINT8               *Buffer;
  QUICK_SPI_OUTPUT_REPORT_HEADER      OutputReportHeader;

  THC_LOCAL_DEBUG(L"QuickSpiReadHidDescriptor\n")

  OutputReportHeader.Fields.ReportType    = QuickSpiOutputReportReportDescriptor;
  OutputReportHeader.Fields.ContentLength = 0x0;
  OutputReportHeader.Fields.ContentId     = 0x0;

  QuickSpiDev->HidDataAvailable = FALSE;
  QuickSpiDev->HidActive        = TRUE;
  QuickSpiDev->HidBuffer        = AllocateAlignedPages (EFI_SIZE_TO_PAGES (THC_MAX_HID_BUFFER_SIZE), sizeof (UINT8));
  if (QuickSpiDev->HidBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Buffer = QuickSpiDev->HidBuffer;

  ZeroMem (QuickSpiDev->HidBuffer, THC_MAX_HID_BUFFER_SIZE);

  Status = QuickSpiDmaWriteTx (QuickSpiDev, OutputReportHeader, NULL);
  THC_LOCAL_DEBUG(L"QuickSpiReadHidDescriptor QuickSpiDmaWriteTx: %r\n", Status)
  if (EFI_ERROR (Status)) {
    FreeAlignedPages (QuickSpiDev->HidBuffer, EFI_SIZE_TO_PAGES (THC_MAX_HID_BUFFER_SIZE));
    return Status;
  }

  Timeout = HidResponseTimeout;

  //
  // Wait for Response
  //
  if (Timeout > 0) {
    THC_LOCAL_DEBUG(L"QuickSpiReadHidDescriptor Waiting for Response \n")
    do {
      if (QuickSpiDev->HidDataAvailable == TRUE && QuickSpiDev->ReadDone == TRUE) {
        Status = EFI_SUCCESS;
        break;
      }
      MicroSecondDelay (1000);
      Timeout--;
    } while (Timeout > 0);

    if (Timeout == 0) {
      THC_LOCAL_DEBUG(L"QuickSpiReadHidDescriptor Response Timeout\n")
      DEBUG ((DEBUG_ERROR, "QuickSpiReadHidDescriptor Response Timeout\n"));
      return EFI_TIMEOUT;
    }
  }

  QuickSpiDev->HidDataAvailable = FALSE;
  QuickSpiDev->HidActive        = FALSE;

  //
  // Restore initial pointer
  //
  QuickSpiDev->HidBuffer = Buffer;

  //
  // Calculate Recieved data length
  //
  HidMessageLength = ((QUICK_SPI_INPUT_REPORT_BODY_HEADER*) (QuickSpiDev->HidBuffer))->Fields.ContentLength;

  //
  // Shift the pointer to data location
  //
  QuickSpiDev->HidBuffer += sizeof (QUICK_SPI_INPUT_REPORT_BODY_HEADER);

  THC_LOCAL_DEBUG(L"QuickSpiReadHidDescriptor HidMessageLength = %d\n", HidMessageLength)
  //
  // Parse HID data
  //
  HidParseDescriptor (QuickSpiDev, QuickSpiDev->HidBuffer, HidMessageLength);

  THC_LOCAL_DEBUG(L"QuickSpiReadHidDescriptor End\n")

  return EFI_SUCCESS;
}

/**
  Touch Enable

  @param[in]  QuickSpiDev      Context of QuickSpi device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device did not respond to Set Feature command
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for QuickSpi HID support
**/
EFI_STATUS
QuickSpiTouchEnable (
  IN  QUICK_SPI_DEV              *QuickSpiDev
  )
{
  EFI_STATUS          Status;
  UINT32              Timeout;
  UINT8               Content;
  QUICK_SPI_OUTPUT_REPORT_HEADER      OutputReportHeader;

  THC_LOCAL_DEBUG(L"QuickSpiTouchEnable\n")

  OutputReportHeader.Fields.ReportType    = QuickSpiOutputReportSetFeature;
  OutputReportHeader.Fields.ContentLength = 0x2;
  OutputReportHeader.Fields.ContentId     = 0xe;
  Content = 0x2;

  QuickSpiDev->HidDataAvailable = FALSE;
  QuickSpiDev->HidActive        = TRUE;
  QuickSpiDev->HidBuffer        = AllocateAlignedPages (EFI_SIZE_TO_PAGES (THC_MAX_HID_BUFFER_SIZE), sizeof (UINT8));
  if (QuickSpiDev->HidBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = QuickSpiDmaWriteTx (QuickSpiDev, OutputReportHeader, &Content);
  THC_LOCAL_DEBUG(L"QuickSpiTouchEnable QuickSpiDmaWriteTx: %r\n", Status)
  if (EFI_ERROR (Status)) {
    FreeAlignedPages (QuickSpiDev->HidBuffer, EFI_SIZE_TO_PAGES (THC_MAX_HID_BUFFER_SIZE));
    return Status;
  }

  Timeout = HidResponseTimeout;

  //
  // Wait for Response
  //
  if (Timeout > 0) {
    THC_LOCAL_DEBUG(L"QuickSpiTouchEnable Waiting for Response \n")
    DEBUG ((DEBUG_INFO, "QuickSpiTouchEnable Waiting for Response \n"));
    do {
      if (QuickSpiDev->HidDataAvailable == TRUE && QuickSpiDev->ReadDone == TRUE) {
        Status = EFI_SUCCESS;
        break;
      }
      MicroSecondDelay (1000);
      Timeout--;
    } while (Timeout > 0);

    if (Timeout == 0) {
      // Output reponse is optional
      THC_LOCAL_DEBUG(L"QuickSpiTouchEnable Response Timeout\n")
      DEBUG ((DEBUG_WARN, "QuickSpiTouchEnable Response Timeout\n"));
      return EFI_NO_RESPONSE;
    }
  }

  QuickSpiDev->HidDataAvailable = FALSE;
  QuickSpiDev->HidActive        = FALSE;

  // Todo - add response check once panel supports it.

  return EFI_SUCCESS;
}

