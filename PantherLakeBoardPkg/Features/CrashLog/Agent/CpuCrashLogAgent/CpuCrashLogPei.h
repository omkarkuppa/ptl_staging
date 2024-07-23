/** @file
  CPU CrashLog PEI driver

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

#ifndef __CPU_CRASHLOG_H__
#define __CPU_CRASHLOG_H__


#include <PiPei.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/CpuPlatformLib.h>
#include <IndustryStandard/Pci.h>
#include <Library/PciSegmentLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Ppi/MasterBootMode.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Register/TelemetryRegs.h>
#include <Defines/HostBridgeDefines.h>

#include <CrashLogVariable.h>
#include <Guid/CrashLogHob.h>
#include <ConvergedStruct.h>


//
// Structure forward declarations
//
typedef struct _CPU_CRASHLOG_AGENT_PRIVATE_DATA  CPU_CRASHLOG_AGENT_PRIVATE_DATA;

//
// Cdie crashlog reigsters
//
#define R_CDIE_MAILBOX_CMD_OFFSET   0x28
#define R_CDIE_MAILBOX_DATA_OFFSET  0x30
#define R_CDIE_CAPABILITY_OFFSET    0x18

#define CRASHLOG_NODE_ALL  0xFFFFFFFF

#define READ   FALSE
#define WRITE  TRUE

#pragma pack(1)
//
// Control configuration
//
typedef struct {
  UINT32    Enable     : 1;        ///< CrashLog CPU control
  UINT32    Clr        : 1;        ///< Clear control
  UINT32    ReArm      : 1;        ///< Re-arm control
  UINT32    StorageOff : 1;        ///< Storage Off control
  UINT32    RsvdBits   : 29;
} CPU_CONTROL_CONFIG;

//
// Structure of DVSEC registers
//
typedef union {
  struct {
    UINT32    PcieCapId            : 16;       ///< PCI Express Capability ID
    UINT32    CapVer               : 4;        ///< Capability Version
    UINT32    NextDvsecOffset      : 12;       ///< Next Capability Offset
    UINT32    VenId                : 16;
    UINT32    Ver                  : 4;
    UINT32    Len                  : 12;
    UINT32    DevscId              : 16;       ///< DVSEC_ID
    UINT32    NumEntries           : 8;        ///< Number of entries, describes the number of telemetry aggregators that would exist in this capability lookup table.
    UINT32    EntrySize            : 8;        ///< Entry Size in DWORDS
    UINT32    TBir                 : 3;        ///< tBIR, The BAR to be used
    UINT32    DiscoveryTableOffset : 29;       ///< Discovery Table Offset
  } Fields;
  UINT32    Data32[4];
} DVSEC_STRUCT;

//
// Structure of Cpu CrashLog Mailbox Interface
//
typedef union {
  struct {
    UINT32    Command  :8;
    UINT32    Param    :8;
    UINT32    Reserved :15;
    UINT32    Busy     :1;
  } Fields;
  UINT32    Data32;
} CPU_CRASHLOG_MAILBOX;
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
// Unique signature for private data structure.
//
struct _CPU_CRASHLOG_AGENT_PRIVATE_DATA {
  CPU_CONTROL_CONFIG           ControlConfig;

  //
  // PCI config space
  //
  UINT32                       Device10MmioBase;
  UINT16                       SavedPciCmdSts;
  BOOLEAN                      MmioInitByThisInstance;
  DVSEC_STRUCT                 DvsecStruct;

  //
  // Crashlog discovery table
  //
  DISCOVERY_STRUCT_VER_1       DiscoveryTbl;
  UINT64                       DiscoveryTblAddr;

  //
  //  Capability and status
  //
  CAPABILITY_STATUS_PHASE_1    CapSts;

  //
  // Crashlog record
  //
  UINT32                       ValidRecordCount;
  CRASH_RECORD_INFORMATION     *CrashRecordInfo;
  BOOLEAN                      RecordCollected;
};

extern CPU_CRASHLOG_AGENT_PRIVATE_DATA  *CpuAgent;


//
// Function prototype
//
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
  IN     UINT32   TBir,
  OUT    UINT16   *SavedStsCmd,
  OUT    BOOLEAN  *MmioProgramed
  );

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
  );

/**
  Read data from MMIO to memory or write data to MMIO

  @param[in,out] MemBuffer  Destination: Memory address.
  @param[in] MmioAddr       Source: MMIO address.
  @param[in] Count          The number of memory or I/O operations to perform.
  @param[in] Write          Indicate read or write operation.

  @retval EFI_SUCCESS             MMIO access sucessfully.

**/
EFI_STATUS
AccessMmioDword (
  IN OUT VOID     *MemBuffer,
  IN     UINT64   MmioAddr,
  IN     UINT32   Count,
  IN     BOOLEAN  Write
  );

/**
  Retrieve crashlog dvsec

  @param[out] CrashLogDevscCap     Pointer to structure of CrashLog DEVSC Data

  @retval EFI_UNSUPPORTED          Cpu CrashLog ID not found
  @retval EFI_SUCCESS              Cpu CrashLog ID found
**/
EFI_STATUS
GetCrashLogDvsec (
  OUT DVSEC_STRUCT  *CrashLogDvsec
  );

/**
  Check if Crash record ready for collection

  @retval     BOOLEAN            True if crash record ready for collection
**/
BOOLEAN
IsCrashRecordReady (
  VOID
  );

/**
  Print discovery table

  @retval     EFI_SUCCESS        The function completes successfully
**/
EFI_STATUS
PrintDiscoveryTbl (
   VOID
  );

/**
  Print crashlog information

  @retval     EFI_STATUS             PMC crash record size
**/
EFI_STATUS
PrintCrashRecordInfo (
  VOID
  );

/**
  Get address, size of crash record on SSRAM

  @param[in]  Index              Specifies the crash record which the function wants to get
  @param[out] RecordAddr         Address of crashlog
  @param[out] RecordSize         Size of crashlog

  @retval     EFI_SUCCESS        The function completes successfully
**/
EFI_STATUS
GetCrashRecordInfoOfIndex (
  IN  UINT32                           Index,
  OUT UINT64                           *RecordAddr,
  OUT UINT32                           *RecordSize
  );

/**
  Get address, size of crash record on SSRAM

  @retval     EFI_SUCCESS        The function completes successfully
**/
EFI_STATUS
UpdateCrashRecordInfo (
  VOID
  );

/**
  Send CPU CrashLog Mailbox command

  @param[in]  Command             Command request for CrashLog management.
  @param[in]  Param               Parameter associated with the requested command.

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Mailbox command failed with an error
  @retval EFI_TIMEOUT             Mailbox command did not complete
**/
EFI_STATUS
CpuCrashLogMailboxCommand (
  IN  UINT8                                  Command,
  IN  UINT8                                  Param
  );
#endif
