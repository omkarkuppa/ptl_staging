/** @file
  Data for SMBIOS Cache tables (Type 7).

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

#include "SmbiosCpu.h"
#include <Protocol/CpuInfo.h>

#define MAX_NUMBER_OF_PACKAGE 64

//
// Cache string Information
//
GLOBAL_REMOVE_IF_UNREFERENCED CHAR8 *mCacheStrings[MAX_NUMBER_OF_CACHE_LEVELS] = {
  "L1 Cache",
  "L2 Cache",
  "L3 Cache",
  "Unknown"
};

UINT8 mCoreType[] = { CPUID_CORE_TYPE_INTEL_CORE, CPUID_CORE_TYPE_INTEL_ATOM, CORE_TYPE_NON_HYBRID};

/**
  Get the total number of package and package ID in the platform.

  @param[in]  CacheInfo           Pointer to the CacheInfo array.
  @param[in]  CacheInfoCount      The length of CacheInfo array.
  @param[out] PackageId           Pointer to the PackageId array.

  @retval  Return the total number of package in the platform.
**/
UINT32
GetNumberOfPackage (
  IN  CPU_CACHE_INFO        *CacheInfo,
  IN  UINTN                 CacheInfoCount,
  OUT UINT32                *PackageId
  )
{
  UINTN                     CacheInfoIndex;
  UINT32                    PackageIndex;
  UINT32                    PackageCount;
  CPU_CACHE_INFO            *CurrentCacheInfo;

  for (CacheInfoIndex = 0, PackageCount = 0; CacheInfoIndex < CacheInfoCount; CacheInfoIndex++) {
    CurrentCacheInfo = &CacheInfo[CacheInfoIndex];

    //
    // For the Package ID that already exists in PackageId array, break out the loop.
    //
    for (PackageIndex = 0; PackageIndex < PackageCount; PackageIndex++) {
      if (CurrentCacheInfo->Package == PackageId[PackageIndex]) {
        break;
      }
    }

    //
    // For the new Package ID, save it in PackageId array.
    //
    if (PackageIndex == PackageCount) {
      ASSERT (PackageCount < MAX_NUMBER_OF_PACKAGE);
      PackageId[PackageCount++] = CurrentCacheInfo->Package;
    }
  }

  return PackageCount;
}

/**
  Initialize the SmbiosCacheData array with cache information.

  @param[in]  CacheInfo           Pointer to the CacheInfo array.
  @param[in]  CacheInfoCount      The length of CacheInfo array.
  @param[in]  PackageIndex        The requested package index.
  @param[in]  PackageId           The requested package number.
  @param[in]  CoreType            Pointer to the CoreType array.
  @param[out] SmbiosCacheData     Pointer to the SmbiosCacheData array
  @param[out] SmbiosCacheString   Pointer to the SmbiosCacheString array
  @param[out] CacheLevelCount     Indicates cache level count at each package.

  @retval EFI_SUCCESS             Update SMBIOS cache information successfully.
  @retval EFI_NOT_FOUND           Requried PackageId or CoreType can not be found.
**/
EFI_STATUS
UpdateSmbiosCacheInfoHobs (
  IN  CPU_CACHE_INFO                  *CacheInfo,
  IN  UINTN                           CacheInfoCount,
  IN  UINT32                          PackageIndex,
  IN  UINT32                          PackageId,
  IN  UINT8                           CoreType,
  OUT SMBIOS_CACHE_INFO               *SmbiosCacheData,
  OUT SMBIOS_CACHE_INFO_STRING_ARRAY  *SmbiosCacheString,
  OUT UINT8                           *CacheLevelCount
  )
{
  UINTN                                 CacheInfoIndex;
  UINT8                                 CacheLevel[MAX_NUMBER_OF_CACHE_LEVELS];
  UINT8                                 CacheType[MAX_NUMBER_OF_CACHE_LEVELS];
  UINT16                                CacheWays[MAX_NUMBER_OF_CACHE_LEVELS];
  UINT32                                CacheSize[MAX_NUMBER_OF_CACHE_LEVELS];
  UINT16                                CacheCount[MAX_NUMBER_OF_CACHE_LEVELS];
  UINT8                                 NumberOfCacheLevels;
  BOOLEAN                               CacheLevel1Combine;
  UINT8                                 CacheIndex;
  UINT8                                 LevelIndex;
  SMBIOS_TYPE7_CACHE_CONFIGURATION_DATA CacheConfig;
  UINT32                                TotalCacheSize;

  ZeroMem (CacheLevel, sizeof (CacheLevel));
  ZeroMem (CacheType, sizeof (CacheType));
  ZeroMem (CacheWays, sizeof (CacheWays));
  ZeroMem (CacheSize, sizeof (CacheSize));

  //
  // Find the total number of cache levels reported via CpuCacheInfoLib.
  // Save the output for later use.
  //
  for (CacheInfoIndex = 0, NumberOfCacheLevels = 0; CacheInfoIndex < CacheInfoCount; CacheInfoIndex++) {
    if (CacheInfo[CacheInfoIndex].Package == PackageId && CacheInfo[CacheInfoIndex].CoreType == CoreType) {
      if (NumberOfCacheLevels >= MAX_NUMBER_OF_CACHE_LEVELS) {
        DEBUG ((DEBUG_ERROR, "Error producing Cache Info HOBs. Total number of reported cache levels exceeded limit.\n"));
        break;
      }

      CacheLevel[NumberOfCacheLevels] = (UINT8) CacheInfo[CacheInfoIndex].CacheLevel;
      CacheType[NumberOfCacheLevels]  = (UINT8) CacheInfo[CacheInfoIndex].CacheType;
      CacheWays[NumberOfCacheLevels]  = CacheInfo[CacheInfoIndex].CacheWays + 1;
      CacheSize[NumberOfCacheLevels]  = CacheInfo[CacheInfoIndex].CacheSizeinKB;
      CacheCount[NumberOfCacheLevels] = CacheInfo[CacheInfoIndex].CacheCount;
      NumberOfCacheLevels++;
    }
  }

  //
  // If can not find the cache info with required PackageId and CoreType, return.
  //
  if (NumberOfCacheLevels == 0) {
    return EFI_NOT_FOUND;
  }

  //
  // Check if L1 Data cache and L1 instruction cache can be combined.
  //
  CacheLevel1Combine = FALSE;
  if (NumberOfCacheLevels == MAX_NUMBER_OF_CACHE_LEVELS) {
    if (CacheLevel[0] == 1 && CacheLevel[1] == 1 && CacheWays[0] == CacheWays[1] && CacheSize[0] == CacheSize[1] &&
        ((CacheType[0] == CPUID_CACHE_PARAMS_CACHE_TYPE_DATA && CacheType[1] == CPUID_CACHE_PARAMS_CACHE_TYPE_INSTRUCTION) ||
        (CacheType[0] == CPUID_CACHE_PARAMS_CACHE_TYPE_INSTRUCTION && CacheType[1] == CPUID_CACHE_PARAMS_CACHE_TYPE_DATA))) {
      CacheLevel1Combine = TRUE;
      CacheType[1] = CPUID_CACHE_PARAMS_CACHE_TYPE_UNIFIED;
      CacheSize[1] = CacheSize[0] + CacheSize[1];
    }
  }

  for (CacheIndex = (CacheLevel1Combine ? 1 : 0), LevelIndex = 0; CacheIndex < NumberOfCacheLevels; CacheIndex++) {
    //
    // Make CacheLevel zero-based for indexing and for SMBIOS Cache Configuration format
    //
    ASSERT (CacheLevel[CacheIndex] > 0);
    CacheLevel[CacheIndex]--;

    SmbiosCacheString[LevelIndex].SocketDesignation       = mCacheStrings[CacheLevel[CacheIndex]];
    SmbiosCacheData[LevelIndex].SocketDesignationStrIndex = STRING_1;
    SmbiosCacheData[LevelIndex].ProcessorSocketNumber     = (UINT16) PackageIndex;
    SmbiosCacheData[LevelIndex].NumberOfCacheLevels       = (CacheLevel1Combine ? (MAX_NUMBER_OF_CACHE_LEVELS - 1) : MAX_NUMBER_OF_CACHE_LEVELS);
    SmbiosCacheData[LevelIndex].SupportedSramType         = SMBIOS_TYPE7_SRAM_SYNCHRONOUS;
    SmbiosCacheData[LevelIndex].CurrentSramType           = SMBIOS_TYPE7_SRAM_SYNCHRONOUS;

    CacheConfig.Level           = CacheLevel[CacheIndex];
    CacheConfig.Socketed        = CACHE_NOTSOCKETED;                    // Not Socketed
    CacheConfig.Reserved1       = 0;
    CacheConfig.Location        = CACHE_LOCATION_INTERNAL;              // Internal Cache
    CacheConfig.Enable          = CACHE_ENABLE;                         // Cache enabled
    CacheConfig.OperationalMode = CACHE_OPMODE_WRITE_BACK;              // Write Back
    CacheConfig.Reserved2       = 0;
    SmbiosCacheData[LevelIndex].CacheConfiguration = *(UINT16 *) &CacheConfig;

    //
    // Configure cache size in SMBIOS format
    //
    TotalCacheSize = (UINT32) (CacheSize[CacheIndex] * CacheCount[CacheIndex]);
    if ((TotalCacheSize >> 31) == 0) {
      //
      // Cache size is not larger than 2047 MB.
      //
      if ((TotalCacheSize >> 15) == 0) {
        //
        // Cache size is not larger than 63 MB.
        //
        SmbiosCacheData[LevelIndex].MaxCacheSize      = (UINT16) TotalCacheSize;
        SmbiosCacheData[LevelIndex].InstalledSize     = (UINT16) TotalCacheSize;
        SmbiosCacheData[LevelIndex].MaximumCacheSize2 = TotalCacheSize;
        SmbiosCacheData[LevelIndex].InstalledSize2    = TotalCacheSize;
      } else {
      //
      // Cache size is larger than 63 MB. 64K Granularity considered.
      //
        SmbiosCacheData[LevelIndex].MaxCacheSize      = (UINT16) (TotalCacheSize >> 6) | BIT15;
        SmbiosCacheData[LevelIndex].InstalledSize     = SmbiosCacheData[LevelIndex].MaxCacheSize;
        SmbiosCacheData[LevelIndex].MaximumCacheSize2 = (TotalCacheSize >> 6) | BIT31;
        SmbiosCacheData[LevelIndex].InstalledSize2    = SmbiosCacheData[LevelIndex].MaximumCacheSize2;
      }
    } else {
      //
      // Cache size is larger than 2047 MB
      //
      SmbiosCacheData[LevelIndex].MaxCacheSize      = (UINT16) (-1);
      SmbiosCacheData[LevelIndex].InstalledSize     = (UINT16) (-1);
      SmbiosCacheData[LevelIndex].MaximumCacheSize2 = (TotalCacheSize >> 6) | BIT31;
      SmbiosCacheData[LevelIndex].InstalledSize2    = SmbiosCacheData[LevelIndex].MaximumCacheSize2;
    }

    //
    // Configure Error Correction Type in SMBIOS format
    //
    switch (CacheLevel[CacheIndex]) {
      case 0:
        SmbiosCacheData[LevelIndex].ErrorCorrectionType = CacheErrorParity;
        break;
      case 1:
        SmbiosCacheData[LevelIndex].ErrorCorrectionType = CacheErrorSingleBit;
        break;
      case 2:
        SmbiosCacheData[LevelIndex].ErrorCorrectionType = CacheErrorMultiBit;
        break;
      default:
        SmbiosCacheData[LevelIndex].ErrorCorrectionType = CacheErrorUnknown;
        break;
    }

    //
    // Convert Cache Type to SMBIOS format
    //
    switch (CacheType[CacheIndex]) {
      case CPUID_CACHE_PARAMS_CACHE_TYPE_DATA:
        SmbiosCacheData[LevelIndex].SystemCacheType = CacheTypeData;
        break;
      case CPUID_CACHE_PARAMS_CACHE_TYPE_INSTRUCTION:
        SmbiosCacheData[LevelIndex].SystemCacheType = CacheTypeInstruction;
        break;
      case CPUID_CACHE_PARAMS_CACHE_TYPE_UNIFIED:
        SmbiosCacheData[LevelIndex].SystemCacheType = CacheTypeUnified;
        break;
      default:
        SmbiosCacheData[LevelIndex].SystemCacheType = CacheTypeUnknown;
        break;
    }

    //
    // Convert Associativity Ways to SMBIOS format
    //
    switch (CacheWays[CacheIndex]) {
      case 2:
        SmbiosCacheData[LevelIndex].Associativity = CacheAssociativity2Way;
        break;
      case 4:
        SmbiosCacheData[LevelIndex].Associativity = CacheAssociativity4Way;
        break;
      case 8:
        SmbiosCacheData[LevelIndex].Associativity = CacheAssociativity8Way;
        break;
      case 12:
        SmbiosCacheData[LevelIndex].Associativity = CacheAssociativity12Way;
        break;
      case 16:
        SmbiosCacheData[LevelIndex].Associativity = CacheAssociativity16Way;
        break;
      case 24:
        SmbiosCacheData[LevelIndex].Associativity = CacheAssociativity24Way;
        break;
      case 32:
        SmbiosCacheData[LevelIndex].Associativity = CacheAssociativity32Way;
        break;
      case 48:
        SmbiosCacheData[LevelIndex].Associativity = CacheAssociativity48Way;
        break;
      case 64:
        SmbiosCacheData[LevelIndex].Associativity = CacheAssociativity64Way;
        break;
      default:
        SmbiosCacheData[LevelIndex].Associativity = CacheAssociativityOther;
        break;
    }

    LevelIndex++;
  }

  *CacheLevelCount = LevelIndex;

  return EFI_SUCCESS;
}

/**
  Produce SMBIOS Cache Information HOBs

**/

VOID
InitializeSmbiosCacheInfoHobs (
  VOID
  )
{
  EFI_STATUS                      Status;
  CPU_CACHE_INFO                  *CacheInfo;
  UINTN                           CacheInfoCount;
  VOID                            *CacheInfoHob;
  UINT32                          PackageId[MAX_NUMBER_OF_PACKAGE];
  UINT32                          PackageCount;
  UINT32                          PackageIndex;
  UINT16                          CoreTypeIndex;
  SMBIOS_CACHE_INFO               SmbiosCacheData[MAX_NUMBER_OF_CACHE_LEVELS];
  SMBIOS_CACHE_INFO_STRING_ARRAY  SmbiosCacheString[MAX_NUMBER_OF_CACHE_LEVELS];
  UINT8                           CacheLevelIndex;
  UINT8                           CacheLevelCount;

  DEBUG ((DEBUG_INFO, "InitializeSmbiosCacheInfoHobs() - Start\n"));

  //
  // Get CpuCacheInfo data array.
  //
  CacheInfoCount = 0;
  Status = GetCpuCacheInfo (NULL, &CacheInfoCount);

  if (Status == EFI_BUFFER_TOO_SMALL) {
    CacheInfo = AllocateZeroPool (CacheInfoCount * sizeof (*CacheInfo));
    if (CacheInfo == NULL) {
      ASSERT (CacheInfo != NULL);
      return;
    }

    Status = GetCpuCacheInfo (CacheInfo, &CacheInfoCount);
    DEBUG ((DEBUG_INFO, "GetCpuCacheInfo: %r (CacheInfoCount = %d)\n", Status, CacheInfoCount));
    ASSERT_EFI_ERROR (Status);

    CacheInfoHob = BuildGuidDataHob (&gCpuCacheInfoHobGuid, (VOID *)CacheInfo, CacheInfoCount * sizeof(*CacheInfo));
    ASSERT (CacheInfoHob != NULL);

  } else {
    DEBUG ((DEBUG_ERROR, "GetCpuCacheInfo: %r (CacheInfoCount = %d)\n", Status, CacheInfoCount));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  PackageCount = GetNumberOfPackage (CacheInfo, CacheInfoCount, PackageId);

  //
  // Use required PackageId and CoreType to search for cache info in CacheInfo array.
  // If found, update SmbiosCacheData and SmbiosCacheString, then build SmbiosCacheInfoHob.
  // For Hybrid CPU, GetCpuCacheInfo API returns two core types (CPUID_CORE_TYPE_INTEL_CORE
  // and CPUID_CORE_TYPE_INTEL_ATOM).
  // For Non-Hybrid CPU, the API returns one core type (CORE_TYPE_NON_HYBRID).
  //
  for (PackageIndex = 0; PackageIndex < PackageCount; PackageIndex++) {
    for (CoreTypeIndex = 0; CoreTypeIndex < ARRAY_SIZE (mCoreType); CoreTypeIndex++) {
      ZeroMem (SmbiosCacheData, sizeof (SmbiosCacheData));
      ZeroMem (SmbiosCacheString, sizeof (SmbiosCacheString));

      Status = UpdateSmbiosCacheInfoHobs (
        CacheInfo,
        CacheInfoCount,
        PackageIndex,
        PackageId[PackageIndex],
        mCoreType[CoreTypeIndex],
        SmbiosCacheData,
        SmbiosCacheString,
        &CacheLevelCount
      );

      if (EFI_ERROR (Status)) {
        continue;
      }

      for (CacheLevelIndex = 0; CacheLevelIndex < CacheLevelCount; CacheLevelIndex++) {
        AddSmbiosStringsAndBuildGuidDataHob (
          (VOID *)(SmbiosCacheData + CacheLevelIndex),
          sizeof (*SmbiosCacheData),
          (CHAR8 **)(SmbiosCacheString + CacheLevelIndex),
          SMBIOS_CACHE_INFO_NUMBER_OF_STRINGS,
          &gSmbiosCacheInfoHobGuid
        );
      }
    }
  }

  FreePool (CacheInfo);

  DEBUG ((DEBUG_INFO, "InitializeSmbiosCacheInfoHobs() - End\n"));
  return;
}
