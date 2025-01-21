/** @file
  Library for Fsp Performance functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/Performance.h>
#include <Library/PerformanceLib.h>
#include <Library/TimerLib.h>
#include <Library/FspPerformanceLib.h>

//
// Global array of enum and string pairs
// Max String size = PEI_PERFORMANCE_STRING_SIZE
//
FSP_PERF_DATA  FspPerfData[FspMaxPerf] = {
  { FspuGopPerf,     "uGOPInit" },
  { FspuGopExitPerf, "uGOPExit" },
};

/**
  Create FSP Performance Hob

  @retval EFI_SUCCESS          FSP Performance HOB is created.
  @retval EFI_OUT_OF_RESOURCES Could not allocate memory.
**/
EFI_STATUS
EFIAPI
CreateFspPerformanceHob (
  VOID
  )
{
  VOID  *PerformanceHob;

  PerformanceHob = BuildGuidHob (
                                 &gFspPerfHobGuid,
                                 sizeof (PEI_PERFORMANCE_LOG_ENTRY) * FspMaxPerf
                                 );
  ASSERT (PerformanceHob != NULL);
  if (PerformanceHob == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem ((VOID *)PerformanceHob, sizeof (PEI_PERFORMANCE_LOG_ENTRY) * FspMaxPerf);
  return EFI_SUCCESS;
}

/**
  This function returns the current time.

  @retval           Returns the current timestamp
**/
UINT64
EFIAPI
GetFspCurrentTime (
  VOID
  )
{
  return AsmReadTsc ();
}

/**
  This function stores the timestamps into FspPerformanceHob and logs corresponding
  FPDT record.

  @param[in]  Identifier          Identifier for the global FSP_PERF_DATA
  @param[in]  StartTime           StartTime of the module
**/
VOID
EFIAPI
LogFspPerformanceData (
  IN FSP_PERFORMANCE  Identifier,
  IN UINT64           StartTime
  )
{
  EFI_HOB_GUID_TYPE          *GuidHob;
  PEI_PERFORMANCE_LOG_ENTRY  *PerfRecord;
  UINT64                     EndTime;

  if (Identifier >= FspMaxPerf) {
    DEBUG ((DEBUG_ERROR, "Identifier %d out of bounds\n", Identifier));
    return;
  }

  EndTime = GetFspCurrentTime ();

  GuidHob = GetFirstGuidHob (&gFspPerfHobGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Fsp Performance Hob not found\n"));
    return;
  }

  //
  // Navigate and fill the performance data as per the identifier
  //
  PerfRecord  = (PEI_PERFORMANCE_LOG_ENTRY *)(GET_GUID_HOB_DATA (GuidHob));
  PerfRecord += Identifier;
  CopyMem (PerfRecord->Module, FspPerfData[Identifier].String, PEI_PERFORMANCE_STRING_SIZE);
  PerfRecord->StartTimeStamp = DivU64x32 (GetTimeInNanoSecond (StartTime), 1000000);
  PerfRecord->EndTimeStamp   = DivU64x32 (GetTimeInNanoSecond (EndTime), 1000000);

  PERF_START_EX (&gFspPerfHobGuid, NULL, PerfRecord->Module, StartTime, BASE_IDENTIFIER + 2 * Identifier);
  PERF_END_EX (&gFspPerfHobGuid, NULL, PerfRecord->Module, EndTime, BASE_IDENTIFIER + 2 * Identifier + 1);
}

/**
  This function returns the time taken by FSP module.

  @retval           Returns the time taken by a FSP module
**/
UINT64
EFIAPI
GetFspModuleTime (
  IN FSP_PERFORMANCE  Identifier
  )
{
  EFI_HOB_GUID_TYPE          *GuidHob;
  PEI_PERFORMANCE_LOG_ENTRY  *PerfRecord;
  UINT64                     TimeElapsed;

  if (Identifier >= FspMaxPerf) {
    DEBUG ((DEBUG_ERROR, "Identifier %d out of bounds\n", Identifier));
    return 0;
  }

  GuidHob = GetFirstGuidHob (&gFspPerfHobGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Fsp Performance Hob not found\n"));
    return 0;
  }

  PerfRecord = (VOID *)(GET_GUID_HOB_DATA (GuidHob));
  //
  // Navigate and fill the performance data as per the identifier
  //
  PerfRecord += Identifier;

  TimeElapsed = PerfRecord->EndTimeStamp - PerfRecord->StartTimeStamp;
  return TimeElapsed;
}
