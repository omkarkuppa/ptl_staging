/** @file
  Library to provide the generic function to operate on ACM image.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Register/Cpuid.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
//
// Advanced Feature Package Foundation.
//
#include <Library/AcmHelperLib.h>
//
// Module Self Foundation.
//
#include "AcmImageParserInternal.h"

/**
  Check if the input ACM buffer is supported on current CPU.

  @param[in]  Image      Pointer that point to ACM image buffer.
  @param[in]  ImageSize  Size of ACM image buffer.

  @retval  TRUE   Input ACM buffer is supported on current CPU.
  @retval  FALSE  Input ACM buffer is not supported on current CPU.

**/
BOOLEAN
EFIAPI
IsSAcmImageForCurrentCpu (
  IN CONST VOID   *Image,
  IN       UINTN  ImageSize
  )
{
  EFI_STATUS              Status;
  UINT32                  Index;
  UINT32                  FmsValue;
  UINT32                  FmsMaskValue;
  CPUID_VERSION_INFO_EAX  CpuSignature;
  ACM_INFO_TABLE          *AcmInfoTable;
  PROCESSOR_ID_LIST       *AcmProcessorIdList;

  CpuSignature.Uint32 = 0;
  Index               = 0;
  FmsValue            = 0;
  FmsMaskValue        = 0;
  AcmInfoTable        = NULL;
  AcmProcessorIdList  = NULL;

  Status = GetAcmInfoTable (Image, ImageSize, (VOID **)&AcmInfoTable);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = GetProcessorIdList (Image, ImageSize, AcmInfoTable, (VOID **)&AcmProcessorIdList);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  //
  // Get the current CPU signature.
  //
  AsmCpuid (CPUID_VERSION_INFO, &CpuSignature.Uint32, NULL, NULL, NULL);
  DEBUG ((DEBUG_INFO, "CpuSignature: 0x%x\n", CpuSignature.Uint32));

  for (Index = 0; Index < AcmProcessorIdList->Count; Index++) {
    FmsValue     = AcmProcessorIdList->ProcessorID[Index].Fms;
    FmsMaskValue = AcmProcessorIdList->ProcessorID[Index].FmsMask;

    DEBUG ((DEBUG_INFO, "Index         : 0x%X\n", Index));
    DEBUG ((DEBUG_INFO, "FMS Value     : 0x%X\n", FmsValue));
    DEBUG ((DEBUG_INFO, "FMS Mask Value: 0x%X\n", FmsMaskValue));

    //
    // Check if ACM image is supported current platform CPU.
    //
    if ((CpuSignature.Uint32 & FmsMaskValue) == FmsValue) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Get Min FBM SVN from input ACM buffer

  @param[in]  Image      A pointer to ACM image buffer.
  @param[in]  ImageSize  Size of ACM image buffer.

  @retval     MIN FBM SVN value in ACM. If failed to find MinFbmSvn, return 0xFF

**/
UINT8
EFIAPI
GetFbmMinSvnFromAcm (
  IN CONST VOID   *Image,
  IN       UINTN  ImageSize
  )
{
  EFI_STATUS         Status;
  ACM_INFO_TABLE     *AcmInfoTable;
  ACM_VER_INFO_LIST  *AcmVerInfoList;

  AcmInfoTable   = NULL;
  AcmVerInfoList = NULL;

  Status = GetAcmInfoTable (Image, ImageSize, (VOID **)&AcmInfoTable);
  if (EFI_ERROR (Status)) {
    return MAX_UINT8;
  }

  Status = AcmInfoTableGetListById (
             Image,
             ImageSize,
             AcmInfoTable,
             ACM_VERSION_INFO_LIST_ID,
             (ACM_ID_LIST_COMMON_HEADER **)&AcmVerInfoList
             );

  if (EFI_ERROR (Status)) {
    return MAX_UINT8;
  }

  return ((AcmVerInfoList->AcmSecVer >> 8) & MAX_UINT8);
}
