/** @file
  Header file for Cpu Common Lib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#ifndef _CPU_COMMON_LIB_H_
#define _CPU_COMMON_LIB_H_
#include <Protocol/MpService.h>

///
/// Enums for Time Window Convert Type
///
typedef enum {
  SecondsTimeWindowConvert = 1,
  MilliSecondsTimeWindowConvert,
  TimeWindowConvertMaximum
} TIME_WINDOW_CONV;

/**
  Return CPU Identifier used to identify various CPU types

  @retval CPU_IDENTIFIER           CPU Identifier
**/
UINT32
EFIAPI
GetCpuIdentifier (
  VOID
  );

/**
  This function is to program Trace Hub ACPI base address to processor's MSR TRACE_HUB_STH_ACPIBAR_BASE.

  @param[in]  TraceHubAcpiBase - Base address of Trace Hub ACPI Base address
**/
VOID
EFIAPI
CpuWriteTraceHubAcpiBase (
  IN UINT64  TraceHubAcpiBase
  );

/**
  Private helper function to convert various Turbo Power Limit Time from milli Seconds to CPU units

  @param[in] TimeInMilliSeconds       Time in milli seconds

  @retval UINT8 Converted time in CPU units
**/
UINT8
EFIAPI
GetConvertedTime (
  IN UINT32          TimeInMilliSeconds
  );

/**
  Set the PRMRR MSRs.

  @param PrmrrBase PRMRR base address
  @param PrmrrSize PRMRR size

  @retval RETURN_SUCCESS       PRMRR MSRs are set successfully.
  @retval RETURN_UNSUPPORTED   PRMRR is not supported.
  @retval RETURN_ACCESS_DENIED PRMRR MSRs are already locked and cannot be changed.
*/
RETURN_STATUS
EFIAPI
SetPrmrrMsr (
  UINT64 PrmrrBase,
  UINT32 PrmrrSize
  );

/**
  Set the SEAMRR MSRs.
  @param SeamrrBase SEAMRR base address
  @param SeamrrSize SEAMRR size
  @retval RETURN_SUCCESS       SEAMRR MSRs are set successfully.
  @retval RETURN_UNSUPPORTED   SEAMRR is not supported.
  @retval RETURN_ACCESS_DENIED SEAMRR MSRs are already locked and cannot be changed.
*/
RETURN_STATUS
EFIAPI
SetSeamrrMsr (
  UINT64 SeamrrBase,
  UINT32 SeamrrSize
  );

/**
  Lock the SEAMRR MSRs.
  @retval RETURN_SUCCESS       SEAMRR MSRs are Lock successfully.
  @retval RETURN_UNSUPPORTED   SEAMRR is not supported.
  @retval RETURN_ACCESS_DENIED SEAMRR MSRs are already locked and cannot be changed.
*/
RETURN_STATUS
EFIAPI
LockSeamrrMsr (
  VOID
  );

/**
  Configures SE_SVN_MSR.

  @param SeSvnMsr                   Value of SeSvn Msr to program.

  @retval RETURN_SUCCESS            SeSvn MSR are set successfully.
  @retval RETURN_SECURITY_VIOLATION SeSvn MSR cannot be programmed.
*/
RETURN_STATUS
EFIAPI
SetBiosSeSvnMsr (
  UINT64 SeSvn
  );

/**
  Detect if Hetero Core is supported.

  @retval TRUE - Processor Support HeteroCore
  @retval FALSE - Processor doesnt support HeteroCore
**/
BOOLEAN
EFIAPI
IsHeteroCoreSupported (
  VOID
  );

/**
  Detect the type of core, whether it is Big/Small Core.

  @param[out]    CoreType      Output pointer that get CPUID_NATIVE_MODEL_ID_INFO data
                 10h - Quark
                 20h - Atom
                 30H - Knights
                 40H - Core
**/
VOID
EFIAPI
GetCoreType (
  OUT  UINT8   *CoreType
  );

/**
  This function returns Max Turbo Ratio.

  @return Max Turbo Ratio
**/
UINT8
EFIAPI
GetMaxTurboRatio (
  VOID
  );

/**
  Get 32-bit CPU Soft Strap data of multi sets.

  @param[out] CpuStrapSet1High  CPU strap Set1 High value
  @param[out] CpuStrapSet2Low   CPU strap Set2 Low value
  @param[out] CpuStrapSet2High  CPU strap Set2 High value
  @param[out] CpuStrapSet3Low   CPU strap Set3 Low value
  @param[out] CpuStrapSet3High  CPU strap Set3 High value

  @retval EFI_SUCCESS           Get CPU Soft Strap data successfully.
  @retval EFI_INVALID_PARAMETER All the parameters are NULL.
  @retval Others                Failed to get CPU Soft Strap data.
**/
EFI_STATUS
EFIAPI
GetCpuStrapData (
  OUT UINT32  *CpuStrapSet1High OPTIONAL,
  OUT UINT32  *CpuStrapSet2Low  OPTIONAL,
  OUT UINT32  *CpuStrapSet2High OPTIONAL,
  OUT UINT32  *CpuStrapSet3Low  OPTIONAL,
  OUT UINT32  *CpuStrapSet3High OPTIONAL
  );

/**
  Set 32-bit CPU Soft Strap data of multi sets.

  @param[in] CpuStrapSet1High  CPU strap Set1 High value
  @param[in] CpuStrapSet2Low   CPU strap Set2 Low value
  @param[in] CpuStrapSet2High  CPU strap Set2 High value
  @param[in] CpuStrapSet3Low   CPU strap Set3 Low value
  @param[in] CpuStrapSet3High  CPU strap Set3 High value

  @retval EFI_SUCCESS           Set CPU Soft Strap data successfully
  @retval EFI_INVALID_PARAMETER The parameters - CpuStrapSet1High, CpuStrapSet2Low, CpuStrapSet2High
                                and CpuStrapSet3Low are inconsistent(should be all NULL or non-NULL).
  @retval Others                Failed to set CPU Soft Strap data.
**/
EFI_STATUS
EFIAPI
SetCpuStrapData (
  IN  UINT32  *CpuStrapSet1High OPTIONAL,
  IN  UINT32  *CpuStrapSet2Low  OPTIONAL,
  IN  UINT32  *CpuStrapSet2High OPTIONAL,
  IN  UINT32  *CpuStrapSet3Low  OPTIONAL,
  IN  UINT32  *CpuStrapSet3High OPTIONAL
  );

/**
  Get 32-bit CPU Soft Strap data by each set.

  @param[in]  CpuStrapSetNum    CPU strap set number
  @param[out] CpuStrapSetData   CPU strap set value

  @retval EFI_SUCCESS           Get CPU Soft Strap data successfully or do not need to get data.
  @retval Others                Failed to get CPU Soft Strap data.
**/
EFI_STATUS
EFIAPI
GetCpuStrapDataBySet (
  IN  UINT8   CpuStrapSetNum,
  OUT UINT32  *CpuStrapSetData
  );

/**
  This function returns ratio

  @param[in]  ConfigTdpLevel  Config Tdp Level

  @retval intended ratio
**/
UINT8
MsrGetSelectCtdpRatio (
  IN UINT8 ConfigTdpLevel
  );

/**
  Get Processor Power Unit in Watts

  @retval Processpr power unit
**/
UINT8
EFIAPI
GetPowerUnit (
  VOID
  );

/**
  Get processor information and core type.
  This function retrieves processor information and core type for all processors
  in the system. This information is obtained from the HOB characterized by
  gMpInformation2HobGuid.
  @param[in out] ProcessorInfo            A pointer to an array of EFI_PROCESSOR_INFORMATION structures.
                                          One entry for each processor.
  @param[in out] CoreType                 A pointer to an array of CoreType.
                                          One entry for each processor.
  @param[in]    NumberOfProcessors       Number Of Processors
**/
VOID
EFIAPI
GetProcessorInfoAndCoreType (
  IN OUT  EFI_PROCESSOR_INFORMATION  *ProcessorInfo,
  IN OUT  UINT8                      *CoreType,
  IN      UINTN                      NumberOfProcessors
  );

#endif
