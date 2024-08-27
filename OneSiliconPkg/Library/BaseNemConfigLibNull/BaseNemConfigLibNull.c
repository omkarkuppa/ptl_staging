/**@file
  NEM config library provides APIs for MTRR operation to change NEM (No-Evict Mode) range.

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

#include <Library/BaseLib.h>
#include <Library/NemConfigLib.h>

/**
  Decode ACM real module size

  @param[out] AcmSize    The real size of the ACM.

**/
VOID
EFIAPI
GetAcmSize (
  OUT UINT32    *AcmSize
  )
{
  return;
}

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
  )
{
  return EFI_SUCCESS;
}

/**
  Calculate Effective Cache size and Effective WayCache size.

  @param[out] EffCacheSize        The size of the EffCache.
  @param[out] EffWayCacheSize     The size of the EffWayCache.

  @retval  UINT8                  The number of ACMWay that have been used.

**/
UINT16
EFIAPI
GetEffCacheSize (
  OUT UINT32    *EffCacheSize,
  OUT UINT32    *EffWayCacheSize
  )
{
  return 0;
}

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
  )
{
  return;
}

/**
  Calculate the available NEM size

  @param[in]  DataStackSize    The size of the data stack.
  @param[in]  IbbSize          The size of the common IBB.
  @param[in]  SizeOfNewRange   The size of the new NEM range.

  @retval     UINTN            The size of the remaining NEM range.

**/
UINTN
EFIAPI
NemSizeCalculate (
  IN  UINTN          DataStackSize,
  IN  UINTN          IbbSize,
  IN  UINTN          *SizeOfNewRange
  )
{
  return 0;
}

/**
  Configure the variable MTRRs to change NEM range

  @param[in]  VariableSettings   A buffer to hold variable MTRRs content.

**/
VOID
EFIAPI
SetVariableMtrrForNemRange (
  IN MTRR_VARIABLE_SETTINGS         *VariableSettings
  )
{
  return;
}



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
  @retval EFI_OUT_OF_RESOURCES          There's no enough MTRR pairs or no enough cache size.

**/
EFI_STATUS
EFIAPI
AllocateNemForFlashRegion (
  IN OUT  FLASH_REGION_LIST       *FlashRegionsOnNem,
  IN      FLASH_REGION_SEGMENT    *NewFlashRegionToNem,
  IN      UINT32                  DataStackBase,
  IN      UINT32                  DataStackSize
  )
{
  return EFI_SUCCESS;
}