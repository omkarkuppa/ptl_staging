/** @file
  Header file for the SiMtrrLib library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _SI_MTRR_LIBRARY_H_
#define _SI_MTRR_LIBRARY_H_

#include <Base.h>
#include <Library/HostBridgeDataLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Register/Msr.h>
#include <Register/Cpuid.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MtrrLib.h>
#include <Library/SiMtrrLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Uefi.h>
#include <SpiInfoHob.h>
#include <Library/IGpuInfoLib.h>
//
// Structure to describe a updated variable MTRR
//
typedef struct {
  UINT64                  BaseAddress;
  UINT64                  Length;
  MTRR_MEMORY_CACHE_TYPE  OrgMtrrType;
  MTRR_MEMORY_CACHE_TYPE  ChgMtrrType;
  BOOLEAN Valid;
} UPDATED_VARIABLE_MTRR;

//
// Memory cache types
//
typedef enum {
  Flash              = 0,
  InternalGraphics   = 1,
  ExtendedBiosRegion = 2
} MTRR_UPDATED;

typedef union {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
} UINT64_STRUCT;

//
// Below function's implemented are inside UefiCpuPkg\Library\MtrrLib\MtrrLib.c
// This is a backport from BP1510
//
/**
  Initializes the valid bits mask and valid address mask for MTRRs.

  This function initializes the valid bits mask and valid address mask for MTRRs.

  @param[out]  MtrrValidBitsMask     The mask for the valid bit of the MTRR
  @param[out]  MtrrValidAddressMask  The valid address mask for the MTRR

**/
VOID
MtrrLibInitializeMtrrMask (
  OUT UINT64 *MtrrValidBitsMask,
  OUT UINT64 *MtrrValidAddressMask
  );

/**
  Convert variable MTRRs to a RAW MTRR_MEMORY_RANGE array.
  One MTRR_MEMORY_RANGE element is created for each MTRR setting.
  The routine doesn't remove the overlap or combine the near-by region.

  @param[in]   VariableSettings      The variable MTRR values to shadow
  @param[in]   VariableMtrrCount     The number of variable MTRRs
  @param[in]   MtrrValidBitsMask     The mask for the valid bit of the MTRR
  @param[in]   MtrrValidAddressMask  The valid address mask for MTRR
  @param[out]  VariableMtrr          The array to shadow variable MTRRs content

  @return      Number of MTRRs which has been used.

**/
UINT32
MtrrLibGetRawVariableRanges (
  IN  MTRR_VARIABLE_SETTINGS  *VariableSettings,
  IN  UINTN                   VariableMtrrCount,
  IN  UINT64                  MtrrValidBitsMask,
  IN  UINT64                  MtrrValidAddressMask,
  OUT MTRR_MEMORY_RANGE       *VariableMtrr
  );

/**
  Apply the variable MTRR settings to memory range array.

  @param[in]      VariableMtrr      The variable MTRR array.
  @param[in]      VariableMtrrCount The count of variable MTRRs.
  @param[in, out] Ranges            Return the memory range array with new MTRR settings applied.
  @param[in]      RangeCapacity     The capacity of memory range array.
  @param[in, out] RangeCount        Return the count of memory range.

  @retval RETURN_SUCCESS            The memory range array is returned successfully.
  @retval RETURN_OUT_OF_RESOURCES   The count of memory ranges exceeds capacity.
**/
RETURN_STATUS
MtrrLibApplyVariableMtrrs (
  IN     CONST MTRR_MEMORY_RANGE *VariableMtrr,
  IN     UINT32                  VariableMtrrCount,
  IN OUT MTRR_MEMORY_RANGE       *Ranges,
  IN     UINTN                   RangeCapacity,
  IN OUT UINTN                   *RangeCount
  );
#endif // _SI_MTRR_LIBRARY_H_
