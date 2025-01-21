/** @file
  PEI Module to validate FSPM

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
#ifndef _FSPMV_INIT_H_
#define _FSPMV_INIT_H_

#include <Uefi.h>
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Ppi/ReportStatusCodeHandler.h>
#include <Library/FspVInfoLib.h>
#include <Library/FspVTestCaseLib.h>
#include <IntelRcStatusCode.h>
#include <Library/FspCommonLib.h>
#include <FspmUpd.h>
#include <Ppi/SiPolicy.h>
#include <FspVConfig.h>
#include <Library/ConfigBlockLib.h>

  /**
  FSP-N test case callback based on return codes
  @param[in]  PeiServices         An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param[in]  CodeType            Indicates the type of status code being reported.
  @param[in]  Value               Describes the current status of a hardware or software entity.
                                  This included information about the class and subclass that is used to
                                  classify the entity as well as an operation.
  @param[in]  Instance            The enumeration of a hardware or software entity within
                                  the system. Valid instance numbers start with 1.
  @param[in]  CallerId            This optional parameter may be used to identify the caller.
                                  This parameter allows the status code driver to apply different rules to
                                  different callers.
  @param[in]  Data                This optional parameter may be used to pass additional data.
  @retval EFI_SUCCESS             Status code is what we expected.
  @retval EFI_UNSUPPORTED         Status code not supported.
**/
EFI_STATUS
EFIAPI
FspMVTestCasesLoadCallback (
  IN CONST  EFI_PEI_SERVICES       **PeiServices,
  IN        EFI_STATUS_CODE_TYPE   CodeType,
  IN        EFI_STATUS_CODE_VALUE  Value,
  IN        UINT32                 Instance,
  IN CONST  EFI_GUID               *CallerId,
  IN CONST  EFI_STATUS_CODE_DATA   *Data
  );

#endif // _FSPMV_INIT_H
