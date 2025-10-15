/** @file
  Library to provide service for sending FSP error information to bootloader.

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
#include <Uefi.h>
#include <Pi/PiMultiPhase.h>
#include <Guid/FspErrorInfo.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/BaseMemoryLib.h>

/**
  Function attempts to convert FSP error status code from EFI_STATUS to UINT32

  @param[in] Status             - FSP error status code in EFI_STATUS.

  @retval Status32              - FSP error status code in UINT32.
**/
UINT32
EFIAPI
EfiStatusToUint32 (
  EFI_STATUS Status
  )
{
  UINT32 Status32;

  Status32 = (UINT32) (((UINTN) Status) & 0x3FFFFFFF);
  Status32 |= (Status & MAX_BIT) ? BIT31 : 0;
  Status32 |= (Status & (MAX_BIT >> 1)) ? BIT30 : 0; // BIT30 indicates that an API is requesting that a reset is required.

  return Status32;
}

/**
  Function attempts to convert FSP error status code from UINT32 to EFI_STATUS

  @param[in] Status32           - FSP error status code in UINT32.

  @retval Status                - FSP error status code in EFI_STATUS.
**/
EFI_STATUS
EFIAPI
Uint32ToEfiStatus (
  UINT32 Status32
  )
{
  EFI_STATUS Status;

  Status = (EFI_STATUS) (((UINTN) Status32) & 0x3FFFFFFF);
  Status |= (Status32 & BIT31) ? MAX_BIT : 0;
  Status |= (Status32 & BIT30) ? (MAX_BIT >> 1) : 0; // BIT30 indicates that an API is requesting that a reset is required.

  return Status;
}

/**
  Function attempts to send FSP error information to bootloader
  by both FSP_ERROR_INFO_HOB and ReportStatusCode service.

  @param[in] CallerId           - GUID indicates which component is executing.
  @param[in] ErrorType          - GUID indicates what error was encountered.
  @param[in] Status             - EFI_STATUS code for the error.

  @retval EFI_SUCCESS           - The function always return EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
SendFspErrorInfo (
  IN EFI_GUID CallerId,
  IN EFI_GUID ErrorType,
  IN UINT32   Status
  )
{
  FSP_ERROR_INFO_HOB    *FspErrorInfoHob;
  EFI_HOB_GUID_TYPE     *HobData;
  FSP_ERROR_INFO        FspErrorInfo;

  //
  // Create Guid type HOB
  //
  HobData = BuildGuidHob (&gFspErrorInfoHobGuid, sizeof (FSP_ERROR_INFO_HOB) - sizeof (EFI_HOB_GUID_TYPE));
  if (HobData != NULL) {
    HobData -= 1;
    FspErrorInfoHob = (FSP_ERROR_INFO_HOB *) HobData;
    FspErrorInfoHob->Type = FSP_ERROR_INFO_STATUS_CODE_TYPE;
    FspErrorInfoHob->Value = FSP_ERROR_INFO_STATUS_CODE_VALUE;
    FspErrorInfoHob->Instance = FSP_ERROR_INFO_STATUS_CODE_INSTANCE;
    FspErrorInfoHob->CallerId = CallerId;
    FspErrorInfoHob->ErrorType = ErrorType;
    FspErrorInfoHob->Status = EfiStatusToUint32 (Status);
  }

  //
  // Send Report Status Code
  //
  FspErrorInfo.ErrorType = ErrorType;
  FspErrorInfo.Status = Status;

  REPORT_STATUS_CODE_EX (
    FSP_ERROR_INFO_STATUS_CODE_TYPE,
    FSP_ERROR_INFO_STATUS_CODE_VALUE,
    FSP_ERROR_INFO_STATUS_CODE_INSTANCE,
    &CallerId,
    &gStatusCodeDataTypeFspErrorGuid,
    &FspErrorInfo,
    sizeof (FSP_ERROR_INFO)
    );
  return EFI_SUCCESS;
}

/**
  Function attempts to send FSP error information to bootloader
  by ReportStatusCode service.
  This typically is used by DXE drivers inside FSP which cannot
  create hob.

  @param[in] CallerId           - GUID indicates which component is executing.
  @param[in] ErrorType          - GUID indicates what error was encountered.
  @param[in] Status             - EFI_STATUS code for the error.

  @retval EFI_SUCCESS           - The function always return EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
SendFspErrorInfoStatusCode (
  IN EFI_GUID   CallerId,
  IN EFI_GUID   ErrorType,
  IN EFI_STATUS Status
  )
{
  FSP_ERROR_INFO        FspErrorInfo;

  //
  // Send Report Status Code
  //
  FspErrorInfo.ErrorType = ErrorType;
  FspErrorInfo.Status = Status;

  REPORT_STATUS_CODE_EX (
    FSP_ERROR_INFO_STATUS_CODE_TYPE,
    FSP_ERROR_INFO_STATUS_CODE_VALUE,
    FSP_ERROR_INFO_STATUS_CODE_INSTANCE,
    &CallerId,
    &gStatusCodeDataTypeFspErrorGuid,
    &FspErrorInfo,
    sizeof (FSP_ERROR_INFO)
    );
  return EFI_SUCCESS;
}

/**
  Function attempts to dump all FSP error information hobs.

  @param[in] HobList            - Pointer to the HOB data structure.

  @retval EFI_SUCCESS           - No FSP_ERROR_INFO_HOB found.
  @retval EFI_DEVICE_ERROR      - At least one FSP_ERROR_INFO_HOB found.
**/
EFI_STATUS
EFIAPI
DumpFspErrorInfo (
  IN VOID *HobList
  )
{
  EFI_STATUS            Status;
  FSP_ERROR_INFO_HOB    *FspErrorInfoHob;
  EFI_HOB_GUID_TYPE     *HobData;

  DEBUG ((DEBUG_ERROR, "Dump FSP_ERROR_INFO_HOB - start:\n"));
  Status = EFI_SUCCESS;
  HobData = GetNextGuidHob (&gFspErrorInfoHobGuid, HobList);

  while (HobData != NULL) {
    FspErrorInfoHob = (FSP_ERROR_INFO_HOB *) HobData;
    DEBUG ((DEBUG_ERROR, "\n"));
    DEBUG ((DEBUG_ERROR, "FspErrorInfoHob->CallerId  = %g\n", (EFI_GUID *) &FspErrorInfoHob->CallerId));
    DEBUG ((DEBUG_ERROR, "FspErrorInfoHob->ErrorType = %g\n", (EFI_GUID *) &FspErrorInfoHob->ErrorType));
    DEBUG ((DEBUG_ERROR, "FspErrorInfoHob->Status    = %r\n", Uint32ToEfiStatus (FspErrorInfoHob->Status)));
    Status = EFI_DEVICE_ERROR;
    HobData = GET_NEXT_HOB (HobData);
    HobData = GetNextGuidHob (&gFspErrorInfoHobGuid, HobData);
  }
  DEBUG ((DEBUG_ERROR, "\n"));
  DEBUG ((DEBUG_ERROR, "Dump FSP_ERROR_INFO_HOB - done.\n"));
  return Status;
}

/**
  ReportStatusCode worker for FSP Error Information.

  @param  CodeType         Always (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED)
  @param  Value            Always 0
  @param  Instance         Always 0
  @param  CallerId         This optional parameter may be used to identify the caller.
                           It may be used to identify which internal component of the FSP
                           was executing at the time of the error.
  @param  Data             This data contains FSP error type and status code.

  @retval EFI_SUCCESS      Show error status sent by FSP successfully.
  @retval RETURN_ABORTED   Function skipped as unrelated.

**/
EFI_STATUS
EFIAPI
FspErrorStatusCodeReportWorker (
  IN EFI_STATUS_CODE_TYPE       CodeType,
  IN EFI_STATUS_CODE_VALUE      Value,
  IN UINT32                     Instance,
  IN CONST EFI_GUID             *CallerId,
  IN CONST EFI_STATUS_CODE_DATA *Data OPTIONAL
  )
{
  FSP_ERROR_INFO  *FspErrorInfo;

  if ((CodeType == FSP_ERROR_INFO_STATUS_CODE_TYPE) &&
      (Value == FSP_ERROR_INFO_STATUS_CODE_VALUE) &&
      (Data != NULL) &&
      (CompareGuid (&Data->Type, &gStatusCodeDataTypeFspErrorGuid))) {
    FspErrorInfo = (FSP_ERROR_INFO *) (Data + 1);
    DEBUG ((DEBUG_ERROR, "\nFSP_ERROR signaled by FSP:\n"));
    DEBUG ((DEBUG_ERROR, "  CallerId  = %g\n", (EFI_GUID *) CallerId));
    DEBUG ((DEBUG_ERROR, "  ErrorType = %g\n", (EFI_GUID *) &FspErrorInfo->ErrorType));
    DEBUG ((DEBUG_ERROR, "  Status    = %r\n", FspErrorInfo->Status));
    DEBUG ((DEBUG_ERROR, "\n"));
    return EFI_SUCCESS;
  } else {
    return RETURN_ABORTED;
  }
}
