/** @file
  Support to parse the Startup ACM image.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Register/Cpuid.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
//
// Module Self Foundation.
//
#include "AcmImageParser.h"

/**
  Get the ACM information table from input buffer.

  @param[in]   Image         Pointer that point to ACM image buffer.
  @param[in]   ImageSize     Size of ACM image buffer.
  @param[out]  AcmInfoTable  Double pointer to place the ACM information
                             table pointer.

  @retval  EFI_SUCCESS            Succeed to get the correspond table.
  @retval  EFI_NOT_FOUND          AcmInfoTable offset is invalid.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.

**/
EFI_STATUS
GetAcmInfoTable (
  IN     CONST VOID   *Image,
  IN           UINTN  ImageSize,
     OUT       VOID   **AcmInfoTable
  )
{
  ACM_HEADER            *AcmHeader;
  UINT64                AcmHeaderSize;
  UINT64                AcmScratchSize;
  EFI_PHYSICAL_ADDRESS  Address;

  if ((Image == NULL) || (ImageSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if (AcmInfoTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *AcmInfoTable  = NULL;
  AcmHeader      = (ACM_HEADER *)Image;
  AcmHeaderSize  = ((UINT64)(AcmHeader->HeaderLen)) * ACM_HEADER_LEN_MULTIPLES_VALUE;
  AcmScratchSize = ((UINT64)(AcmHeader->ScratchSize)) * ACM_SCRATCH_SIZE_MULTIPLES_VALUE;

  Address = (EFI_PHYSICAL_ADDRESS)AcmHeader + AcmHeaderSize + AcmScratchSize;
  if (Address < ((EFI_PHYSICAL_ADDRESS)Image + sizeof (ACM_HEADER))) {
    DEBUG ((DEBUG_ERROR, "AcmInfoTable offset is overlap the AcmHeader.\n"));
    return EFI_NOT_FOUND;
  } else if (Address > ((EFI_PHYSICAL_ADDRESS)Image + (UINT64)ImageSize)) {
    DEBUG ((DEBUG_ERROR, "AcmInfoTable offset is out of the ACM image.\n"));
    return EFI_NOT_FOUND;
  }

  *AcmInfoTable = (VOID *)Address;

  return EFI_SUCCESS;
}

/**
  Check if the input ACM information table is supported.

  Note:
    - Function would check below items,
        (1) Check if input pointer is NULL.
        (2) Check if the table GUID is matched to gTxtAcmInfoTableGuid.
        (3) Check if the table version is not less than 9.

  @param[in]  AcmInfoTable  Pointer that point to ACM information table.

  @retval  TRUE   Input ACM information table is supported.
  @retval  FALSE  Input ACM information table is not supported.

**/
BOOLEAN
IsAcmInfoTableSupported (
  IN CONST ACM_INFO_TABLE  *AcmInfoTable
  )
{
  if (AcmInfoTable == NULL) {
    return FALSE;
  }

  if (!CompareGuid (&(AcmInfoTable->Guid), &gTxtAcmInfoTableGuid)) {
    return FALSE;
  }

  //
  // Version 9 adds support for the flexible ACM information table format.
  // It is the final version of this table, further updates will be reflected
  // in the ACM Version Information list.
  //
  if (AcmInfoTable->AitVersion < ACM_INFO_TABLE_VERSION_9) {
    return FALSE;
  }

  return TRUE;
}

/**
  Get the processor ID list from input buffer.

  @param[in]   Image               Pointer that point to ACM image
                                   buffer.
  @param[in]   ImageSize           Size of ACM image buffer.
  @param[in]   AcmInfoTable        Pointer that point to ACM information
                                   table.
  @param[out]  AcmProcessorIdList  Double pointer to place the processor
                                   ID list.

  @retval  EFI_SUCCESS            Succeed to get the correspond list.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_UNSUPPORTED        ACM information table is not supported.
  @retval  EFI_UNSUPPORTED        Address from ACM info table is invalid.
  @retval  EFI_UNSUPPORTED        Table ID is not valid.

**/
EFI_STATUS
GetProcessorIdList (
  IN     CONST VOID            *Image,
  IN           UINTN           ImageSize,
  IN     CONST ACM_INFO_TABLE  *AcmInfoTable,
     OUT       VOID            **AcmProcessorIdList
  )
{
  EFI_PHYSICAL_ADDRESS  Address;
  PROCESSOR_ID_LIST     *ProcessorIdList;

  Address         = 0;
  ProcessorIdList = NULL;

  if ((Image == NULL) || (ImageSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if (AcmInfoTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (AcmProcessorIdList == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsAcmInfoTableSupported (AcmInfoTable)) {
    return EFI_UNSUPPORTED;
  }

  *AcmProcessorIdList = NULL;

  //
  // Check the address is not out of boundary.
  //
  Address = (EFI_PHYSICAL_ADDRESS)Image + ((EFI_PHYSICAL_ADDRESS)(AcmInfoTable->ProcessorIdList));
  if (Address < PROCESSOR_ID_LIST_HEADER_PREPRENDED_SIZE) {
    DEBUG ((DEBUG_INFO, "AcmProcessorIdList pointer is incorrect.\n"));
    return EFI_UNSUPPORTED;
  }

  //
  // Get the processor ID list pointer and check the table ID.
  //
  Address         = Address - PROCESSOR_ID_LIST_HEADER_PREPRENDED_SIZE;
  ProcessorIdList = (PROCESSOR_ID_LIST *)Address;
  if (ProcessorIdList->TableId != ACM_PROCESSOR_ID_LIST_ID) {
    DEBUG ((DEBUG_INFO, "AcmProcessorIdList signature is incorrect.\n"));
    return EFI_UNSUPPORTED;
  }

  *AcmProcessorIdList = (VOID *)ProcessorIdList;

  return EFI_SUCCESS;
}

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
