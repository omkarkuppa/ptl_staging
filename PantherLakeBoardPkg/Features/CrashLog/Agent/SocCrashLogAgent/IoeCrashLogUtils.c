/** @file
  Provide the interface for IOE PMC CrashLog

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

  @param[in,out] MmioBase  Base address of MMIO

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
EnableIoeSsramMmio (
  IN OUT UINT32  *MmioBase
  )
{
  UINT16  OriStsCmd;

  if (PciSegmentRead16 (SocAgent->IoeSsramCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_ERROR, "IOE SSRAM is disabled\n"));
    *MmioBase = 0;
    return EFI_DEVICE_ERROR;
  }

  OriStsCmd = PciSegmentRead16 (SocAgent->IoeSsramCfgBase + PCI_COMMAND_OFFSET);

  if ((OriStsCmd & EFI_PCI_COMMAND_MEMORY_SPACE) == 0) {
    //
    // Program BAR 0 if the BAR is not programmed
    //
    PciSegmentWrite32 (SocAgent->IoeSsramCfgBase + PCI_BASE_ADDRESSREG_OFFSET, *MmioBase);

    //
    // Save and Enable command register memory space decoding
    //
    PciSegmentOr16 (SocAgent->IoeSsramCfgBase + PCI_COMMAND_OFFSET, (UINT16)(EFI_PCI_COMMAND_MEMORY_SPACE));
    SocAgent->IoeMmioInitByThisInstance= TRUE;
    SocAgent->IoeSavedPciCmdSts = OriStsCmd;

  } else {
    *MmioBase = PciSegmentRead32 (SocAgent->IoeSsramCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;
  }

  DEBUG ((DEBUG_INFO, "IOE SSRAM pci cfg base = 0x%X\n", SocAgent->IoeSsramCfgBase));
  DEBUG ((DEBUG_INFO, "IOE SSRAM MMIO base = 0x%X\n", *MmioBase));

  return EFI_SUCCESS;
}

/**
  Restore BAR address and command register

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
RestoreIoeSsramMmio (
  VOID
  )
{
  if (PciSegmentRead16 (SocAgent->IoeSsramCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_ERROR, "IOE SSRAM is disabled\n"));
    return EFI_DEVICE_ERROR;
  }

  PciSegmentAnd16 (SocAgent->IoeSsramCfgBase + PCI_COMMAND_OFFSET, SocAgent->IoeSavedPciCmdSts);
  PciSegmentWrite32 (SocAgent->IoeSsramCfgBase + PCI_BASE_ADDRESSREG_OFFSET, 0);

  return EFI_SUCCESS;
}


/**
  Get IOE SSRAM assignment entry

  @retval EFI_SUCCESS  The function completes successfully
**/
EFI_STATUS
GetIoeSsramCfgBase (
  VOID
  )
{
  EFI_STATUS                       Status;
  CRASHLOG_DESCRIPTOR_TABLE_ENTRY  *TblEntry;
  UINT32                           Index;

  Status = EFI_NOT_FOUND;

  TblEntry = (CRASHLOG_DESCRIPTOR_TABLE_ENTRY *)((UINTN)SocAgent->DescTbl + sizeof (CRASHLOG_DESCRIPTOR_TABLE));
  for (Index = 0; Index < SocAgent->DescTbl->NumOfRegions; Index ++) {
    if (TblEntry[Index].AssignmentEntry.AssignTag == CRASHLOG_DESCRIPTOR_TABLE_TAG_ASSIGN_BDF &&
        TblEntry[Index].AssignmentEntry.AgentTag == CRASHLOG_DESCRIPTOR_TABLE_TAG_IOE_PMC) {
      Status = EFI_SUCCESS;
      break;
    }
  }

  if (Status == EFI_SUCCESS) {
    SocAgent->IoeSsramCfgBase = PCI_SEGMENT_LIB_ADDRESS (0, TblEntry[Index].AssignmentEntry.Bus, TblEntry[Index].AssignmentEntry.Dev, TblEntry[Index].AssignmentEntry.Fun, 0);
    //
    // Enable IOE SSRAM MMIO
    //
    SocAgent->IoeSsramMmioBase = SocAgent->PmcSsramMmioBase + IOE_SSRAM_MMIO_OFFSET;
    EnableIoeSsramMmio (&(SocAgent->IoeSsramMmioBase));
  }

  return Status;
}
