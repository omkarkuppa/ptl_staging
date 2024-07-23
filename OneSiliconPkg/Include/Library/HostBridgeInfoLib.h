/** @file
  Header file for Host Bridge Info Lib implementation.

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

#ifndef _HOST_BRIDGE_INFO_LIB_H_
#define _HOST_BRIDGE_INFO_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>


typedef enum {
  // NOC
  NocExclusionRangeLow  = 0,
  NocExclusionRangeHigh,
  // IA
  IaExclusionRange,
  IaExclusionRange1
} IMR_EXCLUSION_OFFSET;

typedef enum {
  ExclusionOffsetBase = 0,
  ExclusionOffsetMask,
  ExclusionOffsetLimit
} IMR_EXCLUSION_OFFSET_TYPE;
/**
  Checks if SAM is enabled or not.

  @retval FALSE  SAM is disabled
  @retval TRUE   SAM is enabled
**/
BOOLEAN
EFIAPI
IsSamEnabled (
  VOID
  );

/**
  GetSamFwInfo - This function performs SAM ready state and get its FW version info.

  This function does the following steps:
  (1) Examines SAM_READY (Register SAM_STATUS_1, bit 30)
  (2) If 0, retry in SAM_READY_TIMEOUT ms (1ms timeout)
  (3) Return & print  FW version in success, else return 0x0.

  @retval UINT32       - SAM FW Version if SAM is ready, else 0x0.
**/
UINT32
EFIAPI
GetSamFwInfo (
  VOID
  );

  /**
  Regbar size- This function return RegBar Size.

  @retval UINT32       - RegBar Size.
**/
UINT32
EFIAPI
GetRegBarSize (
  VOID
  );

/**
  This function will check if Tseg base is consistent in different memory scopes.

  @retval TRUE          Tseg Base is consistent
  @retval FALSE         Tseg Base is not consistent
**/
BOOLEAN
IsTsegBaseAndImrMemoryConsistent (
  VOID
  );

/**
  Checks whether MMIO above 4GB is enabled or not
  @return TRUE if MMIO above 4GB is enabled, FALSE otherwise
**/
BOOLEAN
IsAbove4GBMmioEnabled (
  VOID
  );

/**
  Checks whether Crid is enabled or not
  @return Crid is disabled
**/
BOOLEAN
IsCridEnabled (
  VOID
  );

/**
  This function will check if IMR BGSM range is enabled or not

  @retval TRUE          IMR BGSM range is enabled
  @retval FALSE         IMR BGSM range is disabled
**/
BOOLEAN
EFIAPI
IsImrBgsmRangeEnabled (
  VOID
  );

/**
  This function will check if IMR BDSM range is enabled or not

  @retval TRUE          IMR BDSM range is enabled
  @retval FALSE         IMR BDSM range is disabled
**/
BOOLEAN
EFIAPI
IsImrBdsmRangeEnabled (
  VOID
  );

/**
  This function will check if IMR CCS range is enabled or not

  @retval TRUE          IMR CCS range is enabled
  @retval FALSE         IMR CCS range is disabled
**/
BOOLEAN
EFIAPI
IsImrCcsRangeEnabled (
  VOID
  );

/**
  This function will check if TSEG range is enabled or not

  @retval TRUE          IMR TSEG range is enabled
  @retval FALSE         IMR TSEG range is disabled
**/
BOOLEAN
EFIAPI
IsImrTsegRangeEnabled (
  VOID
  );

/**
  This function is provide Imr related details (Base, Mask, Limit).

  @param[in] ExclusionRange     - IMR name or ID
  @param[in] ExclusionRangeType - Requested data (Base, Mask, Limit)

  @retval UINT64                - Register Details based on DataType
**/
UINT64
EFIAPI
GetExclusionRegisterOffset (
  IN IMR_EXCLUSION_OFFSET          ExclusionRange,
  IN IMR_EXCLUSION_OFFSET_TYPE     ExclusionRangeType
  );

/**
  This function is provide Imr Size based on selected Imr

  @param[in] ExclusionRange     - IMR name or ID

  @retval UINT32                - IMR Size
**/
UINT32
EFIAPI
GetExclusionRegisterSize (
  IN IMR_EXCLUSION_OFFSET          ExclusionRange
  );
#endif
