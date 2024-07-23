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
#ifndef _PEI_DXE_FSP_ERROR_INFO_LIB_H_
#define _PEI_DXE_FSP_ERROR_INFO_LIB_H_

/**
  Function attempts to convert FSP error status code from EFI_STATUS to UINT32

  @param[in] Status             - FSP error status code in EFI_STATUS.

  @retval Status32              - FSP error status code in UINT32.
**/
UINT32
EFIAPI
EfiStatusToUint32 (
  EFI_STATUS Status
  );

/**
  Function attempts to convert FSP error status code from UINT32 to EFI_STATUS

  @param[in] Status32           - FSP error status code in UINT32.

  @retval Status                - FSP error status code in EFI_STATUS.
**/
EFI_STATUS
EFIAPI
Uint32ToEfiStatus (
  UINT32 Status32
  );

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
  IN EFI_GUID   CallerId,
  IN EFI_GUID   ErrorType,
  IN UINT32     Status
  );

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
  );

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
  );

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
  );
#endif //_PEI_DXE_FSP_ERROR_INFO_LIB_H_
