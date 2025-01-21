/** @file
  CPU CrashLog PEI driver

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

#include "CpuCrashLogPei.h"

/**
  Program BAR address and enable command register memory space decoding

  @param[in,out] MmioBase    Base address of MMIO
  @param[in] TBir            tBIR used to indicate which BAR to be used
  @param[out] SavedStsCmd    Original value of command registers
  @param[out] MmioProgramed  Flag to indicate MMIO programed by this function

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
EnableDevice10Mmio (
  IN OUT UINT32   *MmioBase,
  IN     UINT32    TBir,
  OUT    UINT16   *SavedStsCmd,
  OUT    BOOLEAN  *MmioProgramed
  )
{
  UINT64  PciCfgBase;
  UINT16  OriStsCmd;
  UINT64  PciBarBaseOffset;

  *MmioProgramed = FALSE;

  PciCfgBase = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, TEL_BUS_NUM, TEL_DEV_NUM, TEL_FUN_NUM, 0);
  if (PciSegmentRead16 (PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_ERROR, "Device 10 is disabled\n"));
    *MmioBase = 0;
    return EFI_DEVICE_ERROR;
  }

  if (TBir == V_TEL_DVSEC_TBIR_BAR0) {
    PciBarBaseOffset = PciCfgBase + R_TEL_CFG_BAR0;
  } else if (TBir == V_TEL_DVSEC_TBIR_BAR1) {
    PciBarBaseOffset = PciCfgBase + R_TEL_CFG_BAR1;
  } else {
    return EFI_DEVICE_ERROR;
  }

  OriStsCmd = PciSegmentRead16 (PciCfgBase + PCI_COMMAND_OFFSET);

  if ((OriStsCmd & EFI_PCI_COMMAND_MEMORY_SPACE) == 0) {
    //
    // Program BAR 0 if the BAR is not programmed
    //
    PciSegmentWrite32 (PciBarBaseOffset, *MmioBase);

    //
    // Save and Enable command register memory space decoding
    //
    PciSegmentOr16 (PciCfgBase + PCI_COMMAND_OFFSET, (UINT16) (EFI_PCI_COMMAND_MEMORY_SPACE));
    *MmioProgramed = TRUE;
    *SavedStsCmd = OriStsCmd;

  } else {
    *MmioBase = PciSegmentRead32 (PciBarBaseOffset);
  }

  DEBUG ((DEBUG_INFO, "Devcie 10 pci cfg base = 0x%X\n", PciCfgBase));
  DEBUG ((DEBUG_INFO, "Device 10 MMIO base = 0x%X\n", *MmioBase));

  return EFI_SUCCESS;
}

/**
  Restore BAR address and command register

  @param[in] TBir         tBIR used to indicate which BAR to be used
  @param[in] SavedStsCmd  Original value of command registers

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
RestoreDevice10Mmio (
  IN UINTN   TBir,
  IN UINT16  SavedStsCmd
  )
{
  UINT64  PciCfgBase;

  PciCfgBase = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, TEL_BUS_NUM, TEL_DEV_NUM, TEL_FUN_NUM, 0);
  if (PciSegmentRead16 (PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_ERROR, "Device 10 is disabled\n"));
    return EFI_DEVICE_ERROR;
  }

  PciSegmentAnd16 (PciCfgBase + PCI_COMMAND_OFFSET, SavedStsCmd);
  if (TBir == V_TEL_DVSEC_TBIR_BAR0) {
    PciSegmentWrite32 (PciCfgBase + R_TEL_CFG_BAR0, 0);
  } else if (TBir == V_TEL_DVSEC_TBIR_BAR1) {
    PciSegmentWrite32 (PciCfgBase + R_TEL_CFG_BAR1, 0);
  }
  return EFI_SUCCESS;
}


/**
  Read data from MMIO to memory or write data to MMIO

  @param[in,out] MemBuffer  Destination: Memory address.
  @param[in]     MmioAddr   Source: MMIO address.
  @param[in]     Count      The number of memory or I/O operations to perform.
  @param[in]     Write      Indicate read or write operation.

  @retval EFI_SUCCESS  MMIO access sucessfully.

**/
EFI_STATUS
AccessMmioDword (
  IN OUT VOID     *MemBuffer,
  IN     UINT64   MmioAddr,
  IN     UINT32   Count,
  IN     BOOLEAN  Write
  )
{
  UINT32  *DataPtr;
  UINT32  Index;

  if (MemBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DataPtr = (UINT32*)MemBuffer;
  for (Index = 0; Index < Count; Index ++, DataPtr ++, MmioAddr += 4) {
    if (Write) {
      MmioWrite32 ((UINTN)MmioAddr, *DataPtr);
    } else {
      *DataPtr = MmioRead32 ((UINTN)MmioAddr);
    }
  }

  return EFI_SUCCESS;
}

/**
  Retrieve crashlog dvsec

  @param[out] CrashLogDevscCap  Pointer to structure of CrashLog DEVSC Data

  @retval EFI_UNSUPPORTED  Cpu CrashLog ID not found
  @retval EFI_SUCCESS      Cpu CrashLog ID found
**/
EFI_STATUS
GetCrashLogDvsec (
  OUT DVSEC_STRUCT  *CrashLogDvsec
  )
{
  UINT64        DeviceCfgBase;
  UINT64        DvsecAddr;
  DVSEC_STRUCT  Dvsec;
  UINT32        Index;

  if (CrashLogDvsec == NULL)  {
    return EFI_INVALID_PARAMETER;
  }

  DeviceCfgBase = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, TEL_BUS_NUM, TEL_DEV_NUM, TEL_FUN_NUM, 0);
  if (PciSegmentRead16 (DeviceCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_ERROR, "Device 10 is disabled\n"));
    return EFI_UNSUPPORTED;
  }

  DvsecAddr = DeviceCfgBase + R_TEL_DVSEC_OFFSET;
  while (TRUE) {
    ZeroMem (&Dvsec, sizeof (DVSEC_STRUCT));
    for (Index = 0; Index < sizeof (DVSEC_STRUCT)/sizeof (UINT32); Index ++) {
      Dvsec.Data32[Index] = PciSegmentRead32 (DvsecAddr + Index*4);
    }

    //
    // Find crashlog dvsec
    //
    if ((Dvsec.Fields.PcieCapId == V_TELEMETRY_EXTENDED_CAP_ID) &&
        (Dvsec.Fields.DevscId == V_CRASHLOG_DVSEC_ID)) {
      break;
    }

    if ((Dvsec.Fields.NextDvsecOffset == 0) || (Dvsec.Fields.NextDvsecOffset == 0xFFF)) {
      return EFI_UNSUPPORTED;
    }

    DvsecAddr = DeviceCfgBase + Dvsec.Fields.NextDvsecOffset;
  }

  CopyMem (CrashLogDvsec, &Dvsec, sizeof (DVSEC_STRUCT));
  DEBUG ((DEBUG_INFO, "  Discovery Table offset = 0x%x\n", CrashLogDvsec->Fields.DiscoveryTableOffset));
  DEBUG ((DEBUG_INFO, "  tbir = 0x%x\n", CrashLogDvsec->Fields.TBir));

  return EFI_SUCCESS;
}

/**
  Check if set storage-off bit success

  @retval     BOOLEAN            True if set storage-off bit success
**/
BOOLEAN
IsSetStorageOff (
  VOID
  )
{
  UINT32                               StallCount;

  StallCount = 0;

  //
  // Poll the storage-off status bit until bit get set or timeout
  //
  do {
    AccessMmioDword (&CpuAgent->DiscoveryTbl, CpuAgent->DiscoveryTblAddr, 1, READ);
    MicroSecondDelay (CPU_CRASHLOG_COMMAND_WAIT_STALL);
    StallCount ++;
  } while (CpuAgent->CapStsPtr->StorageOffSts == 0 && StallCount < CPU_CRASHLOG_COMMAND_WAIT_TIMEOUT);

  if (CpuAgent->CapStsPtr->StorageOffSts == 0) {
    return FALSE;
  }

  return TRUE;
}

/**
  Print discovery table

  @retval EFI_SUCCESS  The function completes successfully
**/
EFI_STATUS
PrintDiscoveryTbl (
  VOID
  )
{
  UINT32  Index;

  DEBUG ((DEBUG_INFO, " -------------------------------  \n"));
  DEBUG ((DEBUG_INFO, " ---- PUNIT Discovery table ----  \n"));
  DEBUG ((DEBUG_INFO, " -------------------------------  \n"));
  DEBUG ((DEBUG_INFO, " Table base = 0x%08x \n", CpuAgent->DiscoveryTblAddr));

  for (Index = 0; Index < sizeof (DISCOVERY_STRUCT_VER_1)/sizeof (UINT32); Index ++) {
    DEBUG ((DEBUG_INFO, " 0x%08x  \n", CpuAgent->DiscoveryTbl.Data32[Index]));
  }
  DEBUG ((DEBUG_INFO, " -------------------------------  \n"));
  return EFI_SUCCESS;
}


/**
  Print crash record information

  @retval EFI_SUCCESS  The function completes successfully
**/
EFI_STATUS
PrintCrashRecordInfo (
  VOID
  )
{
  UINT32    Index;

  DEBUG ((DEBUG_INFO, " -----------------------------------------------  \n"));
  DEBUG ((DEBUG_INFO, " --------------- Crash record info -------------  \n"));
  DEBUG ((DEBUG_INFO, " -----------------------------------------------  \n"));
  for (Index = 0; Index < CpuAgent->DiscoveryTbl.Header.DW0.Count; Index ++) {
    if (CpuAgent->CrashRecordInfo[Index].Valid) {
      DEBUG ((DEBUG_INFO, " record %d:: Addr= 0x%08x\n", Index, CpuAgent->CrashRecordInfo[Index].Addr));
    }
  }
  DEBUG ((DEBUG_INFO, " -----------------------------------------------  \n"));
  return EFI_SUCCESS;
}

/**
  Get address, size of crash record on SSRAM

  @param[in] Index        Specifies the crash record which the function wants to get
  @param[out] RecordAddr  Address of crashlog
  @param[out] RecordSize  Size of crashlog

  @retval EFI_SUCCESS  The function completes successfully
**/
EFI_STATUS
GetCrashRecordInfoOfIndex (
  IN  UINT32  Index,
  OUT UINT64  *RecordAddr,
  OUT UINT32  *RecordSize
  )
{
  EFI_STATUS                        Status;
  DISCOVERY_STRUCT_VER_1            DiscoveryStruct;
  UINT64                            MmioAddr;

  if(RecordAddr == NULL ||
     RecordSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Index start from 0, it must small than total record count
  if (Index >= CpuAgent->DiscoveryTbl.Header.DW0.Count) {
    return EFI_INVALID_PARAMETER;
  }

  Status        = EFI_SUCCESS;
  ZeroMem (&DiscoveryStruct, sizeof (DISCOVERY_STRUCT_VER_1));

  *RecordAddr = 0;
  *RecordSize = 0;
  MmioAddr = CpuAgent->DiscoveryTblAddr + sizeof (DISCOVERY_STRUCT_VER_1) * Index;
  Status = AccessMmioDword (&DiscoveryStruct, MmioAddr, sizeof (DISCOVERY_STRUCT_VER_1) / sizeof(UINT32), READ);
  if (!EFI_ERROR (Status)) {
    *RecordAddr = CpuAgent->Device10MmioBase + DiscoveryStruct.Header.DW2;
    *RecordSize = DiscoveryStruct.Header.DW3 * sizeof (UINT32);
  }

  return Status;
}

/**
  Get address, size of crash record and update private data structure

  @retval EFI_SUCCESS  The function completes successfully
**/
EFI_STATUS
UpdateCrashRecordInfo (
  VOID
  )
{
  UINT64                RecordAddr;
  UINT32                RecordSize;
  UINT32                Data32;
  CRASH_RECORD_VERSION  *Version;
  UINT32                Index;

  RecordAddr = 0;
  RecordSize = 0;

  //
  // Check if record cound is 0
  //
  if (CpuAgent->DiscoveryTbl.Header.DW0.Count == 0) {
    DEBUG ((DEBUG_ERROR, " error: record cound is 0.\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Check if the PUNIT crash record is valid. if no, skip all crash record extraction.
  //
  GetCrashRecordInfoOfIndex (0, &RecordAddr, &RecordSize);
  if ((RecordSize == 0) || (RecordAddr == 0)) {
    DEBUG ((DEBUG_ERROR, " error: PUNIT crash record is not found.\n"));
    return EFI_NOT_FOUND;
  } else {
    // The crash record is invalid if first DW is 0 or consumed bit is set
    Data32 = MmioRead32 ((UINTN)RecordAddr);
    Version = (CRASH_RECORD_VERSION *)&Data32;
    if ((Data32 == CRASHLOG_INVALID_CHECK_0) || (Data32 == CRASHLOG_INVALID_CHECK_DEAD_BEEF) || (Version->Consumed == 1)) {
      DEBUG ((DEBUG_ERROR, " error: PUNIT crash record is not valid, value is 0x%x.\n", Data32));
      return EFI_NOT_FOUND;
    }
  }

  //
  // Allocate memory for crash record information
  //
  CpuAgent->CrashRecordInfo = AllocateZeroPool (CpuAgent->DiscoveryTbl.Header.DW0.Count * sizeof (CRASH_RECORD_INFORMATION));
  if (CpuAgent->CrashRecordInfo == NULL) {
    DEBUG ((DEBUG_ERROR, " error: Fail to allocate memory for store crash record information.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Locate all records info on SSRAM and update CrashRecordInfo table
  //
  for (Index = 0; Index < CpuAgent->DiscoveryTbl.Header.DW0.Count; Index++) {
    RecordAddr = RecordSize = 0;
    GetCrashRecordInfoOfIndex (Index, &RecordAddr, &RecordSize);
    CpuAgent->CrashRecordInfo[Index].Addr = RecordAddr;
    CpuAgent->CrashRecordInfo[Index].Size = RecordSize;
    if ((RecordSize == 0) || (RecordAddr == 0)) {
      CpuAgent->CrashRecordInfo[Index].Valid = FALSE;
    } else {
      //
      // Check if crash record valid
      //
      Data32 = MmioRead32 ((UINTN)RecordAddr);
      if ((Data32 == CRASHLOG_INVALID_CHECK_0) || (Data32 == CRASHLOG_INVALID_CHECK_DEAD_BEEF)) {
        CpuAgent->CrashRecordInfo[Index].Valid = FALSE;
      } else  {
        CpuAgent->CrashRecordInfo[Index].Valid = TRUE;
        CpuAgent->ValidRecordCount ++;
      }
    }
  }

  //
  // Print crash record information
  //
  PrintCrashRecordInfo ();

  return EFI_SUCCESS;
}

