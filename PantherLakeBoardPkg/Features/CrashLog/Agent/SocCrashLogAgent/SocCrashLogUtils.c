/** @file
  SOC PMC CrashLog PEI driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "SocCrashLogPei.h"

/**
  Program BAR address and enable command register memory space decoding

  @param[in,out] MmioBase    Base address of MMIO
  @param[out] SavedStsCmd    Original value of command registers
  @param[out] MmioProgramed  Flag to indicate MMIO programed by this function

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
EnablePmcSsramMmio (
  IN OUT UINT32   *MmioBase,
  OUT    UINT16   *SavedStsCmd,
  OUT    BOOLEAN  *MmioProgramed
  )
{
  UINT64  PciCfgBase;
  UINT16  OriStsCmd;

  *MmioProgramed = FALSE;

  PciCfgBase = PmcSsramPciCfgBase ();
  if (PciSegmentRead16 (PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_ERROR, "PMC SSRAM is disabled\n"));
    *MmioBase = 0;
    return EFI_DEVICE_ERROR;
  }

  OriStsCmd = PciSegmentRead16 (PciCfgBase + PCI_COMMAND_OFFSET);

  if ((OriStsCmd & EFI_PCI_COMMAND_MEMORY_SPACE) == 0) {
    //
    // Program BAR 0 if the BAR is not programmed
    //
    PciSegmentWrite32 (PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET, *MmioBase);

    //
    // Save and Enable command register memory space decoding
    //
    PciSegmentOr16 (PciCfgBase + PCI_COMMAND_OFFSET, (UINT16) (EFI_PCI_COMMAND_MEMORY_SPACE));
    *MmioProgramed = TRUE;
    *SavedStsCmd = OriStsCmd;

  } else {
    *MmioBase = PciSegmentRead32 (PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;
  }

  DEBUG ((DEBUG_INFO, "PMC SSRAM pci cfg base = 0x%X\n", PciCfgBase));
  DEBUG ((DEBUG_INFO, "PMC SSRAM MMIO base = 0x%X\n", *MmioBase));

  return EFI_SUCCESS;
}

/**
  Restore BAR address and command register

  @param[in] SavedStsCmd  Original value of command registers

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
RestorePmcSsramMmio (
  IN UINT16  SavedStsCmd
  )
{
  UINT64  PciCfgBase;

  PciCfgBase = PmcSsramPciCfgBase ();
  if (PciSegmentRead16 (PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_ERROR, "PMC SSRAM is disabled\n"));
    return EFI_DEVICE_ERROR;
  }

  PciSegmentAnd16 (PciCfgBase + PCI_COMMAND_OFFSET, SavedStsCmd);
  PciSegmentWrite32 (PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET, 0);

  return EFI_SUCCESS;
}

/**
  Read data from MMIO to memory or write data to MMIO

  @param[in,out] MemBuffer  Destination: Memory address.
  @param[in] MmioAddr       Source: MMIO address.
  @param[in] Count          The number of memory or I/O operations to perform.
  @param[in] Write          Indicate read or write operation.

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
  Get PMC crashlog discovery table by PMC command

  @retval EFI_SUCCESS  Get the PMC Crashlog discovery table
**/
EFI_STATUS
GetPmcDiscoveryTbl (
  VOID
  )
{
  EFI_STATUS             Status;
  PMC_IPC_DISCOVERY_BUF  PmcDiscoveryBuffer;

  PmcDiscoveryBuffer.Uint64 = 0;
  Status = PmcCrashLogDiscovery (&PmcDiscoveryBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, " error: unable to get crashlog discovery table.\n"));
    return Status;
  }

  CopyMem (
    &SocAgent->DiscoveryTbl,
    &PmcDiscoveryBuffer,
    sizeof (PMC_DISCOVERY_STRUCT_1)
    );

  //
  // Check if discovery mechanism is descriptor table
  //
  SocAgent->CapStsPtr = (CAPABILITY_STATUS_PHASE_1 *)&(SocAgent->DiscoveryTbl);
  if (SocAgent->CapStsPtr->Mech != CRASHLOG_MECH_DESCRIPTOR_TABLE) {
    DEBUG ((DEBUG_INFO, " error: discovery mechanism is not descriptor table.\n"));
    return EFI_UNSUPPORTED;
  }

  SocAgent->DescTblAddr = SocAgent->PmcSsramMmioBase + SocAgent->DiscoveryTbl.Header.DW1.DescTblOffset;
  return EFI_SUCCESS;
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
  DEBUG ((DEBUG_INFO, " ---- PMC discovery table ----  \n"));
  DEBUG ((DEBUG_INFO, " -------------------------------  \n"));

  for (Index = 0; Index < sizeof(PMC_DISCOVERY_STRUCT_1)/sizeof (UINT32); Index ++) {
    DEBUG ((DEBUG_INFO, " 0x%08x  \n", SocAgent->DiscoveryTbl.Data32[Index]));
  }

  DEBUG ((DEBUG_INFO, " -------------------------------  \n"));
  DEBUG ((DEBUG_INFO, " PMC descriptor table addr = 0x%08x \n", SocAgent->DescTblAddr));
  DEBUG ((DEBUG_INFO, " -------------------------------  \n"));
  return EFI_SUCCESS;
}

/**
  Print discovery table

  @retval EFI_SUCCESS  The function completes successfully
**/
EFI_STATUS
PrintDescriptorTbl (
  VOID
  )
{
  UINT32  Index;
  UINT32  *DataPtr;

  if (SocAgent->DescTbl == NULL) {
    return EFI_UNSUPPORTED;
  }

  DataPtr = (VOID *)(SocAgent->DescTbl);

  DEBUG ((DEBUG_INFO, " -------------------------------  \n"));
  DEBUG ((DEBUG_INFO, " ---- PMC descriptor table ----  \n"));
  DEBUG ((DEBUG_INFO, " -------------------------------  \n"));

  for (Index = 0; Index < SocAgent->DescTbl->NumOfRegions + 3; Index ++, DataPtr ++) {
    DEBUG ((DEBUG_INFO, " 0x%08x  \n", *DataPtr));
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
  UINT32  Index;

  if (SocAgent->CrashRecordInfo == NULL) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, " -----------------------------------------------  \n"));
  DEBUG ((DEBUG_INFO, " --------------- Crash record info -------------  \n"));
  DEBUG ((DEBUG_INFO, " -----------------------------------------------  \n"));
  for (Index = 0; Index < SocAgent->DescTbl->NumOfRegions; Index ++) {
    if (SocAgent->CrashRecordInfo[Index].Valid) {
      DEBUG ((DEBUG_INFO, " record %d:: Addr= 0x%08x\n", Index, SocAgent->CrashRecordInfo[Index].Addr));
    }
  }
  DEBUG ((DEBUG_INFO, " -----------------------------------------------  \n"));
  return EFI_SUCCESS;
}


/**
  Get address, size of certain crash record on SSRAM

  @param[in] RecordIndex  Specifies the crash record which the function wants to get
  @param[out] RecordAddr  Address of crashlog
  @param[out] RecordSize  Size of crashlog

  @retval EFI_SUCCESS  The function completes successfully
**/
EFI_STATUS
GetCrashRecordInfoOfIndex (
  IN  UINT32                           RecordIndex,
  OUT UINT64                           *RecordAddr,
  OUT UINT32                           *RecordSize
  )
{
  EFI_STATUS                       Status;
  CRASHLOG_DESCRIPTOR_TABLE_ENTRY  *TblEntry;
  UINT32                           Index;

  if((RecordAddr == NULL) || (RecordSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  // start from 0, it must not more than total record count
  if (RecordIndex >= SocAgent->DescTbl->NumOfRegions) {
    return EFI_INVALID_PARAMETER;
  }

  *RecordAddr = 0;
  *RecordSize = 0;
  Status = EFI_NOT_FOUND;

  TblEntry = (CRASHLOG_DESCRIPTOR_TABLE_ENTRY *) ((UINTN) SocAgent->DescTbl + sizeof (CRASHLOG_DESCRIPTOR_TABLE));
  Index = 0;
  while (TRUE) {
    if (TblEntry->RecordEntry.AssignTag != CRASHLOG_DESCRIPTOR_TABLE_TAG_ASSIGN_BDF) {
      Index ++;
    }
    if (Index == (RecordIndex + 1)) {
      if ((TblEntry->RecordEntry.AssignTag == CRASHLOG_DESCRIPTOR_TABLE_TAG_SOC_PMC) ||
          (TblEntry->RecordEntry.AssignTag == CRASHLOG_DESCRIPTOR_TABLE_TAG_IOE_PMC)) {
        Status = EFI_SUCCESS;
      }
      break;
    }
    TblEntry ++;
  }

  if (Status == EFI_SUCCESS) {
    if (TblEntry->RecordEntry.AssignTag == CRASHLOG_DESCRIPTOR_TABLE_TAG_SOC_PMC) {
      *RecordAddr = SocAgent->PmcSsramMmioBase;
    } else {
      *RecordAddr = SocAgent->IoeSsramMmioBase;
    }

    if (*RecordAddr != 0) {
      *RecordAddr += TblEntry->RecordEntry.Offset;
    }
    *RecordSize = TblEntry->RecordEntry.Size * sizeof (UINT32);
  }

  return Status;
}

/**
  Get address, size of crash record on SSRAM

  @retval EFI_SUCCESS  The function completes successfully
**/
EFI_STATUS
InitCrashRecordInfo (
  VOID
  )
{
  UINT64                RecordAddr;
  UINT32                RecordSize;
  UINT32                Data32;
  CRASH_RECORD_VERSION  *Version;
  UINT32                Index;
  UINT32                Count;
  UINT32                MemSize;

  Count = MmioRead32 ((UINTN) SocAgent->DescTblAddr);
  RecordAddr = 0;
  RecordSize = 0;

  if (Count == 0 || Count > MAX_CRASH_RECORD_COUNT) {
    DEBUG((DEBUG_ERROR, " error: record count [%x] is not valid.\n", Count));
    return EFI_NOT_FOUND;
  }
  //
  // Initialize descrptor table if it is not initialized
  //
  if (SocAgent->DescTbl == NULL) {
    MemSize = sizeof (CRASHLOG_DESCRIPTOR_TABLE) + sizeof (CRASHLOG_DESCRIPTOR_TABLE_ENTRY) * (Count + 2);
    SocAgent->DescTbl = AllocateZeroPool (MemSize);
    if (SocAgent->DescTbl == NULL) {
      DEBUG ((DEBUG_ERROR, " error: Fail to allocate memory for store crash record information.\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    AccessMmioDword (SocAgent->DescTbl, SocAgent->DescTblAddr, MemSize / sizeof(UINT32), READ);
    PrintDescriptorTbl ();
  }

  //
  // Locate IOE PMC assignment entry and Enable IOE SSRAM MMIO
  //
  GetIoeSsramCfgBase ();

  //
  // Check if the first PMC crash record is valid. if no, skip all crash record extraction.
  //
  GetCrashRecordInfoOfIndex (0, &RecordAddr, &RecordSize);
  if (RecordSize == 0 || RecordAddr == 0) {
    DEBUG ((DEBUG_ERROR, " error: SOC crash record is not found.\n"));
    return EFI_NOT_FOUND;
  } else {
    // The crash record is invalid if first DW is 0 or consumed bit is set
    Data32 = MmioRead32 ((UINTN) RecordAddr);
    Version = (CRASH_RECORD_VERSION *) &Data32;
    if ((Data32 == CRASHLOG_INVALID_CHECK_0) || (Data32 == CRASHLOG_INVALID_CHECK_DEAD_BEEF) || (Version->Consumed == 1)) {
      DEBUG ((DEBUG_ERROR, " error: SOC crash record is not valid, value is 0x%x.\n", Data32));
      return EFI_NOT_FOUND;
    }
  }

  //
  // Allocate memory for crash record information
  //
  SocAgent->CrashRecordInfo = AllocateZeroPool (Count * sizeof(CRASH_RECORD_INFORMATION));
  if (SocAgent->CrashRecordInfo == NULL) {
    DEBUG ((DEBUG_ERROR, " error: Fail to allocate memory for store crash record information.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Locate all records info on SSRAM and update CrashRecordInfo table
  //
  for (Index = 0; Index < Count; Index++) {
    RecordAddr = RecordSize = 0;
    GetCrashRecordInfoOfIndex (Index, &RecordAddr, &RecordSize);
    SocAgent->CrashRecordInfo[Index].Addr = RecordAddr;
    SocAgent->CrashRecordInfo[Index].Size = RecordSize;
    if (RecordSize == 0 || RecordAddr == 0) {
      SocAgent->CrashRecordInfo[Index].Valid = FALSE;
    } else {
      //
      // Check if crash record valid by checking first DWORD of data
      //
      Data32 = MmioRead32 ((UINTN) RecordAddr);
      if ((Data32 == CRASHLOG_INVALID_CHECK_0) || (Data32 == CRASHLOG_INVALID_CHECK_DEAD_BEEF)) {
        SocAgent->CrashRecordInfo[Index].Valid = FALSE;
      } else  {
        SocAgent->CrashRecordInfo[Index].Valid = TRUE;
        SocAgent->ValidRecordCount ++;
      }
    }
  }

  //
  // Print crash record information
  //
  PrintCrashRecordInfo ();
  return EFI_SUCCESS;
}

/**
  Process PMC crashlog global reset trigger mask

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
PmcCrashlogSetGblRstMask (
  VOID
  )
{
  EFI_STATUS  Status;
  PMC_GLOBAL_RESET_MASK   NewGblRstMask0, CurGblRstMask0;
  PMC_GLOBAL_RESET_MASK1  NewGblRstMask1, CurGblRstMask1;

  NewGblRstMask0.Value = SocAgent->GblRstMask0;
  NewGblRstMask1.Value = SocAgent->GblRstMask1;

  //
  // Check if current settings are equal to the desired settings
  //
  if (SocAgent->ControlConfig.GblRst == 0) {
    if (SocAgent->CapStsPtr->GlbRstTriMskSts != SocAgent->ControlConfig.GblRst) {
      DEBUG ((DEBUG_INFO, "%a - reset to default. \n", __FUNCTION__));
      PmcCrashLogRestoreGlobalResetTriggerMask ();
    }
    SocAgent->GblRstMask0 = 0xFFFFFFFF;
    SocAgent->GblRstMask1 = 0xFFFFFFFF;
  } else {
    //
    // Check if mask is requested to be modify, then send command with corresponding mask value
    //
    //
    // read current mask value
    //
    PmcCrashLogReadGlobalResetTriggerMask (&CurGblRstMask0, &CurGblRstMask1);
    if (CurGblRstMask0.Value == NewGblRstMask0.Value && CurGblRstMask1.Value == NewGblRstMask1.Value) {
    } else {        // Override glbal reset trigger with setup settings
      DEBUG ((DEBUG_INFO, "%a - update to new mask. \n", __FUNCTION__));
      PmcCrashLogProgramGlobalResetTriggerEnableMask (NewGblRstMask0, NewGblRstMask1);
    }
  }

  //
  // Get current mask value
  //
  Status = PmcCrashLogReadGlobalResetTriggerMask (&CurGblRstMask0, &CurGblRstMask1);
  if(!EFI_ERROR (Status)) {
    if (SocAgent->ControlConfig.GblRst == 1) {
      SocAgent->GblRstMask0 = CurGblRstMask0.Value;
      SocAgent->GblRstMask1 = CurGblRstMask1.Value;
    }
    DEBUG ((DEBUG_INFO, "%a - current gblrst mask 0 = 0x%08x, gblrst mask 1 = 0x%08x\n", __FUNCTION__, CurGblRstMask0.Value, CurGblRstMask1.Value));
  }

  return Status;
}

