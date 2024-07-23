/** @file
  This header file contains processors power management definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _POWER_MANAGEMENT_H_
#define _POWER_MANAGEMENT_H_

#include <Uefi.h>
#include <IndustryStandard/Acpi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/MpService.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/CpuNvsArea.h>
#include <Library/CpuPlatformLib.h>
#include <PowerMgmtNvsStruct.h>
#include <Library/AslUpdateLib.h>
#include <Library/PciSegmentLib.h>
#include <CpuPowerManagementDataHob.h>
#include <CpuPowerManagementPrivateDataHob.h>
#include <Library/TimerLib.h>
#include <Library/CpuMailboxLib.h>
#include <Protocol/BiosGuardNvsArea.h>
#include <Library/MsrFruLib.h>


#define FADT_C3_LATENCY                 57
#define FADT_C3_LATENCY_DISABLED        1001

#define NATIVE_PSTATE_LATENCY           10
#define PSTATE_BM_LATENCY               10

#define EFI_FIELD_OFFSET(TYPE,Field) ((UINTN)(&(((TYPE *) 0)->Field)))

//
// Global variables
//
extern EFI_GUID gCpuAcpiTableStorageGuid;

extern CPU_NVS_AREA_PROTOCOL *gCpuNvsAreaProtocol; ///< CPU Nvs Protocol
extern BIOSGUARD_NVS_AREA_PROTOCOL *gBiosGuardNvsAreaProtocol;  ///< Bios Guard GlobalNvs Protocol

//
// Globals to support updating ACPI Tables
//
extern EFI_ACPI_TABLE_PROTOCOL                *mAcpiTable;
extern EFI_ACPI_DESCRIPTION_HEADER            *mCpu0IstTable;
extern EFI_ACPI_DESCRIPTION_HEADER            *mApIstTable;
extern EFI_ACPI_DESCRIPTION_HEADER            *mCpu0CstTable;
extern EFI_ACPI_DESCRIPTION_HEADER            *mApCstTable;
extern EFI_ACPI_DESCRIPTION_HEADER            *mCpuSsdtTable;
extern EFI_ACPI_DESCRIPTION_HEADER            *mCpu0TstTable;
extern EFI_ACPI_DESCRIPTION_HEADER            *mApTstTable;
extern EFI_ACPI_DESCRIPTION_HEADER            *mCpu0HwpTable;
extern EFI_ACPI_DESCRIPTION_HEADER            *mApHwpTable;
extern EFI_ACPI_DESCRIPTION_HEADER            *mHwpLvtTable;
extern EFI_ACPI_DESCRIPTION_HEADER            *mCpu0PsdTable;
extern EFI_ACPI_DESCRIPTION_HEADER            *mApPsdTable;

extern CPU_POWER_MANAGEMENT_DATA_HOB          *mCpuPowerMgmtDataHob;
///
/// ASL SSDT structure layout
///
#pragma pack(1)
typedef struct {
  UINT8  NameOp;          ///< First opcode is a NameOp.
  UINT32 NameString;      ///< 'TDSS'   ; Name of object.
  UINT8  PackageOp;       ///< db 12h   ; Sixth OpCode is PackageOp.
  UINT16 PackageLen;      ///< dw 0h    ; Seventh/Eighth OpCode is PackageLength.
  UINT8  PackageEntryNum; ///< db 0Ch   ; Ninth OpCode is number of package entries.
  UINT8  StringPrefix1;   ///< 0Dh
  UINT64 Cpu0IstStr;      ///< 00h
  UINT8  StringNull1;     ///< 00h
  UINT8  DwordPrefix1a;   ///< 0Ch
  UINT32 Cpu0IstAddr;     ///< 00h
  UINT8  DwordPrefix1b;   ///< 0Ch
  UINT32 Cpu0IstLen;      ///< 00h
  UINT8  StringPrefix2;   ///< 0Dh
  UINT64 Cpu1IstStr;      ///< 00h
  UINT8  StringNull2;     ///< 00h
  UINT8  DwordPrefix2a;   ///< 0Ch
  UINT32 ApIstAddr;       ///< 00h
  UINT8  DwordPrefix2b;   ///< 0Ch
  UINT32 ApIstLen;        ///< 00h
  UINT8  StringPrefix3;   ///< 0Dh
  UINT64 Cpu0CstStr;      ///< 00h
  UINT8  StringNull3;     ///< 00h
  UINT8  DwordPrefix3a;   ///< 0Ch
  UINT32 Cpu0CstAddr;     ///< 00h
  UINT8  DwordPrefix3b;   ///< 0Ch
  UINT32 Cpu0CstLen;      ///< 00h
  UINT8  StringPrefix4;   ///< 0Dh
  UINT64 ApCstStr;        ///< 00h
  UINT8  StringNull4;     ///< 00h
  UINT8  DwordPrefix4a;   ///< 0Ch
  UINT32 ApCstAddr;       ///< 00h
  UINT8  DwordPrefix4b;   ///< 0Ch
  UINT32 ApCstLen;        ///< 00h
  UINT8  StringPrefix5;   ///< 0Dh
  UINT64 Cpu0HwpStr;      ///< 00h
  UINT8  StringNull5;     ///< 00h
  UINT8  DwordPrefix5a;   ///< 0Ch
  UINT32 Cpu0HwpAddr;     ///< 00h
  UINT8  DwordPrefix5b;   ///< 0Ch
  UINT32 Cpu0HwpLen;      ///< 00h
  UINT8  StringPrefix6;   ///< 0Dh
  UINT64 ApHwpStr;        ///< 00h
  UINT8  StringNull6;     ///< 00h
  UINT8  DwordPrefix6a;   ///< 0Ch
  UINT32 ApHwpAddr;       ///< 00h
  UINT8  DwordPrefix6b;   ///< 0Ch
  UINT32 ApHwpLen;        ///< 00h
  UINT8  StringPrefix7;   ///< 0Dh
  UINT64 HwpLvtStr;       ///< 00h
  UINT8  StringNull7;     ///< 00h
  UINT8  DwordPrefix7a;   ///< 0Ch
  UINT32 HwpLvtAddr;      ///< 00h
  UINT8  DwordPrefix7b;   ///< 0Ch
  UINT32 HwpLvtLen;       ///< 00h
  UINT8  StringPrefix8;   ///< 0Dh
  UINT64 Cpu0PsdStr;      ///< 00h
  UINT8  StringNull8;     ///< 00h
  UINT8  DwordPrefix8a;   ///< 0Ch
  UINT32 Cpu0PsdAddr;     ///< 00h
  UINT8  DwordPrefix8b;   ///< 0Ch
  UINT32 Cpu0PsdLen;      ///< 00h
  UINT8  StringPrefix9;   ///< 0Dh
  UINT64 Cpu1PsdStr;      ///< 00h
  UINT8  StringNull9;     ///< 00h
  UINT8  DwordPrefix9a;   ///< 0Ch
  UINT32 ApPsdAddr;       ///< 00h
  UINT8  DwordPrefix9b;   ///< 0Ch
  UINT32 ApPsdLen;        ///< 00h
} SSDT_LAYOUT;
#pragma pack()

//
// ASL PSS package structure layout
//
#pragma pack (1)
typedef struct {
  UINT8     NameOp;           // 12h ;First opcode is a NameOp.
  UINT8     PackageLead;      // 20h ;First opcode is a NameOp.
  UINT8     NumEntries;       // 06h ;First opcode is a NameOp.
  UINT8     DwordPrefix1;     // 0Ch
  UINT32    CoreFrequency;    // 00h
  UINT8     DwordPrefix2;     // 0Ch
  UINT32    Power;            // 00h
  UINT8     DwordPrefix3;     // 0Ch
  UINT32    TransLatency;     // 00h
  UINT8     DwordPrefix4;     // 0Ch
  UINT32    BmLatency;        // 00h
  UINT8     DwordPrefix5;     // 0Ch
  UINT32    Control;          // 00h
  UINT8     DwordPrefix6;     // 0Ch
  UINT32    Status;           // 00h
} PSS_PACKAGE_LAYOUT;
#pragma pack()

//
// FVID Table Information
// Default FVID table
// One header field plus states
//
extern UINT16     mNumberOfStates;
extern FVID_TABLE *mFvidPointer;

//
// Function prototypes
//

/**
  Patch the native _PSS package with the EIST values
  Uses ratio/VID values from the FVID table to fix up the control values in the _PSS.

  (1) Find _PSS package:
    (1.1) Find the _PR_CPU0 scope.
    (1.2) Save a pointer to the package length.
    (1.3) Find the _PSS AML name object.
  (2) Resize the _PSS package.
  (3) Fix up the _PSS package entries
    (3.1) Check Turbo mode support.
    (3.2) Check Dynamic FSB support.
  (4) Fix up the Processor block and \_PR_CPU0 Scope length.
  (5) Update SSDT Header with new length.

  @retval EFI_SUCCESS
  @retval EFI_NOT_FOUND - If _PR_.CPU0 scope is not foud in the ACPI tables
**/
EFI_STATUS
AcpiPatchPss (
  VOID
  );

/**
  Locate the CPU ACPI tables data file and read ACPI SSDT tables.
  Publish the appropriate SSDT based on current configuration and capabilities.

  @param[in] This                Pointer to the protocol instance

  @retval EFI_SUCCESS - on success
  @retval Appropiate failure code on error
**/
EFI_STATUS
InitCpuAcpiTable (
  VOID
  );

/**
  Initializes the CPU power management global variable.
  This must be called prior to any of the functions being used.
**/
VOID
InitPowerManagementGlobalVariables (
  VOID
  );

/**
  Initialize the platform power management based on hardware capabilities
  and user configuration settings.

  This includes creating FVID table, updating ACPI tables,
  and updating processor and chipset hardware configuration.

  This should be called prior to any Px, Cx, Tx activity.

  @retval EFI_SUCCESS - on success
  @retval Appropiate failure code on error
**/
EFI_STATUS
InitPpmDxe (
  VOID
  );
#endif
