/** @file
  Header file for SMBIOS related functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
  System Management BIOS (SMBIOS) Reference Specification v3.0.0
  dated 2015-Feb-12 (DSP0134)
  http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_3.0.0.pdf
**/

#ifndef _SMBIOS_CPU_H_
#define _SMBIOS_CPU_H_

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/CpuCommonLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/MsrFruLib.h>
#include <Library/CpuCacheInfoLib.h>
#include <Library/SmbiosCpuLib.h>
#include <SmbiosCacheInfoHob.h>
#include <SmbiosProcessorInfoHob.h>

#define PROCESSOR_STATUS_DATA PROCESSOR_STATUS_DATA_DISABLE_UPSTREAM
#include <IndustryStandard/SmBios.h>
#undef PROCESSOR_STATUS_DATA

#include <Register/Cpuid.h>
#include <Register/Msr.h>

#include <Ppi/SiPolicy.h>

///
/// Cache configuration details as per spec
///

#define CACHE_OPMODE_WRITE_THOUGH        0x00
#define CACHE_OPMODE_WRITE_BACK          0x01
#define CACHE_ENABLE                     0x01
#define CACHE_DISABLE                    0x00
#define CACHE_LOCATION_INTERNAL          0x00
#define CACHE_LOCATION_EXTERNAL          0x01
#define CACHE_SOCKETED                   0x01
#define CACHE_NOTSOCKETED                0x00

///
/// Non-static SMBIOS table data to be filled later with a dynamically generated value
///
#define TO_BE_FILLED  0
#define TO_BE_FILLED_STRING  " "        ///< Initial value should not be NULL

///
/// String references in SMBIOS tables. This eliminates the need for pointers. See spec for details.
///
#define NO_STRING_AVAILABLE  0
#define STRING_1  1
#define STRING_2  2
#define STRING_3  3
#define STRING_4  4
#define STRING_5  5
#define STRING_6  6
#define STRING_7  7

#define BRAND_STRING_UNSUPPORTED  "CPU Brand String Not Supported"
#define INTEL_CORPORATION_STRING  "Intel(R) Corporation"

///
/// This constant defines the maximum length of the CPU brand string. According to the
/// IA manual, the brand string is in EAX through EDX (thus 16 bytes) after executing
/// the CPUID instructions with EAX as 80000002, 80000003, 80000004.
///
#define MAXIMUM_CPU_BRAND_STRING_LENGTH 48

///
/// This constant defines the maximum size of any SMBIOS string provided by this library, including terminating null character.
///
#define MAXIMUM_SIZE_OF_CPU_SMBIOS_STRING 200

#define MAX_NUMBER_OF_CACHE_LEVELS 4

///
/// SMBIOS Table values with special meaning
///
#define SMBIOS_TYPE4_64BIT_CAPABLE            BIT2
#define SMBIOS_TYPE4_MULTI_CORE               BIT3
#define SMBIOS_TYPE4_HARDWARE_THREAD          BIT4
#define SMBIOS_TYPE4_EXECUTE_PROTECTION       BIT5
#define SMBIOS_TYPE4_ENHANCED_VIRTUALIZATION  BIT6
#define SMBIOS_TYPE4_POWER_PERF_CONTROL       BIT7

#define SMBIOS_TYPE7_SRAM_SYNCHRONOUS         BIT5

#pragma pack(1)
typedef struct {
  CHAR8 *ProcessorManufacturer;
  CHAR8 *ProcessorVersion;
} SMBIOS_PROCESSOR_INFO_STRING_ARRAY;
#define SMBIOS_PROCESSOR_INFO_NUMBER_OF_STRINGS  2

typedef struct {
  CHAR8 *SocketDesignation;
} SMBIOS_CACHE_INFO_STRING_ARRAY;
#define SMBIOS_CACHE_INFO_NUMBER_OF_STRINGS  1
#pragma pack()

typedef enum {
  CpuStatusUnknown        = 0,
  CpuStatusEnabled        = 1,
  CpuStatusDisabledByUser = 2,
  CpuStatusDisabledbyBios = 3,
  CpuStatusIdle           = 4,
  CpuStatusOther          = 7
} SMBIOS_TYPE4_CPU_STATUS;

typedef union {
  struct {
    UINT8 CpuStatus       :3; // Indicates the status of the processor.
    UINT8 Reserved1       :3; // Reserved for future use. Should be set to zero.
    UINT8 SocketPopulated :1; // Indicates if the processor socket is populated or not.
    UINT8 Reserved2       :1; // Reserved for future use. Should be set to zero.
  } Bits;
  UINT8 Data;
} PROCESSOR_STATUS_DATA;

typedef struct {
  UINT16  Level           :3;
  UINT16  Socketed        :1;
  UINT16  Reserved1       :1;
  UINT16  Location        :2;
  UINT16  Enable          :1;
  UINT16  OperationalMode :2;
  UINT16  Reserved2       :6;
} SMBIOS_TYPE7_CACHE_CONFIGURATION_DATA;

/**
  Add SMBIOS strings to the end of the HOB, then call the BuildGuidDataHob function.
  Add each non-null string, plus the terminating (double) null.

  @param[in]  FixedDataRegion       The data for the fixed portion of the HOB.
  @param[in]  FixedRegionSize       The size of the fixed portion of the HOB.
  @param[in]  TableStrings          Set of string pointers to append onto the full record.
                                    If TableStrings is null, no strings are appended. Null strings
                                    are skipped.
  @param[in]  NumberOfStrings       Number of TableStrings to append, null strings are skipped.
  @param[in]  HobGuid               The GUID to pass to the BuildGuidDataHob function.
**/
VOID
AddSmbiosStringsAndBuildGuidDataHob (
  IN  VOID          *FixedDataRegion,
  IN  UINT8         FixedRegionSize,
  IN  CHAR8         **TableStrings,
  IN  UINT8         NumberOfStrings,
  IN  EFI_GUID      *HobGuid
  );

/**
  This function produces an SMBIOS Processor Info HOB.

**/
VOID
InitializeSmbiosProcessorInfoHob (
  VOID
  );

/**
  This function produces SMBIOS Cache Info HOBs.

**/
VOID
InitializeSmbiosCacheInfoHobs (
  VOID
  );

#endif
