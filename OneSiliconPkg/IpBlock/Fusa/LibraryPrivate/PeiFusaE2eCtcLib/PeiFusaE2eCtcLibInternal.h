/** @file
  Header file for Fusa Private Lib Internal functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _FUSA_E2E_CTC_LIB_INTERNAL_H_
#define _FUSA_E2E_CTC_LIB_INTERNAL_H_

#include "PeiFusaResultReportingLib.h"
#include <Protocol/MpService.h>
#include <Register/Ptl/Msr/MsrRegs.h>

typedef struct {
  EFI_PROCESSOR_INFORMATION           ProcessorInfo;
  MSR_FUSA_CAPABILITIES_REGISTER      Fusa_Capabilities;
  UINT32                              CoreType;
} PROCESSOR_INFO;

typedef struct {
  PROCESSOR_INFO                  Processor[64];
  UINTN                           BspCpu;
  UINTN                          TotalProcessor;
  UINTN                          TotalEnProcessor;
  UINTN                          TotalCoreProcessor;
  UINTN                          TotalAtomProcessor;
} FUSA_INFO;

///
/// Fusa Configuration Registers
///
#define MSR_TRIGGER_STARTUP_SCAN_BIST       0x000002D2
#define MSR_STARTUP_SCAN_BIST_STATUS        0x000002D3

#define MSR_INTEGRITY_CAPABILITIES             0x000002D9
#define B_MSR_FUSA_CAP_STARTUP_SCAN_DIAGNOGSIS_MASK     BIT0

#define FUSA_STARTUP_EXT_BASE   0xF9000000
#define FUSA_PERIODIC_EXT_BASE  0xF9800000
#define FUSA_PATTERN_FILE_OFF   0x7
#define FUSA_PATTERN_DATA_OFF   12

#define FUSA_PATTERN_MAX_FILE        8
#define FUSA_PATTERN_SIGNATURE_START "_FSTART_"
#define FUSA_PATTERN_SIGNATURE_END   "__FEND__"

#define NUMBER_OF_UNKNOWN_CHARACTER_AFTER_GUID 12 //CE AA 02 00 4E 44 76 F8
#define NUMBER_OF_UNKNOWN_CHARACTER_BEFORE_START_SIG  0 //0C 00 00 19 
#define NUMBER_OF_UNKNOWN_CHARACTER_AFTER_START_SIG   0  //F4 23 0D 19

#define  MAX_FUSA_PATTERN_FILE 8



/// Structure of the parameter passed into the FspDxCheckGrScanBistSetInternal
typedef struct {
    UINT64    buffAddr;
    UINT8     result;
} TEST_CONFIG_SCANBIST;

typedef struct _FUSA_PATTERN_FILE_HEADER {
    UINT32    HeaderVersion;
    UINT32    PatchID;
    UINT32    Date;
    UINT32    CpuId;
    UINT32    Checksum;
    UINT32    LoaderVersion;
    UINT32    PlatformID;
    UINT32    DataSize;
    UINT32    TotalSize;
    UINT32    FuSaInfoReserved;
    UINT32    Reserved1;
    UINT32    Reserved2;
    UINT8     TargetAddr[1];
}FUSA_PATTERN_FILE_HEADER;

/**
  Perform Core IDI Parity Error E2E CTC.
  The test targets all active unique core. For core not
  detected, their test result is updated to
  FUSA_TEST_DEVICE_NOTAVAILABLE

  @note The test expects to be run within  Memory Type =
        Writeback (MTRR Encoding = 06h), which is the usual
        case of the memory type covering the code/data region
        of the bootloader.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from FusaTestNumCpu0Idi
                         to FusaTestNumCpu7Idi

**/
VOID
FspDxCheckCoreIdi (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform LLC ECC Error E2E CTC.
  The test targets all active unique CBO (which in turn maps to
  unique core). For CBO/core not detected, their test result is
  updated to FUSA_TEST_DEVICE_NOTAVAILABLE

  @note The test expects to be run within Memory Type =
        Writeback (MTRR Encoding = 06h), which is the usual
        case of the memory type covering the code/data region
        of the bootloader.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from
                         FuSaTestNumCboSlice0EgressLlcEccCtC to
                         FuSaTestNumCboSlice7EgressLlcEccCtC

**/
VOID
FspDxCheckLlc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform OPI Link Data Parity Error E2E CTC.
  The test targets OPI Link Data path.

  @note The test assumes MchBar and DmiBar have been set up
        with 32 bit addressing and being enabled.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test
                         FusaTestNumOpiLinkIosfData

**/
VOID
FspDxCheckOpiLink (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform LLC ECC Engress
  The test targets all active unique CBO (which in turn maps to
  unique core). For CBO/core not detected, their test result is
  updated to FUSA_TEST_DEVICE_NOTAVAILABLE

  @note The test expects to be run within Memory Type =
        Writeback (MTRR Encoding = 06h), which is the usual
        case of the memory type covering the code/data region
        of the bootloader.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from
                         FusaTestNumCboSlice0Ingress to
                         FusaTestNumCboSlice7Ingress

**/
VOID
FspDxCheckLlcEccEngress (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform IO Port Parity Error E2E CTC.
  The test targets IO Port multiple interfaces.

  @note The test assumes MchBar, GmBar and HpetBase have been
        set up with 32 bit addressing and being enabled.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test FusaTestNumIop

**/

/**
  Perform LLC Double Parity
  The test targets all active unique CBO (which in turn maps to
  unique core). For CBO/core not detected, their test result is
  updated to FUSA_TEST_DEVICE_NOTAVAILABLE

  @note The test expects to be run within Memory Type =
        Writeback (MTRR Encoding = 06h), which is the usual
        case of the memory type covering the code/data region
        of the bootloader.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from
                         FusaTestNumCboSlice0Ingress to
                         FusaTestNumCboSlice7Ingress

**/
VOID
FspDxCheckLlcDoubleParity (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform IO Port Parity Error E2E CTC.
  The test targets IO Port multiple interfaces.

  @note The test assumes MchBar, GmBar and HpetBase have been
        set up with 32 bit addressing and being enabled.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test FusaTestNumIop

**/

/**
  Perform WRC
  The test targets all active unique CBO (which in turn maps to
  unique core). For CBO/core not detected, their test result is
  updated to FUSA_TEST_DEVICE_NOTAVAILABLE

  @note The test expects to be run within Memory Type =
        Writeback (MTRR Encoding = 06h), which is the usual
        case of the memory type covering the code/data region
        of the bootloader.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from
                         FusaTestNumWRC

**/
VOID
FspDxCheckWrc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform NCU IDP
  The test targets all active unique CBO (which in turn maps to
  unique core). For CBO/core not detected, their test result is
  updated to FUSA_TEST_DEVICE_NOTAVAILABLE

  @note The test expects to be run within Memory Type =
        Writeback (MTRR Encoding = 06h), which is the usual
        case of the memory type covering the code/data region
        of the bootloader.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from
                         FusaTestNumWRC

**/
VOID
FspDxCheckNcuIdp (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform IO Port Parity Error E2E CTC.
  The test targets IO Port multiple interfaces.

  @note The test assumes MchBar, GmBar and HpetBase have been
        set up with 32 bit addressing and being enabled.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test FusaTestNumNcuIdp

**/

VOID
FspDxCheckIoPort (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform Display Isoch Port (DIP) Parity Error E2E CTC. The
  test targets DIP multiple interfaces.

  @note The test assumes MchBar and GttMmBar have been set up
        with 32 bit addressing and being enabled.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test FusaTestNumDip

**/
VOID
FspDxCheckDip (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform MBIST Test on CPU array

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from
                         FusaTestNumCpu0Mbist to
                         FusaTestNumCpu7Mbist
  @param[in] PeiServices    Pointer to PEI Services Table
**/
VOID
FspDxCheckCoreArrayMbist (
  OUT FUSA_TEST_RESULT *pFusaTestResult,
  IN  CONST EFI_PEI_SERVICES  **PeiServices
  );

/**
  Extract Memory SubSystem MBIST result which resided within
  registers, where the MBIST has been performed within the MRC
  stage and yet for some reason not able to populate the result
  HOB at the point of time.

  @note The test assumes MchBar has been set up with 32 bit
        addressing and being enabled.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from
                         FusaTestNumMc0Mbist to
                         FusaTestNumIbecc1Mbist
**/
VOID
FspDxCheckMemoryMbist (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform Memory Subsystem Parity Error E2E CTC.
  For IP not detected, their test result is updated to
  FUSA_TEST_DEVICE_NOTAVAILABLE

  @note The test expects to be run within  Memory Type =
        Writeback (MTRR Encoding = 06h), which is the usual
        case of the memory type covering the code/data region
        of the bootloader.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from FusaTestNumCpu0Idi
                         to FusaTestNumCpu7Idi

**/
VOID
FspDxCheckMemSubsystem (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform Atom ArrayBist.
  The test targets all active unique core. For core not
  detected, their test result is updated to
  FUSA_TEST_DEVICE_NOTAVAILABLE

  @note The test expects to be run within  Memory Type =
        Writeback (MTRR Encoding = 06h), which is the usual
        case of the memory type covering the code/data region
        of the bootloader.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from FusaTestNumCpu0Idi
                         to FusaTestNumCpu7Idi

**/
VOID
FspDxCheckArrayAndCRCBist (
  OUT FUSA_TEST_RESULT *pFusaTestResult,
  OUT FUSA_TEST_RESULT *pFusaCRCTestResult
  );

/**
  Perform Atom ScanBist.
  The test targets all active unique core. For core not
  detected, their test result is updated to
  FUSA_TEST_DEVICE_NOTAVAILABLE

  @note The test expects to be run within  Memory Type =
        Writeback (MTRR Encoding = 06h), which is the usual
        case of the memory type covering the code/data region
        of the bootloader.
  @param[in] FusaStartupFileAddr - Addr point to Startup Pattern file
  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from FusaTestNumCpu0Idi
                         to FusaTestNumCpu7Idi

**/
VOID
FspDxCheckScanBist (
  IN  UINT32 FusaStartupFileAddr,
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

#define MAX_PTL_ATOM_CORE_COUNT 12

/**
  To retrieve  mUniqueCoreList

  @retval mUniqueCoreList
**/
CONST UINT64 *
UniqueCoreListGet (
  VOID
  );

  /**
  To retrieve  mUniqueCoreList

  @retval mUniqueCoreList
**/
FUSA_INFO *
FusaInfoListGet (
  VOID
  );

/**
  To retrieve  TotalCoreProcessor

  @retval TotalCoreProcessor
**/
CONST UINTN
GetNumberofCoreCPU (
  VOID
  );

/**
  To retrieve  TotalAtomProcessor

  @retval TotalAtomProcessor
**/
CONST UINTN
GetNumberofAtomCPU (
  VOID
  );

/**
  Switch to Core CPU as BSP

  @param[in] UINTN Core - switching to target Core

  @return error status if SwitchCoreBSP was not success
**/
EFI_STATUS
Switch2BSP (
    IN FUSA_INFO *Fusa_info_ptr,
    IN UINTN Core
  );

/**
  Switch to Core CPU as BSP

  @param[in] FUSA_INFO

  @return error status if SwitchCoreBSP was not success
**/
EFI_STATUS
SwitchCoreBSP (
    IN FUSA_INFO *Fusa_info_ptr
  );

/**
  Find the core Number

  @return error status if SwitchCoreBSP was not success
**/
EFI_STATUS
CpuIDtoCoreNumber (
    IN UINT32 ProcessorId,
    OUT UINT8 *CoreIndex
  );

/**
  The function prototype for invoking a function on an Application Processor.

  @param[in,out] Buffer  The pointer to private data buffer.
**/
typedef
VOID
(*AP_PROCEDURE)(
  IN OUT VOID  *Buffer
  );

/**
  This function to update variable
**/
VOID
UpdateBSPInfo (
  VOID
  );

/**
  To execute routine "Routine" with paramters "Param" at AP
  indicated by ProcessorNumber, in blocking mode.

  @param[in] Routine - routine to be executed
  @param[in] ProcessorNumber - target AP processor number
                         enumerated by the MP services
  @param[in] Param - parameters to be used by the said routine

**/
VOID
RunAtAp (
  IN AP_PROCEDURE Routine,
  IN UINT32       ProcessorNumber,
  IN VOID         *Param
  );

/**
  To execute routine "Routine" with parameters "Param" at
  All AP, in blocking mode.

  @param[in] Routine - routine to be executed
  @param[in] ProcessorNumber - target AP processor number
                         enumerated by the MP services
  @param[in] Param - parameters to be used by the said routine

**/
VOID
RunAllAp (
  IN AP_PROCEDURE Routine,
  IN VOID         *Param
  );


FUSA_PATTERN_FILE_HEADER**
GetTotalNumberOfPatternFiles(
    IN   UINT32     FusaStartupFileAddr,
    OUT  UINT8      *NumberOfPatternFiles
);

#endif // _FUSA_E2E_CTC_LIB_INTERNAL_H_
