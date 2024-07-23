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

#ifndef __SOC_CRASHLOG_H__
#define __SOC_CRASHLOG_H__


#include <PiPei.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <IndustryStandard/Pci.h>
#include <Library/PciSegmentLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PmcLib.h>

#include <Ppi/MasterBootMode.h>
#include <Ppi/ReadOnlyVariable2.h>

#include <CrashLogVariable.h>
#include <Guid/CrashLogHob.h>
#include <ConvergedStruct.h>


#define MAX_CRASH_RECORD_COUNT  10

#define READ   FALSE
#define WRITE  TRUE

#define CRASHLOG_NODE_ALL  0xFFFFFFFF

//
// Structure forward declarations
//
typedef struct _SOC_CRASHLOG_AGENT_PRIVATE_DATA  SOC_CRASHLOG_AGENT_PRIVATE_DATA;

#pragma pack(1)
//
// Control configuration
//
typedef struct {
  UINT32    Enable   : 1; ///< CrashLog PMC control
  UINT32    Clr      : 1; ///< Clear control
  UINT32    AllReset : 1; ///< Trigger on all reset control
  UINT32    ReArm    : 1; ///< Re-arm control
  UINT32    GblRst   : 1; ///< Global reset override control
  UINT32    RsvdBits : 27;
} SOC_CONTROL_CONFIG;

//-------------------------------------------------------- //
// CRASHLOG DESCRIPTOR TABLE ENTRY                         //
//-------------------------------------------------------- //
typedef union {
  struct {
    UINT32    Offset    : 16;
    UINT32    Size      : 13;
    UINT32    AssignTag : 3;
  } RecordEntry;
  struct {
    UINT32    Fun       : 3;
    UINT32    Dev       : 5;
    UINT32    Bus       : 8;
    UINT32    Rsvd      : 10;
    UINT32    AgentTag  : 3;
    UINT32    AssignTag : 3;
  } AssignmentEntry;
  UINT32    Uint32;
} CRASHLOG_DESCRIPTOR_TABLE_ENTRY;

//
// CRASHLOG DESCRIPTOR TABLE
//
typedef struct _CRASHLOG_DESCRIPTOR_TABLE {
  UINT32    NumOfRegions;
  //CRASHLOG_DESCRIPTOR_TABLE_ENTRY    *TableEntry;
} CRASHLOG_DESCRIPTOR_TABLE;

//-----------------------------------------------------------
// PMC discovery structure
//-----------------------------------------------------------
typedef union {
  struct {
    UINT32    CrashlogSupproted : 1;
    UINT32    Dis               : 1;
    UINT32    Rsvd              : 2;
    UINT32    Size              : 12;
    UINT32    Offset            : 16;
  } Fields;
  UINT32    Data32;
} PMC_DISCOVERY_STRUCT_0;

typedef union {
  struct {
    UINT32     DW0;                 // Capability and status
    struct {
      UINT32    Rsvd          : 16;
      UINT32    DescTblOffset : 16; // descriptor table offset
    }                           DW1;
  }                             Header;
  UINT32    Data32[2];
} PMC_DISCOVERY_STRUCT_1;

#pragma pack()

//
// Structure to be used store crash record information
//
typedef struct {
  UINT64     Addr;
  UINT32     Size;
  BOOLEAN    Valid;
} CRASH_RECORD_INFORMATION;

//
// Tag field definition
//
#define CRASHLOG_DESCRIPTOR_TABLE_TAG_SOC_PMC     0x0        // SOC PMC SSRAM
#define CRASHLOG_DESCRIPTOR_TABLE_TAG_IOE_PMC     0x1        // IOE PMC SSRAM
#define CRASHLOG_DESCRIPTOR_TABLE_TAG_ASSIGN_BDF  0x7        // Assigned BDF

#define IOE_SSRAM_MMIO_OFFSET  0x100000

//
// Unique signature for private data structure.
//
struct _SOC_CRASHLOG_AGENT_PRIVATE_DATA {
  SOC_CONTROL_CONFIG           ControlConfig;
  UINT32                       GblRstMask0;
  UINT32                       GblRstMask1;

  //
  // PCI config space: PMC SSRAM
  //
  UINT32                       PmcSsramMmioBase;
  UINT16                       PmcSavedPciCmdSts;
  BOOLEAN                      PmcMmioInitByThisInstance;

  //
  // PCI config space: IOE SSRAM
  //
  UINT64                       IoeSsramCfgBase;
  UINT32                       IoeSsramMmioBase;
  UINT16                       IoeSavedPciCmdSts;
  BOOLEAN                      IoeMmioInitByThisInstance;

  //
  // Crashlog discovery table and descriptor table
  //
  PMC_DISCOVERY_STRUCT_1       DiscoveryTbl;
  CAPABILITY_STATUS_PHASE_1    *CapStsPtr;
  UINT64                       DescTblAddr;
  CRASHLOG_DESCRIPTOR_TABLE    *DescTbl;

  //
  // Crashlog record
  //
  UINT32                       ValidRecordCount;
  CRASH_RECORD_INFORMATION     *CrashRecordInfo;
  BOOLEAN                      RecordCollected;
};

extern SOC_CRASHLOG_AGENT_PRIVATE_DATA  *SocAgent;

//
// Function prototype
//

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
  );

/**
  Restore BAR address and command register

  @param[in] SavedStsCmd  Original value of command registers

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
RestorePmcSsramMmio (
  IN UINT16  SavedStsCmd
  );

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
  );

/**
  Get PMC crashlog discovery table

  @retval EFI_SUCCESS  Get the PMC Crashlog discovery table
**/
EFI_STATUS
GetPmcDiscoveryTbl (
  VOID
  );

/**
  Print discovery table

  @retval EFI_SUCCESS  The function completes successfully
**/
EFI_STATUS
PrintDiscoveryTbl (
  VOID
  );

/**
  Get address, size of crash record on SSRAM

  @retval EFI_SUCCESS  The function completes successfully
**/
EFI_STATUS
InitCrashRecordInfo (
  VOID
  );

/**
  Process PMC crashlog global reset trigger mask

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
PmcCrashlogSetGblRstMask (
  VOID
  );

/**
  Program BAR address and enable command register memory space decoding

  @param[in,out] MmioBase  Base address of MMIO

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
EnableIoeSsramMmio (
  IN OUT UINT32  *MmioBase
  );

/**
  Restore BAR address and command register

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
RestoreIoeSsramMmio (
  VOID
  );

/**
  Get IOE SSRAM assignment entry

  @retval EFI_SUCCESS  The function completes successfully
**/
EFI_STATUS
GetIoeSsramCfgBase (
  VOID
  );

#endif
