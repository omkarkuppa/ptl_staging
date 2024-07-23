/** @file
  MTRR library provides APIs for MTRR operation to change NEM (No-Evict Mode) range.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef  _NEM_CONFIG_LIB_H_
#define  _NEM_CONFIG_LIB_H_

//
// Include files
//
#include <Uefi.h>
#include <PiPei.h>
#include <Library/MtrrLib.h>

#define FLASH_REGION_COUNT_MAX    8

typedef struct _FLASH_REGION_SEGMENT {
  UINT32                  TargetNemBufferType;
  UINT32                  FlashRegionBase;
  UINT32                  FlashRegionSize;
} FLASH_REGION_SEGMENT;

typedef struct _FLASH_REGION_LIST {
  UINT32                  Count;
  FLASH_REGION_SEGMENT    FlashRegionList[FLASH_REGION_COUNT_MAX];
} FLASH_REGION_LIST;

#define NEM_BUFFER_COUNT_MAX    9

typedef struct _NEM_BUFFER_SEGMENT {
  UINT32                  NemBufferType;
  UINT32                  NemBufferBase;
  UINT32                  NemBufferSize;
} NEM_BUFFER_SEGMENT;

typedef struct _NEM_BUFFER_LIST {
  UINT32                  Count;
  NEM_BUFFER_SEGMENT      NemBufferList[NEM_BUFFER_COUNT_MAX];
} NEM_BUFFER_LIST;

/**
  Check ACM setups initial NEM or not.

  @param[out] AcmEnable             Return True if ACM sets up initial NEM.

  @retval EFI_SUCCESS               Function is executed successfully.
  @retval EFI_INVALID_PARAMETER     AcmEnable is NULL.
**/
EFI_STATUS
EFIAPI
AcmSetNemCheck (
  OUT BOOLEAN    *AcmEnable
  );

/**
  Function to count how many variable MTRRs have been used.
  @retval  UINTN      The number of MTRRs that have been used.
**/
UINTN
EFIAPI
UsedMtrrCount (
  VOID
  );

/**
  Decode ACM real module size

  @param[out] AcmSize    The real size of the ACM.

**/
VOID
EFIAPI
GetAcmSize (
  OUT UINT32    *AcmSize
  );

/**
  Calculate Effective Cache size and Effective WayCache size.

  @param[out] EffCacheSize        The size of the EffCache.
  @param[out] EffWayCacheSize     The size of the EffWayCache.

  @retval  UINT16             The number of ACMWay that have been used.

**/
UINT16
EFIAPI
GetEffCacheSize (
  OUT UINT32    *EffCacheSize,
  OUT UINT32    *EffWayCacheSize
  );

/**
  Calculate the available NEM size

  @param[in]  DataStackSize    The size of the data stack.
  @param[in]  IbbSize          The size of the common IBB.
  @param[in]  SizeOfNewRange   The size of the new NEM range.
  @param[in]  NumberOfGb       The number of guard band that will be applied.
                               Input 0 if no specific, then the default will be 2 guard bands.

  @retval     UINTN            The size of the remaining NEM range.

**/
UINTN
EFIAPI
NemSizeCalculate (
  IN  UINTN          DataStackSize,
  IN  UINTN          IbbSize,
  IN  UINTN          *SizeOfNewRange
  );

/**
  Calculate overall Guard Band size.

  @param[in] EffWayCacheSize        The size of the EffWayCache.
  @param[out] AllGbSize             The size of all the GuardBand add up.

**/
VOID
EFIAPI
GetGbSize (
  IN  UINT32    EffWayCacheSize,
  OUT UINTN     *AllGbSize
  );

/**
  Configure the variable MTRRs to change NEM range

  @param[in]  VariableSettings   A buffer to hold variable MTRRs content.

**/
VOID
EFIAPI
SetVariableMtrrForNemRange (
  IN MTRR_VARIABLE_SETTINGS      *VariableSettings
  );

/**
  This API will not perform any load operations, but only calculate if there are enough NEM size
  for a given flash region expect to load to NEM, and config MTRR for this region.
  Load operation will be actually performed by cache mechanism, after MTRR is correctly configured.

  @param[in, out]  FlashRegionsOnNem    Existing list which contains all regions were already
                                        loaded on NEM, caller should be clear of this information.
                                        All flash regions in list should be reorganized in ascending
                                        order on flash region base, overlaps are not allowed between
                                        different flash regions.
                                        On OUT, this list will be updated if this flash region is
                                        allowed to load to NEM.
  @param[in]       NewFlashRegionToNem  Region expected to load to NEM. Caller must make sure this
                                        region has no overlaps with existing regions in FlashRegionsOnNem.
  @param[in]       DataStackBase        The base address of data stack allocated by bios. Must be
                                        aligned to Effective WayCacheSize.
  @param[in]       DataStackSize        The size of data stack allocated by bios. Must be aligned to
                                        Effective WayCacheSize.

  @retval EFI_SUCCESS                   MTRR has been configured successfully.
  @retval EFI_INVALID_PARAMETER         One or more parameters are not valid.
  @retval EFI_OUT_OF_RESOURCES          There's no enough MTRR pairs or no enough cahce size.

**/
EFI_STATUS
EFIAPI
AllocateNemForFlashRegion (
  IN OUT  FLASH_REGION_LIST       *FlashRegionsOnNem,
  IN      FLASH_REGION_SEGMENT    *NewFlashRegionToNem,
  IN      UINT32                  DataStackBase,
  IN      UINT32                  DataStackSize
  );

#endif // _NEM_CONFIG_LIB_H_
