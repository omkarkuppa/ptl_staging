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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
//
// Module Self Foundation.
//
#include "AcmImageParserInternal.h"

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
  if (ProcessorIdList->Header.TableId != ACM_PROCESSOR_ID_LIST_ID) {
    DEBUG ((DEBUG_INFO, "AcmProcessorIdList signature is incorrect.\n"));
    return EFI_UNSUPPORTED;
  }

  *AcmProcessorIdList = (VOID *)ProcessorIdList;

  return EFI_SUCCESS;
}

/**
  Get specified list from input buffer.

  Assumption:
    1. AcmInfoTable should start with the Base Table.
    2. There is no gaps between lists

  @param[in]   Image               A pointer to ACM image buffer.
  @param[in]   ImageSize           Size of ACM image buffer.
  @param[in]   AcmInfoTable        A pointer to ACM information table.
  @param[in]   TableId             List ID
  @param[out]  AcmRequestedIdList  A pointer to the requested ID list.

  @retval  EFI_SUCCESS            Succeed to get the corresponding list.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_UNSUPPORTED        ACM information table is not supported.
  @retval  EFI_UNSUPPORTED        Address from ACM info table is invalid.
  @retval  EFI_NOT_FOUND          TableId is not found in the lists.

**/
EFI_STATUS
AcmInfoTableGetListById (
  IN     CONST VOID                       *Image,
  IN           UINTN                      ImageSize,
  IN     CONST ACM_INFO_TABLE             *AcmInfoTable,
  IN           UINT32                     TableId,
     OUT       ACM_ID_LIST_COMMON_HEADER  **AcmRequestedIdList
  )
{
  UINT64                     Address;
  UINT64                     ImageStart;
  UINT64                     ImageEnd;
  ACM_ID_LIST_COMMON_HEADER  *IdListPointer;

  Address       = 0;
  ImageStart    = 0;
  ImageEnd      = 0;
  IdListPointer = NULL;

  if ((Image == NULL) || (ImageSize == 0) || (AcmInfoTable == NULL) || (AcmRequestedIdList == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (ImageSize > (MAX_UINT64 - (UINT64)Image)) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsAcmInfoTableSupported (AcmInfoTable)) {
    return EFI_UNSUPPORTED;
  }

  *AcmRequestedIdList = NULL;
  Address             = (UINT64)AcmInfoTable + AcmInfoTable->AitLength;
  ImageStart          = (UINT64)Image;
  ImageEnd            = (UINT64)Image + ImageSize;

  while ((Address >= ImageStart) && (Address < ImageEnd)) {
    IdListPointer = (ACM_ID_LIST_COMMON_HEADER *)Address;

    if (IdListPointer->TableId == TableId) {
      DEBUG ((DEBUG_INFO, "Find a match with ID 0x%x\n", TableId));
      *AcmRequestedIdList = IdListPointer;
      return EFI_SUCCESS;
    }

    if (IdListPointer->TableId == ACM_TERMINATOR_LIST_ID) {
      DEBUG ((DEBUG_INFO, "Reach to the end of lists\n"));
      break;
    }

    if (IdListPointer->Length > (ImageEnd - Address)) {
      return EFI_BAD_BUFFER_SIZE;
    }

    Address = Address + IdListPointer->Length;
  }

  return EFI_NOT_FOUND;
}
